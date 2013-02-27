
#ifndef __CSUMPATHEFFECT_H__
#define __CSUMPATHEFFECT_H__

#include "_CSumPathEffect.h"
#include "graphics/PathEffect.h"

CarClass(CSumPathEffect), public PathEffect
{
public:
    CARAPI constructor(
        /* [in] */ IPathEffect * pFirst,
        /* [in] */ IPathEffect * pSecond);

private:
    static CARAPI_(SkPathEffect*) NativeCreate(
        /* [in] */ SkPathEffect* first,
        /* [in] */ SkPathEffect* second);
};

#endif // __CSUMPATHEFFECT_H__
