
#include "graphics/CLightingColorFilter.h"

ECode CLightingColorFilter::constructor(
    /* [in] */ Int32 mul,
    /* [in] */ Int32 add)
{
    mNativeInstance = Native_CreateLightingFilter(mul, add);
    return NOERROR;
}

SkColorFilter* CLightingColorFilter::Native_CreateLightingFilter(
    /* [in] */ Int32 mul,
    /* [in] */ Int32 add)
{
    return SkColorFilter::CreateLightingFilter(mul, add);
}