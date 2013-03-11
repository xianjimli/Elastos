
#include "hardware/usb/CUsbEndpoint.h"
#include <elastos/System.h>
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>

using namespace Elastos::Core;

ECode CUsbEndpoint::constructor(
        /* [in] */ Int32 address,
        /* [in] */ Int32 attributes,
        /* [in] */ Int32 size,
        /* [in] */ Int32 interval)
{
    mAddress = address;
    mAttributes = attributes;
    mMaxPacketSize = size;
    mInterval = interval;

    return NOERROR;
}

ECode CUsbEndpoint::GetAddress(
    /* [out] */ Int32* address)
{
    VALIDATE_NOT_NULL(address);
    *address = mAddress;

    return NOERROR;
}

ECode CUsbEndpoint::GetEndpointNumber(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = mAddress & UsbConstants_USB_ENDPOINT_NUMBER_MASK;

    return NOERROR;
}

ECode CUsbEndpoint::GetDirection(
    /* [out] */ Int32* direction)
{
    VALIDATE_NOT_NULL(direction);
    *direction = mAddress & UsbConstants_USB_ENDPOINT_DIR_MASK;

    return NOERROR;
}

ECode CUsbEndpoint::GetAttributes(
    /* [out] */ Int32* attributes)
{
    VALIDATE_NOT_NULL(attributes);
    *attributes = mAttributes;

    return NOERROR;
}

ECode CUsbEndpoint::GetType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mAttributes & UsbConstants_USB_ENDPOINT_XFERTYPE_MASK;

    return NOERROR;
}

ECode CUsbEndpoint::GetMaxPacketSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    *size = mMaxPacketSize;

    return NOERROR;
}

ECode CUsbEndpoint::GetInterval(
    /* [out] */ Int32* interval)
{
    VALIDATE_NOT_NULL(interval);
    *interval = mInterval;

    return NOERROR;
}

ECode CUsbEndpoint::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuffer buf;
    buf += "UsbEndpoint[mAddress=";
    buf += mAddress;

    buf += ",mAttributes=";
    buf += mAttributes;

    buf += ",mMaxPacketSize=";
    buf += mMaxPacketSize;

    buf += ",mInterval=";
    buf += mInterval;
    buf += "]";

    *str = (String)buf;

    return NOERROR;
}

ECode CUsbEndpoint::DescribeContents(
    /* [out] */ Int32* contents)
{
    VALIDATE_NOT_NULL(contents);
    *contents = 0;

    return NOERROR;
}

ECode CUsbEndpoint::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    Int32 address, attributes, size, interval;

    source->ReadInt32(&address);
    source->ReadInt32(&attributes);
    source->ReadInt32(&size);
    source->ReadInt32(&interval);

    AutoPtr<CUsbEndpoint> endpoint;
    CUsbEndpoint::NewByFriend(address, attributes, size, interval, (CUsbEndpoint**)&endpoint);

    return NOERROR;
}

ECode CUsbEndpoint::WriteToParcel(
    /* [out] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);

    dest->WriteInt32(mAddress);
    dest->WriteInt32(mAttributes);
    dest->WriteInt32(mMaxPacketSize);
    dest->WriteInt32(mInterval);

    return NOERROR;
}
