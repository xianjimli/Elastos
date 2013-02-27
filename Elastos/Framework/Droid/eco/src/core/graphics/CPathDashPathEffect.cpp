
#include "graphics/CPathDashPathEffect.h"
#include <skia/effects/Sk1DPathEffect.h>

ECode CPathDashPathEffect::constructor(
    /* [in] */ IPath * pShape,
    /* [in] */ Float advance,
    /* [in] */ Float phase,
    /* [in] */ PathDashPathEffectStyle style)
{
    mNativeInstance = NativeCreate(
                        ((CPath*)pShape)->Ni(),
                        advance,
                        phase,
                        style);
    return NOERROR;
}

SkPathEffect* CPathDashPathEffect::NativeCreate(
    /* [in] */ SkPath* native_path,
    /* [in] */ Float advance,
    /* [in] */ Float phase,
    /* [in] */ Int32 native_style)
{
    SkASSERT(native_path != NULL);

    return new SkPath1DPathEffect(
                    *native_path,
                    SkFloatToScalar(advance),
                    SkFloatToScalar(phase),
                    (SkPath1DPathEffect::Style)native_style);
}