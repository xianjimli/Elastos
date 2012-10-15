
#ifndef __ABSLISTVIEW_H__
#define __ABSLISTVIEW_H__

#include "widget/AdapterView.h"
#include "widget/EdgeGlow.h"
#include "view/VelocityTracker.h"

class OverScroller;
class FastScroller;

class AbsListView : public AdapterView
{
    friend class FastScroller;
protected:
    class CheckForTap : public Runnable
    {
    public:
        CheckForTap(
            /* [in] */ AbsListView* host);

        CARAPI Run();

    private:
        AbsListView* mHost;
    };

    /**
     * The RecycleBin facilitates reuse of views across layouts. The RecycleBin has two levels of
     * storage: ActiveViews and ScrapViews. ActiveViews are those views which were onscreen at the
     * start of a layout. By construction, they are displaying current information. At the end of
     * layout, all views in ActiveViews are demoted to ScrapViews. ScrapViews are old views that
     * could potentially be used by the adapter to avoid allocating views unnecessarily.
     *
     * @see android.widget.AbsListView#setRecyclerListener(android.widget.AbsListView.RecyclerListener)
     * @see android.widget.AbsListView.RecyclerListener
     */
    class RecycleBin
    {
        friend class AbsListView;
        friend class ListView;
    public:
        RecycleBin(
            /* [in] */ AbsListView* host);

        ~RecycleBin();

        CARAPI SetViewTypeCount(
            /* [in] */ Int32 viewTypeCount);

        CARAPI_(void) MarkChildrenDirty();

        CARAPI_(Boolean) ShouldRecycleViewType(
            /* [in] */ Int32 viewType);

    protected:
        CARAPI_(void) Clear();

        CARAPI_(void) FillActiveViews(
            /* [in] */ Int32 childCount,
            /* [in] */ Int32 firstActivePosition);

        CARAPI_(AutoPtr<IView>) GetActiveView(
            /* [in] */ Int32 position);

        CARAPI_(AutoPtr<IView>) GetScrapView(
            /* [in] */ Int32 position);

        CARAPI_(void) AddScrapView(
            /* [in ]*/ IView* scrap);

        CARAPI_(void) ScrapActiveViews();

        CARAPI_(void) ReclaimScrapViews(
            /* [in] */ IObjectContainer* views);

        CARAPI_(void) SetCacheColorHint(
            /* [in] */ Int32 color);

    private:
        CARAPI_(void) PruneScrapViews();

    private:
        AutoPtr<IRecyclerListener> mRecyclerListener;

        /**
         * The position of the first view stored in mActiveViews.
         */
        Int32 mFirstActivePosition;

        /**
         * Views that were on screen at the start of layout. This array is populated at the start of
         * layout, and at the end of layout all view in mActiveViews are moved to mScrapViews.
         * Views in mActiveViews represent a contiguous range of Views, with position of the first
         * view store in mFirstActivePosition.
         */
        AutoPtr<IView>* mActiveViews;

        Int32 mActiveViewsLength;

        /**
         * Unsorted views that can be used by the adapter as a convert view.
         */
        Vector<AutoPtr<IView> >* mScrapViews;

        Int32 mViewTypeCount;

        Vector<AutoPtr<IView> > mCurrentScrap;

        AbsListView* mHost;
    };

private:
    /**
     * A base class for Runnables that will check that their view is still attached to
     * the original window as when the Runnable was created.
     *
     */
    class WindowRunnnable
    {
    public:
        WindowRunnnable(
            /* [in] */ AbsListView* host);

        CARAPI_(void) RememberWindowAttachCount();

        CARAPI_(Boolean) SameWindow();

    protected:
        AbsListView* mHost;

    private:
        Int32 mOriginalAttachCount;
    };

    class AbsListViewPerformClick : public WindowRunnnable, public Runnable
    {
        friend class AbsListView;
    public:
        AbsListViewPerformClick (
            /* [in] */ AbsListView* host);

        CARAPI Run();

    protected:
        AutoPtr<IView> mChild;
        Int32 mClickMotionPosition;
    };

    class CheckForLongPress : public WindowRunnnable, public Runnable
    {
    public:
        CheckForLongPress (
            /* [in] */ AbsListView* host);

        CARAPI Run();
    };

    class CheckForKeyLongPress : public WindowRunnnable, public Runnable
    {
    public:
        CheckForKeyLongPress (
            /* [in] */ AbsListView* host);

        CARAPI Run();
    };

    class OnTouchEventRunnable : public Runnable
    {
    public:
        OnTouchEventRunnable(
            /* [in] */ AbsListView* host,
            /* [in] */ IView* child);

        CARAPI Run();

    private:
        AbsListView* mHost;
        AutoPtr<IView> mChild;
    };

    /**
     * Responsible for fling behavior. Use {@link #start(Int32)} to
     * initiate a fling. Each frame of the fling is handled in {@link #run()}.
     * A FlingRunnable will keep re-posting itself until the fling is done.
     *
     */
    class FlingRunnable : public Runnable
    {
        friend class AbsListView;
    protected:
        FlingRunnable(
            /* [in] */ AbsListView* host);

    public:
        ~FlingRunnable();

        CARAPI Run();

    protected:
        CARAPI_(void) Start(
            /* [in] */ Int32 initialVelocity);

        CARAPI_(void) StartSpringback();

        CARAPI_(void) StartOverfling(
            /* [in] */ Int32 initialVelocity);

        CARAPI_(void) EdgeReached(
            /* [in] */ Int32 delta);

        CARAPI_(void) StartScroll(
            /* [in] */ Int32 distance,
            /* [in] */ Int32 duration);

    private:
        CARAPI_(void) EndFling();

    private:
        /**
         * Tracks the decay of a fling scroll
         */
        OverScroller* mScroller;

        /**
         * Y value reported by mScroller on the previous fling
         */
        Int32 mLastFlingY;

        AbsListView* mHost;
    };

    class PositionScroller : public Runnable
    {
        friend class AbsListView;
    protected:
        PositionScroller(
            /* [in] */ AbsListView* host);

    public:
        CARAPI Run();

    protected:
        CARAPI_(void) Start(
            /* [in] */ Int32 position);

        CARAPI_(void) Start(
            /* [in] */ Int32 position,
            /* [in] */ Int32 boundPosition);

        CARAPI_(void) Stop();

    private:
        static const Int32 SCROLL_DURATION = 400;

        static const Int32 MOVE_DOWN_POS = 1;
        static const Int32 MOVE_UP_POS = 2;
        static const Int32 MOVE_DOWN_BOUND = 3;
        static const Int32 MOVE_UP_BOUND = 4;

    private:
        Int32 mMode;
        Int32 mTargetPos;
        Int32 mBoundPos;
        Int32 mLastSeenPos;
        Int32 mScrollDuration;
        Int32 mExtraScroll; //const
        AbsListView* mHost;
    };

    class ClearScrollingCacheRunnable : public Runnable
    {
    public:
        ClearScrollingCacheRunnable(
            /* [in] */ AbsListView* host);

        CARAPI Run();

    private:
        AbsListView* mHost;
    };

public:
    AbsListView();

    AbsListView(
        /* [in] */ IContext* context);

    AbsListView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle = 0x0101006a/* com.android.internal.R.attr.absListViewStyle */);

    ~AbsListView();

    //@Override
    CARAPI SetOverScrollMode(
        /* [in] */ Int32 mode);

    /**
     * Enables fast scrolling by letting the user quickly scroll through lists by
     * dragging the fast scroll thumb. The adapter attached to the list may want
     * to implement {@link SectionIndexer} if it wishes to display alphabet preview and
     * jump between sections of the list.
     * @see SectionIndexer
     * @see #isFastScrollEnabled()
     * @param enabled whether or not to enable fast scrolling
     */
    virtual CARAPI SetFastScrollEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Returns the current state of the fast scroll feature.
     * @see #setFastScrollEnabled(Boolean)
     * @return TRUE if fast scroll is enabled, FALSE otherwise
     */
    virtual CARAPI_(Boolean) IsFastScrollEnabled();

    /**
     * When smooth scrollbar is enabled, the position and size of the scrollbar thumb
     * is computed based on the number of visible pixels in the visible items. This
     * however assumes that all list items have the same height. If you use a list in
     * which items have different heights, the scrollbar will change appearance as the
     * user scrolls through the list. To avoid this issue, you need to disable this
     * property.
     *
     * When smooth scrollbar is disabled, the position and size of the scrollbar thumb
     * is based solely on the number of items in the adapter and the position of the
     * visible items inside the adapter. This provides a stable scrollbar as the user
     * navigates through a list of items with varying heights.
     *
     * @param enabled Whether or not to enable smooth scrollbar.
     *
     * @see #setSmoothScrollbarEnabled(Boolean)
     * @attr ref android.R.styleable#AbsListView_smoothScrollbar
     */
    virtual CARAPI SetSmoothScrollbarEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Returns the current state of the fast scroll feature.
     *
     * @return True if smooth scrollbar is enabled is enabled, FALSE otherwise.
     *
     * @see #setSmoothScrollbarEnabled(Boolean)
     */
    //@ViewDebug.ExportedProperty
    virtual CARAPI_(Boolean) IsSmoothScrollbarEnabled();

    /**
     * Set the listener that will receive notifications every time the list scrolls.
     *
     * @param l the scroll listener
     */
    virtual CARAPI SetOnScrollListener(
        /* [in] */ IOnScrollListener* l);

    /**
     * Indicates whether the children's drawing cache is used during a scroll.
     * By default, the drawing cache is enabled but this will consume more memory.
     *
     * @return TRUE if the scrolling cache is enabled, FALSE otherwise
     *
     * @see #setScrollingCacheEnabled(Boolean)
     * @see View#setDrawingCacheEnabled(Boolean)
     */
    //@ViewDebug.ExportedProperty
    virtual CARAPI_(Boolean) IsScrollingCacheEnabled();

    /**
     * Enables or disables the children's drawing cache during a scroll.
     * By default, the drawing cache is enabled but this will use more memory.
     *
     * When the scrolling cache is enabled, the caches are kept after the
     * first scrolling. You can manually clear the cache by calling
     * {@link android.view.ViewGroup#setChildrenDrawingCacheEnabled(Boolean)}.
     *
     * @param enabled TRUE to enable the scroll cache, FALSE otherwise
     *
     * @see #isScrollingCacheEnabled()
     * @see View#setDrawingCacheEnabled(Boolean)
     */
    virtual CARAPI SetScrollingCacheEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Enables or disables the type filter window. If enabled, typing when
     * this view has focus will filter the children to match the users input.
     * Note that the {@link Adapter} used by this view must implement the
     * {@link Filterable} interface.
     *
     * @param textFilterEnabled TRUE to enable type filtering, FALSE otherwise
     *
     * @see Filterable
     */
    virtual CARAPI SetTextFilterEnabled(
        /* [in] */ Boolean textFilterEnabled);

    /**
     * Indicates whether type filtering is enabled for this view
     *
     * @return TRUE if type filtering is enabled, FALSE otherwise
     *
     * @see #setTextFilterEnabled(Boolean)
     * @see Filterable
     */
    //@ViewDebug.ExportedProperty
    virtual CARAPI_(Boolean) IsTextFilterEnabled();

    //@Override
    CARAPI GetFocusedRect(
        /* [in] */ IRect* r);

    /**
     * Indicates whether the content of this view is pinned to, or stacked from,
     * the bottom edge.
     *
     * @return TRUE if the content is stacked from the bottom edge, FALSE otherwise
     */
    //@ViewDebug.ExportedProperty
    virtual CARAPI_(Boolean) IsStackFromBottom();

    /**
     * When stack from bottom is set to TRUE, the list fills its content starting from
     * the bottom of the view.
     *
     * @param stackFromBottom TRUE to pin the view's content to the bottom edge,
     *        FALSE to pin the view's content to the top edge
     */
    virtual CARAPI SetStackFromBottom(
        /* [in] */ Boolean stackFromBottom);

    //@Override
    CARAPI_(AutoPtr<IParcelable>) OnSaveInstanceState();

    //@Override
    CARAPI_(void) OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

    /**
     * Sets the initial value for the text filter.
     * @param filterText The text to use for the filter.
     *
     * @see #setTextFilterEnabled
     */
    virtual CARAPI SetFilterText(
        /* [in] */ const String& filterText);

    /**
     * Returns the list's text filter, if available.
     * @return the list's text filter or NULL if filtering isn't enabled
     */
    virtual CARAPI_(AutoPtr<ICharSequence>) GetTextFilter();

    //@Override
    //@ViewDebug.ExportedProperty
    CARAPI_(AutoPtr<IView>) GetSelectedView();

    /**
     * List padding is the maximum of the normal view's padding and the padding of the selector.
     *
     * @see android.view.View#getPaddingTop()
     * @see #getSelector()
     *
     * @return The top list padding.
     */
    virtual CARAPI_(Int32) GetListPaddingTop();

    /**
     * List padding is the maximum of the normal view's padding and the padding of the selector.
     *
     * @see android.view.View#getPaddingBottom()
     * @see #getSelector()
     *
     * @return The bottom list padding.
     */
    virtual CARAPI_(Int32) GetListPaddingBottom();

    /**
     * List padding is the maximum of the normal view's padding and the padding of the selector.
     *
     * @see android.view.View#getPaddingLeft()
     * @see #getSelector()
     *
     * @return The left list padding.
     */
    virtual CARAPI_(Int32) GetListPaddingLeft();

    /**
     * List padding is the maximum of the normal view's padding and the padding of the selector.
     *
     * @see android.view.View#getPaddingRight()
     * @see #getSelector()
     *
     * @return The right list padding.
     */
    virtual CARAPI_(Int32) GetListPaddingRight();

    /**
     * Controls whether the selection highlight drawable should be drawn on top of the item or
     * behind it.
     *
     * @param onTop If TRUE, the selector will be drawn on the item it is highlighting. The default
     *        is FALSE.
     *
     * @attr ref android.R.styleable#AbsListView_drawSelectorOnTop
     */
    virtual CARAPI SetDrawSelectorOnTop(
        /* [in] */ Boolean onTop);

    /**
     * Set a Drawable that should be used to highlight the currently selected item.
     *
     * @param resID A Drawable resource to use as the selection highlight.
     *
     * @attr ref android.R.styleable#AbsListView_listSelector
     */
    virtual CARAPI SetSelector(
        /* [in] */ Int32 resID);

    virtual CARAPI SetSelector(
        /* [in] */ IDrawable* sel);

    /**
     * Returns the selector {@link android.graphics.drawable.Drawable} that is used to draw the
     * selection in the list.
     *
     * @return the drawable used to display the selector
     */
    virtual CARAPI_(AutoPtr<IDrawable>) GetSelector();

    virtual CARAPI SetScrollIndicators(
        /* [in] */ IView* up,
        /* [in] */ IView* down);

    //@Override
    CARAPI_(Boolean) VerifyDrawable(
        /* [in] */ IDrawable* dr);

    //@Override
    CARAPI OnWindowFocusChanged(
        /* [in] */ Boolean hasWindowFocus);

    //@Override
    CARAPI_(Boolean) ShowContextMenuForChild(
        /* [in] */ IView* originalView);

    //@Override
    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    //@Override
    CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    /**
     * Maps a point to a position in the list.
     *
     * @param x X in local coordinate
     * @param y Y in local coordinate
     * @return The position of the item which contains the specified point, or
     *         {@link #AdapterView_INVALID_POSITION} if the point does not intersect an item.
     */
    virtual CARAPI_(Int32) PointToPosition(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    /**
     * Maps a point to a the rowId of the item which intersects that point.
     *
     * @param x X in local coordinate
     * @param y Y in local coordinate
     * @return The rowId of the item which contains the specified point, or {@link #AdapterView_INVALID_ROW_ID}
     *         if the point does not intersect an item.
     */
    virtual CARAPI_(Int64) PointToRowId(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    virtual CARAPI OnTouchModeChanged(
        /* [in] */ Boolean isInTouchMode);

    //@Override
    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* ev);

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    //@Override
    CARAPI_(Boolean) OnInterceptTouchEvent(
        /* [in] */ IMotionEvent* ev);

    /**
     * {@inheritDoc}
     */
    //@Override
    CARAPI AddTouchables(
        /* [in] */ IObjectContainer* views);

    /**
     * Smoothly scroll to the specified adapter position. The view will
     * scroll such that the indicated position is displayed.
     * @param position Scroll to this adapter position.
     */
    virtual CARAPI SmoothScrollToPosition(
        /* [in] */ Int32 position);

    /**
     * Smoothly scroll to the specified adapter position. The view will
     * scroll such that the indicated position is displayed, but it will
     * stop early if scrolling further would scroll boundPosition out of
     * view.
     * @param position Scroll to this adapter position.
     * @param boundPosition Do not scroll if it would move this adapter
     *          position out of view.
     */
    virtual CARAPI SmoothScrollToPosition(
        /* [in] */ Int32 position,
        /* [in] */ Int32 boundPosition);

    /**
     * Smoothly scroll by distance pixels over duration milliseconds.
     * @param distance Distance to scroll in pixels.
     * @param duration Duration of the scroll animation in milliseconds.
     */
    virtual CARAPI SmoothScrollBy(
        /* [in] */ Int32 distance,
        /* [in] */ Int32 duration);

    /**
     * Causes all the views to be rebuilt and redrawn.
     */
    virtual CARAPI InvalidateViews();

    /**
     * Return an InputConnection for editing of the filter text.
     */
    //@Override
    //InputConnection onCreateInputConnection(EditorInfo outAttrs);

    /**
     * For filtering we proxy an input connection to an internal text editor,
     * and this allows the proxying to happen.
     */
    //@Override
    CARAPI_(Boolean) CheckInputConnectionProxy(
        /* [in] */ IView* view);

    /**
     * Clear the text filter.
     */
    virtual CARAPI ClearTextFilter();

    /**
     * Returns if the ListView currently has a text filter.
     */
    virtual CARAPI_(Boolean) HasTextFilter();

    virtual CARAPI OnGlobalLayout();

    /**
     * For our text watcher that is associated with the text filter.  Does
     * nothing.
     */
    virtual CARAPI BeforeTextChanged(
        /* [in] */ ICharSequence* s,
        /* [in] */ Int32 start,
        /* [in] */ Int32 count,
        /* [in] */ Int32 after);

    /**
     * For our text watcher that is associated with the text filter. Performs
     * the actual filtering as the text changes, and takes care of hiding and
     * showing the popup displaying the currently entered filter text.
     */
    virtual CARAPI OnTextChanged(
        /* [in] */ ICharSequence* s,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 count);

    virtual CARAPI AfterTextChanged(
        /* [in] */ IEditable* s);

    virtual CARAPI OnFilterComplete(
        /* [in] */ Int32 count);

    //@Override
    CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs);

    /**
     * Puts the list or grid into transcript mode. In this mode the list or grid will always scroll
     * to the bottom to show new items.
     *
     * @param mode the transcript mode to set
     *
     * @see #AbsListView_TRANSCRIPT_MODE_DISABLED
     * @see #AbsListView_TRANSCRIPT_MODE_NORMAL
     * @see #AbsListView_TRANSCRIPT_MODE_ALWAYS_SCROLL
     */
    virtual CARAPI SetTranscriptMode(
        /* [in] */ Int32 mode);

    /**
     * Returns the current transcript mode.
     *
     * @return {@link #AbsListView_TRANSCRIPT_MODE_DISABLED}, {@link #AbsListView_TRANSCRIPT_MODE_NORMAL} or
     *         {@link #AbsListView_TRANSCRIPT_MODE_ALWAYS_SCROLL}
     */
    virtual CARAPI_(Int32) GetTranscriptMode();

    //@Override
    CARAPI_(Int32) GetSolidColor();

    /**
     * When set to a non-zero value, the cache color hint indicates that this list is always drawn
     * on top of a solid, single-color, opaque background
     *
     * @param color The background color
     */
    virtual CARAPI SetCacheColorHint(
        /* [in] */ Int32 color);

    /**
     * When set to a non-zero value, the cache color hint indicates that this list is always drawn
     * on top of a solid, single-color, opaque background
     *
     * @return The cache color hint
     */
    virtual CARAPI_(Int32) GetCacheColorHint();

    /**
     * Move all views (excluding headers and footers) held by this AbsListView into the supplied
     * List. This includes views displayed on the screen as well as views stored in AbsListView's
     * internal view recycler.
     *
     * @param views A list into which to put the reclaimed views
     */
    virtual CARAPI ReclaimViews(
        /* [in] */ IObjectContainer* views);

    /**
     * Sets the recycler listener to be notified whenever a View is set aside in
     * the recycler for later reuse. This listener can be used to free resources
     * associated to the View::
     *
     * @param listener The recycler listener to be notified of views set aside
     *        in the recycler.
     *
     * @see android.widget.AbsListView.RecycleBin
     * @see android.widget.AbsListView.RecyclerListener
     */
    virtual CARAPI SetRecyclerListener(
        /* [in] */ IRecyclerListener* listener);

protected:
    static CARAPI_(Int32) GetDistance(
        /* [in] */ IRect* sourceRect,
        /* [in] */ IRect* destRect,
        /* [in] */ Int32 direction);

protected:
    //@Override
    CARAPI_(Boolean) IsVerticalScrollBarHidden();

    virtual CARAPI InvokeOnItemScrollListener();

    virtual CARAPI RequestLayoutIfNecessary();

    //@Override
    CARAPI_(void) OnFocusChanged(
        /* [in] */ Boolean gainFocus,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    //@Override
    CARAPI RequestLayout();

    virtual CARAPI_(void) ResetList();

    //@Override
    CARAPI_(Int32) ComputeVerticalScrollExtent();

    //@Override
    CARAPI_(Int32) ComputeVerticalScrollOffset();

    //@Override
    CARAPI_(Int32) ComputeVerticalScrollRange();

    //@Override
    CARAPI_(Float) GetTopFadingEdgeStrength();

    //@Override
    CARAPI_(Float) GetBottomFadingEdgeStrength();

    //@Override
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    //@Override
    CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    //@Override
    CARAPI_(Boolean) SetFrame(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI_(void) LayoutChildren();

    virtual CARAPI_(void) UpdateScrollIndicators();

    virtual CARAPI_(AutoPtr<IView>) ObtainView(
        /* [in] */ Int32 position,
        /* [in] */ Boolean* isScrap);

    virtual CARAPI_(void) PositionSelector(
        /* [in] */ IView* sel);

    //@Override
    CARAPI_(void) DispatchDraw(
        /* [in] */ ICanvas* canvas);

    //@Override
    CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

    virtual CARAPI_(Boolean) TouchModeDrawsInPressedState();

    virtual CARAPI_(Boolean) ShouldShowSelector();

    virtual CARAPI_(void) KeyPressed();

    //@Override
    CARAPI DrawableStateChanged();

    //@Override
    CARAPI OnCreateDrawableState(
        /* [in] */ Int32 extraSpace,
        /* [out] */ ArrayOf<Int32>** drawableState);

    //@Override
    CARAPI_(void) OnAttachedToWindow();

    //@Override
    CARAPI_(void) OnDetachedFromWindow();

    virtual CARAPI_(AutoPtr<IContextMenuInfo>) CreateContextMenuInfo(
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    //@Override
    CARAPI_(AutoPtr<IContextMenuInfo>) GetContextMenuInfo();

    //@Override
    CARAPI_(void) DispatchSetPressed(
        /* [in] */ Boolean pressed);

    //@Override
    CARAPI_(void) OnOverScrolled(
        /* [in] */ Int32 scrollX,
        /* [in] */ Int32 scrollY,
        /* [in] */ Boolean clampedX,
        /* [in] */ Boolean clampedY);

    virtual CARAPI_(void) ReportScrollStateChange(
        /* [in] */ Int32 newState);

    virtual CARAPI_(Boolean) TrackMotionScroll(
        /* [in] */ Int32 deltaY,
        /* [in] */ Int32 incrementalDeltaY);

    virtual CARAPI_(Int32) GetHeaderViewsCount();

    virtual CARAPI_(Int32) GetFooterViewsCount();

    virtual CARAPI_(void) FillGap(
        /* [in] */ Boolean down) = 0;

    virtual CARAPI_(void) HideSelector();

    virtual CARAPI_(Int32) ReconcileSelectedPosition();

    virtual CARAPI_(Int32) FindMotionRow(
        /* [in] */ Int32 y) = 0;

    virtual CARAPI_(Int32) FindClosestMotionRow(
        /* [in] */ Int32 y);

    virtual CARAPI_(void) SetSelectionInt(
        /* [in] */ Int32 position) = 0;

    virtual CARAPI_(Boolean) ResurrectSelection();

    //@Override
    CARAPI_(void) HandleDataChanged();

    //@Override
    CARAPI_(void) OnDisplayHint(
        /* [in] */ Int32 hint);

    //@Override
    CARAPI_(Boolean) IsInFilterMode();

    virtual CARAPI_(Boolean) SendToTextFilter(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 count,
        /* [in] */ IKeyEvent* event);

    //@Override
    CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

    //@Override
    CARAPI_(Boolean) CheckLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

    //@Override
    CARAPI_(Boolean) OnConsistencyCheck(
        /* [in] */ Int32 consistency);

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle = 0x0101006a/*com.android.internal.R.attr.absListViewStyle*/);

private:
    CARAPI_(void) InitAbsListView();

    CARAPI_(Boolean) ContentFits();

    CARAPI_(void) UseDefaultSelector();

    CARAPI_(Boolean) AcceptFilter();

    CARAPI_(void) PositionSelector(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI_(void) DrawSelector(
        /* [in] */ ICanvas* canvas);

    CARAPI_(Boolean) PerformLongPress(
        /* [in] */ IView* child,
        /* [in] */ Int32 longPressPosition,
        /* [in] */ Int64 longPressId);

    CARAPI_(Boolean) StartScrollIfNeeded(
        /* [in] */ Int32 deltaY);

    CARAPI_(void) OnSecondaryPointerUp(
        /* [in] */ IMotionEvent* ev);

    CARAPI_(void) CreateScrollingCache();

    CARAPI_(void) ClearScrollingCache();

    CARAPI_(void) DismissPopup();

    CARAPI_(void) ShowPopup();

    CARAPI_(void) PositionPopup();

    CARAPI_(void) CreateTextFilter(
        /* [in] */ Boolean animateEntrance);

    CARAPI_(Boolean) CheckScrap(
        /* [in] */ Vector<AutoPtr<IView> >& scrap);

    CARAPI_(void) FinishGlows();

protected:
    /**
     * Indicates that we are not in the middle of a touch gesture
     */
    static const Int32 TOUCH_MODE_REST = -1;

    /**
     * Indicates we just received the touch event and we are waiting to see if the it is a tap or a
     * scroll gesture.
     */
    static const Int32 TOUCH_MODE_DOWN = 0;

    /**
     * Indicates the touch has been recognized as a tap and we are now waiting to see if the touch
     * is a longpress
     */
    static const Int32 TOUCH_MODE_TAP = 1;

    /**
     * Indicates we have waited for everything we can wait for, but the user's finger is still down
     */
    static const Int32 TOUCH_MODE_DONE_WAITING = 2;

    /**
     * Indicates the touch gesture is a scroll
     */
    static const Int32 TOUCH_MODE_SCROLL = 3;

    /**
     * Indicates the view is in the process of being flung
     */
    static const Int32 TOUCH_MODE_FLING = 4;

    /**
     * Indicates the touch gesture is an overscroll - a scroll beyond the beginning or end.
     */
    static const Int32 TOUCH_MODE_OVERSCROLL = 5;

    /**
     * Indicates the view is being flung outside of normal content bounds
     * and will spring back.
     */
    static const Int32 TOUCH_MODE_OVERFLING = 6;

    /**
     * Regular layout - usually an unsolicited layout from the view system
     */
    static const Int32 LAYOUT_NORMAL = 0;

    /**
     * Show the first item
     */
    static const Int32 LAYOUT_FORCE_TOP = 1;

    /**
     * Force the selected item to be on somewhere on the screen
     */
    static const Int32 LAYOUT_SET_SELECTION = 2;

    /**
     * Show the last item
     */
    static const Int32 LAYOUT_FORCE_BOTTOM = 3;

    /**
     * Make a mSelectedItem appear in a specific location and build the rest of
     * the views from there. The top is specified by mSpecificTop.
     */
    static const Int32 LAYOUT_SPECIFIC = 4;

    /**
     * Layout to sync as a result of a data change. Restore mSyncPosition to have its top
     * at mSpecificTop
     */
    static const Int32 LAYOUT_SYNC = 5;

    /**
     * Layout as a result of using the navigation keys
     */
    static const Int32 LAYOUT_MOVE_SELECTION = 6;

    /**
     * Content height divided by this is the overscroll limit.
     */
    static const Int32 OVERSCROLL_LIMIT_DIVISOR = 3;

private:
    /**
     * Used to request a layout when we changed touch mode
     */
    static const Int32 TOUCH_MODE_UNKNOWN = -1;
    static const Int32 TOUCH_MODE_ON = 0;
    static const Int32 TOUCH_MODE_OFF = 1;
    static const Boolean PROFILE_SCROLLING = FALSE;
    static const Boolean PROFILE_FLINGING = FALSE;

    /**
     * Sentinel value for no current active pointer.
     * Used by {@link #mActivePointerId}.
     */
    static const Int32 INVALID_POINTER = -1;

protected:
    /**
     * Controls how the next layout will happen
     */
    Int32 mLayoutMode;

    /**
     * Should be used by subclasses to listen to changes in the dataset
     */
    AutoPtr<AdapterDataSetObserver> mDataSetObserver;

    /**
     * The adapter containing the data to be displayed by this view
     */
    AutoPtr<IListAdapter> mAdapter;

    /**
     * Indicates whether the list selector should be drawn on top of the children or behind
     */
    Boolean mDrawSelectorOnTop;

    /**
     * The drawable used to draw the selector
     */
    AutoPtr<IDrawable> mSelector;

    /**
     * Defines the selector's location and dimension at drawing time
     */
    AutoPtr<CRect> mSelectorRect;

    /**
     * The data set used to store unused views that should be reused during the next layout
     * to avoid creating new ones
     */
    RecycleBin* mRecycler;

    /**
     * The selection's left padding
     */
    Int32 mSelectionLeftPadding;

    /**
     * The selection's top padding
     */
    Int32 mSelectionTopPadding;

    /**
     * The selection's right padding
     */
    Int32 mSelectionRightPadding;

    /**
     * The selection's bottom padding
     */
    Int32 mSelectionBottomPadding;

    /**
     * This view's padding
     */
    AutoPtr<CRect> mListPadding;

    /**
     * Subclasses must retain their measure spec from onMeasure() into this member
     */
    Int32 mWidthMeasureSpec;

    /**
     * The top scroll indicator
     */
    AutoPtr<IView> mScrollUp;

    /**
     * The down scroll indicator
     */
    AutoPtr<IView> mScrollDown;

    /**
     * When the view is scrolling, this flag is set to TRUE to indicate subclasses that
     * the drawing cache was enabled on the children
     */
    Boolean mCachingStarted;

    /**
     * The position of the view that received the down motion event
     */
    Int32 mMotionPosition;

    /**
     * The offset to the top of the mMotionPosition view when the down motion event was received
     */
    Int32 mMotionViewOriginalTop;

    /**
     * The desired offset to the top of the mMotionPosition view after a scroll
     */
    Int32 mMotionViewNewTop;

    /**
     * The X value associated with the the down motion event
     */
    Int32 mMotionX;

    /**
     * The Y value associated with the the down motion event
     */
    Int32 mMotionY;

    /**
     * One of TOUCH_MODE_REST, TOUCH_MODE_DOWN, TOUCH_MODE_TAP, TOUCH_MODE_SCROLL, or
     * TOUCH_MODE_DONE_WAITING
     */
    Int32 mTouchMode;

    /**
     * Y value from on the previous motion event (if any)
     */
    Int32 mLastY;

    /**
     * How far the finger moved before we started scrolling
     */
    Int32 mMotionCorrection;

    /**
     * The offset in pixels form the top of the AdapterView to the top
     * of the currently selected view. Used to save and restore state.
     */
    Int32 mSelectedTop;

    /**
     * Indicates whether the list is stacked from the bottom edge or
     * the top edge.
     */
    Boolean mStackFromBottom;

    /**
     * When set to TRUE, the list automatically discards the children's
     * bitmap cache after scrolling.
     */
    Boolean mScrollingCacheEnabled;

    /**
     * Whether or not to enable the fast scroll feature on this list
     */
    Boolean mFastScrollEnabled;

    /**
     * Keeps track of our accessory window
     */
    //PopupWindow mPopup;

    /**
     * Used with type filter window
     */
    //EditText mTextFilter;

    /**
     * The position to resurrect the selected position to.
     */
    Int32 mResurrectToPosition;

    /**
     * Maximum distance to record overscroll
     */
    Int32 mOverscrollMax;

    Boolean mIsScrap[1];

    /**
     * Maximum distance to overscroll by during edge effects
     */
    Int32 mOverscrollDistance;

    /**
     * Maximum distance to overfling during edge effects
     */
    Int32 mOverflingDistance;

private:
    /**
     * Optional callback to notify client when scroll position has changed
     */
    AutoPtr<IOnScrollListener> mOnScrollListener;

    /**
     * Determines speed during touch scrolling
     */
    AutoPtr<VelocityTracker> mVelocityTracker;

    /**
     * Handles one frame of a fling
     */
    AutoPtr<FlingRunnable> mFlingRunnable;

    /**
     * Handles scrolling between positions within the list.
     */
    AutoPtr<PositionScroller> mPositionScroller;

    /**
     * Indicates whether to use pixels-based or position-based scrollbar
     * properties.
     */
    Boolean mSmoothScrollbarEnabled;

    /**
     * Indicates that this view supports filtering
     */
    Boolean mTextFilterEnabled;

    /**
     * Indicates that this view is currently displaying a filtered view of the data
     */
    Boolean mFiltered;

    /**
     * Rectangle used for hit testing children
     */
    AutoPtr<IRect> mTouchFrame;

    AutoPtr<IContextMenuInfo> mContextMenuInfo;

    Int32 mLastTouchMode;
    Boolean mScrollProfilingStarted;
    Boolean mFlingProfilingStarted;

    /**
     * The last CheckForLongPress runnable we posted, if any
     */
    AutoPtr<CheckForLongPress> mPendingCheckForLongPress;

    /**
     * The last CheckForTap runnable we posted, if any
     */
    AutoPtr<IRunnable> mPendingCheckForTap;

    /**
     * The last CheckForKeyLongPress runnable we posted, if any
     */
    AutoPtr<CheckForKeyLongPress> mPendingCheckForKeyLongPress;

    /**
     * Acts upon click
     */
    AutoPtr<AbsListViewPerformClick> mPerformClick;

    /**
     * This view is in transcript mode -- it shows the bottom of the list when the data
     * changes
     */
    Int32 mTranscriptMode;

    /**
     * Indicates that this list is always drawn on top of a solid, single-color, opaque
     * background
     */
    Int32 mCacheColorHint;

    /**
     * The select child's view (from the adapter's getView) is enabled.
     */
    Boolean mIsChildViewEnabled;

    /**
     * The last scroll state reported to clients through {@link OnScrollListener}.
     */
    Int32 mLastScrollState;

    /**
     * Helper object that renders and controls the fast scroll thumb.
     */
    FastScroller* mFastScroller;

    Boolean mGlobalLayoutListenerAddedFilter;

    Int32 mTouchSlop;
    Float mDensityScale;

    //InputConnection mDefInputConnection;
    //InputConnectionWrapper mPublicInputConnection;

    AutoPtr<IRunnable> mClearScrollingCache;
    Int32 mMinimumVelocity;
    Int32 mMaximumVelocity;

    // True when the popup should be hidden because of a call to
    // dispatchDisplayHint()
    Boolean mPopupHidden;

    /**
     * ID of the active pointer. This is used to retain consistency during
     * drags/flings if multiple pointers are used.
     */
    Int32 mActivePointerId;

    // These two EdgeGlows are always set and used together.
    // Checking one for NULL is as good as checking both.

    /**
     * Tracks the state of the top edge glow.
     */
    EdgeGlow* mEdgeGlowTop;

    /**
     * Tracks the state of the bottom edge glow.
     */
    EdgeGlow* mEdgeGlowBottom;

    /**
     * An estimate of how many pixels are between the top of the list and
     * the top of the first position in the adapter, based on the last time
     * we saw it. Used to hint where to draw edge glows.
     */
    Int32 mFirstPositionDistanceGuess;

    /**
     * An estimate of how many pixels are between the bottom of the list and
     * the bottom of the last position in the adapter, based on the last time
     * we saw it. Used to hint where to draw edge glows.
     */
    Int32 mLastPositionDistanceGuess;

    /**
     * Used for determining when to cancel out of overscroll.
     */
    Int32 mDirection;

};
#endif //__ABSLISTVIEW_H__
