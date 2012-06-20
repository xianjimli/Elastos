
#include "graphics/Interpolator.h"
#include "os/SystemClock.h"

Interpolator::Interpolator(
    /* [in] */ Int32 valueCount,
    /* [in] */ Int32 frameCount) :
    mValueCount(valueCount),
    mFrameCount(frameCount)
{
    mSkInstance = new SkInterpolator(valueCount, frameCount);
}

Interpolator::Result Interpolator::TimeToValues(
    /* [in] */ ArrayOf<Float>* values)
{
    return TimeToValues((Int32)SystemClock::GetUptimeMillis(), values);
}

Interpolator::Result Interpolator::TimeToValues(
    /* [in] */ Int32 msec,
    /* [in] */ ArrayOf<Float>* values)
{
    Int32 valueCount = values->GetLength();
    if (values != NULL) {
        assert(valueCount >= mValueCount);
    }

    SkInterpolatorBase::Result result =
            mSkInstance->timeToValues(msec, (SkScalar*)values->GetPayload());
    if (values) {
        for (Int32 i = 0; i < valueCount; i++) {
            (*values)[i] = SkScalarToFloat(*(SkScalar*)&(*values)[i]);
        }
    }
    if (result == 0) return NORMAL;
    else if (result == 1) return FREEZE_START;
    else return FREEZE_END;
}
