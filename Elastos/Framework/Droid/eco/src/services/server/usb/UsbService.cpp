
#include "server/usb/UsbService.h"

using namespace Elastos;

UsbService::UsbService(
    /* [in] */ IContext* context)
{
    // NOT IMPLEMENTED
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

/* Returns a list of all currently attached USB devices */
ECode UsbService::GetDeviceList(
    /* [out] */ IBundle** devices)
{
    VALIDATE_NOT_NULL(devices);
    // NOT IMPLEMENTED
    return NOERROR;
}

/* Returns a file descriptor for communicating with the USB device.
 * The native fd can be passed to usb_device_new() in libusbhost.
 */
ECode UsbService::OpenDevice(
    /* [in] */ const String& deviceName,
    /* [out] */ IParcelFileDescriptor** descriptor)
{
    VALIDATE_NOT_NULL(descriptor);
    // NOT IMPLEMENTED
    return NOERROR;
}

/* Returns the currently attached USB accessory */
ECode UsbService::GetCurrentAccessory(
    /* [out] */ IUsbAccessory** accessory)
{
    VALIDATE_NOT_NULL(accessory);
    // NOT IMPLEMENTED
    return NOERROR;
}

/* Returns a file descriptor for communicating with the USB accessory.
 * This file descriptor can be used with standard Java file operations.
 */
ECode UsbService::OpenAccessory(
    /* [in] */ IUsbAccessory* accessory,
    /* [out] */ IParcelFileDescriptor** descriptor)
{
    VALIDATE_NOT_NULL(descriptor);
    // NOT IMPLEMENTED
    return NOERROR;
}

/* Sets the default package for a USB device
 * (or clears it if the package name is null)
 */
ECode UsbService::SetDevicePackage(
    /* [in] */ IUsbDevice* device,
    /* [in] */ const String& packageName)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

/* Sets the default package for a USB accessory
 * (or clears it if the package name is null)
 */
ECode UsbService::SetAccessoryPackage(
    /* [in] */ IUsbAccessory* accessory,
    /* [in] */ const String& packageName)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

/* Returns true if the caller has permission to access the device. */
ECode UsbService::HasDevicePermission(
    /* [in] */ IUsbDevice* device,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // NOT IMPLEMENTED
    return NOERROR;
}

/* Returns true if the caller has permission to access the accessory. */
ECode UsbService::HasAccessoryPermission(
    /* [in] */ IUsbAccessory* accessory,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // NOT IMPLEMENTED
    return NOERROR;
}

/* Requests permission for the given package to access the device.
 * Will display a system dialog to query the user if permission
 * had not already been given.
 */
ECode UsbService::RequestDevicePermission(
    /* [in] */ IUsbDevice* device,
    /* [in] */ const String& packageName,
    /* [in] */ IPendingIntent* pi)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

/* Requests permission for the given package to access the accessory.
 * Will display a system dialog to query the user if permission
 * had not already been given. Result is returned via pi.
 */
ECode UsbService::RequestAccessoryPermission(
    /* [in] */ IUsbAccessory* accessory,
    /* [in] */ const String& packageName,
    /* [in] */ IPendingIntent* pi)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

/* Grants permission for the given UID to access the device */
ECode UsbService::GrantDevicePermission(
    /* [in] */ IUsbDevice* device,
    /* [in] */ Int32 uid)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

/* Grants permission for the given UID to access the accessory */
ECode UsbService::GrantAccessoryPermission(
    /* [in] */ IUsbAccessory* accessory,
    /* [in] */ Int32 uid)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

/* Returns true if the USB manager has default preferences or permissions for the package */
ECode UsbService::HasDefaults(
    /* [in] */ const String& packageName,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // NOT IMPLEMENTED
    return NOERROR;
}

/* Clears default preferences and permissions for the package */
ECode UsbService::ClearDefaults(
    /* [in] */ const String& packageName)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

/* Sets the current USB function. */
ECode UsbService::SetCurrentFunction(
    /* [in] */ const String& function,
    /* [in] */ Boolean makeDefault)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

/* Sets the file path for USB mass storage backing file. */
ECode UsbService::SetMassStorageBackingFile(
    /* [in] */ const String* path)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

void UsbService::SystemReady()
{
    // NOT IMPLEMENTED
}
