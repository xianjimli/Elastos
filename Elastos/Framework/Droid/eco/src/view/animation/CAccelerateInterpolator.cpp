
#include "view/animation/CAccelerateInterpolator.h"
#include "ext/frameworkdef.h"
#include <elastos/Math.h>
#include <elastos/AutoPtr.h>

using namespace Elastos::Core;

ECode CAccelerateInterpolator::constructor()
{
    mFactor = 1.0f;
    mDoubleFactor = 2.0;

    return NOERROR;
}

ECode CAccelerateInterpolator::constructor(
    /* [in] */ Float factor)
{
    mFactor = factor;
    mDoubleFactor = 2 * mFactor;

    return NOERROR;
}

static Int32 R_Styleable_AccelerateInterpolator[] = {
    0x010101d3
};

ECode CAccelerateInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx2(
        attrs, ArrayOf<Int32>(R_Styleable_AccelerateInterpolator,
        sizeof(R_Styleable_AccelerateInterpolator) / sizeof(Int32))
        /*com.android.internal.R.styleable.AccelerateInterpolator*/, (ITypedArray**)&a);

    a->GetFloat(0/*com.android.internal.R.styleable.AccelerateInterpolator_factor*/, 1.0f, &mFactor);
    mDoubleFactor = 2 * mFactor;

    a->Recycle();

    return NOERROR;
}

ECode CAccelerateInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);

    if (mFactor == 1.0f) {
        *output = input * input;
    }
    else {
        *output =  (Float)Math::Pow(input, mDoubleFactor);
    }

    return NOERROR;
}
