
#include "ext/frameworkdef.h"
#include "graphics/CPointFHelper.h"
#include "graphics/CPointF.h"


ECode CPointFHelper::Length(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [out] */ Float* length)
{
    VALIDATE_NOT_NULL(length);

    *length = CPointF::Length(x, y);
    return NOERROR;
}
