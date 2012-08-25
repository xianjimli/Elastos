
#ifndef __CMOTIONEVENT_H__
#define __CMOTIONEVENT_H__

#include "_CMotionEvent.h"
#include <elastos/AutoPtr.h>
#include "view/InputEvent.h"
#include <ui/Input.h>

CarClass(CMotionEvent), public InputEvent
{
    friend ECode create_CMotionEvent_fromNative(
        /* [in] */ const android::MotionEvent* event,
        /* [out] */ CMotionEvent** eventObj);
public:
    /**
    * Transfer object for pointer coordinates.
    *
    * Objects of this type can be used to manufacture new {@link MotionEvent} objects
    * and to query pointer coordinate information in bulk.
    *
    * Refer to {@link InputDevice} for information about how different kinds of
    * input devices and sources represent pointer coordinates.
    */
    class PointerCoords
    {
    public:
        /**
        * The X coordinate of the pointer movement.
        * The interpretation varies by input source and may represent the position of
        * the center of the contact area, a relative displacement in device-specific units
        * or something else.
        */
        Float mX;

        /**
        * The Y coordinate of the pointer movement.
        * The interpretation varies by input source and may represent the position of
        * the center of the contact area, a relative displacement in device-specific units
        * or something else.
        */
        Float mY;

        /**
        * A scaled value that describes the pressure applied to the pointer.
        * The pressure generally ranges from 0 (no pressure at all) to 1 (normal pressure),
        * however values higher than 1 may be generated depending on the calibration of
        * the input device.
        */
        Float mPressure;

        /**
        * A scaled value of the approximate size of the pointer touch area.
        * This represents some approximation of the area of the screen being
        * pressed; the actual value in pixels corresponding to the
        * touch is normalized with the device specific range of values
        * and scaled to a value between 0 and 1. The value of size can be used to
        * determine fat touch events.
        */
        Float mSize;

        /**
        * The length of the major axis of an ellipse that describes the touch area at
        * the point of contact.
        */
        Float mTouchMajor;

        /**
        * The length of the minor axis of an ellipse that describes the touch area at
        * the point of contact.
        */
        Float mTouchMinor;

        /**
        * The length of the major axis of an ellipse that describes the size of
        * the approaching tool.
        * The tool area represents the estimated size of the finger or pen that is
        * touching the device independent of its actual touch area at the point of contact.
        */
        Float mToolMajor;

        /**
        * The length of the minor axis of an ellipse that describes the size of
        * the approaching tool.
        * The tool area represents the estimated size of the finger or pen that is
        * touching the device independent of its actual touch area at the point of contact.
        */
        Float mToolMinor;

        /**
        * The orientation of the touch area and tool area in radians clockwise from vertical.
        * An angle of 0 degrees indicates that the major axis of contact is oriented
        * upwards, is perfectly circular or is of unknown orientation.  A positive angle
        * indicates that the major axis of contact is oriented to the right.  A negative angle
        * indicates that the major axis of contact is oriented to the left.
        * The full range is from -PI/2 radians (finger pointing fully left) to PI/2 radians
        * (finger pointing fully right).
        */
        Float mOrientation;
    };

public:
    static CARAPI Obtain(
        /* [in] */ Int64 downTime,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 action,
        /* [in] */ Int32 pointers,
        /* [in] */ Int32* pointerIds,
        /* [in] */ PointerCoords** pointerCoords,
        /* [in] */ Int32 metaState,
        /* [in] */ Float xPrecision,
        /* [in] */ Float yPrecision,
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 edgeFlags,
        /* [in] */ Int32 source,
        /* [in] */ Int32 flags,
        /* [out] */ CMotionEvent** event);

    static CARAPI Obtain(
        /* [in] */ Int64 downTime,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 action,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float pressure,
        /* [in] */ Float size,
        /* [in] */ Int32 metaState,
        /* [in] */ Float xPrecision,
        /* [in] */ Float yPrecision,
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 edgeFlags,
        /* [out] */ CMotionEvent** event);

    //@Deprecated
    static CARAPI Obtain(
        /* [in] */ Int64 downTime,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 action,
        /* [in] */ Int32 pointers,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float pressure,
        /* [in] */ Float size,
        /* [in] */ Int32 metaState,
        /* [in] */ Float xPrecision,
        /* [in] */ Float yPrecision,
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 edgeFlags,
        /* [out] */ CMotionEvent** event);

    static CARAPI Obtain(
        /* [in] */ Int64 downTime,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 action,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int32 metaState,
        /* [out] */ CMotionEvent** event);

    static CARAPI Obtain(
        /* [in] */ CMotionEvent* o,
        /* [out] */ CMotionEvent** event);

    static CARAPI ObtainNoHistory(
        /* [in] */ CMotionEvent* o,
        /* [out] */ CMotionEvent** event);

    static CARAPI CreateFromParcelBody(
        /* [in] */ IParcel* in,
        /* [out] */ CMotionEvent** event);

private:
    static CARAPI Obtain(
        /* [in] */ Int32 pointerCount,
        /* [in] */ Int32 sampleCount,
        /* [out] */ CMotionEvent** event);

public:
    ~CMotionEvent();

    CARAPI constructor(
        /* [in] */ Int32 pointerCount,
        /* [in] */ Int32 sampleCount);

    CARAPI GetDeviceId(
        /* [out] */ Int32* deviceId);

    CARAPI GetDevice(
        /* [out] */ IInputDevice** device);

    CARAPI GetSource(
        /* [out] */ Int32* source);

    CARAPI SetSource(
        /* [in] */ Int32 source);

    /**
     * Recycle the MotionEvent, to be re-used by a later caller.  After calling
     * this function you must not ever touch the event again.
     */
    CARAPI Recycle();

    /**
     * Scales down the coordination of this event by the given scale.
     *
     * @hide
     */
    CARAPI Scale(
        /* [in] */ Float scale);

    /**
     * Return the kind of action being performed -- one of either
     * {@link #ACTION_DOWN}, {@link #ACTION_MOVE}, {@link #ACTION_UP}, or
     * {@link #ACTION_CANCEL}.  Consider using {@link #getActionMasked}
     * and {@link #getActionIndex} to retrieve the separate masked action
     * and pointer index.
     */
    CARAPI GetAction(
        /* [out] */ Int32* action);

    /**
     * Return the masked action being performed, without pointer index
     * information.  May be any of the actions: {@link #ACTION_DOWN},
     * {@link #ACTION_MOVE}, {@link #ACTION_UP}, {@link #ACTION_CANCEL},
     * {@link #ACTION_POINTER_DOWN}, or {@link #ACTION_POINTER_UP}.
     * Use {@link #getActionIndex} to return the index associated with
     * pointer actions.
     */
    CARAPI GetActionMasked(
        /* [out] */ Int32* actionMasked);

    /**
     * For {@link #ACTION_POINTER_DOWN} or {@link #ACTION_POINTER_UP}
     * as returned by {@link #getActionMasked}, this returns the associated
     * pointer index.  The index may be used with {@link #getPointerId(Int32)},
     * {@link #getX(Int32)}, {@link #getY(Int32)}, {@link #getPressure(Int32)},
     * and {@link #getSize(Int32)} to get information about the pointer that has
     * gone down or up.
     */
    CARAPI GetActionIndex(
        /* [out] */ Int32* actionIndex);

    /**
     * Gets the motion event flags.
     *
     * @see #FLAG_WINDOW_IS_OBSCURED
     */
    CARAPI GetFlags(
        /* [out] */ Int32* flags);

    /**
     * Returns the time (in ms) when the user originally pressed down to start
     * a stream of position events.
     */
    CARAPI GetDownTime(
        /* [out] */ Int64* downTime);

    /**
     * Returns the time (in ms) when this specific event was generated.
     */
    CARAPI GetEventTime(
        /* [out] */ Int64* eventTime);

    /**
     * Returns the time (in ns) when this specific event was generated.
     * The value is in nanosecond precision but it may not have nanosecond accuracy.
     *
     * @hide
     */
    CARAPI GetEventTimeNano(
        /* [out] */ Int64* eventTimeNano);

    /**
     * {@link #getX(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetX(
        /* [out] */ Float* x);

    /**
     * {@link #getY(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetY(
        /* [out] */ Float* y);

    /**
     * {@link #getPressure(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetPressure(
        /* [out] */ Float* pressure);

    /**
     * {@link #getSize(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetSize(
        /* [out] */ Float* size);

    /**
     * {@link #getTouchMajor(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetTouchMajor(
        /* [out] */ Float* touchMajor);

    /**
     * {@link #getTouchMinor(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetTouchMinor(
        /* [out] */ Float* touchMinor);

    /**
     * {@link #getToolMajor(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetToolMajor(
        /* [out] */ Float* toolMajor);

    /**
     * {@link #getToolMinor(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetToolMinor(
        /* [out] */ Float* toolMinor);

    /**
     * {@link #getOrientation(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetOrientation(
        /* [out] */ Float* orientation);

    /**
     * The number of pointers of data contained in this event.  Always
     * >= 1.
     */
    CARAPI GetPointerCount(
        /* [out] */ Int32* count);

    /**
     * Return the pointer identifier associated with a particular pointer
     * data index is this event.  The identifier tells you the actual pointer
     * number associated with the data, accounting for individual pointers
     * going up and down since the start of the current gesture.
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetPointerId(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Int32* pointerId);

    /**
     * Given a pointer identifier, find the index of its data in the event.
     *
     * @param pointerId The identifier of the pointer to be found.
     * @return Returns either the index of the pointer (for use with
     * {@link #getX(Int32)} et al.), or -1 if there is no data available for
     * that pointer identifier.
     */
    CARAPI FindPointerIndex(
        /* [in] */ Int32 pointerId,
        /* [out] */ Int32* pointerIndex);

    /**
     * Returns the X coordinate of this event for the given pointer
     * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
     * identifier for this index).
     * Whole numbers are pixels; the
     * value may have a fraction for input devices that are sub-pixel precise.
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetXEx(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Float* x);

    /**
     * Returns the Y coordinate of this event for the given pointer
     * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
     * identifier for this index).
     * Whole numbers are pixels; the
     * value may have a fraction for input devices that are sub-pixel precise.
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetYEx(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Float* y);

    /**
     * Returns the current pressure of this event for the given pointer
     * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
     * identifier for this index).
     * The pressure generally
     * ranges from 0 (no pressure at all) to 1 (normal pressure), however
     * values higher than 1 may be generated depending on the calibration of
     * the input device.
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetPressureEx(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Float* pressure);

    /**
     * Returns a scaled value of the approximate size for the given pointer
     * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
     * identifier for this index).
     * This represents some approximation of the area of the screen being
     * pressed; the actual value in pixels corresponding to the
     * touch is normalized with the device specific range of values
     * and scaled to a value between 0 and 1. The value of size can be used to
     * determine fat touch events.
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetSizeEx(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Float* size);

    /**
     * Returns the length of the major axis of an ellipse that describes the touch
     * area at the point of contact for the given pointer
     * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
     * identifier for this index).
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetTouchMajorEx(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Float* touchMajor);

    /**
     * Returns the length of the minor axis of an ellipse that describes the touch
     * area at the point of contact for the given pointer
     * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
     * identifier for this index).
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetTouchMinorEx(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Float* touchMinor);

    /**
     * Returns the length of the major axis of an ellipse that describes the size of
     * the approaching tool for the given pointer
     * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
     * identifier for this index).
     * The tool area represents the estimated size of the finger or pen that is
     * touching the device independent of its actual touch area at the point of contact.
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetToolMajorEx(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Float* toolMajor);

    /**
     * Returns the length of the minor axis of an ellipse that describes the size of
     * the approaching tool for the given pointer
     * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
     * identifier for this index).
     * The tool area represents the estimated size of the finger or pen that is
     * touching the device independent of its actual touch area at the point of contact.
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetToolMinorEx(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Float* toolMinor);

    /**
     * Returns the orientation of the touch area and tool area in radians clockwise from vertical
     * for the given pointer <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
     * identifier for this index).
     * An angle of 0 degrees indicates that the major axis of contact is oriented
     * upwards, is perfectly circular or is of unknown orientation.  A positive angle
     * indicates that the major axis of contact is oriented to the right.  A negative angle
     * indicates that the major axis of contact is oriented to the left.
     * The full range is from -PI/2 radians (finger pointing fully left) to PI/2 radians
     * (finger pointing fully right).
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     */
    CARAPI GetOrientationEx(
        /* [in] */ Int32 pointerIndex,
        /* [out] */ Float* orientation);

    /**
     * Populates a {@link PointerCoords} object with pointer coordinate data for
     * the specified pointer index.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param outPointerCoords The pointer coordinate object to populate.
     */
    CARAPI GetPointerCoords(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Handle32 outPointerCoords);

    /**
     * Returns the state of any meta / modifier keys that were in effect when
     * the event was generated.  This is the same values as those
     * returned by {@link KeyEvent#getMetaState() KeyEvent.getMetaState}.
     *
     * @return an integer in which each bit set to 1 represents a pressed
     *         meta key
     *
     * @see KeyEvent#getMetaState()
     */
    CARAPI GetMetaState(
        /* [out] */ Int32* metaState);

    /**
     * Returns the original raw X coordinate of this event.  For touch
     * events on the screen, this is the original location of the event
     * on the screen, before it had been adjusted for the containing window
     * and views.
     */
    CARAPI GetRawX(
        /* [out] */ Float* rawX);

    /**
     * Returns the original raw Y coordinate of this event.  For touch
     * events on the screen, this is the original location of the event
     * on the screen, before it had been adjusted for the containing window
     * and views.
     */
    CARAPI GetRawY(
        /* [out] */ Float* rawY);

    /**
     * Return the precision of the X coordinates being reported.  You can
     * multiple this number with {@link #getX} to find the actual hardware
     * value of the X coordinate.
     * @return Returns the precision of X coordinates being reported.
     */
    CARAPI GetXPrecision(
        /* [out] */ Float* xPrecision);
    /**
     * Return the precision of the Y coordinates being reported.  You can
     * multiple this number with {@link #getY} to find the actual hardware
     * value of the Y coordinate.
     * @return Returns the precision of Y coordinates being reported.
     */
    CARAPI GetYPrecision(
        /* [out] */ Float* yPrecision);

    /**
     * Returns the number of historical points in this event.  These are
     * movements that have occurred between this event and the previous event.
     * This only applies to ACTION_MOVE events -- all other actions will have
     * a size of 0.
     *
     * @return Returns the number of historical points in the event.
     */
    CARAPI GetHistorySize(
        /* [out] */ Int32* historySize);

    /**
     * Returns the time that a historical movement occurred between this event
     * and the previous event.  Only applies to ACTION_MOVE events.
     *
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getEventTime
     */
    CARAPI GetHistoricalEventTime(
        /* [in] */ Int32 pos,
        /* [out] */ Int64* hEventTime);

    /**
     * {@link #getHistoricalX(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetHistoricalX(
        /* [in] */ Int32 pos,
        /* [out] */ Float* hX);

    /**
     * {@link #getHistoricalY(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetHistoricalY(
        /* [in] */ Int32 pos,
        /* [out] */ Float* hY);

    /**
     * {@link #getHistoricalPressure(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetHistoricalPressure(
        /* [in] */ Int32 pos,
        /* [out] */ Float* hPressure);

    /**
     * {@link #getHistoricalSize(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetHistoricalSize(
        /* [in] */ Int32 pos,
        /* [out] */ Float* hSize);

    /**
     * {@link #getHistoricalTouchMajor(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetHistoricalTouchMajor(
        /* [in] */ Int32 pos,
        /* [out] */ Float* hTouchMajor);

    /**
     * {@link #getHistoricalTouchMinor(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetHistoricalTouchMinor(
        /* [in] */ Int32 pos,
        /* [out] */ Float* hTouchMinor);

    /**
     * {@link #getHistoricalToolMajor(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetHistoricalToolMajor(
        /* [in] */ Int32 pos,
        /* [out] */ Float* hToolMajor);

    /**
     * {@link #getHistoricalToolMinor(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetHistoricalToolMinor(
        /* [in] */ Int32 pos,
        /* [out] */ Float* hToolMinor);

    /**
     * {@link #getHistoricalOrientation(Int32)} for the first pointer index (may be an
     * arbitrary pointer identifier).
     */
    CARAPI GetHistoricalOrientation(
        /* [in] */ Int32 pos,
        /* [out] */ Float* hOrientation);

    /**
     * Returns a historical X coordinate, as per {@link #getX(Int32)}, that
     * occurred between this event and the previous event for the given pointer.
     * Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getX
     */
    CARAPI GetHistoricalXEx(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [out] */ Float* hX);

    /**
     * Returns a historical Y coordinate, as per {@link #getY(Int32)}, that
     * occurred between this event and the previous event for the given pointer.
     * Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getY
     */
    CARAPI GetHistoricalYEx(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [out] */ Float* hY);

    /**
     * Returns a historical pressure coordinate, as per {@link #getPressure(Int32)},
     * that occurred between this event and the previous event for the given
     * pointer.  Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getPressure
     */
    CARAPI GetHistoricalPressureEx(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [out] */ Float* hPressure);

    /**
     * Returns a historical size coordinate, as per {@link #getSize(Int32)}, that
     * occurred between this event and the previous event for the given pointer.
     * Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getSize
     */
    CARAPI GetHistoricalSizeEx(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [out] */ Float* hSize);

    /**
     * Returns a historical touch major axis coordinate, as per {@link #getTouchMajor(Int32)}, that
     * occurred between this event and the previous event for the given pointer.
     * Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getTouchMajor
     */
    CARAPI GetHistoricalTouchMajorEx(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [out] */ Float* hTouchMajor);

    /**
     * Returns a historical touch minor axis coordinate, as per {@link #getTouchMinor(Int32)}, that
     * occurred between this event and the previous event for the given pointer.
     * Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getTouchMinor
     */
    CARAPI GetHistoricalTouchMinorEx(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [out] */ Float* hTouchMinor);

    /**
     * Returns a historical tool major axis coordinate, as per {@link #getToolMajor(Int32)}, that
     * occurred between this event and the previous event for the given pointer.
     * Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getToolMajor
     */
    CARAPI GetHistoricalToolMajorEx(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [out] */ Float* hToolMajor);

    /**
     * Returns a historical tool minor axis coordinate, as per {@link #getToolMinor(Int32)}, that
     * occurred between this event and the previous event for the given pointer.
     * Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getToolMinor
     */
    CARAPI GetHistoricalToolMinorEx(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [out] */ Float* hToolMinorEx);

    /**
     * Returns a historical orientation coordinate, as per {@link #getOrientation(Int32)}, that
     * occurred between this event and the previous event for the given pointer.
     * Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     *
     * @see #getHistorySize
     * @see #getOrientation
     */
    CARAPI GetHistoricalOrientationEx(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [out] */ Float* hOrientation);

    /**
     * Populates a {@link PointerCoords} object with historical pointer coordinate data,
     * as per {@link #getPointerCoords}, that occurred between this event and the previous
     * event for the given pointer.
     * Only applies to ACTION_MOVE events.
     *
     * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
     * (the first pointer that is down) to {@link #getPointerCount()}-1.
     * @param pos Which historical value to return; must be less than
     * {@link #getHistorySize}
     * @param outPointerCoords The pointer coordinate object to populate.
     *
     * @see #getHistorySize
     * @see #getPointerCoords
     */
    CARAPI GetHistoricalPointerCoords(
        /* [in] */ Int32 pointerIndex,
        /* [in] */ Int32 pos,
        /* [in] */ Handle32 outPointerCoords);

    /**
     * Returns a bitfield indicating which edges, if any, were touched by this
     * MotionEvent. For touch events, clients can use this to determine if the
     * user's finger was touching the edge of the display.
     *
     * @see #EDGE_LEFT
     * @see #EDGE_TOP
     * @see #EDGE_RIGHT
     * @see #EDGE_BOTTOM
     */
    CARAPI GetEdgeFlags(
        /* [out] */ Int32* edgeFlags);

    /**
     * Sets the bitfield indicating which edges, if any, were touched by this
     * MotionEvent.
     *
     * @see #getEdgeFlags()
     */
    CARAPI SetEdgeFlags(
        /* [in] */ Int32 flags);

    /**
     * Sets this event's action.
     */
    CARAPI SetAction(
        /* [in] */ Int32 action);

    /**
     * Adjust this event's location.
     * @param deltaX Amount to add to the current X coordinate of the event.
     * @param deltaY Amount to add to the current Y coordinate of the event.
     */
    CARAPI OffsetLocation(
        /* [in] */ Float deltaX,
        /* [in] */ Float deltaY);

    /**
     * Set this event's location.  Applies {@link #offsetLocation} with a
     * delta from the current location to the given new location.
     *
     * @param x New absolute X location.
     * @param y New absolute Y location.
     */
    CARAPI SetLocation(
        /* [in] */ Float x,
        /* [in] */ Float y);

    /**
     * Add a new movement to the batch of movements in this event.  The event's
     * current location, position and size is updated to the new values.
     * The current values in the event are added to a list of historical values.
     *
     * Only applies to {@link #ACTION_MOVE} events.
     *
     * @param eventTime The time stamp (in ms) for this data.
     * @param x The new X position.
     * @param y The new Y position.
     * @param pressure The new pressure.
     * @param size The new size.
     * @param metaState Meta key state.
     */
    CARAPI AddBatch(
        /* [in] */ Int64 eventTime,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float pressure,
        /* [in] */ Float size,
        /* [in] */ Int32 metaState);

    /**
     * Add a new movement to the batch of movements in this event.  The event's
     * current location, position and size is updated to the new values.
     * The current values in the event are added to a list of historical values.
     *
     * Only applies to {@link #ACTION_MOVE} events.
     *
     * @param eventTime The time stamp (in ms) for this data.
     * @param pointerCoords The new pointer coordinates.
     * @param metaState Meta key state.
     */
    CARAPI AddBatch(
        /* [in] */ Int64 eventTime,
        /* [in] */ PointerCoords** pointerCoords,
        /* [in] */ Int32 metaState);

    //@Override
    //public String toString() {
    //    return "MotionEvent{" + Integer.toHexString(System.identityHashCode(this))
    //        + " action=" + mAction + " x=" + getX()
    //        + " y=" + getY() + " pressure=" + getPressure() + " size=" + getSize() + "}";
    //}

    //public static const Parcelable.Creator<MotionEvent> CREATOR
    //        = new Parcelable.Creator<MotionEvent>() {
    //    public MotionEvent createFromParcel(Parcel in) {
    //        in.readInt(); // skip token, we already know this is a MotionEvent
    //        return MotionEvent.createFromParcelBody(in);
    //    }

    //    public MotionEvent[] newArray(Int32 size) {
    //        return new MotionEvent[size];
    //    }
    //};

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

private:
    CARAPI_(void) GetPointerCoordsAtSampleIndex(
        /* [in] */ Int32 sampleIndex,
        /* [in] */ PointerCoords* outPointerCoords);

    CARAPI_(void) SetPointerCoordsAtSampleIndex(
        /* [in] */ Int32 sampleIndex,
        /* [in] */ PointerCoords** pointerCoords);

    CARAPI_(void) SetPointerCoordsAtSampleIndex(
        /* [in] */ Int32 sampleIndex,
        /* [in] */ PointerCoords* pointerCoords);

    CARAPI_(void) SetPointerCoordsAtSampleIndex(
        /* [in] */ Int32 sampleIndex,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float pressure,
        /* [in] */ Float size);

    CARAPI_(void) IncrementNumSamplesAndReserveStorage(
        /* [in] */ Int32 dataSampleStride);

private:
    static const Int64 MS_PER_NS = 1000000;
    static const Boolean TRACK_RECYCLED_LOCATION = FALSE;

    /*
    * Offset for the sample's X coordinate.
    */
    static const Int32 SAMPLE_X = 0;

    /*
    * Offset for the sample's Y coordinate.
    */
    static const Int32 SAMPLE_Y = 1;

    /*
    * Offset for the sample's pressure.
    */
    static const Int32 SAMPLE_PRESSURE = 2;

    /*
    * Offset for the sample's size
    */
    static const Int32 SAMPLE_SIZE = 3;

    /*
    * Offset for the sample's touch major axis length.
    */
    static const Int32 SAMPLE_TOUCH_MAJOR = 4;

    /*
    * Offset for the sample's touch minor axis length.
    */
    static const Int32 SAMPLE_TOUCH_MINOR = 5;

    /*
    * Offset for the sample's tool major axis length.
    */
    static const Int32 SAMPLE_TOOL_MAJOR = 6;

    /*
    * Offset for the sample's tool minor axis length.
    */
    static const Int32 SAMPLE_TOOL_MINOR = 7;

    /*
    * Offset for the sample's orientation.
    */
    static const Int32 SAMPLE_ORIENTATION = 8;

    /*
    * Number of data items for each sample.
    */
    static const Int32 NUM_SAMPLE_DATA = 9;

    /*
    * Minimum number of pointers for which to reserve space when allocating new
    * motion events.  This is explicitly not a bound on the maximum number of pointers.
    */
    static const Int32 BASE_AVAIL_POINTERS = 5;

    /*
    * Minimum number of samples for which to reserve space when allocating new motion events.
    */
    static const Int32 BASE_AVAIL_SAMPLES = 8;

    static const Int32 MAX_RECYCLED = 10;

    static Mutex gRecyclerLock;
    static Int32 gRecyclerUsed;
    static AutoPtr<CMotionEvent> gRecyclerTop;

private:
    Int64 mDownTimeNano;
    Int32 mAction;
    Float mXOffset;
    Float mYOffset;
    Float mXPrecision;
    Float mYPrecision;
    Int32 mEdgeFlags;
    Int32 mMetaState;
    Int32 mFlags;

    Int32 mNumPointers;
    Int32 mNumSamples;

    Int32 mLastDataSampleIndex;
    Int32 mLastEventTimeNanoSampleIndex;

    // Array of mNumPointers size of identifiers for each pointer of data.
    Int32* mPointerIdentifiers;
    Int32 mPointerIdentifiersLength;

    // Array of (mNumSamples * mNumPointers * NUM_SAMPLE_DATA) size of event data.
    // Samples are ordered from oldest to newest.
    Float* mDataSamples;
    Int32 mDataSamplesLength;

    // Array of mNumSamples size of event time stamps in nanoseconds.
    // Samples are ordered from oldest to newest.
    Int64* mEventTimeNanoSamples;
    Int32 mEventTimeNanoSamplesLength;

    AutoPtr<CMotionEvent> mNext;
    //RuntimeException mRecycledLocation;
    Boolean mRecycled;
};

#endif //__CMOTIONEVENT_H__
