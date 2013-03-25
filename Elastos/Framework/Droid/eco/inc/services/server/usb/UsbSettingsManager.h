
#ifndef __USBSETTINGSMANAGER_H__
#define __USBSETTINGSMANAGER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>

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

    /*
    private class MyPackageMonitor extends PackageMonitor {

        public void onPackageAdded(String packageName, int uid) {
            handlePackageUpdate(packageName);
        }

        public void onPackageChanged(String packageName, int uid, String[] components) {
            handlePackageUpdate(packageName);
        }

        public void onPackageRemoved(String packageName, int uid) {
            clearDefaults(packageName);
        }
    }
    */

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

    CARAPI_(void) CheckPermission(
        /* [in] */ IUsbDevice* device);

    CARAPI_(void) CheckPermission(
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
    static const Boolean DEBUG;

    static const String TAG;

    // For usb_audio card and device number
    static const String USB_AUDIO_SOURCE_PATH;

    static const String SETTINGS_FILE_PATH;

    AutoPtr<IContext> mContext;
    AutoPtr<ICapsuleManager> mPackageManager;

    // Temporary mapping USB device name to list of UIDs with permissions for the device
    //private final HashMap<String, SparseBooleanArray> mDevicePermissionMap = new HashMap<String, SparseBooleanArray>();

    // Temporary mapping UsbAccessory to list of UIDs with permissions for the accessory
    //private final HashMap<UsbAccessory, SparseBooleanArray> mAccessoryPermissionMap = new HashMap<UsbAccessory, SparseBooleanArray>();

    // Maps DeviceFilter to user preferred application package
    HashMap< AutoPtr<DeviceFilter>, String > mDevicePreferenceMap;

    // Maps AccessoryFilter to user preferred application package
    HashMap< AutoPtr<AccessoryFilter>, String > mAccessoryPreferenceMap;

    AutoPtr<IInterface> mLock;

    //MyPackageMonitor mPackageMonitor = new MyPackageMonitor();;
};

#endif // __USBSETTINGSMANAGER_H__
