
#ifndef __CBLURMASKFILTER_H__
#define __CBLURMASKFILTER_H__

#include "_CBlurMaskFilter.h"
#include "graphics/MaskFilter.h"

CarClass(CBlurMaskFilter), public MaskFilter
{
public:
    CARAPI constructor(
        /* [in] */ Float radius,
        /* [in] */ Int32 style);

    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

private:
    static CARAPI_(SkMaskFilter*) NativeConstructor(
        /* [in] */ Float radius,
        /* [in] */ Int32 style);
};

#endif // __CBLURMASKFILTER_H__
