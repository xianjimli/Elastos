
#include "widget/CPopupWindow.h"

ECode CPopupWindow::GetBackground(
    /* [out] */ IDrawable** background)
{
    return PopupWindow::GetBackground(background);
}

ECode CPopupWindow::SetBackgroundDrawable(
    /* [in] */ IDrawable* background)
{
    return PopupWindow::SetBackgroundDrawable(background);
}

ECode CPopupWindow::GetAnimationStyle(
    /* [out] */ Int32* style)
{
    return PopupWindow::GetAnimationStyle(style);
}

ECode CPopupWindow::SetIgnoreCheekPress()
{
    return PopupWindow::SetIgnoreCheekPress();
}

ECode CPopupWindow::SetAnimationStyle(
    /* [in] */ Int32 animationStyle)
{
    return PopupWindow::SetAnimationStyle(animationStyle);
}

ECode CPopupWindow::GetContentView(
    /* [out] */ IView** contentView)
{
    return PopupWindow::GetContentView(contentView);
}

ECode CPopupWindow::SetContentView(
    /* [in] */ IView* contentView)
{
    return PopupWindow::SetContentView(contentView);
}

ECode CPopupWindow::SetTouchInterceptor(
    /* [in] */ IViewOnTouchListener* l)
{
    return PopupWindow::SetTouchInterceptor(l);
}

ECode CPopupWindow::IsFocusable(
    /* [out] */ Boolean* isFocusable)
{
    return PopupWindow::IsFocusable(isFocusable);
}

ECode CPopupWindow::SetFocusable(
     /* [in] */ Boolean focusable)
{
    return PopupWindow::SetFocusable(focusable);
}

ECode CPopupWindow::GetInputMethodMode(
    /* [out] */ Int32* inputMethodMode)
{
    return PopupWindow::GetInputMethodMode(inputMethodMode);
}

ECode CPopupWindow::SetInputMethodMode(
    /* [in] */ Int32 mode)
{
    return PopupWindow::SetInputMethodMode(mode);
}

ECode CPopupWindow::SetSoftInputMode(
    /* [in] */ Int32 mode)
{
    return PopupWindow::SetSoftInputMode(mode);
}

ECode CPopupWindow::GetSoftInputMode(
    /* [out] */ Int32* softInputMode)
{
    return PopupWindow::GetSoftInputMode(softInputMode);
}

ECode CPopupWindow::IsTouchable(
    /* [out] */ Boolean* touchable)
{
    return PopupWindow::IsTouchable(touchable);
}

ECode CPopupWindow::SetTouchable(
    /* [in] */ Boolean touchable)
{
    return PopupWindow::SetTouchable(touchable);
}

ECode CPopupWindow::IsOutsideTouchable(
    /* [out] */ Boolean* touchable)
{
    return PopupWindow::IsOutsideTouchable(touchable);
}

ECode CPopupWindow::SetOutsideTouchable(
    /* [in] */ Boolean touchable)
{
    return PopupWindow::SetOutsideTouchable(touchable);
}

ECode CPopupWindow::IsClippingEnabled(
    /* [out] */ Boolean* enabled)
{
    return PopupWindow::IsClippingEnabled(enabled);
}

ECode CPopupWindow::SetClippingEnabled(
    /* [in] */ Boolean enabled)
{
    return PopupWindow::SetClippingEnabled(enabled);
}

ECode CPopupWindow::IsSplitTouchEnabled(
    /* [out] */ Boolean* enabled)
{
    return PopupWindow::IsSplitTouchEnabled(enabled);
}

ECode CPopupWindow::SetSplitTouchEnabled(
    /* [in] */ Boolean enabled)
{
    return PopupWindow::SetSplitTouchEnabled(enabled);
}

ECode CPopupWindow::IsLayoutInScreenEnabled(
    /* [out] */ Boolean* enabled)
{
    return PopupWindow::IsLayoutInScreenEnabled(enabled);
}

ECode CPopupWindow::SetLayoutInScreenEnabled(
    /* [in] */ Boolean enabled)
{
    return PopupWindow::SetLayoutInScreenEnabled(enabled);
}

ECode CPopupWindow::SetWindowLayoutType(
    /* [in] */ Int32 layoutType)
{
    return PopupWindow::SetWindowLayoutType(layoutType);
}

ECode CPopupWindow::GetWindowLayoutType(
    /* [out] */ Int32* layoutType)
{
    return PopupWindow::GetWindowLayoutType(layoutType);
}

ECode CPopupWindow::SetWindowLayoutMode(
    /* [in] */ Int32 widthSpec,
    /* [in] */ Int32 heightSpec)
{
    return PopupWindow::SetWindowLayoutMode(widthSpec, heightSpec);
}

ECode CPopupWindow::GetHeight(
    /* [out] */ Int32* height)
{
    return PopupWindow::GetHeight(height);
}

ECode CPopupWindow::SetHeight(
    /* [in] */ Int32 height)
{
    return PopupWindow::SetHeight(height);
}

ECode CPopupWindow::GetWidth(
    /* [out] */ Int32* width)
{
    return PopupWindow::GetWidth(width);
}

ECode CPopupWindow::SetWidth(
    /* [in] */ Int32 width)
{
    return PopupWindow::SetWidth(width);
}

ECode CPopupWindow::IsShowing(
    /* [out] */ Boolean* isShowing)
{
    return PopupWindow::IsShowing(isShowing);
}

ECode CPopupWindow::ShowAtLocation(
    /* [in] */ IView* parent,
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return PopupWindow::ShowAtLocation(parent, gravity, x, y);
}

ECode CPopupWindow::ShowAsDropDown(
    /* [in] */ IView* anchor)
{
    return PopupWindow::ShowAsDropDown(anchor);
}

ECode CPopupWindow::ShowAsDropDownEx(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 xoff,
    /* [in] */ Int32 yoff)
{
    return PopupWindow::ShowAsDropDownEx(anchor, xoff, yoff);
}

ECode CPopupWindow::IsAboveAnchor(
    /* [out] */ Boolean* isAboveAnchor)
{
    return PopupWindow::IsAboveAnchor(isAboveAnchor);
}

ECode CPopupWindow::GetMaxAvailableHeight(
    /* [in] */ IView* anchor,
    /* [out] */ Int32* maxAvailableHeight)
{
    return PopupWindow::GetMaxAvailableHeight(anchor, maxAvailableHeight);
}

ECode CPopupWindow::GetMaxAvailableHeightEx(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 yOffset,
    /* [out] */ Int32* maxAvailableHeight)
{
    return PopupWindow::GetMaxAvailableHeightEx(anchor, yOffset, maxAvailableHeight);
}

ECode CPopupWindow::GetMaxAvailableHeightEx2(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 yOffset,
    /* [in] */ Boolean ignoreBottomDecorations,
    /* [out] */ Int32* maxAvailableHeight)
{
    return PopupWindow::GetMaxAvailableHeightEx2(anchor, yOffset,
            ignoreBottomDecorations, maxAvailableHeight);
}

ECode CPopupWindow::Dismiss()
{
    return PopupWindow::Dismiss();
}

ECode CPopupWindow::SetOnDismissListener(
    /* [in] */ IPopupWindowOnDismissListener* l)
{
    return PopupWindow::SetOnDismissListener(l);
}

ECode CPopupWindow::Update()
{
    return PopupWindow::Update();
}

ECode CPopupWindow::UpdateEx(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return PopupWindow::UpdateEx(width, height);
}

ECode CPopupWindow::UpdateEx2(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return PopupWindow::UpdateEx2(x, y, width, height);
}

ECode CPopupWindow::UpdateEx3(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean force)
{
    return PopupWindow::UpdateEx3(x, y, width, height, force);
}

ECode CPopupWindow::UpdateEx4(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return PopupWindow::UpdateEx4(anchor, width, height);
}

ECode CPopupWindow::UpdateEx5(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 width,
    /* [in] */ Int32 xoff,
    /* [in] */ Int32 yoff,
    /* [in] */ Int32 height)
{
    return PopupWindow::UpdateEx5(anchor, width, xoff, yoff, height);
}

ECode CPopupWindow::OnScrollChanged()
{
    return PopupWindow::OnScrollChanged();
}

ECode CPopupWindow::constructor(
    /* [in] */ IContext* ctx)
{
    return PopupWindow::Init(ctx);
}

ECode CPopupWindow::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return PopupWindow::Init(ctx, attrs);
}

ECode CPopupWindow::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return PopupWindow::Init(ctx, attrs, defStyle);
}

ECode CPopupWindow::constructor()
{
    return PopupWindow::Init();
}

ECode CPopupWindow::constructor(
    /* [in] */ IView* contentView)
{
    return PopupWindow::Init(contentView);
}

ECode CPopupWindow::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return PopupWindow::Init(width, height);
}

ECode CPopupWindow::constructor(
    /* [in] */ IView* contentView,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return PopupWindow::Init(contentView, width, height);
}

ECode CPopupWindow::constructor(
    /* [in] */ IView* contentView,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean focusable)
{
    return PopupWindow::Init(contentView, width, height, focusable);
}
