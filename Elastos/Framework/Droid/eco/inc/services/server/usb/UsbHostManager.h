
#ifndef __USBHOSTMANAGER_H__
#define __USBHOSTMANAGER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include <elastos/HashMap.h>
#include <Logger.h>
#include <usbhost/usbhost.h>
#include "server/usb/UsbSettingsManager.h"

using namespace Elastos;
using namespace Elastos::Core::Threading;
using namespace Elastos::Utility::Logging;

/**
 * UsbHostManager manages USB state in host mode.
 */
class UsbHostManager : public ElRefBase
{
private:
    class SRRunnable : public ElRefBase, public IRunnable
    {
    public:
        SRRunnable(
            /* [in] */ UsbHostManager* host);

        CARAPI Run();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    private:
        AutoPtr<UsbHostManager> mHost;
    };

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
    CARAPI OpenDevice(
        /* [in] */ const String& deviceName,
        /* [out] */ IParcelFileDescriptor** pfd);

private:
    CARAPI_(Boolean) IsBlackListed(
        /* [in] */ const String& deviceName);

    /* returns true if the USB device should not be accessible by applications */
    CARAPI_(Boolean) IsBlackListed(
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
        /* [in] */ const ArrayOf<Int32>& interfaceValues,
        /* [in] */ const ArrayOf<Int32>& endpointValues);

    /* Called from JNI in monitorUsbHostBus to report USB device removal */
    CARAPI_(void) UsbDeviceRemoved(
        /* [in] */ const String& deviceName);

    CARAPI_(void) NativeMonitorUsbHostBus();

    CARAPI_(IParcelFileDescriptor*) NativeOpenDevice(
        /* [in] */ const String& deviceName);

private:
    CARAPI_(Boolean) IsDeviceExistsRef(
        /* [in] */ const String& deviceName);

private:
    static const String TAG;
    static const Boolean LOG;

    // contains all connected USB devices
    HashMap< String, AutoPtr<IUsbDevice> > mDevices;

    // USB busses to exclude from USB host support
    ArrayOf<String>* mHostBlacklist;

    AutoPtr<IContext> mContext;

    Mutex mLock;

    AutoPtr<UsbSettingsManager> mSettingsManager;
};

#endif // __USBHOSTMANAGER_H__
