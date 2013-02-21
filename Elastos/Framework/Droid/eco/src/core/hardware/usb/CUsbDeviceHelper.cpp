
#include "hardware/usb/CUsbDeviceHelper.h"
#include "hardware/usb/UsbDevice.h"

ECode CUsbDeviceHelper::GetDeviceId(
    /* [in] */ const String& name,
    /* [out] */ Int32* id)
{
    return UsbDevice::GetDeviceId(name, id);
}

ECode CUsbDeviceHelper::GetDeviceName(
    /* [in] */ Int32 id,
    /* [out] */ String* name)
{
    return UsbDevice::GetDeviceName(id, name);
}
