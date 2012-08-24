
#ifndef __CTABLEROW_H__
#define __CTABLEROW_H__

#include "_CTableRow.h"
#include "widget/TableRow.h"
#include "view/ViewMacro.h"

CarClass(CTableRow), public TableRow
{
public:
    IVIEW_METHODS_DECL();

    IVIEWGROUP_METHODS_DECL();

    IVIEWPARENT_METHODS_DECL();

    IVIEWMANAGER_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI IsBaselineAligned(
        /* [out] */  Boolean* baselineAligned);

    CARAPI SetBaselineAligned(
        /* [in] */ Boolean baselineAligned);

    CARAPI GetBaselineAlignedChildIndex(
        /* [out] */ Int32 * pBaselineAlignedChildIndex);

    CARAPI SetBaselineAlignedChildIndex(
        /* [in] */ Int32 baselineAlignedChildIndex);

    CARAPI GetWeightSum(
        /* [out] */ Float * pWeightSum);

    CARAPI SetWeightSum(
        /* [in] */ Float weightSum);

    CARAPI SetOrientation(
        /* [in] */ Int32 orientation);

    CARAPI GetOrientation(
        /* [out] */ Int32* orientation);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI SetHorizontalGravity(
        /* [in] */ Int32 horizontalGravity);

    CARAPI SetVerticalGravity(
        /* [in] */ Int32 verticalGravity);

    CARAPI SetColumnCollapsed(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Boolean collapsed);

    CARAPI GetColumnsWidths(
        /* [in] */ Int32 widthMeasureSpec,
        /* [out, callee] */ ArrayOf<Int32> ** ppWidths);

    CARAPI SetColumnsWidthConstraints(
        /* [in] */ const ArrayOf<Int32> & columnWidths);

    CARAPI constructor(
        /* [in] */ IContext * pContext);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IAttributeSet * pAttrs);

private:
    // TODO: Add your private member variables here.
};

#endif // __CTABLEROW_H__
