
#ifndef __CNETWORKINFO_H__
#define __CNETWORKINFO_H__

#include "_CNetworkInfo.h"

CarClass(CNetworkInfo)
{
public:
    /**
     * Reports the type of network (currently mobile or Wi-Fi) to which the
     * info in this object pertains.
     * @return the network type
     */
    CARAPI GetType(
        /* [out] */ Int32* type);

    CARAPI GetSubtype(
        /* [out] */ Int32* subtype);

    CARAPI GetTypeName(
        /* [out] */ String* typeName);

    CARAPI GetSubtypeName(
        /* [out] */ String* subtypeName);

    CARAPI IsConnectedOrConnecting(
        /* [out] */ Boolean* result);

    CARAPI IsConnected(
        /* [out] */ Boolean* connected);

    CARAPI IsAvailable(
        /* [out] */ Boolean* available);

    CARAPI SetIsAvailable(
        /* [in] */ Boolean isAvailable);

    CARAPI IsFailover(
        /* [out] */ Boolean* failover);

    CARAPI SetFailover(
        /* [in] */ Boolean isFailover);

    CARAPI IsRoaming(
        /* [out] */ Boolean* roaming);

    CARAPI GetState(
        /* [out] */ NetworkInfoState* state);

    CARAPI GetDetailedState(
        /* [out] */ NetworkInfoDetailedState* state);

    CARAPI GetReason(
        /* [out] */ String* reason);

    CARAPI GetExtraInfo(
        /* [out] */ String* info);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    /**
     * @param type network type
     * @deprecated
     * @hide because this constructor was only meant for internal use (and
     * has now been superseded by the package-private constructor below).
     */
    CARAPI constructor(
        /* [in] */ Int32 type);

    CARAPI constructor(
        /* [in] */ Int32 type,
        /* [in] */ Int32 subtype,
        /* [in] */ const String& typeName,
        /* [in] */ const String& subtypeName);

public:
    CNetworkInfo();

    /* pacakge */ CARAPI_(Void) SetSubtype(
        /* [in] */ Int32 subtype,
        /* [in] */ const String& subtypeName);

    /* package */ CARAPI_(Void) SetRoaming(
        /* [in] */ Boolean isRoaming);

    /* package */ CARAPI_(Void) SetDetailedState(
        /* [in] */ NetworkInfoDetailedState detailedState,
        /* [in] */ const String& reason,
        /* [in] */ const String& extraInfo);

private:
    Int32 mNetworkType;
    Int32 mSubtype;
    String mTypeName;
    String mSubtypeName;
    NetworkInfoState mState;
    NetworkInfoDetailedState mDetailedState;
    String mReason;
    String mExtraInfo;
    Boolean mIsFailover;
    Boolean mIsRoaming;
    /**
     * Indicates whether network connectivity is possible:
     */
    Boolean mIsAvailable;
};

#endif // __CNETWORKINFO_H__
