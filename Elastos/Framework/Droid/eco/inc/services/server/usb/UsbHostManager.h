
#ifndef __USBHOSTMANAGER_H__
#define __USBHOSTMANAGER_H__

#include "ext/frameworkext.h"

/**
 * UsbHostManager manages USB state in host mode.
 */
class UsbHostManager : public ElRefBase
{
public:
    UsbHostManager(
        /* [in] */ IContext* context,
        /* [in] */ UsbSettingsManager* settingsManager);

    CARAPI_(void) SystemReady();

    /* Returns a list of all currently attached USB devices */
    CARAPI_(void) GetDeviceList(
        /* [in] */ IBundle* devices);

    /* Opens the specified USB device */
    CARAPI_(IParcelFileDescriptor*) OpenDevice(
        /* [in] */ const String& deviceName);

    CARAPI_(void) Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw);

private:
};

#endif // __USBHOSTMANAGER_H__
