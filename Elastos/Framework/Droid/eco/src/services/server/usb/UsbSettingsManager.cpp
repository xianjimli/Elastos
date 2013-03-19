
#include "server/usb/UsbSettingsManager.h"

using namespace Elastos;

UsbSettingsManager::UsbSettingsManager(
    /* [in] */ IContext* context)
{
    // NOT IMPLEMENTED
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

Boolean UsbSettingsManager::HasDevicePermission(
    /* [in] */ IUsbDevice* device)
{
    // NOT IMPLEMENTED
    return FALSE;
}

Boolean UsbSettingsManager::HasAccessoryPermission(
    /* [in] */ IUsbAccessory* accessory)
{
    // NOT IMPLEMENTED
    return FALSE;
}

void UsbSettingsManager::CheckDevicePermission(
    /* [in] */ IUsbDevice* device)
{
    // NOT IMPLEMENTED
}

void UsbSettingsManager::CheckAccessoryPermission(
    /* [in] */ IUsbAccessory* accessory)
{
    // NOT IMPLEMENTED
}

void UsbSettingsManager::RequestDevicePermission(
    /* [in] */ IUsbDevice* device,
    /* [in] */ const String& packageName,
    /* [in] */ IPendingIntent* pi)
{
    // NOT IMPLEMENTED
}

void UsbSettingsManager::RequestAccessoryPermission(
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

void UsbSettingsManager::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw)
{
    // NOT IMPLEMENTED
}
