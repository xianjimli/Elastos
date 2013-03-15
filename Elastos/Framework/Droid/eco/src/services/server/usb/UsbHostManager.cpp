
#include "server/usb/UsbHostManager.h"

using namespace Elastos;

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
