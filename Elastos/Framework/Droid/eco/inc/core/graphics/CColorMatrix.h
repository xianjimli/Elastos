
#ifndef __CCOLORMATRIX_H__
#define __CCOLORMATRIX_H__

#include "_CColorMatrix.h"
#include <elastos/AutoPtr.h>

CarClass(CColorMatrix)
{
public:
    CARAPI GetArray(
        /* [out, callee] */ ArrayOf<Float> ** ppPArray);

    CARAPI Reset();

    CARAPI Set(
        /* [in] */ IColorMatrix * pSrc);

    CARAPI SetEx(
        /* [in] */ const ArrayOf<Float> & src);

    CARAPI SetScale(
        /* [in] */ Float rScale,
        /* [in] */ Float gScale,
        /* [in] */ Float bScale,
        /* [in] */ Float aScale);

    CARAPI SetRotate(
        /* [in] */ Int32 axis,
        /* [in] */ Float degrees);

    CARAPI SetConcat(
        /* [in] */ IColorMatrix * pMatA,
        /* [in] */ IColorMatrix * pMatB);

    CARAPI PreConcat(
        /* [in] */ IColorMatrix * pPrematrix);

    CARAPI PostConcat(
        /* [in] */ IColorMatrix * pPostmatrix);

    CARAPI SetSaturation(
        /* [in] */ Float sat);

    CARAPI SetRGB2YUV();

    CARAPI SetYUV2RGB();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const ArrayOf<Float> & mul);

    CARAPI constructor(
        /* [in] */ IColorMatrix * pArray);

    ~CColorMatrix();
private:
    ArrayOf<Float>* mArray;
};

#endif // __CCOLORMATRIX_H__
