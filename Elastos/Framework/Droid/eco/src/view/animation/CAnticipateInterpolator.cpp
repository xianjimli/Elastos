
#include "view/animation/CAnticipateInterpolator.h"
#include "ext/frameworkdef.h"
#include <elastos/Math.h>
#include <elastos/AutoPtr.h>

using namespace Elastos::Core;

ECode CAnticipateInterpolator::constructor()
{
    mTension = 2.0f;
    return NOERROR;
}

ECode CAnticipateInterpolator::constructor(
    /* [in] */ Float tension)
{
    mTension = tension;

    return NOERROR;
}

static Int32 R_Styleable_AnticipateInterpolator[] = {
    0x0101026a
};

ECode CAnticipateInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx2(
        attrs, ArrayOf<Int32>(R_Styleable_AnticipateInterpolator,
        sizeof(R_Styleable_AnticipateInterpolator) / sizeof(Int32))
        /*com.android.internal.R.styleable.AnticipateInterpolator*/, (ITypedArray**)&a);

    a->GetFloat(
        0/*com.android.internal.R.styleable.AnticipateInterpolator_tension*/,
        2.0f, &mTension);

    a->Recycle();

    return NOERROR;
}

ECode CAnticipateInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);

    // a(t) = t * t * ((tension + 1) * t - tension)
    *output = input * input * ((mTension + 1) * input - mTension);

    return NOERROR;
}
