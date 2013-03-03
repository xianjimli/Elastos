
#include "graphics/CPaintFlagsDrawFilter.h"
#include <skia/effects/SkPaintFlagsDrawFilter.h>
#include <skia/core/SkPaint.h>


ECode CPaintFlagsDrawFilter::constructor(
    /* [in] */ Int32 clearBits,
    /* [in] */ Int32 setBits)
{
    nativeDrawFilter = NativeConstructor(clearBits, setBits);
    return NOERROR;
}

PInterface CPaintFlagsDrawFilter::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_DrawFilter) {
        return reinterpret_cast<PInterface>((DrawFilter*)this);
    }
    return _CPaintFlagsDrawFilter::Probe(riid);
}

SkDrawFilter* CPaintFlagsDrawFilter::NativeConstructor(
    /* [in] */ Int32 clearBits,
    /* [in] */ Int32 setBits)
{
    // trim off any out-of-range bits
    clearBits &= SkPaint::kAllFlags;
    setBits &= SkPaint::kAllFlags;

    if (clearBits | setBits) {
        return new SkPaintFlagsDrawFilter(clearBits, setBits);
    }
    else {
        return NULL;
    }
}
