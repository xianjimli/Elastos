
#ifndef __GRIDVIEW_H__
#define __GRIDVIEW_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include "widget/AbsListView.h"

/**
 * A view that shows items in two-dimensional scrolling grid. The items in the
 * grid come from the {@link ListAdapter} associated with this view.
 *
 * <p>See the <a href="{@docRoot}resources/tutorials/views/hello-gridview.html">Grid
 * View tutorial</a>.</p>
 */
class GridView : public AbsListView 
{
public:
    GridView();

    GridView(
        /* [in] */ IContext* context);

    GridView(
        /* [in] */ IContext* context, 
        /* [in] */ IAttributeSet* attrs);

    GridView(
        /* [in] */ IContext* context, 
        /* [in] */ IAttributeSet* attrs, 
        /* [in] */ Int32 defStyle);

    virtual CARAPI_(AutoPtr<IAdapter>) GetAdapter();

    /**
     * Sets the data behind this GridView.
     *
     * @param adapter the adapter providing the grid's data
     */
    virtual CARAPI SetAdapter(
        /* [in] */ IListAdapter* adapter);

    virtual CARAPI_(Int32) LookForSelectablePosition(
        /* [in] */ Int32 position, 
        /* [in] */ Boolean lookDown);

    /**
     * {@inheritDoc}
     */
    virtual CARAPI_(void) FillGap(
        /* [in] */ Boolean down);

    virtual CARAPI_(Int32) FindMotionRow(
        /* [in] */ Int32 y);

private:
    CARAPI_(void) Init(
        /* [in] */ IContext* context, 
        /* [in] */ IAttributeSet* attrs, 
        /* [in] */ Int32 defStyle);
    /**
     * Fills the list from pos down to the end of the list view.
     *
     * @param pos The first position to put in the list
     *
     * @param nextTop The location where the top of the item associated with pos
     *        should be drawn
     *
     * @return The view that is currently selected, if it happens to be in the
     *         range that we draw.
     */
    virtual CARAPI_(AutoPtr<IView>) FillDown(
        /* [in] */ Int32 pos, 
        /* [in] */ Int32 nextTop);

    virtual CARAPI_(AutoPtr<IView>) MakeRow(
        /* [in] */ Int32 startPos, 
        /* [in] */ Int32 y, 
        /* [in] */ Boolean flow);

    /**
     * Fills the list from pos up to the top of the list view.
     *
     * @param pos The first position to put in the list
     *
     * @param nextBottom The location where the bottom of the item associated
     *        with pos should be drawn
     *
     * @return The view that is currently selected
     */
    virtual CARAPI_(AutoPtr<IView>) FillUp(
        /* [in] */ Int32 pos, 
        /* [in] */ Int32 nextBottom);

    /**
     * Fills the list from top to bottom, starting with mFirstPosition
     *
     * @param nextTop The location where the top of the first item should be
     *        drawn
     *
     * @return The view that is currently selected
     */
    virtual CARAPI_(AutoPtr<IView>) FillFromTop(
        /* [in] */ Int32 nextTop);

    virtual CARAPI_(AutoPtr<IView>) FillFromBottom(
        /* [in] */ Int32 lastPosition, 
        /* [in] */ Int32 nextBottom);

    virtual CARAPI_(AutoPtr<IView>) FillSelection(
        /* [in] */ Int32 childrenTop, 
        /* [in] */ Int32 childrenBottom);

    virtual CARAPI_(void) PinToTop(
        /* [in] */ Int32 childrenTop);

    virtual CARAPI_(void) PinToBottom(
        /* [in] */ Int32 childrenBottom);

    /**
     * Layout during a scroll that results from tracking motion events. Places
     * the mMotionPosition view at the offset specified by mMotionViewTop, and
     * then build surrounding views from there.
     *
     * @param position the position at which to start filling
     * @param top the top of the view at that position
     * @return The selected view, or NULL if the selected view is outside the
     *         visible area.
     */
    virtual CARAPI_(AutoPtr<IView>) FillSpecific(
        /* [in] */ Int32 position, 
        /* [in] */ Int32 top);

    virtual CARAPI_(void) CorrectTooHigh(
        /* [in] */ Int32 numColumns, 
        /* [in] */ Int32 verticalSpacing, 
        /* [in] */ Int32 childCount);

    virtual CARAPI_(void) CorrectTooLow(
        /* [in] */ Int32 numColumns, 
        /* [in] */ Int32 verticalSpacing, 
        /* [in] */ Int32 childCount);

    /**
     * Fills the grid based on positioning the new selection at a specific
     * location. The selection may be moved so that it does not intersect the
     * faded edges. The grid is then filled upwards and downwards from there.
     *
     * @param selectedTop Where the selected item should be
     * @param childrenTop Where to start drawing children
     * @param childrenBottom Last pixel where children can be drawn
     * @return The view that currently has selection
     */
    virtual CARAPI_(AutoPtr<IView>) FillFromSelection(
        /* [in] */ Int32 selectedTop, 
        /* [in] */ Int32 childrenTop, 
        /* [in] */ Int32 childrenBottom);

    /**
     * Calculate the bottom-most pixel we can draw the selection into
     *
     * @param childrenBottom Bottom pixel were children can be drawn
     * @param fadingEdgeLength Length of the fading edge in pixels, if present
     * @param numColumns Number of columns in the grid
     * @param rowStart The start of the row that will contain the selection
     * @return The bottom-most pixel we can draw the selection into
     */
    virtual CARAPI_(Int32) GetBottomSelectionPixel(
        /* [in] */ Int32 childrenBottom, 
        /* [in] */ Int32 fadingEdgeLength,
        /* [in] */ Int32 numColumns, 
        /* [in] */ Int32 rowStart);

    /**
     * Calculate the top-most pixel we can draw the selection into
     *
     * @param childrenTop Top pixel were children can be drawn
     * @param fadingEdgeLength Length of the fading edge in pixels, if present
     * @param rowStart The start of the row that will contain the selection
     * @return The top-most pixel we can draw the selection into
     */
    virtual CARAPI_(Int32) GetTopSelectionPixel(
        /* [in] */ Int32 childrenTop, 
        /* [in] */ Int32 fadingEdgeLength, 
        /* [in] */ Int32 rowStart);

    /**
     * Move all views upwards so the selected row does not interesect the bottom
     * fading edge (if necessary).
     *
     * @param childInSelectedRow A child in the row that contains the selection
     * @param topSelectionPixel The topmost pixel we can draw the selection into
     * @param bottomSelectionPixel The bottommost pixel we can draw the
     *        selection into
     */
    virtual CARAPI_(void) AdjustForBottomFadingEdge(
        /* [in] */ IView* childInSelectedRow,
        /* [in] */ Int32 topSelectionPixel, 
        /* [in] */ Int32 bottomSelectionPixel);

    /**
     * Move all views upwards so the selected row does not interesect the top
     * fading edge (if necessary).
     *
     * @param childInSelectedRow A child in the row that contains the selection
     * @param topSelectionPixel The topmost pixel we can draw the selection into
     * @param bottomSelectionPixel The bottommost pixel we can draw the
     *        selection into
     */
    virtual CARAPI_(void) AdjustForTopFadingEdge(
        /* [in] */ IView* childInSelectedRow,
        /* [in] */ Int32 topSelectionPixel, 
        /* [in] */ Int32 bottomSelectionPixel);

    /**
     * Fills the grid based on positioning the new selection relative to the old
     * selection. The new selection will be placed at, above, or below the
     * location of the new selection depending on how the selection is moving.
     * The selection will then be pinned to the visible part of the screen,
     * excluding the edges that are faded. The grid is then filled upwards and
     * downwards from there.
     *
     * @param delta Which way we are moving
     * @param childrenTop Where to start drawing children
     * @param childrenBottom Last pixel where children can be drawn
     * @return The view that currently has selection
     */
    virtual CARAPI_(AutoPtr<IView>) MoveSelection(
        /* [in] */ Int32 delta, 
        /* [in] */ Int32 childrenTop, 
        /* [in] */ Int32 childrenBottom);

    virtual CARAPI_(void) DetermineColumns(
        /* [in] */ Int32 availableSpace);

protected:
    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec, 
        /* [in] */ Int32 heightMeasureSpec);

    virtual CARAPI AttachLayoutAnimationParameters(
        /* [in] */ IView* child,
        /* [in] */ IViewGroupLayoutParams* params, 
        /* [in] */ Int32 index, 
        /* [in] */ Int32 count);

    virtual CARAPI_(void) LayoutChildren();

private:
    /**
     * Obtain the view and add it to our list of children. The view can be made
     * fresh, converted from an unused view, or used as is if it was in the
     * recycle bin.
     *
     * @param position Logical position in the list
     * @param y Top or bottom edge of the view to add
     * @param flow if TRUE, align top edge to y. If FALSE, align bottom edge to
     *        y.
     * @param childrenLeft Left edge where children should be positioned
     * @param selected Is this position selected?
     * @param where to add new item in the list
     * @return View that was added
     */
    virtual CARAPI_(AutoPtr<IView>) MakeAndAddView(
        /* [in] */ Int32 position, 
        /* [in] */ Int32 y, 
        /* [in] */ Boolean flow, 
        /* [in] */ Int32 childrenLeft,
        /* [in] */ Boolean selected, 
        /* [in] */ Int32 where);

    /**
     * Add a view as a child and make sure it is measured (if necessary) and
     * positioned properly.
     *
     * @param child The view to add
     * @param position The position of the view
     * @param y The y position relative to which this view will be positioned
     * @param flow if TRUE, align top edge to y. If FALSE, align bottom edge
     *        to y.
     * @param childrenLeft Left edge where children should be positioned
     * @param selected Is this position selected?
     * @param recycled Has this view been pulled from the recycle bin? If so it
     *        does not need to be remeasured.
     * @param where Where to add the item in the list
     *
     */
    virtual CARAPI_(void) SetupChild(
        /* [in] */ IView* child, 
        /* [in] */ Int32 position, 
        /* [in] */ Int32 y, 
        /* [in] */ Boolean flow, 
        /* [in] */ Int32 childrenLeft,
        /* [in] */ Boolean selected, 
        /* [in] */ Boolean recycled, 
        /* [in] */ Int32 where);

public:
    /**
     * Sets the currently selected item
     * 
     * @param position Index (starting at 0) of the data item to be selected.
     * 
     * If in touch mode, the item will not be selected but it will still be positioned
     * appropriately.
     */
    virtual CARAPI SetSelection(
        /* [in] */ Int32 position);

    /**
     * Makes the item at the supplied position selected.
     *
     * @param position the position of the new selection
     */
    virtual CARAPI_(void) SetSelectionInt(
        /* [in] */ Int32 position);

    virtual CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode, 
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyMultiple(
        /* [in] */ Int32 keyCode, 
        /* [in] */ Int32 repeatCount, 
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode, 
        /* [in] */ IKeyEvent* event);

private:
    virtual CARAPI_(Boolean) CommonKey(
        /* [in] */ Int32 keyCode, 
        /* [in] */ Int32 count, 
        /* [in] */ IKeyEvent* event);

public:
    /**
     * Scrolls up or down by the number of items currently present on screen.
     *
     * @param direction either {@link View#FOCUS_UP} or {@link View#FOCUS_DOWN}
     * @return whether selection was moved
     */
    virtual CARAPI_(Boolean) PageScroll(
        /* [in] */ Int32 direction);

    /**
     * Go to the last or first item if possible.
     *
     * @param direction either {@link View#FOCUS_UP} or {@link View#FOCUS_DOWN}.
     *
     * @return Whether selection was moved.
     */
    virtual CARAPI_(Boolean) FullScroll(
        /* [in] */ Int32 direction);

    /**
     * Scrolls to the next or previous item, horizontally or vertically.
     *
     * @param direction either {@link View#FOCUS_LEFT}, {@link View#FOCUS_RIGHT},
     *        {@link View#FOCUS_UP} or {@link View#FOCUS_DOWN}
     *
     * @return whether selection was moved
     */
    virtual CARAPI_(Boolean) ArrowScroll(
        /* [in] */ Int32 direction);

protected:
    virtual CARAPI_(void) OnFocusChanged(
        /* [in] */ Boolean gainFocus, 
        /* [in] */ Int32 direction, 
        /* [in] */ IRect* previouslyFocusedRect);

private:
    /**
     * Is childIndex a candidate for next focus given the direction the focus
     * change is coming from?
     * @param childIndex The index to check.
     * @param direction The direction, one of
     *        {FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}
     * @return Whether childIndex is a candidate.
     */
    virtual CARAPI_(Boolean) IsCandidateSelection(
        /* [in] */ Int32 childIndex, 
        /* [in] */ Int32 direction);

public:
    /**
     * Describes how the child views are horizontally aligned. Defaults to Gravity.LEFT
     *
     * @param gravity the gravity to apply to this grid's children
     *
     * @attr ref android.R.styleable#GridView_gravity
     */
    virtual CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    /**
     * Set the amount of horizontal (x) spacing to place between each item
     * in the grid.
     *
     * @param horizontalSpacing The amount of horizontal space between items,
     * in pixels.
     *
     * @attr ref android.R.styleable#GridView_horizontalSpacing
     */
    virtual CARAPI SetHorizontalSpacing(
        /* [in] */ Int32 horizontalSpacing);

    /**
     * Set the amount of vertical (y) spacing to place between each item
     * in the grid.
     *
     * @param verticalSpacing The amount of vertical space between items,
     * in pixels.
     *
     * @attr ref android.R.styleable#GridView_verticalSpacing
     */
    virtual CARAPI SetVerticalSpacing(
        /* [in] */ Int32 verticalSpacing);

    /**
     * Control how items are stretched to fill their space.
     *
     * @param stretchMode Either {@link #NO_STRETCH},
     * {@link #STRETCH_SPACING}, {@link #STRETCH_SPACING_UNIFORM}, or {@link #STRETCH_COLUMN_WIDTH}.
     *
     * @attr ref android.R.styleable#GridView_stretchMode
     */
    virtual CARAPI SetStretchMode(
        /* [in] */ Int32 stretchMode);

    virtual CARAPI_(Int32) GetStretchMode();

    /**
     * Set the width of columns in the grid.
     *
     * @param columnWidth The column width, in pixels.
     *
     * @attr ref android.R.styleable#GridView_columnWidth
     */
    virtual CARAPI SetColumnWidth(
        /* [in] */ Int32 columnWidth);

    /**
     * Set the number of columns in the grid
     *
     * @param numColumns The desired number of columns.
     *
     * @attr ref android.R.styleable#GridView_numColumns
     */
    virtual CARAPI SetNumColumns(
        /* [in] */ Int32 numColumns);

private:
    /**
     * Make sure views are touching the top or bottom edge, as appropriate for
     * our gravity
     */
    virtual CARAPI AdjustViewsUpOrDown();
    
protected:
    virtual CARAPI_(Int32) ComputeVerticalScrollExtent();
    
    virtual CARAPI_(Int32) ComputeVerticalScrollOffset();
    
    virtual CARAPI_(Int32) ComputeVerticalScrollRange();

public:
    static const Int32 NO_STRETCH = 0;
    static const Int32 STRETCH_SPACING = 1;
    static const Int32 STRETCH_COLUMN_WIDTH = 2;
    static const Int32 STRETCH_SPACING_UNIFORM = 3;

    static const Int32 AUTO_FIT = -1;

private:
    Int32 mNumColumns;

    Int32 mHorizontalSpacing;
    Int32 mRequestedHorizontalSpacing;
    Int32 mVerticalSpacing;
    Int32 mStretchMode;
    Int32 mColumnWidth;
    Int32 mRequestedColumnWidth;
    Int32 mRequestedNumColumns;

    AutoPtr<IView> mReferenceView;
    AutoPtr<IView> mReferenceViewInSelectedRow;

    Int32 mGravity;

    AutoPtr<CRect> mTempRect;
};

#endif
