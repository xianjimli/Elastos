
#ifndef __POPUPWINDOW_H__
#define __POPUPWINDOW_H__

#include "ext/frameworkext.h"
#include "view/ViewMacro.h"
#include "widget/FrameLayout.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos.h>

using namespace Elastos;

class PopupWindow
{
private:
    class PopupViewContainer
        : public FrameLayout
        , public ElRefBase
        , public IFrameLayout
    {
    public:
        PopupViewContainer(
            /* [in] */ PopupWindow* popupWindow,
            /* [in] */ IContext* ctx);

        IVIEW_METHODS_DECL();

        IVIEWGROUP_METHODS_DECL();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI SetForegroundGravity(
            /* [in] */ Int32 foregroundGravity);

        CARAPI SetForeground(
            /* [in] */ IDrawable* drawable);

        CARAPI GetForeground(
            /* [out] */ IDrawable** foreground);

        CARAPI SetMeasureAllChildren(
            /* [in] */ Boolean measureAll);

        CARAPI GetConsiderGoneChildrenWhenMeasuring(
            /* [out] */ Boolean* measureAll);

        CARAPI_(Boolean) DispatchKeyEvent(
            /* [in] */ IKeyEvent* event);

        CARAPI_(Boolean) DispatchTouchEvent(
            /* [in] */ IMotionEvent* event);

        CARAPI_(Boolean) OnTouchEvent(
            /* [in] */ IMotionEvent* event);

//        CARAPI SendAccessibilityEvent(
//            /* [in] */ Int32 eventType);

    protected:
        CARAPI OnCreateDrawableState(
            /* [in] */ Int32 extraSpace,
            /* [out] */ ArrayOf<Int32>** drawableState);

    private:
        PopupWindow* mPopupWindow;
    };

public:
    PopupWindow();

    virtual CARAPI GetBackground(
        /* [out] */ IDrawable** background);

    virtual CARAPI SetBackgroundDrawable(
        /* [in] */ IDrawable* background);

    virtual CARAPI GetAnimationStyle(
        /* [out] */ Int32* style);

    virtual CARAPI SetIgnoreCheekPress();

    virtual CARAPI SetAnimationStyle(
        /* [in] */ Int32 animationStyle);

    virtual CARAPI GetContentView(
        /* [out] */ IView** contentView);

    virtual CARAPI SetContentView(
        /* [in] */ IView* contentView);

    virtual CARAPI SetTouchInterceptor(
        /* [in] */ IViewOnTouchListener* l);

    virtual CARAPI IsFocusable(
        /* [out] */ Boolean* isFocusable);

    virtual CARAPI SetFocusable(
         /* [in] */ Boolean focusable);

    virtual CARAPI GetInputMethodMode(
        /* [out] */ Int32* inputMethodMode);

    virtual CARAPI SetInputMethodMode(
        /* [in] */ Int32 mode);

    virtual CARAPI SetSoftInputMode(
        /* [in] */ Int32 mode);

    virtual CARAPI GetSoftInputMode(
        /* [out] */ Int32* softInputMode);

    virtual CARAPI IsTouchable(
        /* [out] */ Boolean* touchable);

    virtual CARAPI SetTouchable(
        /* [in] */ Boolean touchable);

    virtual CARAPI IsOutsideTouchable(
        /* [out] */ Boolean* touchable);

    virtual CARAPI SetOutsideTouchable(
        /* [in] */ Boolean touchable);

    virtual CARAPI IsClippingEnabled(
        /* [out] */ Boolean* enabled);

    virtual CARAPI SetClippingEnabled(
        /* [in] */ Boolean enabled);

    virtual CARAPI IsSplitTouchEnabled(
        /* [out] */ Boolean* enabled);

    virtual CARAPI SetSplitTouchEnabled(
        /* [in] */ Boolean enabled);

    virtual CARAPI IsLayoutInScreenEnabled(
        /* [out] */ Boolean* enabled);

    virtual CARAPI SetLayoutInScreenEnabled(
        /* [in] */ Boolean enabled);

    virtual CARAPI SetWindowLayoutType(
        /* [in] */ Int32 layoutType);

    virtual CARAPI GetWindowLayoutType(
        /* [out] */ Int32* layoutType);

    virtual CARAPI SetWindowLayoutMode(
        /* [in] */ Int32 widthSpec,
        /* [in] */ Int32 heightSpec);

    virtual CARAPI GetHeight(
        /* [out] */ Int32* height);

    virtual CARAPI SetHeight(
        /* [in] */ Int32 height);

    virtual CARAPI GetWidth(
        /* [out] */ Int32* width);

    virtual CARAPI SetWidth(
        /* [in] */ Int32 width);

    virtual CARAPI IsShowing(
        /* [out] */ Boolean* isShowing);

    virtual CARAPI ShowAtLocation(
        /* [in] */ IView* parent,
        /* [in] */ Int32 gravity,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    virtual CARAPI ShowAsDropDown(
        /* [in] */ IView* anchor);

    virtual CARAPI ShowAsDropDownEx(
        /* [in] */ IView* anchor,
        /* [in] */ Int32 xoff,
        /* [in] */ Int32 yoff);

    virtual CARAPI IsAboveAnchor(
        /* [out] */ Boolean* isAboveAnchor);

    virtual CARAPI GetMaxAvailableHeight(
        /* [in] */ IView* anchor,
        /* [out] */ Int32* maxAvailableHeight);

    virtual CARAPI GetMaxAvailableHeightEx(
        /* [in] */ IView* anchor,
        /* [in] */ Int32 yOffset,
        /* [out] */ Int32* maxAvailableHeight);

    virtual CARAPI GetMaxAvailableHeightEx2(
        /* [in] */ IView* anchor,
        /* [in] */ Int32 yOffset,
        /* [in] */ Boolean ignoreBottomDecorations,
        /* [out] */ Int32* maxAvailableHeight);

    virtual CARAPI Dismiss();

    virtual CARAPI SetOnDismissListener(
        /* [in] */ IPopupWindowOnDismissListener* onDismissListener);

    virtual CARAPI Update();

    virtual CARAPI UpdateEx(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    virtual CARAPI UpdateEx2(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    virtual CARAPI UpdateEx3(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Boolean force);

    virtual CARAPI UpdateEx4(
        /* [in] */ IView* anchor,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    virtual CARAPI UpdateEx5(
        /* [in] */ IView* anchor,
        /* [in] */ Int32 width,
        /* [in] */ Int32 xoff,
        /* [in] */ Int32 yoff,
        /* [in] */ Int32 height);

    virtual CARAPI OnScrollChanged();

    CARAPI Init(
        /* [in] */ IContext* ctx);

    CARAPI Init(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI Init();

    CARAPI Init(
        /* [in] */ IView* contentView);

    CARAPI Init(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI Init(
        /* [in] */ IView* contentView,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI Init(
        /* [in] */ IView* contentView,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Boolean focusable);

private:
    CARAPI_(void) UpdateAboveAnchor(
        /* [in] */ Boolean aboveAnchor);

    /**
     * <p>Prepare the popup by embedding in into a new ViewGroup if the
     * background drawable is not null. If embedding is required, the layout
     * parameters' height is mnodified to take into account the background's
     * padding.</p>
     *
     * @param p the layout parameters of the popup's content view
     */
    CARAPI PreparePopup(
        /* [in] */ IWindowManagerLayoutParams* p);

    /**
     * <p>Invoke the popup window by adding the content view to the window
     * manager.</p>
     *
     * <p>The content view must be non-null when this method is invoked.</p>
     *
     * @param p the layout parameters of the popup's content view
     */
    CARAPI_(void) InvokePopup(
        /* [in] */ IWindowManagerLayoutParams* p);

    /**
     * <p>Generate the layout parameters for the popup window.</p>
     *
     * @param token the window token used to bind the popup's window
     *
     * @return the layout parameters to pass to the window manager
     */
   CARAPI_(AutoPtr<IWindowManagerLayoutParams>) CreatePopupLayout(
       /* [in] */ IBinder* token);

   CARAPI_(Int32) ComputeFlags(
       /* [in] */ Int32 curFlags);

   CARAPI_(Int32) ComputeAnimationResource();

   /**
     * <p>Positions the popup window on screen. When the popup window is too
     * tall to fit under the anchor, a parent scroll view is seeked and scrolled
     * up to reclaim space. If scrolling is not possible or not enough, the
     * popup window gets moved on top of the anchor.</p>
     *
     * <p>The height must have been set on the layout parameters prior to
     * calling this method.</p>
     *
     * @param anchor the view on which the popup window must be anchored
     * @param p the layout parameters used to display the drop down
     *
     * @return true if the popup is translated upwards to fit on screen
     */
    CARAPI_(Boolean) FindDropDownPosition(
        /* [in] */ IView* anchor,
        /* [in] */ IWindowManagerLayoutParams* p,
        /* [in] */ Int32 xoff,
        /* [in] */ Int32 yoff);

   CARAPI_(void) UnregisterForScrollChanged();

   CARAPI_(void) RegisterForScrollChanged(
       /* [in] */ IView* anchor,
       /* [in] */ Int32 xoff,
       /* [in] */ Int32 yoff);

   CARAPI UpdateEx6(
        /* [in] */ IView* anchor,
        /* [in] */ Boolean updateLocation,
        /* [in] */ Int32 xoff,
        /* [in] */ Int32 yoff,
        /* [in] */ Boolean updateDimension,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

private:
    AutoPtr<IContext> mContext;
    AutoPtr<IWindowManager> mWindowManager;

    Boolean mIsShowing;
    Boolean mIsDropdown;

    AutoPtr<IView> mContentView;
    AutoPtr<IView> mPopupView;
    Boolean mFocusable;
    Int32 mInputMethodMode;
    Int32 mSoftInputMode;
    Boolean mTouchable;
    Boolean mOutsideTouchable;
    Boolean mClippingEnabled;
    Boolean mSplitTouchEnabled;
    Boolean mLayoutInScreen;

    AutoPtr<IViewOnTouchListener> mTouchInterceptor;

    Int32 mWidthMode;
    Int32 mWidth;
    Int32 mLastWidth;
    Int32 mHeightMode;
    Int32 mHeight;
    Int32 mLastHeight;

    Int32 mPopupWidth;
    Int32 mPopupHeight;

    Int32 mDrawingLocation[2];
    Int32 mScreenLocation[2];
    AutoPtr<IRect> mTempRect;

    AutoPtr<IDrawable> mBackground;
    AutoPtr<IDrawable> mAboveAnchorBackgroundDrawable;
    AutoPtr<IDrawable> mBelowAnchorBackgroundDrawable;

    Boolean mAboveAnchor;
    Int32 mWindowLayoutType;

    AutoPtr<IPopupWindowOnDismissListener> mOnDismissListener;
    Boolean mIgnoreCheekPress;

    Int32 mAnimationStyle;

    static Int32 ABOVE_ANCHOR_STATE_SET[];

    AutoPtr<IView> mAnchor;
    Int32 mAnchorXoff;
    Int32 mAnchorYoff;


};

#endif // __POPUPWINDOW_H__
