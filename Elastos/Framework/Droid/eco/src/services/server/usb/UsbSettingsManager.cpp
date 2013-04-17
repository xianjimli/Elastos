
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

    // NOT IMPLEMENTED
}

UsbSettingsManager::~UsbSettingsManager()
{
    ClearDevicePermissionMapRecursiveRef();
    ClearAccessoryPermissionMapRecursiveRef();

    mDevicePreferenceMap.Clear();
    mAccessoryPreferenceMap.Clear();
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

    //AutoPtr<IUsbAccessory> key = accessory;
    //HashMap< IUsbAccessory*, HashMap<Int32, Boolean>* >::Iterator it = mAccessoryPermissionMap.Find(accessory);

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
        mDevicePreferenceMap[filter] = packageName;
    } else if (elmtName == "usb-accessory") {
        AutoPtr<AccessoryFilter> filter;
        AccessoryFilter::Read(parser, (AccessoryFilter**)&filter);
        mAccessoryPreferenceMap[filter] = packageName;
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

Boolean UsbSettingsManager::IsDevicePreferenceExistsRef(
    /* [in] */ const String& packageName)
{
    HashMap< AutoPtr<DeviceFilter>, String >::Iterator it;

    for (it = mDevicePreferenceMap.Begin(); it != mDevicePreferenceMap.End(); ++it)
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

    for (it = mAccessoryPreferenceMap.Begin(); it != mAccessoryPreferenceMap.End(); ++it)
    {
        String name = it->mSecond;

        if (name != packageName) {
            continue;
        }

        return TRUE;
    }

    return FALSE;
}

void UsbSettingsManager::RemoveDevicePreferenceRef(
    /* [in] */ const String& packageName,
    /* [out] */ Boolean* result)
{
    // make a copy of the key set to avoid ConcurrentModificationException
    HashMap< AutoPtr<DeviceFilter>, String >::Iterator it;

    for (it = mDevicePreferenceMap.Begin(); it != mDevicePreferenceMap.End(); ++it)
    {
        String name = it->mSecond;

        if (name != packageName) {
            continue;
        }

        *result = TRUE;
        mDevicePreferenceMap.Erase(it);
    }
}

void UsbSettingsManager::RemoveAccessoryPreferenceRef(
    /* [in] */ const String& packageName,
    /* [out] */ Boolean* result)
{
    // make a copy of the key set to avoid ConcurrentModificationException
    HashMap< AutoPtr<AccessoryFilter>, String >::Iterator it;

    for (it = mAccessoryPreferenceMap.Begin(); it != mAccessoryPreferenceMap.End(); ++it)
    {
        String name = it->mSecond;

        if (name != packageName) {
            continue;
        }

        *result = TRUE;
        mAccessoryPreferenceMap.Erase(it);
    }
}

void UsbSettingsManager::ClearDevicePermissionMapRecursiveRef()
{
    mDevicePermissionMap.Clear();
}

void UsbSettingsManager::ClearAccessoryPermissionMapRecursiveRef()
{
    mAccessoryPermissionMap.Clear();
}
