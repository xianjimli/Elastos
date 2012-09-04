
#include "widget/CPopupWindow.h"


PInterface CPopupWindow::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CPopupWindow::Probe(riid);
}

ECode CPopupWindow::GetBackground(
    /* [out] */ IDrawable** background)
{
    VALIDATE_NOT_NULL(background);

    *background = PopupWindow::GetBackground();

    return NOERROR;
}

ECode CPopupWindow::SetBackgroundDrawable(
    /* [in] */ IDrawable* background)
{
    return PopupWindow::SetBackgroundDrawable(background);
}

ECode CPopupWindow::GetAnimationStyle(
    /* [out] */ Int32* style)
{
    VALIDATE_NOT_NULL(style);

    *style = PopupWindow::GetAnimationStyle();

    return NOERROR;
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
    VALIDATE_NOT_NULL(contentView);

    *contentView = PopupWindow::GetContentView();
    if (*contentView != NULL) {
        (*contentView)->AddRef();
    }

    return NOERROR;
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
    /* [out] */ Boolean* focusable)
{
    VALIDATE_NOT_NULL(focusable);

    *focusable = PopupWindow::IsFocusable();

    return NOERROR;
}

ECode CPopupWindow::SetFocusable(
     /* [in] */ Boolean focusable)
{
    return PopupWindow::SetFocusable(focusable);
}

ECode CPopupWindow::GetInputMethodMode(
    /* [out] */ Int32* inputMethodMode)
{
    VALIDATE_NOT_NULL(inputMethodMode);

    *inputMethodMode = PopupWindow::GetInputMethodMode();

    return NOERROR;
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
    VALIDATE_NOT_NULL(softInputMode);

    *softInputMode = PopupWindow::GetSoftInputMode();

    return NOERROR;
}

ECode CPopupWindow::IsTouchable(
    /* [out] */ Boolean* touchable)
{
    VALIDATE_NOT_NULL(touchable);

    *touchable = PopupWindow::IsTouchable();

    return NOERROR;
}

ECode CPopupWindow::SetTouchable(
    /* [in] */ Boolean touchable)
{
    return PopupWindow::SetTouchable(touchable);
}

ECode CPopupWindow::IsOutsideTouchable(
    /* [out] */ Boolean* touchable)
{
    VALIDATE_NOT_NULL(touchable);

    *touchable = PopupWindow::IsOutsideTouchable();

    return NOERROR;
}

ECode CPopupWindow::SetOutsideTouchable(
    /* [in] */ Boolean touchable)
{
    return PopupWindow::SetOutsideTouchable(touchable);
}

ECode CPopupWindow::IsClippingEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);

    *enabled = PopupWindow::IsClippingEnabled();

    return NOERROR;
}

ECode CPopupWindow::SetClippingEnabled(
    /* [in] */ Boolean enabled)
{
    return PopupWindow::SetClippingEnabled(enabled);
}

ECode CPopupWindow::IsSplitTouchEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);

    *enabled = PopupWindow::IsSplitTouchEnabled();

    return NOERROR;
}

ECode CPopupWindow::SetSplitTouchEnabled(
    /* [in] */ Boolean enabled)
{
    return PopupWindow::SetSplitTouchEnabled(enabled);
}

ECode CPopupWindow::IsLayoutInScreenEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);

    *enabled = PopupWindow::IsLayoutInScreenEnabled();

    return NOERROR;
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
    VALIDATE_NOT_NULL(layoutType);

    *layoutType = PopupWindow::GetWindowLayoutType();

    return NOERROR;
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
    VALIDATE_NOT_NULL(height);

    *height = PopupWindow::GetHeight();

    return NOERROR;
}

ECode CPopupWindow::SetHeight(
    /* [in] */ Int32 height)
{
    return PopupWindow::SetHeight(height);
}

ECode CPopupWindow::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    *width = PopupWindow::GetWidth();

    return NOERROR;
}

ECode CPopupWindow::SetWidth(
    /* [in] */ Int32 width)
{
    return PopupWindow::SetWidth(width);
}

ECode CPopupWindow::IsShowing(
    /* [out] */ Boolean* showing)
{
    VALIDATE_NOT_NULL(showing);

    *showing = PopupWindow::IsShowing();

    return NOERROR;
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
    return PopupWindow::ShowAsDropDown(anchor, xoff, yoff);
}

ECode CPopupWindow::IsAboveAnchor(
    /* [out] */ Boolean* isAboveAnchor)
{
    VALIDATE_NOT_NULL(isAboveAnchor);

    *isAboveAnchor = PopupWindow::IsAboveAnchor();

    return NOERROR;
}

ECode CPopupWindow::GetMaxAvailableHeight(
    /* [in] */ IView* anchor,
    /* [out] */ Int32* maxAvailableHeight)
{
    VALIDATE_NOT_NULL(maxAvailableHeight);

    *maxAvailableHeight = PopupWindow::GetMaxAvailableHeight(anchor);

    return NOERROR;
}

ECode CPopupWindow::GetMaxAvailableHeightEx(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 yOffset,
    /* [out] */ Int32* maxAvailableHeight)
{
    VALIDATE_NOT_NULL(maxAvailableHeight);

    *maxAvailableHeight = PopupWindow::GetMaxAvailableHeight(anchor, yOffset);

    return NOERROR;
}

ECode CPopupWindow::GetMaxAvailableHeightEx2(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 yOffset,
    /* [in] */ Boolean ignoreBottomDecorations,
    /* [out] */ Int32* maxAvailableHeight)
{
    VALIDATE_NOT_NULL(maxAvailableHeight);

    *maxAvailableHeight = PopupWindow::GetMaxAvailableHeight(anchor, yOffset,
            ignoreBottomDecorations);

    return NOERROR;
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
    return PopupWindow::Update(width, height);
}

ECode CPopupWindow::UpdateEx2(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return PopupWindow::Update(x, y, width, height);
}

ECode CPopupWindow::UpdateEx3(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean force)
{
    return PopupWindow::Update(x, y, width, height, force);
}

ECode CPopupWindow::UpdateEx4(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return PopupWindow::Update(anchor, width, height);
}

ECode CPopupWindow::UpdateEx5(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 width,
    /* [in] */ Int32 xoff,
    /* [in] */ Int32 yoff,
    /* [in] */ Int32 height)
{
    return PopupWindow::Update(anchor, width, xoff, yoff, height);
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
    return PopupWindow::Init(NULL, width, height);
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
