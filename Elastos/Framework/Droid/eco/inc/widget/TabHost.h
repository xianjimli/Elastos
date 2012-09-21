
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
class TabHost : public FrameLayout
{
private:
    interface IIndicatorStrategy : public IInterface
    {
        /**
         * Return the view for the indicator.
         */
        virtual CARAPI CreateIndicatorView(
            /* [out] */ IView** view) = 0;
    };

    interface IContentStrategy : public IInterface
    {
        /**
         * Return the content view.  The view should may be cached locally.
         */
        virtual CARAPI GetContentView(
            /* [out] */ IView** view) = 0;

        /**
         * Perhaps do something when the tab associated with this content has
         * been closed (i.e make it invisible, or remove it).
         */
        virtual CARAPI TabClosed() = 0;
    };

    /**
     * How to create a tab indicator that just has a label.
     */
    class LabelIndicatorStrategy : public ElRefBase, public IIndicatorStrategy
    {
        friend class TabHost;

    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI CreateIndicatorView(
            /* [out] */ IView** view);

    private:
        LabelIndicatorStrategy(
            /* [in] */ ICharSequence* label,
            /* [in] */ TabHost* owner);

    private:
        AutoPtr<ICharSequence> mLabel;
        TabHost* mOwner;
    };

    /**
     * How we create a tab indicator that has a label and an icon
     */
    class LabelAndIconIndicatorStrategy : public ElRefBase, public IIndicatorStrategy
    {
        friend class TabHost;

    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI CreateIndicatorView(
            /* [out] */ IView** view);

    private:
        LabelAndIconIndicatorStrategy(
            /* [in] */ ICharSequence* label,
            /* [in] */ IDrawable* icon,
            /* [in] */ TabHost* owner);

    private:
        AutoPtr<ICharSequence> mLabel;
        AutoPtr<IDrawable> mIcon;
        TabHost* mOwner;
    };

    /**
     * How to create a tab indicator by specifying a view.
     */
    class ViewIndicatorStrategy : public ElRefBase, public IIndicatorStrategy
    {
        friend class TabHost;

    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI CreateIndicatorView(
            /* [out] */ IView** view);

    private:
        ViewIndicatorStrategy(
            /* [in] */ IView* view);

    private:
        AutoPtr<IView> mView;
    };

    /**
     * How to create the tab content via a view id.
     */
    class ViewIdContentStrategy : public ElRefBase, public IContentStrategy
    {
        friend class TabHost;

    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetContentView(
            /* [out] */ IView** view);

        CARAPI TabClosed();

    private:
        ViewIdContentStrategy(
            /* [in] */ Int32 viewId,
            /* [in] */ TabHost* owner);

    private:
        AutoPtr<IView> mView;
        TabHost* mOwner;
    };

    /**
     * How tab content is managed using {@link TabContentFactory}.
     */
    class FactoryContentStrategy : public ElRefBase, public IContentStrategy
    {
    public:
        FactoryContentStrategy(
            /* [in] */ ICharSequence* tag,
            /* [in] */ ITabHostTabContentFactory* factory);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetContentView(
            /* [out] */ IView** view);

        CARAPI TabClosed();

    private:
        AutoPtr<IView> mTabContent;
        AutoPtr<ICharSequence> mTag;
        AutoPtr<ITabHostTabContentFactory> mFactory;
    };

    /**
     * How tab content is managed via an {@link Intent}: the content view is the
     * decorview of the launched activity.
     */
    class IntentContentStrategy : public ElRefBase, public IContentStrategy
    {
        friend class TabHost;

    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetContentView(
            /* [out] */ IView** view);

        CARAPI TabClosed();

    private:
        IntentContentStrategy(
            /* [in] */ String tag,
            /* [in] */ IIntent* intent);

    private:
        String mTag;
        AutoPtr<IIntent> mIntent;

        AutoPtr<IView> mLaunchedView;
    };

    class TabKeyListener : public ElRefBase, public IViewOnKeyListener
    {
        friend class TabHost;

    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnKey(
            /* [in] */ IView* v,
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* result);

    private:
        TabKeyListener(
            /* [in] */ TabHost* owner);

    private:
        TabHost* mOwner;
    };

    class TabSelectionListener
        : public ITabWidgetOnTabSelectionChanged
        , public ElRefBase
    {
        friend class TabHost;

    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnTabSelectionChanged(
            /* [in] */ Int32 tabIndex,
            /* [in] */ Boolean clicked);

    private:
        TabSelectionListener(
            /* [in] */ TabHost* owner);

    private:
        TabHost* mOwner;
    };

public:
    /**
     * A tab has a tab indicator, content, and a tag that is used to keep
     * track of it.  This builder helps choose among these options.
     *
     * For the tab indicator, your choices are:
     * 1) set a label
     * 2) set a label and an icon
     *
     * For the tab content, your choices are:
     * 1) the id of a {@link View}
     * 2) a {@link TabContentFactory} that creates the {@link View} content.
     * 3) an {@link Intent} that launches an {@link android.app.Activity}.
     */
    class TabSpec : public ElRefBase, public ITabSpec
    {
        friend class TabHost;

    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

         /**
         * Specify a label as the tab indicator.
         */
        CARAPI SetIndicator(
            /* [in] */ ICharSequence* label);

        /**
         * Specify a label and icon as the tab indicator.
         */
        CARAPI SetIndicatorEx(
            /* [in] */ ICharSequence* label,
            /* [in] */ IDrawable* icon);

        /**
         * Specify a view as the tab indicator.
         */
        CARAPI SetIndicatorEx2(
            /* [in] */ IView* view);

        /**
         * Specify the id of the view that should be used as the content
         * of the tab.
         */
        CARAPI SetContent(
            /* [in] */ Int32 viewId);

        /**
         * Specify a {@link android.widget.TabHost.TabContentFactory} to use to
         * create the content of the tab.
         */
        CARAPI SetContentEx(
            /* [in] */ ITabHostTabContentFactory* contentFactory);

        /**
         * Specify an intent to use to launch an activity as the tab content.
         */
        CARAPI SetContentEx2(
            /* [in] */ IIntent* intent);

        CARAPI GetTag(
            /* [out] */ String* tag);

    private:
        TabSpec(
            /* [in] */ const String& tag,
            /* [in] */ TabHost* owner);

    private:
        String mTag;

        AutoPtr<IIndicatorStrategy> mIndicatorStrategy;
        AutoPtr<IContentStrategy> mContentStrategy;
        TabHost* mOwner;
    };

public:
    TabHost(
        /* [in] */ IContext* context);

    TabHost(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    /**
     * Get a new {@link TabSpec} associated with this tab host.
     * @param tag required tag of tab.
     */
    virtual CARAPI_(AutoPtr<ITabSpec>) NewTabSpec(
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
        /* [in] */ ITabSpec* tabSpec);

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

    //@Override
    CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event);

    //@Override
    CARAPI DispatchWindowFocusChanged(
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
    TabHost();

    //@Override
    CARAPI_(void) OnAttachedToWindow();

    //@Override
    virtual CARAPI_(void) OnDetachedFromWindow();

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

private:
    CARAPI_(void) InitTabHost();

    CARAPI_(void) InvokeOnTabChangeListener();

protected:
    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    Int32 mCurrentTab;

    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
//    LocalActivityManager mLocalActivityManager = NULL;

private:
    AutoPtr<ITabWidget> mTabWidget;
    AutoPtr<IFrameLayout> mTabContent;
    List< AutoPtr<TabSpec> > mTabSpecs;

    AutoPtr<IView> mCurrentView;

    AutoPtr<ITabHostOnTabChangeListener> mOnTabChangeListener;
    AutoPtr<IViewOnKeyListener> mTabKeyListener;
};

#endif
