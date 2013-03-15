
#ifndef __USBDEVICEMANAGER_H__
#define __USBDEVICEMANAGER_H__

#include "ext/frameworkext.h"

/**
 * UsbDeviceManager manages USB state in device mode.
 */
class UsbDeviceManager : public ElRefBase
{
public:
    UsbDeviceManager(
        /* [in] */ IContext* context,
        /* [in] */ UsbSettingsManager* settingsManager);

    CARAPI_(void) SystemReady();

    /* returns the currently attached USB accessory */
    CARAPI_(IUsbAccessory*) GetCurrentAccessory();

    /* opens the currently attached USB accessory */
    CARAPI_(IParcelFileDescriptor*) OpenAccessory(
        /* [in] */ IUsbAccessory* accessory);

    CARAPI_(void) SetCurrentFunctions(
        /* [in] */ const String& functions,
        /* [in] */ Boolean makeDefault);

    CARAPI_(void) SetMassStorageBackingFile(
        /* [in] */ const String& path);

    CARAPI_(void) Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw);

private:
};

#endif // __USBDEVICEMANAGER_H__
