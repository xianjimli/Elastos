
#ifndef __USBSETTINGSMANAGER_H__
#define __USBSETTINGSMANAGER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>

class UsbSettingsManager : public ElRefBase
{
public:
    UsbSettingsManager(
        /* [in] */ IContext* context);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI_(void) DeviceAttached(
        /* [in] */ IUsbDevice* device);

    CARAPI_(void) DeviceDetached(
        /* [in] */ IUsbDevice* device);

    CARAPI_(void) AccessoryAttached(
        /* [in] */ IUsbAccessory* accessory);

    CARAPI_(void) AccessoryDetached(
        /* [in] */ IUsbAccessory* accessory);

    CARAPI_(Boolean) HasDevicePermission(
        /* [in] */ IUsbDevice* device);

    CARAPI_(Boolean) HasAccessoryPermission(
        /* [in] */ IUsbAccessory* accessory);

    CARAPI_(void) CheckDevicePermission(
        /* [in] */ IUsbDevice* device);

    CARAPI_(void) CheckAccessoryPermission(
        /* [in] */ IUsbAccessory* accessory);

    CARAPI_(void) RequestDevicePermission(
        /* [in] */ IUsbDevice* device,
        /* [in] */ const String& packageName,
        /* [in] */ IPendingIntent* pi);

    CARAPI_(void) RequestAccessoryPermission(
        /* [in] */ IUsbAccessory* accessory,
        /* [in] */ const String& packageName,
        /* [in] */ IPendingIntent* pi);

    CARAPI_(void) SetDevicePackage(
        /* [in] */ IUsbDevice* device,
        /* [in] */ const String& packageName);

    CARAPI_(void) SetAccessoryPackage(
        /* [in] */ IUsbAccessory* accessory,
        /* [in] */ const String& packageName);

    CARAPI_(void) GrantDevicePermission(
        /* [in] */ IUsbDevice* device,
        /* [in] */ Int32 uid);

    CARAPI_(void) GrantAccessoryPermission(
        /* [in] */ IUsbAccessory* accessory,
        /* [in] */ Int32 uid);

    CARAPI_(Boolean) HasDefaults(
        /* [in] */ const String& packageName);

    CARAPI_(void) ClearDefaults(
        /* [in] */ const String& packageName);

    CARAPI_(void) Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw);

private:
};

#endif // __USBSETTINGSMANAGER_H__
