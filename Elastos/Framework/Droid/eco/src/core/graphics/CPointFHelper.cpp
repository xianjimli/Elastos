
#include "graphics/CPointFHelper.h"
#include <elastos/Math.h>


ECode CPointFHelper::Length(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [out] */ Float * pLength)
{
    *pLength = Elastos::Core::Math::Sqrt(x * x + y * y);

    return NOERROR;
}

