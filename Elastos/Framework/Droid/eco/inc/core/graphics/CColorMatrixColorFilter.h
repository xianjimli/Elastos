
#ifndef __CCOLORMATRIXCOLORFILTER_H__
#define __CCOLORMATRIXCOLORFILTER_H__

#include "_CColorMatrixColorFilter.h"
#include "graphics/ColorFilter.h"

CarClass(CColorMatrixColorFilter), public ColorFilter
{
public:
    CARAPI constructor(
        /* [in] */ IColorMatrix * pMatrix);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Float> & array);

private:
    static SkColorFilter* nativeColorMatrixFilter(ArrayOf<Float>* pArray);
};

#endif // __CCOLORMATRIXCOLORFILTER_H__
