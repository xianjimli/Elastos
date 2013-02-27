
#include "graphics/CCornerPathEffect.h"
#include <skia/effects/SkCornerPathEffect.h>

ECode CCornerPathEffect::constructor(
    /* [in] */ Float radius)
{
    mNativeInstance = NativeCreate(radius);
    return NOERROR;
}

SkPathEffect* CCornerPathEffect::NativeCreate(
    /* [in] */ Float radius)
{
    return new SkCornerPathEffect(
                SkFloatToScalar(radius));
}
