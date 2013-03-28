
#include "content/CapsuleItemInfo.h"

CapsuleItemInfo::CapsuleItemInfo()
    : mLabelRes(0)
    , mIcon(0)
    , mLogo(0)
{}

CapsuleItemInfo::~CapsuleItemInfo()
{}

ECode CapsuleItemInfo::constructor(
    /* [in] */ CapsuleItemInfo* orig)
{
    mName = orig->mName;
//    if (name != null) name = name.trim();
    mCapsuleName = orig->mCapsuleName;
    return NOERROR;
}

ECode CapsuleItemInfo::LoadLabel(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);

    if (mNonLocalizedLabel != NULL) {
        *label = mNonLocalizedLabel;
        (*label)->AddRef();
        return NOERROR;
    }
    if (mLabelRes != 0) {
       pm->GetText(mCapsuleName, mLabelRes, GetApplicationInfo(), label);
       if (*label != NULL) {
            String str;
            (*label)->ToString(&str);
            return CStringWrapper::New(str.Trim(), label);
       }
    }
    if (mName.IsNull()) {
        return CStringWrapper::New(mName, label);
    }
    return CStringWrapper::New(mCapsuleName, label);
}

ECode CapsuleItemInfo::LoadDefaultIcon(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);

    return pm->GetDefaultActivityIcon(icon);
}


ECode CapsuleItemInfo::LoadIcon(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);

    if (mIcon != 0) {
       pm->GetDrawable(mCapsuleName, mIcon, GetApplicationInfo(), icon);
       if (*icon != NULL) {
            return NOERROR;
       }
    }

   return LoadDefaultIcon(pm, icon);
}

ECode CapsuleItemInfo::LoadDefaultLogo(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);

    *icon = NULL;
    return NOERROR;
}

ECode CapsuleItemInfo::LoadLogo(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    
    if (mLogo != 0) {
       pm->GetDrawable(mCapsuleName, mLogo, GetApplicationInfo(), icon);
       if (*icon != NULL) {
           return NOERROR;
       }
    }
   return LoadDefaultLogo(pm, icon);
}

ECode CapsuleItemInfo::LoadXmlMetaData(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [in] */ const String& name,
    /* [out] */ IXmlResourceParser** resource)
{
    assert(resource != NULL);
    if (mMetaData != NULL) {
        Int32 resid = 0;
        mMetaData->GetInt32(name, &resid);
        if (resid != 0) {
            return pm->GetXml(mCapsuleName, resid, GetApplicationInfo(), resource);
        }
    }

    *resource = NULL;
    return NOERROR;
}

String CapsuleItemInfo::GetCapsuleName()
{
    return mCapsuleName;
}

String CapsuleItemInfo::GetName()
{
    return mName;
}

ECode CapsuleItemInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadString(&mName);
    source->ReadString(&mCapsuleName);
    return NOERROR;
}

ECode CapsuleItemInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteString(mName);
    dest->WriteString(mCapsuleName);
    return NOERROR;
}

AutoPtr<IApplicationInfo> CapsuleItemInfo::GetApplicationInfo()
{
    return NULL;
}
