
#include "graphics/CComposePathEffect.h"


ECode CComposePathEffect::constructor(
    /* [in] */ IPathEffect * pOuterpe,
    /* [in] */ IPathEffect * pInnerpe)
{
    mNativeInstance = nativeCreate(((PathEffect*)pOuterpe)->Ni(),
                                       ((PathEffect*)pInnerpe)->Ni());
    return NOERROR;
}

SkPathEffect* CComposePathEffect::nativeCreate(SkPathEffect* outerpe, SkPathEffect* innerpe)
{
    return new SkComposePathEffect(outerpe, innerpe);
}