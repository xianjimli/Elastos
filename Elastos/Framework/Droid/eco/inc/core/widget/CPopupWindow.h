
#ifndef __CPOPUPWINDOW_H__
#define __CPOPUPWINDOW_H__

#include "_CPopupWindow.h"
#include "widget/PopupWindow.h"

CarClass(CPopupWindow), public PopupWindow
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetBackground(
        /* [out] */ IDrawable** background);

    CARAPI SetBackgroundDrawable(
        /* [in] */ IDrawable* background);

    CARAPI GetAnimationStyle(
        /* [out] */ Int32* style);

    CARAPI SetIgnoreCheekPress();

    CARAPI SetAnimationStyle(
        /* [in] */ Int32 animationStyle);

    CARAPI GetContentView(
        /* [out] */ IView** contentView);

    CARAPI SetContentView(
        /* [in] */ IView* contentView);

    CARAPI SetTouchInterceptor(
        /* [in] */ IViewOnTouchListener* l);

    CARAPI IsFocusable(
        /* [out] */ Boolean* focusable);

    CARAPI SetFocusable(
         /* [in] */ Boolean focusable);

    CARAPI GetInputMethodMode(
        /* [out] */ Int32* inputMethodMode);

    CARAPI SetInputMethodMode(
        /* [in] */ Int32 mode);

    CARAPI SetSoftInputMode(
        /* [in] */ Int32 mode);

    CARAPI GetSoftInputMode(
        /* [out] */ Int32* softInputMode);

    CARAPI IsTouchable(
        /* [out] */ Boolean* touchable);

    CARAPI SetTouchable(
        /* [in] */ Boolean touchable);

    CARAPI IsOutsideTouchable(
        /* [out] */ Boolean* touchable);

    CARAPI SetOutsideTouchable(
        /* [in] */ Boolean touchable);

    CARAPI IsClippingEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetClippingEnabled(
        /* [in] */ Boolean enabled);

    CARAPI IsSplitTouchEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetSplitTouchEnabled(
        /* [in] */ Boolean enabled);

    CARAPI IsLayoutInScreenEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetLayoutInScreenEnabled(
        /* [in] */ Boolean enabled);

    CARAPI SetWindowLayoutType(
        /* [in] */ Int32 layoutType);

    CARAPI GetWindowLayoutType(
        /* [out] */ Int32* layoutType);

    CARAPI SetWindowLayoutMode(
        /* [in] */ Int32 widthSpec,
        /* [in] */ Int32 heightSpec);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI SetHeight(
        /* [in] */ Int32 height);

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI SetWidth(
        /* [in] */ Int32 width);

    CARAPI IsShowing(
        /* [out] */ Boolean* showing);

    CARAPI ShowAtLocation(
        /* [in] */ IView* parent,
        /* [in] */ Int32 gravity,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI ShowAsDropDown(
        /* [in] */ IView* anchor);

    CARAPI ShowAsDropDownEx(
        /* [in] */ IView* anchor,
        /* [in] */ Int32 xoff,
        /* [in] */ Int32 yoff);

    CARAPI IsAboveAnchor(
        /* [out] */ Boolean* isAboveAnchor);

    CARAPI GetMaxAvailableHeight(
        /* [in] */ IView* anchor,
        /* [out] */ Int32* maxAvailableHeight);

    CARAPI GetMaxAvailableHeightEx(
        /* [in] */ IView* anchor,
        /* [in] */ Int32 yOffset,
        /* [out] */ Int32* maxAvailableHeight);

    CARAPI GetMaxAvailableHeightEx2(
        /* [in] */ IView* anchor,
        /* [in] */ Int32 yOffset,
        /* [in] */ Boolean ignoreBottomDecorations,
        /* [out] */ Int32* maxAvailableHeight);

    CARAPI Dismiss();

    CARAPI SetOnDismissListener(
        /* [in] */ IPopupWindowOnDismissListener* l);

    CARAPI Update();

    CARAPI UpdateEx(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI UpdateEx2(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI UpdateEx3(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Boolean force);

    CARAPI UpdateEx4(
        /* [in] */ IView* anchor,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI UpdateEx5(
        /* [in] */ IView* anchor,
        /* [in] */ Int32 xoff,
        /* [in] */ Int32 yoff,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IView* contentView);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ IView* contentView,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ IView* contentView,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Boolean focusable);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPOPUPWINDOW_H__
