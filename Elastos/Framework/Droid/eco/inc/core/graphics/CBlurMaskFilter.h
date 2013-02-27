
#ifndef __CBLURMASKFILTER_H__
#define __CBLURMASKFILTER_H__

#include "_CBlurMaskFilter.h"
#include "graphics/MaskFilter.h"

CarClass(CBlurMaskFilter), public MaskFilter
{
public:
    CARAPI Blur(
        /* [in] */ Int32 value);

    CARAPI constructor(
        /* [in] */ Float radius,
        /* [in] */ Int32 style);

private:
    static CARAPI_(SkMaskFilter*) NativeConstructor(
        /* [in] */ Float radius,
        /* [in] */ Int32 style);

    Int32 native_int;
};

#endif // __CBLURMASKFILTER_H__
