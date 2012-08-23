
#include "view/CMotionEvent.h"
#include "view/CInputDevice.h"
#include <Logger.h>

using namespace Elastos::Utility;
using namespace Elastos::Utility::Logging;

const Int64 CMotionEvent::MS_PER_NS;
const Boolean CMotionEvent::TRACK_RECYCLED_LOCATION;
const Int32 CMotionEvent::SAMPLE_X;
const Int32 CMotionEvent::SAMPLE_Y;
const Int32 CMotionEvent::SAMPLE_PRESSURE;
const Int32 CMotionEvent::SAMPLE_SIZE;
const Int32 CMotionEvent::SAMPLE_TOUCH_MAJOR;
const Int32 CMotionEvent::SAMPLE_TOUCH_MINOR;
const Int32 CMotionEvent::SAMPLE_TOOL_MAJOR;
const Int32 CMotionEvent::SAMPLE_TOOL_MINOR;
const Int32 CMotionEvent::SAMPLE_ORIENTATION;
const Int32 CMotionEvent::NUM_SAMPLE_DATA;
const Int32 CMotionEvent::BASE_AVAIL_POINTERS;
const Int32 CMotionEvent::BASE_AVAIL_SAMPLES;
const Int32 CMotionEvent::MAX_RECYCLED;

Mutex CMotionEvent::gRecyclerLock;
Int32 CMotionEvent::gRecyclerUsed = 0;
AutoPtr<CMotionEvent> CMotionEvent::gRecyclerTop;

ECode CMotionEvent::Obtain(
    /* [in] */ Int32 pointerCount,
    /* [in] */ Int32 sampleCount,
    /* [out] */ CMotionEvent** event)
{
    if (event == NULL) {
        return E_INVALID_ARGUMENT;
    }

    {
        Mutex::Autolock lock(gRecyclerLock);

        if (gRecyclerTop == NULL) {
            if (pointerCount < BASE_AVAIL_POINTERS) {
                pointerCount = BASE_AVAIL_POINTERS;
            }
            if (sampleCount < BASE_AVAIL_SAMPLES) {
                sampleCount = BASE_AVAIL_SAMPLES;
            }

            return CMotionEvent::NewByFriend(pointerCount, sampleCount, event);
        }
        *event = gRecyclerTop;
        (*event)->AddRef();
        gRecyclerTop = (*event)->mNext;
        --gRecyclerUsed;
    }

    //(*event)->mRecycledLocation = NULL;
    (*event)->mRecycled = FALSE;
    (*event)->mNext = NULL;

    if ((*event)->mPointerIdentifiersLength < pointerCount) {
        delete[] (*event)->mPointerIdentifiers;
        (*event)->mPointerIdentifiers = new Int32[pointerCount];
        (*event)->mPointerIdentifiersLength = pointerCount;
    }

    if ((*event)->mEventTimeNanoSamplesLength < sampleCount) {
        delete[] (*event)->mEventTimeNanoSamples;
        (*event)->mEventTimeNanoSamples = new Int64[sampleCount];
        (*event)->mEventTimeNanoSamplesLength = sampleCount;
    }

    Int32 neededDataSamplesCount = pointerCount * sampleCount * NUM_SAMPLE_DATA;
    if ((*event)->mDataSamplesLength < neededDataSamplesCount) {
        delete[] (*event)->mDataSamples;
        (*event)->mDataSamples = new Float[neededDataSamplesCount];
        (*event)->mDataSamplesLength =  neededDataSamplesCount;
    }

    return NOERROR;
}

/**
 * Create a new MotionEvent, filling in all of the basic values that
 * define the motion.
 *
 * @param downTime The time (in ms) when the user originally pressed down to start
 * a stream of position events.  This must be obtained from {@link SystemClock#uptimeMillis()}.
 * @param eventTime The the time (in ms) when this specific event was generated.  This
 * must be obtained from {@link SystemClock#uptimeMillis()}.
 * @param action The kind of action being performed -- one of either
 * {@link #ACTION_DOWN}, {@link #ACTION_MOVE}, {@link #ACTION_UP}, or
 * {@link #ACTION_CANCEL}.
 * @param pointers The number of points that will be in this event.
 * @param pointerIds An array of <em>pointers</em> values providing
 * an identifier for each pointer.
 * @param pointerCoords An array of <em>pointers</em> values providing
 * a {@link PointerCoords} coordinate object for each pointer.
 * @param metaState The state of any meta / modifier keys that were in effect when
 * the event was generated.
 * @param xPrecision The precision of the X coordinate being reported.
 * @param yPrecision The precision of the Y coordinate being reported.
 * @param deviceId The id for the device that this event came from.  An id of
 * zero indicates that the event didn't come from a physical device; other
 * numbers are arbitrary and you shouldn't depend on the values.
 * @param edgeFlags A bitfield indicating which edges, if any, were touched by this
 * MotionEvent.
 * @param source The source of this event.
 * @param flags The motion event flags.
 */
ECode CMotionEvent::Obtain(
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
    /* [out] */ CMotionEvent** event)
{
    ECode ec = Obtain(pointers, 1, event);
    if (FAILED(ec)) {
        return ec;
    }

    (*event)->mDeviceId = deviceId;
    (*event)->mSource = source;
    (*event)->mEdgeFlags = edgeFlags;
    (*event)->mDownTimeNano = downTime * MS_PER_NS;
    (*event)->mAction = action;
    (*event)->mFlags = flags;
    (*event)->mMetaState = metaState;
    (*event)->mXOffset = 0;
    (*event)->mYOffset = 0;
    (*event)->mXPrecision = xPrecision;
    (*event)->mYPrecision = yPrecision;

    (*event)->mNumPointers = pointers;
    (*event)->mNumSamples = 1;

    (*event)->mLastDataSampleIndex = 0;
    (*event)->mLastEventTimeNanoSampleIndex = 0;

    //System.arraycopy(pointerIds, 0, (*event)->mPointerIdentifiers, 0, pointers);
    memcpy((*event)->mPointerIdentifiers, pointerIds, sizeof(Int32) * pointers);

    (*event)->mEventTimeNanoSamples[0] = eventTime * MS_PER_NS;

    (*event)->SetPointerCoordsAtSampleIndex(0, pointerCoords);

    return NOERROR;
}

/**
 * Create a new MotionEvent, filling in all of the basic values that
 * define the motion.
 *
 * @param downTime The time (in ms) when the user originally pressed down to start
 * a stream of position events.  This must be obtained from {@link SystemClock#uptimeMillis()}.
 * @param eventTime  The the time (in ms) when this specific event was generated.  This
 * must be obtained from {@link SystemClock#uptimeMillis()}.
 * @param action The kind of action being performed -- one of either
 * {@link #ACTION_DOWN}, {@link #ACTION_MOVE}, {@link #ACTION_UP}, or
 * {@link #ACTION_CANCEL}.
 * @param x The X coordinate of this event.
 * @param y The Y coordinate of this event.
 * @param pressure The current pressure of this event.  The pressure generally
 * ranges from 0 (no pressure at all) to 1 (normal pressure), however
 * values higher than 1 may be generated depending on the calibration of
 * the input device.
 * @param size A scaled value of the approximate size of the area being pressed when
 * touched with the finger. The actual value in pixels corresponding to the finger
 * touch is normalized with a device specific range of values
 * and scaled to a value between 0 and 1.
 * @param metaState The state of any meta / modifier keys that were in effect when
 * the event was generated.
 * @param xPrecision The precision of the X coordinate being reported.
 * @param yPrecision The precision of the Y coordinate being reported.
 * @param deviceId The id for the device that this event came from.  An id of
 * zero indicates that the event didn't come from a physical device; other
 * numbers are arbitrary and you shouldn't depend on the values.
 * @param edgeFlags A bitfield indicating which edges, if any, were touched by this
 * MotionEvent.
 */
ECode CMotionEvent::Obtain(
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
    /* [out] */ CMotionEvent** event)
{
    ECode ec = Obtain(1, 1, event);
    if (FAILED(ec)) {
        return ec;
    }

    (*event)->mDeviceId = deviceId;
    (*event)->mSource = CInputDevice::SOURCE_UNKNOWN;
    (*event)->mEdgeFlags = edgeFlags;
    (*event)->mDownTimeNano = downTime * MS_PER_NS;
    (*event)->mAction = action;
    (*event)->mFlags = 0;
    (*event)->mMetaState = metaState;
    (*event)->mXOffset = 0;
    (*event)->mYOffset = 0;
    (*event)->mXPrecision = xPrecision;
    (*event)->mYPrecision = yPrecision;

    (*event)->mNumPointers = 1;
    (*event)->mNumSamples = 1;

    (*event)->mLastDataSampleIndex = 0;
    (*event)->mLastEventTimeNanoSampleIndex = 0;

    (*event)->mPointerIdentifiers[0] = 0;

    (*event)->mEventTimeNanoSamples[0] = eventTime * MS_PER_NS;

    (*event)->SetPointerCoordsAtSampleIndex(0, x, y, pressure, size);

    return NOERROR;
}

/**
 * Create a new MotionEvent, filling in all of the basic values that
 * define the motion.
 *
 * @param downTime The time (in ms) when the user originally pressed down to start
 * a stream of position events.  This must be obtained from {@link SystemClock#uptimeMillis()}.
 * @param eventTime  The the time (in ms) when this specific event was generated.  This
 * must be obtained from {@link SystemClock#uptimeMillis()}.
 * @param action The kind of action being performed -- one of either
 * {@link #ACTION_DOWN}, {@link #ACTION_MOVE}, {@link #ACTION_UP}, or
 * {@link #ACTION_CANCEL}.
 * @param pointers The number of pointers that are active in this event.
 * @param x The X coordinate of this event.
 * @param y The Y coordinate of this event.
 * @param pressure The current pressure of this event.  The pressure generally
 * ranges from 0 (no pressure at all) to 1 (normal pressure), however
 * values higher than 1 may be generated depending on the calibration of
 * the input device.
 * @param size A scaled value of the approximate size of the area being pressed when
 * touched with the finger. The actual value in pixels corresponding to the finger
 * touch is normalized with a device specific range of values
 * and scaled to a value between 0 and 1.
 * @param metaState The state of any meta / modifier keys that were in effect when
 * the event was generated.
 * @param xPrecision The precision of the X coordinate being reported.
 * @param yPrecision The precision of the Y coordinate being reported.
 * @param deviceId The id for the device that this event came from.  An id of
 * zero indicates that the event didn't come from a physical device; other
 * numbers are arbitrary and you shouldn't depend on the values.
 * @param edgeFlags A bitfield indicating which edges, if any, were touched by this
 * MotionEvent.
 *
 * @deprecated Use {@link #obtain(Int64, Int64, Int32, Float, Float, Float, Float, Int32, Float, Float, Int32, Int32)}
 * instead.
 */
//@Deprecated
ECode CMotionEvent::Obtain(
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
    /* [out] */ CMotionEvent** event)
{
    return Obtain(downTime, eventTime, action, x, y, pressure, size,
            metaState, xPrecision, yPrecision, deviceId, edgeFlags, event);
}

/**
 * Create a new MotionEvent, filling in a subset of the basic motion
 * values.  Those not specified here are: device id (always 0), pressure
 * and size (always 1), x and y precision (always 1), and edgeFlags (always 0).
 *
 * @param downTime The time (in ms) when the user originally pressed down to start
 * a stream of position events.  This must be obtained from {@link SystemClock#uptimeMillis()}.
 * @param eventTime  The the time (in ms) when this specific event was generated.  This
 * must be obtained from {@link SystemClock#uptimeMillis()}.
 * @param action The kind of action being performed -- one of either
 * {@link #ACTION_DOWN}, {@link #ACTION_MOVE}, {@link #ACTION_UP}, or
 * {@link #ACTION_CANCEL}.
 * @param x The X coordinate of this event.
 * @param y The Y coordinate of this event.
 * @param metaState The state of any meta / modifier keys that were in effect when
 * the event was generated.
 */
ECode CMotionEvent::Obtain(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 metaState,
    /* [out] */ CMotionEvent** event)
{
    return Obtain(downTime, eventTime, action, x, y, 1.0f, 1.0f,
            metaState, 1.0f, 1.0f, 0, 0, event);
}

/**
 * Create a new MotionEvent, copying from an existing one.
 */
ECode CMotionEvent::Obtain(
    /* [in] */ CMotionEvent* o,
    /* [out] */ CMotionEvent** event)
{
    ECode ec = Obtain(o->mNumPointers, o->mNumSamples, event);
    if (FAILED(ec)) {
        return ec;
    }

    (*event)->mDeviceId = o->mDeviceId;
    (*event)->mSource = o->mSource;
    (*event)->mEdgeFlags = o->mEdgeFlags;
    (*event)->mDownTimeNano = o->mDownTimeNano;
    (*event)->mAction = o->mAction;
    (*event)->mFlags = o->mFlags;
    (*event)->mMetaState = o->mMetaState;
    (*event)->mXOffset = o->mXOffset;
    (*event)->mYOffset = o->mYOffset;
    (*event)->mXPrecision = o->mXPrecision;
    (*event)->mYPrecision = o->mYPrecision;
    Int32 numPointers = (*event)->mNumPointers = o->mNumPointers;
    Int32 numSamples = (*event)->mNumSamples = o->mNumSamples;

    (*event)->mLastDataSampleIndex = o->mLastDataSampleIndex;
    (*event)->mLastEventTimeNanoSampleIndex = o->mLastEventTimeNanoSampleIndex;

    //System.arraycopy(o.mPointerIdentifiers, 0, (*event)->mPointerIdentifiers, 0, numPointers);
    memcpy((*event)->mPointerIdentifiers, o->mPointerIdentifiers, sizeof(Int32) * numPointers);

    //System.arraycopy(o.mEventTimeNanoSamples, 0, (*event)->mEventTimeNanoSamples, 0, numSamples);
    memcpy((*event)->mEventTimeNanoSamples, o->mEventTimeNanoSamples, sizeof(Int64) * numPointers);

    //System.arraycopy(o.mDataSamples, 0, (*event)->mDataSamples, 0,
    //        numPointers * numSamples * NUM_SAMPLE_DATA);
    memcpy((*event)->mDataSamples, o->mDataSamples,
        sizeof(Float) * numPointers * numSamples * NUM_SAMPLE_DATA);

    return NOERROR;
}

/**
 * Create a new MotionEvent, copying from an existing one, but not including
 * any historical point information.
 */
ECode CMotionEvent::ObtainNoHistory(
    /* [in] */ CMotionEvent* o,
    /* [out] */ CMotionEvent** event)
{
    ECode ec = Obtain(o->mNumPointers, 1, event);
    if (FAILED(ec)) {
        return ec;
    }

    (*event)->mDeviceId = o->mDeviceId;
    (*event)->mSource = o->mSource;
    (*event)->mEdgeFlags = o->mEdgeFlags;
    (*event)->mDownTimeNano = o->mDownTimeNano;
    (*event)->mAction = o->mAction;
    (*event)->mFlags = o->mFlags;
    (*event)->mMetaState = o->mMetaState;
    (*event)->mXOffset = o->mXOffset;
    (*event)->mYOffset = o->mYOffset;
    (*event)->mXPrecision = o->mXPrecision;
    (*event)->mYPrecision = o->mYPrecision;

    Int32 numPointers = (*event)->mNumPointers = o->mNumPointers;
    (*event)->mNumSamples = 1;

    (*event)->mLastDataSampleIndex = 0;
    (*event)->mLastEventTimeNanoSampleIndex = 0;

    //System.arraycopy(o.mPointerIdentifiers, 0, (*event)->mPointerIdentifiers, 0, numPointers);
    memcpy((*event)->mPointerIdentifiers, o->mPointerIdentifiers, sizeof(Int32) * numPointers);

    (*event)->mEventTimeNanoSamples[0] = o->mEventTimeNanoSamples[o->mLastEventTimeNanoSampleIndex];

    //System.arraycopy(o.mDataSamples, o.mLastDataSampleIndex, (*event)->mDataSamples, 0,
    //        numPointers * NUM_SAMPLE_DATA);
    memcpy((*event)->mDataSamples , o->mDataSamples + o->mLastEventTimeNanoSampleIndex,
        sizeof(Float) * numPointers * NUM_SAMPLE_DATA);

    return NOERROR;
}

CMotionEvent::~CMotionEvent()
{
    delete[] mPointerIdentifiers;
    delete[] mDataSamples;
    delete[] mEventTimeNanoSamples;
}

ECode CMotionEvent::constructor(
    /* [in] */ Int32 pointerCount,
    /* [in] */ Int32 sampleCount)
{
    mPointerIdentifiers = new Int32[pointerCount];
    if (mPointerIdentifiers == NULL) {
        return E_OUT_OF_MEMORY;
    }
    mPointerIdentifiersLength = pointerCount;

    mDataSamplesLength = pointerCount * sampleCount * NUM_SAMPLE_DATA;
    mDataSamples = new Float[mDataSamplesLength];
    if (mDataSamples == NULL) {
        mDataSamplesLength = 0;
        return E_OUT_OF_MEMORY;
    }

    mEventTimeNanoSamples = new Int64[sampleCount];
    if (mEventTimeNanoSamples == NULL) {
        return E_OUT_OF_MEMORY;
    }
    mEventTimeNanoSamplesLength = sampleCount;

    return NOERROR;
}

ECode CMotionEvent::GetDeviceId(
    /* [out] */ Int32* deviceId)
{
    if (deviceId == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *deviceId = InputEvent::GetDeviceId();

    return NOERROR;
}

ECode CMotionEvent::GetDevice(
    /* [out] */ IInputDevice** device)
{
    if (device == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *device = InputEvent::GetDevice();

    return NOERROR;
}

ECode CMotionEvent::GetSource(
    /* [out] */ Int32* source)
{
    if (source == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *source = InputEvent::GetSource();

    return NOERROR;
}

ECode CMotionEvent::SetSource(
    /* [in] */ Int32 source)
{
    InputEvent::SetSource(source);

    return NOERROR;
}

/**
 * Recycle the MotionEvent, to be re-used by a later caller.  After calling
 * this function you must not ever touch the event again.
 */
ECode CMotionEvent::Recycle()
{
    // Ensure recycle is only called once!
    if (TRACK_RECYCLED_LOCATION) {
        //if (mRecycledLocation != NULL) {
        //    throw new RuntimeException(toString() + " recycled twice!", mRecycledLocation);
        //}
        //mRecycledLocation = new RuntimeException("Last recycled here");
        ////Log.w("MotionEvent", "Recycling event " + this, mRecycledLocation);
    }
    else {
        if (mRecycled) {
            Logger::E("CMotionEvent", " recycled twice!");
            return E_RUNTIME_EXCEPTION;
        }
        mRecycled = TRUE;
    }

    Mutex::Autolock lock(gRecyclerLock);

    if (gRecyclerUsed < MAX_RECYCLED) {
        gRecyclerUsed++;
        mNumSamples = 0;
        mNext = gRecyclerTop;
        gRecyclerTop = this;
    }

    return NOERROR;
}

/**
 * Scales down the coordination of this event by the given scale.
 *
 * @hide
 */
ECode CMotionEvent::Scale(
    /* [in] */ Float scale)
{
    mXOffset *= scale;
    mYOffset *= scale;
    mXPrecision *= scale;
    mYPrecision *= scale;

    Int32 length = mNumPointers * mNumSamples * NUM_SAMPLE_DATA;
    for (Int32 i = 0; i < length; i += NUM_SAMPLE_DATA) {
        mDataSamples[i + SAMPLE_X] *= scale;
        mDataSamples[i + SAMPLE_Y] *= scale;
        // no need to scale pressure
        mDataSamples[i + SAMPLE_SIZE] *= scale;    // TODO: square this?
        mDataSamples[i + SAMPLE_TOUCH_MAJOR] *= scale;
        mDataSamples[i + SAMPLE_TOUCH_MINOR] *= scale;
        mDataSamples[i + SAMPLE_TOOL_MAJOR] *= scale;
        mDataSamples[i + SAMPLE_TOOL_MINOR] *= scale;
    }

    return NOERROR;
}

/**
 * Return the kind of action being performed -- one of either
 * {@link #ACTION_DOWN}, {@link #ACTION_MOVE}, {@link #ACTION_UP}, or
 * {@link #ACTION_CANCEL}.  Consider using {@link #getActionMasked}
 * and {@link #getActionIndex} to retrieve the separate masked action
 * and pointer index.
 */
ECode CMotionEvent::GetAction(
    /* [out] */ Int32* action)
{
    if (action == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *action = mAction;

    return NOERROR;
}

/**
 * Return the masked action being performed, without pointer index
 * information.  May be any of the actions: {@link #ACTION_DOWN},
 * {@link #ACTION_MOVE}, {@link #ACTION_UP}, {@link #ACTION_CANCEL},
 * {@link #ACTION_POINTER_DOWN}, or {@link #ACTION_POINTER_UP}.
 * Use {@link #getActionIndex} to return the index associated with
 * pointer actions.
 */
ECode CMotionEvent::GetActionMasked(
    /* [out] */ Int32* actionMasked)
{
    if (actionMasked == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *actionMasked = mAction & MotionEvent_ACTION_MASK;

    return NOERROR;
}

/**
 * For {@link #ACTION_POINTER_DOWN} or {@link #ACTION_POINTER_UP}
 * as returned by {@link #getActionMasked}, this returns the associated
 * pointer index.  The index may be used with {@link #getPointerId(Int32)},
 * {@link #getX(Int32)}, {@link #getY(Int32)}, {@link #getPressure(Int32)},
 * and {@link #getSize(Int32)} to get information about the pointer that has
 * gone down or up.
 */
ECode CMotionEvent::GetActionIndex(
    /* [out] */ Int32* actionIndex)
{
    if (actionIndex == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *actionIndex = (mAction & MotionEvent_ACTION_POINTER_INDEX_MASK)
        >> MotionEvent_ACTION_POINTER_INDEX_SHIFT;

    return NOERROR;
}

/**
 * Gets the motion event flags.
 *
 * @see #FLAG_WINDOW_IS_OBSCURED
 */
ECode CMotionEvent::GetFlags(
    /* [out] */ Int32* flags)
{
    if (flags == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *flags = mFlags;

    return NOERROR;
}

/**
 * Returns the time (in ms) when the user originally pressed down to start
 * a stream of position events.
 */
ECode CMotionEvent::GetDownTime(
    /* [out] */ Int64* downTime)
{
    if (downTime == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *downTime = mDownTimeNano / MS_PER_NS;

    return NOERROR;
}

/**
 * Returns the time (in ms) when this specific event was generated.
 */
ECode CMotionEvent::GetEventTime(
    /* [out] */ Int64* eventTime)
{
    if (eventTime == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *eventTime = mEventTimeNanoSamples[mLastEventTimeNanoSampleIndex] / MS_PER_NS;

    return NOERROR;
}

/**
 * Returns the time (in ns) when this specific event was generated.
 * The value is in nanosecond precision but it may not have nanosecond accuracy.
 *
 * @hide
 */
ECode CMotionEvent::GetEventTimeNano(
    /* [out] */ Int64* eventTimeNano)
{
    if (eventTimeNano == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *eventTimeNano = mEventTimeNanoSamples[mLastEventTimeNanoSampleIndex];

    return NOERROR;
}

/**
 * {@link #getX(Int32)} for the first pointer index (may be an
 * arbitrary pointer identifier).
 */
ECode CMotionEvent::GetX(
    /* [out] */ Float* x)
{
    if (x == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *x = mDataSamples[mLastDataSampleIndex + SAMPLE_X] + mXOffset;

    return NOERROR;
}

/**
 * {@link #getY(Int32)} for the first pointer index (may be an
 * arbitrary pointer identifier).
 */
ECode CMotionEvent::GetY(
    /* [out] */ Float* y)
{
    if (y == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *y = mDataSamples[mLastDataSampleIndex + SAMPLE_Y] + mYOffset;

    return NOERROR;
}

/**
 * {@link #getPressure(Int32)} for the first pointer index (may be an
 * arbitrary pointer identifier).
 */
ECode CMotionEvent::GetPressure(
    /* [out] */ Float* pressure)
{
    if (pressure == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *pressure = mDataSamples[mLastDataSampleIndex + SAMPLE_PRESSURE];

    return NOERROR;
}

/**
 * {@link #getSize(Int32)} for the first pointer index (may be an
 * arbitrary pointer identifier).
 */
ECode CMotionEvent::GetSize(
    /* [out] */ Float* size)
{
    if (size == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *size = mDataSamples[mLastDataSampleIndex + SAMPLE_SIZE];

    return NOERROR;
}

/**
 * {@link #getTouchMajor(Int32)} for the first pointer index (may be an
 * arbitrary pointer identifier).
 */
ECode CMotionEvent::GetTouchMajor(
    /* [out] */ Float* touchMajor)
{
    if (touchMajor == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *touchMajor = mDataSamples[mLastDataSampleIndex + SAMPLE_TOUCH_MAJOR];

    return NOERROR;
}

/**
 * {@link #getTouchMinor(Int32)} for the first pointer index (may be an
 * arbitrary pointer identifier).
 */
ECode CMotionEvent::GetTouchMinor(
    /* [out] */ Float* touchMinor)
{
    if (touchMinor == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *touchMinor = mDataSamples[mLastDataSampleIndex + SAMPLE_TOUCH_MINOR];

    return NOERROR;
}

/**
 * {@link #getToolMajor(Int32)} for the first pointer index (may be an
 * arbitrary pointer identifier).
 */
ECode CMotionEvent::GetToolMajor(
    /* [out] */ Float* toolMajor)
{
    if (toolMajor == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *toolMajor = mDataSamples[mLastDataSampleIndex + SAMPLE_TOOL_MAJOR];

    return NOERROR;
}

/**
 * {@link #getToolMinor(Int32)} for the first pointer index (may be an
 * arbitrary pointer identifier).
 */
ECode CMotionEvent::GetToolMinor(
    /* [out] */ Float* toolMinor)
{
    if (toolMinor == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *toolMinor = mDataSamples[mLastDataSampleIndex + SAMPLE_TOOL_MINOR];

    return NOERROR;
}

/**
 * {@link #getOrientation(Int32)} for the first pointer index (may be an
 * arbitrary pointer identifier).
 */
ECode CMotionEvent::GetOrientation(
    /* [out] */ Float* orientation)
{
    if (orientation == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *orientation = mDataSamples[mLastDataSampleIndex + SAMPLE_ORIENTATION];

    return NOERROR;
}

/**
 * The number of pointers of data contained in this event.  Always
 * >= 1.
 */
ECode CMotionEvent::GetPointerCount(
    /* [out] */ Int32* count)
{
    if (count == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *count = mNumPointers;

    return NOERROR;
}

/**
 * Return the pointer identifier associated with a particular pointer
 * data index is this event.  The identifier tells you the actual pointer
 * number associated with the data, accounting for individual pointers
 * going up and down since the start of the current gesture.
 * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
 * (the first pointer that is down) to {@link #getPointerCount()}-1.
 */
ECode CMotionEvent::GetPointerId(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Int32* pointerId)
{
    if (pointerId == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *pointerId = mPointerIdentifiers[pointerIndex];

    return NOERROR;
}

/**
 * Given a pointer identifier, find the index of its data in the event.
 *
 * @param pointerId The identifier of the pointer to be found.
 * @return Returns either the index of the pointer (for use with
 * {@link #getX(Int32)} et al.), or -1 if there is no data available for
 * that pointer identifier.
 */
ECode CMotionEvent::FindPointerIndex(
    /* [in] */ Int32 pointerId,
    /* [out] */ Int32* pointerIndex)
{
    if (pointerIndex == NULL) {
        return E_INVALID_ARGUMENT;
    }

    Int32 i = mNumPointers;
    Int32 index = -1;
    while (i > 0) {
        i--;
        if (mPointerIdentifiers[i] == pointerId) {
            index = i;
            break;
        }
    }

    *pointerIndex = index;

    return NOERROR;
}

/**
 * Returns the X coordinate of this event for the given pointer
 * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
 * identifier for this index).
 * Whole numbers are pixels; the
 * value may have a fraction for input devices that are sub-pixel precise.
 * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
 * (the first pointer that is down) to {@link #getPointerCount()}-1.
 */
ECode CMotionEvent::GetXEx(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Float* x)
{
    if (x == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *x = mDataSamples[mLastDataSampleIndex
        + pointerIndex * NUM_SAMPLE_DATA + SAMPLE_X] + mXOffset;

    return NOERROR;
}

/**
 * Returns the Y coordinate of this event for the given pointer
 * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
 * identifier for this index).
 * Whole numbers are pixels; the
 * value may have a fraction for input devices that are sub-pixel precise.
 * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
 * (the first pointer that is down) to {@link #getPointerCount()}-1.
 */
ECode CMotionEvent::GetYEx(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Float* y)
{
    if (y == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *y = mDataSamples[mLastDataSampleIndex
        + pointerIndex * NUM_SAMPLE_DATA + SAMPLE_Y] + mYOffset;

    return NOERROR;
}

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
ECode CMotionEvent::GetPressureEx(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Float* pressure)
{
    if (pressure == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *pressure = mDataSamples[mLastDataSampleIndex
        + pointerIndex * NUM_SAMPLE_DATA + SAMPLE_PRESSURE];

    return NOERROR;
}

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
ECode CMotionEvent::GetSizeEx(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Float* size)
{
    if (size == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *size = mDataSamples[mLastDataSampleIndex
        + pointerIndex * NUM_SAMPLE_DATA + SAMPLE_SIZE];

    return NOERROR;
}

/**
 * Returns the length of the major axis of an ellipse that describes the touch
 * area at the point of contact for the given pointer
 * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
 * identifier for this index).
 * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
 * (the first pointer that is down) to {@link #getPointerCount()}-1.
 */
ECode CMotionEvent::GetTouchMajorEx(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Float* touchMajor)
{
    if (touchMajor == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *touchMajor = mDataSamples[mLastDataSampleIndex
        + pointerIndex * NUM_SAMPLE_DATA + SAMPLE_TOUCH_MAJOR];

    return NOERROR;
}

/**
 * Returns the length of the minor axis of an ellipse that describes the touch
 * area at the point of contact for the given pointer
 * <em>index</em> (use {@link #getPointerId(Int32)} to find the pointer
 * identifier for this index).
 * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
 * (the first pointer that is down) to {@link #getPointerCount()}-1.
 */
ECode CMotionEvent::GetTouchMinorEx(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Float* touchMinor)
{
    if (touchMinor == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *touchMinor = mDataSamples[mLastDataSampleIndex
        + pointerIndex * NUM_SAMPLE_DATA + SAMPLE_TOUCH_MINOR];

    return NOERROR;
}

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
ECode CMotionEvent::GetToolMajorEx(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Float* toolMajor)
{
    if (toolMajor == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *toolMajor = mDataSamples[mLastDataSampleIndex
        + pointerIndex * NUM_SAMPLE_DATA + SAMPLE_TOOL_MAJOR];

    return NOERROR;
}

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
ECode CMotionEvent::GetToolMinorEx(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Float* toolMinor)
{
    if (toolMinor == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *toolMinor = mDataSamples[mLastDataSampleIndex
        + pointerIndex * NUM_SAMPLE_DATA + SAMPLE_TOOL_MINOR];

    return NOERROR;
}

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
ECode CMotionEvent::GetOrientationEx(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Float* orientation)
{
    if (orientation == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *orientation = mDataSamples[mLastDataSampleIndex
        + pointerIndex * NUM_SAMPLE_DATA + SAMPLE_ORIENTATION];

    return NOERROR;
}

/**
 * Populates a {@link PointerCoords} object with pointer coordinate data for
 * the specified pointer index.
 *
 * @param pointerIndex Raw index of pointer to retrieve.  Value may be from 0
 * (the first pointer that is down) to {@link #getPointerCount()}-1.
 * @param outPointerCoords The pointer coordinate object to populate.
 */
ECode CMotionEvent::GetPointerCoords(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Handle32 outPointerCoords)
{
    if (!outPointerCoords) {
        return E_INVALID_ARGUMENT;
    }

    Int32 sampleIndex = mLastDataSampleIndex + pointerIndex * NUM_SAMPLE_DATA;
    GetPointerCoordsAtSampleIndex(sampleIndex, (PointerCoords*)outPointerCoords);

    return NOERROR;
}

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
ECode CMotionEvent::GetMetaState(
    /* [out] */ Int32* metaState)
{
    if (metaState == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *metaState = mMetaState;

    return NOERROR;
}

/**
 * Returns the original raw X coordinate of this event.  For touch
 * events on the screen, this is the original location of the event
 * on the screen, before it had been adjusted for the containing window
 * and views.
 */
ECode CMotionEvent::GetRawX(
    /* [out] */ Float* rawX)
{
    if (rawX == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *rawX = mDataSamples[mLastDataSampleIndex + SAMPLE_X];

    return NOERROR;
}

/**
 * Returns the original raw Y coordinate of this event.  For touch
 * events on the screen, this is the original location of the event
 * on the screen, before it had been adjusted for the containing window
 * and views.
 */
ECode CMotionEvent::GetRawY(
    /* [out] */ Float* rawY)
{
    if (rawY == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *rawY = mDataSamples[mLastDataSampleIndex + SAMPLE_Y];

    return NOERROR;
}

/**
 * Return the precision of the X coordinates being reported.  You can
 * multiple this number with {@link #getX} to find the actual hardware
 * value of the X coordinate.
 * @return Returns the precision of X coordinates being reported.
 */
ECode CMotionEvent::GetXPrecision(
    /* [out] */ Float* xPrecision)
{
    if (xPrecision == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *xPrecision = mXPrecision;

    return NOERROR;
}

/**
 * Return the precision of the Y coordinates being reported.  You can
 * multiple this number with {@link #getY} to find the actual hardware
 * value of the Y coordinate.
 * @return Returns the precision of Y coordinates being reported.
 */
ECode CMotionEvent::GetYPrecision(
    /* [out] */ Float* yPrecision)
{
    if (yPrecision == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *yPrecision = mYPrecision;

    return NOERROR;
}

/**
 * Returns the number of historical points in this event.  These are
 * movements that have occurred between this event and the previous event.
 * This only applies to ACTION_MOVE events -- all other actions will have
 * a size of 0.
 *
 * @return Returns the number of historical points in the event.
 */
ECode CMotionEvent::GetHistorySize(
    /* [out] */ Int32* historySize)
{
    if (historySize == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *historySize = mLastEventTimeNanoSampleIndex;

    return NOERROR;
}

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
ECode CMotionEvent::GetHistoricalEventTime(
    /* [in] */ Int32 pos,
    /* [out] */ Int64* hEventTime)
{
    if (hEventTime == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hEventTime = mEventTimeNanoSamples[pos] / MS_PER_NS;

    return NOERROR;
}

/**
 * {@link #getHistoricalX(Int32)} for the first pointer index (may be an
 * arbitrary pointer identifier).
 */
ECode CMotionEvent::GetHistoricalX(
    /* [in] */ Int32 pos,
    /* [out] */ Float* hX)
{
    if (hX == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hX = mDataSamples[pos * mNumPointers * NUM_SAMPLE_DATA + SAMPLE_X] + mXOffset;

    return NOERROR;
}

/**
 * {@link #getHistoricalY(Int32)} for the first pointer index (may be an
 * arbitrary pointer identifier).
 */
ECode CMotionEvent::GetHistoricalY(
    /* [in] */ Int32 pos,
    /* [out] */ Float* hY)
{
    if (hY == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hY = mDataSamples[pos * mNumPointers * NUM_SAMPLE_DATA + SAMPLE_Y] + mYOffset;

    return NOERROR;
}

/**
 * {@link #getHistoricalPressure(Int32)} for the first pointer index (may be an
 * arbitrary pointer identifier).
 */
ECode CMotionEvent::GetHistoricalPressure(
    /* [in] */ Int32 pos,
    /* [out] */ Float* hPressure)
{
    if (hPressure == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hPressure = mDataSamples[pos * mNumPointers * NUM_SAMPLE_DATA + SAMPLE_PRESSURE];

    return NOERROR;
}

/**
 * {@link #getHistoricalSize(Int32)} for the first pointer index (may be an
 * arbitrary pointer identifier).
 */
ECode CMotionEvent::GetHistoricalSize(
    /* [in] */ Int32 pos,
    /* [out] */ Float* hSize)
{
    if (hSize == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hSize = mDataSamples[pos * mNumPointers * NUM_SAMPLE_DATA + SAMPLE_SIZE];

    return NOERROR;
}

/**
 * {@link #getHistoricalTouchMajor(Int32)} for the first pointer index (may be an
 * arbitrary pointer identifier).
 */
ECode CMotionEvent::GetHistoricalTouchMajor(
    /* [in] */ Int32 pos,
    /* [out] */ Float* hTouchMajor)
{
    if (hTouchMajor == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hTouchMajor = mDataSamples[pos * mNumPointers * NUM_SAMPLE_DATA + SAMPLE_TOUCH_MAJOR];

    return NOERROR;
}

/**
 * {@link #getHistoricalTouchMinor(Int32)} for the first pointer index (may be an
 * arbitrary pointer identifier).
 */
ECode CMotionEvent::GetHistoricalTouchMinor(
    /* [in] */ Int32 pos,
    /* [out] */ Float* hTouchMinor)
{
    if (hTouchMinor == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hTouchMinor = mDataSamples[pos * mNumPointers * NUM_SAMPLE_DATA + SAMPLE_TOUCH_MINOR];

    return NOERROR;
}

/**
 * {@link #getHistoricalToolMajor(Int32)} for the first pointer index (may be an
 * arbitrary pointer identifier).
 */
ECode CMotionEvent::GetHistoricalToolMajor(
    /* [in] */ Int32 pos,
    /* [out] */ Float* hToolMajor)
{
    if (hToolMajor == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hToolMajor = mDataSamples[pos * mNumPointers * NUM_SAMPLE_DATA + SAMPLE_TOOL_MAJOR];

    return NOERROR;
}

/**
 * {@link #getHistoricalToolMinor(Int32)} for the first pointer index (may be an
 * arbitrary pointer identifier).
 */
ECode CMotionEvent::GetHistoricalToolMinor(
    /* [in] */ Int32 pos,
    /* [out] */ Float* hToolMinor)
{
    if (hToolMinor == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hToolMinor = mDataSamples[pos * mNumPointers * NUM_SAMPLE_DATA + SAMPLE_TOOL_MINOR];

    return NOERROR;
}

/**
 * {@link #getHistoricalOrientation(Int32)} for the first pointer index (may be an
 * arbitrary pointer identifier).
 */
ECode CMotionEvent::GetHistoricalOrientation(
    /* [in] */ Int32 pos,
    /* [out] */ Float* hOrientation)
{
    if (hOrientation == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hOrientation = mDataSamples[pos * mNumPointers * NUM_SAMPLE_DATA + SAMPLE_ORIENTATION];

    return NOERROR;
}

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
ECode CMotionEvent::GetHistoricalXEx(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [out] */ Float* hX)
{
    if (hX == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hX = mDataSamples[(pos * mNumPointers + pointerIndex)
        * NUM_SAMPLE_DATA + SAMPLE_X] + mXOffset;

    return NOERROR;
}

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
ECode CMotionEvent::GetHistoricalYEx(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [out] */ Float* hY)
{
    if (hY == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hY = mDataSamples[(pos * mNumPointers + pointerIndex)
        * NUM_SAMPLE_DATA + SAMPLE_Y] + mYOffset;

    return NOERROR;
}

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
ECode CMotionEvent::GetHistoricalPressureEx(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [out] */ Float* hPressure)
{
    if (hPressure == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hPressure = mDataSamples[(pos * mNumPointers + pointerIndex)
        * NUM_SAMPLE_DATA + SAMPLE_PRESSURE];;

    return NOERROR;
}

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
ECode CMotionEvent::GetHistoricalSizeEx(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [out] */ Float* hSize)
{
    if (hSize == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hSize = mDataSamples[(pos * mNumPointers + pointerIndex)
        * NUM_SAMPLE_DATA + SAMPLE_SIZE];

    return NOERROR;
}

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
ECode CMotionEvent::GetHistoricalTouchMajorEx(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [out] */ Float* hTouchMajor)
{
    if (hTouchMajor == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hTouchMajor = mDataSamples[(pos * mNumPointers + pointerIndex)
        * NUM_SAMPLE_DATA + SAMPLE_TOUCH_MAJOR];

    return NOERROR;
}

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
ECode CMotionEvent::GetHistoricalTouchMinorEx(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [out] */ Float* hTouchMinor)
{
    if (hTouchMinor == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hTouchMinor = mDataSamples[(pos * mNumPointers + pointerIndex)
        * NUM_SAMPLE_DATA + SAMPLE_TOUCH_MINOR];

    return NOERROR;
}

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
ECode CMotionEvent::GetHistoricalToolMajorEx(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [out] */ Float* hToolMajor)
{
    if (hToolMajor == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hToolMajor = mDataSamples[(pos * mNumPointers + pointerIndex)
        * NUM_SAMPLE_DATA + SAMPLE_TOOL_MAJOR];

    return NOERROR;
}

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
ECode CMotionEvent::GetHistoricalToolMinorEx(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [out] */ Float* hToolMinorEx)
{
    if (hToolMinorEx == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hToolMinorEx = mDataSamples[(pos * mNumPointers + pointerIndex)
        * NUM_SAMPLE_DATA + SAMPLE_TOOL_MINOR];

    return NOERROR;
}

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
ECode CMotionEvent::GetHistoricalOrientationEx(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [out] */ Float* hOrientation)
{
    if (hOrientation == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hOrientation = mDataSamples[(pos * mNumPointers + pointerIndex)
        * NUM_SAMPLE_DATA + SAMPLE_ORIENTATION];

    return NOERROR;
}

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
ECode CMotionEvent::GetHistoricalPointerCoords(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [in] */ Handle32 outPointerCoords)
{
    if (!outPointerCoords) {
        return E_INVALID_ARGUMENT;
    }

    Int32 sampleIndex = (pos * mNumPointers + pointerIndex) * NUM_SAMPLE_DATA;
    GetPointerCoordsAtSampleIndex(sampleIndex, (PointerCoords*)outPointerCoords);

    return NOERROR;
}

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
ECode CMotionEvent::GetEdgeFlags(
    /* [out] */ Int32* edgeFlags)
{
    if (edgeFlags == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *edgeFlags = mEdgeFlags;

    return NOERROR;
}


/**
 * Sets the bitfield indicating which edges, if any, were touched by this
 * MotionEvent.
 *
 * @see #getEdgeFlags()
 */
ECode CMotionEvent::SetEdgeFlags(
    /* [in] */ Int32 flags)
{
    mEdgeFlags = flags;

    return NOERROR;
}

/**
 * Sets this event's action.
 */
ECode CMotionEvent::SetAction(
    /* [in] */ Int32 action)
{
    mAction = action;

    return NOERROR;
}

/**
 * Adjust this event's location.
 * @param deltaX Amount to add to the current X coordinate of the event.
 * @param deltaY Amount to add to the current Y coordinate of the event.
 */
ECode CMotionEvent::OffsetLocation(
    /* [in] */ Float deltaX,
    /* [in] */ Float deltaY)
{
    mXOffset += deltaX;
    mYOffset += deltaY;

    return NOERROR;
}

/**
 * Set this event's location.  Applies {@link #offsetLocation} with a
 * delta from the current location to the given new location.
 *
 * @param x New absolute X location.
 * @param y New absolute Y location.
 */
ECode CMotionEvent::SetLocation(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    mXOffset = x - mDataSamples[mLastDataSampleIndex + SAMPLE_X];
    mYOffset = y - mDataSamples[mLastDataSampleIndex + SAMPLE_Y];

    return NOERROR;
}

void CMotionEvent::GetPointerCoordsAtSampleIndex(
    /* [in] */ Int32 sampleIndex,
    /* [in] */ PointerCoords* outPointerCoords)
{
    assert(outPointerCoords);
    outPointerCoords->mX = mDataSamples[sampleIndex + SAMPLE_X] + mXOffset;
    outPointerCoords->mY = mDataSamples[sampleIndex + SAMPLE_Y] + mYOffset;
    outPointerCoords->mPressure = mDataSamples[sampleIndex + SAMPLE_PRESSURE];
    outPointerCoords->mSize = mDataSamples[sampleIndex + SAMPLE_SIZE];
    outPointerCoords->mTouchMajor = mDataSamples[sampleIndex + SAMPLE_TOUCH_MAJOR];
    outPointerCoords->mTouchMinor = mDataSamples[sampleIndex + SAMPLE_TOUCH_MINOR];
    outPointerCoords->mToolMajor = mDataSamples[sampleIndex + SAMPLE_TOOL_MAJOR];
    outPointerCoords->mToolMinor = mDataSamples[sampleIndex + SAMPLE_TOOL_MINOR];
    outPointerCoords->mOrientation = mDataSamples[sampleIndex + SAMPLE_ORIENTATION];
}

void CMotionEvent::SetPointerCoordsAtSampleIndex(
    /* [in] */ Int32 sampleIndex,
    /* [in] */ PointerCoords** pointerCoords)
{
    for (Int32 i = 0; i < mNumPointers; i++) {
        SetPointerCoordsAtSampleIndex(sampleIndex, pointerCoords[i]);
        sampleIndex += NUM_SAMPLE_DATA;
    }
}

void CMotionEvent::SetPointerCoordsAtSampleIndex(
    /* [in] */ Int32 sampleIndex,
    /* [in] */ PointerCoords* pointerCoords)
{
    mDataSamples[sampleIndex + SAMPLE_X] = pointerCoords->mX - mXOffset;
    mDataSamples[sampleIndex + SAMPLE_Y] = pointerCoords->mY - mYOffset;
    mDataSamples[sampleIndex + SAMPLE_PRESSURE] = pointerCoords->mPressure;
    mDataSamples[sampleIndex + SAMPLE_SIZE] = pointerCoords->mSize;
    mDataSamples[sampleIndex + SAMPLE_TOUCH_MAJOR] = pointerCoords->mTouchMajor;
    mDataSamples[sampleIndex + SAMPLE_TOUCH_MINOR] = pointerCoords->mTouchMinor;
    mDataSamples[sampleIndex + SAMPLE_TOOL_MAJOR] = pointerCoords->mToolMajor;
    mDataSamples[sampleIndex + SAMPLE_TOOL_MINOR] = pointerCoords->mToolMinor;
    mDataSamples[sampleIndex + SAMPLE_ORIENTATION] = pointerCoords->mOrientation;
}

void CMotionEvent::SetPointerCoordsAtSampleIndex(
    /* [in] */ Int32 sampleIndex,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float pressure,
    /* [in] */ Float size)
{
    mDataSamples[sampleIndex + SAMPLE_X] = x - mXOffset;
    mDataSamples[sampleIndex + SAMPLE_Y] = y - mYOffset;
    mDataSamples[sampleIndex + SAMPLE_PRESSURE] = pressure;
    mDataSamples[sampleIndex + SAMPLE_SIZE] = size;
    mDataSamples[sampleIndex + SAMPLE_TOUCH_MAJOR] = pressure;
    mDataSamples[sampleIndex + SAMPLE_TOUCH_MINOR] = pressure;
    mDataSamples[sampleIndex + SAMPLE_TOOL_MAJOR] = size;
    mDataSamples[sampleIndex + SAMPLE_TOOL_MINOR] = size;
    mDataSamples[sampleIndex + SAMPLE_ORIENTATION] = 0;
}

void CMotionEvent::IncrementNumSamplesAndReserveStorage(
    /* [in] */ Int32 dataSampleStride)
{
    if (mNumSamples == mEventTimeNanoSamplesLength) {
        Int64* newEventTimeNanoSamples = new Int64[mNumSamples + BASE_AVAIL_SAMPLES];
        mEventTimeNanoSamplesLength = mNumSamples + BASE_AVAIL_SAMPLES;
        //System.arraycopy(mEventTimeNanoSamples, 0, newEventTimeNanoSamples, 0, mNumSamples);
        memcpy(newEventTimeNanoSamples, mEventTimeNanoSamples, sizeof(Int64) * mNumSamples);
        delete[] mEventTimeNanoSamples;
        mEventTimeNanoSamples = newEventTimeNanoSamples;
    }

    Int32 nextDataSampleIndex = mLastDataSampleIndex + dataSampleStride;
    if (nextDataSampleIndex + dataSampleStride > mDataSamplesLength) {
        Float* newDataSamples = new Float[nextDataSampleIndex
                                           + BASE_AVAIL_SAMPLES * dataSampleStride];
        mDataSamplesLength = nextDataSampleIndex + BASE_AVAIL_SAMPLES * dataSampleStride;
        //System.arraycopy(mDataSamples, 0, newDataSamples, 0, nextDataSampleIndex);
        memcpy(newDataSamples, mDataSamples, sizeof(Float) * nextDataSampleIndex);
        delete[] mDataSamples;
        mDataSamples = newDataSamples;
    }

    mLastEventTimeNanoSampleIndex = mNumSamples;
    mLastDataSampleIndex = nextDataSampleIndex;
    mNumSamples += 1;
}

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
ECode CMotionEvent::AddBatch(
    /* [in] */ Int64 eventTime,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float pressure,
    /* [in] */ Float size,
    /* [in] */ Int32 metaState)
{
    IncrementNumSamplesAndReserveStorage(NUM_SAMPLE_DATA);

    mEventTimeNanoSamples[mLastEventTimeNanoSampleIndex] = eventTime * MS_PER_NS;
    SetPointerCoordsAtSampleIndex(mLastDataSampleIndex, x, y, pressure, size);

    mMetaState |= metaState;

    return NOERROR;
}

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
ECode CMotionEvent::AddBatch(
    /* [in] */ Int64 eventTime,
    /* [in] */ PointerCoords** pointerCoords,
    /* [in] */ Int32 metaState)
{
    Int32 dataSampleStride = mNumPointers * NUM_SAMPLE_DATA;
    IncrementNumSamplesAndReserveStorage(dataSampleStride);

    mEventTimeNanoSamples[mLastEventTimeNanoSampleIndex] = eventTime * MS_PER_NS;
    SetPointerCoordsAtSampleIndex(mLastDataSampleIndex, pointerCoords);

    mMetaState |= metaState;

    return NOERROR;
}

/** @hide */
ECode CMotionEvent::CreateFromParcelBody(
    /* [in] */ IParcel* in,
    /* [out] */ CMotionEvent** event)
{
    Int32 NP;
    in->ReadInt32(&NP);

    Int32 NS;
    in->ReadInt32(&NS);

    Int32 NI= NP * NS * NUM_SAMPLE_DATA;

    ECode ec = Obtain(NP, NS, event);
    if (FAILED(ec)) {
        return ec;
    }

    (*event)->mNumPointers = NP;
    (*event)->mNumSamples = NS;

    (*event)->ReadBaseFromParcel(in);

    in->ReadInt64(&(*event)->mDownTimeNano);
    in->ReadInt32(&(*event)->mAction);
    in->ReadFloat(&(*event)->mXOffset);
    in->ReadFloat(&(*event)->mYOffset);
    in->ReadFloat(&(*event)->mXPrecision);
    in->ReadFloat(&(*event)->mYPrecision);
    in->ReadInt32(&(*event)->mEdgeFlags);
    in->ReadInt32(&(*event)->mMetaState);
    in->ReadInt32(&(*event)->mFlags);

    for (Int32 i = 0; i < NP; i++) {
        in->ReadInt32((*event)->mPointerIdentifiers + i);
    }

    for (Int32 i = 0; i < NS; i++) {
        in->ReadInt64((*event)->mEventTimeNanoSamples + i);
    }

    for (Int32 i = 0; i < NI; i++) {
        in->ReadFloat((*event)->mDataSamples + i);
    }

    (*event)->mLastEventTimeNanoSampleIndex = NS - 1;
    (*event)->mLastDataSampleIndex = (NS - 1) * NP * NUM_SAMPLE_DATA;

    return NOERROR;
}

ECode CMotionEvent::WriteToParcel(
    /* [in] */ IParcel* out)
{
    out->WriteInt32(PARCEL_TOKEN_MOTION_EVENT);

    Int32 NP = mNumPointers;
    Int32 NS = mNumSamples;
    Int32 NI = NP * NS * NUM_SAMPLE_DATA;

    out->WriteInt32(NP);
    out->WriteInt32(NS);

    WriteBaseToParcel(out);

    out->WriteInt64(mDownTimeNano);
    out->WriteInt32(mAction);
    out->WriteFloat(mXOffset);
    out->WriteFloat(mYOffset);
    out->WriteFloat(mXPrecision);
    out->WriteFloat(mYPrecision);
    out->WriteInt32(mEdgeFlags);
    out->WriteInt32(mMetaState);
    out->WriteInt32(mFlags);

    for (Int32 i = 0; i < NP; i++) {
        out->WriteInt32(mPointerIdentifiers[i]);
    }

    for (Int32 i = 0; i < NS; i++) {
        out->WriteInt64(mEventTimeNanoSamples[i]);
    }

    for (Int32 i = 0; i < NI; i++) {
        out->WriteFloat(mDataSamples[i]);
    }

    return NOERROR;
}

ECode CMotionEvent::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    Int32 parcelToken;
    source->ReadInt32(&parcelToken);

    Int32 NP;
    source->ReadInt32(&NP);

    Int32 NS;
    source->ReadInt32(&NS);

    Int32 NI= NP * NS * NUM_SAMPLE_DATA;

    mNumPointers = NP;
    mNumSamples = NS;

    ReadBaseFromParcel(source);

    source->ReadInt64(&mDownTimeNano);
    source->ReadInt32(&mAction);
    source->ReadFloat(&mXOffset);
    source->ReadFloat(&mYOffset);
    source->ReadFloat(&mXPrecision);
    source->ReadFloat(&mYPrecision);
    source->ReadInt32(&mEdgeFlags);
    source->ReadInt32(&mMetaState);
    source->ReadInt32(&mFlags);

    for (Int32 i = 0; i < NP; i++) {
        source->ReadInt32(mPointerIdentifiers + i);
    }

    for (Int32 i = 0; i < NS; i++) {
        source->ReadInt64(mEventTimeNanoSamples + i);
    }

    for (Int32 i = 0; i < NI; i++) {
        source->ReadFloat(mDataSamples + i);
    }

    mLastEventTimeNanoSampleIndex = NS - 1;
    mLastDataSampleIndex = (NS - 1) * NP * NUM_SAMPLE_DATA;

    return NOERROR;
}
