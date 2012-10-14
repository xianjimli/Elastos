
#include "widget/CTableLayout.h"

IVIEW_METHODS_IMPL(CTableLayout, TableLayout, TableLayout);

IVIEWGROUP_METHODS_IMPL(CTableLayout, TableLayout, TableLayout);

IVIEWPARENT_METHODS_IMPL(CTableLayout, TableLayout, TableLayout);

IVIEWMANAGER_METHODS_IMPL(CTableLayout, TableLayout, TableLayout);

IDrawableCallback_METHODS_IMPL(CTableLayout, TableLayout, TableLayout);

IKeyEventCallback_METHODS_IMPL(CTableLayout, TableLayout, TableLayout);

IAccessibilityEventSource_METHODS_IMPL(CTableLayout, TableLayout, TableLayout);

CARAPI_(PInterface) CTableLayout::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CTableLayout::Probe(riid);
}

ECode CTableLayout::IsBaselineAligned(
    /* [out] */  Boolean* baselineAligned)
{
    VALIDATE_NOT_NULL(baselineAligned);

    *baselineAligned = TableLayout::IsBaselineAligned();
    return NOERROR;
}

ECode CTableLayout::SetBaselineAligned(
    /* [in] */ Boolean baselineAligned)
{
    return TableLayout::SetBaselineAligned(baselineAligned);
}

ECode CTableLayout::GetBaselineAlignedChildIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    *index = TableLayout::GetBaselineAlignedChildIndex();
    return NOERROR;
}

ECode CTableLayout::SetBaselineAlignedChildIndex(
    /* [in] */ Int32 index)
{
    return TableLayout::SetBaselineAlignedChildIndex(index);
}

ECode CTableLayout::GetWeightSum(
    /* [out] */ Float* weightSum)
{
    VALIDATE_NOT_NULL(weightSum);

    *weightSum = TableLayout::GetWeightSum();
    return NOERROR;
}

ECode CTableLayout::SetWeightSum(
    /* [in] */ Float weightSum)
{
    return TableLayout::SetWeightSum(weightSum);
}

ECode CTableLayout::SetOrientation(
    /* [in] */ Int32 orientation)
{
    return TableLayout::SetOrientation(orientation);
}

ECode CTableLayout::GetOrientation(
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);

    *orientation = TableLayout::GetOrientation();
    return NOERROR;
}

ECode CTableLayout::SetGravity(
    /* [in] */ Int32 gravity)
{
    return TableLayout::SetGravity(gravity);
}

ECode CTableLayout::SetHorizontalGravity(
    /* [in] */ Int32 gravity)
{
    return TableLayout::SetHorizontalGravity(gravity);
}

ECode CTableLayout::SetVerticalGravity(
    /* [in] */ Int32 gravity)
{
    return TableLayout::SetVerticalGravity(gravity);
}

ECode CTableLayout::IsShrinkAllColumns(
    /* [out] */ Boolean* shrinkAllColumns)
{
    VALIDATE_NOT_NULL(shrinkAllColumns);

    *shrinkAllColumns = TableLayout::IsShrinkAllColumns();
    return NOERROR;
}

ECode CTableLayout::SetShrinkAllColumns(
    /* [in] */ Boolean shrinkAllColumns)
{
    return TableLayout::SetShrinkAllColumns(shrinkAllColumns);
}

ECode CTableLayout::IsStretchAllColumns(
    /* [out] */ Boolean* stretchAllColumns)
{
    VALIDATE_NOT_NULL(stretchAllColumns);

    *stretchAllColumns = TableLayout::IsStretchAllColumns();
    return NOERROR;
}

ECode CTableLayout::SetStretchAllColumns(
    /* [in] */ Boolean stretchAllColumns)
{
    return TableLayout::SetStretchAllColumns(stretchAllColumns);
}

ECode CTableLayout::SetColumnCollapsed(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Boolean isCollapsed)
{
    return TableLayout::SetColumnCollapsed(columnIndex, isCollapsed);
}

ECode CTableLayout::IsColumnCollapsed(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* isCollapsed)
{
    VALIDATE_NOT_NULL(isCollapsed);

    *isCollapsed = TableLayout::IsColumnCollapsed(columnIndex);
    return NOERROR;
}

ECode CTableLayout::SetColumnStretchable(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Boolean isStretchable)
{
    return TableLayout::SetColumnStretchable(columnIndex, isStretchable);
}

ECode CTableLayout::IsColumnStretchable(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* isStretchable)
{
    VALIDATE_NOT_NULL(isStretchable);

    *isStretchable = TableLayout::IsColumnStretchable(columnIndex);
    return NOERROR;
}

ECode CTableLayout::SetColumnShrinkable(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Boolean isShrinkable)
{
    return TableLayout::SetColumnShrinkable(columnIndex, isShrinkable);
}

ECode CTableLayout::IsColumnShrinkable(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* isShrinkable)
{
    VALIDATE_NOT_NULL(isShrinkable);

    *isShrinkable = TableLayout::IsColumnShrinkable(columnIndex);
    return NOERROR;
}

ECode CTableLayout::constructor(
    /* [in] */ IContext* context)
{
    return TableLayout::Init(context);
}

ECode CTableLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return TableLayout::Init(context, attrs);
}

