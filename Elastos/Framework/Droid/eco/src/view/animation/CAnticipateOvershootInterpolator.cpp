
#include "view/animation/CAnticipateOvershootInterpolator.h"
#include "ext/frameworkdef.h"
#include <elastos/Math.h>
#include <elastos/AutoPtr.h>

using namespace Elastos::Core;

ECode CAnticipateOvershootInterpolator::constructor()
{
    mTension = 2.0f * 1.5f;

    return NOERROR;
}

ECode CAnticipateOvershootInterpolator::constructor(
    /* [in] */ Float tension)
{
    mTension = tension * 1.5f;

    return NOERROR;
}

ECode CAnticipateOvershootInterpolator::constructor(
    /* [in] */ Float tension,
    /* [in] */ Float extraTension)
{
    mTension = tension * extraTension;

    return NOERROR;
}

static Int32 R_Styleable_AnticipateOvershootInterpolator[] = {
    0x0101026a, 0x0101026b
};

ECode CAnticipateOvershootInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx2(
        attrs, ArrayOf<Int32>(R_Styleable_AnticipateOvershootInterpolator,
        sizeof(R_Styleable_AnticipateOvershootInterpolator) / sizeof(Int32))
        /*com.android.internal.R.styleable.AnticipateOvershootInterpolator*/, (ITypedArray**)&a);

    Float tension, extraTension;
    a->GetFloat(
        0/*com.android.internal.R.styleable.AnticipateOvershootInterpolator_tension*/,
        2.0f, &tension);
    a->GetFloat(
        1/*com.android.internal.R.styleable.AnticipateOvershootInterpolator_extraTension*/,
        1.5f, &extraTension);

    mTension = tension * extraTension;

    a->Recycle();

    return NOERROR;
}

static Float A(
    /* [in] */ Float t,
    /* [in] */ Float s)
{
    return t * t * ((s + 1) * t - s);
}

static Float O(
    /* [in] */ Float t,
    /* [in] */ Float s)
{
    return t * t * ((s + 1) * t + s);
}

ECode CAnticipateOvershootInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);

    // a(t, s) = t * t * ((s + 1) * t - s)
    // o(t, s) = t * t * ((s + 1) * t + s)
    // f(t) = 0.5 * a(t * 2, tension * extraTension), when t < 0.5
    // f(t) = 0.5 * (o(t * 2 - 2, tension * extraTension) + 2), when t <= 1.0
    if (input < 0.5f) {
        *output = 0.5f * A(input * 2.0f, mTension);
    }
    else {
        *output = 0.5f * (O(input * 2.0f - 2.0f, mTension) + 2.0f);
    }

    return NOERROR;
}
