
#ifndef __CPERIODICSYNC_H__
#define __CPERIODICSYNC_H__

#include "_CPeriodicSync.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

CarClass(CPeriodicSync)
{
public:
    CPeriodicSync();

    ~CPeriodicSync();

    CARAPI GetAccount(
        /* [out] */ IAccount** account);

    CARAPI GetAuthority(
        /* [out] */ String* authority);

    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    CARAPI GetPeriod(
        /* [out] */ Int64* period);

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    /** Creates a new PeriodicSync, copying the Bundle */
    CARAPI constructor(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [in] */ IBundle* extras,
        /* [in] */ Int64 period);

private:
    /** The account to be synced */
    AutoPtr<IAccount> mAccount;

    /** The authority of the sync */
    String mAuthority;

    /** Any extras that parameters that are to be passed to the sync adapter. */
    AutoPtr<IBundle> mExtras;
    
    /** How frequently the sync should be scheduled, in seconds. */
    Int64 mPeriod;
};

#endif // __CPERIODICSYNC_H__
