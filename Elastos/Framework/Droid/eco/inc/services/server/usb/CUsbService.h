
#ifndef __CUSBSERVICE_H__
#define __CUSBSERVICE_H__

#include "ext/frameworkext.h"
#include "_CUsbService.h"

/**
 * UsbService manages all USB related state, including both host and device support.
 * Host related events and calls are delegated to UsbHostManager, and device related
 * support is delegated to UsbDeviceManager.
 */
CarClass(CUsbService)
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI SystemReady();

    /* Returns a list of all currently attached USB devices (host mdoe) */
    CARAPI GetDeviceList(
        /* [in] */ IBundle* devices);

    /* Opens the specified USB device (host mode) */
    CARAPI OpenDevice(
        /* [in] */ const String& deviceName,
        /* [out] */ IParcelFileDescriptor** pfd);

    /* returns the currently attached USB accessory (device mode) */
    CARAPI GetCurrentAccessory(
        /* [out] */ IUsbAccessory** accessory);

    /* opens the currently attached USB accessory (device mode) */
    CARAPI OpenAccessory(
        /* [in] */ IUsbAccessory* accessory,
        /* [out] */ IParcelFileDescriptor** pfd);

    CARAPI SetDevicePackage(
        /* [in] */ IUsbDevice* device,
        /* [in] */ const String& packageName);

    CARAPI SetAccessoryPackage(
        /* [in] */ IUsbAccessory* accessory,
        /* [in] */ const String& packageName);

    CARAPI HasDevicePermission(
        /* [in] */ IUsbDevice* device,
        /* [out] */ Boolean* result);

    CARAPI HasAccessoryPermission(
        /* [in] */ IUsbAccessory* accessory,
        /* [out] */ Boolean* result);

    CARAPI RequestDevicePermission(
        /* [in] */ IUsbDevice* device,
        /* [in] */ const String& packageName,
        /* [in] */ IPendingIntent* pi);

    CARAPI RequestAccessoryPermission(
        /* [in] */ IUsbAccessory* accessory,
        /* [in] */ const String& packageName,
        /* [in] */ IPendingIntent* pi);

    CARAPI GrantDevicePermission(
        /* [in] */ IUsbDevice* device,
        /* [in] */ Int32 uid);

    CARAPI GrantAccessoryPermission(
        /* [in] */ IUsbAccessory* accessory,
        /* [in] */ Int32 uid);

    CARAPI HasDefaults(
        /* [in] */ const String& packageName,
        /* [out] */ Boolean* result);

    CARAPI ClearDefaults(
        /* [in] */ const String& packageName);

    CARAPI SetCurrentFunction(
        /* [in] */ const String& function,
        /* [in] */ Boolean makeDefault);

    CARAPI SetMassStorageBackingFile(
        /* [in] */ const String& path);

private:
};

#endif // __CUSBSERVICE_H__
