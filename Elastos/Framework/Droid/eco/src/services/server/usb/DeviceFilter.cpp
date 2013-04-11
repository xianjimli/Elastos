
#include "server/usb/DeviceFilter.h"
#include <StringBuffer.h>

DeviceFilter::DeviceFilter(
    /* [in] */ Int32 vid,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 clasz,
    /* [in] */ Int32 subclass,
    /* [in] */ Int32 protocol)
{
    mVendorId = vid;
    mProductId = pid;
    mClass = clasz;
    mSubclass = subclass;
    mProtocol = protocol;
}

DeviceFilter::DeviceFilter(
    /* [in] */ IUsbDevice* device)
{
    device->GetVendorId(&mVendorId);
    device->GetProductId(&mProductId);
    device->GetDeviceClass(&mClass);
    device->GetDeviceSubclass(&mSubclass);
    device->GetDeviceProtocol(&mProtocol);
}

UInt32 DeviceFilter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 DeviceFilter::Release()
{
    return ElRefBase::Release();
}

ECode DeviceFilter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode DeviceFilter::Read(
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ DeviceFilter** filter)
{
    VALIDATE_NOT_NULL(filter);

    Int32 vendorId = -1;
    Int32 productId = -1;
    Int32 deviceClass = -1;
    Int32 deviceSubclass = -1;
    Int32 deviceProtocol = -1;

    Int32 count = 0;
    parser->GetAttributeCount(&count);

    for (Int32 i = 0; i < count; i++) {
        String name;
        parser->GetAttributeName(i, &name);

        // All attribute values are ints
        String attrValue;
        parser->GetAttributeValue(i, &attrValue);

        Int32 value = attrValue.ToInt32();

        if (name == "vendor-id") {
            vendorId = value;
        } else if (name == "product-id") {
            productId = value;
        } else if (name == "class") {
            deviceClass = value;
        } else if (name == "subclass") {
            deviceSubclass = value;
        } else if (name == "protocol") {
            deviceProtocol = value;
        }
    }

    *filter = new DeviceFilter(vendorId, productId, deviceClass, deviceSubclass, deviceProtocol);
    return NOERROR;
}

ECode DeviceFilter::Write(
    /* [in] */ IXmlSerializer* serializer)
{
    serializer->WriteStartTag(NULL, "usb-device");

    if (mVendorId != -1) {
        serializer->WriteAttribute(NULL, "vendor-id", String::FromInt32(mVendorId));
    }

    if (mProductId != -1) {
        serializer->WriteAttribute(NULL, "product-id", String::FromInt32(mProductId));
    }

    if (mClass != -1) {
        serializer->WriteAttribute(NULL, "class", String::FromInt32(mClass));
    }

    if (mSubclass != -1) {
        serializer->WriteAttribute(NULL, "subclass", String::FromInt32(mSubclass));
    }

    if (mProtocol != -1) {
        serializer->WriteAttribute(NULL, "protocol", String::FromInt32(mProtocol));
    }

    serializer->WriteEndTag(NULL, "usb-device");
    return NOERROR;
}

Boolean DeviceFilter::Matches(
    /* [in] */ IUsbDevice* device)
{
    // 1. Check Vendor ID
    Int32 vendorId;
    device->GetVendorId(&vendorId);

    if (mVendorId != -1 && vendorId != mVendorId) {
        return FALSE;
    }

    // 2. Check Product ID
    Int32 productId;
    device->GetProductId(&productId);

    if (mProductId != -1 && productId != mProductId) {
        return FALSE;
    }

    // 3. check device class/subclass/protocol
    Int32 deviceCls;
    device->GetDeviceClass(&deviceCls);

    Int32 deviceSubCls;
    device->GetDeviceSubclass(&deviceSubCls);

    Int32 deviceProtocol;
    device->GetDeviceProtocol(&deviceProtocol);

    if (Matches(deviceCls, deviceSubCls, deviceProtocol) == TRUE) {
        return TRUE;
    }

    // 4. if device doesn't match, check the interfaces
    Int32 count = 0;
    device->GetInterfaceCount(&count);

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IUsbInterface> intf;
        device->GetInterface(i, (IUsbInterface**)&intf);

        Int32 intfCls;
        intf->GetInterfaceClass(&intfCls);

        Int32 intfSubCls;
        intf->GetInterfaceSubclass(&intfSubCls);

        Int32 intfProtocol;
        intf->GetInterfaceProtocol(&intfProtocol);

        if (Matches(intfCls, intfSubCls, intfProtocol) == FALSE) {
            continue;
        }

        return TRUE;
    }

    return FALSE;
}

Boolean DeviceFilter::Matches(
    /* [in] */ DeviceFilter* filter)
{
    if (mVendorId != -1 && filter->mVendorId != mVendorId) {
        return false;
    }

    if (mProductId != -1 && filter->mProductId != mProductId) {
        return false;
    }

    // check device class/subclass/protocol
    return Matches(filter->mClass, filter->mSubclass, filter->mProtocol);
}

ECode DeviceFilter::Equals(
    /* [in] */ DeviceFilter* filter,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // can't compare if we have wildcard strings
    if (mVendorId == -1 || mProductId == -1 ||
        mClass == -1 || mSubclass == -1 || mProtocol == -1)
    {
        *result = FALSE;
        return NOERROR;
    }

    // 1. Check Vendor ID
    if (filter->mVendorId != mVendorId) {
        *result = FALSE;
        return NOERROR;
    }

    // 2. Check Product ID
    if (filter->mProductId != mProductId) {
        *result = FALSE;
        return NOERROR;
    }

    // 3. Check Class
    if (filter->mClass != mClass) {
        *result = FALSE;
        return NOERROR;
    }

    // 4. Check SubClass
    if (filter->mSubclass != mSubclass) {
        *result = FALSE;
        return NOERROR;
    }

    // 5. Check Protocol
    if (filter->mProtocol != mProtocol) {
        *result = FALSE;
        return NOERROR;
    }

    *result = TRUE;
    return NOERROR;
}

ECode DeviceFilter::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // can't compare if we have wildcard strings
    if (mVendorId == -1 || mProductId == -1 ||
        mClass == -1 || mSubclass == -1 || mProtocol == -1)
    {
        *result = FALSE;
        return NOERROR;
    }

    if (obj == NULL || IUsbDevice::Probe(obj) == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    IUsbDevice* device = (IUsbDevice*)obj;
    Int32 val;

    // 1. Check Vendor ID
    device->GetVendorId(&val);
    if (val != mVendorId) {
        *result = FALSE;
        return NOERROR;
    }

    // 2. Check Product ID
    device->GetProductId(&val);
    if (val != mProductId) {
        *result = FALSE;
        return NOERROR;
    }

    // 3. Check Class
    device->GetDeviceClass(&val);
    if (val != mClass) {
        *result = FALSE;
        return NOERROR;
    }

    // 4. Check SubClass
    device->GetDeviceSubclass(&val);
    if (val != mSubclass) {
        *result = FALSE;
        return NOERROR;
    }

    // 5. Check Protocol
    device->GetDeviceProtocol(&val);
    if (val != mProtocol) {
        *result = FALSE;
        return NOERROR;
    }

    *result = TRUE;
    return NOERROR;
}

ECode DeviceFilter::HashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = (((mVendorId << 16) | mProductId) ^
              ((mClass << 16) | (mSubclass << 8) | mProtocol));

    return NOERROR;
}

ECode DeviceFilter::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuffer buf;

    buf += "DeviceFilter[mVendorId=";
    buf += mVendorId;

    buf += ",mProductId=";
    buf += mProductId;

    buf += ",mClass=";
    buf += mClass;

    buf += ",mSubclass=";
    buf += mSubclass;

    buf += ",mProtocol=";
    buf += mProtocol;
    buf += "]";

    *str = (String)buf;
    return NOERROR;
}

Boolean DeviceFilter::Matches(
    /* [in] */ Int32 clasz,
    /* [in] */ Int32 subclass,
    /* [in] */ Int32 protocol)
{
    return ((mClass == -1 || clasz == mClass) &&
            (mSubclass == -1 || subclass == mSubclass) &&
            (mProtocol == -1 || protocol == mProtocol));
}
