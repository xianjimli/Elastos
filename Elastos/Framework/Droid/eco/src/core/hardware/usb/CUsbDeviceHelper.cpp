
#include "hardware/usb/CUsbDeviceHelper.h"
#include "hardware/usb/UsbDevice.h"

ECode CUsbDeviceHelper::GetDeviceId(
    /* [in] */ const String& name,
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = UsbDevice::NativeGetDeviceId(name);
    return NOERROR;
}

ECode CUsbDeviceHelper::GetDeviceName(
    /* [in] */ Int32 id,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = UsbDevice::NativeGetDeviceName(id);
    return NOERROR;
}
