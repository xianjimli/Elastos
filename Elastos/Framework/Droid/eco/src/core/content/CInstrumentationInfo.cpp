
#include "content/CInstrumentationInfo.h"

ECode CInstrumentationInfo::constructor()
{
    return E_NOT_IMPLEMENTED;
}

ECode CInstrumentationInfo::constructor(
    /* [in] */ IInstrumentationInfo* orig)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInstrumentationInfo::LoadLabel(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return CapsuleItemInfo::LoadLabel(pm, label);
}

ECode CInstrumentationInfo::LoadIcon(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return CapsuleItemInfo::LoadIcon(pm, icon);
}

ECode CInstrumentationInfo::LoadLogo(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return CapsuleItemInfo::LoadLogo(pm, icon);
}

ECode CInstrumentationInfo::LoadXmlMetaData(
    /* [in] */ ICapsuleManager* pm,
    /* [in] */ const String& name,
    /* [out] */ IXmlResourceParser** resource)
{
    VALIDATE_NOT_NULL(resource);
    return CapsuleItemInfo::LoadXmlMetaData(pm, name, resource);
}

ECode CInstrumentationInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode CInstrumentationInfo::SetName(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

ECode CInstrumentationInfo::GetCapsuleName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mCapsuleName;
    return NOERROR;
}

ECode CInstrumentationInfo::SetCapsuleName(
    /* [in] */ const String& name)
{
    mCapsuleName = name;
    return NOERROR;
}

ECode CInstrumentationInfo::GetLabelRes(
    /* [out] */ Int32* labelRes)
{
    VALIDATE_NOT_NULL(labelRes);
    *labelRes = mLabelRes;
    return NOERROR;
}

ECode CInstrumentationInfo::SetLabelRes(
    /* [in] */ Int32 labelRes)
{
    mLabelRes = labelRes;
    return NOERROR;
}

ECode CInstrumentationInfo::GetNonLocalizedLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    *label = mNonLocalizedLabel;
    if (*label != NULL) (*label)->AddRef();
    return NOERROR;
}

ECode CInstrumentationInfo::SetNonLocalizedLabel(
    /* [in] */ ICharSequence* label)
{
    mNonLocalizedLabel = label;
    return NOERROR;
}

ECode CInstrumentationInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    *icon = mIcon;
    return NOERROR;
}

ECode CInstrumentationInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    mIcon = icon;
    return NOERROR;
}

ECode CInstrumentationInfo::GetLogo(
    /* [out] */ Int32* logo)
{
    VALIDATE_NOT_NULL(logo);
    *logo = mLogo;
    return NOERROR;
}

ECode CInstrumentationInfo::SetLogo(
    /* [in] */ Int32 logo)
{
    mLogo = logo;
    return NOERROR;
}

ECode CInstrumentationInfo::GetMetaData(
    /* [out] */ IBundle** metaData)
{
    VALIDATE_NOT_NULL(metaData);
    *metaData = mMetaData;
    if (*metaData != NULL) (*metaData)->AddRef();
    return NOERROR;
}

ECode CInstrumentationInfo::SetMetaData(
    /* [in] */ IBundle* metaData)
{
    mMetaData = metaData;
    return NOERROR;
}

ECode CInstrumentationInfo::GetTargetCapsule(
    /* [out] */ String* capsule)
{
    VALIDATE_NOT_NULL(capsule);
    *capsule = mTargetCapsule;
    return NOERROR;
}

ECode CInstrumentationInfo::SetTargetCapsule(
    /* [in] */ const String& capsule)
{
    mTargetCapsule = capsule;
    return NOERROR;
}

ECode CInstrumentationInfo::GetSourceDir(
    /* [out] */ String* sourceDir)
{
    VALIDATE_NOT_NULL(sourceDir);
    *sourceDir = mSourceDir;
    return NOERROR;
}

ECode CInstrumentationInfo::SetSourceDir(
    /* [in] */ const String& sourceDir)
{
    mSourceDir = sourceDir;
    return NOERROR;
}

ECode CInstrumentationInfo::GetPublicSourceDir(
    /* [out] */ String* pubSrcDir)
{
    VALIDATE_NOT_NULL(pubSrcDir);
    *pubSrcDir = mPublicSourceDir;
    return NOERROR;
}

ECode CInstrumentationInfo::SetPublicSourceDir(
    /* [in] */ const String& pubSrcDir)
{
    mPublicSourceDir = pubSrcDir;
    return NOERROR;
}

ECode CInstrumentationInfo::GetDataDir(
    /* [out] */ String* dataDir)
{
    VALIDATE_NOT_NULL(dataDir);
    *dataDir = mDataDir;
    return NOERROR;
}

ECode CInstrumentationInfo::SetDataDir(
    /* [in] */ const String& dataDir)
{
    mDataDir = dataDir;
    return NOERROR;
}

ECode CInstrumentationInfo::GetNativeLibraryDir(
    /* [out] */ String* libraryDir)
{
    VALIDATE_NOT_NULL(libraryDir);
    *libraryDir = mNativeLibraryDir;
    return NOERROR;
}

ECode CInstrumentationInfo::SetNativeLibraryDir(
    /* [in] */ const String& libraryDir)
{
    mNativeLibraryDir = libraryDir;
    return NOERROR;
}

ECode CInstrumentationInfo::IsHandleProfiling(
    /* [out] */ Boolean* handleProfiling)
{
    VALIDATE_NOT_NULL(handleProfiling);
    *handleProfiling = mHandleProfiling;
    return NOERROR;
}

ECode CInstrumentationInfo::SetHandleProfiling(
    /* [in] */ Boolean handleProfiling)
{
    mHandleProfiling = handleProfiling;
    return NOERROR;
}

ECode CInstrumentationInfo::IsFunctionalTest(
    /* [out] */ Boolean* functionalTest)
{
    VALIDATE_NOT_NULL(functionalTest);
    *functionalTest = mFunctionalTest;
    return NOERROR;
}

ECode CInstrumentationInfo::SetFunctionalTest(
    /* [in] */ Boolean functionalTest)
{
    mFunctionalTest = functionalTest;
    return NOERROR;
}

ECode CInstrumentationInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInstrumentationInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
}
