
#ifndef __CAPSULEITEMINFO_H__
#define __CAPSULEITEMINFO_H__

#include <ext/frameworkext.h>
#include <elastos.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

/**
 * Base class containing information common to all package items held by
 * the package manager.  This provides a very common basic set of attributes:
 * a label, icon, and meta-data.  This class is not intended
 * to be used by itself; it is simply here to share common definitions
 * between all items returned by the package manager.  As such, it does not
 * itself implement Parcelable, but does provide convenience methods to assist
 * in the implementation of Parcelable in subclasses.
 */
class CapsuleItemInfo
{
public:
    CapsuleItemInfo();

    virtual ~CapsuleItemInfo();

    CARAPI constructor(
        /* [in] */ CapsuleItemInfo* orig);

    /**
     * Retrieve the current textual label associated with this item.  This
     * will call back on the given PackageManager to load the label from
     * the application.
     *
     * @param pm A PackageManager from which the label can be loaded; usually
     * the PackageManager from which you originally retrieved this item.
     *
     * @return Returns a CharSequence containing the item's label.  If the
     * item does not have a label, its name is returned.
     */
    virtual CARAPI LoadLabel(
        /* [in] */ ICapsuleManager* pm,
        /* [out] */ ICharSequence** label);

    /**
     * Retrieve the current graphical icon associated with this item.  This
     * will call back on the given PackageManager to load the icon from
     * the application.
     *
     * @param pm A PackageManager from which the icon can be loaded; usually
     * the PackageManager from which you originally retrieved this item.
     *
     * @return Returns a Drawable containing the item's icon.  If the
     * item does not have an icon, the item's default icon is returned
     * such as the default activity icon.
     */
    virtual CARAPI LoadIcon(
        /* [in] */ ICapsuleManager* pm,
        /* [out] */ IDrawable** icon);

    /**
     * Retrieve the current graphical logo associated with this item. This
     * will call back on the given PackageManager to load the logo from
     * the application.
     *
     * @param pm A PackageManager from which the logo can be loaded; usually
     * the PackageManager from which you originally retrieved this item.
     *
     * @return Returns a Drawable containing the item's logo. If the item
     * does not have a logo, this method will return null.
     */
    virtual CARAPI LoadLogo(
        /* [in] */ ICapsuleManager* pm,
        /* [out] */ IDrawable** icon);

    /**
     * Load an XML resource attached to the meta-data of this item.  This will
     * retrieved the name meta-data entry, and if defined call back on the
     * given PackageManager to load its XML file from the application.
     *
     * @param pm A PackageManager from which the XML can be loaded; usually
     * the PackageManager from which you originally retrieved this item.
     * @param name Name of the meta-date you would like to load.
     *
     * @return Returns an XmlPullParser you can use to parse the XML file
     * assigned as the given meta-data.  If the meta-data name is not defined
     * or the XML resource could not be found, null is returned.
     */
    virtual CARAPI LoadXmlMetaData(
        /* [in] */ ICapsuleManager* pm,
        /* [in] */ const String& name,
        /* [out] */ IXmlResourceParser** resource);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

public:
    /**
     * Public name of this item. From the "android:name" attribute.
     */
    String mName;

    /**
     * Name of the capsule that this item is in.
     */
    String mCapsuleName;

    /**
     * A string resource identifier (in the package's resources) of this
     * component's label.  From the "label" attribute or, if not set, 0.
     */
    Int32 mLabelRes;

    /**
     * The string provided in the AndroidManifest file, if any.  You
     * probably don't want to use this.  You probably want
     * {@link PackageManager#getApplicationLabel}
     */
    AutoPtr<ICharSequence> mNonLocalizedLabel;

    /**
     * A drawable resource identifier (in the package's resources) of this
     * component's icon.  From the "icon" attribute or, if not set, 0.
     */
    Int32 mIcon;

    /**
     * A drawable resource identifier (in the package's resources) of this
     * component's logo. Logos may be larger/wider than icons and are
     * displayed by certain UI elements in place of a name or name/icon
     * combination. From the "logo" attribute or, if not set, 0.
     */
    Int32 mLogo;

    /**
     * Additional meta-data associated with this component.  This field
     * will only be filled in if you set the
     * {@link PackageManager#GET_META_DATA} flag when requesting the info.
     */
    AutoPtr<IBundle> mMetaData;
};

#endif //__CAPSULEITEMINFO_H__
