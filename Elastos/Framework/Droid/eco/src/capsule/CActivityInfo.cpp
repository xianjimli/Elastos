
#include "capsule/CActivityInfo.h"
#include "ext/frameworkdef.h"

const Int32 CActivityInfo::LAUNCH_MULTIPLE;
const Int32 CActivityInfo::LAUNCH_SINGLE_TOP;
const Int32 CActivityInfo::LAUNCH_SINGLE_TASK;
const Int32 CActivityInfo::LAUNCH_SINGLE_INSTANCE;
const Int32 CActivityInfo::FLAG_MULTIPROCESS;
const Int32 CActivityInfo::FLAG_FINISH_ON_TASK_LAUNCH;
const Int32 CActivityInfo::FLAG_CLEAR_TASK_ON_LAUNCH;
const Int32 CActivityInfo::FLAG_ALWAYS_RETAIN_TASK_STATE;
const Int32 CActivityInfo::FLAG_STATE_NOT_NEEDED;
const Int32 CActivityInfo::FLAG_EXCLUDE_FROM_RECENTS;
const Int32 CActivityInfo::FLAG_ALLOW_TASK_REPARENTING;
const Int32 CActivityInfo::FLAG_NO_HISTORY;
const Int32 CActivityInfo::FLAG_FINISH_ON_CLOSE_SYSTEM_DIALOGS;
const Int32 CActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED;
const Int32 CActivityInfo::SCREEN_ORIENTATION_LANDSCAPE;
const Int32 CActivityInfo::SCREEN_ORIENTATION_PORTRAIT;
const Int32 CActivityInfo::SCREEN_ORIENTATION_USER;
const Int32 CActivityInfo::SCREEN_ORIENTATION_BEHIND;
const Int32 CActivityInfo::SCREEN_ORIENTATION_SENSOR;
const Int32 CActivityInfo::SCREEN_ORIENTATION_NOSENSOR;
const Int32 CActivityInfo::SCREEN_ORIENTATION_SENSOR_LANDSCAPE;
const Int32 CActivityInfo::SCREEN_ORIENTATION_SENSOR_PORTRAIT;
const Int32 CActivityInfo::SCREEN_ORIENTATION_REVERSE_LANDSCAPE;
const Int32 CActivityInfo::SCREEN_ORIENTATION_REVERSE_PORTRAIT;
const Int32 CActivityInfo::SCREEN_ORIENTATION_FULL_SENSOR;
const Int32 CActivityInfo::CONFIG_MCC;
const Int32 CActivityInfo::CONFIG_MNC;
const Int32 CActivityInfo::CONFIG_LOCALE;
const Int32 CActivityInfo::CONFIG_TOUCHSCREEN;
const Int32 CActivityInfo::CONFIG_KEYBOARD;
const Int32 CActivityInfo::CONFIG_KEYBOARD_HIDDEN;
const Int32 CActivityInfo::CONFIG_NAVIGATION;
const Int32 CActivityInfo::CONFIG_ORIENTATION;
const Int32 CActivityInfo::CONFIG_SCREEN_LAYOUT;
const Int32 CActivityInfo::CONFIG_UI_MODE;
const Int32 CActivityInfo::CONFIG_FONT_SCALE;

CActivityInfo::CActivityInfo()
    : mTheme(0)
    , mLaunchMode(0)
    , mFlags(0)
    , mScreenOrientation(SCREEN_ORIENTATION_UNSPECIFIED)
    , mConfigChanges(0)
    , mSoftInputMode(0)
{}

CActivityInfo::~CActivityInfo()
{}

ECode CActivityInfo::constructor()
{
    return NOERROR;
}

ECode CActivityInfo::constructor(
    /* [in] */ IActivityInfo* orig)
{
    VALIDATE_NOT_NULL(orig);

    AutoPtr<CActivityInfo> corig = (CActivityInfo*)orig;
    ComponentInfo::constructor((ComponentInfo*)(CActivityInfo*)corig);
//    theme = orig.theme;
    mLaunchMode = corig->mLaunchMode;
//    permission = orig.permission;
    mTaskAffinity = corig->mTaskAffinity;
    mTargetActivity = corig->mTargetActivity;
    mFlags = corig->mFlags;
//    screenOrientation = orig.screenOrientation;
//    configChanges = orig.configChanges;
//    softInputMode = orig.softInputMode;
    return NOERROR;
}

ECode CActivityInfo::LoadLabel(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return ComponentInfo::LoadLabel(pm, label);
}

ECode CActivityInfo::LoadIcon(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::LoadIcon(pm, icon);
}

ECode CActivityInfo::LoadLogo(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::LoadLogo(pm, icon);
}

ECode CActivityInfo::LoadXmlMetaData(
    /* [in] */ ICapsuleManager* pm,
    /* [in] */ const String& name,
    /* [out] */ IXmlResourceParser** resource)
{
    VALIDATE_NOT_NULL(resource);
    return ComponentInfo::LoadXmlMetaData(pm, name, resource);
}

ECode CActivityInfo::GetCapsuleName(
    /* [out] */ String* capsuleName)
{
    VALIDATE_NOT_NULL(capsuleName);
    *capsuleName = ComponentInfo::GetCapsuleName();
    return NOERROR;
}

ECode CActivityInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = ComponentInfo::GetName();
    return NOERROR;
}

ECode CActivityInfo::GetComponentName(
    /* [out] */ String *capsuleName,
    /* [out] */ String *name)
{
    *capsuleName = mCapsuleName;
    *name = mName;

    return NOERROR;
}

ECode CActivityInfo::SetComponentName(
    /* [in] */ const String& capsuleName,
    /* [in] */ const String& className)
{
    mCapsuleName = capsuleName;
    mName = className;

    return NOERROR;
}

ECode CActivityInfo::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    if (info == NULL) return E_INVALID_ARGUMENT;

    *info = (IApplicationInfo*)(CApplicationInfo*)mApplicationInfo;
    if (*info != NULL) (*info)->AddRef();

    return NOERROR;
}

ECode CActivityInfo::SetApplicationInfo(
    /* [in] */ IApplicationInfo* info)
{
    mApplicationInfo = (CApplicationInfo*)info;

    return NOERROR;
}

ECode CActivityInfo::IsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = mEnabled;
    return NOERROR;
}

ECode CActivityInfo::GetLaunchMode(
    /* [out] */ Int32 *mode)
{
    if (mode == NULL) return E_INVALID_ARGUMENT;

    *mode = mLaunchMode;
    return NOERROR;
}

ECode CActivityInfo::SetLaunchMode(
    /* [in] */ Int32 mode)
{
    mLaunchMode = mode;
    return NOERROR;
}

ECode CActivityInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    ComponentInfo::ReadFromParcel(source);
    source->ReadInt32(&mLaunchMode);
    return NOERROR;
}

ECode CActivityInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    ComponentInfo::WriteToParcel(dest);
    dest->WriteInt32(mLaunchMode);
    return NOERROR;
}

