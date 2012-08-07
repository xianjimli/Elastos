
#include "capsule/CServiceInfo.h"

CServiceInfo::CServiceInfo()
{}

CServiceInfo::~CServiceInfo()
{}

ECode CServiceInfo::constructor()
{
    return NOERROR;
}

ECode CServiceInfo::constructor(
    /* [in] */ IServiceInfo* orig)
{
    AutoPtr<CServiceInfo> corig = (CServiceInfo*)orig;
    ComponentInfo::constructor((ComponentInfo*)(CServiceInfo*)corig);
    mPermission = corig->mPermission;
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
    /* [in] */ const String& name,
    /* [out] */ IXmlResourceParser** resource)
{
    VALIDATE_NOT_NULL(resource);
    return ComponentInfo::LoadXmlMetaData(pm, name, resource);
}

ECode CServiceInfo::GetCapsuleName(
    /* [out] */ String* capsuleName)
{
    VALIDATE_NOT_NULL(capsuleName);
    *capsuleName = ComponentInfo::GetCapsuleName();
    return NOERROR;
}

ECode CServiceInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = ComponentInfo::GetName();
    return NOERROR;
}

ECode CServiceInfo::GetComponentName(
    /* [out] */ String * pCapsuleName,
    /* [out] */ String * pClassName)
{
    *pCapsuleName = mCapsuleName;
    *pClassName = mName;

    return NOERROR;
}

ECode CServiceInfo::SetComponentName(
    /* [in] */ const String& capsuleName,
    /* [in] */ const String& className)
{
    mCapsuleName = capsuleName;
    mName = className;

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

ECode CServiceInfo::IsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = mEnabled;
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
