
#ifndef __USBDEVICE_H__
#define __USBDEVICE_H__

#include "ext/frameworkext.h"

class UsbDevice
{
public:
    static CARAPI_(Int32) NativeGetDeviceId(
        /* [in] */ const String& name);

    static CARAPI_(String) NativeGetDeviceName(
        /* [in] */ Int32 id);
};

#endif // __USBDEVICE_H__
