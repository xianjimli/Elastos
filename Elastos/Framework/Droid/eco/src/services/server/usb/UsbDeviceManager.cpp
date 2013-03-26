
#include "server/usb/UsbDeviceManager.h"

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

UsbDeviceManager::AdbSettingsObserver::AdbSettingsObserver(
    /* [in] */ UsbDeviceManager* host)
{
    mHost = host;
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

ECode UsbDeviceManager::AdbSettingsObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    Boolean enable;
    // Java
    // boolean enable = (Settings.Secure.getInt(mContentResolver, Settings.Secure.ADB_ENABLED, 0) > 0);

    // Elastos
    // Settings.Secure.getInt(mHost->GetContentResolverRef(), SettingsSecure_ADB_ENABLED);
    mHost->mHandler->SendMessage(MSG_ENABLE_ADB, enable);

    return NOERROR;
}

UsbDeviceManager::UDMUEventObserver::UDMUEventObserver(
    /* [in] */ UsbDeviceManager* host)
{
    mHost = host;
}

UInt32 UsbDeviceManager::UDMUEventObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 UsbDeviceManager::UDMUEventObserver::Release()
{
    return ElRefBase::Release();
}

ECode UsbDeviceManager::UDMUEventObserver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

UsbDeviceManager::UsbHandler::UsbHandler(
    /* [in] */ UsbDeviceManager* host)
{


    mHost = host;
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
    return mCurrentAccessory;
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
    if (DEBUG) {
        StringBuffer buf;
        buf += "setUsbConfig(";
        buf += config;
        buf += ")";

        Logger::E(UsbDeviceManager::TAG, (String)buf);
    }

    // set the new configuration
    SystemProperties::Set("sys.usb.config", config);
    return WaitForState(config);
}

void UsbDeviceManager::UsbHandler::SetAdbEnabled(
    /* [in] */ Boolean enable)
{
    if (DEBUG) {
        StringBuffer buf;
        buf += "setAdbEnabled: ";
        buf += enable;

        Logger::E(UsbDeviceManager::TAG, (String)buf);
    }

    if (enable == mHost->IsAdbEnabledRef()) {
        return;
    }

    mHost->SetAdbEnabledRef(enable);

    // Due to the persist.sys.usb.config property trigger, changing adb state requires
    // switching to default function
    SetEnabledFunctions(mDefaultFunctions, true);
    UpdateAdbNotification();
}

void UsbDeviceManager::UsbHandler::SetEnabledFunctions(
    /* [in] */ const String& functions,
    /* [in] */ Boolean makeDefault)
{
    // Do not update persystent.sys.usb.config if the device is booted up
    // with OEM specific mode.
    if (functions != NULL && makeDefault ==TRUE && mHost->NeedsOemUsbOverride() == FALSE)
    {
        EnableFunctionsWithBootModeRef(functions, makeDefault);
        return;
    }

    EnableFunctionsWithOemSpecificModeRef(functions, makeDefault);
    return;
}

void UsbDeviceManager::UsbHandler::UpdateCurrentAccessory()
{
    if (mHost->HasUsbAccessoryRef() == FALSE) {
        return;
    }

    if (mConfigured == TRUE) {
        PerformAccessoryAttachedRef(mCurrentAccessory);
        return;
    }

    PerformAccessoryDetachedRef(mCurrentAccessory);
    return;
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
    if (mHost->mNotificationManager == NULL || mHost->IsUsbNotificationUsedRef() == FALSE) {
        return;
    }

    Int32 id = 0;
    AutoPtr<IResources> resources;
    mHost->mContext->GetResources((IResources**)&resources);

    if (mConnected == TRUE) {
        id = RetrieveNotificationIdRef();
    }

    if (mUsbNotificationId != id) {
        ClearOldNotificationRef();
        SendNewNotificationRef();
    }
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
    if (mHost->mNotificationManager == NULL) {
        return;
    }

    Int32 id = 0x01040448; // com.android.internal.R.string.adb_active_notification_title;

    if (mHost->mAdbEnabled == TRUE && mConnected == TRUE) {
        PerformAdbNotificationShownRef(id);
        return;
    }

    if (mAdbNotificationShown == TRUE) {
        CancelAdbNotificationShownRef(id);
        return;
    }

    // NOT IMPLEMENTED
}

void UsbDeviceManager::UsbHandler::PerformAccessoryAttachedRef(
    /* [in] */ IUsbAccessory* accessory)
{
    //if (mHost->GetAccessoryStringsRef() == NULL) {
    if (mHost->mAccessoryStrings == NULL) {
        Logger::E(UsbDeviceManager::TAG, "nativeGetAccessoryStrings failed");
        return;
    }

    CUsbAccessory::New(*(mHost->GetAccessoryStringsRef()), (IUsbAccessory**)&mCurrentAccessory);

    // Slog.d(TAG, "entering USB accessory mode: " + mCurrentAccessory);
    Logger::E(UsbDeviceManager::TAG, "entering USB accessory mode: ");

    // defer accessoryAttached if system is not ready
    if (mHost->IsBootCompletedRef() == FALSE) {
        return;
    }

    // else handle in mBootCompletedReceiver
    mHost->mSettingsManager->AccessoryAttached(mCurrentAccessory);
}

void UsbDeviceManager::UsbHandler::PerformAccessoryDetachedRef(
    /* [in] */ IUsbAccessory* accessory)
{
    // make sure accessory mode is off
    // and restore default functions
    Logger::E(UsbDeviceManager::TAG, "exited USB accessory mode");
    SetEnabledFunctions(mDefaultFunctions, false);

    if (mCurrentAccessory == NULL) {
        return;
    }

    if (mHost->IsBootCompletedRef() == TRUE) {
        mHost->mSettingsManager->AccessoryDetached(mCurrentAccessory);
    }

    mCurrentAccessory = NULL;
    mHost->ResetAccessoryStringsRef();
}

void UsbDeviceManager::UsbHandler::PerformAdbNotificationShownRef(
    /* [in] */ Int32 id)
{
    // NOT IMPLEMENTED
}

void UsbDeviceManager::UsbHandler::CancelAdbNotificationShownRef(
    /* [in] */ Int32 id)
{
    // NOT IMPLEMENTED
}

Int32 UsbDeviceManager::UsbHandler::RetrieveNotificationIdRef()
{
    // NOT IMPLEMENTED
    return -1;
}

void UsbDeviceManager::UsbHandler::ClearOldNotificationRef()
{
    // NOT IMPLEMENTED
}

void UsbDeviceManager::UsbHandler::SendNewNotificationRef()
{
    // NOT IMPLEMENTED
}

void UsbDeviceManager::UsbHandler::EnableFunctionsWithBootModeRef(
    /* [in] */ const String& functions,
    /* [in] */ Boolean makeDefault)
{
    String targetFunctions = AddOrRemoveAdbFunctionRef(functions);

    if (mDefaultFunctions == targetFunctions) {
        return;
    }

    String config = String("none");
    if (SetUsbConfig(config) == FALSE) {
        Logger::E(UsbDeviceManager::TAG, "Failed to disable USB");

        // revert to previous configuration if we fail
        SetUsbConfig(mCurrentFunctions);
        return;
    }

    // setting this property will also change the current USB state
    // via a property trigger
    SystemProperties::Set("persist.sys.usb.config", targetFunctions);

    if (WaitForState(targetFunctions) == FALSE) {
        StringBuffer buf;
        buf += "Failed to switch persistent USB config to ";
        buf += targetFunctions;

        Logger::E(UsbDeviceManager::TAG, (String)buf);

        // revert to previous configuration if we fail
        SystemProperties::Set("persist.sys.usb.config", mDefaultFunctions);
        return;
    }

    mCurrentFunctions = targetFunctions;
    mDefaultFunctions = targetFunctions;
    return;
}

void UsbDeviceManager::UsbHandler::EnableFunctionsWithOemSpecificModeRef(
    /* [in] */ const String& functions,
    /* [in] */ Boolean makeDefault)
{
    String targetFunctions;

    if (functions == NULL) {
        targetFunctions = mDefaultFunctions;
    } else {
        targetFunctions = functions;
    }

    // Override with bootmode specific usb mode if needed
    targetFunctions = mHost->ProcessOemUsbOverride(targetFunctions);

    targetFunctions = AddOrRemoveAdbFunctionRef(targetFunctions);

    if (mCurrentFunctions == targetFunctions) {
        return;
    }

    String config = String("none");
    if (SetUsbConfig(config) == FALSE) {
        Logger::E(UsbDeviceManager::TAG, "Failed to disable USB");

        // revert to previous configuration if we fail
        SetUsbConfig(mCurrentFunctions);
        return;
    }

    if (SetUsbConfig(targetFunctions) == FALSE) {
        StringBuffer buf;
        buf += "Failed to switch USB config to ";
        buf += targetFunctions;
        Logger::E(UsbDeviceManager::TAG, (String)buf);

        // revert to previous configuration if we fail
        SetUsbConfig(mCurrentFunctions);
        return;
    }

    mCurrentFunctions = targetFunctions;
    return;
}

CARAPI_(String) UsbDeviceManager::UsbHandler::AddOrRemoveAdbFunctionRef(
    /* [in] */ const String& functions)
{
    if (mHost->IsAdbEnabledRef() == TRUE) {
        String function = String(UsbManager_USB_FUNCTION_ADB);
        return UsbDeviceManager::AddFunction(functions, function);
    }

    String function = String(UsbManager_USB_FUNCTION_ADB);
    return UsbDeviceManager::RemoveFunction(functions, function);
}

UsbDeviceManager::UsbBootCompletedReceiver::UsbBootCompletedReceiver(
    /* [in] */ UsbHandler* host)
{
    mHost = host;
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
    return mHandler->GetCurrentAccessory();
}

ECode UsbDeviceManager::OpenAccessory(
    /* [in] */ IUsbAccessory* accessory,
    /* [out] */ IParcelFileDescriptor** pfd)
{
    AutoPtr<IUsbAccessory> currentAccessory = mHandler->GetCurrentAccessory();

    if (currentAccessory == NULL) {
        // throw new IllegalArgumentException("no accessory attached");
        Logger::E(UsbDeviceManager::TAG, "no accessory attached");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    CUsbAccessory* lAccessory = (CUsbAccessory*)IUsbAccessory::Probe(accessory);
    CUsbAccessory* rAccessory = (CUsbAccessory*)IUsbAccessory::Probe(currentAccessory);

    Boolean result = FALSE;
    //lAccessory->Equals(rAccessory, &result);
    if (result == FALSE) {
        // String error = accessory.toString() + " does not match current accessory " + currentAccessory;
        // throw new IllegalArgumentException(error);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mSettingsManager->CheckPermission(accessory);
    *pfd = NativeOpenAccessory();
    return NOERROR;
}

void UsbDeviceManager::SetCurrentFunctions(
    /* [in] */ const String& functions,
    /* [in] */ Boolean makeDefault)
{
    if (DEBUG) {
        StringBuffer buf;
        buf += "setCurrentFunctions(";
        buf += functions;

        buf += ") default: ";
        buf += makeDefault;

        Logger::D(UsbDeviceManager::TAG, (String)buf);
    }

    //mHandler->SendMessage(MSG_SET_CURRENT_FUNCTIONS, functions, makeDefault);

    // NOT IMPLEMENTED
}

void UsbDeviceManager::SetMassStorageBackingFile(
    /* [in] */ const String& path)
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
    StringBuffer buf;

    if (functions == "none") {
        buf += function;
        return (String)buf;
    }

    if (ContainsFunction(functions, function) == TRUE) {
        buf += functions;
        return (String)buf;
    }

    if (functions.GetLength() > 0) {
        buf += functions;
        buf += ",";
    }

    buf += function;
    return (String)buf;
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
    Int32 index = functions.IndexOf(function);

    if (index < 0) {
        return false;
    }







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
    // if (mOemModeMap == null) return false;

    String bootMode = SystemProperties::Get(BOOT_MODE_PROPERTY, "unknown");
    return (IsOemModeExistsRef(bootMode) == TRUE) ? TRUE : FALSE;
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

Boolean UsbDeviceManager::IsBootCompletedRef()
{
    return mBootCompleted;
}

Boolean UsbDeviceManager::HasUsbAccessoryRef()
{
    return mHasUsbAccessory;
}

Boolean UsbDeviceManager::IsUsbNotificationUsedRef()
{
    return mUseUsbNotification;
}

void UsbDeviceManager::SetAdbEnabledRef(
    /* [in] */ Boolean enabled)
{
    mAdbEnabled = enabled;
}

Boolean UsbDeviceManager::IsAdbEnabledRef()
{
    return mAdbEnabled;
}

void UsbDeviceManager::ResetAccessoryStringsRef()
{
    mAccessoryStrings = NULL;
}

ArrayOf<String>* UsbDeviceManager::GetAccessoryStringsRef()
{
    return mAccessoryStrings;
}

void UsbDeviceManager::GetOemModeListRef(
    /* [in] */ const String& mode)
{
    //return NULL;
}

Boolean UsbDeviceManager::IsOemModeExistsRef(
    /* [in] */ const String& mode)
{
    HashMap< String, List< Pair<String, String> > >::Iterator it = mOemModeMap.Find(mode);

    if (it == mOemModeMap.End()) {
        return FALSE;
    }

    List< Pair<String, String> > value = it->mSecond;
    return TRUE;
}
