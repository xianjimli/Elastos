
#include "content/CPermissionInfo.h"
#include "ext/frameworkdef.h"

CPermissionInfo::CPermissionInfo()
    : mDescriptionRes(0)
    , mProtectionLevel(0)
{
    // E_NOT_IMPLEMENTED
}

CPermissionInfo::~CPermissionInfo()
{
}

ECode CPermissionInfo::LoadDescription(
    /* [in] */ ICapsuleManager* cm,
    /* [out, callee] */ ArrayOf<Char8>** des)
{
    VALIDATE_NOT_NULL(cm);
    VALIDATE_NOT_NULL(des);

    return E_NOT_IMPLEMENTED;
}

ECode CPermissionInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPermissionInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPermissionInfo::GetDescription(
    /* [out] */ String* des)
{
    VALIDATE_NOT_NULL(des);

    return E_NOT_IMPLEMENTED;
}

ECode CPermissionInfo::constructor()
{
    return E_NOT_IMPLEMENTED;
}

ECode CPermissionInfo::constructor(
    /* [in] */ IPermissionInfo* orig)
{
    VALIDATE_NOT_NULL(orig);

    return E_NOT_IMPLEMENTED;
}

ECode CPermissionInfo::constructor(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);

    return E_NOT_IMPLEMENTED;
}

