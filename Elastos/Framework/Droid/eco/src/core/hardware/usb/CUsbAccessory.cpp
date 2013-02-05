
#include "hardware/usb/CUsbAccessory.h"
#include <elastos/System.h>
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>

using namespace Elastos::Core;

const String CUsbAccessory::TAG("UsbAccessory");

ECode CUsbAccessory::constructor(
    /* [in] */ const String& manufacturer,
    /* [in] */ const String& model,
    /* [in] */ const String& description,
    /* [in] */ const String& ver,
    /* [in] */ const String& uri,
    /* [in] */ const String& serial)
{
    mManufacturer = manufacturer;
    mModel = model;
    mDescription = description;
    mVersion = ver;
    mUri = uri;
    mSerial = serial;

    return NOERROR;
}

ECode CUsbAccessory::constructor(
    /* [in] */ const ArrayOf<String>& strings)
{
    mManufacturer = strings[0];
    mModel = strings[1];
    mDescription = strings[2];
    mVersion = strings[3];
    mUri = strings[4];
    mSerial = strings[5];

    return NOERROR;
}

ECode CUsbAccessory::GetManufacturer(
    /* [out] */ String* manufacturer)
{
    VALIDATE_NOT_NULL(manufacturer);
    *manufacturer = mManufacturer;

    return NOERROR;
}

ECode CUsbAccessory::GetModel(
    /* [out] */ String* model)
{
    VALIDATE_NOT_NULL(model);
    *model = mModel;

    return NOERROR;
}

ECode CUsbAccessory::GetDescription(
    /* [out] */ String* description)
{
    VALIDATE_NOT_NULL(description);
    *description = mDescription;

    return NOERROR;
}

ECode CUsbAccessory::GetVersion(
    /* [out] */ String* ver)
{
    VALIDATE_NOT_NULL(ver);
    *ver = mVersion;

    return NOERROR;
}

ECode CUsbAccessory::GetUri(
    /* [out] */ String* uri)
{
    VALIDATE_NOT_NULL(uri);
    *uri = mUri;

    return NOERROR;
}

ECode CUsbAccessory::GetSerial(
    /* [out] */ String* serial)
{
    VALIDATE_NOT_NULL(serial);
    *serial = mSerial;

    return NOERROR;
}

ECode CUsbAccessory::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (obj == NULL || IUsbAccessory::Probe(obj) == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    CUsbAccessory* accessory = (CUsbAccessory*)IUsbAccessory::Probe(obj);
    String str;

    accessory->GetManufacturer(&str);
    if (!Compare(mManufacturer, str)) {
        *result = FALSE;
        return NOERROR;
    }

    accessory->GetModel(&str);
    if (!Compare(mModel, str)) {
        *result = FALSE;
        return NOERROR;
    }

    accessory->GetDescription(&str);
    if (!Compare(mDescription, str)) {
        *result = FALSE;
        return NOERROR;
    }

    accessory->GetVersion(&str);
    if (!Compare(mVersion, str)) {
        *result = FALSE;
        return NOERROR;
    }

    accessory->GetUri(&str);
    if (!Compare(mUri, str)) {
        *result = FALSE;
        return NOERROR;
    }

    accessory->GetSerial(&str);
    if (!Compare(mSerial, str)) {
        *result = FALSE;
        return NOERROR;
    }

    *result = TRUE;
    return NOERROR;
}

ECode CUsbAccessory::HashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 hash;
    hash = (mManufacturer == NULL ? 0 : mManufacturer.GetHashCode()) ^
           (mModel == NULL ? 0 : mModel.GetHashCode()) ^
           (mDescription == NULL ? 0 : mDescription.GetHashCode()) ^
           (mVersion == NULL ? 0 : mVersion.GetHashCode()) ^
           (mUri == NULL ? 0 : mUri.GetHashCode()) ^
           (mSerial == NULL ? 0 : mSerial.GetHashCode());

    *value = hash;

    return NOERROR;
}

ECode CUsbAccessory::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuffer buf;
    buf += "UsbAccessory[mManufacturer=";
    buf += mManufacturer;

    buf += ", mModel=";
    buf += mModel;

    buf += ", mDescription=";
    buf += mDescription;

    buf += ", mVersion=";
    buf += mVersion;

    buf += ", mUri=";
    buf += mUri;

    buf += ", mSerial=";
    buf += mSerial;
    buf += "]";

    *str = (String)buf;

    return NOERROR;
}

ECode CUsbAccessory::DescribeContents(
    /* [out] */ Int32* contents)
{
    VALIDATE_NOT_NULL(contents);
    *contents = 0;

    return NOERROR;
}

ECode CUsbAccessory::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    String manufacturer, model, description, ver, uri, serial;

    source->ReadString(&manufacturer);
    source->ReadString(&model);
    source->ReadString(&description);
    source->ReadString(&ver);
    source->ReadString(&uri);
    source->ReadString(&serial);

    AutoPtr<CUsbAccessory> accessory;
    CUsbAccessory::NewByFriend(manufacturer, model, description, ver, uri, serial, (CUsbAccessory**)&accessory);

    return NOERROR;
}

ECode CUsbAccessory::WriteToParcel(
    /* [out] */ IParcel* dest)
{
    dest->WriteString(mManufacturer);
    dest->WriteString(mModel);
    dest->WriteString(mDescription);
    dest->WriteString(mVersion);
    dest->WriteString(mUri);
    dest->WriteString(mSerial);

    return NOERROR;
}

Boolean CUsbAccessory::Compare(
    /* [in] */ const String& s1,
    /* [in] */ const String& s2)
{
    if (s1 == NULL) {
        return (s2 == NULL);
    }

    return s1.Equals(s2);
}
