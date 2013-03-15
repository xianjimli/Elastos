
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
    if (mNonLocalizedLabel != NULL) {
        *label = mNonLocalizedLabel;
        (*label)->AddRef();
        return NOERROR;
    }
    if (mLabelRes != 0) {
//        CharSequence label = pm.getText(packageName, labelRes, getApplicationInfo());
//        if (label != null) {
//            return label.toString().trim();
//        }
        assert(0);
    }
    if (mName.IsNull()) {
        return CStringWrapper::New(mName, label);
    }
    return CStringWrapper::New(mCapsuleName, label);
}

ECode CapsuleItemInfo::LoadIcon(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
//    if (icon != 0) {
//        Drawable dr = pm.getDrawable(packageName, icon, getApplicationInfo());
//        if (dr != null) {
//            return dr;
//        }
//    }
//    return loadDefaultIcon(pm);
    return E_NOT_IMPLEMENTED;
}

ECode CapsuleItemInfo::LoadLogo(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
//    if (logo != 0) {
//        Drawable d = pm.getDrawable(packageName, logo, getApplicationInfo());
//        if (d != null) {
//            return d;
//        }
//    }
//    return loadDefaultLogo(pm);
    return E_NOT_IMPLEMENTED;
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
