
#include "graphics/CPaintFlagsDrawFilter.h"
#include <skia/effects/SkPaintFlagsDrawFilter.h>
#include <skia/core/SkPaint.h>

ECode CPaintFlagsDrawFilter::constructor(
    /* [in] */ Int32 clearBits,
    /* [in] */ Int32 setBits)
{
    nativeDrawFilter = nativeConstructor(clearBits, setBits);
    return NOERROR;
}

SkDrawFilter* CPaintFlagsDrawFilter::nativeConstructor(
                                        Int32 clearBits,
                                        Int32 setBits)
{
	// trim off any out-of-range bits
	clearBits &= SkPaint::kAllFlags;
	setBits &= SkPaint::kAllFlags;

	if (clearBits | setBits) {
	    return new SkPaintFlagsDrawFilter(clearBits, setBits);
	} else {
	    return NULL;
	}
}
