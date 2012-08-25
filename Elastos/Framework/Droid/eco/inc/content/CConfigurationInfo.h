
#ifndef __CCONFIGURATIONINFO_H__
#define __CCONFIGURATIONINFO_H__

#include "_CConfigurationInfo.h"

CarClass(CConfigurationInfo)
{
public:
    CConfigurationInfo()
        : mReqTouchScreen(0)
        , mReqKeyboardType(0)
        , mReqNavigation(0)
        , mReqInputFeatures(0)
        , mReqGlEsVersion(0) {}

    ~CConfigurationInfo() {}

public:
    CARAPI GetDescription(
        /* [out] */ String* des);

    CARAPI GetGlEsVersion(
        /* [out] */ String* ver);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IConfigurationInfo* orig);

    CARAPI constructor(
        /* [in] */ IParcel* source);

public:
    /**
     * The kind of touch screen attached to the device.
     * One of: {@link android.content.res.Configuration#TOUCHSCREEN_NOTOUCH},
     * {@link android.content.res.Configuration#TOUCHSCREEN_STYLUS},
     * {@link android.content.res.Configuration#TOUCHSCREEN_FINGER}.
     */
    Int32 mReqTouchScreen;

    /**
     * Application's input method preference.
     * One of: {@link android.content.res.Configuration#KEYBOARD_UNDEFINED},
     * {@link android.content.res.Configuration#KEYBOARD_NOKEYS},
     * {@link android.content.res.Configuration#KEYBOARD_QWERTY},
     * {@link android.content.res.Configuration#KEYBOARD_12KEY}
     */
    Int32 mReqKeyboardType;

    /**
     * A flag indicating whether any keyboard is available.
     * one of: {@link android.content.res.Configuration#NAVIGATION_UNDEFINED},
     * {@link android.content.res.Configuration#NAVIGATION_DPAD},
     * {@link android.content.res.Configuration#NAVIGATION_TRACKBALL},
     * {@link android.content.res.Configuration#NAVIGATION_WHEEL}
     */
    Int32 mReqNavigation;

    /**
     * Flags associated with the input features.  Any combination of
     * {@link #INPUT_FEATURE_HARD_KEYBOARD},
     * {@link #INPUT_FEATURE_FIVE_WAY_NAV}
     */
    Int32 mReqInputFeatures;

    /**
     * The GLES version used by an application. The upper order 16 bits represent the
     * major version and the lower order 16 bits the minor version.
     */
    Int32 mReqGlEsVersion;
};

#endif // __CCONFIGURATIONINFO_H__
