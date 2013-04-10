
#ifndef __USBSETTINGSMANAGER_DEVICEFILTER_H__
#define __USBSETTINGSMANAGER_DEVICEFILTER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

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
        /* [in] */ DeviceFilter* filter,
        /* [out] */ Boolean* result);

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

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash< AutoPtr<DeviceFilter> >
{
    size_t operator()(AutoPtr<DeviceFilter> name) const
    {
        assert(name != NULL);
        return (size_t)name.Get();
    }
};

template<> struct EqualTo< AutoPtr<DeviceFilter> >
{
    Boolean operator()(const AutoPtr<DeviceFilter>& x,
                       const AutoPtr<DeviceFilter>& y) const
    {
        return x.Get() == y.Get();
    }
};

_ELASTOS_NAMESPACE_END

#endif // __USBSETTINGSMANAGER_DEVICEFILTER_H__
