
#include "graphics/CSumPathEffect.h"

ECode CSumPathEffect::constructor(
    /* [in] */ IPathEffect * pFirst,
    /* [in] */ IPathEffect * pSecond)
{
    mNativeInstance = NativeCreate((
                        (PathEffect*)pFirst)->Ni(),
                        ((PathEffect*)pSecond)->Ni());
    return NOERROR;
}

SkPathEffect* CSumPathEffect::NativeCreate(
    /* [in] */ SkPathEffect* first,
    /* [in] */ SkPathEffect* second) {

    return new SkSumPathEffect(first, second);
}