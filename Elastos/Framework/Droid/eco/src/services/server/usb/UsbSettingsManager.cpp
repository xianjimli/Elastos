
#include "server/usb/UsbSettingsManager.h"

const Boolean UsbSettingsManager::DEBUG = FALSE;

const String UsbSettingsManager::TAG = String("UsbSettingsManager");
const String UsbSettingsManager::SETTINGS_FILE_PATH = String("/data/system/usb_device_manager.xml");
const String UsbSettingsManager::USB_AUDIO_SOURCE_PATH = String("/proc/asound/usb_audio_info");

UsbSettingsManager::MyPackageMonitor::MyPackageMonitor(
    /* [in] */ UsbSettingsManager* host)
{
    mHost = host;
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
    mHost->HandlePackageUpdate(packageName);
}

void UsbSettingsManager::MyPackageMonitor::OnPackageChanged(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid,
    /* [in] */ const ArrayOf<String>& components)
{
    mHost->HandlePackageUpdate(packageName);
}

void UsbSettingsManager::MyPackageMonitor::OnPackageRemoved(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid)
{
    mHost->ClearDefaults(packageName);
}

UsbSettingsManager::UsbSettingsManager(
    /* [in] */ IContext* context)
{
    mContext = context;
    mPackageMonitor = new MyPackageMonitor(this);

    {
        Mutex::Autolock lock(mLock);
        ReadSettingsLocked();
    }

    // RYAN
    //mPackageMonitor.register(context, null, true);

    mDevicePermissionMap = new HashMap< String, HashMap<Int32, Boolean>* >();
    mAccessoryPermissionMap = new HashMap< AutoPtr<IUsbAccessory>, HashMap<Int32, Boolean>* >();
    mDevicePreferenceMap = new HashMap< AutoPtr<DeviceFilter>, String >();
    mAccessoryPreferenceMap = new HashMap< AutoPtr<AccessoryFilter>, String >();
}

UsbSettingsManager::~UsbSettingsManager()
{
    if (mDevicePermissionMap != NULL) {
        mDevicePermissionMap->Clear();
        delete mDevicePermissionMap;
    }

    if (mAccessoryPermissionMap != NULL) {
        mAccessoryPermissionMap->Clear();
        delete mAccessoryPermissionMap;
    }

    if (mDevicePreferenceMap != NULL) {
        mDevicePreferenceMap->Clear();
        delete mDevicePreferenceMap;
    }

    if (mAccessoryPreferenceMap != NULL) {
        mAccessoryPreferenceMap->Clear();
        delete mAccessoryPreferenceMap;
    }
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
    Mutex::Autolock lock(mLock);

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
    AutoPtr<DeviceFilter> filter = new DeviceFilter(device);
    Boolean changed = FALSE;

    {
        Mutex::Autolock lock(mLock);

        if (packageName.IsNull() == TRUE) {
            RemoveDevicePreferenceRef(filter, &changed);
        } else {
            String preference = GetDevicePreferenceRef(filter);

            changed = (packageName == preference) ? FALSE : TRUE;

            if (changed == TRUE) {
                (*mDevicePreferenceMap)[filter] = packageName;
            }
        }

        if (changed == TRUE) {
            WriteSettingsLocked();
        }
    }
}

void UsbSettingsManager::SetAccessoryPackage(
    /* [in] */ IUsbAccessory* accessory,
    /* [in] */ const String& packageName)
{
    AutoPtr<AccessoryFilter> filter = new AccessoryFilter(accessory);
    Boolean changed = FALSE;

    {
        Mutex::Autolock lock(mLock);

        if (packageName.IsNull() == TRUE) {
            RemoveAccessoryPreferenceRef(filter, &changed);
        } else {
            String preference = GetAccessoryPreferenceRef(filter);

            changed = (packageName == preference) ? FALSE : TRUE;

            if (changed == TRUE) {
                (*mAccessoryPreferenceMap)[filter] = packageName;
            }
        }

        if (changed == TRUE) {
            WriteSettingsLocked();
        }
    }
}

void UsbSettingsManager::GrantDevicePermission(
    /* [in] */ IUsbDevice* device,
    /* [in] */ Int32 uid)
{
    Mutex::Autolock lock(mLock);

    String deviceName;
    device->GetDeviceName(&deviceName);

    HashMap< String, HashMap<Int32, Boolean>* >::Iterator it = mDevicePermissionMap->Find(deviceName);

    HashMap<Int32, Boolean>* uidList;

    if (it == mDevicePermissionMap->End()) {
        uidList = new HashMap<Int32, Boolean>();
        (*mDevicePermissionMap)[deviceName] = uidList;
    }

    (*uidList)[uid] = TRUE;
}

void UsbSettingsManager::GrantAccessoryPermission(
    /* [in] */ IUsbAccessory* accessory,
    /* [in] */ Int32 uid)
{
    Mutex::Autolock lock(mLock);

    HashMap< AutoPtr<IUsbAccessory>, HashMap<Int32, Boolean>* >::Iterator it = mAccessoryPermissionMap->Find(accessory);

    HashMap<Int32, Boolean>* uidList;

    if (it == mAccessoryPermissionMap->End()) {
        uidList = new HashMap<Int32, Boolean>();
        (*mAccessoryPermissionMap)[accessory] = uidList;
    }

    (*uidList)[uid] = TRUE;
}

Boolean UsbSettingsManager::HasDefaults(
    /* [in] */ const String& packageName)
{
    Mutex::Autolock lock(mLock);

    if (IsDevicePreferenceExistsRef(packageName) == TRUE) {
        return TRUE;
    }

    if (IsAccessoryPreferenceExistsRef(packageName) ==TRUE) {
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
    String packageName;

    Int32 count = 0;
    parser->GetAttributeCount(&count);

    for (Int32 i = 0; i < count; i++) {
        String name;
        parser->GetAttributeName(i, &name);

        if (name != "package") {
            continue;
        }

        parser->GetAttributeValue(i, &packageName);
        break;
    }

    XmlUtils::NextElement(parser);

    String elmtName;
    parser->GetName(&elmtName);

    if (elmtName == "usb-device") {
        AutoPtr<DeviceFilter> filter;
        DeviceFilter::Read(parser, (DeviceFilter**)&filter);
        (*mDevicePreferenceMap)[filter] = packageName;
    } else if (elmtName == "usb-accessory") {
        AutoPtr<AccessoryFilter> filter;
        AccessoryFilter::Read(parser, (AccessoryFilter**)&filter);
        (*mAccessoryPreferenceMap)[filter] = packageName;
    }

    XmlUtils::NextElement(parser);
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
    Boolean cleared = FALSE;

    {
        Mutex::Autolock lock(mLock);
        RemoveDevicePreferenceRef(packageName, &cleared);
        RemoveAccessoryPreferenceRef(packageName, &cleared);
    }

    return cleared;
}

String UsbSettingsManager::GetDevicePreferenceRef(
    /* [in] */ DeviceFilter* filter)
{
    HashMap< AutoPtr<DeviceFilter>, String >::Iterator it = mDevicePreferenceMap->Find(filter);

    if (it == mDevicePreferenceMap->End()) {
        return String(NULL);
    }

    return it->mSecond;
}

String UsbSettingsManager::GetAccessoryPreferenceRef(
    /* [in] */ AccessoryFilter* filter)
{
    HashMap< AutoPtr<AccessoryFilter>, String >::Iterator it = mAccessoryPreferenceMap->Find(filter);

    if (it == mAccessoryPreferenceMap->End()) {
        return String(NULL);
    }

    return it->mSecond;
}

void UsbSettingsManager::RemoveDevicePreferenceRef(
    /* [in] */ const String& packageName,
    /* [out] */ Boolean* result)
{
    *result = FALSE;

    // make a copy of the key set to avoid ConcurrentModificationException
    HashMap< AutoPtr<DeviceFilter>, String >::Iterator it;

    for (it = mDevicePreferenceMap->Begin(); it != mDevicePreferenceMap->End(); ++it)
    {
        String name = it->mSecond;

        if (name != packageName) {
            continue;
        }

        *result = TRUE;
        mDevicePreferenceMap->Erase(it);
    }
}

void UsbSettingsManager::RemoveDevicePreferenceRef(
    /* [in] */ DeviceFilter* filter,
    /* [out] */ Boolean* result)
{
    *result = FALSE;
    HashMap< AutoPtr<DeviceFilter>, String >::Iterator it;

    for (it = mDevicePreferenceMap->Begin(); it != mDevicePreferenceMap->End(); ++it)
    {
        Boolean isEquals = FALSE;
        it->mFirst->Equals(filter, &isEquals);

        if (isEquals == FALSE) {
            continue;
        }

        *result = TRUE;
        mDevicePreferenceMap->Erase(it);
    }
}

void UsbSettingsManager::RemoveAccessoryPreferenceRef(
    /* [in] */ const String& packageName,
    /* [out] */ Boolean* result)
{
    *result = FALSE;

    // make a copy of the key set to avoid ConcurrentModificationException
    HashMap< AutoPtr<AccessoryFilter>, String >::Iterator it;

    for (it = mAccessoryPreferenceMap->Begin(); it != mAccessoryPreferenceMap->End(); ++it)
    {
        String name = it->mSecond;

        if (name != packageName) {
            continue;
        }

        *result = TRUE;
        mAccessoryPreferenceMap->Erase(it);
    }
}

void UsbSettingsManager::RemoveAccessoryPreferenceRef(
    /* [in] */ AccessoryFilter* filter,
    /* [out] */ Boolean* result)
{
    *result = FALSE;
    HashMap< AutoPtr<AccessoryFilter>, String >::Iterator it;

    for (it = mAccessoryPreferenceMap->Begin(); it != mAccessoryPreferenceMap->End(); ++it)
    {
        Boolean isEquals = FALSE;
        it->mFirst->Equals(filter, &isEquals);

        if (isEquals == FALSE) {
            continue;
        }

        *result = TRUE;
        mAccessoryPreferenceMap->Erase(it);
    }
}

Boolean UsbSettingsManager::IsDevicePreferenceExistsRef(
    /* [in] */ const String& packageName)
{
    HashMap< AutoPtr<DeviceFilter>, String >::Iterator it;

    for (it = mDevicePreferenceMap->Begin(); it != mDevicePreferenceMap->End(); ++it)
    {
        String name = it->mSecond;

        if (name != packageName) {
            continue;
        }

        return TRUE;
    }

    return FALSE;
}

Boolean UsbSettingsManager::IsAccessoryPreferenceExistsRef(
    /* [in] */ const String& packageName)
{
    HashMap< AutoPtr<AccessoryFilter>, String >::Iterator it;

    for (it = mAccessoryPreferenceMap->Begin(); it != mAccessoryPreferenceMap->End(); ++it)
    {
        String name = it->mSecond;

        if (name != packageName) {
            continue;
        }

        return TRUE;
    }

    return FALSE;
}
