
#include "graphics/CColorMatrixColorFilter.h"
#include <elastos/AutoFree.h>
#include <skia/effects/SkColorMatrixFilter.h>

ECode CColorMatrixColorFilter::constructor(
    /* [in] */ IColorMatrix* matrix)
{
    AutoFree< ArrayOf<Float> > array;

    matrix->GetArray((ArrayOf<Float> **)&array);
    mNativeInstance = NativeColorMatrixFilter(*array.Get());
    return NOERROR;
}

ECode CColorMatrixColorFilter::constructor(
    /* [in] */ const ArrayOf<Float>& array)
{
    if (array.GetLength() < 20) {
        // throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    mNativeInstance = NativeColorMatrixFilter(array);
    return NOERROR;
}

PInterface CColorMatrixColorFilter::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_ColorFilter) {
        return reinterpret_cast<PInterface>((ColorFilter*)this);
    }
    return _CColorMatrixColorFilter::Probe(riid);
}

SkColorFilter* CColorMatrixColorFilter::NativeColorMatrixFilter(
    /* [in] */ const ArrayOf<Float>& _array)
{
    if (_array.GetLength() < 20) {
        sk_throw();
    }
    const Float* src = _array.GetPayload();

#ifdef SK_SCALAR_IS_FIXED
    SkFixed array[20];
    for (Int32 i = 0; i < 20; i++) {
        array[i] = SkFloatToScalar(src[i]);
    }
    return new SkColorMatrixFilter(array);
#else
    return new SkColorMatrixFilter(src);
#endif
}
