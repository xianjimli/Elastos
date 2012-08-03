
#include "view/animation/CAccelerateDecelerateInterpolator.h"
#include "ext/frameworkdef.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

ECode CAccelerateDecelerateInterpolator::constructor()
{
    return NOERROR;
}

ECode CAccelerateDecelerateInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return NOERROR;
}

ECode CAccelerateDecelerateInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);

    *output = (Float)(Math::Cos((input + 1) * Math::DOUBLE_PI) / 2.0f) + 0.5f;

    return NOERROR;
}
