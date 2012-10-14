
#ifndef __GESTUREDETECTOR_H__
#define __GESTUREDETECTOR_H__

#include "view/VelocityTracker.h"

class GestureDetector
{
public:
     /**
      * The listener that is used to notify when a double-tap or a confirmed
      * single-tap occur.
      */
    class OnDoubleTapListener
    {
    public:
        /**
         * Notified when a single-tap occurs.
         * <p>
         * Unlike {@link OnGestureListener#onSingleTapUp(MotionEvent)}, this
         * will only be called after the detector is confident that the user's
         * first tap is not followed by a second tap leading to a double-tap
         * gesture.
         *
         * @param e The down motion event of the single-tap.
         * @return TRUE if the event is consumed, else FALSE
         */
        virtual CARAPI_(Boolean) OnSingleTapConfirmed(
            /* [in] */ IMotionEvent* e) = 0;

        /**
         * Notified when a double-tap occurs.
         *
         * @param e The down motion event of the first tap of the double-tap.
         * @return TRUE if the event is consumed, else FALSE
         */
        virtual CARAPI_(Boolean) OnDoubleTap(
            /* [in] */ IMotionEvent* e) = 0;

        /**
         * Notified when an event within a double-tap gesture occurs, including
         * the down, move, and up events.
         *
         * @param e The motion event that occurred during the double-tap gesture.
         * @return TRUE if the event is consumed, else FALSE
         */
        virtual CARAPI_(Boolean) OnDoubleTapEvent(
            /* [in] */ IMotionEvent* e) = 0;
    };

    /**
     * The listener that is used to notify when gestures occur.
     * If you want to listen for all the different gestures then implement
     * this interface. If you only want to listen for a subset it might
     * be easier to extend {@link SimpleOnGestureListener}.
     */
    class OnGestureListener
    {
    public:
        /**
         * Notified when a tap occurs with the down {@link MotionEvent}
         * that triggered it. This will be triggered immediately for
         * every down event. All other events should be preceded by this.
         *
         * @param e The down motion event.
         */
        virtual CARAPI_(Boolean) OnDown(
            /* [in] */ IMotionEvent* e) = 0;

        /**
         * The user has performed a down {@link MotionEvent} and not performed
         * a move or up yet. This event is commonly used to provide visual
         * feedback to the user to let them know that their action has been
         * recognized i.e. highlight an element.
         *
         * @param e The down motion event
         */
        virtual CARAPI_(void) OnShowPress(
            /* [in] */ IMotionEvent* e) = 0;

        /**
         * Notified when a tap occurs with the up {@link MotionEvent}
         * that triggered it.
         *
         * @param e The up motion event that completed the first tap
         * @return TRUE if the event is consumed, else FALSE
         */
        virtual CARAPI_(Boolean) OnSingleTapUp(
            /* [in] */ IMotionEvent* e) = 0;

        /**
         * Notified when a scroll occurs with the initial on down {@link MotionEvent} and the
         * current move {@link MotionEvent}. The distance in x and y is also supplied for
         * convenience.
         *
         * @param e1 The first down motion event that started the scrolling.
         * @param e2 The move motion event that triggered the current onScroll.
         * @param distanceX The distance along the X axis that has been scrolled since the last
         *              call to onScroll. This is NOT the distance between {@code e1}
         *              and {@code e2}.
         * @param distanceY The distance along the Y axis that has been scrolled since the last
         *              call to onScroll. This is NOT the distance between {@code e1}
         *              and {@code e2}.
         * @return TRUE if the event is consumed, else FALSE
         */
        virtual CARAPI_(Boolean) OnScroll(
            /* [in] */ IMotionEvent* e1,
            /* [in] */ IMotionEvent* e2,
            /* [in] */ Float distanceX,
            /* [in] */ Float distanceY) = 0;

        /**
         * Notified when a long press occurs with the initial on down {@link MotionEvent}
         * that trigged it.
         *
         * @param e The initial on down motion event that started the longpress.
         */
        virtual CARAPI_(void) OnLongPress(
            /* [in] */ IMotionEvent* e) = 0;

        /**
         * Notified of a fling event when it occurs with the initial on down {@link MotionEvent}
         * and the matching up {@link MotionEvent}. The calculated velocity is supplied along
         * the x and y axis in pixels per second.
         *
         * @param e1 The first down motion event that started the fling.
         * @param e2 The move motion event that triggered the current onFling.
         * @param velocityX The velocity of this fling measured in pixels per second
         *              along the x axis.
         * @param velocityY The velocity of this fling measured in pixels per second
         *              along the y axis.
         * @return TRUE if the event is consumed, else FALSE
         */
        virtual CARAPI_(Boolean) OnFling(
            /* [in] */ IMotionEvent* e1,
            /* [in] */ IMotionEvent* e2,
            /* [in] */ Float velocityX,
            /* [in] */ Float velocityY) = 0;

        virtual CARAPI_(OnDoubleTapListener*) ProbeToOnDoubleTapEvent()
        {
            return NULL;
        }
    };

    /**
     * A convenience class to extend when you only want to listen for a subset
     * of all the gestures. This implements all methods in the
     * {@link OnGestureListener} and {@link OnDoubleTapListener} but does
     * nothing and return {@code FALSE} for all applicable methods.
     */
    class SimpleOnGestureListener : public OnGestureListener, public OnDoubleTapListener
    {
    public:
        Boolean OnSingleTapUp(
            /* [in] */ IMotionEvent* e);

        void OnLongPress(
            /* [in] */ IMotionEvent* e);

        Boolean OnScroll(
            /* [in] */ IMotionEvent* e1,
            /* [in] */ IMotionEvent* e2,
            /* [in] */ Float distanceX,
            /* [in] */ Float distanceY);

        Boolean OnFling(
            /* [in] */ IMotionEvent* e1,
            /* [in] */ IMotionEvent* e2,
            /* [in] */ Float velocityX,
            /* [in] */ Float velocityY);

        void OnShowPress(
            /* [in] */ IMotionEvent* e);

        Boolean OnDown(
            /* [in] */ IMotionEvent* e);

        Boolean OnDoubleTap(
            /* [in] */ IMotionEvent* e);

        Boolean OnDoubleTapEvent(
            /* [in] */ IMotionEvent* e);

        Boolean OnSingleTapConfirmed(
            /* [in] */ IMotionEvent* e);

        OnDoubleTapListener* ProbeToOnDoubleTapEvent();
    };

public:
    GestureDetector(
        /* [in] */ IContext* context,
        /* [in] */ OnGestureListener* listener,
        /* [in] */ IApartment* handler = NULL);

    GestureDetector(
        /* [in] */ IContext* context,
        /* [in] */ OnGestureListener* listener,
        /* [in] */ IApartment* handler,
        /* [in] */ Boolean ignoreMultitouch);

    void SetOnDoubleTapListener(
        /* [in] */ OnDoubleTapListener* onDoubleTapListener);

    void SetIsLongpressEnabled(
        /* [in] */ Boolean isLongpressEnabled);

    Boolean IsLongpressEnabled();

    Boolean OnTouchEvent(
        /* [in] */ IMotionEvent* ev);

private:
    void Init(
        /* [in] */ IContext* context,
        /* [in] */ OnGestureListener* listener,
        /* [in] */ IApartment* handler,
        /* [in] */ Boolean ignoreMultitouch);

    void Init2(
        /* [in] */ IContext* context,
        /* [in] */ Boolean ignoreMultitouch);

    void Cancel();

    Boolean IsConsideredDoubleTap(
        /* [in] */ IMotionEvent* firstDown,
        /* [in] */ IMotionEvent* firstUp,
        /* [in] */ IMotionEvent* secondDown);

    void DispatchLongPress();

    void HandleShowPress();

    void HandleTap();

    void RemoveMessages(
        /* [in] */ Int32 msgCode);

private:
    // TODO: ViewConfiguration
    Int32 mBiggerTouchSlopSquare;

    Int32 mTouchSlopSquare;
    Int32 mDoubleTapSlopSquare;
    Int32 mMinimumFlingVelocity;
    Int32 mMaximumFlingVelocity;

    static const Int32 LONGPRESS_TIMEOUT;
    static const Int32 TAP_TIMEOUT;
    static const Int32 DOUBLE_TAP_TIMEOUT;

    // constants for Message.what used by GestureHandler below
    static const Int32 SHOW_PRESS = 1;
    static const Int32 LONG_PRESS = 2;
    static const Int32 TAP = 3;

    AutoPtr<IApartment> mApartment;
    OnGestureListener* mListener;
    OnDoubleTapListener* mDoubleTapListener;

    Boolean mStillDown;
    Boolean mInLongPress;
    Boolean mAlwaysInTapRegion;
    Boolean mAlwaysInBiggerTapRegion;

    AutoPtr<IMotionEvent> mCurrentDownEvent;
    AutoPtr<IMotionEvent> mPreviousUpEvent;

    /**
     * True when the user is still touching for the second tap (down, move, and
     * up events). Can only be TRUE if there is a double tap listener attached.
     */
    Boolean mIsDoubleTapping;

    Float mLastMotionY;
    Float mLastMotionX;

    Boolean mIsLongpressEnabled;

    /**
     * True if we are at a target API level of >= Froyo or the developer can
     * explicitly set it. If TRUE, input events with > 1 pointer will be ignored
     * so we can work side by side with multitouch gesture detectors.
     */
    Boolean mIgnoreMultitouch;

    /**
     * Determines speed during touch scrolling
     */
    AutoPtr<VelocityTracker> mVelocityTracker;
};
#endif //__GESTUREDETECTOR_H__
