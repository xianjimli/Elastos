
#include "server/usb/UsbSettingsManager.h"

const Boolean UsbSettingsManager::DEBUG = FALSE;

const String UsbSettingsManager::TAG = String("UsbSettingsManager");
const String UsbSettingsManager::SETTINGS_FILE_PATH = String("/data/system/usb_device_manager.xml");
const String UsbSettingsManager::USB_AUDIO_SOURCE_PATH = String("/proc/asound/usb_audio_info");

UsbSettingsManager::DeviceFilter::DeviceFilter(
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

UsbSettingsManager::DeviceFilter::DeviceFilter(
    /* [in] */ IUsbDevice* device)
{
    device->GetVendorId(&mVendorId);
    device->GetProductId(&mProductId);
    device->GetDeviceClass(&mClass);
    device->GetDeviceSubclass(&mSubclass);
    device->GetDeviceProtocol(&mProtocol);
}

UInt32 UsbSettingsManager::DeviceFilter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 UsbSettingsManager::DeviceFilter::Release()
{
    return ElRefBase::Release();
}

ECode UsbSettingsManager::DeviceFilter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode UsbSettingsManager::DeviceFilter::Read(
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

ECode UsbSettingsManager::DeviceFilter::Write(
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

Boolean UsbSettingsManager::DeviceFilter::Matches(
    /* [in] */ IUsbDevice* device)
{
    // NOT IMPLEMENTED
    return FALSE;
}

Boolean UsbSettingsManager::DeviceFilter::Matches(
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

ECode UsbSettingsManager::DeviceFilter::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // NOT IMPLEMENTED
    return NOERROR;
}

ECode UsbSettingsManager::DeviceFilter::HashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = (((mVendorId << 16) | mProductId) ^
              ((mClass << 16) | (mSubclass << 8) | mProtocol));

    return NOERROR;
}

ECode UsbSettingsManager::DeviceFilter::ToString(
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

Boolean UsbSettingsManager::DeviceFilter::Matches(
    /* [in] */ Int32 clasz,
    /* [in] */ Int32 subclass,
    /* [in] */ Int32 protocol)
{
    return ((mClass == -1 || clasz == mClass) &&
            (mSubclass == -1 || subclass == mSubclass) &&
            (mProtocol == -1 || protocol == mProtocol));
}

UsbSettingsManager::AccessoryFilter::AccessoryFilter(
    /* [in] */ const String& manufacturer,
    /* [in] */ const String& model,
    /* [in] */ const String& ver)
{
    mManufacturer = manufacturer;
    mModel = model;
    mVersion = ver;
}

UsbSettingsManager::AccessoryFilter::AccessoryFilter(
    /* [in] */ IUsbAccessory* accessory)
{
    accessory->GetManufacturer(&mManufacturer);
    accessory->GetModel(&mModel);
    accessory->GetVersion(&mVersion);
}

UInt32 UsbSettingsManager::AccessoryFilter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 UsbSettingsManager::AccessoryFilter::Release()
{
    return ElRefBase::Release();
}

ECode UsbSettingsManager::AccessoryFilter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode UsbSettingsManager::AccessoryFilter::Read(
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

ECode UsbSettingsManager::AccessoryFilter::Write(
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

Boolean UsbSettingsManager::AccessoryFilter::Matches(
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

Boolean UsbSettingsManager::AccessoryFilter::Matches(
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

ECode UsbSettingsManager::AccessoryFilter::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // NOT IMPLEMENTED
    return NOERROR;
}

ECode UsbSettingsManager::AccessoryFilter::HashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = ((mManufacturer == NULL ? 0 : mManufacturer.GetHashCode()) ^
              (mModel == NULL ? 0 : mModel.GetHashCode()) ^
              (mVersion == NULL ? 0 : mVersion.GetHashCode()));

    return NOERROR;
}

ECode UsbSettingsManager::AccessoryFilter::ToString(
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

UInt32 UsbSettingsManager::MyPackageMonitor::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 UsbSettingsManager::MyPackageMonitor::Release()
{
    return ElRefBase::Release();
}

ECode UsbSettingsManager::MyPackageMonitor::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

void UsbSettingsManager::MyPackageMonitor::OnPackageAdded(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid)
{
    // NOT IMPLEMENTED
}

void UsbSettingsManager::MyPackageMonitor::OnPackageChanged(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid,
    /* [in] */ const ArrayOf<String>& components)
{
    // NOT IMPLEMENTED
}

void UsbSettingsManager::MyPackageMonitor::OnPackageRemoved(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid)
{
    // NOT IMPLEMENTED
}

UsbSettingsManager::UsbSettingsManager(
    /* [in] */ IContext* context)
{
    // NOT IMPLEMENTED
}

UInt32 UsbSettingsManager::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 UsbSettingsManager::Release()
{
    return ElRefBase::Release();
}

ECode UsbSettingsManager::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

void UsbSettingsManager::DeviceAttached(
    /* [in] */ IUsbDevice* device)
{
    // NOT IMPLEMENTED
}

void UsbSettingsManager::DeviceDetached(
    /* [in] */ IUsbDevice* device)
{
    // NOT IMPLEMENTED
}

void UsbSettingsManager::AccessoryAttached(
    /* [in] */ IUsbAccessory* accessory)
{
    // NOT IMPLEMENTED
}

void UsbSettingsManager::AccessoryDetached(
    /* [in] */ IUsbAccessory* accessory)
{
    // NOT IMPLEMENTED
}

Boolean UsbSettingsManager::HasPermission(
    /* [in] */ IUsbDevice* device)
{
    // NOT IMPLEMENTED
    return FALSE;
}

Boolean UsbSettingsManager::HasPermission(
    /* [in] */ IUsbAccessory* accessory)
{
    // NOT IMPLEMENTED
    return FALSE;
}

ECode UsbSettingsManager::CheckPermission(
    /* [in] */ IUsbDevice* device)
{
    if (HasPermission (device) == TRUE) {
        return NOERROR;
    }

    // throw new SecurityException("User has not given permission to device " + device);
    Logger::E(UsbSettingsManager::TAG, "User has not given permission to device.");
    return E_SECURITY_EXCEPTION;
}

ECode UsbSettingsManager::CheckPermission(
    /* [in] */ IUsbAccessory* accessory)
{
    if (HasPermission (accessory) == TRUE) {
        return NOERROR;
    }

    // throw new SecurityException("User has not given permission to accessory " + accessory);
    Logger::E(UsbSettingsManager::TAG, "User has not given permission to accessory.");
    return E_SECURITY_EXCEPTION;
}

void UsbSettingsManager::RequestPermission(
    /* [in] */ IUsbDevice* device,
    /* [in] */ const String& packageName,
    /* [in] */ IPendingIntent* pi)
{
    // NOT IMPLEMENTED
}

void UsbSettingsManager::RequestPermission(
    /* [in] */ IUsbAccessory* accessory,
    /* [in] */ const String& packageName,
    /* [in] */ IPendingIntent* pi)
{
    // NOT IMPLEMENTED
}

void UsbSettingsManager::SetDevicePackage(
    /* [in] */ IUsbDevice* device,
    /* [in] */ const String& packageName)
{
    // NOT IMPLEMENTED
}

void UsbSettingsManager::SetAccessoryPackage(
    /* [in] */ IUsbAccessory* accessory,
    /* [in] */ const String& packageName)
{
    // NOT IMPLEMENTED
}

void UsbSettingsManager::GrantDevicePermission(
    /* [in] */ IUsbDevice* device,
    /* [in] */ Int32 uid)
{
    // NOT IMPLEMENTED
}

void UsbSettingsManager::GrantAccessoryPermission(
    /* [in] */ IUsbAccessory* accessory,
    /* [in] */ Int32 uid)
{
    Mutex::Autolock lock(mLock);

    HashMap< AutoPtr<IUsbAccessory>, HashMap<Int32, Boolean> >::Iterator it;

    //HashMap<Int32, Boolean> uidList =

    /*
    synchronized (mLock) {
        SparseBooleanArray uidList = mAccessoryPermissionMap.get(accessory);
        if (uidList == null) {
            uidList = new SparseBooleanArray(1);
            mAccessoryPermissionMap.put(accessory, uidList);
        }
        uidList.put(uid, true);
    }
    */
    // NOT IMPLEMENTED
}

Boolean UsbSettingsManager::HasDefaults(
    /* [in] */ const String& packageName)
{
    Mutex::Autolock lock(mLock);

    if (IsDeviceFilterExistsRef(packageName) == TRUE) {
        return TRUE;
    }

    if (IsAccessoryFilterExistsRef(packageName) ==TRUE) {
        return TRUE;
    }

    return FALSE;
}

void UsbSettingsManager::ClearDefaults(
    /* [in] */ const String& packageName)
{
    Mutex::Autolock lock(mLock);

    if (ClearPackageDefaultsLocked(packageName) == FALSE) {
        return;
    }

    WriteSettingsLocked();
}

ECode UsbSettingsManager::ReadPreference(
    /* [in] */ IXmlPullParser* parser)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

void UsbSettingsManager::ReadSettingsLocked()
{
    // NOT IMPLEMENTED
}

void UsbSettingsManager::WriteSettingsLocked()
{
    // NOT IMPLEMENTED
}

Boolean UsbSettingsManager::PackageMatchesLocked(
    /* [in] */ IResolveInfo* info,
    /* [in] */ const String& metaDataName,
    /* [in] */ IUsbDevice* device,
    /* [in] */ IUsbAccessory* accessory)
{
    // NOT IMPLEMENTED
    return FALSE;
}

ArrayOf<IResolveInfo*>* UsbSettingsManager::GetDeviceMatchesLocked(
    /* [in] */ IUsbDevice* device,
    /* [in] */ IIntent* intent)
{
    // NOT IMPLEMENTED
    return NULL;
}

ArrayOf<IResolveInfo*>* UsbSettingsManager::GetAccessoryMatchesLocked(
    /* [in] */ IUsbAccessory* accessory,
    /* [in] */ IIntent* intent)
{
    // NOT IMPLEMENTED
    return NULL;
}

void UsbSettingsManager::ResolveActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ const ArrayOf<IResolveInfo*>& matches,
    /* [in] */ const String* defaultPackage,
    /* [in] */ IUsbDevice* device,
    /* [in] */ IUsbAccessory* accessory)
{
    // NOT IMPLEMENTED
}

Boolean UsbSettingsManager::ClearCompatibleMatchesLocked(
    /* [in] */ const String& packageName,
    /* [in] */ DeviceFilter* filter)
{
    // NOT IMPLEMENTED
    return FALSE;
}

Boolean UsbSettingsManager::ClearCompatibleMatchesLocked(
    /* [in] */ const String& packageName,
    /* [in] */ AccessoryFilter* filter)
{
    // NOT IMPLEMENTED
    return FALSE;
}

Boolean UsbSettingsManager::HandlePackageUpdateLocked(
    /* [in] */ const String& packageName,
    /* [in] */ IActivityInfo* aInfo,
    /* [in] */ const String& metaDataName)
{
    // NOT IMPLEMENTED
    return FALSE;
}

void UsbSettingsManager::HandlePackageUpdate(
    /* [in] */ const String& packageName)
{
    // NOT IMPLEMENTED
}

void UsbSettingsManager::RequestPermissionDialog(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& packageName,
    /* [in] */ IPendingIntent* pi)
{
    // NOT IMPLEMENTED
}

Boolean UsbSettingsManager::ClearPackageDefaultsLocked(
    /* [in] */ const String& packageName)
{
    // NOT IMPLEMENTED
    return FALSE;
}

Boolean UsbSettingsManager::IsDeviceFilterExistsRef(
    /* [in] */ const String& packageName)
{
    HashMap< AutoPtr<DeviceFilter>, String >::Iterator it;

    /*
    for (it = mDevicePreferenceMap.Begin(); it != mDevicePreferenceMap.End(); ++it)
    {
        String name = it->mSecond;

        if (name != packageName) {
            continue;
        }

        return TRUE;
    }
    //*/

    return FALSE;
}

Boolean UsbSettingsManager::IsAccessoryFilterExistsRef(
    /* [in] */ const String& packageName)
{
    HashMap< AutoPtr<AccessoryFilter>, String >::Iterator it;

    /*
    for (it = mAccessoryPreferenceMap.Begin(); it != mAccessoryPreferenceMap.End(); ++it)
    {
        String name = it->mSecond;

        if (name != packageName) {
            continue;
        }

        return TRUE;
    }
    //*/

    return FALSE;
}
