
#include "content/CResolveInfo.h"

ECode CResolveInfo::GetActivityInfo(
    /* [out] */ IActivityInfo** activityInfo)
{
    if (activityInfo == NULL) return E_INVALID_ARGUMENT;

    *activityInfo = mActivityInfo;
    if (*activityInfo != NULL) (*activityInfo)->AddRef();

    return NOERROR;
}

ECode CResolveInfo::GetServiceInfo(
    /* [out] */ IServiceInfo** serviceInfo)
{
    if (serviceInfo == NULL) return E_INVALID_ARGUMENT;

    *serviceInfo = mServiceInfo;
    if (*serviceInfo != NULL) (*serviceInfo)->AddRef();

    return NOERROR;
}

ECode CResolveInfo::GetIntentFilter(
    /* [out] */ IIntentFilter** intentFilter)
{
    if (intentFilter == NULL) return E_INVALID_ARGUMENT;

    *intentFilter = mFilter;
    (*intentFilter)->AddRef();

    return NOERROR;
}

ECode CResolveInfo::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    return NOERROR;
}

ECode CResolveInfo::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    return NOERROR;
}
