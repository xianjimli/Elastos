
#include "graphics/CColorMatrixColorFilter.h"
#include <elastos/AutoFree.h>
#include <skia/effects/SkColorMatrixFilter.h>

ECode CColorMatrixColorFilter::constructor(
    /* [in] */ IColorMatrix * pMatrix)
{
    AutoFree< ArrayOf<Float> > pArray;

    pMatrix->getArray((ArrayOf<Float> **)& pArray);
    mNativeInstance = nativeColorMatrixFilter(pArray);
    return NOERROR;
}

ECode CColorMatrixColorFilter::constructor(
    /* [in] */ const ArrayOf<Float> & array)
{
    if (array.GetLength() < 20) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    AutoFree< ArrayOf<Float> > pArray;
    pArray = array.Clone();
    mNativeInstance = nativeColorMatrixFilter(pArray);
    return NOERROR;
}

SkColorFilter* CColorMatrixColorFilter::nativeColorMatrixFilter(ArrayOf<Float>* pArray)
{
	SkASSERT(pArray);

	if (pArray->GetLength() < 20) {
		return NULL;
	}

	const Float* src = pArray->GetPayload();
	
#ifdef SK_SCALAR_IS_FIXED
	SkFixed array[20];
	for (int i = 0; i < 20; i++) {
	    array[i] = SkFloatToScalar(src[i]);
	}
	return new SkColorMatrixFilter(array);
#else
	return new SkColorMatrixFilter(src);
#endif
}
