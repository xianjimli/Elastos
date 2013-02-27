
#ifndef __CDISCRETEPATHEFFECT_H__
#define __CDISCRETEPATHEFFECT_H__

#include "_CDiscretePathEffect.h"
#include "graphics/PathEffect.h"

CarClass(CDiscretePathEffect), public PathEffect
{
public:
    CARAPI constructor(
        /* [in] */ Float segmentLength,
        /* [in] */ Float deviation);

private:
    static CARAPI_(SkPathEffect*) NativeCreate(
        /* [in] */ Float length,
        /* [in] */ Float deviation);
};

#endif // __CDISCRETEPATHEFFECT_H__
