
#include "ext/frameworkdef.h"
#include "view/CMotionEventHelper.h"
#include "view/CMotionEvent.h"

/**
 * Create a new MotionEvent, copying from an existing one.
 */
ECode CMotionEventHelper::Obtain(
    /* [in] */ IMotionEvent* _o,
    /* [out] */ IMotionEvent** _event)
{
    CMotionEvent* o = (CMotionEvent*)_o;
    CMotionEvent* event;
    ECode ec = CMotionEvent::Obtain(o->mNumPointers, o->mNumSamples, &event);
    if (FAILED(ec)) {
        return ec;
    }

    event->mDeviceId = o->mDeviceId;
    event->mSource = o->mSource;
    event->mEdgeFlags = o->mEdgeFlags;
    event->mDownTimeNano = o->mDownTimeNano;
    event->mAction = o->mAction;
    event->mFlags = o->mFlags;
    event->mMetaState = o->mMetaState;
    event->mXOffset = o->mXOffset;
    event->mYOffset = o->mYOffset;
    event->mXPrecision = o->mXPrecision;
    event->mYPrecision = o->mYPrecision;
    Int32 numPointers = event->mNumPointers = o->mNumPointers;
    Int32 numSamples = event->mNumSamples = o->mNumSamples;

    event->mLastDataSampleIndex = o->mLastDataSampleIndex;
    event->mLastEventTimeNanoSampleIndex = o->mLastEventTimeNanoSampleIndex;

    //System.arraycopy(o.mPointerIdentifiers, 0, (*event)->mPointerIdentifiers, 0, numPointers);
    memcpy(event->mPointerIdentifiers, o->mPointerIdentifiers, sizeof(Int32) * numPointers);

    //System.arraycopy(o.mEventTimeNanoSamples, 0, (*event)->mEventTimeNanoSamples, 0, numSamples);
    memcpy(event->mEventTimeNanoSamples, o->mEventTimeNanoSamples, sizeof(Int64) * numPointers);

    //System.arraycopy(o.mDataSamples, 0, (*event)->mDataSamples, 0,
    //        numPointers * numSamples * NUM_SAMPLE_DATA);
    memcpy(event->mDataSamples, o->mDataSamples,
        sizeof(Float) * numPointers * numSamples * CMotionEvent::NUM_SAMPLE_DATA);

    *_event = (IMotionEvent*)event;
    return NOERROR;
}
