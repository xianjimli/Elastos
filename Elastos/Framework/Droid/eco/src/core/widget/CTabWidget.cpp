
#include "widget/CTabWidget.h"


IVIEW_METHODS_IMPL(CTabWidget, TabWidget, TabWidget);

IVIEWGROUP_METHODS_IMPL(CTabWidget, TabWidget, TabWidget);

IVIEWPARENT_METHODS_IMPL(CTabWidget, TabWidget, TabWidget);

IVIEWMANAGER_METHODS_IMPL(CTabWidget, TabWidget, TabWidget);

IDrawableCallback_METHODS_IMPL(CTabWidget, TabWidget, TabWidget);

IKeyEventCallback_METHODS_IMPL(CTabWidget, TabWidget, TabWidget);

IAccessibilityEventSource_METHODS_IMPL(CTabWidget, TabWidget, TabWidget);

PInterface CTabWidget::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CTabWidget::Probe(riid);
}

ECode CTabWidget::IsBaselineAligned(
    /* [out] */  Boolean* baselineAligned)
{
    VALIDATE_NOT_NULL(baselineAligned);

    *baselineAligned = TabWidget::IsBaselineAligned();
    return NOERROR;
}

ECode CTabWidget::SetBaselineAligned(
    /* [in] */ Boolean baselineAligned)
{
    return TabWidget::SetBaselineAligned(baselineAligned);
}

ECode CTabWidget::GetBaselineAlignedChildIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    *index = TabWidget::GetBaselineAlignedChildIndex();
    return NOERROR;
}

ECode CTabWidget::SetBaselineAlignedChildIndex(
    /* [in] */ Int32 index)
{
    return TabWidget::SetBaselineAlignedChildIndex(index);
}

ECode CTabWidget::GetWeightSum(
    /* [out] */ Float* weightSum)
{
    VALIDATE_NOT_NULL(weightSum);

    *weightSum = TabWidget::GetWeightSum();
    return NOERROR;
}

ECode CTabWidget::SetWeightSum(
    /* [in] */ Float weightSum)
{
    return TabWidget::SetWeightSum(weightSum);
}

ECode CTabWidget::SetOrientation(
    /* [in] */ Int32 orientation)
{
    return TabWidget::SetOrientation(orientation);
}

ECode CTabWidget::GetOrientation(
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);

    *orientation = TabWidget::GetOrientation();
    return NOERROR;
}

ECode CTabWidget::SetGravity(
    /* [in] */ Int32 gravity)
{
    return TabWidget::SetGravity(gravity);
}

ECode CTabWidget::SetHorizontalGravity(
    /* [in] */ Int32 gravity)
{
    return TabWidget::SetHorizontalGravity(gravity);
}

ECode CTabWidget::SetVerticalGravity(
    /* [in] */ Int32 gravity)
{
    return TabWidget::SetVerticalGravity(gravity);
}

ECode CTabWidget::GetChildTabViewAt(
    /* [in] */ Int32 index,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);

    AutoPtr<IView> child = TabWidget::GetChildTabViewAt(index);
    *view = child;
    if (*view != NULL) (*view)->AddRef();
    return NOERROR;
}

ECode CTabWidget::GetTabCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = TabWidget::GetTabCount();
    return NOERROR;
}

ECode CTabWidget::SetDividerDrawable(
    /* [in] */ IDrawable* drawable)
{
    return TabWidget::SetDividerDrawable(drawable);
}

ECode CTabWidget::SetDividerDrawableEx(
    /* [in] */ Int32 resId)
{
    return TabWidget::SetDividerDrawable(resId);
}

ECode CTabWidget::SetLeftStripDrawable(
    /* [in] */ IDrawable* drawable)
{
    return TabWidget::SetLeftStripDrawable(drawable);
}

ECode CTabWidget::SetLeftStripDrawableEx(
    /* [in] */ Int32 resId)
{
    return TabWidget::SetLeftStripDrawable(resId);
}

ECode CTabWidget::SetRightStripDrawable(
    /* [in] */ IDrawable* drawable)
{
    return TabWidget::SetRightStripDrawable(drawable);
}

ECode CTabWidget::SetRightStripDrawableEx(
    /* [in] */ Int32 resId)
{
    return TabWidget::SetRightStripDrawable(resId);
}

ECode CTabWidget::SetStripEnabled(
    /* [in] */ Boolean stripEnabled)
{
    return TabWidget::SetStripEnabled(stripEnabled);
}

ECode CTabWidget::IsStripEnabled(
    /* [out] */ Boolean*  stripEnabled)
{
    VALIDATE_NOT_NULL(stripEnabled);
    *stripEnabled = TabWidget::IsStripEnabled();
    return NOERROR;
}

ECode CTabWidget::SetCurrentTab(
    /* [in] */ Int32 index)
{
    return TabWidget::SetCurrentTab(index);
}

ECode CTabWidget::FocusCurrentTab(
    /* [in] */ Int32 index)
{
    return TabWidget::FocusCurrentTab(index);
}

ECode CTabWidget::OnFocusChange(
    /* [in] */ IView* v,
    /* [in] */ Boolean hasFocus)
{
    return TabWidget::OnFocusChange(v, hasFocus);
}

ECode CTabWidget::constructor(
    /* [in] */ IContext* ctx)
{
    return TabWidget::Init(ctx);
}

ECode CTabWidget::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return TabWidget::Init(ctx, attrs);
}

ECode CTabWidget::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return TabWidget::Init(ctx, attrs, defStyle);
}

