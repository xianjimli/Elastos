
#ifndef __CCOLORMATRIX_H__
#define __CCOLORMATRIX_H__

#include "_CColorMatrix.h"
#include <elastos/AutoPtr.h>

CarClass(CColorMatrix)
{
public:
    CColorMatrix();

    ~CColorMatrix();

    CARAPI GetArray(
        /* [out, callee] */ ArrayOf<Float>** array);

    CARAPI Reset();

    CARAPI Set(
        /* [in] */ IColorMatrix* src);

    CARAPI SetEx(
        /* [in] */ const ArrayOf<Float>& src);

    CARAPI SetScale(
        /* [in] */ Float rScale,
        /* [in] */ Float gScale,
        /* [in] */ Float bScale,
        /* [in] */ Float aScale);

    CARAPI SetRotate(
        /* [in] */ Int32 axis,
        /* [in] */ Float degrees);

    CARAPI SetConcat(
        /* [in] */ IColorMatrix* matA,
        /* [in] */ IColorMatrix* matB);

    CARAPI PreConcat(
        /* [in] */ IColorMatrix* prematrix);

    CARAPI PostConcat(
        /* [in] */ IColorMatrix* postmatrix);

    CARAPI SetSaturation(
        /* [in] */ Float sat);

    CARAPI SetRGB2YUV();

    CARAPI SetYUV2RGB();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const ArrayOf<Float>& src);

    CARAPI constructor(
        /* [in] */ IColorMatrix* src);

private:
    ArrayOf<Float>* mArray;
};

#endif // __CCOLORMATRIX_H__
