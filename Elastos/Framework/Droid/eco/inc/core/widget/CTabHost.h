
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
        /* [out] */ ITabSpec** tabspec);

    CARAPI Setup();

    CARAPI AddTab(
        /* [in] */ ITabSpec* tabSpec);

    CARAPI ClearAllTabs();

    CARAPI GetTabWidget(
        /* [out] */ ITabWidget** tabWidget);

    CARAPI GetCurrentTab(
        /* [out] */ Int32* tab);

    CARAPI GetCurrentTabTag(
        /* [out] */ String* tag);

    CARAPI GetCurrentTabView(
        /* [out] */ IView** view);

    CARAPI GetCurrentView(
        /* [out] */ IView** view);

    CARAPI SetCurrentTabByTag(
        /* [in] */ const String& tag);

    CARAPI GetTabContentView(
        /* [out] */ IFrameLayout** view);

    CARAPI SetCurrentTab(
        /* [in] */ Int32 index);

    CARAPI SetOnTabChangedListener(
        /* [in] */ ITabHostOnTabChangeListener* l);

    CARAPI OnTouchModeChanged(
        /* [in] */ Boolean isInTouchMode);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

private:
    // TODO: Add your private member variables here.
};

#endif // __CTABHOST_H__
