
#include "server/usb/UsbSettingsManager.h"

using namespace Elastos;

const Boolean UsbSettingsManager::DEBUG = FALSE;

const String UsbSettingsManager::TAG = String("UsbSettingsManager");
const String UsbSettingsManager::USB_AUDIO_SOURCE_PATH = String("/proc/asound/usb_audio_info");
const String UsbSettingsManager::SETTINGS_FILE_PATH = String("/data/system/usb_device_manager.xml");

UsbSettingsManager::DeviceFilter::DeviceFilter(
    /* [in] */ Int32 vid,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 clasz,
    /* [in] */ Int32 subclass,
    /* [in] */ Int32 protocol)
{
    // NOT IMPLEMENTED
}

UsbSettingsManager::DeviceFilter::DeviceFilter(
    /* [in] */ IUsbDevice* device)
{
    // NOT IMPLEMENTED
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
    // NOT IMPLEMENTED
    return NOERROR;
}

ECode UsbSettingsManager::DeviceFilter::Write(
    /* [in] */ IXmlSerializer* serializer)
{
    // NOT IMPLEMENTED
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
    // NOT IMPLEMENTED
    return FALSE;
}

ECode UsbSettingsManager::DeviceFilter::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

ECode UsbSettingsManager::DeviceFilter::HashCode(
    /* [out] */ Int32* value)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

ECode UsbSettingsManager::DeviceFilter::ToString(
    /* [out] */ String* str)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

Boolean UsbSettingsManager::DeviceFilter::Matches(
    /* [in] */ Int32 clasz,
    /* [in] */ Int32 subclass,
    /* [in] */ Int32 protocol)
{
    // NOT IMPLEMENTED
    return FALSE;
}

UsbSettingsManager::AccessoryFilter::AccessoryFilter(
    /* [in] */ const String& manufacturer,
    /* [in] */ const String& model,
    /* [in] */ const String& ver)
{
    // NOT IMPLEMENTED
}

UsbSettingsManager::AccessoryFilter::AccessoryFilter(
    /* [in] */ IUsbAccessory* accessory)
{
    // NOT IMPLEMENTED
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
    // NOT IMPLEMENTED
    return NOERROR;
}

ECode UsbSettingsManager::AccessoryFilter::Write(
    /* [in] */ IXmlSerializer* serializer)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

Boolean UsbSettingsManager::AccessoryFilter::Matches(
    /* [in] */ IUsbAccessory* accessory)
{
    // NOT IMPLEMENTED
    return FALSE;
}

Boolean UsbSettingsManager::AccessoryFilter::Matches(
    /* [in] */ AccessoryFilter* filter)
{
    // NOT IMPLEMENTED
    return FALSE;
}

ECode UsbSettingsManager::AccessoryFilter::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

ECode UsbSettingsManager::AccessoryFilter::HashCode(
    /* [out] */ Int32* value)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

ECode UsbSettingsManager::AccessoryFilter::ToString(
    /* [out] */ String* str)
{
    // NOT IMPLEMENTED
    return NOERROR;
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

void UsbSettingsManager::CheckPermission(
    /* [in] */ IUsbDevice* device)
{
    // NOT IMPLEMENTED
}

void UsbSettingsManager::CheckPermission(
    /* [in] */ IUsbAccessory* accessory)
{
    // NOT IMPLEMENTED
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
    // NOT IMPLEMENTED
}

Boolean UsbSettingsManager::HasDefaults(
    /* [in] */ const String& packageName)
{
    // NOT IMPLEMENTED
    return FALSE;
}

void UsbSettingsManager::ClearDefaults(
    /* [in] */ const String& packageName)
{
    // NOT IMPLEMENTED
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
