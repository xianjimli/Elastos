
#include "server/usb/CUsbService.h"

using namespace Elastos;

ECode CUsbService::constructor(
    /* [in] */ IContext* context)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

ECode CUsbService::SystemReady()
{
    // NOT IMPLEMENTED
    return NOERROR;
}

/* Returns a list of all currently attached USB devices (host mdoe) */
ECode CUsbService::GetDeviceList(
    /* [in] */ IBundle* devices)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

/* Opens the specified USB device (host mode) */
ECode CUsbService::OpenDevice(
    /* [in] */ const String& deviceName,
    /* [out] */ IParcelFileDescriptor** pfd)
{
    //VALIDATE_NOT_NULL(pfd);
    // NOT IMPLEMENTED
    return NOERROR;
}

/* returns the currently attached USB accessory (device mode) */
ECode CUsbService::GetCurrentAccessory(
    /* [out] */ IUsbAccessory** accessory)
{
    //VALIDATE_NOT_NULL(accessory);
    // NOT IMPLEMENTED
    return NOERROR;
}

/* opens the currently attached USB accessory (device mode) */
ECode CUsbService::OpenAccessory(
    /* [in] */ IUsbAccessory* accessory,
    /* [out] */ IParcelFileDescriptor** pfd)
{
    //VALIDATE_NOT_NULL(pfd);
    // NOT IMPLEMENTED
    return NOERROR;
}

ECode CUsbService::SetDevicePackage(
    /* [in] */ IUsbDevice* device,
    /* [in] */ const String& packageName)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

ECode CUsbService::SetAccessoryPackage(
    /* [in] */ IUsbAccessory* accessory,
    /* [in] */ const String& packageName)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

ECode CUsbService::HasDevicePermission(
    /* [in] */ IUsbDevice* device,
    /* [out] */ Boolean* result)
{
    //VALIDATE_NOT_NULL(result);
    // NOT IMPLEMENTED
    return NOERROR;
}

ECode CUsbService::HasAccessoryPermission(
    /* [in] */ IUsbAccessory* accessory,
    /* [out] */ Boolean* result)
{
    //VALIDATE_NOT_NULL(result);
    // NOT IMPLEMENTED
    return NOERROR;
}

ECode CUsbService::RequestDevicePermission(
    /* [in] */ IUsbDevice* device,
    /* [in] */ const String& packageName,
    /* [in] */ IPendingIntent* pi)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

ECode CUsbService::RequestAccessoryPermission(
    /* [in] */ IUsbAccessory* accessory,
    /* [in] */ const String& packageName,
    /* [in] */ IPendingIntent* pi)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

ECode CUsbService::GrantDevicePermission(
    /* [in] */ IUsbDevice* device,
    /* [in] */ Int32 uid)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

ECode CUsbService::GrantAccessoryPermission(
    /* [in] */ IUsbAccessory* accessory,
    /* [in] */ Int32 uid)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

ECode CUsbService::HasDefaults(
    /* [in] */ const String& packageName,
    /* [out] */ Boolean* result)
{
    //VALIDATE_NOT_NULL(result);
    // NOT IMPLEMENTED
    return NOERROR;
}

ECode CUsbService::ClearDefaults(
    /* [in] */ const String& packageName)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

ECode CUsbService::SetCurrentFunction(
    /* [in] */ const String& function,
    /* [in] */ Boolean makeDefault)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

ECode CUsbService::SetMassStorageBackingFile(
    /* [in] */ const String& path)
{
    // NOT IMPLEMENTED
    return NOERROR;
}
