
#include "capsule/CServiceInfo.h"

CServiceInfo::CServiceInfo()
{}

CServiceInfo::~CServiceInfo()
{
    String::Free(mPermission);
}

ECode CServiceInfo::constructor()
{
    return NOERROR;
}

ECode CServiceInfo::constructor(
    /* [in] */ IServiceInfo* orig)
{
    AutoPtr<CServiceInfo> corig = (CServiceInfo*)orig;
    ComponentInfo::constructor((ComponentInfo*)(CServiceInfo*)corig);
    mPermission = String::Duplicate(corig->mPermission);
    return NOERROR;
}

ECode CServiceInfo::LoadLabel(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return ComponentInfo::LoadLabel(pm, label);
}

ECode CServiceInfo::LoadIcon(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::LoadIcon(pm, icon);
}

ECode CServiceInfo::LoadLogo(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::LoadLogo(pm, icon);
}

ECode CServiceInfo::LoadXmlMetaData(
    /* [in] */ ICapsuleManager* pm,
    /* [in] */ String name,
    /* [out] */ IXmlResourceParser** resource)
{
    VALIDATE_NOT_NULL(resource);
    return ComponentInfo::LoadXmlMetaData(pm, name, resource);
}

ECode CServiceInfo::GetComponentName(
    /* [out] */ String * pCapsuleName,
    /* [out] */ String * pClassName)
{
    *pCapsuleName = String::Duplicate(mCapsuleName);
    *pClassName = String::Duplicate(mName);

    return NOERROR;
}

ECode CServiceInfo::SetComponentName(
    /* [in] */ String capsuleName,
    /* [in] */ String className)
{
    mCapsuleName = String::Duplicate(capsuleName);
    mName = String::Duplicate(className);

    return NOERROR;
}

ECode CServiceInfo::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    if (info == NULL) return E_INVALID_ARGUMENT;

    *info = (IApplicationInfo*)(CApplicationInfo*)mApplicationInfo;
    if (*info != NULL) (*info)->AddRef();

    return NOERROR;
}

ECode CServiceInfo::SetApplicationInfo(
    /* [in] */ IApplicationInfo* info)
{
    mApplicationInfo = (CApplicationInfo*)info;

    return NOERROR;
}

ECode CServiceInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    ComponentInfo::ReadFromParcel(source);
    source->ReadString(&mPermission);
    return NOERROR;
}

ECode CServiceInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    ComponentInfo::WriteToParcel(dest);
    dest->WriteString(mPermission);
    return NOERROR;
}
