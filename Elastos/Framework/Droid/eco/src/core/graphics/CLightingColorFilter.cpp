
#include "graphics/CLightingColorFilter.h"

ECode CLightingColorFilter::constructor(
    /* [in] */ Int32 mul,
    /* [in] */ Int32 add)
{
    mNativeInstance = native_CreateLightingFilter(mul, add);
    return NOERROR;
}

SkColorFilter* CLightingColorFilter::native_CreateLightingFilter(Int32 mul, Int32 add)
{
    return SkColorFilter::CreateLightingFilter(mul, add);
}