
#include "content/CApplicationInfo.h"
#include "capsule/CCapsuleInfo.h"
#include "ext/frameworkdef.h"
#include <elastos/AutoPtr.h>


const Int32 CApplicationInfo::FLAG_SYSTEM;
const Int32 CApplicationInfo::FLAG_DEBUGGABLE;
const Int32 CApplicationInfo::FLAG_HAS_CODE;
const Int32 CApplicationInfo::FLAG_PERSISTENT;
const Int32 CApplicationInfo::FLAG_FACTORY_TEST;
const Int32 CApplicationInfo::FLAG_ALLOW_TASK_REPARENTING;
const Int32 CApplicationInfo::FLAG_ALLOW_CLEAR_USER_DATA;
const Int32 CApplicationInfo::FLAG_UPDATED_SYSTEM_APP;
const Int32 CApplicationInfo::FLAG_TEST_ONLY;
const Int32 CApplicationInfo::FLAG_SUPPORTS_SMALL_SCREENS;
const Int32 CApplicationInfo::FLAG_SUPPORTS_NORMAL_SCREENS;
const Int32 CApplicationInfo::FLAG_SUPPORTS_LARGE_SCREENS;
const Int32 CApplicationInfo::FLAG_RESIZEABLE_FOR_SCREENS;
const Int32 CApplicationInfo::FLAG_SUPPORTS_SCREEN_DENSITIES;
const Int32 CApplicationInfo::FLAG_VM_SAFE_MODE;
const Int32 CApplicationInfo::FLAG_ALLOW_BACKUP;
const Int32 CApplicationInfo::FLAG_KILL_AFTER_RESTORE;
const Int32 CApplicationInfo::FLAG_RESTORE_ANY_VERSION;
const Int32 CApplicationInfo::FLAG_EXTERNAL_STORAGE;
const Int32 CApplicationInfo::FLAG_SUPPORTS_XLARGE_SCREENS;
const Int32 CApplicationInfo::FLAG_NEVER_ENCRYPT;
const Int32 CApplicationInfo::FLAG_FORWARD_LOCK;
const Int32 CApplicationInfo::FLAG_CANT_SAVE_STATE;

CApplicationInfo::CApplicationInfo()
    : mDescriptionRes(0)
    , mTheme(0)
    , mFlags(0)
    , mUid(0)
    , mTargetSdkVersion(0)
    , mEnabled(TRUE)
    , mInstallLocation(CCapsuleInfo::INSTALL_LOCATION_UNSPECIFIED)
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

ECode CApplicationInfo::GetSourceDir(
    /* [out] */ String* sourceDir)
{
    *sourceDir = mSourceDir;

    return NOERROR;
}

ECode CApplicationInfo::SetSourceDir(
    /* [in] */ const String& sourceDir)
{
    mSourceDir = sourceDir;

    return NOERROR;
}

ECode CApplicationInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    CapsuleItemInfo::ReadFromParcel(source);
    source->ReadString(&mProcessName);
    source->ReadString(&mSourceDir);

    return NOERROR;
}

ECode CApplicationInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    CapsuleItemInfo::WriteToParcel(dest);
    dest->WriteString(mProcessName);
    dest->WriteString(mSourceDir);

    return NOERROR;
}

ECode CApplicationInfo::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

void CApplicationInfo::DisableCompatibilityMode()
{
    mFlags |= (FLAG_SUPPORTS_LARGE_SCREENS | FLAG_SUPPORTS_NORMAL_SCREENS |
        FLAG_SUPPORTS_SMALL_SCREENS | FLAG_RESIZEABLE_FOR_SCREENS |
        FLAG_SUPPORTS_SCREEN_DENSITIES | FLAG_SUPPORTS_XLARGE_SCREENS);
}


