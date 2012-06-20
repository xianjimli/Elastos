
#include "widget/CLinearLayout.h"

IVIEW_METHODS_IMPL(CLinearLayout, LinearLayout, LinearLayout);

IVIEWGROUP_METHODS_IMPL(CLinearLayout, LinearLayout, LinearLayout);

IVIEWPARENT_METHODS_IMPL(CLinearLayout, LinearLayout, LinearLayout);

IVIEWMANAGER_METHODS_IMPL(CLinearLayout, LinearLayout, LinearLayout);

PInterface CLinearLayout::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CLinearLayout::Probe(riid);
}

ECode CLinearLayout::IsBaselineAligned(
    /* [out] */  Boolean* baselineAligned)
{
    VALIDATE_NOT_NULL(baselineAligned);

    *baselineAligned = LinearLayout::IsBaselineAligned();
    return NOERROR;
}

ECode CLinearLayout::SetBaselineAligned(
    /* [in] */ Boolean baselineAligned)
{
    return LinearLayout::SetBaselineAligned(baselineAligned);
}

ECode CLinearLayout::GetBaselineAlignedChildIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    *index = LinearLayout::GetBaselineAlignedChildIndex();
    return NOERROR;
}

ECode CLinearLayout::SetBaselineAlignedChildIndex(
    /* [in] */ Int32 index)
{
    return LinearLayout::SetBaselineAlignedChildIndex(index);
}

ECode CLinearLayout::GetWeightSum(
    /* [out] */ Float* weightSum)
{
    VALIDATE_NOT_NULL(weightSum);

    *weightSum = LinearLayout::GetWeightSum();
    return NOERROR;
}

ECode CLinearLayout::SetWeightSum(
    /* [in] */ Float weightSum)
{
    return LinearLayout::SetWeightSum(weightSum);
}

ECode CLinearLayout::SetOrientation(
    /* [in] */ Int32 orientation)
{
    return LinearLayout::SetOrientation(orientation);
}

ECode CLinearLayout::GetOrientation(
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);

    *orientation = LinearLayout::GetOrientation();
    return NOERROR;
}

ECode CLinearLayout::SetGravity(
    /* [in] */ Int32 gravity)
{
    return LinearLayout::SetGravity(gravity);
}

ECode CLinearLayout::SetHorizontalGravity(
    /* [in] */ Int32 gravity)
{
    return LinearLayout::SetHorizontalGravity(gravity);
}

ECode CLinearLayout::SetVerticalGravity(
    /* [in] */ Int32 gravity)
{
    return LinearLayout::SetVerticalGravity(gravity);
}

ECode CLinearLayout::constructor(
    /* [in] */ IContext* ctx)
{
    return LinearLayout::Init(ctx);
}

ECode CLinearLayout::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return LinearLayout::Init(ctx, attrs);
}

