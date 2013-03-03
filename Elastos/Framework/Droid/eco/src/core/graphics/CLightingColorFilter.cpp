
#include "graphics/CLightingColorFilter.h"


ECode CLightingColorFilter::constructor(
    /* [in] */ Int32 mul,
    /* [in] */ Int32 add)
{
    mNativeInstance = NativeCreateLightingFilter(mul, add);
    return NOERROR;
}

PInterface CLightingColorFilter::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_ColorFilter) {
        return reinterpret_cast<PInterface>((ColorFilter*)this);
    }
    return _CLightingColorFilter::Probe(riid);
}

SkColorFilter* CLightingColorFilter::NativeCreateLightingFilter(
    /* [in] */ Int32 mul,
    /* [in] */ Int32 add)
{
    return SkColorFilter::CreateLightingFilter(mul, add);
}
