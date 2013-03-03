
#ifndef __CCOLORMATRIXCOLORFILTER_H__
#define __CCOLORMATRIXCOLORFILTER_H__

#include "_CColorMatrixColorFilter.h"
#include "graphics/ColorFilter.h"

CarClass(CColorMatrixColorFilter), public ColorFilter
{
public:
    CARAPI constructor(
        /* [in] */ IColorMatrix* matrix);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Float>& array);

    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

private:
    static CARAPI_(SkColorFilter*) NativeColorMatrixFilter(
        /* [in] */ const ArrayOf<Float>& array);
};

#endif // __CCOLORMATRIXCOLORFILTER_H__
