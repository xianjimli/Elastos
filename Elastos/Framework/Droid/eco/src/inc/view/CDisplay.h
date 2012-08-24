
#ifndef __CDISPLAY_H__
#define __CDISPLAY_H__

#include "_CDisplay.h"

CarClass(CDisplay)
{
public:
    /**
     * Specify the default Display
     */
    static const Int32 DEFAULT_DISPLAY = 0;

public:
    /**
     * Returns the number of displays connected to the device.  This is
     * currently undefined; do not use.
     */
    static Int32 GetDisplayCount();

public:
    /**
     * Use {@link android.view.WindowManager#getDefaultDisplay()
     * WindowManager.getDefaultDisplay()} to create a Display object.
     * Display gives you access to some information about a particular display
     * connected to the device.
     */
    CARAPI constructor(
        /* [in] */ Int32 display);

    /**
     * Returns the index of this display.  This is currently undefined; do
     * not use.
    */
    CARAPI GetDisplayId(
        /* [out] */ Int32* id);

    /**
     * Returns the raw width of the display, in pixels.  Note that this
     * should <em>not</em> generally be used for computing layouts, since
     * a device will typically have screen decoration (such as a status bar)
     * along the edges of the display that reduce the amount of application
     * space available from the raw size returned here.  This value is
     * adjusted for you based on the current rotation of the display.
     */
    CARAPI GetWidth(
         /* [out] */ Int32 *width);

    /**
     * Returns the raw height of the display, in pixels.  Note that this
     * should <em>not</em> generally be used for computing layouts, since
     * a device will typically have screen decoration (such as a status bar)
     * along the edges of the display that reduce the amount of application
     * space available from the raw size returned here.  This value is
     * adjusted for you based on the current rotation of the display.
     */
    CARAPI GetHeight(
        /* [out] */ Int32 *height);

    /**
     * Returns the rotation of the screen from its "natural" orientation.
     * The returned value may be {@link Surface#ROTATION_0 Surface.ROTATION_0}
     * (no rotation), {@link Surface#ROTATION_90 Surface.ROTATION_90},
     * {@link Surface#ROTATION_180 Surface.ROTATION_180}, or
     * {@link Surface#ROTATION_270 Surface.ROTATION_270}.  For
     * example, if a device has a naturally tall screen, and the user has
     * turned it on its side to go into a landscape orientation, the value
     * returned here may be either {@link Surface#ROTATION_90 Surface.ROTATION_90}
     * or {@link Surface#ROTATION_270 Surface.ROTATION_270} depending on
     * the direction it was turned.  The angle is the rotation of the drawn
     * graphics on the screen, which is the opposite direction of the physical
     * rotation of the device.  For example, if the device is rotated 90
     * degrees counter-clockwise, to compensate rendering will be rotated by
     * 90 degrees clockwise and thus the returned value here will be
     * {@link Surface#ROTATION_90 Surface.ROTATION_90}.
     */
    CARAPI GetRotation(
        /* [out] */ Int32* rotation);

    /**
     * Return the native pixel format of the display.  The returned value
     * may be one of the constants int {@link android.graphics.PixelFormat}.
     */
    CARAPI GetPixelFormat(
        /* [out] */ Int32* pixelFormat);

    /**
     * Return the refresh rate of this display in frames per second.
     */
    CARAPI GetRefreshRate(
        /* [out] */ Float* refreshRate);

    /**
     * Initialize a DisplayMetrics object from this display's data.
     *
     * @param outMetrics
     */
    CARAPI GetMetrics(
        /* [in, out] */ IDisplayMetrics* outMetrics);

    static CARAPI CreateMetricsBasedDisplay(
        /* [in] */ Int32 displayId,
        /* [in] */ IDisplayMetrics* metrics,
        /* [out] */ IDisplay** display);

private:
    Int32 mDisplay;
    Int32 mPixelFormat;
    Float mRefreshRate;
    Float mDensity;
    Float mDpiX;
    Float mDpiY;
};

#endif //__CDISPLAY_H__
