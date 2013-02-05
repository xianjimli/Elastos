
#ifndef __CEMBOSSMASKFILTER_H__
#define __CEMBOSSMASKFILTER_H__

#include "_CEmbossMaskFilter.h"
#include "graphics/MaskFilter.h"
#include <elastos/AutoFree.h>

CarClass(CEmbossMaskFilter), public MaskFilter
{
public:
    CARAPI constructor(
        /* [in] */ const ArrayOf<Float> & direction,
        /* [in] */ Float ambient,
        /* [in] */ Float specular,
        /* [in] */ Float blurRadius);

private:
    static SkMaskFilter* nativeConstructor(
            const ArrayOf<Float> & dirArray,
            Float ambient,
            Float specular,
            Float blurRadius);
};

#endif // __CEMBOSSMASKFILTER_H__
