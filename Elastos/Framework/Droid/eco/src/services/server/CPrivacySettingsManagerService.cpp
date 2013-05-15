
#include "ext/frameworkdef.h"
#include "server/CPrivacySettingsManagerService.h"
#include <stdio.h>


ECode CPrivacySettingsManagerService::constructor(
    /* [in] */ IContext* context)
{
    printf("\t + CPrivacySettingsManagerService::constructor");
    mContext = context;
    // CPrivacyPersistenceAdapter::New(context, &mPersistence);
    return NOERROR;
}

ECode CPrivacySettingsManagerService::GetSettings(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 uid,
    /* [out] */ IPrivacySettings** privacySettings)
{
    printf("\t + CPrivacySettingsManagerService::GetSettings");
    //VALIDATE_NOT_NULL(privacySettings);

    // Log.d(TAG, "getSettings - " + packageName + " UID: " + uid);
    // return persistenceAdapter.getSettings(packageName, uid, false);
    //TODO:
    /*
    AutoPtr<IPrivacySettings> settings;
    CPrivacySettings::New(0, String("ButtonDemo"), 0, PrivacySettings_RANDOM,
            String("0.0"), String("0.0"), (IPrivacySettings**)&settings);
    *privacySettings = settings.Get();
    (*privacySettings)->AddRef();
    */

    // return mPersistence->GetSettings(capsuleName, 0, false, privacySettings);
    return NOERROR;
}

ECode CPrivacySettingsManagerService::SaveSettings(
    /* [in] */ IPrivacySettings* settings,
    /* [out] */ Boolean* result)
{
    printf("\t + CPrivacySettingsManagerService::SaveSettings");
    // return mPersistence->SaveSettings(settings, result);
    return NOERROR;
}

ECode CPrivacySettingsManagerService::DeleteSettings(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 uid,
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPrivacySettingsManagerService::GetVersion(
    /* [out] */ Double* version)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPrivacySettingsManagerService::Notification(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 uid,
    /* [in] */ Byte accessMode,
    /* [in] */ const String& dataType,
    /* [in] */ const String& output)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPrivacySettingsManagerService::RegisterObservers()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPrivacySettingsManagerService::AddObserver(
    /* [in] */ const String& capsuleName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPrivacySettingsManagerService::PurgeSettings(
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

