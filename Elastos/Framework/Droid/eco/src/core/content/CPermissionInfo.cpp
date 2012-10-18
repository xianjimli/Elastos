
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

ECode CPermissionInfo::LoadLabel(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return CapsuleItemInfo::LoadLabel(pm, label);
}

ECode CPermissionInfo::LoadIcon(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return CapsuleItemInfo::LoadIcon(pm, icon);
}

ECode CPermissionInfo::LoadLogo(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return CapsuleItemInfo::LoadLogo(pm, icon);
}

ECode CPermissionInfo::LoadXmlMetaData(
    /* [in] */ ICapsuleManager* pm,
    /* [in] */ const String& name,
    /* [out] */ IXmlResourceParser** resource)
{
    VALIDATE_NOT_NULL(resource);
    return CapsuleItemInfo::LoadXmlMetaData(pm, name, resource);
}

ECode CPermissionInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode CPermissionInfo::SetName(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

ECode CPermissionInfo::GetCapsuleName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mCapsuleName;
    return NOERROR;
}

ECode CPermissionInfo::SetCapsuleName(
    /* [in] */ const String& name)
{
    mCapsuleName = name;
    return NOERROR;
}

ECode CPermissionInfo::GetLabelRes(
    /* [out] */ Int32* labelRes)
{
    VALIDATE_NOT_NULL(labelRes);
    *labelRes = mLabelRes;
    return NOERROR;
}

ECode CPermissionInfo::SetLabelRes(
    /* [in] */ Int32 labelRes)
{
    mLabelRes = labelRes;
    return NOERROR;
}

ECode CPermissionInfo::GetNonLocalizedLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    *label = mNonLocalizedLabel;
    if (*label != NULL) (*label)->AddRef();
    return NOERROR;
}

ECode CPermissionInfo::SetNonLocalizedLabel(
    /* [in] */ ICharSequence* label)
{
    mNonLocalizedLabel = label;
    return NOERROR;
}

ECode CPermissionInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    *icon = mIcon;
    return NOERROR;
}

ECode CPermissionInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    mIcon = icon;
    return NOERROR;
}

ECode CPermissionInfo::GetLogo(
    /* [out] */ Int32* logo)
{
    VALIDATE_NOT_NULL(logo);
    *logo = mLogo;
    return NOERROR;
}

ECode CPermissionInfo::SetLogo(
    /* [in] */ Int32 logo)
{
    mLogo = logo;
    return NOERROR;
}

ECode CPermissionInfo::GetMetaData(
    /* [out] */ IBundle** metaData)
{
    VALIDATE_NOT_NULL(metaData);
    *metaData = mMetaData;
    if (*metaData != NULL) (*metaData)->AddRef();
    return NOERROR;
}

ECode CPermissionInfo::SetMetaData(
    /* [in] */ IBundle* metaData)
{
    mMetaData = metaData;
    return NOERROR;
}

ECode CPermissionInfo::LoadDescription(
    /* [in] */ ICapsuleManager* cm,
    /* [out, callee] */ ArrayOf<Char8>** des)
{
    VALIDATE_NOT_NULL(cm);
    VALIDATE_NOT_NULL(des);

    return E_NOT_IMPLEMENTED;
}

ECode CPermissionInfo::GetGroup(
    /* [out] */ String* group)
{
    VALIDATE_NOT_NULL(group);
    *group = mGroup;
    return NOERROR;
}

ECode CPermissionInfo::SetGroup(
    /* [in] */ const String& group)
{
    mGroup = group;
    return NOERROR;
}

ECode CPermissionInfo::GetDescriptionRes(
    /* [out] */ Int32* desRes)
{
    VALIDATE_NOT_NULL(desRes);
    *desRes = mDescriptionRes;
    return NOERROR;
}

ECode CPermissionInfo::SetDescriptionRes(
    /* [in] */ Int32 desRes)
{
    mDescriptionRes = desRes;
    return NOERROR;
}

ECode CPermissionInfo::GetNonLocalizedDescription(
    /* [out] */ ICharSequence** des)
{
    VALIDATE_NOT_NULL(des);
    *des = mNonLocalizedDescription;
    if (*des != NULL) (*des)->AddRef();
    return NOERROR;
}

ECode CPermissionInfo::SetNonLocalizedDescription(
    /* [in] */ ICharSequence* des)
{
    mNonLocalizedDescription = des;
    return NOERROR;
}

ECode CPermissionInfo::GetProtectionLevel(
    /* [out] */ Int32* level)
{
    VALIDATE_NOT_NULL(level);
    *level = mProtectionLevel;
    return NOERROR;
}

ECode CPermissionInfo::SetProtectionLevel(
    /* [in] */ Int32 level)
{
    mProtectionLevel = level;
    return NOERROR;
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
