
#ifndef __CTABLELAYOUT_H__
#define __CTABLELAYOUT_H__

#include "_CTableLayout.h"
#include "widget/TableLayout.h"
#include "view/ViewMacro.h"

CarClass(CTableLayout), public TableLayout
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

    CARAPI IsShrinkAllColumns(
        /* [out] */ Boolean * pShrinkAllColumns);

    CARAPI SetShrinkAllColumns(
        /* [in] */ Boolean shrinkAllColumns);

    CARAPI IsStretchAllColumns(
        /* [out] */ Boolean * pStretchAllColumns);

    CARAPI SetStretchAllColumns(
        /* [in] */ Boolean stretchAllColumns);

    CARAPI SetColumnCollapsed(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Boolean isCollapsed);

    CARAPI IsColumnCollapsed(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean * pIsCollapsed);

    CARAPI SetColumnStretchable(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Boolean isStretchable);

    CARAPI IsColumnStretchable(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean * pIsStretchable);

    CARAPI SetColumnShrinkable(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Boolean isShrinkable);

    CARAPI IsColumnShrinkable(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean * pIsShrinkable);

    CARAPI constructor(
        /* [in] */ IContext * pContext);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IAttributeSet * pAttrs);

private:
    // TODO: Add your private member variables here.
};

#endif // __CTABLELAYOUT_H__
