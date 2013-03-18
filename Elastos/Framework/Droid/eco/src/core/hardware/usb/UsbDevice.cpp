
#include "hardware/usb/UsbDevice.h"
#include <usbhost/usbhost.h>

Int32 UsbDevice::NativeGetDeviceId(
    /* [in] */ const String& name)
{
    return usb_device_get_unique_id_from_name(name.string());
}

String UsbDevice::NativeGetDeviceName(
    /* [in] */ Int32 id)
{
    char* name = usb_device_get_name_from_unique_id(id);
    String deviceName = String(name);
    free(name);
    return deviceName;
}
