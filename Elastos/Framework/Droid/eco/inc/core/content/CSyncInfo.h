
#ifndef __CSYNCINFO_H__
#define __CSYNCINFO_H__

#include "_CSyncInfo.h"
#include <ext/frameworkdef.h>
#include <elastos/AutoPtr.h>

CarClass(CSyncInfo)
{
public:
    CARAPI DescribeContents(
        /* [out] */ Int32 * result);

    CARAPI GetAuthorityId(
        /* [out] */ Int32* authorityId);

    CARAPI GetAccount(
        /* [out] */ IAccount** account);

    CARAPI GetAuthority(
        /* [out] */ String* authority);

    CARAPI GetStartTime(
        /* [out] */ Int64* startTime);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ IParcel* parcel);

    CARAPI constructor(
        /* [in] */ Int32 authorityId,
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [in] */ Int64 startTime);

public:
    /** @hide */
    /*final*/ Int32 authorityId;

    /**
     * The {@link Account} that is currently being synced.
     */
    /*final*/ AutoPtr<IAccount> account;

    /**
     * The authority of the provider that is currently being synced.
     */
    /*final*/ String authority;

    /**
     * The start time of the current sync operation in milliseconds since boot.
     * This is represented in elapsed real time.
     * See {@link android.os.SystemClock#elapsedRealtime()}.
     */
    /*final*/ Int64 startTime;

private:
    // TODO: Add your private member variables here.
};

#endif // __CSYNCINFO_H__
