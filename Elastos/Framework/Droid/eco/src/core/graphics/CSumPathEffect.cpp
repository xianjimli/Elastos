
#include "graphics/CSumPathEffect.h"

ECode CSumPathEffect::constructor(
    /* [in] */ IPathEffect * pFirst,
    /* [in] */ IPathEffect * pSecond)
{
    mNativeInstance = nativeCreate(((PathEffect*)pFirst)->Ni(),
                                       ((PathEffect*)pSecond)->Ni());
    return NOERROR;
}

SkPathEffect* CSumPathEffect::nativeCreate(SkPathEffect* first, SkPathEffect* second) {

    return new SkSumPathEffect(first, second);
}