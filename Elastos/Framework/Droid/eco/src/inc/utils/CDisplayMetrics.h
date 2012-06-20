#ifndef __CDISPLAYMETRICS_H__
#define __CDISPLAYMETRICS_H__

#include "_CDisplayMetrics.h"
#include "ext/frameworkdef.h"

CarClass(CDisplayMetrics)
{
public:
    /**
    * Standard quantized DPI for low-density screens.
    */
    static const Int32 DENSITY_LOW = 120;

    /**
    * Standard quantized DPI for medium-density screens.
    */
    static const Int32 DENSITY_MEDIUM = 160;

    /**
    * Standard quantized DPI for high-density screens.
    */
    static const Int32 DENSITY_HIGH = 240;

    /**
    * Standard quantized DPI for extra-high-density screens.
    */
    static const Int32 DENSITY_XHIGH = 320;

    /**
    * The reference density used throughout the system.
    */
    static const Int32 DENSITY_DEFAULT = DENSITY_MEDIUM;

    /**
    * The device's density.
    * @hide becase eventually this should be able to change while
    * running, so shouldn't be a constant.
    */
    static const Int32 DENSITY_DEVICE;

public:
    CARAPI SetTo(
        /* [in] */ IDisplayMetrics* o);

    CARAPI SetToDefaults();

    /**
    * Update the display metrics based on the compatibility info and orientation
    * NOTE: DO NOT EXPOSE THIS API!  It is introducing a circular dependency
    * with the higher-level android.res package.
    * {@hide}
    */
    CARAPI UpdateMetrics(
        /* [in] */ ICompatibilityInfo* compatibilityInfo,
        /* [in] */ Int32 orientation,
        /* [in] */ Int32 screenLayout);

    CARAPI GetDescription(
        /* [out] */ String* str);

private:
    static CARAPI_(Int32) GetDeviceDensity();

public:
    /**
    * The absolute width of the display in pixels.
    */
    Int32 mWidthPixels;

    /**
    * The absolute height of the display in pixels.
    */
    Int32 mHeightPixels;

    /**
    * The logical density of the display.  This is a scaling factor for the
    * Density Independent Pixel unit, where one DIP is one pixel on an
    * approximately 160 dpi screen (for example a 240x320, 1.5"x2" screen),
    * providing the baseline of the system's display. Thus on a 160dpi screen
    * this density value will be 1; on a 120 dpi screen it would be .75; etc.
    *
    * <p>This value does not exactly follow the real screen size (as given by
    * {@link #xdpi} and {@link #ydpi}, but rather is used to scale the size of
    * the overall UI in steps based on gross changes in the display dpi.  For
    * example, a 240x320 screen will have a density of 1 even if its width is
    * 1.8", 1.3", etc. However, if the screen resolution is increased to
    * 320x480 but the screen size remained 1.5"x2" then the density would be
    * increased (probably to 1.5).
    *
    * @see #DENSITY_DEFAULT
    */
    Float mDensity;

    /**
    * The screen density expressed as dots-per-inch.  May be either
    * {@link #DENSITY_LOW}, {@link #DENSITY_MEDIUM}, or {@link #DENSITY_HIGH}.
    */
    Int32 mDensityDpi;

    /**
    * A scaling factor for fonts displayed on the display.  This is the same
    * as {@link #density}, except that it may be adjusted in smaller
    * increments at runtime based on a user preference for the font size.
    */
    Float mScaledDensity;

    /**
    * The exact physical pixels per inch of the screen in the X dimension.
    */
    Float mXdpi;

    /**
    * The exact physical pixels per inch of the screen in the Y dimension.
    */
    Float mYdpi;
};
#endif //__CDISPLAYMETRICS_H__
