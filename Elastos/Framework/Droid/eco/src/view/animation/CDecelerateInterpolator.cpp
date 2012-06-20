
#include "view/animation/CDecelerateInterpolator.h"
#include "ext/frameworkdef.h"
#include <elastos/Math.h>

using namespace Elastos::System;

CDecelerateInterpolator::CDecelerateInterpolator()
    : mFactor(1.0f)
{}

ECode CDecelerateInterpolator::constructor()
{
    return NOERROR;
}

/**
 * Constructor
 * 
 * @param factor Degree to which the animation should be eased. Setting factor to 1.0f produces
 *        an upside-down y=x^2 parabola. Increasing factor above 1.0f makes exaggerates the
 *        ease-out effect (i.e., it starts even faster and ends evens slower)
 */
ECode CDecelerateInterpolator::constructor(
    /* [in] */ Float factor)
{
    mFactor = factor;

    return NOERROR;
}

ECode CDecelerateInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    //TypedArray a =
    //    context.obtainStyledAttributes(attrs, com.android.internal.R.styleable.DecelerateInterpolator);
    //
    //mFactor = a.getFloat(com.android.internal.R.styleable.DecelerateInterpolator_factor, 1.0f);
    //
    //a.recycle();

    return NOERROR;
}

ECode CDecelerateInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);

    if (mFactor == 1.0f) {
        *output = (Float)(1.0f - (1.0f - input) * (1.0f - input));
    }
    else {
        *output = (Float)(1.0f - Math::Pow((1.0f - input), 2 * mFactor));
    }

    return NOERROR;
}
