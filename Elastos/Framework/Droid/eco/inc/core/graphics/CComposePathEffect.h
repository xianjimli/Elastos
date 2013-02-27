
#ifndef __CCOMPOSEPATHEFFECT_H__
#define __CCOMPOSEPATHEFFECT_H__

#include "_CComposePathEffect.h"
#include "graphics/PathEffect.h"

CarClass(CComposePathEffect), public PathEffect
{
public:
    CARAPI constructor(
        /* [in] */ IPathEffect * pOuterpe,
        /* [in] */ IPathEffect * pInnerpe);

private:
    static SkPathEffect* NativeCreate(
        /* [in] */ SkPathEffect* outerpe,
        /* [in] */ SkPathEffect* innerpe);
};

#endif // __CCOMPOSEPATHEFFECT_H__
