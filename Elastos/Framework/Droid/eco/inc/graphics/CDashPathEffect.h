
#ifndef __CDASHPATHEFFECT_H__
#define __CDASHPATHEFFECT_H__

#include "_CDashPathEffect.h"
#include "graphics/PathEffect.h"

CarClass(CDashPathEffect), public PathEffect
{
public:
    /**
     * The intervals array must contain an even number of entries (>=2), with
     * the even indices specifying the "on" intervals, and the odd indices
     * specifying the "off" intervals. phase is an offset into the intervals
     * array (mod the sum of all of the intervals). The intervals array
     * controls the length of the dashes. The paint's strokeWidth controls the
     * thickness of the dashes.
     * Note: this patheffect only affects drawing with the paint's style is set
     * to STROKE or STROKE_AND_FILL. It is ignored if the drawing is done with
     * style == FILL.
     * @param intervals array of ON and OFF distances
     * @param phase offset into the intervals array
     */
    CARAPI constructor(
        /* [in] */ const ArrayOf<Float>& intervals,
        /* [in] */ Float phase);

private:
    static CARAPI_(SkPathEffect*) NativeCreate(
        /* [in] */ const ArrayOf<Float>& intervals,
        /* [in] */ Float phase);
};

#endif //__CDASHPATHEFFECT_H__
