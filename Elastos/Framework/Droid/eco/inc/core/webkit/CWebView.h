
#ifndef __CWEBVIEW_H__
#define __CWEBVIEW_H__

#include <elastos/ElRefBase.h>

#include "ext/frameworkext.h"

#include "_CWebView.h"

#include "view/VelocityTracker.h"
#include "widget/OverScroller.h"
#include "widget/FrameLayout.h"
#include "widget/AbsoluteLayout.h"
#include "widget/EdgeGlow.h"
#include "database/DataSetObserver.h"
#include "PluginFullScreenHolder.h"
#include "ViewManager.h"
#include "WebViewCore.h"
#include "WebTextView.h"

CarClass(CWebView), public AbsoluteLayout
{
public:
    // FIXME: Want to make this public, but need to change the API file.
    class HitTestResult : public ElRefBase, IHitTestResult {
        friend class CWebView;
    public:
        HitTestResult();

        CARAPI GetType(
           /* out */ Int32* type);

        CARAPI GetExtra(
           /* out */ String* extra);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    private:
        CARAPI_(void) SetType(
            /* [in] */ Int32 type);

        CARAPI_(void) SetExtra(
            /* [in] */ const String& extra);

    private:
        Int32 mType;
        String mExtra;
    };

public:
    CWebView() {}

    IVIEW_METHODS_DECL();

    IVIEWGROUP_METHODS_DECL();

    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

#if 0
    CARAPI GetVerticalFadingEdgeLength(
        /* [out] */ Int32* length);

    CARAPI SetFadingEdgeLength(
        /* [in] */ Int32 length);

    CARAPI GetHorizontalFadingEdgeLength(
        /* [out] */ Int32* length);

    CARAPI GetVerticalScrollbarWidth(
        /* [out] */ Int32* width);

    CARAPI GetHorizontalScrollbarHeight(
        /* [out] */ Int32* height);

    CARAPI SetOnFocusChangeListener(
        /* [in] */ IViewOnFocusChangeListener* l);

    CARAPI GetOnFocusChangeListener(
        /* [out] */ IViewOnFocusChangeListener** l);

    CARAPI SetOnClickListener(
        /* [in] */ IViewOnClickListener* l);

    CARAPI SetOnLongClickListener(
        /* [in] */ IViewOnLongClickListener* l);

    CARAPI SetOnCreateContextMenuListener(
        /* [in] */ IViewOnCreateContextMenuListener* l);

    CARAPI PerformClick(
        /* [out] */ Boolean* result);

    CARAPI PerformLongClick(
        /* [out] */ Boolean* result);

    CARAPI ShowContextMenu(
        /* [out] */ Boolean* result);

    CARAPI SetOnKeyListener(
        /* [in] */ IViewOnKeyListener* l);

    CARAPI SetOnTouchListener(
        /* [in] */ IViewOnTouchListener* l);

    CARAPI RequestRectangleOnScreen(
        /* [in] */ IRect* rectangle,
        /* [out] */ Boolean* result);

    CARAPI RequestRectangleOnScreenEx(
        /* [in] */ IRect* rectangle,
        /* [in] */ Boolean immediate,
        /* [out] */ Boolean* result);

    CARAPI ClearFocus();

    CARAPI HasFocus(
        /* [out] */ Boolean* hasFocus);

    CARAPI HasFocusable(
        /* [out] */ Boolean* hasFocusable);

    CARAPI DispatchPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event,
        /* [out] */ Boolean* result);

    CARAPI GetContentDescription(
        /* [out, callee] */ ArrayOf<Char8>** description);

    CARAPI SetContentDescription(
        /* [in] */ const ArrayOf<Char8>& contentDescription);

    CARAPI IsFocused(
        /* [out] */ Boolean* focused);

    CARAPI FindFocus(
        /* [out] */ IView** view);

    CARAPI SetScrollContainer(
        /* [in] */ Boolean isScrollContainer);

    CARAPI GetDrawingCacheQuality(
        /* [out] */ Int32* quality);

    CARAPI SetDrawingCacheQuality(
        /* [in] */ Int32 quality);

    CARAPI GetKeepScreenOn(
        /* [out] */ Boolean* result);

    CARAPI SetKeepScreenOn(
        /* [in] */ Boolean keepScreenOn);

    CARAPI GetNextFocusLeftId(
        /* [out] */ Int32* nextFocusLeftId);

    CARAPI SetNextFocusLeftId(
        /* [in] */ Int32 nextFocusLeftId);

    CARAPI GetNextFocusRightId(
        /* [out] */ Int32* nextFocusRightId);

    CARAPI SetNextFocusRightId(
        /* [in] */ Int32 nextFocusRightId);

    CARAPI GetNextFocusUpId(
        /* [out] */ Int32* nextFocusUpId);

    CARAPI SetNextFocusUpId(
        /* [in] */ Int32 nextFocusUpId);

    CARAPI GetNextFocusDownId(
        /* [out] */ Int32* nextFocusDownId);

    CARAPI SetNextFocusDownId(
        /* [in] */ Int32 nextFocusDownId);

    CARAPI IsShown(
        /* [out] */ Boolean* shown);

    CARAPI IsFitsSystemWindowsFlagSet(
        /* [out] */ Boolean* flag);

    CARAPI GetVisibility(
        /* [out] */ Int32* visibility);

    CARAPI SetVisibility(
        /* [in] */ Int32 visibility);

    CARAPI IsEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI SetFocusable(
        /* [in] */ Boolean focusable);

    CARAPI SetFocusableInTouchMode(
        /* [in] */ Boolean focusableInTouchMode);

    CARAPI SetSoundEffectsEnabled(
        /* [in] */ Boolean soundEffectsEnabled);

    CARAPI IsSoundEffectsEnabled(
        /* [out] */ Boolean* soundEffectsEnabled);

    CARAPI SetHapticFeedbackEnabled(
        /* [in] */ Boolean hapticFeedbackEnabled);

    CARAPI IsHapticFeedbackEnabled(
        /* [out] */ Boolean* hapticFeedbackEnabled);

    CARAPI SetWillNotDraw(
        /* [in] */ Boolean willNotDraw);

    CARAPI WillNotDraw(
        /* [out] */ Boolean* willNotDraw);

    CARAPI SetWillNotCacheDrawing(
        /* [in] */ Boolean willNotCacheDrawing);

    CARAPI WillNotCacheDrawing(
        /* [out] */ Boolean* cache);

    CARAPI IsClickable(
        /* [out] */ Boolean* clickable);

    CARAPI SetClickable(
        /* [in] */ Boolean clickable);

    CARAPI IsLongClickable(
        /* [out] */ Boolean* longClickable);

    CARAPI SetLongClickable(
        /* [in] */ Boolean longClickable);

    CARAPI SetPressed(
        /* [in] */ Boolean pressed);

    CARAPI IsPressed(
        /* [out] */ Boolean* pressed);

    CARAPI IsSaveEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetSaveEnabled(
        /* [in] */ Boolean enabled);

    CARAPI GetFilterTouchesWhenObscured(
        /* [out] */ Boolean* enabled);

    CARAPI SetFilterTouchesWhenObscured(
        /* [in] */ Boolean enabled);

    CARAPI IsFocusable(
        /* [out] */ Boolean* focusable);

    CARAPI IsFocusableInTouchMode(
        /* [out] */ Boolean* focusable);

    CARAPI FocusSearch(
        /* [in] */ Int32 direction,
        /* [out] */ IView** view);

    CARAPI DispatchUnhandledMove(
        /* [in] */ IView* focused,
        /* [in] */ Int32 direction,
        /* [out] */ Boolean* result);

    CARAPI GetFocusables(
        /* [in] */ Int32 direction,
        /* [out] */ IObjectContainer** views);

    CARAPI AddFocusables(
        /* [in] */ IObjectContainer* views,
        /* [in] */ Int32 direction);

    CARAPI AddFocusablesEx(
        /* [in] */ IObjectContainer* views,
        /* [in] */ Int32 direction,
        /* [in] */ Int32 focusableMode);

    CARAPI GetTouchables(
        /* [out] */ IObjectContainer** views);

    CARAPI AddTouchables(
        /* [in] */ IObjectContainer* views);

    CARAPI RequestFocus(
        /* [out] */ Boolean* result);

    CARAPI RequestFocusEx(
        /* [in] */ Int32 direction,
        /* [out] */ Boolean* result);

    CARAPI RequestFocusEx2(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect,
        /* [out] */ Boolean* result);

    CARAPI RequestFocusFromTouch(
        /* [out] */ Boolean* result);

    CARAPI DispatchStartTemporaryDetach();

    CARAPI OnStartTemporaryDetach();

    CARAPI DispatchFinishTemporaryDetach();

    CARAPI OnFinishTemporaryDetach();

    CARAPI GetKeyDispatcherState(
        /* [out] */ IDispatcherState** state);

    CARAPI DispatchKeyEventPreIme(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI DispatchKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI DispatchKeyShortcutEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI DispatchTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnFilterTouchEventForSecurity(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    CARAPI DispatchTrackballEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    CARAPI DispatchWindowFocusChanged(
        /* [in] */ Boolean hasFocus);

    CARAPI OnWindowFocusChanged(
        /* [in] */ Boolean hasWindowFocus);

    CARAPI HasWindowFocus(
        /* [out] */ Boolean* focus);

    CARAPI DispatchDisplayHint(
        /* [in] */ Int32 hint);

    CARAPI DispatchWindowVisibilityChanged(
        /* [in] */ Int32 visibility);

    CARAPI GetWindowVisibility(
        /* [out] */ Int32* visibility);

    CARAPI GetWindowVisibleDisplayFrame(
        /* [in] */ IRect* outRect);

    CARAPI DispatchConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI IsInTouchMode(
        /* [out] */ Boolean* result);

    CARAPI GetContext(
        /* [out] */ IContext** ctx);

    CARAPI OnKeyPreIme(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyShortcut(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnCheckIsTextEditor(
        /* [out] */ Boolean* result);

    CARAPI OnCreateInputConnection(
        /* [in] */ IEditorInfo* outAttrs,
        /* [out] */ IInputConnection** inputConnection);

    CARAPI CheckInputConnectionProxy(
        /* [in] */ IView* view,
        /* [out] */ Boolean* result);

    CARAPI CreateContextMenu(
        /* [in] */ IContextMenu* menu);

    CARAPI OnTrackballEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    CARAPI SetTouchDelegate(
        /* [in] */ ITouchDelegate* touchDelegate);

    CARAPI GetTouchDelegate(
        /* [out] */ ITouchDelegate** touchDelegate);

    CARAPI CancelLongPress();

    CARAPI BringToFront();

    CARAPI GetParent(
        /* [out] */ IViewParent** parent);

    CARAPI GetScrollX(
        /* [out] */ Int32* x);

    CARAPI GetScrollY(
        /* [out] */ Int32* y);

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI GetDrawingRect(
        /* [in] */ IRect* outRect);

    CARAPI GetMeasuredWidth(
        /* [out] */ Int32* width);

    CARAPI GetMeasuredHeight(
        /* [out] */ Int32* height);

    CARAPI GetTop(
        /* [out] */ Int32* top);

    CARAPI GetBottom(
        /* [out] */ Int32* bottom);

    CARAPI GetLeft(
        /* [out] */ Int32* left);

    CARAPI GetRight(
        /* [out] */ Int32* right);

    CARAPI GetHitRect(
        /* [in] */ IRect* outRect);

    CARAPI GetFocusedRect(
        /* [in] */ IRect* outRect);

    CARAPI GetGlobalVisibleRect(
        /* [in] */ IRect* outRect,
        /* [in] */ IPoint* outGlobalOffset,
        /* [out] */ Boolean* result);

    CARAPI GetGlobalVisibleRectEx(
        /* [in] */ IRect* outRect,
        /* [out] */ Boolean* result);

    CARAPI GetLocalVisibleRect(
        /* [in] */ IRect* outRect,
        /* [out] */ Boolean* result);

    CARAPI OffsetTopAndBottom(
        /* [in] */ Int32 offset);

    CARAPI OffsetLeftAndRight(
        /* [in] */ Int32 offset);

    CARAPI GetLayoutParams(
        /* [out] */ IViewGroupLayoutParams** params);

    CARAPI SetLayoutParams(
        /* [in] */ IViewGroupLayoutParams* params);

    CARAPI ScrollTo(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI ScrollBy(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI Invalidate(
        /* [in] */ IRect* dirty);

    CARAPI InvalidateEx(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI InvalidateEx2();

    CARAPI IsOpaque(
        /* [out] */ Boolean* opaque);

    CARAPI GetHandler(
        /* [out] */ IApartment** apartment);

    CARAPI PostInvalidate();

    CARAPI PostInvalidateEx(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI PostInvalidateDelayed(
        /* [in] */ Int64 delayMilliseconds);

    CARAPI PostInvalidateDelayedEx(
        /* [in] */ Int64 delayMilliseconds,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI ComputeScroll();

    CARAPI IsHorizontalFadingEdgeEnabled(
        /* [out] */ Boolean* horizontalFadingEdgeEnabled);

    CARAPI SetHorizontalFadingEdgeEnabled(
        /* [in] */ Boolean horizontalFadingEdgeEnabled);

    CARAPI IsVerticalFadingEdgeEnabled(
        /* [out] */ Boolean* verticalFadingEdgeEnabled);

    CARAPI SetVerticalFadingEdgeEnabled(
        /* [in] */ Boolean verticalFadingEdgeEnabled);

    CARAPI IsHorizontalScrollBarEnabled(
        /* [out] */ Boolean* horizontalScrollBarEnabled);

    CARAPI SetHorizontalScrollBarEnabled(
        /* [in] */ Boolean horizontalScrollBarEnabled);

    CARAPI IsVerticalScrollBarEnabled(
        /* [out] */ Boolean* verticalScrollBarEnabled);

    CARAPI SetVerticalScrollBarEnabled(
        /* [in] */ Boolean verticalScrollBarEnabled);

    CARAPI SetScrollbarFadingEnabled(
        /* [in] */ Boolean fadeScrollbars);

    CARAPI IsScrollbarFadingEnabled(
        /* [out] */ Boolean* fadeScrollbars);

    CARAPI SetScrollBarStyle(
        /* [in] */ Int32 style);

    CARAPI GetScrollBarStyle(
        /* [out] */ Int32* style);

    CARAPI AssignParent(
        /* [in] */ IViewParent* parent);

    CARAPI GetWindowToken(
        /* [out] */ IBinder** token);

    CARAPI GetApplicationWindowToken(
        /* [out] */ IBinder** token);

    CARAPI SaveHierarchyState(
        /* [in] */ IObjectIntegerMap* container);

    CARAPI RestoreHierarchyState(
        /* [in] */ IObjectIntegerMap* container);

    CARAPI GetDrawingTime(
        /* [out] */ Int64* time);

    CARAPI SetDuplicateParentStateEnabled(
        /* [in] */ Boolean enabled);

    CARAPI IsDuplicateParentStateEnabled(
        /* [out] */ Boolean* result);

    CARAPI SetDrawingCacheEnabled(
        /* [in] */ Boolean enabled);

    CARAPI IsDrawingCacheEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI GetDrawingCache(
        /* [out] */ IBitmap** bitmap);

    CARAPI GetDrawingCacheEx(
        /* [in] */ Boolean autoScale,
        /* [out] */ IBitmap** bitmap);

    CARAPI DestroyDrawingCache();

    CARAPI SetDrawingCacheBackgroundColor(
        /* [in] */ Int32 color);

    CARAPI GetDrawingCacheBackgroundColor(
        /* [out] */ Int32* color);

    CARAPI BuildDrawingCache();

    CARAPI BuildDrawingCacheEx(
        /* [in] */ Boolean autoScale);

    CARAPI CreateSnapshot(
        /* [in] */ BitmapConfig quality,
        /* [in] */ Int32 backgroundColor,
        /* [in] */ Boolean skipChildren,
        /* [out] */ IBitmap** bitmap);

    CARAPI IsInEditMode(
        /* [out] */ Boolean* result);

    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    CARAPI GetSolidColor(
        /* [out] */ Int32* color);

    CARAPI OnFinishInflate();

    CARAPI IsLayoutRequested(
        /* [out] */ Boolean* result);

    CARAPI Layout(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI GetResources(
        /* [out] */ IResources** resources);

    CARAPI UnscheduleDrawableEx(
        /* [in] */ IDrawable* who);

    CARAPI RefreshDrawableState();

    CARAPI GetDrawableState(
        /* [out, callee] */ ArrayOf<Int32>** state);

    CARAPI SetBackgroundColor(
        /* [in] */ Int32 color);

    CARAPI SetBackgroundResource(
        /* [in] */ Int32 resid);

    CARAPI SetBackgroundDrawable(
        /* [in] */ IDrawable* d);

    CARAPI GetBackground(
        /* [out] */ IDrawable** drawable);

    CARAPI SetPadding(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI GetPaddingTop(
        /* [out] */ Int32* top);

    CARAPI GetPaddingBottom(
        /* [out] */ Int32* bottom);

    CARAPI GetPaddingLeft(
        /* [out] */ Int32* left);

    CARAPI GetPaddingRight(
        /* [out] */ Int32* right);

    CARAPI SetSelected(
        /* [in] */ Boolean selected);

    CARAPI IsSelected(
        /* [out] */ Boolean* selected);

    CARAPI GetViewTreeObserver(
        /* [out] */ IViewTreeObserver** observer);

    CARAPI GetRootView(
        /* [out] */ IView** view);

    CARAPI GetLocationOnScreen(
        /* [out] */ Int32* x,
        /* [out] */ Int32* y);

    CARAPI GetLocationInWindow(
        /* [out] */ Int32* x,
        /* [out] */ Int32* y);

    CARAPI FindViewById(
        /* [in] */ Int32 id,
        /* [out] */ IView** view);

    CARAPI SetId(
        /* [in] */ Int32 id);

    CARAPI GetId(
        /* [out] */ Int32* id);

    CARAPI IsRootNamespace(
        /* [out] */ Boolean* result);

    CARAPI GetBaseline(
        /* [out] */ Int32* result);

    CARAPI RequestLayout();

    CARAPI ForceLayout();

    CARAPI Measure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI SetMinimumHeight(
        /* [in] */ Int32 minHeight);

    CARAPI SetMinimumWidth(
        /* [in] */ Int32 minWidth);

    CARAPI GetAnimation(
        /* [out] */ IAnimation** animation);

    CARAPI StartAnimation(
        /* [in] */ IAnimation* animation);

    CARAPI ClearAnimation();

    CARAPI SetAnimation(
        /* [in] */ IAnimation* animation);

    CARAPI GatherTransparentRegion(
        /* [in] */ IRegion* region,
        /* [out] */ Boolean* result);

    CARAPI PlaySoundEffect(
        /* [in] */ Int32 soundConstant);

    CARAPI PerformHapticFeedback(
        /* [in] */ Int32 feedbackConstant,
        /* [out] */ Boolean* result);

    CARAPI PerformHapticFeedbackEx(
        /* [in] */ Int32 feedbackConstant,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* result);

    CARAPI OnCloseSystemDialogs(
        /* [in] */ const String& reason);

    CARAPI ApplyDrawableToTransparentRegion(
        /* [in] */ IDrawable* dr,
        /* [in] */ IRegion* region);

    CARAPI GetOverScrollMode(
        /* [out] */ Int32* mode);

    CARAPI SetOverScrollMode(
        /* [in] */ Int32 overScrollMode);
#endif

#if 0
    CARAPI GetDescendantFocusability(
        /* [out] */ Int32* focusability);

    CARAPI SetDescendantFocusability(
        /* [in] */ Int32 focusability);

    CARAPI GetFocusedChild(
        /* [out] */ IView** focusedChild);

    CARAPI OnInterceptTouchEvent(
        /* [in] */ IMotionEvent* ev,
        /* [out] */ Boolean* value);

    CARAPI SetClipChildren(
        /* [in] */ Boolean clipChildren);

    CARAPI SetClipToPadding(
        /* [in] */ Boolean clipToPadding);

    CARAPI AddView(
        /* [in] */ IView* child);

    CARAPI AddViewEx(
        /* [in] */ IView* child,
        /* [in] */ Int32 index);

    CARAPI AddViewEx2(
        /* [in] */ IView* child,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI AddViewEx3(
        /* [in] */ IView* child,
        /* [in] */ IViewGroupLayoutParams* params);

    CARAPI AddViewEx4(
        /* [in] */ IView* child,
        /* [in] */ Int32 index,
        /* [in] */ IViewGroupLayoutParams* params);

    CARAPI SetOnHierarchyChangeListener(
        /* [in] */ IViewGroupOnHierarchyChangeListener* listener);

    CARAPI RemoveViewInLayout(
        /* [in] */ IView* view);

    CARAPI RemoveViewsInLayout(
        /* [in] */ Int32 start,
        /* [in] */ Int32 count);

    CARAPI RemoveViewAt(
        /* [in] */ Int32 index);

    CARAPI RemoveViews(
        /* [in] */ Int32 start,
        /* [in] */ Int32 count);

    CARAPI RemoveAllViews();

    CARAPI RemoveAllViewsInLayout();

    CARAPI OffsetDescendantRectToMyCoords(
        /* [in] */ IView* descendant,
        /* [in] */ IRect* rect);

    CARAPI OffsetRectIntoDescendantCoords(
        /* [in] */ IView* descendant,
        /* [in] */ IRect* rect);

    CARAPI OffsetChildrenTopAndBottom(
        /* [in] */ Int32 offset);

    CARAPI StartLayoutAnimation();

    CARAPI ScheduleLayoutAnimation();

    CARAPI SetLayoutAnimation(
        /* [in] */ ILayoutAnimationController* controller);

    CARAPI GetLayoutAnimation(
        /* [out] */ ILayoutAnimationController** controller);

    CARAPI IsAnimationCacheEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetAnimationCacheEnabled(
        /* [in] */ Boolean enabled);

    CARAPI IsAlwaysDrawnWithCacheEnabled(
        /* [out] */ Boolean* always);

    CARAPI SetAlwaysDrawnWithCacheEnabled(
        /* [in] */ Boolean always);

    CARAPI GetPersistentDrawingCache(
        /* [out] */ Int32* drawingCacheToKeep);

    CARAPI SetPersistentDrawingCache(
        /* [in] */ Int32 drawingCacheToKeep);

    CARAPI GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IViewGroupLayoutParams** layoutParams);

    CARAPI IndexOfChild(
        /* [in] */ IView* child,
        /* [out] */ Int32* index);

    CARAPI GetChildCount(
        /* [out] */ Int32* count);

    CARAPI GetChildAt(
        /* [in] */ Int32 index,
        /* [out] */ IView** child);

    CARAPI ClearDisappearingChildren();

    CARAPI GetLayoutAnimationListener(
        /* [out] */ IAnimationListener** listener);

    CARAPI SetAddStatesFromChildren(
        /* [in] */ Boolean addsStates);

    CARAPI AddStatesFromChildren(
        /* [out] */ Boolean* drawable);

    CARAPI SetLayoutAnimationListener(
        /* [in] */ IAnimationListener* animationListener);
#endif

    CARAPI SetHorizontalScrollbarOverlay(
        /* [in] */ Boolean overlay);

    CARAPI SetVerticalScrollbarOverlay(
        /* [in] */ Boolean overlay);

    CARAPI OverlayHorizontalScrollbar(
        /* [out] */ Boolean* bFlag);

    CARAPI OverlayVerticalScrollbar(
        /* [out] */ Boolean* bFlag);

    CARAPI GetCertificate(
        /* [out] */ ISslCertificate** sc);

    CARAPI SetCertificate(
        /* [in] */ ISslCertificate* certificate);

    CARAPI SavePassword(
        /* [in] */ const String& host,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    CARAPI SetHttpAuthUsernamePassword(
        /* [in] */ const String& host,
        /* [in] */ const String& realm,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    CARAPI GetHttpAuthUsernamePassword(
        /* [in] */ const String& host,
        /* [in] */ const String& realm,
        /* [out] */ ArrayOf<String>* strlist);

    CARAPI Destroy();

    CARAPI EnablePlatformNotifications();

    CARAPI DisablePlatformNotifications();

    CARAPI SetJsFlags(
        /* [in] */ const String& flags);

    CARAPI SetNetworkAvailable(
        /* [in] */ Boolean networkUp);

    CARAPI SetNetworkType(
        /* [in] */ const String& type,
        /* [in] */ const String& subtype);

    CARAPI SaveState(
        /* [in] */ IBundle* outState,
        /* [out] */ IWebBackForwardList** wfl);

    CARAPI SavePicture(
        /* [in] */ IBundle* b,
        /* [in] */ IFile* dest,
        /* [out] */ Boolean* bFlag);

    CARAPI RestorePicture(
        /* [in] */ IBundle* b,
        /* [in] */ IFile* src,
        /* [out] */ Boolean* bFlag);

    CARAPI RestoreState(
        /* [in] */ IBundle* inState,
        /* [out] */ IWebBackForwardList** wfl);

    CARAPI LoadUrlEx(
        /* [in] */ const String& url,
        /* [in] */ IObjectStringMap* extraHeaders);

    CARAPI LoadUrl(
        /* [in] */ const String& url);

    CARAPI PostUrl(
        /* [in] */ const String& url,
        /* [in] */ const ArrayOf<Byte>& postData);

    CARAPI LoadData(
        /* [in] */ const String& data,
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding);

    CARAPI LoadDataWithBaseURL(
        /* [in] */ const String& baseUrl,
        /* [in] */ const String& data,
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding,
        /* [in] */ const String& historyUrl);

    CARAPI StopLoading();

    CARAPI Reload();

    CARAPI CanGoBack(
        /* [out] */ Boolean* bFlag);

    CARAPI GoBack();

    CARAPI CanGoForward(
        /* [out] */ Boolean* bFlag);

    CARAPI GoForward();

    CARAPI CanGoBackOrForward(
        /* [in] */ Int32 steps,
        /* [out] */ Boolean* bFlag);

    CARAPI GoBackOrForward(
        /* [in] */ Int32 steps);

    CARAPI PageUp(
        /* [in] */ Boolean top,
        /* [out] */ Boolean* bFlag);

    CARAPI PageDown(
        /* [in] */ Boolean bottom,
        /* [out] */ Boolean* bFlag);

    CARAPI ClearView();

    CARAPI CapturePicture(
        /* [out] */ IPicture** pic);

    CARAPI GetScale(
        /* [out] */ Float* scale);

    CARAPI SetInitialScale(
        /* [in] */ Int32 scaleInPercent);

    CARAPI InvokeZoomPicker();

    CARAPI GetHitTestResult(
        /* [out] */ IHitTestResult** result);

    CARAPI RequestFocusNodeHref(
        /* [in] */ IMessage* hrefMsg);

    CARAPI RequestImageRef(
        /* [in] */ IMessage* msg);

    CARAPI SetEmbeddedTitleBar(
        /* [in] */ IView* v);

    CARAPI GetUrl(
        /* [out] */ String* str);

    CARAPI GetOriginalUrl(
        /* [out] */ String* str);

    CARAPI GetTitle(
        /* [out] */ String* str);

    CARAPI GetFavicon(
        /* [out] */ IBitmap** bm);

    CARAPI GetTouchIconUrl(
        /* [out] */ String* str);

    CARAPI GetProgress(
        /* [out] */ Int32* progress);

    CARAPI GetContentHeight(
        /* [out] */ Int32* progress);

    CARAPI GetContentWidth(
        /* [out] */ Int32* width);

    CARAPI PauseTimers();

    CARAPI ResumeTimers();

    CARAPI OnPause();

    CARAPI OnResume();

    CARAPI IsPaused(
        /* [out] */ Boolean* bFlag);

    CARAPI FreeMemory();

    CARAPI ClearCache(
        /* [in] */ Boolean includeDiskFiles);

    CARAPI ClearFormData();

    CARAPI ClearHistory();

    CARAPI ClearSslPreferences();

    CARAPI CopyBackForwardList(
        /* [out] */ IWebBackForwardList** wfl);

    CARAPI FindNext(
        /* [in] */ Boolean forward);

    CARAPI FindAll(
        /* [in] */ const String& find,
        /* [out] */ Int32* f);

    CARAPI SetFindIsUp(
        /* [in] */ Boolean isUp);

    CARAPI FindIndex(
        /* [out] */ Int32* index);

    CARAPI FindAddress(
        /* [in] */ const String& addr,
        /* [out] */ String* address);

    CARAPI FindAddressEx(
        /* [in] */ const String& addr,
        /* [in] */ Boolean caseInsensitive,
        /* [out] */ String* address);

    CARAPI ClearMatches();

    CARAPI NotifyFindDialogDismissed();

    CARAPI SetFindDialogHeight(
        /* [in] */ Int32 height);

    CARAPI DocumentHasImages(
        /* [in] */ IMessage* response);

    CARAPI SetWebViewClient(
        /* [in] */ IWebViewClient* client);

    CARAPI GetWebViewClient(
        /* [out] */ IWebViewClient** wc);

    CARAPI SetDownloadListener(
        /* [in] */ IDownloadListener* listener);

    CARAPI SetWebChromeClient(
        /* [in] */ IWebChromeClient* client);

    CARAPI GetWebChromeClient(
        /* [out] */ IWebChromeClient** wcc);

    CARAPI SetWebBackForwardListClient(
        /* [in] */ IWebBackForwardListClient* client);

    CARAPI GetWebBackForwardListClient(
        /* [out] */ IWebBackForwardListClient** wfl);

    CARAPI SetPictureListener(
        /* [in] */ IPictureListener* listener);

    CARAPI ExternalRepresentation(
        /* [in] */ IMessage* callBack);

    CARAPI DocumentAsText(
        /* [in] */ IMessage* callBack);

    CARAPI AddJavascriptInterface(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& interfaceName);

    CARAPI GetSettings(
        /* [out] */ IWebSettings** ws);

    CARAPI AddPackageNames(
        /* [in] */ IObjectContainer* packageNames);

    CARAPI AddPackageName(
        /* [in] */ const String& packageName);

    CARAPI RemovePackageName(
        /* [in] */ const String& packageName);

    CARAPI GetPluginList(
        /* [out] */ IPluginList** pl);

    CARAPI RefreshPlugins(
        /* [in] */ Boolean reloadOpenPages);

    CARAPI DumpDisplayTree();

    CARAPI DumpDomTree(
        /* [in] */ Boolean toFile);

    CARAPI DumpRenderTree(
        /* [in] */ Boolean toFile);

    CARAPI DumpV8Counters();

    CARAPI SetUpSelect();

    CARAPI EmulateShiftHeld();

    CARAPI SelectAll();

    CARAPI SelectDialogIsUp(
        /* [out] */ Boolean* bFlag);

    CARAPI NotifySelectDialogDismissed();

    CARAPI SelectionDone();

    CARAPI CopySelection(
        /* [out] */ Boolean* bFlag);

    CARAPI GetSelection(
        /* [out] */ String* selection);

    CARAPI OnChildViewAdded(
        /* [in] */ IView* parent,
        /* [in] */ IView* child);

    CARAPI OnChildViewRemoved(
        /* [in] */ IView* p,
        /* [in] */ IView* child);

    CARAPI OnGlobalFocusChanged(
        /* [in] */ IView* oldFocus,
        /* [in] */ IView* newFocus);

    CARAPI GetDragTracker(
        /* [out] */ IDragTracker** dragTracker);

    CARAPI SetDragTracker(
        /* [in] */ IDragTracker* tracker);

    CARAPI SetMapTrackballToArrowKeys(
        /* [in] */ Boolean setMap);

    CARAPI FlingScroll(
        /* [in] */ Int32 vx,
        /* [in] */ Int32 vy);

    CARAPI GetZoomControls(
        /* [out] */ IView** view);

    CARAPI GetZoomButtonsController(
        /* [out] */ Handle32* zoom);

    CARAPI ZoomIn(
        /* [out] */ Boolean* bFlag);

    CARAPI ZoomOut(
        /* [out] */ Boolean* bFlag);

    CARAPI DebugDump();

    CARAPI DrawPage(
        /* [in] */ ICanvas* canvas);

    CARAPI SetTouchInterval(
        /* [in] */ Int32 interval);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

public:  

    static const CString LOGTAG;// = "webview";

    /*
     * Package message ids
     */
    //! arg1=x, arg2=y
    static const Int32 SCROLL_TO_MSG_ID                   = 101;
    static const Int32 SCROLL_BY_MSG_ID                   = 102;
    //! arg1=x, arg2=y
    static const Int32 SPAWN_SCROLL_TO_MSG_ID             = 103;
    //! arg1=x, arg2=y
    static const Int32 SYNC_SCROLL_TO_MSG_ID              = 104;
    static const Int32 NEW_PICTURE_MSG_ID                 = 105;
    static const Int32 UPDATE_TEXT_ENTRY_MSG_ID           = 106;
    static const Int32 WEBCORE_INITIALIZED_MSG_ID         = 107;
    static const Int32 UPDATE_TEXTFIELD_TEXT_MSG_ID       = 108;
    static const Int32 UPDATE_ZOOM_RANGE                  = 109;
    static const Int32 MOVE_OUT_OF_PLUGIN                 = 110;
    static const Int32 CLEAR_TEXT_ENTRY                   = 111;
    static const Int32 UPDATE_TEXT_SELECTION_MSG_ID       = 112;
    static const Int32 SHOW_RECT_MSG_ID                   = 113;
    static const Int32 LONG_PRESS_CENTER                  = 114;
    static const Int32 PREVENT_TOUCH_ID                   = 115;
    static const Int32 WEBCORE_NEED_TOUCH_EVENTS          = 116;
    // obj=Rect in doc coordinates
    static const Int32 INVAL_RECT_MSG_ID                  = 117;
    static const Int32 REQUEST_KEYBOARD                   = 118;
    static const Int32 DO_MOTION_UP                       = 119;
    static const Int32 SHOW_FULLSCREEN                    = 120;
    static const Int32 HIDE_FULLSCREEN                    = 121;
    static const Int32 DOM_FOCUS_CHANGED                  = 122;
    static const Int32 IMMEDIATE_REPAINT_MSG_ID           = 123;
    static const Int32 SET_ROOT_LAYER_MSG_ID              = 124;
    static const Int32 RETURN_LABEL                       = 125;
    static const Int32 FIND_AGAIN                         = 126;
    static const Int32 CENTER_FIT_RECT                    = 127;
    static const Int32 REQUEST_KEYBOARD_WITH_SELECTION_MSG_ID = 128;
    static const Int32 SET_SCROLLBAR_MODES                = 129;

    static const CString HandlerPrivateDebugString[];/* = {
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
    };*/

    static const CString HandlerPackageDebugString[]; /* = {
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
    };*/

    // If the site doesn't use the viewport meta tag to specify the viewport,
    // use DEFAULT_VIEWPORT_WIDTH as the default viewport width
    static const Int32 DEFAULT_VIEWPORT_WIDTH = 800;

public:
    struct ViewSizeData {
        Int32 mWidth;
        Int32 mHeight;
        Int32 mTextWrapWidth;
        Int32 mAnchorX;
        Int32 mAnchorY;
        Float mScale;
        Boolean mIgnoreHeight;
    };

    /**
     * General handler to receive message coming from webkit thread
     */
    class PrivateHandler : public IHandler {
        
    public:
        //@Override
        CARAPI_(void) HandleMessage(
            /* [in] */ IMessage* msg);
    };

public:
    CARAPI_(void) UpdateMultiTouchSupport(
        /* [in] */ IContext* context);

    /* package */
    CARAPI_(void) UpdateDefaultZoomDensity(
        /* [in] */ Int32 zoomDensity);

    /* package */
    CARAPI_(Boolean) OnSavePassword(
        /* [in] */ const String& schemePlusHost,
        /* [in] */ const String& username,
        /* [in] */ const String& password,
        /* [in] */ IApartment* resumeMsgHandle,
        /* [in] */ Int32 resumeMsgId,
        /* [in] */ IParcel* resumeMsgParams);

    /*
     * Return the width of the view where the content of WebView should render
     * to.
     * Note: this can be called from WebCoreThread.
     */
    /* package */
    CARAPI_(Int32) GetViewWidth();

    /**
     * Given an x coordinate in view space, convert it to content space.  Also
     * may be used for absolute heights (such as for the WebTextView's
     * textSize, which is unaffected by the height of the title bar).
     */
    /*package*/
    CARAPI_(Int32) ViewToContentX(
        /* [in] */ Int32 x);

    /**
     * Given a y coordinate in view space, convert it to content space.
     * Takes into account the height of the title bar if there is one
     * embedded into the WebView.
     */
    /*package*/
    CARAPI_(Int32) ViewToContentY(
        /* [in] */ Int32 y);

    /**
     * Given a distance in content space, convert it to view space. Note: this
     * does not reflect translation, just scaling, so this should not be called
     * with coordinates, but should be called for dimensions like width or
     * height.
     */
    /*package*/
    CARAPI_(Int32) ContentToViewDimension(
        /* [in] */ Int32 d);

    /**
     * Given an x coordinate in content space, convert it to view
     * space.
     */
    /*package*/
    CARAPI_(Int32) ContentToViewX(
        /* [in] */ Int32 x);

    /**
     * Given a y coordinate in content space, convert it to view
     * space.  Takes into account the height of the title bar.
     */
    /*package*/
    CARAPI_(Int32) ContentToViewY(
        /* [in] */ Int32 y);

    /*
     * Return the height of the view where the content of WebView should render
     * to.  Note that this excludes mTitleBar, if there is one.
     * Note: this can be called from WebCoreThread.
     */
    /* package */
    CARAPI_(Int32) GetViewHeight();

    /**
     * Called by CallbackProxy when the page finishes loading.
     * @param url The URL of the page which has finished loading.
     */
    /* package */
    CARAPI_(void) OnPageFinished(
        /* [in] */ const String& url);

    CARAPI_(Boolean) InAnimateZoom();

    // Only check the flag, can be called from WebCore thread
    CARAPI_(Boolean) DrawHistory();

    // Should only be called in UI thread
    CARAPI_(void) SwitchOutDrawHistory();

    CARAPI_(WebViewCore::CursorData*) CursorData();

    /**
     *  Delete text from start to end in the focused textfield. If there is no
     *  focus, or if start == end, silently fail.  If start and end are out of
     *  order, swap them.
     *  @param  start   Beginning of selection to delete.
     *  @param  end     End of selection to delete.
     */
    /* package */
    CARAPI_(void) DeleteSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     *  Set the selection to (start, end) in the focused textfield. If start and
     *  end are out of order, swap them.
     *  @param  start   Beginning of selection.
     *  @param  end     End of selection.
     */
    /* package */
    CARAPI_(void) SetSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /*
     * This method checks the current focus and cursor and potentially rebuilds
     * mWebTextView to have the appropriate properties, such as password,
     * multiline, and what text it contains.  It also removes it if necessary.
     */
    /* package */
    CARAPI_(void) RebuildWebTextView();

    /**
     * Called by WebTextView to find saved form data associated with the
     * textfield
     * @param name Name of the textfield.
     * @param nodePointer Pointer to the node of the textfield, so it can be
     *          compared to the currently focused textfield when the data is
     *          retrieved.
     */
    /* package */
    CARAPI_(void) RequestFormData(
        /* [in] */ const String& name,
        /* [in] */ Int32 nodePointer);

    /**
     * Pass a message to find out the <label> associated with the <input>
     * identified by nodePointer
     * @param framePointer Pointer to the frame containing the <input> node
     * @param nodePointer Pointer to the node for which a <label> is desired.
     */
    /* package */
    CARAPI_(void) RequestLabel(
        /* [in] */ Int32 framePointer,
        /* [in] */ Int32 nodePointer);

    /*
     * Pass a message to WebCore Thread, telling the WebCore::Page's
     * FocusController to be  "inactive" so that it will
     * not draw the blinking cursor.  It gets set to "active" to draw the cursor
     * in WebViewCore.cpp, when the WebCore thread receives key events/clicks.
     */
    /* package */
    CARAPI_(void) SetFocusControllerInactive();

    CARAPI_(void) ResetTrackballTime();

    CARAPI_(void) MoveSelection(
        /* [in] */ Float xRate,
        /* [in] */ Float yRate);

    /**
     * Scroll the focused text field/area to match the WebTextView
     * @param xPercent New x position of the WebTextView from 0 to 1.
     * @param y New y position of the WebTextView in view coordinates
     */
    /*package*/
    CARAPI_(void) ScrollFocusedTextInput(
        /* [in] */ Float xPercent,
        /* [in] */ Int32 y);

    /**
     * Set our starting point and time for a drag from the WebTextView.
     */
    /*package*/
    CARAPI_(void) InitiateTextFieldDrag(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int64 eventTime);

    /**
     * Given a motion event from the WebTextView, set its location to our
     * coordinates, and handle the event.
     */
    /*package*/
    CARAPI_(Boolean) TextFieldDrag(
        /* [in] */ IMotionEvent* event);

    /**
     * Due a touch up from a WebTextView.  This will be handled by webkit to
     * change the selection.
     * @param event MotionEvent in the WebTextView's coordinates.
     */
    /*package*/
    CARAPI_(void) TouchUpOnTextField(
        /* [in] */ IMotionEvent* event);

    /**
     * Called when pressing the center key or trackball on a textfield.
     */
    /*package*/
    CARAPI_(void) CenterKeyPressOnTextField();

         /*
     * Request a dropdown menu for a listbox with single selection or a single
     * <select> element.
     *
     * @param array Labels for the listbox.
     * @param enabledArray  State for each element in the list.  See static
     *      integers in Container class.
     * @param selection Which position is initally selected.
     */
    CARAPI_(void) RequestListBox(
        /* [in] */ ArrayOf<String>* array,
        /* [in] */ ArrayOf<Int32>* enabledArray,
        /* [in] */ Int32 selection);

    CARAPI_(void) DismissZoomControl();

    /* package */
    CARAPI_(void) ReplaceTextfieldText(
        /* [in] */ Int32 oldStart,
        /* [in] */ Int32 oldEnd,
        /* [in] */ const String& replace,
        /* [in] */ Int32 newStart,
        /* [in] */ Int32 newEnd);

    /* package */
    CARAPI_(void) PassToJavaScript(
        /* [in] */ const String& currentText,
        /* [in] */ IKeyEvent* event);

    /* package */
    /*synchronized*/
    CARAPI_(AutoPtr<WebViewCore>) GetWebViewCore();

    /*
     * Request a dropdown menu for a listbox with multiple selection.
     *
     * @param array Labels for the listbox.
     * @param enabledArray  State for each element in the list.  See static
     *      integers in Container class.
     * @param selectedArray Which positions are initally selected.
     */
    CARAPI_(void) RequestListBox(
        /* [in] */ ArrayOf<String>* array,
        /* [in] */ ArrayOf<Int32>*  enabledArray,
        /* [in] */ ArrayOf<Int32>*  selectedArray);

    /**
     *  Update our cache with updatedText.
     *  @param updatedText  The new text to put in our cache.
     */
    /* package */
    CARAPI_(void) UpdateCachedTextfield(
        /* [in] */ const String& updatedText);

    /* package */
    CARAPI_(AutoPtr<ViewManager>) GetViewManager();

    /* package */
    CARAPI_(void)     NativeClearCursor();
    /* package */
    CARAPI_(Boolean)  NativeCursorMatchesFocus();    
    /* package */
    CARAPI_(Int32)    NativeFocusCandidateFramePointer();
    /* package */
    CARAPI_(Boolean)  NativeFocusCandidateHasNextTextfield();
    /* package */
    CARAPI_(Boolean)  NativeFocusCandidateIsPassword();
    /* package */
    CARAPI_(Int32)    NativeFocusCandidateMaxLength();
    /* package */
    CARAPI_(String)   NativeFocusCandidateName();
    /* package */
    CARAPI_(Int32)    NativeFocusCandidatePointer();
    /* package */
    CARAPI_(Int32)    NativeFocusNodePointer();
    /* package */
    CARAPI_(Boolean)  NativeMoveCursorToNextTextInput();

public:

    // Handler for dispatching UI messages.
    /* package */
    const AutoPtr<IHandler> mPrivateHandler;// = new PrivateHandler();

    // Used by WebViewCore to create child views.
    /* package */
    /*const*/ AutoPtr<ViewManager> mViewManager;

    // Used to display in full screen mode
    PluginFullScreenHolder* mFullScreenHolder;

    /**
     * Helper class to get velocity for fling
     */
    VelocityTracker* mVelocityTracker;

    // To keep track of whether the current drag was initiated by a WebTextView,
    // so that we know not to hide the cursor
    Boolean mDragFromTextInput;

    /**
     * These prevent calling requestLayout if either dimension is fixed. This
     * depends on the layout parameters and the measure specs.
     */
    Boolean mWidthCanMeasure;
    Boolean mHeightCanMeasure;

    // Remember the last dimensions we sent to the native side so we can avoid
    // sending the same dimensions more than once.
    Int32 mLastWidthSent;
    Int32 mLastHeightSent;

    // while in the zoom overview mode, the page's width is fully fit to the
    // current window. The page is alive, in another words, you can click to
    // follow the links. Double tap will toggle between zoom overview mode and
    // the last zoom scale.
    Boolean mInZoomOverview;// = false;

    // ideally mZoomOverviewWidth should be mContentWidth. But sites like espn,
    // engadget always have wider mContentWidth no matter what viewport size is.
    Int32 mZoomOverviewWidth;// = DEFAULT_VIEWPORT_WIDTH;
    Float mTextWrapScale;

    // default scale. Depending on the display density.
    static Int32 DEFAULT_SCALE_PERCENT;

    /* package */
    static Boolean mLogEvent;// = true;

    // normally we try to fit the content to the minimum preferred width
    // calculated by the Webkit. To avoid the bad behavior when some site's
    // minimum preferred width keeps growing when changing the viewport width or
    // the minimum preferred width is huge, an upper limit is needed.
    static Int32 sMaxViewportWidth;// = DEFAULT_VIEWPORT_WIDTH;

protected:

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
    CWebView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle,
        /* [in] */ IObjectStringMap* javascriptInterfaces);

    //@Override
    CARAPI_(Int32) ComputeHorizontalScrollRange();

    //@Override
    CARAPI_(Int32) ComputeHorizontalScrollOffset();

    //@Override
    CARAPI_(Int32) ComputeVerticalScrollRange();

    //@Override
    CARAPI_(Int32) ComputeVerticalScrollOffset();

    //@Override
    CARAPI_(Int32) ComputeVerticalScrollExtent();

    /** @hide */
    //@Override
    CARAPI_(void) OnDrawVerticalScrollBar(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IDrawable* scrollBar,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    //@Override
    CARAPI_(void) OnOverScrolled(
        /* [in] */ Int32 scrollX,
        /* [in] */ Int32 scrollY,
        /* [in] */ Boolean clampedX,
        /* [in] */ Boolean clampedY);


    //-------------------------------------------------------------------------
    // Override View methods
    //-------------------------------------------------------------------------

    //@Override
    CARAPI_(void) Finalize();

    //@Override
    CARAPI_(Boolean) DrawChild(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IView* child,
        /* [in] */ Int64 drawingTime);    

    //@Override
    CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    //@Override
    CARAPI_(void) OnAttachedToWindow();

    //@Override
    CARAPI_(void) OnDetachedFromWindow();

    //@Override
    CARAPI_(void) OnFocusChanged(
        /* [in] */ Boolean focused,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    /**
     * @hide
     */
    //@Override
    CARAPI_(Boolean) SetFrame(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);
    

    //@Override
    CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 ow,
        /* [in] */ Int32 oh);

    //@Override
    CARAPI_(void) OnScrollChanged(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 oldl,
        /* [in] */ Int32 oldt);

    //@Override
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

private:

    // if AUTO_REDRAW_HACK is true, then the CALL key will toggle redrawing
    // the screen all-the-time. Good for profiling our drawing code
    static const Boolean AUTO_REDRAW_HACK = false;

    // enable debug output for drag trackers
    static const Boolean DEBUG_DRAG_TRACKER = false;

    /**
     * The minimum elapsed time before sending another ACTION_MOVE event to
     * WebViewCore. This really should be tuned for each type of the devices.
     * For example in Google Map api test case, it takes Dream device at least
     * 150ms to do a full cycle in the WebViewCore by processing a touch event,
     * triggering the layout and drawing the picture. While the same process
     * takes 60+ms on the current high speed device. If we make
     * TOUCH_SENT_INTERVAL too small, there will be multiple touch events sent
     * to WebViewCore queue and the real layout and draw events will be pushed
     * to further, which slows down the refresh rate. Choose 50 to favor the
     * current high speed devices. For Dream like devices, 100 is a better
     * choice. Maybe make this in the buildspec later.
     */
    static const Int32 TOUCH_SENT_INTERVAL;// = 50;

    /**
     * Touch mode
     */
    Int32 mTouchMode;// = TOUCH_DONE_MODE;
    static const Int32 TOUCH_INIT_MODE = 1;
    static const Int32 TOUCH_DRAG_START_MODE = 2;
    static const Int32 TOUCH_DRAG_MODE = 3;
    static const Int32 TOUCH_SHORTPRESS_START_MODE = 4;
    static const Int32 TOUCH_SHORTPRESS_MODE = 5;
    static const Int32 TOUCH_DOUBLE_TAP_MODE = 6;
    static const Int32 TOUCH_DONE_MODE = 7;
    static const Int32 TOUCH_PINCH_DRAG = 8;

    // Whether to prevent default during touch. The initial value depends on
    // mForwardTouchEvents. If WebCore wants all the touch events, it says yes
    // for touch down. Otherwise UI will wait for the answer of the first
    // confirmed move before taking over the control.
    static const Int32 PREVENT_DEFAULT_NO = 0;
    static const Int32 PREVENT_DEFAULT_MAYBE_YES = 1;
    static const Int32 PREVENT_DEFAULT_NO_FROM_TOUCH_DOWN = 2;
    static const Int32 PREVENT_DEFAULT_YES = 3;
    static const Int32 PREVENT_DEFAULT_IGNORE = 4;
    Int32 mPreventDefault;// = PREVENT_DEFAULT_IGNORE;

    // This should be ViewConfiguration.getTapTimeout()
    // But system time out is 100ms, which is too short for the browser.
    // In the browser, if it switches out of tap too soon, jump tap won't work.
    static const Int32 TAP_TIMEOUT = 200;
    // This should be ViewConfiguration.getLongPressTimeout()
    // But system time out is 500ms, which is too short for the browser.
    // With a short timeout, it's difficult to treat trigger a short press.
    static const Int32 LONG_PRESS_TIMEOUT = 1000;
    // needed to avoid flinging after a pause of no movement
    static const Int32 MIN_FLING_TIME = 250;
    // draw unfiltered after drag is held without movement
    static const Int32 MOTIONLESS_TIME = 100;
    // The time that the Zoom Controls are visible before fading away
    static const Int64 ZOOM_CONTROLS_TIMEOUT;// = ViewConfiguration.getZoomControlsTimeout();
    // The amount of content to overlap between two screens when going through
    // pages with the space bar, in pixels.
    static const Int32 PAGE_SCROLL_OVERLAP = 24;

    // our standard speed. this way small distances will be traversed in less
    // time than large distances, but we cap the duration, so that very large
    // distances won't take too long to get there.
    static const Int32 STD_SPEED = 480;  // pixels per second
    // time for the longest scroll animation
    static const Int32 MAX_DURATION = 750;   // milliseconds
    static const Int32 SLIDE_TITLE_DURATION = 500;   // milliseconds
    OverScroller* mScroller;
    Boolean mInOverScrollMode;// = false;
    static AutoPtr<IPaint> mOverScrollBackground;
    static AutoPtr<IPaint> mOverScrollBorder;

    Boolean mWrapContent;
    static const Int32 MOTIONLESS_FALSE           = 0;
    static const Int32 MOTIONLESS_PENDING         = 1;
    static const Int32 MOTIONLESS_TRUE            = 2;
    static const Int32 MOTIONLESS_IGNORE          = 3;
    Int32 mHeldMotionless;

    /*
     * Private message ids
     */
    static const Int32 REMEMBER_PASSWORD          = 1;
    static const Int32 NEVER_REMEMBER_PASSWORD    = 2;
    static const Int32 SWITCH_TO_SHORTPRESS       = 3;
    static const Int32 SWITCH_TO_LONGPRESS        = 4;
    static const Int32 RELEASE_SINGLE_TAP         = 5;
    static const Int32 REQUEST_FORM_DATA          = 6;
    static const Int32 RESUME_WEBCORE_PRIORITY    = 7;
    static const Int32 DRAG_HELD_MOTIONLESS       = 8;
    static const Int32 AWAKEN_SCROLL_BARS         = 9;
    static const Int32 PREVENT_DEFAULT_TIMEOUT    = 10;

    static const Int32 FIRST_PRIVATE_MSG_ID = REMEMBER_PASSWORD;
    static const Int32 LAST_PRIVATE_MSG_ID = PREVENT_DEFAULT_TIMEOUT;

    static const Int32 FIRST_PACKAGE_MSG_ID = SCROLL_TO_MSG_ID;
    static const Int32 LAST_PACKAGE_MSG_ID = SET_SCROLLBAR_MODES;

    // default scale limit. Depending on the display density
    static Float DEFAULT_MAX_ZOOM_SCALE;
    static Float DEFAULT_MIN_ZOOM_SCALE;

    static Float MINIMUM_SCALE_INCREMENT;// = 0.01f;

    static const Int32 SNAP_NONE = 0;
    static const Int32 SNAP_LOCK = 1; // not a separate state
    static const Int32 SNAP_X = 2; // may be combined with SNAP_LOCK
    static const Int32 SNAP_Y = 4; // may be combined with SNAP_LOCK
    Boolean mSnapPositive;

    // keep these in sync with their counterparts in WebView.cpp
    static const Int32 DRAW_EXTRAS_NONE = 0;
    static const Int32 DRAW_EXTRAS_FIND = 1;
    static const Int32 DRAW_EXTRAS_SELECTION = 2;
    static const Int32 DRAW_EXTRAS_CURSOR_RING = 3;

    // keep this in sync with WebCore:ScrollbarMode in WebKit
    static const Int32 SCROLLBAR_AUTO = 0;
    static const Int32 SCROLLBAR_ALWAYSOFF = 1;
    // as we auto fade scrollbar, this is ignored.
    static const Int32 SCROLLBAR_ALWAYSON = 2;
    Int32 mHorizontalScrollBarMode;// = SCROLLBAR_AUTO;
    Int32 mVerticalScrollBarMode;// = SCROLLBAR_AUTO;

    // if the page can scroll <= this value, we won't allow the drag tracker
    // to have any effect.
    static const Int32 MIN_SCROLL_AMOUNT_TO_DISABLE_DRAG_TRACKER = 4;

    // return NO_LEFTEDGE means failure.
    static const Int32 NO_LEFTEDGE = -1;

private:
    class ExtendedZoomControls : public ElRefBase, FrameLayout
    {

    public:
        ExtendedZoomControls(
            /* [in] */ IContext* context,
            /* [in] */ IAttributeSet* attrs);

        CARAPI_(void) Show(
            /* [in] */ Boolean showZoom,
            /* [in] */ Boolean canZoomOut);

        CARAPI_(void) Hide();        

        CARAPI_(Boolean) HasFocus();

        CARAPI_(void) SetOnZoomInClickListener(
            /* [in] */ IViewOnClickListener* listener);

        CARAPI_(void) SetOnZoomOutClickListener(
            /* [in] */ IViewOnClickListener* listener);

    public:

//        ZoomControls    mPlusMinusZoomControls;

    private:
        CARAPI_(void) Fade(
            /* [in] */ Int32 visibility,
            /* [in] */ Float startAlpha,
            /* [in] */ Float endAlpha);
    };

        /*
     * This class requests an Adapter for the WebTextView which shows past
     * entries stored in the database.  It is a Runnable so that it can be done
     * in its own thread, without slowing down the UI.
     */
    class RequestFormData //: public Runnable
    {
    public:
        RequestFormData(
            /* [in] */ const String& name,
            /* [in] */ const String& url,
            /* [in] */ IMessage* msg);

        CARAPI_(void) Run();

    private:
        String mName;
        String mUrl;
        AutoPtr<IMessage> mUpdateMessage;        
    };

    class PostScale //: public Runnable
    {
    public:
        PostScale(
            /* [in] */ IWebView* webView,
            /* [in] */ Boolean updateTextWrap);

        CARAPI_(void) Run();

    public:
        const AutoPtr<IWebView> mWebView;
        const Boolean mUpdateTextWrap;        
    };

    class DragTrackerHandler : public ElRefBase {
    public:
        DragTrackerHandler(
            /* [in] */ Float x,
            /* [in] */ Float y,
            /* [in] */ IDragTracker* proxy);

        CARAPI_(void) DragTo(
            /* [in] */ Float x,
            /* [in] */ Float y);

        CARAPI_(void) StopDrag();

        // Call this after each draw. If it ruturns null, the tracker is done
        CARAPI_(Boolean) IsFinished();

        /*  If the tracker draws, then this returns true, otherwise it will
            return false, and draw nothing.
         */
        CARAPI_(Boolean) Draw(
            /* [in] */ ICanvas* canvas);

    private:
        CARAPI_(Float) ComputeStretch(
            /* [in] */ Float delta,
            /* [in] */ Float min,
            /* [in] */ Float max);        

        CARAPI_(Int32) HiddenHeightOfTitleBar();

        // need a way to know if 565 or 8888 is the right config for
        // capturing the display and giving it to the drag proxy
        CARAPI_(BitmapConfig) OffscreenBitmapConfig();     

        CARAPI_(void) BuildBitmap(
            /* [in] */ Int32 sx,
            /* [in] */ Int32 sy);

    private:

        /*const*/ AutoPtr<IDragTracker> mProxy;
        /*const*/ Float mStartY, mStartX;
        /*const*/ Float mMinDY, mMinDX;
        /*const*/ Float mMaxDY, mMaxDX;
        Float mCurrStretchY, mCurrStretchX;
        Int32 mSX, mSY;
        AutoPtr<IInterpolator> mInterp;
        Float mXY[2];// = new float[2];

        // inner (non-state) classes can't have enums :(
        static const Int32 DRAGGING_STATE = 0;
        static const Int32 ANIMATING_STATE = 1;
        static const Int32 FINISHED_STATE = 2;
        Int32 mState;

        CWebView* webview;   
    };

    class ScaleDetectorListener //: public ScaleGestureDetector.OnScaleGestureListener 
    {
    public:
//        CARAPI_(Boolean) OnScaleBegin(ScaleGestureDetector detector);

//        CARAPI_(void) OnScaleEnd(ScaleGestureDetector detector);

//        CARAPI_(Boolean) OnScale(ScaleGestureDetector detector);
    };

        // Class used to use a dropdown for a <select> element
    class InvokeListBox //: public Runnable
    {
    public:
        CARAPI_(void) Run();

    private:       

        // Need these to provide stable ids to my ArrayAdapter,
        // which normally does not have stable ids. (Bug 1250098)
        class Container /*extends Object*/ {
        public:
            /**
             * Possible values for mEnabled.  Keep in sync with OptionStatus in
             * WebViewCore.cpp
             */
            const static Int32 OPTGROUP = -1;
            const static Int32 OPTION_DISABLED = 0;
            const static Int32 OPTION_ENABLED = 1;

        public:
            CARAPI_(String) ToString();

        public:
            String  mString;
            Int32     mEnabled;
            Int32     mId;            
        };

        /**
         *  Subclass ArrayAdapter so we can disable OptionGroupLabels,
         *  and allow filtering.
         */
        class MyArrayListAdapter //: public ArrayAdapter<Container>
        {
        public:
            MyArrayListAdapter(
                /* [in] */ IContext* context,
//                /* [in] */ Container[] objects,
                /* [in] */ Boolean multiple);

        public:
            //@Override
            CARAPI_(AutoPtr<IView>) GetView(
                /* [in] */ Int32 position,
                /* [in] */ IView* convertView,
                /* [in] */ IViewGroup* parent);

            //@Override
            CARAPI_(Boolean) HasStableIds();            

            //@Override
            CARAPI_(Int64) GetItemId(
                /* [in] */ Int32 position);

            //@Override
            CARAPI_(Boolean) AreAllItemsEnabled();

            //@Override
            CARAPI_(Boolean) IsEnabled(
                /* [in] */ Int32 position);

        private:
//            private Container item(int position);
        };

        

        /*
         * Whenever the data set changes due to filtering, this class ensures
         * that the checked item remains checked.
         */
        class SingleDataSetObserver : public DataSetObserver {
        public:
            /*
             * Create a new observer.
             * @param id The ID of the item to keep checked.
             * @param l ListView for getting and clearing the checked states
             * @param a Adapter for getting the IDs
             */
            SingleDataSetObserver(
                /* [in] */ Int64 id,
                /* [in] */ IListView* l, 
                /* [in] */ IAdapter* a);

            CARAPI OnChanged();

            CARAPI_(void) OnInvalidate();

        private:
            Int64                 mCheckedId;
            AutoPtr<IListView>    mListView;
            AutoPtr<IAdapter>     mAdapter;            
        };

    private:
        InvokeListBox(
            /* [in] */ ArrayOf<String>& array,
            /* [in] */ ArrayOf<Int32>& enabled,
            /* [in] */ ArrayOf<Int32>& selected);

        InvokeListBox(
            /* [in] */ ArrayOf<String>& array,
            /* [in] */ ArrayOf<Int32> enabled,
            /* [in] */ ArrayOf<Int32> selection);

    private:
        // Whether the listbox allows multiple selection.
        Boolean     mMultiple;
        // Passed in to a list with multiple selection to tell
        // which items are selected.
        AutoFree<ArrayOf<Int32> >       mSelectedArray;
        // Passed in to a list with single selection to tell
        // where the initial selection is.
        Int32         mSelection;

        AutoFree<ArrayOf<Container> > mContainers;        
    };

private:

    CARAPI_(void) UpdateZoomButtonsEnabled();

    CARAPI_(void) Init();

    /*
     * returns the height of the titlebarview (if any). Does not care about
     * scrolling
     */
    CARAPI_(Int32) GetTitleHeight();

    /*
     * Return the amount of the titlebarview (if any) that is visible
     */
    CARAPI_(Int32) GetVisibleTitleHeight();

    CARAPI_(Int32) GetViewHeightWithTitle();

    CARAPI_(void) ClearHelpers();

    CARAPI_(void) RestoreHistoryPictureFields(
        /* [in] */ IPicture* p,
        /* [in] */ IBundle* b);

    CARAPI_(void) GoBackOrForward(
        /* [in] */ Int32 steps,
        /* [in] */ Boolean ignoreSnapshot);

    CARAPI_(Boolean) ExtendScroll(
        /* [in] */ Int32 y);

    /**
     *  Return true if the browser is displaying a TextView for text input.
     */
    CARAPI_(Boolean) InEditingMode();

    /**
     * Remove the WebTextView.
     * @param disableFocusController If true, send a message to webkit
     *     disabling the focus controller, so the caret stops blinking.
     */
    CARAPI_(void) ClearTextEntry(
        /* [in] */ Boolean disableFocusController);

    // Called by JNI when the DOM has changed the focus.  Clear the focus so
    // that new keys will go to the newly focused field
    CARAPI_(void) DomChangedFocus();

    static CARAPI_(Int32) PinLoc(
        /* [in] */ Int32 x,
        /* [in] */ Int32 viewMax,
        /* [in] */ Int32 docMax);

    // Expects x in view coordinates
    CARAPI_(Int32) PinLocX(
        /* [in] */ Int32 x);

    // Expects y in view coordinates
    CARAPI_(Int32) PinLocY(
        /* [in] */ Int32 y);

    CARAPI_(Int32) ComputeRealVerticalScrollRange();

    CARAPI_(void) DrawContent(
        /* [in] */ ICanvas* canvas);

    /**
     * Given a distance in view space, convert it to content space. Note: this
     * does not reflect translation, just scaling, so this should not be called
     * with coordinates, but should be called for dimensions like width or
     * height.
     */
    CARAPI_(Int32) ViewToContentDimension(
        /* [in] */ Int32 d);

    /**
     * Given a x coordinate in view space, convert it to content space.
     * Returns the result as a float.
     */
    CARAPI_(Float) ViewToContentXf(
        /* [in] */ Int32 x);

    /**
     * Given a y coordinate in view space, convert it to content space.
     * Takes into account the height of the title bar if there is one
     * embedded into the WebView. Returns the result as a float.
     */
    CARAPI_(Float) ViewToContentYf(
        /* [in] */ int y);

    CARAPI_(AutoPtr<IRect>) ContentToViewRect(
        /* [in] */ IRect* x);

    /*  To invalidate a rectangle in content coordinates, we need to transform
        the rect into view coordinates, so we can then call invalidate(...).

        Normally, we would just call contentToView[XY](...), which eventually
        calls Math.round(coordinate * mActualScale). However, for invalidates,
        we need to account for the slop that occurs with antialiasing. To
        address that, we are a little more liberal in the size of the rect that
        we invalidate.

        This liberal calculation calls floor() for the top/left, and ceil() for
        the bottom/right coordinates. This catches the possible extra pixels of
        antialiasing that we might have missed with just round().
     */

    // Called by JNI to invalidate the View, given rectangle coordinates in
    // content space
    CARAPI_(void) ViewInvalidate(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    // Called by JNI to invalidate the View after a delay, given rectangle
    // coordinates in content space
    CARAPI_(void) ViewInvalidateDelayed(
        /* [in] */ Int64 delay,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI_(void) InvalidateContentRect(
        /* [in] */ IRect* r);

    // stop the scroll animation, and don't let a subsequent fling add
    // to the existing velocity
    CARAPI_(void) AbortAnimation();

    /* call from webcoreview.draw(), so we're still executing in the UI thread
    */
    CARAPI_(void) RecordNewContentSize(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Boolean updateLayout);

    CARAPI_(void) SetNewZoomScale(
        /* [in] */ Float scale,
        /* [in] */ Boolean updateTextWrapScale,
        /* [in] */ Boolean force);

    CARAPI_(AutoPtr<IRect>) SendOurVisibleRect();

    // Sets r to be the visible rectangle of our webview in view coordinates
    CARAPI_(void) CalcOurVisibleRect(
        /* [in] */ IRect* r);

    // Sets r to be our visible rectangle in content coordinates
    CARAPI_(void) CalcOurContentVisibleRect(
        /* [in] */ IRect* r);

    // Sets r to be our visible rectangle in content coordinates. We use this
    // method on the native side to compute the position of the fixed layers.
    // Uses floating coordinates (necessary to correctly place elements when
    // the scale factor is not 1)
    CARAPI_(void) CalcOurContentVisibleRectF(
        /* [in] */ IRectF* r);

    /**
     * Compute unzoomed width and height, and if they differ from the last
     * values we sent, send them to webkit (to be used has new viewport)
     *
     * @return true if new values were sent
     */
    CARAPI_(Boolean) SendViewSizeZoom();

    CARAPI_(Int32) ComputeRealHorizontalScrollRange();

    static CARAPI_(Int32) ComputeDuration(
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy);

    // helper to pin the scrollBy parameters (already in view coordinates)
    // returns true if the scroll was changed
    CARAPI_(Boolean) PinScrollBy(
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy,
        /* [in] */ Boolean animate,
        /* [in] */ Int32 animationDuration);

    // helper to pin the scrollTo parameters (already in view coordinates)
    // returns true if the scroll was changed
    CARAPI_(Boolean) PinScrollTo(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Boolean animate,
        /* [in] */ Int32 animationDuration);

    // Scale from content to view coordinates, and pin.
    // Also called by jni webview.cpp
    CARAPI_(Boolean) SetContentScrollBy(
        /* [in] */ Int32 cx,
        /* [in] */ Int32 cy,
        /* [in] */ Boolean animate);

    // scale from content to view coordinates, and pin
    // return true if pin caused the final x/y different than the request cx/cy,
    // and a future scroll may reach the request cx/cy after our size has
    // changed
    // return false if the view scroll to the exact position as it is requested,
    // where negative numbers are taken to mean 0
    CARAPI_(Boolean) SetContentScrollTo(
        /* [in] */ Int32 cx,
        /* [in] */ Int32 cy);

    // scale from content to view coordinates, and pin
    CARAPI_(void) SpawnContentScrollTo(
        /* [in] */ Int32 cx,
        /* [in] */ Int32 cy);

    /**
     * These are from webkit, and are in content coordinate system (unzoomed)
     */
    CARAPI_(void) ContentSizeChanged(
        /* [in] */ Boolean updateLayout);

    /**
     * Draw the glow effect along the sides of the widget. mEdgeGlow* must be non-null.
     *
     * @param canvas Canvas to draw into, transformed into view coordinates.
     * @return true if glow effects are still animating and the view should invalidate again.
     */
    CARAPI_(Boolean) DrawEdgeGlows(
        /* [in] */ ICanvas* canvas);

    /**
     * Need to adjust the WebTextView after a change in zoom, since mActualScale
     * has changed.  This is especially important for password fields, which are
     * drawn by the WebTextView, since it conveys more information than what
     * webkit draws.  Thus we need to reposition it to show in the correct
     * place.
     */
    Boolean mNeedToAdjustWebTextView;

    CARAPI_(Boolean) DidUpdateTextViewBounds(
        /* [in] */ Boolean allowIntersect);

    CARAPI_(void) DrawExtras(
        /* [in] */ ICanvas* canvas,
        /* [in] */ Int32 extras,
        /* [in] */ Boolean animationsRunning);

    CARAPI_(void) DrawCoreAndCursorRing(
        /* [in] */ ICanvas* canvas,
        /* [in] */ Int32 color,
        /* [in] */ Boolean drawCursorRing);

    /**
     * Called in response to a message from webkit telling us that the soft
     * keyboard should be launched.
     */
    CARAPI_(void) DisplaySoftKeyboard(
        /* [in] */ Boolean isTextView);

    // Called by WebKit to instruct the UI to hide the keyboard
    CARAPI_(void) HideSoftKeyboard();

    CARAPI_(void) SetActive(
        /* [in] */ Boolean active);

    static CARAPI_(Int32) Sign(
        /* [in] */ Float x);

    CARAPI_(Boolean) HitFocusedPlugin(
        /* [in] */ Int32 contentX,
        /* [in] */ Int32 contentY);

    CARAPI_(Boolean) ShouldForwardTouchEvent();

    CARAPI_(Boolean) InFullScreenMode();

    CARAPI_(void) CancelWebCoreTouchEvent(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Boolean removeEvents);

    CARAPI_(void) StartTouch(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int64 eventTime);

    CARAPI_(void) StartDrag();

    CARAPI_(void) DoDrag(
        /* [in] */ Int32 deltaX,
        /* [in] */ Int32 deltaY);

    CARAPI_(void) StopTouch();

    CARAPI_(void) CancelTouch();

    CARAPI_(Int32) ScaleTrackballX(
        /* [in] */ Float xRate,
        /* [in] */ Int32 width);

    CARAPI_(Int32) ScaleTrackballY(
        /* [in] */ Float yRate,
        /* [in] */ Int32 height);

    CARAPI_(Int32) KeyCodeToSoundsEffect(
        /* [in] */ Int32 keyCode);

    CARAPI_(void) DoTrackball(
        /* [in] */ Int64 time);

    CARAPI_(Int32) ComputeMaxScrollX();

    CARAPI_(Int32) ComputeMaxScrollY();

    CARAPI_(void) DoFling();

    CARAPI_(Boolean) ZoomWithPreview(
        /* [in] */ Float scale,
        /* [in] */ Boolean updateTextWrapScale);

    CARAPI_(AutoPtr<ExtendedZoomControls>) CreateZoomControls();

    CARAPI_(void) UpdateSelection();

    CARAPI_(void) DoShortPress();

    CARAPI_(void) DoMotionUp(
        /* [in] */ Int32 contentX,
        /* [in] */ Int32 contentY);

    /*
     * Return true if the view (Plugin) is fully visible and maximized inside
     * the WebView.
     */
    CARAPI_(Boolean) IsPluginFitOnScreen(
        /* [in] */ ViewManager::ChildView* view);

    /*
     * Maximize and center the rectangle, specified in the document coordinate
     * space, inside the WebView. If the zoom doesn't need to be changed, do an
     * animated scroll to center it. If the zoom needs to be changed, find the
     * zoom center and do a smooth zoom transition.
     */
    CARAPI_(void) CenterFitRect(
        /* [in] */ Int32 docX,
        /* [in] */ Int32 docY,
        /* [in] */ Int32 docWidth,
        /* [in] */ Int32 docHeight);    

    // Rule for double tap:
    // 1. if the current scale is not same as the text wrap scale and layout
    //    algorithm is NARROW_COLUMNS, fit to column;
    // 2. if the current state is not overview mode, change to overview mode;
    // 3. if the current state is overview mode, change to default scale.
    CARAPI_(void) DoDoubleTap();

    // Called by JNI to handle a touch on a node representing an email address,
    // address, or phone number
    CARAPI_(void) OverrideLoading(
        /* [in] */ const String& url);

    /**
     * Used when receiving messages for REQUEST_KEYBOARD_WITH_SELECTION_MSG_ID
     * and UPDATE_TEXT_SELECTION_MSG_ID.  Update the selection of WebTextView.
     */
    CARAPI_(void) UpdateTextSelectionFromMessage(
        /* [in] */ Int32 nodePointer,
        /* [in] */ Int32 textGeneration,
        /* [in] */ WebViewCore::TextSelectionData* data);


    CARAPI_(void) UpdateZoomRange(
        /* [in] */ WebViewCore::RestoreState* restoreState,
        /* [in] */ Int32 viewWidth,
        /* [in] */ Int32 minPrefWidth,
        /* [in] */ Boolean updateZoomOverview);

    // called by JNI
    CARAPI_(void) SendMoveFocus(
        /* [in] */ Int32 frame,
        /* [in] */ Int32 node);

    // called by JNI
    CARAPI_(void) SendMoveMouse(
        /* [in] */ Int32 frame,
        /* [in] */ Int32 node,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

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
    CARAPI_(void) SendMoveMouseIfLatest(
        /* [in] */ Boolean removeFocus);

    // called by JNI
    CARAPI_(void) SendMotionUp(
        /* [in] */ Int32 touchGeneration,
        /* [in] */ Int32 frame,
        /* [in] */ Int32 node,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI_(Int32) GetScaledMaxXScroll();

    CARAPI_(Int32) GetScaledMaxYScroll();

    /**
     * Called by JNI to invalidate view
     */
    CARAPI_(void) ViewInvalidate();

    /**
     * Pass the key to the plugin.  This assumes that nativeFocusIsPlugin()
     * returned true.
     */
    CARAPI_(void) LetPluginHandleNavKey(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int64 time,
        /* [in] */ Boolean down);

    // return true if the key was handled
    CARAPI_(Boolean) NavHandledKey(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 count,
        /* [in] */ Boolean noScroll,
        /* [in] */ Int64 time);

    CARAPI_(Int32) NativeCacheHitFramePointer();
    CARAPI_(AutoPtr<IRect>) NativeCacheHitNodeBounds();
    CARAPI_(Int32) NativeCacheHitNodePointer();

    CARAPI_(void)     NativeCreate(
        /* [in] */ Int32 ptr);
    CARAPI_(Int32)      NativeCursorFramePointer();
    CARAPI_(AutoPtr<IRect>)     NativeCursorNodeBounds();
    CARAPI_(Int32) NativeCursorNodePointer();

    CARAPI_(Boolean)  NativeCursorIntersects(
        /* [in] */ IRect* visibleRect);
    CARAPI_(Boolean)  NativeCursorIsAnchor();
    CARAPI_(Boolean)  NativeCursorIsTextInput();
    CARAPI_(AutoPtr<IPoint>)    NativeCursorPosition();
    CARAPI_(String)   NativeCursorText();

    /**
     * Returns true if the native cursor node says it wants to handle key events
     * (ala plugins). This can only be called if mNativeClass is non-zero!
     */
    CARAPI_(Boolean)  NativeCursorWantsKeyEvents();
    CARAPI_(void)     NativeDebugDump();
    CARAPI_(void)     NativeDestroy();
    CARAPI_(Boolean)  NativeEvaluateLayersAnimations();
    CARAPI_(void)     NativeExtendSelection(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI_(void)     NativeDrawExtras(
        /* [in] */ ICanvas* canvas,
        /* [in] */ Int32 extra);
    CARAPI_(void)     NativeDumpDisplayTree(
        /* [in] */ const String& urlOrNull);
    CARAPI_(Int32)    NativeFindAll(
        /* [in] */ const String& findLower,
        /* [in] */ const String& findUpper);
    CARAPI_(void)     NativeFindNext(
        /* [in] */ Boolean forward);


    CARAPI_(Boolean)  NativeFocusCandidateIsRtlText();
    CARAPI_(Boolean)  NativeFocusCandidateIsTextInput();

    CARAPI_(AutoPtr<IRect>)     NativeFocusCandidateNodeBounds();

    /**
     * @return A Rect with left, top, right, bottom set to the corresponding
     * padding values in the focus candidate, if it is a textfield/textarea with
     * a style.  Otherwise return null.  This is not actually a rectangle; Rect
     * is being used to pass four integers.
     */
    CARAPI_(AutoPtr<IRect>)     NativeFocusCandidatePaddingRect();

    CARAPI_(String)   NativeFocusCandidateText();
    CARAPI_(Int32)    NativeFocusCandidateTextSize();

    /**
     * Returns an integer corresponding to WebView.cpp::type.
     * See WebTextView.setType()
     */
    CARAPI_(Int32)              NativeFocusCandidateType();
    CARAPI_(Boolean)            NativeFocusIsPlugin();
    CARAPI_(AutoPtr<IRect>)     NativeFocusNodeBounds();
    
    CARAPI_(AutoPtr<IRect>)     NativeGetCursorRingBounds();
    CARAPI_(String)             NativeGetSelection();
    CARAPI_(Boolean)            NativeHasCursorNode();
    CARAPI_(Boolean)            NativeHasFocusNode();
    CARAPI_(void)               NativeHideCursor();
    CARAPI_(Boolean) NativeHitSelection(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);
    CARAPI_(String) NativeImageURI(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);
    CARAPI_(void)     NativeInstrumentReport();
    
    // return true if the page has been scrolled
    CARAPI_(Boolean)  NativeMotionUp(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 slop);
    // returns false if it handled the key
    CARAPI_(Boolean)  NativeMoveCursor(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 count,
        /* [in] */ Boolean noScroll);
    CARAPI_(Int32)    NativeMoveGeneration();
    CARAPI_(void)     NativeMoveSelection(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);
    CARAPI_(Boolean)  NativePointInNavCache(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 slop);
    // Like many other of our native methods, you must make sure that
    // mNativeClass is not null before calling this method.
    CARAPI_(void) NativeRecordButtons(
        /* [in] */ Boolean focused,
        /* [in] */ Boolean pressed,
        /* [in] */ Boolean invalidate);
    CARAPI_(void) NativeResetSelection();
    CARAPI_(void) NativeSelectAll();
    CARAPI_(void) NativeSelectBestAt(
        /* [in] */ IRect* rect);
    CARAPI_(Int32) NativeSelectionX();
    CARAPI_(Int32) NativeSelectionY();
    CARAPI_(Int32) NativeFindIndex();
    CARAPI_(void)  NativeSetExtendSelection();
    CARAPI_(void)  NativeSetFindIsEmpty();
    CARAPI_(void)  NativeSetFindIsUp(
        /* [in] */ Boolean isUp);
    CARAPI_(void)  NativeSetFollowedLink(
        /* [in] */ Boolean followed);
    CARAPI_(void)  NativeSetHeightCanMeasure(
        /* [in] */ Boolean measure);
    CARAPI_(void)  NativeSetRootLayer(
        /* [in] */ Int32 layer);
    CARAPI_(void)  NativeSetSelectionPointer(
        /* [in] */ Boolean set,
        /* [in] */ Float scale,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);
    CARAPI_(Boolean)  NativeStartSelection(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);
    CARAPI_(void)     NativeSetSelectionRegion(
        /* [in] */ Boolean set);
    CARAPI_(AutoPtr<IRect>) NativeSubtractLayers(
        /* [in] */ IRect* content);
    CARAPI_(Int32)    NativeTextGeneration();

    // Never call this version except by updateCachedTextfield(String) -
    // we always want to pass in our generation number.
    CARAPI_(void)   NativeUpdateCachedTextfield(
        /* [in] */ const String& updatedText,
        /* [in] */ Int32 generation);

    CARAPI_(Boolean) NativeWordSelection(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);
    
    CARAPI_(Int32)   NativeGetBlockLeftEdge(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Float scale);

private:  

    // Here are the snap align logic:
    // 1. If it starts nearly horizontally or vertically, snap align;
    // 2. If there is a dramitic direction change, let it go;
    // 3. If there is a same direction back and forth, lock it.

    // adjustable parameters
    Int32 mMinLockSnapReverseDistance;
    static const Float MAX_SLOPE_FOR_DIAG = 1.5f;
    static const Int32 MIN_BREAK_SNAP_CROSS_DISTANCE = 80;

    AutoPtr<IDragTracker>         mDragTracker;
    AutoPtr<DragTrackerHandler>  mDragTrackerHandler;

    // true means redraw the screen all-the-time. Only with AUTO_REDRAW_HACK
    Boolean mAutoRedraw; 

    // A final CallbackProxy shared by WebViewCore and BrowserFrame.
    const AutoPtr<ICallbackProxy>   mCallbackProxy;

    const AutoPtr<IWebViewDatabase> mDatabase;

    // SSL certificate for the main top-level page (if secure)
    AutoPtr<ISslCertificate> mCertificate;

    // Native WebView pointer that is 0 until the native object has been
    // created.
    Int32 mNativeClass;
    // This would be final but it needs to be set to null when the WebView is
    // destroyed.
    AutoPtr<WebViewCore> mWebViewCore;
    
    AutoPtr<WebTextView> mWebTextView;

    // Used to ignore changes to webkit text that arrives to the UI side after
    // more key events.
    Int32 mTextGeneration;    

    /**
     * Position of the last touch event.
     */
    Float mLastTouchX;
    Float mLastTouchY;

    /**
     * Time of the last touch event.
     */
    Int64 mLastTouchTime;

    /**
     * Time of the last time sending touch event to WebViewCore
     */
    Int64 mLastSentTouchTime;

    Int32 mCurrentTouchInterval;// = TOUCH_SENT_INTERVAL;

    Int32 mMaximumFling;
    Float mLastVelocity;
    Float mLastVelX;
    Float mLastVelY;

    /**
     * True if we have a touch panel capable of detecting smooth pan/scale at the same time
     */
    Boolean mAllowPanAndScale;

    // Whether to forward the touch events to WebCore
    Boolean mForwardTouchEvents;// = false;

    // true when the touch movement exceeds the slop
    Boolean mConfirmMove;

    // if true, touch events will be first processed by WebCore, if prevent
    // default is not set, the UI will continue handle them.
    Boolean mDeferTouchProcess;

    // to avoid interfering with the current touch events, track them
    // separately. Currently no snapping or fling in the deferred process mode
    Int32 mDeferTouchMode;// = TOUCH_DONE_MODE;
    Float mLastDeferTouchX;
    Float mLastDeferTouchY;

    // Whether or not to draw the cursor ring.
    Boolean mDrawCursorRing;// = true;

    // true if onPause has been called (and not onResume)
    Boolean mIsPaused;

    // true if, during a transition to a new page, we're delaying
    // deleting a root layer until there's something to draw of the new page.
    Boolean mDelayedDeleteRootLayer;

    /**
     * Customizable constant
     */
    // pre-computed square of ViewConfiguration.getScaledTouchSlop()
    Int32 mTouchSlopSquare;
    // pre-computed square of ViewConfiguration.getScaledDoubleTapSlop()
    Int32 mDoubleTapSlopSquare;
    // pre-computed density adjusted navigation slop
    Int32 mNavSlop;

    // These keep track of the center point of the zoom.  They are used to
    // determine the point around which we should zoom.
    Float mZoomCenterX;
    Float mZoomCenterY;

    Int32 mContentWidth;   // cache of value from WebViewCore
    Int32 mContentHeight;  // cache of value from WebViewCore

    // Need to have the separate control for horizontal and vertical scrollbar
    // style than the View's single scrollbar style
    Boolean mOverlayHorizontalScrollbar;// = true;
    Boolean mOverlayVerticalScrollbar;// = false;

    // whether support multi-touch
    Boolean mSupportMultiTouch;
    // use the framework's ScaleGestureDetector to handle multi-touch
//    ScaleGestureDetector mScaleDetector;

    // the anchor point in the document space where VIEW_SIZE_CHANGED should
    // apply to
    Int32 mAnchorX;
    Int32 mAnchorY;

    // scale limit, which can be set through viewport meta tag in the web page
    Float mMaxZoomScale;
    Float mMinZoomScale;
    Boolean mMinZoomScaleFixed;// = true;

    // initial scale in percent. 0 means using default.
    Int32 mInitialScaleInPercent;// = 0;

    Float mDefaultScale;    

    // set to true temporarily during ScaleGesture triggered zoom
    Boolean mPreviewZoomOnly;// = false;

    // computed scale and inverse, from mZoomWidth.
    Float mActualScale;
    Float mInvActualScale;
    // if this is non-zero, it is used on drawing rather than mActualScale
    Float mZoomScale;
    Float mInvInitialZoomScale;
    Float mInvFinalZoomScale;
    Int32 mInitialScrollX;
    Int32 mInitialScrollY;
    Int64 mZoomStart;
    static const Int32 ZOOM_ANIMATION_LENGTH = 500;

    Boolean mUserScroll;// = false;

    Int32 mSnapScrollMode;// = SNAP_NONE;

    /**
     * Max distance to overscroll by in pixels.
     * This how far content can be pulled beyond its normal bounds by the user.
     */
    Int32 mOverscrollDistance;

    /**
     * Max distance to overfling by in pixels.
     * This is how far flinged content can move beyond the end of its normal bounds.
     */
    Int32 mOverflingDistance;

    /*
     * These manage the edge glow effect when flung or pulled beyond the edges.
     * If one is not null, all are not null. Checking one for null is as good as checking each.
     */
    EdgeGlow* mEdgeGlowTop;
    EdgeGlow* mEdgeGlowBottom;
    EdgeGlow* mEdgeGlowLeft;
    EdgeGlow* mEdgeGlowRight;

    /*
     * These manage the delta the user has pulled beyond the edges.
     */
    Int32 mOverscrollDeltaX;
    Int32 mOverscrollDeltaY;

    // Used to match key downs and key ups
    Boolean mGotKeyDown;    

    // for event log
    Int64 mLastTouchUpTime;// = 0;

    Int32 mBackgroundColor;// = Color.WHITE;

    // Used to notify listeners of a new picture.
    AutoPtr<IPictureListener> mPictureListener;

    // The View containing the zoom controls
    AutoPtr<ExtendedZoomControls> mZoomControls;
//    Runnable mZoomControlRunnable;

    // mZoomButtonsController will be lazy initialized in
    // getZoomButtonsController() to get better performance.
//    ZoomButtonsController mZoomButtonsController;

//    ZoomButtonsController.OnZoomListener mZoomListener;

    /**
     * A title bar which is embedded in this WebView, and scrolls along with it
     * vertically, but not horizontally.
     */
    AutoPtr<IView> mTitleBar;

    /**
     * Since we draw the title bar ourselves, we removed the shadow from the
     * browser's activity.  We do want a shadow at the bottom of the title bar,
     * or at the top of the screen if the title bar is not visible.  This
     * drawable serves that purpose.
     */
    AutoPtr<IDrawable> mTitleShadow;

    // Used to avoid sending many visible rect messages.
    AutoPtr<IRect> mLastVisibleRectSent;
    AutoPtr<IRect> mLastGlobalRect;

    // Used to know whether the find dialog is open.  Affects whether
    // or not we draw the highlights for matches.
    Boolean mFindIsUp;

    Int32 mFindHeight;
    // Keep track of the last string sent, so we can search again after an
    // orientation change or the dismissal of the soft keyboard.
    String mLastFind;

    /**
     * The URL of a page that sent a message to scroll the title bar off screen.
     *
     * Many mobile sites tell the page to scroll to (0,1) in order to scroll the
     * title bar off the screen.  Sometimes, the scroll position is set before
     * the page finishes loading.  Rather than scrolling while the page is still
     * loading, keep track of the URL and new scroll position so we can perform
     * the scroll once the page finishes loading.
     */
    String mPageThatNeedsToSlideTitleBarOffScreen;

    /**
     * The destination Y scroll position to be used when the page finishes
     * loading.  See mPageThatNeedsToSlideTitleBarOffScreen.
     */
    Int32 mYDistanceToSlideTitleOffScreen;

    // draw history
    Boolean mDrawHistory;// = false;
    AutoPtr<IPicture> mHistoryPicture;// = null;
    Int32 mHistoryWidth;// = 0;
    Int32 mHistoryHeight;// = 0;

    Int64 mTrackballFirstTime;// = 0;
    Int64 mTrackballLastTime;// = 0;
    Float mTrackballRemainsX;// = 0.0f;
    Float mTrackballRemainsY;// = 0.0f;
    Int32 mTrackballXMove;// = 0;
    Int32 mTrackballYMove;// = 0;
    Boolean mSelectingText;// = false;
    Boolean mSelectionStarted;// = false;
    Boolean mExtendSelection;// = false;
    Boolean mDrawSelectionPointer;// = false;
    static const Int32 TRACKBALL_KEY_TIMEOUT = 1000;
    static const Int32 TRACKBALL_TIMEOUT = 200;
    static const Int32 TRACKBALL_WAIT = 100;
    static const Int32 TRACKBALL_SCALE = 400;
    static const Int32 TRACKBALL_SCROLL_COUNT = 5;
    static const Int32 TRACKBALL_MOVE_COUNT = 10;
    static const Int32 TRACKBALL_MULTIPLIER = 3;
    static const Int32 SELECT_CURSOR_OFFSET = 16;
    Int32 mSelectX;// = 0;
    Int32 mSelectY;// = 0;
    Boolean mFocusSizeChanged;// = false;
    Boolean mShiftIsPressed;// = false;
    Boolean mTrackballDown;// = false;
    Int64 mTrackballUpTime;// = 0;
    Int64 mLastCursorTime;// = 0;
    AutoPtr<IRect> mLastCursorBounds;

    // Set by default; BrowserActivity clears to interpret trackball data
    // directly for movement. Currently, the framework only passes
    // arrow key events, not trackball events, from one child to the next
    Boolean mMapTrackballToArrowKeys;// = true;
};

#endif //__CWEBVIEW_H__