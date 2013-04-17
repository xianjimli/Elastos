
#include "server/usb/UsbDeviceManager.h"
#include "os/SystemProperties.h"
#include <Elastos.IO.h>

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
    Boolean enable = FALSE;
    // RYAN
    // Java
    // boolean enable = (Settings.Secure.getInt(mContentResolver, Settings.Secure.ADB_ENABLED, 0) > 0);

    // RYAN
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

UsbDeviceManager::~UsbDeviceManager()
{
    if (mOemModeMap == NULL) {
        return;
    }

    mOemModeMap->Clear();
    delete mOemModeMap;
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

    CString arg = CString(functions.string());
    mHandler->SendMessage(MSG_SET_CURRENT_FUNCTIONS, (IInterface*)&arg, makeDefault);
}

void UsbDeviceManager::SetMassStorageBackingFile(
    /* [in] */ const String& path)
{
    // NOT IMPLEMENTED
}

void UsbDeviceManager::InitRndisAddress()
{
    // configure RNDIS ethernet address based on our serial number using the same algorithm
    // we had been previously using in kernel board files
    Int32 ETH_ALEN = 6;
    Int32* address = new Int32[ETH_ALEN];

    // first byte is 0x02 to signify a locally administered address
    address[0] = 0x02;

    String serial = SystemProperties::Get("ro.serialno", "1234567890ABCDEF");

    Int32 serialLength = serial.GetLength();

    // XOR the USB serial across the remaining 5 bytes
    for (Int32 i = 0; i < serialLength; i++) {
        address[i % (ETH_ALEN - 1) + 1] ^= (Int32)serial.GetChar(i);
    }

    // RYAN
    //String addrString = String.format("%02X:%02X:%02X:%02X:%02X:%02X",
    //    address[0], address[1], address[2], address[3], address[4], address[5]);

    String addrString;

    AutoPtr<IFile> file;
    CFile::New(RNDIS_ETH_ADDR_PATH, (IFile**)&file);

    ECode ec;
    IOutputStreamWriter* writer = NULL;
    ec = CFileWriter::New(file, (IOutputStreamWriter**)&writer);

    ec = writer->WriteString(addrString);

    if (FAILED(ec)) {
        StringBuffer buf;
        buf += "failed to write to ";
        buf += RNDIS_ETH_ADDR_PATH;

        Logger::E(UsbDeviceManager::TAG, (String)buf);
        writer->Release();
        return;
    }

    writer->Close();
    writer->Release();
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
    ArrayOf<String>* strArray;
    CommonUtil::SplitString(functions, ',', &strArray);

    for (Int32 i = 0, size = strArray->GetLength(); i < size; i++) {
        if (function != (*strArray)[i]) {
            continue;
        }

        (*strArray)[i] = NULL;
    }

    if (strArray->GetLength() == 1 && (*strArray)[0] == NULL) {
        return String("none");
    }

    StringBuffer buf;

    for (Int32 i = 0, size = strArray->GetLength(); i < size; i++) {
        String s = (*strArray)[i];

        if (s == NULL) {
            continue;
        }

        if (buf.GetLength() > 0) {
            buf += ",";
        }

        buf += s;
    }

    return (String)buf;
}

Boolean UsbDeviceManager::ContainsFunction(
    /* [in] */ const String& functions,
    /* [in] */ const String& function)
{
    Int32 index = functions.IndexOf(function);

    if (index < 0) {
        return FALSE;
    }

    Char32 c1;
    Character::GetCharAt(functions, (index - 1), &c1);
    if (index > 0 && c1 != ',') {
        return FALSE;
    }

    Int32 charAfter = index + function.GetLength();

    Char32 c2 = 0;
    Character::GetCharAt(functions, charAfter, &c2);

    if (charAfter < functions.GetLength() && c2 != ',') {
        return FALSE;
    }

    return TRUE;
}

void UsbDeviceManager::StartAccessoryMode()
{
    mAccessoryStrings = NativeGetAccessoryStrings();

    Boolean enableAudio = (NativeGetAudioMode() == AUDIO_MODE_SOURCE);

    // don't start accessory mode if our mandatory strings have not been set
    Boolean enableAccessory = (mAccessoryStrings != NULL &&
                               mAccessoryStrings[UsbAccessory_MANUFACTURER_STRING] != NULL &&
                               mAccessoryStrings[UsbAccessory_MODEL_STRING] != NULL);

    String functions = (String)NULL;

    if (enableAccessory && enableAudio) {
        StringBuffer buf;
        buf += UsbManager_USB_FUNCTION_ACCESSORY;
        buf += ",";
        buf += UsbManager_USB_FUNCTION_AUDIO_SOURCE;

        functions = (String)buf;
    } else if (enableAccessory) {
        functions = UsbManager_USB_FUNCTION_ACCESSORY;
    } else if (enableAudio) {
        functions = UsbManager_USB_FUNCTION_AUDIO_SOURCE;
    }

    if (functions == NULL) {
        return;
    }

    SetCurrentFunctions(functions, FALSE);
}

void UsbDeviceManager::ReadOemUsbOverrideConfig()
{
    ArrayOf<String>* configList;
    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);
    resources->GetStringArray(0x0107002e /* com.android.internal.R.array.config_oemUsbModeOverride */, &configList);

    if (configList == NULL) {
        return;
    }

    for (Int32 i = 0, size = configList->GetLength(); i < size; i++) {
        String config = (*configList)[i];

        ArrayOf<String>* items;
        CommonUtil::SplitString(config, ':', &items);

        if (items->GetLength() != 3) {
            continue;
        }

        if (mOemModeMap == NULL) {
            mOemModeMap == new HashMap< String, List< Pair<String, String> > >();
        }

        List< Pair<String, String> >* overrideList;
        GetOemModeListRef((*items)[0], &overrideList);

        if (overrideList == NULL) {
            overrideList = new List< Pair<String, String> >;
            (*mOemModeMap)[(*items)[0]] = *overrideList;
        }

        overrideList->PushBack(Pair<String, String>((*items)[1], (*items)[2]));
    }
}

Boolean UsbDeviceManager::NeedsOemUsbOverride()
{
    if (mOemModeMap == NULL) {
        return FALSE;
    }

    String bootMode = SystemProperties::Get(BOOT_MODE_PROPERTY, "unknown");
    return (IsOemModeExistsRef(bootMode) == TRUE) ? TRUE : FALSE;
}

String UsbDeviceManager::ProcessOemUsbOverride(
    /* [in] */ const String& usbFunctions)
{
    if ((usbFunctions == NULL) || (mOemModeMap == NULL)) {
        return usbFunctions;
    }

    String bootMode = SystemProperties::Get(BOOT_MODE_PROPERTY, "unknown");

    List< Pair<String, String> >* overrides;
    GetOemModeListRef(bootMode, &overrides);

    if (overrides == NULL) {
        return usbFunctions;
    }

    List< Pair<String, String> >::Iterator it;
    for (it = overrides->Begin(); it != overrides->End(); ++it)
    {
        Pair<String, String> pair = *it;

        if (pair.mFirst != usbFunctions) {
            continue;
        }

        StringBuffer buf;
        buf += "OEM USB override: ";
        buf += pair.mFirst;
        buf += " ==> ";
        buf += pair.mSecond;

        Logger::D(UsbDeviceManager::TAG, (String)buf);
        return pair.mSecond;
    }

    return usbFunctions;
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
    /* [in] */ const String& mode,
    /* [out] */ List< Pair<String, String> >** list)
{
    HashMap< String, List< Pair<String, String> > >::Iterator it = mOemModeMap->Find(mode);

    if (it == mOemModeMap->End()) {
        *list = NULL;
        return;
    }

    *list = &(it->mSecond);
    return;
}

Boolean UsbDeviceManager::IsOemModeExistsRef(
    /* [in] */ const String& mode)
{
    HashMap< String, List< Pair<String, String> > >::Iterator it = mOemModeMap->Find(mode);

    if (it == mOemModeMap->End()) {
        return FALSE;
    }

    List< Pair<String, String> > value = it->mSecond;
    return TRUE;
}
