#ifndef __COMPATIBILITYINFO_H__
#define __COMPATIBILITYINFO_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

/**
* CompatibilityInfo class keeps the information about compatibility mode that the application is
* running under.
*
*  {@hide}
*/
class CompatibilityInfo
{
private:
    /** default compatibility info object for compatible applications */
    static AutoPtr<ICompatibilityInfo> DEFAULT_COMPATIBILITY_INFO;

public:
    static CARAPI GetDefaultCompatibilityInfo(
        /* [out] */ ICompatibilityInfo** info);

    /**
     * The default width of the screen in portrait mode.
     */
    static const Int32 DEFAULT_PORTRAIT_WIDTH = 320;

    /**
     * The default height of the screen in portrait mode.
     */
    static const Int32 DEFAULT_PORTRAIT_HEIGHT = 480;

    static const char* TAG;

private:
    static const Boolean DBG = FALSE;

    /**
     * A flag mask to tell if the application needs scaling (when mApplicationScale != 1.0f)
     * {@see compatibilityFlag}
     */
    static const Int32 SCALING_REQUIRED = 1;

    /**
     * A flag mask to indicates that the application can expand over the original size.
     * The flag is set to true if
     * 1) Application declares its expandable in manifest file using <supports-screens> or
     * 2) Configuration.SCREENLAYOUT_COMPAT_NEEDED is not set
     * {@see compatibilityFlag}
     */
    static const Int32 EXPANDABLE = 2;

    /**
     * A flag mask to tell if the application is configured to be expandable. This differs
     * from EXPANDABLE in that the application that is not expandable will be
     * marked as expandable if Configuration.SCREENLAYOUT_COMPAT_NEEDED is not set.
     */
    static const Int32 CONFIGURED_EXPANDABLE = 4;

    /**
     * A flag mask to indicates that the application supports large screens.
     * The flag is set to true if
     * 1) Application declares it supports large screens in manifest file using <supports-screens> or
     * 2) The screen size is not large
     * {@see compatibilityFlag}
     */
    static const Int32 LARGE_SCREENS = 8;

    /**
     * A flag mask to tell if the application supports large screens. This differs
     * from LARGE_SCREENS in that the application that does not support large
     * screens will be marked as supporting them if the current screen is not
     * large.
     */
    static const Int32 CONFIGURED_LARGE_SCREENS = 16;

    /**
     * A flag mask to indicates that the application supports xlarge screens.
     * The flag is set to true if
     * 1) Application declares it supports xlarge screens in manifest file using <supports-screens> or
     * 2) The screen size is not xlarge
     * {@see compatibilityFlag}
     */
    static const Int32 XLARGE_SCREENS = 32;

    /**
     * A flag mask to tell if the application supports xlarge screens. This differs
     * from XLARGE_SCREENS in that the application that does not support xlarge
     * screens will be marked as supporting them if the current screen is not
     * xlarge.
     */
    static const Int32 CONFIGURED_XLARGE_SCREENS = 64;

public:
    /**
    * A helper object to translate the screen and window coordinates back and forth.
    * @hide
    */
    class Translator : public ElRefBase, public ITranslator
    {
        friend class CompatibilityInfo;
    protected:
        Translator(
            /* [in] */ Float applicationScale,
            /* [in] */ Float applicationInvertedScale);

    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI TranslateRectInScreenToAppWinFrame(
            /* [in] */ IRect* rect);

        CARAPI TranslateRegionInWindowToScreen(
            /* [in] */ IRegion* transparentRegion);

        CARAPI TranslateCanvas(
            /* [in] */ ICanvas* canvas);

        CARAPI TranslateEventInScreenToAppWindow(
            /* [in] */ IMotionEvent* event);

        CARAPI TranslateWindowLayout(
            /* [in] */ IWindowManagerLayoutParams* params);

        CARAPI TranslateRectInAppWindowToScreen(
            /* [in] */ IRect* rect);

        CARAPI TranslateRectInScreenToAppWindow(
            /* [in] */ IRect* rect);

        CARAPI TranslateLayoutParamsInAppWindowToScreen(
           /* [in] */ IWindowManagerLayoutParams* params);

        CARAPI GetTranslatedContentInsets(
            /* [in] */ IRect* contentInsets,
            /* [out] */ IRect** rect);

        CARAPI GetTranslatedVisbileInsets(
            /* [in] */ IRect* visibleInsets,
            /* [out] */ IRect** rect);

        CARAPI GetApplicationInvertedScale(
            /* [out] */ Float* appInvertedScale);

    public:
        Float mApplicationScale;
        Float mApplicationInvertedScale;

    private:
        AutoPtr<IRect> mContentInsetsBuffer;
        AutoPtr<IRect> mVisibleInsetsBuffer;
    };

public:
    /**
     * Returns the frame Rect for applications runs under compatibility mode.
     *
     * @param dm the display metrics used to compute the frame size.
     * @param orientation the orientation of the screen.
     * @param outRect the output parameter which will contain the result.
     */
    static CARAPI_(void) UpdateCompatibleScreenFrame(
        /* [in] */ IDisplayMetrics* dm,
        /* [in] */ Int32 orientation,
        /* [in] */ IRect* outRect);

protected:
    CompatibilityInfo();

    CARAPI_(void) constructor();

    CARAPI_(void) constructor(
        /* [in] */ IApplicationInfo* appInfo);

    CARAPI_(void) constructor(
        /* [in] */ Int32 appFlags,
        /* [in] */ Int32 compFlags,
        /* [in] */ Int32 dens,
        /* [in] */ Float scale,
        /* [in] */ Float invertedScale);

    ~CompatibilityInfo();

    /**
     * Returns the copy of this instance.
     */
    CARAPI_(CompatibilityInfo*) Copy();

    /**
     * Sets expandable bit in the compatibility flag.
     */
    CARAPI_(void) SetExpandable(
         /* [in] */ Boolean expandable);

    /**
     * Sets large screen bit in the compatibility flag.
     */
    CARAPI_(void) SetLargeScreens(
        /* [in] */ Boolean expandable);

    /**
     * Sets large screen bit in the compatibility flag.
     */
    CARAPI_(void) SetXLargeScreens(
        /* [in] */ Boolean expandable);

    /**
     * @return true if the application is configured to be expandable.
     */
    CARAPI_(Boolean) IsConfiguredExpandable();

    /**
     * @return true if the application is configured to be expandable.
     */
    CARAPI_(Boolean) IsConfiguredLargeScreens();

    /**
     * @return true if the application is configured to be expandable.
     */
    CARAPI_(Boolean) IsConfiguredXLargeScreens();

    /**
     * @return true if the scaling is required
     */
    CARAPI_(Boolean) IsScalingRequired();

    CARAPI_(Boolean) SupportsScreen();

    CARAPI_(String) GetDescription();

    /**
     * Returns the translator which translates the coordinates in compatibility mode.
     * @param params the window's parameter
     */
    CARAPI GetTranslator(
        /* [out] */ ITranslator** translator);

public:
    /**
     * The effective screen density we have selected for this application.
     */
    Int32 mApplicationDensity;

    /**
     * Application's scale.
     */
    Float mApplicationScale;

    /**
     * Application's inverted scale.
     */
    Float mApplicationInvertedScale;

    /**
     * The flags from ApplicationInfo::
     */
    Int32 mAppFlags;

protected:
    /**
     *  A compatibility flags
     */
    Int32 mCompatibilityFlags;
};
#endif //__COMPATIBILITYINFO_H__

