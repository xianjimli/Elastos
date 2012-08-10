
#include "widget/PopupWindow.h"
#include "widget/CFrameLayoutLayoutParams.h"
#include "view/CViewGroupLayoutParams.h"
#include "graphics/CRect.h"
#include "graphics/ElPixelFormat.h"
#include "view/CWindowManagerLayoutParams.h"
#include "utils/CDisplayMetrics.h"
#include <elastos/Math.h>
#include <Logger.h>

//using namespace Elastos::Core;
//using namespace Elastos::Utility;
using namespace Elastos::Utility::Logging;

PopupWindow::PopupWindow() :
    mIsShowing(FALSE),
    mIsDropdown(FALSE),
    mFocusable(FALSE),
    mInputMethodMode(INPUT_METHOD_FROM_FOCUSABLE),
    mSoftInputMode(0),
    mTouchable(TRUE),
    mOutsideTouchable(FALSE),
    mClippingEnabled(TRUE),
    mSplitTouchEnabled(FALSE),
    mLayoutInScreen(FALSE),
    mWidthMode(0),
    mWidth(0),
    mLastWidth(0),
    mHeightMode(0),
    mHeight(0),
    mLastHeight(0),
    mPopupWidth(0),
    mPopupHeight(0),
    mAboveAnchor(FALSE),
    mWindowLayoutType(WindowManagerLayoutParams_TYPE_APPLICATION_PANEL),
    mIgnoreCheekPress(FALSE),
    mAnimationStyle(-1),
    mAnchorXoff(0),
    mAnchorYoff(0)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mTempRect));
}

Int32 PopupWindow::ABOVE_ANCHOR_STATE_SET[] = {
        0x010100aa/*com.android.internal.R.attr.state_above_anchor*/};

ECode PopupWindow::GetBackground(
    /* [out] */ IDrawable** background)
{
    *background = mBackground;

    return NOERROR;
}

ECode PopupWindow::SetBackgroundDrawable(
    /* [in] */ IDrawable* background)
{
    mBackground = background;

    return NOERROR;
}

ECode PopupWindow::GetAnimationStyle(
    /* [out] */ Int32* style)
{
    *style = mAnimationStyle;

    return NOERROR;
}

ECode PopupWindow::SetIgnoreCheekPress()
{
    mIgnoreCheekPress = TRUE;

    return NOERROR;
}

ECode PopupWindow::SetAnimationStyle(
    /* [in] */ Int32 animationStyle)
{
    mAnimationStyle = animationStyle;

    return NOERROR;
}

ECode PopupWindow::GetContentView(
    /* [out] */ IView** contentView)
{
    *contentView = mContentView;

    return NOERROR;
}

ECode PopupWindow::SetContentView(
    /* [in] */ IView* contentView)
{
    Boolean isShowing;
    IsShowing(&isShowing);
    if (isShowing) {
        return NOERROR;
    }

    mContentView = contentView;

    if (mContext == NULL) {
        mContentView->GetContext((IContext**)&mContext);
    }

    if (mWindowManager == NULL) {
        mContext->GetSystemService(Context_WINDOW_SERVICE, (IInterface**)&mWindowManager);
    }

    return NOERROR;
}

ECode PopupWindow::SetTouchInterceptor(
    /* [in] */ IViewOnTouchListener* l)
{
    mTouchInterceptor = l;

    return NOERROR;
}

ECode PopupWindow::IsFocusable(
    /* [out] */ Boolean* isFocusable)
{
    *isFocusable = mFocusable;

    return NOERROR;
}

ECode PopupWindow::SetFocusable(
     /* [in] */ Boolean focusable)
{
    mFocusable = focusable;

    return NOERROR;
}

ECode PopupWindow::GetInputMethodMode(
    /* [out] */ Int32* inputMethodMode)
{
    *inputMethodMode = mInputMethodMode;

    return NOERROR;
}

ECode PopupWindow::SetInputMethodMode(
    /* [in] */ Int32 mode)
{
    mInputMethodMode = mode;

    return NOERROR;
}

ECode PopupWindow::SetSoftInputMode(
    /* [in] */ Int32 mode)
{
    mSoftInputMode = mode;

    return NOERROR;
}

ECode PopupWindow::GetSoftInputMode(
    /* [out] */ Int32* softInputMode)
{
    *softInputMode = mSoftInputMode;

    return NOERROR;
}

ECode PopupWindow::IsTouchable(
    /* [out] */ Boolean* touchable)
{
    *touchable = mTouchable;

    return NOERROR;
}

ECode PopupWindow::SetTouchable(
    /* [in] */ Boolean touchable)
{
    mTouchable = touchable;

    return NOERROR;
}

ECode PopupWindow::IsOutsideTouchable(
    /* [out] */ Boolean* touchable)
{
    *touchable = mOutsideTouchable;

    return NOERROR;
}

ECode PopupWindow::SetOutsideTouchable(
    /* [in] */ Boolean touchable)
{
    mOutsideTouchable = touchable;

    return NOERROR;
}

ECode PopupWindow::IsClippingEnabled(
    /* [out] */ Boolean* enabled)
{
    *enabled = mClippingEnabled;

    return NOERROR;
}

ECode PopupWindow::SetClippingEnabled(
    /* [in] */ Boolean enabled)
{
    mClippingEnabled = enabled;

    return NOERROR;
}

ECode PopupWindow::IsSplitTouchEnabled(
    /* [out] */ Boolean* enabled)
{
    *enabled = mSplitTouchEnabled;

    return NOERROR;
}

ECode PopupWindow::SetSplitTouchEnabled(
    /* [in] */ Boolean enabled)
{
    mSplitTouchEnabled = enabled;

    return NOERROR;
}

ECode PopupWindow::IsLayoutInScreenEnabled(
    /* [out] */ Boolean* enabled)
{
    *enabled = mLayoutInScreen;

    return NOERROR;
}

ECode PopupWindow::SetLayoutInScreenEnabled(
    /* [in] */ Boolean enabled)
{
    mLayoutInScreen = enabled;

    return NOERROR;
}

ECode PopupWindow::SetWindowLayoutType(
    /* [in] */ Int32 layoutType)
{
    mWindowLayoutType = layoutType;

    return NOERROR;
}

ECode PopupWindow::GetWindowLayoutType(
    /* [out] */ Int32* layoutType)
{
    *layoutType = mWindowLayoutType;

    return NOERROR;
}

ECode PopupWindow::SetWindowLayoutMode(
    /* [in] */ Int32 widthSpec,
    /* [in] */ Int32 heightSpec)
{
    mWidthMode = widthSpec;
    mHeightMode = heightSpec;

    return NOERROR;
}

ECode PopupWindow::GetHeight(
    /* [out] */ Int32* height)
{
    *height = mHeight;

    return NOERROR;
}

ECode PopupWindow::SetHeight(
    /* [in] */ Int32 height)
{
    mHeight = height;

    return NOERROR;
}

ECode PopupWindow::GetWidth(
    /* [out] */ Int32* width)
{
    *width = mWidth;

    return NOERROR;
}

ECode PopupWindow::SetWidth(
    /* [in] */ Int32 width)
{
    mWidth = width;

    return NOERROR;
}

ECode PopupWindow::IsShowing(
    /* [out] */ Boolean* isShowing)
{
    *isShowing = mIsShowing;

    return NOERROR;
}

ECode PopupWindow::ShowAtLocation(
    /* [in] */ IView* parent,
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    Boolean isShowing;
    IsShowing(&isShowing);
    if (isShowing || mContentView == NULL) {
        return NOERROR;
    }

    UnregisterForScrollChanged();

    mIsShowing = TRUE;
    mIsDropdown = FALSE;

    AutoPtr<IBinder> token;
    parent->GetWindowToken((IBinder**)&token);
    AutoPtr<IWindowManagerLayoutParams> p = CreatePopupLayout(token);
    ((CWindowManagerLayoutParams*)p.Get())->mWindowAnimations = ComputeAnimationResource();

    FAIL_RETURN(PreparePopup(p));
    if (gravity == Gravity_NO_GRAVITY) {
        gravity = Gravity_TOP | Gravity_LEFT;
    }
    ((CWindowManagerLayoutParams*)p.Get())->mGravity = gravity;
    ((CWindowManagerLayoutParams*)p.Get())->mX = x;
    ((CWindowManagerLayoutParams*)p.Get())->mY = y;
    InvokePopup(p);

    return NOERROR;
}

ECode PopupWindow::ShowAsDropDown(
    /* [in] */ IView* anchor)
{
    return ShowAsDropDownEx(anchor, 0, 0);
}

ECode PopupWindow::ShowAsDropDownEx(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 xoff,
    /* [in] */ Int32 yoff)
{
    Boolean isShowing;
    IsShowing(&isShowing);
    if (isShowing || mContentView == NULL) {
        return NOERROR;
    }

    RegisterForScrollChanged(anchor, xoff, yoff);

    mIsShowing = TRUE;
    mIsDropdown = TRUE;

    AutoPtr<IBinder> token;
    anchor->GetWindowToken((IBinder**)&token);

    AutoPtr<IWindowManagerLayoutParams> p = CreatePopupLayout(token);
    FAIL_RETURN(PreparePopup(p));

    UpdateAboveAnchor(FindDropDownPosition(anchor, p, xoff, yoff));

    if (mHeightMode < 0)
        ((CWindowManagerLayoutParams*)p.Get())->mHeight = mLastHeight = mHeightMode;
    if (mWidthMode < 0)
        ((CWindowManagerLayoutParams*)p.Get())->mWidth = mLastWidth = mWidthMode;

    ((CWindowManagerLayoutParams*)p.Get())->mWindowAnimations = ComputeAnimationResource();

    InvokePopup(p);

    return NOERROR;
}

void PopupWindow::UpdateAboveAnchor(
    /* [in] */ Boolean aboveAnchor)
{
    if (aboveAnchor != mAboveAnchor) {
        mAboveAnchor = aboveAnchor;
        if (mBackground != NULL) {
            // If the background drawable provided was a StateListDrawable with above-anchor
            // and below-anchor states, use those. Otherwise rely on refreshDrawableState to
            // do the job.
            if (mAboveAnchorBackgroundDrawable != NULL) {
                if (mAboveAnchor) {
                    mPopupView->SetBackgroundDrawable(mAboveAnchorBackgroundDrawable);
                }
                else {
                    mPopupView->SetBackgroundDrawable(mBelowAnchorBackgroundDrawable);
                }
            }
            else {
                mPopupView->RefreshDrawableState();
            }
        }
    }
}

ECode PopupWindow::IsAboveAnchor(
    /* [out] */ Boolean* isAboveAnchor)
{
    *isAboveAnchor = mAboveAnchor;

    return NOERROR;
}

/**
 * <p>Prepare the popup by embedding in into a new ViewGroup if the
 * background drawable is not null. If embedding is required, the layout
 * parameters' height is mnodified to take into account the background's
 * padding.</p>
 *
 * @param p the layout parameters of the popup's content view
 */
ECode PopupWindow::PreparePopup(
    /* [in] */ IWindowManagerLayoutParams* p)
{
    if (mContentView == NULL || mContext == NULL || mWindowManager == NULL) {
//        throw new IllegalStateException("You must specify a valid content view by "
//                + "calling setContentView() before attempting to show the popup.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (mBackground != NULL) {
        AutoPtr<IViewGroupLayoutParams> layoutParams;
        mContentView->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
        Int32 height = ViewGroupLayoutParams_MATCH_PARENT;
        if (layoutParams != NULL &&
                ((CViewGroupLayoutParams*)layoutParams.Get())->mHeight == ViewGroupLayoutParams_WRAP_CONTENT) {
            height = ViewGroupLayoutParams_WRAP_CONTENT;
        }

        // when a background is available, we embed the content view
        // within another view that owns the background drawable
        PopupViewContainer* popupViewContainer = new PopupViewContainer(this, mContext);
        AutoPtr<IFrameLayoutLayoutParams> listParams;
        CFrameLayoutLayoutParams::New(ViewGroupLayoutParams_MATCH_PARENT, height,
                (IFrameLayoutLayoutParams**)&listParams);
        popupViewContainer->SetBackgroundDrawable(mBackground);
        popupViewContainer->AddViewEx3(mContentView, (IViewGroupLayoutParams*)listParams);

        if (popupViewContainer->Probe(EIID_IView) != NULL) {
            mPopupView = (IView*)mBackground->Probe(EIID_IView);
        }
    }
    else {
        mPopupView = mContentView;
    }
    mPopupWidth = ((CWindowManagerLayoutParams*)p)->mWidth;
    mPopupHeight = ((CWindowManagerLayoutParams*)p)->mHeight;

    return NOERROR;
}


/**
 * <p>Invoke the popup window by adding the content view to the window
 * manager.</p>
 *
 * <p>The content view must be non-null when this method is invoked.</p>
 *
 * @param p the layout parameters of the popup's content view
 */
void PopupWindow::InvokePopup(
    /* [in] */ IWindowManagerLayoutParams* p)
{
    mContext->GetCapsuleName(&(((CWindowManagerLayoutParams*)p)->mCapsuleName));
    mWindowManager->AddViewEx5(mPopupView,
            (IViewGroupLayoutParams*)p->Probe(EIID_IViewGroupLayoutParams));
}

/**
 * <p>Generate the layout parameters for the popup window.</p>
 *
 * @param token the window token used to bind the popup's window
 *
 * @return the layout parameters to pass to the window manager
 */
AutoPtr<IWindowManagerLayoutParams> PopupWindow::CreatePopupLayout(
   /* [in] */ IBinder* token)
{
    // generates the layout parameters for the drop down
    // we want a fixed size view located at the bottom left of the anchor
    AutoPtr<IWindowManagerLayoutParams> p;
    CWindowManagerLayoutParams::New((IWindowManagerLayoutParams**)&p);
    // these gravity settings put the view at the top left corner of the
    // screen. The view is then positioned to the appropriate location
    // by setting the x and y offsets to match the anchor's bottom
    // left corner
    ((CWindowManagerLayoutParams*)p.Get())->mGravity = Gravity_LEFT | Gravity_TOP;
    ((CWindowManagerLayoutParams*)p.Get())->mWidth = mLastWidth = mWidth;
    ((CWindowManagerLayoutParams*)p.Get())->mHeight = mLastHeight = mHeight;
    if (mBackground != NULL) {
        mBackground->GetOpacity(&(((CWindowManagerLayoutParams*)p.Get())->mFormat));
    }
    else {
        ((CWindowManagerLayoutParams*)p.Get())->mFormat = ElPixelFormat::TRANSLUCENT;
    }
    ((CWindowManagerLayoutParams*)p.Get())->mFlags =
            ComputeFlags(((CWindowManagerLayoutParams*)p.Get())->mFlags);
    ((CWindowManagerLayoutParams*)p.Get())->mType = mWindowLayoutType;
    ((CWindowManagerLayoutParams*)p.Get())->mToken = token;
    ((CWindowManagerLayoutParams*)p.Get())->mSoftInputMode = mSoftInputMode;
//    p->SetTitle(ArrayOf<Char8>("PopupWindow:"/* + Integer.toHexString(hashCode())*/, 32));

    return p;
}

Int32 PopupWindow::ComputeFlags(
    /* [in] */ Int32 curFlags)
{
    curFlags &= ~(
            WindowManagerLayoutParams_FLAG_IGNORE_CHEEK_PRESSES |
            WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE |
            WindowManagerLayoutParams_FLAG_NOT_TOUCHABLE |
            WindowManagerLayoutParams_FLAG_WATCH_OUTSIDE_TOUCH |
            WindowManagerLayoutParams_FLAG_LAYOUT_NO_LIMITS |
            WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM |
            WindowManagerLayoutParams_FLAG_SPLIT_TOUCH);
    if(mIgnoreCheekPress) {
        curFlags |= WindowManagerLayoutParams_FLAG_IGNORE_CHEEK_PRESSES;
    }
    if (!mFocusable) {
        curFlags |= WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE;
        if (mInputMethodMode == INPUT_METHOD_NEEDED) {
            curFlags |= WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM;
        }
    }
    else if (mInputMethodMode == INPUT_METHOD_NOT_NEEDED) {
        curFlags |= WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM;
    }
    if (!mTouchable) {
        curFlags |= WindowManagerLayoutParams_FLAG_NOT_TOUCHABLE;
    }
    if (mOutsideTouchable) {
        curFlags |= WindowManagerLayoutParams_FLAG_WATCH_OUTSIDE_TOUCH;
    }
    if (!mClippingEnabled) {
        curFlags |= WindowManagerLayoutParams_FLAG_LAYOUT_NO_LIMITS;
    }
    if (mSplitTouchEnabled) {
        curFlags |= WindowManagerLayoutParams_FLAG_SPLIT_TOUCH;
    }
    if (mLayoutInScreen) {
        curFlags |= WindowManagerLayoutParams_FLAG_LAYOUT_IN_SCREEN;
    }

    return curFlags;
}

Int32 PopupWindow::ComputeAnimationResource()
{
    if (mAnimationStyle == -1) {
        if (mIsDropdown) {
            return mAboveAnchor
                    ? 0x01030074/*com.android.internal.R.style.Animation_DropDownUp*/
                    : 0x01030073/*com.android.internal.R.style.Animation_DropDownDown*/;
        }
        return 0;
    }
    return mAnimationStyle;
}

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
Boolean PopupWindow::FindDropDownPosition(
    /* [in] */ IView* anchor,
    /* [in] */ IWindowManagerLayoutParams* p,
    /* [in] */ Int32 xoff,
    /* [in] */ Int32 yoff)
{
    anchor->GetLocationInWindow(&mDrawingLocation[0], &mDrawingLocation[1]);

    ((CWindowManagerLayoutParams*)p)->mX = mDrawingLocation[0] + xoff;

    Int32 height;
    anchor->GetHeight(&height);
    ((CWindowManagerLayoutParams*)p)->mY = mDrawingLocation[1] + height + yoff;

    Boolean onTop = FALSE;

    ((CWindowManagerLayoutParams*)p)->mGravity = Gravity_LEFT | Gravity_TOP;

    anchor->GetLocationOnScreen(&mScreenLocation[0], &mScreenLocation[1]);

    AutoPtr<IRect> displayFrame;
    ASSERT_SUCCEEDED(CRect::New((IRect**)&displayFrame));

    anchor->GetWindowVisibleDisplayFrame(displayFrame);

    AutoPtr<IView> root;
    anchor->GetRootView((IView**)&root);
    Int32 width;
    root->GetWidth(&width);

    if (((CWindowManagerLayoutParams*)p)->mY + mPopupHeight > ((CRect*)displayFrame.Get())->mBottom
                || ((CWindowManagerLayoutParams*)p)->mX + mPopupWidth - width > 0) {
        // if the drop down disappears at the bottom of the screen. we try to
        // scroll a parent scrollview or move the drop down back up on top of
        // the edit box
        Int32 scrollX, scrollY;
        anchor->GetScrollX(&scrollX);
        anchor->GetScrollY(&scrollY);

        AutoPtr<IRect> r;
        Int32 h;
        anchor->GetHeight(&h);
        ASSERT_SUCCEEDED(CRect::New(scrollX, scrollY,  scrollX + mPopupWidth + xoff,
                scrollY + mPopupHeight + h + yoff, (IRect**)&r));
        Boolean result;
        anchor->RequestRectangleOnScreenEx(r, TRUE, &result);

        // now we re-evaluate the space available, and decide from that
        // whether the pop-up will go above or below the anchor.
        anchor->GetLocationInWindow(&mDrawingLocation[0], &mDrawingLocation[1]);
        ((CWindowManagerLayoutParams*)p)->mX = mDrawingLocation[0] + xoff;
        anchor->GetHeight(&h);
        ((CWindowManagerLayoutParams*)p)->mY = mDrawingLocation[1] + h + yoff;

        // determine whether there is more space above or below the anchor
        anchor->GetLocationOnScreen(&mScreenLocation[0], &mScreenLocation[1]);
        anchor->GetHeight(&h);
        onTop = (((CRect*)displayFrame.Get())->mBottom - mScreenLocation[1] - h - yoff) <
                (mScreenLocation[1] - yoff - ((CRect*)displayFrame.Get())->mTop);
        if (onTop) {
            ((CWindowManagerLayoutParams*)p)->mGravity = Gravity_LEFT | Gravity_BOTTOM;
            Int32 rootH;
            root->GetHeight(&rootH);
            ((CWindowManagerLayoutParams*)p)->mY = rootH - mDrawingLocation[1] + yoff;
        }
        else {
            Int32 anchorH;
            anchor->GetHeight(&anchorH);
            ((CWindowManagerLayoutParams*)p)->mY = mDrawingLocation[1] + anchorH + yoff;
        }
    }

    ((CWindowManagerLayoutParams*)p)->mGravity |= Gravity_DISPLAY_CLIP_VERTICAL;

    return onTop;
}

ECode PopupWindow::GetMaxAvailableHeight(
    /* [in] */ IView* anchor,
    /* [out] */ Int32* maxAvailableHeight)
{
    return GetMaxAvailableHeightEx(anchor, 0, maxAvailableHeight);
}

ECode PopupWindow::GetMaxAvailableHeightEx(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 yOffset,
    /* [out] */ Int32* maxAvailableHeight)
{
    return GetMaxAvailableHeightEx2(anchor, yOffset, FALSE, maxAvailableHeight);
}

ECode PopupWindow::GetMaxAvailableHeightEx2(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 yOffset,
    /* [in] */ Boolean ignoreBottomDecorations,
    /* [out] */ Int32* maxAvailableHeight)
{
    AutoPtr<IRect> displayFrame;
    ASSERT_SUCCEEDED(CRect::New((IRect**)&displayFrame));
    anchor->GetWindowVisibleDisplayFrame(displayFrame);

    Int32* anchorPos = mDrawingLocation;
    anchor->GetLocationOnScreen(&anchorPos[0], &anchorPos[1]);

    Int32 bottomEdge = ((CRect*)displayFrame.Get())->mBottom;
    if (ignoreBottomDecorations) {
        AutoPtr<IContext> ctx;
        anchor->GetContext((IContext**)&ctx);
        AutoPtr<IResources> res;
        ctx->GetResources((IResources**)&res);
        AutoPtr<IDisplayMetrics> mtr;
        res->GetDisplayMetrics((IDisplayMetrics**)&mtr);
        bottomEdge = ((CDisplayMetrics*)mtr.Get())->mHeightPixels;
    }
    Int32 height;
    anchor->GetHeight(&height);
    Int32 distanceToBottom = bottomEdge - (anchorPos[1] + height) - yOffset;
    Int32 distanceToTop = anchorPos[1] - ((CRect*)displayFrame.Get())->mTop + yOffset;

    // anchorPos[1] is distance from anchor to top of screen
    Int32 returnedHeight = Math::Max(distanceToBottom, distanceToTop);
    if (mBackground != NULL) {
        Boolean result;
        mBackground->GetPadding(mTempRect, &result);
        returnedHeight -= ((CRect*)mTempRect.Get())->mTop + ((CRect*)mTempRect.Get())->mBottom;
    }

    *maxAvailableHeight = returnedHeight;

    return NOERROR;
}

ECode PopupWindow::Dismiss()
{
    Boolean isShowing;
    IsShowing(&isShowing);
    if (isShowing && mPopupView != NULL) {
        UnregisterForScrollChanged();

        //try {
        mWindowManager->RemoveView(mPopupView);

//        } finally {
        if (mPopupView != mContentView && mPopupView &&
                mPopupView->Probe(EIID_IViewManager) != NULL) {
            ((IViewManager*)mPopupView->Probe(EIID_IViewManager))->RemoveView(mContentView);
        }
        mPopupView = NULL;
        mIsShowing = FALSE;

        if (mOnDismissListener != NULL) {
            mOnDismissListener->OnDismiss();
        }
        //}
    }

    return NOERROR;
}

ECode PopupWindow::SetOnDismissListener(
    /* [in] */ IPopupWindowOnDismissListener* onDismissListener)
{
    mOnDismissListener = onDismissListener;

    return NOERROR;
}

ECode PopupWindow::Update()
{
    Boolean isShowing;
    IsShowing(&isShowing);
    if (!isShowing || mContentView == NULL) {
        return NOERROR;
    }

    AutoPtr<IViewGroupLayoutParams> p;
    mPopupView->GetLayoutParams((IViewGroupLayoutParams**)&p);
    IWindowManagerLayoutParams* lp = (IWindowManagerLayoutParams*)p->Probe(EIID_IWindowManagerLayoutParams);


    Boolean update = FALSE;

    Int32 newAnim = ComputeAnimationResource();
    if (newAnim != ((CWindowManagerLayoutParams*)lp)->mWindowAnimations) {
        ((CWindowManagerLayoutParams*)lp)->mWindowAnimations = newAnim;
        update = TRUE;
    }

    Int32 newFlags = ComputeFlags(((CWindowManagerLayoutParams*)lp)->mFlags);
    if (newFlags != ((CWindowManagerLayoutParams*)lp)->mFlags) {
        ((CWindowManagerLayoutParams*)lp)->mFlags = newFlags;
        update = TRUE;
    }

    if (update) {
        mWindowManager->UpdateViewLayout(mPopupView, lp);
    }

    return NOERROR;
}

ECode PopupWindow::UpdateEx(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    AutoPtr<IViewGroupLayoutParams> p;
    mPopupView->GetLayoutParams((IViewGroupLayoutParams**)&p);
    IWindowManagerLayoutParams* lp = (IWindowManagerLayoutParams*)p->Probe(EIID_IWindowManagerLayoutParams);

    return UpdateEx3(((CWindowManagerLayoutParams*)lp)->mX,
            ((CWindowManagerLayoutParams*)lp)->mY, width, height, FALSE);
}

ECode PopupWindow::UpdateEx2(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return UpdateEx3(x, y, width, height, FALSE);
}

ECode PopupWindow::UpdateEx3(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean force)
{
    if (width != -1) {
        mLastWidth = width;
        SetWidth(width);
    }

    if (height != -1) {
        mLastHeight = height;
        SetHeight(height);
    }

    Boolean isShowing;
    IsShowing(&isShowing);
    if (!isShowing || mContentView == NULL) {
        return NOERROR;
    }

    AutoPtr<IViewGroupLayoutParams> p;
    mPopupView->GetLayoutParams((IViewGroupLayoutParams**)&p);
    IWindowManagerLayoutParams* lp = (IWindowManagerLayoutParams*)p->Probe(EIID_IWindowManagerLayoutParams);

    Boolean update = force;

    Int32 finalWidth = mWidthMode < 0 ? mWidthMode : mLastWidth;
    if (width != -1 && ((CWindowManagerLayoutParams*)lp)->mWidth != finalWidth) {
        ((CWindowManagerLayoutParams*)lp)->mWidth = mLastWidth = finalWidth;
        update = TRUE;
    }

    Int32 finalHeight = mHeightMode < 0 ? mHeightMode : mLastHeight;
    if (height != -1 && ((CWindowManagerLayoutParams*)lp)->mHeight != finalHeight) {
        ((CWindowManagerLayoutParams*)lp)->mHeight = mLastHeight = finalHeight;
        update = TRUE;
    }

    if (((CWindowManagerLayoutParams*)lp)->mX != x) {
        ((CWindowManagerLayoutParams*)lp)->mX = x;
        update = TRUE;
    }

    if (((CWindowManagerLayoutParams*)lp)->mY != y) {
        ((CWindowManagerLayoutParams*)lp)->mY = y;
        update = TRUE;
    }

    Int32 newAnim = ComputeAnimationResource();
    if (newAnim != ((CWindowManagerLayoutParams*)lp)->mWindowAnimations) {
        ((CWindowManagerLayoutParams*)lp)->mWindowAnimations = newAnim;
        update = TRUE;
    }

    Int32 newFlags = ComputeFlags(((CWindowManagerLayoutParams*)lp)->mFlags);
    if (newFlags != ((CWindowManagerLayoutParams*)lp)->mFlags) {
        ((CWindowManagerLayoutParams*)lp)->mFlags = newFlags;
        update = TRUE;
    }

    if (update) {
        mWindowManager->UpdateViewLayout(mPopupView, lp);
    }

    return NOERROR;
}

ECode PopupWindow::UpdateEx4(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return UpdateEx6(anchor, FALSE, 0, 0, TRUE, width, height);
}

ECode PopupWindow::UpdateEx5(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 width,
    /* [in] */ Int32 xoff,
    /* [in] */ Int32 yoff,
    /* [in] */ Int32 height)
{
    return UpdateEx6(anchor, TRUE, xoff, yoff, TRUE, width, height);
}

ECode PopupWindow::UpdateEx6(
        /* [in] */ IView* anchor,
        /* [in] */ Boolean updateLocation,
        /* [in] */ Int32 xoff,
        /* [in] */ Int32 yoff,
        /* [in] */ Boolean updateDimension,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height)
{
    Boolean isShowing;
    IsShowing(&isShowing);
    if (!isShowing || mContentView == NULL) {
        return NOERROR;
    }

    IView* oldAnchor = mAnchor;
    if (oldAnchor == NULL || oldAnchor != anchor ||
            (updateLocation && (mAnchorXoff != xoff || mAnchorYoff != yoff))) {
        RegisterForScrollChanged(anchor, xoff, yoff);
    }

    AutoPtr<IViewGroupLayoutParams> p;
    mPopupView->GetLayoutParams((IViewGroupLayoutParams**)&p);
    IWindowManagerLayoutParams* lp = (IWindowManagerLayoutParams*)p->Probe(EIID_IWindowManagerLayoutParams);

    if (updateDimension) {
        if (width == -1) {
            width = mPopupWidth;
        }
        else {
            mPopupWidth = width;
        }
        if (height == -1) {
            height = mPopupHeight;
        }
        else {
            mPopupHeight = height;
        }
    }

    Int32 x = ((CWindowManagerLayoutParams*)lp)->mX;
    Int32 y = ((CWindowManagerLayoutParams*)lp)->mY;

    if (updateLocation) {
        UpdateAboveAnchor(FindDropDownPosition(anchor, lp, xoff, yoff));
    }
    else {
        UpdateAboveAnchor(FindDropDownPosition(anchor, lp, mAnchorXoff, mAnchorYoff));
    }

    return UpdateEx3(((CWindowManagerLayoutParams*)lp)->mX,
            ((CWindowManagerLayoutParams*)lp)->mY, width, height,
            x != ((CWindowManagerLayoutParams*)lp)->mX || y != ((CWindowManagerLayoutParams*)lp)->mY);
}

void PopupWindow::UnregisterForScrollChanged()
{
    IView* anchorRef = mAnchor;
    AutoPtr<IView> anchor;
    if (anchorRef != NULL) {
        anchor = anchorRef;
    }
    if (anchor != NULL) {
        AutoPtr<IViewTreeObserver> vto;
//        anchor->GetViewTreeObserver((IViewTreeObserver**)&vto);
//        vto->RemoveOnScrollChangedListener(mOnScrollChangedListener);
    }
    mAnchor = NULL;
}

void PopupWindow::RegisterForScrollChanged(
    /* [in] */ IView* anchor,
   /* [in] */ Int32 xoff,
   /* [in] */ Int32 yoff)
{
    UnregisterForScrollChanged();

    mAnchor = anchor;
    AutoPtr<IViewTreeObserver> vto;
//    anchor->GetViewTreeObserver((IViewTreeObserver**)&vto);
    if (vto != NULL) {
//        vto->AddOnScrollChangedListener(mOnScrollChangedListener);
    }

    mAnchorXoff = xoff;
    mAnchorYoff = yoff;
}

ECode PopupWindow::OnScrollChanged()
{
    IView* anchor = mAnchor.Get();
    if (anchor != NULL && mPopupView != NULL) {
        AutoPtr<IViewGroupLayoutParams> p;
        mPopupView->GetLayoutParams((IViewGroupLayoutParams**)&p);
        IWindowManagerLayoutParams* lp = (IWindowManagerLayoutParams*)p->Probe(EIID_IWindowManagerLayoutParams);

        UpdateAboveAnchor(FindDropDownPosition(anchor, lp, mAnchorXoff, mAnchorYoff));
        UpdateEx3(((CWindowManagerLayoutParams*)lp)->mX,
                ((CWindowManagerLayoutParams*)lp)->mY, -1, -1, TRUE);
    }

    return NOERROR;
}

ECode PopupWindow::Init(
    /* [in] */ IContext* ctx)
{
    return Init(ctx, NULL);
}

ECode PopupWindow::Init(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return Init(ctx, attrs, 0x01010076/*com.android.internal.R.attr.popupWindowStyle*/);
}

static Int32 R_Styleable_PopupWindow[] = {
    0x01010176, 0x010102c9
};

ECode PopupWindow::Init(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    assert(ctx != NULL);

    mContext = ctx;
    ctx->GetSystemService(Context_WINDOW_SERVICE, (IInterface**)&mWindowManager);

    AutoPtr<ITypedArray> a;
    FAIL_RETURN(ctx->ObtainStyledAttributesEx3(attrs,
            ArrayOf<Int32>(R_Styleable_PopupWindow, 2)/*com.android.internal.R.styleable.PopupWindow*/,
            defStyle, 0, (ITypedArray**)&a));

    a->GetDrawable(0/*R.styleable.PopupWindow_popupBackground*/, (IDrawable**)&mBackground);

    Int32 animStyle;
    a->GetResourceId(1/*R.styleable.PopupWindow_popupAnimationStyle*/, -1, &animStyle);
    mAnimationStyle = animStyle == 0x0103007b/*com.android.internal.R.style.Animation_PopupWindow*/ ? -1 :
            animStyle;

    // If this is a StateListDrawable, try to find and store the drawable to be
    // used when the drop-down is placed above its anchor view, and the one to be
    // used when the drop-down is placed below its anchor view. We extract
    // the drawables ourselves to work around a problem with using refreshDrawableState
    // that it will take into account the padding of all drawables specified in a
    // StateListDrawable, thus adding superfluous padding to drop-down views.
    //
    // We assume a StateListDrawable will have a drawable for ABOVE_ANCHOR_STATE_SET and
    // at least one other drawable, intended for the 'below-anchor state'.
    if (mBackground && mBackground->Probe(EIID_IStateListDrawable) != NULL) {
        IStateListDrawable* background =
                    (IStateListDrawable*)mBackground->Probe(EIID_IStateListDrawable);

        // Find the above-anchor view - this one's easy, it should be labeled as such.
        Int32 aboveAnchorStateIndex;
        background->GetStateDrawableIndex(
                ArrayOf<Int32>(ABOVE_ANCHOR_STATE_SET, 1), &aboveAnchorStateIndex);

        // Now, for the below-anchor view, look for any other drawable specified in the
        // StateListDrawable which is not for the above-anchor state and use that.
        Int32 count;
        background->GetStateCount(&count);
        Int32 belowAnchorStateIndex = -1;
        for (Int32 i = 0; i < count; i++) {
            if (i != aboveAnchorStateIndex) {
                belowAnchorStateIndex = i;
                break;
            }
        }

        // Store the drawables we found, if we found them. Otherwise, set them both
        // to null so that we'll just use refreshDrawableState.
        if (aboveAnchorStateIndex != -1 && belowAnchorStateIndex != -1) {
            background->GetStateDrawable(aboveAnchorStateIndex,
                    (IDrawable**)&mAboveAnchorBackgroundDrawable);
            background->GetStateDrawable(belowAnchorStateIndex,
                    (IDrawable**)&mBelowAnchorBackgroundDrawable);
        }
        else {
            mBelowAnchorBackgroundDrawable = NULL;
            mAboveAnchorBackgroundDrawable = NULL;
        }
    }

    a->Recycle();

    return NOERROR;
}

ECode PopupWindow::Init()
{
    return Init(NULL, 0, 0);
}

ECode PopupWindow::Init(
    /* [in] */ IView* contentView)
{
    return Init(contentView, 0, 0);
}

ECode PopupWindow::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return Init(NULL, width, height);
}

ECode PopupWindow::Init(
    /* [in] */ IView* contentView,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return Init(contentView, width, height, FALSE);
}

ECode PopupWindow::Init(
    /* [in] */ IView* contentView,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean focusable)
{
    if (contentView != NULL) {
        contentView->GetContext((IContext**)&mContext);
        mContext->GetSystemService(Context_WINDOW_SERVICE, (IInterface**)&mWindowManager);
    }
    SetContentView(contentView);
    SetWidth(width);
    SetHeight(height);
    SetFocusable(focusable);

    return NOERROR;
}

IVIEW_METHODS_IMPL(PopupWindow::PopupViewContainer, FrameLayout, FrameLayout);

IVIEWGROUP_METHODS_IMPL(PopupWindow::PopupViewContainer, FrameLayout, FrameLayout);

PopupWindow::PopupViewContainer::PopupViewContainer(
    /* [in] */ PopupWindow* popupWindow,
    /* [in] */ IContext* ctx)
    : FrameLayout(ctx)
{
    mPopupWindow = popupWindow;
}

PInterface PopupWindow::PopupViewContainer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        assert(0);
    }
    else if (riid == EIID_IInterface) {
        return (IInterface*)(IFrameLayout*)this;
    }
    else if (riid == EIID_IView) {
        return (IView*)(IFrameLayout*)this;
    }
    else if (riid == EIID_IFrameLayout) {
        return (IFrameLayout*)this;
    }

    return NULL;
}

UInt32 PopupWindow::PopupViewContainer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 PopupWindow::PopupViewContainer::Release()
{
    return ElRefBase::Release();
}

ECode PopupWindow::PopupViewContainer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode PopupWindow::PopupViewContainer::SetForegroundGravity(
    /* [in] */ Int32 foregroundGravity)
{
    FrameLayout::SetForegroundGravity(foregroundGravity);

    return NOERROR;
}

ECode PopupWindow::PopupViewContainer::SetForeground(
    /* [in] */ IDrawable* drawable)
{
    FrameLayout::SetForeground(drawable);

    return NOERROR;
}

ECode PopupWindow::PopupViewContainer::GetForeground(
    /* [out] */ IDrawable** foreground)
{
    VALIDATE_NOT_NULL(foreground);
    AutoPtr<IDrawable> d = FrameLayout::GetForeground();
    *foreground = d.Get();
    if (*foreground) {
        (*foreground)->AddRef();
    }

    return NOERROR;
}

ECode PopupWindow::PopupViewContainer::SetMeasureAllChildren(
    /* [in] */ Boolean measureAll)
{
    FrameLayout::SetMeasureAllChildren(measureAll);

    return NOERROR;
}

ECode PopupWindow::PopupViewContainer::GetConsiderGoneChildrenWhenMeasuring(
    /* [out] */ Boolean* measureAll)
{
    VALIDATE_NOT_NULL(measureAll);
    *measureAll = FrameLayout::GetConsiderGoneChildrenWhenMeasuring();

    return NOERROR;
}

ECode PopupWindow::PopupViewContainer::OnCreateDrawableState(
    /* [in] */ Int32 extraSpace,
    /* [out] */ ArrayOf<Int32>** drawableState)
{
    if (mPopupWindow->mAboveAnchor) {
        // 1 more needed for the above anchor state
        FrameLayout::OnCreateDrawableState(extraSpace + 1, drawableState);
        ArrayOf<Int32> state(ABOVE_ANCHOR_STATE_SET, 1);
        View::MergeDrawableStates(*drawableState, &state);
        return NOERROR;
    }
    else {
        return FrameLayout::OnCreateDrawableState(extraSpace, drawableState);
    }
}

Boolean PopupWindow::PopupViewContainer::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    Int32 keyCode;
    event->GetKeyCode(&keyCode);
    if (keyCode == KeyEvent_KEYCODE_BACK) {
        Int32 action;
        event->GetAction(&action);
        if (action == KeyEvent_ACTION_DOWN) {
            Int32 count;
            event->GetRepeatCount(&count);
            if (count == 0) {
                AutoPtr<IDispatcherState> dispatcherState;
                GetKeyDispatcherState((IDispatcherState**)&dispatcherState);
                dispatcherState->StartTracking(event, this->Probe(EIID_IInterface));
                return TRUE;
            }
        }
        else if (action == KeyEvent_ACTION_UP) {
            AutoPtr<IDispatcherState> dispatcherState;
            GetKeyDispatcherState((IDispatcherState**)&dispatcherState);
            Boolean isTracking;
            dispatcherState->IsTracking(event, &isTracking);
            if (isTracking) {
                Boolean isCanceled;
                event->IsCanceled(&isCanceled);
                if (!isCanceled) {
                    mPopupWindow->Dismiss();
                    return TRUE;
                }
            }
        }
        else {
            return FrameLayout::DispatchKeyEvent(event);
        }
    }
    else {
        return FrameLayout::DispatchKeyEvent(event);
    }

    return FALSE;
}

Boolean PopupWindow::PopupViewContainer::DispatchTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    if (mPopupWindow->mTouchInterceptor != NULL) {
        Boolean result;
        mPopupWindow->mTouchInterceptor->OnTouch(
                (IView*)this->Probe(EIID_IView), event, &result);
        if (result)
            return TRUE;
    }

    return FrameLayout::DispatchTouchEvent(event);
}

Boolean PopupWindow::PopupViewContainer::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    Int32 x, y;
    event->GetX((Float*)&x);
    event->GetY((Float*)&y);

    Int32 action;
    event->GetAction(&action);
    if (action == MotionEvent_ACTION_DOWN) {
        Int32 width, height;
        mPopupWindow->GetWidth(&width);
        mPopupWindow->GetHeight(&height);
        if ((x < 0) || (x >= width) || (y < 0) || (y >= height)) {
            mPopupWindow->Dismiss();
            return TRUE;
        }
    }
    else if (action == MotionEvent_ACTION_OUTSIDE) {
        mPopupWindow->Dismiss();
        return TRUE;
    }
    else {
        return FrameLayout::OnTouchEvent(event);
    }
}

//ECode PopupWindow::PopupViewContainer::SendAccessibilityEvent(
//    /* [in] */ Int32 eventType)
//{
//    // clinets are interested in the content not the container, make it event source
//    if (mContentView != NULL) {
//        return mContentView->SendAccessibilityEvent(eventType);
//    }
//    else {
//        return super::sendAccessibilityEvent(eventType);
//    }
//}
