
#include "ext/frameworkext.h"
#include "net/CNetworkInfo.h"
#include "net/CConnectivityManager.h"
#include <elastos/HashMap.h>

static HashMap<NetworkInfoDetailedState, NetworkInfoState> sStateMap;

class Static_
{
public:

    static Int32 Init()
    {
        sStateMap[NetworkInfoDetailedState_IDLE] = NetworkInfoState_DISCONNECTED;
        sStateMap[NetworkInfoDetailedState_SCANNING] = NetworkInfoState_DISCONNECTED;
        sStateMap[NetworkInfoDetailedState_CONNECTING] = NetworkInfoState_CONNECTING;
        sStateMap[NetworkInfoDetailedState_AUTHENTICATING] = NetworkInfoState_CONNECTING;
        sStateMap[NetworkInfoDetailedState_OBTAINING_IPADDR] = NetworkInfoState_CONNECTING;
        sStateMap[NetworkInfoDetailedState_CONNECTED] = NetworkInfoState_CONNECTED;
        sStateMap[NetworkInfoDetailedState_SUSPENDED] = NetworkInfoState_SUSPENDED;
        sStateMap[NetworkInfoDetailedState_DISCONNECTING] = NetworkInfoState_DISCONNECTING;
        sStateMap[NetworkInfoDetailedState_DISCONNECTED] = NetworkInfoState_DISCONNECTED;
        sStateMap[NetworkInfoDetailedState_FAILED] = NetworkInfoState_DISCONNECTED;

        return 0;
    }
};

static Int32 sStatic_ = Static_::Init();

struct EnumInfo {
    Int32 value;
    const char * name;
};

static EnumInfo sNetworkInfoStateInfo[] = {
    {NetworkInfoState_CONNECTING, "CONNECTING"},
    {NetworkInfoState_CONNECTED, "CONNECTED"},
    {NetworkInfoState_SUSPENDED, "SUSPENDED"},
    {NetworkInfoState_DISCONNECTING, "DISCONNECTING"},
    {NetworkInfoState_DISCONNECTED, "DISCONNECTED"},
    {NetworkInfoState_UNKNOWN, "UNKNOWN"}
};

static EnumInfo sNetworkInfoDetailedStateInfo[] = {
    {NetworkInfoDetailedState_IDLE, "IDLE"},
    {NetworkInfoDetailedState_SCANNING, "SCANNING"},
    {NetworkInfoDetailedState_CONNECTING, "CONNECTING"},
    {NetworkInfoDetailedState_AUTHENTICATING, "AUTHENTICATING"},
    {NetworkInfoDetailedState_OBTAINING_IPADDR, "OBTAINING_IPADDR"},
    {NetworkInfoDetailedState_CONNECTED, "CONNECTED"},
    {NetworkInfoDetailedState_SUSPENDED, "SUSPENDED"},
    {NetworkInfoDetailedState_DISCONNECTING, "DISCONNECTING"},
    {NetworkInfoDetailedState_DISCONNECTED, "DISCONNECTED"},
    {NetworkInfoDetailedState_FAILED, "FAILED"}
};

static CString GetNetworkInfoStateName(NetworkInfoState state)
{
    for (UInt32 i = 0; i < sizeof(sNetworkInfoStateInfo) / sizeof(EnumInfo); ++i) {
        if (sNetworkInfoStateInfo[i].value == state) {
            return CString(sNetworkInfoStateInfo[i].name);
        }
    }

    assert(0);
    return CString("");
}

static NetworkInfoState GetNetworkInfoStateByName(String name)
{
    for (UInt32 i = 0;
        i < sizeof(sNetworkInfoStateInfo) / sizeof(EnumInfo);
        ++i) {
        if (name.Equals(sNetworkInfoStateInfo[i].name)) {
            return (NetworkInfoState)sNetworkInfoStateInfo[i].value;
        }
    }

    assert(0);
    return NetworkInfoState_UNKNOWN;
}

static CString GetNetworkInfoDetailedStateName(NetworkInfoDetailedState state)
{
    for (UInt32 i = 0;
        i < sizeof(sNetworkInfoDetailedStateInfo) / sizeof(EnumInfo);
        ++i) {
        if (sNetworkInfoDetailedStateInfo[i].value == state) {
            return CString(sNetworkInfoDetailedStateInfo[i].name);
        }
    }

    assert(0);
    return CString("");
}

static NetworkInfoDetailedState GetNetworkInfoDetailedStateByName(String name)
{
    for (UInt32 i = 0;
        i < sizeof(sNetworkInfoDetailedStateInfo) / sizeof(EnumInfo);
        ++i) {
        if (name.Equals(sNetworkInfoDetailedStateInfo[i].name)) {
            return (NetworkInfoDetailedState)
                sNetworkInfoDetailedStateInfo[i].value;
        }
    }

    assert(0);
    return NetworkInfoDetailedState_FAILED;
}

CNetworkInfo::CNetworkInfo()
    : mNetworkType(0)
    , mSubtype(0)
    , mIsFailover(FALSE)
    , mIsRoaming(FALSE)
{
}

ECode CNetworkInfo::GetType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mNetworkType;

    return NOERROR;
}

ECode CNetworkInfo::GetSubtype(
    /* [out] */ Int32* subtype)
{
    VALIDATE_NOT_NULL(subtype);
    *subtype = mNetworkType;

    return NOERROR;
}

Void CNetworkInfo::SetSubtype(
    /* [in] */ Int32 subtype,
    /* [in] */ const String& subtypeName)
{
    mSubtype = subtype;
    mSubtypeName = subtypeName;
}

ECode CNetworkInfo::GetTypeName(
    /* [out] */ String* typeName)
{
    VALIDATE_NOT_NULL(typeName);
    *typeName = mTypeName;

    return NOERROR;
}

ECode CNetworkInfo::GetSubtypeName(
    /* [out] */ String* subtypeName)
{
    VALIDATE_NOT_NULL(subtypeName);
    *subtypeName = mSubtypeName;

    return NOERROR;
}

ECode CNetworkInfo::IsConnectedOrConnecting(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ((mState == NetworkInfoState_CONNECTED)
        || (mState == NetworkInfoState_CONNECTING));

    return NOERROR;
}

ECode CNetworkInfo::IsConnected(
    /* [out] */ Boolean* connected)
{
    VALIDATE_NOT_NULL(connected);
    *connected = (mState == NetworkInfoState_CONNECTED);

    return NOERROR;
}

ECode CNetworkInfo::IsAvailable(
    /* [out] */ Boolean* available)
{
    VALIDATE_NOT_NULL(available);
    *available = mIsAvailable;

    return NOERROR;
}

ECode CNetworkInfo::SetIsAvailable(
    /* [in] */ Boolean isAvailable)
{
    mIsAvailable = isAvailable;

    return NOERROR;
}

ECode CNetworkInfo::IsFailover(
    /* [out] */ Boolean* failover)
{
    VALIDATE_NOT_NULL(failover);
    *failover = mIsFailover;

    return NOERROR;
}

ECode CNetworkInfo::SetFailover(
    /* [in] */ Boolean isFailover)
{
    mIsFailover = isFailover;

    return NOERROR;
}

ECode CNetworkInfo::IsRoaming(
    /* [out] */ Boolean* roaming)
{
    VALIDATE_NOT_NULL(roaming);
    *roaming = mIsRoaming;

    return NOERROR;
}

Void CNetworkInfo::SetRoaming(
    /* [in] */ Boolean isRoaming)
{
    mIsRoaming = isRoaming;
}

ECode CNetworkInfo::GetState(
    /* [out] */ NetworkInfoState* state)
{
    VALIDATE_NOT_NULL(state);
    *state = mState;

    return NOERROR;
}

ECode CNetworkInfo::GetDetailedState(
    /* [out] */ NetworkInfoDetailedState* state)
{
    VALIDATE_NOT_NULL(state);
    *state = mDetailedState;

    return NOERROR;
}

ECode CNetworkInfo::GetReason(
    /* [out] */ String* reason)
{
    VALIDATE_NOT_NULL(reason);
    *reason = mReason;

    return NOERROR;
}

ECode CNetworkInfo::GetExtraInfo(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = mExtraInfo;

    return NOERROR;
}

ECode CNetworkInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mNetworkType);
    dest->WriteInt32(mSubtype);
    dest->WriteString(mTypeName);
    dest->WriteString(mSubtypeName);
    dest->WriteCString(GetNetworkInfoStateName(mState));
    dest->WriteCString(GetNetworkInfoDetailedStateName(mDetailedState));
    dest->WriteInt32(mIsFailover? 1: 0);
    dest->WriteInt32(mIsAvailable? 1: 0);
    dest->WriteInt32(mIsRoaming? 1: 0);
    dest->WriteString(mReason);
    dest->WriteString(mExtraInfo);

    return NOERROR;
}

ECode CNetworkInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    Int32 netType;
    Int32 subtype;
    String typeName;
    String subtypeName;

    FAIL_RETURN(source->ReadInt32(&netType));
    FAIL_RETURN(source->ReadInt32(&subtype));
    FAIL_RETURN(source->ReadString(&typeName));
    FAIL_RETURN(source->ReadString(&subtypeName));
    FAIL_RETURN(constructor(netType, subtype, typeName, subtypeName));

    String state;
    FAIL_RETURN(source->ReadString(&state));
    mState = GetNetworkInfoStateByName(state);

    String detailedState;
    FAIL_RETURN(source->ReadString(&detailedState));
    mDetailedState = GetNetworkInfoDetailedStateByName(detailedState);

    Int32 temp;

    FAIL_RETURN(source->ReadInt32(&temp));
    mIsFailover = (temp != 0);
    FAIL_RETURN(source->ReadInt32(&temp));
    mIsAvailable = (temp != 0);
    FAIL_RETURN(source->ReadInt32(&temp));
    mIsRoaming = (temp != 0);

    FAIL_RETURN(source->ReadString(&mReason));
    FAIL_RETURN(source->ReadString(&mExtraInfo));

    return NOERROR;
}

ECode CNetworkInfo::constructor(
    /* [in] */ Int32 type)
{
    return NOERROR;
}

ECode CNetworkInfo::constructor(
    /* [in] */ Int32 type,
    /* [in] */ Int32 subtype,
    /* [in] */ const String& typeName,
    /* [in] */ const String& subtypeName)
{
    if (!CConnectivityManager::IsNetworkTypeValid(type)) {
        // throw new IllegalArgumentException("Invalid network type: " + type);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mNetworkType = type;
    mSubtype = subtype;
    mTypeName = typeName;
    mSubtypeName = subtypeName;
    SetDetailedState(NetworkInfoDetailedState_IDLE, String(NULL), String(NULL));
    mState = NetworkInfoState_UNKNOWN;
    mIsAvailable = FALSE; // until we're told otherwise, assume unavailable
    mIsRoaming = FALSE;

    return NOERROR;
}

Void CNetworkInfo::SetDetailedState(
    /* [in] */ NetworkInfoDetailedState detailedState,
    /* [in] */ const String& reason,
    /* [in] */ const String& extraInfo)
{
    mDetailedState = detailedState;

    HashMap<NetworkInfoDetailedState, NetworkInfoState>::Iterator it =
        sStateMap.Find(detailedState);
    assert(it != sStateMap.End());
    mState = it->mSecond;
    mReason = reason;
    mExtraInfo = extraInfo;
}

