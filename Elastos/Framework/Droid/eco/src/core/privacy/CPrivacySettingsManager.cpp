
#include "CPrivacySettingsManager.h"

ECode CPrivacySettingsManager::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IPrivacySettingsManager * pService)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPrivacySettingsManager::GetSettings(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid,
    /* [out] */ IPrivacySettings ** ppPrivactySettings)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPrivacySettingsManager::SaveSettings(
    /* [in] */ IPrivacySettings * pSettings,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPrivacySettingsManager::DeleteSettings(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPrivacySettingsManager::Notification(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid,
    /* [in] */ Byte accessMode,
    /* [in] */ const String& dataType,
    /* [in] */ const String& output)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPrivacySettingsManager::RegisterObservers()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPrivacySettingsManager::AddObserver(
    /* [in] */ const String& packageName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPrivacySettingsManager::PurgeSettings(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPrivacySettingsManager::GetVersion(
    /* [out] */ Double * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}


