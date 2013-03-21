
#include "server/usb/UsbDeviceManager.h"

using namespace Elastos;

const Boolean UsbDeviceManager::DEBUG = FALSE;

const String UsbDeviceManager::TAG = String("UsbDeviceManager");
const String UsbDeviceManager::USB_STATE_MATCH = String("DEVPATH=/devices/virtual/android_usb/android0");
const String UsbDeviceManager::ACCESSORY_START_MATCH = String("DEVPATH=/devices/virtual/misc/usb_accessory");
const String UsbDeviceManager::FUNCTIONS_PATH = String("/sys/class/android_usb/android0/functions");
const String UsbDeviceManager::STATE_PATH = String("/sys/class/android_usb/android0/state");
const String UsbDeviceManager::MASS_STORAGE_FILE_PATH = String("/sys/class/android_usb/android0/f_mass_storage/lun/file");
const String UsbDeviceManager::RNDIS_ETH_ADDR_PATH = String("/sys/class/android_usb/android0/f_rndis/ethaddr");
const String UsbDeviceManager::AUDIO_SOURCE_PCM_PATH = String("/sys/class/android_usb/android0/f_audio_source/pcm");
const String UsbDeviceManager::BOOT_MODE_PROPERTY = String("ro.bootmode");

const Int32 UsbDeviceManager::MSG_UPDATE_STATE = 0;
const Int32 UsbDeviceManager::MSG_ENABLE_ADB = 1;
const Int32 UsbDeviceManager::MSG_SET_CURRENT_FUNCTIONS = 2;
const Int32 UsbDeviceManager::MSG_SYSTEM_READY = 3;
const Int32 UsbDeviceManager::MSG_BOOT_COMPLETED = 4;
const Int32 UsbDeviceManager::AUDIO_MODE_NONE = 0;
const Int32 UsbDeviceManager::AUDIO_MODE_SOURCE = 1;
const Int32 UsbDeviceManager::UPDATE_DELAY = 1000;

UsbDeviceManager::AdbSettingsObserver::AdbSettingsObserver()
{
    // NOT IMPLEMENTED
}

UInt32 UsbDeviceManager::AdbSettingsObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 UsbDeviceManager::AdbSettingsObserver::Release()
{
    return ElRefBase::Release();
}

ECode UsbDeviceManager::AdbSettingsObserver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode OnChange(
    /* [in] */ Boolean selfChange)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

UsbDeviceManager::UsbHandler::UsbHandler()
{
    // NOT IMPLEMENTED
}

UInt32 UsbDeviceManager::UsbHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 UsbDeviceManager::UsbHandler::Release()
{
    return ElRefBase::Release();
}

ECode UsbDeviceManager::UsbHandler::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}


void UsbDeviceManager::UsbHandler::SendMessage(
    /* [in] */ Int32 what,
    /* [in] */ Boolean arg)
{
    // NOT IMPLEMENTED
}

void UsbDeviceManager::UsbHandler::SendMessage(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* arg)
{
    // NOT IMPLEMENTED
}

void UsbDeviceManager::UsbHandler::SendMessage(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* arg0,
    /* [in] */ Boolean arg1)
{
    // NOT IMPLEMENTED
}

void UsbDeviceManager::UsbHandler::UpdateState(
    /* [in] */ const String& state)
{
    // NOT IMPLEMENTED
}

ECode UsbDeviceManager::UsbHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

IUsbAccessory* UsbDeviceManager::UsbHandler::GetCurrentAccessory()
{
    // NOT IMPLEMENTED
    return NULL;
}

void UsbDeviceManager::UsbHandler::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw)
{
    // NOT IMPLEMENTED
}

Boolean UsbDeviceManager::UsbHandler::WaitForState(
    /* [in] */ const String& state)
{
    // NOT IMPLEMENTED
    return FALSE;
}

Boolean UsbDeviceManager::UsbHandler::SetUsbConfig(
    /* [in] */ const String& config)
{
    // NOT IMPLEMENTED
    return FALSE;
}

void UsbDeviceManager::UsbHandler::SetAdbEnabled(
    /* [in] */ Boolean enable)
{
    // NOT IMPLEMENTED
}

void UsbDeviceManager::UsbHandler::SetEnabledFunctions(
    /* [in] */ const String& functions,
    /* [in] */ Boolean makeDefault)
{
    // NOT IMPLEMENTED
}

void UsbDeviceManager::UsbHandler::UpdateCurrentAccessory()
{
    // NOT IMPLEMENTED
}

void UsbDeviceManager::UsbHandler::UpdateUsbState()
{
    // NOT IMPLEMENTED
}

void UsbDeviceManager::UsbHandler::UpdateAudioSourceFunction()
{
    // NOT IMPLEMENTED
}

void UsbDeviceManager::UsbHandler::UpdateUsbNotification()
{
    // NOT IMPLEMENTED
}

Boolean UsbDeviceManager::UsbHandler::GetStorageSoundStatus()
{
    // NOT IMPLEMENTED
    return FALSE;
}

void UsbDeviceManager::UsbHandler::PlayDefaultSound()
{
    // NOT IMPLEMENTED
}

void UsbDeviceManager::UsbHandler::UpdateAdbNotification()
{
    // NOT IMPLEMENTED
}

UsbDeviceManager::UsbBootCompletedReceiver::UsbBootCompletedReceiver(
    /* [in] */ UsbHandler* host)
{
    // NOT IMPLEMENTED
}

UInt32 UsbDeviceManager::UsbBootCompletedReceiver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 UsbDeviceManager::UsbBootCompletedReceiver::Release()
{
    return ElRefBase::Release();
}

ECode UsbDeviceManager::UsbBootCompletedReceiver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode UsbDeviceManager::UsbBootCompletedReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // NOT IMPLEMENTED
    return NOERROR;
}

UsbDeviceManager::UsbDeviceManager(
    /* [in] */ IContext* context,
    /* [in] */ UsbSettingsManager* settingsManager)
{
    // NOT IMPLEMENTED
}

UInt32 UsbDeviceManager::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 UsbDeviceManager::Release()
{
    return ElRefBase::Release();
}

ECode UsbDeviceManager::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
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

void UsbDeviceManager::InitRndisAddress()
{
    // NOT IMPLEMENTED
}

String UsbDeviceManager::AddFunction(
    /* [in] */ const String& functions,
    /* [in] */ const String& function)
{
    // NOT IMPLEMENTED
    return String("");
}

String UsbDeviceManager::RemoveFunction(
    /* [in] */ const String& functions,
    /* [in] */ const String& function)
{
    // NOT IMPLEMENTED
    return String("");
}

Boolean UsbDeviceManager::ContainsFunction(
    /* [in] */ const String& functions,
    /* [in] */ const String& function)
{
    // NOT IMPLEMENTED
    return FALSE;
}

void UsbDeviceManager::StartAccessoryMode()
{
    // NOT IMPLEMENTED
}

void UsbDeviceManager::ReadOemUsbOverrideConfig()
{
    // NOT IMPLEMENTED
}

Boolean UsbDeviceManager::NeedsOemUsbOverride()
{
    // NOT IMPLEMENTED
    return FALSE;
}

String UsbDeviceManager::ProcessOemUsbOverride(
    /* [in] */ const String& usbFunctions)
{
    // NOT IMPLEMENTED
    return String("");
}

ArrayOf<String>* UsbDeviceManager::NativeGetAccessoryStrings()
{
    // NOT IMPLEMENTED
    return NULL;
}

IParcelFileDescriptor* UsbDeviceManager::NativeOpenAccessory()
{
    // NOT IMPLEMENTED
    return NULL;
}

Boolean UsbDeviceManager::NativeIsStartRequested()
{
    // NOT IMPLEMENTED
    return FALSE;
}

Int32 UsbDeviceManager::NativeGetAudioMode()
{
    // NOT IMPLEMENTED
    return -1;
}
