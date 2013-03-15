
#ifndef __CUSBSERVICE_H__
#define __CUSBSERVICE_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include "server/usb/UsbSettingsManager.h"
#include "server/usb/UsbDeviceManager.h"
#include "server/usb/UsbHostManager.h"

/**
 * UsbService manages all USB related state, including both host and device support.
 * Host related events and calls are delegated to UsbHostManager, and device related
 * support is delegated to UsbDeviceManager.
 */
class UsbService : public ElRefBase, public IUsbManager
{
public:
    UsbService(
        /* [in] */ IContext* context);

    /* Returns a list of all currently attached USB devices */
    CARAPI GetDeviceList(
        /* [out] */ IBundle** devices);

    /* Returns a file descriptor for communicating with the USB device.
     * The native fd can be passed to usb_device_new() in libusbhost.
     */
    CARAPI OpenDevice(
        /* [in] */ const String& deviceName,
        /* [out] */ IParcelFileDescriptor** descriptor);

    /* Returns the currently attached USB accessory */
    CARAPI GetCurrentAccessory(
        /* [out] */ IUsbAccessory** accessory);

    /* Returns a file descriptor for communicating with the USB accessory.
     * This file descriptor can be used with standard Java file operations.
     */
    CARAPI OpenAccessory(
        /* [in] */ IUsbAccessory* accessory,
        /* [out] */ IParcelFileDescriptor** descriptor);

    /* Sets the default package for a USB device
     * (or clears it if the package name is null)
     */
    CARAPI SetDevicePackage(
        /* [in] */ IUsbDevice* device,
        /* [in] */ const String& packageName);

    /* Sets the default package for a USB accessory
     * (or clears it if the package name is null)
     */
    CARAPI SetAccessoryPackage(
        /* [in] */ IUsbAccessory* accessory,
        /* [in] */ const String& packageName);

    /* Returns true if the caller has permission to access the device. */
    CARAPI HasDevicePermission(
        /* [in] */ IUsbDevice* device,
        /* [out] */ Boolean* result);

    /* Returns true if the caller has permission to access the accessory. */
    CARAPI HasAccessoryPermission(
        /* [in] */ IUsbAccessory* accessory,
        /* [out] */ Boolean* result);

    /* Requests permission for the given package to access the device.
     * Will display a system dialog to query the user if permission
     * had not already been given.
     */
    CARAPI RequestDevicePermission(
        /* [in] */ IUsbDevice* device,
        /* [in] */ const String& packageName,
        /* [in] */ IPendingIntent* pi);

    /* Requests permission for the given package to access the accessory.
     * Will display a system dialog to query the user if permission
     * had not already been given. Result is returned via pi.
     */
    CARAPI RequestAccessoryPermission(
        /* [in] */ IUsbAccessory* accessory,
        /* [in] */ const String& packageName,
        /* [in] */ IPendingIntent* pi);

    /* Grants permission for the given UID to access the device */
    CARAPI GrantDevicePermission(
        /* [in] */ IUsbDevice* device,
        /* [in] */ Int32 uid);

    /* Grants permission for the given UID to access the accessory */
    CARAPI GrantAccessoryPermission(
        /* [in] */ IUsbAccessory* accessory,
        /* [in] */ Int32 uid);

    /* Returns true if the USB manager has default preferences or permissions for the package */
    CARAPI HasDefaults(
        /* [in] */ const String& packageName,
        /* [out] */ Boolean* result);

    /* Clears default preferences and permissions for the package */
    CARAPI ClearDefaults(
        /* [in] */ const String& packageName);

    /* Sets the current USB function. */
    CARAPI SetCurrentFunction(
        /* [in] */ const String& function,
        /* [in] */ Boolean makeDefault);

    /* Sets the file path for USB mass storage backing file. */
    CARAPI SetMassStorageBackingFile(
        /* [in] */ const String* path);

    CARAPI_(void) SystemReady();

private:
    AutoPtr<IContext> mContext;
    AutoPtr<UsbDeviceManager> mDeviceManager;
    AutoPtr<UsbHostManager> mHostManager;
    AutoPtr<UsbSettingsManager> mSettingsManager;
};

#endif // __CUSBSERVICE_H__
