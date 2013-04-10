
#include "server/usb/AccessoryFilter.h"
#include <StringBuffer.h>

AccessoryFilter::AccessoryFilter(
    /* [in] */ const String& manufacturer,
    /* [in] */ const String& model,
    /* [in] */ const String& ver)
{
    mManufacturer = manufacturer;
    mModel = model;
    mVersion = ver;
}

AccessoryFilter::AccessoryFilter(
    /* [in] */ IUsbAccessory* accessory)
{
    accessory->GetManufacturer(&mManufacturer);
    accessory->GetModel(&mModel);
    accessory->GetVersion(&mVersion);
}

UInt32 AccessoryFilter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AccessoryFilter::Release()
{
    return ElRefBase::Release();
}

ECode AccessoryFilter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode AccessoryFilter::Read(
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ AccessoryFilter** filter)
{
    VALIDATE_NOT_NULL(filter);

    String manufacturer;
    String model;
    String version;

    Int32 count = 0;
    parser->GetAttributeCount(&count);

    for (Int32 i = 0; i < count; i++) {
        String name;
        parser->GetAttributeName(i, &name);

        String value;
        parser->GetAttributeValue(i, &value);

        if (name == "manufacturer") {
            manufacturer = value;
        } else if (name == "model") {
            model = value;
        } else if (name == "version") {
            version = value;
        }
    }

    *filter = new AccessoryFilter(manufacturer, model, version);
    return NOERROR;
}

ECode AccessoryFilter::Write(
    /* [in] */ IXmlSerializer* serializer)
{
    serializer->WriteStartTag(NULL, "usb-accessory");

    if (mManufacturer != NULL) {
        serializer->WriteAttribute(NULL, "manufacturer", mManufacturer);
    }

    if (mModel != NULL) {
        serializer->WriteAttribute(NULL, "model", mModel);
    }

    if (mVersion != NULL) {
        serializer->WriteAttribute(NULL, "version", mVersion);
    }

    serializer->WriteEndTag(NULL, "usb-accessory");
    return NOERROR;
}

Boolean AccessoryFilter::Matches(
    /* [in] */ IUsbAccessory* accessory)
{
    // 1. Check Manufacturer
    String manufacturer;
    accessory->GetManufacturer(&manufacturer);

    if (mManufacturer != NULL && manufacturer != mManufacturer) {
        return FALSE;
    }

    // 2. Check Model
    String model;
    accessory->GetModel(&model);

    if (mModel != NULL && model != mModel) {
        return FALSE;
    }

    // 3. Check Version
    String ver;
    accessory->GetVersion(&ver);

    if (mVersion != NULL && ver != mVersion) {
        return FALSE;
    }

    // 4. All Pass
    return TRUE;
}

Boolean AccessoryFilter::Matches(
    /* [in] */ AccessoryFilter* filter)
{
    if (mManufacturer != NULL && filter->mManufacturer != mManufacturer) {
        return FALSE;
    }

    if (mModel != NULL && filter->mModel != mModel) {
        return FALSE;
    }

    if (mVersion != NULL && filter->mVersion != mVersion) {
        return FALSE;
    }

    return TRUE;
}

ECode AccessoryFilter::Equals(
    /* [in] */ AccessoryFilter* filter,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // can't compare if we have wildcard strings
    if (mManufacturer.IsNull() || mModel.IsNull() || mVersion.IsNull()) {
        *result = FALSE;
        return NOERROR;
    }

    // 1. Check Manufacturer
    if (filter->mManufacturer != mManufacturer) {
        *result = FALSE;
        return NOERROR;
    }

    // 2. Check Model
    if (filter->mModel != mModel) {
        *result = FALSE;
        return NOERROR;
    }

    // 3. Check Version
    if (filter->mVersion != mVersion) {
        *result = FALSE;
        return NOERROR;
    }

    *result = TRUE;
    return NOERROR;
}

ECode AccessoryFilter::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // can't compare if we have wildcard strings
    if (mManufacturer.IsNull() || mModel.IsNull() || mVersion.IsNull()) {
        *result = FALSE;
        return NOERROR;
    }

    if (obj == NULL || IUsbAccessory::Probe(obj) == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    IUsbAccessory* accessory = (IUsbAccessory*)obj;
    String str;

    // 1. Check Manufacturer
    accessory->GetManufacturer(&str);
    if (str != mManufacturer) {
        *result = FALSE;
        return NOERROR;
    }

    // 2. Check Model
    accessory->GetModel(&str);
    if (str != mModel) {
        *result = FALSE;
        return NOERROR;
    }

    // 3. Check Version
    accessory->GetVersion(&str);
    if (str != mVersion) {
        *result = FALSE;
        return NOERROR;
    }

    *result = TRUE;
    return NOERROR;
}

ECode AccessoryFilter::HashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = ((mManufacturer == NULL ? 0 : mManufacturer.GetHashCode()) ^
              (mModel == NULL ? 0 : mModel.GetHashCode()) ^
              (mVersion == NULL ? 0 : mVersion.GetHashCode()));

    return NOERROR;
}

ECode AccessoryFilter::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuffer buf;

    buf += "AccessoryFilter[mManufacturer=\"";
    buf += mManufacturer;

    buf += "\", mModel=\"";
    buf += mModel;

    buf += "\", mVersion=\"";
    buf += mVersion;
    buf += "\"]";

    *str = (String)buf;
    return NOERROR;
}
