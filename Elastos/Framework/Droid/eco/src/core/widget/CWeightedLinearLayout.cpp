
#include "widget/CWeightedLinearLayout.h"
#include "utils/CDisplayMetrics.h"
#include <elastos/AutoPtr.h>


static Int32 R_Styleable_WeightedLinearLayout[] = {
    0x010102d5, 0x010102d6
};

IVIEW_METHODS_IMPL(CWeightedLinearLayout, LinearLayout, LinearLayout);

IVIEWGROUP_METHODS_IMPL(CWeightedLinearLayout, LinearLayout, LinearLayout);

IVIEWPARENT_METHODS_IMPL(CWeightedLinearLayout, LinearLayout, LinearLayout);

IVIEWMANAGER_METHODS_IMPL(CWeightedLinearLayout, LinearLayout, LinearLayout);

IDrawableCallback_METHODS_IMPL(CWeightedLinearLayout, LinearLayout, LinearLayout);

IKeyEventCallback_METHODS_IMPL(CWeightedLinearLayout, LinearLayout, LinearLayout);

IAccessibilityEventSource_METHODS_IMPL(CWeightedLinearLayout, LinearLayout, LinearLayout);

CWeightedLinearLayout::CWeightedLinearLayout()
{}

ECode CWeightedLinearLayout::constructor(
    /* [in] */ IContext* ctx)
{
    return LinearLayout::Init(ctx);
}

ECode CWeightedLinearLayout::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(LinearLayout::Init(ctx, attrs));

    AutoPtr<ITypedArray> a;
    ctx->ObtainStyledAttributesEx2(
        attrs, ArrayOf<Int32>(R_Styleable_WeightedLinearLayout, /* styleable.WeightedLinearLayout */
        sizeof(R_Styleable_WeightedLinearLayout) / sizeof(Int32)),
        (ITypedArray**)&a);

    a->GetFloat(0/*styleable.WeightedLinearLayout_majorWeight*/, 0.0f, &mMajorWeight);
    a->GetFloat(1/*styleable.WeightedLinearLayout_minorWeight*/, 0.0f, &mMinorWeight);

    a->Recycle();
    return NOERROR;
}

PInterface CWeightedLinearLayout::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }

    return _CWeightedLinearLayout::Probe(riid);
}

void CWeightedLinearLayout::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    AutoPtr<IResources> res;
    LinearLayout::GetContext()->GetResources((IResources**)&res);
    AutoPtr<IDisplayMetrics> metrics;
    res->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    Int32 screenWidth = ((CDisplayMetrics*)metrics.Get())->mWidthPixels;
    Boolean isPortrait = screenWidth < ((CDisplayMetrics*)metrics.Get())->mHeightPixels;

    Int32 widthMode = MeasureSpec::GetMode(widthMeasureSpec);

    LinearLayout::OnMeasure(widthMeasureSpec, heightMeasureSpec);

    Int32 width = LinearLayout::GetMeasuredWidth();
    Int32 height = LinearLayout::GetMeasuredHeight();
    Boolean measure = FALSE;

    widthMeasureSpec = MeasureSpec::MakeMeasureSpec(width, MeasureSpec::EXACTLY);
    heightMeasureSpec = MeasureSpec::MakeMeasureSpec(height, MeasureSpec::EXACTLY);

    Float widthWeight = isPortrait ? mMinorWeight : mMajorWeight;
    if (widthMode == MeasureSpec::AT_MOST && widthWeight > 0.0f) {
        if (width < (Int32)(screenWidth * widthWeight)) {
            widthMeasureSpec = MeasureSpec::MakeMeasureSpec((Int32)(screenWidth * widthWeight),
                    MeasureSpec::EXACTLY);
            measure = TRUE;
        }
    }

    // TODO: Support height?

    if (measure) {
        LinearLayout::OnMeasure(widthMeasureSpec, heightMeasureSpec);
    }
}

ECode CWeightedLinearLayout::IsBaselineAligned(
    /* [out] */  Boolean* baselineAligned)
{
    VALIDATE_NOT_NULL(baselineAligned);

    *baselineAligned = LinearLayout::IsBaselineAligned();
    return NOERROR;
}

ECode CWeightedLinearLayout::SetBaselineAligned(
    /* [in] */ Boolean baselineAligned)
{
    return LinearLayout::SetBaselineAligned(baselineAligned);
}

ECode CWeightedLinearLayout::GetBaselineAlignedChildIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    *index = LinearLayout::GetBaselineAlignedChildIndex();
    return NOERROR;
}

ECode CWeightedLinearLayout::SetBaselineAlignedChildIndex(
    /* [in] */ Int32 index)
{
    return LinearLayout::SetBaselineAlignedChildIndex(index);
}

ECode CWeightedLinearLayout::GetWeightSum(
    /* [out] */ Float* weightSum)
{
    VALIDATE_NOT_NULL(weightSum);

    *weightSum = LinearLayout::GetWeightSum();
    return NOERROR;
}

ECode CWeightedLinearLayout::SetWeightSum(
    /* [in] */ Float weightSum)
{
    return LinearLayout::SetWeightSum(weightSum);
}

ECode CWeightedLinearLayout::SetOrientation(
    /* [in] */ Int32 orientation)
{
    return LinearLayout::SetOrientation(orientation);
}

ECode CWeightedLinearLayout::GetOrientation(
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);

    *orientation = LinearLayout::GetOrientation();
    return NOERROR;
}

ECode CWeightedLinearLayout::SetGravity(
    /* [in] */ Int32 gravity)
{
    return LinearLayout::SetGravity(gravity);
}

ECode CWeightedLinearLayout::SetHorizontalGravity(
    /* [in] */ Int32 gravity)
{
    return LinearLayout::SetHorizontalGravity(gravity);
}

ECode CWeightedLinearLayout::SetVerticalGravity(
    /* [in] */ Int32 gravity)
{
    return LinearLayout::SetVerticalGravity(gravity);
}
