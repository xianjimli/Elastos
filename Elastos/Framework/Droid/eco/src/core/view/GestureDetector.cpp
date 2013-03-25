
#include "view/GestureDetector.h"
#include "view/ViewConfiguration.h"
#include "view/CMotionEvent.h"
#include "os/CApartment.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

const Int32 GestureDetector::LONGPRESS_TIMEOUT = ViewConfiguration::GetLongPressTimeout();
const Int32 GestureDetector::TAP_TIMEOUT = ViewConfiguration::GetTapTimeout();
const Int32 GestureDetector::DOUBLE_TAP_TIMEOUT = ViewConfiguration::GetDoubleTapTimeout();

// constants for Message.what used by GestureHandler below
const Int32 GestureDetector::SHOW_PRESS;
const Int32 GestureDetector::LONG_PRESS;
const Int32 GestureDetector::TAP;

Boolean GestureDetector::SimpleOnGestureListener::OnSingleTapUp(
    /* [in] */ IMotionEvent* e)
{
    return FALSE;
}

void GestureDetector::SimpleOnGestureListener::OnLongPress(
    /* [in] */ IMotionEvent* e)
{
}

Boolean GestureDetector::SimpleOnGestureListener::OnScroll(
    /* [in] */ IMotionEvent* e1,
    /* [in] */ IMotionEvent* e2,
    /* [in] */ Float distanceX,
    /* [in] */ Float distanceY)
{
    return FALSE;
}

Boolean GestureDetector::SimpleOnGestureListener::OnFling(
    /* [in] */ IMotionEvent* e1,
    /* [in] */ IMotionEvent* e2,
    /* [in] */ Float velocityX,
    /* [in] */ Float velocityY)
{
    return FALSE;
}

void GestureDetector::SimpleOnGestureListener::OnShowPress(
    /* [in] */ IMotionEvent* e)
{
}

Boolean GestureDetector::SimpleOnGestureListener::OnDown(
    /* [in] */ IMotionEvent* e)
{
    return FALSE;
}

Boolean GestureDetector::SimpleOnGestureListener::OnDoubleTap(
    /* [in] */ IMotionEvent* e)
{
    return FALSE;
}

Boolean GestureDetector::SimpleOnGestureListener::OnDoubleTapEvent(
    /* [in] */ IMotionEvent* e)
{
    return FALSE;
}

Boolean GestureDetector::SimpleOnGestureListener::OnSingleTapConfirmed(
    /* [in] */ IMotionEvent* e)
{
    return FALSE;
}

GestureDetector::OnDoubleTapListener* GestureDetector::SimpleOnGestureListener::ProbeToOnDoubleTapEvent()
{
    return (OnDoubleTapListener*)this;
}

/**
 * Creates a GestureDetector with the supplied listener.
 * You may only use this constructor from a UI thread (this is the usual situation).
 * @see android.os.Handler#Handler()
 *
 * @param context the application's context
 * @param listener the listener invoked for all the callbacks, this must
 * not be NULL.
 * @param handler the handler to use
 *
 * @throws NullPointerException if {@code listener} is NULL.
 */
GestureDetector::GestureDetector(
    /* [in] */ IContext* context,
    /* [in] */ OnGestureListener* listener,
    /* [in] */ IApartment* handler)
    : mBiggerTouchSlopSquare(20 * 20)
    , mListener(NULL)
    , mDoubleTapListener(NULL)
    , mStillDown(FALSE)
    , mInLongPress(FALSE)
    , mAlwaysInTapRegion(FALSE)
    , mAlwaysInBiggerTapRegion(FALSE)
    , mIsDoubleTapping(FALSE)
    , mIsLongpressEnabled(FALSE)
    , mIgnoreMultitouch(FALSE)
{
    Boolean ignoreMultitouch = TRUE;//FALSE;
    //if (context != NULL) {
    //    AutoPtr<IApplicationInfo> info;
    //    context->GetApplicationInfo((IApplicationInfo**)&info);
    //    ignoreMultitouch =
    //        ((CApplicationInfo*)info.Get())->mTargetSdkVersion >= Build::VERSION_CODES::FROYO;
    //}
    Init(context, listener, handler, ignoreMultitouch);
}

/**
 * Creates a GestureDetector with the supplied listener.
 * You may only use this constructor from a UI thread (this is the usual situation).
 * @see android.os.Handler#Handler()
 *
 * @param context the application's context
 * @param listener the listener invoked for all the callbacks, this must
 * not be NULL.
 * @param handler the handler to use
 * @param ignoreMultitouch whether events involving more than one pointer should
 * be ignored.
 *
 * @throws NullPointerException if {@code listener} is NULL.
 */
GestureDetector::GestureDetector(
    /* [in] */ IContext* context,
    /* [in] */ OnGestureListener* listener,
    /* [in] */ IApartment* handler,
    /* [in] */ Boolean ignoreMultitouch)
    : mBiggerTouchSlopSquare(20 * 20)
    , mListener(NULL)
    , mDoubleTapListener(NULL)
    , mStillDown(FALSE)
    , mInLongPress(FALSE)
    , mAlwaysInTapRegion(FALSE)
    , mAlwaysInBiggerTapRegion(FALSE)
    , mIsDoubleTapping(FALSE)
    , mIsLongpressEnabled(FALSE)
    , mIgnoreMultitouch(FALSE)
{
    Init(context, listener, handler, ignoreMultitouch);
}

void GestureDetector::Init(
    /* [in] */ IContext* context,
    /* [in] */ OnGestureListener* listener,
    /* [in] */ IApartment* handler,
    /* [in] */ Boolean ignoreMultitouch)
{
    if (handler == NULL) {
        ASSERT_SUCCEEDED(CApartment::GetMainApartment((IApartment**)&mApartment));
    }

    mListener = listener;
    if (listener->ProbeToOnDoubleTapEvent()) {
        SetOnDoubleTapListener(listener->ProbeToOnDoubleTapEvent());
    }
    Init2(context, ignoreMultitouch);
}

void GestureDetector::Init2(
    /* [in] */ IContext* context,
    /* [in] */ Boolean ignoreMultitouch)
{
    assert(mListener);

    mIsLongpressEnabled = TRUE;
    mIgnoreMultitouch = ignoreMultitouch;

    // Fallback to support pre-donuts releases
    Int32 touchSlop, doubleTapSlop;
    if (context == NULL) {
        //noinspection deprecation
        touchSlop = ViewConfiguration::GetTouchSlop();
        doubleTapSlop = ViewConfiguration::GetDoubleTapSlop();
        //noinspection deprecation
        mMinimumFlingVelocity = ViewConfiguration::GetMinimumFlingVelocity();
        mMaximumFlingVelocity = ViewConfiguration::GetMaximumFlingVelocity();
    }
    else {
        AutoPtr<ViewConfiguration> configuration = ViewConfiguration::Get(context);
        touchSlop = configuration->GetScaledTouchSlop();
        doubleTapSlop = configuration->GetScaledDoubleTapSlop();
        mMinimumFlingVelocity = configuration->GetScaledMinimumFlingVelocity();
        mMaximumFlingVelocity = configuration->GetScaledMaximumFlingVelocity();
    }
    mTouchSlopSquare = touchSlop * touchSlop;
    mDoubleTapSlopSquare = doubleTapSlop * doubleTapSlop;
}

/**
 * Sets the listener which will be called for double-tap and related
 * gestures.
 *
 * @param onDoubleTapListener the listener invoked for all the callbacks, or
 *        NULL to stop listening for double-tap gestures.
 */
void GestureDetector::SetOnDoubleTapListener(
    /* [in] */ OnDoubleTapListener* onDoubleTapListener)
{
    mDoubleTapListener = onDoubleTapListener;
}

/**
 * Set whether longpress is enabled, if this is enabled when a user
 * presses and holds down you get a longpress event and nothing further.
 * If it's disabled the user can press and hold down and then later
 * moved their finger and you will get scroll events. By default
 * longpress is enabled.
 *
 * @param isLongpressEnabled whether longpress should be enabled.
 */
void GestureDetector::SetIsLongpressEnabled(
    /* [in] */ Boolean isLongpressEnabled)
{
    mIsLongpressEnabled = isLongpressEnabled;
}

/**
 * @return TRUE if longpress is enabled, else FALSE.
 */
Boolean GestureDetector::IsLongpressEnabled()
{
    return mIsLongpressEnabled;
}

/**
 * Analyzes the given motion event and if applicable triggers the
 * appropriate callbacks on the {@link OnGestureListener} supplied.
 *
 * @param ev The current motion event.
 * @return TRUE if the {@link OnGestureListener} consumed the event,
 *              else FALSE.
 */
Boolean GestureDetector::OnTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    Int32 action;
    ev->GetAction(&action);
    Float y;
    ev->GetY(&y);
    Float x;
    ev->GetX(&x);

    if (mVelocityTracker == NULL) {
        mVelocityTracker = VelocityTracker::Obtain();
    }
    mVelocityTracker->AddMovement(ev);

    Boolean handled = FALSE;

    switch (action & MotionEvent_ACTION_MASK) {
    case MotionEvent_ACTION_POINTER_DOWN:
        if (mIgnoreMultitouch) {
            // Multitouch event - abort.
            Cancel();
        }
        break;

    case MotionEvent_ACTION_POINTER_UP:
        // Ending a multitouch gesture and going back to 1 finger
        Int32 count;
        ev->GetPointerCount(&count);
        if (mIgnoreMultitouch && count == 2) {
            Int32 index = (((action & MotionEvent_ACTION_POINTER_INDEX_MASK)
                    >> MotionEvent_ACTION_POINTER_INDEX_SHIFT) == 0) ? 1 : 0;
            ev->GetXEx(index, &mLastMotionX);
            ev->GetYEx(index, &mLastMotionY);
            mVelocityTracker->Recycle();
            mVelocityTracker = VelocityTracker::Obtain();
        }
        break;

    case MotionEvent_ACTION_DOWN:
        {
            if (mDoubleTapListener != NULL) {
                void (STDCALL GestureDetector::*pHandlerFunc)();
                pHandlerFunc = &GestureDetector::HandleTap;

                Boolean hadTapMessage;
                mApartment->HasCppCallbacks(
                    (Handle32)this, *(Handle32*)&pHandlerFunc, &hadTapMessage);
                if (hadTapMessage)
                    mApartment->RemoveCppCallbacks(
                        (Handle32)this, *(Handle32*)&pHandlerFunc);

                if ((mCurrentDownEvent != NULL) && (mPreviousUpEvent != NULL)/* && hadTapMessage */
                    && IsConsideredDoubleTap(mCurrentDownEvent, mPreviousUpEvent, ev)) {
                        // This is a second tap
                        mIsDoubleTapping = TRUE;
                        // Give a callback with the first tap of the double-tap
                        handled |= mDoubleTapListener->OnDoubleTap(mCurrentDownEvent);
                        // Give a callback with down event of the double-tap
                        handled |= mDoubleTapListener->OnDoubleTapEvent(ev);
                }
                else {
                    // This is a first tap
                    mApartment->PostCppCallbackDelayed(
                        (Handle32)this, *(Handle32*)&pHandlerFunc,
                        NULL, 0, DOUBLE_TAP_TIMEOUT);
                }
            }

            mLastMotionX = x;
            mLastMotionY = y;
            if (mCurrentDownEvent != NULL) {
                mCurrentDownEvent->Recycle();
            }
            AutoPtr<CMotionEvent> event;
            CMotionEvent::Obtain((CMotionEvent*)ev, (CMotionEvent**)&event);
            mCurrentDownEvent = event.Get();
            mAlwaysInTapRegion = TRUE;
            mAlwaysInBiggerTapRegion = TRUE;
            mStillDown = TRUE;
            mInLongPress = FALSE;

            Int64 downTime;
            mCurrentDownEvent->GetDownTime(&downTime);
            if (mIsLongpressEnabled) {
                RemoveMessages(LONG_PRESS);
                void (STDCALL GestureDetector::*pHandlerFunc)();
                pHandlerFunc = &GestureDetector::DispatchLongPress;

                mApartment->PostCppCallbackAtTime(
                    (Handle32)this, *(Handle32*)&pHandlerFunc,
                    NULL, 0, downTime + TAP_TIMEOUT + LONGPRESS_TIMEOUT);
            }

            void (STDCALL GestureDetector::*pHandlerFunc)();
            pHandlerFunc = &GestureDetector::HandleShowPress;

            mApartment->PostCppCallbackAtTime(
                (Handle32)this, *(Handle32*)&pHandlerFunc,
                NULL, 0, downTime + TAP_TIMEOUT);
            handled |= mListener->OnDown(ev);
        }
        break;

    case MotionEvent_ACTION_MOVE:
        {
            Int32 count;
            ev->GetPointerCount(&count);
            if (mInLongPress || (mIgnoreMultitouch && count > 1)) {
                break;
            }
            Float scrollX = mLastMotionX - x;
            Float scrollY = mLastMotionY - y;
            if (mIsDoubleTapping) {
                // Give the move events of the double-tap
                handled |= mDoubleTapListener->OnDoubleTapEvent(ev);
            }
            else if (mAlwaysInTapRegion) {
                Float cx, cy;
                mCurrentDownEvent->GetX(&cx);
                mCurrentDownEvent->GetY(&cy);
                Int32 deltaX = (Int32)(x - cx);
                Int32 deltaY = (Int32)(y - cy);
                Int32 distance = (deltaX * deltaX) + (deltaY * deltaY);
                if (distance > mTouchSlopSquare) {
                    handled = mListener->OnScroll(mCurrentDownEvent, ev, scrollX, scrollY);
                    mLastMotionX = x;
                    mLastMotionY = y;
                    mAlwaysInTapRegion = FALSE;
                    RemoveMessages(TAP);
                    RemoveMessages(SHOW_PRESS);
                    RemoveMessages(LONG_PRESS);
                }
                if (distance > mBiggerTouchSlopSquare) {
                    mAlwaysInBiggerTapRegion = FALSE;
                }
            }
            else if ((Math::Abs(scrollX) >= 1) || (Math::Abs(scrollY) >= 1)) {
                handled = mListener->OnScroll(mCurrentDownEvent, ev, scrollX, scrollY);
                mLastMotionX = x;
                mLastMotionY = y;
            }
        }
        break;
    case MotionEvent_ACTION_UP:
        {
            mStillDown = FALSE;
            AutoPtr<CMotionEvent> currentUpEvent;
            CMotionEvent::Obtain((CMotionEvent*)ev, (CMotionEvent**)&currentUpEvent);
            if (mIsDoubleTapping) {
                // Finally, give the up event of the double-tap
                handled |= mDoubleTapListener->OnDoubleTapEvent(ev);
            }
            else if (mInLongPress) {
                RemoveMessages(TAP);
                mInLongPress = FALSE;
            }
            else if (mAlwaysInTapRegion) {
                handled = mListener->OnSingleTapUp(ev);
            } else {

                // A fling must travel the minimum tap distance
                VelocityTracker* velocityTracker = mVelocityTracker;
                velocityTracker->ComputeCurrentVelocity(1000, mMaximumFlingVelocity);
                Float velocityY = velocityTracker->GetYVelocity();
                Float velocityX = velocityTracker->GetXVelocity();

                if ((Math::Abs(velocityY) > mMinimumFlingVelocity)
                    || (Math::Abs(velocityX) > mMinimumFlingVelocity)){
                        handled = mListener->OnFling(mCurrentDownEvent, ev, velocityX, velocityY);
                }
            }
            if (mPreviousUpEvent != NULL) {
                mPreviousUpEvent->Recycle();
            }
            // Hold the event we obtained above - listeners may have changed the original.
            mPreviousUpEvent = currentUpEvent;
            mVelocityTracker->Recycle();
            mVelocityTracker = NULL;
            mIsDoubleTapping = FALSE;
            RemoveMessages(SHOW_PRESS);
            RemoveMessages(LONG_PRESS);
        }
        break;
    case MotionEvent_ACTION_CANCEL:
        Cancel();
    }
    return handled;
}

void GestureDetector::Cancel()
{
    RemoveMessages(SHOW_PRESS);
    RemoveMessages(LONG_PRESS);
    RemoveMessages(TAP);
    mVelocityTracker->Recycle();
    mVelocityTracker = NULL;
    mIsDoubleTapping = FALSE;
    mStillDown = FALSE;
    if (mInLongPress) {
        mInLongPress = FALSE;
    }
}

Boolean GestureDetector::IsConsideredDoubleTap(
    /* [in] */ IMotionEvent* firstDown,
    /* [in] */ IMotionEvent* firstUp,
    /* [in] */ IMotionEvent* secondDown)
{
    if (!mAlwaysInBiggerTapRegion) {
        return FALSE;
    }

    Int64 secondDownTime, firstUpTime;
    secondDown->GetEventTime(&secondDownTime);
    firstUp->GetEventTime(&firstUpTime);
    if (secondDownTime - firstUpTime > DOUBLE_TAP_TIMEOUT) {
        return FALSE;
    }

    Float firstDownX, secondDownX, firstDownY, secondDownY;
    firstDown->GetX(&firstDownX);
    firstDown->GetY(&firstDownY);
    secondDown->GetX(&secondDownX);
    secondDown->GetY(&secondDownY);

    Int32 deltaX = (Int32)firstDownX - (Int32)secondDownX;
    Int32 deltaY = (Int32)firstDownY - (Int32)secondDownY;
    return (deltaX * deltaX + deltaY * deltaY < mDoubleTapSlopSquare);
}

void GestureDetector::DispatchLongPress()
{
    RemoveMessages(TAP);
    mInLongPress = TRUE;
    mListener->OnLongPress(mCurrentDownEvent);
}

void GestureDetector::HandleShowPress()
{
    mListener->OnShowPress(mCurrentDownEvent);
}

void GestureDetector::HandleTap()
{
    // If the user's finger is still down, do not count it as a tap
    if (mDoubleTapListener != NULL && !mStillDown) {
        mDoubleTapListener->OnSingleTapConfirmed(mCurrentDownEvent);
    }
}

void GestureDetector::RemoveMessages(
    /* [in] */ Int32 msgCode)
{
    switch (msgCode) {
    case SHOW_PRESS:
        {
            void (STDCALL OnGestureListener::*pHandlerFunc)(IMotionEvent* event);
            pHandlerFunc = &OnGestureListener::OnShowPress;

            mApartment->RemoveCppCallbacks(
                (Handle32)mListener, *(Handle32*)&pHandlerFunc);
        }
        break;
    case LONG_PRESS:
        {
            void (STDCALL GestureDetector::*pHandlerFunc)();
            pHandlerFunc = &GestureDetector::DispatchLongPress;

            mApartment->RemoveCppCallbacks(
                (Handle32)this, *(Handle32*)&pHandlerFunc);
        }
        break;
    case TAP:
        {
            void (STDCALL GestureDetector::*pHandlerFunc)();
            pHandlerFunc = &GestureDetector::HandleTap;

            mApartment->RemoveCppCallbacks(
                (Handle32)this, *(Handle32*)&pHandlerFunc);
        }
        break;
    default:
        assert(0);
    }
}
