
#include "server/usb/UsbService.h"

using namespace Elastos;

const String UsbService::PACKAGE_MANAGER_FEATURE_USB_HOST = String("android.hardware.usb.host");
const String UsbService::MANIFEST_PERMISSION_MANAGE_USB = String("android.permission.MANAGE_USB");

UsbService::UsbService(
    /* [in] */ IContext* context)
{
    mContext = context;
    mSettingsManager = new UsbSettingsManager(context);

    AutoPtr<ILocalCapsuleManager> cm;
    mContext->GetCapsuleManager((ILocalCapsuleManager**)&cm);

    Boolean hasFeature = FALSE;
    cm->HasSystemFeature(PACKAGE_MANAGER_FEATURE_USB_HOST, &hasFeature);

    if (hasFeature == TRUE) {
        mHostManager = new UsbHostManager(context, mSettingsManager);
    }

    AutoPtr<IFile> file;
    CFile::New(String("/sys/class/android_usb"), (IFile**)&file);

    Boolean isExist = FALSE;
    file->Exists(&isExist);

    if (isExist == TRUE) {
        mDeviceManager = new UsbDeviceManager(context, mSettingsManager);
    }
}

UInt32 UsbService::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 UsbService::Release()
{
    return ElRefBase::Release();
}

ECode UsbService::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode UsbService::GetDeviceList(
    /* [out] */ IBundle** devices)
{
    VALIDATE_NOT_NULL(devices);

    if (mHostManager == NULL) {
        return NOERROR;
    }

    mHostManager->GetDeviceList(*devices);
    return NOERROR;
}

ECode UsbService::OpenDevice(
    /* [in] */ const String& deviceName,
    /* [out] */ IParcelFileDescriptor** descriptor)
{
    VALIDATE_NOT_NULL(descriptor);

    if (mHostManager == NULL) {
        *descriptor = NULL;
        return NOERROR;
    }

    *descriptor = mHostManager->OpenDevice(deviceName);
    return NOERROR;
}

ECode UsbService::GetCurrentAccessory(
    /* [out] */ IUsbAccessory** accessory)
{
    VALIDATE_NOT_NULL(accessory);

    if (mDeviceManager == NULL) {
        *accessory = NULL;
        return NOERROR;
    }

    *accessory = mDeviceManager->GetCurrentAccessory();
    return NOERROR;
}

ECode UsbService::OpenAccessory(
    /* [in] */ IUsbAccessory* accessory,
    /* [out] */ IParcelFileDescriptor** descriptor)
{
    VALIDATE_NOT_NULL(descriptor);

    if (mDeviceManager == NULL) {
        *descriptor = NULL;
        return NOERROR;
    }

    *descriptor = mDeviceManager->OpenAccessory(accessory);
    return NOERROR;
}

ECode UsbService::SetDevicePackage(
    /* [in] */ IUsbDevice* device,
    /* [in] */ const String& packageName)
{
    mContext->EnforceCallingOrSelfPermission(MANIFEST_PERMISSION_MANAGE_USB, NULL);
    mSettingsManager->SetDevicePackage(device, packageName);

    return NOERROR;
}

ECode UsbService::SetAccessoryPackage(
    /* [in] */ IUsbAccessory* accessory,
    /* [in] */ const String& packageName)
{
    mContext->EnforceCallingOrSelfPermission(MANIFEST_PERMISSION_MANAGE_USB, NULL);
    mSettingsManager->SetAccessoryPackage(accessory, packageName);

    return NOERROR;
}

ECode UsbService::HasDevicePermission(
    /* [in] */ IUsbDevice* device,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mSettingsManager->HasPermission(device);

    return NOERROR;
}

ECode UsbService::HasAccessoryPermission(
    /* [in] */ IUsbAccessory* accessory,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mSettingsManager->HasPermission(accessory);

    return NOERROR;
}

ECode UsbService::RequestDevicePermission(
    /* [in] */ IUsbDevice* device,
    /* [in] */ const String& packageName,
    /* [in] */ IPendingIntent* pi)
{
    mSettingsManager->RequestPermission(device, packageName, pi);

    return NOERROR;
}

ECode UsbService::RequestAccessoryPermission(
    /* [in] */ IUsbAccessory* accessory,
    /* [in] */ const String& packageName,
    /* [in] */ IPendingIntent* pi)
{
    mSettingsManager->RequestPermission(accessory, packageName, pi);

    return NOERROR;
}

ECode UsbService::GrantDevicePermission(
    /* [in] */ IUsbDevice* device,
    /* [in] */ Int32 uid)
{
    mContext->EnforceCallingOrSelfPermission(MANIFEST_PERMISSION_MANAGE_USB, NULL);
    mSettingsManager->GrantDevicePermission(device, uid);

    return NOERROR;
}

ECode UsbService::GrantAccessoryPermission(
    /* [in] */ IUsbAccessory* accessory,
    /* [in] */ Int32 uid)
{
    mContext->EnforceCallingOrSelfPermission(MANIFEST_PERMISSION_MANAGE_USB, NULL);
    mSettingsManager->GrantAccessoryPermission(accessory, uid);

    return NOERROR;
}

ECode UsbService::HasDefaults(
    /* [in] */ const String& packageName,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    mContext->EnforceCallingOrSelfPermission(MANIFEST_PERMISSION_MANAGE_USB, NULL);
    *result = mSettingsManager->HasDefaults(packageName);

    return NOERROR;
}

ECode UsbService::ClearDefaults(
    /* [in] */ const String& packageName)
{
    mContext->EnforceCallingOrSelfPermission(MANIFEST_PERMISSION_MANAGE_USB, NULL);
    mSettingsManager->ClearDefaults(packageName);

    return NOERROR;
}

ECode UsbService::SetCurrentFunction(
    /* [in] */ const String& function,
    /* [in] */ Boolean makeDefault)
{
    mContext->EnforceCallingOrSelfPermission(MANIFEST_PERMISSION_MANAGE_USB, NULL);

    if (mDeviceManager == NULL) {
        // throw new IllegalStateException("USB device mode not supported");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    mDeviceManager->SetCurrentFunctions(function, makeDefault);
    return NOERROR;
}

ECode UsbService::SetMassStorageBackingFile(
    /* [in] */ const String& path)
{
    mContext->EnforceCallingOrSelfPermission(MANIFEST_PERMISSION_MANAGE_USB, NULL);

    if (mDeviceManager == NULL) {
        // throw new IllegalStateException("USB device mode not supported");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    mDeviceManager->SetMassStorageBackingFile(path);
    return NOERROR;
}

void UsbService::SystemReady()
{
    if (mDeviceManager != NULL) {
        mDeviceManager->SystemReady();
    }

    if (mHostManager != NULL) {
        mHostManager->SystemReady();
    }
}
