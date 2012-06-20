
#ifndef __INTERPOLATOR_H__
#define __INTERPOLATOR_H__

#ifdef _linux
#include <SkInterpolator.h>
#endif
#include <elastos.h>

using namespace Elastos;

class Interpolator
{
public:
    enum Result
    {
        NORMAL,
        FREEZE_START,
        FREEZE_END
    };

public:
    Interpolator(
        /* [in] */ Int32 valueCount,
        /* [in] */ Int32 frameCount);

    /**
     * Calls timeToValues(msec, values) with the msec set to now (by calling
     * (int)SystemClock.uptimeMillis().)
     */
    CARAPI_(Result) TimeToValues(
        /* [in] */ ArrayOf<Float>* values);

    /**
     * Given a millisecond time value (msec), return the interpolated values and
     * return whether the specified time was within the range of key times
     * (NORMAL), was before the first key time (FREEZE_START) or after the last
     * key time (FREEZE_END). In any event, computed values are always returned.
     *
     * @param msec The time (in milliseconds) used to sample into the
     *        Interpolator. Based on the SystemClock.uptimeMillis() clock
     * @param values Where to write the computed values (may be NULL).
     * @return how the values were computed (even if values == null)
     */
    CARAPI_(Result) TimeToValues(
        /* [in] */ Int32 msec,
        /* [in] */ ArrayOf<Float>* values);

private:
    Int32 mValueCount;
    Int32 mFrameCount;
    SkInterpolator* mSkInstance;
};

#endif //__INTERPOLATOR_H__