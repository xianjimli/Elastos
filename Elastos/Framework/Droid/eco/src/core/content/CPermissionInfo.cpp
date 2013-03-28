
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
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return CapsuleItemInfo::LoadLabel(pm, label);
}

ECode CPermissionInfo::LoadIcon(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return CapsuleItemInfo::LoadIcon(pm, icon);
}

ECode CPermissionInfo::LoadLogo(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return CapsuleItemInfo::LoadLogo(pm, icon);
}

ECode CPermissionInfo::LoadXmlMetaData(
    /* [in] */ ILocalCapsuleManager* pm,
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
    /* [in] */ ILocalCapsuleManager* cm,
    /* [out] */ ICharSequence** des)
{
    VALIDATE_NOT_NULL(cm);
    VALIDATE_NOT_NULL(des);

    if (mNonLocalizedDescription != NULL) {
        *des = mNonLocalizedDescription;
        return NOERROR;
    }

    if (mDescriptionRes != 0) {
        ICharSequence* label;
        cm->GetText(mCapsuleName, mDescriptionRes, NULL, &label);
        if (label != NULL) {
            *des = label;
            return NOERROR;
        }
    }

    return NOERROR;
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
    CapsuleItemInfo::WriteToParcel(dest);
    dest->WriteString(mGroup);
    dest->WriteInt32(mDescriptionRes);
    dest->WriteInt32(mProtectionLevel);
    //TODO:
    //TextUtils::WriteToParcel(nonLocalizedDescription, dest, parcelableFlags);
    return NOERROR;
}

// ECode CPermissionInfo::GetDescription(
//     /* [out] */ String* des)
// {
//     VALIDATE_NOT_NULL(des);

//     return E_NOT_IMPLEMENTED;
// }

ECode CPermissionInfo::constructor()
{
    return NOERROR;
}

ECode CPermissionInfo::constructor(
    /* [in] */ IPermissionInfo* orig)
{
    VALIDATE_NOT_NULL(orig);

    CPermissionInfo* corig = (CPermissionInfo*) orig;
    CapsuleItemInfo::constructor((CapsuleItemInfo*)(CPermissionInfo*)orig);
    mGroup = corig->mGroup;
    mDescriptionRes = corig->mDescriptionRes;
    mProtectionLevel = corig->mProtectionLevel;
    mNonLocalizedDescription = corig->mNonLocalizedDescription;

    return NOERROR;
}

ECode CPermissionInfo::constructor(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);

    //TODO:
    //super(source);
    //CapsuleItemInfo::constructor((CapsuleItemInfo*)(CPermissionInfo*)source);

    source->ReadString(&mGroup);
    source->ReadInt32(&mDescriptionRes);
    source->ReadInt32(&mProtectionLevel);
    //TODO:
    // nonLocalizedDescription = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(source);
    return E_NOT_IMPLEMENTED;
}

