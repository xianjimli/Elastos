
#include "ext/frameworkdef.h"
#include "privacy/PrivacySettingsManager.h"


PrivacySettingsManager::PrivacySettingsManager(
    /* [in] */ IContext* context,
    /* [in] */ IPrivacySettingsManager* service)
    : mService(service)
{
    // Log.d(TAG, "PrivacySettingsManager - initializing for package: " + context.getPackageName() +
    //         " UID:" + Binder.getCallingUid());
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
    VALIDATE_NOT_NULL(privactySettings);

    // try {
    //     Log.d(TAG, "getSettings for package: " + packageName + " UID: " + uid);
    if (mService != NULL) {
        return mService->GetSettings(capsuleName, uid, privactySettings);
    }
    else {
        // Log.e(TAG, "getSettings - PrivacySettingsManagerService is null");
        *privactySettings = NULL;
        return NOERROR;
    }
    // } catch (RemoteException e) {
    //     e.printStackTrace();
    //     return null;
    // }
}

ECode PrivacySettingsManager::SaveSettings(
    /* [in] */ IPrivacySettings* settings,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // try { //
        // Log.d(TAG, "saveSettings - " + settings);
    if (mService != NULL) {
        return mService->SaveSettings(settings, result);
    }
    else {
        // Log.e(TAG, "saveSettings - PrivacySettingsManagerService is null");
        *result = FALSE;
        return NOERROR;
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "RemoteException insaveSettings: ", e);
    //     return false;
    // }
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


