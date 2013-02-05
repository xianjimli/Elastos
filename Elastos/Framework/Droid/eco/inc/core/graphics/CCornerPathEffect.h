
#ifndef __CCORNERPATHEFFECT_H__
#define __CCORNERPATHEFFECT_H__

#include "_CCornerPathEffect.h"
#include "graphics/PathEffect.h"

CarClass(CCornerPathEffect), public PathEffect
{
public:
    CARAPI constructor(
        /* [in] */ Float radius);

private:
    static CARAPI_(SkPathEffect*)nativeCreate(Float radius);
};

#endif // __CCORNERPATHEFFECT_H__
