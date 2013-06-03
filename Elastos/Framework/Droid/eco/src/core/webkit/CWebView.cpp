#include <Logger.h>

#include "webkit/CWebView.h"
#include "webkit/CWebViewDatabase.h"
#include "webkit/Network.h"
#include "webkit/CURLUtil.h"
#include "webkit/CCallbackProxy.h"
#include "webkit/WebTextView.h"
#include "webkit/CWebBackForwardList.h"
#include "webkit/CPluginList.h"
#include "widget/CToastHelper.h"
#include "webkit/DebugFlags.h"
#include "utils/CDisplayMetrics.h"
#include "content/CResources.h"
#include "content/CConfiguration.h"
#include "graphics/CPoint.h"
#include "graphics/CPaint.h"
#include "graphics/CCanvas.h"
#include "view/CKeyEvent.h"
#include "view/animation/CAlphaAnimation.h"
#include "text/Selection.h"
#include "view/inputmethod/CLocalInputMethodManager.h"

using namespace Elastos::Utility::Logging;

const CString CWebView::LOGTAG("webview");

const CString CWebView::HandlerPrivateDebugString[] = {
    "REMEMBER_PASSWORD", //              = 1;
    "NEVER_REMEMBER_PASSWORD", //        = 2;
    "SWITCH_TO_SHORTPRESS", //           = 3;
    "SWITCH_TO_LONGPRESS", //            = 4;
    "RELEASE_SINGLE_TAP", //             = 5;
    "REQUEST_FORM_DATA", //              = 6;
    "RESUME_WEBCORE_PRIORITY", //        = 7;
    "DRAG_HELD_MOTIONLESS", //           = 8;
    "AWAKEN_SCROLL_BARS", //             = 9;
    "PREVENT_DEFAULT_TIMEOUT" //         = 10;
};

const CString CWebView::HandlerPackageDebugString[] = {
    "SCROLL_TO_MSG_ID", //               = 101;
    "SCROLL_BY_MSG_ID", //               = 102;
    "SPAWN_SCROLL_TO_MSG_ID", //         = 103;
    "SYNC_SCROLL_TO_MSG_ID", //          = 104;
    "NEW_PICTURE_MSG_ID", //             = 105;
    "UPDATE_TEXT_ENTRY_MSG_ID", //       = 106;
    "WEBCORE_INITIALIZED_MSG_ID", //     = 107;
    "UPDATE_TEXTFIELD_TEXT_MSG_ID", //   = 108;
    "UPDATE_ZOOM_RANGE", //              = 109;
    "MOVE_OUT_OF_PLUGIN", //             = 110;
    "CLEAR_TEXT_ENTRY", //               = 111;
    "UPDATE_TEXT_SELECTION_MSG_ID", //   = 112;
    "SHOW_RECT_MSG_ID", //               = 113;
    "LONG_PRESS_CENTER", //              = 114;
    "PREVENT_TOUCH_ID", //               = 115;
    "WEBCORE_NEED_TOUCH_EVENTS", //      = 116;
    "INVAL_RECT_MSG_ID", //              = 117;
    "REQUEST_KEYBOARD", //               = 118;
    "DO_MOTION_UP", //                   = 119;
    "SHOW_FULLSCREEN", //                = 120;
    "HIDE_FULLSCREEN", //                = 121;
    "DOM_FOCUS_CHANGED", //              = 122;
    "IMMEDIATE_REPAINT_MSG_ID", //       = 123;
    "SET_ROOT_LAYER_MSG_ID", //          = 124;
    "RETURN_LABEL", //                   = 125;
    "FIND_AGAIN", //                     = 126;
    "CENTER_FIT_RECT", //                = 127;
    "REQUEST_KEYBOARD_WITH_SELECTION_MSG_ID", // = 128;
    "SET_SCROLLBAR_MODES" //             = 129;
};


// if AUTO_REDRAW_HACK is true, then the CALL key will toggle redrawing
// the screen all-the-time. Good for profiling our drawing code
const Boolean CWebView::AUTO_REDRAW_HACK;

// enable debug output for drag trackers
const Boolean CWebView::DEBUG_DRAG_TRACKER;

const Int32 CWebView::TOUCH_SENT_INTERVAL;// = 50;

const Int32 CWebView::TOUCH_INIT_MODE;
const Int32 CWebView::TOUCH_DRAG_START_MODE;
const Int32 CWebView::TOUCH_DRAG_MODE;
const Int32 CWebView::TOUCH_SHORTPRESS_START_MODE;
const Int32 CWebView::TOUCH_SHORTPRESS_MODE;
const Int32 CWebView::TOUCH_DOUBLE_TAP_MODE;
const Int32 CWebView::TOUCH_DONE_MODE;
const Int32 CWebView::TOUCH_PINCH_DRAG;

// Whether to prevent default during touch. The initial value depends on
// mForwardTouchEvents. If WebCore wants all the touch events, it says yes
// for touch down. Otherwise UI will wait for the answer of the first
// confirmed move before taking over the control.
const Int32 CWebView::PREVENT_DEFAULT_NO;
const Int32 CWebView::PREVENT_DEFAULT_MAYBE_YES;
const Int32 CWebView::PREVENT_DEFAULT_NO_FROM_TOUCH_DOWN;
const Int32 CWebView::PREVENT_DEFAULT_YES;
const Int32 CWebView::PREVENT_DEFAULT_IGNORE;

// This should be ViewConfiguration.getTapTimeout()
// But system time out is 100ms, which is too short for the browser.
// In the browser, if it switches out of tap too soon, jump tap won't work.
const Int32 CWebView::TAP_TIMEOUT;
// This should be ViewConfiguration.getLongPressTimeout()
// But system time out is 500ms, which is too short for the browser.
// With a short timeout, it's difficult to treat trigger a short press.
const Int32 CWebView::LONG_PRESS_TIMEOUT;
// needed to avoid flinging after a pause of no movement
const Int32 CWebView::MIN_FLING_TIME;
// draw unfiltered after drag is held without movement
const Int32 CWebView::MOTIONLESS_TIME;
// The time that the Zoom Controls are visible before fading away
//const Int64 CWebView::ZOOM_CONTROLS_TIMEOUT;// = ViewConfiguration.getZoomControlsTimeout();
// The amount of content to overlap between two screens when going through
// pages with the space bar, in pixels.
const Int32 CWebView::PAGE_SCROLL_OVERLAP;

// our standard speed. this way small distances will be traversed in less
// time than large distances, but we cap the duration, so that very large
// distances won't take too long to get there.
const Int32 CWebView::STD_SPEED;  // pixels per second
// time for the longest scroll animation
const Int32 CWebView::MAX_DURATION;   // milliseconds
const Int32 CWebView::SLIDE_TITLE_DURATION;   // milliseconds

const Int32 CWebView::MOTIONLESS_FALSE;
const Int32 CWebView::MOTIONLESS_PENDING;
const Int32 CWebView::MOTIONLESS_TRUE;
const Int32 CWebView::MOTIONLESS_IGNORE;

/*
 * Private message ids
 */
const Int32 CWebView::REMEMBER_PASSWORD;
const Int32 CWebView::NEVER_REMEMBER_PASSWORD;
const Int32 CWebView::SWITCH_TO_SHORTPRESS;
const Int32 CWebView::SWITCH_TO_LONGPRESS;
const Int32 CWebView::RELEASE_SINGLE_TAP;
const Int32 CWebView::REQUEST_FORM_DATA;
const Int32 CWebView::RESUME_WEBCORE_PRIORITY;
const Int32 CWebView::DRAG_HELD_MOTIONLESS;
const Int32 CWebView::AWAKEN_SCROLL_BARS;
const Int32 CWebView::PREVENT_DEFAULT_TIMEOUT;

const Int32 CWebView::FIRST_PRIVATE_MSG_ID;
const Int32 CWebView::LAST_PRIVATE_MSG_ID;

const Int32 CWebView::FIRST_PACKAGE_MSG_ID;
const Int32 CWebView::LAST_PACKAGE_MSG_ID;

const Int32 CWebView::SNAP_NONE;
const Int32 CWebView::SNAP_LOCK; // not a separate state
const Int32 CWebView::SNAP_X; // may be combined with SNAP_LOCK
const Int32 CWebView::SNAP_Y; // may be combined with SNAP_LOCK

// keep these in sync with their counterparts in WebView.cpp
const Int32 CWebView::DRAW_EXTRAS_NONE;
const Int32 CWebView::DRAW_EXTRAS_FIND;
const Int32 CWebView::DRAW_EXTRAS_SELECTION;
const Int32 CWebView::DRAW_EXTRAS_CURSOR_RING;

// keep this in sync with WebCore:ScrollbarMode in WebKit
const Int32 CWebView::SCROLLBAR_AUTO;
const Int32 CWebView::SCROLLBAR_ALWAYSOFF;
// as we auto fade scrollbar, this is ignored.
const Int32 CWebView::SCROLLBAR_ALWAYSON;

// if the page can scroll <= this value, we won't allow the drag tracker
// to have any effect.
const Int32 CWebView::MIN_SCROLL_AMOUNT_TO_DISABLE_DRAG_TRACKER;

// return NO_LEFTEDGE means failure.
const Int32 CWebView::NO_LEFTEDGE;

const Float CWebView::MAX_SLOPE_FOR_DIAG;
const Int32 CWebView::MIN_BREAK_SNAP_CROSS_DISTANCE;

const Int32 CWebView::ZOOM_ANIMATION_LENGTH;

const Int32 CWebView::TRACKBALL_KEY_TIMEOUT;
const Int32 CWebView::TRACKBALL_TIMEOUT;
const Int32 CWebView::TRACKBALL_WAIT;
const Int32 CWebView::TRACKBALL_SCALE;
const Int32 CWebView::TRACKBALL_SCROLL_COUNT;
const Int32 CWebView::TRACKBALL_MOVE_COUNT;
const Int32 CWebView::TRACKBALL_MULTIPLIER;
const Int32 CWebView::SELECT_CURSOR_OFFSET;


// default scale. Depending on the display density.
Int32 CWebView::DEFAULT_SCALE_PERCENT;

// default scale limit. Depending on the display density
Float CWebView::DEFAULT_MAX_ZOOM_SCALE;
Float CWebView::DEFAULT_MIN_ZOOM_SCALE;

Float CWebView::MINIMUM_SCALE_INCREMENT = 0.01f;

AutoPtr<IPaint> CWebView::mOverScrollBackground;
AutoPtr<IPaint> CWebView::mOverScrollBorder;

/* package */
Boolean CWebView::mLogEvent = TRUE;

Int32 CWebView::sMaxViewportWidth = DEFAULT_VIEWPORT_WIDTH;

IVIEW_METHODS_IMPL(CWebView, AbsoluteLayout, AbsoluteLayout);
IVIEWGROUP_METHODS_IMPL(CWebView, AbsoluteLayout, AbsoluteLayout);

CARAPI_(PInterface) CWebView::Probe(
            /* [in]  */ REIID riid)
{}

UInt32 CWebView::AddRef()
{}

UInt32 CWebView::Release()
{}

ECode CWebView::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{}

#if 0
ECode CWebView::GetVerticalFadingEdgeLength(
    /* [out] */ Int32 * pLength)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetFadingEdgeLength(
    /* [in] */ Int32 length)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetHorizontalFadingEdgeLength(
    /* [out] */ Int32 * pLength)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetVerticalScrollbarWidth(
    /* [out] */ Int32 * pWidth)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetHorizontalScrollbarHeight(
    /* [out] */ Int32 * pHeight)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetOnFocusChangeListener(
    /* [in] */ IViewOnFocusChangeListener * pL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetOnFocusChangeListener(
    /* [out] */ IViewOnFocusChangeListener ** ppL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetOnClickListener(
    /* [in] */ IViewOnClickListener * pL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetOnLongClickListener(
    /* [in] */ IViewOnLongClickListener * pL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetOnCreateContextMenuListener(
    /* [in] */ IViewOnCreateContextMenuListener * pL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::PerformClick(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::PerformLongClick(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::ShowContextMenu(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetOnKeyListener(
    /* [in] */ IViewOnKeyListener * pL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetOnTouchListener(
    /* [in] */ IViewOnTouchListener * pL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::RequestRectangleOnScreen(
    /* [in] */ IRect * pRectangle,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::RequestRectangleOnScreenEx(
    /* [in] */ IRect * pRectangle,
    /* [in] */ Boolean immediate,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::ClearFocus()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::HasFocus(
    /* [out] */ Boolean * pHasFocus)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::HasFocusable(
    /* [out] */ Boolean * pHasFocusable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetContentDescription(
    /* [out, callee] */ ArrayOf<Char8> ** ppDescription)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetContentDescription(
    /* [in] */ const ArrayOf<Char8> & contentDescription)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsFocused(
    /* [out] */ Boolean * pFocused)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::FindFocus(
    /* [out] */ IView ** ppView)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetScrollContainer(
    /* [in] */ Boolean isScrollContainer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetDrawingCacheQuality(
    /* [out] */ Int32 * pQuality)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetDrawingCacheQuality(
    /* [in] */ Int32 quality)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetKeepScreenOn(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetKeepScreenOn(
    /* [in] */ Boolean keepScreenOn)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetNextFocusLeftId(
    /* [out] */ Int32 * pNextFocusLeftId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetNextFocusLeftId(
    /* [in] */ Int32 nextFocusLeftId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetNextFocusRightId(
    /* [out] */ Int32 * pNextFocusRightId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetNextFocusRightId(
    /* [in] */ Int32 nextFocusRightId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetNextFocusUpId(
    /* [out] */ Int32 * pNextFocusUpId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetNextFocusUpId(
    /* [in] */ Int32 nextFocusUpId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetNextFocusDownId(
    /* [out] */ Int32 * pNextFocusDownId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetNextFocusDownId(
    /* [in] */ Int32 nextFocusDownId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsShown(
    /* [out] */ Boolean * pShown)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsFitsSystemWindowsFlagSet(
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetVisibility(
    /* [out] */ Int32 * pVisibility)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetVisibility(
    /* [in] */ Int32 visibility)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsEnabled(
    /* [out] */ Boolean * pEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetEnabled(
    /* [in] */ Boolean enabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetFocusable(
    /* [in] */ Boolean focusable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetFocusableInTouchMode(
    /* [in] */ Boolean focusableInTouchMode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetSoundEffectsEnabled(
    /* [in] */ Boolean soundEffectsEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsSoundEffectsEnabled(
    /* [out] */ Boolean * pSoundEffectsEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetHapticFeedbackEnabled(
    /* [in] */ Boolean hapticFeedbackEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsHapticFeedbackEnabled(
    /* [out] */ Boolean * pHapticFeedbackEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetWillNotDraw(
    /* [in] */ Boolean willNotDraw)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::WillNotDraw(
    /* [out] */ Boolean * pWillNotDraw)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetWillNotCacheDrawing(
    /* [in] */ Boolean willNotCacheDrawing)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::WillNotCacheDrawing(
    /* [out] */ Boolean * pCache)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsClickable(
    /* [out] */ Boolean * pClickable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetClickable(
    /* [in] */ Boolean clickable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsLongClickable(
    /* [out] */ Boolean * pLongClickable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetLongClickable(
    /* [in] */ Boolean longClickable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetPressed(
    /* [in] */ Boolean pressed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsPressed(
    /* [out] */ Boolean * pPressed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsSaveEnabled(
    /* [out] */ Boolean * pEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetSaveEnabled(
    /* [in] */ Boolean enabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetFilterTouchesWhenObscured(
    /* [out] */ Boolean * pEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetFilterTouchesWhenObscured(
    /* [in] */ Boolean enabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsFocusable(
    /* [out] */ Boolean * pFocusable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsFocusableInTouchMode(
    /* [out] */ Boolean * pFocusable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::FocusSearch(
    /* [in] */ Int32 direction,
    /* [out] */ IView ** ppView)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::DispatchUnhandledMove(
    /* [in] */ IView * pFocused,
    /* [in] */ Int32 direction,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetFocusables(
    /* [in] */ Int32 direction,
    /* [out] */ IObjectContainer ** ppViews)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::AddFocusables(
    /* [in] */ IObjectContainer * pViews,
    /* [in] */ Int32 direction)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::AddFocusablesEx(
    /* [in] */ IObjectContainer * pViews,
    /* [in] */ Int32 direction,
    /* [in] */ Int32 focusableMode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetTouchables(
    /* [out] */ IObjectContainer ** ppViews)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::AddTouchables(
    /* [in] */ IObjectContainer * pViews)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::RequestFocus(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::RequestFocusEx(
    /* [in] */ Int32 direction,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::RequestFocusEx2(
    /* [in] */ Int32 direction,
    /* [in] */ IRect * pPreviouslyFocusedRect,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::RequestFocusFromTouch(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::DispatchStartTemporaryDetach()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::OnStartTemporaryDetach()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::DispatchFinishTemporaryDetach()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::OnFinishTemporaryDetach()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetKeyDispatcherState(
    /* [out] */ IDispatcherState ** ppState)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::DispatchKeyEventPreIme(
    /* [in] */ IKeyEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::DispatchKeyEvent(
    /* [in] */ IKeyEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::DispatchKeyShortcutEvent(
    /* [in] */ IKeyEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::DispatchTouchEvent(
    /* [in] */ IMotionEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::OnFilterTouchEventForSecurity(
    /* [in] */ IMotionEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::DispatchTrackballEvent(
    /* [in] */ IMotionEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::DispatchWindowFocusChanged(
    /* [in] */ Boolean hasFocus)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::HasWindowFocus(
    /* [out] */ Boolean * pFocus)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::DispatchDisplayHint(
    /* [in] */ Int32 hint)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::DispatchWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetWindowVisibility(
    /* [out] */ Int32 * pVisibility)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetWindowVisibleDisplayFrame(
    /* [in] */ IRect * pOutRect)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::DispatchConfigurationChanged(
    /* [in] */ IConfiguration * pNewConfig)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsInTouchMode(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetContext(
    /* [out] */ IContext ** ppCtx)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::OnKeyPreIme(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::OnKeyShortcut(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::OnCheckIsTextEditor(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::OnCreateInputConnection(
    /* [in] */ IEditorInfo * pOutAttrs,
    /* [out] */ IInputConnection ** ppInputConnection)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::CheckInputConnectionProxy(
    /* [in] */ IView * pView,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::CreateContextMenu(
    /* [in] */ IContextMenu * pMenu)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::OnTrackballEvent(
    /* [in] */ IMotionEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::OnTouchEvent(
    /* [in] */ IMotionEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetTouchDelegate(
    /* [in] */ ITouchDelegate * pTouchDelegate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetTouchDelegate(
    /* [out] */ ITouchDelegate ** ppTouchDelegate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::CancelLongPress()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::BringToFront()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetParent(
    /* [out] */ IViewParent ** ppParent)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetScrollX(
    /* [out] */ Int32 * pX)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetScrollY(
    /* [out] */ Int32 * pY)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetWidth(
    /* [out] */ Int32 * pWidth)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetHeight(
    /* [out] */ Int32 * pHeight)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetDrawingRect(
    /* [in] */ IRect * pOutRect)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetMeasuredWidth(
    /* [out] */ Int32 * pWidth)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetMeasuredHeight(
    /* [out] */ Int32 * pHeight)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetTop(
    /* [out] */ Int32 * pTop)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetBottom(
    /* [out] */ Int32 * pBottom)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetLeft(
    /* [out] */ Int32 * pLeft)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetRight(
    /* [out] */ Int32 * pRight)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetHitRect(
    /* [in] */ IRect * pOutRect)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetFocusedRect(
    /* [in] */ IRect * pOutRect)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetGlobalVisibleRect(
    /* [in] */ IRect * pOutRect,
    /* [in] */ IPoint * pOutGlobalOffset,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetGlobalVisibleRectEx(
    /* [in] */ IRect * pOutRect,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetLocalVisibleRect(
    /* [in] */ IRect * pOutRect,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::OffsetTopAndBottom(
    /* [in] */ Int32 offset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::OffsetLeftAndRight(
    /* [in] */ Int32 offset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetLayoutParams(
    /* [out] */ IViewGroupLayoutParams ** ppParams)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetLayoutParams(
    /* [in] */ IViewGroupLayoutParams * pParams)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::ScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::ScrollBy(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::Invalidate(
    /* [in] */ IRect * pDirty)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::InvalidateEx(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::InvalidateEx2()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsOpaque(
    /* [out] */ Boolean * pOpaque)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetHandler(
    /* [out] */ IApartment ** ppApartment)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::PostInvalidate()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::PostInvalidateEx(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::PostInvalidateDelayed(
    /* [in] */ Int64 delayMilliseconds)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::PostInvalidateDelayedEx(
    /* [in] */ Int64 delayMilliseconds,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::ComputeScroll()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsHorizontalFadingEdgeEnabled(
    /* [out] */ Boolean * pHorizontalFadingEdgeEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetHorizontalFadingEdgeEnabled(
    /* [in] */ Boolean horizontalFadingEdgeEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsVerticalFadingEdgeEnabled(
    /* [out] */ Boolean * pVerticalFadingEdgeEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetVerticalFadingEdgeEnabled(
    /* [in] */ Boolean verticalFadingEdgeEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsHorizontalScrollBarEnabled(
    /* [out] */ Boolean * pHorizontalScrollBarEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetHorizontalScrollBarEnabled(
    /* [in] */ Boolean horizontalScrollBarEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsVerticalScrollBarEnabled(
    /* [out] */ Boolean * pVerticalScrollBarEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetVerticalScrollBarEnabled(
    /* [in] */ Boolean verticalScrollBarEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetScrollbarFadingEnabled(
    /* [in] */ Boolean fadeScrollbars)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsScrollbarFadingEnabled(
    /* [out] */ Boolean * pFadeScrollbars)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetScrollBarStyle(
    /* [in] */ Int32 style)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetScrollBarStyle(
    /* [out] */ Int32 * pStyle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::AssignParent(
    /* [in] */ IViewParent * pParent)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetWindowToken(
    /* [out] */ IBinder ** ppToken)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetApplicationWindowToken(
    /* [out] */ IBinder ** ppToken)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SaveHierarchyState(
    /* [in] */ IObjectIntegerMap * pContainer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::RestoreHierarchyState(
    /* [in] */ IObjectIntegerMap * pContainer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetDrawingTime(
    /* [out] */ Int64 * pTime)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetDuplicateParentStateEnabled(
    /* [in] */ Boolean enabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsDuplicateParentStateEnabled(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetDrawingCacheEnabled(
    /* [in] */ Boolean enabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsDrawingCacheEnabled(
    /* [out] */ Boolean * pEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetDrawingCache(
    /* [out] */ IBitmap ** ppBitmap)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetDrawingCacheEx(
    /* [in] */ Boolean autoScale,
    /* [out] */ IBitmap ** ppBitmap)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::DestroyDrawingCache()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetDrawingCacheBackgroundColor(
    /* [in] */ Int32 color)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetDrawingCacheBackgroundColor(
    /* [out] */ Int32 * pColor)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::BuildDrawingCache()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::BuildDrawingCacheEx(
    /* [in] */ Boolean autoScale)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::CreateSnapshot(
    /* [in] */ BitmapConfig quality,
    /* [in] */ Int32 backgroundColor,
    /* [in] */ Boolean skipChildren,
    /* [out] */ IBitmap ** ppBitmap)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsInEditMode(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::Draw(
    /* [in] */ ICanvas * pCanvas)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetSolidColor(
    /* [out] */ Int32 * pColor)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::OnFinishInflate()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsLayoutRequested(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::Layout(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetResources(
    /* [out] */ IResources ** ppResources)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::UnscheduleDrawableEx(
    /* [in] */ IDrawable * pWho)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::RefreshDrawableState()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetDrawableState(
    /* [out, callee] */ ArrayOf<Int32> ** ppState)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetBackgroundColor(
    /* [in] */ Int32 color)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetBackgroundResource(
    /* [in] */ Int32 resid)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetBackgroundDrawable(
    /* [in] */ IDrawable * pD)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetBackground(
    /* [out] */ IDrawable ** ppDrawable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetPadding(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetPaddingTop(
    /* [out] */ Int32 * pTop)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetPaddingBottom(
    /* [out] */ Int32 * pBottom)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetPaddingLeft(
    /* [out] */ Int32 * pLeft)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetPaddingRight(
    /* [out] */ Int32 * pRight)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetSelected(
    /* [in] */ Boolean selected)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsSelected(
    /* [out] */ Boolean * pSelected)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetViewTreeObserver(
    /* [out] */ IViewTreeObserver ** ppObserver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetRootView(
    /* [out] */ IView ** ppView)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetLocationOnScreen(
    /* [out] */ Int32 * pX,
    /* [out] */ Int32 * pY)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetLocationInWindow(
    /* [out] */ Int32 * pX,
    /* [out] */ Int32 * pY)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::FindViewById(
    /* [in] */ Int32 id,
    /* [out] */ IView ** ppView)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetId(
    /* [in] */ Int32 id)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetId(
    /* [out] */ Int32 * pId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsRootNamespace(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetBaseline(
    /* [out] */ Int32 * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::RequestLayout()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::ForceLayout()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::Measure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetMinimumHeight(
    /* [in] */ Int32 minHeight)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetMinimumWidth(
    /* [in] */ Int32 minWidth)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetAnimation(
    /* [out] */ IAnimation ** ppAnimation)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::StartAnimation(
    /* [in] */ IAnimation * pAnimation)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::ClearAnimation()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetAnimation(
    /* [in] */ IAnimation * pAnimation)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GatherTransparentRegion(
    /* [in] */ IRegion * pRegion,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::PlaySoundEffect(
    /* [in] */ Int32 soundConstant)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::PerformHapticFeedback(
    /* [in] */ Int32 feedbackConstant,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::PerformHapticFeedbackEx(
    /* [in] */ Int32 feedbackConstant,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::OnCloseSystemDialogs(
    /* [in] */ const String& reason)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::ApplyDrawableToTransparentRegion(
    /* [in] */ IDrawable * pDr,
    /* [in] */ IRegion * pRegion)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetOverScrollMode(
    /* [out] */ Int32 * pMode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetOverScrollMode(
    /* [in] */ Int32 overScrollMode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}
#endif

#if 0
ECode CWebView::GetDescendantFocusability(
    /* [out] */ Int32 * pFocusability)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetDescendantFocusability(
    /* [in] */ Int32 focusability)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetFocusedChild(
    /* [out] */ IView ** ppFocusedChild)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent * pEv,
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetClipChildren(
    /* [in] */ Boolean clipChildren)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetClipToPadding(
    /* [in] */ Boolean clipToPadding)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::AddView(
    /* [in] */ IView * pChild)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::AddViewEx(
    /* [in] */ IView * pChild,
    /* [in] */ Int32 index)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::AddViewEx2(
    /* [in] */ IView * pChild,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::AddViewEx3(
    /* [in] */ IView * pChild,
    /* [in] */ IViewGroupLayoutParams * pParams)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::AddViewEx4(
    /* [in] */ IView * pChild,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams * pParams)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetOnHierarchyChangeListener(
    /* [in] */ IViewGroupOnHierarchyChangeListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::RemoveViewInLayout(
    /* [in] */ IView * pView)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::RemoveViewsInLayout(
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::RemoveViewAt(
    /* [in] */ Int32 index)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::RemoveViews(
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::RemoveAllViews()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::RemoveAllViewsInLayout()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::OffsetDescendantRectToMyCoords(
    /* [in] */ IView * pDescendant,
    /* [in] */ IRect * pRect)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::OffsetRectIntoDescendantCoords(
    /* [in] */ IView * pDescendant,
    /* [in] */ IRect * pRect)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::OffsetChildrenTopAndBottom(
    /* [in] */ Int32 offset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::StartLayoutAnimation()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::ScheduleLayoutAnimation()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetLayoutAnimation(
    /* [in] */ ILayoutAnimationController * pController)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetLayoutAnimation(
    /* [out] */ ILayoutAnimationController ** ppController)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsAnimationCacheEnabled(
    /* [out] */ Boolean * pEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetAnimationCacheEnabled(
    /* [in] */ Boolean enabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IsAlwaysDrawnWithCacheEnabled(
    /* [out] */ Boolean * pAlways)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetAlwaysDrawnWithCacheEnabled(
    /* [in] */ Boolean always)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetPersistentDrawingCache(
    /* [out] */ Int32 * pDrawingCacheToKeep)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetPersistentDrawingCache(
    /* [in] */ Int32 drawingCacheToKeep)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GenerateLayoutParams(
    /* [in] */ IAttributeSet * pAttrs,
    /* [out] */ IViewGroupLayoutParams ** ppLayoutParams)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::IndexOfChild(
    /* [in] */ IView * pChild,
    /* [out] */ Int32 * pIndex)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetChildCount(
    /* [out] */ Int32 * pCount)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetChildAt(
    /* [in] */ Int32 index,
    /* [out] */ IView ** ppChild)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::ClearDisappearingChildren()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetLayoutAnimationListener(
    /* [out] */ IAnimationListener ** ppListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetAddStatesFromChildren(
    /* [in] */ Boolean addsStates)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::AddStatesFromChildren(
    /* [out] */ Boolean * pDrawable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::SetLayoutAnimationListener(
    /* [in] */ IAnimationListener * pAnimationListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}
#endif


ECode CWebView::SetHorizontalScrollbarOverlay(
    /* [in] */ Boolean overlay)
{
    mOverlayHorizontalScrollbar = overlay;

    return NOERROR;
}

ECode CWebView::SetVerticalScrollbarOverlay(
    /* [in] */ Boolean overlay)
{
    mOverlayVerticalScrollbar = overlay;

    return NOERROR;
}

ECode CWebView::OverlayHorizontalScrollbar(
    /* [out] */ Boolean* bFlag)
{
    VALIDATE_NOT_NULL(bFlag);

    *bFlag = mOverlayHorizontalScrollbar;

    return NOERROR;
}

ECode CWebView::OverlayVerticalScrollbar(
    /* [out] */ Boolean * bFlag)
{
    VALIDATE_NOT_NULL(bFlag);

    *bFlag = mOverlayVerticalScrollbar;

    return NOERROR;
}

ECode CWebView::GetCertificate(
    /* [out] */ ISslCertificate** sc)
{
    VALIDATE_NOT_NULL(sc);

    *sc = mCertificate;

    return NOERROR;
}

ECode CWebView::SetCertificate(
    /* [in] */ ISslCertificate* certificate)
{
    VALIDATE_NOT_NULL(certificate);

    if (DebugFlags::sWEB_VIEW) {
//        Logger::V(LOGTAG, "setCertificate=" + certificate);
    }

    // here, the certificate can be null (if the site is not secure)
    mCertificate = certificate;

    return NOERROR;
}

ECode CWebView::SavePassword(
    /* [in] */ const String& host,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    ((CWebViewDatabase*)mDatabase.Get())->SetUsernamePassword(host, username, password);

    return NOERROR;
}

ECode CWebView::SetHttpAuthUsernamePassword(
    /* [in] */ const String& host,
    /* [in] */ const String& realm,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    ((CWebViewDatabase*)mDatabase.Get())->SetHttpAuthUsernamePassword(host, realm, username, password);

    return NOERROR;
}

ECode CWebView::GetHttpAuthUsernamePassword(
    /* [in] */ const String& host,
    /* [in] */ const String& realm,
    /* [out] */ ArrayOf<String>* strlist)
{
    VALIDATE_NOT_NULL(strlist);

    ((CWebViewDatabase*)mDatabase.Get())->GetHttpAuthUsernamePassword(host, realm, strlist);
    
    return NOERROR;
}

ECode CWebView::Destroy()
{
    ClearHelpers();
    if (mWebViewCore != NULL) {
        // Set the handlers to null before destroying WebViewCore so no
        // more messages will be posted.
        mCallbackProxy->SetWebViewClient(NULL);
        mCallbackProxy->SetWebChromeClient(NULL);
        // Tell WebViewCore to destroy itself
//        synchronized (this) {
            AutoPtr<WebViewCore> webViewCore = mWebViewCore;
            mWebViewCore = NULL; // prevent using partial webViewCore
            webViewCore->Destroy();
//        }
        // Remove any pending messages that might not be serviced yet.
//        mPrivateHandler->RemoveCallbacksAndMessages(NULL);
//        mCallbackProxy->RemoveCallbacksAndMessages(NULL);
        // Wake up the WebCore thread just in case it is waiting for a
        // javascript dialog.
//        synchronized (mCallbackProxy) {
//            mCallbackProxy.notify();
//        }
    }
    if (mNativeClass != 0) {
        NativeDestroy();
        mNativeClass = 0;
    }

    return NOERROR;
}

ECode CWebView::EnablePlatformNotifications()
{
    Network::EnablePlatformNotifications();
    
    return NOERROR;
}

ECode CWebView::DisablePlatformNotifications()
{
    Network::DisablePlatformNotifications();

    return NOERROR;
}

ECode CWebView::SetJsFlags(
    /* [in] */ const String& flags)
{
    mWebViewCore->SendMessage(WebViewCore::EventHub::SET_JS_FLAGS, (IInterface*)&flags);

    return NOERROR;
}

ECode CWebView::SetNetworkAvailable(
    /* [in] */ Boolean networkUp)
{
    mWebViewCore->SendMessage(WebViewCore::EventHub::SET_NETWORK_STATE,
                networkUp ? 1 : 0, 0);

    return NOERROR;
}

ECode CWebView::SetNetworkType(
    /* [in] */ const String& type,
    /* [in] */ const String& subtype)
{
    AutoPtr<IObjectStringMap> map;
    CObjectStringMap::New((IObjectStringMap**)&map);

    map->Put((String)"type", (IInterface*)&type);
    map->Put((String)"subtype", (IInterface*)&subtype);
    mWebViewCore->SendMessage(WebViewCore::EventHub::SET_NETWORK_TYPE, (IInterface*)map);

    return NOERROR;
}

ECode CWebView::SaveState(
    /* [in] */ IBundle* outState,
    /* [out] */ IWebBackForwardList** wfl)
{
    VALIDATE_NOT_NULL(wfl);

    if (outState == NULL) {
        *wfl = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // We grab a copy of the back/forward list because a client of WebView
    // may have invalidated the history list by calling clearHistory.
    AutoPtr<IWebBackForwardList> list;
    CopyBackForwardList((IWebBackForwardList**)&list);
    assert(list);

    Int32 currentIndex, size;

    list->GetCurrentIndex(&currentIndex);
    list->GetSize(&size);

    // We should fail saving the state if the list is empty or the index is
    // not in a valid range.
    if (currentIndex < 0 || currentIndex >= size || size == 0) {
        *wfl = NULL;
        return E_FAIL;
    }
#if 0
    outState.putInt("index", currentIndex);

    // FIXME: This should just be a byte[][] instead of ArrayList but
    // Parcel.java does not have the code to handle multi-dimensional
    // arrays.
    ArrayList<byte[]> history = new ArrayList<byte[]>(size);
    for (int i = 0; i < size; i++) {
        WebHistoryItem item = list.getItemAtIndex(i);
        if (null == item) {
            // FIXME: this shouldn't happen
            // need to determine how item got set to null
            Log.w(LOGTAG, "saveState: Unexpected null history item.");
            return null;
        }
        byte[] data = item.getFlattenedData();
        if (data == null) {
            // It would be very odd to not have any data for a given history
            // item. And we will fail to rebuild the history list without
            // flattened data.
            return null;
        }
        history.add(data);
    }
    outState.PutSerializable("history", history);
    if (mCertificate != null) {
        outState.putBundle("certificate",
                           SslCertificate.saveState(mCertificate));
    }
    return list;
#endif
    
    return NOERROR;
}

ECode CWebView::SavePicture(
    /* [in] */ IBundle* b,
    /* [in] */ IFile* dest,
    /* [out] */ Boolean* bFlag)
{
    VALIDATE_NOT_NULL(bFlag);

    if (dest == NULL || b == NULL) {
        *bFlag = FALSE;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    const AutoPtr<IPicture> p;
    CapturePicture((IPicture**)&p);
    assert(p);

    // Use a temporary file while writing to ensure the destination file
    // contains valid data.
    String path, filePath;
    dest->GetPath(&path);
    filePath = path + ".writing";
    const AutoPtr<IFile> temp;
    CFile::New(filePath, (IFile**)&temp);
#if 0
    new Thread(new Runnable() {
        public void run() {
            FileOutputStream out = null;
            try {
                out = new FileOutputStream(temp);
                p.writeToStream(out);
                // Writing the picture succeeded, rename the temporary file
                // to the destination.
                temp.renameTo(dest);
            } catch (Exception e) {
                // too late to do anything about it.
            } finally {
                if (out != null) {
                    try {
                        out.close();
                    } catch (Exception e) {
                        // Can't do anything about that
                    }
                }
                temp.delete();
            }
        }
    }).start();

    // now update the bundle
    b->PutInt32("scrollX", mScrollX);
    b->PutInt32("scrollY", mScrollY);
    b->PutFloat("scale", mActualScale);
    b->PutFloat("textwrapScale", mTextWrapScale);
    b->PutBoolean("overview", mInZoomOverview);
#endif    
    *bFlag = TRUE;

    return NOERROR;
}

ECode CWebView::RestorePicture(
    /* [in] */ IBundle* b,
    /* [in] */ IFile* src,
    /* [out] */ Boolean* bFlag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::RestoreState(
    /* [in] */ IBundle* inState,
    /* [out] */ IWebBackForwardList** wfl)
{
    VALIDATE_NOT_NULL(inState);

    if (inState == NULL) {
        *wfl = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
#if 0
    if (inState.containsKey("index") && inState.containsKey("history")) {
        mCertificate = SslCertificate.restoreState(
            inState.getBundle("certificate"));

        final WebBackForwardList list = mCallbackProxy.getBackForwardList();
        final int index = inState.getInt("index");
        // We can't use a clone of the list because we need to modify the
        // shared copy, so synchronize instead to prevent concurrent
        // modifications.
        synchronized (list) {
            final List<byte[]> history =
                    (List<byte[]>) inState.getSerializable("history");
            final int size = history.size();
            // Check the index bounds so we don't crash in native code while
            // restoring the history index.
            if (index < 0 || index >= size) {
                return null;
            }
            for (int i = 0; i < size; i++) {
                byte[] data = history.remove(0);
                if (data == null) {
                    // If we somehow have null data, we cannot reconstruct
                    // the item and thus our history list cannot be rebuilt.
                    return null;
                }
                WebHistoryItem item = new WebHistoryItem(data);
                list.addHistoryItem(item);
            }
            // Grab the most recent copy to return to the caller.
            returnList = copyBackForwardList();
            // Update the copy to have the correct index.
            returnList.setCurrentIndex(index);
        }
        // Remove all pending messages because we are restoring previous
        // state.
        mWebViewCore.removeMessages();
        // Send a restore state message.
        mWebViewCore.sendMessage(EventHub.RESTORE_STATE, index);
    }
    return returnList;
#endif

    return NOERROR;
}

ECode CWebView::LoadUrlEx(
    /* [in] */ const String& url,
    /* [in] */ IObjectStringMap* extraHeaders)
{
//    VALIDATE_NOT_NULL(extraHeaders);

    SwitchOutDrawHistory();
    WebViewCore::GetUrlData* arg = new WebViewCore::GetUrlData();
    arg->mUrl = url;
    arg->mExtraHeaders = extraHeaders;
    mWebViewCore->SendMessage(WebViewCore::EventHub::LOAD_URL, (IInterface*)arg);
    ClearHelpers();

    return NOERROR;
}

ECode CWebView::LoadUrl(
    /* [in] */ const String& url)
{
    LoadUrlEx(url, NULL);

    return NOERROR;
}

ECode CWebView::PostUrl(
    /* [in] */ const String& url,
    /* [in] */ const ArrayOf<Byte> & postData)
{
    Boolean bFlag = FALSE;

    bFlag = CURLUtil::IsNetworkUrl(url);
    if (bFlag) {
        SwitchOutDrawHistory();
        WebViewCore::PostUrlData* arg = new WebViewCore::PostUrlData();
        arg->mUrl = url;
        arg->mPostData = const_cast<ArrayOf<Byte>*>(&postData);
        mWebViewCore->SendMessage(WebViewCore::EventHub::POST_URL, (IInterface*)arg);
        ClearHelpers();
    } else {
        LoadUrl(url);
    }

    return NOERROR;
}

ECode CWebView::LoadData(
    /* [in] */ const String& data,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding)
{
    String str("data:");
    str += mimeType;
    str += ";";
    str += encoding;
    str += ",";
    str += data;

    return LoadUrl(str);
}

ECode CWebView::LoadDataWithBaseURL(
    /* [in] */ const String& _baseUrl,
    /* [in] */ const String& data,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding,
    /* [in] */ const String& historyUrl)
{
    String baseUrl = const_cast<String&>(_baseUrl);
    if (baseUrl.GetLength() != 0 && (baseUrl.ToLowerCase(), baseUrl.StartWith("data:"))) {
        LoadData(data, mimeType, encoding);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    SwitchOutDrawHistory();
    WebViewCore::BaseUrlData* arg = new WebViewCore::BaseUrlData();
    arg->mBaseUrl = baseUrl;
    arg->mData = data;
    arg->mMimeType = mimeType;
    arg->mEncoding = encoding;
    arg->mHistoryUrl = historyUrl;
    mWebViewCore->SendMessage(WebViewCore::EventHub::LOAD_DATA, (IInterface*)arg);
    ClearHelpers();

    return NOERROR;
}

ECode CWebView::StopLoading()
{
    // TODO: should we clear all the messages in the queue before sending
    // STOP_LOADING?
    SwitchOutDrawHistory();
    mWebViewCore->SendMessage(WebViewCore::EventHub::STOP_LOADING);

    return NOERROR;
}

ECode CWebView::Reload()
{
    ClearHelpers();
    SwitchOutDrawHistory();
    mWebViewCore->SendMessage(WebViewCore::EventHub::RELOAD);

    return NOERROR;
}

ECode CWebView::CanGoBack(
    /* [out] */ Boolean* bFlag)
{
    VALIDATE_NOT_NULL(bFlag);

    AutoPtr<IWebBackForwardList> l;
    mCallbackProxy->GetBackForwardList((IWebBackForwardList**)&l);
    assert(l != NULL);
//    synchronized (l) {
        *bFlag=((CWebBackForwardList*)l.Get())->GetClearPending();
        if (*bFlag) {
            *bFlag = FALSE;
            return NOERROR;
        } else {
            Int32 index = 0;
            l->GetCurrentIndex(&index);
            *bFlag = index > 0;
            return NOERROR;
        }
//    }

    return NOERROR;
}

ECode CWebView::GoBack()
{
    return GoBackOrForward(-1);
}

ECode CWebView::CanGoForward(
    /* [out] */ Boolean* bFlag)
{
    VALIDATE_NOT_NULL(bFlag);

    AutoPtr<IWebBackForwardList> l;
    mCallbackProxy->GetBackForwardList((IWebBackForwardList**)&l);
    assert(l.Get());
//    synchronized (l) {
        *bFlag=((CWebBackForwardList*)l.Get())->GetClearPending();
        if (*bFlag) {
            *bFlag = FALSE;
            return NOERROR;
        } else {
            Int32 index = 0;
            Int32 size = 0;

            l->GetCurrentIndex(&index);
            l->GetSize(&size);
            *bFlag = index < size - 1;

            return NOERROR;
        }
//    }

    return NOERROR;
}

ECode CWebView::GoForward()
{
    return GoBackOrForward(1);
}

ECode CWebView::CanGoBackOrForward(
    /* [in] */ Int32 steps,
    /* [out] */ Boolean* bFlag)
{
    VALIDATE_NOT_NULL(bFlag);

    AutoPtr<IWebBackForwardList> l;
    mCallbackProxy->GetBackForwardList((IWebBackForwardList**)&l);
    assert(l.Get());
//    synchronized (l) {
        Boolean clearPending = FALSE;
        clearPending=((CWebBackForwardList*)l.Get())->GetClearPending();
        if (clearPending) {
            *bFlag = FALSE;
            return NOERROR;
        } else {
            Int32 newIndex = 0;
            l->GetCurrentIndex(&newIndex);
            newIndex += steps;

            Int32 size = 0;
            l->GetSize(&size);
            *bFlag = (newIndex >= 0) && (newIndex < size);

            return NOERROR;
        }
//    }

    return NOERROR;
}

ECode CWebView::GoBackOrForward(
    /* [in] */ Int32 steps)
{
    GoBackOrForward(steps, FALSE);

    return NOERROR;
}

ECode CWebView::PageUp(
    /* [in] */ Boolean top,
    /* [out] */ Boolean * bFlag)
{
    VALIDATE_NOT_NULL(bFlag);

    if (mNativeClass == 0) {
        *bFlag = FALSE;
        return NOERROR;
    }

    NativeClearCursor(); // start next trackball movement from page edge
    
    if (top) {
        // go to the top of the document
        *bFlag = PinScrollTo(mScrollX, 0, TRUE, 0);
        return NOERROR;
    }

    // Page up
    Int32 h = 0, y = 0;
    GetHeight(&h);
    if (h > 2 * PAGE_SCROLL_OVERLAP) {
        y = -h + PAGE_SCROLL_OVERLAP;
    } else {
        y = -h / 2;
    }
    mUserScroll = TRUE;
    *bFlag = mScroller->IsFinished() ? PinScrollBy(0, y, TRUE, 0)
            : ExtendScroll(y);

    return NOERROR;
}

ECode CWebView::PageDown(
    /* [in] */ Boolean bottom,
    /* [out] */ Boolean* bFlag)
{
    VALIDATE_NOT_NULL(bFlag);

    if (mNativeClass == 0) {
        *bFlag = FALSE;
        return NOERROR;
    }

    NativeClearCursor(); // start next trackball movement from page edge
    
    if (bottom) {
        *bFlag = PinScrollTo(mScrollX, ComputeRealVerticalScrollRange(), TRUE, 0);
        return NOERROR;
    }

    // Page down.
    Int32 h = 0, y = 0;
    GetHeight(&h);
    if (h > 2 * PAGE_SCROLL_OVERLAP) {
        y = h - PAGE_SCROLL_OVERLAP;
    } else {
        y = h / 2;
    }
    mUserScroll = TRUE;

    *bFlag = mScroller->IsFinished() ? PinScrollBy(0, y, TRUE, 0)
            : ExtendScroll(y);

    return NOERROR;
}

ECode CWebView::ClearView()
{
    mContentWidth = 0;
    mContentHeight = 0;
    mWebViewCore->SendMessage(WebViewCore::EventHub::CLEAR_CONTENT);

    return NOERROR;
}

ECode CWebView::CapturePicture(
    /* [out] */ IPicture** pic)
{
    VALIDATE_NOT_NULL(pic);

    if (NULL == mWebViewCore)
    {
        *pic = NULL;
        return E_FAIL; // check for out of memory tab
    }

    *pic = mWebViewCore->CopyContentPicture();

    return NOERROR;
}

ECode CWebView::GetScale(
    /* [out] */ Float* scale)
{
    VALIDATE_NOT_NULL(scale);

    *scale = mActualScale;

    return NOERROR;
}

ECode CWebView::SetInitialScale(
    /* [in] */ Int32 scaleInPercent)
{
    mInitialScaleInPercent = scaleInPercent;
    
    return NOERROR;
}

ECode CWebView::InvokeZoomPicker()
{
    AutoPtr<IWebSettings> ws;

    GetSettings((IWebSettings**)&ws);
    assert(ws.Get());

    Boolean bFlag = FALSE;
    ws->SupportZoom(&bFlag);

    if (!bFlag) {
        Logger::W(LOGTAG, "This WebView doesn't support zoom.");
        return NOERROR;
    }

    ClearTextEntry(FALSE);

    ws->GetBuiltInZoomControls(&bFlag);
    if (bFlag) {
//        GetZoomButtonsController().setVisible(TRUE);
    } else {
//        mPrivateHandler.removeCallbacks(mZoomControlRunnable);
//        mPrivateHandler.postDelayed(mZoomControlRunnable, ZOOM_CONTROLS_TIMEOUT);
    }

    return NOERROR;
}

ECode CWebView::GetHitTestResult(
    /* [out] */ IHitTestResult** result)
{
    VALIDATE_NOT_NULL(result);

    if (mNativeClass == 0) {
        *result = NULL;
        return E_FAIL;
    }

    //HitTestResult result = new HitTestResult();
    *result = new HitTestResult();
    assert(*result);
    if (NativeHasCursorNode()) {
        if (NativeCursorIsTextInput()) {
            ((HitTestResult*)*result)->SetType(HitTestResult_EDIT_TEXT_TYPE);
        } else {
            String text = NativeCursorText();
            if (text.GetLength() != 0) {
                if (text.StartWith(SCHEME_TEL)) {
                    ((HitTestResult*)*result)->SetType(HitTestResult_PHONE_TYPE);
                    ((HitTestResult*)*result)->SetExtra(text.Substring(String(SCHEME_TEL).GetLength()));
                } else if (text.StartWith(SCHEME_MAILTO)) {
                    ((HitTestResult*)*result)->SetType(HitTestResult_EMAIL_TYPE);
                    ((HitTestResult*)*result)->SetExtra(text.Substring(String(SCHEME_MAILTO).GetLength()));
                } else if (text.StartWith(SCHEME_GEO)) {
                    ((HitTestResult*)*result)->SetType(HitTestResult_GEO_TYPE);
//                    ((HitTestResult*)*result)->SetExtra(URLDecoder.decode(text.substring(SCHEME_GEO.length())));
                } else if (NativeCursorIsAnchor()) {
                    ((HitTestResult*)*result)->SetType(HitTestResult_SRC_ANCHOR_TYPE);
                    ((HitTestResult*)*result)->SetExtra(text);
                }
            }
        }
    }

    Int32 type;
    (*result)->GetType(&type);
    if (type == HitTestResult_UNKNOWN_TYPE
            || type == HitTestResult_SRC_ANCHOR_TYPE) {
        // Now check to see if it is an image.
        Int32 contentX = ViewToContentX((Int32) mLastTouchX + mScrollX);
        Int32 contentY = ViewToContentY((Int32) mLastTouchY + mScrollY);
        String text = NativeImageURI(contentX, contentY);
        if (text.GetLength() != 0) {
            ((HitTestResult*)*result)->SetType(type == HitTestResult_UNKNOWN_TYPE ?
                    HitTestResult_IMAGE_TYPE :
                    HitTestResult_SRC_IMAGE_ANCHOR_TYPE);
            ((HitTestResult*)*result)->SetExtra(text);
        }
    }

    return NOERROR;
}

ECode CWebView::RequestFocusNodeHref(
    /* [in] */ IMessage* hrefMsg)
{
    if (hrefMsg == NULL || mNativeClass == 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (NativeCursorIsAnchor()) {
        mWebViewCore->SendMessage(WebViewCore::EventHub::REQUEST_CURSOR_HREF,
                NativeCursorFramePointer(), NativeCursorNodePointer(),
                hrefMsg);
    }

    return NOERROR;
}

ECode CWebView::RequestImageRef(
    /* [in] */ IMessage* msg)
{
    if (0 == mNativeClass) {
        return E_FAIL; // client isn't initialized
    }

    Int32 contentX = ViewToContentX((Int32) mLastTouchX + mScrollX);
    Int32 contentY = ViewToContentY((Int32) mLastTouchY + mScrollY);
    String ref = NativeImageURI(contentX, contentY);
//    Bundle data = msg.getData();
//    data.putString("url", ref);
//    msg.setData(data);
//    msg.sendToTarget();

    return NOERROR;
}

ECode CWebView::SetEmbeddedTitleBar(
    /* [in] */ IView* v)
{
    if (mTitleBar.Get() == v) {
        return NOERROR;
    }

    if (mTitleBar != NULL) {
        RemoveView(mTitleBar);
    }

    if (NULL != v) {
//        AddView(v, new AbsoluteLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
//                ViewGroup.LayoutParams.WRAP_CONTENT, 0, 0));
        if (mTitleShadow == NULL) {
//            mTitleShadow = (Drawable) mContext.getResources().getDrawable(com.android.internal.R.drawable.title_bar_shadow);
        }
    }

    mTitleBar = v;

    return NOERROR;
}

ECode CWebView::GetUrl(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    AutoPtr<IWebBackForwardList> wfl;
    AutoPtr<IWebHistoryItem> h;

    mCallbackProxy->GetBackForwardList((IWebBackForwardList**)&wfl);
    assert(wfl.Get());
    wfl->GetCurrentItem((IWebHistoryItem**)&h);

    //h != NULL ? h->GetUrl((CString*)str) : str = NULL;
    if (h != NULL) {
        h->GetUrl(str);
    }

    return NOERROR;
}

ECode CWebView::GetOriginalUrl(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    AutoPtr<IWebBackForwardList> wfl;
    AutoPtr<IWebHistoryItem> h;

    mCallbackProxy->GetBackForwardList((IWebBackForwardList**)&wfl);
    assert(wfl.Get());
    wfl->GetCurrentItem((IWebHistoryItem**)&h);

    //return h != null ? h.getOriginalUrl() : null;
    if (h != NULL) {
        h->GetOriginalUrl(str);
    }

    return NOERROR;
}

ECode CWebView::GetTitle(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    AutoPtr<IWebBackForwardList> wfl;
    AutoPtr<IWebHistoryItem> h;

    mCallbackProxy->GetBackForwardList((IWebBackForwardList**)&wfl);
    assert(wfl.Get());
    wfl->GetCurrentItem((IWebHistoryItem**)&h);

    if (h != NULL) {
        h->GetTitle(str);
    }

    return NOERROR;
}

ECode CWebView::GetFavicon(
    /* [out] */ IBitmap** bm)
{
    VALIDATE_NOT_NULL(bm);

    AutoPtr<IWebBackForwardList> wfl;
    AutoPtr<IWebHistoryItem> h;

    mCallbackProxy->GetBackForwardList((IWebBackForwardList**)&wfl);
    assert(wfl.Get());
    wfl->GetCurrentItem((IWebHistoryItem**)&h);

    if (h != NULL) {
        h->GetFavicon(bm);
    }

    return NOERROR;
}

ECode CWebView::GetTouchIconUrl(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    AutoPtr<IWebBackForwardList> wfl;
    AutoPtr<IWebHistoryItem> h;

    mCallbackProxy->GetBackForwardList((IWebBackForwardList**)&wfl);
    assert(wfl.Get());
    wfl->GetCurrentItem((IWebHistoryItem**)&h);

    if (h != NULL) {
        h->GetTouchIconUrl(str);
    }

    return NOERROR;
}

ECode CWebView::GetProgress(
    /* [out] */ Int32* progress)
{
    VALIDATE_NOT_NULL(progress);

    mCallbackProxy->GetProgress(progress);

    return NOERROR;
}

ECode CWebView::GetContentHeight(
    /* [out] */ Int32* progress)
{
    VALIDATE_NOT_NULL(progress);
    *progress = mContentHeight;

    return NOERROR;
}

ECode CWebView::GetContentWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = mContentWidth;

    return NOERROR;
}

ECode CWebView::PauseTimers()
{
    mWebViewCore->SendMessage(WebViewCore::EventHub::PAUSE_TIMERS);

    return NOERROR;
}

ECode CWebView::ResumeTimers()
{
    mWebViewCore->SendMessage(WebViewCore::EventHub::RESUME_TIMERS);

    return NOERROR;
}

ECode CWebView::OnPause()
{
    if (!mIsPaused) {
        mIsPaused = TRUE;
        mWebViewCore->SendMessage(WebViewCore::EventHub::ON_PAUSE);
    }

    return NOERROR;
}

ECode CWebView::OnResume()
{
    if (mIsPaused) {
        mIsPaused = FALSE;
        mWebViewCore->SendMessage(WebViewCore::EventHub::ON_RESUME);
    }

    return NOERROR;
}

ECode CWebView::IsPaused(
    /* [out] */ Boolean* bFlag)
{
    VALIDATE_NOT_NULL(bFlag);
    *bFlag = mIsPaused;

    return NOERROR;
}

ECode CWebView::FreeMemory()
{
    mWebViewCore->SendMessage(WebViewCore::EventHub::FREE_MEMORY);

    return NOERROR;
}

ECode CWebView::ClearCache(
    /* [in] */ Boolean includeDiskFiles)
{
    // Note: this really needs to be a static method as it clears cache for all
    // WebView. But we need mWebViewCore to send message to WebCore thread, so
    // we can't make this static.
    mWebViewCore->SendMessage(WebViewCore::EventHub::CLEAR_CACHE,
            includeDiskFiles ? 1 : 0, 0);

    return NOERROR;
}

ECode CWebView::ClearFormData()
{
    if (InEditingMode()) {
        WebTextView::AutoCompleteAdapter* adapter = NULL;
        mWebTextView->SetAdapterCustom(adapter);
    }

    return NOERROR;
}

ECode CWebView::ClearHistory()
{
    AutoPtr<IWebBackForwardList> wfl;
    mCallbackProxy->GetBackForwardList((IWebBackForwardList**)&wfl);
    assert(wfl.Get());
    ((CWebBackForwardList*)wfl.Get())->SetClearPending();
    mWebViewCore->SendMessage(WebViewCore::EventHub::CLEAR_HISTORY);

    return NOERROR;
}

ECode CWebView::ClearSslPreferences()
{
    mWebViewCore->SendMessage(WebViewCore::EventHub::CLEAR_SSL_PREF_TABLE);

    return NOERROR;
}

ECode CWebView::CopyBackForwardList(
    /* [out] */ IWebBackForwardList** wfl)
{
    VALIDATE_NOT_NULL(wfl);

    mCallbackProxy->GetBackForwardList(wfl);
    assert(*wfl);
//    *wfl = ((CWebBackForwardList*)*wfl)->Clone();

    return NOERROR;
}

ECode CWebView::FindNext(
    /* [in] */ Boolean forward)
{
    if (0 == mNativeClass) {
        return E_FAIL; // client isn't initialized
    }

    NativeFindNext(forward);

    return NOERROR;
}

ECode CWebView::FindAll(
    /* [in] */ const String& find,
    /* [out] */ Int32* f)
{
    if (0 == mNativeClass) {
        if (f) {
            *f = 0;
        }
        
        return E_FAIL; // client isn't initialized
    }

    String lower(find), upper(find);
    lower.ToLowerCase();
    upper.ToUpperCase();
    Int32 result = find.GetLength() != 0 ? NativeFindAll(lower,
            upper) : 0;
    InvalidateEx2();
    mLastFind = find;
    
    if (f) {
        *f = result;
    }
    
    return NOERROR;
}

ECode CWebView::SetFindIsUp(
    /* [in] */ Boolean isUp)
{
    mFindIsUp = isUp;
    if (isUp) {
        RecordNewContentSize(mContentWidth, mContentHeight + mFindHeight,
                FALSE);
    }

    if (0 == mNativeClass) {
        return E_FAIL; // client isn't initialized
    }

    NativeSetFindIsUp(isUp);

    return NOERROR;
}

ECode CWebView::FindIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    if (0 == mNativeClass) {
        *index = -1;
        return E_FAIL;
    }

    *index = NativeFindIndex();

    return NOERROR;
}

ECode CWebView::FindAddress(
    /* [in] */ const String& addr,
    /* [out] */ String* address)
{
    VALIDATE_NOT_NULL(address);

    return FindAddressEx(addr, false, address);
}

ECode CWebView::FindAddressEx(
    /* [in] */ const String& addr,
    /* [in] */ Boolean caseInsensitive,
    /* [out] */ String* address)
{
    VALIDATE_NOT_NULL(address);

    *address = WebViewCore::NativeFindAddress(addr, caseInsensitive);

    return NOERROR;
}

ECode CWebView::ClearMatches()
{
    mLastFind = "";
    if (mNativeClass == 0) {
        return E_FAIL;
    }

    NativeSetFindIsEmpty();
    InvalidateEx2();

    return NOERROR;
}

ECode CWebView::NotifyFindDialogDismissed()
{
    if (mWebViewCore == NULL) {
        return E_FAIL;
    }

    ClearMatches();
    SetFindIsUp(FALSE);
    RecordNewContentSize(mContentWidth, mContentHeight - mFindHeight,
            FALSE);
    // Now that the dialog has been removed, ensure that we scroll to a
    // location that is not beyond the end of the page.
    PinScrollTo(mScrollX, mScrollY, FALSE, 0);
    InvalidateEx2();

    return NOERROR;
}

ECode CWebView::SetFindDialogHeight(
    /* [in] */ Int32 height)
{
    if (DebugFlags::sWEB_VIEW) {
        Logger::V(LOGTAG, "setFindDialogHeight height=%d" + height);
    }
 
    mFindHeight = height;
    
    return NOERROR;
}

ECode CWebView::DocumentHasImages(
    /* [in] */ IMessage* response)
{
    if (response == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mWebViewCore->SendMessage(WebViewCore::EventHub::DOC_HAS_IMAGES, response);

    return NOERROR;
}

ECode CWebView::SetWebViewClient(
    /* [in] */ IWebViewClient* client)
{
    VALIDATE_NOT_NULL(client);

    return mCallbackProxy->SetWebViewClient(client);
}

ECode CWebView::GetWebViewClient(
    /* [out] */ IWebViewClient** wc)
{
    VALIDATE_NOT_NULL(wc);

    return mCallbackProxy->GetWebViewClient(wc);
}

ECode CWebView::SetDownloadListener(
    /* [in] */ IDownloadListener* listener)
{
    VALIDATE_NOT_NULL(listener);

    return mCallbackProxy->SetDownloadListener(listener);
}

ECode CWebView::SetWebChromeClient(
    /* [in] */ IWebChromeClient* client)
{
    VALIDATE_NOT_NULL(client);

    return mCallbackProxy->SetWebChromeClient(client);
}

ECode CWebView::GetWebChromeClient(
    /* [out] */ IWebChromeClient** wcc)
{
    VALIDATE_NOT_NULL(wcc);

    return mCallbackProxy->GetWebChromeClient(wcc);
}

ECode CWebView::SetWebBackForwardListClient(
    /* [in] */ IWebBackForwardListClient* client)
{
    VALIDATE_NOT_NULL(client);

    ((CCallbackProxy*)mCallbackProxy.Get())->SetWebBackForwardListClient(client);
}

ECode CWebView::GetWebBackForwardListClient(
    /* [out] */ IWebBackForwardListClient** wfl)
{
    VALIDATE_NOT_NULL(wfl);

    *wfl = ((CCallbackProxy*)mCallbackProxy.Get())->GetWebBackForwardListClient();

    return NOERROR;
}

ECode CWebView::SetPictureListener(
    /* [in] */ IPictureListener* listener)
{
    VALIDATE_NOT_NULL(listener);

    mPictureListener = listener;

    return NOERROR;
}

ECode CWebView::ExternalRepresentation(
    /* [in] */ IMessage* callBack)
{
    VALIDATE_NOT_NULL(callBack);

    mWebViewCore->SendMessage(WebViewCore::EventHub::REQUEST_EXT_REPRESENTATION, callBack);

    return NOERROR;
}

ECode CWebView::DocumentAsText(
    /* [in] */ IMessage* callBack)
{
    VALIDATE_NOT_NULL(callBack);

    mWebViewCore->SendMessage(WebViewCore::EventHub::REQUEST_DOC_AS_TEXT, callBack);

    return NOERROR;
}

ECode CWebView::AddJavascriptInterface(
    /* [in] */ IInterface* obj,
    /* [in] */ const String& interfaceName)
{
    VALIDATE_NOT_NULL(obj);

    WebViewCore::JSInterfaceData* arg = new WebViewCore::JSInterfaceData();
    arg->mObject = obj;
    arg->mInterfaceName = interfaceName;
    mWebViewCore->SendMessage(WebViewCore::EventHub::ADD_JS_INTERFACE, (IInterface*)arg);

    return NOERROR;
}

ECode CWebView::GetSettings(
    /* [out] */ IWebSettings** ws)
{
    VALIDATE_NOT_NULL(ws);

//    *ws = mWebViewCore->GetSettings();

    return NOERROR;
}

ECode CWebView::AddPackageNames(
    /* [in] */ IObjectContainer* packageNames)
{
    VALIDATE_NOT_NULL(packageNames);

    mWebViewCore->SendMessage(WebViewCore::EventHub::ADD_PACKAGE_NAMES, packageNames);

    return NOERROR;
}

ECode CWebView::AddPackageName(
    /* [in] */ const String& packageName)
{
    mWebViewCore->SendMessage(WebViewCore::EventHub::ADD_PACKAGE_NAME, (IInterface*)&packageName);

    return NOERROR;
}

ECode CWebView::RemovePackageName(
    /* [in] */ const String& packageName)
{
    mWebViewCore->SendMessage(WebViewCore::EventHub::REMOVE_PACKAGE_NAME, (IInterface*)&packageName);

    return NOERROR;
}

ECode CWebView::GetPluginList(
    /* [out] */ IPluginList** pl)
{
    VALIDATE_NOT_NULL(pl);

    return CPluginList::New(pl);
}

ECode CWebView::RefreshPlugins(
    /* [in] */ Boolean reloadOpenPages)
{
    return NOERROR;
}

ECode CWebView::DumpDisplayTree()
{
    String url;
    GetUrl(&url);
    NativeDumpDisplayTree(url);

    return NOERROR;
}

ECode CWebView::DumpDomTree(
    /* [in] */ Boolean toFile)
{
    mWebViewCore->SendMessage(WebViewCore::EventHub::DUMP_DOMTREE, toFile ? 1 : 0, 0);

    return NOERROR;
}

ECode CWebView::DumpRenderTree(
    /* [in] */ Boolean toFile)
{
    mWebViewCore->SendMessage(WebViewCore::EventHub::DUMP_RENDERTREE, toFile ? 1 : 0, 0);

    return NOERROR;
}

ECode CWebView::DumpV8Counters()
{
    mWebViewCore->SendMessage(WebViewCore::EventHub::DUMP_V8COUNTERS);

    return NOERROR;
}

ECode CWebView::SetUpSelect()
{
    if (0 == mNativeClass) {
        return E_FAIL; // client isn't initialized
    }

    if (InFullScreenMode()) {
        return E_FAIL;
    }

    if (mSelectingText) {
        return E_FAIL;
    }

    mExtendSelection = FALSE;
    mSelectingText = mDrawSelectionPointer = TRUE;
    // don't let the picture change during text selection
    WebViewCore::PauseUpdatePicture(mWebViewCore);
    NativeResetSelection();
    if (NativeHasCursorNode()) {
        AutoPtr<IRect> rect = NativeCursorNodeBounds();
        Int32 left, top;
        rect->GetLeft(&left);
        rect->GetTop(&top);
        mSelectX = ContentToViewX(left);
        mSelectY = ContentToViewY(top);
    } else if (mLastTouchY > GetVisibleTitleHeight()) {
        mSelectX = mScrollX + (Int32) mLastTouchX;
        mSelectY = mScrollY + (Int32) mLastTouchY;
    } else {
        mSelectX = mScrollX + GetViewWidth() / 2;
        mSelectY = mScrollY + GetViewHeightWithTitle() / 2;
    }

    NativeHideCursor();

    return NOERROR;
}

ECode CWebView::EmulateShiftHeld()
{
    return SetUpSelect();
}

ECode CWebView::SelectAll()
{
    if (0 == mNativeClass) {
        return E_FAIL; // client isn't initialized
    }

    if (InFullScreenMode()) {
        return E_FAIL;
    }

    if (!mSelectingText) {
        SetUpSelect();
    }

    NativeSelectAll();
    mDrawSelectionPointer = FALSE;
    mExtendSelection = TRUE;
    InvalidateEx2();

    return NOERROR;
}

ECode CWebView::SelectDialogIsUp(
    /* [out] */ Boolean* bFlag)
{
    VALIDATE_NOT_NULL(bFlag);
    *bFlag = mSelectingText;

    return NOERROR;
}

ECode CWebView::NotifySelectDialogDismissed()
{
    mSelectingText = FALSE;
    WebViewCore::ResumeUpdatePicture(mWebViewCore);

    return NOERROR;
}

ECode CWebView::SelectionDone()
{
    if (mSelectingText) {
        AutoPtr<IWebChromeClient> client;
        GetWebChromeClient((IWebChromeClient**)&client);
        if (client != NULL) {
            client->OnSelectionDone(this);
        }

        InvalidateEx2(); // redraw without selection
        NotifySelectDialogDismissed();
    }

    return NOERROR;
}

ECode CWebView::CopySelection(
    /* [out] */ Boolean* bFlag)
{
    VALIDATE_NOT_NULL(bFlag);

    Boolean copiedSomething = false;
    String selection;
    GetSelection(&selection);
    if (selection != "") {
        if (DebugFlags::sWEB_VIEW) {
            Logger::V(LOGTAG, "copySelection \"" + selection + "\"");
        }

        AutoPtr<IToastHelper> toast;
        CToastHelper::AcquireSingleton((IToastHelper**)&toast);
        assert(toast);
//        toast->MakeText(mContext, com.android.internal.R.string.text_copied, Toast.LENGTH_SHORT).show();
        copiedSomething = TRUE;

//        IClipboard clip = IClipboard.Stub.asInterface(ServiceManager.getService("clipboard"));
//        clip.setClipboardText(selection);
    }

    InvalidateEx2(); // remove selection region and pointer
    *bFlag = copiedSomething;

    return NOERROR;
}

ECode CWebView::GetSelection(
    /* [out] */ String* selection)
{
    VALIDATE_NOT_NULL(selection);

    if (mNativeClass == 0) {
        *selection = "";
        return E_FAIL;
    }

    *selection = NativeGetSelection();

    return NOERROR;
}

ECode CWebView::OnChildViewAdded(
    /* [in] */ IView* parent,
    /* [in] */ IView* child)
{
    return NOERROR;
}

ECode CWebView::OnChildViewRemoved(
    /* [in] */ IView* p,
    /* [in] */ IView* child)
{
    return NOERROR;
}

ECode CWebView::OnGlobalFocusChanged(
    /* [in] */ IView* oldFocus,
    /* [in] */ IView* newFocus)
{
    return NOERROR;
}

ECode CWebView::GetDragTracker(
    /* [out] */ IDragTracker** dragTracker)
{
    VALIDATE_NOT_NULL(dragTracker);
    *dragTracker = mDragTracker;

    return NOERROR;
}

ECode CWebView::SetDragTracker(
    /* [in] */ IDragTracker* tracker)
{
    mDragTracker = tracker;

    return NOERROR;
}

ECode CWebView::SetMapTrackballToArrowKeys(
    /* [in] */ Boolean setMap)
{
    mMapTrackballToArrowKeys = setMap;

    return NOERROR;
}

ECode CWebView::FlingScroll(
    /* [in] */ Int32 vx,
    /* [in] */ Int32 vy)
{
    mScroller->Fling(mScrollX, mScrollY, vx, vy, 0, ComputeMaxScrollX(), 0,
                ComputeMaxScrollY(), mOverflingDistance, mOverflingDistance);
        
    InvalidateEx2();

    return NOERROR;
}

ECode CWebView::GetZoomControls(
    /* [out] */ IView** view)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::GetZoomButtonsController(
    /* [out] */ Handle32* zoom)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::ZoomIn(
    /* [out] */ Boolean* bFlag)
{
    VALIDATE_NOT_NULL(bFlag);

    // TODO: alternatively we can disallow this during draw history mode
    SwitchOutDrawHistory();
    mInZoomOverview = FALSE;
    // Center zooming to the center of the screen.
    mZoomCenterX = GetViewWidth() * .5f;
    mZoomCenterY = GetViewHeight() * .5f;
    mAnchorX = ViewToContentX((Int32) mZoomCenterX + mScrollX);
    mAnchorY = ViewToContentY((Int32) mZoomCenterY + mScrollY);
    *bFlag = ZoomWithPreview(mActualScale * 1.25f, TRUE);

    return NOERROR;
}

ECode CWebView::ZoomOut(
    /* [out] */ Boolean* bFlag)
{
    VALIDATE_NOT_NULL(bFlag);

    // TODO: alternatively we can disallow this during draw history mode
    SwitchOutDrawHistory();
    // Center zooming to the center of the screen.
    mZoomCenterX = GetViewWidth() * .5f;
    mZoomCenterY = GetViewHeight() * .5f;
    mAnchorX = ViewToContentX((Int32) mZoomCenterX + mScrollX);
    mAnchorY = ViewToContentY((Int32) mZoomCenterY + mScrollY);
    *bFlag = ZoomWithPreview(mActualScale * 0.8f, true);

    return NOERROR;
}

ECode CWebView::DebugDump()
{
    NativeDebugDump();
    mWebViewCore->SendMessage(WebViewCore::EventHub::DUMP_NAVTREE);

    return NOERROR;
}

ECode CWebView::DrawPage(
    /* [in] */ ICanvas* canvas)
{
    VALIDATE_NOT_NULL(canvas);
    mWebViewCore->DrawContentPicture(canvas, 0, FALSE, FALSE);

    return NOERROR;
}

ECode CWebView::SetTouchInterval(
    /* [in] */ Int32 interval)
{
    mCurrentTouchInterval = interval;

    return NOERROR;
}

ECode CWebView::constructor(
    /* [in] */ IContext* context)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

CARAPI_(void) CWebView::UpdateMultiTouchSupport(
    /* [in] */ IContext* context)
{
#if 0
    WebSettings settings = getSettings();
    const PackageManager pm = context.GetPackageManager();
    mSupportMultiTouch = pm.hasSystemFeature(PackageManager.FEATURE_TOUCHSCREEN_MULTITOUCH)
            && settings.supportZoom() && settings.getBuiltInZoomControls();
    mAllowPanAndScale = pm.hasSystemFeature(
            PackageManager.FEATURE_TOUCHSCREEN_MULTITOUCH_DISTINCT);
    if (mSupportMultiTouch && (mScaleDetector == null)) {
        mScaleDetector = new ScaleGestureDetector(context,
                new ScaleDetectorListener());
    } else if (!mSupportMultiTouch && (mScaleDetector != null)) {
        mScaleDetector = null;
    }
#endif
}

/* package */
CARAPI_(void) CWebView::UpdateDefaultZoomDensity(
    /* [in] */ Int32 zoomDensity)
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    assert(context.Get());

    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    assert(resources.Get());

    AutoPtr<IDisplayMetrics> displayMetrics;
    resources->GetDisplayMetrics((IDisplayMetrics**)&displayMetrics);
    assert(displayMetrics.Get());

    const Float density = ((CDisplayMetrics*)displayMetrics.Get())->mDensity * 100 / zoomDensity;

//    if (Math.abs(density - mDefaultScale) > 0.01) {
        Float scaleFactor = density / mDefaultScale;
        // adjust the limits
        mNavSlop = (Int32) (16 * density);
        DEFAULT_SCALE_PERCENT = (Int32) (100 * density);
        DEFAULT_MAX_ZOOM_SCALE = 4.0f * density;
        DEFAULT_MIN_ZOOM_SCALE = 0.25f * density;
        mDefaultScale = density;
        mMaxZoomScale *= scaleFactor;
        mMinZoomScale *= scaleFactor;
        SetNewZoomScale(mActualScale * scaleFactor, TRUE, FALSE);
//    }
}

/* package */
CARAPI_(Boolean) CWebView::OnSavePassword(
    /* [in] */ const String& schemePlusHost,
    /* [in] */ const String& username,
    /* [in] */ const String& password,
    /* [in] */ IApartment* resumeMsgHandle,
    /* [in] */ Int32 resumeMsgId,
    /* [in] */ IParcel* resumeMsgParams)
{
    Boolean rVal = FALSE;
   if (resumeMsgHandle == NULL) {
       // null resumeMsg implies saving password silently
       ((CWebViewDatabase*)(mDatabase.Get()))->SetUsernamePassword(schemePlusHost, username, password);
   } else {

        //final Message remember = mPrivateHandler.obtainMessage(
        //        REMEMBER_PASSWORD);
        //remember.getData().putString("host", schemePlusHost);
        //remember.getData().putString("username", username);
        //remember.getData().putString("password", password);
        //remember.obj = resumeMsg;

        ECode (STDCALL CWebView::PrivateHandler::*pHandlerFunc)(
                     IApartment*, Int32, IParcel*, String&, String&, String&);

        pHandlerFunc = &CWebView::PrivateHandler::HandleRememberPassword;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInterfacePtr(resumeMsgHandle);
        params->WriteInt32(resumeMsgId);
        params->WriteInterfacePtr(resumeMsgParams);
        params->WriteString(schemePlusHost);
        params->WriteString(username);
        params->WriteString(password);


        //final Message neverRemember = mPrivateHandler.obtainMessage(
        //        NEVER_REMEMBER_PASSWORD);
        //neverRemember.getData().putString("host", schemePlusHost);
        //neverRemember.getData().putString("username", username);
        //neverRemember.getData().putString("password", password);
        //neverRemember.obj = resumeMsg;

        ECode (STDCALL CWebView::PrivateHandler::*pHandlerFuncNever)(
                     IApartment*, Int32, IParcel*, String&, String&, String&);

        pHandlerFuncNever = &CWebView::PrivateHandler::HandleNeverRememberPassword;

#if 0
        new AlertDialog.Builder(getContext())
                .setTitle(com.android.internal.R.string.save_password_label)
                .setMessage(com.android.internal.R.string.save_password_message)
                .setPositiveButton(com.android.internal.R.string.save_password_notnow,
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        resumeMsg.sendToTarget();
                    }
                })
                .setNeutralButton(com.android.internal.R.string.save_password_remember,
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        remember.sendToTarget();
                    }
                })
                .setNegativeButton(com.android.internal.R.string.save_password_never,
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        neverRemember.sendToTarget();
                    }
                })
                .setOnCancelListener(new OnCancelListener() {
                    public void onCancel(DialogInterface dialog) {
                        resumeMsg.sendToTarget();
                    }
                }).show();
        // Return true so that WebViewCore will pause while the dialog is
        // up.
        rVal = true;
#endif
    }

   return rVal;
}

/*
 * Return the width of the view where the content of WebView should render
 * to.
 * Note: this can be called from WebCoreThread.
 */
/* package */
CARAPI_(Int32) CWebView::GetViewWidth()
{
    Boolean bFlag = FALSE;
    Int32 width = 0;
    IsVerticalScrollBarEnabled(&bFlag);
    if (!bFlag || mOverlayVerticalScrollbar) {        
        GetWidth(&width);
        return width;
    } else {
        Int32 barWidth = 0;
        GetWidth(&width);
        GetVerticalScrollbarWidth(&barWidth);
        return width - barWidth;
    }
}

/**
 * Given an x coordinate in view space, convert it to content space.  Also
 * may be used for absolute heights (such as for the WebTextView's
 * textSize, which is unaffected by the height of the title bar).
 */
/*package*/
CARAPI_(Int32) CWebView::ViewToContentX(
    /* [in] */ Int32 x)
{
    return ViewToContentDimension(x);
}

/**
 * Given a y coordinate in view space, convert it to content space.
 * Takes into account the height of the title bar if there is one
 * embedded into the WebView.
 */
/*package*/
CARAPI_(Int32) CWebView::ViewToContentY(
    /* [in] */ Int32 y)
{
    return ViewToContentDimension(y - GetTitleHeight());
}

/**
 * Given a distance in content space, convert it to view space. Note: this
 * does not reflect translation, just scaling, so this should not be called
 * with coordinates, but should be called for dimensions like width or
 * height.
 */
/*package*/
CARAPI_(Int32) CWebView::ContentToViewDimension(
    /* [in] */ Int32 d)
{}

/**
 * Given an x coordinate in content space, convert it to view
 * space.
 */
/*package*/
CARAPI_(Int32) CWebView::ContentToViewX(
    /* [in] */ Int32 x)
{
    return ContentToViewDimension(x);
}

/**
 * Given a y coordinate in content space, convert it to view
 * space.  Takes into account the height of the title bar.
 */
/*package*/
CARAPI_(Int32) CWebView::ContentToViewY(
    /* [in] */ Int32 y)
{
    return ContentToViewDimension(y) + GetTitleHeight();
}

/*
 * Return the height of the view where the content of WebView should render
 * to.  Note that this excludes mTitleBar, if there is one.
 * Note: this can be called from WebCoreThread.
 */
/* package */
CARAPI_(Int32) CWebView::GetViewHeight()
{
    return GetViewHeightWithTitle() - GetVisibleTitleHeight();
}

/**
 * Called by CallbackProxy when the page finishes loading.
 * @param url The URL of the page which has finished loading.
 */
/* package */
CARAPI_(void) CWebView::OnPageFinished(
    /* [in] */ const String& url)
{
    if (mPageThatNeedsToSlideTitleBarOffScreen.GetLength() != 0) {
        // If the user is now on a different page, or has scrolled the page
        // past the point where the title bar is offscreen, ignore the
        // scroll request.
        if (mPageThatNeedsToSlideTitleBarOffScreen.Equals(url)
                && mScrollX == 0 && mScrollY == 0) {
            PinScrollTo(0, mYDistanceToSlideTitleOffScreen, TRUE,
                    SLIDE_TITLE_DURATION);
        }
        mPageThatNeedsToSlideTitleBarOffScreen = "";
    }
}

CARAPI_(Boolean) CWebView::InAnimateZoom()
{
    return mZoomScale != 0;
}

// Only check the flag, can be called from WebCore thread
CARAPI_(Boolean) CWebView::DrawHistory()
{
    return mDrawHistory;
}

// Should only be called in UI thread
CARAPI_(void) CWebView::SwitchOutDrawHistory()
{
    if (NULL == mWebViewCore) {
        return; // CallbackProxy may trigger this
    }

    if (mDrawHistory && mWebViewCore->PictureReady()) {
        mDrawHistory = FALSE;
        mHistoryPicture = NULL;
        InvalidateEx2();
        Int32 oldScrollX = mScrollX;
        Int32 oldScrollY = mScrollY;
        mScrollX = PinLocX(mScrollX);
        mScrollY = PinLocY(mScrollY);
        if (oldScrollX != mScrollX || oldScrollY != mScrollY) {
            mUserScroll = FALSE;
            mWebViewCore->SendMessage(WebViewCore::EventHub::SYNC_SCROLL, oldScrollX,
                    oldScrollY);
            OnScrollChanged(mScrollX, mScrollY, oldScrollX, oldScrollY);
        } else {
            SendOurVisibleRect();
        }
    }
}

CARAPI_(WebViewCore::CursorData*) CWebView::CursorData()
{
    WebViewCore::CursorData* result = new WebViewCore::CursorData();
    result->mMoveGeneration = NativeMoveGeneration();
    result->mFrame = NativeCursorFramePointer();
    AutoPtr<IPoint> position = NativeCursorPosition();
    result->mX = ((CPoint*)position.Get())->mX;
    result->mY = ((CPoint*)position.Get())->mY;
    return result;
}

/**
 *  Delete text from start to end in the focused textfield. If there is no
 *  focus, or if start == end, silently fail.  If start and end are out of
 *  order, swap them.
 *  @param  start   Beginning of selection to delete.
 *  @param  end     End of selection to delete.
 */
/* package */
CARAPI_(void) CWebView::DeleteSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    mTextGeneration++;
    WebViewCore::TextSelectionData* data = new WebViewCore::TextSelectionData(start, end);
    mWebViewCore->SendMessage(WebViewCore::EventHub::DELETE_SELECTION, 
        mTextGeneration, 0, (IInterface*)data);
}

/**
 *  Set the selection to (start, end) in the focused textfield. If start and
 *  end are out of order, swap them.
 *  @param  start   Beginning of selection.
 *  @param  end     End of selection.
 */
/* package */
CARAPI_(void) CWebView::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    mWebViewCore->SendMessage(WebViewCore::EventHub::SET_SELECTION, start, end);
}

/*
 * This method checks the current focus and cursor and potentially rebuilds
 * mWebTextView to have the appropriate properties, such as password,
 * multiline, and what text it contains.  It also removes it if necessary.
 */
/* package */
CARAPI_(void) CWebView::RebuildWebTextView()
{
    // If the WebView does not have focus, do nothing until it gains focus.
    Boolean bFlag = FALSE;
    HasFocus(&bFlag);
    if (!bFlag && (NULL == mWebTextView /*|| !mWebTextView->HasFocus()*/)) {
        return;
    }

    Boolean alreadyThere = InEditingMode();
    // inEditingMode can only return true if mWebTextView is non-null,
    // so we can safely call remove() if (alreadyThere)
    if (0 == mNativeClass || !NativeFocusCandidateIsTextInput()) {
        if (alreadyThere) {
            mWebTextView->Remove();
        }
        return;
    }

    // At this point, we know we have found an input field, so go ahead
    // and create the WebTextView if necessary.
    if (mWebTextView == NULL) {
//        mWebTextView = new WebTextView(mContext, /*WebView*/this);
        // Initialize our generation number.
        mTextGeneration = 0;
    }
//    mWebTextView->SetTextSize(TypedValue_COMPLEX_UNIT_PX, ContentToViewDimension(NativeFocusCandidateTextSize()));
    AutoPtr<IRect> visibleRect;
    CRect::New((IRect**)&visibleRect);
    CalcOurContentVisibleRect(visibleRect);
    // Note that sendOurVisibleRect calls viewToContent, so the coordinates
    // should be in content coordinates.
    AutoPtr<IRect> bounds = NativeFocusCandidateNodeBounds();
    AutoPtr<IRect> vBox = ContentToViewRect(bounds);
    Int32 left, top, width, height;
    vBox->GetLeft(&left);
    vBox->GetTop(&top);
    vBox->GetWidth(&width);
    vBox->GetHeight(&height);
    mWebTextView->SetRect(left, top, width, height);

    Boolean b = FALSE;
    bounds->IntersectsEx(bounds, visibleRect, &b);
    if (!b) {
        mWebTextView->BringIntoView();
    }

    String text = NativeFocusCandidateText();
    Int32 nodePointer = NativeFocusCandidatePointer();
    if (alreadyThere && mWebTextView->IsSameTextField(nodePointer)) {
        // It is possible that we have the same textfield, but it has moved,
        // i.e. In the case of opening/closing the screen.
        // In that case, we need to set the dimensions, but not the other
        // aspects.
        // If the text has been changed by webkit, update it.  However, if
        // there has been more UI text input, ignore it.  We will receive
        // another update when that text is recognized.
        String toStr;
//        mWebTextView->GetText().ToString(&toStr);
        if (text.GetLength() != 0 && !text.Equals(toStr)
                && NativeTextGeneration() == mTextGeneration) {
            mWebTextView->SetTextAndKeepSelection(text);
        }
    } else {
//        mWebTextView->SetGravity(NativeFocusCandidateIsRtlText() ? Gravity_RIGHT : Gravity_NO_GRAVITY);
        // This needs to be called before setType, which may call
        // requestFormData, and it needs to have the correct nodePointer.
//        mWebTextView->SetNodePointer(nodePointer);
        mWebTextView->SetType(NativeFocusCandidateType());
        AutoPtr<IRect> paddingRect = NativeFocusCandidatePaddingRect();
        if (paddingRect != NULL) {
            // Use contentToViewDimension since these are the dimensions of
            // the padding.
            Int32 left, top, right, bottom;
            paddingRect->GetLeft(&left);
            paddingRect->GetTop(&top);
            paddingRect->GetRight(&right);
            paddingRect->GetBottom(&bottom);
//            mWebTextView->SetPadding(
//                    ContentToViewDimension(left),
//                    ContentToViewDimension(top),
//                    ContentToViewDimension(right),
//                    ContentToViewDimension(bottom));
        }
        if (text.GetLength() == 0) {
            if (DebugFlags::sWEB_VIEW) {
                Logger::V(LOGTAG, "rebuildWebTextView null == text");
            }
            text = "";
        }
#if 0
        mWebTextView->SetTextAndKeepSelection(text);
        InputMethodManager imm = InputMethodManager.peekInstance();
        if (imm != null && imm.IsActive(mWebTextView)) {
            imm.RestartInput(mWebTextView);
        }
#endif
    }

//    mWebTextView->RequestFocus();
}

/**
 * Called by WebTextView to find saved form data associated with the
 * textfield
 * @param name Name of the textfield.
 * @param nodePointer Pointer to the node of the textfield, so it can be
 *          compared to the currently focused textfield when the data is
 *          retrieved.
 */
/* package */
CARAPI_(void) CWebView::RequestFormData(
    /* [in] */ const String& name,
    /* [in] */ Int32 nodePointer)
{
#if 0
    if (mWebViewCore.getSettings().getSaveFormData()) {
        Message update = mPrivateHandler.obtainMessage(REQUEST_FORM_DATA);
        update.arg1 = nodePointer;
        RequestFormData updater = new RequestFormData(name, getUrl(),
                update);
        Thread t = new Thread(updater);
        t.start();
    }
#endif
}

/**
 * Pass a message to find out the <label> associated with the <input>
 * identified by nodePointer
 * @param framePointer Pointer to the frame containing the <input> node
 * @param nodePointer Pointer to the node for which a <label> is desired.
 */
/* package */
CARAPI_(void) CWebView::RequestLabel(
    /* [in] */ Int32 framePointer,
    /* [in] */ Int32 nodePointer)
{
    mWebViewCore->SendMessage(WebViewCore::EventHub::REQUEST_LABEL, framePointer,
                nodePointer);
}

/*
 * Pass a message to WebCore Thread, telling the WebCore::Page's
 * FocusController to be  "inactive" so that it will
 * not draw the blinking cursor.  It gets set to "active" to draw the cursor
 * in WebViewCore.cpp, when the WebCore thread receives key events/clicks.
 */
/* package */
CARAPI_(void) CWebView::SetFocusControllerInactive()
{
    // Do not need to also check whether mWebViewCore is null, because
    // mNativeClass is only set if mWebViewCore is non null
    if (mNativeClass == 0) {
        return;
    }

    mWebViewCore->SendMessage(WebViewCore::EventHub::SET_ACTIVE, 0, 0);
}

CARAPI_(void) CWebView::ResetTrackballTime()
{
    mTrackballLastTime = 0;
}

CARAPI_(void) CWebView::MoveSelection(
    /* [in] */ Float xRate,
    /* [in] */ Float yRate)
{
    if (mNativeClass == 0) {
            return;
    }

    Int32 width = GetViewWidth();
    Int32 height = GetViewHeight();
    mSelectX += xRate;
    mSelectY += yRate;
    Int32 maxX = width + mScrollX;
    Int32 maxY = height + mScrollY;
    //mSelectX = Math.min(maxX, Math.max(mScrollX - SELECT_CURSOR_OFFSET, mSelectX));
    //mSelectY = Math.min(maxY, Math.max(mScrollY - SELECT_CURSOR_OFFSET, mSelectY));

    mSelectX = mScrollX - SELECT_CURSOR_OFFSET > mSelectX ? mScrollX - SELECT_CURSOR_OFFSET : mSelectX;
    mSelectX = maxX < mSelectX ? maxX : mSelectX;

    mSelectY = mScrollY - SELECT_CURSOR_OFFSET > mSelectY ? mScrollY - SELECT_CURSOR_OFFSET : mSelectY;
    mSelectY = maxY < mSelectY ? maxY : mSelectY;

    if (DebugFlags::sWEB_VIEW) {
//        Logger::V(LOGTAG, "moveSelection mSelectX=%d mSelectY=%d mScrollX=%d mScrollY=%d xRate=%d yRate=%d"
//                + mSelectX + mSelectY + mScrollX + mScrollY + xRate + yRate);
    }

    NativeMoveSelection(ViewToContentX(mSelectX), ViewToContentY(mSelectY));
    Int32 scrollX = mSelectX < mScrollX ? -SELECT_CURSOR_OFFSET
            : mSelectX > maxX - SELECT_CURSOR_OFFSET ? SELECT_CURSOR_OFFSET
            : 0;
    Int32 scrollY = mSelectY < mScrollY ? -SELECT_CURSOR_OFFSET
            : mSelectY > maxY - SELECT_CURSOR_OFFSET ? SELECT_CURSOR_OFFSET
            : 0;
    PinScrollBy(scrollX, scrollY, true, 0);
    AutoPtr<IRect> select;
    CRect::New(mSelectX, mSelectY, mSelectX + 1, mSelectY + 1, (IRect**)&select);
    assert(select.Get());
    Boolean bFlag = FALSE;
    RequestRectangleOnScreen(select, &bFlag);
    InvalidateEx2();
}

/**
 * Scroll the focused text field/area to match the WebTextView
 * @param xPercent New x position of the WebTextView from 0 to 1.
 * @param y New y position of the WebTextView in view coordinates
 */
/*package*/
CARAPI_(void) CWebView::ScrollFocusedTextInput(
    /* [in] */ Float xPercent,
    /* [in] */ Int32 y)
{
    if (!InEditingMode() || mWebViewCore == NULL) {
        return;
    }
    mWebViewCore->SendMessage(WebViewCore::EventHub::SCROLL_TEXT_INPUT,
            // Since this position is relative to the top of the text input
            // field, we do not need to take the title bar's height into
            // consideration.
            ViewToContentDimension(y),
            /*new Float(*/xPercent/*)*/);
}

/**
 * Set our starting point and time for a drag from the WebTextView.
 */
/*package*/
CARAPI_(void) CWebView::InitiateTextFieldDrag(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int64 eventTime)
{
    if (!InEditingMode()) {
        return;
    }

//    mLastTouchX = x + (Float) (mWebTextView->GetLeft() - mScrollX);
//    mLastTouchY = y + (Float) (mWebTextView->GetTop() - mScrollY);
    mLastTouchTime = eventTime;
    if (!mScroller->IsFinished()) {
        AbortAnimation();
//        mPrivateHandler->RemoveMessage(*(Handle32*)&CWebView::PrivateHandler::HandleResumeWebcorePriority);
    }
    mSnapScrollMode = SNAP_NONE;
    mVelocityTracker = VelocityTracker::Obtain();
    mTouchMode = TOUCH_DRAG_START_MODE;
}

/**
 * Given a motion event from the WebTextView, set its location to our
 * coordinates, and handle the event.
 */
/*package*/
CARAPI_(Boolean) CWebView::TextFieldDrag(
    /* [in] */ IMotionEvent* event)
{
    assert(event != NULL);

    if (!InEditingMode()) {
        return FALSE;
    }

    mDragFromTextInput = TRUE;
//    event->OffsetLocation((Float) (mWebTextView->GetLeft() - mScrollX),
//            (Float) (mWebTextView->GetTop() - mScrollY));
    Boolean result;
    OnTouchEvent(event, &result);
    mDragFromTextInput = FALSE;

    return result;
}

/**
 * Due a touch up from a WebTextView.  This will be handled by webkit to
 * change the selection.
 * @param event MotionEvent in the WebTextView's coordinates.
 */
/*package*/
CARAPI_(void) CWebView::TouchUpOnTextField(
    /* [in] */ IMotionEvent* event)
{
    assert(event != NULL);

    if (!InEditingMode()) {
        return;
    }
    Int32 x;// = ViewToContentX((Int32) event.getX() + mWebTextView->GetLeft());
    Int32 y;// = ViewToContentY((Int32) event.getY() + mWebTextView->GetTop());
    NativeMotionUp(x, y, mNavSlop);
}

/**
 * Called when pressing the center key or trackball on a textfield.
 */
/*package*/
CARAPI_(void) CWebView::CenterKeyPressOnTextField()
{
    mWebViewCore->SendMessage(WebViewCore::EventHub::CLICK, NativeCursorFramePointer(),
                    NativeCursorNodePointer());
}

 /*
 * Request a dropdown menu for a listbox with single selection or a single
 * <select> element.
 *
 * @param array Labels for the listbox.
 * @param enabledArray  State for each element in the list.  See static
 *      integers in Container class.
 * @param selection Which position is initally selected.
 */
CARAPI_(void) CWebView::RequestListBox(
    /* [in] */ ArrayOf<String>* array,
    /* [in] */ ArrayOf<Int32>* enabledArray,
    /* [in] */ Int32 selection)
{}

CARAPI_(void) CWebView::DismissZoomControl()
{}

/* package */
CARAPI_(void) CWebView::ReplaceTextfieldText(
    /* [in] */ Int32 oldStart,
    /* [in] */ Int32 oldEnd,
    /* [in] */ const String& replace,
    /* [in] */ Int32 newStart,
    /* [in] */ Int32 newEnd)
{}

/* package */
CARAPI_(void) CWebView::PassToJavaScript(
    /* [in] */ const String& currentText,
    /* [in] */ IKeyEvent* event)
{}

/* package */
/*synchronized*/
CARAPI_(AutoPtr<WebViewCore>) CWebView::GetWebViewCore()
{
    Mutex::Autolock lock(_m_syncLock);
    return mWebViewCore;
}

/*
 * Request a dropdown menu for a listbox with multiple selection.
 *
 * @param array Labels for the listbox.
 * @param enabledArray  State for each element in the list.  See static
 *      integers in Container class.
 * @param selectedArray Which positions are initally selected.
 */
CARAPI_(void) CWebView::RequestListBox(
    /* [in] */ ArrayOf<String>* array,
    /* [in] */ ArrayOf<Int32>*  enabledArray,
    /* [in] */ ArrayOf<Int32>*  selectedArray)
{}

/**
 *  Update our cache with updatedText.
 *  @param updatedText  The new text to put in our cache.
 */
/* package */
CARAPI_(void) CWebView::UpdateCachedTextfield(
    /* [in] */ const String& updatedText)
{
    // Also place our generation number so that when we look at the cache
    // we recognize that it is up to date.
    NativeUpdateCachedTextfield(updatedText, mTextGeneration);
}

/* package */
CARAPI_(AutoPtr<ViewManager>) CWebView::GetViewManager()
{
    return mViewManager;
}

/* package */
CARAPI_(void)     CWebView::NativeClearCursor()
{}

/* package */
CARAPI_(Boolean)  CWebView::NativeCursorMatchesFocus()
{}

/* package */
CARAPI_(Int32)    CWebView::NativeFocusCandidateFramePointer()
{}

/* package */
CARAPI_(Boolean)  CWebView::NativeFocusCandidateHasNextTextfield()
{}

/* package */
CARAPI_(Boolean)  CWebView::NativeFocusCandidateIsPassword()
{}

/* package */
CARAPI_(Int32)    CWebView::NativeFocusCandidateMaxLength()
{}

/* package */
CARAPI_(String)   CWebView::NativeFocusCandidateName()
{}

/* package */
CARAPI_(Int32)    CWebView::NativeFocusCandidatePointer()
{}

/* package */
CARAPI_(Int32)    CWebView::NativeFocusNodePointer()
{}

/* package */
CARAPI_(Boolean)  CWebView::NativeMoveCursorToNextTextInput()
{}


/**
 * Construct a new WebView with layout parameters, a default style and a set
 * of custom Javscript interfaces to be added to the WebView at initialization
 * time. This guarantees that these interfaces will be available when the JS
 * context is initialized.
 * @param context A Context object used to access application assets.
 * @param attrs An AttributeSet passed to our parent.
 * @param defStyle The default style resource ID.
 * @param javascriptInterfaces is a Map of intareface names, as keys, and
 * object implementing those interfaces, as values.
 * @hide pending API council approval.
 */
CWebView::CWebView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ IObjectStringMap* javascriptInterfaces) : AbsoluteLayout(context, attrs, defStyle)
{
    Init();

    //mCallbackProxy = new CallbackProxy(context, this);
    CCallbackProxy::New(context, this, (ICallbackProxy**)&mCallbackProxy);
    mViewManager = new ViewManager((IWebView*)this);
    mWebViewCore = new WebViewCore(context, this, mCallbackProxy, javascriptInterfaces);
//    mDatabase = WebViewDatabase.GetInstance(context);
    mScroller = new OverScroller(context);

    UpdateMultiTouchSupport(context);
}

//@Override
CARAPI_(Int32) CWebView::ComputeHorizontalScrollRange()
{
    Int32 range = ComputeRealHorizontalScrollRange();

    // Adjust reported range if overscrolled to compress the scroll bars
    const Int32 scrollX = mScrollX;
    const Int32 overscrollRight = ComputeMaxScrollX();
    if (scrollX < 0) {
        range -= scrollX;
    } else if (scrollX > overscrollRight) {
        range += scrollX - overscrollRight;
    }

    return range;
}

//@Override
CARAPI_(Int32) CWebView::ComputeHorizontalScrollOffset()
{
    //return Math.max(mScrollX, 0);
    return mScrollX > 0 ? mScrollX : 0;
}

//@Override
CARAPI_(Int32) CWebView::ComputeVerticalScrollRange()
{
    Int32 range = ComputeRealVerticalScrollRange();

    // Adjust reported range if overscrolled to compress the scroll bars
    const Int32 scrollY = mScrollY;
    const Int32 overscrollBottom = ComputeMaxScrollY();
    if (scrollY < 0) {
        range -= scrollY;
    } else if (scrollY > overscrollBottom) {
        range += scrollY - overscrollBottom;
    }

    return range;
}

//@Override
CARAPI_(Int32) CWebView::ComputeVerticalScrollOffset()
{
    //return Math.max(mScrollY - getTitleHeight(), 0);
    return mScrollY - GetTitleHeight() > 0 ? mScrollY - GetTitleHeight() : 0;
}

//@Override
CARAPI_(Int32) CWebView::ComputeVerticalScrollExtent()
{
    return GetViewHeight();
}

/** @hide */
//@Override
CARAPI_(void) CWebView::OnDrawVerticalScrollBar(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IDrawable* scrollBar,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    if (mScrollY < 0) {
        t -= mScrollY;
    }

    if (scrollBar)
    {
        scrollBar->SetBounds(l, t + GetVisibleTitleHeight(), r, b);
        scrollBar->Draw(canvas);
    }    
}

//@Override
CARAPI_(void) CWebView::OnOverScrolled(
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY,
    /* [in] */ Boolean clampedX,
    /* [in] */ Boolean clampedY)
{
    mInOverScrollMode = FALSE;
    Int32 maxX = ComputeMaxScrollX();
    Int32 maxY = ComputeMaxScrollY();
    if (maxX == 0) {
        // do not over scroll x if the page just fits the screen
        scrollX = PinLocX(scrollX);
    } else if (scrollX < 0 || scrollX > maxX) {
        mInOverScrollMode = TRUE;
    }

    if (scrollY < 0 || scrollY > maxY) {
        mInOverScrollMode = TRUE;
    }

    Int32 oldX = mScrollX;
    Int32 oldY = mScrollY;

    AbsoluteLayout::ScrollTo(scrollX, scrollY);

    // Only show overscroll bars if there was no movement in any direction
    // as a result of scrolling.
    if (mEdgeGlowTop != NULL && oldY == mScrollY && oldX == mScrollX) {
        // Don't show left/right glows if we fit the whole content.
        // Also don't show if there was vertical movement.
        if (maxX > 0) {
            const Int32 pulledToX = oldX + mOverscrollDeltaX;
            Int32 width = 0;
            GetWidth(&width);
            assert(width != 0);
            if (pulledToX < 0) {
                mEdgeGlowLeft->OnPull((Float) mOverscrollDeltaX / width);
                if (!mEdgeGlowRight->IsFinished()) {
                    mEdgeGlowRight->OnRelease();
                }
            } else if (pulledToX > maxX) {
                mEdgeGlowRight->OnPull((Float) mOverscrollDeltaX / width);
                if (!mEdgeGlowLeft->IsFinished()) {
                    mEdgeGlowLeft->OnRelease();
                }
            }
            mOverscrollDeltaX = 0;
        }

        Int32 mode;
        GetOverScrollMode(&mode);
        if (maxY > 0 || mode == OVER_SCROLL_ALWAYS) {
            const Int32 pulledToY = oldY + mOverscrollDeltaY;
            Int32 height = 0;
            GetHeight(&height);
            assert(height != 0);
            if (pulledToY < 0) {
                mEdgeGlowTop->OnPull((Float) mOverscrollDeltaY / height);
                if (!mEdgeGlowBottom->IsFinished()) {
                    mEdgeGlowBottom->OnRelease();
                }
            } else if (pulledToY > maxY) {
                mEdgeGlowBottom->OnPull((Float) mOverscrollDeltaY / height);
                if (!mEdgeGlowTop->IsFinished()) {
                    mEdgeGlowTop->OnRelease();
                }
            }
            mOverscrollDeltaY = 0;
        }
    }
}


//-------------------------------------------------------------------------
// Override View methods
//-------------------------------------------------------------------------

//@Override
CARAPI_(void) CWebView::Finalize()
{
    //try {
        Destroy();
    //} finally {
    //    super.finalize();
    //}
}

//@Override
CARAPI_(Boolean) CWebView::DrawChild(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IView* child,
    /* [in] */ Int64 drawingTime)
{
    if (child == mTitleBar) {
        // When drawing the title bar, move it horizontally to always show
        // at the top of the WebView.
        Int32 left;
        mTitleBar->GetLeft(&left);
        mTitleBar->OffsetLeftAndRight(mScrollX - left);
    }

    return AbsoluteLayout::DrawChild(canvas, child, drawingTime);
}

//@Override
CARAPI_(void) CWebView::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    assert(canvas != NULL);

    // if mNativeClass is 0, the WebView has been destroyed. Do nothing.
    if (mNativeClass == 0) {
        return;
    }

    // if both mContentWidth and mContentHeight are 0, it means there is no
    // valid Picture passed to WebView yet. This can happen when WebView
    // just starts. Draw the background and return.
    if ((mContentWidth | mContentHeight) == 0 && mHistoryPicture == NULL) {
        canvas->DrawColor(mBackgroundColor);
        return;
    }

    Int32 saveCount;
    canvas->Save(&saveCount);
    AutoPtr<IWebSettings> ws;
    GetSettings((IWebSettings**)&ws);
    assert(ws.Get());
    Boolean bFlag = FALSE;
    ws->GetUseWebViewBackgroundForOverscrollBackground(&bFlag);
    if (mInOverScrollMode && !bFlag) {
        if (mOverScrollBackground == NULL) {
            CPaint::New((IPaint**)&mOverScrollBackground);
//            Bitmap bm = BitmapFactory.decodeResource(mContext.getResources(),
//                    com.android.internal.R.drawable.status_bar_background);
//            mOverScrollBackground.setShader(new BitmapShader(bm,
//                    Shader.TileMode.REPEAT, Shader.TileMode.REPEAT));
            CPaint::New((IPaint**)&mOverScrollBorder);
            assert(mOverScrollBorder.Get());
            mOverScrollBorder->SetStyle(PaintStyle_STROKE);
            mOverScrollBorder->SetStrokeWidth(0);
            mOverScrollBorder->SetColor(0xffbbbbbb);
        }

        Int32 top = 0;
        Int32 right = ComputeRealHorizontalScrollRange();
        Int32 bottom = top + ComputeRealVerticalScrollRange();
        // first draw the background and anchor to the top of the view
        canvas->Save(NULL);
        canvas->Translate(mScrollX, mScrollY);
        canvas->ClipRectEx4(-mScrollX, top - mScrollY, right - mScrollX, bottom
                - mScrollY, RegionOp_DIFFERENCE, NULL);
        canvas->DrawPaint(mOverScrollBackground);
        canvas->Restore();
        // then draw the border
        canvas->DrawRectEx2(-1, top - 1, right, bottom, mOverScrollBorder);
        // next clip the region for the content
        canvas->ClipRectEx5(0, top, right, bottom, NULL);
    }

    if (mTitleBar != NULL) {
        Int32 height;
        mTitleBar->GetHeight(&height);
        canvas->Translate(0, height);
    }

    if (mDragTrackerHandler == NULL) {
        DrawContent(canvas);
    } else {
        if (!mDragTrackerHandler->Draw(canvas)) {
            // sometimes the tracker doesn't draw, even though its active
            DrawContent(canvas);
        }

        if (mDragTrackerHandler->IsFinished()) {
            mDragTrackerHandler = NULL;
        }
    }

    canvas->RestoreToCount(saveCount);

    // Now draw the shadow.
    Int32 titleH = GetVisibleTitleHeight();
    if (mTitleBar != NULL && titleH == 0) {
        AutoPtr<IContext> content;
        GetContext((IContext**)&content);
        assert(content.Get());

        AutoPtr<IResources> resources;
        content->GetResources((IResources**)&resources);
        assert(resources.Get());

        AutoPtr<IDisplayMetrics> displayMetrics;
        resources->GetDisplayMetrics((IDisplayMetrics**)&displayMetrics);
        assert(displayMetrics != NULL);

        Int32 height;// = (Int32) (5.0f * ((CDisplayMetrics*)displayMetrics.Get())->density);
        Int32 width = 0;
        GetWidth(&width);
        mTitleShadow->SetBounds(mScrollX, mScrollY, mScrollX + width,
                mScrollY + height);
        mTitleShadow->Draw(canvas);
    }

    if (AUTO_REDRAW_HACK && mAutoRedraw) {
        InvalidateEx2();
    }
    if (InEditingMode()) {
        mWebTextView->OnDrawSubstitute();
    }
    mWebViewCore->SignalRepaintDone();
}

//@Override
CARAPI_(void) CWebView::OnAttachedToWindow()
{
    AbsoluteLayout::OnAttachedToWindow();
    Boolean bFlag = FALSE;
    HasWindowFocus(&bFlag);
    if (bFlag) {
        SetActive(TRUE);
    }
}

//@Override
CARAPI_(void) CWebView::OnDetachedFromWindow()
{
    ClearHelpers();
    DismissZoomControl();
    Boolean bFlag = FALSE;
    HasWindowFocus(&bFlag);
    if (bFlag) {
        SetActive(FALSE);
    }
    AbsoluteLayout::OnDetachedFromWindow();
}

//@Override
CARAPI_(void) CWebView::OnFocusChanged(
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    if (DebugFlags::sWEB_VIEW) {
        Logger::V(LOGTAG, "MT focusChanged %d, %d" + focused + direction);
    }

    if (focused) {
        // When we regain focus, if we have window focus, resume drawing
        // the cursor ring
        Boolean bFlag = FALSE;
        HasWindowFocus(&bFlag);
        if (bFlag) {
            mDrawCursorRing = TRUE;
            if (mNativeClass != 0) {
                NativeRecordButtons(TRUE, FALSE, TRUE);
            }
        //} else {
            // The WebView has gained focus while we do not have
            // windowfocus.  When our window lost focus, we should have
            // called nativeRecordButtons(false...)
        }
    } else {
        // When we lost focus, unless focus went to the TextView (which is
        // true if we are in editing mode), stop drawing the cursor ring.
        if (!InEditingMode()) {
            mDrawCursorRing = FALSE;
            if (mNativeClass != 0) {
                NativeRecordButtons(FALSE, FALSE, TRUE);
            }
            SetFocusControllerInactive();
        }
        mGotKeyDown = FALSE;
    }

    AbsoluteLayout::OnFocusChanged(focused, direction, previouslyFocusedRect);
}

/**
 * @hide
 */
//@Override
CARAPI_(Boolean) CWebView::SetFrame(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    Boolean changed = AbsoluteLayout::SetFrame(left, top, right, bottom);
    if (!changed && mHeightCanMeasure) {
        // When mHeightCanMeasure is true, we will set mLastHeightSent to 0
        // in WebViewCore after we get the first layout. We do call
        // requestLayout() when we get contentSizeChanged(). But the View
        // system won't call onSizeChanged if the dimension is not changed.
        // In this case, we need to call sendViewSizeZoom() explicitly to
        // notify the WebKit about the new dimensions.
        SendViewSizeZoom();
    }
    return changed;
}


//@Override
CARAPI_(void) CWebView::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 ow,
    /* [in] */ Int32 oh)
{
    AbsoluteLayout::OnSizeChanged(w, h, ow, oh);
    // Center zooming to the center of the screen.
    if (mZoomScale == 0) { // unless we're already zooming
        // To anchor at top left corner.
        mZoomCenterX = 0;
        mZoomCenterY = GetVisibleTitleHeight();
        mAnchorX = ViewToContentX((Int32) mZoomCenterX + mScrollX);
        mAnchorY = ViewToContentY((Int32) mZoomCenterY + mScrollY);
    }

    // adjust the max viewport width depending on the view dimensions. This
    // is to ensure the scaling is not going insane. So do not shrink it if
    // the view size is temporarily smaller, e.g. when soft keyboard is up.
    Int32 max = w > h ? w : h;
    Int32 newMaxViewportWidth = (Int32) (/*Math.max(w, h)*/max / DEFAULT_MIN_ZOOM_SCALE);
    if (newMaxViewportWidth > sMaxViewportWidth) {
        sMaxViewportWidth = newMaxViewportWidth;
    }

    // update mMinZoomScale if the minimum zoom scale is not fixed
    if (!mMinZoomScaleFixed) {
        // when change from narrow screen to wide screen, the new viewWidth
        // can be wider than the old content width. We limit the minimum
        // scale to 1.0f. The proper minimum scale will be calculated when
        // the new picture shows up.
      //  mMinZoomScale = Math.min(1.0f, (float) GetViewWidth()
      //          / (mDrawHistory ? mHistoryPicture.GetWidth()
      //                  : mZoomOverviewWidth));

        Int32 width = 0, min;
        mHistoryPicture->GetWidth(&width);
        min = mDrawHistory ? width : mZoomOverviewWidth;
        min = (float) GetViewWidth() / min;
        mMinZoomScale = 1.0f < min ? min : 1.0f;

        if (mInitialScaleInPercent > 0) {
            // limit the minZoomScale to the initialScale if it is set
            Float initialScale = mInitialScaleInPercent / 100.0f;
            if (mMinZoomScale > initialScale) {
                mMinZoomScale = initialScale;
            }
        }
    }

    DismissZoomControl();

    // onSizeChanged() is called during WebView layout. And any
    // requestLayout() is blocked during layout. As setNewZoomScale() will
    // call its child View to reposition itself through ViewManager's
    // scaleAll(), we need to post a Runnable to ensure requestLayout().
    // <b/>
    // only update the text wrap scale if width changed.
//    Post(new PostScale(this, w != ow));
}

//@Override
CARAPI_(void) CWebView::OnScrollChanged(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 oldl,
    /* [in] */ Int32 oldt)
{
    AbsoluteLayout::OnScrollChanged(l, t, oldl, oldt);
    if (!mInOverScrollMode) {
        SendOurVisibleRect();
        // update WebKit if visible title bar height changed. The logic is same
        // as getVisibleTitleHeight.
        Int32 titleHeight = GetTitleHeight();
        Int32 max1 = titleHeight - t > 0 ? titleHeight - t : 0;
        Int32 max2 = titleHeight - oldt > 0 ? titleHeight - oldt : 0;
        if (/*Math.max(titleHeight - t, 0) != Math.max(titleHeight - oldt, 0)*/max1 != max2) {
            SendViewSizeZoom();
        }
    }
}

//@Override
CARAPI_(void) CWebView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    AbsoluteLayout::OnMeasure(widthMeasureSpec, heightMeasureSpec);

    Int32 heightMode = View::MeasureSpec::GetMode(heightMeasureSpec);
    Int32 heightSize = View::MeasureSpec::GetSize(heightMeasureSpec);
    Int32 widthMode = View::MeasureSpec::GetMode(widthMeasureSpec);
    Int32 widthSize = View::MeasureSpec::GetSize(widthMeasureSpec);

    Int32 measuredHeight = heightSize;
    Int32 measuredWidth = widthSize;

    // Grab the content size from WebViewCore.
    Int32 contentHeight = ContentToViewDimension(mContentHeight);
    Int32 contentWidth = ContentToViewDimension(mContentWidth);

    Logger::D(LOGTAG, "------- measure %d" + heightMode);

    if (heightMode != View::MeasureSpec::EXACTLY) {
        mHeightCanMeasure = TRUE;
        measuredHeight = contentHeight;
        if (heightMode == View::MeasureSpec::AT_MOST) {
            // If we are larger than the AT_MOST height, then our height can
            // no longer be measured and we should scroll internally.
            if (measuredHeight > heightSize) {
                measuredHeight = heightSize;
                mHeightCanMeasure = FALSE;
            }
        }
    } else {
        mHeightCanMeasure = FALSE;
    }

    if (mNativeClass != 0) {
        NativeSetHeightCanMeasure(mHeightCanMeasure);
    }

    // For the width, always use the given size unless unspecified.
    if (widthMode == View::MeasureSpec::UNSPECIFIED) {
        mWidthCanMeasure = TRUE;
        measuredWidth = contentWidth;
    } else {
        mWidthCanMeasure = FALSE;
    }

//    synchronized (this) {
        SetMeasuredDimension(measuredWidth, measuredHeight);
//    }
}


CARAPI_(void) CWebView::UpdateZoomButtonsEnabled()
{
//    if (mZoomButtonsController == NULL) {
//        return;
//    }

    Boolean canZoomIn = mActualScale < mMaxZoomScale;
    Boolean canZoomOut = mActualScale > mMinZoomScale && !mInZoomOverview;
    if (!canZoomIn && !canZoomOut) {
        // Hide the zoom in and out buttons, as well as the fit to page
        // button, if the page cannot zoom
//        mZoomButtonsController.getZoomControls().setVisibility(View.GONE);
    } else {
        // Set each one individually, as a page may be able to zoom in
        // or out.
//        mZoomButtonsController.setZoomInEnabled(canZoomIn);
//        mZoomButtonsController.setZoomOutEnabled(canZoomOut);
    }
}

CARAPI_(void) CWebView::Init()
{
    SetWillNotDraw(FALSE);
    SetFocusable(TRUE);
    SetFocusableInTouchMode(TRUE);
    SetClickable(TRUE);
    SetLongClickable(TRUE);

    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    assert(context.Get());

    AutoPtr<ViewConfiguration> configuration = ViewConfiguration::Get(context);
    Int32 slop = configuration->GetScaledTouchSlop();
    mTouchSlopSquare = slop * slop;
    mMinLockSnapReverseDistance = slop;
    slop = configuration->GetScaledDoubleTapSlop();
    mDoubleTapSlopSquare = slop * slop;
    /*const*/ Float density;// = getContext().getResources().getDisplayMetrics().density;
    // use one line height, 16 based on our current default font, for how
    // far we allow a touch be away from the edge of a link
    mNavSlop = (Int32) (16 * density);
    // density adjusted scale factors
    DEFAULT_SCALE_PERCENT = (Int32) (100 * density);
    mDefaultScale = density;
    mActualScale = density;
    mInvActualScale = 1 / density;
    mTextWrapScale = density;
    DEFAULT_MAX_ZOOM_SCALE = 4.0f * density;
    DEFAULT_MIN_ZOOM_SCALE = 0.25f * density;
    mMaxZoomScale = DEFAULT_MAX_ZOOM_SCALE;
    mMinZoomScale = DEFAULT_MIN_ZOOM_SCALE;
    mMaximumFling = configuration->GetScaledMaximumFlingVelocity();
    mOverscrollDistance = configuration->GetScaledOverscrollDistance();
    mOverflingDistance = configuration->GetScaledOverflingDistance();
}

/*
 * returns the height of the titlebarview (if any). Does not care about
 * scrolling
 */
CARAPI_(Int32) CWebView::GetTitleHeight()
{
    Int32 height = 0;
    mTitleBar->GetHeight(&height);
    return mTitleBar != NULL ? height : 0;
}

/*
 * Return the amount of the titlebarview (if any) that is visible
 */
CARAPI_(Int32) CWebView::GetVisibleTitleHeight()
{
    // need to restrict mScrollY due to over scroll
    //return Math.max(getTitleHeight() - Math.max(0, mScrollY), 0);
    Int32 max = 0 > mScrollY ? 0 : mScrollY;
    Int32 height = GetTitleHeight();

    max = height-max > 0 ? height-max : 0;

    return max;
}

CARAPI_(Int32) CWebView::GetViewHeightWithTitle()
{
    Int32 height = 0;

    GetHeight(&height);
    Boolean bFlag = FALSE;
    IsHorizontalScrollBarEnabled(&bFlag);

    if (bFlag && !mOverlayHorizontalScrollbar) {
        Int32 h = 0;
        GetHorizontalScrollbarHeight(&h);
        height -= h;
    }

    return height;
}

CARAPI_(void) CWebView::ClearHelpers()
{
    ClearTextEntry(FALSE);
    SelectionDone();
}

CARAPI_(void) CWebView::RestoreHistoryPictureFields(
    /* [in] */ IPicture* p,
    /* [in] */ IBundle* b)
{
    assert(p != NULL);
    assert(b != NULL);

    Int32 sx, sy;
    b->GetInt32Ex((String)"scrollX", 0, &sx);
    b->GetInt32Ex((String)"scrollY", 0, &sy);
    Float scale = 0.0;
    b->GetFloatEx((String)"scale", 1.0f, &scale);
    mDrawHistory = TRUE;
    mHistoryPicture = p;
    mScrollX = sx;
    mScrollY = sy;
//    mHistoryWidth = Math.round(p.getWidth() * scale);
//    mHistoryHeight = Math.round(p.getHeight() * scale);
    // as getWidth() / getHeight() of the view are not available yet, set up
    // mActualScale, so that when onSizeChanged() is called, the rest will
    // be set correctly
    mActualScale = scale;
    mInvActualScale = 1 / scale;
    b->GetFloatEx((String)"textwrapScale", scale, &mTextWrapScale);
    b->GetBoolean((String)"overview", &mInZoomOverview);
    InvalidateEx2();
}

CARAPI_(void) CWebView::GoBackOrForward(
    /* [in] */ Int32 steps,
    /* [in] */ Boolean ignoreSnapshot)
{
    if (steps != 0) {
        ClearHelpers();
        mWebViewCore->SendMessage(WebViewCore::EventHub::GO_BACK_FORWARD, steps,
                ignoreSnapshot ? 1 : 0);
    }
}

CARAPI_(Boolean) CWebView::ExtendScroll(
    /* [in] */ Int32 y)
{
    Int32 finalY = mScroller->GetFinalY();
    Int32 newY = PinLocY(finalY + y);
    if (newY == finalY) {
        return FALSE;
    }

    mScroller->SetFinalY(newY);
    mScroller->ExtendDuration(ComputeDuration(0, y));
    
    return TRUE;
}

/**
 *  Return true if the browser is displaying a TextView for text input.
 */
CARAPI_(Boolean) CWebView::InEditingMode()
{
//    return mWebTextView != NULL && mWebTextView->GetParent() != NULL;
}

/**
 * Remove the WebTextView.
 * @param disableFocusController If true, send a message to webkit
 *     disabling the focus controller, so the caret stops blinking.
 */
CARAPI_(void) CWebView::ClearTextEntry(
    /* [in] */ Boolean disableFocusController)
{
    if (InEditingMode()) {
        mWebTextView->Remove();
        if (disableFocusController) {
            SetFocusControllerInactive();
        }
    }
}

// Called by JNI when the DOM has changed the focus.  Clear the focus so
// that new keys will go to the newly focused field
CARAPI_(void) CWebView::DomChangedFocus()
{
    if (InEditingMode()) {
//        mPrivateHandler.obtainMessage(DOM_FOCUS_CHANGED).sendToTarget();
    }
}

CARAPI_(Int32) CWebView::PinLoc(
    /* [in] */ Int32 x,
    /* [in] */ Int32 viewMax,
    /* [in] */ Int32 docMax)
{
    Logger::D(LOGTAG, "-- pinLoc %d %d %d" + x + viewMax + docMax);
    if (docMax < viewMax) {   // the doc has room on the sides for "blank"
        // pin the short document to the top/left of the screen
        x = 0;
        Logger::D(LOGTAG, "--- center %d" + x);
    } else if (x < 0) {
        x = 0;
        Logger::D(LOGTAG, "--- zero");
    } else if (x + viewMax > docMax) {
        x = docMax - viewMax;
        Logger::D(LOGTAG, "--- pin %d" + x);
    }
    return x;
}

// Expects x in view coordinates
CARAPI_(Int32) CWebView::PinLocX(
    /* [in] */ Int32 x)
{
    if (mInOverScrollMode) {
        return x;
    }

    return PinLoc(x, GetViewWidth(), ComputeRealHorizontalScrollRange());
}

// Expects y in view coordinates
CARAPI_(Int32) CWebView::PinLocY(
    /* [in] */ Int32 y)
{
    if (mInOverScrollMode) {
        return y;
    }

    return PinLoc(y, GetViewHeightWithTitle(),
                      ComputeRealVerticalScrollRange() + GetTitleHeight());
}

CARAPI_(Int32) CWebView::ComputeRealVerticalScrollRange()
{
    if (mDrawHistory) {
        return mHistoryHeight;
    } else if (mVerticalScrollBarMode == SCROLLBAR_ALWAYSOFF
            && (mActualScale - mMinZoomScale <= MINIMUM_SCALE_INCREMENT)) {
        // only honor the scrollbar mode when it is at minimum zoom level
        return ComputeVerticalScrollExtent();
    } else {
        // to avoid rounding error caused unnecessary scrollbar, use floor
        return (int) /*Math.floor*/(mContentHeight * mActualScale);
    }
}

CARAPI_(void) CWebView::DrawContent(
    /* [in] */ ICanvas* canvas)
{
    // Update the buttons in the picture, so when we draw the picture
    // to the screen, they are in the correct state.
    // Tell the native side if user is a) touching the screen,
    // b) pressing the trackball down, or c) pressing the enter key
    // If the cursor is on a button, we need to draw it in the pressed
    // state.
    // If mNativeClass is 0, we should not reach here, so we do not
    // need to check it again.
    Boolean bFlag1, bFlag2;
    HasFocus(&bFlag1);
    HasWindowFocus(&bFlag2);
    NativeRecordButtons(bFlag1 && bFlag2,
                        mTouchMode == TOUCH_SHORTPRESS_START_MODE
                        || mTrackballDown /*|| mGotCenterDown*/, FALSE);
    DrawCoreAndCursorRing(canvas, mBackgroundColor, mDrawCursorRing);
}

/**
 * Given a distance in view space, convert it to content space. Note: this
 * does not reflect translation, just scaling, so this should not be called
 * with coordinates, but should be called for dimensions like width or
 * height.
 */
CARAPI_(Int32) CWebView::ViewToContentDimension(
    /* [in] */ Int32 d)
{
//    return Math.round(d * mInvActualScale);
}

/**
 * Given a x coordinate in view space, convert it to content space.
 * Returns the result as a float.
 */
CARAPI_(Float) CWebView::ViewToContentXf(
    /* [in] */ Int32 x)
{
    return x * mInvActualScale;
}

/**
 * Given a y coordinate in view space, convert it to content space.
 * Takes into account the height of the title bar if there is one
 * embedded into the WebView. Returns the result as a float.
 */
CARAPI_(Float) CWebView::ViewToContentYf(
    /* [in] */ Int32 y)
{
    return (y - GetTitleHeight()) * mInvActualScale;
}

CARAPI_(AutoPtr<IRect>) CWebView::ContentToViewRect(
    /* [in] */ IRect* x)
{
    assert(x != NULL);

    AutoPtr<IRect> rect;
    Int32 left, top, right, bottom;

    x->GetLeft(&left);
    x->GetTop(&top);
    x->GetRight(&right);
    x->GetBottom(&bottom);

    CRect::New(ContentToViewX(left), ContentToViewY(top),
                        ContentToViewX(right), ContentToViewY(bottom), 
                        (IRect**)&rect);
    return rect;
}

// Called by JNI to invalidate the View, given rectangle coordinates in
// content space
CARAPI_(void) CWebView::ViewInvalidate(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{}

// Called by JNI to invalidate the View after a delay, given rectangle
// coordinates in content space
CARAPI_(void) CWebView::ViewInvalidateDelayed(
    /* [in] */ Int64 delay,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{}

CARAPI_(void) CWebView::InvalidateContentRect(
    /* [in] */ IRect* r)
{
    assert(r != NULL);

    Int32 left, top, right, bottom;
    r->GetLeft(&left);
    r->GetTop(&top);
    r->GetRight(&right);
    r->GetBottom(&bottom);

    ViewInvalidate(left, top, right, bottom);
}

// stop the scroll animation, and don't let a subsequent fling add
// to the existing velocity
CARAPI_(void) CWebView::AbortAnimation()
{
    mScroller->AbortAnimation();
    mLastVelocity = 0;
}

/* call from webcoreview.draw(), so we're still executing in the UI thread
*/
CARAPI_(void) CWebView::RecordNewContentSize(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Boolean updateLayout)
{
    // premature data from webkit, ignore
    if ((w | h) == 0) {
        return;
    }

    // don't abort a scroll animation if we didn't change anything
    if (mContentWidth != w || mContentHeight != h) {
        // record new dimensions
        mContentWidth = w;
        mContentHeight = h;
        // If history Picture is drawn, don't update scroll. They will be
        // updated when we get out of that mode.
        if (!mDrawHistory) {
            // repin our scroll, taking into account the new content size
            Int32 oldX = mScrollX;
            Int32 oldY = mScrollY;
            mScrollX = PinLocX(mScrollX);
            mScrollY = PinLocY(mScrollY);

            if (oldX != mScrollX || oldY != mScrollY) {
                OnScrollChanged(mScrollX, mScrollY, oldX, oldY);
            }

            if (!mScroller->IsFinished()) {
                // We are in the middle of a scroll.  Repin the final scroll
                // position.
                mScroller->SetFinalX(PinLocX(mScroller->GetFinalX()));
                mScroller->SetFinalY(PinLocY(mScroller->GetFinalY()));
            }
        }
    }

    ContentSizeChanged(updateLayout);
}

CARAPI_(void) CWebView::SetNewZoomScale(
    /* [in] */ Float scale,
    /* [in] */ Boolean updateTextWrapScale,
    /* [in] */ Boolean force)
{
    if (scale < mMinZoomScale) {
        scale = mMinZoomScale;
        // set mInZoomOverview for non mobile sites
        if (scale < mDefaultScale) mInZoomOverview = TRUE;
    } else if (scale > mMaxZoomScale) {
        scale = mMaxZoomScale;
    }
    if (updateTextWrapScale) {
        mTextWrapScale = scale;
        // reset mLastHeightSent to force VIEW_SIZE_CHANGED sent to WebKit
        mLastHeightSent = 0;
    }
    if (scale != mActualScale || force) {
        if (mDrawHistory) {
            // If history Picture is drawn, don't update scroll. They will
            // be updated when we get out of that mode.
            if (scale != mActualScale && !mPreviewZoomOnly) {
                mCallbackProxy->OnScaleChanged(mActualScale, scale);
            }
            mActualScale = scale;
            mInvActualScale = 1 / scale;
            SendViewSizeZoom();
        } else {
            // update our scroll so we don't appear to jump
            // i.e. keep the center of the doc in the center of the view

            Int32 oldX = mScrollX;
            Int32 oldY = mScrollY;
            Float ratio = scale * mInvActualScale;   // old inverse
            Float sx = ratio * oldX + (ratio - 1) * mZoomCenterX;
            Float sy = ratio * oldY + (ratio - 1)
                    * (mZoomCenterY - GetTitleHeight());

            // now update our new scale and inverse
            if (scale != mActualScale && !mPreviewZoomOnly) {
                mCallbackProxy->OnScaleChanged(mActualScale, scale);
            }
            mActualScale = scale;
            mInvActualScale = 1 / scale;

            // Scale all the child views
            mViewManager->ScaleAll();

            // as we don't have animation for scaling, don't do animation
            // for scrolling, as it causes weird intermediate state
            //        pinScrollTo(Math.round(sx), Math.round(sy));
//            mScrollX = PinLocX(Math.round(sx));
//            mScrollY = PinLocY(Math.round(sy));

            // update webkit
            if (oldX != mScrollX || oldY != mScrollY) {
                OnScrollChanged(mScrollX, mScrollY, oldX, oldY);
            } else {
                // the scroll position is adjusted at the beginning of the
                // zoom animation. But we want to update the WebKit at the
                // end of the zoom animation. See comments in onScaleEnd().
                SendOurVisibleRect();
            }

            SendViewSizeZoom();
        }
    }
}

CARAPI_(AutoPtr<IRect>) CWebView::SendOurVisibleRect()
{
    if (mPreviewZoomOnly) {
        return mLastVisibleRectSent;
    }

    AutoPtr<IRect> rect;
    CRect::New((IRect**)&rect);
    assert(rect.Get());

    CalcOurContentVisibleRect(rect);
    // Rect.equals() checks for null input.
    Boolean result = FALSE;
    rect->Equals(mLastVisibleRectSent, &result);
    if (!result) {
        AutoPtr<IPoint> pos;
        Int32 left, top;

        rect->GetLeft(&left);
        rect->GetTop(&top);

        CPoint::New(left, top, (IPoint**)&pos);
        mWebViewCore->SendMessage(WebViewCore::EventHub::SET_SCROLL_OFFSET,
                NativeMoveGeneration(), 0, pos);
        mLastVisibleRectSent = rect;
    }

    AutoPtr<IRect> globalRect;
    CRect::New((IRect**)&globalRect);
    assert(globalRect.Get());

    Boolean bFlag1, bFlag2;
    GetGlobalVisibleRectEx(globalRect, &bFlag1);
    globalRect->Equals(mLastGlobalRect, &bFlag2);
    if (bFlag1 && !bFlag2) {

        if (DebugFlags::sWEB_VIEW) {
            Int32 left, top, right, bottom;
            globalRect->GetLeft(&left);
            globalRect->GetTop(&top);
            globalRect->GetRight(&right);
            globalRect->GetBottom(&bottom);
            Logger::V(LOGTAG, "sendOurVisibleRect=(%d,%d,r=%d,b=%d" + left +
                    + top + right + bottom);
        }
        // TODO: the global offset is only used by windowRect()
        // in ChromeClientAndroid ; other clients such as touch
        // and mouse events could return view + screen relative points.
        mWebViewCore->SendMessage(WebViewCore::EventHub::SET_GLOBAL_BOUNDS, globalRect);
        mLastGlobalRect = globalRect;
    }

    return rect;
}

// Sets r to be the visible rectangle of our webview in view coordinates
CARAPI_(void) CWebView::CalcOurVisibleRect(
    /* [in] */ IRect* r)
{
    assert(r != NULL);

    AutoPtr<IPoint> p;
    CPoint::New((IPoint**)&p);
    assert(p.Get());

    GetGlobalVisibleRect(r, p, NULL);
    r->Offset(-((CPoint*)p.Get())->mX, -((CPoint*)p.Get())->mY);
    
    if (mFindIsUp) {
        Int32 bottom = 0;
        r->GetBottom(&bottom);
        bottom -= mFindHeight;
        r->SetBottom(bottom);
    }
}

// Sets r to be our visible rectangle in content coordinates
CARAPI_(void) CWebView::CalcOurContentVisibleRect(
    /* [in] */ IRect* r)
{
    assert(r != NULL);

    CalcOurVisibleRect(r);

    Int32 top, left, right, bottom, value = 0;

    // since we might overscroll, pin the rect to the bounds of the content
   // r.left = Math.max(viewToContentX(r.left), 0);
    r->GetLeft(&left);
    value = ViewToContentX(left);
    value = value > 0 ? value : 0;
    r->SetLeft(value);

    // viewToContentY will remove the total height of the title bar.  Add
    // the visible height back in to account for the fact that if the title
    // bar is partially visible, the part of the visible rect which is
    // displaying our content is displaced by that amount.
  //  r.top = Math.max(viewToContentY(r.top + getVisibleTitleHeight()), 0);
    r->GetTop(&top);
    value = ViewToContentY(top + GetVisibleTitleHeight());
    value = value > 0 ? value : 0;
    r->SetTop(value);

  //  r.right = Math.min(viewToContentX(r.right), mContentWidth);
    r->GetRight(&right);
    value = ViewToContentX(right);
    value = value > mContentWidth ? mContentWidth : value;
    r->SetRight(value);

  //  r.bottom = Math.min(viewToContentY(r.bottom), mContentHeight);
    r->GetBottom(&bottom);
    value = ViewToContentY(bottom);
    value = value > mContentHeight ? mContentHeight : value;
    r->SetBottom(value);
}

// Sets r to be our visible rectangle in content coordinates. We use this
// method on the native side to compute the position of the fixed layers.
// Uses floating coordinates (necessary to correctly place elements when
// the scale factor is not 1)
CARAPI_(void) CWebView::CalcOurContentVisibleRectF(
    /* [in] */ IRectF* r)
{
    assert(r != NULL);

    AutoPtr<IRect> ri;
    CRect::New(0,0,0,0, (IRect**)&ri);
    assert(ri.Get());

    CalcOurVisibleRect(ri);

    Int32 left, top, right, bottom;
    Float value;

    // pin the rect to the bounds of the content
 //   r.left = Math.max(viewToContentXf(ri.left), 0.0f);
    ri->GetLeft(&left);
    value = ViewToContentXf(left);
    value = value > 0.0f ? value : 0.0f;
    ((CRectF*)r)->mLeft = value;

    // viewToContentY will remove the total height of the title bar.  Add
    // the visible height back in to account for the fact that if the title
    // bar is partially visible, the part of the visible rect which is
    // displaying our content is displaced by that amount.
 //   r.top = Math.max(viewToContentYf(ri.top + getVisibleTitleHeight()), 0.0f);
    ri->GetTop(&top);
    value = top + GetVisibleTitleHeight();
    value = value > 0.0f ? value : 0.0f;
    ((CRectF*)r)->mTop = value;

//    r.right = Math.min(viewToContentXf(ri.right), (float)mContentWidth);
    ri->GetRight(&right);
    value = ViewToContentXf(right);
    value = value > mContentWidth ? mContentWidth : value;
//    r->SetRight(value);
    ((CRectF*)r)->mRight = value;

 //   r.bottom = Math.min(viewToContentYf(ri.bottom), (float)mContentHeight);
    ri->GetBottom(&bottom);
    value = ViewToContentYf(bottom);
    value = value > mContentHeight ? mContentHeight : value;
    ((CRectF*)r)->mBottom = value;
}

/**
 * Compute unzoomed width and height, and if they differ from the last
 * values we sent, send them to webkit (to be used has new viewport)
 *
 * @return true if new values were sent
 */
CARAPI_(Boolean) CWebView::SendViewSizeZoom()
{
    if (mPreviewZoomOnly) {
        return FALSE;
    }

    Int32 viewWidth = GetViewWidth();
    Int32 newWidth;// = Math.round(viewWidth * mInvActualScale);
    Int32 newHeight;// = Math.round(getViewHeight() * mInvActualScale);
    /*
     * Because the native side may have already done a layout before the
     * View system was able to measure us, we have to send a height of 0 to
     * remove excess whitespace when we grow our width. This will trigger a
     * layout and a change in content size. This content size change will
     * mean that contentSizeChanged will either call this method directly or
     * indirectly from onSizeChanged.
     */
    if (newWidth > mLastWidthSent && mWrapContent) {
        newHeight = 0;
    }

    // Avoid sending another message if the dimensions have not changed.
    if (newWidth != mLastWidthSent || newHeight != mLastHeightSent) {
        ViewSizeData* data = new ViewSizeData();
        data->mWidth = newWidth;
        data->mHeight = newHeight;
//        data->mTextWrapWidth = Math.round(viewWidth / mTextWrapScale);;
        data->mScale = mActualScale;
        data->mIgnoreHeight = mZoomScale != 0 && !mHeightCanMeasure;
        data->mAnchorX = mAnchorX;
        data->mAnchorY = mAnchorY;
        mWebViewCore->SendMessage(WebViewCore::EventHub::VIEW_SIZE_CHANGED, (IInterface*)data);
        mLastWidthSent = newWidth;
        mLastHeightSent = newHeight;
        mAnchorX = mAnchorY = 0;
        
        return TRUE;
    }

    return FALSE;
}

CARAPI_(Int32) CWebView::ComputeRealHorizontalScrollRange()
{
    if (mDrawHistory) {
        return mHistoryWidth;
    } else if (mHorizontalScrollBarMode == SCROLLBAR_ALWAYSOFF
            && (mActualScale - mMinZoomScale <= MINIMUM_SCALE_INCREMENT)) {
        // only honor the scrollbar mode when it is at minimum zoom level
        return ComputeHorizontalScrollExtent();
    } else {
        // to avoid rounding error caused unnecessary scrollbar, use floor
        return (Int32) /*Math.floor*/(mContentWidth * mActualScale);
    }
}

CARAPI_(Int32) CWebView::ComputeDuration(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    dx = dx < 0 ? -dx : dx;
    dy = dy < 0 ? -dy : dy;

    Int32 distance = dx > dy ? dx : dy;//Math.max(Math.abs(dx), Math.abs(dy));
    Int32 duration = distance * 1000 / STD_SPEED;

    return duration < MAX_DURATION ? duration : MAX_DURATION;//Math.min(duration, MAX_DURATION);
}

// helper to pin the scrollBy parameters (already in view coordinates)
// returns true if the scroll was changed
CARAPI_(Boolean) CWebView::PinScrollBy(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy,
    /* [in] */ Boolean animate,
    /* [in] */ Int32 animationDuration)
{
    return PinScrollTo(mScrollX + dx, mScrollY + dy, animate, animationDuration);
}

// helper to pin the scrollTo parameters (already in view coordinates)
// returns true if the scroll was changed
CARAPI_(Boolean) CWebView::PinScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Boolean animate,
    /* [in] */ Int32 animationDuration)
{
    x = PinLocX(x);
    y = PinLocY(y);

    Int32 dx = x - mScrollX;
    Int32 dy = y - mScrollY;

    if ((dx | dy) == 0) {
        return FALSE;
    }

    if (animate) {
        Logger::D(LOGTAG, "startScroll: %d %d" + dx + dy);
        mScroller->StartScroll(mScrollX, mScrollY, dx, dy,
                animationDuration > 0 ? animationDuration : ComputeDuration(dx, dy));
        AwakenScrollBars(mScroller->GetDuration());
        InvalidateEx2();
    } else {
        AbortAnimation(); // just in case
        ScrollTo(x, y);
    }

    return TRUE;
}

// Scale from content to view coordinates, and pin.
// Also called by jni webview.cpp
CARAPI_(Boolean) CWebView::SetContentScrollBy(
    /* [in] */ Int32 cx,
    /* [in] */ Int32 cy,
    /* [in] */ Boolean animate)
{
    if (mDrawHistory) {
        // disallow WebView to change the scroll position as History Picture
        // is used in the view system.
        // TODO: as we switchOutDrawHistory when trackball or navigation
        // keys are hit, this should be safe. Right?
        return FALSE;
    }

    cx = ContentToViewDimension(cx);
    cy = ContentToViewDimension(cy);

    if (mHeightCanMeasure) {
        // move our visible rect according to scroll request
        if (cy != 0) {
            AutoPtr<IRect> tempRect;
            CRect::New((IRect**)&tempRect);
            assert(tempRect.Get());
            CalcOurVisibleRect(tempRect);
            tempRect->Offset(cx, cy);
            RequestRectangleOnScreen(tempRect, NULL);
        }
        // FIXME: We scroll horizontally no matter what because currently
        // ScrollView and ListView will not scroll horizontally.
        // FIXME: Why do we only scroll horizontally if there is no
        // vertical scroll?
        Logger::D(LOGTAG, "setContentScrollBy cy=%d" + cy);
        return cy == 0 && cx != 0 && PinScrollBy(cx, 0, animate, 0);
    } else {
        return PinScrollBy(cx, cy, animate, 0);
    }
}

// scale from content to view coordinates, and pin
// return true if pin caused the final x/y different than the request cx/cy,
// and a future scroll may reach the request cx/cy after our size has
// changed
// return false if the view scroll to the exact position as it is requested,
// where negative numbers are taken to mean 0
CARAPI_(Boolean) CWebView::SetContentScrollTo(
    /* [in] */ Int32 cx,
    /* [in] */ Int32 cy)
{
    if (mDrawHistory) {
        // disallow WebView to change the scroll position as History Picture
        // is used in the view system.
        // One known case where this is called is that WebCore tries to
        // restore the scroll position. As history Picture already uses the
        // saved scroll position, it is ok to skip this.
        return FALSE;
    }

    Int32 vx;
    Int32 vy;
    if ((cx | cy) == 0) {
        // If the page is being scrolled to (0,0), do not add in the title
        // bar's height, and simply scroll to (0,0). (The only other work
        // in contentToView_ is to multiply, so this would not change 0.)
        vx = 0;
        vy = 0;
    } else {
        vx = ContentToViewX(cx);
        vy = ContentToViewY(cy);
    }
        Logger::D(LOGTAG, "content scrollTo [%d %d] view=[%d %d]" + cx + cy + 
                      vx + vy);
    // Some mobile sites attempt to scroll the title bar off the page by
    // scrolling to (0,1).  If we are at the top left corner of the
    // page, assume this is an attempt to scroll off the title bar, and
    // animate the title bar off screen slowly enough that the user can see
    // it.
    if (cx == 0 && cy == 1 && mScrollX == 0 && mScrollY == 0
            && mTitleBar != NULL) {
        // FIXME: 100 should be defined somewhere as our max progress.
        Int32 progress = 0;
        GetProgress(&progress);
        if (progress < 100) {
            // Wait to scroll the title bar off screen until the page has
            // finished loading.  Keep track of the URL and the destination
            // Y position
            GetUrl(&mPageThatNeedsToSlideTitleBarOffScreen);
            mYDistanceToSlideTitleOffScreen = vy;
        } else {
            PinScrollTo(vx, vy, TRUE, SLIDE_TITLE_DURATION);
        }
        // Since we are animating, we have not yet reached the desired
        // scroll position.  Do not return true to request another attempt
        return FALSE;
    }

    PinScrollTo(vx, vy, FALSE, 0);
    // If the request was to scroll to a negative coordinate, treat it as if
    // it was a request to scroll to 0
    if ((mScrollX != vx && cx >= 0) || (mScrollY != vy && cy >= 0)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

// scale from content to view coordinates, and pin
CARAPI_(void) CWebView::SpawnContentScrollTo(
    /* [in] */ Int32 cx,
    /* [in] */ Int32 cy)
{
    if (mDrawHistory) {
        // disallow WebView to change the scroll position as History Picture
        // is used in the view system.
        return;
    }

    Int32 vx = ContentToViewX(cx);
    Int32 vy = ContentToViewY(cy);
    PinScrollTo(vx, vy, true, 0);
}

/**
 * These are from webkit, and are in content coordinate system (unzoomed)
 */
CARAPI_(void) CWebView::ContentSizeChanged(
    /* [in] */ Boolean updateLayout)
{
    // suppress 0,0 since we usually see real dimensions soon after
    // this avoids drawing the prev content in a funny place. If we find a
    // way to consolidate these notifications, this check may become
    // obsolete
    if ((mContentWidth | mContentHeight) == 0) {
        return;
    }

    if (mHeightCanMeasure) {
        Int32 height = 0;
        GetMeasuredHeight(&height);
        if (height != ContentToViewDimension(mContentHeight)
                || updateLayout) {
            RequestLayout();
        }
    } else if (mWidthCanMeasure) {
        Int32 width = 0;
        GetMeasuredWidth(&width);
        if (width != ContentToViewDimension(mContentWidth)
                || updateLayout) {
            RequestLayout();
        }
    } else {
        // If we don't request a layout, try to send our view size to the
        // native side to ensure that WebCore has the correct dimensions.
        SendViewSizeZoom();
    }
}

/**
 * Draw the glow effect along the sides of the widget. mEdgeGlow* must be non-null.
 *
 * @param canvas Canvas to draw into, transformed into view coordinates.
 * @return true if glow effects are still animating and the view should invalidate again.
 */
CARAPI_(Boolean) CWebView::DrawEdgeGlows(
    /* [in] */ ICanvas* canvas)
{
    assert(canvas != NULL);

    const Int32 scrollX = mScrollX;
    const Int32 scrollY = mScrollY;
    /*const*/ Int32 width = 0;
    Int32 height = 0;

    GetWidth(&width);
    GetHeight(&height);

    Boolean invalidateForGlow = false;
    if (!mEdgeGlowTop->IsFinished()) {
        /*const*/ Int32 restoreCount = 0;
        canvas->Save(&restoreCount);

        Int32 min = 0;
        min = 0 < scrollY ? 0 : scrollY;
        canvas->Translate(-width / 2 + scrollX, /*Math.min(0, scrollY)*/min);
        mEdgeGlowTop->SetSize(width * 2, height);
        invalidateForGlow |= mEdgeGlowTop->Draw(canvas);
        canvas->RestoreToCount(restoreCount);
    }

    if (!mEdgeGlowBottom->IsFinished()) {
        /*const*/ Int32 restoreCount = 0;
        canvas->Save(&restoreCount);

        Int32 max = 0;
        max = ComputeMaxScrollY();
        max = max > scrollY ? max : scrollY;
        canvas->Translate(-width / 2 + scrollX, /*Math.max(computeMaxScrollY(), scrollY)*/max + height);
        canvas->RotateEx(180.0, (Float)width, 0.0);
        mEdgeGlowBottom->SetSize(width * 2, height);
        invalidateForGlow |= mEdgeGlowBottom->Draw(canvas);
        canvas->RestoreToCount(restoreCount);
    }

    if (!mEdgeGlowLeft->IsFinished()) {
        /*const*/ Int32 restoreCount = 0;
        canvas->Save(&restoreCount);

        canvas->Rotate(270.0f);
        canvas->Translate(-height * 1.5f - scrollY, /*Math.min(0, scrollX)*/
                      0 < scrollX ? 0 : scrollX);
        mEdgeGlowLeft->SetSize(height * 2, width);
        invalidateForGlow |= mEdgeGlowLeft->Draw(canvas);
        canvas->RestoreToCount(restoreCount);
    }

    if (!mEdgeGlowRight->IsFinished()) {
        /*const*/ Int32 restoreCount = 0;
        canvas->Save(&restoreCount);

        canvas->Rotate(90);

        Int32 max = 0;
        max = ComputeMaxScrollX();
        max = max > scrollX ? max : scrollX;
        canvas->Translate(-height / 2 + scrollY,
                -(/*Math.max(computeMaxScrollX(), scrollX)*/max + width));
        mEdgeGlowRight->SetSize(height * 2, width);
        invalidateForGlow |= mEdgeGlowRight->Draw(canvas);
        canvas->RestoreToCount(restoreCount);
    }

    return invalidateForGlow;
}


CARAPI_(Boolean) CWebView::DidUpdateTextViewBounds(
    /* [in] */ Boolean allowIntersect)
{
    AutoPtr<IRect> contentBounds = NativeFocusCandidateNodeBounds();
    AutoPtr<IRect> vBox = ContentToViewRect(contentBounds);
    AutoPtr<IRect> visibleRect;
    CRect::New((IRect**)&visibleRect);
    assert(visibleRect.Get());
    CalcOurVisibleRect(visibleRect);

    // If the textfield is on screen, place the WebTextView in
    // its new place, accounting for our new scroll/zoom values,
    // and adjust its textsize.
    Boolean bFlag1, bFlag2;
    if (allowIntersect ? (visibleRect->IntersectsEx(visibleRect, vBox, &bFlag1), bFlag1)
            : (visibleRect->ContainsEx2(vBox, &bFlag2), bFlag2)) {
        Int32 left, top, width, height;
        vBox->GetLeft(&left);
        vBox->GetTop(&top);
        vBox->GetWidth(&width);
        vBox->GetHeight(&height);
        mWebTextView->SetRect(left, top, width, height);
//        mWebTextView->SetTextSize(TypedValue_COMPLEX_UNIT_PX, ContentToViewDimension(
//                NativeFocusCandidateTextSize()));

        return TRUE;
    } else {
        // The textfield is now off screen.  The user probably
        // was not zooming to see the textfield better.  Remove
        // the WebTextView.  If the user types a key, and the
        // textfield is still in focus, we will reconstruct
        // the WebTextView and scroll it back on screen.
        mWebTextView->Remove();

        return FALSE;
    }
}

CARAPI_(void) CWebView::DrawExtras(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Int32 extras,
    /* [in] */ Boolean animationsRunning)
{
    assert(canvas != NULL);

    // If mNativeClass is 0, we should not reach here, so we do not
    // need to check it again.
    if (animationsRunning) {
        canvas->SetDrawFilter(mWebViewCore->mZoomFilter);
    }
    NativeDrawExtras(canvas, extras);
    canvas->SetDrawFilter(NULL);
}

CARAPI_(void) CWebView::DrawCoreAndCursorRing(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Int32 color,
    /* [in] */ Boolean drawCursorRing)
{
    assert(canvas != NULL);

    if (mDrawHistory) {
        canvas->Scale(mActualScale, mActualScale);
        canvas->DrawPicture(mHistoryPicture);
        return;
    }

    Boolean animateZoom = mZoomScale != 0;
    Boolean animateScroll = ((!mScroller->IsFinished()
            || mVelocityTracker != NULL)
            && (mTouchMode != TOUCH_DRAG_MODE ||
            mHeldMotionless != MOTIONLESS_TRUE))
            || mDeferTouchMode == TOUCH_DRAG_MODE;
    if (mTouchMode == TOUCH_DRAG_MODE) {
        if (mHeldMotionless == MOTIONLESS_PENDING) {
//            mPrivateHandler.removeMessages(DRAG_HELD_MOTIONLESS);
//            mPrivateHandler.removeMessages(AWAKEN_SCROLL_BARS);
            mHeldMotionless = MOTIONLESS_FALSE;
        }
        if (mHeldMotionless == MOTIONLESS_FALSE) {
//            mPrivateHandler.sendMessageDelayed(mPrivateHandler
//                    .obtainMessage(DRAG_HELD_MOTIONLESS), MOTIONLESS_TIME);
            mHeldMotionless = MOTIONLESS_PENDING;
        }
    }
    if (animateZoom) {
        Float zoomScale;
        Int32 interval;// = (int) (SystemClock.uptimeMillis() - mZoomStart);
        if (interval < ZOOM_ANIMATION_LENGTH) {
            Float ratio = (Float) interval / ZOOM_ANIMATION_LENGTH;
            zoomScale = 1.0f / (mInvInitialZoomScale
                    + (mInvFinalZoomScale - mInvInitialZoomScale) * ratio);
            InvalidateEx2();
        } else {
            zoomScale = mZoomScale;
            // set mZoomScale to be 0 as we have done animation
            mZoomScale = 0;
            WebViewCore::ResumeUpdatePicture(mWebViewCore);
            // call invalidate() again to draw with the final filters
            InvalidateEx2();
            if (mNeedToAdjustWebTextView) {
                mNeedToAdjustWebTextView = FALSE;
                if (DidUpdateTextViewBounds(FALSE)
                        && NativeFocusCandidateIsPassword()) {
                    // If it is a password field, start drawing the
                    // WebTextView once again.
                    mWebTextView->SetInPassword(TRUE);
                }
            }
        }
        // calculate the intermediate scroll position. As we need to use
        // zoomScale, we can't use pinLocX/Y directly. Copy the logic here.
        Float scale = zoomScale * mInvInitialZoomScale;
        Int32 tx;// = Math.round(scale * (mInitialScrollX + mZoomCenterX) - mZoomCenterX);
//        tx = -pinLoc(tx, getViewWidth(), Math.round(mContentWidth * zoomScale)) + mScrollX;

        Int32 titleHeight = GetTitleHeight();
        Int32 ty;// = Math.round(scale * (mInitialScrollY + mZoomCenterY - titleHeight)
//                - (mZoomCenterY - titleHeight));
//        ty = -(ty <= titleHeight ? Math.max(ty, 0) : pinLoc(ty
//                - titleHeight, getViewHeight(), Math.round(mContentHeight
//                * zoomScale)) + titleHeight) + mScrollY;

        canvas->Translate(tx, ty);
        canvas->Scale(zoomScale, zoomScale);
        if (InEditingMode() && !mNeedToAdjustWebTextView
                && mZoomScale != 0) {
            // The WebTextView is up.  Keep track of this so we can adjust
            // its size and placement when we finish zooming
            mNeedToAdjustWebTextView = TRUE;
            // If it is in password mode, turn it off so it does not draw
            // misplaced.
            if (NativeFocusCandidateIsPassword()) {
                mWebTextView->SetInPassword(FALSE);
            }
        }
    } else {
        canvas->Scale(mActualScale, mActualScale);
    }

    Boolean UIAnimationsRunning = FALSE;
    // Currently for each draw we compute the animation values;
    // We may in the future decide to do that independently.
    if (mNativeClass != 0 && NativeEvaluateLayersAnimations()) {
        UIAnimationsRunning = TRUE;
        // If we have unfinished (or unstarted) animations,
        // we ask for a repaint.
        InvalidateEx2();
    }

    mWebViewCore->DrawContentPicture(canvas, color,
            (animateZoom || mPreviewZoomOnly || UIAnimationsRunning),
            animateScroll);

    if (mNativeClass == 0) {
        return;
    }

    // decide which adornments to draw
    Int32 extras = DRAW_EXTRAS_NONE;
    if (mFindIsUp) {
            extras = DRAW_EXTRAS_FIND;
    } else if (mSelectingText) {
        extras = DRAW_EXTRAS_SELECTION;
        NativeSetSelectionPointer(mDrawSelectionPointer,
                mInvActualScale,
                mSelectX, mSelectY - GetTitleHeight());
    } else if (drawCursorRing) {
        extras = DRAW_EXTRAS_CURSOR_RING;
    }

    DrawExtras(canvas, extras, UIAnimationsRunning);

    if (extras == DRAW_EXTRAS_CURSOR_RING) {
        if (mTouchMode == TOUCH_SHORTPRESS_START_MODE) {
            mTouchMode = TOUCH_SHORTPRESS_MODE;
        }
    }

    if (mFocusSizeChanged) {
        mFocusSizeChanged = FALSE;
        // If we are zooming, this will get handled above, when the zoom
        // finishes.  We also do not need to do this unless the WebTextView
        // is showing.
        if (!animateZoom && InEditingMode()) {
            DidUpdateTextViewBounds(TRUE);
        }
    }
}

/**
 * Called in response to a message from webkit telling us that the soft
 * keyboard should be launched.
 */
CARAPI_(void) CWebView::DisplaySoftKeyboard(
    /* [in] */ Boolean isTextView)
{
//    InputMethodManager imm = (InputMethodManager)
//            getContext().getSystemService(Context.INPUT_METHOD_SERVICE);

    // bring it back to the default scale so that user can enter text
    Boolean zoom = mActualScale < mDefaultScale;
    if (zoom) {
        mInZoomOverview = FALSE;
        mZoomCenterX = mLastTouchX;
        mZoomCenterY = mLastTouchY;
        // do not change text wrap scale so that there is no reflow
        SetNewZoomScale(mDefaultScale, FALSE, FALSE);
    }
    if (isTextView) {
        RebuildWebTextView();
        if (InEditingMode()) {
//            imm.showSoftInput(mWebTextView, 0);
            if (zoom) {
                DidUpdateTextViewBounds(TRUE);
            }
            return;
        }
    }
    // Used by plugins.
    // Also used if the navigation cache is out of date, and
    // does not recognize that a textfield is in focus.  In that
    // case, use WebView as the targeted view.
    // see http://b/issue?id=2457459
//    imm.showSoftInput(this, 0);
}

// Called by WebKit to instruct the UI to hide the keyboard
CARAPI_(void) CWebView::HideSoftKeyboard()
{}

CARAPI_(void) CWebView::SetActive(
    /* [in] */ Boolean active)
{
    if (active) {
        Boolean bFocus = FALSE;
        HasFocus(&bFocus);
        if (bFocus) {
            // If our window regained focus, and we have focus, then begin
            // drawing the cursor ring
            mDrawCursorRing = TRUE;
            if (mNativeClass != 0) {
                NativeRecordButtons(TRUE, FALSE, TRUE);
                if (InEditingMode()) {
                    mWebViewCore->SendMessage(WebViewCore::EventHub::SET_ACTIVE, 1, 0);
                }
            }
        } else {
            // If our window gained focus, but we do not have it, do not
            // draw the cursor ring.
            mDrawCursorRing = FALSE;
            // We do not call nativeRecordButtons here because we assume
            // that when we lost focus, or window focus, it got called with
            // false for the first parameter
        }
    } else {
        AutoPtr<IWebSettings> ws;
        GetSettings((IWebSettings**)&ws);
        assert(ws.Get());
        Boolean b = FALSE;
        ws->GetBuiltInZoomControls(&b);
        if (mWebViewCore != NULL && b /*&& (mZoomButtonsController == NULL ||
                        !mZoomButtonsController->IsVisible())*/) {
            /*
             * The zoom controls come in their own window, so our window
             * loses focus. Our policy is to not draw the cursor ring if
             * our window is not focused, but this is an exception since
             * the user can still navigate the web page with the zoom
             * controls showing.
             */
            // If our window has lost focus, stop drawing the cursor ring
            mDrawCursorRing = FALSE;
        }

        mGotKeyDown = FALSE;
        mShiftIsPressed = FALSE;
//        mPrivateHandler.removeMessages(SWITCH_TO_LONGPRESS);
        mTouchMode = TOUCH_DONE_MODE;
        if (mNativeClass != 0) {
            NativeRecordButtons(FALSE, FALSE, TRUE);
        }

        SetFocusControllerInactive();
    }

    InvalidateEx2();
}

CARAPI_(Int32) CWebView::Sign(
    /* [in] */ Float x)
{
    return x > 0 ? 1 : (x < 0 ? -1 : 0);
}

CARAPI_(Boolean) CWebView::HitFocusedPlugin(
    /* [in] */ Int32 contentX,
    /* [in] */ Int32 contentY)
{
    if (DebugFlags::sWEB_VIEW) {
        Logger::V(LOGTAG, "nativeFocusIsPlugin()=%d" + NativeFocusIsPlugin());
        AutoPtr<IRect> r = NativeFocusNodeBounds();
        Int32 left, top, right, bottom;
        r->GetLeft(&left);
        r->GetTop(&top);
        r->GetRight(&right);
        r->GetBottom(&bottom);
        Logger::V(LOGTAG, "NativeFocusNodeBounds()=(%d, %d, %d, %d)" + left 
                + top + right + bottom);
    }

    Boolean bFlag = FALSE;
    NativeFocusNodeBounds()->Contains(contentX, contentY, &bFlag);

    return NativeFocusIsPlugin() && bFlag;
}

CARAPI_(Boolean) CWebView::ShouldForwardTouchEvent()
{
    return mFullScreenHolder != NULL || (mForwardTouchEvents
                && !mSelectingText
                && mPreventDefault != PREVENT_DEFAULT_IGNORE);
}

CARAPI_(Boolean) CWebView::InFullScreenMode()
{
    return mFullScreenHolder != NULL;
}

CARAPI_(void) CWebView::CancelWebCoreTouchEvent(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Boolean removeEvents)
{
    if (ShouldForwardTouchEvent()) {
        if (removeEvents) {
            mWebViewCore->RemoveMessages(WebViewCore::EventHub::TOUCH_EVENT);
        }

        WebViewCore::TouchEventData* ted = new WebViewCore::TouchEventData();
        ted->mX = x;
        ted->mY = y;
        ted->mAction = MotionEvent_ACTION_CANCEL;
        mWebViewCore->SendMessage(WebViewCore::EventHub::TOUCH_EVENT, (IInterface*)ted);
        mPreventDefault = PREVENT_DEFAULT_IGNORE;
    }
}

CARAPI_(void) CWebView::StartTouch(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int64 eventTime)
{
    // Remember where the motion event started
    mLastTouchX = x;
    mLastTouchY = y;
    mLastTouchTime = eventTime;
    mVelocityTracker = VelocityTracker::Obtain();
    mSnapScrollMode = SNAP_NONE;
    if (mDragTracker != NULL) {
        mDragTrackerHandler = new DragTrackerHandler(x, y, mDragTracker);
    }
}

CARAPI_(void) CWebView::StartDrag()
{}

CARAPI_(void) CWebView::DoDrag(
    /* [in] */ Int32 deltaX,
    /* [in] */ Int32 deltaY)
{
    if ((deltaX | deltaY) != 0) {
        const Int32 oldX = mScrollX;
        const Int32 oldY = mScrollY;
        const Int32 rangeX = ComputeMaxScrollX();
        const Int32 rangeY = ComputeMaxScrollY();

        if (mEdgeGlowTop != NULL) {
            // Save the deltas for overscroll glow.
            mOverscrollDeltaX = deltaX;
            mOverscrollDeltaY = deltaY;
        }

        OverScrollBy(deltaX, deltaY, oldX, oldY,
                rangeX, rangeY,
                mOverscrollDistance, mOverscrollDistance, TRUE);
        if (mEdgeGlowTop != NULL &&
                (!mEdgeGlowTop->IsFinished() || !mEdgeGlowBottom->IsFinished() ||
                        !mEdgeGlowLeft->IsFinished() || !mEdgeGlowRight->IsFinished())) {
            InvalidateEx2();
        }
    }

    AutoPtr<IWebSettings> ws;
    GetSettings((IWebSettings**)&ws);
    assert(ws.Get());
    Boolean b = FALSE;
    ws->GetBuiltInZoomControls(&b);
    if (!b) {
        Boolean showPlusMinus = mMinZoomScale < mMaxZoomScale;
        if (mZoomControls != NULL && showPlusMinus) {
            if (/*mZoomControls->GetVisibility() == View_VISIBLE*/0) {
//                mPrivateHandler->RemoveCallbacks(mZoomControlRunnable);
            } else {
                mZoomControls->Show(showPlusMinus, FALSE);
            }

//            mPrivateHandler.postDelayed(mZoomControlRunnable, ZOOM_CONTROLS_TIMEOUT);
        }
    }
}

CARAPI_(void) CWebView::StopTouch()
{
    if (mDragTrackerHandler != NULL) {
        mDragTrackerHandler->StopDrag();
    }

    // we also use mVelocityTracker == null to tell us that we are
    // not "moving around", so we can take the slower/prettier
    // mode in the drawing code
    if (mVelocityTracker != NULL) {
        mVelocityTracker->Recycle();
        mVelocityTracker = NULL;
    }

    // Release any pulled glows
    if (mEdgeGlowTop != NULL) {
        mEdgeGlowTop->OnRelease();
        mEdgeGlowBottom->OnRelease();
        mEdgeGlowLeft->OnRelease();
        mEdgeGlowRight->OnRelease();
    }
}

CARAPI_(void) CWebView::CancelTouch()
{
    if (mDragTrackerHandler != NULL) {
        mDragTrackerHandler->StopDrag();
    }

    // we also use mVelocityTracker == null to tell us that we are
    // not "moving around", so we can take the slower/prettier
    // mode in the drawing code
    if (mVelocityTracker != NULL) {
        mVelocityTracker->Recycle();
        mVelocityTracker = NULL;
    }

    // Release any pulled glows
    if (mEdgeGlowTop != NULL) {
        mEdgeGlowTop->OnRelease();
        mEdgeGlowBottom->OnRelease();
        mEdgeGlowLeft->OnRelease();
        mEdgeGlowRight->OnRelease();
    }

    if (mTouchMode == TOUCH_DRAG_MODE) {
        WebViewCore::ResumePriority();
        WebViewCore::ResumeUpdatePicture(mWebViewCore);
    }

//    mPrivateHandler.removeMessages(SWITCH_TO_SHORTPRESS);
//    mPrivateHandler.removeMessages(SWITCH_TO_LONGPRESS);
//    mPrivateHandler.removeMessages(DRAG_HELD_MOTIONLESS);
//    mPrivateHandler.removeMessages(AWAKEN_SCROLL_BARS);
    mHeldMotionless = MOTIONLESS_TRUE;
    mTouchMode = TOUCH_DONE_MODE;
    NativeHideCursor();
}

CARAPI_(Int32) CWebView::ScaleTrackballX(
    /* [in] */ Float xRate,
    /* [in] */ Int32 width)
{
    Int32 xMove = (Int32) (xRate / TRACKBALL_SCALE * width);
    Int32 nextXMove = xMove;
    if (xMove > 0) {
        if (xMove > mTrackballXMove) {
            xMove -= mTrackballXMove;
        }
    } else if (xMove < mTrackballXMove) {
        xMove -= mTrackballXMove;
    }

    mTrackballXMove = nextXMove;

    return xMove;
}

CARAPI_(Int32) CWebView::ScaleTrackballY(
    /* [in] */ Float yRate,
    /* [in] */ Int32 height)
{
    Int32 yMove = (Int32) (yRate / TRACKBALL_SCALE * height);
    Int32 nextYMove = yMove;
    if (yMove > 0) {
        if (yMove > mTrackballYMove) {
            yMove -= mTrackballYMove;
        }
    } else if (yMove < mTrackballYMove) {
        yMove -= mTrackballYMove;
    }

    mTrackballYMove = nextYMove;

    return yMove;
}

CARAPI_(Int32) CWebView::KeyCodeToSoundsEffect(
    /* [in] */ Int32 keyCode)
{
    switch(keyCode) {
        case KeyEvent_KEYCODE_DPAD_UP:
            return SoundEffectConstants_NAVIGATION_UP;
        case KeyEvent_KEYCODE_DPAD_RIGHT:
            return SoundEffectConstants_NAVIGATION_RIGHT;
        case KeyEvent_KEYCODE_DPAD_DOWN:
            return SoundEffectConstants_NAVIGATION_DOWN;
        case KeyEvent_KEYCODE_DPAD_LEFT:
            return SoundEffectConstants_NAVIGATION_LEFT;
    }

//    throw new IllegalArgumentException("keyCode must be one of " +
//            "{KEYCODE_DPAD_UP, KEYCODE_DPAD_RIGHT, KEYCODE_DPAD_DOWN, " +
//            "KEYCODE_DPAD_LEFT}.");
}

CARAPI_(void) CWebView::DoTrackball(
    /* [in] */ Int64 time)
{
    Int32 elapsed = (Int32) (mTrackballLastTime - mTrackballFirstTime);
    if (elapsed == 0) {
        elapsed = TRACKBALL_TIMEOUT;
    }

    Float xRate = mTrackballRemainsX * 1000 / elapsed;
    Float yRate = mTrackballRemainsY * 1000 / elapsed;
    Int32 viewWidth = GetViewWidth();
    Int32 viewHeight = GetViewHeight();
    if (mSelectingText) {
        if (!mDrawSelectionPointer) {
            // The last selection was made by touch, disabling drawing the
            // selection pointer. Allow the trackball to adjust the
            // position of the touch control.
            mSelectX = ContentToViewX(NativeSelectionX());
            mSelectY = ContentToViewY(NativeSelectionY());
            mDrawSelectionPointer = mExtendSelection = TRUE;
            NativeSetExtendSelection();
        }

        MoveSelection(ScaleTrackballX(xRate, viewWidth),
                ScaleTrackballY(yRate, viewHeight));
        mTrackballRemainsX = mTrackballRemainsY = 0;
        return;
    }

    Float ax = /*Math.abs(xRate)*/ xRate < 0 ? -xRate : xRate;
    Float ay = /*Math.abs(yRate)*/ yRate < 0 ? -yRate : yRate;
    Float maxA = /*Math.max(ax, ay)*/ ax > ay ? ax : ay;
#if 0
    if (DebugFlags.WEB_VIEW) {
        Log.v(LOGTAG, "doTrackball elapsed=" + elapsed
                + " xRate=" + xRate
                + " yRate=" + yRate
                + " mTrackballRemainsX=" + mTrackballRemainsX
                + " mTrackballRemainsY=" + mTrackballRemainsY);
    }
#endif
    Int32 width = mContentWidth - viewWidth;
    Int32 height = mContentHeight - viewHeight;
    if (width < 0) width = 0;
    if (height < 0) height = 0;
    //ax = Math.abs(mTrackballRemainsX * TRACKBALL_MULTIPLIER);
    ax = mTrackballRemainsX * TRACKBALL_MULTIPLIER;
    ax = ax < 0 ? -ax : ax;
    //ay = Math.abs(mTrackballRemainsY * TRACKBALL_MULTIPLIER);
    ay = mTrackballRemainsY * TRACKBALL_MULTIPLIER;
    ay = ay < 0 ? -ay : ay;
    maxA = /*Math.max(ax, ay)*/ax > ay ? ax : ay;
    Int32 count = /*Math.max(0, (int) maxA)*/0 > (Int32) maxA ? 0 : (Int32) maxA;
    Int32 oldScrollX = mScrollX;
    Int32 oldScrollY = mScrollY;
    if (count > 0) {
        Int32 selectKeyCode = ax < ay ? mTrackballRemainsY < 0 ?
                KeyEvent_KEYCODE_DPAD_UP : KeyEvent_KEYCODE_DPAD_DOWN :
                mTrackballRemainsX < 0 ? KeyEvent_KEYCODE_DPAD_LEFT :
                KeyEvent_KEYCODE_DPAD_RIGHT;
        count = /*Math.min(count, TRACKBALL_MOVE_COUNT)*/count < TRACKBALL_MOVE_COUNT ? count : TRACKBALL_MOVE_COUNT;
        
        if (DebugFlags::sWEB_VIEW) {
//            Logger::V(LOGTAG, "doTrackball keyCode=%d count=%d mTrackballRemainsX=%d mTrackballRemainsY=%d" + selectKeyCode
//                    + count + mTrackballRemainsX + mTrackballRemainsY);
        }
        
        if (mNativeClass != 0 && NativeFocusIsPlugin()) {
            for (Int32 i = 0; i < count; i++) {
                LetPluginHandleNavKey(selectKeyCode, time, TRUE);
            }

            LetPluginHandleNavKey(selectKeyCode, time, FALSE);
        } else if (NavHandledKey(selectKeyCode, count, FALSE, time)) {
            PlaySoundEffect(KeyCodeToSoundsEffect(selectKeyCode));
        }

        mTrackballRemainsX = mTrackballRemainsY = 0;
    }
    if (count >= TRACKBALL_SCROLL_COUNT) {
        Int32 xMove = ScaleTrackballX(xRate, width);
        Int32 yMove = ScaleTrackballY(yRate, height);
#if 0        
        if (DebugFlags.WEB_VIEW) {
            Log.v(LOGTAG, "doTrackball pinScrollBy"
                    + " count=" + count
                    + " xMove=" + xMove + " yMove=" + yMove
                    + " mScrollX-oldScrollX=" + (mScrollX-oldScrollX)
                    + " mScrollY-oldScrollY=" + (mScrollY-oldScrollY)
                    );
        }
#endif
        Int32 abs1, abs2;
        abs1 = mScrollX - oldScrollX;
        abs1 = abs1 < 0 ? -abs1 : abs1;
        abs2 = xMove < 0 ? -xMove : xMove;
        if (/*Math.abs(mScrollX - oldScrollX) > Math.abs(xMove)*/abs1 > abs2) {
            xMove = 0;
        }

        abs1 = mScrollY - oldScrollY;
        abs1 = abs1 < 0 ? -abs1 : abs1;
        abs2 = yMove < 0 ? -yMove : yMove;
        if (/*Math.abs(mScrollY - oldScrollY) > Math.abs(yMove)*/abs1 > abs2) {
            yMove = 0;
        }

        if (xMove != 0 || yMove != 0) {
            PinScrollBy(xMove, yMove, TRUE, 0);
        }

        mUserScroll = TRUE;
    }
}

CARAPI_(Int32) CWebView::ComputeMaxScrollX()
{
//    return Math.max(computeRealHorizontalScrollRange() - getViewWidth(), 0);
    Int32 max = ComputeRealHorizontalScrollRange() - GetViewWidth();
    max = max > 0 ? max : 0;

    return max;
}

CARAPI_(Int32) CWebView::ComputeMaxScrollY()
{
//    return Math.max(computeRealVerticalScrollRange() + getTitleHeight() - getViewHeightWithTitle(), 0);

    Int32 max = ComputeRealVerticalScrollRange() + GetTitleHeight() - GetViewHeightWithTitle();
    max = max > 0 ? max : 0;

    return max;
}

CARAPI_(void) CWebView::DoFling()
{
    if (mVelocityTracker == NULL) {
        return;
    }

    Int32 maxX = ComputeMaxScrollX();
    Int32 maxY = ComputeMaxScrollY();

    mVelocityTracker->ComputeCurrentVelocity(1000, mMaximumFling);
    Int32 vx = (Int32) mVelocityTracker->GetXVelocity();
    Int32 vy = (Int32) mVelocityTracker->GetYVelocity();

    if (mSnapScrollMode != SNAP_NONE) {
        if ((mSnapScrollMode & SNAP_X) == SNAP_X) {
            vy = 0;
        } else {
            vx = 0;
        }
    }

    if (TRUE /* EMG release: make our fling more like Maps' */) {
        // maps cuts their velocity in half
        vx = vx * 3 / 4;
        vy = vy * 3 / 4;
    }

    if ((maxX == 0 && vy == 0) || (maxY == 0 && vx == 0)) {
        WebViewCore::ResumePriority();
        WebViewCore::ResumeUpdatePicture(mWebViewCore);
        if (mScroller->SpringBack(mScrollX, mScrollY, 0, ComputeMaxScrollX(),
                0, ComputeMaxScrollY())) {
            InvalidateEx2();
        }
        return;
    }

    Float currentVelocity = mScroller->GetCurrVelocity();
    if (mLastVelocity > 0 && currentVelocity > 0) {
        Float deltaR;// = (float) (Math.abs(Math.atan2(mLastVelY, mLastVelX) - Math.atan2(vy, vx)));
        /*const*/ Float circle;// = (float) (Math.PI) * 2.0f;
        if (deltaR > circle * 0.9f || deltaR < circle * 0.1f) {
            vx += currentVelocity * mLastVelX / mLastVelocity;
            vy += currentVelocity * mLastVelY / mLastVelocity;
           
            if (DebugFlags::sWEB_VIEW) {
                Logger::V(LOGTAG, "doFling vx= %d vy=%d" + vx + vy);
            }
        } else if (DebugFlags::sWEB_VIEW) {
//            Logger::V(LOGTAG, "doFling missed %d" + deltaR / circle);      
        }     
    } else if (DebugFlags::sWEB_VIEW) {
//        Logger::V(LOGTAG, "doFling start last=%d current=%d vx=%d vy=%d maxX=%d maxY=%d mScrollX=%d mScrollY=%d"
//                + mLastVelocity + currentVelocity + vx  + vy
//                + maxX + maxY + mScrollX + mScrollY);          
    }

    // Allow sloppy flings without overscrolling at the edges.
    Int32 vxAbs = vx < 0 ? -vx : 0;
    Int32 vyAbs = vy < 0 ? -vy : 0;
    if ((mScrollX == 0 || mScrollX == maxX) && vxAbs < vyAbs) {
        vx = 0;
    }
    if ((mScrollY == 0 || mScrollY == maxY) && vyAbs < vxAbs) {
        vy = 0;
    }

    if (mOverscrollDistance < mOverflingDistance) {
        if (mScrollX == -mOverscrollDistance || mScrollX == maxX + mOverscrollDistance) {
            vx = 0;
        }
        if (mScrollY == -mOverscrollDistance || mScrollY == maxY + mOverscrollDistance) {
            vy = 0;
        }
    }

    mLastVelX = vx;
    mLastVelY = vy;
//    mLastVelocity = (float) Math.hypot(vx, vy);

    // no horizontal overscroll if the content just fits
    mScroller->Fling(mScrollX, mScrollY, -vx, -vy, 0, maxX, 0, maxY,
            maxX == 0 ? 0 : mOverflingDistance, mOverflingDistance);
    // Duration is calculated based on velocity. With range boundaries and overscroll
    // we may not know how long the final animation will take. (Hence the deprecation
    // warning on the call below.) It's not a big deal for scroll bars but if webcore
    // resumes during this effect we will take a performance hit. See computeScroll;
    // we resume webcore there when the animation is finished.
    const Int32 time = mScroller->GetDuration();
    AwakenScrollBars(time);
    InvalidateEx2();
}

CARAPI_(Boolean) CWebView::ZoomWithPreview(
    /* [in] */ Float scale,
    /* [in] */ Boolean updateTextWrapScale)
{
    Float oldScale = mActualScale;
    mInitialScrollX = mScrollX;
    mInitialScrollY = mScrollY;

    // snap to DEFAULT_SCALE if it is close
    Int32 abs = scale - mDefaultScale;
    abs = abs < 0 ? -abs : abs;
    if (/*Math.abs(scale - mDefaultScale)*/abs < MINIMUM_SCALE_INCREMENT) {
        scale = mDefaultScale;
    }

    SetNewZoomScale(scale, updateTextWrapScale, FALSE);

    if (oldScale != mActualScale) {
        // use mZoomPickerScale to see zoom preview first
//        mZoomStart = SystemClock.uptimeMillis();
        mInvInitialZoomScale = 1.0f / oldScale;
        mInvFinalZoomScale = 1.0f / mActualScale;
        mZoomScale = mActualScale;
        WebViewCore::PauseUpdatePicture(mWebViewCore);
        InvalidateEx2();
        return TRUE;
    } else {
        return FALSE;
    }
}

CARAPI_(AutoPtr<CWebView::ExtendedZoomControls>) CWebView::CreateZoomControls()
{}

CARAPI_(void) CWebView::UpdateSelection()
{
    if (mNativeClass == 0) {
        return;
    }
    // mLastTouchX and mLastTouchY are the point in the current viewport
    Int32 contentX = ViewToContentX((Int32) mLastTouchX + mScrollX);
    Int32 contentY = ViewToContentY((Int32) mLastTouchY + mScrollY);
    AutoPtr<IRect> rect;
    CRect::New(contentX - mNavSlop, contentY - mNavSlop,
            contentX + mNavSlop, contentY + mNavSlop, (IRect**)&rect);
    NativeSelectBestAt(rect);
}

CARAPI_(void) CWebView::DoShortPress()
{
    if (mNativeClass == 0) {
        return;
    }

    if (mPreventDefault == PREVENT_DEFAULT_YES) {
        return;
    }

    mTouchMode = TOUCH_DONE_MODE;
    SwitchOutDrawHistory();

    // mLastTouchX and mLastTouchY are the point in the current viewport
    Int32 contentX = ViewToContentX((Int32) mLastTouchX + mScrollX);
    Int32 contentY = ViewToContentY((Int32) mLastTouchY + mScrollY);
    if (NativePointInNavCache(contentX, contentY, mNavSlop)) {
        WebViewCore::MotionUpData* motionUpData = new WebViewCore::MotionUpData();
        motionUpData->mFrame = NativeCacheHitFramePointer();
        motionUpData->mNode = NativeCacheHitNodePointer();
        motionUpData->mBounds = NativeCacheHitNodeBounds();
        motionUpData->mX = contentX;
        motionUpData->mY = contentY;
        mWebViewCore->SendMessageAtFrontOfQueue(WebViewCore::EventHub::VALID_NODE_BOUNDS,
                (IInterface*)motionUpData);
    } else {
        DoMotionUp(contentX, contentY);
    }
}

CARAPI_(void) CWebView::DoMotionUp(
    /* [in] */ Int32 contentX,
    /* [in] */ Int32 contentY)
{
    if (mLogEvent && NativeMotionUp(contentX, contentY, mNavSlop)) {
//        EventLog.writeEvent(EventLogTags.BROWSER_SNAP_CENTER);
    }

    if (NativeHasCursorNode() && !NativeCursorIsTextInput()) {
        PlaySoundEffect(SoundEffectConstants_CLICK);
    }
}

/*
 * Return true if the view (Plugin) is fully visible and maximized inside
 * the WebView.
 */
CARAPI_(Boolean) CWebView::IsPluginFitOnScreen(
    /* [in] */ ViewManager::ChildView* view)
{
    assert (view != NULL);

    Int32 viewWidth = GetViewWidth();
    Int32 viewHeight = GetViewHeightWithTitle();
    Float scale = /*Math.min*/((Float) viewWidth / view->mWidth < (Float) viewHeight / view->mHeight 
              ? (Float) viewWidth / view->mWidth : (Float) viewHeight / view->mHeight);
    if (scale < mMinZoomScale) {
        scale = mMinZoomScale;
    } else if (scale > mMaxZoomScale) {
        scale = mMaxZoomScale;
    }

    Int32 abs = scale - mActualScale;
    abs = abs < 0 ? -abs : abs;
    if (/*Math.abs(scale - mActualScale)*/abs < MINIMUM_SCALE_INCREMENT) {
        if (ContentToViewX(view->mX) >= mScrollX
                && ContentToViewX(view->mX + view->mWidth) <= mScrollX
                        + viewWidth
                && ContentToViewY(view->mY) >= mScrollY
                && ContentToViewY(view->mY + view->mHeight) <= mScrollY
                        + viewHeight) {
            return TRUE;
        }
    }

    return FALSE;
}

/*
 * Maximize and center the rectangle, specified in the document coordinate
 * space, inside the WebView. If the zoom doesn't need to be changed, do an
 * animated scroll to center it. If the zoom needs to be changed, find the
 * zoom center and do a smooth zoom transition.
 */
CARAPI_(void) CWebView::CenterFitRect(
    /* [in] */ Int32 docX,
    /* [in] */ Int32 docY,
    /* [in] */ Int32 docWidth,
    /* [in] */ Int32 docHeight)
{
    Int32 viewWidth = GetViewWidth();
    Int32 viewHeight = GetViewHeightWithTitle();
    Float scale = /*Math.min*/
          ((Float) viewWidth/docWidth < (Float)viewHeight/docHeight
            ? (Float)viewWidth/docWidth : (Float)viewHeight/docHeight);
    if (scale < mMinZoomScale) {
        scale = mMinZoomScale;
    } else if (scale > mMaxZoomScale) {
        scale = mMaxZoomScale;
    }

    Float abs = scale - mActualScale;
    abs = abs < 0 ? -abs : abs;
    if (/*Math.abs(scale - mActualScale)*/abs < MINIMUM_SCALE_INCREMENT) {
        PinScrollTo(ContentToViewX(docX + docWidth / 2) - viewWidth / 2,
                ContentToViewY(docY + docHeight / 2) - viewHeight / 2,
                TRUE, 0);
    } else {
        Float oldScreenX = docX * mActualScale - mScrollX;
        Float rectViewX = docX * scale;
        Float rectViewWidth = docWidth * scale;
        Float newMaxWidth = mContentWidth * scale;
        Float newScreenX = (viewWidth - rectViewWidth) / 2;
        // pin the newX to the WebView
        if (newScreenX > rectViewX) {
            newScreenX = rectViewX;
        } else if (newScreenX > (newMaxWidth - rectViewX - rectViewWidth)) {
            newScreenX = viewWidth - (newMaxWidth - rectViewX);
        }
        mZoomCenterX = (oldScreenX * scale - newScreenX * mActualScale)
                / (scale - mActualScale);
        Float oldScreenY = docY * mActualScale + GetTitleHeight()
                - mScrollY;
        Float rectViewY = docY * scale + GetTitleHeight();
        Float rectViewHeight = docHeight * scale;
        Float newMaxHeight = mContentHeight * scale + GetTitleHeight();
        Float newScreenY = (viewHeight - rectViewHeight) / 2;
        // pin the newY to the WebView
        if (newScreenY > rectViewY) {
            newScreenY = rectViewY;
        } else if (newScreenY > (newMaxHeight - rectViewY - rectViewHeight)) {
            newScreenY = viewHeight - (newMaxHeight - rectViewY);
        }

        mZoomCenterY = (oldScreenY * scale - newScreenY * mActualScale)
                / (scale - mActualScale);
        ZoomWithPreview(scale, FALSE);
    }
}

// Rule for double tap:
// 1. if the current scale is not same as the text wrap scale and layout
//    algorithm is NARROW_COLUMNS, fit to column;
// 2. if the current state is not overview mode, change to overview mode;
// 3. if the current state is overview mode, change to default scale.
CARAPI_(void) CWebView::DoDoubleTap()
{
    if (mWebViewCore->GetSettings()->GetUseWideViewPort() == FALSE) {
        return;
    }

    mZoomCenterX = mLastTouchX;
    mZoomCenterY = mLastTouchY;
    mAnchorX = ViewToContentX((Int32) mZoomCenterX + mScrollX);
    mAnchorY = ViewToContentY((Int32) mZoomCenterY + mScrollY);
    AutoPtr<IWebSettings> settings;
    GetSettings((IWebSettings**)&settings);
    assert(settings.Get());
    settings->SetDoubleTapToastCount(0);
    // remove the zoom control after double tap
    DismissZoomControl();
    ViewManager::ChildView* plugin = mViewManager->HitTest(mAnchorX, mAnchorY);
    if (plugin != NULL) {
        if (IsPluginFitOnScreen(plugin)) {
            mInZoomOverview = TRUE;
            // Force the titlebar fully reveal in overview mode
            if (mScrollY < GetTitleHeight()) mScrollY = 0;
            ZoomWithPreview((Float) GetViewWidth() / mZoomOverviewWidth,
                    TRUE);
        } else {
            mInZoomOverview = FALSE;
            CenterFitRect(plugin->mX, plugin->mY, plugin->mWidth, plugin->mHeight);
        }
        return;
    }

    Boolean zoomToDefault = FALSE;
    Int32 layoutAlgorithm = 0;
    settings->GetLayoutAlgorithm(&layoutAlgorithm);
    Int32 abs = mActualScale - mTextWrapScale;
    abs = abs < 0 ? -abs : abs;
    if ((layoutAlgorithm == WebSettings::NARROW_COLUMNS)
            && (/*Math.abs(mActualScale - mTextWrapScale)*/abs >= MINIMUM_SCALE_INCREMENT)) {
        
        SetNewZoomScale(mActualScale, TRUE, TRUE);
        Float overviewScale = (Float)GetViewWidth() / mZoomOverviewWidth;

        abs = mActualScale - overviewScale;
        abs = abs < 0 ? -abs : abs;
        if (/*Math.abs(mActualScale - overviewScale)*/abs < MINIMUM_SCALE_INCREMENT) {
            mInZoomOverview = TRUE;
        }
    } else if (!mInZoomOverview) {
        Float newScale = (Float)GetViewWidth()/mZoomOverviewWidth;
        abs = mActualScale - newScale;
        abs = abs < 0 ? -abs : abs;
        Int32 abs2 = mActualScale - mDefaultScale;
        abs2 = abs2 < 0 ? -abs2 : abs2;
        if (/*Math.abs(mActualScale - newScale)*/abs >= MINIMUM_SCALE_INCREMENT) {
            mInZoomOverview = TRUE;
            // Force the titlebar fully reveal in overview mode
            if (mScrollY < GetTitleHeight()) mScrollY = 0;
            ZoomWithPreview(newScale, TRUE);
        } else if (/*Math.abs(mActualScale - mDefaultScale)*/abs2 >= MINIMUM_SCALE_INCREMENT) {
            zoomToDefault = TRUE;
        }
    } else {
        zoomToDefault = TRUE;
    }

    if (zoomToDefault) {
        mInZoomOverview = FALSE;
        Int32 left = NativeGetBlockLeftEdge(mAnchorX, mAnchorY, mActualScale);
        if (left != NO_LEFTEDGE) {
            // add a 5pt padding to the left edge.
            Int32 viewLeft = ContentToViewX(left < 5 ? 0 : (left - 5))
                    - mScrollX;
            // Re-calculate the zoom center so that the new scroll x will be
            // on the left edge.
            if (viewLeft > 0) {
                mZoomCenterX = viewLeft * mDefaultScale
                        / (mDefaultScale - mActualScale);
            } else {
                ScrollBy(viewLeft, 0);
                mZoomCenterX = 0;
            }
        }

        ZoomWithPreview(mDefaultScale, TRUE);
    }
}

// Called by JNI to handle a touch on a node representing an email address,
// address, or phone number
CARAPI_(void) CWebView::OverrideLoading(
    /* [in] */ const String& url)
{
    mCallbackProxy->UiOverrideUrlLoading(url, NULL);
}

/**
 * Used when receiving messages for REQUEST_KEYBOARD_WITH_SELECTION_MSG_ID
 * and UPDATE_TEXT_SELECTION_MSG_ID.  Update the selection of WebTextView.
 */
CARAPI_(void) CWebView::UpdateTextSelectionFromMessage(
    /* [in] */ Int32 nodePointer,
    /* [in] */ Int32 textGeneration,
    /* [in] */ WebViewCore::TextSelectionData* data)
{
    assert(data != NULL);

    if (InEditingMode()
            && mWebTextView->IsSameTextField(nodePointer)
            && textGeneration == mTextGeneration) {
        mWebTextView->SetSelectionFromWebKit(data->mStart, data->mEnd);
    }
}


CARAPI_(void) CWebView::UpdateZoomRange(
    /* [in] */ WebViewCore::RestoreState* restoreState,
    /* [in] */ Int32 viewWidth,
    /* [in] */ Int32 minPrefWidth,
    /* [in] */ Boolean updateZoomOverview)
{
    assert(restoreState != NULL);

    if (restoreState->mMinScale == 0) {
        if (restoreState->mMobileSite) {
            Int32 max = 0 > viewWidth ? 0 : viewWidth;
            if (minPrefWidth > /*Math.max(0, viewWidth)*/max) {
                mMinZoomScale = (Float) viewWidth / minPrefWidth;
                mMinZoomScaleFixed = FALSE;
                if (updateZoomOverview) {
                    AutoPtr<IWebSettings> settings;
                    GetSettings((IWebSettings**)&settings);
                    assert(settings.Get());
                    Boolean bFlag1 = FALSE, bFlag2 = FALSE;
                    settings->GetUseWideViewPort(&bFlag1);
                    settings->GetLoadWithOverviewMode(&bFlag2);
                    mInZoomOverview = bFlag1 && bFlag2;
                }
            } else {
                mMinZoomScale = restoreState->mDefaultScale;
                mMinZoomScaleFixed = TRUE;
            }
        } else {
            mMinZoomScale = DEFAULT_MIN_ZOOM_SCALE;
            mMinZoomScaleFixed = FALSE;
        }
    } else {
        mMinZoomScale = restoreState->mMinScale;
        mMinZoomScaleFixed = TRUE;
    }
    if (restoreState->mMaxScale == 0) {
        mMaxZoomScale = DEFAULT_MAX_ZOOM_SCALE;
    } else {
        mMaxZoomScale = restoreState->mMaxScale;
    }
}

// called by JNI
CARAPI_(void) CWebView::SendMoveFocus(
    /* [in] */ Int32 frame,
    /* [in] */ Int32 node)
{
    mWebViewCore->SendMessage(WebViewCore::EventHub::SET_MOVE_FOCUS,
                (IInterface*)(new WebViewCore::CursorData(frame, node, 0, 0)));
}

// called by JNI
CARAPI_(void) CWebView::SendMoveMouse(
    /* [in] */ Int32 frame,
    /* [in] */ Int32 node,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    mWebViewCore->SendMessage(WebViewCore::EventHub::SET_MOVE_MOUSE,
                (IInterface*)(new WebViewCore::CursorData(frame, node, x, y)));
}

/*
 * Send a mouse move event to the webcore thread.
 *
 * @param removeFocus Pass true if the "mouse" cursor is now over a node
 *                    which wants key events, but it is not the focus. This
 *                    will make the visual appear as though nothing is in
 *                    focus.  Remove the WebTextView, if present, and stop
 *                    drawing the blinking caret.
 * called by JNI
 */
CARAPI_(void) CWebView::SendMoveMouseIfLatest(
    /* [in] */ Boolean removeFocus)
{
    if (removeFocus) {
        ClearTextEntry(TRUE);
    }
    mWebViewCore->SendMessage(WebViewCore::EventHub::SET_MOVE_MOUSE_IF_LATEST,
            (IInterface*)CursorData());
}

// called by JNI
CARAPI_(void) CWebView::SendMotionUp(
    /* [in] */ Int32 touchGeneration,
    /* [in] */ Int32 frame,
    /* [in] */ Int32 node,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    WebViewCore::TouchUpData* touchUpData = new WebViewCore::TouchUpData();
    touchUpData->mMoveGeneration = touchGeneration;
    touchUpData->mFrame = frame;
    touchUpData->mNode = node;
    touchUpData->mX = x;
    touchUpData->mY = y;
    mWebViewCore->SendMessage(WebViewCore::EventHub::TOUCH_UP, (IInterface*)touchUpData);
}

CARAPI_(Int32) CWebView::GetScaledMaxXScroll()
{
    Int32 width;
    if (mHeightCanMeasure == FALSE) {
        width = GetViewWidth() / 4;
    } else {
        AutoPtr<IRect> visRect;
        CRect::New((IRect**)&visRect);
        assert(visRect.Get());
        CalcOurVisibleRect(visRect);
        //width = visRect.width() / 2;
        visRect->GetWidth(&width);
        width = width / 2;
    }
    // FIXME the divisor should be retrieved from somewhere
    return ViewToContentX(width);
}

CARAPI_(Int32) CWebView::GetScaledMaxYScroll()
{
    Int32 height;
    if (mHeightCanMeasure == FALSE) {
        height = GetViewHeight() / 4;
    } else {
        AutoPtr<IRect> visRect;
        CRect::New((IRect**)&visRect);
        assert(visRect.Get());
        CalcOurVisibleRect(visRect);
        //height = visRect.height() / 2;
        visRect->GetHeight(&height);
        height = height / 2;
    }

    // FIXME the divisor should be retrieved from somewhere
    // the closest thing today is hard-coded into ScrollView.java
    // (from ScrollView.java, line 363)   int maxJump = height/2;
    return /*Math.round*/(height * mInvActualScale);
}

/**
 * Called by JNI to invalidate view
 */
CARAPI_(void) CWebView::ViewInvalidate()
{
    InvalidateEx2();
}

/**
 * Pass the key to the plugin.  This assumes that nativeFocusIsPlugin()
 * returned true.
 */
CARAPI_(void) CWebView::LetPluginHandleNavKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int64 time,
    /* [in] */ Boolean down)
{
    Int32 keyEventAction;
    Int32 eventHubAction;
    if (down) {
        keyEventAction = KeyEvent_ACTION_DOWN;
        eventHubAction = WebViewCore::EventHub::KEY_DOWN;
        PlaySoundEffect(KeyCodeToSoundsEffect(keyCode));
    } else {
        keyEventAction = KeyEvent_ACTION_UP;
        eventHubAction = WebViewCore::EventHub::KEY_UP;
    }

    AutoPtr<IKeyEvent> event;
    CKeyEvent::New(time, time, keyEventAction, keyCode,
            1, (mShiftIsPressed ? KeyEvent_META_SHIFT_ON : 0)
            | (false ? KeyEvent_META_ALT_ON : 0) // FIXME
            | (false ? KeyEvent_META_SYM_ON : 0) // FIXME
            , 0, 0, 0, (IKeyEvent**)&event);

    mWebViewCore->SendMessage(eventHubAction, (IInterface*)event);
}

// return true if the key was handled
CARAPI_(Boolean) CWebView::NavHandledKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 count,
    /* [in] */ Boolean noScroll,
    /* [in] */ Int64 time)
{
    if (mNativeClass == 0) {
        return FALSE;
    }
    mLastCursorTime = time;
    mLastCursorBounds = NativeGetCursorRingBounds();
    Boolean keyHandled
            = NativeMoveCursor(keyCode, count, noScroll) == FALSE;
    
    if (DebugFlags::sWEB_VIEW) {
        Logger::V(LOGTAG, "navHandledKey mLastCursorBounds=%d mLastCursorTime=%d handled=%d"
                + mLastCursorBounds + mLastCursorTime + keyHandled);
    }

    if (keyHandled == FALSE || mHeightCanMeasure == FALSE) {
        return keyHandled;
    }

    AutoPtr<IRect> contentCursorRingBounds = NativeGetCursorRingBounds();
    Boolean bFlag = FALSE;
    contentCursorRingBounds->IsEmpty(&bFlag);
    if (bFlag) return keyHandled;
    AutoPtr<IRect> viewCursorRingBounds = ContentToViewRect(contentCursorRingBounds);
    AutoPtr<IRect> visRect;
    CRect::New((IRect**)&visRect);
    CalcOurVisibleRect(visRect);
    AutoPtr<IRect> outset;
    CRect::New(visRect, (IRect**)&outset);
    Int32 width, height;
    visRect->GetWidth(&width);
    visRect->GetHeight(&height);
    Int32 maxXScroll = width / 2;
    Int32 maxYScroll = height / 2;
    outset->Inset(-maxXScroll, -maxYScroll);
    visRect->IntersectsEx(outset, viewCursorRingBounds, &bFlag);
    if (bFlag == FALSE) {
        return keyHandled;
    }
    // FIXME: Necessary because ScrollView/ListView do not scroll left/right
    //int maxH = Math.min(viewCursorRingBounds.right - visRect.right, maxXScroll);
    Int32 right1, right2;
    viewCursorRingBounds->GetRight(&right1);
    visRect->GetRight(&right2);
    Int32 maxH = right1 - right2;
    maxH = maxH < maxXScroll ? maxH : maxXScroll;
    if (maxH > 0) {
        PinScrollBy(maxH, 0, TRUE, 0);
    } else {
        Int32 left1, left2;
        viewCursorRingBounds->GetLeft(&left1);
        visRect->GetLeft(&left2);
        maxH = left1 - left2;
        maxH = maxH > -maxXScroll ? maxH : -maxXScroll;
        //maxH = Math.max(viewCursorRingBounds.left - visRect.left, -maxXScroll);
        if (maxH < 0) {
            PinScrollBy(maxH, 0, TRUE, 0);
        }
    }

    mLastCursorBounds->IsEmpty(&bFlag);
    if (bFlag) return keyHandled;

    mLastCursorBounds->Equals(contentCursorRingBounds, &bFlag);
    if (bFlag) {
        return keyHandled;
    }

    if (DebugFlags::sWEB_VIEW) {
        Logger::V(LOGTAG, "navHandledKey contentCursorRingBounds=%d"
                + contentCursorRingBounds);
    }

    RequestRectangleOnScreen(viewCursorRingBounds, NULL);
    mUserScroll = TRUE;

    return keyHandled;
}

CARAPI_(Int32) CWebView::NativeCacheHitFramePointer()
{}

CARAPI_(AutoPtr<IRect>) CWebView::NativeCacheHitNodeBounds()
{}

CARAPI_(Int32) CWebView::NativeCacheHitNodePointer()
{}

CARAPI_(void) CWebView::NativeCreate(
    /* [in] */ Int32 ptr)
{}

CARAPI_(Int32) CWebView::NativeCursorFramePointer()
{}

CARAPI_(AutoPtr<IRect>) CWebView::NativeCursorNodeBounds()
{}

CARAPI_(Int32) CWebView::NativeCursorNodePointer()
{}

CARAPI_(Boolean) CWebView::NativeCursorIntersects(
    /* [in] */ IRect* visibleRect)
{}

CARAPI_(Boolean) CWebView::NativeCursorIsAnchor()
{}

CARAPI_(Boolean) CWebView::NativeCursorIsTextInput()
{}

CARAPI_(AutoPtr<IPoint>) CWebView::NativeCursorPosition()
{}

CARAPI_(String) CWebView::NativeCursorText()
{}

/**
 * Returns true if the native cursor node says it wants to handle key events
 * (ala plugins). This can only be called if mNativeClass is non-zero!
 */
CARAPI_(Boolean) CWebView::NativeCursorWantsKeyEvents()
{}

CARAPI_(void) CWebView::NativeDebugDump()
{}

CARAPI_(void) CWebView::NativeDestroy()
{}

CARAPI_(Boolean) CWebView::NativeEvaluateLayersAnimations()
{}

CARAPI_(void) CWebView::NativeExtendSelection(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{}

CARAPI_(void) CWebView::NativeDrawExtras(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Int32 extra)
{}

CARAPI_(void) CWebView::NativeDumpDisplayTree(
    /* [in] */ const String& urlOrNull)
{}

CARAPI_(Int32) CWebView::NativeFindAll(
    /* [in] */ const String& findLower,
    /* [in] */ const String& findUpper)
{}

CARAPI_(void) CWebView::NativeFindNext(
    /* [in] */ Boolean forward)
{}

CARAPI_(Boolean) CWebView::NativeFocusCandidateIsRtlText()
{}

CARAPI_(Boolean) CWebView::NativeFocusCandidateIsTextInput()
{}

CARAPI_(AutoPtr<IRect>) CWebView::NativeFocusCandidateNodeBounds()
{}

/**
 * @return A Rect with left, top, right, bottom set to the corresponding
 * padding values in the focus candidate, if it is a textfield/textarea with
 * a style.  Otherwise return null.  This is not actually a rectangle; Rect
 * is being used to pass four integers.
 */
CARAPI_(AutoPtr<IRect>) CWebView::NativeFocusCandidatePaddingRect()
{}

CARAPI_(String) CWebView::NativeFocusCandidateText()
{}

CARAPI_(Int32) CWebView::NativeFocusCandidateTextSize()
{}

/**
 * Returns an integer corresponding to WebView.cpp::type.
 * See WebTextView.setType()
 */
CARAPI_(Int32) CWebView::NativeFocusCandidateType()
{}

CARAPI_(Boolean) CWebView::NativeFocusIsPlugin()
{}

CARAPI_(AutoPtr<IRect>) CWebView::NativeFocusNodeBounds()
{}

CARAPI_(AutoPtr<IRect>) CWebView::NativeGetCursorRingBounds()
{}

CARAPI_(String) CWebView::NativeGetSelection()
{}

CARAPI_(Boolean) CWebView::NativeHasCursorNode()
{}

CARAPI_(Boolean) CWebView::NativeHasFocusNode()
{}

CARAPI_(void) CWebView::NativeHideCursor()
{}

CARAPI_(Boolean) CWebView::NativeHitSelection(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{}

CARAPI_(String) CWebView::NativeImageURI(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{}

CARAPI_(void) CWebView::NativeInstrumentReport()
{}

// return true if the page has been scrolled
CARAPI_(Boolean) CWebView::NativeMotionUp(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 slop)
{}

// returns false if it handled the key
CARAPI_(Boolean)  CWebView::NativeMoveCursor(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 count,
    /* [in] */ Boolean noScroll)
{}

CARAPI_(Int32) CWebView::NativeMoveGeneration()
{}

CARAPI_(void) CWebView::NativeMoveSelection(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{}

CARAPI_(Boolean) CWebView::NativePointInNavCache(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 slop)
{}

// Like many other of our native methods, you must make sure that
// mNativeClass is not null before calling this method.
CARAPI_(void) CWebView::NativeRecordButtons(
    /* [in] */ Boolean focused,
    /* [in] */ Boolean pressed,
    /* [in] */ Boolean invalidate)
{}

CARAPI_(void) CWebView::NativeResetSelection()
{}

CARAPI_(void) CWebView::NativeSelectAll()
{}

CARAPI_(void) CWebView::NativeSelectBestAt(
    /* [in] */ IRect* rect)
{}

CARAPI_(Int32) CWebView::NativeSelectionX()
{}

CARAPI_(Int32) CWebView::NativeSelectionY()
{}

CARAPI_(Int32) CWebView::NativeFindIndex()
{}

CARAPI_(void) CWebView::NativeSetExtendSelection()
{}

CARAPI_(void) CWebView::NativeSetFindIsEmpty()
{}

CARAPI_(void) CWebView::NativeSetFindIsUp(
    /* [in] */ Boolean isUp)
{}

CARAPI_(void) CWebView::NativeSetFollowedLink(
    /* [in] */ Boolean followed)
{}

CARAPI_(void) CWebView::NativeSetHeightCanMeasure(
    /* [in] */ Boolean measure)
{}

CARAPI_(void) CWebView::NativeSetRootLayer(
    /* [in] */ Int32 layer)
{}

CARAPI_(void) CWebView::NativeSetSelectionPointer(
    /* [in] */ Boolean set,
    /* [in] */ Float scale,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{}

CARAPI_(Boolean) CWebView::NativeStartSelection(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{}

CARAPI_(void) CWebView::NativeSetSelectionRegion(
    /* [in] */ Boolean set)
{}

CARAPI_(AutoPtr<IRect>) CWebView::NativeSubtractLayers(
    /* [in] */ IRect* content)
{}

CARAPI_(Int32) CWebView::NativeTextGeneration()
{}

// Never call this version except by updateCachedTextfield(String) -
// we always want to pass in our generation number.
CARAPI_(void) CWebView::NativeUpdateCachedTextfield(
    /* [in] */ const String& updatedText,
    /* [in] */ Int32 generation)
{}

CARAPI_(Boolean) CWebView::NativeWordSelection(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{}

CARAPI_(Int32) CWebView::NativeGetBlockLeftEdge(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Float scale)
{}


/******************************HitTestResult*********************************/
CWebView::HitTestResult::HitTestResult() {
    mType = HitTestResult_UNKNOWN_TYPE;
}

CARAPI CWebView::HitTestResult::GetType(
   /* out */ Int32* type) {
    VALIDATE_NOT_NULL(type);
    *type = mType;
    return NOERROR;
}

CARAPI CWebView::HitTestResult::GetExtra(
   /* out */ String* extra) {
    VALIDATE_NOT_NULL(extra);
    *extra = mExtra;
    return NOERROR;
}

CARAPI_(PInterface) CWebView::HitTestResult::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

CARAPI_(UInt32) CWebView::HitTestResult::AddRef()
{
    return ElRefBase::AddRef();
}

CARAPI_(UInt32) CWebView::HitTestResult::Release()
{
    return ElRefBase::Release();
}

CARAPI CWebView::HitTestResult::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

CARAPI_(void) CWebView::HitTestResult::SetType(
    /* [in] */ Int32 type) {
    mType = type;
}

CARAPI_(void) CWebView::HitTestResult::SetExtra(
    /* [in] */ const String& extra) {
    mExtra = extra;
}

/*********************************DragTrackerHandler************************************/
CWebView::DragTrackerHandler::DragTrackerHandler(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IDragTracker* proxy)
{
    assert(proxy != NULL);

    mProxy = proxy;

    Int32 docBottom = webview->ComputeRealVerticalScrollRange() + webview->GetTitleHeight();
    Int32 viewTop = 0;
    webview->GetScrollY(&viewTop);
    Int32 height = 0;
    webview->GetHeight(&height);
    Int32 viewBottom = viewTop + height;

    mStartY = y;
    mMinDY = -viewTop;
    mMaxDY = docBottom - viewBottom;

    if (DebugFlags::sDRAG_TRACKER || DEBUG_DRAG_TRACKER) {
//        Logger::D(DebugFlags::sDRAG_TRACKER_LOGTAG, 
//              " dragtracker y= %d up/down= %d %d" + y
//               + mMinDY + mMaxDY);
    }

    Int32 docRight = webview->ComputeRealHorizontalScrollRange();
    Int32 viewLeft = 0;
    webview->GetScrollX(&viewLeft);
    Int32 width = 0;
    webview->GetWidth(&width);
    Int32 viewRight = viewLeft + width;
    mStartX = x;
    mMinDX = -viewLeft;
    mMaxDX = docRight - viewRight;

    mState = DRAGGING_STATE;
    mProxy->OnStartDrag(x, y);

    // ensure we buildBitmap at least once
    mSX = -99999;
}

CARAPI_(void) CWebView::DragTrackerHandler::DragTo(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    Float sy = ComputeStretch(mStartY - y, mMinDY, mMaxDY);
    Float sx = ComputeStretch(mStartX - x, mMinDX, mMaxDX);

    if ((webview->mSnapScrollMode & SNAP_X) != 0) {
        sy = 0;
    } else if ((webview->mSnapScrollMode & SNAP_Y) != 0) {
        sx = 0;
    }

    if (mCurrStretchX != sx || mCurrStretchY != sy) {
        mCurrStretchX = sx;
        mCurrStretchY = sy;
#if 0
        if (DebugFlags.DRAG_TRACKER || DEBUG_DRAG_TRACKER) {
            Log.d(DebugFlags.DRAG_TRACKER_LOGTAG, "---- stretch " + sx +
                  " " + sy);
        }
#endif
        Boolean bFlag = FALSE;
        mProxy->OnStretchChange(sx, sy, &bFlag);
        if (bFlag) {
            webview->InvalidateEx2();
        }
    }
}

CARAPI_(void) CWebView::DragTrackerHandler::StopDrag()
{}

// Call this after each draw. If it ruturns null, the tracker is done
CARAPI_(Boolean) CWebView::DragTrackerHandler::IsFinished()
{
    return mState == FINISHED_STATE;
}

/*  If the tracker draws, then this returns true, otherwise it will
    return false, and draw nothing.
 */
CARAPI_(Boolean) CWebView::DragTrackerHandler::Draw(
    /* [in] */ ICanvas* canvas)
{
    assert(canvas != NULL);

    if (mCurrStretchX != 0 || mCurrStretchY != 0) {
        Int32 sx = 0;
        Int32 sy = 0;
        webview->GetScrollX(&sx);
        webview->GetScrollY(&sy);
        sy -= HiddenHeightOfTitleBar();

        if (mSX != sx || mSY != sy) {
            BuildBitmap(sx, sy);
            mSX = sx;
            mSY = sy;
        }

        if (mState == ANIMATING_STATE) {
//            Interpolator.Result result = mInterp.timeToValues(mXY);
            if (/*result == Interpolator.Result.FREEZE_END*/0) {
                mState = FINISHED_STATE;
                return FALSE;
            } else {
                mProxy->OnStretchChange(mXY[0], mXY[1], NULL);
                webview->InvalidateEx2();
                // fall through to the draw
            }
        }

        Int32 count = 0;
        canvas->SaveEx(Canvas_MATRIX_SAVE_FLAG, &count);
        canvas->Translate(sx, sy);
        mProxy->OnDraw(canvas);
        canvas->RestoreToCount(count);

        return TRUE;
    }

    if (DebugFlags::sDRAG_TRACKER || DEBUG_DRAG_TRACKER) {
//        Logger::D(DebugFlags::sDRAG_TRACKER_LOGTAG, " -- draw false %d %d" +
//              mCurrStretchX + mCurrStretchY);
    }

    return FALSE;
}

CARAPI_(Float) CWebView::DragTrackerHandler::ComputeStretch(
    /* [in] */ Float delta,
    /* [in] */ Float min,
    /* [in] */ Float max)
{
    Float stretch = 0;
    if (max - min > MIN_SCROLL_AMOUNT_TO_DISABLE_DRAG_TRACKER) {
        if (delta < min) {
            stretch = delta - min;
        } else if (delta > max) {
            stretch = delta - max;
        }
    }

    return stretch;
}

CARAPI_(Int32) CWebView::DragTrackerHandler::HiddenHeightOfTitleBar()
{
    return webview->GetTitleHeight() - webview->GetVisibleTitleHeight();
}

// need a way to know if 565 or 8888 is the right config for
// capturing the display and giving it to the drag proxy
CARAPI_(BitmapConfig) CWebView::DragTrackerHandler::OffscreenBitmapConfig()
{
    // hard code 565 for now
    return BitmapConfig_RGB_565;
}

CARAPI_(void) CWebView::DragTrackerHandler::BuildBitmap(
    /* [in] */ Int32 sx,
    /* [in] */ Int32 sy)
{
    Int32 w = 0;
    Int32 h = webview->GetViewHeight();
    webview->GetWidth(&w);
    AutoPtr<IBitmap> bm;// = Bitmap.CreateBitmap(w, h, offscreenBitmapConfig());
    AutoPtr<ICanvas> canvas;
    CCanvas::New(bm, (ICanvas**)&canvas);
    canvas->Translate(-sx, -sy);
    webview->DrawContent(canvas);

    if (DebugFlags::sDRAG_TRACKER || DEBUG_DRAG_TRACKER) {
        Logger::D(DebugFlags::sDRAG_TRACKER_LOGTAG, 
              "--- buildBitmap %d %d %d %d" + sx + sy + w + h);
    }

    mProxy->OnBitmapChange(bm);
}

/**********************************ExtendedZoomControls**********************************/

CWebView::ExtendedZoomControls::ExtendedZoomControls(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{}

CARAPI_(void) CWebView::ExtendedZoomControls::Show(
    /* [in] */ Boolean showZoom,
    /* [in] */ Boolean canZoomOut)
{}

CARAPI_(void) CWebView::ExtendedZoomControls::Hide()
{
    Fade(View_GONE, 1.0f, 0.0f);
}

CARAPI_(Boolean) CWebView::ExtendedZoomControls::HasFocus()
{}

CARAPI_(void) CWebView::ExtendedZoomControls::SetOnZoomInClickListener(
    /* [in] */ IViewOnClickListener* listener)
{}

CARAPI_(void) CWebView::ExtendedZoomControls::SetOnZoomOutClickListener(
    /* [in] */ IViewOnClickListener* listener)
{}

CARAPI_(void) CWebView::ExtendedZoomControls::Fade(
    /* [in] */ Int32 visibility,
    /* [in] */ Float startAlpha,
    /* [in] */ Float endAlpha)
{}

/********************************PrivateHandler*****************************/
CARAPI_(PInterface) CWebView::PrivateHandler::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

CARAPI_(UInt32) CWebView::PrivateHandler::AddRef()
{
    return ElRefBase::AddRef();
}

CARAPI_(UInt32) CWebView::PrivateHandler::Release()
{
    return ElRefBase::Release();
}

CARAPI CWebView::PrivateHandler::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::PrivateHandler::Start(
    /* [in] */ ApartmentAttr attr)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::PrivateHandler::Finish()
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CWebView::PrivateHandler::PostCppCallback(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallback(target, func, params, id);
}

ECode CWebView::PrivateHandler::PostCppCallbackAtTime(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 uptimeMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtTime(target, func, params, id, uptimeMillis);
}

ECode CWebView::PrivateHandler::PostCppCallbackDelayed(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id,
    /* [in] */ Millisecond64 delayMillis)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackDelayed(target, func, params, id, delayMillis);
}

ECode CWebView::PrivateHandler::PostCppCallbackAtFrontOfQueue(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ IParcel* params,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->PostCppCallbackAtFrontOfQueue(target, func, params, id);
}

ECode CWebView::PrivateHandler::RemoveCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacks(target, func);
}

ECode CWebView::PrivateHandler::RemoveCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id)
{
    assert(mApartment != NULL);
    return mApartment->RemoveCppCallbacksEx(target, func, id);
}

ECode CWebView::PrivateHandler::HasCppCallbacks(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacks(target, func, result);
}

ECode CWebView::PrivateHandler::HasCppCallbacksEx(
    /* [in] */ Handle32 target,
    /* [in] */ Handle32 func,
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    assert(mApartment != NULL);
    return mApartment->HasCppCallbacksEx(target, func, id, result);
}

ECode CWebView::PrivateHandler::SendMessage(
    /* [in] */ Int32 message,
    /* [in] */ IParcel* params)
{
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CWebView::PrivateHandler::SendMessage(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params)
{
    return mApartment->PostCppCallback((Handle32)this, pvFunc, params, 0);
}

ECode CWebView::PrivateHandler::SendMessageAtTime(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params,
    /* [in] */ Millisecond64 uptimeMillis)
{
    return mApartment->PostCppCallbackAtTime(
        (Handle32)this, pvFunc, params, 0, uptimeMillis);
}

ECode CWebView::PrivateHandler::RemoveMessage(
    /* [in] */ Handle32 func)
{
    return mApartment->RemoveCppCallbacks((Handle32)this, func);
}

ECode CWebView::PrivateHandler::HandleRememberPassword(
    /* [in] */ IApartment* resumeMsgHandle,
    /* [in] */ Int32 resumeMsgId,
    /* [in] */ IParcel* resumeMsgParams,
    /* [in] */ String& host,
    /* [in] */ String& username,
    /* [in] */ String& password)
{
    VALIDATE_NOT_NULL(resumeMsgHandle);

    ((CWebViewDatabase*)(mWebView->mDatabase.Get()))->SetUsernamePassword(host,
                           username, password);
    resumeMsgHandle->SendMessage(resumeMsgId, resumeMsgParams);
}

ECode CWebView::PrivateHandler::HandleNeverRememberPassword(
    /* [in] */ IApartment* resumeMsgHandle,
    /* [in] */ Int32 resumeMsgId,
    /* [in] */ IParcel* resumeMsgParams,
    /* [in] */ String& host,
    /* [in] */ String& username,
    /* [in] */ String& password)
{
    VALIDATE_NOT_NULL(resumeMsgHandle);

    ((CWebViewDatabase*)(mWebView->mDatabase.Get()))->SetUsernamePassword(
            host, String(NULL), String(NULL));
    resumeMsgHandle->SendMessage(resumeMsgId, resumeMsgParams);
}

ECode CWebView::PrivateHandler::HandlePreventDefaultTimeout(
    /* [in] */ Int32 action)
{
    // if timeout happens, cancel it so that it won't block UI
    // to continue handling touch events
    if ((action == MotionEvent_ACTION_DOWN
            && mWebView->mPreventDefault == PREVENT_DEFAULT_MAYBE_YES)
            || (action == MotionEvent_ACTION_MOVE
            && mWebView->mPreventDefault == PREVENT_DEFAULT_NO_FROM_TOUCH_DOWN)) {
        mWebView->CancelWebCoreTouchEvent(
                mWebView->ViewToContentX((Int32) mWebView->mLastTouchX + mWebView->mScrollX),
                mWebView->ViewToContentY((Int32) mWebView->mLastTouchY + mWebView->mScrollY),
                TRUE);
    }
}

ECode CWebView::PrivateHandler::HandleSwitchToShortPress()
{
    if (mWebView->mTouchMode == TOUCH_INIT_MODE) {
        if (mWebView->mPreventDefault != PREVENT_DEFAULT_YES) {
            mWebView->mTouchMode = TOUCH_SHORTPRESS_START_MODE;
            mWebView->UpdateSelection();
        } else {
            // set to TOUCH_SHORTPRESS_MODE so that it won't
            // trigger double tap any more
            mWebView->mTouchMode = TOUCH_SHORTPRESS_MODE;
        }
    } else if (mWebView->mTouchMode == TOUCH_DOUBLE_TAP_MODE) {
        mWebView->mTouchMode = TOUCH_DONE_MODE;
    }
}

ECode CWebView::PrivateHandler::HandleSwitchToLongPress()
{
    if (mWebView->InFullScreenMode() || mWebView->mDeferTouchProcess) {
        WebViewCore::TouchEventData* ted = new WebViewCore::TouchEventData();
        ted->mAction = WebViewCore::ACTION_LONGPRESS;
        ted->mX = mWebView->ViewToContentX((Int32) mWebView->mLastTouchX + mWebView->mScrollX);
        ted->mY = mWebView->ViewToContentY((Int32) mWebView->mLastTouchY + mWebView->mScrollY);
        // metaState for long press is tricky. Should it be the
        // state when the press started or when the press was
        // released? Or some intermediary key state? For
        // simplicity for now, we don't set it.
        ted->mMetaState = 0;
        ted->mReprocess = mWebView->mDeferTouchProcess;
        if (mWebView->mDeferTouchProcess) {
            ted->mViewX = mWebView->mLastTouchX;
            ted->mViewY = mWebView->mLastTouchY;
        }
//        mWebViewCore.sendMessage(EventHub.TOUCH_EVENT, ted);
    } else if (mWebView->mPreventDefault != PREVENT_DEFAULT_YES) {
        mWebView->mTouchMode = TOUCH_DONE_MODE;
        mWebView->PerformLongClick(NULL);
        mWebView->RebuildWebTextView();
    }
}

ECode CWebView::PrivateHandler::HandleReleaseSingleTap()
{
    mWebView->DoShortPress();
}

ECode CWebView::PrivateHandler::HandleScrollByMsgId(
    /* [in] */ Int32 cx,
    /* [in] */ Int32 cy,
    /* [in] */ Boolean animate)
{
    mWebView->SetContentScrollBy(cx, cy, animate);
}

ECode CWebView::PrivateHandler::HandleSyncScrollToMsgId()
{
    if (mWebView->mUserScroll) {
        // if user has scrolled explicitly, don't sync the
        // scroll position any more
        mWebView->mUserScroll = FALSE;
    }
}

ECode CWebView::PrivateHandler::HandleScrollToMsgId(
    /* [in] */ Int32 cx,
    /* [in] */ Int32 cy)
{
    if (mWebView->SetContentScrollTo(cx, cy)) {
        // if we can't scroll to the exact position due to pin,
        // send a message to WebCore to re-scroll when we get a
        // new picture
        mWebView->mUserScroll = FALSE;
//        mWebViewCore.sendMessage(EventHub.SYNC_SCROLL, msg.arg1, msg.arg2);
    }
}

ECode CWebView::PrivateHandler::HandleSpawnScrollToMsgId(
    /* [in] */ Int32 cx,
    /* [in] */ Int32 cy)
{
    mWebView->SpawnContentScrollTo(cx, cy);
}

ECode CWebView::PrivateHandler::HandleUpdateZoomRange(
    /* [in] */ WebViewCore::RestoreState* restoreState)
{
    VALIDATE_NOT_NULL(restoreState);

    // mScrollX contains the new minPrefWidth
    mWebView->UpdateZoomRange(restoreState, mWebView->GetViewWidth(),
            restoreState->mScrollX, FALSE);
}

ECode CWebView::PrivateHandler::HandleNewPictureMsgId(
    /* [in] */ WebViewCore::DrawData* draw)
{
    // If we've previously delayed deleting a root
    // layer, do it now.
    if (mWebView->mDelayedDeleteRootLayer) {
        mWebView->mDelayedDeleteRootLayer = FALSE;
        mWebView->NativeSetRootLayer(0);
    }

    WebSettings* settings = mWebView->mWebViewCore->GetSettings();
    // called for new content
    const Int32 viewWidth = mWebView->GetViewWidth();
    //const WebViewCore.DrawData draw = (WebViewCore.DrawData) msg.obj;
    AutoPtr<IPoint> viewSize = draw->mViewPoint;
    Boolean useWideViewport = settings->GetUseWideViewPort();
    WebViewCore::RestoreState* restoreState = draw->mRestoreState;
    Boolean hasRestoreState = restoreState != NULL;
    if (hasRestoreState) {

        Int32 x;
        viewSize->GetX(&x);

        mWebView->UpdateZoomRange(restoreState, x, draw->mMinPrefWidth, TRUE);

        if (!mWebView->mDrawHistory) {
            mWebView->mInZoomOverview = FALSE;

            if (mWebView->mInitialScaleInPercent > 0) {
                mWebView->SetNewZoomScale(mWebView->mInitialScaleInPercent / 100.0f,
                    mWebView->mInitialScaleInPercent != mWebView->mTextWrapScale * 100,
                    FALSE);
            } else if (restoreState->mViewScale > 0) {
                mWebView->mTextWrapScale = restoreState->mTextWrapScale;
                mWebView->SetNewZoomScale(restoreState->mViewScale, FALSE,
                    FALSE);
            } else {

                mWebView->mInZoomOverview = useWideViewport
                    && settings->GetLoadWithOverviewMode();
                
                Float scale;

                if (mWebView->mInZoomOverview) {
                    scale = (Float) viewWidth / DEFAULT_VIEWPORT_WIDTH;
                } else {
                    scale = restoreState->mTextWrapScale;
                }

                Int32 abs = scale - mWebView->mTextWrapScale;
                abs = abs > 0 ? abs : -abs;

                mWebView->SetNewZoomScale(scale, abs >= MINIMUM_SCALE_INCREMENT,
                    FALSE);
            }

            mWebView->SetContentScrollTo(restoreState->mScrollX,
                restoreState->mScrollY);
            // As we are on a new page, remove the WebTextView. This
            // is necessary for page loads driven by webkit, and in
            // particular when the user was on a password field, so
            // the WebTextView was visible.
            mWebView->ClearTextEntry(FALSE);
            // update the zoom buttons as the scale can be changed
//            AutoPtr<IWebSettings> settings;
//            mWebView->GetSettings((IWebSettings**)&settings);
//            if (((CWebSettings*)(settings.Get()))->GetBuiltInZoomControls()) {
                mWebView->UpdateZoomButtonsEnabled();
//            }
        }
    }
    // We update the layout (i.e. request a layout from the
    // view system) if the last view size that we sent to
    // WebCore matches the view size of the picture we just
    // received in the fixed dimension.
    Int32 x, y;

    viewSize->GetX(&x);
    viewSize->GetY(&y);

    const Boolean updateLayout = x == mWebView->mLastWidthSent
            && y == mWebView->mLastHeightSent;
    
    draw->mWidthHeight->GetX(&x);
    draw->mWidthHeight->GetY(&x);
    mWebView->RecordNewContentSize(x, y
            + (mWebView->mFindIsUp ? mWebView->mFindHeight : 0), updateLayout);

    if (DebugFlags::sWEB_VIEW) {
        AutoPtr<IRect> b;
        draw->mInvalRegion->GetBounds((IRect**)&b);
        Int32 left, top, right, bottom;
        b->GetLeft(&left);
        b->GetTop(&top);
        b->GetRight(&right);
        b->GetBottom(&bottom);
        Logger::V(LOGTAG, "NEW_PICTURE_MSG_ID {%d,%d,%d,%d}" +
                left + top + right + bottom);
    }

    AutoPtr<IRect> rect;
    draw->mInvalRegion->GetBounds((IRect**)&rect);
    mWebView->InvalidateContentRect(rect);
    if (mWebView->mPictureListener != NULL) {
        AutoPtr<IPicture> pic;
        mWebView->CapturePicture((IPicture**)&pic);
        mWebView->mPictureListener->OnNewPicture(/*WebView.this*/mWebView, pic);
    }

    if (useWideViewport) {
        // limit mZoomOverviewWidth upper bound to
        // sMaxViewportWidth so that if the page doesn't behave
        // well, the WebView won't go insane. limit the lower
        // bound to match the default scale for mobile sites.
        
        Int32 max1, max2;
        Int32 x, y;

        draw->mViewPoint->GetX(&x);
        draw->mViewPoint->GetY(&y);

        max1 = draw->mMinPrefWidth > x ? draw->mMinPrefWidth : x;
        max2 = (Int32) (viewWidth / mWebView->mDefaultScale) > max1 ? (Int32) (viewWidth / mWebView->mDefaultScale) : max1;
        mWebView->mZoomOverviewWidth = sMaxViewportWidth < max2 ? sMaxViewportWidth : max2;
    }

    if (!mWebView->mMinZoomScaleFixed) {
        mWebView->mMinZoomScale = (Float) viewWidth / mWebView->mZoomOverviewWidth;
    }

    if (!mWebView->mDrawHistory && mWebView->mInZoomOverview) {
        // fit the content width to the current view. Ignore
        // the rounding error case.
        Int32 abs = (viewWidth * mWebView->mInvActualScale) - mWebView->mZoomOverviewWidth;
        abs = abs > 0 ? abs : -abs;
        if (abs > 1) {
            abs = mWebView->mActualScale - mWebView->mTextWrapScale;
            abs = abs > 0 ? abs : -abs;
            mWebView->SetNewZoomScale((Float) viewWidth
                    / mWebView->mZoomOverviewWidth, abs < MINIMUM_SCALE_INCREMENT,
                    FALSE);
        }
    }

    if (draw->mFocusSizeChanged && mWebView->InEditingMode()) {
        mWebView->mFocusSizeChanged = TRUE;
    }

    if (hasRestoreState) {
        mWebView->mViewManager->PostReadyToDrawAll();
    }

}

ECode CWebView::PrivateHandler::HandleWebcoreInitializedMsgId(
    /* [in] */ Int32 arg)
{
    // nativeCreate sets mNativeClass to a non-zero value
    mWebView->NativeCreate(arg);
}

ECode CWebView::PrivateHandler::HandleUpdateTextfieldTextMsgId(
    /* [in] */ String& obj,
    /* [in] */ String& password,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2)
{
    // Make sure that the textfield is currently focused
    // and representing the same node as the pointer.
    if (mWebView->InEditingMode() &&
            mWebView->mWebTextView->IsSameTextField(arg1)) {
        if (password.GetLength() != 0) {
            AutoPtr<ISpannable> text;// = (Spannable) mWebTextView->GetText();
            Int32 start = Selection::GetSelectionStart(text);
            Int32 end = Selection::GetSelectionEnd(text);
            mWebView->mWebTextView->SetInPassword(TRUE);
            // Restore the selection, which may have been
            // ruined by setInPassword.
            AutoPtr<ISpannable> pword;// = (Spannable) mWebTextView.getText();
            Selection::SetSelection(pword, start, end);
        // If the text entry has created more events, ignore
        // this one.
        } else if (arg2 == mWebView->mTextGeneration) {
            mWebView->mWebTextView->SetTextAndKeepSelection(obj);
        }
    }
}

ECode CWebView::PrivateHandler::HandleRequestKeyBoardWithSelectionMsgId(
    /* [in] */ Int32 nodePointer,
    /* [in] */ Int32 textGeneration,
    /* [in] */ WebViewCore::TextSelectionData* data)
{
    mWebView->DisplaySoftKeyboard(TRUE);
    mWebView->UpdateTextSelectionFromMessage(nodePointer, textGeneration, data);
}

ECode CWebView::PrivateHandler::HandleUpdateTextSelectionMsgId(
    /* [in] */ Int32 nodePointer,
    /* [in] */ Int32 textGeneration,
    /* [in] */ WebViewCore::TextSelectionData* data)
{
    // If no textfield was in focus, and the user touched one,
    // causing it to send this message, then WebTextView has not
    // been set up yet.  Rebuild it so it can set its selection.
    mWebView->RebuildWebTextView();
    mWebView->UpdateTextSelectionFromMessage(nodePointer, textGeneration, data);
}

ECode CWebView::PrivateHandler::HandleReturnLabel(
    /* [in] */ Int32 arg,
    /* [in] */ String& obj)
{
#if 0
    if (inEditingMode()
            && mWebTextView.isSameTextField(msg.arg1)) {
        mWebTextView.setHint((String) msg.obj);
        InputMethodManager imm
                = InputMethodManager.peekInstance();
        // The hint is propagated to the IME in
        // onCreateInputConnection.  If the IME is already
        // active, restart it so that its hint text is updated.
        if (imm != null && imm.isActive(mWebTextView)) {
            imm.restartInput(mWebTextView);
        }
    }
#endif
}

ECode CWebView::PrivateHandler::HandleMoveOutOfPlugin(
    /* [in] */ Int32 arg)
{
    mWebView->NavHandledKey(arg, 1, FALSE, 0);
}

ECode CWebView::PrivateHandler::HandleUpdateTextEntryMsgId()
{
    // this is sent after finishing resize in WebViewCore. Make
    // sure the text edit box is still on the  screen.
    mWebView->SelectionDone();
    if (mWebView->InEditingMode() && mWebView->NativeCursorIsTextInput()) {
        mWebView->mWebTextView->BringIntoView();
        mWebView->RebuildWebTextView();
    }
}

ECode CWebView::PrivateHandler::HandleClearTextEntry()
{
    mWebView->ClearTextEntry(FALSE);
}

ECode CWebView::PrivateHandler::HandleInvalRectMsgId(
    /* [in] */ IRect* r)
{    
    if (r == NULL) {
        mWebView->InvalidateEx2();
    } else {
        // we need to scale r from content into view coords,
        // which viewInvalidate() does for us
        Int32 left, top, right, bottom;
        r->GetLeft(&left);
        r->GetTop(&top);
        r->GetRight(&right);
        r->GetBottom(&bottom);
        mWebView->ViewInvalidate(left, top, right, bottom);
    }
}

ECode CWebView::PrivateHandler::HandleImmediateRepaintMsgId()
{
    mWebView->InvalidateEx2();
}

ECode CWebView::PrivateHandler::HandleSetRootLayerMsgId(
    /* [in] */ Int32 arg)
{
    if (0 == arg) {
        // Null indicates deleting the old layer, but
        // don't actually do so until we've got the
        // new page to display.
        mWebView->mDelayedDeleteRootLayer = TRUE;
    } else {
        mWebView->mDelayedDeleteRootLayer = FALSE;
        mWebView->NativeSetRootLayer(arg);
        mWebView->InvalidateEx2();
    }
}

ECode CWebView::PrivateHandler::HandleRequestFormData(
    /* [in] */ Int32 arg,
    /* [in] */ WebTextView::AutoCompleteAdapter* adapter)
{
    if (mWebView->mWebTextView->IsSameTextField(arg)) {
        mWebView->mWebTextView->SetAdapterCustom(adapter);
    }
}

ECode CWebView::PrivateHandler::HandleResumeWebcorePriority()
{
    WebViewCore::ResumePriority();
    WebViewCore::ResumeUpdatePicture(mWebView->mWebViewCore);
}

ECode CWebView::PrivateHandler::HandleLongPressCenter()
{
    // as this is shared by keydown and trackballdown, reset all
    // the states
//    mWebView->mGotCenterDown = FALSE;
    mWebView->mTrackballDown = FALSE;
    mWebView->PerformLongClick(NULL);
}

ECode CWebView::PrivateHandler::HandleWebCoreNeedTouchEvents(
    /* [in] */ Int32 arg)
{
    mWebView->mForwardTouchEvents = (arg != 0);
}

ECode CWebView::PrivateHandler::HandlePreventTouchId(
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ WebViewCore::TouchEventData* obj)
{
    if (mWebView->InFullScreenMode()) {
        return E_NOT_IMPLEMENTED;
    }

    if (obj == NULL) {
        if (arg1 == MotionEvent_ACTION_DOWN
                && mWebView->mPreventDefault == PREVENT_DEFAULT_MAYBE_YES) {
            // if prevent default is called from WebCore, UI
            // will not handle the rest of the touch events any
            // more.
            mWebView->mPreventDefault = arg2 == 1 ? PREVENT_DEFAULT_YES
                    : PREVENT_DEFAULT_NO_FROM_TOUCH_DOWN;
        } else if (arg1 == MotionEvent_ACTION_MOVE
                && mWebView->mPreventDefault == PREVENT_DEFAULT_NO_FROM_TOUCH_DOWN) {
            // the return for the first ACTION_MOVE will decide
            // whether UI will handle touch or not. Currently no
            // support for alternating prevent default
            mWebView->mPreventDefault = arg2 == 1 ? PREVENT_DEFAULT_YES
                    : PREVENT_DEFAULT_NO;
        }
    } else if (arg2 == 0) {
        // prevent default is not called in WebCore, so the
        // message needs to be reprocessed in UI
        WebViewCore::TouchEventData* ted = obj;
        switch (ted->mAction) {

            case MotionEvent_ACTION_DOWN:
                mWebView->mLastDeferTouchX = ted->mViewX;
                mWebView->mLastDeferTouchY = ted->mViewY;
                mWebView->mDeferTouchMode = TOUCH_INIT_MODE;
                break;

            case MotionEvent_ACTION_MOVE: {
                // no snapping in defer process
                if (mWebView->mDeferTouchMode != TOUCH_DRAG_MODE) {
                    mWebView->mDeferTouchMode = TOUCH_DRAG_MODE;
                    mWebView->mLastDeferTouchX = ted->mViewX;
                    mWebView->mLastDeferTouchY = ted->mViewY;
                    mWebView->StartDrag();
                }

                Int32 deltaX = mWebView->PinLocX((Int32) (mWebView->mScrollX
                        + mWebView->mLastDeferTouchX - ted->mViewX))
                        - mWebView->mScrollX;
                Int32 deltaY = mWebView->PinLocY((Int32) (mWebView->mScrollY
                        + mWebView->mLastDeferTouchY - ted->mViewY))
                        - mWebView->mScrollY;
                mWebView->DoDrag(deltaX, deltaY);

                if (deltaX != 0) mWebView->mLastDeferTouchX = ted->mViewX;
                if (deltaY != 0) mWebView->mLastDeferTouchY = ted->mViewY;
                break;
            }

            case MotionEvent_ACTION_UP:
            case MotionEvent_ACTION_CANCEL:
                if (mWebView->mDeferTouchMode == TOUCH_DRAG_MODE) {
                    // no fling in defer process
                    mWebView->mScroller->SpringBack(mWebView->mScrollX, mWebView->mScrollY, 0,
                            mWebView->ComputeMaxScrollX(), 0,
                            mWebView->ComputeMaxScrollY());
                    mWebView->InvalidateEx2();
                    WebViewCore::ResumePriority();
                    WebViewCore::ResumeUpdatePicture(mWebView->mWebViewCore);
                }

                mWebView->mDeferTouchMode = TOUCH_DONE_MODE;
                break;

            case WebViewCore::ACTION_DOUBLETAP:
                // doDoubleTap() needs mLastTouchX/Y as anchor
                mWebView->mLastTouchX = ted->mViewX;
                mWebView->mLastTouchY = ted->mViewY;
                mWebView->DoDoubleTap();
                mWebView->mDeferTouchMode = TOUCH_DONE_MODE;
                break;

            case WebViewCore::ACTION_LONGPRESS:
                AutoPtr<IHitTestResult> hitTest;
                mWebView->GetHitTestResult((IHitTestResult**)&hitTest);
                if (hitTest != NULL && ((HitTestResult*)(hitTest.Get()))->mType
                        != HitTestResult_UNKNOWN_TYPE) {
                    mWebView->PerformLongClick(NULL);
                    mWebView->RebuildWebTextView();
                }
                mWebView->mDeferTouchMode = TOUCH_DONE_MODE;
                break;
        }
    }
}

ECode CWebView::PrivateHandler::HandleRequestKeyBoard(
    /* [in] */ Int32 arg)
{
    if (arg == 0) {
        mWebView->HideSoftKeyboard();
    } else {
        mWebView->DisplaySoftKeyboard(FALSE);
    }
}

ECode CWebView::PrivateHandler::HandleFindAgain()
{
    // Ignore if find has been dismissed.
    if (mWebView->mFindIsUp) {
        mWebView->FindAll(mWebView->mLastFind, NULL);
    }
}

ECode CWebView::PrivateHandler::HandleDragHeldMotionless()
{
    mWebView->mHeldMotionless = MOTIONLESS_TRUE;
    mWebView->InvalidateEx2();
    // fall through to keep scrollbars awake
}

ECode CWebView::PrivateHandler::HandleAwakenScrollBars()
{
    if (mWebView->mTouchMode == TOUCH_DRAG_MODE
            && mWebView->mHeldMotionless == MOTIONLESS_TRUE) {
        mWebView->AwakenScrollBars(ViewConfiguration::GetScrollDefaultDelay(), FALSE);
        
        ECode (STDCALL CWebView::PrivateHandler::*pHandlerFunc)();

        pHandlerFunc = &CWebView::PrivateHandler::HandleAwakenScrollBars;

        mWebView->mPrivateHandler->mApartment->PostCppCallback((Handle32)this, 
             *(Handle32*)&pHandlerFunc, NULL, ViewConfiguration::GetScrollDefaultDelay());
    }
}

ECode CWebView::PrivateHandler::HandleDoMotionUp(
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2)
{
    mWebView->DoMotionUp(arg1, arg2);
}

ECode CWebView::PrivateHandler::HandleShowFullScreen(
    /* [in] */ IView* view,
    /* [in] */ Int32 npp)
{
    if (mWebView->mFullScreenHolder != NULL) {
        Logger::W(LOGTAG, "Should not have another full screen.");
        mWebView->mFullScreenHolder->Dismiss();
    }

    mWebView->mFullScreenHolder = new PluginFullScreenHolder(mWebView, npp);
    mWebView->mFullScreenHolder->SetContentView(view);
    mWebView->mFullScreenHolder->SetCancelable(FALSE);
    mWebView->mFullScreenHolder->SetCanceledOnTouchOutside(FALSE);
    mWebView->mFullScreenHolder->Show();
}

ECode CWebView::PrivateHandler::HandleHideFullScreen()
{
    if (mWebView->InFullScreenMode()) {
        mWebView->mFullScreenHolder->Dismiss();
        mWebView->mFullScreenHolder = NULL;
    }
}

ECode CWebView::PrivateHandler::HandleDomFocusChanged()
{
    if (mWebView->InEditingMode()) {
        mWebView->NativeClearCursor();
        mWebView->RebuildWebTextView();
    }
}

ECode CWebView::PrivateHandler::HandleShowRectMsgId(
    /* [in] */ WebViewCore::ShowRectData* data)
{
    VALIDATE_NOT_NULL(data);

    Int32 x = mWebView->mScrollX;
    Int32 left = mWebView->ContentToViewX(data->mLeft);
    Int32 width = mWebView->ContentToViewDimension(data->mWidth);
    Int32 maxWidth = mWebView->ContentToViewDimension(data->mContentWidth);
    Int32 viewWidth = mWebView->GetViewWidth();
    
    if (width < viewWidth) {
        // center align
        x += left + width / 2 - mWebView->mScrollX - viewWidth / 2;
    } else {
        x += (Int32) (left + data->mXPercentInDoc * width
                - mWebView->mScrollX - data->mXPercentInView * viewWidth);
    }

    if (DebugFlags::sWEB_VIEW) {
//        Logger::V(LOGTAG, "showRectMsg=(left=%d,width=%d,maxWidth=%d,viewWidth=%d,x=%d,xPercentInDoc=%d,xPercentInView=%d)"
//              + left + width + maxWidth + viewWidth + x + data->mXPercentInDoc
//              + data->mXPercentInView);
    }

    // use the passing content width to cap x as the current
    // mContentWidth may not be updated yet
    
    //x = Math.max(0, (Math.min(maxWidth, x + viewWidth)) - viewWidth);
    x += viewWidth;
    x = x < maxWidth ? x : maxWidth;
    x -= viewWidth;
    x = x > 0 ? x : 0;

    Int32 top = mWebView->ContentToViewY(data->mTop);
    Int32 height = mWebView->ContentToViewDimension(data->mHeight);
    Int32 maxHeight = mWebView->ContentToViewDimension(data->mContentHeight);
    Int32 viewHeight = mWebView->GetViewHeight();
    Int32 y = (Int32) (top + data->mYPercentInDoc * height -
                   data->mYPercentInView * viewHeight);
#if 0
    if (DebugFlags.WEB_VIEW) {
        Log.v(LOGTAG, "showRectMsg=(top=" + top + ",height=" +
              height + ",maxHeight=" + maxHeight +
              ",viewHeight=" + viewHeight + ",y="
              + y + ",yPercentInDoc=" + data.mYPercentInDoc +
              ",yPercentInView=" + data.mYPercentInView+ ")");
    }
#endif
    // use the passing content height to cap y as the current
    // mContentHeight may not be updated yet
    //y = Math.max(0, (Math.min(maxHeight, y + viewHeight) - viewHeight));
    y += viewHeight;
    y = y < maxHeight ? y : maxHeight;
    y -= viewHeight;
    y = y > 0 ? y : 0;

    // We need to take into account the visible title height
    // when scrolling since y is an absolute view position.
    //y = Math.max(0, y - getVisibleTitleHeight());
    y -= mWebView->GetVisibleTitleHeight();
    y = y > 0 ? y : 0;
    mWebView->ScrollTo(x, y);
}

ECode CWebView::PrivateHandler::HandleCenterFitRect(
    /* [in] */ IRect* r)
{
    VALIDATE_NOT_NULL(r);

    mWebView->mInZoomOverview = FALSE;
    Int32 left, top, width, height;
    r->GetLeft(&left);
    r->GetTop(&top);
    r->GetWidth(&width);
    r->GetHeight(&height);
    mWebView->CenterFitRect(left, top, width, height);
}

ECode CWebView::PrivateHandler::HandleSetScrollBarModes(
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2)
{
    mWebView->mHorizontalScrollBarMode = arg1;
    mWebView->mVerticalScrollBarMode = arg2;
}