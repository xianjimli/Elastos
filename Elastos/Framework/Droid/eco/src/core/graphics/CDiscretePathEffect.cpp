
#include "graphics/CDiscretePathEffect.h"
#include <skia/effects/SkDiscretePathEffect.h>

ECode CDiscretePathEffect::constructor(
    /* [in] */ Float segmentLength,
    /* [in] */ Float deviation)
{
    mNativeInstance = nativeCreate(segmentLength, deviation);
    return NOERROR;
}

SkPathEffect* CDiscretePathEffect::nativeCreate(float length, float deviation)
{
	return new SkDiscretePathEffect(SkFloatToScalar(length),
																	SkFloatToScalar(deviation));
}
