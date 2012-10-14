
#include "capsule/CContentProviderInfo.h"
#include "utils/CParcelableObjectContainer.h"

CContentProviderInfo::CContentProviderInfo() :
    mGrantUriPermissions(FALSE),
    mUriPermissionPatterns(NULL),
    mPathPermissions(NULL),
    mMultiprocess(FALSE),
    mInitOrder(0),
    mIsSyncable(FALSE)
{}

CContentProviderInfo::~CContentProviderInfo()
{}

ECode CContentProviderInfo::constructor()
{
    return NOERROR;
}

ECode CContentProviderInfo::constructor(
    /* [in] */ IContentProviderInfo* orig)
{
    AutoPtr<CContentProviderInfo> corig = (CContentProviderInfo*)orig;
    ComponentInfo::constructor((ComponentInfo*)(CContentProviderInfo*)corig);
    mAuthority = corig->mAuthority;
//    readPermission = orig.readPermission;
//    writePermission = orig.writePermission;
//    grantUriPermissions = orig.grantUriPermissions;
//    uriPermissionPatterns = orig.uriPermissionPatterns;
//    pathPermissions = orig.pathPermissions;
//    multiprocess = orig.multiprocess;
//    initOrder = orig.initOrder;
    mIsSyncable = corig->mIsSyncable;
    return NOERROR;
}

ECode CContentProviderInfo::LoadLabel(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return ComponentInfo::LoadLabel(pm, label);
}

ECode CContentProviderInfo::LoadIcon(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::LoadIcon(pm, icon);
}

ECode CContentProviderInfo::LoadLogo(
    /* [in] */ ICapsuleManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::LoadLogo(pm, icon);
}

ECode CContentProviderInfo::LoadXmlMetaData(
    /* [in] */ ICapsuleManager* pm,
    /* [in] */ const String& name,
    /* [out] */ IXmlResourceParser** resource)
{
    VALIDATE_NOT_NULL(resource);
    return ComponentInfo::LoadXmlMetaData(pm, name, resource);
}

ECode CContentProviderInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode CContentProviderInfo::SetName(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

ECode CContentProviderInfo::GetCapsuleName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mCapsuleName;
    return NOERROR;
}

ECode CContentProviderInfo::SetCapsuleName(
    /* [in] */ const String& name)
{
    mCapsuleName = name;
    return NOERROR;
}

ECode CContentProviderInfo::GetLabelRes(
    /* [out] */ Int32* labelRes)
{
    VALIDATE_NOT_NULL(labelRes);
    *labelRes = mLabelRes;
    return NOERROR;
}

ECode CContentProviderInfo::SetLabelRes(
    /* [in] */ Int32 labelRes)
{
    mLabelRes = labelRes;
    return NOERROR;
}

ECode CContentProviderInfo::GetNonLocalizedLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    *label = mNonLocalizedLabel;
    if (*label != NULL) (*label)->AddRef();
    return NOERROR;
}

ECode CContentProviderInfo::SetNonLocalizedLabel(
    /* [in] */ ICharSequence* label)
{
    mNonLocalizedLabel = label;
    return NOERROR;
}

ECode CContentProviderInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    *icon = mIcon;
    return NOERROR;
}

ECode CContentProviderInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    mIcon = icon;
    return NOERROR;
}

ECode CContentProviderInfo::GetLogo(
    /* [out] */ Int32* logo)
{
    VALIDATE_NOT_NULL(logo);
    *logo = mLogo;
    return NOERROR;
}

ECode CContentProviderInfo::SetLogo(
    /* [in] */ Int32 logo)
{
    mLogo = logo;
    return NOERROR;
}

ECode CContentProviderInfo::GetMetaData(
    /* [out] */ IBundle** metaData)
{
    VALIDATE_NOT_NULL(metaData);
    *metaData = mMetaData;
    if (*metaData != NULL) (*metaData)->AddRef();
    return NOERROR;
}

ECode CContentProviderInfo::SetMetaData(
    /* [in] */ IBundle* metaData)
{
    mMetaData = metaData;
    return NOERROR;
}

ECode CContentProviderInfo::GetComponentName(
    /* [out] */ String * pCapsuleName,
    /* [out] */ String * pClassName)
{
    *pCapsuleName = mCapsuleName;
    *pClassName = mName;

    return NOERROR;
}

ECode CContentProviderInfo::SetComponentName(
    /* [in] */ const String& capsuleName,
    /* [in] */ const String& className)
{
    mCapsuleName = capsuleName;
    mName = className;

    return NOERROR;
}

ECode CContentProviderInfo::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    if (info == NULL) return E_INVALID_ARGUMENT;

    *info = (IApplicationInfo*)(CApplicationInfo*)mApplicationInfo;
    if (*info != NULL) (*info)->AddRef();

    return NOERROR;
}

ECode CContentProviderInfo::SetApplicationInfo(
    /* [in] */ IApplicationInfo* info)
{
    mApplicationInfo = (CApplicationInfo*)info;

    return NOERROR;
}

ECode CContentProviderInfo::GetProcessName(
    /* [out] */ String* processName)
{
    VALIDATE_NOT_NULL(processName);
    *processName = mProcessName;
    return NOERROR;
}

ECode CContentProviderInfo::SetProcessName(
    /* [in] */ const String& processName)
{
    mProcessName = processName;
    return NOERROR;
}

ECode CContentProviderInfo::GetDescriptionRes(
    /* [out] */ Int32* desRes)
{
    VALIDATE_NOT_NULL(desRes);
    *desRes = mDescriptionRes;
    return NOERROR;
}

ECode CContentProviderInfo::SetDescriptionRes(
    /* [in] */ Int32 desRes)
{
    mDescriptionRes = desRes;
    return NOERROR;
}

ECode CContentProviderInfo::IsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = mEnabled;
    return NOERROR;
}

ECode CContentProviderInfo::SetEnabled(
    /* [in] */ Boolean enabled)
{
    mEnabled = enabled;
    return NOERROR;
}

ECode CContentProviderInfo::IsExported(
    /* [out] */ Boolean* exported)
{
    VALIDATE_NOT_NULL(exported);
    *exported = mExported;
    return NOERROR;
}

ECode CContentProviderInfo::SetExported(
    /* [in] */ Boolean exported)
{
    mExported = exported;
    return NOERROR;
}

ECode CContentProviderInfo::GetAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority);
    *authority = mAuthority;
    return NOERROR;
}

ECode CContentProviderInfo::SetAuthority(
    /* [in] */ const String& authority)
{
    mAuthority = authority;
    return NOERROR;
}

ECode CContentProviderInfo::GetReadPermission(
    /* [out] */ String* permission)
{
    VALIDATE_NOT_NULL(permission);
    *permission = mReadPermission;
    return NOERROR;
}

ECode CContentProviderInfo::SetReadPermission(
    /* [in] */ const String& permission)
{
    mReadPermission = permission;
    return NOERROR;
}

ECode CContentProviderInfo::GetWritePermission(
    /* [out] */ String* permission)
{
    VALIDATE_NOT_NULL(permission);
    *permission = mWritePermission;
    return NOERROR;
}

ECode CContentProviderInfo::SetWritePermission(
    /* [in] */ const String& permission)
{
    mWritePermission = permission;
    return NOERROR;
}

ECode CContentProviderInfo::SetGrantUriPermissions(
    /* [in] */ Boolean grant)
{
    mGrantUriPermissions = grant;
    return NOERROR;
}

ECode CContentProviderInfo::InitUriPermissionPatterns()
{
    mUriPermissionPatterns = new List<AutoPtr<CPatternMatcher> >();
    return NOERROR;
}

ECode CContentProviderInfo::AddUriPermissionPatterns(
    /* [in] */ IPatternMatcher* pattern)
{
    mUriPermissionPatterns->PushBack((CPatternMatcher*)pattern);
    return NOERROR;
}

ECode CContentProviderInfo::GetUriPermissionPatterns(
    /* [out] */ IObjectContainer** permissions)
{
    VALIDATE_NOT_NULL(permissions);
    *permissions = NULL;
    if (mUriPermissionPatterns == NULL) return NOERROR;
    FAIL_RETURN(CParcelableObjectContainer::New(permissions));
    List<AutoPtr<CPatternMatcher> >::Iterator it;
    for (it = mUriPermissionPatterns->Begin();
         it != mUriPermissionPatterns->End(); ++it) {
        (*permissions)->Add((IPatternMatcher*)(*it).Get());
    }
    return NOERROR;
}

ECode CContentProviderInfo::ClearUriPermissionPatterns()
{
    delete mUriPermissionPatterns;
    mUriPermissionPatterns = NULL;
    return NOERROR;
}

ECode CContentProviderInfo::InitPathPermissions()
{
    mPathPermissions = new List< AutoPtr<CPathPermission> >();
    return NOERROR;
}

ECode CContentProviderInfo::AddPathPermission(
    /* [in] */ IPathPermission* permission)
{
    mPathPermissions->PushBack((CPathPermission*)permission);
    return NOERROR;
}

ECode CContentProviderInfo::GetPathPermissions(
    /* [out] */ IObjectContainer** permissions)
{
    VALIDATE_NOT_NULL(permissions);
    *permissions = NULL;
    if (mPathPermissions == NULL) return NOERROR;
    FAIL_RETURN(CParcelableObjectContainer::New(permissions));
    List<AutoPtr<CPathPermission> >::Iterator it;
    for (it = mPathPermissions->Begin(); it != mPathPermissions->End(); ++it) {
        (*permissions)->Add((IPathPermission*)(*it).Get());
    }
    return NOERROR;
}

ECode CContentProviderInfo::SetPathPermissions(
    /* [in] */ IObjectContainer* permissions)
{
    if (permissions == NULL) return NOERROR;
    return E_NOT_IMPLEMENTED;
}

ECode CContentProviderInfo::SetMultiprocess(
    /* [in] */ Boolean multiprocess)
{
    mMultiprocess = multiprocess;
    return NOERROR;
}

ECode CContentProviderInfo::SetInitOrder(
    /* [in] */ Int32 initOrder)
{
    mInitOrder = initOrder;
    return NOERROR;
}

ECode CContentProviderInfo::GetSyncable(
    /* [out] */ Boolean* syncable)
{
    VALIDATE_NOT_NULL(syncable);
    *syncable = mIsSyncable;
    return NOERROR;
}

ECode CContentProviderInfo::SetSyncable(
    /* [in] */ Boolean syncable)
{
    mIsSyncable = syncable;
    return NOERROR;
}

ECode CContentProviderInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    ComponentInfo::ReadFromParcel(source);
    source->ReadString(&mAuthority);
    source->ReadBoolean(&mIsSyncable);
    return NOERROR;
}

ECode CContentProviderInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    ComponentInfo::WriteToParcel(dest);
    dest->WriteString(mAuthority);
    dest->WriteBoolean(mIsSyncable);
    return NOERROR;
}
