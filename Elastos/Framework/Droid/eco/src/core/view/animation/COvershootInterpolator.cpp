
#include "view/animation/COvershootInterpolator.h"
#include "ext/frameworkdef.h"
#include <elastos/Math.h>
#include <elastos/AutoPtr.h>

using namespace Elastos::Core;

ECode COvershootInterpolator::constructor()
{
    mTension = 2.0f;

    return NOERROR;
}

ECode COvershootInterpolator::constructor(
    /* [in] */ Float tension)
{
    mTension = tension;

    return NOERROR;
}

static Int32 R_Styleable_OvershootInterpolator[] = {
    0x0101026a
};

ECode COvershootInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx2(
        attrs, ArrayOf<Int32>(R_Styleable_OvershootInterpolator,
        sizeof(R_Styleable_OvershootInterpolator) / sizeof(Int32))
        /*com.android.internal.R.styleable.OvershootInterpolator*/, (ITypedArray**)&a);

    a->GetFloat(
        0/*com.android.internal.R.styleable.OvershootInterpolator_tension*/,
        2.0f, &mTension);

    a->Recycle();

    return NOERROR;
}

ECode COvershootInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);

    // _o(t) = t * t * ((tension + 1) * t + tension)
    // o(t) = _o(t - 1) + 1
    input -= 1.0f;
    *output = input * input * ((mTension + 1) * input + mTension) + 1.0f;

    return NOERROR;
}
