
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
    // clear temporary permissions for the device
    String name;
    device->GetDeviceName(&name);

    Boolean result = FALSE;
    RemoveDevicePermissionRef(name, &result);

    AutoPtr<IIntent> intent;
    CIntent::New(String(UsbManager_ACTION_USB_DEVICE_DETACHED), (IIntent**)&intent);
    intent->PutParcelableExtra(String(UsbManager_EXTRA_DEVICE), (IParcelable*)device);

    if (DEBUG == TRUE) {
        String shortString;
        intent->ToShortString(TRUE, TRUE, &shortString);

        StringBuffer buf;
        buf += "usbDeviceRemoved, sending ";
        buf += shortString;

        Logger::D(UsbSettingsManager::TAG, (String)buf);
    }

    mContext->SendBroadcast(intent);

    Boolean usbAudio = FALSE;

    Int32 count;
    device->GetInterfaceCount(&count);

    ArrayOf<IUsbInterface*>* interfaces = ArrayOf<IUsbInterface*>::Alloc(count);
    for (Int32 i = 0; i < count; i++) {
        device->GetInterface(i, &(*interfaces)[i]);

        Int32 cls;
        (*interfaces)[i]->GetInterfaceClass(&cls);

        StringBuffer buf;
        buf += "interfaces";
        buf += i;
        buf += "num";
        buf += cls;
        Logger::V(UsbSettingsManager::TAG, (String)buf);

        if (cls != 1) {
            continue;
        }

        usbAudio = TRUE;
    }

    if (usbAudio == FALSE) {
        return;
    }

    AutoPtr<IIntent> intentUsbAudio;
    CIntent::New(String(Intent_ACTION_USB_AUDIO_DEVICE_PLUG), (IIntent**)&intentUsbAudio);
    intentUsbAudio->PutInt32Extra(String("state"), 0);
    mContext->SendBroadcast(intentUsbAudio);
}

void UsbSettingsManager::AccessoryAttached(
    /* [in] */ IUsbAccessory* accessory)
{
    AutoPtr<IIntent> intent;
    CIntent::New(String(UsbManager_ACTION_USB_ACCESSORY_ATTACHED), (IIntent**)&intent);

    intent->PutParcelableExtra(String(UsbManager_EXTRA_ACCESSORY), (IParcelable*)accessory);
    intent->AddFlags(Intent_FLAG_ACTIVITY_NEW_TASK);

    ArrayOf<IResolveInfo*>* matches;
    String defaultPackage;

    {
        Mutex::Autolock lock(mLock);

        matches = GetAccessoryMatchesLocked(accessory, intent);

        /*
         * Launch our default activity directly, if we have one.
         * Otherwise we will start the UsbResolverActivity to allow the user to choose.
         */
        AutoPtr<AccessoryFilter> filter = new AccessoryFilter(accessory);
        defaultPackage = GetAccessoryPreferenceRef(filter);
    }

    ResolveActivity(intent, *matches, defaultPackage, NULL, accessory);
}

void UsbSettingsManager::AccessoryDetached(
    /* [in] */ IUsbAccessory* accessory)
{
    // clear temporary permissions for the accessory
    Boolean result = FALSE;
    RemoveAccessoryPermissionRef(accessory, &result);

    AutoPtr<IIntent> intent;
    CIntent::New(String(UsbManager_ACTION_USB_ACCESSORY_DETACHED), (IIntent**)&intent);

    intent->PutParcelableExtra(String(UsbManager_EXTRA_ACCESSORY), (IParcelable*)accessory);
    mContext->SendBroadcast(intent);
}

Boolean UsbSettingsManager::HasPermission(
    /* [in] */ IUsbDevice* device)
{
    Mutex::Autolock lock(mLock);

    String name;
    device->GetDeviceName(&name);

    HashMap< String, HashMap<Int32, Boolean>* >::Iterator it = mDevicePermissionMap->Find(name);

    if (it == mDevicePermissionMap->End()) {
        return FALSE;
    }

    return IsUidWithPermissionExistsRef(it->mSecond, Binder::GetCallingUid());
}

Boolean UsbSettingsManager::HasPermission(
    /* [in] */ IUsbAccessory* accessory)
{
    Mutex::Autolock lock(mLock);

    HashMap< AutoPtr<IUsbAccessory>, HashMap<Int32, Boolean>* >::Iterator it = mAccessoryPermissionMap->Find(accessory);

    if (it == mAccessoryPermissionMap->End()) {
        return FALSE;
    }

    return IsUidWithPermissionExistsRef(it->mSecond, Binder::GetCallingUid());
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
    AutoPtr<IIntent> intent;
    CIntent::New((IIntent**)&intent);

    // start UsbPermissionActivity so user can choose an activity
    if (HasPermission(device) != FALSE)
    {
        intent->PutParcelableExtra(String(UsbManager_EXTRA_DEVICE), (IParcelable*)device);
        RequestPermissionDialog(intent, packageName, pi);
        return;
    }

    // respond immediately if permission has already been granted
    intent->PutParcelableExtra(String(UsbManager_EXTRA_DEVICE), (IParcelable*)device);
    intent->PutBooleanExtra(String(UsbManager_EXTRA_PERMISSION_GRANTED), TRUE);

    ECode ec;
    ec = pi->Send3(mContext, 0, intent);

    if (ec != E_CANCELED_EXCEPTION) {
        return;
    }

    if (DEBUG == FALSE) {
        return;
    }

    Logger::D(UsbSettingsManager::TAG, "requestPermission PendingIntent was cancelled");
}

void UsbSettingsManager::RequestPermission(
    /* [in] */ IUsbAccessory* accessory,
    /* [in] */ const String& packageName,
    /* [in] */ IPendingIntent* pi)
{
    AutoPtr<IIntent> intent;
    CIntent::New((IIntent**)&intent);

    // respond immediately if permission has already been granted
    if (HasPermission(accessory) == FALSE) {
        intent->PutParcelableExtra(String(UsbManager_EXTRA_ACCESSORY), (IParcelable*)accessory);
        RequestPermissionDialog(intent, packageName, pi);
        return;
    }

    intent->PutParcelableExtra(String(UsbManager_EXTRA_ACCESSORY), (IParcelable*)accessory);
    intent->PutBooleanExtra(String(UsbManager_EXTRA_PERMISSION_GRANTED), TRUE);

    ECode ec;
    ec = pi->Send3(mContext, 0, intent);

    if (ec != E_CANCELED_EXCEPTION) {
        return;
    }

    if (DEBUG == FALSE) {
        return;
    }

    Logger::D(UsbSettingsManager::TAG, "requestPermission PendingIntent was cancelled");
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

    uidList = it->mSecond;
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

    uidList = it->mSecond;
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
    /* [in] */ const String& defaultPackage,
    /* [in] */ IUsbDevice* device,
    /* [in] */ IUsbAccessory* accessory)
{
    // NOT IMPLEMENTED
}

Boolean UsbSettingsManager::ClearCompatibleMatchesLocked(
    /* [in] */ const String& packageName,
    /* [in] */ DeviceFilter* filter)
{
    Boolean changed = FALSE;
    RemoveDevicePreferenceByMatchesRef(filter, &changed);
    return changed;
}

Boolean UsbSettingsManager::ClearCompatibleMatchesLocked(
    /* [in] */ const String& packageName,
    /* [in] */ AccessoryFilter* filter)
{
    Boolean changed = FALSE;
    RemoveAccessoryPreferenceByMatchesRef(filter, &changed);
    return changed;
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
    /*
     * Check to see if the package supports any USB devices or accessories.
     * If so, clear any non-matching preferences for matching devices/accessories.
     */
    Mutex::Autolock lock(mLock);









    // NOT IMPLEMENTED
}

ECode UsbSettingsManager::RequestPermissionDialog(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& packageName,
    /* [in] */ IPendingIntent* pi)
{
    Int32 uid = Binder::GetCallingUid();

    // compare uid with packageName to foil apps pretending to be someone else
    ECode ec;
    AutoPtr<IApplicationInfo> appInfo;
    ec = mPackageManager->GetApplicationInfo(packageName, 0, (IApplicationInfo**)&appInfo);

    if (ec == E_NAME_NOT_FOUND_EXCEPTION) {
        StringBuffer buf;

        buf += "package ";
        buf += packageName;
        buf += " not found";

        Logger::D(UsbSettingsManager::TAG, (String)buf);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 appInfoUid;
    appInfo->GetUid(&appInfoUid);

    if (appInfoUid != uid) {
        StringBuffer buf;

        buf += "package ";
        buf += packageName;
        buf += " does not match caller's uid ";
        buf += uid;

        Logger::D(UsbSettingsManager::TAG, (String)buf);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int64 identity = Binder::ClearCallingIdentity();
    intent->SetClassName(String("com.android.systemui"), String("com.android.systemui.usb.UsbPermissionActivity"));
    intent->AddFlags(Intent_FLAG_ACTIVITY_NEW_TASK);
    intent->PutParcelableExtra(String(Intent_EXTRA_INTENT), (IParcelable*)pi);
    intent->PutStringExtra(String("package"), packageName);
    intent->PutInt32Extra(String("uid"), uid);

    ec = mContext->StartActivity(intent);

    // RYAN
    //if (ec == E_ACTIVITY_NOT_FOUND_EXCEPTION) {
    if (ec == E_RUNTIME_EXCEPTION) {
        Logger::E(UsbSettingsManager::TAG, "unable to start UsbPermissionActivity");
    }

    Binder::RestoreCallingIdentity(identity);
    return NOERROR;
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

void UsbSettingsManager::RemoveDevicePermissionRef(
    /* [in] */ const String& name,
    /* [out] */ Boolean* result)
{
    *result = FALSE;

    HashMap< String, HashMap<Int32, Boolean>* >::Iterator it;

    for (it = mDevicePermissionMap->Begin(); it != mDevicePermissionMap->End(); ++it)
    {
        if (name != it->mFirst) {
            continue;
        }

        *result = TRUE;
        mDevicePermissionMap->Erase(it);
    }
}

void UsbSettingsManager::RemoveAccessoryPermissionRef(
    /* [in] */ IUsbAccessory* filter,
    /* [out] */ Boolean* result)
{
    *result = FALSE;

    HashMap< AutoPtr<IUsbAccessory>, HashMap<Int32, Boolean>* >::Iterator it;

    for (it = mAccessoryPermissionMap->Begin(); it != mAccessoryPermissionMap->End(); ++it)
    {
        Boolean isEquals = FALSE;
        it->mFirst->Equals(filter, &isEquals);

        if (isEquals == FALSE) {
            continue;
        }

        *result = TRUE;
        mAccessoryPermissionMap->Erase(it);
    }
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

void UsbSettingsManager::RemoveDevicePreferenceByMatchesRef(
    /* [in] */ DeviceFilter* filter,
    /* [out] */ Boolean* result)
{
    *result = FALSE;
    HashMap< AutoPtr<DeviceFilter>, String >::Iterator it;

    for (it = mDevicePreferenceMap->Begin(); it != mDevicePreferenceMap->End(); ++it)
    {
        Boolean isMatches = it->mFirst->Matches(filter);

        if (isMatches == FALSE) {
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

void UsbSettingsManager::RemoveAccessoryPreferenceByMatchesRef(
    /* [in] */ AccessoryFilter* filter,
    /* [out] */ Boolean* result)
{
    *result = FALSE;
    HashMap< AutoPtr<AccessoryFilter>, String >::Iterator it;

    for (it = mAccessoryPreferenceMap->Begin(); it != mAccessoryPreferenceMap->End(); ++it)
    {
        Boolean isMatches = it->mFirst->Matches(filter);

        if (isMatches == FALSE) {
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

Boolean UsbSettingsManager::IsUidWithPermissionExistsRef(
    /* [in] */ HashMap<Int32, Boolean>* uidList,
    /* [in] */ Int32 callingUid)
{
    HashMap<Int32, Boolean>::Iterator it = uidList->Find(callingUid);

    if (it == uidList->End()) {
        return FALSE;
    }

    return it->mSecond;
}
