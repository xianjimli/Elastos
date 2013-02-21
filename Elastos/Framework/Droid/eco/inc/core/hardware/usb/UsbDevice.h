
#ifndef __USBDEVICE_H__
#define __USBDEVICE_H__

#include "ext/frameworkext.h"

class UsbDevice
{
public:
    static CARAPI GetDeviceId(
        /* [in] */ const String& name,
        /* [out] */ Int32* id);

    static CARAPI GetDeviceName(
        /* [in] */ Int32 id,
        /* [out] */ String* name);
};

#endif // __USBDEVICE_H__
