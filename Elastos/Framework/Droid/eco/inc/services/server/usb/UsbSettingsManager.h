
#ifndef __USBSETTINGSMANAGER_H__
#define __USBSETTINGSMANAGER_H__

#include "ext/frameworkext.h"
#include "utils/XmlUtils.h"
#include "server/usb/DeviceFilter.h"
#include "server/usb/AccessoryFilter.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include <elastos/HashMap.h>
#include <StringBuffer.h>
#include <Logger.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;
using namespace Elastos::Utility::Logging;

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<IUsbAccessory> >
{
    size_t operator()(AutoPtr<IUsbAccessory> name) const
    {
        return (size_t)name.Get();
    }
};

template<> struct EqualTo<AutoPtr<IUsbAccessory> >
{
    Boolean operator()(const AutoPtr<IUsbAccessory>& x,
                       const AutoPtr<IUsbAccessory>& y) const
    {
        return x.Get() == y.Get();
    }
};

_ELASTOS_NAMESPACE_END

class UsbSettingsManager : public ElRefBase
{
public:
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

    ~UsbSettingsManager();

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
    CARAPI_(String) GetDevicePreferenceRef(
        /* [in] */ DeviceFilter* filter);

    CARAPI_(String) GetAccessoryPreferenceRef(
        /* [in] */ AccessoryFilter* filter);

    CARAPI_(void) RemoveDevicePreferenceRef(
        /* [in] */ const String& packageName,
        /* [out] */ Boolean* result);

    CARAPI_(void) RemoveDevicePreferenceRef(
        /* [in] */ DeviceFilter* filter,
        /* [out] */ Boolean* result);

    CARAPI_(void) RemoveAccessoryPreferenceRef(
        /* [in] */ const String& packageName,
        /* [out] */ Boolean* result);

    CARAPI_(void) RemoveAccessoryPreferenceRef(
        /* [in] */ AccessoryFilter* filter,
        /* [out] */ Boolean* result);

    CARAPI_(Boolean) IsDevicePreferenceExistsRef(
        /* [in] */ const String& packageName);

    CARAPI_(Boolean) IsAccessoryPreferenceExistsRef(
        /* [in] */ const String& packageName);

private:
    static const Boolean DEBUG;

    static const String TAG;

    static const String SETTINGS_FILE_PATH;

    // For usb_audio card and device number
    static const String USB_AUDIO_SOURCE_PATH;

    AutoPtr<IContext> mContext;

    AutoPtr<ILocalCapsuleManager> mPackageManager;

    // Temporary mapping USB device name to list of UIDs with permissions for the device
    HashMap< String, HashMap<Int32, Boolean>* >* mDevicePermissionMap;

    // Temporary mapping UsbAccessory to list of UIDs with permissions for the accessory
    //HashMap< AutoPtr<IUsbAccessory>, HashMap<Int32, Boolean> > mAccessoryPermissionMap;
    HashMap< AutoPtr<IUsbAccessory>, HashMap<Int32, Boolean>* >* mAccessoryPermissionMap;

    // Maps DeviceFilter to user preferred application package
    HashMap< AutoPtr<DeviceFilter>, String >* mDevicePreferenceMap;

    // Maps AccessoryFilter to user preferred application package
    HashMap< AutoPtr<AccessoryFilter>, String >* mAccessoryPreferenceMap;

    Mutex mLock;

    AutoPtr<MyPackageMonitor> mPackageMonitor;
};

#endif // __USBSETTINGSMANAGER_H__
