
#include <surfaceflinger/SurfaceComposerClient.h>
#include <ui/DisplayInfo.h>
#include "view/CDisplay.h"
#include "utils/CDisplayMetrics.h"
#include "ext/frameworkerr.h"

const Int32 CDisplay::DEFAULT_DISPLAY;

ECode CDisplay::constructor(
    /* [in] */ Int32 display)
{
#ifdef _linux
    mDisplay = display;

    android::DisplayInfo info;
    android::status_t err = android::SurfaceComposerClient::getDisplayInfo(
            android::DisplayID(display), &info);
    if (err < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mPixelFormat = info.pixelFormatInfo.format;
    mRefreshRate = info.fps;
    mDensity = info.density;
    mDpiX = info.xdpi;
    mDpiY = info.ydpi;
    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

/**
 * Returns the index of this display.  This is currently undefined; do
 * not use.
 */
ECode CDisplay::GetDisplayId(
    /* [out] */ Int32* id)
{
    if (id == NULL) return E_INVALID_ARGUMENT;

    *id = mDisplay;
    return NOERROR;
}

Int32 CDisplay::GetDisplayCount()
{
#ifdef _linux
    return android::SurfaceComposerClient::getNumberOfDisplays();
#else
    return -1; // E_NOT_IMPLEMENTED
#endif
}

/**
 * Returns the raw width of the display, in pixels.  Note that this
 * should <em>not</em> generally be used for computing layouts, since
 * a device will typically have screen decoration (such as a status bar)
 * along the edges of the display that reduce the amount of application
 * space available from the raw size returned here.  This value is
 * adjusted for you based on the current rotation of the display.
 */
ECode CDisplay::GetWidth(
     /* [out] */ Int32 *width)
{
#ifdef _linux
    if (width == NULL) return E_INVALID_ARGUMENT;

    android::DisplayID dpy = mDisplay;
    *width = android::SurfaceComposerClient::getDisplayWidth(dpy);

    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

/**
 * Returns the raw height of the display, in pixels.  Note that this
 * should <em>not</em> generally be used for computing layouts, since
 * a device will typically have screen decoration (such as a status bar)
 * along the edges of the display that reduce the amount of application
 * space available from the raw size returned here.  This value is
 * adjusted for you based on the current rotation of the display.
 */
ECode CDisplay::GetHeight(
    /* [out] */ Int32 *height)
{
#ifdef _linux
    if (height == NULL) return E_INVALID_ARGUMENT;

    android::DisplayID dpy = mDisplay;
    *height = android::SurfaceComposerClient::getDisplayHeight(dpy);

    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

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
ECode CDisplay::GetRotation(
    /* [out] */ Int32* rotation)
{
#ifdef _linux
    if (rotation == NULL) return E_INVALID_ARGUMENT;

    android::DisplayID dpy = mDisplay;
    *rotation = android::SurfaceComposerClient::getDisplayOrientation(dpy);
    return NOERROR;
#else
    return E_NOT_IMPLEMENTED;
#endif
}

/**
 * Return the native pixel format of the display.  The returned value
 * may be one of the constants int {@link android.graphics.PixelFormat}.
 */
ECode CDisplay::GetPixelFormat(
    /* [out] */ Int32* pixelFormat)
{
    if (pixelFormat == NULL) return E_INVALID_ARGUMENT;

    *pixelFormat = mPixelFormat;
    return NOERROR;
}

/**
 * Return the refresh rate of this display in frames per second.
 */
ECode CDisplay::GetRefreshRate(
    /* [out] */ Float* refreshRate)
{
    if (refreshRate == NULL) return E_INVALID_ARGUMENT;

    *refreshRate = mRefreshRate;
    return NOERROR;
}

/**
 * Initialize a DisplayMetrics object from this display's data.
 *
 * @param outMetrics
 */
ECode CDisplay::GetMetrics(
    /* [in, out] */ IDisplayMetrics* _outMetrics)
{
    CDisplayMetrics* outMetrics = (CDisplayMetrics*)_outMetrics;
    GetWidth(&(outMetrics->mWidthPixels));
    GetHeight(&(outMetrics->mHeightPixels));
    outMetrics->mDensity = mDensity;
    outMetrics->mDensityDpi = (Int32)((mDensity * DisplayMetrics_DENSITY_DEFAULT) + 0.5f);
    outMetrics->mScaledDensity = outMetrics->mDensity;
    outMetrics->mXdpi = mDpiX;
    outMetrics->mYdpi = mDpiY;
    return NOERROR;
}

ECode CDisplay::CreateMetricsBasedDisplay(
    /* [in] */ Int32 displayId,
    /* [in] */ IDisplayMetrics* metrics,
    /* [out] */ IDisplay** display)
{
//	    return new CompatibleDisplay(displayId, metrics, &display);
    return E_NOT_IMPLEMENTED;
}
