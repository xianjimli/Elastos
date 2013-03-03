
#include "graphics/CCornerPathEffect.h"
#include <skia/effects/SkCornerPathEffect.h>


ECode CCornerPathEffect::constructor(
    /* [in] */ Float radius)
{
    mNativeInstance = NativeCreate(radius);
    return NOERROR;
}

PInterface CCornerPathEffect::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_PathEffect) {
        return reinterpret_cast<PInterface>((PathEffect*)this);
    }
    return _CCornerPathEffect::Probe(riid);
}

SkPathEffect* CCornerPathEffect::NativeCreate(
    /* [in] */ Float radius)
{
    return new SkCornerPathEffect(
                SkFloatToScalar(radius));
}
