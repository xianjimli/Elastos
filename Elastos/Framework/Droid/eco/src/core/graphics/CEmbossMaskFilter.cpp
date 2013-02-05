
#include "graphics/CEmbossMaskFilter.h"
#include <skia/effects/SkBlurMaskFilter.h>

ECode CEmbossMaskFilter::constructor(
    /* [in] */ const ArrayOf<Float> & direction,
    /* [in] */ Float ambient,
    /* [in] */ Float specular,
    /* [in] */ Float blurRadius)
{
    if (direction.GetLength() < 3) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    mNativeInstance = nativeConstructor(direction, ambient, specular, blurRadius);

    return E_NOT_IMPLEMENTED;
}

SkMaskFilter* CEmbossMaskFilter::nativeConstructor(
            const ArrayOf<Float> & dirArray,
            Float ambient,
            Float specular,
            Float blurRadius)
{
	SkASSERT(dirArray);

	if (dirArray.GetLength() < 3) {
		return NULL;
	}
	
	SkScalar direction[3];	
	for (int i = 0; i < 3; i++) {
	    direction[i] = SkFloatToScalar(dirArray[i]);
	}

	SkMaskFilter* filter =  SkBlurMaskFilter::CreateEmboss(direction,
	                                              SkFloatToScalar(ambient),
	                                              SkFloatToScalar(specular),
	                                              SkFloatToScalar(blurRadius));	
	return filter;
}