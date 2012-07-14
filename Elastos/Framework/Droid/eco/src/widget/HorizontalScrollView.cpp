
#include "widget/HorizontalScrollView.h"
#include "widget/ScrollView.h"
#include "widget/CFrameLayoutLayoutParams.h"
#include "view/FocusFinder.h"
#include "view/CViewGroupLayoutParams.h"
#include "view/CViewGroupMarginLayoutParams.h"
#include "view/animation/AnimationUtils.h"
#include <elastos/Math.h>

using namespace Elastos::Core;


const Int32 HorizontalScrollView::ANIMATED_SCROLL_GAP = ScrollView::ANIMATED_SCROLL_GAP;
const Float HorizontalScrollView::MAX_SCROLL_FACTOR = ScrollView::MAX_SCROLL_FACTOR;
const Int32 HorizontalScrollView::INVALID_POINTER;

HorizontalScrollView::HorizontalScrollView()
    : mLastScroll(0)
    , mScroller(NULL)
    , mEdgeGlowLeft(NULL)
    , mEdgeGlowRight(NULL)
    , mScrollViewMovedFocus(FALSE)
    , mIsLayoutDirty(TRUE)
    , mIsBeingDragged(FALSE)
    , mFillViewport(FALSE)
    , mSmoothScrollingEnabled(TRUE)
    , mTouchSlop(0)
    , mMinimumVelocity(0)
    , mMaximumVelocity(0)
    , mOverscrollDistance(0)
    , mOverflingDistance(0)
    , mActivePointerId(INVALID_POINTER)

{
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mTempRect));
}

HorizontalScrollView::HorizontalScrollView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : FrameLayout(context, attrs, defStyle)
    , mLastScroll(0)
    , mScroller(NULL)
    , mEdgeGlowLeft(NULL)
    , mEdgeGlowRight(NULL)
    , mScrollViewMovedFocus(FALSE)
    , mIsLayoutDirty(TRUE)
    , mIsBeingDragged(FALSE)
    , mFillViewport(FALSE)
    , mSmoothScrollingEnabled(TRUE)
    , mTouchSlop(0)
    , mMinimumVelocity(0)
    , mMaximumVelocity(0)
    , mOverscrollDistance(0)
    , mOverflingDistance(0)
    , mActivePointerId(INVALID_POINTER)
{
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mTempRect));
    InitScrollView();
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle));
}

HorizontalScrollView::~HorizontalScrollView()
{
    if (mScroller != NULL) delete mScroller;
    if (mEdgeGlowLeft != NULL) delete mEdgeGlowLeft;
    if (mEdgeGlowRight != NULL) delete mEdgeGlowRight;
}

static Int32 R_Styleable_HorizontalScrollView[] = {
    0x0101017a
};

ECode HorizontalScrollView::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ITypedArray> a;

    FAIL_RETURN(context->ObtainStyledAttributesEx3(attrs,
            ArrayOf<Int32>(R_Styleable_HorizontalScrollView, sizeof(R_Styleable_HorizontalScrollView) / sizeof(Int32))/*com.android.internal.R.styleable.HorizontalScrollView*/,
            defStyle, 0,
            (ITypedArray**)&a));

    Boolean value;
    a->GetBoolean(0/*R.styleable.HorizontalScrollView_fillViewport*/, FALSE, &value);

    FAIL_RETURN(SetFillViewport(value));

    a->Recycle();

    return NOERROR;
}

ECode HorizontalScrollView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(FrameLayout::Init(context, attrs, defStyle));
    InitScrollView();
    return InitFromAttributes(context, attrs, defStyle);
}

Float HorizontalScrollView::GetLeftFadingEdgeStrength()
{
    if (GetChildCount() == 0) {
        return 0.0;
    }

    Int32 length = GetHorizontalFadingEdgeLength();

    if (mScrollX < length) {
        return mScrollX / (Float)length;
    }

    return 1.0;
}

Float HorizontalScrollView::GetRightFadingEdgeStrength()
{
    if (GetChildCount() == 0) {
        return 0.0;
    }

    Int32 length = GetHorizontalFadingEdgeLength();
    Int32 rightEdge = GetWidth() - mPaddingRight;
    Int32 right;
    GetChildAt(0)->GetRight(&right);
    Int32 span = right - mScrollX - rightEdge;
    if (span < length) {
        return span / (Float)length;
    }

    return 1.0;
}

Int32 HorizontalScrollView::GetMaxScrollAmount()
{
    return (Int32)(MAX_SCROLL_FACTOR * (mRight - mLeft));
}

void HorizontalScrollView::InitScrollView()
{
    mScroller = new OverScroller(GetContext());
    SetFocusable(TRUE);
    SetDescendantFocusability(ViewGroup::FOCUS_AFTER_DESCENDANTS);
    SetWillNotDraw(FALSE);
    ViewConfiguration* configuration = ViewConfiguration::Get(mContext);
    mTouchSlop = configuration->GetScaledTouchSlop();
    mMinimumVelocity = configuration->GetScaledMinimumFlingVelocity();
    mMaximumVelocity = configuration->GetScaledMaximumFlingVelocity();
    mOverscrollDistance = configuration->GetScaledOverscrollDistance();
    mOverflingDistance = configuration->GetScaledOverflingDistance();
}

ECode HorizontalScrollView::AddView(
    /* [in] */ IView* child)
{
    if (GetChildCount() > 0) {
        //throw new IllegalStateException("HorizontalScrollView can host only one direct child");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return FrameLayout::AddView(child);
}

ECode HorizontalScrollView::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index)
{
    if (GetChildCount() > 0) {
        //throw new IllegalStateException("HorizontalScrollView can host only one direct child");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return FrameLayout::AddView(child, index);
}

ECode HorizontalScrollView::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return ViewGroup::AddView(child, width, height);
}

ECode HorizontalScrollView::AddView(
    /* [in] */ IView* child,
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (GetChildCount() > 0) {
        //throw new IllegalStateException("HorizontalScrollView can host only one direct child");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return FrameLayout::AddView(child, params);
}

ECode HorizontalScrollView::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (GetChildCount() > 0) {
        //throw new IllegalStateException("HorizontalScrollView can host only one direct child");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return FrameLayout::AddView(child, index, params);
}

Boolean HorizontalScrollView::CanScroll()
{
    AutoPtr<IView> child = GetChildAt(0);
    if (child != NULL) {
        Int32 childWidth;
        child->GetWidth(&childWidth);
        return GetWidth() < childWidth + mPaddingLeft + mPaddingRight;
    }
    return FALSE;
}

Boolean HorizontalScrollView::IsFillViewport()
{
    return mFillViewport;
}

ECode HorizontalScrollView::SetFillViewport(
    /* [in] */ Boolean fillViewport)
{
    if (fillViewport != mFillViewport) {
        mFillViewport = fillViewport;
        RequestLayout();
    }

    return NOERROR;
}

Boolean HorizontalScrollView::IsSmoothScrollingEnabled()
{
    return mSmoothScrollingEnabled;
}

ECode HorizontalScrollView::SetSmoothScrollingEnabled(
    /* [in] */ Boolean smoothScrollingEnabled)
{
    mSmoothScrollingEnabled = smoothScrollingEnabled;
    return NOERROR;
}

void HorizontalScrollView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    FrameLayout::OnMeasure(widthMeasureSpec, heightMeasureSpec);

    if (!mFillViewport) {
        return;
    }

    Int32 widthMode = MeasureSpec::GetMode(widthMeasureSpec);
    if (widthMode == MeasureSpec::UNSPECIFIED) {
        return;
    }

    if (GetChildCount() > 0) {
        AutoPtr<IView> child = GetChildAt(0);
        Int32 width = FrameLayout::GetMeasuredWidth();
        Int32 childWidth;
        child->GetMeasuredHeight(&childWidth);
        if (childWidth < width) {
            AutoPtr<IFrameLayoutLayoutParams> params;
            child->GetLayoutParams((IViewGroupLayoutParams**)&params);

            Int32 childHeightMeasureSpec = GetChildMeasureSpec(heightMeasureSpec,
                    mPaddingLeft + mPaddingRight,
                    ((CFrameLayoutLayoutParams*)params.Get())->mHeight);
            width -= mPaddingLeft;
            width -= mPaddingRight;
            Int32 childWidthMeasureSpec =
                    MeasureSpec::MakeMeasureSpec(width, MeasureSpec::EXACTLY);

            child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
        }
    }
}

Boolean HorizontalScrollView::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    // Let the focused view and/or our descendants get the key first
    return FrameLayout::DispatchKeyEvent(event) || ExecuteKeyEvent(event);
}

Boolean HorizontalScrollView::ExecuteKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    mTempRect->SetEmpty();

    if (!CanScroll()) {
        if (IsFocused()) {
            AutoPtr<IView> currentFocused = FindFocus();
            if (currentFocused.Get() == (IView*)this->Probe(EIID_IView)) {
                currentFocused = NULL;
            }
            AutoPtr<IView> nextFocused = FocusFinder::GetInstance()->FindNextFocus(
                    (IViewGroup*)this->Probe(EIID_IViewGroup),
                    currentFocused, View::FOCUS_RIGHT);

            if (nextFocused != NULL && nextFocused.Get() != (IView*)this->Probe(EIID_IView)) {
                Boolean isFocus;
                nextFocused->RequestFocusEx(View::FOCUS_RIGHT, &isFocus);
                return isFocus;
            }
            else return FALSE;
        }
        return FALSE;
    }

    Boolean handled = FALSE;
    Int32 action;
    event->GetAction(&action);
    if (action == KeyEvent_ACTION_DOWN) {
        Int32 keyCode;
        event->GetKeyCode(&keyCode);
        Boolean isPressed;
        switch (keyCode) {
            case KeyEvent_KEYCODE_DPAD_LEFT:
                event->IsAltPressed(&isPressed);
                if (!isPressed) {
                    handled = ArrowScroll(View::FOCUS_LEFT);
                }
                else {
                    handled = FullScroll(View::FOCUS_LEFT);
                }
                break;
            case KeyEvent_KEYCODE_DPAD_RIGHT:
                event->IsAltPressed(&isPressed);
                if (!isPressed) {
                    handled = ArrowScroll(View::FOCUS_RIGHT);
                }
                else {
                    handled = FullScroll(View::FOCUS_RIGHT);
                }
                break;
            case KeyEvent_KEYCODE_SPACE:
                event->IsShiftPressed(&isPressed);
                PageScroll(isPressed ? View::FOCUS_LEFT : View::FOCUS_RIGHT);
                break;
        }
    }

    return handled;
}

Boolean HorizontalScrollView::InChild(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (GetChildCount() > 0) {
        Int32 scrollX = mScrollX;
        AutoPtr<IView> child = GetChildAt(0);
        Int32 top, bottom, left, right;
        child->GetTop(&top);
        child->GetBottom(&bottom);
        child->GetLeft(&left);
        child->GetRight(&right);
        return !(y < top
                || y >= bottom
                || x < left - scrollX
                || x >= right - scrollX);
    }

    return FALSE;
}

Boolean HorizontalScrollView::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    /*
     * This method JUST determines whether we want to intercept the motion.
     * If we return true, onMotionEvent will be called and we do the actual
     * scrolling there.
     */

    /*
    * Shortcut the most recurring case: the user is in the dragging
    * state and he is moving his finger.  We want to intercept this
    * motion.
    */
    Int32 action;
    ev->GetAction(&action);
    if ((action == MotionEvent_ACTION_MOVE) && (mIsBeingDragged)) {
        return TRUE;
    }

    switch (action & MotionEvent_ACTION_MASK) {
        case MotionEvent_ACTION_MOVE: {
            /*
             * mIsBeingDragged == false, otherwise the shortcut would have caught it. Check
             * whether the user has moved far enough from his original down touch.
             */

            /*
            * Locally do absolute value. mLastMotionX is set to the y value
            * of the down event.
            */
            Int32 activePointerId = mActivePointerId;
            if (activePointerId == INVALID_POINTER) {
                // If we don't have a valid id, the touch down wasn't on content.
                break;
            }

            Int32 pointerIndex;
            ev->FindPointerIndex(activePointerId, &pointerIndex);
            Float x;
            ev->GetXEx(pointerIndex, &x);
            Int32 xDiff = (Int32)Math::Abs(x - mLastMotionX);
            if (xDiff > mTouchSlop) {
                mIsBeingDragged = TRUE;
                mLastMotionX = x;
                if (mParent != NULL) {
                    mParent->RequestDisallowInterceptTouchEvent(TRUE);
                }
            }
            break;
        }

        case MotionEvent_ACTION_DOWN: {
            Float y, x;
            ev->GetY(&y);
            ev->GetX(&x);
            if (!InChild((Int32)x, (Int32)y)) {
                mIsBeingDragged = FALSE;
                break;
            }

            /*
             * Remember location of down touch.
             * ACTION_DOWN always refers to pointer index 0.
             */
            mLastMotionX = x;
            ev->GetPointerId(0, &mActivePointerId);

            /*
            * If being flinged and user touches the screen, initiate drag;
            * otherwise don't.  mScroller.isFinished should be false when
            * being flinged.
            */
            mIsBeingDragged = !mScroller->IsFinished();
            break;
        }

        case MotionEvent_ACTION_CANCEL:
        case MotionEvent_ACTION_UP:
            /* Release the drag */
            mIsBeingDragged = FALSE;
            mActivePointerId = INVALID_POINTER;
            if (mScroller->SpringBack(mScrollX, mScrollY, 0, GetScrollRange(), 0, 0)) {
                Invalidate();
            }
            break;
        case MotionEvent_ACTION_POINTER_UP:
            OnSecondaryPointerUp(ev);
            break;
    }

    /*
    * The only time we want to intercept motion events is if we are in the
    * drag mode.
    */
    return mIsBeingDragged;
}

Boolean HorizontalScrollView::OnTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    Int32 action, flags;
    ev->GetAction(&action);
    ev->GetEdgeFlags(&flags);
    if (action == MotionEvent_ACTION_DOWN && flags != 0) {
        // Don't handle edge touches immediately -- they may actually belong to one of our
        // descendants.
        return FALSE;
    }

    if (mVelocityTracker == NULL) {
        mVelocityTracker = VelocityTracker::Obtain();
    }
    mVelocityTracker->AddMovement(ev);

    switch (action & MotionEvent_ACTION_MASK) {
        case MotionEvent_ACTION_DOWN: {
            mIsBeingDragged = GetChildCount() != 0;
            if (!mIsBeingDragged) {
                return FALSE;
            }

            /*
             * If being flinged and user touches, stop the fling. isFinished
             * will be false if being flinged.
             */
            if (!mScroller->IsFinished()) {
                mScroller->AbortAnimation();
            }

            // Remember where the motion event started
            ev->GetX(&mLastMotionX);
            ev->GetPointerId(0, &mActivePointerId);
            break;
        }
        case MotionEvent_ACTION_MOVE:
            if (mIsBeingDragged) {
                // Scroll to follow the motion event
                Int32 activePointerIndex;
                ev->FindPointerIndex(mActivePointerId, &activePointerIndex);
                Float x;
                ev->GetXEx(activePointerIndex, &x);
                Int32 deltaX = (Int32) (mLastMotionX - x);
                mLastMotionX = x;

                Int32 oldX = mScrollX;
                Int32 oldY = mScrollY;
                Int32 range = GetScrollRange();
                if (OverScrollBy(deltaX, 0, mScrollX, 0, range, 0,
                        mOverscrollDistance, 0, TRUE)) {
                    // Break our velocity if we hit a scroll barrier.
                    mVelocityTracker->Clear();
                }
                OnScrollChanged(mScrollX, mScrollY, oldX, oldY);

                Int32 overscrollMode = GetOverScrollMode();
                if (overscrollMode == OVER_SCROLL_ALWAYS ||
                        (overscrollMode == OVER_SCROLL_IF_CONTENT_SCROLLS && range > 0)) {
                    Int32 pulledToX = oldX + deltaX;
                    if (pulledToX < 0) {
                        mEdgeGlowLeft->OnPull((Float)deltaX / GetWidth());
                        if (!mEdgeGlowRight->IsFinished()) {
                            mEdgeGlowRight->OnRelease();
                        }
                    }
                    else if (pulledToX > range) {
                        mEdgeGlowRight->OnPull((Float)deltaX / GetWidth());
                        if (!mEdgeGlowLeft->IsFinished()) {
                            mEdgeGlowLeft->OnRelease();
                        }
                    }
                    if (mEdgeGlowLeft != NULL
                            && (!mEdgeGlowLeft->IsFinished() || !mEdgeGlowRight->IsFinished())) {
                        Invalidate();
                    }
                }
            }
            break;
        case MotionEvent_ACTION_UP:
            if (mIsBeingDragged) {
                mVelocityTracker->ComputeCurrentVelocity(1000, mMaximumVelocity);
                Int32 initialVelocity = (Int32)mVelocityTracker->GetYVelocity(mActivePointerId);

                if (GetChildCount() > 0) {
                    if ((Math::Abs(initialVelocity) > mMinimumVelocity)) {
                        Fling(-initialVelocity);
                    }
                    else {
                        Int32 right = GetScrollRange();
                        if (mScroller->SpringBack(mScrollX, mScrollY, 0, right, 0, 0)) {
                            Invalidate();
                        }
                    }
                }

                mActivePointerId = INVALID_POINTER;
                mIsBeingDragged = FALSE;

                if (mVelocityTracker != NULL) {
                    mVelocityTracker->Recycle();
                    mVelocityTracker = NULL;
                }
                if (mEdgeGlowLeft != NULL) {
                    mEdgeGlowLeft->OnRelease();
                    mEdgeGlowRight->OnRelease();
                }
            }
            break;
        case MotionEvent_ACTION_CANCEL:
            if (mIsBeingDragged && GetChildCount() > 0) {
                if (mScroller->SpringBack(mScrollX, mScrollY, 0, GetScrollRange(), 0, 0)) {
                    Invalidate();
                }
                mActivePointerId = INVALID_POINTER;
                mIsBeingDragged = FALSE;
                if (mVelocityTracker != NULL) {
                    mVelocityTracker->Recycle();
                    mVelocityTracker = NULL;
                }
                if (mEdgeGlowLeft != NULL) {
                    mEdgeGlowLeft->OnRelease();
                    mEdgeGlowRight->OnRelease();
                }
            }
            break;
        case MotionEvent_ACTION_POINTER_UP:
            OnSecondaryPointerUp(ev);
            break;
    }
    return TRUE;
}

void HorizontalScrollView::OnSecondaryPointerUp(
    /* [in] */ IMotionEvent* ev)
{
    Int32 action;
    ev->GetAction(&action);
    Int32 pointerIndex = (action & MotionEvent_ACTION_POINTER_INDEX_MASK) >>
            MotionEvent_ACTION_POINTER_INDEX_SHIFT;
    Int32 pointerId;
    ev->GetPointerId(pointerIndex, &pointerId);
    if (pointerId == mActivePointerId) {
        // This was our active pointer going up. Choose a new
        // active pointer and adjust accordingly.
        // TODO: Make this decision more intelligent.
        Int32 newPointerIndex = pointerIndex == 0 ? 1 : 0;
        ev->GetXEx(newPointerIndex, &mLastMotionX);
        ev->GetPointerId(newPointerIndex, &mActivePointerId);
        if (mVelocityTracker != NULL) {
            mVelocityTracker->Clear();
        }
    }
}

void HorizontalScrollView::OnOverScrolled(
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY,
    /* [in] */ Boolean clampedX,
    /* [in] */ Boolean clampedY)
{
    // Treat animating scrolls differently; see #computeScroll() for why.
    if (!mScroller->IsFinished()) {
        mScrollX = scrollX;
        mScrollY = scrollY;
        if (clampedY) {
            mScroller->SpringBack(mScrollX, mScrollY, 0, GetScrollRange(), 0, 0);
        }
    }
    else {
        FrameLayout::ScrollTo(scrollX, scrollY);
    }
    AwakenScrollBars();
}

Int32 HorizontalScrollView::GetScrollRange()
{
    Int32 scrollRange = 0;
    if (GetChildCount() > 0) {
        AutoPtr<IView> child = GetChildAt(0);
        Int32 childWidth;
        child->GetWidth(&childWidth);
        scrollRange = Math::Max(0,
                 childWidth - (GetWidth() - mPaddingLeft - mPaddingRight));
    }
    return scrollRange;
}

/**
 * <p>
 * Finds the next focusable component that fits in this View's bounds
 * (excluding fading edges) pretending that this View's left is located at
 * the parameter left.
 * </p>
 *
 * @param leftFocus          look for a candidate is the one at the left of the bounds
 *                           if leftFocus is true, or at the right of the bounds if leftFocus
 *                           is false
 * @param left               the left offset of the bounds in which a focusable must be
 *                           found (the fading edge is assumed to start at this position)
 * @param preferredFocusable the View that has highest priority and will be
 *                           returned if it is within my bounds (null is valid)
 * @return the next focusable component in the bounds or null if none can be found
 */
AutoPtr<IView> HorizontalScrollView::FindFocusableViewInMyBounds(
    /* [in] */ Boolean leftFocus,
    /* [in] */ Int32 left,
    /* [in] */ IView* preferredFocusable)
{
    /*
     * The fading edge's transparent side should be considered for focus
     * since it's mostly visible, so we divide the actual fading edge length
     * by 2.
     */
    Int32 fadingEdgeLength = GetHorizontalFadingEdgeLength() / 2;
    Int32 leftWithoutFadingEdge = left + fadingEdgeLength;
    Int32 rightWithoutFadingEdge = left + GetWidth() - fadingEdgeLength;

    if (preferredFocusable != NULL) {
        Int32 preferredLeft, preferredRight;
        preferredFocusable->GetLeft(&preferredLeft);
        preferredFocusable->GetRight(&preferredRight);
        if ((preferredLeft < rightWithoutFadingEdge)
                && (preferredRight > leftWithoutFadingEdge)) {
            return preferredFocusable;
        }
    }

    return FindFocusableViewInBounds(leftFocus, leftWithoutFadingEdge,
            rightWithoutFadingEdge);
}

/**
 * <p>
 * Finds the next focusable component that fits in the specified bounds.
 * </p>
 *
 * @param leftFocus look for a candidate is the one at the left of the bounds
 *                  if leftFocus is true, or at the right of the bounds if
 *                  leftFocus is false
 * @param left      the left offset of the bounds in which a focusable must be
 *                  found
 * @param right     the right offset of the bounds in which a focusable must
 *                  be found
 * @return the next focusable component in the bounds or null if none can
 *         be found
 */
AutoPtr<IView> HorizontalScrollView::FindFocusableViewInBounds(
    /* [in] */ Boolean leftFocus,
    /* [in] */ Int32 left,
    /* [in] */ Int32 right)
{
    AutoPtr<IObjectContainer> focusables;
    ASSERT_SUCCEEDED(GetFocusables(View::FOCUS_FORWARD, (IObjectContainer**)&focusables));

    AutoPtr<IView> focusCandidate;

    /*
     * A fully contained focusable is one where its top is below the bound's
     * top, and its bottom is above the bound's bottom. A partially
     * contained focusable is one where some part of it is within the
     * bounds, but it also has some part that is not within bounds.  A fully contained
     * focusable is preferred to a partially contained focusable.
     */
    Boolean foundFullyContainedFocusable = FALSE;

    AutoPtr<IObjectEnumerator> objEmu;
    ASSERT_SUCCEEDED(focusables->GetObjectEnumerator((IObjectEnumerator**)&objEmu));

    Boolean isSucceeded;
    objEmu->MoveNext(&isSucceeded);
    while (isSucceeded) {
        AutoPtr<IView> view;
        objEmu->Current((IInterface**)&view);
        Int32 viewLeft, viewRight;
        view->GetLeft(&viewLeft);
        view->GetRight(&viewRight);

        if (left < viewRight && viewLeft < right) {
            /*
             * the focusable is in the target area, it is a candidate for
             * focusing
             */

            Boolean viewIsFullyContained = (left < viewLeft) &&
                    (viewRight < right);

            if (focusCandidate == NULL) {
                /* No candidate, take this one */
                focusCandidate = view;
                foundFullyContainedFocusable = viewIsFullyContained;
            }
            else {
                Int32 l, r;
                focusCandidate->GetLeft(&l);
                focusCandidate->GetRight(&r);
                Boolean viewIsCloserToBoundary =
                        (leftFocus && viewLeft < l) || (!leftFocus && viewRight > r);

                if (foundFullyContainedFocusable) {
                    if (viewIsFullyContained && viewIsCloserToBoundary) {
                        /*
                         * We're dealing with only fully contained views, so
                         * it has to be closer to the boundary to beat our
                         * candidate
                         */
                        focusCandidate = view;
                    }
                }
                else {
                    if (viewIsFullyContained) {
                        /* Any fully contained view beats a partially contained view */
                        focusCandidate = view;
                        foundFullyContainedFocusable = TRUE;
                    }
                    else if (viewIsCloserToBoundary) {
                        /*
                         * Partially contained view beats another partially
                         * contained view if it's closer
                         */
                        focusCandidate = view;
                    }
                }
            }
        }
        objEmu->MoveNext(&isSucceeded);
    }

    return focusCandidate;
}

Boolean HorizontalScrollView::PageScroll(
    /* [in] */ Int32 direction)
{
    Boolean right = direction == View::FOCUS_RIGHT;
    Int32 width = GetWidth();

    if (right) {
        mTempRect->mLeft = GetScrollX() + width;
        Int32 count = GetChildCount();
        if (count > 0) {
            AutoPtr<IView> view = GetChildAt(0);
            Int32 right;
            view->GetRight(&right);
            if (mTempRect->mLeft + width > right) {
                mTempRect->mLeft = right - width;
            }
        }
    }
    else {
        mTempRect->mLeft = GetScrollX() - width;
        if (mTempRect->mLeft < 0) {
            mTempRect->mLeft = 0;
        }
    }
    mTempRect->mRight = mTempRect->mLeft + width;

    return ScrollAndFocus(direction,
            mTempRect->mLeft, mTempRect->mRight);
}

Boolean HorizontalScrollView::FullScroll(
    /* [in] */ Int32 direction)
{
    Boolean right = direction == View::FOCUS_RIGHT;
    Int32 width = GetWidth();

    mTempRect->mLeft = 0;
    mTempRect->mRight = width;

    if (right) {
        Int32 count = GetChildCount();
        if (count > 0) {
            AutoPtr<IView> view = GetChildAt(0);
            view->GetRight(&mTempRect->mRight);
            mTempRect->mLeft = mTempRect->mRight - width;
        }
    }

    return ScrollAndFocus(direction,
            mTempRect->mLeft, mTempRect->mRight);
}

Boolean HorizontalScrollView::ScrollAndFocus(
    /* [in] */ Int32 direction,
    /* [in] */ Int32 left,
    /* [in] */ Int32 right)
{
    Boolean handled = TRUE;

    Int32 width = GetWidth();
    Int32 containerLeft = GetScrollX();
    Int32 containerRight = containerLeft + width;
    Boolean goLeft = direction == View::FOCUS_LEFT;

    AutoPtr<IView> newFocused = FindFocusableViewInBounds(goLeft, left, right);
    if (newFocused.Get() == NULL) {
        newFocused = (IView*)this->Probe(EIID_IView);
    }

    if (left >= containerLeft && right <= containerRight) {
        handled = FALSE;
    }
    else {
        Int32 delta = goLeft ? (left - containerLeft) : (right - containerRight);
        DoScrollX(delta);
    }

    Boolean result;
    if (newFocused != FindFocus() && (newFocused->RequestFocusEx(direction, &result), result)) {
        mScrollViewMovedFocus = TRUE;
        mScrollViewMovedFocus = FALSE;
    }

    return handled;
}

Boolean HorizontalScrollView::ArrowScroll(
    /* [in] */ Int32 direction)
{
    AutoPtr<IView> currentFocused = FindFocus();
    if (currentFocused.Get() == (IView*)this->Probe(EIID_IView)) {
        currentFocused = NULL;
    }

    AutoPtr<IView> nextFocused = FocusFinder::GetInstance()->FindNextFocus(
                    (IViewGroup*)this->Probe(EIID_IViewGroup), currentFocused.Get(), direction);

    Int32 maxJump = GetMaxScrollAmount();

    if (nextFocused.Get() != NULL && IsWithinDeltaOfScreen(nextFocused, maxJump)) {
        nextFocused->GetDrawingRect(mTempRect);
        OffsetDescendantRectToMyCoords(nextFocused, mTempRect);
        Int32 scrollDelta = ComputeScrollDeltaToGetChildRectOnScreen(mTempRect);
        DoScrollX(scrollDelta);
        Boolean result;
        nextFocused->RequestFocusEx(direction, &result);
    }
    else {
        // no new focus
        Int32 scrollDelta = maxJump;

        if (direction == View::FOCUS_LEFT && GetScrollX() < scrollDelta) {
            scrollDelta = GetScrollX();
        }
        else if (direction == View::FOCUS_RIGHT) {
            if (GetChildCount() > 0) {
                Int32 daRight;
                GetChildAt(0)->GetRight(&daRight);

                Int32 screenRight = GetScrollX() + GetWidth();

                if (daRight - screenRight < maxJump) {
                    scrollDelta = daRight - screenRight;
                }
            }
        }
        if (scrollDelta == 0) {
            return FALSE;
        }
        DoScrollX(direction == View::FOCUS_RIGHT ? scrollDelta : -scrollDelta);
    }

    Boolean isFocused;
    if (currentFocused != NULL && (currentFocused->IsFocused(&isFocused), isFocused)
            && IsOffScreen(currentFocused)) {
        // previously focused item still has focus and is off screen, give
        // it up (take it back to ourselves)
        // (also, need to temporarily force FOCUS_BEFORE_DESCENDANTS so we are
        // sure to
        // get it)
        Int32 descendantFocusability = GetDescendantFocusability();  // save
        SetDescendantFocusability(ViewGroup::FOCUS_BEFORE_DESCENDANTS);
        View::RequestFocus();
        SetDescendantFocusability(descendantFocusability);  // restore
    }
    return TRUE;
}

/**
 * @return whether the descendant of this scroll view is scrolled off
 *  screen.
 */
Boolean HorizontalScrollView::IsOffScreen(
    /* [in] */ IView* descendant)
{
    return !IsWithinDeltaOfScreen(descendant, 0);
}

/**
 * @return whether the descendant of this scroll view is within delta
 *  pixels of being on the screen.
 */
Boolean HorizontalScrollView::IsWithinDeltaOfScreen(
    /* [in] */ IView* descendant,
    /* [in] */ Int32 delta)
{
    descendant->GetDrawingRect(mTempRect);
    OffsetDescendantRectToMyCoords(descendant, mTempRect);

    return (mTempRect->mRight + delta) >= GetScrollX()
            && (mTempRect->mLeft - delta) <= (GetScrollX() + GetWidth());
}

/**
 * Smooth scroll by a Y delta
 *
 * @param delta the number of pixels to scroll by on the Y axis
 */
void HorizontalScrollView::DoScrollX(
    /* [in] */ Int32 delta)
{
    if (delta != 0) {
        if (mSmoothScrollingEnabled) {
            SmoothScrollBy(delta, 0);
        }
        else {
            ScrollBy(delta, 0);
        }
    }
}

ECode HorizontalScrollView::SmoothScrollBy(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    if (GetChildCount() == 0) {
        // Nothing to do.
        return NOERROR;
    }
    Int64 duration = AnimationUtils::CurrentAnimationTimeMillis() - mLastScroll;
    if (duration > ANIMATED_SCROLL_GAP) {
        Int32 width = GetWidth() - mPaddingRight - mPaddingLeft;
        Int32 right;
        GetChildAt(0)->GetWidth(&right);
        Int32 maxX = Math::Max(0, right - width);
        Int32 scrollX = mScrollX;
        dx = Math::Max(0, Math::Min(scrollX + dx, maxX)) - scrollX;

        mScroller->StartScroll(scrollX, mScrollY, dx, 0);
        Invalidate();
    }
    else {
        if (!mScroller->IsFinished()) {
            mScroller->AbortAnimation();
        }
        ScrollBy(dx, dy);
    }
    mLastScroll = AnimationUtils::CurrentAnimationTimeMillis();
    return NOERROR;
}

ECode HorizontalScrollView::SmoothScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return SmoothScrollBy(x - mScrollX, y - mScrollY);
}

Int32 HorizontalScrollView::ComputeHorizontalScrollRange()
{
    Int32 count = GetChildCount();
    Int32 contentWidth = GetWidth() - mPaddingLeft - mPaddingRight;
    if (count == 0) {
        return contentWidth;
    }

    Int32 scrollRange;
    GetChildAt(0)->GetRight(&scrollRange);
    Int32 scrollX = mScrollX;
    Int32 overscrollRight = Math::Max(0, scrollRange - contentWidth);
    if (scrollX < 0) {
        scrollRange -= scrollX;
    }
    else if (scrollX > overscrollRight) {
        scrollRange += scrollX - overscrollRight;
    }

    return scrollRange;
}

Int32 HorizontalScrollView::ComputeHorizontalScrollOffset()
{
    return Math::Max(0, FrameLayout::ComputeHorizontalScrollOffset());
}

void HorizontalScrollView::MeasureChild(
    /* [in] */ IView* child,
    /* [in] */ Int32 parentWidthMeasureSpec,
    /* [in] */ Int32 parentHeightMeasureSpec)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    child->GetLayoutParams((IViewGroupLayoutParams**)&lp);

    Int32 childWidthMeasureSpec;
    Int32 childHeightMeasureSpec;

    Int32 height;
    lp->GetHeight(&height);
    childHeightMeasureSpec = GetChildMeasureSpec(parentHeightMeasureSpec,
            mPaddingTop + mPaddingBottom, height);

    childWidthMeasureSpec = MeasureSpec::MakeMeasureSpec(0, MeasureSpec::UNSPECIFIED);

    child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
}

void HorizontalScrollView::MeasureChildWithMargins(
    /* [in] */ IView* child,
    /* [in] */ Int32 parentWidthMeasureSpec,
    /* [in] */ Int32 widthUsed,
    /* [in] */ Int32 parentHeightMeasureSpec,
    /* [in] */ Int32 heightUsed)
{
    AutoPtr<IViewGroupMarginLayoutParams> params;
    child->GetLayoutParams((IViewGroupLayoutParams**)&params);
    CViewGroupMarginLayoutParams* lp = (CViewGroupMarginLayoutParams*)params.Get();

    Int32 childHeightMeasureSpec = GetChildMeasureSpec(parentHeightMeasureSpec,
            mPaddingTop + mPaddingBottom + lp->mTopMargin + lp->mBottomMargin
            + heightUsed, lp->mHeight);
    Int32 childWidthMeasureSpec = MeasureSpec::MakeMeasureSpec(
            lp->mLeftMargin + lp->mRightMargin, MeasureSpec::UNSPECIFIED);

    child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
}

ECode HorizontalScrollView::ComputeScroll()
{
    if (mScroller->ComputeScrollOffset()) {
        // This is called at drawing time by ViewGroup.  We don't want to
        // re-show the scrollbars at this point, which scrollTo will do,
        // so we replicate most of scrollTo here.
        //
        //         It's a little odd to call onScrollChanged from inside the drawing.
        //
        //         It is, except when you remember that computeScroll() is used to
        //         animate scrolling. So unless we want to defer the onScrollChanged()
        //         until the end of the animated scrolling, we don't really have a
        //         choice here.
        //
        //         I agree.  The alternative, which I think would be worse, is to post
        //         something and tell the subclasses later.  This is bad because there
        //         will be a window where mScrollX/Y is different from what the app
        //         thinks it is.
        //
        Int32 oldX = mScrollX;
        Int32 oldY = mScrollY;
        Int32 x = mScroller->GetCurrX();
        Int32 y = mScroller->GetCurrY();

        if (oldX != x || oldY != y) {
            OverScrollBy(x - oldX, y - oldY, oldX, oldY, GetScrollRange(), 0,
                    mOverflingDistance, 0, FALSE);
            OnScrollChanged(mScrollX, mScrollY, oldX, oldY);

            Int32 range = GetScrollRange();
            Int32 overscrollMode = GetOverScrollMode();
            if (overscrollMode == OVER_SCROLL_ALWAYS ||
                    (overscrollMode == OVER_SCROLL_IF_CONTENT_SCROLLS && range > 0)) {
                if (x < 0 && oldX >= 0) {
                    mEdgeGlowLeft->OnAbsorb((Int32) mScroller->GetCurrVelocity());
                }
                else if (x > range && oldX <= range) {
                    mEdgeGlowRight->OnAbsorb((Int32) mScroller->GetCurrVelocity());
                }
            }
        }
        AwakenScrollBars();

        // Keep on drawing until the animation has finished.
        PostInvalidate();
    }
    return NOERROR;
}

void HorizontalScrollView::ScrollToChild(
    /* [in] */ IView* child)
{
    child->GetDrawingRect(mTempRect);

    /* Offset from child's local coordinates to HorizontalScrollView coordinates */
    OffsetDescendantRectToMyCoords(child, mTempRect);

    Int32 scrollDelta = ComputeScrollDeltaToGetChildRectOnScreen(mTempRect);

    if (scrollDelta != 0) {
        ScrollBy(scrollDelta, 0);
    }
}

Boolean HorizontalScrollView::ScrollToChildRect(
    /* [in] */ IRect* rect,
    /* [in] */ Boolean immediate)
{
    Int32 delta = ComputeScrollDeltaToGetChildRectOnScreen(rect);
    Boolean scroll = delta != 0;
    if (scroll) {
        if (immediate) {
            ScrollBy(delta, 0);
        }
        else {
            SmoothScrollBy(delta, 0);
        }
    }
    return scroll;
}

/**
 * Compute the amount to scroll in the Y direction in order to get
 * a rectangle completely on the screen (or, if taller than the screen,
 * at least the first screen size chunk of it).
 *
 * @param rect The rect.
 * @return The scroll delta.
 */
Int32 HorizontalScrollView::ComputeScrollDeltaToGetChildRectOnScreen(
    /* [in] */ IRect* rect)
{
    CRect* _rect = (CRect*)rect;
    if (GetChildCount() == 0) return 0;

    Int32 width = GetWidth();
    Int32 screenLeft = GetScrollX();
    Int32 screenRight = screenLeft + width;

    Int32 fadingEdge = GetHorizontalFadingEdgeLength();

    // leave room for top fading edge as long as rect isn't at very top
    if (_rect->mLeft > 0) {
        screenLeft += fadingEdge;
    }

    // leave room for bottom fading edge as long as rect isn't at very bottom
    Int32 childWidth;
    GetChildAt(0)->GetWidth(&childWidth);
    if (_rect->mRight < childWidth) {
        screenRight -= fadingEdge;
    }

    Int32 scrollXDelta = 0;

    if (_rect->mRight > screenRight && _rect->mLeft > screenLeft) {
        // need to move down to get it in view: move down just enough so
        // that the entire rectangle is in view (or at least the first
        // screen size chunk).

        if (_rect->GetWidth() > width) {
            // just enough to get screen size chunk on
            scrollXDelta += (_rect->mLeft - screenLeft);
        }
        else {
            // get entire rect at bottom of screen
            scrollXDelta += (_rect->mRight - screenRight);
        }

        // make sure we aren't scrolling beyond the end of our content
        Int32 right;
        GetChildAt(0)->GetRight(&right);
        Int32 distanceToRight = right - screenRight;
        scrollXDelta = Math::Min(scrollXDelta, distanceToRight);

    }
    else if (_rect->mLeft < screenLeft && _rect->mRight < screenRight) {
        // need to move up to get it in view: move up just enough so that
        // entire rectangle is in view (or at least the first screen
        // size chunk of it).

        if (_rect->GetWidth() > width) {
            // screen size chunk
            scrollXDelta -= (screenRight - _rect->mRight);
        }
        else {
            // entire rect at top
            scrollXDelta -= (screenLeft - _rect->mLeft);
        }

        // make sure we aren't scrolling any further than the top our content
        scrollXDelta = Math::Max(scrollXDelta, -GetScrollX());
    }
    return scrollXDelta;
}

ECode HorizontalScrollView::RequestChildFocus(
    /* [in] */ IView* child,
    /* [in] */ IView* focused)
{
    if (!mScrollViewMovedFocus) {
        if (!mIsLayoutDirty) {
            ScrollToChild(focused);
        }
        else {
            // The child may not be laid out yet, we can't compute the scroll yet
            mChildToScrollTo = focused;
        }
    }
    return FrameLayout::RequestChildFocus(child, focused);
}

/**
 * When looking for focus in children of a scroll view, need to be a little
 * more careful not to give focus to something that is scrolled off screen.
 *
 * This is more expensive than the default {@link android.view.ViewGroup}
 * implementation, otherwise this behavior might have been made the default.
 */
Boolean HorizontalScrollView::OnRequestFocusInDescendants(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    // convert from forward / backward notation to up / down / left / right
    // (ugh).
    if (direction == View::FOCUS_FORWARD) {
        direction = View::FOCUS_RIGHT;
    }
    else if (direction == View::FOCUS_BACKWARD) {
        direction = View::FOCUS_LEFT;
    }

    AutoPtr<IView> nextFocus = previouslyFocusedRect == NULL ?
            FocusFinder::GetInstance()->FindNextFocus(
                    (IViewGroup*)this->Probe(EIID_IViewGroup), NULL, direction) :
            FocusFinder::GetInstance()->FindNextFocusFromRect(
                    (IViewGroup*)this->Probe(EIID_IViewGroup), previouslyFocusedRect, direction);

    if (nextFocus == NULL) {
        return FALSE;
    }

    if (IsOffScreen(nextFocus)) {
        return FALSE;
    }

    Boolean result;
    ASSERT_SUCCEEDED(
            nextFocus->RequestFocusEx2(direction, previouslyFocusedRect, &result));

    return result;
}

Boolean HorizontalScrollView::RequestChildRectangleOnScreen(
    /* [in] */ IView* child,
    /* [in] */ IRect* rectangle,
    /* [in] */ Boolean immediate)
{
    // offset into coordinate space of this scroll view
    Int32 left, scrollX, top, scrollY;
    child->GetLeft(&left);
    child->GetScrollX(&scrollX);
    child->GetTop(&top);
    child->GetScrollY(&scrollY);
    rectangle->Offset(left - scrollX, top - scrollY);

    return ScrollToChildRect(rectangle, immediate);
}

ECode HorizontalScrollView::RequestLayout()
{
    mIsLayoutDirty = TRUE;
    return FrameLayout::RequestLayout();
}

void HorizontalScrollView::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    FrameLayout::OnLayout(changed, left, top, right, bottom);
    mIsLayoutDirty = FALSE;
    // Give a child focus if it needs it
    if (mChildToScrollTo != NULL &&
            IsViewDescendantOf(mChildToScrollTo.Get(), (IView*)this->Probe(EIID_IView))) {
        ScrollToChild(mChildToScrollTo);
    }
    mChildToScrollTo = NULL;

    // Calling this with the present values causes it to re-clam them
    ScrollTo(mScrollX, mScrollY);
}

void HorizontalScrollView::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    FrameLayout::OnSizeChanged(w, h, oldw, oldh);

    AutoPtr<IView> currentFocused = FindFocus();
    if (NULL == currentFocused || (IView*)this->Probe(EIID_IView) == currentFocused.Get()) {
        return;
    }

    Int32 maxJump = mRight - mLeft;

    if (IsWithinDeltaOfScreen(currentFocused, maxJump)) {
        currentFocused->GetDrawingRect(mTempRect);
        OffsetDescendantRectToMyCoords(currentFocused, mTempRect);
        Int32 scrollDelta = ComputeScrollDeltaToGetChildRectOnScreen(mTempRect);
        DoScrollX(scrollDelta);
    }
}

/**
 * Return true if child is an descendant of parent, (or equal to the parent).
 */
Boolean HorizontalScrollView::IsViewDescendantOf(
    /* [in] */ IView* child,
    /* [in] */ IView* parent)
{
    if (child == parent) {
        return TRUE;
    }

    AutoPtr<IViewParent> theParent;
    child->GetParent((IViewParent**)&theParent);

    return (IViewGroup::Probe(theParent.Get()) != NULL) &&
            IsViewDescendantOf(IView::Probe(theParent.Get()), parent);
}

ECode HorizontalScrollView::Fling(
    /* [in] */ Int32 velocityX)
{
    if (GetChildCount() > 0) {
        Int32 width = GetWidth() - mPaddingRight - mPaddingLeft;
        Int32 right;
        GetChildAt(0)->GetWidth(&right);

        mScroller->Fling(mScrollX, mScrollY, velocityX, 0, 0,
                Math::Max(0, right - width), 0, 0, width / 2, 0);

        Boolean movingRight = velocityX > 0;

        AutoPtr<IView> newFocused =
                FindFocusableViewInMyBounds(movingRight, mScroller->GetFinalX(), FindFocus());

        if (newFocused == NULL) {
            newFocused = (IView*)this->Probe(EIID_IView);
        }

        Boolean result;
        if (newFocused != FindFocus() &&
                (newFocused->RequestFocusEx(movingRight ? View::FOCUS_RIGHT : View::FOCUS_LEFT, &result), result)) {
            mScrollViewMovedFocus = TRUE;
            mScrollViewMovedFocus = FALSE;
        }

        Invalidate();
    }
    return NOERROR;
}

ECode HorizontalScrollView::ScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    // we rely on the fact the View.scrollBy calls scrollTo.
    if (GetChildCount() > 0) {
        AutoPtr<IView> child = GetChildAt(0);
        Int32 width, height;
        child->GetWidth(&width);
        child->GetHeight(&height);
        x = Clamp(x, GetWidth() - mPaddingRight - mPaddingLeft, width);
        y = Clamp(y, GetHeight() - mPaddingBottom - mPaddingTop, height);
        if (x != mScrollX || y != mScrollY) {
            FAIL_RETURN(FrameLayout::ScrollTo(x, y));
        }
    }
    return NOERROR;
}

ECode HorizontalScrollView::SetOverScrollMode(
    /* [in] */ Int32 overScrollMode)
{
    if (overScrollMode != OVER_SCROLL_NEVER) {
        if (mEdgeGlowLeft == NULL) {
            AutoPtr<IResources> res;
            GetContext()->GetResources((IResources**)&res);

            AutoPtr<IDrawable> edge;
            //com.android.internal.R.drawable.overscroll_edge
            res->GetDrawable(0x01080238, (IDrawable**)&edge);
            AutoPtr<IDrawable> glow;
            //com.android.internal.R.drawable.overscroll_glow
            res->GetDrawable(0x01080239, (IDrawable**)&glow);
            mEdgeGlowLeft = new EdgeGlow(edge, glow);
            mEdgeGlowRight = new EdgeGlow(edge, glow);
        }
    }
    else {
        if (mEdgeGlowLeft != NULL) delete mEdgeGlowLeft;
        if (mEdgeGlowRight != NULL) delete mEdgeGlowRight;
        mEdgeGlowLeft = NULL;
        mEdgeGlowRight = NULL;
    }
    return FrameLayout::SetOverScrollMode(overScrollMode);
}

ECode HorizontalScrollView::Draw(
    /* [in] */ ICanvas* canvas)
{
    FAIL_RETURN(FrameLayout::Draw(canvas));

    if (mEdgeGlowLeft != NULL) {
        Int32 scrollX = mScrollX;
        if (!mEdgeGlowLeft->IsFinished()) {
            Int32 restoreCount;
            canvas->Save(&restoreCount);
            Int32 height = GetHeight();

            canvas->Rotate(270);
            canvas->Translate(-height * 1.5f, Math::Min(0, scrollX));
            mEdgeGlowLeft->SetSize(height * 2, GetWidth());
            if (mEdgeGlowLeft->Draw(canvas)) {
                Invalidate();
            }
            canvas->RestoreToCount(restoreCount);
        }
        if (!mEdgeGlowRight->IsFinished()) {
            Int32 restoreCount;
            canvas->Save(&restoreCount);
            Int32 width = GetWidth();
            Int32 height = GetHeight();

            canvas->Rotate(90);
            canvas->Translate(-height / 2, Math::Max(GetScrollRange(), scrollX) + width);
            mEdgeGlowRight->SetSize(height * 2, width);
            if (mEdgeGlowRight->Draw(canvas)) {
                Invalidate();
            }
            canvas->RestoreToCount(restoreCount);
        }
    }
    return NOERROR;
}

Int32 HorizontalScrollView::Clamp(
    /* [in] */ Int32 n,
    /* [in] */ Int32 my,
    /* [in] */ Int32 child)
{
    if (my >= child || n < 0) {
        /* my >= child is this case:
         *                    |--------------- me ---------------|
         *     |------ child ------|
         * or
         *     |--------------- me ---------------|
         *            |------ child ------|
         * or
         *     |--------------- me ---------------|
         *                                  |------ child ------|
         *
         * n < 0 is this case:
         *     |------ me ------|
         *                    |-------- child --------|
         *     |-- mScrollX --|
         */
        return 0;
    }
    if ((my + n) > child) {
        /* this case:
         *                    |------ me ------|
         *     |------ child ------|
         *     |-- mScrollX --|
         */
        return child - my;
    }
    return n;
}
