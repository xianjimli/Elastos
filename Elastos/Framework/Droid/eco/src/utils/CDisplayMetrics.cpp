
#include "utils/CDisplayMetrics.h"
#include "content/CCompatibilityInfo.h"

const Int32 CDisplayMetrics::DENSITY_DEVICE = CDisplayMetrics::GetDeviceDensity();
const Int32 CDisplayMetrics::DENSITY_LOW;
const Int32 CDisplayMetrics::DENSITY_MEDIUM;
const Int32 CDisplayMetrics::DENSITY_HIGH;
const Int32 CDisplayMetrics::DENSITY_XHIGH;
const Int32 CDisplayMetrics::DENSITY_DEFAULT;

ECode CDisplayMetrics::SetTo(
    /* [in] */ IDisplayMetrics* o)
{
    if (o == NULL) {
        return E_INVALID_ARGUMENT;
    }

    CDisplayMetrics* dm = (CDisplayMetrics*)o;
    mWidthPixels = dm->mWidthPixels;
    mHeightPixels = dm->mHeightPixels;
    mDensity = dm->mDensity;
    mDensityDpi = dm->mDensityDpi;
    mScaledDensity = dm->mScaledDensity;
    mXdpi = dm->mXdpi;
    mYdpi = dm->mYdpi;

    return NOERROR;
}

ECode CDisplayMetrics::SetToDefaults()
{
    mWidthPixels = 0;
    mHeightPixels = 0;
    mDensity = DENSITY_DEVICE / (Float) DENSITY_DEFAULT;
    mDensityDpi = DENSITY_DEVICE;
    mScaledDensity = mDensity;
    mXdpi = DENSITY_DEVICE;
    mYdpi = DENSITY_DEVICE;

    return NOERROR;
}

/**
* Update the display metrics based on the compatibility info and orientation
* NOTE: DO NOT EXPOSE THIS API!  It is introducing a circular dependency
* with the higher-level android.res package.
* {@hide}
*/
ECode CDisplayMetrics::UpdateMetrics(
    /* [in] */ ICompatibilityInfo* compatibilityInfo,
    /* [in] */ Int32 orientation,
    /* [in] */ Int32 screenLayout)
{
    assert(compatibilityInfo != NULL);

    Boolean expandable = FALSE;
    Boolean largeScreens = FALSE;
    Boolean xlargeScreens = FALSE;
    compatibilityInfo->IsConfiguredExpandable(&expandable);
    compatibilityInfo->IsConfiguredLargeScreens(&largeScreens);
    compatibilityInfo->IsConfiguredXLargeScreens(&largeScreens);

    // Note: this assume that configuration is updated before calling
    // updateMetrics method.
    if (!expandable) {
        if ((screenLayout&Configuration_SCREENLAYOUT_COMPAT_NEEDED) == 0) {
            expandable = TRUE;
            // the current screen size is compatible with non-resizing apps.
            //
            compatibilityInfo->SetExpandable(TRUE);
        }
        else {
            compatibilityInfo->SetExpandable(FALSE);
        }
    }

    if (!largeScreens) {
        if ((screenLayout&Configuration_SCREENLAYOUT_SIZE_MASK)
            != Configuration_SCREENLAYOUT_SIZE_LARGE) {
            largeScreens = TRUE;
            // the current screen size is not large.
            //
            compatibilityInfo->SetLargeScreens(TRUE);
        }
        else {
            compatibilityInfo->SetLargeScreens(FALSE);
        }
    }

    if (!xlargeScreens) {
        if ((screenLayout&Configuration_SCREENLAYOUT_SIZE_MASK)
            != Configuration_SCREENLAYOUT_SIZE_XLARGE) {
            xlargeScreens = TRUE;
            // the current screen size is not large.
            compatibilityInfo->SetXLargeScreens(TRUE);
        }
        else {
            compatibilityInfo->SetXLargeScreens(FALSE);
        }
    }

    if (!expandable || (!largeScreens && !xlargeScreens)) {
        // This is a larger screen device and the app is not
        // compatible with large screens, so diddle it.

        // Figure out the compatibility width and height of the screen.
        Int32 defaultWidth;
        Int32 defaultHeight;

        switch (orientation) {
            case Configuration_ORIENTATION_LANDSCAPE:
            {
                defaultWidth = (Int32)(CompatibilityInfo::DEFAULT_PORTRAIT_HEIGHT
                    * mDensity + 0.5f);
                defaultHeight = (Int32)(CompatibilityInfo::DEFAULT_PORTRAIT_WIDTH
                    * mDensity + 0.5f);
            }
            break;
            case Configuration_ORIENTATION_UNDEFINED:
            {
                // don't change
                return NOERROR;
            }
            case Configuration_ORIENTATION_PORTRAIT:
            case Configuration_ORIENTATION_SQUARE:
            default:
            {
                defaultWidth = (Int32)(CompatibilityInfo::DEFAULT_PORTRAIT_WIDTH
                    * mDensity + 0.5f);
                defaultHeight = (Int32)(CompatibilityInfo::DEFAULT_PORTRAIT_HEIGHT
                    * mDensity + 0.5f);
            }
      }

      if (defaultWidth < mWidthPixels) {
          // content/window's x offset in original pixels
          mWidthPixels = defaultWidth;
      }
      if (defaultHeight < mHeightPixels) {
          mHeightPixels = defaultHeight;
      }
    }

    Boolean isRequired = FALSE;
    compatibilityInfo->IsScalingRequired(&isRequired);
    if (isRequired) {
        Float invertedRatio = ((CCompatibilityInfo*)compatibilityInfo)->mApplicationInvertedScale;
        mDensity *= invertedRatio;
        mDensityDpi = (Int32)((mDensity * DENSITY_DEFAULT) + 0.5f);
        mScaledDensity *= invertedRatio;
        mXdpi *= invertedRatio;
        mYdpi *= invertedRatio;
        mWidthPixels = (Int32)(mWidthPixels * invertedRatio + 0.5f);
        mHeightPixels = (Int32)(mHeightPixels * invertedRatio + 0.5f);
    }

    return NOERROR;
}

ECode CDisplayMetrics::GetDescription(
        /* [out] */ String * str)
{
//	    return "DisplayMetrics{density=" + density + ", width=" + widthPixels +
//	        ", height=" + heightPixels + ", scaledDensity=" + scaledDensity +
//	        ", xdpi=" + xdpi + ", ydpi=" + ydpi + "}";
    return E_NOT_IMPLEMENTED;
}

Int32 CDisplayMetrics::GetDeviceDensity()
{
    // qemu.sf.lcd_density can be used to override ro.sf.lcd_density
    // when running in the emulator, allowing for dynamic configurations.
    // The reason for this is that ro.sf.lcd_density is write-once and is
    // set by the init process when it parses build.prop before anything else.
    //return SystemProperties.getInt("qemu.sf.lcd_density",
    //    SystemProperties.getInt("ro.sf.lcd_density", DENSITY_DEFAULT));
    return DENSITY_DEFAULT;
}
