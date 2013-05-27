
#ifndef __CSYNCSTORAGEENGINEAUTHORITYINFO_H__
#define __CSYNCSTORAGEENGINEAUTHORITYINFO_H__
#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include "_CSyncStorageEngineAuthorityInfo.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/etl_pair.h>

CarClass(CSyncStorageEngineAuthorityInfo)
{
public:
    CSyncStorageEngineAuthorityInfo();

    ~CSyncStorageEngineAuthorityInfo();

    CARAPI GetAccount(
        /* [out] */ IAccount** account);

    CARAPI GetAuthority(
        /* [out] */ String* authority);

    CARAPI GetIdent(
        /* [out] */ Int32* ident);

    CARAPI GetEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI GetSyncable(
        /* [out] */ Int32* syncable);

    CARAPI SetSyncable(
        /* [in] */ Int32 syncable);

    CARAPI GetBackoffTime(
        /* [out] */ Int64* backoffTime);

    CARAPI SetBackoffTime(
        /* [in] */ Int64 backoffTime);

    CARAPI GetBackoffDelay(
        /* [out] */ Int64* backoffDelay);

    CARAPI SetBackoffDelay(
        /* [in] */ Int64 backoffDelay);

    CARAPI GetDelayUntil(
        /* [out] */ Int64* delayUntil);

    CARAPI SetDelayUntil(
        /* [in] */ Int64 delayUntil);

    //CARAPI GetPeriodicSyncs(
    //    /* [out] */ IObjectContainer** periodicSyncs);
    List<Pair<AutoPtr<IBundle>, Int64>* >* GetPeriodicSyncs();    

    CARAPI constructor(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [in] */ Int32 ident);

private:
    AutoPtr<IAccount> mAccount;
    String mAuthority;
    Int32 mIdent;
    Boolean mEnabled;
    Int32 mSyncable;
    Int64 mBackoffTime;
    Int64 mBackoffDelay;
    Int64 mDelayUntil;
    List<Pair<AutoPtr<IBundle>, Int64>* >* mPeriodicSyncs;
};

#endif // __CSYNCSTORAGEENGINEAUTHORITYINFO_H__
