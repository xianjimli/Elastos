
#include "widget/GridView.h"
#include "view/animation/GridLayoutAnimationController.h"
#include "widget/CAbsListViewLayoutParams.h"
#include "view/CViewGroupLayoutParams.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

const Int32 GridView::NO_STRETCH;
const Int32 GridView::STRETCH_SPACING;
const Int32 GridView::STRETCH_COLUMN_WIDTH;
const Int32 GridView::STRETCH_SPACING_UNIFORM;

static const Int32 AUTO_FIT = -1;

static Int32 R_Styleable_GridView[] = {
    0x010100af, 0x01010114, 0x01010115, 0x01010116,
    0x01010117, 0x01010118
};

static const Int32 R_Attr_GridViewStyle=0x01010071;

static const Int32 R_Styleable_GridView_gravity = 0;
static const Int32 R_Styleable_GridView_horizontalSpacing = 1;
static const Int32 R_Styleable_GridView_verticalSpacing = 2;
static const Int32 R_Styleable_GridView_stretchMode = 3;
static const Int32 R_Styleable_GridView_columnWidth = 4;
static const Int32 R_Styleable_GridView_numColumns = 5;


GridView::GridView()
{

}

GridView::GridView(
    /* [in] */ IContext* context) : AbsListView(context)
{
}

GridView::GridView(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs) : AbsListView(context, attrs, R_Attr_GridViewStyle)
{
    Init(context, attrs, R_Attr_GridViewStyle);
}

GridView::GridView(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs, 
    /* [in] */ Int32 defStyle) : AbsListView(context, attrs, defStyle)
{
    Init(context, attrs, defStyle);
}

void GridView::Init(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs, 
    /* [in] */ Int32 defStyle)
{
    mNumColumns = AUTO_FIT;

    mHorizontalSpacing = 0;
    mVerticalSpacing = 0;
    mStretchMode = STRETCH_COLUMN_WIDTH;

    mGravity = Gravity_LEFT;

    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mTempRect));

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(attrs,
        ArrayOf<Int32>(R_Styleable_GridView, 6), defStyle, 0, (ITypedArray**)&a);

    Int32 hSpacing;
    a->GetDimensionPixelOffset(
        R_Styleable_GridView_horizontalSpacing, 0, &hSpacing);
    SetHorizontalSpacing(hSpacing);

    Int32 vSpacing;
    a->GetDimensionPixelOffset(
        R_Styleable_GridView_verticalSpacing, 0, &vSpacing);
    SetVerticalSpacing(vSpacing);

    Int32 index;
    a->GetInt32(R_Styleable_GridView_stretchMode, STRETCH_COLUMN_WIDTH, &index);
    if (index >= 0) {
        SetStretchMode(index);
    }

    Int32 columnWidth;
    a->GetDimensionPixelOffset(R_Styleable_GridView_columnWidth, -1, &columnWidth);
    if (columnWidth > 0) {
        SetColumnWidth(columnWidth);
    }

    Int32 numColumns;
    a->GetInt32(R_Styleable_GridView_numColumns, 1, &numColumns);
    SetNumColumns(numColumns);

    a->GetInt32(R_Styleable_GridView_gravity, -1, &index);
    if (index >= 0) {
        SetGravity(index);
    }

    a->Recycle();
}

AutoPtr<IAdapter> GridView::GetAdapter()
{
    return mAdapter;
}

/**
 * Sets the data behind this GridView.
 *
 * @param adapter the adapter providing the grid's data
 */
ECode GridView::SetAdapter(
    /* [in] */ IListAdapter* adapter) 
{
    if (NULL != mAdapter) {
        mAdapter->UnregisterDataSetObserver(mDataSetObserver);
    }

    ResetList();
    mRecycler->Clear();        
    mAdapter = adapter;

    mOldSelectedPosition = AdapterView_INVALID_POSITION;
    mOldSelectedRowId = AdapterView_INVALID_ROW_ID;
    
    if (mAdapter != NULL) {
        mOldItemCount = mItemCount;
        mAdapter->GetCount(&mItemCount);
        mDataChanged = TRUE;
        CheckFocus();

        mDataSetObserver = new AdapterDataSetObserver(this);
        mAdapter->RegisterDataSetObserver(mDataSetObserver);

        Int32 count;
        mAdapter->GetViewTypeCount(&count);
        mRecycler->SetViewTypeCount(count);

        Int32 position;
        if (mStackFromBottom) {
            position = LookForSelectablePosition(mItemCount - 1, FALSE);
        } else {
            position = LookForSelectablePosition(0, TRUE);
        }
        SetSelectedPositionInt(position);
        SetNextSelectedPositionInt(position);
        CheckSelectionChanged();
    } else {
        CheckFocus();            
        // Nothing selected
        CheckSelectionChanged();
    }

    RequestLayout();

    return NOERROR;
}

Int32 GridView::LookForSelectablePosition(
    /* [in] */ Int32 position, 
    /* [in] */ Boolean lookDown)
{
    AutoPtr<IListAdapter> adapter = mAdapter;
    if (adapter == NULL || IsInTouchMode()) {
        return AdapterView_INVALID_POSITION;
    }

    if (position < 0 || position >= mItemCount) {
        return AdapterView_INVALID_POSITION;
    }
    return position;
}

/**
 * {@inheritDoc}
 */
void GridView::FillGap(
    /* [in] */ Boolean down)
{
    Int32 numColumns = mNumColumns;
    Int32 verticalSpacing = mVerticalSpacing;

    Int32 count = GetChildCount();

    if (down) {
        Int32 bottom;
        GetChildAt(count - 1)->GetBottom(&bottom);
        Int32 startOffset = count > 0 ?
                bottom + verticalSpacing : GetListPaddingTop();
        Int32 position = mFirstPosition + count;
        if (mStackFromBottom) {
            position += numColumns - 1;
        }
        FillDown(position, startOffset);
        CorrectTooHigh(numColumns, verticalSpacing, GetChildCount());
    } else {
        Int32 top;
        GetChildAt(0)->GetTop(&top);

        Int32 startOffset = count > 0 ?
                top - verticalSpacing : GetHeight() - GetListPaddingBottom();
        Int32 position = mFirstPosition;
        if (!mStackFromBottom) {
            position -= numColumns;
        } else {
            position--;
        }
        FillUp(position, startOffset);
        CorrectTooLow(numColumns, verticalSpacing, GetChildCount());
    }
}

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
AutoPtr<IView> GridView::FillDown(
    /* [in] */ Int32 pos, 
    /* [in] */ Int32 nextTop)
{
    AutoPtr<IView> selectedView;

    Int32 end = (mBottom - mTop) - mListPadding->mBottom;

    while (nextTop < end && pos < mItemCount) {
        AutoPtr<IView> temp = MakeRow(pos, nextTop, TRUE);
        if (temp != NULL) {
            selectedView = temp;
        }

        // mReferenceView will change with each call to makeRow()
        // do not cache in a local variable outside of this loop
        mReferenceView->GetBottom(&nextTop);
        nextTop += mVerticalSpacing;

        pos += mNumColumns;
    }

    return selectedView;
}

AutoPtr<IView> GridView::MakeRow(
    /* [in] */ Int32 startPos, 
    /* [in] */ Int32 y, 
    /* [in] */ Boolean flow) 
{
    Int32 columnWidth = mColumnWidth;
    Int32 horizontalSpacing = mHorizontalSpacing;

    Int32 last;
    Int32 nextLeft = mListPadding->mLeft +
            ((mStretchMode == STRETCH_SPACING_UNIFORM) ? horizontalSpacing : 0);

    if (!mStackFromBottom) {
        last = Math::Min(startPos + mNumColumns, mItemCount);
    } else {
        last = startPos + 1;
        startPos = Math::Max(0, startPos - mNumColumns + 1);

        if (last - startPos < mNumColumns) {
            nextLeft += (mNumColumns - (last - startPos)) * (columnWidth + horizontalSpacing);
        }
    }

    AutoPtr<IView> selectedView = NULL;

    Boolean hasFocus = ShouldShowSelector();
    Boolean inClick = TouchModeDrawsInPressedState();
    Int32 selectedPosition = mSelectedPosition;

    AutoPtr<IView> child = NULL;
    for (Int32 pos = startPos; pos < last; pos++) {
        // is this the selected item?
        Boolean selected = pos == selectedPosition;
        // does the list view have focus or contain focus

        Int32 where = flow ? -1 : pos - startPos;
        child = MakeAndAddView(pos, y, flow, nextLeft, selected, where);

        nextLeft += columnWidth;
        if (pos < last - 1) {
            nextLeft += horizontalSpacing;
        }

        if (selected && (hasFocus || inClick)) {
            selectedView = child;
        }
    }

    mReferenceView = child;
    
    if (selectedView != NULL) {
        mReferenceViewInSelectedRow = mReferenceView;
    }

    return selectedView;
}

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
AutoPtr<IView> GridView::FillUp(
    /* [in] */ Int32 pos, 
    /* [in] */ Int32 nextBottom)
{
    AutoPtr<IView> selectedView = NULL;

    Int32 end = mListPadding->mTop;

    while (nextBottom > end && pos >= 0) {

        AutoPtr<IView> temp = MakeRow(pos, nextBottom, FALSE);
        if (temp != NULL) {
            selectedView = temp;
        }

        mReferenceView->GetTop(&nextBottom);
        nextBottom -= mVerticalSpacing;

        mFirstPosition = pos;

        pos -= mNumColumns;
    }

    if (mStackFromBottom) {
        mFirstPosition = Math::Max(0, pos + 1);
    }

    return selectedView;
}

/**
 * Fills the list from top to bottom, starting with mFirstPosition
 *
 * @param nextTop The location where the top of the first item should be
 *        drawn
 *
 * @return The view that is currently selected
 */
AutoPtr<IView> GridView::FillFromTop(
    /* [in] */ Int32 nextTop) 
{
    mFirstPosition = Math::Min(mFirstPosition, mSelectedPosition);
    mFirstPosition = Math::Min(mFirstPosition, mItemCount - 1);
    if (mFirstPosition < 0) {
        mFirstPosition = 0;
    }
    mFirstPosition -= mFirstPosition % mNumColumns;
    return FillDown(mFirstPosition, nextTop);
}

AutoPtr<IView> GridView::FillFromBottom(
    /* [in] */ Int32 lastPosition, 
    /* [in] */ Int32 nextBottom)
{
    lastPosition = Math::Max(lastPosition, mSelectedPosition);
    lastPosition = Math::Min(lastPosition, mItemCount - 1);

    Int32 invertedPosition = mItemCount - 1 - lastPosition;
    lastPosition = mItemCount - 1 - (invertedPosition - (invertedPosition % mNumColumns));

    return FillUp(lastPosition, nextBottom);
}

AutoPtr<IView> GridView::FillSelection(
    /* [in] */ Int32 childrenTop, 
    /* [in] */ Int32 childrenBottom) 
{
    Int32 selectedPosition = ReconcileSelectedPosition();
    Int32 numColumns = mNumColumns;
    Int32 verticalSpacing = mVerticalSpacing;

    Int32 rowStart;
    Int32 rowEnd = -1;

    if (!mStackFromBottom) {
        rowStart = selectedPosition - (selectedPosition % numColumns);
    } else {
        Int32 invertedSelection = mItemCount - 1 - selectedPosition;

        rowEnd = mItemCount - 1 - (invertedSelection - (invertedSelection % numColumns));
        rowStart = Math::Max(0, rowEnd - numColumns + 1);
    }

    Int32 fadingEdgeLength = GetVerticalFadingEdgeLength();
    Int32 topSelectionPixel = GetTopSelectionPixel(childrenTop, fadingEdgeLength, rowStart);

    AutoPtr<IView> sel = MakeRow(mStackFromBottom ? rowEnd : rowStart, topSelectionPixel, TRUE);
    mFirstPosition = rowStart;

    AutoPtr<IView> referenceView = mReferenceView;

    Int32 bottom, top;
    if (!mStackFromBottom) {
        referenceView->GetBottom(&bottom); 
        FillDown(rowStart + numColumns, bottom + verticalSpacing);
        PinToBottom(childrenBottom);
        
        referenceView->GetTop(&top);
        FillUp(rowStart - numColumns, top - verticalSpacing);
        AdjustViewsUpOrDown();
    } else {
        Int32 bottomSelectionPixel = GetBottomSelectionPixel(childrenBottom,
                fadingEdgeLength, numColumns, rowStart);
        referenceView->GetBottom(&bottom);
        Int32 offset = bottomSelectionPixel - bottom;
        OffsetChildrenTopAndBottom(offset);

        referenceView->GetTop(&top);
        FillUp(rowStart - 1, top - verticalSpacing);
        PinToTop(childrenTop);
        FillDown(rowEnd + numColumns, bottom + verticalSpacing);
        AdjustViewsUpOrDown();
    }

    return sel;
}

void GridView::PinToTop(
    /* [in] */ Int32 childrenTop)
{
    if (mFirstPosition == 0) {
        Int32 top;
        GetChildAt(0)->GetTop(&top);
        Int32 offset = childrenTop - top;
        if (offset < 0) {
            OffsetChildrenTopAndBottom(offset);
        }
    }
}

void GridView::PinToBottom(
    /* [in] */ Int32 childrenBottom)
{
    Int32 count = GetChildCount();
    if (mFirstPosition + count == mItemCount) {
        Int32 bottom;
        GetChildAt(count - 1)->GetBottom(&bottom);
        Int32 offset = childrenBottom - bottom;
        if (offset > 0) {
            OffsetChildrenTopAndBottom(offset);
        }
    }
}    

Int32 GridView::FindMotionRow(
    /* [in] */ Int32 y) 
{
    Int32 childCount = GetChildCount();
    if (childCount > 0) {

        Int32 numColumns = mNumColumns;
        if (!mStackFromBottom) {
            for (Int32 i = 0; i < childCount; i += numColumns) {
                Int32 bottom;
                GetChildAt(i)->GetBottom(&bottom);
                if (y <= bottom) {
                    return mFirstPosition + i;
                }
            }
        } else {
            for (Int32 i = childCount - 1; i >= 0; i -= numColumns) {
                Int32 top;
                GetChildAt(i)->GetTop(&top);
                if (y >= top) {
                    return mFirstPosition + i;
                }
            }
        }
    }
    return AdapterView_INVALID_POSITION;
}

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
AutoPtr<IView> GridView::FillSpecific(
    /* [in] */ Int32 position, 
    /* [in] */ Int32 top) 
{
    Int32 numColumns = mNumColumns;

    Int32 motionRowStart;
    Int32 motionRowEnd = -1;

    if (!mStackFromBottom) {
        motionRowStart = position - (position % numColumns);
    } else {
        Int32 invertedSelection = mItemCount - 1 - position;

        motionRowEnd = mItemCount - 1 - (invertedSelection - (invertedSelection % numColumns));
        motionRowStart = Math::Max(0, motionRowEnd - numColumns + 1);
    }

    AutoPtr<IView> temp = MakeRow(mStackFromBottom ? motionRowEnd : motionRowStart, top, TRUE);

    // Possibly changed again in fillUp if we add rows above this one.
    mFirstPosition = motionRowStart;

    AutoPtr<IView> referenceView = mReferenceView;
    // We didn't have anything to layout, bail out
    if (referenceView == NULL) {
        return NULL;
    }

    Int32 verticalSpacing = mVerticalSpacing;

    AutoPtr<IView> above;
    AutoPtr<IView> below;

    Int32 t, bottom;
    if (!mStackFromBottom) {

        referenceView->GetTop(&t); 
        referenceView->GetBottom(&bottom);

        above = FillUp(motionRowStart - numColumns, t - verticalSpacing);
        AdjustViewsUpOrDown();
        below = FillDown(motionRowStart + numColumns, bottom + verticalSpacing);
        // Check if we have dragged the bottom of the grid too high
        Int32 childCount = GetChildCount();
        if (childCount > 0) {
            CorrectTooHigh(numColumns, verticalSpacing, childCount);
        }
    } else {
        referenceView->GetBottom(&bottom);
        referenceView->GetTop(&t);

        below = FillDown(motionRowEnd + numColumns, bottom + verticalSpacing);
        AdjustViewsUpOrDown();
        above = FillUp(motionRowStart - 1, t - verticalSpacing);
        // Check if we have dragged the bottom of the grid too high
        Int32 childCount = GetChildCount();
        if (childCount > 0) {
            CorrectTooLow(numColumns, verticalSpacing, childCount);
        }
    }

    if (temp != NULL) {
        return temp;
    } else if (above != NULL) {
        return above;
    } else {
        return below;
    }
}

void GridView::CorrectTooHigh(
    /* [in] */ Int32 numColumns, 
    /* [in] */ Int32 verticalSpacing, 
    /* [in] */ Int32 childCount) 
{
    // First see if the last item is visible
    Int32 lastPosition = mFirstPosition + childCount - 1;
    if (lastPosition == mItemCount - 1 && childCount > 0) {
        // Get the last child ...
        AutoPtr<IView> lastChild = GetChildAt(childCount - 1);

        // ... and its bottom edge
        Int32 lastBottom;
        lastChild->GetBottom(&lastBottom);
        // This is bottom of our drawable area
        Int32 end = (mBottom - mTop) - mListPadding->mBottom;

        // This is how far the bottom edge of the last view is from the bottom of the
        // drawable area
        Int32 bottomOffset = end - lastBottom;        

        AutoPtr<IView> firstChild = GetChildAt(0);
        Int32 firstTop;
        firstChild->GetTop(&firstTop);

        // Make sure we are 1) Too high, and 2) Either there are more rows above the
        // first row or the first row is scrolled off the top of the drawable area
        if (bottomOffset > 0 && (mFirstPosition > 0 || firstTop < mListPadding->mTop))  {
            if (mFirstPosition == 0) {
                // Don't pull the top too far down
                bottomOffset = Math::Min(bottomOffset, mListPadding->mTop - firstTop);
            }
            
            // Move everything down
            OffsetChildrenTopAndBottom(bottomOffset);
            if (mFirstPosition > 0) {
                // Fill the gap that was opened above mFirstPosition with more rows, if
                // possible
                Int32 top;
                firstChild->GetTop(&top);
                FillUp(mFirstPosition - (mStackFromBottom ? 1 : numColumns),
                        top - verticalSpacing);
                // Close up the remaining gap
                AdjustViewsUpOrDown();
            }
        }
    }
}

void GridView::CorrectTooLow(
    /* [in] */ Int32 numColumns, 
    /* [in] */ Int32 verticalSpacing, 
    /* [in] */ Int32 childCount) 
{
    if (mFirstPosition == 0 && childCount > 0) {
        // Get the first child ...
        AutoPtr<IView> firstChild = GetChildAt(0);

        // ... and its top edge
        Int32 firstTop;
        firstChild->GetTop(&firstTop);

        // This is top of our drawable area
        Int32 start = mListPadding->mTop;

        // This is bottom of our drawable area
        Int32 end = (mBottom - mTop) - mListPadding->mBottom;

        // This is how far the top edge of the first view is from the top of the
        // drawable area
        Int32 topOffset = firstTop - start;
        AutoPtr<IView> lastChild = GetChildAt(childCount - 1);
        Int32 lastBottom;
        lastChild->GetBottom(&lastBottom);
        Int32 lastPosition = mFirstPosition + childCount - 1;

        // Make sure we are 1) Too low, and 2) Either there are more rows below the
        // last row or the last row is scrolled off the bottom of the drawable area
        if (topOffset > 0 && (lastPosition < mItemCount - 1 || lastBottom > end))  {
            if (lastPosition == mItemCount - 1 ) {
                // Don't pull the bottom too far up
                topOffset = Math::Min(topOffset, lastBottom - end);
            }
            
            // Move everything up
            OffsetChildrenTopAndBottom(-topOffset);
            if (lastPosition < mItemCount - 1) {
                // Fill the gap that was opened below the last position with more rows, if
                // possible
                Int32 bottom;
                lastChild->GetBottom(&bottom);
                FillDown(lastPosition + (!mStackFromBottom ? 1 : numColumns),
                       bottom + verticalSpacing);
                // Close up the remaining gap
                AdjustViewsUpOrDown();
            }
        }
    }
}

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
AutoPtr<IView> GridView::FillFromSelection(
    /* [in] */ Int32 selectedTop, 
    /* [in] */ Int32 childrenTop, 
    /* [in] */ Int32 childrenBottom) 
{
    Int32 fadingEdgeLength = GetVerticalFadingEdgeLength();
    Int32 selectedPosition = mSelectedPosition;
    Int32 numColumns = mNumColumns;
    Int32 verticalSpacing = mVerticalSpacing;

    Int32 rowStart;
    Int32 rowEnd = -1;

    if (!mStackFromBottom) {
        rowStart = selectedPosition - (selectedPosition % numColumns);
    } else {
        Int32 invertedSelection = mItemCount - 1 - selectedPosition;

        rowEnd = mItemCount - 1 - (invertedSelection - (invertedSelection % numColumns));
        rowStart = Math::Max(0, rowEnd - numColumns + 1);
    }

    AutoPtr<IView> sel;
    AutoPtr<IView> referenceView;

    Int32 topSelectionPixel = GetTopSelectionPixel(childrenTop, fadingEdgeLength, rowStart);
    Int32 bottomSelectionPixel = GetBottomSelectionPixel(childrenBottom, fadingEdgeLength,
            numColumns, rowStart);

    sel = MakeRow(mStackFromBottom ? rowEnd : rowStart, selectedTop, TRUE);
    // Possibly changed again in fillUp if we add rows above this one.
    mFirstPosition = rowStart;

    referenceView = mReferenceView;
    AdjustForTopFadingEdge(referenceView, topSelectionPixel, bottomSelectionPixel);
    AdjustForBottomFadingEdge(referenceView, topSelectionPixel, bottomSelectionPixel);

    Int32 top, bottom;
    referenceView->GetTop(&top);
    referenceView->GetBottom(&bottom);

    if (!mStackFromBottom) {
        FillUp(rowStart - numColumns, top - verticalSpacing);
        AdjustViewsUpOrDown();
        FillDown(rowStart + numColumns, bottom + verticalSpacing);
    } else {
        FillDown(rowEnd + numColumns, bottom + verticalSpacing);
        AdjustViewsUpOrDown();
        FillUp(rowStart - 1, top - verticalSpacing);
    }


    return sel;
}

/**
 * Calculate the bottom-most pixel we can draw the selection into
 *
 * @param childrenBottom Bottom pixel were children can be drawn
 * @param fadingEdgeLength Length of the fading edge in pixels, if present
 * @param numColumns Number of columns in the grid
 * @param rowStart The start of the row that will contain the selection
 * @return The bottom-most pixel we can draw the selection into
 */
Int32 GridView::GetBottomSelectionPixel(
    /* [in] */ Int32 childrenBottom, 
    /* [in] */ Int32 fadingEdgeLength,
    /* [in] */ Int32 numColumns, 
    /* [in] */ Int32 rowStart)
{
    // Last pixel we can draw the selection into
    Int32 bottomSelectionPixel = childrenBottom;
    if (rowStart + numColumns - 1 < mItemCount - 1) {
        bottomSelectionPixel -= fadingEdgeLength;
    }
    return bottomSelectionPixel;
}

/**
 * Calculate the top-most pixel we can draw the selection into
 *
 * @param childrenTop Top pixel were children can be drawn
 * @param fadingEdgeLength Length of the fading edge in pixels, if present
 * @param rowStart The start of the row that will contain the selection
 * @return The top-most pixel we can draw the selection into
 */
Int32 GridView::GetTopSelectionPixel(
    /* [in] */ Int32 childrenTop, 
    /* [in] */ Int32 fadingEdgeLength, 
    /* [in] */ Int32 rowStart)
{
    // first pixel we can draw the selection into
    Int32 topSelectionPixel = childrenTop;
    if (rowStart > 0) {
        topSelectionPixel += fadingEdgeLength;
    }
    return topSelectionPixel;
}

/**
 * Move all views upwards so the selected row does not interesect the bottom
 * fading edge (if necessary).
 *
 * @param childInSelectedRow A child in the row that contains the selection
 * @param topSelectionPixel The topmost pixel we can draw the selection into
 * @param bottomSelectionPixel The bottommost pixel we can draw the
 *        selection into
 */
void GridView::AdjustForBottomFadingEdge(
    /* [in] */ IView* childInSelectedRow,
    /* [in] */ Int32 topSelectionPixel, 
    /* [in] */ Int32 bottomSelectionPixel) 
{
    // Some of the newly selected item extends below the bottom of the
    // list
    Int32 bottom, top;
    childInSelectedRow->GetBottom(&bottom);
    childInSelectedRow->GetTop(&top);
    if (bottom > bottomSelectionPixel) {

        // Find space available above the selection into which we can
        // scroll upwards
        Int32 spaceAbove = top - topSelectionPixel;

        // Find space required to bring the bottom of the selected item
        // fully into view
        Int32 spaceBelow = bottom - bottomSelectionPixel;
        Int32 offset = Math::Min(spaceAbove, spaceBelow);

        // Now offset the selected item to get it into view
        OffsetChildrenTopAndBottom(-offset);
    }
}

/**
 * Move all views upwards so the selected row does not interesect the top
 * fading edge (if necessary).
 *
 * @param childInSelectedRow A child in the row that contains the selection
 * @param topSelectionPixel The topmost pixel we can draw the selection into
 * @param bottomSelectionPixel The bottommost pixel we can draw the
 *        selection into
 */
void GridView::AdjustForTopFadingEdge(
    /* [in] */ IView* childInSelectedRow,
    /* [in] */ Int32 topSelectionPixel, 
    /* [in] */ Int32 bottomSelectionPixel)
{
    Int32 top, bottom;
    childInSelectedRow->GetTop(&top);
    childInSelectedRow->GetBottom(&bottom);
    // Some of the newly selected item extends above the top of the list
    if (top < topSelectionPixel) {
        // Find space required to bring the top of the selected item
        // fully into view
        Int32 spaceAbove = topSelectionPixel - top;

        // Find space available below the selection into which we can
        // scroll downwards
        Int32 spaceBelow = bottomSelectionPixel - bottom;
        Int32 offset = Math::Min(spaceAbove, spaceBelow);

        // Now offset the selected item to get it into view
        OffsetChildrenTopAndBottom(offset);
    }
}

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
AutoPtr<IView> GridView::MoveSelection(
    /* [in] */ Int32 delta, 
    /* [in] */ Int32 childrenTop, 
    /* [in] */ Int32 childrenBottom) 
{
    Int32 fadingEdgeLength = GetVerticalFadingEdgeLength();
    Int32 selectedPosition = mSelectedPosition;
    Int32 numColumns = mNumColumns;
    Int32 verticalSpacing = mVerticalSpacing;

    Int32 oldRowStart;
    Int32 rowStart;
    Int32 rowEnd = -1;

    if (!mStackFromBottom) {
        oldRowStart = (selectedPosition - delta) - ((selectedPosition - delta) % numColumns);

        rowStart = selectedPosition - (selectedPosition % numColumns);
    } else {
        Int32 invertedSelection = mItemCount - 1 - selectedPosition;

        rowEnd = mItemCount - 1 - (invertedSelection - (invertedSelection % numColumns));
        rowStart = Math::Max(0, rowEnd - numColumns + 1);

        invertedSelection = mItemCount - 1 - (selectedPosition - delta);
        oldRowStart = mItemCount - 1 - (invertedSelection - (invertedSelection % numColumns));
        oldRowStart = Math::Max(0, oldRowStart - numColumns + 1);
    }

    Int32 rowDelta = rowStart - oldRowStart;

    Int32 topSelectionPixel = GetTopSelectionPixel(childrenTop, fadingEdgeLength, rowStart);
    Int32 bottomSelectionPixel = GetBottomSelectionPixel(childrenBottom, fadingEdgeLength,
            numColumns, rowStart);

    // Possibly changed again in fillUp if we add rows above this one.
    mFirstPosition = rowStart;

    AutoPtr<IView> sel;
    AutoPtr<IView> referenceView;

    Int32 bottom, top;
    mReferenceViewInSelectedRow->GetBottom(&bottom);
    mReferenceViewInSelectedRow ->GetTop(&top);

    if (rowDelta > 0) {
        /*
         * Case 1: Scrolling down.
         */

        Int32 oldBottom = mReferenceViewInSelectedRow == NULL ? 0 :
                bottom;

        sel = MakeRow(mStackFromBottom ? rowEnd : rowStart, oldBottom + verticalSpacing, TRUE);
        referenceView = mReferenceView;

        AdjustForBottomFadingEdge(referenceView, topSelectionPixel, bottomSelectionPixel);
    } else if (rowDelta < 0) {
        /*
         * Case 2: Scrolling up.
         */
        Int32 oldTop = mReferenceViewInSelectedRow == NULL ? 0 : top;

        sel = MakeRow(mStackFromBottom ? rowEnd : rowStart, oldTop - verticalSpacing, FALSE);
        referenceView = mReferenceView;

        AdjustForTopFadingEdge(referenceView, topSelectionPixel, bottomSelectionPixel);
    } else {
        /*
         * Keep selection where it was
         */
        mReferenceViewInSelectedRow->GetTop(&top);
        Int32 oldTop = mReferenceViewInSelectedRow == NULL ? 0 : top;

        sel = MakeRow(mStackFromBottom ? rowEnd : rowStart, oldTop, TRUE);
        referenceView = mReferenceView;
    }

    referenceView->GetTop(&top);
    referenceView->GetBottom(&bottom);

    if (!mStackFromBottom) {
        FillUp(rowStart - numColumns, top - verticalSpacing);
        AdjustViewsUpOrDown();
        FillDown(rowStart + numColumns, bottom + verticalSpacing);
    } else {
        FillDown(rowEnd + numColumns, bottom + verticalSpacing);
        AdjustViewsUpOrDown();
        FillUp(rowStart - 1, top - verticalSpacing);
    }

    return sel;
}

void GridView::DetermineColumns(
    /* [in] */ Int32 availableSpace)
{
    Int32 requestedHorizontalSpacing = mRequestedHorizontalSpacing;
    Int32 stretchMode = mStretchMode;
    Int32 requestedColumnWidth = mRequestedColumnWidth;
    
    if (mRequestedNumColumns == AUTO_FIT) {
        if (requestedColumnWidth > 0) {
            // Client told us to pick the number of columns
            mNumColumns = (availableSpace + requestedHorizontalSpacing) /
                    (requestedColumnWidth + requestedHorizontalSpacing);
        } else {
            // Just make up a number if we don't have enough info
            mNumColumns = 2;
        }
    } else {
        // We picked the columns
        mNumColumns = mRequestedNumColumns;
    }
    
    if (mNumColumns <= 0) {
        mNumColumns = 1;
    }

    switch (stretchMode) {
    case NO_STRETCH:
        // Nobody stretches
        mColumnWidth = requestedColumnWidth;
        mHorizontalSpacing = requestedHorizontalSpacing;
        break;

    default:
        Int32 spaceLeftOver = availableSpace - (mNumColumns * requestedColumnWidth) -
                ((mNumColumns - 1) * requestedHorizontalSpacing);
        switch (stretchMode) {
        case STRETCH_COLUMN_WIDTH:
            // Stretch the columns
            mColumnWidth = requestedColumnWidth + spaceLeftOver / mNumColumns;
            mHorizontalSpacing = requestedHorizontalSpacing;
            break;

        case STRETCH_SPACING:
            // Stretch the spacing between columns
            mColumnWidth = requestedColumnWidth;
            if (mNumColumns > 1) {
                mHorizontalSpacing = requestedHorizontalSpacing + 
                    spaceLeftOver / (mNumColumns - 1);
            } else {
                mHorizontalSpacing = requestedHorizontalSpacing + spaceLeftOver;
            }
            break;

        case STRETCH_SPACING_UNIFORM:
            // Stretch the spacing between columns
            mColumnWidth = requestedColumnWidth;
            if (mNumColumns > 1) {
                mHorizontalSpacing = requestedHorizontalSpacing + 
                    spaceLeftOver / (mNumColumns + 1);
            } else {
                mHorizontalSpacing = requestedHorizontalSpacing + spaceLeftOver;
            }
            break;
        }

        break;
    }
}

void GridView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec, 
    /* [in] */ Int32 heightMeasureSpec)
{
    // Sets up mListPadding
    AbsListView::OnMeasure(widthMeasureSpec, heightMeasureSpec);

    Int32 widthMode = MeasureSpec::GetMode(widthMeasureSpec);
    Int32 heightMode = MeasureSpec::GetMode(heightMeasureSpec);
    Int32 widthSize = MeasureSpec::GetSize(widthMeasureSpec);
    Int32 heightSize = MeasureSpec::GetSize(heightMeasureSpec);

    if (widthMode == MeasureSpec::UNSPECIFIED) {
        if (mColumnWidth > 0) {
            widthSize = mColumnWidth + mListPadding->mLeft + mListPadding->mRight;
        } else {
            widthSize = mListPadding->mLeft + mListPadding->mRight;
        }
        widthSize += GetVerticalScrollbarWidth();
    }
    
    Int32 childWidth = widthSize - mListPadding->mLeft - mListPadding->mRight;
    DetermineColumns(childWidth);

    Int32 childHeight = 0;
    Int32 count;
    mAdapter->GetCount(&count);
    mItemCount = mAdapter == NULL ? 0 : count;
    count = mItemCount;
    if (count > 0) {
        AutoPtr<IView> child = ObtainView(0, mIsScrap);

        AutoPtr<IAbsListViewLayoutParams> p;
        child->GetLayoutParams((IViewGroupLayoutParams**)&p);
        if (p == NULL) {
            CAbsListViewLayoutParams::New(ViewGroupLayoutParams_MATCH_PARENT,
                    ViewGroupLayoutParams_WRAP_CONTENT, 0, (IAbsListViewLayoutParams**)&p);
            child->SetLayoutParams(p);
        }
        mAdapter->GetItemViewType(0, &((CAbsListViewLayoutParams*)p.Get())->mViewType);
        ((CAbsListViewLayoutParams*)p.Get())->mForceAdd = TRUE;

        Int32 childHeightSpec = GetChildMeasureSpec(
            MeasureSpec::MakeMeasureSpec(0, MeasureSpec::UNSPECIFIED), 0, ((CAbsListViewLayoutParams*)p.Get())->mHeight);
        Int32 childWidthSpec = GetChildMeasureSpec(
            MeasureSpec::MakeMeasureSpec(mColumnWidth, MeasureSpec::EXACTLY), 0, ((CAbsListViewLayoutParams*)p.Get())->mWidth);
        child->Measure(childWidthSpec, childHeightSpec);

        child->GetMeasuredHeight(&childHeight);

        if (mRecycler->ShouldRecycleViewType(((CAbsListViewLayoutParams*)p.Get())->mViewType)) {
            mRecycler->AddScrapView(child);
        }
    }
    
    if (heightMode == MeasureSpec::UNSPECIFIED) {
        heightSize = mListPadding->mTop + mListPadding->mBottom + childHeight +
                GetVerticalFadingEdgeLength() * 2;
    }

    if (heightMode == MeasureSpec::AT_MOST) {
        Int32 ourSize =  mListPadding->mTop + mListPadding->mBottom;
       
        Int32 numColumns = mNumColumns;
        for (Int32 i = 0; i < count; i += numColumns) {
            ourSize += childHeight;
            if (i + numColumns < count) {
                ourSize += mVerticalSpacing;
            }
            if (ourSize >= heightSize) {
                ourSize = heightSize;
                break;
            }
        }
        heightSize = ourSize;
    }

    SetMeasuredDimension(widthSize, heightSize);
    mWidthMeasureSpec = widthMeasureSpec;
}

ECode GridView::AttachLayoutAnimationParameters(
    /* [in] */ IView* child,
    /* [in] */ IViewGroupLayoutParams* params, 
    /* [in] */ Int32 index, 
    /* [in] */ Int32 count)
{

    GridLayoutAnimationController::AnimationParameters* animationParams =
        (GridLayoutAnimationController::AnimationParameters*) (((CViewGroupLayoutParams*)params)->mLayoutAnimationParameters.Get());

    if (animationParams == NULL) {
        animationParams = new GridLayoutAnimationController::AnimationParameters();
        ((CViewGroupLayoutParams*)params)->mLayoutAnimationParameters = animationParams;
    }

    animationParams->mCount = count;
    animationParams->mIndex = index;
    animationParams->mColumnsCount = mNumColumns;
    animationParams->mRowsCount = count / mNumColumns;

    if (!mStackFromBottom) {
        animationParams->mColumn = index % mNumColumns;
        animationParams->mRow = index / mNumColumns;
    } else {
        Int32 invertedIndex = count - 1 - index;

        animationParams->mColumn = mNumColumns - 1 - (invertedIndex % mNumColumns);
        animationParams->mRow = animationParams->mRowsCount - 1 - invertedIndex / mNumColumns;
    }

    return NOERROR;
}

void GridView::LayoutChildren()
{
    Boolean blockLayoutRequests = mBlockLayoutRequests;
    if (!blockLayoutRequests) {
        mBlockLayoutRequests = TRUE;
    }

    //try {
        AbsListView::LayoutChildren();

        Invalidate();

        if (mAdapter == NULL) {
            ResetList();
            InvokeOnItemScrollListener();
            return;
        }

        Int32 childrenTop = mListPadding->mTop;
        Int32 childrenBottom = mBottom - mTop - mListPadding->mBottom;

        Int32 childCount = GetChildCount();
        Int32 index;
        Int32 delta = 0;

        AutoPtr<IView> sel;
        AutoPtr<IView> oldSel = NULL;
        AutoPtr<IView> oldFirst = NULL;
        AutoPtr<IView> newSel = NULL;

        // Remember stuff we will need down below
        switch (mLayoutMode) {
        case LAYOUT_SET_SELECTION:
            index = mNextSelectedPosition - mFirstPosition;
            if (index >= 0 && index < childCount) {
                newSel = GetChildAt(index);
            }
            break;
        case LAYOUT_FORCE_TOP:
        case LAYOUT_FORCE_BOTTOM:
        case LAYOUT_SPECIFIC:
        case LAYOUT_SYNC:
            break;
        case LAYOUT_MOVE_SELECTION:
            if (mNextSelectedPosition >= 0) {
                delta = mNextSelectedPosition - mSelectedPosition;
            }
            break;
        default:
            // Remember the previously selected view
            index = mSelectedPosition - mFirstPosition;
            if (index >= 0 && index < childCount) {
                oldSel = GetChildAt(index);
            }

            // Remember the previous first child
            oldFirst = GetChildAt(0);
        }

        Boolean dataChanged = mDataChanged;
        if (dataChanged) {
            HandleDataChanged();
        }

        // Handle the empty set by removing all views that are visible
        // and calling it a day
        if (mItemCount == 0) {
            ResetList();
            InvokeOnItemScrollListener();
            return;
        }

        SetSelectedPositionInt(mNextSelectedPosition);

        // Pull all children into the RecycleBin.
        // These views will be reused if possible
        Int32 firstPosition = mFirstPosition;
        RecycleBin* recycleBin = mRecycler;

        if (dataChanged) {
            for (Int32 i = 0; i < childCount; i++) {
                recycleBin->AddScrapView(GetChildAt(i));
            }
        } else {
            recycleBin->FillActiveViews(childCount, firstPosition);
        }

        // Clear out old views
        //removeAllViewsInLayout();
        DetachAllViewsFromParent();

        Int32 top;
        newSel->GetTop(&top);
        switch (mLayoutMode) {
        case LAYOUT_SET_SELECTION:
            if (newSel != NULL) {
                sel = FillFromSelection(top, childrenTop, childrenBottom);
            } else {
                sel = FillSelection(childrenTop, childrenBottom);
            }
            break;
        case LAYOUT_FORCE_TOP:
            mFirstPosition = 0;
            sel = FillFromTop(childrenTop);
            AdjustViewsUpOrDown();
            break;
        case LAYOUT_FORCE_BOTTOM:
            sel = FillUp(mItemCount - 1, childrenBottom);
            AdjustViewsUpOrDown();
            break;
        case LAYOUT_SPECIFIC:
            sel = FillSpecific(mSelectedPosition, mSpecificTop);
            break;
        case LAYOUT_SYNC:
            sel = FillSpecific(mSyncPosition, mSpecificTop);
            break;
        case LAYOUT_MOVE_SELECTION:
            // Move the selection relative to its old position
            sel = MoveSelection(delta, childrenTop, childrenBottom);
            break;
        default:
            if (childCount == 0) {
                if (!mStackFromBottom) {
                    SetSelectedPositionInt(mAdapter == NULL || IsInTouchMode() ?
                            AdapterView_INVALID_POSITION : 0);
                    sel = FillFromTop(childrenTop);
                } else {
                    Int32 last = mItemCount - 1;
                    SetSelectedPositionInt(mAdapter == NULL || IsInTouchMode() ?
                            AdapterView_INVALID_POSITION : last);
                    sel = FillFromBottom(last, childrenBottom);
                }
            } else {
                oldSel->GetTop(&top);
                if (mSelectedPosition >= 0 && mSelectedPosition < mItemCount) {
                    sel = FillSpecific(mSelectedPosition, oldSel == NULL ?
                            childrenTop : top);
                } else if (mFirstPosition < mItemCount)  {
                    sel = FillSpecific(mFirstPosition, oldFirst == NULL ?
                            childrenTop : top);
                } else {
                    sel = FillSpecific(0, childrenTop);
                }
            }
            break;
        }

        // Flush any cached views that did not get reused above
        recycleBin->ScrapActiveViews();

        if (sel != NULL) {
           PositionSelector(sel);
           sel->GetTop(&mSelectedTop);
        } else if (mTouchMode > TOUCH_MODE_DOWN && mTouchMode < TOUCH_MODE_SCROLL) {
            AutoPtr<IView> child = GetChildAt(mMotionPosition - mFirstPosition);
            if (child != NULL) PositionSelector(child);                
        } else {
            mSelectedTop = 0;
            mSelectorRect->SetEmpty();
        }

        mLayoutMode = LAYOUT_NORMAL;
        mDataChanged = FALSE;
        mNeedSync = FALSE;
        SetNextSelectedPositionInt(mSelectedPosition);

        UpdateScrollIndicators();

        if (mItemCount > 0) {
            CheckSelectionChanged();
        }

        InvokeOnItemScrollListener();
    /*} finally {
        if (!blockLayoutRequests) {
            mBlockLayoutRequests = FALSE;
        }
    }*/
}


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
AutoPtr<IView> GridView::MakeAndAddView(
    /* [in] */ Int32 position, 
    /* [in] */ Int32 y, 
    /* [in] */ Boolean flow, 
    /* [in] */ Int32 childrenLeft,
    /* [in] */ Boolean selected, 
    /* [in] */ Int32 where)
{
    AutoPtr<IView> child;

    if (!mDataChanged) {
        // Try to use an existing view for this position
        child = mRecycler->GetActiveView(position);
        if (child != NULL) {
            // Found it -- we're using an existing child
            // This just needs to be positioned
            SetupChild(child, position, y, flow, childrenLeft, selected, TRUE, where);
            return child;
        }
    }

    // Make a new view for this position, or convert an unused view if
    // possible
    child = ObtainView(position, mIsScrap);

    // This needs to be positioned and measured
    SetupChild(child, position, y, flow, childrenLeft, selected, mIsScrap[0], where);

    return child;
}

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
void GridView::SetupChild(
    /* [in] */ IView* child, 
    /* [in] */ Int32 position, 
    /* [in] */ Int32 y, 
    /* [in] */ Boolean flow, 
    /* [in] */ Int32 childrenLeft,
    /* [in] */ Boolean selected, 
    /* [in] */ Boolean recycled, 
    /* [in] */ Int32 where)
{
    Boolean isSelected = selected && ShouldShowSelector();

    Boolean sel;
    child->IsSelected(&sel);

    Boolean updateChildSelected = isSelected != sel;
    Int32 mode = mTouchMode;
    Boolean isPressed = mode > TOUCH_MODE_DOWN && mode < TOUCH_MODE_SCROLL &&
            mMotionPosition == position;

    Boolean press;
    child->IsPressed(&press);
    Boolean updateChildPressed = isPressed != press;
    
    Boolean layoutRequested;
    child->IsLayoutRequested(&layoutRequested);
    Boolean needToMeasure = !recycled || updateChildSelected || layoutRequested;

    // Respect layout params that are already in the view. Otherwise make
    // some up...
    AutoPtr<IAbsListViewLayoutParams> p;
    child->GetLayoutParams((IViewGroupLayoutParams**)&p);
    if (p == NULL) {
        CAbsListViewLayoutParams::New(ViewGroupLayoutParams_MATCH_PARENT,
                ViewGroupLayoutParams_WRAP_CONTENT, 0, (IAbsListViewLayoutParams**)&p);
    }
    mAdapter->GetItemViewType(position, &((CAbsListViewLayoutParams*)p.Get())->mViewType);

    if (recycled && !((CAbsListViewLayoutParams*)p.Get())->mForceAdd) {
        AttachViewToParent(child, where, p);
    } else {
        ((CAbsListViewLayoutParams*)p.Get())->mForceAdd = FALSE;
        AddViewInLayout(child, where, p, TRUE);
    }

    if (updateChildSelected) {
        child->SetSelected(isSelected);
        if (isSelected) {
            View::RequestFocus();
        }
    }

    if (updateChildPressed) {
        child->SetPressed(isPressed);
    }

    if (needToMeasure) {
        Int32 childHeightSpec = ViewGroup::GetChildMeasureSpec(
            MeasureSpec::MakeMeasureSpec(0, MeasureSpec::UNSPECIFIED), 0, ((CAbsListViewLayoutParams*)p.Get())->mHeight);

        Int32 childWidthSpec = ViewGroup::GetChildMeasureSpec(
            MeasureSpec::MakeMeasureSpec(mColumnWidth, MeasureSpec::EXACTLY), 0, ((CAbsListViewLayoutParams*)p.Get())->mWidth);
        child->Measure(childWidthSpec, childHeightSpec);
    } else {
        CleanupLayoutState(child);
    }

    Int32 w;
    child->GetMeasuredWidth(&w);
    Int32 h;
    child->GetMeasuredHeight(&h);

    Int32 childLeft;
    Int32 childTop = flow ? y : y - h;

    switch (mGravity & Gravity_HORIZONTAL_GRAVITY_MASK) {
    case Gravity_LEFT:
        childLeft = childrenLeft;
        break;
    case Gravity_CENTER_HORIZONTAL:
        childLeft = childrenLeft + ((mColumnWidth - w) / 2);
        break;
    case Gravity_RIGHT:
        childLeft = childrenLeft + mColumnWidth - w;
        break;
    default:
        childLeft = childrenLeft;
        break;
    }

    if (needToMeasure) {
        Int32 childRight = childLeft + w;
        Int32 childBottom = childTop + h;
        child->Layout(childLeft, childTop, childRight, childBottom);
    } else {
        Int32 left, top;
        child->GetLeft(&left);
        child->GetTop(&top);
        child->OffsetLeftAndRight(childLeft - left);
        child->OffsetTopAndBottom(childTop - top);
    }

    if (mCachingStarted) {
        child->SetDrawingCacheEnabled(TRUE);
    }
}

/**
 * Sets the currently selected item
 * 
 * @param position Index (starting at 0) of the data item to be selected.
 * 
 * If in touch mode, the item will not be selected but it will still be positioned
 * appropriately.
 */
ECode GridView::SetSelection(
    /* [in] */ Int32 position) 
{
    if (!IsInTouchMode()) {
        SetNextSelectedPositionInt(position);
    } else {
        mResurrectToPosition = position;
    }
    mLayoutMode = LAYOUT_SET_SELECTION;
    RequestLayout();

    return NOERROR;
}

/**
 * Makes the item at the supplied position selected.
 *
 * @param position the position of the new selection
 */
void GridView::SetSelectionInt(
    /* [in] */ Int32 position)
{
    Int32 previousSelectedPosition = mNextSelectedPosition;

    SetNextSelectedPositionInt(position);
    LayoutChildren();
    
    Int32 next = mStackFromBottom ? mItemCount - 1  - mNextSelectedPosition : 
        mNextSelectedPosition;
    Int32 previous = mStackFromBottom ? mItemCount - 1
            - previousSelectedPosition : previousSelectedPosition;

    Int32 nextRow = next / mNumColumns;
    Int32 previousRow = previous / mNumColumns;

    if (nextRow != previousRow) {
        AwakenScrollBars();
    }

}

Boolean GridView::OnKeyDown(
    /* [in] */ Int32 keyCode, 
    /* [in] */ IKeyEvent* event)
{
    return CommonKey(keyCode, 1, event);
}

Boolean GridView::OnKeyMultiple(
    /* [in] */ Int32 keyCode, 
    /* [in] */ Int32 repeatCount, 
    /* [in] */ IKeyEvent* event) 
{
    return CommonKey(keyCode, repeatCount, event);
}

Boolean GridView::OnKeyUp(
    /* [in] */ Int32 keyCode, 
    /* [in] */ IKeyEvent* event) 
{
    return CommonKey(keyCode, 1, event);
}

Boolean GridView::CommonKey(
    /* [in] */ Int32 keyCode, 
    /* [in] */ Int32 count, 
    /* [in] */ IKeyEvent* event)
{
    if (mAdapter == NULL) {
        return FALSE;
    }

    if (mDataChanged) {
        LayoutChildren();
    }

    Boolean handled = FALSE;
    Int32 action;
    event->GetAction(&action);

    if (action != KeyEvent_ACTION_UP) {
        if (mSelectedPosition < 0) {
            switch (keyCode) {
                case KeyEvent_KEYCODE_DPAD_UP:
                case KeyEvent_KEYCODE_DPAD_DOWN:
                case KeyEvent_KEYCODE_DPAD_LEFT:
                case KeyEvent_KEYCODE_DPAD_RIGHT:
                case KeyEvent_KEYCODE_DPAD_CENTER:
                case KeyEvent_KEYCODE_SPACE:
                case KeyEvent_KEYCODE_ENTER:
                    ResurrectSelection();
                    return TRUE;
            }
        }

        Boolean press;
        switch (keyCode) {
            case KeyEvent_KEYCODE_DPAD_LEFT:
                handled = ArrowScroll(FOCUS_LEFT);
                break;

            case KeyEvent_KEYCODE_DPAD_RIGHT:
                handled = ArrowScroll(FOCUS_RIGHT);
                break;

            case KeyEvent_KEYCODE_DPAD_UP:
                
                event->IsAltPressed(&press);
                if (!press) {
                    handled = ArrowScroll(FOCUS_UP);

                } else {
                    handled = FullScroll(FOCUS_UP);
                }
                break;

            case KeyEvent_KEYCODE_DPAD_DOWN:
                event->IsAltPressed(&press);
                if (!press) {
                    handled = ArrowScroll(FOCUS_DOWN);
                } else {
                    handled = FullScroll(FOCUS_DOWN);
                }
                break;

            case KeyEvent_KEYCODE_DPAD_CENTER:
            case KeyEvent_KEYCODE_ENTER: {
                Int32 count;
                event->GetRepeatCount(&count);
                if (GetChildCount() > 0 && count == 0) {
                    KeyPressed();
                }

                return TRUE;
            }

            case KeyEvent_KEYCODE_SPACE:
                Boolean show;
                mPopup->IsShowing(&show);
                if (mPopup == NULL || !show) {
                    event->IsShiftPressed(&press);
                    if (!press) {
                        handled = PageScroll(FOCUS_DOWN);
                    } else {
                        handled = PageScroll(FOCUS_UP);
                    }
                }
                break;
        }
    }

    if (!handled) {
        handled = SendToTextFilter(keyCode, count, event);
    }

    if (handled) {
        return TRUE;
    } else {
        switch (action) {
            case KeyEvent_ACTION_DOWN:
                return AbsListView::OnKeyDown(keyCode, event);
            case KeyEvent_ACTION_UP:
                return AbsListView::OnKeyUp(keyCode, event);
            case KeyEvent_ACTION_MULTIPLE:
                return AbsListView::OnKeyMultiple(keyCode, count, event);
            default:
                return FALSE;
        }
    }
}

/**
 * Scrolls up or down by the number of items currently present on screen.
 *
 * @param direction either {@link View#FOCUS_UP} or {@link View#FOCUS_DOWN}
 * @return whether selection was moved
 */
Boolean GridView::PageScroll(
    /* [in] */ Int32 direction) 
{
    Int32 nextPage = -1;

    if (direction == FOCUS_UP) {
        nextPage = Math::Max(0, mSelectedPosition - GetChildCount() - 1);
    } else if (direction == FOCUS_DOWN) {
        nextPage = Math::Min(mItemCount - 1, mSelectedPosition + GetChildCount() - 1);
    }

    if (nextPage >= 0) {
        SetSelectionInt(nextPage);
        InvokeOnItemScrollListener();
        AwakenScrollBars();
        return TRUE;
    }

    return FALSE;
}

/**
 * Go to the last or first item if possible.
 *
 * @param direction either {@link View#FOCUS_UP} or {@link View#FOCUS_DOWN}.
 *
 * @return Whether selection was moved.
 */
Boolean GridView::FullScroll(
    /* [in] */ Int32 direction)
{
    Boolean moved = FALSE;
    if (direction == FOCUS_UP) {
        mLayoutMode = LAYOUT_SET_SELECTION;
        SetSelectionInt(0);
        InvokeOnItemScrollListener();
        moved = TRUE;
    } else if (direction == FOCUS_DOWN) {
        mLayoutMode = LAYOUT_SET_SELECTION;
        SetSelectionInt(mItemCount - 1);
        InvokeOnItemScrollListener();
        moved = TRUE;
    }
    
    if (moved) {
        AwakenScrollBars();
    }

    return moved;
}

/**
 * Scrolls to the next or previous item, horizontally or vertically.
 *
 * @param direction either {@link View#FOCUS_LEFT}, {@link View#FOCUS_RIGHT},
 *        {@link View#FOCUS_UP} or {@link View#FOCUS_DOWN}
 *
 * @return whether selection was moved
 */
Boolean GridView::ArrowScroll(
    /* [in] */ Int32 direction)
{
    Int32 selectedPosition = mSelectedPosition;
    Int32 numColumns = mNumColumns;

    Int32 startOfRowPos;
    Int32 endOfRowPos;

    Boolean moved = FALSE;

    if (!mStackFromBottom) {
        startOfRowPos = (selectedPosition / numColumns) * numColumns;
        endOfRowPos = Math::Min(startOfRowPos + numColumns - 1, mItemCount - 1);
    } else {
        Int32 invertedSelection = mItemCount - 1 - selectedPosition;
        endOfRowPos = mItemCount - 1 - (invertedSelection / numColumns) * numColumns;
        startOfRowPos = Math::Max(0, endOfRowPos - numColumns + 1);
    }

    switch (direction) {
        case FOCUS_UP:
            if (startOfRowPos > 0) {
                mLayoutMode = LAYOUT_MOVE_SELECTION;
                SetSelectionInt(Math::Max(0, selectedPosition - numColumns));
                moved = TRUE;
            }
            break;
        case FOCUS_DOWN:
            if (endOfRowPos < mItemCount - 1) {
                mLayoutMode = LAYOUT_MOVE_SELECTION;
                SetSelectionInt(Math::Min(selectedPosition + numColumns, mItemCount - 1));
                moved = TRUE;
            }
            break;
        case FOCUS_LEFT:
            if (selectedPosition > startOfRowPos) {
                mLayoutMode = LAYOUT_MOVE_SELECTION;
                SetSelectionInt(Math::Max(0, selectedPosition - 1));
                moved = TRUE;
            }
            break;
        case FOCUS_RIGHT:
            if (selectedPosition < endOfRowPos) {
                mLayoutMode = LAYOUT_MOVE_SELECTION;
                SetSelectionInt(Math::Min(selectedPosition + 1, mItemCount - 1));
                moved = TRUE;
            }
            break;
    }

    if (moved) {
        //PlaySoundEffect(SoundEffectConstants.getContantForFocusDirection(direction));
        InvokeOnItemScrollListener();
    }

    if (moved) {
        AwakenScrollBars();
    }
    
    return moved;
}

void GridView::OnFocusChanged(
    /* [in] */ Boolean gainFocus, 
    /* [in] */ Int32 direction, 
    /* [in] */ IRect* previouslyFocusedRect)
{
    AbsListView::OnFocusChanged(gainFocus, direction, previouslyFocusedRect);

    Int32 closestChildIndex = -1;
    if (gainFocus && previouslyFocusedRect != NULL) {
        previouslyFocusedRect->Offset(mScrollX, mScrollY);

        // figure out which item should be selected based on previously
        // focused rect
        AutoPtr<CRect> otherRect = mTempRect;
        Int32 minDistance = Math::INT32_MAX_VALUE;
        Int32 childCount = GetChildCount();
        for (Int32 i = 0; i < childCount; i++) {
            // only consider view's on appropriate edge of grid
            if (!IsCandidateSelection(i, direction)) {
                continue;
            }

            AutoPtr<IView> other = GetChildAt(i);
            other->GetDrawingRect(otherRect);
            OffsetDescendantRectToMyCoords(other, otherRect);
            Int32 distance = GetDistance(previouslyFocusedRect, otherRect, direction);

            if (distance < minDistance) {
                minDistance = distance;
                closestChildIndex = i;
            }
        }
    }

    if (closestChildIndex >= 0) {
        SetSelection(closestChildIndex + mFirstPosition);
    } else {
        RequestLayout();
    }
}

/**
 * Is childIndex a candidate for next focus given the direction the focus
 * change is coming from?
 * @param childIndex The index to check.
 * @param direction The direction, one of
 *        {FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}
 * @return Whether childIndex is a candidate.
 */
Boolean GridView::IsCandidateSelection(
    /* [in] */ Int32 childIndex, 
    /* [in] */ Int32 direction) 
{
    Int32 count = GetChildCount();
    Int32 invertedIndex = count - 1 - childIndex;

    Int32 rowStart;
    Int32 rowEnd;

    if (!mStackFromBottom) {
        rowStart = childIndex - (childIndex % mNumColumns);
        rowEnd = Math::Max(rowStart + mNumColumns - 1, count);
    } else {
        rowEnd = count - 1 - (invertedIndex - (invertedIndex % mNumColumns));
        rowStart = Math::Max(0, rowEnd - mNumColumns + 1);
    }

    switch (direction) {
        case View::FOCUS_RIGHT:
            // coming from left, selection is only valid if it is on left
            // edge
            return childIndex == rowStart;
        case View::FOCUS_DOWN:
            // coming from top; only valid if in top row
            return rowStart == 0;
        case View::FOCUS_LEFT:
            // coming from right, must be on right edge
            return childIndex == rowEnd;
        case View::FOCUS_UP:
            // coming from bottom, need to be in last row
            return rowEnd == count - 1;
        /*default:
            throw new IllegalArgumentException("direction must be one of "
                    + "{FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}.");*/
    }
}

/**
 * Describes how the child views are horizontally aligned. Defaults to Gravity.LEFT
 *
 * @param gravity the gravity to apply to this grid's children
 *
 * @attr ref android.R.styleable#GridView_gravity
 */
ECode GridView::SetGravity(
    /* [in] */ Int32 gravity) 
{
    if (mGravity != gravity) {
        mGravity = gravity;
        RequestLayoutIfNecessary();
    }

    return NOERROR;
}

/**
 * Set the amount of horizontal (x) spacing to place between each item
 * in the grid.
 *
 * @param horizontalSpacing The amount of horizontal space between items,
 * in pixels.
 *
 * @attr ref android.R.styleable#GridView_horizontalSpacing
 */
ECode GridView::SetHorizontalSpacing(
    /* [in] */ Int32 horizontalSpacing)
{
    if (horizontalSpacing != mRequestedHorizontalSpacing) {
        mRequestedHorizontalSpacing = horizontalSpacing;
        RequestLayoutIfNecessary();
    }

    return NOERROR;
}


/**
 * Set the amount of vertical (y) spacing to place between each item
 * in the grid.
 *
 * @param verticalSpacing The amount of vertical space between items,
 * in pixels.
 *
 * @attr ref android.R.styleable#GridView_verticalSpacing
 */
ECode GridView::SetVerticalSpacing(
    /* [in] */ Int32 verticalSpacing) 
{
    if (verticalSpacing != mVerticalSpacing) {
        mVerticalSpacing = verticalSpacing;
        RequestLayoutIfNecessary();
    }

    return NOERROR;
}

/**
 * Control how items are stretched to fill their space.
 *
 * @param stretchMode Either {@link #NO_STRETCH},
 * {@link #STRETCH_SPACING}, {@link #STRETCH_SPACING_UNIFORM}, or {@link #STRETCH_COLUMN_WIDTH}.
 *
 * @attr ref android.R.styleable#GridView_stretchMode
 */
ECode GridView::SetStretchMode(
    /* [in] */ Int32 stretchMode) 
{
    if (stretchMode != mStretchMode) {
        mStretchMode = stretchMode;
        RequestLayoutIfNecessary();
    }

    return NOERROR;
}

Int32 GridView::GetStretchMode()
{
    return mStretchMode;
}

/**
 * Set the width of columns in the grid.
 *
 * @param columnWidth The column width, in pixels.
 *
 * @attr ref android.R.styleable#GridView_columnWidth
 */
ECode GridView::SetColumnWidth(
    /* [in] */ Int32 columnWidth)
{
    if (columnWidth != mRequestedColumnWidth) {
        mRequestedColumnWidth = columnWidth;
        RequestLayoutIfNecessary();
    }

    return NOERROR;
}

/**
 * Set the number of columns in the grid
 *
 * @param numColumns The desired number of columns.
 *
 * @attr ref android.R.styleable#GridView_numColumns
 */
ECode GridView::SetNumColumns(Int32 numColumns) 
{
    if (numColumns != mRequestedNumColumns) {
        mRequestedNumColumns = numColumns;
        RequestLayoutIfNecessary();
    }

    return NOERROR;
}

/**
 * Make sure views are touching the top or bottom edge, as appropriate for
 * our gravity
 */
ECode GridView::AdjustViewsUpOrDown()
{
    Int32 childCount = GetChildCount();

    if (childCount > 0) {
        Int32 delta;
        AutoPtr<IView> child;

        Int32 top, bottom;
        child->GetTop(&top);
        child->GetBottom(&bottom);

        if (!mStackFromBottom) {
            // Uh-oh -- we came up short. Slide all views up to make them
            // align with the top
            child = GetChildAt(0);
            delta = top - mListPadding->mTop;
            if (mFirstPosition != 0) {
                // It's OK to have some space above the first item if it is
                // part of the vertical spacing
                delta -= mVerticalSpacing;
            }
            if (delta < 0) {
                // We only are looking to see if we are too low, not too high
                delta = 0;
            }
        } else {
            // we are too high, slide all views down to align with bottom
            child = GetChildAt(childCount - 1);
            delta = bottom - (GetHeight() - mListPadding->mBottom);
            
            if (mFirstPosition + childCount < mItemCount) {
                // It's OK to have some space below the last item if it is
                // part of the vertical spacing
                delta += mVerticalSpacing;
            }
            
            if (delta > 0) {
                // We only are looking to see if we are too high, not too low
                delta = 0;
            }
        }

        if (delta != 0) {
            OffsetChildrenTopAndBottom(-delta);
        }
    }

    return NOERROR;
}

Int32 GridView::ComputeVerticalScrollExtent() 
{
    Int32 count = GetChildCount();
    if (count > 0) {
        Int32 numColumns = mNumColumns;
        Int32 rowCount = (count + numColumns - 1) / numColumns;
        
        Int32 extent = rowCount * 100;

        AutoPtr<IView> view = GetChildAt(0);
        Int32 top;
        view->GetTop(&top);
        Int32 height;
        view->GetHeight(&height);
        if (height > 0) {
            extent += (top * 100) / height;
        }

        view = GetChildAt(count - 1);
        Int32 bottom;
        view->GetBottom(&bottom);
        view->GetHeight(&height);
        if (height > 0) {
            extent -= ((bottom - GetHeight()) * 100) / height;
        }

        return extent;
    }
    return 0;
}

Int32 GridView::ComputeVerticalScrollOffset()
{
    if (mFirstPosition >= 0 && GetChildCount() > 0) {
        AutoPtr<IView> view = GetChildAt(0);
        Int32 top;
        view->GetTop(&top);
        Int32 height;
        view->GetHeight(&height);
        if (height > 0) {
            Int32 numColumns = mNumColumns;
            Int32 whichRow = mFirstPosition / numColumns;
            Int32 rowCount = (mItemCount + numColumns - 1) / numColumns;
            return Math::Max(whichRow * 100 - (top * 100) / height +
                    (Int32) ((Float) mScrollY / GetHeight() * rowCount * 100), 0);
        }
    }
    return 0;
}

Int32 GridView::ComputeVerticalScrollRange() 
{
    // TODO: Account for vertical spacing too
    Int32 numColumns = mNumColumns;
    Int32 rowCount = (mItemCount + numColumns - 1) / numColumns;
    Int32 result = Math::Max(rowCount * 100, 0);
    if (mScrollY != 0) {
        // Compensate for overscroll
        result += Math::Abs((Int32) ((Float) mScrollY / GetHeight() * rowCount * 100));
    }
    return result;
}

