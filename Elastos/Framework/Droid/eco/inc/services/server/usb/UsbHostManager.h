
#ifndef __USBHOSTMANAGER_H__
#define __USBHOSTMANAGER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include "server/usb/UsbSettingsManager.h"

/**
 * UsbHostManager manages USB state in host mode.
 */
class UsbHostManager : public ElRefBase
{
public:
    UsbHostManager(
        /* [in] */ IContext* context,
        /* [in] */ UsbSettingsManager* settingsManager);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI_(void) SystemReady();

    /* Returns a list of all currently attached USB devices */
    CARAPI_(void) GetDeviceList(
        /* [in] */ IBundle* devices);

    /* Opens the specified USB device */
    CARAPI_(IParcelFileDescriptor*) OpenDevice(
        /* [in] */ const String& deviceName);

    CARAPI_(void) Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw);

private:
    CARAPI_(Boolean) IsBlackListedByName(
        /* [in] */ const String& deviceName);


    /* returns true if the USB device should not be accessible by applications */
    CARAPI_(Boolean) IsBlackListedByUSBClass(
        /* [in] */ Int32 clazz,
        /* [in] */ Int32 subClass,
        /* [in] */ Int32 protocol);


    /*
     * Called from JNI in monitorUsbHostBus() to report new USB devices
     *
     * array of quintuples containing id, class, subclass, protocol and number of endpoints for each interface
     * array of quadruples containing address, attributes, max packet size and interval for each endpoint
     */
    CARAPI_(void) UsbDeviceAdded(
        /* [in] */ const String& deviceName,
        /* [in] */ Int32 vendorID,
        /* [in] */ Int32 productID,
        /* [in] */ Int32 deviceClass,
        /* [in] */ Int32 deviceSubclass,
        /* [in] */ Int32 deviceProtocol,
        /* [in] */ ArrayOf<Int32>* interfaceValues,
        /* [in] */ ArrayOf<Int32>* endpointValues);

    /* Called from JNI in monitorUsbHostBus to report USB device removal */
    CARAPI_(void) UsbDeviceRemoved(
        /* [in] */ const String& deviceName);

    CARAPI_(void) NativeMonitorUsbHostBus();

    CARAPI_(IParcelFileDescriptor*) NativeOpenDevice(
        /* [in] */ const String& deviceName);

private:
    static const String TAG;

    // contains all connected USB devices
    AutoPtr<IObjectStringMap> mDevices;

    // USB busses to exclude from USB host support
    ArrayOf<String>* mHostBlacklist;

    AutoPtr<IContext> mContext;

    AutoPtr<IInterface> mLock;

    AutoPtr<UsbSettingsManager> mSettingsManager;
};

#endif // __USBHOSTMANAGER_H__
