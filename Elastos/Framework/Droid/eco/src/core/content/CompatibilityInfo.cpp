
#include "content/CompatibilityInfo.h"
#include "content/CCompatibilityInfo.h"
#include "content/CApplicationInfo.h"
#include "utils/CDisplayMetrics.h"
#include "graphics/CRect.h"
#include <StringBuffer.h>
#include <Logger.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

CompatibilityInfo::Translator::Translator(
    /* [in] */ Float applicationScale,
    /* [in] */ Float applicationInvertedScale) :
    mApplicationScale(applicationScale),
    mApplicationInvertedScale(applicationInvertedScale)
{
}

PInterface CompatibilityInfo::Translator::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IRunnable*)this;
    }
    else if (riid == EIID_ITranslator) {
        return (ITranslator*)this;
    }
    return NULL;
}

UInt32 CompatibilityInfo::Translator::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CompatibilityInfo::Translator::Release()
{
    return ElRefBase::Release();
}

ECode CompatibilityInfo::Translator::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

/**
* Translate the screen rect to the application frame.
*/
ECode CompatibilityInfo::Translator::TranslateRectInScreenToAppWinFrame(
    /* [in] */ IRect* rect)
{
    return rect->Scale(mApplicationInvertedScale);
}

/**
* Translate the region in window to screen.
*/
ECode CompatibilityInfo::Translator::TranslateRegionInWindowToScreen(
    /* [in] */ IRegion* transparentRegion)
{
    return transparentRegion->Scale(mApplicationScale);
}

/**
* Apply translation to the canvas that is necessary to draw the content.
*/
ECode CompatibilityInfo::Translator::TranslateCanvas(
    /* [in] */ ICanvas* canvas)
{
    if (mApplicationScale == 1.5f) {
        /*  When we scale for compatibility, we can put our stretched
        bitmaps and ninepatches on exacty 1/2 pixel boundaries,
        which can give us inconsistent drawing due to imperfect
        Float precision in the graphics engine's inverse matrix.

        As a work-around, we translate by a tiny amount to avoid
        landing on exact pixel centers and boundaries, giving us
        the slop we need to draw consistently.

        This constant is meant to resolve to 1/255 after it is
        scaled by 1.5 (mApplicationScale). Note, this is just a guess
        as to what is small enough not to create its own artifacts,
        and big enough to avoid the precision problems. Feel free
        to experiment with smaller values as you choose.
        */
        const Float tinyOffset = 2.0f / (3 * 255);
        canvas->Translate(tinyOffset, tinyOffset);
    }

    return canvas->Scale(mApplicationScale, mApplicationScale);
}

/**
* Translate the motion event captured on screen to the application's window.
*/
ECode CompatibilityInfo::Translator::TranslateEventInScreenToAppWindow(
    /* [in] */ IMotionEvent* event)
{
    assert(event);
    event->Scale(mApplicationInvertedScale);

    return NOERROR;
}

/**
* Translate the window's layout parameter, from application's view to
* Screen's view.
*/
ECode CompatibilityInfo::Translator::TranslateWindowLayout(
    /* [in] */ IWindowManagerLayoutParams* params)
{
    return params->Scale(mApplicationScale);
}

/**
* Translate a Rect in application's window to screen.
*/
ECode CompatibilityInfo::Translator::TranslateRectInAppWindowToScreen(
    /* [in] */ IRect* rect)
{
    return rect->Scale(mApplicationScale);
}

/**
* Translate a Rect in screen coordinates into the app window's coordinates.
*/
ECode CompatibilityInfo::Translator::TranslateRectInScreenToAppWindow(
    /* [in] */ IRect* rect)
{
    return rect->Scale(mApplicationInvertedScale);
}

/**
* Translate the location of the sub window.
* @param params
*/
ECode CompatibilityInfo::Translator::TranslateLayoutParamsInAppWindowToScreen(
   /* [in] */ IWindowManagerLayoutParams* params)
{
    params->Scale(mApplicationScale);
}

/**
* Translate the content insets in application window to Screen. This uses
* the internal buffer for content insets to avoid extra object allocation.
*/
ECode CompatibilityInfo::Translator::GetTranslatedContentInsets(
    /* [in] */ IRect* contentInsets,
    /* [out] */ IRect** rect)
{
    if (rect == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (mContentInsetsBuffer == NULL) {
        ECode ec = CRect::New((IRect**)&mContentInsetsBuffer);
        if (FAILED(ec)) {
            return ec;
        }
    }

    mContentInsetsBuffer->SetEx(contentInsets);
    TranslateRectInAppWindowToScreen(mContentInsetsBuffer);
    *rect = mContentInsetsBuffer;
    (*rect)->AddRef();

    return NOERROR;
}

/**
* Translate the visible insets in application window to Screen. This uses
* the internal buffer for content insets to avoid extra object allocation.
*/
ECode CompatibilityInfo::Translator::GetTranslatedVisbileInsets(
    /* [in] */ IRect* visibleInsets,
    /* [out] */ IRect** rect)
{
    if (rect == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (mVisibleInsetsBuffer == NULL) {
        ECode ec = CRect::New((IRect**)&mVisibleInsetsBuffer);
        if (FAILED(ec)) {
            return ec;
        }
    }

    mVisibleInsetsBuffer->SetEx(visibleInsets);
    TranslateRectInAppWindowToScreen(mVisibleInsetsBuffer);
    *rect = mVisibleInsetsBuffer;
    (*rect)->AddRef();

    return NOERROR;
}

ECode CompatibilityInfo::Translator::GetApplicationInvertedScale(
    /* [out] */ Float* appInvertedScale)
{
    VALIDATE_NOT_NULL(appInvertedScale);
    *appInvertedScale = mApplicationInvertedScale;
    return NOERROR;
}

class CDefaultCompatibilityInfo :
    public ElRefBase,
    public ICompatibilityInfo,
    public CompatibilityInfo
{
public:
    CDefaultCompatibilityInfo() {}

    ~CDefaultCompatibilityInfo() {}

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid)
    {
        if (riid == EIID_IInterface) {
            return (PInterface)this;
        }
        else if (riid == EIID_ICompatibilityInfo) {
            return (ICompatibilityInfo*)this;
        }
        return NULL;
    }

    CARAPI_(UInt32) AddRef()
    {
        return ElRefBase::AddRef();
    }

    CARAPI_(UInt32) Release()
    {
        return ElRefBase::Release();
    }

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID)
    {
        if (NULL == pIID) return E_INVALID_ARGUMENT;

        if (pObject == (IInterface*)(ICompatibilityInfo*)this) {
            *pIID = EIID_ICompatibilityInfo;
        }
        else {
            return E_INVALID_ARGUMENT;
        }
        return NOERROR;
    }

public:
    CARAPI Copy(
        /* [out] */ ICompatibilityInfo** info)
    {
        VALIDATE_NOT_NULL(info);

        return CCompatibilityInfo::New(mAppFlags, mCompatibilityFlags,
                mApplicationDensity, mApplicationScale, mApplicationInvertedScale, info);
    }

    CARAPI SetExpandable(
        /* [in] */ Boolean expandable)
    {
        Logger::E(CompatibilityInfo::TAG, "trying to change default compatibility info");
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    }

    CARAPI SetLargeScreens(
        /* [in] */ Boolean expandable)
    {
        CompatibilityInfo::SetLargeScreens(expandable);
        return NOERROR;
    }

    CARAPI SetXLargeScreens(
        /* [in] */ Boolean expandable)
    {
        CompatibilityInfo::SetXLargeScreens(expandable);
        return NOERROR;
    }

    CARAPI IsConfiguredExpandable(
        /* [out] */ Boolean* isExpandable)
    {
        VALIDATE_NOT_NULL(isExpandable);

        *isExpandable = CompatibilityInfo::IsConfiguredExpandable();
        return NOERROR;
    }

    CARAPI IsConfiguredLargeScreens(
        /* [out] */ Boolean* isLarge)
    {
        VALIDATE_NOT_NULL(isLarge);

        *isLarge = CompatibilityInfo::IsConfiguredLargeScreens();
        return NOERROR;
    }

    CARAPI IsConfiguredXLargeScreens(
        /* [out] */ Boolean* isLarge)
    {
        VALIDATE_NOT_NULL(isLarge);

        *isLarge = CompatibilityInfo::IsConfiguredXLargeScreens();
        return NOERROR;
    }

    CARAPI IsScalingRequired(
        /* [out] */ Boolean* isRequired)
    {
        VALIDATE_NOT_NULL(isRequired);

        *isRequired = CompatibilityInfo::IsScalingRequired();
        return NOERROR;
    }

    CARAPI SupportsScreen(
        /* [out] */ Boolean* isSupported)
    {
        VALIDATE_NOT_NULL(isSupported);

        *isSupported = CompatibilityInfo::SupportsScreen();
        return NOERROR;
    }

    CARAPI GetDescription(
        /* [out] */ String* str)
    {
        VALIDATE_NOT_NULL(str);

        *str = CompatibilityInfo::GetDescription();
        return NOERROR;
    }

    CARAPI GetTranslator(
        /* [out] */ ITranslator** translator)
    {
        VALIDATE_NOT_NULL(translator);

        return CompatibilityInfo::GetTranslator(translator);
    }

    CARAPI UpdateCompatibleScreenFrame(
        /* [in] */ IDisplayMetrics* dm,
        /* [in] */ Int32 orientation,
        /* [in, out] */ IRect* outRect)
    {
        VALIDATE_NOT_NULL(dm);
        VALIDATE_NOT_NULL(outRect);

        CompatibilityInfo::UpdateCompatibleScreenFrame(dm, orientation, outRect);
        return NOERROR;
    }
};

AutoPtr<ICompatibilityInfo> CompatibilityInfo::DEFAULT_COMPATIBILITY_INFO;

CARAPI CompatibilityInfo::GetDefaultCompatibilityInfo(
        /* [out] */ ICompatibilityInfo** info)
{
    assert(info);

    if (DEFAULT_COMPATIBILITY_INFO == NULL) {
        DEFAULT_COMPATIBILITY_INFO = new CDefaultCompatibilityInfo();
        if (DEFAULT_COMPATIBILITY_INFO == NULL) {
            return E_OUT_OF_MEMORY;
        }
    }
    *info = DEFAULT_COMPATIBILITY_INFO.Get();
    (*info)->AddRef();
    return NOERROR;
}


const Int32 CompatibilityInfo::DEFAULT_PORTRAIT_WIDTH;
const Int32 CompatibilityInfo::DEFAULT_PORTRAIT_HEIGHT;
const char* CompatibilityInfo::TAG = "CompatibilityInfo";
const Boolean CompatibilityInfo::DBG;
const Int32 CompatibilityInfo::SCALING_REQUIRED;
const Int32 CompatibilityInfo::EXPANDABLE;
const Int32 CompatibilityInfo::CONFIGURED_EXPANDABLE;
const Int32 CompatibilityInfo::LARGE_SCREENS;
const Int32 CompatibilityInfo::CONFIGURED_LARGE_SCREENS;
const Int32 CompatibilityInfo::XLARGE_SCREENS;
const Int32 CompatibilityInfo::CONFIGURED_XLARGE_SCREENS;

CompatibilityInfo::CompatibilityInfo()
    : mApplicationDensity(0)
    , mApplicationScale(0.0f)
    , mApplicationInvertedScale(0.0f)
    , mAppFlags(0)
    , mCompatibilityFlags(0)
{
}

CompatibilityInfo::~CompatibilityInfo()
{
}

void CompatibilityInfo::constructor(
    /* [in] */ IApplicationInfo* appInfo)
{
    assert(appInfo);

    mAppFlags = ((CApplicationInfo*)appInfo)->mFlags;

    if ((mAppFlags & ApplicationInfo_FLAG_SUPPORTS_LARGE_SCREENS) != 0) {
        // Saying you support large screens also implies you support xlarge
        // screens; there is no compatibility mode for a large app on an
        // xlarge screen.
        mCompatibilityFlags |= LARGE_SCREENS | CONFIGURED_LARGE_SCREENS
            | XLARGE_SCREENS | CONFIGURED_XLARGE_SCREENS
            | EXPANDABLE | CONFIGURED_EXPANDABLE;
    }

    if ((mAppFlags & ApplicationInfo_FLAG_SUPPORTS_XLARGE_SCREENS) != 0) {
        mCompatibilityFlags |= XLARGE_SCREENS | CONFIGURED_XLARGE_SCREENS
            | EXPANDABLE | CONFIGURED_EXPANDABLE;
    }

    if ((mAppFlags & ApplicationInfo_FLAG_RESIZEABLE_FOR_SCREENS) != 0) {
        mCompatibilityFlags |= EXPANDABLE | CONFIGURED_EXPANDABLE;
    }

    if ((mAppFlags & ApplicationInfo_FLAG_SUPPORTS_SCREEN_DENSITIES) != 0) {
        mApplicationDensity = CDisplayMetrics::DENSITY_DEVICE;
        mApplicationScale = 1.0f;
        mApplicationInvertedScale = 1.0f;
    }
    else {
        mApplicationDensity = CDisplayMetrics::DENSITY_DEFAULT;
        mApplicationScale = CDisplayMetrics::DENSITY_DEVICE
            / (Float) CDisplayMetrics::DENSITY_DEFAULT;
        mApplicationInvertedScale = 1.0f / mApplicationScale;
        mCompatibilityFlags |= SCALING_REQUIRED;
    }
}

void CompatibilityInfo::constructor(
    /* [in] */ Int32 appFlags,
    /* [in] */ Int32 compFlags,
    /* [in] */ Int32 dens,
    /* [in] */ Float scale,
    /* [in] */ Float invertedScale)
{
    mAppFlags = appFlags;
    mCompatibilityFlags = compFlags;
    mApplicationDensity = dens;
    mApplicationScale = scale;
    mApplicationInvertedScale = invertedScale;
}

void CompatibilityInfo::constructor()
{
    constructor(ApplicationInfo_FLAG_SUPPORTS_SMALL_SCREENS
        | ApplicationInfo_FLAG_SUPPORTS_NORMAL_SCREENS
        | ApplicationInfo_FLAG_SUPPORTS_LARGE_SCREENS
        | ApplicationInfo_FLAG_SUPPORTS_XLARGE_SCREENS
        | ApplicationInfo_FLAG_RESIZEABLE_FOR_SCREENS,
        EXPANDABLE | CONFIGURED_EXPANDABLE,
        CDisplayMetrics::DENSITY_DEVICE,
        1.0f,
        1.0f);
}

/**
* Returns the copy of this instance.
*/
CompatibilityInfo* CompatibilityInfo::Copy()
{
    // implemented in CCompatibilityInfo
    return NULL;
}

/**
* Sets expandable bit in the compatibility flag.
*/
void CompatibilityInfo::SetExpandable(
     /* [in] */ Boolean expandable)
 {
    if (expandable) {
        mCompatibilityFlags |= EXPANDABLE;
    } else {
        mCompatibilityFlags &= ~EXPANDABLE;
    }
}

/**
* Sets large screen bit in the compatibility flag.
*/
void CompatibilityInfo::SetLargeScreens(
    /* [in] */ Boolean expandable)
{
    if (expandable) {
        mCompatibilityFlags |= LARGE_SCREENS;
    } else {
        mCompatibilityFlags &= ~LARGE_SCREENS;
    }
}

/**
* Sets large screen bit in the compatibility flag.
*/
void CompatibilityInfo::SetXLargeScreens(
    /* [in] */ Boolean expandable){
    if (expandable) {
        mCompatibilityFlags |= XLARGE_SCREENS;
    } else {
        mCompatibilityFlags &= ~XLARGE_SCREENS;
    }
}

/**
* @return true if the application is configured to be expandable.
*/
Boolean CompatibilityInfo::IsConfiguredExpandable()
{
    return (mCompatibilityFlags & CONFIGURED_EXPANDABLE) != 0;
}

/**
* @return true if the application is configured to be expandable.
*/
Boolean CompatibilityInfo::IsConfiguredLargeScreens()
{
    return (mCompatibilityFlags & CONFIGURED_LARGE_SCREENS) != 0;
}

/**
* @return true if the application is configured to be expandable.
*/
Boolean CompatibilityInfo::IsConfiguredXLargeScreens()
{
    return (mCompatibilityFlags & CONFIGURED_XLARGE_SCREENS) != 0;
}

/**
* @return true if the scaling is required
*/
Boolean CompatibilityInfo::IsScalingRequired()
{
    return (mCompatibilityFlags & SCALING_REQUIRED) != 0;
}

Boolean CompatibilityInfo::SupportsScreen()
{
    return (mCompatibilityFlags & (EXPANDABLE|LARGE_SCREENS))
        == (EXPANDABLE|LARGE_SCREENS);
}

String CompatibilityInfo::GetDescription()
{
    StringBuffer s("CompatibilityInfo{scale=");
    s += (Int32)mApplicationScale;
    s += ", supports screen=";
    s += (Int32)SupportsScreen();
    s += "}";
    return String((const char*)s);
}

/**
* Returns the translator which translates the coordinates in compatibility mode.
* @param params the window's parameter
*/
ECode CompatibilityInfo::GetTranslator(
    /* [out] */ ITranslator** translator)
{
    if (translator == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *translator = IsScalingRequired()
        ? new Translator(mApplicationScale, mApplicationInvertedScale) : NULL;
    if (*translator != NULL) (*translator)->AddRef();

    return NOERROR;
}

/**
* Returns the frame Rect for applications runs under compatibility mode.
*
* @param dm the display metrics used to compute the frame size.
* @param orientation the orientation of the screen.
* @param outRect the output parameter which will contain the result.
*/
void CompatibilityInfo::UpdateCompatibleScreenFrame(
    /* [in] */ IDisplayMetrics* dm,
    /* [in] */ Int32 orientation,
    /* [in] */ IRect* outRect)
{
    assert(dm);

    CDisplayMetrics* displayMetrics = (CDisplayMetrics*)dm;
    Int32 width = displayMetrics->mWidthPixels;
    Int32 portraitHeight = (Int32) (DEFAULT_PORTRAIT_HEIGHT
        * displayMetrics->mDensity + 0.5f);
    Int32 portraitWidth = (Int32) (DEFAULT_PORTRAIT_WIDTH
        * displayMetrics->mDensity + 0.5f);

    if (orientation == Configuration_ORIENTATION_LANDSCAPE) {
        Int32 xOffset = (width - portraitHeight) / 2 ;
        outRect->Set(xOffset, 0, xOffset + portraitHeight, portraitWidth);
    } else {
        Int32 xOffset = (width - portraitWidth) / 2 ;
        outRect->Set(xOffset, 0, xOffset + portraitWidth, portraitHeight);
    }
}
