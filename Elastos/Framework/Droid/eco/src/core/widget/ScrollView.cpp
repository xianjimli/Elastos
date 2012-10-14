
#include "widget/ScrollView.h"
#include "widget/CFrameLayoutLayoutParams.h"
#include "view/FocusFinder.h"
#include "view/CViewGroupLayoutParams.h"
#include "view/CViewGroupMarginLayoutParams.h"
#include "view/animation/AnimationUtils.h"
#include <elastos/Math.h>

using namespace Elastos::Core;


const Int32 ScrollView::ANIMATED_SCROLL_GAP;
const Float ScrollView::MAX_SCROLL_FACTOR;
const Int32 ScrollView::INVALID_POINTER;

ScrollView::ScrollView()
    : mLastScroll(0)
    , mScroller(NULL)
    , mEdgeGlowTop(NULL)
    , mEdgeGlowBottom(NULL)
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

ScrollView::ScrollView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : FrameLayout(context, attrs, defStyle)
    , mLastScroll(0)
    , mScroller(NULL)
    , mEdgeGlowTop(NULL)
    , mEdgeGlowBottom(NULL)
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

ScrollView::~ScrollView()
{
    if (mScroller != NULL) delete mScroller;
    if (mEdgeGlowTop != NULL) delete mEdgeGlowTop;
    if (mEdgeGlowBottom != NULL) delete mEdgeGlowBottom;
}

ECode ScrollView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(FrameLayout::Init(context, attrs, defStyle));
    InitScrollView();
    return InitFromAttributes(context, attrs, defStyle);
}

static Int32 R_Styleable_ScrollView[] = {
    0x0101017a
};

ECode ScrollView::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ITypedArray> a;

    FAIL_RETURN(context->ObtainStyledAttributesEx3(attrs,
            ArrayOf<Int32>(R_Styleable_ScrollView, sizeof(R_Styleable_ScrollView) / sizeof(Int32))/*com.android.internal.R.styleable.ScrollView*/,
            defStyle, 0,
            (ITypedArray**)&a));

    Boolean value;
    a->GetBoolean(0/*R.styleable.ScrollView_fillViewport*/, FALSE, &value);

    FAIL_RETURN(SetFillViewport(value));

    a->Recycle();

    return NOERROR;
}

Float ScrollView::GetTopFadingEdgeStrength()
{
    if (GetChildCount() == 0) {
        return 0.0;
    }

    Int32 length = GetVerticalFadingEdgeLength();
    if (mScrollY < length) {
        return mScrollY / (Float) length;
    }

    return 1.0;
}

Float ScrollView::GetBottomFadingEdgeStrength()
{
    if (GetChildCount() == 0) {
        return 0.0;
    }

    Int32 length = GetVerticalFadingEdgeLength();
    Int32 bottomEdge = GetHeight() - mPaddingBottom;
    Int32 bottom;
    GetChildAt(0)->GetBottom(&bottom);
    Int32 span = bottom - mScrollY - bottomEdge;
    if (span < length) {
        return span / (Float)length;
    }

    return 1.0;
}

Int32 ScrollView::GetMaxScrollAmount()
{
    return (Int32)(MAX_SCROLL_FACTOR * (mBottom - mTop));
}

void ScrollView::InitScrollView()
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

ECode ScrollView::AddView(
    /* [in] */ IView* child)
{
    if (GetChildCount() > 0) {
//        throw new IllegalStateException("ScrollView can host only one direct child");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return FrameLayout::AddView(child);
}

ECode ScrollView::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index)
{
    if (GetChildCount() > 0) {
//        throw new IllegalStateException("ScrollView can host only one direct child");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return FrameLayout::AddView(child, index);
}

ECode ScrollView::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return ViewGroup::AddView(child, width, height);
}

ECode ScrollView::AddView(
    /* [in] */ IView* child,
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (GetChildCount() > 0) {
//        throw new IllegalStateException("ScrollView can host only one direct child");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return FrameLayout::AddView(child, params);
}

ECode ScrollView::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (GetChildCount() > 0) {
//        throw new IllegalStateException("ScrollView can host only one direct child");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return FrameLayout::AddView(child, index, params);
}

Boolean ScrollView::CanScroll()
{
    AutoPtr<IView> child = GetChildAt(0);
    if (child != NULL) {
        Int32 childHeight;
        child->GetHeight(&childHeight);
        return GetHeight() < childHeight + mPaddingTop + mPaddingBottom;
    }
    return FALSE;
}

Boolean ScrollView::IsFillViewport()
{
    return mFillViewport;
}

ECode ScrollView::SetFillViewport(
    /* [in] */ Boolean fillViewport)
{
    if (fillViewport != mFillViewport) {
        mFillViewport = fillViewport;
        RequestLayout();
    }

    return NOERROR;
}

Boolean ScrollView::IsSmoothScrollingEnabled()
{
    return mSmoothScrollingEnabled;
}

ECode ScrollView::SetSmoothScrollingEnabled(
    /* [in] */ Boolean smoothScrollingEnabled)
{
    mSmoothScrollingEnabled = smoothScrollingEnabled;

    return NOERROR;
}

void ScrollView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    FrameLayout::OnMeasure(widthMeasureSpec, heightMeasureSpec);

    if (!mFillViewport) {
        return;
    }

    Int32 heightMode = MeasureSpec::GetMode(heightMeasureSpec);
    if (heightMode == MeasureSpec::UNSPECIFIED) {
        return;
    }

    if (GetChildCount() > 0) {
        AutoPtr<IView> child = GetChildAt(0);
        Int32 height = FrameLayout::GetMeasuredHeight();
        Int32 childHeight;
        child->GetMeasuredHeight(&childHeight);
        if (childHeight < height) {
            AutoPtr<IFrameLayoutLayoutParams> params;
            child->GetLayoutParams((IViewGroupLayoutParams**)&params);

            Int32 childWidthMeasureSpec = GetChildMeasureSpec(widthMeasureSpec,
                    mPaddingLeft + mPaddingRight,
                    ((CFrameLayoutLayoutParams*)params.Get())->mWidth);
            height -= mPaddingTop;
            height -= mPaddingBottom;
            Int32 childHeightMeasureSpec =
                    MeasureSpec::MakeMeasureSpec(height, MeasureSpec::EXACTLY);

            child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
        }
    }
}

Boolean ScrollView::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    // Let the focused view and/or our descendants get the key first
    return FrameLayout::DispatchKeyEvent(event) || ExecuteKeyEvent(event);
}

Boolean ScrollView::ExecuteKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    mTempRect->SetEmpty();

    if (!CanScroll()) {
        Int32 keyCode;
        event->GetKeyCode(&keyCode);
        if (IsFocused() && keyCode != KeyEvent_KEYCODE_BACK) {
            AutoPtr<IView> currentFocused = FindFocus();
            if (currentFocused.Get() == (IView*)this->Probe(EIID_IView)) {
                currentFocused = NULL;
            }
            AutoPtr<IView> nextFocused = FocusFinder::GetInstance()->FindNextFocus(
                    (IViewGroup*)this->Probe(EIID_IViewGroup),
                    currentFocused, View::FOCUS_DOWN);

            if (nextFocused != NULL && nextFocused.Get() != (IView*)this->Probe(EIID_IView)) {
                Boolean isFocus;
                nextFocused->RequestFocusEx(View::FOCUS_DOWN, &isFocus);
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
            case KeyEvent_KEYCODE_DPAD_UP:
                event->IsAltPressed(&isPressed);
                if (!isPressed) {
                    handled = ArrowScroll(View::FOCUS_UP);
                }
                else {
                    handled = FullScroll(View::FOCUS_UP);
                }
                break;
            case KeyEvent_KEYCODE_DPAD_DOWN:
                event->IsAltPressed(&isPressed);
                if (!isPressed) {
                    handled = ArrowScroll(View::FOCUS_DOWN);
                }
                else {
                    handled = FullScroll(View::FOCUS_DOWN);
                }
                break;
            case KeyEvent_KEYCODE_SPACE:
                event->IsShiftPressed(&isPressed);
                PageScroll(isPressed ? View::FOCUS_UP : View::FOCUS_DOWN);
                break;
        }
    }

    return handled;
}

Boolean ScrollView::InChild(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (GetChildCount() > 0) {
        Int32 scrollY = mScrollY;
        AutoPtr<IView> child = GetChildAt(0);
        Int32 top, bottom, left, right;
        child->GetTop(&top);
        child->GetBottom(&bottom);
        child->GetLeft(&left);
        child->GetRight(&right);
        return !(y < top - scrollY
                || y >= bottom - scrollY
                || x < left
                || x >= right);
    }

    return FALSE;
}

Boolean ScrollView::OnInterceptTouchEvent(
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
            * Locally do absolute value. mLastMotionY is set to the y value
            * of the down event.
            */
            Int32 activePointerId = mActivePointerId;
            if (activePointerId == INVALID_POINTER) {
                // If we don't have a valid id, the touch down wasn't on content.
                break;
            }

            Int32 pointerIndex;
            ev->FindPointerIndex(activePointerId, &pointerIndex);
            Float y;
            ev->GetYEx(pointerIndex, &y);
            Int32 yDiff = (Int32)Math::Abs(y - mLastMotionY);
            if (yDiff > mTouchSlop) {
                mIsBeingDragged = TRUE;
                mLastMotionY = y;
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
            mLastMotionY = y;
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
            if (mScroller->SpringBack(mScrollX, mScrollY, 0, 0, 0, GetScrollRange())) {
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

Boolean ScrollView::OnTouchEvent(
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
            ev->GetY(&mLastMotionY);
            ev->GetPointerId(0, &mActivePointerId);
            break;
        }
        case MotionEvent_ACTION_MOVE:
            if (mIsBeingDragged) {
                // Scroll to follow the motion event
                Int32 activePointerIndex;
                ev->FindPointerIndex(mActivePointerId, &activePointerIndex);
                Float y;
                ev->GetYEx(activePointerIndex, &y);
                Int32 deltaY = (Int32)(mLastMotionY - y);
                mLastMotionY = y;

                Int32 oldX = mScrollX;
                Int32 oldY = mScrollY;
                Int32 range = GetScrollRange();
                if (OverScrollBy(0, deltaY, 0, mScrollY, 0, range,
                        0, mOverscrollDistance, TRUE)) {
                    // Break our velocity if we hit a scroll barrier.
                    mVelocityTracker->Clear();
                }
                OnScrollChanged(mScrollX, mScrollY, oldX, oldY);

                Int32 overscrollMode = GetOverScrollMode();
                if (overscrollMode == OVER_SCROLL_ALWAYS ||
                        (overscrollMode == OVER_SCROLL_IF_CONTENT_SCROLLS && range > 0)) {
                    Int32 pulledToY = oldY + deltaY;
                    if (pulledToY < 0) {
                        mEdgeGlowTop->OnPull((Float)deltaY / GetHeight());
                        if (!mEdgeGlowBottom->IsFinished()) {
                            mEdgeGlowBottom->OnRelease();
                        }
                    }
                    else if (pulledToY > range) {
                        mEdgeGlowBottom->OnPull((Float)deltaY / GetHeight());
                        if (!mEdgeGlowTop->IsFinished()) {
                            mEdgeGlowTop->OnRelease();
                        }
                    }
                    if (mEdgeGlowTop != NULL
                            && (!mEdgeGlowTop->IsFinished() || !mEdgeGlowBottom->IsFinished())) {
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
                        Int32 bottom = GetScrollRange();
                        if (mScroller->SpringBack(mScrollX, mScrollY, 0, 0, 0, bottom)) {
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
                if (mEdgeGlowTop != NULL) {
                    mEdgeGlowTop->OnRelease();
                    mEdgeGlowBottom->OnRelease();
                }
            }
            break;
        case MotionEvent_ACTION_CANCEL:
            if (mIsBeingDragged && GetChildCount() > 0) {
                if (mScroller->SpringBack(mScrollX, mScrollY, 0, 0, 0, GetScrollRange())) {
                    Invalidate();
                }
                mActivePointerId = INVALID_POINTER;
                mIsBeingDragged = FALSE;
                if (mVelocityTracker != NULL) {
                    mVelocityTracker->Recycle();
                    mVelocityTracker = NULL;
                }
                if (mEdgeGlowTop != NULL) {
                    mEdgeGlowTop->OnRelease();
                    mEdgeGlowBottom->OnRelease();
                }
            }
            break;
        case MotionEvent_ACTION_POINTER_UP:
            OnSecondaryPointerUp(ev);
            break;
    }

    return TRUE;
}

void ScrollView::OnSecondaryPointerUp(
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
        ev->GetYEx(newPointerIndex, &mLastMotionY);
        ev->GetPointerId(newPointerIndex, &mActivePointerId);
        if (mVelocityTracker != NULL) {
            mVelocityTracker->Clear();
        }
    }
}

void ScrollView::OnOverScrolled(
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
            mScroller->SpringBack(mScrollX, mScrollY, 0, 0, 0, GetScrollRange());
        }
    }
    else {
        FrameLayout::ScrollTo(scrollX, scrollY);
    }

    AwakenScrollBars();
}

Int32 ScrollView::GetScrollRange()
{
    Int32 scrollRange = 0;
    if (GetChildCount() > 0) {
        AutoPtr<IView> child = GetChildAt(0);
        Int32 childHeight;
        child->GetHeight(&childHeight);
        scrollRange = Math::Max(0,
                 childHeight- (GetHeight() - mPaddingBottom - mPaddingTop));
    }
    return scrollRange;
}

/**
 * <p>
 * Finds the next focusable component that fits in this View's bounds
 * (excluding fading edges) pretending that this View's top is located at
 * the parameter top.
 * </p>
 *
 * @param topFocus           look for a candidate is the one at the top of the bounds
 *                           if topFocus is true, or at the bottom of the bounds if topFocus is
 *                           false
 * @param top                the top offset of the bounds in which a focusable must be
 *                           found (the fading edge is assumed to start at this position)
 * @param preferredFocusable the View that has highest priority and will be
 *                           returned if it is within my bounds (null is valid)
 * @return the next focusable component in the bounds or null if none can be
 *         found
 */
AutoPtr<IView> ScrollView::FindFocusableViewInMyBounds(
    /* [in] */ Boolean topFocus,
    /* [in] */ Int32 top,
    /* [in] */ IView* preferredFocusable)
{
    /*
     * The fading edge's transparent side should be considered for focus
     * since it's mostly visible, so we divide the actual fading edge length
     * by 2.
     */
    Int32 fadingEdgeLength = GetVerticalFadingEdgeLength() / 2;
    Int32 topWithoutFadingEdge = top + fadingEdgeLength;
    Int32 bottomWithoutFadingEdge = top + GetHeight() - fadingEdgeLength;

    if (preferredFocusable != NULL) {
        Int32 preferredTop, preferredBottom;
        preferredFocusable->GetTop(&preferredTop);
        preferredFocusable->GetBottom(&preferredBottom);
        if ((preferredTop < bottomWithoutFadingEdge)
                && (preferredBottom > topWithoutFadingEdge)) {
            return preferredFocusable;
        }
    }

    return FindFocusableViewInBounds(topFocus, topWithoutFadingEdge,
            bottomWithoutFadingEdge);
}

/**
 * <p>
 * Finds the next focusable component that fits in the specified bounds.
 * </p>
 *
 * @param topFocus look for a candidate is the one at the top of the bounds
 *                 if topFocus is true, or at the bottom of the bounds if topFocus is
 *                 false
 * @param top      the top offset of the bounds in which a focusable must be
 *                 found
 * @param bottom   the bottom offset of the bounds in which a focusable must
 *                 be found
 * @return the next focusable component in the bounds or null if none can
 *         be found
 */
AutoPtr<IView> ScrollView::FindFocusableViewInBounds(
    /* [in] */ Boolean topFocus,
    /* [in] */ Int32 top,
    /* [in] */ Int32 bottom)
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

        Int32 viewTop, viewBottom;
        view->GetTop(&viewTop);
        view->GetBottom(&viewBottom);

        if (top < viewBottom && viewTop < bottom) {
            /*
             * the focusable is in the target area, it is a candidate for
             * focusing
             */

            Boolean viewIsFullyContained = (top < viewTop) &&
                    (viewBottom < bottom);

            if (focusCandidate == NULL) {
                /* No candidate, take this one */
                focusCandidate = view;
                foundFullyContainedFocusable = viewIsFullyContained;
            }
            else {
                Int32 t, b;
                focusCandidate->GetTop(&t);
                focusCandidate->GetBottom(&b);
                Boolean viewIsCloserToBoundary =
                        (topFocus && viewTop < t) || (!topFocus && viewBottom > b);

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

Boolean ScrollView::PageScroll(
    /* [in] */ Int32 direction)
{
    Boolean down = direction == View::FOCUS_DOWN;
    Int32 height = GetHeight();

    if (down) {
        mTempRect->mTop = GetScrollY() + height;
        Int32 count = GetChildCount();
        if (count > 0) {
            AutoPtr<IView> view = GetChildAt(count - 1);
            Int32 bottom;
            view->GetBottom(&bottom);
            if (mTempRect->mTop + height > bottom) {
                mTempRect->mTop = bottom - height;
            }
        }
    }
    else {
        mTempRect->mTop = GetScrollY() - height;
        if (mTempRect->mTop < 0) {
            mTempRect->mTop = 0;
        }
    }
    mTempRect->mBottom = mTempRect->mTop + height;

    return ScrollAndFocus(direction,
            mTempRect->mTop, mTempRect->mBottom);
}

Boolean ScrollView::FullScroll(
    /* [in] */ Int32 direction)
{
    Boolean down = direction == View::FOCUS_DOWN;
    Int32 height = GetHeight();

    mTempRect->mTop = 0;
    mTempRect->mBottom = height;

    if (down) {
        Int32 count = GetChildCount();
        if (count > 0) {
            AutoPtr<IView> view = GetChildAt(count - 1);
            view->GetBottom(&mTempRect->mBottom);
            mTempRect->mTop = mTempRect->mBottom - height;
        }
    }

    return ScrollAndFocus(direction,
            mTempRect->mTop, mTempRect->mBottom);
}

Boolean ScrollView::ScrollAndFocus(
    /* [in] */ Int32 direction,
    /* [in] */ Int32 top,
    /* [in] */ Int32 bottom)
{
    Boolean handled = TRUE;

    Int32 height = GetHeight();
    Int32 containerTop = GetScrollY();
    Int32 containerBottom = containerTop + height;
    Boolean up = direction == View::FOCUS_UP;

    AutoPtr<IView> newFocused = FindFocusableViewInBounds(up, top, bottom);
    if (newFocused == NULL) {
        newFocused = (IView*)this->Probe(EIID_IView);
    }

    if (top >= containerTop && bottom <= containerBottom) {
        handled = FALSE;
    }
    else {
        Int32 delta = up ? (top - containerTop) : (bottom - containerBottom);
        DoScrollY(delta);
    }

    Boolean result;
    if (newFocused != FindFocus() &&
            (newFocused->RequestFocusEx(direction, &result), result)) {
        mScrollViewMovedFocus = TRUE;
        mScrollViewMovedFocus = FALSE;
    }

    return handled;
}

Boolean ScrollView::ArrowScroll(
    /* [in] */ Int32 direction)
{
    AutoPtr<IView> currentFocused = FindFocus();
    if (currentFocused.Get() == (IView*)this->Probe(EIID_IView)) {
        currentFocused = NULL;
    }

    AutoPtr<IView> nextFocused = FocusFinder::GetInstance()->FindNextFocus(
            (IViewGroup*)this->Probe(EIID_IViewGroup), currentFocused, direction);

    Int32 maxJump = GetMaxScrollAmount();

    if (nextFocused != NULL && IsWithinDeltaOfScreen(nextFocused, maxJump, GetHeight())) {
        nextFocused->GetDrawingRect(mTempRect);
        OffsetDescendantRectToMyCoords(nextFocused, mTempRect);
        Int32 scrollDelta = ComputeScrollDeltaToGetChildRectOnScreen(mTempRect);
        DoScrollY(scrollDelta);
        Boolean result;
        nextFocused->RequestFocusEx(direction, &result);
    }
    else {
        // no new focus
        Int32 scrollDelta = maxJump;

        if (direction == View::FOCUS_UP && GetScrollY() < scrollDelta) {
            scrollDelta = GetScrollY();
        }
        else if (direction == View::FOCUS_DOWN) {
            if (GetChildCount() > 0) {
                Int32 daBottom;
                GetChildAt(0)->GetBottom(&daBottom);

                Int32 screenBottom = GetScrollY() + GetHeight();

                if (daBottom - screenBottom < maxJump) {
                    scrollDelta = daBottom - screenBottom;
                }
            }
        }
        if (scrollDelta == 0) {
            return FALSE;
        }
        DoScrollY(direction == View::FOCUS_DOWN ? scrollDelta : -scrollDelta);
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
Boolean ScrollView::IsOffScreen(
    /* [in] */ IView* descendant)
{
    return !IsWithinDeltaOfScreen(descendant, 0, GetHeight());
}

/**
 * @return whether the descendant of this scroll view is within delta
 *  pixels of being on the screen.
 */
Boolean ScrollView::IsWithinDeltaOfScreen(
    /* [in] */ IView* descendant,
    /* [in] */ Int32 delta,
    /* [in] */ Int32 height)
{
    descendant->GetDrawingRect(mTempRect);
    OffsetDescendantRectToMyCoords(descendant, mTempRect);

    return (mTempRect->mBottom + delta) >= GetScrollY()
            && (mTempRect->mTop - delta) <= (GetScrollY() + height);
}

/**
 * Smooth scroll by a Y delta
 *
 * @param delta the number of pixels to scroll by on the Y axis
 */
void ScrollView::DoScrollY(
    /* [in] */ Int32 delta)
{
    if (delta != 0) {
        if (mSmoothScrollingEnabled) {
            SmoothScrollBy(0, delta);
        }
        else {
            ScrollBy(0, delta);
        }
    }
}

ECode ScrollView::SmoothScrollBy(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    if (GetChildCount() == 0) {
        // Nothing to do.
        return NOERROR;
    }
    Int64 duration = AnimationUtils::CurrentAnimationTimeMillis() - mLastScroll;
    if (duration > ANIMATED_SCROLL_GAP) {
        Int32 height = GetHeight() - mPaddingBottom - mPaddingTop;
        Int32 bottom;
        GetChildAt(0)->GetHeight(&bottom);
        Int32 maxY = Math::Max(0, bottom - height);
        Int32 scrollY = mScrollY;
        dy = Math::Max(0, Math::Min(scrollY + dy, maxY)) - scrollY;

        mScroller->StartScroll(mScrollX, scrollY, 0, dy);
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

ECode ScrollView::SmoothScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return SmoothScrollBy(x - mScrollX, y - mScrollY);
}

Int32 ScrollView::ComputeVerticalScrollRange()
{
    Int32 count = GetChildCount();
    Int32 contentHeight = GetHeight() - mPaddingBottom - mPaddingTop;
    if (count == 0) {
        return contentHeight;
    }

    Int32 scrollRange;
    GetChildAt(0)->GetBottom(&scrollRange);
    Int32 scrollY = mScrollY;
    Int32 overscrollBottom = Math::Max(0, scrollRange - contentHeight);
    if (scrollY < 0) {
        scrollRange -= scrollY;
    }
    else if (scrollY > overscrollBottom) {
        scrollRange += scrollY - overscrollBottom;
    }

    return scrollRange;
}

Int32 ScrollView::ComputeVerticalScrollOffset()
{
    return Math::Max(0, FrameLayout::ComputeVerticalScrollOffset());
}

void ScrollView::MeasureChild(
    /* [in] */ IView* child,
    /* [in] */ Int32 parentWidthMeasureSpec,
    /* [in] */ Int32 parentHeightMeasureSpec)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    child->GetLayoutParams((IViewGroupLayoutParams**)&lp);

    Int32 childWidthMeasureSpec;
    Int32 childHeightMeasureSpec;

    Int32 width;
    lp->GetWidth(&width);
    childWidthMeasureSpec = GetChildMeasureSpec(parentWidthMeasureSpec,
            mPaddingLeft + mPaddingRight, width);

    childHeightMeasureSpec = MeasureSpec::MakeMeasureSpec(0, MeasureSpec::UNSPECIFIED);

    child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
}

void ScrollView::MeasureChildWithMargins(
    /* [in] */ IView* child,
    /* [in] */ Int32 parentWidthMeasureSpec,
    /* [in] */ Int32 widthUsed,
    /* [in] */ Int32 parentHeightMeasureSpec,
    /* [in] */ Int32 heightUsed)
{
    AutoPtr<IViewGroupMarginLayoutParams> params;
    child->GetLayoutParams((IViewGroupLayoutParams**)&params);
    CViewGroupMarginLayoutParams* lp = (CViewGroupMarginLayoutParams*)params.Get();

    Int32 childWidthMeasureSpec = GetChildMeasureSpec(parentWidthMeasureSpec,
            mPaddingLeft + mPaddingRight + lp->mLeftMargin + lp->mRightMargin
            + widthUsed, lp->mWidth);
    Int32 childHeightMeasureSpec = MeasureSpec::MakeMeasureSpec(
            lp->mTopMargin + lp->mBottomMargin, MeasureSpec::UNSPECIFIED);

    child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
}

ECode ScrollView::ComputeScroll()
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
            OverScrollBy(x - oldX, y - oldY, oldX, oldY, 0, GetScrollRange(),
                    0, mOverflingDistance, FALSE);
            OnScrollChanged(mScrollX, mScrollY, oldX, oldY);

            Int32 range = GetScrollRange();
            Int32 overscrollMode = GetOverScrollMode();
            if (overscrollMode == OVER_SCROLL_ALWAYS ||
                    (overscrollMode == OVER_SCROLL_IF_CONTENT_SCROLLS && range > 0)) {
                if (y < 0 && oldY >= 0) {
                    mEdgeGlowTop->OnAbsorb((Int32)mScroller->GetCurrVelocity());
                }
                else if (y > range && oldY <= range) {
                    mEdgeGlowBottom->OnAbsorb((Int32)mScroller->GetCurrVelocity());
                }
            }
        }
        AwakenScrollBars();

        // Keep on drawing until the animation has finished.
        PostInvalidate();
    }

    return NOERROR;
}

void ScrollView::ScrollToChild(
    /* [in] */ IView* child)
{
    child->GetDrawingRect(mTempRect);

    /* Offset from child's local coordinates to ScrollView coordinates */
    OffsetDescendantRectToMyCoords(child, mTempRect);

    Int32 scrollDelta = ComputeScrollDeltaToGetChildRectOnScreen(mTempRect);

    if (scrollDelta != 0) {
        ScrollBy(0, scrollDelta);
    }
}

Boolean ScrollView::ScrollToChildRect(
    /* [in] */ IRect* rect,
    /* [in] */ Boolean immediate)
{
    Int32 delta = ComputeScrollDeltaToGetChildRectOnScreen(rect);
    Boolean scroll = delta != 0;
    if (scroll) {
        if (immediate) {
            ScrollBy(0, delta);
        }
        else {
            SmoothScrollBy(0, delta);
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
Int32 ScrollView::ComputeScrollDeltaToGetChildRectOnScreen(
    /* [in] */ IRect* rect)
{
    CRect* _rect = (CRect*)rect;

    if (GetChildCount() == 0) return 0;

    Int32 height = GetHeight();
    Int32 screenTop = GetScrollY();
    Int32 screenBottom = screenTop + height;

    Int32 fadingEdge = GetVerticalFadingEdgeLength();

    // leave room for top fading edge as long as rect isn't at very top
    if (_rect->mTop > 0) {
        screenTop += fadingEdge;
    }

    // leave room for bottom fading edge as long as rect isn't at very bottom
    Int32 childHeight;
    GetChildAt(0)->GetHeight(&childHeight);
    if (_rect->mBottom < childHeight) {
        screenBottom -= fadingEdge;
    }

    Int32 scrollYDelta = 0;

    if (_rect->mBottom > screenBottom && _rect->mTop > screenTop) {
        // need to move down to get it in view: move down just enough so
        // that the entire rectangle is in view (or at least the first
        // screen size chunk).

        if (_rect->GetHeight() > height) {
            // just enough to get screen size chunk on
            scrollYDelta += (_rect->mTop - screenTop);
        }
        else {
            // get entire rect at bottom of screen
            scrollYDelta += (_rect->mBottom - screenBottom);
        }

        // make sure we aren't scrolling beyond the end of our content
        Int32 bottom;
        GetChildAt(0)->GetBottom(&bottom);
        Int32 distanceToBottom = bottom - screenBottom;
        scrollYDelta = Math::Min(scrollYDelta, distanceToBottom);

    }
    else if (_rect->mTop < screenTop && _rect->mBottom < screenBottom) {
        // need to move up to get it in view: move up just enough so that
        // entire rectangle is in view (or at least the first screen
        // size chunk of it).

        if (_rect->GetHeight() > height) {
            // screen size chunk
            scrollYDelta -= (screenBottom - _rect->mBottom);
        }
        else {
            // entire rect at top
            scrollYDelta -= (screenTop - _rect->mTop);
        }

        // make sure we aren't scrolling any further than the top our content
        scrollYDelta = Math::Max(scrollYDelta, -GetScrollY());
    }
    return scrollYDelta;
}

ECode ScrollView::RequestChildFocus(
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
Boolean ScrollView::OnRequestFocusInDescendants(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    // convert from forward / backward notation to up / down / left / right
    // (ugh).
    if (direction == View::FOCUS_FORWARD) {
        direction = View::FOCUS_DOWN;
    }
    else if (direction == View::FOCUS_BACKWARD) {
        direction = View::FOCUS_UP;
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

Boolean ScrollView::RequestChildRectangleOnScreen(
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

ECode ScrollView::RequestLayout()
{
    mIsLayoutDirty = TRUE;
    return FrameLayout::RequestLayout();
}

void ScrollView::OnLayout(
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
            IsViewDescendantOf(mChildToScrollTo, (IView*)this->Probe(EIID_IView))) {
        ScrollToChild(mChildToScrollTo);
    }
    mChildToScrollTo = NULL;

    // Calling this with the present values causes it to re-clam them
    ScrollTo(mScrollX, mScrollY);
}

void ScrollView::OnSizeChanged(
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

    // If the currently-focused view was visible on the screen when the
    // screen was at the old height, then scroll the screen to make that
    // view visible with the new screen height.
    if (IsWithinDeltaOfScreen(currentFocused, 0, oldh)) {
        currentFocused->GetDrawingRect(mTempRect);
        OffsetDescendantRectToMyCoords(currentFocused, mTempRect);
        Int32 scrollDelta = ComputeScrollDeltaToGetChildRectOnScreen(mTempRect);
        DoScrollY(scrollDelta);
    }
}

/**
 * Return true if child is an descendant of parent, (or equal to the parent).
 */
Boolean ScrollView::IsViewDescendantOf(
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

ECode ScrollView::Fling(
    /* [in] */ Int32 velocityY)
{
    if (GetChildCount() > 0) {
        Int32 height = GetHeight() - mPaddingBottom - mPaddingTop;
        Int32 bottom;
        GetChildAt(0)->GetHeight(&bottom);

        mScroller->Fling(mScrollX, mScrollY, 0, velocityY, 0, 0, 0,
                Math::Max(0, bottom - height), 0, height / 2);

        Boolean movingDown = velocityY > 0;

        AutoPtr<IView> newFocused =
                FindFocusableViewInMyBounds(movingDown, mScroller->GetFinalY(), FindFocus());

        if (newFocused == NULL) {
            newFocused = (IView*)this->Probe(EIID_IView);
        }

        Boolean result;
        if (newFocused != FindFocus() &&
                (newFocused->RequestFocusEx(movingDown ? View::FOCUS_DOWN : View::FOCUS_UP, &result), result)) {
            mScrollViewMovedFocus = TRUE;
            mScrollViewMovedFocus = FALSE;
        }

        Invalidate();
    }
    return NOERROR;
}

ECode ScrollView::ScrollTo(
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

ECode ScrollView::SetOverScrollMode(
    /* [in] */ Int32 overScrollMode)
{
    if (overScrollMode != OVER_SCROLL_NEVER) {
        if (mEdgeGlowTop == NULL) {
            AutoPtr<IResources> res;
            GetContext()->GetResources((IResources**)&res);

            AutoPtr<IDrawable> edge;
            //com.android.internal.R.drawable.overscroll_edge
            res->GetDrawable(0x01080238, (IDrawable**)&edge);
            AutoPtr<IDrawable> glow;
            //com.android.internal.R.drawable.overscroll_glow
            res->GetDrawable(0x01080239, (IDrawable**)&glow);
            mEdgeGlowTop = new EdgeGlow(edge, glow);
            mEdgeGlowBottom = new EdgeGlow(edge, glow);
        }
    }
    else {
        if (mEdgeGlowTop != NULL) delete mEdgeGlowTop;
        if (mEdgeGlowBottom != NULL) delete mEdgeGlowBottom;
        mEdgeGlowTop = NULL;
        mEdgeGlowBottom = NULL;
    }
    return FrameLayout::SetOverScrollMode(overScrollMode);
}

ECode ScrollView::Draw(
    /* [in] */ ICanvas* canvas)
{
    FAIL_RETURN(FrameLayout::Draw(canvas));
    if (mEdgeGlowTop != NULL) {
        Int32 scrollY = mScrollY;
        if (!mEdgeGlowTop->IsFinished()) {
            Int32 restoreCount;
            canvas->Save(&restoreCount);
            Int32 width = GetWidth();

            canvas->Translate(-width / 2, Math::Min(0, scrollY));
            mEdgeGlowTop->SetSize(width * 2, GetHeight());
            if (mEdgeGlowTop->Draw(canvas)) {
                Invalidate();
            }
            canvas->RestoreToCount(restoreCount);
        }
        if (!mEdgeGlowBottom->IsFinished()) {
            Int32 restoreCount;
            canvas->Save(&restoreCount);
            Int32 width = GetWidth();
            Int32 height = GetHeight();

            canvas->Translate(-width / 2, Math::Max(GetScrollRange(), scrollY) + height);
            canvas->RotateEx(180, width, 0);
            mEdgeGlowBottom->SetSize(width * 2, height);
            if (mEdgeGlowBottom->Draw(canvas)) {
                Invalidate();
            }
            canvas->RestoreToCount(restoreCount);
        }
    }
    return NOERROR;
}

Int32 ScrollView::Clamp(
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
