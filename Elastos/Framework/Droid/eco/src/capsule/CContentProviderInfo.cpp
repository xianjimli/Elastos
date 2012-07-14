
#include "capsule/CContentProviderInfo.h"

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
