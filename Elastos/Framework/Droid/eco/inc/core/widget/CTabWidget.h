
#ifndef __CTABWIDGET_H__
#define __CTABWIDGET_H__

#include "_CTabWidget.h"
#include "widget/TabWidget.h"
#include "view/ViewMacro.h"

CarClass(CTabWidget), public TabWidget
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

    CARAPI GetChildTabViewAt(
        /* [in] */ Int32 index,
        /* [out] */ IView** view);

    CARAPI GetTabCount(
        /* [out] */ Int32* count);

    CARAPI SetDividerDrawable(
        /* [in] */ IDrawable* drawable);

    CARAPI SetDividerDrawableEx(
        /* [in] */ Int32 resId);

    CARAPI SetLeftStripDrawable(
        /* [in] */ IDrawable* drawable);

    CARAPI SetLeftStripDrawableEx(
        /* [in] */ Int32 resId);

    CARAPI SetRightStripDrawable(
        /* [in] */ IDrawable* drawable);

    CARAPI SetRightStripDrawableEx(
        /* [in] */ Int32 resId);

    CARAPI SetStripEnabled(
        /* [in] */ Boolean stripEnabled);

    CARAPI IsStripEnabled(
        /* [out] */ Boolean* stripEnabled);

    CARAPI SetCurrentTab(
        /* [in] */ Int32 index);

    CARAPI FocusCurrentTab(
        /* [in] */ Int32 index);

    CARAPI OnFocusChange(
        /* [in] */ IView* v,
        /* [in] */ Boolean hasFocus);

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

private:
    // TODO: Add your private member variables here.
};

#endif // __CTABWIDGET_H__
