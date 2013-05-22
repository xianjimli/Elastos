
#include "content/CApplicationInfo.h"
#include "capsule/CCapsuleInfo.h"
#include "ext/frameworkdef.h"
#include <elastos/AutoPtr.h>


CApplicationInfo::CApplicationInfo()
    : mDescriptionRes(0)
    , mTheme(0)
    , mFlags(0)
    , mUid(0)
    , mTargetSdkVersion(0)
    , mEnabled(TRUE)
    , mInstallLocation(CapsuleInfo_INSTALL_LOCATION_UNSPECIFIED)
{}

CApplicationInfo::~CApplicationInfo()
{}

ECode CApplicationInfo::constructor()
{
    return NOERROR;
}

ECode CApplicationInfo::constructor(
    /* [in] */ IApplicationInfo* orig)
{
    VALIDATE_NOT_NULL(orig);

    AutoPtr<CApplicationInfo> corig = (CApplicationInfo*)orig;
    CapsuleItemInfo::constructor((CapsuleItemInfo*)(CApplicationInfo*)corig);
//    taskAffinity = orig.taskAffinity;
//    permission = orig.permission;
    mProcessName = corig->mProcessName;
    mClassName = corig->mClassName;
//    theme = orig.theme;
//    flags = orig.flags;
    mSourceDir = corig->mSourceDir;
    mPublicSourceDir = corig->mPublicSourceDir;
//    resourceDirs = orig.resourceDirs;
//    sharedLibraryFiles = orig.sharedLibraryFiles;
    mDataDir = corig->mDataDir;
    mUid = corig->mUid;
//    targetSdkVersion = orig.targetSdkVersion;
//    enabled = orig.enabled;
//    manageSpaceActivityName = orig.manageSpaceActivityName;
//    descriptionRes = orig.descriptionRes;
    return NOERROR;
}

ECode CApplicationInfo::LoadLabel(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return CapsuleItemInfo::LoadLabel(pm, label);
}

ECode CApplicationInfo::LoadIcon(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return CapsuleItemInfo::LoadIcon(pm, icon);
}

ECode CApplicationInfo::LoadLogo(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return CapsuleItemInfo::LoadLogo(pm, icon);
}

ECode CApplicationInfo::LoadXmlMetaData(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [in] */ const String& name,
    /* [out] */ IXmlResourceParser** resource)
{
    VALIDATE_NOT_NULL(resource);
    return CapsuleItemInfo::LoadXmlMetaData(pm, name, resource);
}

ECode CApplicationInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode CApplicationInfo::SetName(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

ECode CApplicationInfo::GetCapsuleName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mCapsuleName;
    return NOERROR;
}

ECode CApplicationInfo::SetCapsuleName(
    /* [in] */ const String& name)
{
    mCapsuleName = name;
    return NOERROR;
}

ECode CApplicationInfo::GetLabelRes(
    /* [out] */ Int32* labelRes)
{
    VALIDATE_NOT_NULL(labelRes);
    *labelRes = mLabelRes;
    return NOERROR;
}

ECode CApplicationInfo::SetLabelRes(
    /* [in] */ Int32 labelRes)
{
    mLabelRes = labelRes;
    return NOERROR;
}

ECode CApplicationInfo::GetNonLocalizedLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    *label = mNonLocalizedLabel;
    if (*label != NULL) (*label)->AddRef();
    return NOERROR;
}

ECode CApplicationInfo::SetNonLocalizedLabel(
    /* [in] */ ICharSequence* label)
{
    mNonLocalizedLabel = label;
    return NOERROR;
}

ECode CApplicationInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    *icon = mIcon;
    return NOERROR;
}

ECode CApplicationInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    mIcon = icon;
    return NOERROR;
}

ECode CApplicationInfo::GetLogo(
    /* [out] */ Int32* logo)
{
    VALIDATE_NOT_NULL(logo);
    *logo = mLogo;
    return NOERROR;
}

ECode CApplicationInfo::SetLogo(
    /* [in] */ Int32 logo)
{
    mLogo = logo;
    return NOERROR;
}

ECode CApplicationInfo::GetMetaData(
    /* [out] */ IBundle** metaData)
{
    VALIDATE_NOT_NULL(metaData);
    *metaData = mMetaData;
    if (*metaData != NULL) (*metaData)->AddRef();
    return NOERROR;
}

ECode CApplicationInfo::SetMetaData(
    /* [in] */ IBundle* metaData)
{
    mMetaData = metaData;
    return NOERROR;
}

ECode CApplicationInfo::GetTaskAffinity(
    /* [out] */ String* task)
{
    VALIDATE_NOT_NULL(task);
    *task = mTaskAffinity;
    return NOERROR;
}

ECode CApplicationInfo::SetTaskAffinity(
    /* [in] */ const String& task)
{
    mTaskAffinity = task;
    return NOERROR;
}

ECode CApplicationInfo::GetPermission(
    /* [out] */ String* permission)
{
    VALIDATE_NOT_NULL(permission);
    *permission = mPermission;
    return NOERROR;
}

ECode CApplicationInfo::SetPermission(
    /* [in] */ const String& permission)
{
    mPermission = permission;
    return NOERROR;
}

ECode CApplicationInfo::GetProcessName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mProcessName;
    return NOERROR;
}

ECode CApplicationInfo::SetProcessName(
    /* [in] */ const String& name)
{
    mProcessName = name;
    return NOERROR;
}

ECode CApplicationInfo::GetClassName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mClassName;
    return NOERROR;
}

ECode CApplicationInfo::SetClassName(
    /* [in] */ const String& name)
{
    mClassName = name;
    return NOERROR;
}

ECode CApplicationInfo::GetDescriptionRes(
    /* [out] */ Int32* desRes)
{
    VALIDATE_NOT_NULL(desRes);
    *desRes = mDescriptionRes;
    return NOERROR;
}

ECode CApplicationInfo::SetDescriptionRes(
    /* [in] */ Int32 desRes)
{
    mDescriptionRes = desRes;
    return NOERROR;
}

ECode CApplicationInfo::GetTheme(
    /* [out] */ Int32* theme)
{
    VALIDATE_NOT_NULL(theme);
    *theme = mTheme;
    return NOERROR;
}

ECode CApplicationInfo::SetTheme(
    /* [in] */ Int32 theme)
{
    mTheme = theme;
    return NOERROR;
}

ECode CApplicationInfo::GetManageSpaceActivityName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mManageSpaceActivityName;
    return NOERROR;
}

ECode CApplicationInfo::SetManageSpaceActivityName(
    /* [in] */ const String& name)
{
    mManageSpaceActivityName = name;
    return NOERROR;
}

ECode CApplicationInfo::GetBackupAgentName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mBackupAgentName;
    return NOERROR;
}

ECode CApplicationInfo::SetBackupAgentName(
    /* [in] */ const String& name)
{
    mBackupAgentName = name;
    return NOERROR;
}

ECode CApplicationInfo::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;
    return NOERROR;
}

ECode CApplicationInfo::SetFlags(
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return NOERROR;
}

ECode CApplicationInfo::GetSourceDir(
    /* [out] */ String* sourceDir)
{
    VALIDATE_NOT_NULL(sourceDir);
    *sourceDir = mSourceDir;

    return NOERROR;
}

ECode CApplicationInfo::SetSourceDir(
    /* [in] */ const String& sourceDir)
{
    mSourceDir = sourceDir;

    return NOERROR;
}

ECode CApplicationInfo::GetPublicSourceDir(
    /* [out] */ String* publicSourceDir)
{
    VALIDATE_NOT_NULL(publicSourceDir);
    *publicSourceDir = mPublicSourceDir;
    return NOERROR;
}

ECode CApplicationInfo::SetPublicSourceDir(
    /* [in] */ const String& pubicSourceDir)
{
    mPublicSourceDir = pubicSourceDir;
    return NOERROR;
}

ECode CApplicationInfo::GetSharedLibraryFiles(
    /* [out, callee] */ ArrayOf<String>** sharedLibraryFiles)
{
    assert(sharedLibraryFiles != NULL);
    if (mSharedLibraryFiles != NULL) {
        *sharedLibraryFiles = ArrayOf<String>::Alloc(mSharedLibraryFiles->GetLength());

        for (Int32 i = 0; i < (*sharedLibraryFiles)->GetLength(); ++i) {
            (**sharedLibraryFiles)[i] = (*mSharedLibraryFiles)[i];
        }
    }

    return NOERROR;
}

ECode CApplicationInfo::SetSharedLibraryFiles(
    /* [in] */ ArrayOf<String>* sharedLibraryFiles)
{
    if (mSharedLibraryFiles != NULL) {
        for (Int32 i = 0; i < mSharedLibraryFiles->GetLength(); ++i) {
            (*mSharedLibraryFiles)[i].~String();
        }
        ArrayOf<String>::Free(mSharedLibraryFiles);
        mSharedLibraryFiles = NULL;
    }

    if (sharedLibraryFiles != NULL) {
        mSharedLibraryFiles = ArrayOf<String>::Alloc(sharedLibraryFiles->GetLength());
        for (Int32 i = 0; i < mSharedLibraryFiles->GetLength(); ++i) {
            (*mSharedLibraryFiles)[i] = (*sharedLibraryFiles)[i];
        }
    }

    return NOERROR;
}

ECode CApplicationInfo::GetDataDir(
    /* [out] */  String* dataDir)
{
    VALIDATE_NOT_NULL(dataDir);
    *dataDir = mDataDir;
    return NOERROR;
}

ECode CApplicationInfo::SetDataDir(
    /* [in] */ const String& dataDir)
{
    mDataDir = dataDir;
    return NOERROR;
}

ECode CApplicationInfo::GetNativeLibraryDir(
    /* [out] */ String* libraryDir)
{
    VALIDATE_NOT_NULL(libraryDir);
    *libraryDir = mNativeLibraryDir;
    return NOERROR;
}

ECode CApplicationInfo::SetNativeLibraryDir(
    /* [in] */ const String& libraryDir)
{
    mNativeLibraryDir = libraryDir;
    return NOERROR;
}

ECode CApplicationInfo::GetUid(
    /* [out] */ Int32* uid)
{
    VALIDATE_NOT_NULL(uid);
    *uid = mUid;
    return NOERROR;
}

ECode CApplicationInfo::SetUid(
    /* [in] */ Int32 uid)
{
    mUid = uid;
    return NOERROR;
}

ECode CApplicationInfo::GetTargetSdkVersion(
    /* [out] */ Int32* sdkVersion)
{
    VALIDATE_NOT_NULL(sdkVersion);
    *sdkVersion = mTargetSdkVersion;
    return NOERROR;
}

ECode CApplicationInfo::SetTargetSdkVersion(
    /* [in] */ Int32 sdkVersion)
{
    mTargetSdkVersion = sdkVersion;
    return NOERROR;
}

ECode CApplicationInfo::IsEnabled(
    /* [out] */ Boolean* isEnabled)
{
    VALIDATE_NOT_NULL(isEnabled);
    *isEnabled = mEnabled;
    return NOERROR;
}

ECode CApplicationInfo::SetEnabled(
    /* [in] */ Boolean enabled)
{
    mEnabled = enabled;
    return NOERROR;
}

ECode CApplicationInfo::GetInstallLocation(
    /* [out] */ Int32* location)
{
    VALIDATE_NOT_NULL(location);
    *location = mInstallLocation;
    return NOERROR;
}

ECode CApplicationInfo::SetInstallLocation(
    /* [in] */ Int32 location)
{
    mInstallLocation = location;
    return NOERROR;
}

ECode CApplicationInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    CapsuleItemInfo::ReadFromParcel(source);
    source->ReadString(&mProcessName);
    source->ReadString(&mClassName);
    source->ReadString(&mSourceDir);
    source->ReadString(&mDataDir);

    return NOERROR;
}

ECode CApplicationInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    CapsuleItemInfo::WriteToParcel(dest);
    dest->WriteString(mProcessName);
    dest->WriteString(mClassName);
    dest->WriteString(mSourceDir);
    dest->WriteString(mDataDir);

    return NOERROR;
}

ECode CApplicationInfo::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplicationInfo::DisableCompatibilityMode()
{
    mFlags |= ApplicationInfo_FLAG_SUPPORTS_LARGE_SCREENS |
              ApplicationInfo_FLAG_SUPPORTS_NORMAL_SCREENS |
              ApplicationInfo_FLAG_SUPPORTS_SMALL_SCREENS |
              ApplicationInfo_FLAG_RESIZEABLE_FOR_SCREENS |
              ApplicationInfo_FLAG_SUPPORTS_SCREEN_DENSITIES |
              ApplicationInfo_FLAG_SUPPORTS_XLARGE_SCREENS;
    return NOERROR;
}


