
#include "capsule/ComponentInfo.h"

ComponentInfo::ComponentInfo() :
    mEnabled(TRUE),
    mExported(FALSE)
{}

ComponentInfo::~ComponentInfo()
{}

ECode ComponentInfo::constructor(
    /* [in] */ ComponentInfo* orig)
{
    CapsuleItemInfo::constructor(orig);
    mApplicationInfo = orig->mApplicationInfo;
    mProcessName = orig->mProcessName;
    mDescriptionRes = orig->mDescriptionRes;
    mEnabled = orig->mEnabled;
    mExported = orig->mExported;
    return NOERROR;
}

ECode ComponentInfo::LoadLabel(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ ICharSequence** label)
{
    if (mNonLocalizedLabel != NULL) {
        *label = mNonLocalizedLabel;
        (*label)->AddRef();
        return NOERROR;
    }
    CApplicationInfo* ai = mApplicationInfo;
    if (mLabelRes != 0) {
//        label = pm.getText(packageName, labelRes, ai);
//        if (label != null) {
//            return label;
//        }
        assert(0);
    }
    if (ai->mNonLocalizedLabel != NULL) {
        *label = ai->mNonLocalizedLabel;
        (*label)->AddRef();
        return NOERROR;
    }
    if (ai->mLabelRes != 0) {
//        label = pm.getText(packageName, ai.labelRes, ai);
//        if (label != null) {
//            return label;
//        }
        assert(0);
    }
    return CStringWrapper::New(mName, label);
}

ECode ComponentInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    AutoPtr<IInterface> obj;
    CapsuleItemInfo::ReadFromParcel(source);
    source->ReadInterfacePtr((Handle32*)&obj);
    mApplicationInfo = obj != NULL ? (CApplicationInfo*)
            (IApplicationInfo*)obj->Probe(EIID_IApplicationInfo) : NULL;
    source->ReadString(&mProcessName);
    return NOERROR;
}

ECode ComponentInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    CapsuleItemInfo::WriteToParcel(dest);
    dest->WriteInterfacePtr(
            (IApplicationInfo*)(CApplicationInfo*)mApplicationInfo);
    dest->WriteString(mProcessName);
    return NOERROR;
}

AutoPtr<IApplicationInfo> ComponentInfo::GetApplicationInfo()
{
    return (IApplicationInfo*)mApplicationInfo.Get();
}
