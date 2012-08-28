
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
    /* [out] */ ITabSpec** tabSpec)
{
    VALIDATE_NOT_NULL(tabSpec);
    *tabSpec = TabHost::NewTabSpec(tag);
    if (*tabSpec != NULL) (*tabSpec)->AddRef();
    return NOERROR;
}

ECode CTabHost::Setup()
{
    return TabHost::Setup();
}

ECode CTabHost::AddTab(
    /* [in] */ ITabSpec* tabSpec)
{
    return TabHost::AddTab(tabSpec);
}

ECode CTabHost::ClearAllTabs()
{
    return TabHost::ClearAllTabs();
}

ECode CTabHost::GetTabWidget(
    /* [out] */ ITabWidget** tabWidget)
{
    VALIDATE_NOT_NULL(tabWidget);
    *tabWidget = TabHost::GetTabWidget();
    if (*tabWidget != NULL) (*tabWidget)->AddRef();
    return NOERROR;
}

ECode CTabHost::GetCurrentTab(
    /* [out] */ Int32* tab)
{
    VALIDATE_NOT_NULL(tab);
    *tab = TabHost::GetCurrentTab();
    return NOERROR;
}

ECode CTabHost::GetCurrentTabTag(
    /* [out] */ String* tag)
{
    VALIDATE_NOT_NULL(tag);
    *tag = TabHost::GetCurrentTabTag();
    return NOERROR;
}

ECode CTabHost::GetCurrentTabView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = TabHost::GetCurrentTabView();
    if (*view != NULL) (*view)->AddRef();
    return NOERROR;
}

ECode CTabHost::GetCurrentView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = TabHost::GetCurrentView();
    if (*view != NULL) (*view)->AddRef();
    return NOERROR;
}

ECode CTabHost::SetCurrentTabByTag(
    /* [in] */ const String& tag)
{
    return TabHost::SetCurrentTabByTag(tag);
}

ECode CTabHost::GetTabContentView(
    /* [out] */ IFrameLayout** view)
{
    VALIDATE_NOT_NULL(view);
    *view = TabHost::GetTabContentView();
    if (*view != NULL) (*view)->AddRef();
    return NOERROR;
}

ECode CTabHost::SetCurrentTab(
    /* [in] */ Int32 index)
{
    return TabHost::SetCurrentTab(index);
}

ECode CTabHost::SetOnTabChangedListener(
    /* [in] */ ITabHostOnTabChangeListener* l)
{
    return TabHost::SetOnTabChangedListener(l);
}

ECode CTabHost::OnTouchModeChanged(
    /* [in] */ Boolean isInTouchMode)
{
    return TabHost::OnTouchModeChanged(isInTouchMode);
}

ECode CTabHost::constructor(
    /* [in] */ IContext* context)
{
    return TabHost::Init(context);
}

ECode CTabHost::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return TabHost::Init(context, attrs);
}

