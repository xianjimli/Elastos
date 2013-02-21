
#include "hardware/usb/CUsbDevice.h"
#include "hardware/usb/UsbDevice.h"
#include <elastos/System.h>
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>

using namespace Elastos::Core;

ECode CUsbDevice::constructor(
    /* [in] */ const String& name,
    /* [in] */ Int32 vendorId,
    /* [in] */ Int32 productId,
    /* [in] */ Int32 cls,
    /* [in] */ Int32 subCls,
    /* [in] */ Int32 protocol,
    /* [in] */ const ArrayOf<IParcelable*>& interfaces)
{
    mName = name;
    mVendorId = vendorId;
    mProductId = productId;
    mClass = cls;
    mSubclass = subCls;
    mProtocol = protocol;
    mInterfaces = interfaces.Clone();

    return NOERROR;
}

ECode CUsbDevice::GetDeviceName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;

    return NOERROR;
}

ECode CUsbDevice::GetDeviceId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    UsbDevice::GetDeviceId(mName, id);

    return NOERROR;
}

ECode CUsbDevice::GetVendorId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mVendorId;

    return NOERROR;
}

ECode CUsbDevice::GetProductId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mProductId;

    return NOERROR;
}

ECode CUsbDevice::GetDeviceClass(
    /* [out] */ Int32* cls)
{
    VALIDATE_NOT_NULL(cls);
    *cls = mClass;

    return NOERROR;
}

ECode CUsbDevice::GetDeviceSubclass(
    /* [out] */ Int32* cls)
{
    VALIDATE_NOT_NULL(cls);
    *cls = mSubclass;

    return NOERROR;
}

ECode CUsbDevice::GetDeviceProtocol(
    /* [out] */ Int32* protocol)
{
    VALIDATE_NOT_NULL(protocol);
    *protocol = mProtocol;

    return NOERROR;
}

ECode CUsbDevice::GetInterfaceCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mInterfaces->GetLength();

    return NOERROR;
}

ECode CUsbDevice::GetInterface(
    /* [out] */ Int32 index,
    /* [out] */ IUsbInterface** usbInterface)
{
    VALIDATE_NOT_NULL(usbInterface);
    *usbInterface = (IUsbInterface*)((*mInterfaces)[index]);

    return NOERROR;
}

ECode CUsbDevice::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    /*
    if (o instanceof UsbDevice) {
        return ((UsbDevice)o).mName.equals(mName);
    } else if (o instanceof String) {
        return ((String)o).equals(mName);
    } else {
        return false;
    }
    //*/

    VALIDATE_NOT_NULL(result);

    if (obj == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    if (IUsbDevice::Probe(obj) != NULL) {
        CUsbDevice* device = (CUsbDevice*)IUsbDevice::Probe(obj);
        *result = device->mName.Equals(mName);
        return NOERROR;
    }

    *result = FALSE;
    return NOERROR;
}

ECode CUsbDevice::HashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mName.GetHashCode();

    return NOERROR;
}

ECode CUsbDevice::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuffer buf;
    buf += "UsbDevice[mName=";
    buf += mName;

    buf += ",mVendorId=";
    buf += mVendorId;

    buf += ",mProductId=";
    buf += mProductId;

    buf += ",mClass=";
    buf += mClass;

    buf += ",mSubclass=";
    buf += mSubclass;

    buf += ",mProtocol=";
    buf += mProtocol;

    buf += ",mInterfaces=";
    buf += mInterfaces;
    buf += "]";

    *str = (String)buf;

    return NOERROR;
}

ECode CUsbDevice::DescribeContents(
    /* [out] */ Int32* contents)
{
    VALIDATE_NOT_NULL(contents);
    *contents = 0;

    return NOERROR;
}

ECode CUsbDevice::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    String name;
    Int32 vendorId, productId, cls, subCls, protocol;

    source->ReadString(&name);
    source->ReadInt32(&vendorId);
    source->ReadInt32(&productId);
    source->ReadInt32(&cls);
    source->ReadInt32(&subCls);
    source->ReadInt32(&protocol);

    // Parcelable[] interfaces = in.readParcelableArray(UsbInterface.class.getClassLoader());
    ArrayOf<IParcelable*>* interfaces;

    AutoPtr<CUsbDevice> device;
    CUsbDevice::NewByFriend(name, vendorId, productId, cls, subCls, protocol, *interfaces, (CUsbDevice**)&device);

    return NOERROR;
}

ECode CUsbDevice::WriteToParcel(
    /* [out] */ IParcel* dest)
{
    dest->WriteString(mName);
    dest->WriteInt32(mVendorId);
    dest->WriteInt32(mProductId);
    dest->WriteInt32(mClass);
    dest->WriteInt32(mSubclass);
    dest->WriteInt32(mProtocol);
    // parcel.writeParcelableArray(mInterfaces, 0);
    
    return NOERROR;
}
