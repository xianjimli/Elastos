
#include "widget/CNumberPicker.h"
#include <elastos/AutoFree.h>

IVIEW_METHODS_IMPL(CNumberPicker, NumberPicker, NumberPicker);

IVIEWGROUP_METHODS_IMPL(CNumberPicker, NumberPicker, NumberPicker);

PInterface CNumberPicker::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CNumberPicker::Probe(riid);
}

ECode CNumberPicker::IsBaselineAligned(
    /* [out] */ Boolean* baselineAligned)
{
    VALIDATE_NOT_NULL(baselineAligned);

    *baselineAligned = NumberPicker::IsBaselineAligned();
    return NOERROR;
}

ECode CNumberPicker::SetBaselineAligned(
    /* [in] */ Boolean baselineAligned)
{
    return NumberPicker::SetBaselineAligned(baselineAligned);
}

ECode CNumberPicker::GetBaselineAlignedChildIndex(
    /* [out] */ Int32* baselineAlignedChildIndex)
{
    VALIDATE_NOT_NULL(baselineAlignedChildIndex);

    *baselineAlignedChildIndex = NumberPicker::GetBaselineAlignedChildIndex();
    return NOERROR;
}

ECode CNumberPicker::SetBaselineAlignedChildIndex(
    /* [in] */ Int32 baselineAlignedChildIndex)
{
    return NumberPicker::SetBaselineAlignedChildIndex(baselineAlignedChildIndex);
}

ECode CNumberPicker::GetWeightSum(
    /* [out] */ Float* weightSum)
{
    VALIDATE_NOT_NULL(weightSum);

    *weightSum = NumberPicker::GetWeightSum();
    return NOERROR;
}

ECode CNumberPicker::SetWeightSum(
    /* [in] */ Float weightSum)
{
    return NumberPicker::SetWeightSum(weightSum);
}

ECode CNumberPicker::SetOrientation(
    /* [in] */ Int32 orientation)
{
    return NumberPicker::SetOrientation(orientation);
}

ECode CNumberPicker::GetOrientation(
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);

    *orientation = NumberPicker::GetOrientation();
    return NOERROR;
}

ECode CNumberPicker::SetGravity(
    /* [in] */ Int32 gravity)
{
    return NumberPicker::SetGravity(gravity);
}

ECode CNumberPicker::SetHorizontalGravity(
    /* [in] */ Int32 horizontalGravity)
{
    return NumberPicker::SetHorizontalGravity(horizontalGravity);
}

ECode CNumberPicker::SetVerticalGravity(
    /* [in] */ Int32 verticalGravity)
{
    return NumberPicker::SetVerticalGravity(verticalGravity);
}

ECode CNumberPicker::SetOnChangeListener(
    /* [in] */ IOnChangedListener* listener)
{
    return NumberPicker::SetOnChangeListener(listener);
}

ECode CNumberPicker::SetFormatter(
    /* [in] */ IFormatter* formatter)
{
    return NumberPicker::SetFormatter(formatter);
}

ECode CNumberPicker::SetRange(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return NumberPicker::SetRange(start, end);
}

ECode CNumberPicker::SetRangeEx(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const ArrayOf<String> & displayedValues)
{
    AutoFree< ArrayOf<String> > values = displayedValues.Clone();
    return NumberPicker::SetRangeEx(start, end, values);
}

ECode CNumberPicker::SetCurrent(
    /* [in] */ Int32 current)
{
    return NumberPicker::SetCurrent(current);
}

ECode CNumberPicker::SetSpeed(
    /* [in] */ Int64 speed)
{
    return NumberPicker::SetSpeed(speed);
}

ECode CNumberPicker::CancelIncrement()
{
    return NumberPicker::CancelIncrement();
}

ECode CNumberPicker::CancelDecrement()
{
    return NumberPicker::CancelDecrement();
}

ECode CNumberPicker::GetCurrent(
    /* [out] */ Int32* current)
{
    VALIDATE_NOT_NULL(current);

    *current = NumberPicker::GetCurrent();
    return NOERROR;
}

ECode CNumberPicker::constructor(
    /* [in] */ IContext* context)
{
    NumberPicker::Init(context, NULL);
    return NOERROR;
}

ECode CNumberPicker::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    NumberPicker::Init(context, attrs);
    return NOERROR;
}

