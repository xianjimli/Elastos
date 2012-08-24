
#include "widget/CTabHost.h"

IVIEW_METHODS_IMPL(CTabHost, TabHost, TabHost);

IVIEWGROUP_METHODS_IMPL(CTabHost, TabHost, TabHost);

IVIEWPARENT_METHODS_IMPL(CTabHost, TabHost, TabHost);

IVIEWMANAGER_METHODS_IMPL(CTabHost, TabHost, TabHost);

IDrawableCallback_METHODS_IMPL(CTabHost, TabHost, TabHost);

IKeyEventCallback_METHODS_IMPL(CTabHost, TabHost, TabHost);

IAccessibilityEventSource_METHODS_IMPL(CTabHost, TabHost, TabHost);

PInterface CTabHost::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CTabHost::Probe(riid);
}

ECode CTabHost::SetForegroundGravity(
    /* [in] */ Int32 foregroundGravity)
{
    return FrameLayout::SetForegroundGravity(foregroundGravity);
}

ECode CTabHost::SetForeground(
    /* [in] */ IDrawable* drawable)
{
    return FrameLayout::SetForeground(drawable);
}

ECode CTabHost::GetForeground(
    /* [out] */ IDrawable** foreground)
{
    VALIDATE_NOT_NULL(foreground);
    AutoPtr<IDrawable> d = FrameLayout::GetForeground();
    *foreground = d.Get();
    if (*foreground) {
        (*foreground)->AddRef();
    }

    return NOERROR;
}

ECode CTabHost::SetMeasureAllChildren(
    /* [in] */ Boolean measureAll)
{
    return FrameLayout::SetMeasureAllChildren(measureAll);
}

ECode CTabHost::GetConsiderGoneChildrenWhenMeasuring(
    /* [out] */ Boolean* measureAll)
{
    VALIDATE_NOT_NULL(measureAll);
    *measureAll = FrameLayout::GetConsiderGoneChildrenWhenMeasuring();
    return NOERROR;
}

ECode CTabHost::NewTabSpec(
    /* [in] */ const String& tag,
    /* [out] */ ITabHostTabSpec ** ppTabspec)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabHost::Setup()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabHost::AddTab(
    /* [in] */ ITabHostTabSpec * pTabSpec)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabHost::ClearAllTabs()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabHost::GetTabWidget(
    /* [out] */ ITabWidget ** ppTabWidget)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabHost::GetCurrentTab(
    /* [out] */ Int32 * pTab)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabHost::GetCurrentTabView(
    /* [out] */ IView ** ppView)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabHost::GetCurrentView(
    /* [out] */ IView ** ppView)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabHost::SetCurrentTabByTag(
    /* [in] */ const String& tag)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabHost::GetTabContentView(
    /* [out] */ IFrameLayout ** ppView)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabHost::SetCurrentTab(
    /* [in] */ Int32 index)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabHost::SetOnTabChangedListener(
    /* [in] */ ITabHostOnTabChangeListener * pL)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabHost::constructor(
    /* [in] */ IContext * pContext)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTabHost::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

