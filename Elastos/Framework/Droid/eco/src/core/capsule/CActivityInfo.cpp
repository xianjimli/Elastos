
#include "capsule/CActivityInfo.h"
#include "ext/frameworkdef.h"


CActivityInfo::CActivityInfo()
    : mTheme(0)
    , mLaunchMode(0)
    , mFlags(0)
    , mScreenOrientation(ActivityInfo_SCREEN_ORIENTATION_UNSPECIFIED)
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
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return ComponentInfo::LoadLabel(pm, label);
}

ECode CActivityInfo::LoadIcon(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::LoadIcon(pm, icon);
}

ECode CActivityInfo::LoadLogo(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::LoadLogo(pm, icon);
}

ECode CActivityInfo::LoadXmlMetaData(
    /* [in] */ ILocalCapsuleManager* pm,
    /* [in] */ const String& name,
    /* [out] */ IXmlResourceParser** resource)
{
    VALIDATE_NOT_NULL(resource);
    return ComponentInfo::LoadXmlMetaData(pm, name, resource);
}

ECode CActivityInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode CActivityInfo::SetName(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

ECode CActivityInfo::GetCapsuleName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mCapsuleName;
    return NOERROR;
}

ECode CActivityInfo::SetCapsuleName(
    /* [in] */ const String& name)
{
    mCapsuleName = name;
    return NOERROR;
}

ECode CActivityInfo::GetLabelRes(
    /* [out] */ Int32* labelRes)
{
    VALIDATE_NOT_NULL(labelRes);
    *labelRes = mLabelRes;
    return NOERROR;
}

ECode CActivityInfo::SetLabelRes(
    /* [in] */ Int32 labelRes)
{
    mLabelRes = labelRes;
    return NOERROR;
}

ECode CActivityInfo::GetNonLocalizedLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    *label = mNonLocalizedLabel;
    if (*label != NULL) (*label)->AddRef();
    return NOERROR;
}

ECode CActivityInfo::SetNonLocalizedLabel(
    /* [in] */ ICharSequence* label)
{
    mNonLocalizedLabel = label;
    return NOERROR;
}

ECode CActivityInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    *icon = mIcon;
    return NOERROR;
}

ECode CActivityInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    mIcon = icon;
    return NOERROR;
}

ECode CActivityInfo::GetLogo(
    /* [out] */ Int32* logo)
{
    VALIDATE_NOT_NULL(logo);
    *logo = mLogo;
    return NOERROR;
}

ECode CActivityInfo::SetLogo(
    /* [in] */ Int32 logo)
{
    mLogo = logo;
    return NOERROR;
}

ECode CActivityInfo::GetMetaData(
    /* [out] */ IBundle** metaData)
{
    VALIDATE_NOT_NULL(metaData);
    *metaData = mMetaData;
    if (*metaData != NULL) (*metaData)->AddRef();
    return NOERROR;
}

ECode CActivityInfo::SetMetaData(
    /* [in] */ IBundle* metaData)
{
    mMetaData = metaData;
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

ECode CActivityInfo::GetProcessName(
    /* [out] */ String* processName)
{
    VALIDATE_NOT_NULL(processName);
    *processName = mProcessName;
    return NOERROR;
}

ECode CActivityInfo::SetProcessName(
    /* [in] */ const String& processName)
{
    mProcessName = processName;
    return NOERROR;
}

ECode CActivityInfo::GetDescriptionRes(
    /* [out] */ Int32* desRes)
{
    VALIDATE_NOT_NULL(desRes);
    *desRes = mDescriptionRes;
    return NOERROR;
}

ECode CActivityInfo::SetDescriptionRes(
    /* [in] */ Int32 desRes)
{
    mDescriptionRes = desRes;
    return NOERROR;
}

ECode CActivityInfo::IsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = mEnabled;
    return NOERROR;
}

ECode CActivityInfo::SetEnabled(
    /* [in] */ Boolean enabled)
{
    mEnabled = enabled;
    return NOERROR;
}

ECode CActivityInfo::IsExported(
    /* [out] */ Boolean* exported)
{
    VALIDATE_NOT_NULL(exported);
    *exported = mExported;
    return NOERROR;
}

ECode CActivityInfo::SetExported(
    /* [in] */ Boolean exported)
{
    mExported = exported;
    return NOERROR;
}

ECode CActivityInfo::GetTheme(
    /* [out] */ Int32* theme)
{
    VALIDATE_NOT_NULL(theme);
    *theme = mTheme;
    return NOERROR;
}

ECode CActivityInfo::SetTheme(
    /* [in] */ Int32 theme)
{
    mTheme = theme;
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

ECode CActivityInfo::GetSoftInputMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = mSoftInputMode;
    return NOERROR;
}

ECode CActivityInfo::SetSoftInputMode(
    /* [in] */ Int32 mode)
{
    mSoftInputMode = mode;
    return NOERROR;
}

ECode CActivityInfo::GetPermission(
    /* [out] */ String* permission)
{
    VALIDATE_NOT_NULL(permission);
    *permission = mPermission;
    return NOERROR;
}

ECode CActivityInfo::SetPermission(
    /* [in] */ const String& permission)
{
    mPermission = permission;
    return NOERROR;
}

ECode CActivityInfo::GetTaskAffinity(
    /* [out] */ String* task)
{
    VALIDATE_NOT_NULL(task);
    *task = mTaskAffinity;
    return NOERROR;
}

ECode CActivityInfo::SetTaskAffinity(
    /* [in] */ const String& task)
{
    mTaskAffinity = task;
    return NOERROR;
}

ECode CActivityInfo::GetTargetActivity(
    /* [out] */ String* target)
{
    VALIDATE_NOT_NULL(target);
    *target = mTargetActivity;
    return NOERROR;
}

ECode CActivityInfo::SetTargetActivity(
    /* [in] */ const String& target)
{
    mTargetActivity = target;
    return NOERROR;
}

ECode CActivityInfo::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;
    return NOERROR;
}

ECode CActivityInfo::SetFlags(
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return NOERROR;
}

ECode CActivityInfo::GetScreenOrientation(
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);
    *orientation = mScreenOrientation;
    return NOERROR;
}

ECode CActivityInfo::SetScreenOrientation(
    /* [in] */ Int32 orientation)
{
    mScreenOrientation = orientation;
    return NOERROR;
}

ECode CActivityInfo::GetConfigChanges(
    /* [out] */ Int32* changes)
{
    VALIDATE_NOT_NULL(changes);
    *changes = mConfigChanges;
    return NOERROR;
}

ECode CActivityInfo::SetConfigChanges(
    /* [in] */ Int32 changes)
{
    mConfigChanges = changes;
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

ECode CActivityInfo::GetIconResource(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::GetIconResource(icon);
}

ECode CActivityInfo::GetThemeResource(
    /* [out] */ Int32* theme)
{
    VALIDATE_NOT_NULL(theme);
    if (mTheme !=0) *theme = mTheme;
    else *theme = ComponentInfo::mApplicationInfo->mTheme;
    return NOERROR;
}
