
#include "content/CPermissionGroupInfo.h"

ECode CPermissionGroupInfo::LoadLabel(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return CapsuleItemInfo::LoadLabel(pm, label);
}

ECode CPermissionGroupInfo::LoadIcon(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return CapsuleItemInfo::LoadIcon(pm, icon);
}

ECode CPermissionGroupInfo::LoadLogo(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return CapsuleItemInfo::LoadLogo(pm, icon);
}

ECode CPermissionGroupInfo::LoadXmlMetaData(
    /* [in] */ ICapsuleManager* pm,
    /* [in] */ const String& name,
    /* [out] */ IXmlResourceParser** resource)
{
    VALIDATE_NOT_NULL(resource);
    return CapsuleItemInfo::LoadXmlMetaData(pm, name, resource);
}

ECode CPermissionGroupInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode CPermissionGroupInfo::SetName(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

ECode CPermissionGroupInfo::GetCapsuleName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mCapsuleName;
    return NOERROR;
}

ECode CPermissionGroupInfo::SetCapsuleName(
    /* [in] */ const String& name)
{
    mCapsuleName = name;
    return NOERROR;
}

ECode CPermissionGroupInfo::GetLabelRes(
    /* [out] */ Int32* labelRes)
{
    VALIDATE_NOT_NULL(labelRes);
    *labelRes = mLabelRes;
    return NOERROR;
}

ECode CPermissionGroupInfo::SetLabelRes(
    /* [in] */ Int32 labelRes)
{
    mLabelRes = labelRes;
    return NOERROR;
}

ECode CPermissionGroupInfo::GetNonLocalizedLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    *label = mNonLocalizedLabel;
    if (*label != NULL) (*label)->AddRef();
    return NOERROR;
}

ECode CPermissionGroupInfo::SetNonLocalizedLabel(
    /* [in] */ ICharSequence* label)
{
    mNonLocalizedLabel = label;
    return NOERROR;
}

ECode CPermissionGroupInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    *icon = mIcon;
    return NOERROR;
}

ECode CPermissionGroupInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    mIcon = icon;
    return NOERROR;
}

ECode CPermissionGroupInfo::GetLogo(
    /* [out] */ Int32* logo)
{
    VALIDATE_NOT_NULL(logo);
    *logo = mLogo;
    return NOERROR;
}

ECode CPermissionGroupInfo::SetLogo(
    /* [in] */ Int32 logo)
{
    mLogo = logo;
    return NOERROR;
}

ECode CPermissionGroupInfo::GetMetaData(
    /* [out] */ IBundle** metaData)
{
    VALIDATE_NOT_NULL(metaData);
    *metaData = mMetaData;
    if (*metaData != NULL) (*metaData)->AddRef();
    return NOERROR;
}

ECode CPermissionGroupInfo::SetMetaData(
    /* [in] */ IBundle* metaData)
{
    mMetaData = metaData;
    return NOERROR;
}

ECode CPermissionGroupInfo::LoadDescription(
    /* [in] */ ICapsuleManager* cm,
    /* [out] */ ICharSequence** des)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPermissionGroupInfo::GetDescription(
    /* [out] */ String* des)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPermissionGroupInfo::GetDescriptionRes(
    /* [out] */ Int32* desRes)
{
    VALIDATE_NOT_NULL(desRes);
    *desRes = mDescriptionRes;
    return NOERROR;
}

ECode CPermissionGroupInfo::SetDescriptionRes(
    /* [in] */ Int32 desRes)
{
    mDescriptionRes = desRes;
    return NOERROR;
}

ECode CPermissionGroupInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPermissionGroupInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPermissionGroupInfo::constructor()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPermissionGroupInfo::constructor(
    /* [in] */ IPermissionGroupInfo* orig)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPermissionGroupInfo::constructor(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}


