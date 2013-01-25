
#ifndef __CWEBVIEW_H__
#define __CWEBVIEW_H__

#include "_CWebView.h"

CarClass(CWebView)
{
public:
    CARAPI GetVerticalFadingEdgeLength(
        /* [out] */ Int32 * pLength);

    CARAPI SetFadingEdgeLength(
        /* [in] */ Int32 length);

    CARAPI GetHorizontalFadingEdgeLength(
        /* [out] */ Int32 * pLength);

    CARAPI GetVerticalScrollbarWidth(
        /* [out] */ Int32 * pWidth);

    CARAPI GetHorizontalScrollbarHeight(
        /* [out] */ Int32 * pHeight);

    CARAPI SetOnFocusChangeListener(
        /* [in] */ IViewOnFocusChangeListener * pL);

    CARAPI GetOnFocusChangeListener(
        /* [out] */ IViewOnFocusChangeListener ** ppL);

    CARAPI SetOnClickListener(
        /* [in] */ IViewOnClickListener * pL);

    CARAPI SetOnLongClickListener(
        /* [in] */ IViewOnLongClickListener * pL);

    CARAPI SetOnCreateContextMenuListener(
        /* [in] */ IViewOnCreateContextMenuListener * pL);

    CARAPI PerformClick(
        /* [out] */ Boolean * pResult);

    CARAPI PerformLongClick(
        /* [out] */ Boolean * pResult);

    CARAPI ShowContextMenu(
        /* [out] */ Boolean * pResult);

    CARAPI SetOnKeyListener(
        /* [in] */ IViewOnKeyListener * pL);

    CARAPI SetOnTouchListener(
        /* [in] */ IViewOnTouchListener * pL);

    CARAPI RequestRectangleOnScreen(
        /* [in] */ IRect * pRectangle,
        /* [out] */ Boolean * pResult);

    CARAPI RequestRectangleOnScreenEx(
        /* [in] */ IRect * pRectangle,
        /* [in] */ Boolean immediate,
        /* [out] */ Boolean * pResult);

    CARAPI ClearFocus();

    CARAPI HasFocus(
        /* [out] */ Boolean * pHasFocus);

    CARAPI HasFocusable(
        /* [out] */ Boolean * pHasFocusable);

    CARAPI DispatchPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent * pEvent,
        /* [out] */ Boolean * pResult);

    CARAPI GetContentDescription(
        /* [out, callee] */ ArrayOf<Char8> ** ppDescription);

    CARAPI SetContentDescription(
        /* [in] */ const ArrayOf<Char8> & contentDescription);

    CARAPI IsFocused(
        /* [out] */ Boolean * pFocused);

    CARAPI FindFocus(
        /* [out] */ IView ** ppView);

    CARAPI SetScrollContainer(
        /* [in] */ Boolean isScrollContainer);

    CARAPI GetDrawingCacheQuality(
        /* [out] */ Int32 * pQuality);

    CARAPI SetDrawingCacheQuality(
        /* [in] */ Int32 quality);

    CARAPI GetKeepScreenOn(
        /* [out] */ Boolean * pResult);

    CARAPI SetKeepScreenOn(
        /* [in] */ Boolean keepScreenOn);

    CARAPI GetNextFocusLeftId(
        /* [out] */ Int32 * pNextFocusLeftId);

    CARAPI SetNextFocusLeftId(
        /* [in] */ Int32 nextFocusLeftId);

    CARAPI GetNextFocusRightId(
        /* [out] */ Int32 * pNextFocusRightId);

    CARAPI SetNextFocusRightId(
        /* [in] */ Int32 nextFocusRightId);

    CARAPI GetNextFocusUpId(
        /* [out] */ Int32 * pNextFocusUpId);

    CARAPI SetNextFocusUpId(
        /* [in] */ Int32 nextFocusUpId);

    CARAPI GetNextFocusDownId(
        /* [out] */ Int32 * pNextFocusDownId);

    CARAPI SetNextFocusDownId(
        /* [in] */ Int32 nextFocusDownId);

    CARAPI IsShown(
        /* [out] */ Boolean * pShown);

    CARAPI IsFitsSystemWindowsFlagSet(
        /* [out] */ Boolean * pFlag);

    CARAPI GetVisibility(
        /* [out] */ Int32 * pVisibility);

    CARAPI SetVisibility(
        /* [in] */ Int32 visibility);

    CARAPI IsEnabled(
        /* [out] */ Boolean * pEnabled);

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI SetFocusable(
        /* [in] */ Boolean focusable);

    CARAPI SetFocusableInTouchMode(
        /* [in] */ Boolean focusableInTouchMode);

    CARAPI SetSoundEffectsEnabled(
        /* [in] */ Boolean soundEffectsEnabled);

    CARAPI IsSoundEffectsEnabled(
        /* [out] */ Boolean * pSoundEffectsEnabled);

    CARAPI SetHapticFeedbackEnabled(
        /* [in] */ Boolean hapticFeedbackEnabled);

    CARAPI IsHapticFeedbackEnabled(
        /* [out] */ Boolean * pHapticFeedbackEnabled);

    CARAPI SetWillNotDraw(
        /* [in] */ Boolean willNotDraw);

    CARAPI WillNotDraw(
        /* [out] */ Boolean * pWillNotDraw);

    CARAPI SetWillNotCacheDrawing(
        /* [in] */ Boolean willNotCacheDrawing);

    CARAPI WillNotCacheDrawing(
        /* [out] */ Boolean * pCache);

    CARAPI IsClickable(
        /* [out] */ Boolean * pClickable);

    CARAPI SetClickable(
        /* [in] */ Boolean clickable);

    CARAPI IsLongClickable(
        /* [out] */ Boolean * pLongClickable);

    CARAPI SetLongClickable(
        /* [in] */ Boolean longClickable);

    CARAPI SetPressed(
        /* [in] */ Boolean pressed);

    CARAPI IsPressed(
        /* [out] */ Boolean * pPressed);

    CARAPI IsSaveEnabled(
        /* [out] */ Boolean * pEnabled);

    CARAPI SetSaveEnabled(
        /* [in] */ Boolean enabled);

    CARAPI GetFilterTouchesWhenObscured(
        /* [out] */ Boolean * pEnabled);

    CARAPI SetFilterTouchesWhenObscured(
        /* [in] */ Boolean enabled);

    CARAPI IsFocusable(
        /* [out] */ Boolean * pFocusable);

    CARAPI IsFocusableInTouchMode(
        /* [out] */ Boolean * pFocusable);

    CARAPI FocusSearch(
        /* [in] */ Int32 direction,
        /* [out] */ IView ** ppView);

    CARAPI DispatchUnhandledMove(
        /* [in] */ IView * pFocused,
        /* [in] */ Int32 direction,
        /* [out] */ Boolean * pResult);

    CARAPI GetFocusables(
        /* [in] */ Int32 direction,
        /* [out] */ IObjectContainer ** ppViews);

    CARAPI AddFocusables(
        /* [in] */ IObjectContainer * pViews,
        /* [in] */ Int32 direction);

    CARAPI AddFocusablesEx(
        /* [in] */ IObjectContainer * pViews,
        /* [in] */ Int32 direction,
        /* [in] */ Int32 focusableMode);

    CARAPI GetTouchables(
        /* [out] */ IObjectContainer ** ppViews);

    CARAPI AddTouchables(
        /* [in] */ IObjectContainer * pViews);

    CARAPI RequestFocus(
        /* [out] */ Boolean * pResult);

    CARAPI RequestFocusEx(
        /* [in] */ Int32 direction,
        /* [out] */ Boolean * pResult);

    CARAPI RequestFocusEx2(
        /* [in] */ Int32 direction,
        /* [in] */ IRect * pPreviouslyFocusedRect,
        /* [out] */ Boolean * pResult);

    CARAPI RequestFocusFromTouch(
        /* [out] */ Boolean * pResult);

    CARAPI DispatchStartTemporaryDetach();

    CARAPI OnStartTemporaryDetach();

    CARAPI DispatchFinishTemporaryDetach();

    CARAPI OnFinishTemporaryDetach();

    CARAPI GetKeyDispatcherState(
        /* [out] */ IDispatcherState ** ppState);

    CARAPI DispatchKeyEventPreIme(
        /* [in] */ IKeyEvent * pEvent,
        /* [out] */ Boolean * pResult);

    CARAPI DispatchKeyEvent(
        /* [in] */ IKeyEvent * pEvent,
        /* [out] */ Boolean * pResult);

    CARAPI DispatchKeyShortcutEvent(
        /* [in] */ IKeyEvent * pEvent,
        /* [out] */ Boolean * pResult);

    CARAPI DispatchTouchEvent(
        /* [in] */ IMotionEvent * pEvent,
        /* [out] */ Boolean * pResult);

    CARAPI OnFilterTouchEventForSecurity(
        /* [in] */ IMotionEvent * pEvent,
        /* [out] */ Boolean * pResult);

    CARAPI DispatchTrackballEvent(
        /* [in] */ IMotionEvent * pEvent,
        /* [out] */ Boolean * pResult);

    CARAPI DispatchWindowFocusChanged(
        /* [in] */ Boolean hasFocus);

    CARAPI OnWindowFocusChanged(
        /* [in] */ Boolean hasWindowFocus);

    CARAPI HasWindowFocus(
        /* [out] */ Boolean * pFocus);

    CARAPI DispatchDisplayHint(
        /* [in] */ Int32 hint);

    CARAPI DispatchWindowVisibilityChanged(
        /* [in] */ Int32 visibility);

    CARAPI GetWindowVisibility(
        /* [out] */ Int32 * pVisibility);

    CARAPI GetWindowVisibleDisplayFrame(
        /* [in] */ IRect * pOutRect);

    CARAPI DispatchConfigurationChanged(
        /* [in] */ IConfiguration * pNewConfig);

    CARAPI IsInTouchMode(
        /* [out] */ Boolean * pResult);

    CARAPI GetContext(
        /* [out] */ IContext ** ppCtx);

    CARAPI OnKeyPreIme(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent * pEvent,
        /* [out] */ Boolean * pResult);

    CARAPI OnKeyShortcut(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent * pEvent,
        /* [out] */ Boolean * pResult);

    CARAPI OnCheckIsTextEditor(
        /* [out] */ Boolean * pResult);

    CARAPI OnCreateInputConnection(
        /* [in] */ IEditorInfo * pOutAttrs,
        /* [out] */ IInputConnection ** ppInputConnection);

    CARAPI CheckInputConnectionProxy(
        /* [in] */ IView * pView,
        /* [out] */ Boolean * pResult);

    CARAPI CreateContextMenu(
        /* [in] */ IContextMenu * pMenu);

    CARAPI OnTrackballEvent(
        /* [in] */ IMotionEvent * pEvent,
        /* [out] */ Boolean * pResult);

    CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent * pEvent,
        /* [out] */ Boolean * pResult);

    CARAPI SetTouchDelegate(
        /* [in] */ ITouchDelegate * pTouchDelegate);

    CARAPI GetTouchDelegate(
        /* [out] */ ITouchDelegate ** ppTouchDelegate);

    CARAPI CancelLongPress();

    CARAPI BringToFront();

    CARAPI GetParent(
        /* [out] */ IViewParent ** ppParent);

    CARAPI GetScrollX(
        /* [out] */ Int32 * pX);

    CARAPI GetScrollY(
        /* [out] */ Int32 * pY);

    CARAPI GetWidth(
        /* [out] */ Int32 * pWidth);

    CARAPI GetHeight(
        /* [out] */ Int32 * pHeight);

    CARAPI GetDrawingRect(
        /* [in] */ IRect * pOutRect);

    CARAPI GetMeasuredWidth(
        /* [out] */ Int32 * pWidth);

    CARAPI GetMeasuredHeight(
        /* [out] */ Int32 * pHeight);

    CARAPI GetTop(
        /* [out] */ Int32 * pTop);

    CARAPI GetBottom(
        /* [out] */ Int32 * pBottom);

    CARAPI GetLeft(
        /* [out] */ Int32 * pLeft);

    CARAPI GetRight(
        /* [out] */ Int32 * pRight);

    CARAPI GetHitRect(
        /* [in] */ IRect * pOutRect);

    CARAPI GetFocusedRect(
        /* [in] */ IRect * pOutRect);

    CARAPI GetGlobalVisibleRect(
        /* [in] */ IRect * pOutRect,
        /* [in] */ IPoint * pOutGlobalOffset,
        /* [out] */ Boolean * pResult);

    CARAPI GetGlobalVisibleRectEx(
        /* [in] */ IRect * pOutRect,
        /* [out] */ Boolean * pResult);

    CARAPI GetLocalVisibleRect(
        /* [in] */ IRect * pOutRect,
        /* [out] */ Boolean * pResult);

    CARAPI OffsetTopAndBottom(
        /* [in] */ Int32 offset);

    CARAPI OffsetLeftAndRight(
        /* [in] */ Int32 offset);

    CARAPI GetLayoutParams(
        /* [out] */ IViewGroupLayoutParams ** ppParams);

    CARAPI SetLayoutParams(
        /* [in] */ IViewGroupLayoutParams * pParams);

    CARAPI ScrollTo(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI ScrollBy(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI Invalidate(
        /* [in] */ IRect * pDirty);

    CARAPI InvalidateEx(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI InvalidateEx2();

    CARAPI IsOpaque(
        /* [out] */ Boolean * pOpaque);

    CARAPI GetHandler(
        /* [out] */ IApartment ** ppApartment);

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
        /* [out] */ Boolean * pHorizontalFadingEdgeEnabled);

    CARAPI SetHorizontalFadingEdgeEnabled(
        /* [in] */ Boolean horizontalFadingEdgeEnabled);

    CARAPI IsVerticalFadingEdgeEnabled(
        /* [out] */ Boolean * pVerticalFadingEdgeEnabled);

    CARAPI SetVerticalFadingEdgeEnabled(
        /* [in] */ Boolean verticalFadingEdgeEnabled);

    CARAPI IsHorizontalScrollBarEnabled(
        /* [out] */ Boolean * pHorizontalScrollBarEnabled);

    CARAPI SetHorizontalScrollBarEnabled(
        /* [in] */ Boolean horizontalScrollBarEnabled);

    CARAPI IsVerticalScrollBarEnabled(
        /* [out] */ Boolean * pVerticalScrollBarEnabled);

    CARAPI SetVerticalScrollBarEnabled(
        /* [in] */ Boolean verticalScrollBarEnabled);

    CARAPI SetScrollbarFadingEnabled(
        /* [in] */ Boolean fadeScrollbars);

    CARAPI IsScrollbarFadingEnabled(
        /* [out] */ Boolean * pFadeScrollbars);

    CARAPI SetScrollBarStyle(
        /* [in] */ Int32 style);

    CARAPI GetScrollBarStyle(
        /* [out] */ Int32 * pStyle);

    CARAPI AssignParent(
        /* [in] */ IViewParent * pParent);

    CARAPI GetWindowToken(
        /* [out] */ IBinder ** ppToken);

    CARAPI GetApplicationWindowToken(
        /* [out] */ IBinder ** ppToken);

    CARAPI SaveHierarchyState(
        /* [in] */ IObjectIntegerMap * pContainer);

    CARAPI RestoreHierarchyState(
        /* [in] */ IObjectIntegerMap * pContainer);

    CARAPI GetDrawingTime(
        /* [out] */ Int64 * pTime);

    CARAPI SetDuplicateParentStateEnabled(
        /* [in] */ Boolean enabled);

    CARAPI IsDuplicateParentStateEnabled(
        /* [out] */ Boolean * pResult);

    CARAPI SetDrawingCacheEnabled(
        /* [in] */ Boolean enabled);

    CARAPI IsDrawingCacheEnabled(
        /* [out] */ Boolean * pEnabled);

    CARAPI GetDrawingCache(
        /* [out] */ IBitmap ** ppBitmap);

    CARAPI GetDrawingCacheEx(
        /* [in] */ Boolean autoScale,
        /* [out] */ IBitmap ** ppBitmap);

    CARAPI DestroyDrawingCache();

    CARAPI SetDrawingCacheBackgroundColor(
        /* [in] */ Int32 color);

    CARAPI GetDrawingCacheBackgroundColor(
        /* [out] */ Int32 * pColor);

    CARAPI BuildDrawingCache();

    CARAPI BuildDrawingCacheEx(
        /* [in] */ Boolean autoScale);

    CARAPI CreateSnapshot(
        /* [in] */ BitmapConfig quality,
        /* [in] */ Int32 backgroundColor,
        /* [in] */ Boolean skipChildren,
        /* [out] */ IBitmap ** ppBitmap);

    CARAPI IsInEditMode(
        /* [out] */ Boolean * pResult);

    CARAPI Draw(
        /* [in] */ ICanvas * pCanvas);

    CARAPI GetSolidColor(
        /* [out] */ Int32 * pColor);

    CARAPI OnFinishInflate();

    CARAPI IsLayoutRequested(
        /* [out] */ Boolean * pResult);

    CARAPI Layout(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI GetResources(
        /* [out] */ IResources ** ppResources);

    CARAPI UnscheduleDrawableEx(
        /* [in] */ IDrawable * pWho);

    CARAPI RefreshDrawableState();

    CARAPI GetDrawableState(
        /* [out, callee] */ ArrayOf<Int32> ** ppState);

    CARAPI SetBackgroundColor(
        /* [in] */ Int32 color);

    CARAPI SetBackgroundResource(
        /* [in] */ Int32 resid);

    CARAPI SetBackgroundDrawable(
        /* [in] */ IDrawable * pD);

    CARAPI GetBackground(
        /* [out] */ IDrawable ** ppDrawable);

    CARAPI SetPadding(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI GetPaddingTop(
        /* [out] */ Int32 * pTop);

    CARAPI GetPaddingBottom(
        /* [out] */ Int32 * pBottom);

    CARAPI GetPaddingLeft(
        /* [out] */ Int32 * pLeft);

    CARAPI GetPaddingRight(
        /* [out] */ Int32 * pRight);

    CARAPI SetSelected(
        /* [in] */ Boolean selected);

    CARAPI IsSelected(
        /* [out] */ Boolean * pSelected);

    CARAPI GetViewTreeObserver(
        /* [out] */ IViewTreeObserver ** ppObserver);

    CARAPI GetRootView(
        /* [out] */ IView ** ppView);

    CARAPI GetLocationOnScreen(
        /* [out] */ Int32 * pX,
        /* [out] */ Int32 * pY);

    CARAPI GetLocationInWindow(
        /* [out] */ Int32 * pX,
        /* [out] */ Int32 * pY);

    CARAPI FindViewById(
        /* [in] */ Int32 id,
        /* [out] */ IView ** ppView);

    CARAPI SetId(
        /* [in] */ Int32 id);

    CARAPI GetId(
        /* [out] */ Int32 * pId);

    CARAPI IsRootNamespace(
        /* [out] */ Boolean * pResult);

    CARAPI GetBaseline(
        /* [out] */ Int32 * pResult);

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
        /* [out] */ IAnimation ** ppAnimation);

    CARAPI StartAnimation(
        /* [in] */ IAnimation * pAnimation);

    CARAPI ClearAnimation();

    CARAPI SetAnimation(
        /* [in] */ IAnimation * pAnimation);

    CARAPI GatherTransparentRegion(
        /* [in] */ IRegion * pRegion,
        /* [out] */ Boolean * pResult);

    CARAPI PlaySoundEffect(
        /* [in] */ Int32 soundConstant);

    CARAPI PerformHapticFeedback(
        /* [in] */ Int32 feedbackConstant,
        /* [out] */ Boolean * pResult);

    CARAPI PerformHapticFeedbackEx(
        /* [in] */ Int32 feedbackConstant,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean * pResult);

    CARAPI OnCloseSystemDialogs(
        /* [in] */ const String& reason);

    CARAPI ApplyDrawableToTransparentRegion(
        /* [in] */ IDrawable * pDr,
        /* [in] */ IRegion * pRegion);

    CARAPI GetOverScrollMode(
        /* [out] */ Int32 * pMode);

    CARAPI SetOverScrollMode(
        /* [in] */ Int32 overScrollMode);

    CARAPI GetDescendantFocusability(
        /* [out] */ Int32 * pFocusability);

    CARAPI SetDescendantFocusability(
        /* [in] */ Int32 focusability);

    CARAPI GetFocusedChild(
        /* [out] */ IView ** ppFocusedChild);

    CARAPI OnInterceptTouchEvent(
        /* [in] */ IMotionEvent * pEv,
        /* [out] */ Boolean * pValue);

    CARAPI SetClipChildren(
        /* [in] */ Boolean clipChildren);

    CARAPI SetClipToPadding(
        /* [in] */ Boolean clipToPadding);

    CARAPI AddView(
        /* [in] */ IView * pChild);

    CARAPI AddViewEx(
        /* [in] */ IView * pChild,
        /* [in] */ Int32 index);

    CARAPI AddViewEx2(
        /* [in] */ IView * pChild,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI AddViewEx3(
        /* [in] */ IView * pChild,
        /* [in] */ IViewGroupLayoutParams * pParams);

    CARAPI AddViewEx4(
        /* [in] */ IView * pChild,
        /* [in] */ Int32 index,
        /* [in] */ IViewGroupLayoutParams * pParams);

    CARAPI SetOnHierarchyChangeListener(
        /* [in] */ IViewGroupOnHierarchyChangeListener * pListener);

    CARAPI RemoveViewInLayout(
        /* [in] */ IView * pView);

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
        /* [in] */ IView * pDescendant,
        /* [in] */ IRect * pRect);

    CARAPI OffsetRectIntoDescendantCoords(
        /* [in] */ IView * pDescendant,
        /* [in] */ IRect * pRect);

    CARAPI OffsetChildrenTopAndBottom(
        /* [in] */ Int32 offset);

    CARAPI StartLayoutAnimation();

    CARAPI ScheduleLayoutAnimation();

    CARAPI SetLayoutAnimation(
        /* [in] */ ILayoutAnimationController * pController);

    CARAPI GetLayoutAnimation(
        /* [out] */ ILayoutAnimationController ** ppController);

    CARAPI IsAnimationCacheEnabled(
        /* [out] */ Boolean * pEnabled);

    CARAPI SetAnimationCacheEnabled(
        /* [in] */ Boolean enabled);

    CARAPI IsAlwaysDrawnWithCacheEnabled(
        /* [out] */ Boolean * pAlways);

    CARAPI SetAlwaysDrawnWithCacheEnabled(
        /* [in] */ Boolean always);

    CARAPI GetPersistentDrawingCache(
        /* [out] */ Int32 * pDrawingCacheToKeep);

    CARAPI SetPersistentDrawingCache(
        /* [in] */ Int32 drawingCacheToKeep);

    CARAPI GenerateLayoutParams(
        /* [in] */ IAttributeSet * pAttrs,
        /* [out] */ IViewGroupLayoutParams ** ppLayoutParams);

    CARAPI IndexOfChild(
        /* [in] */ IView * pChild,
        /* [out] */ Int32 * pIndex);

    CARAPI GetChildCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetChildAt(
        /* [in] */ Int32 index,
        /* [out] */ IView ** ppChild);

    CARAPI ClearDisappearingChildren();

    CARAPI GetLayoutAnimationListener(
        /* [out] */ IAnimationListener ** ppListener);

    CARAPI SetAddStatesFromChildren(
        /* [in] */ Boolean addsStates);

    CARAPI AddStatesFromChildren(
        /* [out] */ Boolean * pDrawable);

    CARAPI SetLayoutAnimationListener(
        /* [in] */ IAnimationListener * pAnimationListener);

    CARAPI UpdateMultiTouchSupport(
        /* [in] */ IContext * pContext);

    CARAPI SetHorizontalScrollbarOverlay(
        /* [in] */ Boolean overlay);

    CARAPI SetVerticalScrollbarOverlay(
        /* [in] */ Boolean overlay);

    CARAPI OverlayHorizontalScrollbar();

    CARAPI OverlayVerticalScrollbar();

    CARAPI GetCertificate();

    CARAPI SetCertificate(
        /* [in] */ ISslCertificate * pCertificate);

    CARAPI SavePassword(
        /* [in] */ CString host,
        /* [in] */ CString username,
        /* [in] */ CString password);

    CARAPI SetHttpAuthUsernamePassword(
        /* [in] */ CString host,
        /* [in] */ CString realm,
        /* [in] */ CString username,
        /* [in] */ CString password);

    CARAPI GetHttpAuthUsernamePassword(
        /* [in] */ CString host,
        /* [in] */ CString realm);

    CARAPI Destroy();

    CARAPI EnablePlatformNotifications();

    CARAPI DisablePlatformNotifications();

    CARAPI SetJsFlags(
        /* [in] */ CString flags);

    CARAPI SetNetworkAvailable(
        /* [in] */ Boolean networkUp);

    CARAPI SetNetworkType(
        /* [in] */ CString type,
        /* [in] */ CString subtype);

    CARAPI SaveState(
        /* [in] */ IBundle * pOutState);

    CARAPI SavePicture(
        /* [in] */ IBundle * pB,
        /* [in] */ IFile * pDest);

    CARAPI RestorePicture(
        /* [in] */ IBundle * pB,
        /* [in] */ IFile * pSrc);

    CARAPI RestoreState(
        /* [in] */ IBundle * pInState);

    CARAPI LoadUrlEx(
        /* [in] */ CString url,
        /* [in] */ IObjectStringMap * pExtraHeaders);

    CARAPI LoadUrl(
        /* [in] */ CString url);

    CARAPI PostUrl(
        /* [in] */ CString url,
        /* [in] */ const ArrayOf<Byte> & postData);

    CARAPI LoadData(
        /* [in] */ CString data,
        /* [in] */ CString mimeType,
        /* [in] */ CString encoding);

    CARAPI LoadDataWithBaseURL(
        /* [in] */ CString baseUrl,
        /* [in] */ CString data,
        /* [in] */ CString mimeType,
        /* [in] */ CString encoding,
        /* [in] */ CString historyUrl);

    CARAPI StopLoading();

    CARAPI Reload();

    CARAPI CanGoBack();

    CARAPI GoBack();

    CARAPI CanGoForward();

    CARAPI GoForward();

    CARAPI CanGoBackOrForward(
        /* [in] */ Int32 steps);

    CARAPI GoBackOrForward(
        /* [in] */ Int32 steps);

    CARAPI PageUp(
        /* [in] */ Boolean top);

    CARAPI PageDown(
        /* [in] */ Boolean bottom);

    CARAPI ClearView();

    CARAPI CapturePicture();

    CARAPI GetScale();

    CARAPI SetInitialScale(
        /* [in] */ Int32 scaleInPercent);

    CARAPI InvokeZoomPicker();

    CARAPI GetHitTestResult();

    CARAPI RequestFocusNodeHref(
        /* [in] */ IMessage * pHrefMsg);

    CARAPI RequestImageRef(
        /* [in] */ IMessage * pMsg);

    CARAPI SetEmbeddedTitleBar(
        /* [in] */ IView * pV);

    CARAPI GetUrl();

    CARAPI GetOriginalUrl();

    CARAPI GetTitle();

    CARAPI GetFavicon();

    CARAPI GetTouchIconUrl();

    CARAPI GetProgress();

    CARAPI GetContentHeight();

    CARAPI GetContentWidth();

    CARAPI PauseTimers();

    CARAPI ResumeTimers();

    CARAPI OnPause();

    CARAPI OnResume();

    CARAPI IsPaused();

    CARAPI FreeMemory();

    CARAPI ClearCache(
        /* [in] */ Boolean includeDiskFiles);

    CARAPI ClearFormData();

    CARAPI ClearHistory();

    CARAPI ClearSslPreferences();

    CARAPI CopyBackForwardList();

    CARAPI FindNext(
        /* [in] */ Boolean forward);

    CARAPI FindAll(
        /* [in] */ CString find);

    CARAPI SetFindIsUp(
        /* [in] */ Boolean isUp);

    CARAPI FindIndex();

    CARAPI FindAddress(
        /* [in] */ CString addr);

    CARAPI FindAddressEx(
        /* [in] */ CString addr,
        /* [in] */ Boolean caseInsensitive);

    CARAPI ClearMatches();

    CARAPI NotifyFindDialogDismissed();

    CARAPI SetFindDialogHeight(
        /* [in] */ Int32 height);

    CARAPI DocumentHasImages(
        /* [in] */ IMessage * pResponse);

    CARAPI SetWebViewClient(
        /* [in] */ IWebViewClient * pClient);

    CARAPI GetWebViewClient();

    CARAPI SetDownloadListener(
        /* [in] */ IDownloadListener * pListener);

    CARAPI SetWebChromeClient(
        /* [in] */ IWebChromeClient * pClient);

    CARAPI GetWebChromeClient();

    CARAPI SetWebBackForwardListClient(
        /* [in] */ IWebBackForwardListClient * pClient);

    CARAPI GetWebBackForwardListClient();

    CARAPI SetPictureListener(
        /* [in] */ IWebViewPictureListener * pListener);

    CARAPI ExternalRepresentation(
        /* [in] */ IMessage * pCallBack);

    CARAPI DocumentAsText(
        /* [in] */ IMessage * pCallBack);

    CARAPI AddJavascriptInterface(
        /* [in] */ IInterface * pObj,
        /* [in] */ CString interfaceName);

    CARAPI GetSettings();

    CARAPI AddPackageNames(
        /* [in] */ IObjectContainer * pPackageNames);

    CARAPI AddPackageName(
        /* [in] */ CString packageName);

    CARAPI RemovePackageName(
        /* [in] */ CString packageName);

    CARAPI GetPluginList();

    CARAPI RefreshPlugins(
        /* [in] */ Boolean reloadOpenPages);

    CARAPI DumpDisplayTree();

    CARAPI DumpDomTree(
        /* [in] */ Boolean toFile);

    CARAPI DumpRenderTree(
        /* [in] */ Boolean toFile);

    CARAPI DumpV8Counters();

    CARAPI OnKeyMultiple(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 repeatCount,
        /* [in] */ IKeyEvent * pEvent);

    CARAPI OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent * pEvent);

    CARAPI OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent * pEvent);

    CARAPI SetUpSelect();

    CARAPI EmulateShiftHeld();

    CARAPI SelectAll();

    CARAPI SelectDialogIsUp();

    CARAPI NotifySelectDialogDismissed();

    CARAPI SelectionDone();

    CARAPI CopySelection();

    CARAPI GetSelection();

    CARAPI OnChildViewAdded(
        /* [in] */ IView * pParent,
        /* [in] */ IView * pChild);

    CARAPI OnChildViewRemoved(
        /* [in] */ IView * pP,
        /* [in] */ IView * pChild);

    CARAPI OnGlobalFocusChanged(
        /* [in] */ IView * pOldFocus,
        /* [in] */ IView * pNewFocus);

    CARAPI GetDragTracker();

    CARAPI SetDragTracker(
        /* [in] */ IWebViewDragTracker * pTracker);

    CARAPI SetMapTrackballToArrowKeys(
        /* [in] */ Boolean setMap);

    CARAPI FlingScroll(
        /* [in] */ Int32 vx,
        /* [in] */ Int32 vy);

    CARAPI GetZoomControls();

    CARAPI GetZoomButtonsController();

    CARAPI ZoomIn();

    CARAPI ZoomOut();

    CARAPI RequestChildRectangleOnScreen(
        /* [in] */ IView * pChild,
        /* [in] */ IRect * pRect,
        /* [in] */ Boolean immediate);

    CARAPI DebugDump();

    CARAPI DrawPage(
        /* [in] */ ICanvas * pCanvas);

    CARAPI SetTouchInterval(
        /* [in] */ Int32 interval);

    CARAPI constructor(
        /* [in] */ IContext * pContext);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IAttributeSet * pAttrs);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IAttributeSet * pAttrs,
        /* [in] */ Int32 defStyle);

private:
    // TODO: Add your private member variables here.
};

#endif // __CWEBVIEW_H__
