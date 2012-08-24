
#include "view/animation/CLinearInterpolator.h"
#include "ext/frameworkdef.h"

ECode CLinearInterpolator::constructor()
{
    return NOERROR;
}

ECode CLinearInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return NOERROR;
}

ECode CLinearInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);
    *output = input;

    return NOERROR;
}
