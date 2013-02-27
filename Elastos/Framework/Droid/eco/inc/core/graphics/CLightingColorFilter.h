
#ifndef __CLIGHTINGCOLORFILTER_H__
#define __CLIGHTINGCOLORFILTER_H__

#include "_CLightingColorFilter.h"
#include "graphics/ColorFilter.h"

CarClass(CLightingColorFilter), public ColorFilter
{
public:
    CARAPI constructor(
        /* [in] */ Int32 mul,
        /* [in] */ Int32 add);

private:
    static CARAPI_(SkColorFilter*) Native_CreateLightingFilter(
        /* [in] */ Int32 mul,
        /* [in] */ Int32 add);
};

#endif // __CLIGHTINGCOLORFILTER_H__
