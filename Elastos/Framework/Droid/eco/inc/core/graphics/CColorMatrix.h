
#ifndef __CCOLORMATRIX_H__
#define __CCOLORMATRIX_H__

#include "_CColorMatrix.h"
#include <elastos/AutoPtr.h>

CarClass(CColorMatrix)
{
public:
    CARAPI getArray(
        /* [out, callee] */ ArrayOf<Float> ** ppPArray);

    CARAPI reset();

    CARAPI set(
        /* [in] */ IColorMatrix * pSrc);

    CARAPI setEx(
        /* [in] */ const ArrayOf<Float> & src);

    CARAPI setScale(
        /* [in] */ Float rScale,
        /* [in] */ Float gScale,
        /* [in] */ Float bScale,
        /* [in] */ Float aScale);

    CARAPI setRotate(
        /* [in] */ Int32 axis,
        /* [in] */ Float degrees);

    CARAPI setConcat(
        /* [in] */ IColorMatrix * pMatA,
        /* [in] */ IColorMatrix * pMatB);

    CARAPI preConcat(
        /* [in] */ IColorMatrix * pPrematrix);

    CARAPI postConcat(
        /* [in] */ IColorMatrix * pPostmatrix);

    CARAPI setSaturation(
        /* [in] */ Float sat);

    CARAPI setRGB2YUV();

    CARAPI setYUV2RGB();

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
