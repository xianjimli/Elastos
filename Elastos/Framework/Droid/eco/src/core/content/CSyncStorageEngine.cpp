
#include "content/CSyncStorageEngine.h"
#include "ext/frameworkext.h"
#include <Logger.h>

using namespace Elastos::Core;

const CString CSyncStorageEngine::TAG = "SyncManager";
const Boolean CSyncStorageEngine::DEBUG_FILE = FALSE;
const Int64 CSyncStorageEngine::DEFAULT_POLL_FREQUENCY_SECONDS = 60 * 60 * 24; // One day
const Int64 CSyncStorageEngine::MILLIS_IN_4WEEKS = 1000L * 60 * 60 * 24 * 7 * 4;
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
    , mAuthorities(NULL)
    , mSyncStatus(NULL)
{
    mDayStats = ArrayOf<AutoPtr<ISyncStorageEngineDayStats> >::Alloc(7*4);
}

CSyncStorageEngine::~CSyncStorageEngine()
{
    ArrayOf<AutoPtr<ISyncStorageEngineDayStats> >::Free(mDayStats);
}

ECode CSyncStorageEngine::HandleMessage(
    /* [in] */ IMessage* msg)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::AddStatusChangeListener(
    /* [in] */ Int32 mask,
    /* [in] */ ISyncStatusObserver* syncStatusObserver)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::RemoveStatusChangeListener(
    /* [in] */ ISyncStatusObserver* syncStatusObserver)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetSyncAutomatically(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::SetSyncAutomatically(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [in] */ Boolean sync)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetIsSyncable(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::SetIsSyncable(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [in] */ Int32 syncable)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::SetBackoff(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [in] */ Int64 nextSyncTime,
    /* [in] */ Int64 nextDelay)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::ClearAllBackoffs()
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::SetDelayUntilTime(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [in] */ Int64 delayUntil)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetDelayUntilTime(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [out] */ Int64* delayUntilTime)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::AddPeriodicSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 pollFrequency)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::RemovePeriodicSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [in] */ IBundle* extras)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetPeriodicSyncs(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [out] */ IObjectContainer** periodicSyncList)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::SetMasterSyncAutomatically(
    /* [in] */ Boolean flag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetMasterSyncAutomatically(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetOrCreateAuthority(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::RemoveAuthority(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetAuthority(
    /* [in] */ Int32 authoriyId,
    /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::IsSyncActive(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ Boolean* isActive)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::InsertIntoPending(
    /* [in] */ ISyncStorageEnginePendingOperation* op,
    /* [out] */ ISyncStorageEnginePendingOperation** pendingOperation)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::DeleteFromPending(
    /* [in] */ ISyncStorageEnginePendingOperation* op,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::ClearPending(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetPendingOperations(
    /* [out] */ IObjectContainer** pendingOperationList)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetPendingOperationCount(
    /* [out] */ Int32* count)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::DoDatabaseCleanup(
    /* [in] */ ArrayOf<IAccount *>* accounts)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::ReportActiveChange()
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::InsertStartSyncEvent(
    /* [in] */ IAccount* accountName,
    /* [in] */ const String& authorityName,
    /* [in] */ Int64 now,
    /* [in] */ Int32 source,
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::StopSyncEvent(
    /* [in] */ Int64 historyId,
    /* [in] */ Int64 elapsedTime,
    /* [in] */ const String& resultMessage,
    /* [in] */ Int64 downstreamActivity,
    /* [in] */ Int64 upstreamActivity)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetCurrentSync(
    /* [out] */ ISyncInfo** syncInfo)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetSyncStatus(
    /* [out] */ IObjectContainer** syncStatusInfoList)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetAuthorities(
    /* [out] */ IObjectContainer** authorityInfoList)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetStatusByAccountAndAuthority(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ ISyncStatusInfo** result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::IsSyncPending(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ Boolean* isPending)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetSyncHistory(
    /* [out] */ IObjectContainer** syncHistoryItemList)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetDayStatistics(
    /* [out, callee] */ ArrayOf<ISyncStorageEngineDayStats *>** dayStats)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetInitialSyncFailureTime(
    /* [out] */ Int64* failureTime)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetOrCreateSyncStatus(
    /* [in] */ ISyncStorageEngineAuthorityInfo* authority,
    /* [out] */ ISyncStatusInfo** syncStatus)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::WriteAllState()
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::ClearAndReadState()
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IFile* dataDir)
{
    return E_NOT_IMPLEMENTED;
}

void CSyncStorageEngine::GetBackoff(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [out] */ Pair<Int64, Int64>** pairs)
{}

ECode CSyncStorageEngine::GetIntColumn(
    /* [in] */ ICursor* cursor,
    /* [in] */ const String& name,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetLongColumn(
    /* [in] */ ICursor* cursor,
    /* [in] */ const String& name,
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
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
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::UnflattenBundle(
        /* [in] */ ArrayOf<Byte>* flatData,
        /* [out] */ IBundle** bundle)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::ReportChange(
    /* [in] */ Int32 which)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::UpdateOrRemovePeriodicSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& providerName,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 period,
    /* [in] */ Boolean add)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetCurrentDayLocked(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetAuthorityLocked(
    /* [in] */ IAccount* accountName,
    /* [in] */ const String& authorityName,
    /* [in] */ const String& tag,
    /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetOrCreateAuthorityLocked(
    /* [in] */ IAccount* accountName,
    /* [in] */ const String& authorityName,
    /* [in] */ Int32 ident,
    /* [in] */ Boolean doWrite,
    /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::RemoveAuthorityLocked(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authorityName,
    /* [in] */ Boolean doWrite)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::GetOrCreateSyncStatusLocked(
    /* [in] */ Int32 authorityId,
    /* [out] */ ISyncStatusInfo** syncStatusInfo)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::ReadAccountInfoLocked()
{
    return E_NOT_IMPLEMENTED;
}

ECode CSyncStorageEngine::MaybeMigrateSettingsForRenamedAuthorities(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
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

