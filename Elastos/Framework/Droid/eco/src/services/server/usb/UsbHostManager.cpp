
#include "server/usb/UsbHostManager.h"

using namespace Elastos;

const String UsbHostManager::TAG("UsbHostManager");

UsbHostManager::UsbHostManager(
    /* [in] */ IContext* context,
    /* [in] */ UsbSettingsManager* settingsManager)
{
    // NOT IMPLEMENTED
}

void UsbHostManager::SystemReady()
{
    // NOT IMPLEMENTED
}

void UsbHostManager::GetDeviceList(
    /* [in] */ IBundle* devices)
{
    // NOT IMPLEMENTED
}

IParcelFileDescriptor* UsbHostManager::OpenDevice(
    /* [in] */ const String& deviceName)
{
    // NOT IMPLEMENTED
    return NULL;
}

void UsbHostManager::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw)
{
    // NOT IMPLEMENTED
}

Boolean UsbHostManager::IsBlackListedByName(
    /* [in] */ const String& deviceName)
{
    // NOT IMPLEMENTED
    return FALSE;
}

Boolean UsbHostManager::IsBlackListedByUSBClass(
    /* [in] */ Int32 clazz,
    /* [in] */ Int32 subClass,
    /* [in] */ Int32 protocol)
{
    // NOT IMPLEMENTED
    return FALSE;
}

void UsbHostManager::UsbDeviceAdded(
    /* [in] */ const String& deviceName,
    /* [in] */ Int32 vendorID,
    /* [in] */ Int32 productID,
    /* [in] */ Int32 deviceClass,
    /* [in] */ Int32 deviceSubclass,
    /* [in] */ Int32 deviceProtocol,
    /* [in] */ ArrayOf<Int32>* interfaceValues,
    /* [in] */ ArrayOf<Int32>* endpointValues)
{
    // NOT IMPLEMENTED
}

void UsbHostManager::UsbDeviceRemoved(
    /* [in] */ const String& deviceName)
{
    // NOT IMPLEMENTED
}

void UsbHostManager::NativeMonitorUsbHostBus()
{
    // NOT IMPLEMENTED
}

IParcelFileDescriptor* UsbHostManager::NativeOpenDevice(
    /* [in] */ const String& deviceName)
{
    // NOT IMPLEMENTED
    return NULL;
}
