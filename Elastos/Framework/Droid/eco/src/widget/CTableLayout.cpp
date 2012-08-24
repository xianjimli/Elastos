
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
    /* [out] */ Boolean * pShrinkAllColumns)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTableLayout::SetShrinkAllColumns(
    /* [in] */ Boolean shrinkAllColumns)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTableLayout::IsStretchAllColumns(
    /* [out] */ Boolean * pStretchAllColumns)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTableLayout::SetStretchAllColumns(
    /* [in] */ Boolean stretchAllColumns)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTableLayout::SetColumnCollapsed(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Boolean isCollapsed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTableLayout::IsColumnCollapsed(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean * pIsCollapsed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTableLayout::SetColumnStretchable(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Boolean isStretchable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTableLayout::IsColumnStretchable(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean * pIsStretchable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTableLayout::SetColumnShrinkable(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Boolean isShrinkable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTableLayout::IsColumnShrinkable(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean * pIsShrinkable)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTableLayout::constructor(
    /* [in] */ IContext * pContext)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTableLayout::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

