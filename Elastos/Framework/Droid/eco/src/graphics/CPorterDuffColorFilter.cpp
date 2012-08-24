
#include "graphics/CPorterDuffColorFilter.h"

PInterface CPorterDuffColorFilter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_ColorFilter) {
        return reinterpret_cast<PInterface>((ColorFilter*)this);
    }
    return _CPorterDuffColorFilter::Probe(riid);
}

ECode CPorterDuffColorFilter::constructor(
    /* [in] */ Int32 srcColor,
    /* [in] */ PorterDuffMode mode)
{
    return E_NOT_IMPLEMENTED;
}
