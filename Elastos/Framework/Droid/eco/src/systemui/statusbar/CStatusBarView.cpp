
#include "systemui/statusbar/CStatusBarView.h"

IVIEW_METHODS_IMPL(CStatusBarView, StatusBarView, StatusBarView);

IVIEWGROUP_METHODS_IMPL(CStatusBarView, StatusBarView, StatusBarView);

IVIEWPARENT_METHODS_IMPL(CStatusBarView, StatusBarView, StatusBarView);

IVIEWMANAGER_METHODS_IMPL(CStatusBarView, StatusBarView, StatusBarView);

IDrawableCallback_METHODS_IMPL(CStatusBarView, StatusBarView, StatusBarView);

IKeyEventCallback_METHODS_IMPL(CStatusBarView, StatusBarView, StatusBarView);

IAccessibilityEventSource_METHODS_IMPL(CStatusBarView, StatusBarView, StatusBarView);

PInterface CStatusBarView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CStatusBarView::Probe(riid);
}

ECode CStatusBarView::SetForegroundGravity(
    /* [in] */ Int32 foregroundGravity)
{
    return StatusBarView::SetForegroundGravity(foregroundGravity);
}

ECode CStatusBarView::SetForeground(
    /* [in] */ IDrawable* drawable)
{
    return StatusBarView::SetForeground(drawable);
}

ECode CStatusBarView::GetForeground(
    /* [out] */ IDrawable** foreground)
{
    VALIDATE_NOT_NULL(foreground);
    AutoPtr<IDrawable> d = StatusBarView::GetForeground();
    *foreground = d.Get();
    if (*foreground) {
        (*foreground)->AddRef();
    }

    return NOERROR;
}

ECode CStatusBarView::SetMeasureAllChildren(
    /* [in] */ Boolean measureAll)
{
    return StatusBarView::SetMeasureAllChildren(measureAll);
}

ECode CStatusBarView::GetConsiderGoneChildrenWhenMeasuring(
    /* [out] */ Boolean* measureAll)
{
    VALIDATE_NOT_NULL(measureAll);
    *measureAll = StatusBarView::GetConsiderGoneChildrenWhenMeasuring();

    return NOERROR;
}

ECode CStatusBarView::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return StatusBarView::Init(ctx, attrs);
}
