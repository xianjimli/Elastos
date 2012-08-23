
#ifndef __CTABHOST_H__
#define __CTABHOST_H__

#include "_CTabHost.h"
#include "widget/TabHost.h"
#include "view/ViewMacro.h"

CarClass(CTabHost), public TabHost
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

    CARAPI SetForegroundGravity(
        /* [in] */ Int32 foregroundGravity);

    CARAPI SetForeground(
        /* [in] */ IDrawable* drawable);

    CARAPI GetForeground(
        /* [out] */ IDrawable** foreground);

    CARAPI SetMeasureAllChildren(
        /* [in] */ Boolean measureAll);

    CARAPI GetConsiderGoneChildrenWhenMeasuring(
        /* [out] */ Boolean* measureAll);

    CARAPI NewTabSpec(
        /* [in] */ const String& tag,
        /* [out] */ ITabHostTabSpec ** ppTabspec);

    CARAPI Setup();

    CARAPI AddTab(
        /* [in] */ ITabHostTabSpec * pTabSpec);

    CARAPI ClearAllTabs();

    CARAPI GetTabWidget(
        /* [out] */ ITabWidget ** ppTabWidget);

    CARAPI GetCurrentTab(
        /* [out] */ Int32 * pTab);

    CARAPI GetCurrentTabView(
        /* [out] */ IView ** ppView);

    CARAPI GetCurrentView(
        /* [out] */ IView ** ppView);

    CARAPI SetCurrentTabByTag(
        /* [in] */ const String& tag);

    CARAPI GetTabContentView(
        /* [out] */ IFrameLayout ** ppView);

    CARAPI SetCurrentTab(
        /* [in] */ Int32 index);

    CARAPI SetOnTabChangedListener(
        /* [in] */ ITabHostOnTabChangeListener * pL);

    CARAPI constructor(
        /* [in] */ IContext * pContext);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IAttributeSet * pAttrs);

private:
    // TODO: Add your private member variables here.
};

#endif // __CTABHOST_H__
