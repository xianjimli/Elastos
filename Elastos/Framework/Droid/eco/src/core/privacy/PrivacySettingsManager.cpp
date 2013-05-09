
#include "privacy/PrivacySettingsManager.h"

PrivacySettingsManager::PrivacySettingsManager(
    /* [in] */ IContext* context,
    /* [in] */ IPrivacySettingsManager* service)
{
}

PInterface PrivacySettingsManager::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IPrivacySettingsManager*)this;
    }
    else if (riid == EIID_IPrivacySettingsManager) {
        return (IPrivacySettingsManager*)this;
    }

    return NULL;
}

UInt32 PrivacySettingsManager::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 PrivacySettingsManager::Release()
{
    return ElRefBase::Release();
}

ECode PrivacySettingsManager::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IPrivacySettingsManager*)this) {
        *pIID = EIID_IPrivacySettingsManager;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode PrivacySettingsManager::GetSettings(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 uid,
    /* [out] */ IPrivacySettings** privactySettings)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode PrivacySettingsManager::SaveSettings(
    /* [in] */ IPrivacySettings* settings,
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode PrivacySettingsManager::DeleteSettings(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 uid,
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode PrivacySettingsManager::Notification(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 uid,
    /* [in] */ Byte accessMode,
    /* [in] */ const String& dataType,
    /* [in] */ const String& output)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode PrivacySettingsManager::RegisterObservers()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode PrivacySettingsManager::AddObserver(
    /* [in] */ const String& capsuleName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode PrivacySettingsManager::PurgeSettings(
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode PrivacySettingsManager::GetVersion(
    /* [out] */ Double* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}


