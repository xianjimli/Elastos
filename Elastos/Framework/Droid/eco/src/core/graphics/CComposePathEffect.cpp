
#include "graphics/CComposePathEffect.h"


ECode CComposePathEffect::constructor(
    /* [in] */ IPathEffect * pOuterpe,
    /* [in] */ IPathEffect * pInnerpe)
{
    mNativeInstance = NativeCreate((
                        (PathEffect*)pOuterpe)->Ni(),
                        ((PathEffect*)pInnerpe)->Ni());
    return NOERROR;
}

SkPathEffect* CComposePathEffect::NativeCreate(
    /* [in] */ SkPathEffect* outerpe,
    /* [in] */ SkPathEffect* innerpe)
{
    return new SkComposePathEffect(
                    outerpe,
                    innerpe);
}