
#include "graphics/CBlurMaskFilter.h"
#include <skia/effects/SkBlurMaskFilter.h>

ECode CBlurMaskFilter::Blur(
    /* [in] */ Int32 value)
{
    native_int = value;
    return NOERROR;
}

ECode CBlurMaskFilter::constructor(
    /* [in] */ Float radius,
    /* [in] */ Int32 style)
{
    mNativeInstance = NativeConstructor(radius, style);
    return NOERROR;
}

SkMaskFilter* CBlurMaskFilter::NativeConstructor(
    /* [in] */ Float radius,
    /* [in] */ Int32 style)
{
    SkMaskFilter* filter = SkBlurMaskFilter::Create(
                                SkFloatToScalar(radius),
                                (SkBlurMaskFilter::BlurStyle)style);

    return filter;
}