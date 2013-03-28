
#ifndef __USBSETTINGSMANAGER_H__
#define __USBSETTINGSMANAGER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include <elastos/HashMap.h>
#include <StringBuffer.h>
#include <Logger.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;
using namespace Elastos::Utility::Logging;

//_ELASTOS_NAMESPACE_BEGIN

class UsbSettingsManager : public ElRefBase
{
private:
    /*
     * This class is used to describe a USB device.
     * When used in HashMaps all values must be specified,
     * but wildcards can be used for any of the fields in
     * the package meta-data.
     */
    class DeviceFilter : public ElRefBase
    {
    public:
        DeviceFilter(
            /* [in] */ Int32 vid,
            /* [in] */ Int32 pid,
            /* [in] */ Int32 clasz,
            /* [in] */ Int32 subclass,
            /* [in] */ Int32 protocol);

        DeviceFilter(
            /* [in] */ IUsbDevice* device);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        /*
         * public static DeviceFilter read(XmlPullParser parser) throws XmlPullParserException, IOException {...}
         */
        static CARAPI Read(
            /* [in] */ IXmlPullParser* parser,
            /* [out] */ DeviceFilter** filter);

        /*
         * public void write(XmlSerializer serializer) throws IOException {...}
         */
        CARAPI Write(
            /* [in] */ IXmlSerializer* serializer);

        CARAPI_(Boolean) Matches(
            /* [in] */ IUsbDevice* device);

        CARAPI_(Boolean) Matches(
            /* [in] */ DeviceFilter* filter);

        CARAPI Equals(
            /* [in] */ IInterface* obj,
            /* [out] */ Boolean* result);

        CARAPI HashCode(
            /* [out] */ Int32* value);

        CARAPI ToString(
            /* [out] */ String* str);

    private:
        CARAPI_(Boolean) Matches(
            /* [in] */ Int32 clasz,
            /* [in] */ Int32 subclass,
            /* [in] */ Int32 protocol);

    private:
        // USB Vendor ID (or -1 for unspecified)
        Int32 mVendorId;

        // USB Product ID (or -1 for unspecified)
        Int32 mProductId;

        // USB device or interface class (or -1 for unspecified)
        Int32 mClass;

        // USB device subclass (or -1 for unspecified)
        Int32 mSubclass;

        // USB device protocol (or -1 for unspecified)
        Int32 mProtocol;
    };

    /*
     * This class is used to describe a USB accessory.
     * When used in HashMaps all values must be specified,
     * but wildcards can be used for any of the fields in
     * the package meta-data.
     */
    class AccessoryFilter : public ElRefBase
    {
    public:
        AccessoryFilter(
            /* [in] */ const String& manufacturer,
            /* [in] */ const String& model,
            /* [in] */ const String& ver);

        AccessoryFilter(
            /* [in] */ IUsbAccessory* accessory);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        /*
         * public static AccessoryFilter read(XmlPullParser parser) throws XmlPullParserException, IOException {...}
         */
        static CARAPI Read(
            /* [in] */ IXmlPullParser* parser,
            /* [out] */ AccessoryFilter** filter);

        /*
         * public void write(XmlSerializer serializer)throws IOException {...}
         */
        CARAPI Write(
            /* [in] */ IXmlSerializer* serializer);

        CARAPI_(Boolean) Matches(
            /* [in] */ IUsbAccessory* accessory);

        CARAPI_(Boolean) Matches(
            /* [in] */ AccessoryFilter* filter);

        CARAPI Equals(
            /* [in] */ IInterface* obj,
            /* [out] */ Boolean* result);

        CARAPI HashCode(
            /* [out] */ Int32* value);

        CARAPI ToString(
            /* [out] */ String* str);

    private:
        // USB accessory manufacturer (or null for unspecified)
        String mManufacturer;

        // USB accessory model (or null for unspecified)
        String mModel;

        // USB accessory version (or null for unspecified)
        String mVersion;
    };

    class MyPackageMonitor : public ElRefBase // private class MyPackageMonitor extends PackageMonitor
    {
    public:
        MyPackageMonitor(
            /* [in] */ UsbSettingsManager* host);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI_(void) OnPackageAdded(
            /* [in] */ const String& packageName,
            /* [in] */ Int32 uid);

        CARAPI_(void) OnPackageChanged(
            /* [in] */ const String& packageName,
            /* [in] */ Int32 uid,
            /* [in] */ const ArrayOf<String>& components);

        CARAPI_(void) OnPackageRemoved(
            /* [in] */ const String& packageName,
            /* [in] */ Int32 uid);

    private:
        AutoPtr<UsbSettingsManager> mHost;
    };

public:
    UsbSettingsManager(
        /* [in] */ IContext* context);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI_(void) DeviceAttached(
        /* [in] */ IUsbDevice* device);

    CARAPI_(void) DeviceDetached(
        /* [in] */ IUsbDevice* device);

    CARAPI_(void) AccessoryAttached(
        /* [in] */ IUsbAccessory* accessory);

    CARAPI_(void) AccessoryDetached(
        /* [in] */ IUsbAccessory* accessory);

    CARAPI_(Boolean) HasPermission(
        /* [in] */ IUsbDevice* device);

    CARAPI_(Boolean) HasPermission(
        /* [in] */ IUsbAccessory* accessory);

    CARAPI CheckPermission(
        /* [in] */ IUsbDevice* device);

    CARAPI CheckPermission(
        /* [in] */ IUsbAccessory* accessory);

    CARAPI_(void) RequestPermission(
        /* [in] */ IUsbDevice* device,
        /* [in] */ const String& packageName,
        /* [in] */ IPendingIntent* pi);

    CARAPI_(void) RequestPermission(
        /* [in] */ IUsbAccessory* accessory,
        /* [in] */ const String& packageName,
        /* [in] */ IPendingIntent* pi);

    CARAPI_(void) SetDevicePackage(
        /* [in] */ IUsbDevice* device,
        /* [in] */ const String& packageName);

    CARAPI_(void) SetAccessoryPackage(
        /* [in] */ IUsbAccessory* accessory,
        /* [in] */ const String& packageName);

    CARAPI_(void) GrantDevicePermission(
        /* [in] */ IUsbDevice* device,
        /* [in] */ Int32 uid);

    CARAPI_(void) GrantAccessoryPermission(
        /* [in] */ IUsbAccessory* accessory,
        /* [in] */ Int32 uid);

    CARAPI_(Boolean) HasDefaults(
        /* [in] */ const String& packageName);

    CARAPI_(void) ClearDefaults(
        /* [in] */ const String& packageName);

private:
    /*
     * private void readPreference(XmlPullParser parser) throws XmlPullParserException, IOException {...}
     */
    CARAPI ReadPreference(
        /* [in] */ IXmlPullParser* parser);

    CARAPI_(void) ReadSettingsLocked();

    CARAPI_(void) WriteSettingsLocked();

    CARAPI_(Boolean) PackageMatchesLocked(
        /* [in] */ IResolveInfo* info,
        /* [in] */ const String& metaDataName,
        /* [in] */ IUsbDevice* device,
        /* [in] */ IUsbAccessory* accessory);

    CARAPI_(ArrayOf<IResolveInfo*>*) GetDeviceMatchesLocked(
        /* [in] */ IUsbDevice* device,
        /* [in] */ IIntent* intent);

    CARAPI_(ArrayOf<IResolveInfo*>*) GetAccessoryMatchesLocked(
        /* [in] */ IUsbAccessory* accessory,
        /* [in] */ IIntent* intent);

    CARAPI_(void) ResolveActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ const ArrayOf<IResolveInfo*>& matches,
        /* [in] */ const String* defaultPackage,
        /* [in] */ IUsbDevice* device,
        /* [in] */ IUsbAccessory* accessory);

    CARAPI_(Boolean) ClearCompatibleMatchesLocked(
        /* [in] */ const String& packageName,
        /* [in] */ DeviceFilter* filter);

    CARAPI_(Boolean) ClearCompatibleMatchesLocked(
        /* [in] */ const String& packageName,
        /* [in] */ AccessoryFilter* filter);

    CARAPI_(Boolean) HandlePackageUpdateLocked(
        /* [in] */ const String& packageName,
        /* [in] */ IActivityInfo* aInfo,
        /* [in] */ const String& metaDataName);

    CARAPI_(void) HandlePackageUpdate(
        /* [in] */ const String& packageName);

    CARAPI_(void) RequestPermissionDialog(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& packageName,
        /* [in] */ IPendingIntent* pi);

    CARAPI_(Boolean) ClearPackageDefaultsLocked(
        /* [in] */ const String& packageName);

private:
    CARAPI_(Boolean) IsDeviceFilterExistsRef(
        /* [in] */ const String& packageName);

    CARAPI_(Boolean) IsAccessoryFilterExistsRef(
        /* [in] */ const String& packageName);

private:
    /*
    template<> struct Hash<AutoPtr<DeviceFilter> >
    {
        size_t operator()(AutoPtr<DeviceFilter> name) const
        {
            assert(name != NULL);
            return (size_t)name.Get();
        }
    };

    template<> struct EqualTo<AutoPtr<DeviceFilter> >
    {
        Boolean operator()(const AutoPtr<DeviceFilter>& x,
                           const AutoPtr<DeviceFilter>& y) const
        {
            return x.Get() == y.Get();
        }
    };

    template<> struct Hash<AutoPtr<AccessoryFilter> >
    {
        size_t operator()(AutoPtr<AccessoryFilter> name) const
        {
            assert(name != NULL);
            return (size_t)name.Get();
        }
    };

    template<> struct EqualTo<AutoPtr<AccessoryFilter> >
    {
        Boolean operator()(const AutoPtr<AccessoryFilter>& x,
                           const AutoPtr<AccessoryFilter>& y) const
        {
            return x.Get() == y.Get();
        }
    };
    */

    static const Boolean DEBUG;

    static const String TAG;

    static const String SETTINGS_FILE_PATH;

    // For usb_audio card and device number
    static const String USB_AUDIO_SOURCE_PATH;

    AutoPtr<IContext> mContext;

    AutoPtr<ILocalCapsuleManager> mPackageManager;

    // Temporary mapping USB device name to list of UIDs with permissions for the device
    HashMap< String, HashMap<Int32, Boolean> > mDevicePermissionMap;

    // Temporary mapping UsbAccessory to list of UIDs with permissions for the accessory
    HashMap< AutoPtr<IUsbAccessory>, HashMap<Int32, Boolean> > mAccessoryPermissionMap;

    // Maps DeviceFilter to user preferred application package
    HashMap< AutoPtr<DeviceFilter>, String > mDevicePreferenceMap;

    // Maps AccessoryFilter to user preferred application package
    HashMap< AutoPtr<AccessoryFilter>, String > mAccessoryPreferenceMap;

    Mutex mLock;

    AutoPtr<MyPackageMonitor> mPackageMonitor;
};

//_ELASTOS_NAMESPACE_END

#endif // __USBSETTINGSMANAGER_H__
