
#ifndef __CEMBOSSMASKFILTER_H__
#define __CEMBOSSMASKFILTER_H__

#include "_CEmbossMaskFilter.h"
#include "graphics/MaskFilter.h"
#include <elastos/AutoFree.h>

CarClass(CEmbossMaskFilter), public MaskFilter
{
public:
    CARAPI constructor(
        /* [in] */ const ArrayOf<Float>& direction,
        /* [in] */ Float ambient,
        /* [in] */ Float specular,
        /* [in] */ Float blurRadius);

    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

private:
    static CARAPI_(SkMaskFilter*) NativeConstructor(
        /* [in] */ const ArrayOf<Float>& dirArray,
        /* [in] */ Float ambient,
        /* [in] */ Float specular,
        /* [in] */ Float blurRadius);
};

#endif // __CEMBOSSMASKFILTER_H__
