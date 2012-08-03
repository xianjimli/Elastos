
#include "view/animation/CDecelerateInterpolator.h"
#include "ext/frameworkdef.h"
#include <elastos/Math.h>
#include <elastos/AutoPtr.h>

using namespace Elastos::Core;

ECode CDecelerateInterpolator::constructor()
{
    mFactor = 1.0f;

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

static Int32 R_Styleable_DecelerateInterpolator[] = {
    0x010101d3
};

ECode CDecelerateInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx2(
        attrs, ArrayOf<Int32>(R_Styleable_DecelerateInterpolator,
        sizeof(R_Styleable_DecelerateInterpolator) / sizeof(Int32))
        /*com.android.internal.R.styleable.DecelerateInterpolator*/, (ITypedArray**)&a);

    a->GetFloat(0/*com.android.internal.R.styleable.DecelerateInterpolator_factor*/, 1.0f, &mFactor);

    a->Recycle();

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
