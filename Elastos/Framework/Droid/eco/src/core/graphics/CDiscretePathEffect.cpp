
#include "graphics/CDiscretePathEffect.h"
#include <skia/effects/SkDiscretePathEffect.h>


ECode CDiscretePathEffect::constructor(
    /* [in] */ Float segmentLength,
    /* [in] */ Float deviation)
{
    mNativeInstance = NativeCreate(
                        segmentLength,
                        deviation);
    return NOERROR;
}

PInterface CDiscretePathEffect::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_PathEffect) {
        return reinterpret_cast<PInterface>((PathEffect*)this);
    }
    return _CDiscretePathEffect::Probe(riid);
}

SkPathEffect* CDiscretePathEffect::NativeCreate(
    /* [in] */ Float length,
    /* [in] */ Float deviation)
{
    return new SkDiscretePathEffect(
                SkFloatToScalar(length),
                SkFloatToScalar(deviation));
}
