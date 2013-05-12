
#include "ext/frameworkdef.h"
#include "capsule/CCapsuleInfo.h"


ECode CCapsuleInfo::GetCapsuleName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mCapsuleName;
    return NOERROR;
}

ECode CCapsuleInfo::SetCapsuleName(
    /* [in] */ const String& name)
{
    mCapsuleName = name;
    return NOERROR;
}

ECode CCapsuleInfo::GetVersionCode(
    /* [out] */ Int32* versionCode)
{
    VALIDATE_NOT_NULL(versionCode);
    *versionCode = mVersionCode;
    return NOERROR;
}

ECode CCapsuleInfo::SetVersionCode(
    /* [in] */ Int32 versionCode)
{
    mVersionCode = versionCode;
    return NOERROR;
}

ECode CCapsuleInfo::GetVersionName(
    /* [out] */ String* versionName)
{
    VALIDATE_NOT_NULL(versionName);
    *versionName = mVersionName;
    return NOERROR;
}

ECode CCapsuleInfo::SetVersionName(
    /* [in] */ const String& versionName)
{
    mVersionName = versionName;
    return NOERROR;
}

ECode CCapsuleInfo::GetSharedUserId(
    /* [out] */ String* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mSharedUserId;
    return NOERROR;
}

ECode CCapsuleInfo::SetSharedUserId(
    /* [in] */ const String& id)
{
    mSharedUserId = id;
    return NOERROR;
}

ECode CCapsuleInfo::GetSharedUserLabel(
    /* [out] */ Int32* label)
{
    VALIDATE_NOT_NULL(label);
    *label = mSharedUserLabel;
    return NOERROR;
}

ECode CCapsuleInfo::SetSharedUserLabel(
    /* [in] */ Int32 label)
{
    mSharedUserLabel = label;
    return NOERROR;
}

ECode CCapsuleInfo::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = mApplicationInfo;
    if (*info != NULL) (*info)->AddRef();
    return NOERROR;
}

ECode CCapsuleInfo::SetApplicationInfo(
    /* [in] */ IApplicationInfo* info)
{
    mApplicationInfo = info;
    return NOERROR;
}

ECode CCapsuleInfo::GetFirstInstallTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = mFirstInstallTime;
    return NOERROR;
}

ECode CCapsuleInfo::SetFirstInstallTime(
    /* [in] */ Int64 time)
{
    mFirstInstallTime = time;
    return NOERROR;
}

ECode CCapsuleInfo::GetLastUpdateTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = mLastUpdateTime;
    return NOERROR;
}

ECode CCapsuleInfo::SetLastUpdateTime(
    /* [in] */ Int64 time)
{
    mLastUpdateTime = time;
    return NOERROR;
}

ECode CCapsuleInfo::GetGids(
    /* [out, callee] */ ArrayOf<Int32>** gids)
{
    VALIDATE_NOT_NULL(gids);
    *gids = mGids != NULL ? mGids->Clone() : NULL;
    return NOERROR;
}

ECode CCapsuleInfo::SetGids(
    /* [in] */ ArrayOf<Int32>* gids)
{
    mGids = gids != NULL ? gids->Clone() : NULL;
    return NOERROR;
}

ECode CCapsuleInfo::AddActivity(
    /* [in] */ IActivityInfo* activity)
{
    if (activity == NULL) return NOERROR;
    mActivities.PushBack(activity);
    return NOERROR;
}

ECode CCapsuleInfo::AddReceiver(
    /* [in] */ IActivityInfo* receiver)
{
    if (receiver == NULL) return NOERROR;
    mReceivers.PushBack(receiver);
    return NOERROR;
}

ECode CCapsuleInfo::AddService(
    /* [in] */ IServiceInfo* service)
{
    if (service == NULL) return NOERROR;
    mServices.PushBack(service);
    return NOERROR;
}

ECode CCapsuleInfo::AddContentProvider(
    /* [in] */ IContentProviderInfo* provider)
{
    if (provider == NULL) return NOERROR;
    mContentProviders.PushBack(provider);
    return NOERROR;
}

ECode CCapsuleInfo::AddInstrumentation(
    /* [in] */ IInstrumentationInfo* instumentation)
{
    if (instumentation == NULL) return NOERROR;
    mInstrumentations.PushBack(instumentation);
    return NOERROR;
}

ECode CCapsuleInfo::AddPermission(
    /* [in] */ IPermissionInfo* permission)
{
    if (permission == NULL) return NOERROR;
    mPermissions.PushBack(permission);
    return NOERROR;
}

ECode CCapsuleInfo::AddRequestedPermission(
    /* [in] */ const String& permission)
{
    if (permission.IsNull()) return NOERROR;
    mRequestedPermissions.PushBack(permission);
    return NOERROR;
}

ECode CCapsuleInfo::AddSignature(
    /* [in] */ ISignature* signature)
{
    if (signature == NULL) return NOERROR;
    mSignatures.PushBack(signature);
    return NOERROR;
}

ECode CCapsuleInfo::GetSignatures(
    /* [out] */ IObjectContainer** signatures)
{
    assert(signatures != NULL && *signatures != NULL);

    List< AutoPtr<ISignature> >::Iterator it = mSignatures.Begin();
    for (; it != mSignatures.End(); ++it) {
        (*signatures)->Add(*it);
    }

    return NOERROR;
}

ECode CCapsuleInfo::AddConfigPreference(
    /* [in] */ IConfigurationInfo* config)
{
    if (config == NULL) return NOERROR;
    mConfigPreferences.PushBack(config);
    return NOERROR;
}

ECode CCapsuleInfo::AddReqFeature(
    /* [in] */ IFeatureInfo* feature)
{
    if (feature == NULL) return NOERROR;
    mReqFeatures.PushBack(feature);
    return NOERROR;
}

ECode CCapsuleInfo::GetInstallLocation(
    /* [out] */ Int32* location)
{
    VALIDATE_NOT_NULL(location);
    *location = mInstallLocation;
    return NOERROR;
}

ECode CCapsuleInfo::SetInstallLocation(
    /* [in] */ Int32 location)
{
    mInstallLocation = location;
    return NOERROR;
}

ECode CCapsuleInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadString(&mCapsuleName);
    return NOERROR;
}

ECode CCapsuleInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteString(mCapsuleName);
    return NOERROR;
}

