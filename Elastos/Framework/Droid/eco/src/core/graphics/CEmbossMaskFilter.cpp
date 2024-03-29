
#include "graphics/CEmbossMaskFilter.h"
#include <skia/effects/SkBlurMaskFilter.h>


ECode CEmbossMaskFilter::constructor(
    /* [in] */ const ArrayOf<Float>& direction,
    /* [in] */ Float ambient,
    /* [in] */ Float specular,
    /* [in] */ Float blurRadius)
{
    if (direction.GetLength() < 3) {
        // throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    mNativeInstance = NativeConstructor(direction, ambient, specular, blurRadius);
    return NOERROR;
}

PInterface CEmbossMaskFilter::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_MaskFilter) {
        return reinterpret_cast<PInterface>((MaskFilter*)this);
    }
    return _CEmbossMaskFilter::Probe(riid);
}

SkMaskFilter* CEmbossMaskFilter::NativeConstructor(
    /* [in] */ const ArrayOf<Float>& dirArray,
    /* [in] */ Float ambient,
    /* [in] */ Float specular,
    /* [in] */ Float blurRadius)
{
    SkScalar direction[3];
    for (Int32 i = 0; i < 3; i++) {
        direction[i] = SkFloatToScalar(dirArray[i]);
    }

    SkMaskFilter* filter =  SkBlurMaskFilter::CreateEmboss(
                                direction,
                                SkFloatToScalar(ambient),
                                SkFloatToScalar(specular),
                                SkFloatToScalar(blurRadius));
    return filter;
}
