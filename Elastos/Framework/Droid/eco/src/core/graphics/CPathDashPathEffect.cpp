
#include "graphics/CPathDashPathEffect.h"
#include <skia/effects/Sk1DPathEffect.h>

ECode CPathDashPathEffect::constructor(
    /* [in] */ IPath* shape,
    /* [in] */ Float advance,
    /* [in] */ Float phase,
    /* [in] */ PathDashPathEffectStyle style)
{
    mNativeInstance = NativeCreate(
                        ((CPath*)shape)->Ni(),
                        advance,
                        phase,
                        style);
    return NOERROR;
}

PInterface CPathDashPathEffect::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_PathEffect) {
        return reinterpret_cast<PInterface>((PathEffect*)this);
    }
    return _CPathDashPathEffect::Probe(riid);
}

SkPathEffect* CPathDashPathEffect::NativeCreate(
    /* [in] */ SkPath* nativePath,
    /* [in] */ Float advance,
    /* [in] */ Float phase,
    /* [in] */ Int32 nativeStyle)
{
    SkASSERT(nativePath != NULL);

    return new SkPath1DPathEffect(
                    *nativePath,
                    SkFloatToScalar(advance),
                    SkFloatToScalar(phase),
                    (SkPath1DPathEffect::Style)nativeStyle);
}
