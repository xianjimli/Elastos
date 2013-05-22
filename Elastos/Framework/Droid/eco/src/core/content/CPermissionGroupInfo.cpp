
#include "content/CPermissionGroupInfo.h"

ECode CPermissionGroupInfo::LoadLabel(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return CapsuleItemInfo::LoadLabel(pm, label);
}

ECode CPermissionGroupInfo::LoadIcon(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return CapsuleItemInfo::LoadIcon(pm, icon);
}

ECode CPermissionGroupInfo::LoadLogo(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return CapsuleItemInfo::LoadLogo(pm, icon);
}

ECode CPermissionGroupInfo::LoadXmlMetaData(
    /* [in] */ ILocalCapsuleManager* pm,
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
    /* [in] */ ILocalCapsuleManager* cm,
    /* [out] */ ICharSequence** des)
{
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
    CapsuleItemInfo::WriteToParcel(dest);
    dest->WriteInt32(mDescriptionRes);
    //TODO:
    // TextUtils.writeToParcel(nonLocalizedDescription, dest, parcelableFlags);

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
    CPermissionGroupInfo* corig = (CPermissionGroupInfo*) orig;
    CapsuleItemInfo::constructor((CapsuleItemInfo*)(CPermissionGroupInfo*)orig);
    mDescriptionRes = corig->mDescriptionRes;
    mNonLocalizedDescription = corig->mNonLocalizedDescription;

    return NOERROR;
}

ECode CPermissionGroupInfo::constructor(
    /* [in] */ IParcel* source)
{
    //TODO:
    // super(source);

    source->ReadInt32(&mDescriptionRes);
    //TODO:
    // nonLocalizedDescription = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(source);

    return E_NOT_IMPLEMENTED;
}



