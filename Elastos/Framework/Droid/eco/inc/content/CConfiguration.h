
#ifndef __CCONFIGURATION_H__
#define __CCONFIGURATION_H__

#include "_CConfiguration.h"
#include <elastos/Locale.h>

using namespace Elastos::Utility;

CarClass(CConfiguration)
{
public:
    CConfiguration();

    ~CConfiguration();

public:
    CARAPI SetTo(
        /* [in] */ IConfiguration* o);

    CARAPI SetToDefaults();

    CARAPI UpdateFrom(
        /* [in] */ IConfiguration* delta,
        /* [out] */ Int32* changes);

    CARAPI Diff(
        /* [in] */ IConfiguration* delta,
        /* [out] */ Boolean* isChanged);

    CARAPI IsOtherSeqNewer(
        /* [in] */ IConfiguration* other,
        /* [out] */ Boolean* isBetter);

    CARAPI Equals(
        /* [in] */ IConfiguration* that,
        /* [out] */ Boolean* isEqual);

    CARAPI EqualsEx(
        /* [in] */ IInterface* that,
        /* [out] */ Boolean* isEqual);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI GetDescription(
        /* [out] */ String* description);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IConfiguration* o);

    CARAPI constructor(
        /* [in] */ IParcel* source);

public:
    CARAPI_(Int32) Diff(
        /* [in] */ CConfiguration* delta);

public:
    /**
    * Current user preference for the scaling factor for fonts, relative
    * to the base density scaling.
    */
    Float mFontScale;

    /**
    * IMSI MCC (Mobile Country Code).  0 if undefined.
    */
    Int32 mMcc;

    /**
    * IMSI MNC (Mobile Network Code).  0 if undefined.
    */
    Int32 mMnc;

    /**
     * Current user preference for the locale.
     */
    Locale* mLocale;

    /**
    * Current user preference for the locale.
    */
    //Locale mLocale;

    /**
    * Locale should persist on setting.  This is hidden because it is really
    * questionable whether this is the right way to expose the functionality.
    * @hide
    */
    Boolean mUserSetLocale;

    /**
    * Bit mask of overall layout of the screen.  Currently there are two
    * fields:
    * <p>The {@link #SCREENLAYOUT_SIZE_MASK} bits define the overall size
    * of the screen.  They may be one of
    * {@link #SCREENLAYOUT_SIZE_SMALL}, {@link #SCREENLAYOUT_SIZE_NORMAL},
    * {@link #SCREENLAYOUT_SIZE_LARGE}.
    *
    * <p>The {@link #SCREENLAYOUT_LONG_MASK} defines whether the screen
    * is wider/taller than normal.  They may be one of
    * {@link #SCREENLAYOUT_LONG_NO} or {@link #SCREENLAYOUT_LONG_YES}.
    */
    Int32 mScreenLayout;

    /**
    * The kind of touch screen attached to the device.
    * One of: {@link #TOUCHSCREEN_NOTOUCH}, {@link #TOUCHSCREEN_STYLUS},
    * {@link #TOUCHSCREEN_FINGER}.
    */
    Int32 mTouchscreen;

    /**
    * The kind of keyboard attached to the device.
    * One of: {@link #KEYBOARD_NOKEYS}, {@link #KEYBOARD_QWERTY},
    * {@link #KEYBOARD_12KEY}.
    */
    Int32 mKeyboard;

    /**
    * A flag indicating whether any keyboard is available.  Unlike
    * {@link #hardKeyboardHidden}, this also takes Int32o account a soft
    * keyboard, so if the hard keyboard is hidden but there is soft
    * keyboard available, it will be set to NO.  Value is one of:
    * {@link #KEYBOARDHIDDEN_NO}, {@link #KEYBOARDHIDDEN_YES}.
    */
    Int32 mKeyboardHidden;

    /**
    * A flag indicating whether the hard keyboard has been hidden.  This will
    * be set on a device with a mechanism to hide the keyboard from the
    * user, when that mechanism is closed.  One of:
    * {@link #HARDKEYBOARDHIDDEN_NO}, {@link #HARDKEYBOARDHIDDEN_YES}.
    */
    Int32 mHardKeyboardHidden;

    /**
    * The kind of navigation method available on the device.
    * One of: {@link #NAVIGATION_NONAV}, {@link #NAVIGATION_DPAD},
    * {@link #NAVIGATION_TRACKBALL}, {@link #NAVIGATION_WHEEL}.
    */
    Int32 mNavigation;

    /**
    * A flag indicating whether any 5-way or DPAD navigation available.
    * This will be set on a device with a mechanism to hide the navigation
    * controls from the user, when that mechanism is closed.  One of:
    * {@link #NAVIGATIONHIDDEN_NO}, {@link #NAVIGATIONHIDDEN_YES}.
    */
    Int32 mNavigationHidden;

    /**
    * Overall orientation of the screen.  May be one of
    * {@link #ORIENTATION_LANDSCAPE}, {@link #ORIENTATION_PORTRAIT},
    * or {@link #ORIENTATION_SQUARE}.
    */
    Int32 mOrientation;

    /**
    * Bit mask of the ui mode.  Currently there are two fields:
    * <p>The {@link #UI_MODE_TYPE_MASK} bits define the overall ui mode of the
    * device. They may be one of {@link #UI_MODE_TYPE_UNDEFINED},
    * {@link #UI_MODE_TYPE_NORMAL}, {@link #UI_MODE_TYPE_DESK},
    * or {@link #UI_MODE_TYPE_CAR}.
    *
    * <p>The {@link #UI_MODE_NIGHT_MASK} defines whether the screen
    * is in a special mode. They may be one of {@link #UI_MODE_NIGHT_UNDEFINED},
    * {@link #UI_MODE_NIGHT_NO} or {@link #UI_MODE_NIGHT_YES}.
    */
    Int32 mUiMode;

    /**
     * @hide Internal book-keeping.
     */
    Int32 mSeq;

//	    public static final Parcelable.Creator<Configuration> CREATOR
//	            = new Parcelable.Creator<Configuration>() {
};

#endif // __CCONFIGURATION_H__
