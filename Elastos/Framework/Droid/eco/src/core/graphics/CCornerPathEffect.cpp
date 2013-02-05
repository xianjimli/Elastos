
#include "graphics/CCornerPathEffect.h"
#include <skia/effects/SkCornerPathEffect.h>

ECode CCornerPathEffect::constructor(
    /* [in] */ Float radius)
{
    mNativeInstance = nativeCreate(radius);
    return NOERROR;
}

SkPathEffect* CCornerPathEffect::nativeCreate(Float radius)
{
    return new SkCornerPathEffect(SkFloatToScalar(radius));
}
