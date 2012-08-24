
#ifndef __TABHOST_H__
#define __TABHOST_H__

#include "widget/FrameLayout.h"

/**
 * Container for a tabbed window view. This object holds two children: a set of tab labels that the
 * user clicks to select a specific tab, and a FrameLayout object that displays the contents of that
 * page. The individual elements are typically controlled using this container object, rather than
 * setting values on the child elements themselves.
 *
 * <p>See the <a href="{@docRoot}resources/tutorials/views/hello-tabwidget.html">Tab Layout
 * tutorial</a>.</p>
 */
class TabHost : public FrameLayout //implements ViewTreeObserver.OnTouchModeChangeListener 
{
public:
    TabHost();

    TabHost(
        /* [in] */ IContext* context);

    TabHost(
        /* [in] */ IContext* context, 
        /* [in] */ IAttributeSet* attrs);

    /**
     * Get a new {@link TabSpec} associated with this tab host.
     * @param tag required tag of tab.
     */
    virtual CARAPI_(AutoPtr<ITabHostTabSpec>) NewTabSpec(
        /* [in] */ String tag);

    /**
      * <p>Call setup() before adding tabs if loading TabHost using findViewById().
      * <i><b>However</i></b>: You do not need to call setup() after getTabHost()
      * in {@link android.app.TabActivity TabActivity}.
      * Example:</p>
        <pre>mTabHost = (TabHost)findViewById(R.id.tabhost);
        mTabHost.setup();
        mTabHost.addTab(TAB_TAG_1, "Hello, world!", "Tab 1");
      */
    virtual CARAPI Setup();

    /**
     * If you are using {@link TabSpec#setContent(android.content.Intent)}, this
     * must be called since the activityGroup is needed to launch the local activity.
     *
     * This is done for you if you extend {@link android.app.TabActivity}.
     * @param activityGroup Used to launch activities for tab content.
     */
    //virtual CARAPI Setup(
    //    /* [in] */ ILocalActivityManager* activityGroup);

    /**
     * {@inheritDoc}
     */
    virtual CARAPI OnTouchModeChanged(
        /* [in] */ Boolean isInTouchMode);

    /**
     * Add a tab.
     * @param tabSpec Specifies how to create the indicator and content.
     */
    virtual CARAPI AddTab(
        /* [in] */ ITabHostTabSpec* tabSpec);

    /**
     * Removes all tabs from the tab widget associated with this tab host.
     */
    virtual CARAPI ClearAllTabs();

    virtual CARAPI_(AutoPtr<ITabWidget>) GetTabWidget();

    virtual CARAPI_(Int32) GetCurrentTab();

    virtual CARAPI_(String) GetCurrentTabTag();

    virtual CARAPI_(AutoPtr<IView>) GetCurrentTabView();

    virtual CARAPI_(AutoPtr<IView>) GetCurrentView();

    virtual CARAPI SetCurrentTabByTag(
        /* [in] */ String tag);

    /**
     * Get the FrameLayout which holds tab content
     */
    virtual CARAPI_(AutoPtr<IFrameLayout>) GetTabContentView();

    virtual CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event);


    virtual CARAPI DispatchWindowFocusChanged(
        /* [in] */ Boolean hasFocus);

    virtual CARAPI SetCurrentTab(
        /* [in] */ Int32 index);

    /**
     * Register a callback to be invoked when the selected state of any of the items
     * in this list changes
     * @param l
     * The callback that will run
     */
    virtual CARAPI SetOnTabChangedListener(
        /* [in] */ ITabHostOnTabChangeListener* l);

protected:
    virtual CARAPI_(void) OnAttachedToWindow();

    virtual CARAPI_(void) OnDetachedFromWindow();

private:
    CARAPI_(void) InitTabHost();

    CARAPI_(void) InvokeOnTabChangeListener();
    
public:

    /**
     * How to create a tab indicator that just has a label.
     */
    class LabelIndicatorStrategy : public IIndicatorStrategy, public ElRefBase
    {
    public:
        virtual CARAPI CreateIndicatorView(
            /* [out] */ IView** view);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        LabelIndicatorStrategy(
            /* [in] */ ICharSequence* label);

    private:
        AutoPtr<ICharSequence> mLabel; 
    };

    /**
     * How we create a tab indicator that has a label and an icon
     */
    class LabelAndIconIndicatorStrategy : public IIndicatorStrategy, public ElRefBase 
    {
    public:
        virtual CARAPI CreateIndicatorView(
            /* [out] */ IView** view);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        LabelAndIconIndicatorStrategy(
            /* [in] */ ICharSequence* label, 
            /* [in] */ IDrawable* icon);

    private:
        AutoPtr<ICharSequence> mLabel;
        AutoPtr<IDrawable> mIcon;
    };

    /**
     * How to create a tab indicator by specifying a view.
     */
    class ViewIndicatorStrategy : public IIndicatorStrategy, public ElRefBase
    {
    public:
        virtual CARAPI CreateIndicatorView(
            /* [out] */ IView** view);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);
        
        ViewIndicatorStrategy(
            /* [in] */ IView* view);

    private:
        AutoPtr<IView> mView;        
    };

    /**
     * How to create the tab content via a view id.
     */
    class ViewIdContentStrategy : public IContentStrategy, public ElRefBase
    {
    public:
        virtual CARAPI GetContentView(
            /* [out] */ IView** view);

        virtual CARAPI TabClosed();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        ViewIdContentStrategy(
            /* [in] */ Int32 viewId);

    private:
        AutoPtr<IView> mView;        
    };

    /**
     * How tab content is managed using {@link TabContentFactory}.
     */
    class FactoryContentStrategy : public IContentStrategy, public ElRefBase 
    {
    public:
        FactoryContentStrategy(
            /* [in] */ ICharSequence* tag, 
            /* [in] */ ITabHostTabContentFactory* factory);

        virtual CARAPI GetContentView(
            /* [out] */ IView** view);

        virtual CARAPI TabClosed();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    private:
        AutoPtr<IView> mTabContent;
        AutoPtr<ICharSequence> mTag;
        AutoPtr<ITabHostTabContentFactory> mFactory;
    };

    /**
     * How tab content is managed via an {@link Intent}: the content view is the
     * decorview of the launched activity.
     */
    class IntentContentStrategy : public IContentStrategy, public ElRefBase
    {
    public:
        virtual CARAPI GetContentView(
            /* [out] */ IView** view);
        
        virtual CARAPI TabClosed();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        IntentContentStrategy(
            /* [in] */ String tag, 
            /* [in] */ IIntent* intent);

    private:
        String mTag;
        AutoPtr<IIntent> mIntent;

        AutoPtr<IView> mLaunchedView;        
    };

protected:
    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    Int32 mCurrentTab;// = -1;

    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    //LocalActivityManager mLocalActivityManager = NULL;

private:
    AutoPtr<ITabWidget> mTabWidget;
    AutoPtr<IFrameLayout> mTabContent;
    ArrayOf<AutoPtr<ITabHostTabSpec> >* mTabSpecs;// = new ArrayList<TabSpec>(2);
    
    AutoPtr<IView> mCurrentView;// = NULL;
    
    AutoPtr<ITabHostOnTabChangeListener> mOnTabChangeListener;
    AutoPtr<IViewOnKeyListener> mTabKeyListener;
};

#endif
