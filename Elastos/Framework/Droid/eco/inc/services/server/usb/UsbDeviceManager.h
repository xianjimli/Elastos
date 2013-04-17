
#ifndef __USBDEVICEMANAGER_H__
#define __USBDEVICEMANAGER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>
#include <elastos/List.h>
#include <elastos/Character.h>
#include <StringBuffer.h>
#include <Logger.h>
#include "os/SystemProperties.h"
#include "BroadcastReceiver.h"
#include "server/usb/UsbSettingsManager.h"
#include "server/usb/CommonUtil.h"

using namespace Elastos;
using namespace Elastos::Utility::Logging;

/**
 * UsbDeviceManager manages USB state in device mode.
 */
class UsbDeviceManager : public ElRefBase
{
private:
    class UsbBootCompletedReceiver;

    class AdbSettingsObserver : public ElRefBase, public IContentObserver
    {
    public:
        AdbSettingsObserver(
            /* [in] */ UsbDeviceManager* host);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnChange(
            /* [in] */ Boolean selfChange);

    private:
        AutoPtr<UsbDeviceManager> mHost;
    };

    /*
     * Listens for uevent messages from the kernel to monitor the USB state
     */
    class UDMUEventObserver : public ElRefBase
    {
    public:
        UDMUEventObserver(
            /* [in] */ UsbDeviceManager* host);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    private:
        AutoPtr<UsbDeviceManager> mHost;
    };

    /*
    private final UEventObserver mUEventObserver = new UEventObserver() {
        @Override
        public void onUEvent(UEventObserver.UEvent event) {
            if (DEBUG) Slog.v(TAG, "USB UEVENT: " + event.toString());

            String state = event.get("USB_STATE");
            String accessory = event.get("ACCESSORY");
            if (state != null) {
                mHandler.updateState(state);
            } else if ("START".equals(accessory)) {
                if (DEBUG) Slog.d(TAG, "got accessory start");
                startAccessoryMode();
            }
        }
    };
    */

    class UsbHandler : public ElRefBase, public IHandler
    {
    public:
        UsbHandler(
            /* [in] */ UsbDeviceManager* host);
            ///* [in] */ ILooper* looper);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI_(void) SendMessage(
            /* [in] */ Int32 what,
            /* [in] */ Boolean arg);

        CARAPI_(void) SendMessage(
            /* [in] */ Int32 what,
            /* [in] */ IInterface* arg);

        CARAPI_(void) SendMessage(
            /* [in] */ Int32 what,
            /* [in] */ IInterface* arg0,
            /* [in] */ Boolean arg1);

        CARAPI_(void) UpdateState(
            /* [in] */ const String& state);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

        CARAPI_(IUsbAccessory*) GetCurrentAccessory();

    private:
        CARAPI_(Boolean) WaitForState(
            /* [in] */ const String& state);

        CARAPI_(Boolean) SetUsbConfig(
            /* [in] */ const String& config);

        CARAPI_(void) SetAdbEnabled(
            /* [in] */ Boolean enable);

        CARAPI_(void) SetEnabledFunctions(
            /* [in] */ const String& functions,
            /* [in] */ Boolean makeDefault);

        CARAPI_(void) UpdateCurrentAccessory();

        CARAPI_(void) UpdateUsbState();

        CARAPI_(void) UpdateAudioSourceFunction();

        CARAPI_(void) UpdateUsbNotification();

        CARAPI_(Boolean) GetStorageSoundStatus();

        CARAPI_(void) PlayDefaultSound();

        CARAPI_(void) UpdateAdbNotification();

    private:
        CARAPI_(void) PerformAccessoryAttachedRef(
            /* [in] */ IUsbAccessory* accessory);

        CARAPI_(void) PerformAccessoryDetachedRef(
            /* [in] */ IUsbAccessory* accessory);

        CARAPI_(void) PerformAdbNotificationShownRef(
            /* [in] */ Int32 id);

        CARAPI_(void) CancelAdbNotificationShownRef(
            /* [in] */ Int32 id);

        CARAPI_(Int32) RetrieveNotificationIdRef();

        CARAPI_(void) ClearOldNotificationRef();

        CARAPI_(void) SendNewNotificationRef();

        CARAPI_(void) EnableFunctionsWithBootModeRef(
            /* [in] */ const String& functions,
            /* [in] */ Boolean makeDefault);

        CARAPI_(void) EnableFunctionsWithOemSpecificModeRef(
            /* [in] */ const String& functions,
            /* [in] */ Boolean makeDefault);

        CARAPI_(String) AddOrRemoveAdbFunctionRef(
            /* [in] */ const String& functions);

    private:
        // current USB state
        Boolean mConnected;
        Boolean mConfigured;
        Boolean mAdbNotificationShown;

        String mCurrentFunctions;
        String mDefaultFunctions;

        Int32 mUsbNotificationId;

        AutoPtr<IUsbAccessory> mCurrentAccessory;
        AutoPtr<UsbBootCompletedReceiver> mBootCompletedReceiver;
        AutoPtr<UsbDeviceManager> mHost;
    };

    class UsbBootCompletedReceiver : public BroadcastReceiver
    {
    public:
        UsbBootCompletedReceiver(
            /* [in] */ UsbHandler* host);

    protected:
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        AutoPtr<UsbHandler> mHost;
    };

public:
    UsbDeviceManager(
        /* [in] */ IContext* context,
        /* [in] */ UsbSettingsManager* settingsManager);

    ~UsbDeviceManager();

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI_(void) SystemReady();

    /* returns the currently attached USB accessory */
    CARAPI_(IUsbAccessory*) GetCurrentAccessory();

    /* opens the currently attached USB accessory */
    CARAPI OpenAccessory(
        /* [in] */ IUsbAccessory* accessory,
        /* [out] */ IParcelFileDescriptor** pfd);

    CARAPI_(void) SetCurrentFunctions(
        /* [in] */ const String& functions,
        /* [in] */ Boolean makeDefault);

    CARAPI_(void) SetMassStorageBackingFile(
        /* [in] */ const String& path);

private:
    static CARAPI_(void) InitRndisAddress();

    static CARAPI_(String) AddFunction(
        /* [in] */ const String& functions,
        /* [in] */ const String& function);

    static CARAPI_(String) RemoveFunction(
        /* [in] */ const String& functions,
        /* [in] */ const String& function);

    static CARAPI_(Boolean) ContainsFunction(
        /* [in] */ const String& functions,
        /* [in] */ const String& function);

private:
    CARAPI_(void) StartAccessoryMode();

    CARAPI_(void) ReadOemUsbOverrideConfig();

    CARAPI_(Boolean) NeedsOemUsbOverride();

    CARAPI_(String) ProcessOemUsbOverride(
        /* [in] */ const String& usbFunctions);

    CARAPI_(ArrayOf<String>*) NativeGetAccessoryStrings();

    CARAPI_(IParcelFileDescriptor*) NativeOpenAccessory();

    CARAPI_(Boolean) NativeIsStartRequested();

    CARAPI_(Int32) NativeGetAudioMode();

private:
    CARAPI_(Boolean) IsBootCompletedRef();

    CARAPI_(Boolean) HasUsbAccessoryRef();

    CARAPI_(Boolean) IsUsbNotificationUsedRef();

    CARAPI_(void) SetAdbEnabledRef(
        /* [in] */ Boolean enabled);

    CARAPI_(Boolean) IsAdbEnabledRef();

    CARAPI_(void) ResetAccessoryStringsRef();

    CARAPI_(ArrayOf<String>*) GetAccessoryStringsRef();

    CARAPI_(void) GetOemModeListRef(
        /* [in] */ const String& mode,
        /* [out] */ List< Pair<String, String> >** list);

    CARAPI_(Boolean) IsOemModeExistsRef(
        /* [in] */ const String& mode);

private:
    static const Boolean DEBUG;

    static const String TAG;
    static const String USB_STATE_MATCH;
    static const String ACCESSORY_START_MATCH;
    static const String FUNCTIONS_PATH;
    static const String STATE_PATH;
    static const String MASS_STORAGE_FILE_PATH;
    static const String RNDIS_ETH_ADDR_PATH;
    static const String AUDIO_SOURCE_PCM_PATH;
    static const String BOOT_MODE_PROPERTY;

    static const Int32 MSG_UPDATE_STATE;
    static const Int32 MSG_ENABLE_ADB;
    static const Int32 MSG_SET_CURRENT_FUNCTIONS;
    static const Int32 MSG_SYSTEM_READY;
    static const Int32 MSG_BOOT_COMPLETED;
    static const Int32 AUDIO_MODE_NONE;
    static const Int32 AUDIO_MODE_SOURCE;

    // Delay for debouncing USB disconnects.
    // We often get rapid connect/disconnect events when enabling USB functions,
    // which need debouncing.
    static const Int32 UPDATE_DELAY;

    AutoPtr<UsbHandler> mHandler;
    AutoPtr<IContext> mContext;
    AutoPtr<IContentResolver> mContentResolver;
    AutoPtr<UsbSettingsManager> mSettingsManager;
    AutoPtr<INotificationManager> mNotificationManager;
    AutoPtr<UDMUEventObserver> mUEventObserver;

    Boolean mBootCompleted;
    Boolean mHasUsbAccessory;
    Boolean mUseUsbNotification;
    Boolean mAdbEnabled;
    Boolean mAudioSourceEnabled;

    ArrayOf<String>* mAccessoryStrings;
    HashMap< String, List< Pair<String, String> > >* mOemModeMap;
};

#endif // __USBDEVICEMANAGER_H__
