
#include "content/CSyncStorageEngineAuthorityInfo.h"
#include "ext/frameworkdef.h"
#include "os/CBundle.h"

CSyncStorageEngineAuthorityInfo::CSyncStorageEngineAuthorityInfo()
    : mPeriodicSyncs(NULL)
{}

CSyncStorageEngineAuthorityInfo::~CSyncStorageEngineAuthorityInfo()
{
    if (NULL != mPeriodicSyncs) {
        List<Pair<AutoPtr<IBundle>, Int64>* >::Iterator iter = mPeriodicSyncs->Begin();

        for (; iter != mPeriodicSyncs->End(); ++iter) {
            delete *iter;
        }

        mPeriodicSyncs->Clear();
        delete mPeriodicSyncs;
    }
}

ECode CSyncStorageEngineAuthorityInfo::GetAccount(
    /* [out] */ IAccount** account)
{
    VALIDATE_NOT_NULL(account);
    *account = mAccount;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::GetAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority);
    *authority = mAuthority;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::GetIdent(
    /* [out] */ Int32* ident)
{
    VALIDATE_NOT_NULL(ident);
    *ident = mIdent;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::GetEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = mEnabled;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::SetEnabled(
    /* [in] */ Boolean enabled)
{
    mEnabled = enabled;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::GetSyncable(
    /* [out] */ Int32* syncable)
{
    VALIDATE_NOT_NULL(syncable);
    *syncable = mSyncable;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::SetSyncable(
    /* [in] */ Int32 syncable)
{
    mSyncable = syncable;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::GetBackoffTime(
    /* [out] */ Int64* backoffTime)
{
    VALIDATE_NOT_NULL(backoffTime);
    *backoffTime = mBackoffTime;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::SetBackoffTime(
    /* [in] */ Int64 backoffTime)
{
    mBackoffTime = backoffTime;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::GetBackoffDelay(
    /* [out] */ Int64* backoffDelay)
{
    VALIDATE_NOT_NULL(backoffDelay);
    *backoffDelay = mBackoffDelay;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::SetBackoffDelay(
    /* [in] */ Int64 backoffDelay)
{
    mBackoffDelay = backoffDelay;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::GetDelayUntil(
    /* [out] */ Int64* delayUntil)
{
    VALIDATE_NOT_NULL(delayUntil);
    *delayUntil = mDelayUntil;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::SetDelayUntil(
    /* [in] */ Int64 delayUntil)
{
    mDelayUntil = delayUntil;
    return NOERROR;
}

List<Pair<AutoPtr<IBundle>, Int64>* >* CSyncStorageEngineAuthorityInfo::GetPeriodicSyncs()
{
    return mPeriodicSyncs;
}

ECode CSyncStorageEngineAuthorityInfo::constructor(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ Int32 ident)
{
    mAccount = account;
    mAuthority = authority;
    mIdent = ident;
    mEnabled = FALSE; // SYNC_ENABLED_DEFAULT = false;
    mSyncable = -1; // default to "unknown"
    mBackoffTime = -1; // if < 0 then we aren't in backoff mode
    mBackoffDelay = -1; // if < 0 then we aren't in backoff mode
    mPeriodicSyncs = new List<Pair<AutoPtr<IBundle>, Int64>* >();
    AutoPtr<IBundle> newBundle;
    FAIL_RETURN(CBundle::New((IBundle**)&newBundle));
    Pair<AutoPtr<IBundle>, Int64>* newPair = new Pair<AutoPtr<IBundle>, Int64>(newBundle, 60 * 60 * 24); // DEFAULT_POLL_FREQUENCY_SECONDS = 60 * 60 * 24; // One day
    mPeriodicSyncs->PushBack(newPair);
    return NOERROR;
}

