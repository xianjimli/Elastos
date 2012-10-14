
#ifndef __TABWIDGET_H__
#define __TABWIDGET_H__

#include "widget/LinearLayout.h"


/**
 *
 * Displays a list of tab labels representing each page in the parent's tab
 * collection. The container object for this widget is
 * {@link android.widget.TabHost TabHost}. When the user selects a tab, this
 * object sends a message to the parent container, TabHost, to tell it to switch
 * the displayed page. You typically won't use many methods directly on this
 * object. The container TabHost is used to add labels, add the callback
 * handler, and manage callbacks. You might call this object to iterate the list
 * of tabs, or to tweak the layout of the tab list, but most methods should be
 * called on the containing TabHost object.
 *
 * <p>See the <a href="{@docRoot}resources/tutorials/views/hello-tabwidget.html">Tab Layout
 * tutorial</a>.</p>
 *
 * @attr ref android.R.styleable#TabWidget_divider
 * @attr ref android.R.styleable#TabWidget_tabStripEnabled
 * @attr ref android.R.styleable#TabWidget_tabStripLeft
 * @attr ref android.R.styleable#TabWidget_tabStripRight
 */

class TabWidget : public LinearLayout //implements OnFocusChangeListener
{
private:
    // registered with each tab indicator so we can notify tab host
    class TabClickListener : public IViewOnClickListener, public ElRefBase
    {
        friend class TabWidget;

    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        TabClickListener(
            /* [in] */ Int32 tabIndex,
            /* [in] */ TabWidget* owner);

    private:
        Int32 mTabIndex;

        TabWidget* mOwner;
    };

public:
    TabWidget(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0x01010083/*com.android.internal.R.attr.tabWidgetStyle*/);

    /**
     * Returns the tab indicator view at the given index.
     *
     * @param index the zero-based index of the tab indicator view to return
     * @return the tab indicator view at the given index
     */
    virtual CARAPI_(AutoPtr<IView>) GetChildTabViewAt(
        /* [in] */ Int32 index);

    /**
     * Returns the number of tab indicator views.
     * @return the number of tab indicator views.
     */
    virtual CARAPI_(Int32) GetTabCount();

    /**
     * Sets the drawable to use as a divider between the tab indicators.
     * @param drawable the divider drawable
     */
    virtual CARAPI SetDividerDrawable(
        /* [in] */ IDrawable* drawable);

    /**
     * Sets the drawable to use as a divider between the tab indicators.
     * @param resId the resource identifier of the drawable to use as a
     * divider.
     */
    virtual CARAPI SetDividerDrawable(
        /* [in] */ Int32 resId);

    /**
     * Sets the drawable to use as the left part of the strip below the
     * tab indicators.
     * @param drawable the left strip drawable
     */
    virtual CARAPI SetLeftStripDrawable(
        /* [in] */ IDrawable* drawable);

    /**
     * Sets the drawable to use as the left part of the strip below the
     * tab indicators.
     * @param resId the resource identifier of the drawable to use as the
     * left strip drawable
     */
    virtual CARAPI SetLeftStripDrawable(
        /* [in] */ Int32 resId);

    /**
     * Sets the drawable to use as the right part of the strip below the
     * tab indicators.
     * @param drawable the right strip drawable
     */
    virtual CARAPI SetRightStripDrawable(
        /* [in] */ IDrawable* drawable);

    /**
     * Sets the drawable to use as the right part of the strip below the
     * tab indicators.
     * @param resId the resource identifier of the drawable to use as the
     * right strip drawable
     */
    virtual CARAPI SetRightStripDrawable(
        /* [in] */ Int32 resId);

    /**
     * Controls whether the bottom strips on the tab indicators are drawn or
     * not.  The default is to draw them.  If the user specifies a custom
     * view for the tab indicators, then the TabHost class calls this method
     * to disable drawing of the bottom strips.
     * @param stripEnabled true if the bottom strips should be drawn.
     */
    virtual CARAPI SetStripEnabled(
        /* [in] */ Boolean stripEnabled);

    /**
     * Indicates whether the bottom strips on the tab indicators are drawn
     * or not.
     */
    virtual CARAPI_(Boolean) IsStripEnabled();

    //@Override
    CARAPI ChildDrawableStateChanged(
        /* [in] */ IView* child);

    //@Override
    virtual CARAPI_(void) DispatchDraw(
        /* [in] */ ICanvas* canvas);

    /**
     * Sets the current tab.
     * This method is used to bring a tab to the front of the Widget,
     * and is used to post to the rest of the UI that a different tab
     * has been brought to the foreground.
     *
     * Note, this is separate from the traditional "focus" that is
     * employed from the view logic.
     *
     * For instance, if we have a list in a tabbed view, a user may be
     * navigating up and down the list, moving the UI focus (orange
     * highlighting) through the list items.  The cursor movement does
     * not effect the "selected" tab though, because what is being
     * scrolled through is all on the same tab.  The selected tab only
     * changes when we navigate between tabs (moving from the list view
     * to the next tabbed view, in this example).
     *
     * To move both the focus AND the selected tab at once, please use
     * {@link #setCurrentTab}. Normally, the view logic takes care of
     * adjusting the focus, so unless you're circumventing the UI,
     * you'll probably just focus your interest here.
     *
     *  @param index The tab that you want to indicate as the selected
     *  tab (tab brought to the front of the widget)
     *
     *  @see #focusCurrentTab
     */
    virtual CARAPI SetCurrentTab(
        /* [in] */ Int32 index);

    /**
     * Sets the current tab and focuses the UI on it.
     * This method makes sure that the focused tab matches the selected
     * tab, normally at {@link #setCurrentTab}.  Normally this would not
     * be an issue if we go through the UI, since the UI is responsible
     * for calling TabWidget.onFocusChanged(), but in the case where we
     * are selecting the tab programmatically, we'll need to make sure
     * focus keeps up.
     *
     *  @param index The tab that you want focused (highlighted in orange)
     *  and selected (tab brought to the front of the widget)
     *
     *  @see #setCurrentTab
     */
    virtual CARAPI FocusCurrentTab(
        /* [in] */ Int32 index);

    //@Override
    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    using LinearLayout::AddView;

    //@Override
    CARAPI AddView(
        /* [in] */ IView* child);
    /**
     * Provides a way for {@link TabHost} to be notified that the user clicked on a tab indicator.
     */
    virtual CARAPI_(void) SetTabSelectionListener(
        /* [in] */ ITabWidgetOnTabSelectionChanged* listener);

    virtual CARAPI OnFocusChange(
        /* [in] */ IView* v,
        /* [in] */ Boolean hasFocus);

protected:
    TabWidget();

    //@Override
    CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

    //@Override
    CARAPI_(Int32) GetChildDrawingOrder(
        /* [in] */ Int32 childCount,
        /* [in] */ Int32 i);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0x01010083/*com.android.internal.R.attr.tabWidgetStyle*/);

private:
    CARAPI_(void) InitTabWidget();

    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

private:

    AutoPtr<ITabWidgetOnTabSelectionChanged> mSelectionChangedListener;

    Int32 mSelectedTab;

    AutoPtr<IDrawable> mLeftStrip;
    AutoPtr<IDrawable> mRightStrip;

    Boolean mDrawBottomStrips;
    Boolean mStripMoved;

    AutoPtr<IDrawable> mDividerDrawable;

    AutoPtr<CRect> mBounds;

};

#endif
