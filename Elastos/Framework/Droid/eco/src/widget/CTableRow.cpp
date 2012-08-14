
#include "widget/CTableRow.h"

IVIEW_METHODS_IMPL(CTableRow, TableRow, TableRow);

IVIEWGROUP_METHODS_IMPL(CTableRow, TableRow, TableRow);

IVIEWPARENT_METHODS_IMPL(CTableRow, TableRow, TableRow);

IVIEWMANAGER_METHODS_IMPL(CTableRow, TableRow, TableRow);

IDrawableCallback_METHODS_IMPL(CTableRow, TableRow, TableRow);

IKeyEventCallback_METHODS_IMPL(CTableRow, TableRow, TableRow);

IAccessibilityEventSource_METHODS_IMPL(CTableRow, TableRow, TableRow);

CARAPI_(PInterface) CTableRow::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CTableRow::Probe(riid);
}

ECode CTableRow::IsBaselineAligned(
    /* [out] */  Boolean* baselineAligned)
{
    VALIDATE_NOT_NULL(baselineAligned);

    *baselineAligned = TableRow::IsBaselineAligned();
    return NOERROR;
}

ECode CTableRow::SetBaselineAligned(
    /* [in] */ Boolean baselineAligned)
{
    return TableRow::SetBaselineAligned(baselineAligned);
}

ECode CTableRow::GetBaselineAlignedChildIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    *index = TableRow::GetBaselineAlignedChildIndex();
    return NOERROR;
}

ECode CTableRow::SetBaselineAlignedChildIndex(
    /* [in] */ Int32 index)
{
    return TableRow::SetBaselineAlignedChildIndex(index);
}

ECode CTableRow::GetWeightSum(
    /* [out] */ Float* weightSum)
{
    VALIDATE_NOT_NULL(weightSum);

    *weightSum = TableRow::GetWeightSum();
    return NOERROR;
}

ECode CTableRow::SetWeightSum(
    /* [in] */ Float weightSum)
{
    return TableRow::SetWeightSum(weightSum);
}

ECode CTableRow::SetOrientation(
    /* [in] */ Int32 orientation)
{
    return TableRow::SetOrientation(orientation);
}

ECode CTableRow::GetOrientation(
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);

    *orientation = TableRow::GetOrientation();
    return NOERROR;
}

ECode CTableRow::SetGravity(
    /* [in] */ Int32 gravity)
{
    return TableRow::SetGravity(gravity);
}

ECode CTableRow::SetHorizontalGravity(
    /* [in] */ Int32 gravity)
{
    return TableRow::SetHorizontalGravity(gravity);
}

ECode CTableRow::SetVerticalGravity(
    /* [in] */ Int32 gravity)
{
    return TableRow::SetVerticalGravity(gravity);
}

ECode CTableRow::SetColumnCollapsed(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Boolean collapsed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTableRow::GetColumnsWidths(
    /* [in] */ Int32 widthMeasureSpec,
    /* [out, callee] */ ArrayOf<Int32> ** ppWidths)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTableRow::SetColumnsWidthConstraints(
    /* [in] */ const ArrayOf<Int32> & columnWidths)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTableRow::constructor(
    /* [in] */ IContext * pContext)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTableRow::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

