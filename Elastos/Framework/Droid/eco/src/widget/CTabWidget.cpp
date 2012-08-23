
#include "widget/CTabWidget.h"

PInterface CTabWidget::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CTabWidget::Probe(riid);
}

ECode CTabWidget::GetVerticalFadingEdgeLength(
    /* [out] */ Int32 * pLength)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetFadingEdgeLength(
    /* [in] */ Int32 length)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetHorizontalFadingEdgeLength(
    /* [out] */ Int32 * pLength)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetVerticalScrollbarWidth(
    /* [out] */ Int32 * pWidth)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetHorizontalScrollbarHeight(
    /* [out] */ Int32 * pHeight)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetOnFocusChangeListener(
    /* [in] */ IViewOnFocusChangeListener * pL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetOnFocusChangeListener(
    /* [out] */ IViewOnFocusChangeListener ** ppL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetOnClickListener(
    /* [in] */ IViewOnClickListener * pL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetOnLongClickListener(
    /* [in] */ IViewOnLongClickListener * pL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetOnCreateContextMenuListener(
    /* [in] */ IViewOnCreateContextMenuListener * pL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::PerformClick(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::PerformLongClick(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::ShowContextMenu(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetOnKeyListener(
    /* [in] */ IViewOnKeyListener * pL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetOnTouchListener(
    /* [in] */ IViewOnTouchListener * pL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::RequestRectangleOnScreen(
    /* [in] */ IRect * pRectangle,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::RequestRectangleOnScreenEx(
    /* [in] */ IRect * pRectangle,
    /* [in] */ Boolean immediate,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::ClearFocus()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::HasFocus(
    /* [out] */ Boolean * pHasFocus)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::HasFocusable(
    /* [out] */ Boolean * pHasFocusable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetContentDescription(
    /* [out, callee] */ ArrayOf<Char8> ** ppDescription)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetContentDescription(
    /* [in] */ const ArrayOf<Char8> & contentDescription)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsFocused(
    /* [out] */ Boolean * pFocused)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::FindFocus(
    /* [out] */ IView ** ppView)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetScrollContainer(
    /* [in] */ Boolean isScrollContainer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetDrawingCacheQuality(
    /* [out] */ Int32 * pQuality)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetDrawingCacheQuality(
    /* [in] */ Int32 quality)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetKeepScreenOn(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetKeepScreenOn(
    /* [in] */ Boolean keepScreenOn)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetNextFocusLeftId(
    /* [out] */ Int32 * pNextFocusLeftId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetNextFocusLeftId(
    /* [in] */ Int32 nextFocusLeftId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetNextFocusRightId(
    /* [out] */ Int32 * pNextFocusRightId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetNextFocusRightId(
    /* [in] */ Int32 nextFocusRightId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetNextFocusUpId(
    /* [out] */ Int32 * pNextFocusUpId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetNextFocusUpId(
    /* [in] */ Int32 nextFocusUpId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetNextFocusDownId(
    /* [out] */ Int32 * pNextFocusDownId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetNextFocusDownId(
    /* [in] */ Int32 nextFocusDownId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsShown(
    /* [out] */ Boolean * pShown)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsFitsSystemWindowsFlagSet(
    /* [out] */ Boolean * pFlag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetVisibility(
    /* [out] */ Int32 * pVisibility)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetVisibility(
    /* [in] */ Int32 visibility)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsEnabled(
    /* [out] */ Boolean * pEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetEnabled(
    /* [in] */ Boolean enabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetFocusable(
    /* [in] */ Boolean focusable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetFocusableInTouchMode(
    /* [in] */ Boolean focusableInTouchMode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetSoundEffectsEnabled(
    /* [in] */ Boolean soundEffectsEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsSoundEffectsEnabled(
    /* [out] */ Boolean * pSoundEffectsEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetHapticFeedbackEnabled(
    /* [in] */ Boolean hapticFeedbackEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsHapticFeedbackEnabled(
    /* [out] */ Boolean * pHapticFeedbackEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetWillNotDraw(
    /* [in] */ Boolean willNotDraw)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::WillNotDraw(
    /* [out] */ Boolean * pWillNotDraw)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetWillNotCacheDrawing(
    /* [in] */ Boolean willNotCacheDrawing)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::WillNotCacheDrawing(
    /* [out] */ Boolean * pCache)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsClickable(
    /* [out] */ Boolean * pClickable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetClickable(
    /* [in] */ Boolean clickable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsLongClickable(
    /* [out] */ Boolean * pLongClickable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetLongClickable(
    /* [in] */ Boolean longClickable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetPressed(
    /* [in] */ Boolean pressed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsPressed(
    /* [out] */ Boolean * pPressed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsSaveEnabled(
    /* [out] */ Boolean * pEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetSaveEnabled(
    /* [in] */ Boolean enabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetFilterTouchesWhenObscured(
    /* [out] */ Boolean * pEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetFilterTouchesWhenObscured(
    /* [in] */ Boolean enabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsFocusable(
    /* [out] */ Boolean * pFocusable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsFocusableInTouchMode(
    /* [out] */ Boolean * pFocusable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::FocusSearch(
    /* [in] */ Int32 direction,
    /* [out] */ IView ** ppView)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::DispatchUnhandledMove(
    /* [in] */ IView * pFocused,
    /* [in] */ Int32 direction,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetFocusables(
    /* [in] */ Int32 direction,
    /* [out] */ IObjectContainer ** ppViews)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::AddFocusables(
    /* [in] */ IObjectContainer * pViews,
    /* [in] */ Int32 direction)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::AddFocusablesEx(
    /* [in] */ IObjectContainer * pViews,
    /* [in] */ Int32 direction,
    /* [in] */ Int32 focusableMode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetTouchables(
    /* [out] */ IObjectContainer ** ppViews)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::AddTouchables(
    /* [in] */ IObjectContainer * pViews)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::RequestFocus(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::RequestFocusEx(
    /* [in] */ Int32 direction,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::RequestFocusEx2(
    /* [in] */ Int32 direction,
    /* [in] */ IRect * pPreviouslyFocusedRect,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::RequestFocusFromTouch(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::DispatchStartTemporaryDetach()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::OnStartTemporaryDetach()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::DispatchFinishTemporaryDetach()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::OnFinishTemporaryDetach()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetKeyDispatcherState(
    /* [out] */ IDispatcherState ** ppState)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::DispatchKeyEventPreIme(
    /* [in] */ IKeyEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::DispatchKeyEvent(
    /* [in] */ IKeyEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::DispatchKeyShortcutEvent(
    /* [in] */ IKeyEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::DispatchTouchEvent(
    /* [in] */ IMotionEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::OnFilterTouchEventForSecurity(
    /* [in] */ IMotionEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::DispatchTrackballEvent(
    /* [in] */ IMotionEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::DispatchWindowFocusChanged(
    /* [in] */ Boolean hasFocus)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::HasWindowFocus(
    /* [out] */ Boolean * pFocus)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::DispatchDisplayHint(
    /* [in] */ Int32 hint)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::DispatchWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetWindowVisibility(
    /* [out] */ Int32 * pVisibility)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetWindowVisibleDisplayFrame(
    /* [in] */ IRect * pOutRect)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::DispatchConfigurationChanged(
    /* [in] */ IConfiguration * pNewConfig)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsInTouchMode(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetContext(
    /* [out] */ IContext ** ppCtx)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::OnKeyPreIme(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::OnKeyShortcut(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::OnCheckIsTextEditor(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::CheckInputConnectionProxy(
    /* [in] */ IView * pView,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::CreateContextMenu(
    /* [in] */ IContextMenu * pMenu)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::OnTrackballEvent(
    /* [in] */ IMotionEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::OnTouchEvent(
    /* [in] */ IMotionEvent * pEvent,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetTouchDelegate(
    /* [in] */ ITouchDelegate * pTouchDelegate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetTouchDelegate(
    /* [out] */ ITouchDelegate ** ppTouchDelegate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::CancelLongPress()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::BringToFront()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetParent(
    /* [out] */ IViewParent ** ppParent)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetScrollX(
    /* [out] */ Int32 * pX)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetScrollY(
    /* [out] */ Int32 * pY)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetWidth(
    /* [out] */ Int32 * pWidth)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetHeight(
    /* [out] */ Int32 * pHeight)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetDrawingRect(
    /* [in] */ IRect * pOutRect)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetMeasuredWidth(
    /* [out] */ Int32 * pWidth)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetMeasuredHeight(
    /* [out] */ Int32 * pHeight)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetTop(
    /* [out] */ Int32 * pTop)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetBottom(
    /* [out] */ Int32 * pBottom)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetLeft(
    /* [out] */ Int32 * pLeft)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetRight(
    /* [out] */ Int32 * pRight)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetHitRect(
    /* [in] */ IRect * pOutRect)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetFocusedRect(
    /* [in] */ IRect * pOutRect)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetGlobalVisibleRect(
    /* [in] */ IRect * pOutRect,
    /* [in] */ IPoint * pOutGlobalOffset,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetGlobalVisibleRectEx(
    /* [in] */ IRect * pOutRect,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetLocalVisibleRect(
    /* [in] */ IRect * pOutRect,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::OffsetTopAndBottom(
    /* [in] */ Int32 offset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::OffsetLeftAndRight(
    /* [in] */ Int32 offset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetLayoutParams(
    /* [out] */ IViewGroupLayoutParams ** ppParams)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetLayoutParams(
    /* [in] */ IViewGroupLayoutParams * pParams)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::ScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::ScrollBy(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::Invalidate(
    /* [in] */ IRect * pDirty)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::InvalidateEx(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::InvalidateEx2()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsOpaque(
    /* [out] */ Boolean * pOpaque)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::PostInvalidate()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::PostInvalidateEx(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::PostInvalidateDelayed(
    /* [in] */ Int64 delayMilliseconds)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::PostInvalidateDelayedEx(
    /* [in] */ Int64 delayMilliseconds,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::ComputeScroll()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsHorizontalFadingEdgeEnabled(
    /* [out] */ Boolean * pHorizontalFadingEdgeEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetHorizontalFadingEdgeEnabled(
    /* [in] */ Boolean horizontalFadingEdgeEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsVerticalFadingEdgeEnabled(
    /* [out] */ Boolean * pVerticalFadingEdgeEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetVerticalFadingEdgeEnabled(
    /* [in] */ Boolean verticalFadingEdgeEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsHorizontalScrollBarEnabled(
    /* [out] */ Boolean * pHorizontalScrollBarEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetHorizontalScrollBarEnabled(
    /* [in] */ Boolean horizontalScrollBarEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsVerticalScrollBarEnabled(
    /* [out] */ Boolean * pVerticalScrollBarEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetVerticalScrollBarEnabled(
    /* [in] */ Boolean verticalScrollBarEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetScrollbarFadingEnabled(
    /* [in] */ Boolean fadeScrollbars)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsScrollbarFadingEnabled(
    /* [out] */ Boolean * pFadeScrollbars)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetScrollBarStyle(
    /* [in] */ Int32 style)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetScrollBarStyle(
    /* [out] */ Int32 * pStyle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::AssignParent(
    /* [in] */ IViewParent * pParent)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetWindowToken(
    /* [out] */ IBinder ** ppToken)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetApplicationWindowToken(
    /* [out] */ IBinder ** ppToken)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SaveHierarchyState(
    /* [in] */ IObjectIntegerMap * pContainer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::RestoreHierarchyState(
    /* [in] */ IObjectIntegerMap * pContainer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetDrawingTime(
    /* [out] */ Int64 * pTime)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetDuplicateParentStateEnabled(
    /* [in] */ Boolean enabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsDuplicateParentStateEnabled(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetDrawingCacheEnabled(
    /* [in] */ Boolean enabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsDrawingCacheEnabled(
    /* [out] */ Boolean * pEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetDrawingCache(
    /* [out] */ IBitmap ** ppBitmap)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetDrawingCacheEx(
    /* [in] */ Boolean autoScale,
    /* [out] */ IBitmap ** ppBitmap)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::DestroyDrawingCache()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetDrawingCacheBackgroundColor(
    /* [in] */ Int32 color)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetDrawingCacheBackgroundColor(
    /* [out] */ Int32 * pColor)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::BuildDrawingCache()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::BuildDrawingCacheEx(
    /* [in] */ Boolean autoScale)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::CreateSnapshot(
    /* [in] */ BitmapConfig quality,
    /* [in] */ Int32 backgroundColor,
    /* [in] */ Boolean skipChildren,
    /* [out] */ IBitmap ** ppBitmap)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsInEditMode(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::Draw(
    /* [in] */ ICanvas * pCanvas)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetSolidColor(
    /* [out] */ Int32 * pColor)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::OnFinishInflate()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsLayoutRequested(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::Layout(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetResources(
    /* [out] */ IResources ** ppResources)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::UnscheduleDrawableEx(
    /* [in] */ IDrawable * pWho)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::RefreshDrawableState()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetDrawableState(
    /* [out, callee] */ ArrayOf<Int32> ** ppState)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetBackgroundColor(
    /* [in] */ Int32 color)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetBackgroundResource(
    /* [in] */ Int32 resid)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetBackgroundDrawable(
    /* [in] */ IDrawable * pD)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetBackground(
    /* [out] */ IDrawable ** ppDrawable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetPadding(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetPaddingTop(
    /* [out] */ Int32 * pTop)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetPaddingBottom(
    /* [out] */ Int32 * pBottom)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetPaddingLeft(
    /* [out] */ Int32 * pLeft)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetPaddingRight(
    /* [out] */ Int32 * pRight)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetSelected(
    /* [in] */ Boolean selected)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsSelected(
    /* [out] */ Boolean * pSelected)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetViewTreeObserver(
    /* [out] */ IViewTreeObserver ** ppObserver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetRootView(
    /* [out] */ IView ** ppView)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetLocationOnScreen(
    /* [out] */ Int32 * pX,
    /* [out] */ Int32 * pY)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetLocationInWindow(
    /* [out] */ Int32 * pX,
    /* [out] */ Int32 * pY)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::FindViewById(
    /* [in] */ Int32 id,
    /* [out] */ IView ** ppView)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetId(
    /* [in] */ Int32 id)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetId(
    /* [out] */ Int32 * pId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsRootNamespace(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetBaseline(
    /* [out] */ Int32 * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::RequestLayout()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::ForceLayout()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::Measure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetMinimumHeight(
    /* [in] */ Int32 minHeight)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetMinimumWidth(
    /* [in] */ Int32 minWidth)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetAnimation(
    /* [out] */ IAnimation ** ppAnimation)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::StartAnimation(
    /* [in] */ IAnimation * pAnimation)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::ClearAnimation()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetAnimation(
    /* [in] */ IAnimation * pAnimation)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GatherTransparentRegion(
    /* [in] */ IRegion * pRegion,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::PlaySoundEffect(
    /* [in] */ Int32 soundConstant)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::PerformHapticFeedback(
    /* [in] */ Int32 feedbackConstant,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::PerformHapticFeedbackEx(
    /* [in] */ Int32 feedbackConstant,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::OnCloseSystemDialogs(
    /* [in] */ const String& reason)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::ApplyDrawableToTransparentRegion(
    /* [in] */ IDrawable * pDr,
    /* [in] */ IRegion * pRegion)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetOverScrollMode(
    /* [out] */ Int32 * pMode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetOverScrollMode(
    /* [in] */ Int32 overScrollMode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetDescendantFocusability(
    /* [out] */ Int32 * pFocusability)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetDescendantFocusability(
    /* [in] */ Int32 focusability)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetFocusedChild(
    /* [out] */ IView ** ppFocusedChild)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent * pEv,
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetClipChildren(
    /* [in] */ Boolean clipChildren)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetClipToPadding(
    /* [in] */ Boolean clipToPadding)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::AddView(
    /* [in] */ IView * pChild)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::AddViewEx(
    /* [in] */ IView * pChild,
    /* [in] */ Int32 index)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::AddViewEx2(
    /* [in] */ IView * pChild,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::AddViewEx3(
    /* [in] */ IView * pChild,
    /* [in] */ IViewGroupLayoutParams * pParams)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::AddViewEx4(
    /* [in] */ IView * pChild,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams * pParams)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetOnHierarchyChangeListener(
    /* [in] */ IViewGroupOnHierarchyChangeListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::RemoveViewInLayout(
    /* [in] */ IView * pView)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::RemoveViewsInLayout(
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::RemoveViewAt(
    /* [in] */ Int32 index)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::RemoveViews(
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::RemoveAllViews()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::RemoveAllViewsInLayout()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::OffsetDescendantRectToMyCoords(
    /* [in] */ IView * pDescendant,
    /* [in] */ IRect * pRect)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::OffsetRectIntoDescendantCoords(
    /* [in] */ IView * pDescendant,
    /* [in] */ IRect * pRect)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::OffsetChildrenTopAndBottom(
    /* [in] */ Int32 offset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::StartLayoutAnimation()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::ScheduleLayoutAnimation()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetLayoutAnimation(
    /* [in] */ ILayoutAnimationController * pController)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetLayoutAnimation(
    /* [out] */ ILayoutAnimationController ** ppController)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsAnimationCacheEnabled(
    /* [out] */ Boolean * pEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetAnimationCacheEnabled(
    /* [in] */ Boolean enabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsAlwaysDrawnWithCacheEnabled(
    /* [out] */ Boolean * pAlways)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetAlwaysDrawnWithCacheEnabled(
    /* [in] */ Boolean always)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetPersistentDrawingCache(
    /* [out] */ Int32 * pDrawingCacheToKeep)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetPersistentDrawingCache(
    /* [in] */ Int32 drawingCacheToKeep)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GenerateLayoutParams(
    /* [in] */ IAttributeSet * pAttrs,
    /* [out] */ IViewGroupLayoutParams ** ppLayoutParams)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IndexOfChild(
    /* [in] */ IView * pChild,
    /* [out] */ Int32 * pIndex)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetChildCount(
    /* [out] */ Int32 * pCount)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetChildAt(
    /* [in] */ Int32 index,
    /* [out] */ IView ** ppChild)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::ClearDisappearingChildren()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetLayoutAnimationListener(
    /* [out] */ IAnimationListener ** ppListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetAddStatesFromChildren(
    /* [in] */ Boolean addsStates)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::AddStatesFromChildren(
    /* [out] */ Boolean * pDrawable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetLayoutAnimationListener(
    /* [in] */ IAnimationListener * pAnimationListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsBaselineAligned(
    /* [out] */ Boolean * pBaselineAligned)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetBaselineAligned(
    /* [in] */ Boolean baselineAligned)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetBaselineAlignedChildIndex(
    /* [out] */ Int32 * pBaselineAlignedChildIndex)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetBaselineAlignedChildIndex(
    /* [in] */ Int32 baselineAlignedChildIndex)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetWeightSum(
    /* [out] */ Float * pWeightSum)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetWeightSum(
    /* [in] */ Float weightSum)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetOrientation(
    /* [in] */ Int32 orientation)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetOrientation(
    /* [out] */ Int32 * pOrientation)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetGravity(
    /* [in] */ Int32 gravity)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetHorizontalGravity(
    /* [in] */ Int32 horizontalGravity)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetVerticalGravity(
    /* [in] */ Int32 verticalGravity)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetChildTabViewAt(
    /* [in] */ Int32 index,
    /* [out] */ IView ** ppView)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::GetTabCount(
    /* [out] */ Int32 * pCount)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetDividerDrawable(
    /* [in] */ IDrawable * pDrawable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetDividerDrawableEx(
    /* [in] */ Int32 resId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetLeftStripDrawable(
    /* [in] */ IDrawable * pDrawable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetLeftStripDrawableEx(
    /* [in] */ Int32 resId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetRightStripDrawable(
    /* [in] */ IDrawable * pDrawable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetRightStripDrawableEx(
    /* [in] */ Int32 resId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetStripEnabled(
    /* [in] */ Boolean stripEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::IsStripEnabled(
    /* [out] */ Boolean * pStripEnabled)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::SetCurrentTab(
    /* [in] */ Int32 index)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::FocusCurrentTab(
    /* [in] */ Int32 index)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::OnFocusChange(
    /* [in] */ IView * pV,
    /* [in] */ Boolean hasFocus)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::constructor(
    /* [in] */ IContext * pCtx)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabWidget::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

