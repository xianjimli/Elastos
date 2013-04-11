
#ifndef __USBSETTINGSMANAGER_ACCESSORYFILTER_H__
#define __USBSETTINGSMANAGER_ACCESSORYFILTER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

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
        /* [in] */ AccessoryFilter* filter,
        /* [out] */ Boolean* result);

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

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash< AutoPtr<AccessoryFilter> >
{
    size_t operator()(AutoPtr<AccessoryFilter> name) const
    {
        assert(name != NULL);
        return (size_t)name.Get();
    }
};

template<> struct EqualTo< AutoPtr<AccessoryFilter> >
{
    Boolean operator()(const AutoPtr<AccessoryFilter>& x,
                       const AutoPtr<AccessoryFilter>& y) const
    {
        return x.Get() == y.Get();
    }
};

_ELASTOS_NAMESPACE_END

#endif // __USBSETTINGSMANAGER_ACCESSORYFILTER_H__
