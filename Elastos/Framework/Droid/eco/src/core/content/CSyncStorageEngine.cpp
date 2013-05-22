
#include "accounts/CAccount.h"
#include "content/CContentResolverHelper.h"
#include "content/CIntent.h"
#include "content/CSyncStorageEngine.h"
#include "content/CSyncStorageEngineAuthorityInfo.h"
#include "content/CSyncStorageEngineDayStats.h"
#include "content/CSyncStorageEnginePendingOperation.h"
#include "content/CSyncStorageEngineSyncHistoryItem.h"
#include "os/CBundle.h"
#include "utils/CParcelableObjectContainer.h"
#include <Logger.h>
#include <elastos/Math.h>

#define FAIL_WithGoto(ec) do { \
                                if (FAILED(ec)) {   \
                                    goto EXIT;      \
                                }                   \
                            } while (0)

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

const CString CSyncStorageEngine::TAG = "SyncManager";
const Boolean CSyncStorageEngine::DEBUG_FILE = FALSE;
const Int64 CSyncStorageEngine::DEFAULT_POLL_FREQUENCY_SECONDS = 60 * 60 * 24; // One day
const Int64 CSyncStorageEngine::MILLIS_IN_4WEEKS = 1000 * 60 * 60 * 24 * 7 * 4;
const Int32 CSyncStorageEngine::MSG_WRITE_STATUS = 1;
const Int64 CSyncStorageEngine::WRITE_STATUS_DELAY = 1000 * 60 * 10; // 10 minutes
const Int32 CSyncStorageEngine::MSG_WRITE_STATISTICS = 2;
const Int64 CSyncStorageEngine::WRITE_STATISTICS_DELAY = 1000 * 60 * 30; // 1/2 hour
const Boolean CSyncStorageEngine::SYNC_ENABLED_DEFAULT = FALSE;
const Int32 CSyncStorageEngine::ACCOUNTS_VERSION = 2;
const Int32 CSyncStorageEngine::PENDING_FINISH_TO_WRITE = 4;
HashMap<String, String>* CSyncStorageEngine::sAuthorityRenames = new HashMap<String, String>();
const ECode CSyncStorageEngine::staticECode = CSyncStorageEngine::DoStaticInitialization();
volatile ISyncStorageEngine* CSyncStorageEngine::sSyncStorageEngine = NULL;

CSyncStorageEngine::AccountInfo::AccountInfo(
    /* [in] */ IAccount* account)
    : mAuthorities(NULL)
{
    mAccount = account;
}

CSyncStorageEngine::AccountInfo::~AccountInfo()
{
    if (NULL != mAuthorities) {
        mAuthorities->Clear();
        delete mAuthorities;
    }
}

PInterface CSyncStorageEngine::AccountInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 CSyncStorageEngine::AccountInfo::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CSyncStorageEngine::AccountInfo::Release()
{
    return ElRefBase::Release();
}

ECode CSyncStorageEngine::AccountInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)this) {
        *pIID = EIID_IInterface;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

CSyncStorageEngine::CSyncStorageEngine()
    : mNextAuthorityId(0)
    , mNumPendingFinished(0)
    , mNextHistoryId(0)
    , mMasterSyncAutomatically(TRUE)
    , mSyncStatus(NULL)
{
    mAuthorities = new SparseArray();
    mAccounts = new HashMap<AutoPtr<IAccount>, AutoPtr<CSyncStorageEngine::AccountInfo> >();
    mPendingOperations = new  List<AutoPtr<ISyncStorageEnginePendingOperation> >();
    mDayStats = ArrayOf<AutoPtr<ISyncStorageEngineDayStats> >::Alloc(7*4);
    mSyncHistory = new List<AutoPtr<ISyncStorageEngineSyncHistoryItem> >();
}

CSyncStorageEngine::~CSyncStorageEngine()
{
    ArrayOf<AutoPtr<ISyncStorageEngineDayStats> >::Free(mDayStats);

    if (NULL != mAuthorities) {
        delete mAuthorities;
    }
    if (NULL != mAccounts) {
        mAccounts->Clear();
        delete mAccounts;
    }
    if (NULL != mPendingOperations) {
        mPendingOperations->Clear();
        delete mPendingOperations;
    }
    if (NULL != mSyncHistory) {
        mSyncHistory->Clear();
        delete mSyncHistory;
    }
}

ECode CSyncStorageEngine::HandleMessage(
    /* [in] */ IMessage* msg)
{
    //TODO
    //if (msg.what == MSG_WRITE_STATUS) {
    //    synchronized (mAuthorities) {
    //        writeStatusLocked();
    //    }
    //} else if (msg.what == MSG_WRITE_STATISTICS) {
    //    synchronized (mAuthorities) {
    //        writeStatisticsLocked();
    //    }
    //}
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::AddStatusChangeListener(
    /* [in] */ Int32 mask,
    /* [in] */ ISyncStatusObserver* syncStatusObserver)
{
    Mutex::Autolock lock(mAuthoritiesLock);
    AutoPtr<IInteger32> maskObj;
    FAIL_RETURN(CInteger32::New(mask, (IInteger32**) &maskObj));
    Boolean result;
    FAIL_RETURN(mChangeListeners->RegisterEx(syncStatusObserver, maskObj, &result));
    return NOERROR;
}

ECode CSyncStorageEngine::RemoveStatusChangeListener(
    /* [in] */ ISyncStatusObserver* syncStatusObserver)
{
    Mutex::Autolock lock(mAuthoritiesLock);
    Boolean result;
    FAIL_RETURN(mChangeListeners->Unregister(syncStatusObserver, &result));
    return NOERROR;
}

ECode CSyncStorageEngine::GetSyncAutomatically(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Mutex::Autolock lock(mAuthoritiesLock);

    if (NULL != account) {
        AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
        FAIL_RETURN(GetAuthorityLocked(account, providerName, String("getSyncAutomatically"), (ISyncStorageEngineAuthorityInfo**)&authority));
        if (NULL != authority) {
            Boolean isEnable;
            authority->GetEnabled(&isEnable);
            if (isEnable) {
                *result = TRUE;
                return NOERROR;
            }
        }
        *result = FALSE;
        return NOERROR;
    }

    Int32 i = mAuthorities->Size();
    AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
    String strAuthority;

    while (i > 0) {
        i--;
        authorityInfo = (ISyncStorageEngineAuthorityInfo*) mAuthorities->ValueAt(i);
        authorityInfo->GetAuthority(&strAuthority);
   
        if (!strAuthority.Compare(providerName)) {
            Boolean isEnable;
            FAIL_RETURN(authorityInfo->GetEnabled(&isEnable));
            if (isEnable) {
                *result = TRUE;
                return NOERROR;
            }
        }

    }

    *result = FALSE;
    return NOERROR;
}

ECode CSyncStorageEngine::SetSyncAutomatically(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [in] */ Boolean sync)
{
    String str("setSyncAutomatically: ");
    str.Append(", provider ");
    str.Append(providerName);
    str.Append(" -> ");
    str.Append(sync ? "true" : "false");
    Logger::D(TAG, str);
    Mutex::Autolock lock(mAuthoritiesLock);
    AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
    FAIL_RETURN(GetOrCreateAuthorityLocked(account, providerName, -1, FALSE, (ISyncStorageEngineAuthorityInfo**)&authorityInfo));
    Boolean isEnable;
    FAIL_RETURN(authorityInfo->GetEnabled(&isEnable));

    if (isEnable == sync) {
        String info("setSyncAutomatically: already set to ");
        info.Append(sync ? "true" : "false");
        info.Append(", doing nothing");
        Logger::D(TAG, info);
        return NOERROR;
    }

    FAIL_RETURN(authorityInfo->SetEnabled(sync));
    FAIL_RETURN(WriteAccountInfoLocked());

    if (sync) {
        AutoPtr<IContentResolverHelper> contentResolverHelper;
        FAIL_RETURN(CContentResolverHelper::AcquireSingleton((IContentResolverHelper**)&contentResolverHelper));
        AutoPtr<IBundle> bundle;
        FAIL_RETURN(CBundle::New((IBundle**)&bundle));
        FAIL_RETURN(contentResolverHelper->RequestSync(account, providerName, bundle));
    }

    FAIL_RETURN(ReportChange(ContentResolver_SYNC_OBSERVER_TYPE_SETTINGS));
    return NOERROR;
}

ECode CSyncStorageEngine::GetIsSyncable(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    Mutex::Autolock lock(mAuthoritiesLock);

    if (NULL != account) {
        AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
        FAIL_RETURN(GetAuthorityLocked(account, providerName, String("getIsSyncable"), (ISyncStorageEngineAuthorityInfo**)&authority));
        if (NULL == authority) {
            *result = -1;
            return NOERROR;
        }
        Int32 syncable;
        FAIL_RETURN(authority->GetSyncable(&syncable));
        *result = syncable;
        return NOERROR;
    }

    Int32 i = mAuthorities->Size();
    AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
    String strAuthority;

    while (i > 0) {
        i--;
        authorityInfo = (ISyncStorageEngineAuthorityInfo*) mAuthorities->ValueAt(i);
        authorityInfo->GetAuthority(&strAuthority);

        if (!strAuthority.Compare(providerName)) {
            Int32 syncable;
            FAIL_RETURN(authorityInfo->GetSyncable(&syncable));
            *result = syncable;
            return NOERROR;
        }
    }

    *result = -1;
    return NOERROR;
}

ECode CSyncStorageEngine::SetIsSyncable(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [in] */ Int32 syncable)
{
    if (syncable > 1) {
        syncable = 1;
    } else if (syncable < -1) {
        syncable = -1;
    }

    String str("setIsSyncable: ");
    if (NULL != account) {
        //TODO: account->ToString();
    } else {
        str.Append(" null ");
    }
    str.Append(", provider ");
    str.Append(providerName);
    str.Append(" -> ");
    String strSyncable = String::FromInt32(syncable);
    str.Append(strSyncable);
    Logger::D(TAG, str);
    Mutex::Autolock lock(mAuthoritiesLock);
    AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
    FAIL_RETURN(GetOrCreateAuthorityLocked(account, providerName, -1, FALSE, (ISyncStorageEngineAuthorityInfo**)&authorityInfo));
    Int32 newSyncable;
    FAIL_RETURN(authorityInfo->GetSyncable(&newSyncable));

    if (newSyncable == syncable) {
        String info("setIsSyncable: already set to ");
        String strSyncable2 = String::FromInt32(syncable);
        info.Append(strSyncable2);
        info.Append(", doing nothing");
        Logger::D(TAG, info);
        return NOERROR;
    }

    FAIL_RETURN(authorityInfo->SetSyncable(syncable));
    FAIL_RETURN(WriteAccountInfoLocked());

    if (syncable > 0) {
        AutoPtr<IContentResolverHelper> contentResolverHelper;
        FAIL_RETURN(CContentResolverHelper::AcquireSingleton((IContentResolverHelper**)&contentResolverHelper));
        AutoPtr<IBundle> bundle;
        FAIL_RETURN(CBundle::New((IBundle**)&bundle));
        FAIL_RETURN(contentResolverHelper->RequestSync(account, providerName, bundle));
    }

    FAIL_RETURN(ReportChange(ContentResolver_SYNC_OBSERVER_TYPE_SETTINGS));
    return NOERROR;
}

ECode CSyncStorageEngine::SetBackoff(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [in] */ Int64 nextSyncTime,
    /* [in] */ Int64 nextDelay)
{
    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
        String str("setBackoff: ");
        if (NULL != account) {
            //TODO: account->ToString();
        } else {
            str.Append(" null ");
        }
        str.Append(", provider ");
        str.Append(providerName);
        str.Append(" -> nextSyncTime ");
        String strNextSyncTime = String::FromInt64(nextSyncTime);
        str.Append(strNextSyncTime);
        str.Append(", nextDelay ");
        String strNextDelay = String::FromInt64(nextDelay);
        str.Append(strNextDelay);
        Logger::V(TAG, str);
    }

    Boolean changed = FALSE;
    Mutex::Autolock lock(mAuthoritiesLock);

    if (NULL == account || providerName.IsNull()) {
        HashMap<AutoPtr<IAccount>, AutoPtr<CSyncStorageEngine::AccountInfo> >::Iterator it = mAccounts->Begin();
        AutoPtr<CSyncStorageEngine::AccountInfo> accountInfo;
        AutoPtr<IAccount> tmpAccount;

        for (; it != mAccounts->End(); ++it) {
            accountInfo = it->mSecond;
            tmpAccount = accountInfo->mAccount;
            if (NULL != account && !_CObject_Compare(account, tmpAccount.Get())) continue;
            HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> >::Iterator iter = accountInfo->mAuthorities->Begin();
            AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
            for (; iter != accountInfo->mAuthorities->End(); ++iter) {
                authorityInfo = iter->mSecond;
                String authority;
                FAIL_RETURN(authorityInfo->GetAuthority(&authority));
                if (!providerName.IsNull() && providerName.Compare(authority)) continue;
                Int64 backoffTime;
                Int64 backoffDelay;
                FAIL_RETURN(authorityInfo->GetBackoffTime(&backoffTime));
                FAIL_RETURN(authorityInfo->GetBackoffDelay(&backoffDelay));
                if (backoffTime != nextSyncTime || backoffDelay != nextDelay) {
                    FAIL_RETURN(authorityInfo->SetBackoffTime(nextSyncTime));
                    FAIL_RETURN(authorityInfo->SetBackoffDelay(nextDelay));
                    changed = TRUE;
                }
            }
        }

    } else {
        AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
        FAIL_RETURN(GetOrCreateAuthorityLocked(account, providerName, -1, TRUE, (ISyncStorageEngineAuthorityInfo**)&authorityInfo));
        Int64 backoffTime;
        Int64 backoffDelay;
        FAIL_RETURN(authorityInfo->GetBackoffTime(&backoffTime));
        FAIL_RETURN(authorityInfo->GetBackoffDelay(&backoffDelay));

        if (backoffTime == nextSyncTime && backoffDelay == nextDelay) {
            return NOERROR;
        }

        FAIL_RETURN(authorityInfo->SetBackoffTime(nextSyncTime));
        FAIL_RETURN(authorityInfo->SetBackoffDelay(nextDelay));
        changed = TRUE;
    }

    if (changed) {
        FAIL_RETURN(ReportChange(ContentResolver_SYNC_OBSERVER_TYPE_SETTINGS));
    }

    return NOERROR;
}

ECode CSyncStorageEngine::ClearAllBackoffs()
{
    Boolean changed = FALSE;
    Mutex::Autolock lock(mAuthoritiesLock);
    HashMap<AutoPtr<IAccount>, AutoPtr<CSyncStorageEngine::AccountInfo> >::Iterator it = mAccounts->Begin();
    AutoPtr<CSyncStorageEngine::AccountInfo> accountInfo;

    for (; it != mAccounts->End(); ++it) {
        accountInfo = it->mSecond;
        HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> >::Iterator iter = accountInfo->mAuthorities->Begin();
        AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;

        for (; iter != accountInfo->mAuthorities->End(); ++iter) {
            authorityInfo = iter->mSecond;
            Int64 backoffTime;
            Int64 backoffDelay;
            FAIL_RETURN(authorityInfo->GetBackoffTime(&backoffTime));
            FAIL_RETURN(authorityInfo->GetBackoffDelay(&backoffDelay));

            if(SyncStorageEngine_NOT_IN_BACKOFF_MODE != backoffTime || SyncStorageEngine_NOT_IN_BACKOFF_MODE != backoffDelay) {

                if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
                    String str("clearAllBackoffs:");
                    str.Append(" authority:");
                    String authority;
                    FAIL_RETURN(authorityInfo->GetAuthority(&authority));
                    str.Append(authority);
                    str.Append(" account:");
                    AutoPtr<IAccount> account;
                    FAIL_RETURN(authorityInfo->GetAccount((IAccount**)&account));
                    String accountName;
                    FAIL_RETURN(account->GetName(&accountName));
                    str.Append(accountName);
                    str.Append(" backoffTime was: ");
                    String strBackoffTime = String::FromInt64(backoffTime);
                    str.Append(strBackoffTime);
                    String strBackoffDelay = String::FromInt64(backoffDelay);
                    str.Append(strBackoffDelay);
                    Logger::V(TAG, str);
                }

                FAIL_RETURN(authorityInfo->SetBackoffTime(SyncStorageEngine_NOT_IN_BACKOFF_MODE));
                FAIL_RETURN(authorityInfo->SetBackoffDelay(SyncStorageEngine_NOT_IN_BACKOFF_MODE));
                changed = TRUE;
            }

        }

    }

    if (changed) {
        FAIL_RETURN(ReportChange(ContentResolver_SYNC_OBSERVER_TYPE_SETTINGS));
    }

    return NOERROR;
}

ECode CSyncStorageEngine::SetDelayUntilTime(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [in] */ Int64 delayUntil)
{
    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
        String str("setDelayUntil: ");
        if (NULL != account) {
            //TODO: account->ToString();
        } else {
            str.Append(" null ");
        }
        str.Append(", provider ");
        str.Append(providerName);
        str.Append(" -> delayUntil ");
        String strDelayUntil = String::FromInt64(delayUntil);
        str.Append(strDelayUntil);
        Logger::V(TAG, str);
    }

    Mutex::Autolock lock(mAuthoritiesLock);
    AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
    FAIL_RETURN(GetOrCreateAuthorityLocked(account, providerName, -1, TRUE, (ISyncStorageEngineAuthorityInfo**)&authorityInfo));
    Int64 newDelayUntil;
    FAIL_RETURN(authorityInfo->GetDelayUntil(&newDelayUntil));

    if (newDelayUntil == delayUntil) {
        return NOERROR;
    }

    FAIL_RETURN(authorityInfo->SetDelayUntil(delayUntil));
    FAIL_RETURN(ReportChange(ContentResolver_SYNC_OBSERVER_TYPE_SETTINGS));
    return NOERROR;
}

ECode CSyncStorageEngine::GetDelayUntilTime(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [out] */ Int64* delayUntilTime)
{
    VALIDATE_NOT_NULL(delayUntilTime);
    Mutex::Autolock lock(mAuthoritiesLock);
    AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
    FAIL_RETURN(GetAuthorityLocked(account, providerName, String("getDelayUntil"), (ISyncStorageEngineAuthorityInfo**)&authority));

    if (NULL == authority) {
        *delayUntilTime = 0;
        return NOERROR;
    }

    Int64 delayUntil;
    FAIL_RETURN(authority->GetDelayUntil(&delayUntil));
    *delayUntilTime = delayUntil;
    return NOERROR;
}

ECode CSyncStorageEngine::AddPeriodicSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 pollFrequency)
{
    return UpdateOrRemovePeriodicSync(account, providerName, extras, pollFrequency, TRUE /* add */);
}

ECode CSyncStorageEngine::RemovePeriodicSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [in] */ IBundle* extras)
{
    return UpdateOrRemovePeriodicSync(account, providerName, extras, 0 /* period, ignored */, FALSE /* remove */);
}

ECode CSyncStorageEngine::GetPeriodicSyncs(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [out] */ IObjectContainer** periodicSyncList)
{
    VALIDATE_NOT_NULL(periodicSyncList);
    Mutex::Autolock lock(mAuthoritiesLock);
    FAIL_RETURN(CParcelableObjectContainer::New(periodicSyncList));
    AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
    FAIL_RETURN(GetAuthorityLocked(account, providerName, String("getPeriodicSyncs"), (ISyncStorageEngineAuthorityInfo**)&authority));

    if (NULL == authority) {
        CSyncStorageEngineAuthorityInfo* authorityInfo = (CSyncStorageEngineAuthorityInfo*) authority.Get();
        List<Pair<AutoPtr<IBundle>, Int64> >* periodicSyncs = authorityInfo->GetPeriodicSyncs();
        List<Pair<AutoPtr<IBundle>, Int64> >::Iterator iter = periodicSyncs->Begin();
        Pair<AutoPtr<IBundle>, Int64> item;
        for (; iter != periodicSyncs->End(); ++iter) {
            item = *iter;
            //TODO: syncs.add(new PeriodicSync(account, providerName, item.first, item.second));
            //periodicSyncList->Add();
        }
    }

    return NOERROR;
}

ECode CSyncStorageEngine::SetMasterSyncAutomatically(
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(mAuthoritiesLock);

    if (mMasterSyncAutomatically == flag) {
        return NOERROR;
    }

    mMasterSyncAutomatically = flag;
    FAIL_RETURN(WriteAccountInfoLocked());

    if (flag) {
        AutoPtr<IContentResolverHelper> contentResolverHelper;
        FAIL_RETURN(CContentResolverHelper::AcquireSingleton((IContentResolverHelper**)&contentResolverHelper));
        AutoPtr<IBundle> bundle;
        FAIL_RETURN(CBundle::New((IBundle**)&bundle));
        AutoPtr<IAccount> account;
        String authority;
        FAIL_RETURN(contentResolverHelper->RequestSync(account, authority, bundle));
    }

    FAIL_RETURN(ReportChange(ContentResolver_SYNC_OBSERVER_TYPE_SETTINGS));
    AutoPtr<IIntent> intent;
    String action("com.android.sync.SYNC_CONN_STATUS_CHANGED");
    FAIL_RETURN(CIntent::New(action, (IIntent**)&intent));
    FAIL_RETURN(mContext->SendBroadcast(intent));
    return NOERROR;
}

ECode CSyncStorageEngine::GetMasterSyncAutomatically(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Mutex::Autolock lock(mAuthoritiesLock);
    *result = mMasterSyncAutomatically;
    return NOERROR;
}

ECode CSyncStorageEngine::GetOrCreateAuthority(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo)
{
    VALIDATE_NOT_NULL(authorityInfo);
    Mutex::Autolock lock(mAuthoritiesLock);
    FAIL_RETURN(GetOrCreateAuthorityLocked(account, authority, -1/* assign a new identifier if creating a new authority */,
         TRUE /* write to storage if this results in a change */, authorityInfo));
    return NOERROR;
}

ECode CSyncStorageEngine::RemoveAuthority(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority)
{
    Mutex::Autolock lock(mAuthoritiesLock);
    FAIL_RETURN(RemoveAuthorityLocked(account, authority, TRUE /* doWrite */));
    return NOERROR;
}

ECode CSyncStorageEngine::GetAuthority(
    /* [in] */ Int32 authorityId,
    /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo)
{
    VALIDATE_NOT_NULL(authorityInfo);
    Mutex::Autolock lock(mAuthoritiesLock);
    *authorityInfo = (ISyncStorageEngineAuthorityInfo*) mAuthorities->Get(authorityId);
    return NOERROR;
}

ECode CSyncStorageEngine::IsSyncActive(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ Boolean* isActive)
{
    VALIDATE_NOT_NULL(isActive);
    Mutex::Autolock lock(mAuthoritiesLock);
    Int32 i = mPendingOperations->GetSize();

    while (i > 0) {
        i--;
        // TODO(fredq): this probably shouldn't be considering
        // pending operations.
        List<AutoPtr<ISyncStorageEnginePendingOperation> >::Iterator iter = mPendingOperations->Begin();
        AutoPtr<ISyncStorageEnginePendingOperation> operation;
        Int32 count = 0;
        while (count != i) {
            count++;
            iter++;
        }
        operation = *iter;
        AutoPtr<IAccount> newAccount;
        String newAuthority;
        FAIL_RETURN(operation->GetAccount((IAccount**)&newAccount));
        FAIL_RETURN(operation->GetAuthority(&newAuthority));

        if (_CObject_Compare(newAccount, account) && !newAuthority.Compare(authority)) {
            *isActive = TRUE;
            return NOERROR;
        }

    }

    if (NULL != mCurrentSync) {
        Int32 authorityId;
        AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
        //TODO: mCurrentSync->XXX(&authorityId);
        FAIL_RETURN(GetAuthority(authorityId, (ISyncStorageEngineAuthorityInfo**)&authorityInfo));

        if (NULL != authorityInfo) {
            AutoPtr<IAccount> newAccount;
            String newAuthority;
            FAIL_RETURN(authorityInfo->GetAccount((IAccount**)&newAccount));
            FAIL_RETURN(authorityInfo->GetAuthority(&newAuthority));
            if (_CObject_Compare(newAccount, account) && !newAuthority.Compare(authority)) {
                *isActive = TRUE;
                return NOERROR;
            }
        }

    }

    *isActive = FALSE;
    return NOERROR;
}

ECode CSyncStorageEngine::InsertIntoPending(
    /* [in] */ ISyncStorageEnginePendingOperation* op,
    /* [out] */ ISyncStorageEnginePendingOperation** pendingOperation)
{
    VALIDATE_NOT_NULL(pendingOperation);
    Mutex::Autolock lock(mAuthoritiesLock);
    AutoPtr<IAccount> account;
    String authority;
    FAIL_RETURN(op->GetAccount((IAccount**)&account));
    FAIL_RETURN(op->GetAuthority(&authority));

    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
        Int32 syncSource;
        AutoPtr<IBundle> extras;
        FAIL_RETURN(op->GetSyncSource(&syncSource));
        FAIL_RETURN(op->GetExtras((IBundle**)&extras));
        String strSyncSource = String::FromInt32(syncSource);
        String strExtras;
        //TODO: extras->ToString(&strExtras);
        String str("insertIntoPending: account=");
        if (NULL != account) {
            //TODO: account->ToString();
        } else {
            str.Append(" null ");
        }
        str.Append(" auth=");
        str.Append(authority);
        str.Append(" src=");
        str.Append(strSyncSource);
        str.Append(" extras=");
        str.Append(strExtras);
        Logger::V(TAG, str);
    }

    AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
    FAIL_RETURN(GetOrCreateAuthorityLocked(account, authority, -1 /* desired identifier */,
        TRUE /* write accounts to storage */, (ISyncStorageEngineAuthorityInfo**)&authorityInfo));

    if (NULL == authorityInfo) {
        *pendingOperation = NULL;
        return NOERROR;
    }

    FAIL_RETURN(CSyncStorageEnginePendingOperation::New(op, pendingOperation));
    Int32 ident;
    FAIL_RETURN(authorityInfo->GetIdent(&ident));
    FAIL_RETURN((*pendingOperation)->SetAuthorityId(ident));
    mPendingOperations->PushBack(*pendingOperation);
    FAIL_RETURN(AppendPendingOperationLocked(*pendingOperation));
    AutoPtr<ISyncStatusInfo> statusInfo;
    FAIL_RETURN(GetOrCreateSyncStatusLocked(ident, (ISyncStatusInfo**)&statusInfo));
    //TODO: status.pending = true;
    FAIL_RETURN(ReportChange(ContentResolver_SYNC_OBSERVER_TYPE_PENDING));
    return NOERROR;
}

ECode CSyncStorageEngine::DeleteFromPending(
    /* [in] */ ISyncStorageEnginePendingOperation* op,
    /* [out] */ Boolean* result)
{
    Boolean res = FALSE;
    Mutex::Autolock lock(mAuthoritiesLock);
    AutoPtr<IAccount> account;
    String authority;
    FAIL_RETURN(op->GetAccount((IAccount**)&account));
    FAIL_RETURN(op->GetAuthority(&authority));

    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
        Int32 syncSource;
        AutoPtr<IBundle> extras;
        FAIL_RETURN(op->GetSyncSource(&syncSource));
        FAIL_RETURN(op->GetExtras((IBundle**)&extras));
        String strSyncSource = String::FromInt32(syncSource);
        String strExtras;
        //TODO: extras->ToString(&strExtras);
        String str("deleteFromPending: account=");
        if (NULL != account) {
            //TODO: account->ToString();
        } else {
            str.Append(" null ");
        }
        str.Append(" auth=");
        str.Append(authority);
        str.Append(" src=");
        str.Append(strSyncSource);
        str.Append(" extras=");
        str.Append(strExtras);
        Logger::V(TAG, str);
    }

    Boolean isRemove = FALSE;
    List<AutoPtr<ISyncStorageEnginePendingOperation> >::Iterator first = mPendingOperations->Begin();
    List<AutoPtr<ISyncStorageEnginePendingOperation> >::Iterator last = mPendingOperations->End();
    while (first != last) {
        List<AutoPtr<ISyncStorageEnginePendingOperation> >::Iterator next = first;
        ++next;
        if (_CObject_Compare(*first, op)){
             mPendingOperations->Erase(first);
             isRemove = TRUE;
        } 
        first = next;
    }

    if (isRemove) {
        Int32 N = mPendingOperations->GetSize();

        if (0 == N || mNumPendingFinished >= CSyncStorageEngine::PENDING_FINISH_TO_WRITE) {
            FAIL_RETURN(WritePendingOperationsLocked());
            mNumPendingFinished = 0;
        } else {
            mNumPendingFinished++;
        }

        AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
        FAIL_RETURN(GetAuthorityLocked(account, authority, String("deleteFromPending"), (ISyncStorageEngineAuthorityInfo**)&authorityInfo));

        if (NULL != authorityInfo) {
            if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
                String str("removing - ");
                //TODO: authorityInfo->ToString();
                Logger::V(TAG, str);
            }
            Boolean morePending = FALSE;

            for (Int32 i=0; i<N; i++) {
                List<AutoPtr<ISyncStorageEnginePendingOperation> >::Iterator iter = mPendingOperations->Begin();
                AutoPtr<ISyncStorageEnginePendingOperation> cur;
                Int32 count = 0;
                while (count != i) {
                    count++;
                    iter++;
                }
                cur = *iter;
                AutoPtr<IAccount> newAccount;
                String newAuthority;
                FAIL_RETURN(cur->GetAccount((IAccount**)&newAccount));
                FAIL_RETURN(cur->GetAuthority(&newAuthority));

                if (_CObject_Compare(newAccount, account) && !newAuthority.Compare(authority)){
                    morePending = TRUE;
                    break;
                }

            }

            if (!morePending) {
                if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
                    String str("no more pending!");
                    Logger::V(TAG, str);
                }

                Int32 ident;
                AutoPtr<ISyncStatusInfo> statusInfo;
                FAIL_RETURN(authorityInfo->GetIdent(&ident));
                FAIL_RETURN(GetOrCreateSyncStatusLocked(ident, (ISyncStatusInfo**)&statusInfo));
                //TODO: status.pending = false;
            }

        }

        res = TRUE;
    }
    
    FAIL_RETURN(ReportChange(ContentResolver_SYNC_OBSERVER_TYPE_PENDING));
    *result = res;
    return NOERROR;
}

ECode CSyncStorageEngine::ClearPending(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 num;
    Mutex::Autolock lock(mAuthoritiesLock);

    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
        String str("clearPending");
        Logger::V(TAG, str);
    }

    num = mPendingOperations->GetSize();
    mPendingOperations->Clear();
    const Int32 N = mSyncStatus->Size();

    for (Int32 i = 0; i < N; i++) {
        AutoPtr<ISyncStatusInfo> statusInfo = (ISyncStatusInfo*) mSyncStatus->ValueAt(i);
        //TODO: statusInfo.pending = false;
    }

    FAIL_RETURN(WritePendingOperationsLocked());
    FAIL_RETURN(ReportChange(ContentResolver_SYNC_OBSERVER_TYPE_PENDING));
    *result = num;
    return NOERROR;
}

ECode CSyncStorageEngine::GetPendingOperations(
    /* [out] */ IObjectContainer** pendingOperationList)
{
    VALIDATE_NOT_NULL(pendingOperationList);
    Mutex::Autolock lock(mAuthoritiesLock);
    FAIL_RETURN(CParcelableObjectContainer::New(pendingOperationList));
    List<AutoPtr<ISyncStorageEnginePendingOperation> >::Iterator iter = mPendingOperations->Begin();
    AutoPtr<ISyncStorageEnginePendingOperation> operation;

    for (; iter != mPendingOperations->End(); ++iter) {
        operation = *iter;
        (*pendingOperationList)->Add(operation);
    }

    return NOERROR;
}

ECode CSyncStorageEngine::GetPendingOperationCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    Mutex::Autolock lock(mAuthoritiesLock);
    *count = mPendingOperations->GetSize();
    return NOERROR;
}

ECode CSyncStorageEngine::DoDatabaseCleanup(
    /* [in] */ ArrayOf<IAccount *>* accounts)
{
    VALIDATE_NOT_NULL(accounts);
    Mutex::Autolock lock(mAuthoritiesLock);

    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
        String str("Updating for new accounts...");
        Logger::W(TAG, str);
    }

    SparseArray* removing = new SparseArray();
    Int32 len = accounts->GetLength();
    HashMap<AutoPtr<IAccount>, AutoPtr<CSyncStorageEngine::AccountInfo> >::Iterator it = mAccounts->Begin();
    AutoPtr<CSyncStorageEngine::AccountInfo> accountInfo;
    AutoPtr<IAccount> account;
    for (; it != mAccounts->End(); ++it) {
        accountInfo = it->mSecond;
        account = accountInfo->mAccount;
        Boolean isContain = FALSE;

        for (Int32 i = 0; i != len; i++) {
            if (_CObject_Compare((*accounts)[i], account)) {
                isContain = TRUE;
                break;
            }
        }

        if (!isContain) {
            if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
                String str("Account removed: ");
                //TODO: account->ToString();
                Logger::W(TAG, str);
            }
            HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> >::Iterator iter = accountInfo->mAuthorities->Begin();
            AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
            Int32 ident;
            for (; iter != accountInfo->mAuthorities->End(); ++iter) {
                authorityInfo = iter->mSecond;
                FAIL_RETURN(authorityInfo->GetIdent(&ident));
                removing->Put(ident, authorityInfo);
            }

            mAccounts->Erase(it);
        }

    }

    // Clean out all data structures.
    Int32 i = removing->Size();
    if (i > 0) {
        while (i > 0) {
            i--;
            Int32 ident = removing->KeyAt(i);
            mAuthorities->Remove(ident);
            Int32 j = mSyncStatus->Size();
            while (j > 0) {
                j--;
                if (mSyncStatus->KeyAt(j) == ident) {
                    mSyncStatus->Remove(mSyncStatus->KeyAt(j));
                }
            }
            j = mSyncHistory->GetSize();
            while (j > 0) {
                j--;
                List<AutoPtr<ISyncStorageEngineSyncHistoryItem> >::Iterator iter = mSyncHistory->Begin();
                AutoPtr<ISyncStorageEngineSyncHistoryItem> historyItem;
                Int32 count = 0;
                while (count != j) {
                    count++;
                    iter++;
                }
                historyItem = *iter;
                Int32 authorityId;
                FAIL_RETURN(historyItem->GetAuthorityId(&authorityId));
                if (authorityId == ident) {
                    mSyncHistory->Remove(j);
                }
            }
        }

        FAIL_RETURN(WriteAccountInfoLocked());
        FAIL_RETURN(WriteStatusLocked());
        FAIL_RETURN(WritePendingOperationsLocked());
        FAIL_RETURN(WriteStatisticsLocked());
    }
    
    return NOERROR;
}

ECode CSyncStorageEngine::ReportActiveChange()
{
    FAIL_RETURN(ReportChange(ContentResolver_SYNC_OBSERVER_TYPE_ACTIVE));
    return NOERROR;
}

ECode CSyncStorageEngine::InsertStartSyncEvent(
    /* [in] */ IAccount* accountName,
    /* [in] */ const String& authorityName,
    /* [in] */ Int64 now,
    /* [in] */ Int32 source,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    Int64 id;
    Mutex::Autolock lock(mAuthoritiesLock);

    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
        String str("insertStartSyncEvent: account=");
        if (NULL != accountName) {
            //TODO: accountName->ToString();
        } else {
            str.Append(" null ");
        }
        str.Append(" auth=");
        str.Append(authorityName);
        str.Append(" source=");
        String strSource = String::FromInt32(source);
        str.Append(strSource);
        Logger::V(TAG, str);
    }

    AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
    FAIL_RETURN(GetAuthorityLocked(accountName, authorityName, String("insertStartSyncEvent"), (ISyncStorageEngineAuthorityInfo**)&authority));

    if (NULL == authority) {
        *result = -1;
        return NOERROR;
    }

    AutoPtr<ISyncStorageEngineSyncHistoryItem> item;
    FAIL_RETURN(CSyncStorageEngineSyncHistoryItem::New((ISyncStorageEngineSyncHistoryItem**)&item));
    Int32 ident;
    FAIL_RETURN(authority->GetIdent(&ident));
    FAIL_RETURN(item->SetAuthorityId(ident));
    FAIL_RETURN(item->SetHistoryId(mNextHistoryId++));
    if (mNextHistoryId < 0) mNextHistoryId = 0;
    FAIL_RETURN(item->SetEventTime(now));
    FAIL_RETURN(item->SetSource(source));
    FAIL_RETURN(item->SetEvent(SyncStorageEngine_EVENT_START));
    mSyncHistory->PushFront(item);

    while (mSyncHistory->GetSize() > SyncStorageEngine_MAX_HISTORY) {
        mSyncHistory->Remove(mSyncHistory->GetSize() - 1);
    }

    Int32 historyId;
    FAIL_RETURN(item->GetHistoryId(&historyId));
    id = historyId;

    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
        String str("returning historyId ");
        String strId = String::FromInt64(id);
        str.Append(strId);
        Logger::V(TAG, str);
    }

    FAIL_RETURN(ReportChange(ContentResolver_SYNC_OBSERVER_TYPE_STATUS));
    *result = id;
    return NOERROR;
}

ECode CSyncStorageEngine::StopSyncEvent(
    /* [in] */ Int64 historyId,
    /* [in] */ Int64 elapsedTime,
    /* [in] */ const String& resultMessage,
    /* [in] */ Int64 downstreamActivity,
    /* [in] */ Int64 upstreamActivity)
{
    Mutex::Autolock lock(mAuthoritiesLock);

    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
        String str("stopSyncEvent: historyId=");
        String strHistoryId = String::FromInt64(historyId);
        str.Append(strHistoryId);
        Logger::V(TAG, str);
    }

    AutoPtr<ISyncStorageEngineSyncHistoryItem> item;
    Int32 i = mSyncHistory->GetSize();

    while (i > 0) {
        i--;
        List<AutoPtr<ISyncStorageEngineSyncHistoryItem> >::Iterator iter = mSyncHistory->Begin();
        Int32 count = 0;
        while (count != i) {
            count++;
            iter++;
        }
        item = *iter;
        Int32 tmpHistoryId;
        FAIL_RETURN(item->GetHistoryId(&tmpHistoryId));
        if (tmpHistoryId == historyId) {
            break;
        }
        item = NULL;
    }

    if (NULL == item) {
        String str("stopSyncEvent: no history for id ");
        String strHistoryId = String::FromInt64(historyId);
        str.Append(strHistoryId);
        Logger::W(TAG, str);
        return NOERROR;
    }

    FAIL_RETURN(item->SetElapsedTime(elapsedTime));
    FAIL_RETURN(item->SetEvent(SyncStorageEngine_EVENT_STOP));
    FAIL_RETURN(item->SetMesg(resultMessage));
    FAIL_RETURN(item->SetDownstreamActivity(downstreamActivity));
    FAIL_RETURN(item->SetUpstreamActivity(upstreamActivity));
    Int32 authorityId;
    FAIL_RETURN(item->GetAuthorityId(&authorityId));
    AutoPtr<ISyncStatusInfo> statusInfo;
    FAIL_RETURN(GetOrCreateSyncStatusLocked(authorityId, (ISyncStatusInfo**)&statusInfo));
    //TODO: status.numSyncs++;
    //TODO: status.totalElapsedTime += elapsedTime;
    Int32 source;
    FAIL_RETURN(item->GetSource(&source));

    switch (source) {
        case SyncStorageEngine_SOURCE_LOCAL:
            //TODO: status.numSourceLocal++;
            break;
        case SyncStorageEngine_SOURCE_POLL:
            //TODO: status.numSourcePoll++;
            break;
        case SyncStorageEngine_SOURCE_USER:
            //TODO: status.numSourceUser++;
            break;
        case SyncStorageEngine_SOURCE_SERVER:
            //TODO: status.numSourceServer++;
            break;
        case SyncStorageEngine_SOURCE_PERIODIC:
            //TODO: status.numSourcePeriodic++;
            break;
    }

    Boolean writeStatisticsNow = FALSE;
    Int32 day;
    Int32 tmpDay;
    FAIL_RETURN(GetCurrentDayLocked(&day));

    if (NULL == (*mDayStats)[0]) {
        FAIL_RETURN(CSyncStorageEngineDayStats::New(day, (ISyncStorageEngineDayStats**)&(*mDayStats)[0]));
    } else if (day != ((*mDayStats)[0]->GetDay(&tmpDay), tmpDay)) {
        mDayStats->Replace(1, mDayStats->GetPayload(), mDayStats->GetLength() - 1); //System.arraycopy(mDayStats, 0, mDayStats, 1, mDayStats.length-1);
        FAIL_RETURN(CSyncStorageEngineDayStats::New(day, (ISyncStorageEngineDayStats**)&(*mDayStats)[0]));
        writeStatisticsNow = TRUE;
    }

    const AutoPtr<ISyncStorageEngineDayStats> ds = (*mDayStats)[0];
    Int64 eventTime;
    FAIL_RETURN(item->GetEventTime(&eventTime));
    const Int64 lastSyncTime = (eventTime + elapsedTime);
    Boolean writeStatusNow = FALSE;

    if (String(SyncStorageEngine_MESG_SUCCESS).Compare(resultMessage)) {
        // - if successful, update the successful columns
        //TODO: if (status.lastSuccessTime == 0 || status.lastFailureTime != 0) {
        //TODO:     writeStatusNow = true;
        //TODO: }
        //TODO: status.lastSuccessTime = lastSyncTime;
        //TODO: status.lastSuccessSource = item.source;
        //TODO: status.lastFailureTime = 0;
        //TODO: status.lastFailureSource = -1;
        //TODO: status.lastFailureMesg = null;
        //TODO: status.initialFailureTime = 0;
        Int32 successCount;
        FAIL_RETURN(ds->GetSuccessCount(&successCount));
        successCount++;
        FAIL_RETURN(ds->SetSuccessCount(successCount));
        Int64 successTime;
        FAIL_RETURN(ds->GetSuccessTime(&successTime));
        successTime += elapsedTime;
        FAIL_RETURN(ds->SetSuccessTime(successTime));
    } else if (!String(SyncStorageEngine_MESG_CANCELED).Compare(resultMessage)) {
        //TODO: if (status.lastFailureTime == 0) {
        //TODO:     writeStatusNow = true;
        //TODO: }
        //TODO: status.lastFailureTime = lastSyncTime;
        //TODO: status.lastFailureSource = item.source;
        //TODO: status.lastFailureMesg = resultMessage;
        //TODO: if (status.initialFailureTime == 0) {
        //TODO:     status.initialFailureTime = lastSyncTime;
        //TODO: }
        Int32 failureCount;
        FAIL_RETURN(ds->GetFailureCount(&failureCount));
        failureCount++;
        FAIL_RETURN(ds->SetFailureCount(failureCount));
        Int64 failureTime;
        FAIL_RETURN(ds->GetFailureTime(&failureTime));
        failureTime += elapsedTime;
        FAIL_RETURN(ds->SetFailureTime(failureTime));
    }

    if (writeStatusNow) {
        FAIL_RETURN(WriteStatusLocked());
    //TODO: } else if (!hasMessages(CSyncStorageEngine::MSG_WRITE_STATUS)) {
    //TODO:     sendMessageDelayed(obtainMessage(MSG_WRITE_STATUS), WRITE_STATUS_DELAY);
    }
    if (writeStatisticsNow) {
        FAIL_RETURN(WriteStatisticsLocked());
    //TODO: } else if (!hasMessages(MSG_WRITE_STATISTICS)) {
    //TODO:     sendMessageDelayed(obtainMessage(MSG_WRITE_STATISTICS), WRITE_STATISTICS_DELAY);
    }
    
    FAIL_RETURN(ReportChange(ContentResolver_SYNC_OBSERVER_TYPE_STATUS));
    return NOERROR;
}

ECode CSyncStorageEngine::GetCurrentSync(
    /* [out] */ ISyncInfo** syncInfo)
{
    VALIDATE_NOT_NULL(syncInfo);
    Mutex::Autolock lock(mAuthoritiesLock);
    *syncInfo = mCurrentSync;
    return NOERROR;
}

ECode CSyncStorageEngine::GetSyncStatus(
    /* [out] */ IObjectContainer** syncStatusInfoList)
{
    VALIDATE_NOT_NULL(syncStatusInfoList);
    Mutex::Autolock lock(mAuthoritiesLock);
    const Int32 N = mSyncStatus->Size();
    FAIL_RETURN(CParcelableObjectContainer::New(syncStatusInfoList));

    for (Int32 i = 0; i < N; i++) {
        (*syncStatusInfoList)->Add(mSyncStatus->ValueAt(i));
    }
   
    return NOERROR;
}

ECode CSyncStorageEngine::GetAuthorities(
    /* [out] */ IObjectContainer** authorityInfoList)
{
    VALIDATE_NOT_NULL(authorityInfoList);
    Mutex::Autolock lock(mAuthoritiesLock);
    const Int32 N = mAuthorities->Size();
    FAIL_RETURN(CParcelableObjectContainer::New(authorityInfoList));

    for (Int32 i = 0; i < N; i++) {
        (*authorityInfoList)->Add(mAuthorities->ValueAt(i));
    }
    
    return NOERROR;
}

ECode CSyncStorageEngine::GetStatusByAccountAndAuthority(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ ISyncStatusInfo** result)
{
    VALIDATE_NOT_NULL(account);
    VALIDATE_NOT_NULL(result);

    if (authority.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Mutex::Autolock lock(mAuthoritiesLock);
    const Int32 N = mSyncStatus->Size();
    AutoPtr<ISyncStatusInfo> cur;
    AutoPtr<ISyncStorageEngineAuthorityInfo> ainfo;

    for (Int32 i = 0; i < N; i++) {
        cur = (ISyncStatusInfo*) mSyncStatus->ValueAt(i);
        //TODO: AuthorityInfo ainfo = mAuthorities.get(cur.authorityId);

        if (NULL != ainfo) {
            String tmpAuthority;
            AutoPtr<IAccount> tmpAccount;
            FAIL_RETURN(ainfo->GetAuthority(&tmpAuthority));
            FAIL_RETURN(ainfo->GetAccount((IAccount**)&tmpAccount));
            if (!tmpAuthority.Compare(authority) && _CObject_Compare(account, tmpAccount)) {
                *result = cur;
                return NOERROR;
            }
        }

    }

    *result = NULL;
    return NOERROR;
}

ECode CSyncStorageEngine::IsSyncPending(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ Boolean* isPending)
{
    VALIDATE_NOT_NULL(isPending);
    Mutex::Autolock lock(mAuthoritiesLock);
    const Int32 N = mSyncStatus->Size();
    AutoPtr<ISyncStatusInfo> cur;
    AutoPtr<ISyncStorageEngineAuthorityInfo> ainfo;

    for (Int32 i = 0; i < N; i++) {
        cur = (ISyncStatusInfo*) mSyncStatus->ValueAt(i);
        //TODO: AuthorityInfo ainfo = mAuthorities.get(cur.authorityId);
        if (NULL == ainfo) {
            continue;
        }

        if (NULL != account) {
            AutoPtr<IAccount> tmpAccount;
            FAIL_RETURN(ainfo->GetAccount((IAccount**)&tmpAccount));
            if (_CObject_Compare(tmpAccount, account)) continue;
        }
        //TODO: if (ainfo.authority.equals(authority) && cur.pending) {
        //TODO:     return true;
        //TODO: }
    }

    *isPending = FALSE;
    return NOERROR;
}

ECode CSyncStorageEngine::GetSyncHistory(
    /* [out] */ IObjectContainer** syncHistoryItemList)
{
    VALIDATE_NOT_NULL(syncHistoryItemList);
    Mutex::Autolock lock(mAuthoritiesLock);
    FAIL_RETURN(CParcelableObjectContainer::New(syncHistoryItemList));
    List<AutoPtr<ISyncStorageEngineSyncHistoryItem> >::Iterator iter = mSyncHistory->Begin();
    AutoPtr<ISyncStorageEngineSyncHistoryItem> historyItem;

    for (; iter != mSyncHistory->End(); ++iter) {
        historyItem = *iter;
        (*syncHistoryItemList)->Add(historyItem);
    }
    
    return NOERROR;
}

ECode CSyncStorageEngine::GetDayStatistics(
    /* [out, callee] */ ArrayOf<ISyncStorageEngineDayStats *>** dayStats)
{
    VALIDATE_NOT_NULL(dayStats);
    Mutex::Autolock lock(mAuthoritiesLock);
    //*dayStats = mDayStats->Clone();
    Int32 N = mDayStats->GetLength();

    for (Int32 i = 0; i != N; i++) {
        (**dayStats)[i] = (*mDayStats)[i];
    }

    return NOERROR;
}

ECode CSyncStorageEngine::GetInitialSyncFailureTime(
    /* [out] */ Int64* failureTime)
{
    VALIDATE_NOT_NULL(failureTime);
    Mutex::Autolock lock(mAuthoritiesLock);

    if (!mMasterSyncAutomatically) {
        *failureTime = 0;
        return NOERROR;
    }

    Boolean enabled;
    Int64 oldest = 0;
    Int32 i = mSyncStatus->Size();
    AutoPtr<ISyncStatusInfo> stats;
    AutoPtr<ISyncStorageEngineAuthorityInfo> authority;

    while (i > 0) {
        i--;
        stats = (ISyncStatusInfo*) mSyncStatus->ValueAt(i);
        //TODO: AuthorityInfo authority = mAuthorities.get(stats.authorityId);
        if (NULL != authority && (authority->GetEnabled(&enabled), enabled)) {
            //TODO: if (oldest == 0 || stats.initialFailureTime < oldest) {
            //TODO:     oldest = stats.initialFailureTime;
            //TODO: }
        }
    }

    *failureTime = oldest;
    return NOERROR;
}

ECode CSyncStorageEngine::GetOrCreateSyncStatus(
    /* [in] */ ISyncStorageEngineAuthorityInfo* authority,
    /* [out] */ ISyncStatusInfo** syncStatus)
{
    VALIDATE_NOT_NULL(authority);
    VALIDATE_NOT_NULL(syncStatus);
    Mutex::Autolock lock(mAuthoritiesLock);
    Int32 ident;
    FAIL_RETURN(authority->GetIdent(&ident));
    FAIL_RETURN(GetOrCreateSyncStatusLocked(ident, syncStatus));
    return NOERROR;
}

ECode CSyncStorageEngine::WriteAllState()
{
    Mutex::Autolock lock(mAuthoritiesLock);
    // Account info is always written so no need to do it here.

    if (mNumPendingFinished > 0) {
        // Only write these if they are out of date.
        FAIL_RETURN(WritePendingOperationsLocked());
    }

    // Just always write these...  they are likely out of date.
    FAIL_RETURN(WriteStatusLocked());
    FAIL_RETURN(WriteStatisticsLocked());
    return NOERROR;
}

ECode CSyncStorageEngine::ClearAndReadState()
{
    Mutex::Autolock lock(mAuthoritiesLock);

    if (NULL != mAuthorities) mAuthorities->Clear();
    if (NULL != mAccounts) mAccounts->Clear();
    if (NULL != mPendingOperations) mPendingOperations->Clear();
    if (NULL != mSyncStatus) mSyncStatus->Clear();
    if (NULL != mSyncHistory) mSyncHistory->Clear();

    FAIL_RETURN(ReadAccountInfoLocked());
    FAIL_RETURN(ReadStatusLocked());
    FAIL_RETURN(ReadPendingOperationsLocked());
    FAIL_RETURN(ReadStatisticsLocked());
    FAIL_RETURN(ReadAndDeleteLegacyAccountInfoLocked());
    FAIL_RETURN(WriteAccountInfoLocked());
    FAIL_RETURN(WriteStatusLocked());
    FAIL_RETURN(WritePendingOperationsLocked());
    FAIL_RETURN(WriteStatisticsLocked());
    
    return NOERROR;
}

ECode CSyncStorageEngine::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IFile* dataDir)
{
    mContext = context;
    sSyncStorageEngine = (ISyncStorageEngine*) this;

    //TODO: mCal = Calendar.getInstance(TimeZone.getTimeZone("GMT+0"));

    AutoPtr<IFile> systemDir;
    CFile::New(dataDir, String("system"), (IFile**) &systemDir);
    AutoPtr<IFile> syncDir;
    CFile::New(systemDir, String("sync"), (IFile**) &syncDir);
    Boolean succeeded;
    FAIL_RETURN(syncDir->Mkdirs(&succeeded));

    //TODO: mAccountInfoFile = new AtomicFile(new File(syncDir, "accounts.xml"));
    //TODO: mStatusFile = new AtomicFile(new File(syncDir, "status.bin"));
    //TODO: mPendingFile = new AtomicFile(new File(syncDir, "pending.bin"));
    //TODO: mStatisticsFile = new AtomicFile(new File(syncDir, "stats.bin"));

    FAIL_RETURN(ReadAccountInfoLocked());
    FAIL_RETURN(ReadStatusLocked());
    FAIL_RETURN(ReadPendingOperationsLocked());
    FAIL_RETURN(ReadStatisticsLocked());
    FAIL_RETURN(ReadAndDeleteLegacyAccountInfoLocked());
    FAIL_RETURN(WriteAccountInfoLocked());
    FAIL_RETURN(WriteStatusLocked());
    FAIL_RETURN(WritePendingOperationsLocked());
    FAIL_RETURN(WriteStatisticsLocked());
    return NOERROR;
}

Pair<Int64, Int64>* CSyncStorageEngine::GetBackoff(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName)
{
    Mutex::Autolock lock(mAuthoritiesLock);
    AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
    GetAuthorityLocked(account, providerName, String("getBackoff"), (ISyncStorageEngineAuthorityInfo**) &authority);
    Int64 backoffTime;

    if (NULL == authority || (authority->GetBackoffTime(&backoffTime), backoffTime) < 0) {
        return NULL;
    }

    Int64 backoffDelay;
    authority->GetBackoffDelay(&backoffDelay);
    Pair<Int64, Int64>* pair = new Pair<Int64, Int64>(backoffTime, backoffDelay);
    return pair;
}

ECode CSyncStorageEngine::GetIntColumn(
    /* [in] */ ICursor* cursor,
    /* [in] */ const String& name,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(cursor);
    VALIDATE_NOT_NULL(result);
    Int32 columnIndex;
    FAIL_RETURN(cursor->GetColumnIndex(name, &columnIndex));
    FAIL_RETURN(cursor->GetInt32(columnIndex, result));
    return NOERROR;
}

ECode CSyncStorageEngine::GetLongColumn(
    /* [in] */ ICursor* cursor,
    /* [in] */ const String& name,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(cursor);
    VALIDATE_NOT_NULL(result);
    Int32 columnIndex;
    FAIL_RETURN(cursor->GetColumnIndex(name, &columnIndex));
    FAIL_RETURN(cursor->GetInt64(columnIndex, result));
    return NOERROR;
}

ECode CSyncStorageEngine::DoStaticInitialization()
{
    (*sAuthorityRenames)[String("contacts")] = String("com.android.contacts");
    (*sAuthorityRenames)[String("calendar")] = String("com.android.calendar");
    return NOERROR;
}

ECode CSyncStorageEngine::FlattenBundle(
    /* [in] */ IBundle* bundle,
    /* [out, callee] */ ArrayOf<Byte>** flattenBundleArray)
{
    VALIDATE_NOT_NULL(bundle);
    VALIDATE_NOT_NULL(flattenBundleArray);
    AutoPtr<IParcel> parcel; // Parcel parcel = Parcel.obtain();
    ECode ec = NOERROR;
    AutoPtr<IParcelable> parcelable = (IParcelable*) bundle->Probe(EIID_IParcelable);
    ec = parcelable->WriteToParcel(parcel);
    FAIL_WithGoto(ec);
    //TODO: *flattenBundleArray = parcel.marshall();
    EXIT:
    //TODO: parcel.recycle();
    FAIL_RETURN(ec);
    return NOERROR;
}

ECode CSyncStorageEngine::UnflattenBundle(
        /* [in] */ ArrayOf<Byte>* flatData,
        /* [out] */ IBundle** bundle)
{
    VALIDATE_NOT_NULL(bundle);
    AutoPtr<IParcel> parcel; // Parcel parcel = Parcel.obtain();
    ECode ec = NOERROR;
    //TODO: parcel.unmarshall(flatData, 0, flatData.length);
    ec = parcel->SetDataPosition(0);
    FAIL_WithGoto(ec);
    //TODO: *bundle = parcel.readBundle();
    EXIT:
    if (E_RUNTIME_EXCEPTION == ec) {
        // A RuntimeException is thrown if we were unable to parse the parcel.
        // Create an empty parcel in this case.
        FAIL_RETURN(CBundle::New(bundle));
    }
    //TODO: parcel.recycle();
    FAIL_RETURN(ec);
    return NOERROR;
}

ECode CSyncStorageEngine::ReportChange(
    /* [in] */ Int32 which)
{
    List<AutoPtr<ISyncStatusObserver> >* reports = NULL;
    AutoPtr<ISyncStatusObserver> observer;
    Mutex::Autolock lock(mAuthoritiesLock);
    Int32 i;
    FAIL_RETURN(mChangeListeners->BeginBroadcast(&i));

    while (i > 0) {
        i--;
        AutoPtr<IInteger32> mask;
        FAIL_RETURN(mChangeListeners->GetBroadcastCookie(i, (IInterface**)&mask));
        Int32 intValue;
        FAIL_RETURN(mask->GetValue(&intValue));
        if ((which & intValue) == 0) {
            continue;
        }
        if (NULL == reports) {
            reports = new List<AutoPtr<ISyncStatusObserver> >(i);
        }
        FAIL_RETURN(mChangeListeners->GetBroadcastItem(i, (IInterface**) &observer));
        reports->PushBack(observer);
    }

    FAIL_RETURN(mChangeListeners->FinishBroadcast());
    
    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
        String str("reportChange ");
        String strWhich = String::FromInt32(which);
        str.Append(strWhich);
        str.Append(" to: ");
        //TODO: reports.toString();
        Logger::V(TAG, str);
    }

    if (NULL != reports) {
        Int32 i = reports->GetSize();
        while (i > 0) {
            i--;
            //TODO: reports.get(i).onStatusChanged(which);
        }
    }

    return NOERROR;
}

ECode CSyncStorageEngine::UpdateOrRemovePeriodicSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 period,
    /* [in] */ Boolean add)
{
    if (period <= 0) {
        period = 0;
    }
    if (NULL == extras) {
        FAIL_RETURN(CBundle::New(&extras));
    }

    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
        String str("addOrRemovePeriodicSync: ");
        if (NULL != account) {
            //TODO: account->ToString();
        } else {
            str.Append(" null ");
        }
        str.Append(", provider ");
        str.Append(providerName);
        str.Append(" -> period ");
        String strPeriod = String::FromInt64(period);
        str.Append(strPeriod);
        str.Append(", extras ");
        //TODO: extras->ToString();
        Logger::V(TAG, str);
    }

    Mutex::Autolock lock(mAuthoritiesLock);
    ECode ec = NOERROR;
    {
        AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
        ec = GetOrCreateAuthorityLocked(account, providerName, -1, FALSE, (ISyncStorageEngineAuthorityInfo**)&authority);
        FAIL_WithGoto(ec);
        Int32 ident;
        ec = authority->GetIdent(&ident);
        FAIL_WithGoto(ec);
        AutoPtr<ISyncStatusInfo> status;

        CSyncStorageEngineAuthorityInfo* authorityInfo = (CSyncStorageEngineAuthorityInfo*) authority.Get();
        List<Pair<AutoPtr<IBundle>, Int64> >* periodicSyncs = authorityInfo->GetPeriodicSyncs();
        List<Pair<AutoPtr<IBundle>, Int64> >::Iterator iter = periodicSyncs->Begin();
        Pair<AutoPtr<IBundle>, Int64> syncInfo;
        AutoPtr<IBundle> existingExtras;
        Int32 index = 0;

        if (add) {
            // add this periodic sync if one with the same extras doesn't already
            // exist in the periodicSyncs array
            Boolean alreadyPresent = FALSE;

            for (; iter != periodicSyncs->End(); ++iter) {
                syncInfo = *iter;
                existingExtras = syncInfo.mFirst;
                if(_CObject_Compare(existingExtras, extras)) {
                    if (syncInfo.mSecond == period) {
                        return NOERROR;
                    }
                    Pair<AutoPtr<IBundle>, Int64> temPair(extras, period);
                    periodicSyncs->Insert(index, temPair);
                    alreadyPresent = TRUE;
                    break;
                }
                index++;
            }

            // if we added an entry to the periodicSyncs array also add an entry to
            // the periodic syncs status to correspond to it
            if (!alreadyPresent) {
                Pair<AutoPtr<IBundle>, Int64> temPair(extras, period);
                periodicSyncs->PushBack(temPair);
                ec = GetOrCreateSyncStatusLocked(ident, (ISyncStatusInfo**)&status);
                FAIL_WithGoto(ec);
                //TODO: status.setPeriodicSyncTime(authority.periodicSyncs.size() - 1, 0);
            }
        } else {
            // remove any periodic syncs that match the authority and extras
            status = (ISyncStatusInfo*) mSyncStatus->Get(ident);
            Boolean changed = FALSE;

            for (; iter != periodicSyncs->End(); ++iter) {
                syncInfo = *iter;
                existingExtras = syncInfo.mFirst;
                if(_CObject_Compare(existingExtras, extras)) {
                    periodicSyncs->Erase(iter);
                    changed = TRUE;
                    // if we removed an entry from the periodicSyncs array also
                    // remove the corresponding entry from the status
                    if (NULL != status) {
                        //TODO: status.removePeriodicSyncTime(i);
                    }
                } else {
                    index++;
                }
            }

            if (!changed) {
                return NOERROR;
            }
        }
    }
EXIT:
    FAIL_RETURN(WriteAccountInfoLocked());
    FAIL_RETURN(WriteStatusLocked());
    FAIL_RETURN(ec);
    FAIL_RETURN(ReportChange(ContentResolver_SYNC_OBSERVER_TYPE_SETTINGS));
    return NOERROR;
}

ECode CSyncStorageEngine::GetCurrentDayLocked(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    //TODO: mCal.setTimeInMillis(System.currentTimeMillis());
    //TODO: final int dayOfYear = mCal.get(Calendar.DAY_OF_YEAR);
    //TODO: if (mYear != mCal.get(Calendar.YEAR)) {
    //TODO:     mYear = mCal.get(Calendar.YEAR);
    //TODO:     mCal.clear();
    //TODO:     mCal.set(Calendar.YEAR, mYear);
    //TODO:     mYearInDays = (int)(mCal.getTimeInMillis()/86400000);
    //TODO: }
    //TODO: return dayOfYear + mYearInDays;
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetAuthorityLocked(
    /* [in] */ IAccount* accountName,
    /* [in] */ const String& authorityName,
    /* [in] */ const String& tag,
    /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo)
{
    VALIDATE_NOT_NULL(authorityInfo);
    HashMap<AutoPtr<IAccount>, AutoPtr<CSyncStorageEngine::AccountInfo> >::Iterator it = mAccounts->Find(accountName);
    AutoPtr<CSyncStorageEngine::AccountInfo> accountInfo = it->mSecond;

    if (NULL == accountInfo) {
        if (!tag.IsNull()) {
            if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
                String str(tag);
                str.Append(": unknown account ");
                if (NULL != accountName) {
                    //TODO: accountName->ToString();
                } else {
                    str.Append(" null ");
                }
                Logger::V(TAG, str);
            }
        }
        *authorityInfo = NULL;
        return NOERROR;
    }

    HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> >* authorities = accountInfo->mAuthorities;
    HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> >::Iterator iter = authorities->Find(authorityName);
    *authorityInfo = iter->mSecond;

    if (NULL == authorityInfo) {
        if (!tag.IsNull()) {
            if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
                String str(tag);
                str.Append(": unknown authority ");
                str.Append(authorityName);
                Logger::V(TAG, str);
            }
        }
        *authorityInfo = NULL;
        return NOERROR;
    }

    return NOERROR;
}

ECode CSyncStorageEngine::GetOrCreateAuthorityLocked(
    /* [in] */ IAccount* accountName,
    /* [in] */ const String& authorityName,
    /* [in] */ Int32 ident,
    /* [in] */ Boolean doWrite,
    /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo)
{
    HashMap<AutoPtr<IAccount>, AutoPtr<CSyncStorageEngine::AccountInfo> >::Iterator it = mAccounts->Find(accountName);
    AutoPtr<CSyncStorageEngine::AccountInfo> accountInfo = it->mSecond;

    if (NULL == accountInfo) {
        accountInfo = new CSyncStorageEngine::AccountInfo(accountName);
        (*mAccounts)[accountName] = accountInfo;
    }

    HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> >* authorities = accountInfo->mAuthorities;
    HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> >::Iterator iter = authorities->Find(authorityName);
    *authorityInfo = iter->mSecond;

    if (NULL == authorityInfo) {
        if (ident < 0) {
            ident = mNextAuthorityId;
            mNextAuthorityId++;
            doWrite = TRUE;
        }

        if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
            String str("created a new AuthorityInfo for ");
            if (NULL != accountName) {
            //TODO: accountName->ToString();
            } else {
                str.Append(" null ");
            }
            str.Append(", provider ");
            str.Append(authorityName);
            Logger::V(TAG, str);
        }

        FAIL_RETURN(CSyncStorageEngineAuthorityInfo::New(accountName, authorityName, ident, authorityInfo));
        (*authorities)[authorityName] = *authorityInfo;
        mAuthorities->Put(ident, *authorityInfo);

        if (doWrite) {
            FAIL_RETURN(WriteAccountInfoLocked());
        }
    }

    return NOERROR;
}

ECode CSyncStorageEngine::RemoveAuthorityLocked(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authorityName,
    /* [in] */ Boolean doWrite)
{
    HashMap<AutoPtr<IAccount>, AutoPtr<CSyncStorageEngine::AccountInfo> >::Iterator it = mAccounts->Find(account);
    AutoPtr<CSyncStorageEngine::AccountInfo> accountInfo = it->mSecond;

    if (NULL != accountInfo) {
        HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> >* authorities = accountInfo->mAuthorities;
        HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> >::Iterator iter = authorities->Find(authorityName);
        AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo = iter->mSecond;
        authorities->Erase(authorityName);

        if (NULL != authorityInfo) {
            Int32 ident;
            FAIL_RETURN(authorityInfo->GetIdent(&ident));
            mAuthorities->Remove(ident);
            if (doWrite) {
                FAIL_RETURN(WriteAccountInfoLocked());
            }
        }

    }

    return NOERROR;
}

ECode CSyncStorageEngine::GetOrCreateSyncStatusLocked(
    /* [in] */ Int32 authorityId,
    /* [out] */ ISyncStatusInfo** syncStatusInfo)
{
    VALIDATE_NOT_NULL(syncStatusInfo);
    *syncStatusInfo = (ISyncStatusInfo*) mSyncStatus->Get(authorityId);

    if (NULL == *syncStatusInfo) {
        //TODO: FAIL_RETURN(CSyncStatusInfo::New(authorityId, syncStatusInfo));
        //TODO: mSyncStatus.Put(authorityId, *syncStatusInfo);
    }

    return NOERROR;
}

ECode CSyncStorageEngine::ReadAccountInfoLocked()
{
    Int32 highestAuthorityId = -1;
    AutoPtr<IFileInputStream> fis;
    ECode ec = NOERROR;
    {
        //TODO: fis = mAccountInfoFile.openRead();
        if (CSyncStorageEngine::DEBUG_FILE) {
            //TODO: Log.v(TAG, "Reading " + mAccountInfoFile.getBaseFile());
        }
        AutoPtr<IXmlPullParser> parser;
        AutoPtr<IXmlPullParserFactory> parserFactory;
        ec = CXmlPullParserFactory::New((IXmlPullParserFactory**) &parserFactory);
        FAIL_WithGoto(ec);
        ec = parserFactory->NewPullParser((IXmlPullParser**) &parser);
        FAIL_WithGoto(ec);
        String inputEncoding;
        ec = parser->SetInputEx(fis, inputEncoding);
        FAIL_WithGoto(ec);
        Int32 eventType;
        ec = parser->GetEventType(&eventType);
        FAIL_WithGoto(ec);
        while (eventType != IXmlPullParser_START_TAG) {
            ec = parser->Next(&eventType);
            FAIL_WithGoto(ec);
        }
        String tagName;
        ec = parser->GetName(&tagName);
        FAIL_WithGoto(ec);
        if (!String("accounts").Compare(tagName)) {
            String listen;
            ec = parser->GetAttributeValueEx(NULL, String("listen-for-tickles"), &listen);
            FAIL_WithGoto(ec);
            String versionString;
            ec = parser->GetAttributeValueEx(NULL, String("version"), &versionString);
            FAIL_WithGoto(ec);
            Int32 version = 0;
            version = versionString.IsNull() ? 0 : versionString.ToInt32();
            String nextIdString;
            ec = parser->GetAttributeValueEx(NULL, String("nextAuthorityId"), &nextIdString);
            FAIL_WithGoto(ec);
            Int32 id = nextIdString.IsNull() ? 0 : nextIdString.ToInt32();
            mNextAuthorityId = Math::Max(mNextAuthorityId, id);
            mMasterSyncAutomatically = listen.IsNull() || listen.ToBoolean();
            ec = parser->Next(&eventType);
            FAIL_WithGoto(ec);
            AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
            Pair<AutoPtr<IBundle>, Int64>* periodicSync = NULL;
            Int32 depth;
            do {
                if (eventType == IXmlPullParser_START_TAG) {
                    ec = parser->GetName(&tagName);
                    FAIL_WithGoto(ec);
                    ec = parser->GetDepth(&depth);
                    FAIL_WithGoto(ec);
                    if (2 == depth) {
                        if (!String("authority").Compare(tagName)) {
                            ec = ParseAuthority(parser, version, (ISyncStorageEngineAuthorityInfo**) &authority);
                            FAIL_WithGoto(ec);
                            periodicSync = NULL;
                            Int32 ident;
                            ec = authority->GetIdent(&ident);
                            FAIL_WithGoto(ec);
                            if (ident > highestAuthorityId) {
                                highestAuthorityId = ident;
                            }
                        }
                    } else if (3 == depth) {
                        if (!String("periodicSync").Compare(tagName) && NULL != authority) {
                            periodicSync = ParsePeriodicSync(parser, authority);
                        }
                    } else if (4 == depth && NULL != periodicSync) {
                        if (!String("extra").Compare(tagName)) {
                            ParseExtra(parser, periodicSync);
                        }
                    }
                }
                ec = parser->Next(&eventType);
                FAIL_WithGoto(ec);
            } while (eventType != IXmlPullParser_END_DOCUMENT);
        }
    }
    EXIT:
    //TODO: if(ec = XmlPullParserException) {
    //TODO:     String str("Error reading accounts ec = XmlPullParserException");
    //TODO:     Logger::W(TAG, str);
    //TODO: }
    if (ec == E_IO_EXCEPTION) {
        if (NULL == fis) {
            String str("No initial accounts");
            Logger::I(TAG, str);
        } else {
            String str("Error reading accounts ec = E_IO_EXCEPTION");
            Logger::W(TAG, str);
        }
    }
    mNextAuthorityId = Math::Max(highestAuthorityId + 1, mNextAuthorityId);

    if (NULL != fis) {
        FAIL_RETURN(fis->Close());
    }

    FAIL_RETURN(ec);
    Boolean isChange;
    FAIL_RETURN(MaybeMigrateSettingsForRenamedAuthorities(&isChange));
    return NOERROR;
}

ECode CSyncStorageEngine::MaybeMigrateSettingsForRenamedAuthorities(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean writeNeeded = FALSE;
    List<AutoPtr<ISyncStorageEngineAuthorityInfo> >* authoritiesToRemove = new List<AutoPtr<ISyncStorageEngineAuthorityInfo> >();
    AutoPtr<ISyncStorageEngineAuthorityInfo> authority;
    const Int32 N = mAuthorities->Size();

    for (Int32 i = 0; i < N; i++) {
        authority = (ISyncStorageEngineAuthorityInfo*) mAuthorities->ValueAt(i);
        String strAuthority;
        FAIL_RETURN(authority->GetAuthority(&strAuthority));
        // skip this authority if it isn't one of the renamed ones
        HashMap<String, String>::Iterator it = sAuthorityRenames->Find(strAuthority);
        String newAuthorityName;
        newAuthorityName.SetTo(it->mSecond);

        if (newAuthorityName.IsNull()) {
            continue;
        }

        // remember this authority so we can remove it later. we can't remove it
        // now without messing up this loop iteration
        authoritiesToRemove->PushBack(authority);

        // this authority isn't enabled, no need to copy it to the new authority name since
        // the default is "disabled"
        Boolean enabled;
        FAIL_RETURN(authority->GetEnabled(&enabled));
        if (!enabled) {
            continue;
        }

        // if we already have a record of this new authority then don't copy over the settings
        AutoPtr<IAccount> account;
        FAIL_RETURN(authority->GetAccount((IAccount**)&account));
        AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
        FAIL_RETURN(GetAuthorityLocked(account, newAuthorityName, String("cleanup"), (ISyncStorageEngineAuthorityInfo**)&authorityInfo));

        if (NULL != authorityInfo) {
            continue;
        }

        AutoPtr<ISyncStorageEngineAuthorityInfo> newAuthority;
        FAIL_RETURN(GetOrCreateAuthorityLocked(account, newAuthorityName, -1 /* ident */, FALSE /* doWrite */, (ISyncStorageEngineAuthorityInfo**)&newAuthority));
        FAIL_RETURN(newAuthority->SetEnabled(TRUE));
        writeNeeded = TRUE;
    }

    List<AutoPtr<ISyncStorageEngineAuthorityInfo> >::Iterator iter = authoritiesToRemove->Begin();
    AutoPtr<ISyncStorageEngineAuthorityInfo> authorityInfo;
    AutoPtr<IAccount> account;
    String strAuthority;

    for (; iter != authoritiesToRemove->End(); ++iter) {
        authorityInfo = *iter;
        FAIL_RETURN(authorityInfo->GetAccount((IAccount**)&account));
        FAIL_RETURN(authorityInfo->GetAuthority(&strAuthority));
        FAIL_RETURN(RemoveAuthorityLocked(account, strAuthority, FALSE /* doWrite */));
        writeNeeded = TRUE;
    }

    *result = writeNeeded;
    return NOERROR;
}

ECode CSyncStorageEngine::ParseAuthority(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ Int32 version,
    /* [out] */ ISyncStorageEngineAuthorityInfo** authority)
{
    VALIDATE_NOT_NULL(authority);
    if (NULL == parser) {
        String str("the id of the authority is null");
        Logger::E(TAG, str);
        return E_NULL_POINTER_EXCEPTION;
    }
    Int32 id = -1;
    String strValue;
    FAIL_RETURN(parser->GetAttributeValueEx(NULL, String("id"), &strValue));

    if (strValue.IsNull()) {
        String str("error parsing the id of the authority, the id of the authority is null");
        Logger::E(TAG, str);
    } else {
        id = strValue.ToInt32();
    }
    
    if (id >= 0) {
        String authorityName;
        FAIL_RETURN(parser->GetAttributeValueEx(NULL, String("authority"), &authorityName));
        String enabled;
        FAIL_RETURN(parser->GetAttributeValueEx(NULL, String("enabled"), &enabled));
        String syncable;
        FAIL_RETURN(parser->GetAttributeValueEx(NULL, String("syncable"), &syncable));
        String accountName;
        FAIL_RETURN(parser->GetAttributeValueEx(NULL, String("account"), &accountName));
        String accountType;
        FAIL_RETURN(parser->GetAttributeValueEx(NULL, String("type"), &accountType));

        if (accountType.IsNull()) {
            accountType.SetTo("com.google");
            syncable.SetTo("unknown");
        }

        *authority = (ISyncStorageEngineAuthorityInfo*) mAuthorities->Get(id);

        if (CSyncStorageEngine::DEBUG_FILE) {
            String str("Adding authority: account=");
            str.Append(accountName);
            str.Append(" auth=");
            str.Append(authorityName);
            str.Append(" enabled=");
            str.Append(enabled);
            str.Append(" syncable=");
            str.Append(syncable);
            Logger::V(TAG, str);
        } 

        if (NULL == *authority) {
            if (CSyncStorageEngine::DEBUG_FILE) {
                String str("Creating entry");
                Logger::V(TAG, str);
            }
            AutoPtr<IAccount> account;
            FAIL_RETURN(CAccount::New(accountName, accountType, (IAccount**)&account));
            FAIL_RETURN(GetOrCreateAuthorityLocked(account, authorityName, id, FALSE /* doWrite */, authority));
            // If the version is 0 then we are upgrading from a file format that did not
            // know about periodic syncs. In that case don't clear the list since we
            // want the default, which is a daily periodioc sync.
            // Otherwise clear out this default list since we will populate it later with
            // the periodic sync descriptions that are read from the configuration file.
            if (version > 0) {
                List<Pair<AutoPtr<IBundle>, Int64> >* periodicSyncs = ((CSyncStorageEngineAuthorityInfo*) (*authority))->GetPeriodicSyncs();
                if (NULL != periodicSyncs) {
                    periodicSyncs->Clear();
                }
            }
        }

        if (NULL != *authority) {
            FAIL_RETURN((*authority)->SetEnabled(enabled.IsNull() || enabled.ToBoolean()));
            if (String("unknown").Compare(syncable)) {
                FAIL_RETURN((*authority)->SetSyncable(-1));
            } else {
                FAIL_RETURN((*authority)->SetSyncable((syncable.IsNull() || syncable.ToBoolean()) ? 1 : 0));
            }
        } else {
            String str("Failure adding authority: account=");
            str.Append(accountName);
            str.Append(" auth=");
            str.Append(authorityName);
            str.Append(" enabled=");
            str.Append(enabled);
            str.Append(" syncable=");
            str.Append(syncable);
            Logger::W(TAG, str);
        }

    }

    return NOERROR;
}

Pair<AutoPtr<IBundle>, Int64>* CSyncStorageEngine::ParsePeriodicSync(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ ISyncStorageEngineAuthorityInfo* authority)
{
    AutoPtr<IBundle> extras;
    CBundle::New((IBundle**) &extras);
    String periodValue;
    parser->GetAttributeValueEx(NULL, String("period"), &periodValue);
    Int64 period;

    if (periodValue.IsNull()) {
        String str("error parsing the period of a periodic sync. the period of a periodic sync is null");
        Logger::E(TAG, str);
        return NULL;
    } else {
        period = periodValue.ToInt64();
    }

    Pair<AutoPtr<IBundle>, Int64>* periodicSync = new Pair<AutoPtr<IBundle>, Int64>(extras, period);
    CSyncStorageEngineAuthorityInfo* authorityInfo = (CSyncStorageEngineAuthorityInfo*) authority;
    List<Pair<AutoPtr<IBundle>, Int64> >* periodicSyncs = authorityInfo->GetPeriodicSyncs();
    if (NULL != periodicSyncs) {
        periodicSyncs->PushBack(*periodicSync);
    }
    return periodicSync;
}

void CSyncStorageEngine::ParseExtra(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ Pair<AutoPtr<IBundle>, Int64>* periodicSync)
{
    AutoPtr<IBundle> extras = periodicSync->mFirst;
    String name;
    parser->GetAttributeValueEx(NULL, String("name"), &name);
    String type;
    parser->GetAttributeValueEx(NULL, String("type"), &type);
    String strValue1;
    parser->GetAttributeValueEx(NULL, String("value1"), &strValue1);
    String strValue2;
    parser->GetAttributeValueEx(NULL, String("value2"), &strValue2);

    if (!String("long").Compare(type)) {
        if (strValue1.IsNull()) {
            String str("error parsing bundle value. strValue1 is null");
            Logger::E(TAG, str);
        } else {
            Int64 value1 = strValue1.ToInt64();
            extras->PutInt64(name, value1);
        }
    } else if (!String("integer").Compare(type)) {
        if (strValue1.IsNull()) {
            String str("error parsing bundle value. strValue1 is null");
            Logger::E(TAG, str);
        } else {
            Int32 value1 = strValue1.ToInt32();
            extras->PutInt32(name, value1);
        }
    } else if (!String("double").Compare(type)) {
        if (strValue1.IsNull()) {
            String str("error parsing bundle value. strValue1 is null");
            Logger::E(TAG, str);
        } else {
            Double value1 = strValue1.ToDouble();
            extras->PutDouble(name, value1);
        }
    } else if (!String("float").Compare(type)) {
        if (strValue1.IsNull()) {
            String str("error parsing bundle value. strValue1 is null");
            Logger::E(TAG, str);
        } else {
            Float value1 = (Float) strValue1.ToDouble();
            extras->PutFloat(name, value1);
        }
    } else if (!String("boolean").Compare(type)) {
        if (strValue1.IsNull()) {
            String str("error parsing bundle value. strValue1 is null");
            Logger::E(TAG, str);
        } else {
            Boolean value1 = strValue1.ToBoolean();
            extras->PutBoolean(name, value1);
        }
    } else if (!String("string").Compare(type)) {
        extras->PutString(name, strValue1);
    } else if (!String("account").Compare(type)) {
        AutoPtr<IAccount> account;
        CAccount::New(strValue1, strValue2, (IAccount**) &account);
        //TODO: extras->PutParcelable(name, account);
    }

}

ECode CSyncStorageEngine::WriteAccountInfoLocked()
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::ReadAndDeleteLegacyAccountInfoLocked()
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::ReadStatusLocked()
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::WriteStatusLocked()
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::ReadPendingOperationsLocked()
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::WritePendingOperationLocked(
    /* [in] */ ISyncStorageEnginePendingOperation* op,
    /* [in] */ IParcel* out)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::WritePendingOperationsLocked()
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::AppendPendingOperationLocked(
    /* [in] */ ISyncStorageEnginePendingOperation* op)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::ReadStatisticsLocked()
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::WriteStatisticsLocked()
{
    return E_NOT_IMPLEMENTED;
}

