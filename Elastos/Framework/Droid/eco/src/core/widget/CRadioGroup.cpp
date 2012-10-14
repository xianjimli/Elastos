
#include "widget/CRadioGroup.h"

IVIEW_METHODS_IMPL(CRadioGroup, RadioGroup, RadioGroup);

IVIEWGROUP_METHODS_IMPL(CRadioGroup, RadioGroup, RadioGroup);

IVIEWPARENT_METHODS_IMPL(CRadioGroup, RadioGroup, RadioGroup);

IVIEWMANAGER_METHODS_IMPL(CRadioGroup, RadioGroup, RadioGroup);

IDrawableCallback_METHODS_IMPL(CRadioGroup, RadioGroup, RadioGroup);

IKeyEventCallback_METHODS_IMPL(CRadioGroup, RadioGroup, RadioGroup);

IAccessibilityEventSource_METHODS_IMPL(CRadioGroup, RadioGroup, RadioGroup);

PInterface CRadioGroup::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CRadioGroup::Probe(riid);
}

ECode CRadioGroup::IsBaselineAligned(
    /* [out] */  Boolean* baselineAligned)
{
    VALIDATE_NOT_NULL(baselineAligned);

    *baselineAligned = RadioGroup::IsBaselineAligned();
    return NOERROR;
}

ECode CRadioGroup::SetBaselineAligned(
    /* [in] */ Boolean baselineAligned)
{
    return RadioGroup::SetBaselineAligned(baselineAligned);
}

ECode CRadioGroup::GetBaselineAlignedChildIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    *index = RadioGroup::GetBaselineAlignedChildIndex();
    return NOERROR;
}

ECode CRadioGroup::SetBaselineAlignedChildIndex(
    /* [in] */ Int32 index)
{
    return RadioGroup::SetBaselineAlignedChildIndex(index);
}

ECode CRadioGroup::GetWeightSum(
    /* [out] */ Float* weightSum)
{
    VALIDATE_NOT_NULL(weightSum);

    *weightSum = RadioGroup::GetWeightSum();
    return NOERROR;
}

ECode CRadioGroup::SetWeightSum(
    /* [in] */ Float weightSum)
{
    return RadioGroup::SetWeightSum(weightSum);
}

ECode CRadioGroup::SetOrientation(
    /* [in] */ Int32 orientation)
{
    return RadioGroup::SetOrientation(orientation);
}

ECode CRadioGroup::GetOrientation(
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);

    *orientation = RadioGroup::GetOrientation();
    return NOERROR;
}

ECode CRadioGroup::SetGravity(
    /* [in] */ Int32 gravity)
{
    return RadioGroup::SetGravity(gravity);
}

ECode CRadioGroup::SetHorizontalGravity(
    /* [in] */ Int32 gravity)
{
    return RadioGroup::SetHorizontalGravity(gravity);
}

ECode CRadioGroup::SetVerticalGravity(
    /* [in] */ Int32 gravity)
{
    return RadioGroup::SetVerticalGravity(gravity);
}

ECode CRadioGroup::Check(
    /* [in] */ Int32 id)
{
    return RadioGroup::Check(id);
}

ECode CRadioGroup::GetCheckedRadioButtonId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = RadioGroup::GetCheckedRadioButtonId();

    return NOERROR;
}

ECode CRadioGroup::ClearCheck()
{
    return RadioGroup::ClearCheck();
}

ECode CRadioGroup::SetOnCheckedChangeListener(
    /* [in] */ IRadioGroupOnCheckedChangeListener* listener)
{
    return RadioGroup::SetOnCheckedChangeListener(listener);
}

ECode CRadioGroup::constructor(
    /* [in] */ IContext* ctx)
{
    return RadioGroup::Init(ctx);
}

ECode CRadioGroup::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return RadioGroup::Init(ctx, attrs);
}
