
#include "view/animation/CCycleInterpolator.h"
#include "ext/frameworkdef.h"
#include <elastos/Math.h>
#include <elastos/AutoPtr.h>

using namespace Elastos::Core;

ECode CCycleInterpolator::constructor(
    /* [in] */ Float cycles)
{
    mCycles = cycles;

    return NOERROR;
}

static Int32 R_Styleable_CycleInterpolator[] = {
    0x010101d4
};

ECode CCycleInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx2(
        attrs, ArrayOf<Int32>(R_Styleable_CycleInterpolator,
        sizeof(R_Styleable_CycleInterpolator) / sizeof(Int32))
        /*com.android.internal.R.styleable.CycleInterpolator*/, (ITypedArray**)&a);

    a->GetFloat(
        0/*com.android.internal.R.styleable.CycleInterpolator_cycles*/,
        1.0f, &mCycles);

    a->Recycle();

    return NOERROR;
}

ECode CCycleInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);
    *output = (Float)(Math::Sin(2 * mCycles * Math::DOUBLE_PI * input));

    return NOERROR;
}
