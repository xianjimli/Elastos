
#include "server/usb/UsbDeviceManager.h"

using namespace Elastos;

UsbDeviceManager::UsbDeviceManager(
    /* [in] */ IContext* context,
    /* [in] */ UsbSettingsManager* settingsManager)
{
    // NOT IMPLEMENTED
}

void UsbDeviceManager::SystemReady()
{
    // NOT IMPLEMENTED
}

IUsbAccessory* UsbDeviceManager::GetCurrentAccessory()
{
    // NOT IMPLEMENTED
    return NULL;
}

IParcelFileDescriptor* UsbDeviceManager::OpenAccessory(
    /* [in] */ IUsbAccessory* accessory)
{
    // NOT IMPLEMENTED
    return NULL;
}

void UsbDeviceManager::SetCurrentFunctions(
    /* [in] */ const String& functions,
    /* [in] */ Boolean makeDefault)
{
    // NOT IMPLEMENTED
}

void UsbDeviceManager::SetMassStorageBackingFile(
    /* [in] */ const String& path)
{
    // NOT IMPLEMENTED
}

void UsbDeviceManager::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw)
{
    // NOT IMPLEMENTED
}
