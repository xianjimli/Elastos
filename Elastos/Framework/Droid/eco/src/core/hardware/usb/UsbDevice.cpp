
#include "hardware/usb/UsbDevice.h"
/* #include <usbhost/usbhost.h> */

Int32 UsbDevice::NativeGetDeviceId(
    /* [in] */ const String& name)
{
    /*
    const char *nameStr = name.string();
    return usb_device_get_unique_id_from_name(nameStr);
    */
    // NOT IMPLEMENTED
    return -1;
}

String UsbDevice::NativeGetDeviceName(
    /* [in] */ Int32 id)
{
    /*
    char* name = usb_device_get_name_from_unique_id(id);
    String result = String(name);
    free(name);
    return result;
    */
    // NOT IMPLEMENTED
    return String("");
}
