
#include "widget/ListView.h"
#include "graphics/CPaint.h"
#include "graphics/ElPixelFormat.h"
#include "view/CKeyEvent.h"
#include "view/FocusFinder.h"
#include "view/CViewGroupLayoutParams.h"
#include "widget/CAbsListViewLayoutParams.h"
#include "widget/CListViewSavedState.h"
#include "widget/CHeaderViewListAdapter.h"
#include "utils/CParcelableObjectContainer.h"
#include "utils/AutoStringArray.h"
#include <elastos/Math.h>
#include <Logger.h>
#include <StringBuffer.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

const Int32 ListView::CHOICE_MODE_NONE;
const Int32 ListView::CHOICE_MODE_SINGLE;
const Int32 ListView::CHOICE_MODE_MULTIPLE;
const Int32 ListView::NO_POSITION;
const float ListView::MAX_SCROLL_FACTOR;
const Int32 ListView::MIN_SCROLL_PREVIEW_PIXELS;

static Int32 R_Styleable_ListView[] = {
    0x010100b2, 0x01010129, 0x0101012a, 0x0101012b,
    0x0101022e, 0x0101022f, 0x010102c2, 0x010102c3
};

PInterface ListView::FixedViewInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IFixedViewInfo*)this;
    }
    else if (riid == EIID_IFixedViewInfo) {
        return (IFixedViewInfo*)this;
    }
    return NULL;
}

UInt32 ListView::FixedViewInfo::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ListView::FixedViewInfo::Release()
{
    return ElRefBase::Release();
}

ECode ListView::FixedViewInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IFixedViewInfo*)this) {
        *pIID = EIID_IFixedViewInfo;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ListView::FocusSelector::FocusSelector(
    /* [in] */ ListView* host)
    : mHost(host)
{}

AutoPtr<ListView::FocusSelector> ListView::FocusSelector::Setup(
    /* [in] */ Int32 position,
    /* [in] */ Int32 top)
{
    mPosition = position;
    mPositionTop = top;
    return this;
}

ECode ListView::FocusSelector::Run()
{
    return mHost->SetSelectionFromTop(mPosition, mPositionTop);
}

/**
 * How {@link android.widGet.ListView#arrowScrollFocused} returns its values.
 */
void ListView::ArrowScrollFocusResult::Populate(
    /* [in] */ Int32 selectedPosition,
    /* [in] */ Int32 amountToScroll)
{
    mSelectedPosition = selectedPosition;
    mAmountToScroll = amountToScroll;
}

Int32 ListView::ArrowScrollFocusResult::GetSelectedPosition()
{
    return mSelectedPosition;
}

Int32 ListView::ArrowScrollFocusResult::GetAmountToScroll()
{
    return mAmountToScroll;
}

ListView::ListView()
    : mIsCacheColorOpaque(FALSE)
    , mDividerIsOpaque(FALSE)
    , mHeaderDividersEnabled(FALSE)
    , mFooterDividersEnabled(FALSE)
    , mAreAllItemsSelectable(TRUE)
    , mItemsCanFocus(FALSE)
    , mChoiceMode(CHOICE_MODE_NONE)
    , mCheckStates(NULL)
    , mCheckedIdStates(NULL)
    , mArrowScrollFocusResult(NULL)
{}

ListView::ListView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs/* = NULL*/,
    /* [in] */ Int32 defStyle/* = 0x01010074*//*com.android.internal.R.attr.listViewStyle*/)
    : mIsCacheColorOpaque(FALSE)
    , mDividerIsOpaque(FALSE)
    , mHeaderDividersEnabled(FALSE)
    , mFooterDividersEnabled(FALSE)
    , mAreAllItemsSelectable(TRUE)
    , mItemsCanFocus(FALSE)
    , mChoiceMode(CHOICE_MODE_NONE)
    , mCheckStates(NULL)
    , mCheckedIdStates(NULL)
    , mArrowScrollFocusResult(NULL)
{
    Init(context, attrs, defStyle);
}

ListView::~ListView()
{
    delete mCheckStates;
    delete mCheckedIdStates;
    delete mArrowScrollFocusResult;
}
/**
 * @return The maximum amount a list view will scroll in response to
 *   an arrow event.
 */
Int32 ListView::GetMaxScrollAmount()
{
    return (Int32)(MAX_SCROLL_FACTOR * (mBottom - mTop));
}

/**
 * Make sure views are touching the top or bottom edge, as appropriate for
 * our gravity
 */
void ListView::AdjustViewsUpOrDown()
{
    Int32 childCount = GetChildCount();
    Int32 delta;

    if (childCount > 0) {
        AutoPtr<IView> child;

        if (!mStackFromBottom) {
            // Uh-oh -- we came up short. Slide all views up to make them
            // align with the top
            child = GetChildAt(0);
            delta = ((View*)child->Probe(EIID_View))->GetTop() - mListPadding->mTop;
            if (mFirstPosition != 0) {
                // It's OK to have some space above the first item if it is
                // part of the vertical spacing
                delta -= mDividerHeight;
            }

            if (delta < 0) {
                // We only are looking to see if we are too low, not too high
                delta = 0;
            }
        }
        else {
            // we are too high, slide all views down to align with bottom
            child = GetChildAt(childCount - 1);
            delta = ((View*)child->Probe(EIID_View))->GetBottom()
                - (GetHeight() - mListPadding->mBottom);

            if (mFirstPosition + childCount < mItemCount) {
                // It's OK to have some space below the last item if it is
                // part of the vertical spacing
                delta += mDividerHeight;
            }

            if (delta > 0) {
                delta = 0;
            }
        }

        if (delta != 0) {
            OffsetChildrenTopAndBottom(-delta);
        }
    }
}

/**
 * Add a fixed view to appear at the top of the list. If addHeaderView is
 * called more than once, the views will appear in the order they were
 * added. Views added using this call can take focus if they want.
 * <p>
 * NOTE: Call this before calling setAdapter. This is so ListView can wrap
 * the supplied cursor with one that will also account for header and footer
 * views.
 *
 * @param v The view to add.
 * @param data Data to associate with this view
 * @param isSelectable whether the item is selectable
 */
ECode ListView::AddHeaderView(
    /* [in] */ IView* v,
    /* [in] */ IInterface* data,
    /* [in] */ Boolean isSelectable)
{
    if (mAdapter != NULL) {
        Logger::E("ListView",
            "Cannot add header view to list -- setAdapter has already been called.");

        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr<FixedViewInfo> info = new FixedViewInfo();
    info->mView = v;
    info->mData = data;
    info->mIsSelectable = isSelectable;
    mHeaderViewInfos.PushBack(info);

    return NOERROR;
}

/**
 * Add a fixed view to appear at the top of the list. If addHeaderView is
 * called more than once, the views will appear in the order they were
 * added. Views added using this call can take focus if they want.
 * <p>
 * NOTE: Call this before calling setAdapter. This is so ListView can wrap
 * the supplied cursor with one that will also account for header and footer
 * views.
 *
 * @param v The view to add.
 */
ECode ListView::AddHeaderView(
    /* [in] */ IView* v)
{
    return AddHeaderView(v, NULL, TRUE);
}

//@Override
Int32 ListView::GetHeaderViewsCount()
{
    return mHeaderViewInfos.GetSize();
}

/**
 * Removes a previously-added header view.
 *
 * @param v The view to remove
 * @return TRUE if the view was removed, FALSE if the view was not a header
 *         view
 */
Boolean ListView::RemoveHeaderView(
    /* [in] */ IView* v)
{
    Boolean result = FALSE;
    if (mHeaderViewInfos.GetSize() > 0) {
        IHeaderViewListAdapter::Probe(mAdapter)->RemoveHeader(v, &result);
        if (result) {
            mDataSetObserver->OnChanged();
        }
        RemoveFixedViewInfo(v, mHeaderViewInfos);
    }

    return result;
}

void ListView::RemoveFixedViewInfo(
    /* [in] */ IView* v,
    /* [in] */ Vector<AutoPtr<FixedViewInfo> >& where)
{
    Vector<AutoPtr<FixedViewInfo> >::Iterator iter;
    for (iter = where.Begin(); iter != where.End(); ++iter) {
        if ((*iter)->mView.Get() == v) {
            where.Erase(iter);
            break;
        }
    }
}

/**
 * Add a fixed view to appear at the bottom of the list. If addFooterView is
 * called more than once, the views will appear in the order they were
 * added. Views added using this call can take focus if they want.
 * <p>
 * NOTE: Call this before calling setAdapter. This is so ListView can wrap
 * the supplied cursor with one that will also account for header and footer
 * views.
 *
 * @param v The view to add.
 * @param data Data to associate with this view
 * @param isSelectable TRUE if the footer view can be selected
 */
ECode ListView::AddFooterView(
    /* [in] */ IView* v,
    /* [in] */ IInterface* data,
    /* [in] */ Boolean isSelectable)
{
    AutoPtr<FixedViewInfo> info = new FixedViewInfo();
    info->mView = v;
    info->mData = data;
    info->mIsSelectable = isSelectable;
    mFooterViewInfos.PushBack(info);

    // in the case of re-adding a footer view, or adding one later on,
    // we need to notify the observer
    if (mDataSetObserver != NULL) {
        mDataSetObserver->OnChanged();
    }

    return NOERROR;
}

/**
 * Add a fixed view to appear at the bottom of the list. If addFooterView is called more
 * than once, the views will appear in the order they were added. Views added using
 * this call can take focus if they want.
 * <p>NOTE: Call this before calling setAdapter. This is so ListView can wrap the supplied
 * cursor with one that will also account for header and footer views.
 *
 *
 * @param v The view to add.
 */
ECode ListView::AddFooterView(
    /* [in] */ IView* v)
{
    return AddFooterView(v, NULL, TRUE);
}

//@Override
Int32 ListView::GetFooterViewsCount()
{
    return mFooterViewInfos.GetSize();
}

/**
 * Removes a previously-added footer view.
 *
 * @param v The view to remove
 * @return
 * TRUE if the view was removed, FALSE if the view was not a footer view
 */
Boolean ListView::RemoveFooterView(
    /* [in] */ IView* v)
{
    Boolean result = FALSE;
    if (mFooterViewInfos.GetSize() > 0) {
        IHeaderViewListAdapter::Probe(mAdapter)->RemoveFooter(v, &result);
        if (result) {
            mDataSetObserver->OnChanged();
        }
        RemoveFixedViewInfo(v, mFooterViewInfos);
    }

    return result;
}

/**
 * Returns the adapter currently in use in this ListView. The returned adapter
 * might not be the same adapter passed to {@link #setAdapter(ListAdapter)} but
 * might be a {@link WrapperListAdapter}.
 *
 * @return The adapter currently used to display data in this ListView.
 *
 * @see #setAdapter(ListAdapter)
 */
//@Override
AutoPtr<IAdapter> ListView::GetAdapter()
{
    return mAdapter != NULL ? IListAdapter::Probe(mAdapter) : NULL;
}

/**
 * Sets the data behind this ListView.
 *
 * The adapter passed to this method may be wrapped by a {@link WrapperListAdapter},
 * depending on the ListView features currently in use. For instance, adding
 * headers and/or footers will cause the adapter to be wrapped.
 *
 * @param adapter The ListAdapter which is responsible for maintaining the
 *        data backing this list and for producing a view to represent an
 *        item in that data set.
 *
 * @see #GetAdapter()
 */
//@Override
ECode ListView::SetAdapter(
    /* [in] */ IAdapter* adapter)
{
    if (adapter->Probe(EIID_IListAdapter) == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (NULL != mAdapter) {
        mAdapter->UnregisterDataSetObserver(mDataSetObserver);
    }

    ResetList();
    mRecycler->Clear();

    if (mHeaderViewInfos.GetSize() > 0|| mFooterViewInfos.GetSize() > 0) {
        AutoPtr<IObjectContainer> headerViewInfos;
        CParcelableObjectContainer::New((IObjectContainer**)&headerViewInfos);
        Vector<AutoPtr<FixedViewInfo> >::Iterator iter;
        for (iter = mHeaderViewInfos.Begin(); iter != mHeaderViewInfos.End(); ++iter) {
            headerViewInfos->Add((*iter)->Probe(EIID_IFixedViewInfo));
        }

        AutoPtr<IObjectContainer> footerViewInfos;
        CParcelableObjectContainer::New((IObjectContainer**)&footerViewInfos);
        for (iter = mFooterViewInfos.Begin(); iter != mFooterViewInfos.End(); ++iter) {
            footerViewInfos->Add((*iter)->Probe(EIID_IFixedViewInfo));
        }

        CHeaderViewListAdapter::New(
            headerViewInfos, footerViewInfos, (IListAdapter*)adapter,
            (IHeaderViewListAdapter**)&mAdapter);
    }
    else {
        mAdapter = (IListAdapter*)adapter;
    }

    mOldSelectedPosition = AdapterView_INVALID_POSITION;
    mOldSelectedRowId = AdapterView_INVALID_ROW_ID;
    if (mAdapter != NULL) {
        mAdapter->AreAllItemsEnabled(&mAreAllItemsSelectable);
        mOldItemCount = mItemCount;
        mAdapter->GetCount(&mItemCount);
        CheckFocus();

        mDataSetObserver = new AdapterDataSetObserver(this);
        mAdapter->RegisterDataSetObserver(mDataSetObserver);

        Int32 viewTypeCount;
        mAdapter->GetViewTypeCount(&viewTypeCount);
        mRecycler->SetViewTypeCount(viewTypeCount);

        Int32 position;
        if (mStackFromBottom) {
            position = LookForSelectablePosition(mItemCount - 1, FALSE);
        }
        else {
            position = LookForSelectablePosition(0, TRUE);
        }

        SetSelectedPositionInt(position);
        SetNextSelectedPositionInt(position);

        if (mItemCount == 0) {
            // Nothing selected
            CheckSelectionChanged();
        }

        Boolean hasStableIds;
        mAdapter->HasStableIds(&hasStableIds);
        if (mChoiceMode != CHOICE_MODE_NONE &&
            hasStableIds && mCheckedIdStates == NULL) {
            mCheckedIdStates = new HashMap<Int64, Boolean>();
        }
    }
    else {
        mAreAllItemsSelectable = TRUE;
        CheckFocus();
        // Nothing selected
        CheckSelectionChanged();
    }

    if (mCheckStates != NULL) {
        mCheckStates->Clear();
    }

    if (mCheckedIdStates != NULL) {
        mCheckedIdStates->Clear();
    }

    RequestLayout();

    return NOERROR;
}

/**
 * The list is empty. Clear everything out.
 */
//@Override
void ListView::ResetList()
{
    // The parent's resetList() will remove all views from the layout so we need to
    // cleanup the state of our footers and headers
    ClearRecycledState(mHeaderViewInfos);
    ClearRecycledState(mFooterViewInfos);

    AbsListView::ResetList();

    mLayoutMode = LAYOUT_NORMAL;
}

void ListView::ClearRecycledState(
    /* [in] */ Vector<AutoPtr<FixedViewInfo> >& infos)
{
    Vector<AutoPtr<FixedViewInfo> >::Iterator iter;
    for (iter=infos.Begin(); iter!=infos.End(); ++iter) {
        AutoPtr<IViewGroupLayoutParams> lp;
        (*iter)->mView->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        CAbsListViewLayoutParams* p =
            (CAbsListViewLayoutParams*)IAbsListViewLayoutParams::Probe(lp);
        if (p != NULL) {
            p->mRecycledHeaderFooter = FALSE;
        }
    }
}

/**
 * @return Whether the list needs to show the top fading edge
 */
Boolean ListView::ShowingTopFadingEdge()
{
    Int32 listTop = mScrollY + mListPadding->mTop;
    Int32 top;
    GetChildAt(0)->GetTop(&top);
    return (mFirstPosition > 0) || (top > listTop);
}

/**
 * @return Whether the list needs to show the bottom fading edge
 */
Boolean ListView::ShowingBottomFadingEdge()
{
    Int32 childCount = GetChildCount();
    Int32 bottomOfBottomChild;
    GetChildAt(childCount - 1)->GetBottom(&bottomOfBottomChild);
    Int32 lastVisiblePosition = mFirstPosition + childCount - 1;

    Int32 listBottom = mScrollY + GetHeight() - mListPadding->mBottom;

    return (lastVisiblePosition < mItemCount - 1)
            || (bottomOfBottomChild < listBottom);
}

//@Override
Boolean ListView::RequestChildRectangleOnScreen(
    /* [in] */ IView* child,
    /* [in] */ IRect* r,
    /* [in] */ Boolean immediate)
{
    View* childView = (View*)child->Probe(EIID_View);
    CRect* rect = (CRect*)r;
    Int32 rectTopWithinChild = rect->mTop;

    // offset so rect is in coordinates of the this view
    rect->Offset(childView->GetLeft(), childView->GetTop());
    rect->Offset(-childView->GetScrollX(), -childView->GetScrollY());

    Int32 height = GetHeight();
    Int32 listUnfadedTop = GetScrollY();
    Int32 listUnfadedBottom = listUnfadedTop + height;
    Int32 fadingEdge = GetVerticalFadingEdgeLength();

    if (ShowingTopFadingEdge()) {
        // leave room for top fading edge as Int64 as rect isn't at very top
        if ((mSelectedPosition > 0) || (rectTopWithinChild > fadingEdge)) {
            listUnfadedTop += fadingEdge;
        }
    }

    Int32 childCount = GetChildCount();
    Int32 bottomOfBottomChild;
    GetChildAt(childCount - 1)->GetBottom(&bottomOfBottomChild);

    if (ShowingBottomFadingEdge()) {
        // leave room for bottom fading edge as Int64 as rect isn't at very bottom
        if ((mSelectedPosition < mItemCount - 1)
            || (rect->mBottom < (bottomOfBottomChild - fadingEdge))) {
            listUnfadedBottom -= fadingEdge;
        }
    }

    Int32 scrollYDelta = 0;

    if (rect->mBottom > listUnfadedBottom && rect->mTop > listUnfadedTop) {
        // need to MOVE DOWN to Get it in view: move down just enough so
        // that the entire rectangle is in view (or at least the first
        // screen size chunk).

        if (rect->GetHeight() > height) {
            // just enough to Get screen size chunk on
            scrollYDelta += (rect->mTop - listUnfadedTop);
        }
        else {
            // Get entire rect at bottom of screen
            scrollYDelta += (rect->mBottom - listUnfadedBottom);
        }

        // make sure we aren't scrolling beyond the end of our children
        Int32 distanceToBottom = bottomOfBottomChild - listUnfadedBottom;
        scrollYDelta = Math::Min(scrollYDelta, distanceToBottom);
    }
    else if (rect->mTop < listUnfadedTop && rect->mBottom < listUnfadedBottom) {
        // need to MOVE UP to Get it in view: move up just enough so that
        // entire rectangle is in view (or at least the first screen
        // size chunk of it).

        if (rect->GetHeight() > height) {
            // screen size chunk
            scrollYDelta -= (listUnfadedBottom - rect->mBottom);
        }
        else {
            // entire rect at top
            scrollYDelta -= (listUnfadedTop - rect->mTop);
        }

        // make sure we aren't scrolling any further than the top our children
        Int32 top;
        GetChildAt(0)->GetTop(&top);
        Int32 deltaToTop = top - listUnfadedTop;
        scrollYDelta = Math::Max(scrollYDelta, deltaToTop);
    }

    Boolean scroll = scrollYDelta != 0;
    if (scroll) {
        ScrollListItemsBy(-scrollYDelta);
        PositionSelector(child);
        mSelectedTop = childView->GetTop();
        Invalidate();
    }

    return scroll;
}

/**
 * {@inheritDoc}
 */
//@Override
void ListView::FillGap(
    /* [in] */ Boolean down)
{
    Int32 count = GetChildCount();
    if (down) {
        Int32 bottom;
        GetChildAt(count - 1)->GetBottom(&bottom);
        Int32 startOffset = count > 0 ? bottom + mDividerHeight
            : GetListPaddingTop();
        FillDown(mFirstPosition + count, startOffset);
        CorrectTooHigh(GetChildCount());
    }
    else {
        Int32 top;
        GetChildAt(0)->GetTop(&top);
        Int32 startOffset = count > 0 ? top - mDividerHeight :
                GetHeight() - GetListPaddingBottom();
        FillUp(mFirstPosition - 1, startOffset);
        CorrectTooLow(GetChildCount());
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
AutoPtr<IView> ListView::FillDown(
    /* [in] */ Int32 pos,
    /* [in] */ Int32 nextTop)
{
    //printf("ListView::FillDown pos = %d, nextTop = %d\n", pos, nextTop);
    AutoPtr<IView> selectedView;

    Int32 end = (mBottom - mTop) - mListPadding->mBottom;

    while (nextTop < end && pos < mItemCount) {
        // is this the selected item?
        Boolean selected = pos == mSelectedPosition;
        AutoPtr<IView> child = MakeAndAddView(
            pos, nextTop, TRUE, mListPadding->mLeft, selected);

        nextTop = ((View*)child->Probe(EIID_View))->GetBottom()
            + mDividerHeight;
        if (selected) {
            selectedView = child;
        }
        pos++;
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
AutoPtr<IView> ListView::FillUp(
    /* [in] */ Int32 pos,
    /* [in] */ Int32 nextBottom)
{
    //printf("ListView::FillUp pos = %d, nextBottom = %d\n", pos, nextBottom);
    AutoPtr<IView> selectedView;

    Int32 end = mListPadding->mTop;

    while (nextBottom > end && pos >= 0) {
        // is this the selected item?
        Boolean selected = pos == mSelectedPosition;
        AutoPtr<IView> child = MakeAndAddView(
            pos, nextBottom, FALSE, mListPadding->mLeft, selected);
        nextBottom = ((View*)child->Probe(EIID_View))->GetTop()
            - mDividerHeight;
        if (selected) {
            selectedView = child;
        }
        pos--;
    }

    mFirstPosition = pos + 1;

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
AutoPtr<IView> ListView::FillFromTop(
    /* [in] */ Int32 nextTop)
{
    mFirstPosition = Math::Min(mFirstPosition, mSelectedPosition);
    mFirstPosition = Math::Min(mFirstPosition, mItemCount - 1);
    if (mFirstPosition < 0) {
        mFirstPosition = 0;
    }

    return FillDown(mFirstPosition, nextTop);
}


/**
 * Put mSelectedPosition in the middle of the screen and then build up and
 * down from there. This method forces mSelectedPosition to the center.
 *
 * @param childrenTop Top of the area in which children can be drawn, as
 *        measured in pixels
 * @param childrenBottom Bottom of the area in which children can be drawn,
 *        as measured in pixels
 * @return Currently selected view
 */
AutoPtr<IView> ListView::FillFromMiddle(
    /* [in] */ Int32 childrenTop,
    /* [in] */ Int32 childrenBottom)
{
    Int32 height = childrenBottom - childrenTop;
    Int32 position = ReconcileSelectedPosition();

    AutoPtr<IView> sel = MakeAndAddView(
        position, childrenTop, TRUE, mListPadding->mLeft, TRUE);
    mFirstPosition = position;

    Int32 selHeight;
    sel->GetMeasuredHeight(&selHeight);
    if (selHeight <= height) {
        sel->OffsetTopAndBottom((height - selHeight) / 2);
    }

    FillAboveAndBelow(sel, position);

    if (!mStackFromBottom) {
        CorrectTooHigh(GetChildCount());
    }
    else {
        CorrectTooLow(GetChildCount());
    }

    return sel;
}

/**
 * Once the selected view as been placed, fill up the visible area above and
 * below it.
 *
 * @param sel The selected view
 * @param position The position corresponding to sel
 */
void ListView::FillAboveAndBelow(
    /* [in] */ IView* sel,
    /* [in] */ Int32 position)
{
    Int32 dividerHeight = mDividerHeight;
    View* selView = (View*)sel->Probe(EIID_View);
    if (!mStackFromBottom) {
        FillUp(position - 1, selView->GetTop() - dividerHeight);
        AdjustViewsUpOrDown();
        FillDown(position + 1, selView->GetBottom() + dividerHeight);
    }
    else {
        FillDown(position + 1, selView->GetBottom() + dividerHeight);
        AdjustViewsUpOrDown();
        FillUp(position - 1, selView->GetTop() - dividerHeight);
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
AutoPtr<IView> ListView::FillFromSelection(
    /* [in] */ Int32 selectedTop,
    /* [in] */ Int32 childrenTop,
    /* [in] */ Int32 childrenBottom)
{
    Int32 fadingEdgeLength = GetVerticalFadingEdgeLength();
    Int32 selectedPosition = mSelectedPosition;

    Int32 topSelectionPixel = GetTopSelectionPixel(
        childrenTop, fadingEdgeLength, selectedPosition);

    Int32 bottomSelectionPixel = GetBottomSelectionPixel(
        childrenBottom, fadingEdgeLength,selectedPosition);

    AutoPtr<IView> sel = MakeAndAddView(
        selectedPosition, selectedTop, TRUE, mListPadding->mLeft, TRUE);

    Int32 top, bottom;
    sel->GetTop(&top);
    sel->GetBottom(&bottom);
    // Some of the newly selected item extends below the bottom of the list
    if (bottom > bottomSelectionPixel) {
        // Find space available above the selection into which we can scroll
        // upwards
        Int32 spaceAbove = top - topSelectionPixel;

        // Find space required to bring the bottom of the selected item
        // fully into view
        Int32 spaceBelow = bottom - bottomSelectionPixel;
        Int32 offset = Math::Min(spaceAbove, spaceBelow);

        // Now offset the selected item to Get it into view
        sel->OffsetTopAndBottom(-offset);
    }
    else if (top < topSelectionPixel) {
        // Find space required to bring the top of the selected item fully
        // into view
        Int32 spaceAbove = topSelectionPixel - top;

        // Find space available below the selection into which we can scroll
        // downwards
        Int32 spaceBelow = bottomSelectionPixel - bottom;
        Int32 offset = Math::Min(spaceAbove, spaceBelow);

        // Offset the selected item to Get it into view
        sel->OffsetTopAndBottom(offset);
    }

    // Fill in views above and below
    FillAboveAndBelow(sel, selectedPosition);

    if (!mStackFromBottom) {
        CorrectTooHigh(GetChildCount());
    }
    else {
        CorrectTooLow(GetChildCount());
    }

    return sel;
}

/**
 * Calculate the bottom-most pixel we can draw the selection into
 *
 * @param childrenBottom Bottom pixel were children can be drawn
 * @param fadingEdgeLength Length of the fading edge in pixels, if present
 * @param selectedPosition The position that will be selected
 * @return The bottom-most pixel we can draw the selection into
 */
Int32 ListView::GetBottomSelectionPixel(
    /* [in] */ Int32 childrenBottom,
    /* [in] */ Int32 fadingEdgeLength,
    /* [in] */ Int32 selectedPosition)
{
    Int32 bottomSelectionPixel = childrenBottom;
    if (selectedPosition != mItemCount - 1) {
        bottomSelectionPixel -= fadingEdgeLength;
    }

    return bottomSelectionPixel;
}

/**
 * Calculate the top-most pixel we can draw the selection into
 *
 * @param childrenTop Top pixel were children can be drawn
 * @param fadingEdgeLength Length of the fading edge in pixels, if present
 * @param selectedPosition The position that will be selected
 * @return The top-most pixel we can draw the selection into
 */
Int32 ListView::GetTopSelectionPixel(
    /* [in] */ Int32 childrenTop,
    /* [in] */ Int32 fadingEdgeLength,
    /* [in] */ Int32 selectedPosition)
{
    // first pixel we can draw the selection into
    Int32 topSelectionPixel = childrenTop;
    if (selectedPosition > 0) {
        topSelectionPixel += fadingEdgeLength;
    }

    return topSelectionPixel;
}

/**
 * Fills the list based on positioning the new selection relative to the old
 * selection. The new selection will be placed at, above, or below the
 * location of the new selection depending on how the selection is moving.
 * The selection will then be pinned to the visible part of the screen,
 * excluding the edges that are faded. The list is then filled upwards and
 * downwards from there.
 *
 * @param oldSel The old selected view. Useful for trying to put the new
 *        selection in the same place
 * @param newSel The view that is to become selected. Useful for trying to
 *        put the new selection in the same place
 * @param delta Which way we are moving
 * @param childrenTop Where to start drawing children
 * @param childrenBottom Last pixel where children can be drawn
 * @return The view that currently has selection
 */
AutoPtr<IView> ListView::MoveSelection(
    /* [in] */ IView* oldSel,
    /* [in] */ IView* newSel,
    /* [in] */ Int32 delta,
    /* [in] */ Int32 childrenTop,
    /* [in] */ Int32 childrenBottom)
{
    Int32 fadingEdgeLength = GetVerticalFadingEdgeLength();
    Int32 selectedPosition = mSelectedPosition;

    AutoPtr<IView> sel;

    Int32 topSelectionPixel = GetTopSelectionPixel(
                    childrenTop, fadingEdgeLength, selectedPosition);
    Int32 bottomSelectionPixel = GetBottomSelectionPixel(
                    childrenTop, fadingEdgeLength, selectedPosition);

    if (delta > 0) {
        /*
         * Case 1: Scrolling down.
         */

        /*
         *     Before           After
         *    |       |        |       |
         *    +-------+        +-------+
         *    |   A   |        |   A   |
         *    |   1   |   =>   +-------+
         *    +-------+        |   B   |
         *    |   B   |        |   2   |
         *    +-------+        +-------+
         *    |       |        |       |
         *
         *    Try to keep the top of the previously selected item where it was.
         *    oldSel = A
         *    sel = B
         */

        // Put oldSel (A) where it belongs
        oldSel = MakeAndAddView(
            selectedPosition - 1, ((View*)oldSel->Probe(EIID_View))->GetTop(),
            TRUE, mListPadding->mLeft, FALSE);

        Int32 dividerHeight = mDividerHeight;

        // Now put the new selection (B) below that
        sel = MakeAndAddView(
            selectedPosition, ((View*)oldSel->Probe(EIID_View))->GetBottom()
            + dividerHeight, TRUE, mListPadding->mLeft, TRUE);

        Int32 top, bottom;
        sel->GetTop(&top);
        sel->GetBottom(&bottom);
        // Some of the newly selected item extends below the bottom of the list
        if (bottom > bottomSelectionPixel) {

            // Find space available above the selection into which we can scroll upwards
            Int32 spaceAbove = top - topSelectionPixel;

            // Find space required to bring the bottom of the selected item fully into view
            Int32 spaceBelow = bottom - bottomSelectionPixel;

            // Don't scroll more than half the height of the list
            Int32 halfVerticalSpace = (childrenBottom - childrenTop) / 2;
            Int32 offset = Math::Min(spaceAbove, spaceBelow);
            offset = Math::Min(offset, halfVerticalSpace);

            // We placed oldSel, so offset that item
            oldSel->OffsetTopAndBottom(-offset);
            // Now offset the selected item to Get it into view
            sel->OffsetTopAndBottom(-offset);
        }

        // Fill in views above and below
        if (!mStackFromBottom) {
            FillUp(mSelectedPosition - 2, top - dividerHeight);
            AdjustViewsUpOrDown();
            FillDown(mSelectedPosition + 1, bottom + dividerHeight);
        }
        else {
            FillDown(mSelectedPosition + 1, bottom + dividerHeight);
            AdjustViewsUpOrDown();
            FillUp(mSelectedPosition - 2, top - dividerHeight);
        }
    }
    else if (delta < 0) {
        /*
         * Case 2: Scrolling up.
         */

        /*
         *     Before           After
         *    |       |        |       |
         *    +-------+        +-------+
         *    |   A   |        |   A   |
         *    +-------+   =>   |   1   |
         *    |   B   |        +-------+
         *    |   2   |        |   B   |
         *    +-------+        +-------+
         *    |       |        |       |
         *
         *    Try to keep the top of the item about to become selected where it was.
         *    newSel = A
         *    olSel = B
         */

        if (newSel != NULL) {
            // Try to position the top of newSel (A) where it was before it was selected
            sel = MakeAndAddView(
                selectedPosition, ((View*)newSel->Probe(EIID_View))->GetTop(),
                TRUE, mListPadding->mLeft, TRUE);
        }
        else {
            // If (A) was not on screen and so did not have a view, position
            // it above the oldSel (B)
            sel = MakeAndAddView(
                selectedPosition, ((View*)oldSel->Probe(EIID_View))->GetTop(),
                FALSE, mListPadding->mLeft, TRUE);
        }

        Int32 top, bottom;
        sel->GetTop(&top);
        sel->GetBottom(&bottom);
        // Some of the newly selected item extends above the top of the list
        if (top < topSelectionPixel) {
            // Find space required to bring the top of the selected item fully into view
            Int32 spaceAbove = topSelectionPixel - top;

           // Find space available below the selection into which we can scroll downwards
            Int32 spaceBelow = bottomSelectionPixel - bottom;

            // Don't scroll more than half the height of the list
            Int32 halfVerticalSpace = (childrenBottom - childrenTop) / 2;
            Int32 offset = Math::Min(spaceAbove, spaceBelow);
            offset = Math::Min(offset, halfVerticalSpace);

            // Offset the selected item to Get it into view
            sel->OffsetTopAndBottom(offset);
        }

        // Fill in views above and below
        FillAboveAndBelow(sel, selectedPosition);
    }
    else {
        Int32 oldTop;
        oldSel->GetTop(&oldTop);

        /*
         * Case 3: Staying still
         */
        sel = MakeAndAddView(
            selectedPosition, oldTop, TRUE, mListPadding->mLeft, TRUE);

        // We're staying still...
        if (oldTop < childrenTop) {
            // ... but the top of the old selection was off screen.
            // (This can happen if the data changes size out from under us)
            Int32 newBottom;
            sel->GetBottom(&newBottom);
            if (newBottom < childrenTop + 20) {
                // Not enough visible -- bring it onscreen
                sel->OffsetTopAndBottom(
                    childrenTop - ((View*)sel->Probe(EIID_View))->GetTop());
            }
        }

        // Fill in views above and below
        FillAboveAndBelow(sel, selectedPosition);
    }

    return sel;
}

//@Override
void ListView::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    if (GetChildCount() > 0) {
        AutoPtr<IView> focusedChild = GetFocusedChild();
        if (focusedChild != NULL) {
            Int32 childPosition = mFirstPosition + IndexOfChild(focusedChild);
            Int32 childBottom;
            focusedChild->GetBottom(&childBottom);
            Int32 offset = Math::Max(0, childBottom - (h - mPaddingTop));
            Int32 top;
            focusedChild->GetTop(&top);
            top -= offset;
            if (mFocusSelector == NULL) {
                mFocusSelector = new FocusSelector(this);
            }
            Post((IRunnable*)mFocusSelector->Setup(childPosition, top)
                ->Probe(EIID_IRunnable));
        }
    }
    AbsListView::OnSizeChanged(w, h, oldw, oldh);
}

//@Override
void ListView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    // Sets up mListPadding
    AbsListView::OnMeasure(widthMeasureSpec, heightMeasureSpec);

    Int32 widthMode = MeasureSpec::GetMode(widthMeasureSpec);
    Int32 heightMode = MeasureSpec::GetMode(heightMeasureSpec);
    Int32 widthSize = MeasureSpec::GetSize(widthMeasureSpec);
    Int32 heightSize = MeasureSpec::GetSize(heightMeasureSpec);

    Int32 childWidth = 0;
    Int32 childHeight = 0;

    mItemCount = 0;
    if (mAdapter != NULL) {
        mAdapter->GetCount(&mItemCount);
    }

    if (mItemCount > 0 && (widthMode == MeasureSpec::UNSPECIFIED ||
        heightMode == MeasureSpec::UNSPECIFIED)) {
        AutoPtr<IView> child = ObtainView(0, mIsScrap);

        MeasureScrapChild(child, 0, widthMeasureSpec);

        child->GetMeasuredWidth(&childWidth);
        child->GetMeasuredHeight(&childHeight);

        AutoPtr<IViewGroupLayoutParams> lp;
        child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        CAbsListViewLayoutParams* p =
            (CAbsListViewLayoutParams*)IAbsListViewLayoutParams::Probe(lp);
        if (RecycleOnMeasure() && mRecycler->ShouldRecycleViewType(p->mViewType)) {
            mRecycler->AddScrapView(child);
        }
    }

    if (widthMode == MeasureSpec::UNSPECIFIED) {
        widthSize = mListPadding->mLeft + mListPadding->mRight + childWidth +
                GetVerticalScrollbarWidth();
    }

    if (heightMode == MeasureSpec::UNSPECIFIED) {
        heightSize = mListPadding->mTop + mListPadding->mBottom + childHeight +
                GetVerticalFadingEdgeLength() * 2;
    }

    if (heightMode == MeasureSpec::AT_MOST) {
        // TODO: after first layout we should maybe start at the first visible position, not 0
        heightSize = MeasureHeightOfChildren(
                widthMeasureSpec, 0, NO_POSITION, heightSize, -1);
    }

    SetMeasuredDimension(widthSize, heightSize);
    mWidthMeasureSpec = widthMeasureSpec;
}

void ListView::MeasureScrapChild(
    /* [in] */ IView* child,
    /* [in] */ Int32 position,
    /* [in] */ Int32 widthMeasureSpec)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    if (lp == NULL) {
        CAbsListViewLayoutParams::New(
            ViewGroupLayoutParams_MATCH_PARENT,
            ViewGroupLayoutParams_WRAP_CONTENT,
            0, (IAbsListViewLayoutParams**)&lp);
        child->SetLayoutParams(lp);
    }
    CAbsListViewLayoutParams* p = (CAbsListViewLayoutParams*)lp.Get();
    mAdapter->GetItemViewType(position, &p->mViewType);
    p->mForceAdd = TRUE;

    Int32 childWidthSpec = ViewGroup::GetChildMeasureSpec(
        widthMeasureSpec, mListPadding->mLeft + mListPadding->mRight, p->mWidth);
    Int32 lpHeight = p->mHeight;
    Int32 childHeightSpec;
    if (lpHeight > 0) {
        childHeightSpec = MeasureSpec::MakeMeasureSpec(
            lpHeight, MeasureSpec::EXACTLY);
    }
    else {
        childHeightSpec = MeasureSpec::MakeMeasureSpec(
            0, MeasureSpec::UNSPECIFIED);
    }
    child->Measure(childWidthSpec, childHeightSpec);
}

/**
 * @return True to recycle the views used to measure this ListView in
 *         UNSPECIFIED/AT_MOST modes, FALSE otherwise.
 * @hide
 */
Boolean ListView::RecycleOnMeasure()
{
    return TRUE;
}

/**
 * Measures the height of the given range of children (inclusive) and
 * returns the height with this ListView's padding and divider heights
 * included. If maxHeight is provided, the measuring will stop when the
 * current height reaches maxHeight.
 *
 * @param widthMeasureSpec The width measure spec to be given to a child's
 *            {@link View#measure(Int32, Int32)}.
 * @param startPosition The position of the first child to be shown.
 * @param endPosition The (inclusive) position of the last child to be
 *            shown. Specify {@link #NO_POSITION} if the last child should be
 *            the last available child from the adapter.
 * @param maxHeight The maximum height that will be returned (if all the
 *            children don't fit in this value, this value will be
 *            returned).
 * @param disallowPartialChildPosition In general, whether the returned
 *            height should only contain entire children. This is more
 *            powerful--it is the first inclusive position at which partial
 *            children will not be allowed. Example: it looks nice to have
 *            at least 3 completely visible children, and in portrait this
 *            will most likely fit; but in landscape there could be times
 *            when even 2 children can not be completely shown, so a value
 *            of 2 (remember, inclusive) would be good (assuming
 *            startPosition is 0).
 * @return The height of this ListView with the given children.
 */
Int32 ListView::MeasureHeightOfChildren(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 startPosition,
    /* [in] */ Int32 endPosition,
    /* [in] */ Int32 maxHeight,
    /* [in] */ Int32 disallowPartialChildPosition)
{
    AutoPtr<IListAdapter> adapter = mAdapter;
    if (adapter == NULL) {
        return mListPadding->mTop + mListPadding->mBottom;
    }

    // Include the padding of the list
    Int32 returnedHeight = mListPadding->mTop + mListPadding->mBottom;
    Int32 dividerHeight = ((mDividerHeight > 0) && mDivider != NULL) ? mDividerHeight : 0;
    // The previous height value that was less than maxHeight and contained
    // no partial children
    Int32 prevHeightWithoutPartialChild = 0;
    Int32 i;
    AutoPtr<IView> child;

    // mItemCount - 1 since endPosition parameter is inclusive
    Int32 count;
    adapter->GetCount(&count);
    endPosition = (endPosition == NO_POSITION) ? count - 1 : endPosition;
    Boolean recyle = RecycleOnMeasure();

    for (i = startPosition; i <= endPosition; ++i) {
        child = ObtainView(i, mIsScrap);

        MeasureScrapChild(child, i, widthMeasureSpec);

        if (i > 0) {
            // Count the divider for all but one child
            returnedHeight += dividerHeight;
        }

        AutoPtr<IViewGroupLayoutParams> lp;
        child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        // Recycle the view before we possibly return from the method
        if (recyle && mRecycler->ShouldRecycleViewType(
            ((CAbsListViewLayoutParams*)lp.Get())->mViewType)) {
            mRecycler->AddScrapView(child);
        }

        returnedHeight += ((View*)child->Probe(EIID_View))->GetMeasuredHeight();

        if (returnedHeight >= maxHeight) {
            // We went over, figure out which height to return.  If returnedHeight > maxHeight,
            // then the i'th position did not fit completely.
            //
            return (disallowPartialChildPosition >= 0) // Disallowing is enabled (> -1)
                && (i > disallowPartialChildPosition) // We've past the min pos
                && (prevHeightWithoutPartialChild > 0) // We have a prev height
                && (returnedHeight != maxHeight) // i'th child did not fit completely
                ? prevHeightWithoutPartialChild : maxHeight;
        }

        if ((disallowPartialChildPosition >= 0) && (i >= disallowPartialChildPosition)) {
            prevHeightWithoutPartialChild = returnedHeight;
        }
    }

    // At this point, we went through the range of children, and they each
    // completely fit, so return the returnedHeight
    return returnedHeight;
}

//@Override
Int32 ListView::FindMotionRow(
    /* [in] */ Int32 y)
{
    Int32 childCount = GetChildCount();
    if (childCount > 0) {
        if (!mStackFromBottom) {
            for (Int32 i = 0; i < childCount; i++) {
                View* v = (View*)(GetChildAt(i)->Probe(EIID_View));
                if (y <= v->GetBottom()) {
                    return mFirstPosition + i;
                }
            }
        }
        else {
            for (Int32 i = childCount - 1; i >= 0; i--) {
                View* v = (View*)(GetChildAt(i)->Probe(EIID_View));
                if (y >= v->GetTop()) {
                    return mFirstPosition + i;
                }
            }
        }
    }

    return AdapterView_INVALID_POSITION;
}

/**
 * Put a specific item at a specific location on the screen and then build
 * up and down from there.
 *
 * @param position The reference view to use as the starting point
 * @param top Pixel offset from the top of this view to the top of the
 *        reference view.
 *
 * @return The selected view, or NULL if the selected view is outside the
 *         visible area.
 */
AutoPtr<IView> ListView::FillSpecific(
    /* [in] */ Int32 position,
    /* [in] */ Int32 top)
{
    Boolean tempIsSelected = position == mSelectedPosition;
    AutoPtr<IView> temp = MakeAndAddView(
        position, top, TRUE, mListPadding->mLeft, tempIsSelected);
    // Possibly changed again in fillUp if we add rows above this one.
    mFirstPosition = position;

    Int32 topTemp, bottomTemp;
    temp->GetTop(&topTemp);
    temp->GetBottom(&bottomTemp);

    AutoPtr<IView> above;
    AutoPtr<IView> below;

    Int32 dividerHeight = mDividerHeight;
    if (!mStackFromBottom) {
        above = FillUp(position - 1, topTemp - dividerHeight);
        // This will correct for the top of the first view not touching the top of the list
        AdjustViewsUpOrDown();
        below = FillDown(position + 1, bottomTemp + dividerHeight);
        Int32 childCount = GetChildCount();
        if (childCount > 0) {
            CorrectTooHigh(childCount);
        }
    }
    else {
        below = FillDown(position + 1, bottomTemp + dividerHeight);
        // This will correct for the bottom of the last view not touching the bottom of the list
        AdjustViewsUpOrDown();
        above = FillUp(position - 1, topTemp - dividerHeight);
        Int32 childCount = GetChildCount();
        if (childCount > 0) {
             CorrectTooLow(childCount);
        }
    }

    if (tempIsSelected) {
        return temp;
    }
    else if (above != NULL) {
        return above;
    }
    else {
        return below;
    }
}

/**
 * Check if we have dragged the bottom of the list too high (we have pushed the
 * top element off the top of the screen when we did not need to). Correct by sliding
 * everything back down.
 *
 * @param childCount Number of children
 */
void ListView::CorrectTooHigh(
    /* [in] */ Int32 childCount)
{
    // First see if the last item is visible. If it is not, it is OK for the
    // top of the list to be pushed up.
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
                FillUp(mFirstPosition - 1, firstTop - mDividerHeight);
                // Close up the remaining gap
                AdjustViewsUpOrDown();
            }
        }
    }
}

/**
 * Check if we have dragged the bottom of the list too low (we have pushed the
 * bottom element off the bottom of the screen when we did not need to). Correct by sliding
 * everything back up.
 *
 * @param childCount Number of children
 */
void ListView::CorrectTooLow(
    /* [in] */ Int32 childCount)
{
    // First see if the first item is visible. If it is not, it is OK for the
    // bottom of the list to be pushed down.
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
        if (topOffset > 0) {
            if (lastPosition < mItemCount - 1 || lastBottom > end)  {
                if (lastPosition == mItemCount - 1) {
                    // Don't pull the bottom too far up
                    topOffset = Math::Min(topOffset, lastBottom - end);
                }
                // Move everything up
                OffsetChildrenTopAndBottom(-topOffset);
                if (lastPosition < mItemCount - 1) {
                    // Fill the gap that was opened below the last position with more rows, if
                    // possible
                    FillDown(lastPosition + 1, lastBottom + mDividerHeight);
                    // Close up the remaining gap
                    AdjustViewsUpOrDown();
                }
            }
            else if (lastPosition == mItemCount - 1) {
                AdjustViewsUpOrDown();
            }
        }
    }
}

//@Override
void ListView::LayoutChildren()
{
    Boolean blockLayoutRequests = mBlockLayoutRequests;
    if (!blockLayoutRequests) {
        mBlockLayoutRequests = TRUE;
    }
    else {
        return;
    }

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
    Int32 index = 0;
    Int32 delta = 0;

    AutoPtr<IView> sel;
    AutoPtr<IView> oldSel;
    AutoPtr<IView> oldFirst;
    AutoPtr<IView> newSel;

    AutoPtr<IView> focusLayoutRestoreView;

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
        default:
            // Remember the previously selected view
            index = mSelectedPosition - mFirstPosition;
            if (index >= 0 && index < childCount) {
                oldSel = GetChildAt(index);
            }

            // Remember the previous first child
            oldFirst = GetChildAt(0);

            if (mNextSelectedPosition >= 0) {
                delta = mNextSelectedPosition - mSelectedPosition;
            }

            // Caution: newSel might be NULL
            newSel = GetChildAt(index + delta);
    }

    if (mDataChanged) {
        HandleDataChanged();
    }

    Int32 count;
    mAdapter->GetCount(&count);
    // Handle the empty set by removing all views that are visible
    // and calling it a day
    if (mItemCount == 0) {
        ResetList();
        InvokeOnItemScrollListener();
        return;
    }
    else if (mItemCount != count) {
        //throw new IllegalStateException("The content of the adapter has changed but "
        //        + "ListView did not receive a notification. Make sure the content of "
        //        + "your adapter is not modified from a background thread, but only "
        //        + "from the UI thread. [in ListView(" + GetId() + ", " + GetClass()
        //        + ") with Adapter(" + mAdapter.GetClass() + ")]");
        assert(0);
    }

    SetSelectedPositionInt(mNextSelectedPosition);

    // Pull all children into the RecycleBin.
    // These views will be reused if possible
    Int32 firstPosition = mFirstPosition;
    RecycleBin* recycleBin = mRecycler;

    // reset the focus restoration
    AutoPtr<IView> focusLayoutRestoreDirectChild;

    // Don't put header or footer views into the Recycler. Those are
    // already cached in mHeaderViews;
    if (mDataChanged) {
        for (Int32 i = 0; i < childCount; i++) {
            recycleBin->AddScrapView(GetChildAt(i));
            //if (ViewDebug.TRACE_RECYCLER) {
            //    ViewDebug.trace(GetChildAt(i),
            //            ViewDebug.RecyclerTraceType.MOVE_TO_SCRAP_HEAP, index, i);
            //}
        }
    }
    else {
        recycleBin->FillActiveViews(childCount, firstPosition);
    }

    // take focus back to us temporarily to avoid the eventual
    // call to clear focus when removing the focused child below
    // from messing things up when ViewRoot assigns focus back
    // to someone else
    AutoPtr<IView> focusedChild = GetFocusedChild();
    if (focusedChild != NULL) {
        // TODO: in some cases focusedChild.GetParent() == NULL

        // we can remember the focused view to restore after relayout if the
        // data hasn't changed, or if the focused position is a header or footer
        if (!mDataChanged || IsDirectChildHeaderOrFooter(focusedChild)) {
            focusLayoutRestoreDirectChild = focusedChild;
            // remember the specific view that had focus
            focusLayoutRestoreView = FindFocus();
            if (focusLayoutRestoreView != NULL) {
                // tell it we are going to mess with it
                focusLayoutRestoreView->OnStartTemporaryDetach();
            }
        }
        View::RequestFocus();
    }

    // Clear out old views
    DetachAllViewsFromParent();

    switch (mLayoutMode) {
        case LAYOUT_SET_SELECTION:
            if (newSel != NULL) {
                sel = FillFromSelection(
                    ((View*)newSel->Probe(EIID_View))->GetTop(),
                    childrenTop, childrenBottom);
            }
            else {
                sel = FillFromMiddle(childrenTop, childrenBottom);
            }
            break;
        case LAYOUT_SYNC:
            sel = FillSpecific(mSyncPosition, mSpecificTop);
            break;
        case LAYOUT_FORCE_BOTTOM:
            sel = FillUp(mItemCount - 1, childrenBottom);
            AdjustViewsUpOrDown();
            break;
        case LAYOUT_FORCE_TOP:
            mFirstPosition = 0;
            sel = FillFromTop(childrenTop);
            AdjustViewsUpOrDown();
            break;
        case LAYOUT_SPECIFIC:
            sel = FillSpecific(ReconcileSelectedPosition(), mSpecificTop);
            break;
        case LAYOUT_MOVE_SELECTION:
            sel = MoveSelection(oldSel, newSel, delta, childrenTop, childrenBottom);
            break;
        default:
            if (childCount == 0) {
                if (!mStackFromBottom) {
                    Int32 position = LookForSelectablePosition(0, TRUE);
                    SetSelectedPositionInt(position);
                    sel = FillFromTop(childrenTop);
                }
                else {
                    Int32 position = LookForSelectablePosition(mItemCount - 1, FALSE);
                    SetSelectedPositionInt(position);
                    sel = FillUp(mItemCount - 1, childrenBottom);
                }
            }
            else {
                if (mSelectedPosition >= 0 && mSelectedPosition < mItemCount) {
                    sel = FillSpecific(mSelectedPosition,
                        oldSel == NULL ? childrenTop :
                        ((View*)oldSel->Probe(EIID_View))->GetTop());
                }
                else if (mFirstPosition < mItemCount) {
                    sel = FillSpecific(mFirstPosition,
                        oldFirst == NULL ? childrenTop :
                        ((View*)oldFirst->Probe(EIID_View))->GetTop());
                }
                else {
                    sel = FillSpecific(0, childrenTop);
                }
            }
            break;
    }

    // Flush any cached views that did not Get reused above
    recycleBin->ScrapActiveViews();

    if (sel != NULL) {
        View* selView = ((View*)sel->Probe(EIID_View));

        // the current selected item should Get focus if items
        // are focusable
        if (mItemsCanFocus && HasFocus() && !selView->HasFocus()) {
            Boolean focusWasTaken = (sel == focusLayoutRestoreDirectChild &&
                ((View*)focusLayoutRestoreView->Probe(EIID_View))->RequestFocus())
                || selView->RequestFocus();
            if (!focusWasTaken) {
                // selected item didn't take focus, fine, but still want
                // to make sure something else outside of the selected view
                // has focus
                //
                AutoPtr<IView> focused = GetFocusedChild();
                if (focused != NULL) {
                    focused->ClearFocus();
                }
                PositionSelector(sel);
            }
            else {
                sel->SetSelected(FALSE);
                mSelectorRect->SetEmpty();
            }
        }
        else {
            PositionSelector(sel);
        }
        mSelectedTop = selView->GetTop();
    }
    else {
        if (mTouchMode > TOUCH_MODE_DOWN && mTouchMode < TOUCH_MODE_SCROLL) {
            AutoPtr<IView> child = GetChildAt(mMotionPosition - mFirstPosition);
            if (child != NULL)
                PositionSelector(child);
        }
        else {
            mSelectedTop = 0;
            mSelectorRect->SetEmpty();
        }

        // even if there is not selected position, we may need to restore
        // focus (i.e. something focusable in touch mode)
        if (HasFocus() && focusLayoutRestoreView != NULL) {
            Boolean result;
            focusLayoutRestoreView->RequestFocus(&result);
        }
    }

    // tell focus view we are done mucking with it, if it is still in
    // our view hierarchy.
    if (focusLayoutRestoreView != NULL
        && ((View*)focusLayoutRestoreView->Probe(EIID_View))
        ->GetWindowToken() != NULL) {
            focusLayoutRestoreView->OnFinishTemporaryDetach();
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

    if (!blockLayoutRequests) {
        mBlockLayoutRequests = FALSE;
    }
}

/**
 * @param child a direct child of this list.
 * @return Whether child is a header or footer view.
 */
Boolean ListView::IsDirectChildHeaderOrFooter(
    /* [in] */ IView* child)
{
    Vector<AutoPtr<FixedViewInfo> >::Iterator iter;
    for (iter = mHeaderViewInfos.Begin(); iter != mHeaderViewInfos.End(); ++iter) {
        if ((*iter)->mView.Get() == child) {
            return TRUE;
        }
    }

    for (iter = mFooterViewInfos.Begin(); iter != mFooterViewInfos.End(); ++iter) {
        if ((*iter)->mView.Get() == child) {
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * Obtain the view and add it to our list of children. The view can be made
 * fresh, converted from an unused view, or used as is if it was in the
 * recycle bin.
 *
 * @param position Logical position in the list
 * @param y Top or bottom edge of the view to add
 * @param flow If flow is TRUE, align top edge to y. If FALSE, align bottom
 *        edge to y.
 * @param childrenLeft Left edge where children should be positioned
 * @param selected Is this position selected?
 * @return View that was added
 */
AutoPtr<IView> ListView::MakeAndAddView(
    /* [in] */ Int32 position,
    /* [in] */ Int32 y,
    /* [in] */ Boolean flow,
    /* [in] */ Int32 childrenLeft,
    /* [in] */ Boolean selected)
{
    AutoPtr<IView> child;

    if (!mDataChanged) {
        // Try to use an exsiting view for this position
        child = mRecycler->GetActiveView(position);
        if (child != NULL) {
            //if (ViewDebug.TRACE_RECYCLER) {
            //    ViewDebug.trace(child, ViewDebug.RecyclerTraceType.RECYCLE_FROM_ACTIVE_HEAP,
            //            position, GetChildCount());
            //}

            // Found it -- we're using an existing child
            // This just needs to be positioned
            SetupChild(child, position, y, flow, childrenLeft, selected, TRUE);

            return child;
        }
    }

    // Make a new view for this position, or convert an unused view if possible
    child = ObtainView(position, mIsScrap);

    // This needs to be positioned and measured
    SetupChild(child, position, y, flow, childrenLeft, selected, mIsScrap[0]);

    return child;
}

/**
 * Add a view as a child and make sure it is measured (if necessary) and
 * positioned properly.
 *
 * @param child The view to add
 * @param position The position of this child
 * @param y The y position relative to which this view will be positioned
 * @param flowDown If TRUE, align top edge to y. If FALSE, align bottom
 *        edge to y.
 * @param childrenLeft Left edge where children should be positioned
 * @param selected Is this position selected?
 * @param recycled Has this view been pulled from the recycle bin? If so it
 *        does not need to be remeasured.
 */
void ListView::SetupChild(
    /* [in] */ IView* child,
    /* [in] */ Int32 position,
    /* [in] */ Int32 y,
    /* [in] */ Boolean flowDown,
    /* [in] */ Int32 childrenLeft,
    /* [in] */ Boolean selected,
    /* [in] */ Boolean recycled)
{
    View* childView = (View*)child->Probe(EIID_View);
    Boolean isSelected = selected && ShouldShowSelector();
    Boolean updateChildSelected = isSelected != childView->IsSelected();
    Int32 mode = mTouchMode;
    Boolean isPressed = mode > TOUCH_MODE_DOWN && mode < TOUCH_MODE_SCROLL &&
        mMotionPosition == position;
    Boolean updateChildPressed = isPressed != childView->IsPressed();
    Boolean needToMeasure = !recycled || updateChildSelected || childView->IsLayoutRequested();

    // Respect layout params that are already in the view. Otherwise make some up...
    // noinspection unchecked
    AutoPtr<IAbsListViewLayoutParams> lp =
        (IAbsListViewLayoutParams*)(childView->GetLayoutParams().Get());
    if (lp == NULL) {
        ASSERT_SUCCEEDED(CAbsListViewLayoutParams::New(
            ViewGroupLayoutParams_MATCH_PARENT,
            ViewGroupLayoutParams_WRAP_CONTENT, 0,
            (IAbsListViewLayoutParams**)&lp));
    }
    CAbsListViewLayoutParams* p = (CAbsListViewLayoutParams*)lp.Get();
    mAdapter->GetItemViewType(position, &p->mViewType);

    if ((recycled && !p->mForceAdd) || (p->mRecycledHeaderFooter &&
        p->mViewType == AdapterView_ITEM_VIEW_TYPE_HEADER_OR_FOOTER)) {
        AttachViewToParent(child, flowDown ? -1 : 0, p);
    }
    else {
        p->mForceAdd = FALSE;
        if (p->mViewType == AdapterView_ITEM_VIEW_TYPE_HEADER_OR_FOOTER) {
            p->mRecycledHeaderFooter = TRUE;
        }
        AddViewInLayout(child, flowDown ? -1 : 0, p, TRUE);
    }

    if (updateChildSelected) {
        child->SetSelected(isSelected);
    }

    if (updateChildPressed) {
        child->SetPressed(isPressed);
    }

    if (mChoiceMode != CHOICE_MODE_NONE && mCheckStates != NULL) {
//        if (child instanceof Checkable) {
//            ((Checkable) child).setChecked(mCheckStates.Get(position));
//        }
    }

    if (needToMeasure) {
        Int32 childWidthSpec = ViewGroup::GetChildMeasureSpec(
            mWidthMeasureSpec, mListPadding->mLeft + mListPadding->mRight,
            p->mWidth);
        Int32 lpHeight = p->mHeight;
        Int32 childHeightSpec;
        if (lpHeight > 0) {
            childHeightSpec = MeasureSpec::MakeMeasureSpec(
                lpHeight, MeasureSpec::EXACTLY);
        }
        else {
            childHeightSpec = MeasureSpec::MakeMeasureSpec(
                0, MeasureSpec::UNSPECIFIED);
        }
        child->Measure(childWidthSpec, childHeightSpec);
    }
    else {
        CleanupLayoutState(child);
    }

    Int32 w = childView->GetMeasuredWidth();
    Int32 h = childView->GetMeasuredHeight();
    Int32 childTop = flowDown ? y : y - h;

    if (needToMeasure) {
        Int32 childRight = childrenLeft + w;
        Int32 childBottom = childTop + h;
        child->Layout(childrenLeft, childTop, childRight, childBottom);
    }
    else {
        child->OffsetLeftAndRight(childrenLeft - childView->GetLeft());
        child->OffsetTopAndBottom(childTop - childView->GetTop());
    }

    if (mCachingStarted && !childView->IsDrawingCacheEnabled()) {
        child->SetDrawingCacheEnabled(TRUE);
    }
}

//@Override
Boolean ListView::CanAnimate()
{
    return AbsListView::CanAnimate() && mItemCount > 0;
}

/**
 * Sets the currently selected item. If in touch mode, the item will not be selected
 * but it will still be positioned appropriately. If the specified selection position
 * is less than 0, then the item at position 0 will be selected.
 *
 * @param position Index (starting at 0) of the data item to be selected.
 */
//@Override
ECode ListView::SetSelection(
    /* [in] */ Int32 position)
{
    return SetSelectionFromTop(position, 0);
}

/**
 * Sets the selected item and positions the selection y pixels from the top edge
 * of the ListView. (If in touch mode, the item will not be selected but it will
 * still be positioned appropriately.)
 *
 * @param position Index (starting at 0) of the data item to be selected.
 * @param y The distance from the top edge of the ListView (plus padding) that the
 *        item will be positioned.
 */
ECode ListView::SetSelectionFromTop(
    /* [in] */ Int32 position,
    /* [in] */ Int32 y)
{
    if (mAdapter == NULL) {
        return NOERROR;
    }

    if (!IsInTouchMode()) {
        position = LookForSelectablePosition(position, TRUE);
        if (position >= 0) {
            SetNextSelectedPositionInt(position);
        }
    }
    else {
        mResurrectToPosition = position;
    }

    if (position >= 0) {
        mLayoutMode = LAYOUT_SPECIFIC;
        mSpecificTop = mListPadding->mTop + y;

        if (mNeedSync) {
            mSyncPosition = position;
            mAdapter->GetItemId(position, &mSyncRowId);
        }

        RequestLayout();
    }

    return NOERROR;
}

/**
 * Makes the item at the supplied position selected.
 *
 * @param position the position of the item to select
 */
//@Override
void ListView::SetSelectionInt(
    /* [in] */ Int32 position)
{
    SetNextSelectedPositionInt(position);
    Boolean awakeScrollbars = FALSE;

    Int32 selectedPosition = mSelectedPosition;

    if (selectedPosition >= 0) {
        if (position == selectedPosition - 1) {
            awakeScrollbars = TRUE;
        }
        else if (position == selectedPosition + 1) {
            awakeScrollbars = TRUE;
        }
    }

    LayoutChildren();

    if (awakeScrollbars) {
        AwakenScrollBars();
    }
}

/**
 * Find a position that can be selected (i.e., is not a separator).
 *
 * @param position The starting position to look at.
 * @param lookDown Whether to look down for other positions.
 * @return The next selectable position starting at position and then searching either up or
 *         down. Returns {@link #AdapterView_INVALID_POSITION} if nothing can be found.
 */
//@Override
Int32 ListView::LookForSelectablePosition(
    /* [in] */ Int32 position,
    /* [in] */ Boolean lookDown)
{
    AutoPtr<IListAdapter> adapter = mAdapter;
    if (adapter == NULL || IsInTouchMode()) {
        return AdapterView_INVALID_POSITION;
    }

    Int32 count;
    adapter->GetCount(&count);
    if (!mAreAllItemsSelectable) {
        if (lookDown) {
            position = Math::Max(0, position);
            Boolean isEnabled;
            adapter->IsEnabled(position, &isEnabled);
            while (position < count && !isEnabled) {
                position++;
            }
        }
        else {
            position = Math::Min(position, count - 1);
            Boolean isEnabled;
            adapter->IsEnabled(position, &isEnabled);
            while (position >= 0 && !isEnabled) {
                position--;
            }
        }

        if (position < 0 || position >= count) {
            return AdapterView_INVALID_POSITION;
        }
        return position;
    }
    else {
        if (position < 0 || position >= count) {
            return AdapterView_INVALID_POSITION;
        }
        return position;
    }
}

//@Override
Boolean ListView::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    Boolean populated = AbsListView::DispatchPopulateAccessibilityEvent(event);

    // If the item count is less than 15 then subtract disabled items from the count and
    // position. Otherwise ignore disabled items.
    if (!populated) {
        Int32 itemCount = 0;
        Int32 currentItemIndex = GetSelectedItemPosition();

        AutoPtr<IListAdapter> adapter = (IListAdapter*)GetAdapter().Get();
        if (adapter != NULL) {
            Int32 count;
            adapter->GetCount(&count);
            if (count < 15) {
                for (Int32 i = 0; i < count; i++) {
                    Boolean isEnabled;
                    adapter->IsEnabled(i, &isEnabled);
                    if (isEnabled) {
                        itemCount++;
                    }
                    else if (i <= currentItemIndex) {
                        currentItemIndex--;
                    }
                }
            }
            else {
                itemCount = count;
            }
        }

//        event->SetItemCount(itemCount);
//        event->SetCurrentItemIndex(currentItemIndex);
    }

    return populated;
}

/**
 * setSelectionAfterHeaderView set the selection to be the first list item
 * after the header views.
 */
ECode ListView::SetSelectionAfterHeaderView()
{
    Int32 count = mHeaderViewInfos.GetSize();
    if (count > 0) {
        mNextSelectedPosition = 0;
        return NOERROR;
    }

    if (mAdapter != NULL) {
        SetSelection(count);
    }
    else {
        mNextSelectedPosition = count;
        mLayoutMode = LAYOUT_SET_SELECTION;
    }

    return NOERROR;
}

//@Override
Boolean ListView::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    // Dispatch in the normal way
    Boolean handled = AbsListView::DispatchKeyEvent(event);
    if (!handled) {
        // If we didn't handle it...
        AutoPtr<IView> focused = GetFocusedChild();
        Int32 action;
        event->GetAction(&action);
        if (focused != NULL && action == KeyEvent_ACTION_DOWN) {
            // ... and our focused child didn't handle it
            // ... give it to ourselves so we can scroll if necessary
            //
            Int32 keyCode;
            event->GetKeyCode(&keyCode);
            handled = OnKeyDown(keyCode, event);
        }
    }
    return handled;
}

//@Override
Boolean ListView::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return CommonKey(keyCode, 1, event);
}

//@Override
Boolean ListView::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 repeatCount,
    /* [in] */ IKeyEvent* event)
{
    return CommonKey(keyCode, repeatCount, event);
}

//@Override
Boolean ListView::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return CommonKey(keyCode, 1, event);
}

Boolean ListView::CommonKey(
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
            case KeyEvent_KEYCODE_DPAD_CENTER:
            case KeyEvent_KEYCODE_ENTER:
            case KeyEvent_KEYCODE_SPACE:
                if (ResurrectSelection()) {
                    return TRUE;
                }
            }
        }

        switch (keyCode) {
        case KeyEvent_KEYCODE_DPAD_UP:
            {
                Boolean isAltPressed;
                event->IsAltPressed(&isAltPressed);
                if (!isAltPressed) {
                    while (count > 0) {
                        handled = ArrowScroll(FOCUS_UP);
                        count--;
                    }
                }
                else {
                    handled = FullScroll(FOCUS_UP);
                }
            }
            break;
        case KeyEvent_KEYCODE_DPAD_DOWN:
            {
                Boolean isAltPressed;
                event->IsAltPressed(&isAltPressed);
                if (!isAltPressed) {
                    while (count > 0) {
                        handled = ArrowScroll(FOCUS_DOWN);
                        count--;
                    }
                }
                else {
                    handled = FullScroll(FOCUS_DOWN);
                }
            }
            break;
        case KeyEvent_KEYCODE_DPAD_LEFT:
            handled = HandleHorizontalFocusWithinListItem(View::FOCUS_LEFT);
            break;
        case KeyEvent_KEYCODE_DPAD_RIGHT:
            handled = HandleHorizontalFocusWithinListItem(View::FOCUS_RIGHT);
            break;
        case KeyEvent_KEYCODE_DPAD_CENTER:
        case KeyEvent_KEYCODE_ENTER:
            {
                Int32 repeatCount;
                event->GetRepeatCount(&repeatCount);
                if (mItemCount > 0 && repeatCount == 0) {
                    KeyPressed();
                }
                handled = TRUE;
            }
            break;
        case KeyEvent_KEYCODE_SPACE:
            {
//                if (mPopup == NULL || !mPopup.isShowing()) {
                    Boolean isShiftPressed;
                    event->IsShiftPressed(&isShiftPressed);
                    if (!isShiftPressed) {
                        PageScroll(FOCUS_DOWN);
                    }
                    else {
                        PageScroll(FOCUS_UP);
                    }
                    handled = TRUE;
//                }
            }
            break;
        default:
            break;
        }
    }

    if (!handled) {
        handled = SendToTextFilter(keyCode, count, event);
    }

    if (handled) {
        return TRUE;
    }
    else {
        switch (action) {
        case KeyEvent_ACTION_DOWN:
            return AbsListView::OnKeyDown(keyCode, event);
        case KeyEvent_ACTION_UP:
            return AbsListView::OnKeyUp(keyCode, event);
        case KeyEvent_ACTION_MULTIPLE:
            return AbsListView::OnKeyMultiple(keyCode, count, event);
        default: // shouldn't happen
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
Boolean ListView::PageScroll(
    /* [in] */ Int32 direction)
{
    Int32 nextPage = -1;
    Boolean down = FALSE;

    if (direction == FOCUS_UP) {
        nextPage = Math::Max(0, mSelectedPosition - GetChildCount() - 1);
    }
    else if (direction == FOCUS_DOWN) {
        nextPage = Math::Min(mItemCount - 1, mSelectedPosition + GetChildCount() - 1);
        down = TRUE;
    }

    if (nextPage >= 0) {
        Int32 position = LookForSelectablePosition(nextPage, down);
        if (position >= 0) {
            mLayoutMode = LAYOUT_SPECIFIC;
            mSpecificTop = mPaddingTop + GetVerticalFadingEdgeLength();

            if (down && position > mItemCount - GetChildCount()) {
                mLayoutMode = LAYOUT_FORCE_BOTTOM;
            }

            if (!down && position < GetChildCount()) {
                mLayoutMode = LAYOUT_FORCE_TOP;
            }

            SetSelectionInt(position);
            InvokeOnItemScrollListener();
            if (!AwakenScrollBars()) {
                Invalidate();
            }

            return TRUE;
        }
    }

    return FALSE;
}

/**
 * Go to the last or first item if possible (not worrying about panning across or navigating
 * within the internal focus of the currently selected item.)
 *
 * @param direction either {@link View#FOCUS_UP} or {@link View#FOCUS_DOWN}
 *
 * @return whether selection was moved
 */
Boolean ListView::FullScroll(
    /* [in] */ Int32 direction)
{
    Boolean moved = FALSE;
    if (direction == FOCUS_UP) {
        if (mSelectedPosition != 0) {
            Int32 position = LookForSelectablePosition(0, TRUE);
            if (position >= 0) {
                mLayoutMode = LAYOUT_FORCE_TOP;
                SetSelectionInt(position);
                InvokeOnItemScrollListener();
            }
            moved = TRUE;
        }
    }
    else if (direction == FOCUS_DOWN) {
        if (mSelectedPosition < mItemCount - 1) {
            Int32 position = LookForSelectablePosition(mItemCount - 1, TRUE);
            if (position >= 0) {
                mLayoutMode = LAYOUT_FORCE_BOTTOM;
                SetSelectionInt(position);
                InvokeOnItemScrollListener();
            }
            moved = TRUE;
        }
    }

    if (moved && !AwakenScrollBars()) {
        AwakenScrollBars();
        Invalidate();
    }

    return moved;
}

/**
 * To avoid horizontal focus searches changing the selected item, we
 * manually focus search within the selected item (as applicable), and
 * prevent focus from jumping to something within another item.
 * @param direction one of {View::FOCUS_LEFT, View::FOCUS_RIGHT}
 * @return Whether this consumes the key event.
 */
Boolean ListView::HandleHorizontalFocusWithinListItem(
    /* [in] */ Int32 direction)
{
    if (direction != View::FOCUS_LEFT && direction != View::FOCUS_RIGHT)  {
        Logger::E("ListView", StringBuffer("direction must be one of")
            + " {View::FOCUS_LEFT, View::FOCUS_RIGHT}");
        assert(0);
    }

    Int32 numChildren = GetChildCount();
    if (mItemsCanFocus && numChildren > 0 &&
        mSelectedPosition != AdapterView_INVALID_POSITION) {
        AutoPtr<IView> selectedView = GetSelectedView();
        IViewGroup* vg = NULL;
        if (selectedView != NULL) {
            vg = (IViewGroup*)selectedView->Probe(EIID_IViewGroup);
        }
        if (vg != NULL &&
            ((View*)selectedView->Probe(EIID_View))->HasFocus()) {
            AutoPtr<IView> currentFocus;
            selectedView->FindFocus((IView**)&currentFocus);
            AutoPtr<IView> nextFocus = FocusFinder::GetInstance()
                ->FindNextFocus(vg, currentFocus, direction);
            if (nextFocus != NULL) {
                // do the math to Get interesting rect in next focus' coordinates
                currentFocus->GetFocusedRect(mTempRect);
                OffsetDescendantRectToMyCoords(currentFocus, mTempRect);
                OffsetRectIntoDescendantCoords(nextFocus, mTempRect);
                if (((View*)nextFocus->Probe(EIID_View))
                    ->RequestFocus(direction, mTempRect)) {
                    return TRUE;
                }
            }
            // we are blocking the key from being handled (by returning TRUE)
            // if the global result is going to be some other view within this
            // list.  this is to acheive the overall goal of having
            // horizontal d-pad navigation remain in the current item.
            AutoPtr<IView> globalNextFocus =
                FocusFinder::GetInstance()->FindNextFocus(
                    (IViewGroup*)GetRootView()->Probe(EIID_IViewGroup),
                    currentFocus, direction);
            if (globalNextFocus != NULL) {
                return IsViewAncestorOf(
                    globalNextFocus, (IView*)this->Probe(EIID_IView));
            }
        }
    }
    return FALSE;
}

/**
 * Scrolls to the next or previous item if possible.
 *
 * @param direction either {@link View#FOCUS_UP} or {@link View#FOCUS_DOWN}
 *
 * @return whether selection was moved
 */
Boolean ListView::ArrowScroll(
    /* [in] */ Int32 direction)
{
    mInLayout = TRUE;
    Boolean handled = ArrowScrollImpl(direction);
    if (handled) {
//        PlaySoundEffect(SoundEffectConstants.GetContantForFocusDirection(direction));
    }

    mInLayout = FALSE;
    return handled;
}

/**
 * Handle an arrow scroll going up or down.  Take into account whether items are selectable,
 * whether there are focusable items etc.
 *
 * @param direction Either {@link android.view.View#FOCUS_UP} or {@link android.view.View#FOCUS_DOWN}.
 * @return Whether any scrolling, selection or focus change occured.
 */
Boolean ListView::ArrowScrollImpl(
    /* [in] */ Int32 direction)
{
    if (GetChildCount() <= 0) {
        return FALSE;
    }

    AutoPtr<IView> selectedView = GetSelectedView();

    Int32 nextSelectedPosition = LookForSelectablePositionOnScreen(direction);
    Int32 amountToScroll = AmountToScroll(direction, nextSelectedPosition);

    // if we are moving focus, we may OVERRIDE the default behavior
    ArrowScrollFocusResult* focusResult =
        mItemsCanFocus ? ArrowScrollFocused(direction) : NULL;
    if (focusResult != NULL) {
        nextSelectedPosition = focusResult->GetSelectedPosition();
        amountToScroll = focusResult->GetAmountToScroll();
    }

    Boolean needToRedraw = focusResult != NULL;
    if (nextSelectedPosition != AdapterView_INVALID_POSITION) {
        HandleNewSelectionChange(
            selectedView, direction,
            nextSelectedPosition, focusResult != NULL);
        SetSelectedPositionInt(nextSelectedPosition);
        SetNextSelectedPositionInt(nextSelectedPosition);
        selectedView = GetSelectedView();
        if (mItemsCanFocus && focusResult == NULL) {
            // there was no new view found to take focus, make sure we
            // don't leave focus with the old selection
            AutoPtr<IView> focused = GetFocusedChild();
            if (focused != NULL) {
                focused->ClearFocus();
            }
        }
        needToRedraw = TRUE;
        CheckSelectionChanged();
    }

    if (amountToScroll > 0) {
        ScrollListItemsBy((direction == View::FOCUS_UP)
                    ? amountToScroll : -amountToScroll);
        needToRedraw = TRUE;
    }

    // if we didn't find a new focusable, make sure any existing focused
    // item that was panned off screen gives up focus.
    if (mItemsCanFocus && (focusResult == NULL)
        && selectedView != NULL &&
        ((View*)selectedView->Probe(EIID_View))->HasFocus()) {
        AutoPtr<IView> focused;
        selectedView->FindFocus((IView**)&focused);
        if (DistanceToView(focused) > 0) {
            focused->ClearFocus();
        }
    }

    // if  the current selection is panned off, we need to remove the selection
    if (nextSelectedPosition == AdapterView_INVALID_POSITION &&
        selectedView != NULL && !IsViewAncestorOf(
        selectedView, (IView*)this->Probe(EIID_IView))) {

        selectedView = NULL;
        HideSelector();

        // but we don't want to set the ressurect position (that would make subsequent
        // unhandled key events bring back the item we just scrolled off!)
        mResurrectToPosition = AdapterView_INVALID_POSITION;
    }

    if (needToRedraw) {
        if (selectedView != NULL) {
            PositionSelector(selectedView);
            selectedView->GetTop(&mSelectedTop);
        }

        if (!AwakenScrollBars()) {
            Invalidate();
        }

        InvokeOnItemScrollListener();
        return TRUE;
    }

    return FALSE;
}

/**
 * When selection changes, it is possible that the previously selected or the
 * next selected item will change its size.  If so, we need to offset some folks,
 * and re-layout the items as appropriate.
 *
 * @param selectedView The currently selected view (before changing selection).
 *   should be <code>NULL</code> if there was no previous selection.
 * @param direction Either {@link android.view.View#FOCUS_UP} or
 *        {@link android.view.View#FOCUS_DOWN}.
 * @param newSelectedPosition The position of the next selection.
 * @param newFocusAssigned whether new focus was assigned.  This matters because
 *        when something has focus, we don't want to show selection (ugh).
 */
ECode ListView::HandleNewSelectionChange(
    /* [in] */ IView* selectedView,
    /* [in] */ Int32 direction,
    /* [in] */ Int32 newSelectedPosition,
    /* [in] */ Boolean newFocusAssigned)
{
    if (newSelectedPosition == AdapterView_INVALID_POSITION) {
        Logger::E("ListView", "newSelectedPosition needs to be valid");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // whether or not we are moving down or up, we want to preserve the
    // top of whatever view is on top:
    // - moving down: the view that had selection
    // - moving up: the view that is Getting selection
    AutoPtr<IView> topView;
    AutoPtr<IView> bottomView;
    Int32 topViewIndex, bottomViewIndex;
    Boolean topSelected = FALSE;
    Int32 selectedIndex = mSelectedPosition - mFirstPosition;
    Int32 nextSelectedIndex = newSelectedPosition - mFirstPosition;
    if (direction == View::FOCUS_UP) {
        topViewIndex = nextSelectedIndex;
        bottomViewIndex = selectedIndex;
        topView = GetChildAt(topViewIndex);
        bottomView = selectedView;
        topSelected = TRUE;
    }
    else {
        topViewIndex = selectedIndex;
        bottomViewIndex = nextSelectedIndex;
        topView = selectedView;
        bottomView = GetChildAt(bottomViewIndex);
    }

    Int32 numChildren = GetChildCount();

    // start with top view: is it changing size?
    if (topView != NULL) {
        topView->SetSelected(!newFocusAssigned && topSelected);
        MeasureAndAdjustDown(topView, topViewIndex, numChildren);
    }

    // is the bottom view changing size?
    if (bottomView != NULL) {
        bottomView->SetSelected(!newFocusAssigned && !topSelected);
        MeasureAndAdjustDown(bottomView, bottomViewIndex, numChildren);
    }

    return NOERROR;
}

/**
 * Re-measure a child, and if its height changes, lay it out preserving its
 * top, and adjust the children below it appropriately.
 * @param child The child
 * @param childIndex The view group index of the child->
 * @param numChildren The number of children in the view group.
 */
void ListView::MeasureAndAdjustDown(
    /* [in] */ IView* child,
    /* [in] */ Int32 childIndex,
    /* [in] */ Int32 numChildren)
{
    View* childView = (View*)child->Probe(EIID_View);
    Int32 oldHeight;
    child->GetHeight(&oldHeight);
    MeasureItem(child);
    if (childView->GetMeasuredHeight() != oldHeight) {
        // lay out the view, preserving its top
        RelayoutMeasuredItem(child);

        // adjust views below appropriately
        Int32 heightDelta = childView->GetMeasuredHeight() - oldHeight;
        for (Int32 i = childIndex + 1; i < numChildren; i++) {
            GetChildAt(i)->OffsetTopAndBottom(heightDelta);
        }
    }
}

/**
 * Measure a particular list child->
 * TODO: unify with setUpChild.
 * @param child The child->
 */
void ListView::MeasureItem(
    /* [in] */ IView* child)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    if (lp == NULL) {
        ASSERT_SUCCEEDED(CViewGroupLayoutParams::New(
            ViewGroupLayoutParams_MATCH_PARENT,
            ViewGroupLayoutParams_WRAP_CONTENT,
            (IViewGroupLayoutParams**)&lp));
    }
    Int32 width, height;
    lp->GetWidth(&width);
    lp->GetHeight(&height);

    Int32 childWidthSpec = ViewGroup::GetChildMeasureSpec(
        mWidthMeasureSpec, mListPadding->mLeft + mListPadding->mRight, width);
    Int32 lpHeight = height;
    Int32 childHeightSpec;
    if (lpHeight > 0) {
        childHeightSpec = MeasureSpec::MakeMeasureSpec(lpHeight, MeasureSpec::EXACTLY);
    }
    else {
        childHeightSpec = MeasureSpec::MakeMeasureSpec(0, MeasureSpec::UNSPECIFIED);
    }
    child->Measure(childWidthSpec, childHeightSpec);
}

/**
 * Layout a child that has been measured, preserving its top position.
 * TODO: unify with setUpChild.
 * @param child The child->
 */
void ListView::RelayoutMeasuredItem(
    /* [in] */ IView* child)
{
    View* childView = (View*)child->Probe(EIID_View);
    Int32 w = childView->GetMeasuredWidth();
    Int32 h = childView->GetMeasuredHeight();
    Int32 childLeft = mListPadding->mLeft;
    Int32 childRight = childLeft + w;
    Int32 childTop = childView->GetTop();
    Int32 childBottom = childTop + h;
    child->Layout(childLeft, childTop, childRight, childBottom);
}

/**
 * @return The amount to preview next items when arrow srolling.
 */
Int32 ListView::GetArrowScrollPreviewLength()
{
    return Math::Max(MIN_SCROLL_PREVIEW_PIXELS, GetVerticalFadingEdgeLength());
}

/**
 * Determine how much we need to scroll in order to Get the next selected view
 * visible, with a fading edge showing below as applicable.  The amount is
 * capped at {@link #GetMaxScrollAmount()} .
 *
 * @param direction either {@link android.view.View#FOCUS_UP} or
 *        {@link android.view.View#FOCUS_DOWN}.
 * @param nextSelectedPosition The position of the next selection, or
 *        {@link #AdapterView_INVALID_POSITION} if there is no next selectable position
 * @return The amount to scroll. Note: this is always positive!  Direction
 *         needs to be taken into account when actually scrolling.
 */
Int32 ListView::AmountToScroll(
    /* [in] */ Int32 direction,
    /* [in] */ Int32 nextSelectedPosition)
{
    Int32 listBottom = GetHeight() - mListPadding->mBottom;
    Int32 listTop = mListPadding->mTop;
    Int32 numChildren = GetChildCount();

    if (direction == View::FOCUS_DOWN) {
        Int32 indexToMakeVisible = numChildren - 1;
        if (nextSelectedPosition != AdapterView_INVALID_POSITION) {
            indexToMakeVisible = nextSelectedPosition - mFirstPosition;
        }

        Int32 positionToMakeVisible = mFirstPosition + indexToMakeVisible;
        View* viewToMakeVisible =
            (View*)GetChildAt(indexToMakeVisible)->Probe(EIID_View);

        Int32 goalBottom = listBottom;
        if (positionToMakeVisible < mItemCount - 1) {
            goalBottom -= GetArrowScrollPreviewLength();
        }

        if (viewToMakeVisible->GetBottom() <= goalBottom) {
            // item is fully visible.
            return 0;
        }

        if (nextSelectedPosition != AdapterView_INVALID_POSITION
                && (goalBottom - viewToMakeVisible->GetTop()) >= GetMaxScrollAmount()) {
            // item already has enough of it visible, changing selection is good enough
            return 0;
        }

        Int32 amountToScroll = (viewToMakeVisible->GetBottom() - goalBottom);

        if ((mFirstPosition + numChildren) == mItemCount) {
            // last is last in list -> make sure we don't scroll past it
            Int32 max;
            GetChildAt(numChildren - 1)->GetBottom(&max);
            max -= listBottom;
            amountToScroll = Math::Min(amountToScroll, max);
        }

        return Math::Min(amountToScroll, GetMaxScrollAmount());
    }
    else {
        Int32 indexToMakeVisible = 0;
        if (nextSelectedPosition != AdapterView_INVALID_POSITION) {
            indexToMakeVisible = nextSelectedPosition - mFirstPosition;
        }
        Int32 positionToMakeVisible = mFirstPosition + indexToMakeVisible;
        View* viewToMakeVisible =
            (View*)GetChildAt(indexToMakeVisible)->Probe(EIID_View);

        Int32 goalTop = listTop;
        if (positionToMakeVisible > 0) {
            goalTop += GetArrowScrollPreviewLength();
        }

        if (viewToMakeVisible->GetTop() >= goalTop) {
            // item is fully visible.
            return 0;
        }

        if (nextSelectedPosition != AdapterView_INVALID_POSITION &&
            (viewToMakeVisible->GetBottom() - goalTop) >= GetMaxScrollAmount()) {
            // item already has enough of it visible, changing selection is good enough
            return 0;
        }

        Int32 amountToScroll = (goalTop - viewToMakeVisible->GetTop());
        if (mFirstPosition == 0) {
            // first is first in list -> make sure we don't scroll past it
            Int32 max;
            GetChildAt(0)->GetTop(&max);
            max = listTop - max;
            amountToScroll = Math::Min(amountToScroll, max);
        }
        return Math::Min(amountToScroll, GetMaxScrollAmount());
    }
}

/**
 * @param direction either {@link android.view.View#FOCUS_UP} or
 *        {@link android.view.View#FOCUS_DOWN}.
 * @return The position of the next selectable position of the views that
 *         are currently visible, taking into account the fact that there might
 *         be no selection.  Returns {@link #AdapterView_INVALID_POSITION} if there is no
 *         selectable view on screen in the given direction.
 */
Int32 ListView::LookForSelectablePositionOnScreen(
    /* [in] */ Int32 direction)
{
    Int32 firstPosition = mFirstPosition;
    if (direction == View::FOCUS_DOWN) {
        Int32 startPos = (mSelectedPosition != AdapterView_INVALID_POSITION) ?
                mSelectedPosition + 1 : firstPosition;
        Int32 count;
        mAdapter->GetCount(&count);
        if (startPos >= count) {
            return AdapterView_INVALID_POSITION;
        }

        if (startPos < firstPosition) {
            startPos = firstPosition;
        }

        Int32 lastVisiblePos = GetLastVisiblePosition();
        AutoPtr<IListAdapter> adapter = (IListAdapter*)GetAdapter().Get();
        for (Int32 pos = startPos; pos <= lastVisiblePos; pos++) {
            Boolean isEnabled;
            adapter->IsEnabled(pos, &isEnabled);
            Int32 visibility;
            GetChildAt(pos - firstPosition)->GetVisibility(&visibility);
            if (isEnabled && visibility == View::VISIBLE) {
                return pos;
            }
        }
    }
    else {
        Int32 last = firstPosition + GetChildCount() - 1;
        Int32 startPos = (mSelectedPosition != AdapterView_INVALID_POSITION) ?
                mSelectedPosition - 1 : firstPosition + GetChildCount() - 1;
        if (startPos < 0) {
            return AdapterView_INVALID_POSITION;
        }

        if (startPos > last) {
            startPos = last;
        }

        AutoPtr<IListAdapter> adapter = (IListAdapter*)GetAdapter().Get();
        for (Int32 pos = startPos; pos >= firstPosition; pos--) {
            Boolean isEnabled;
            adapter->IsEnabled(pos, &isEnabled);
            Int32 visibility;
            GetChildAt(pos - firstPosition)->GetVisibility(&visibility);
            if (isEnabled && visibility == View::VISIBLE) {
                return pos;
            }
        }
    }
    return AdapterView_INVALID_POSITION;
}

/**
 * Do an arrow scroll based on focus searching.  If a new view is
 * given focus, return the selection delta and amount to scroll via
 * an {@link ArrowScrollFocusResult}, otherwise, return NULL.
 *
 * @param direction either {@link android.view.View#FOCUS_UP} or
 *        {@link android.view.View#FOCUS_DOWN}.
 * @return The result if focus has changed, or <code>NULL</code>.
 */
ListView::ArrowScrollFocusResult* ListView::ArrowScrollFocused(
    /* [in] */ Int32 direction)
{
    AutoPtr<IView> selectedView = GetSelectedView();
    AutoPtr<IView> newFocus;
    if (selectedView != NULL &&
        ((View*)selectedView->Probe(EIID_View))->HasFocus()) {
        AutoPtr<IView> oldFocus;
        selectedView->FindFocus((IView**)&oldFocus);
        newFocus = FocusFinder::GetInstance()->FindNextFocus(
            (IViewGroup*)this->Probe(EIID_IViewGroup), oldFocus, direction);
    }
    else {
        if (direction == View::FOCUS_DOWN) {
            Boolean topFadingEdgeShowing = (mFirstPosition > 0);
            Int32 listTop = mListPadding->mTop +
                (topFadingEdgeShowing ? GetArrowScrollPreviewLength() : 0);
            Int32 ySearchPoint = listTop;
            if (selectedView != NULL) {
                selectedView->GetTop(&ySearchPoint);
                ySearchPoint = Math::Max(ySearchPoint, listTop);
            }
            mTempRect->Set(0, ySearchPoint, 0, ySearchPoint);
        }
        else {
            Boolean bottomFadingEdgeShowing =
                (mFirstPosition + GetChildCount() - 1) < mItemCount;
            Int32 listBottom = GetHeight() - mListPadding->mBottom -
                (bottomFadingEdgeShowing ? GetArrowScrollPreviewLength() : 0);

            Int32 ySearchPoint = listBottom;
            if (selectedView != NULL) {
                selectedView->GetBottom(&ySearchPoint);
                ySearchPoint = Math::Min(ySearchPoint, listBottom);
            }
            mTempRect->Set(0, ySearchPoint, 0, ySearchPoint);
        }
        newFocus = FocusFinder::GetInstance()->FindNextFocusFromRect(
            (IViewGroup*)this->Probe(EIID_IViewGroup), mTempRect, direction);
    }

    if (newFocus != NULL) {
        Int32 positionOfNewFocus = PositionOfNewFocus(newFocus);

        // if the focus change is in a different new position, make sure
        // we aren't jumping over another selectable position
        if (mSelectedPosition != AdapterView_INVALID_POSITION
            && positionOfNewFocus != mSelectedPosition)
        {
            Int32 selectablePosition = LookForSelectablePositionOnScreen(direction);
            if (selectablePosition != AdapterView_INVALID_POSITION &&
                ((direction == View::FOCUS_DOWN &&
                selectablePosition < positionOfNewFocus) ||
                (direction == View::FOCUS_UP &&
                selectablePosition > positionOfNewFocus))) {
                return NULL;
            }
        }

        Int32 focusScroll = AmountToScrollToNewFocus(
            direction, newFocus, positionOfNewFocus);

        Int32 maxScrollAmount = GetMaxScrollAmount();
        if (focusScroll < maxScrollAmount) {
            // not moving too far, safe to give next view focus
            Boolean result;
            newFocus->RequestFocusEx(direction, &result);
            mArrowScrollFocusResult->Populate(positionOfNewFocus, focusScroll);
            return mArrowScrollFocusResult;
        }
        else if (DistanceToView(newFocus) < maxScrollAmount) {
            // Case to consider:
            // too far to Get entire next focusable on screen, but by going
            // max scroll amount, we are Getting it at least partially in view,
            // so give it focus and scroll the max ammount.
            //
            Boolean result;
            newFocus->RequestFocusEx(direction, &result);
            mArrowScrollFocusResult->Populate(positionOfNewFocus, maxScrollAmount);
            return mArrowScrollFocusResult;
        }
    }
    return NULL;
}

/**
 * @param newFocus The view that would have focus.
 * @return the position that contains newFocus
 */
Int32 ListView::PositionOfNewFocus(
    /* [in] */ IView* newFocus)
{
    Int32 numChildren = GetChildCount();
    for (Int32 i = 0; i < numChildren; i++) {
        AutoPtr<IView> child = GetChildAt(i);
        if (IsViewAncestorOf(newFocus, child)) {
            return mFirstPosition + i;
        }
    }
    Logger::E("ListView",
        "newFocus is not a child of any of the children of the list!");
    assert(0);
    return -1;
}

/**
 * Return TRUE if child is an ancestor of parent, (or equal to the parent).
 */
Boolean ListView::IsViewAncestorOf(
    /* [in] */ IView* child,
    /* [in] */ IView* parent)
{
    if (child == parent) {
        return TRUE;
    }

    AutoPtr<IViewParent> theParent;
    child->GetParent((IViewParent**)&theParent);
    return theParent->Probe(EIID_IViewGroup) &&
        IsViewAncestorOf((IView*)theParent->Probe(EIID_IView), parent);
}

/**
 * Determine how much we need to scroll in order to Get newFocus in view.
 * @param direction either {@link android.view.View#FOCUS_UP} or
 *        {@link android.view.View#FOCUS_DOWN}.
 * @param newFocus The view that would take focus.
 * @param positionOfNewFocus The position of the list item containing newFocus
 * @return The amount to scroll.  Note: this is always positive!  Direction
 *   needs to be taken into account when actually scrolling.
 */
Int32 ListView::AmountToScrollToNewFocus(
    /* [in] */ Int32 direction,
    /* [in] */ IView* newFocus,
    /* [in] */ Int32 positionOfNewFocus)
{
    Int32 amountToScroll = 0;
    newFocus->GetDrawingRect(mTempRect);
    OffsetDescendantRectToMyCoords(newFocus, mTempRect);
    if (direction == View::FOCUS_UP) {
        if (mTempRect->mTop < mListPadding->mTop) {
            amountToScroll = mListPadding->mTop - mTempRect->mTop;
            if (positionOfNewFocus > 0) {
                amountToScroll += GetArrowScrollPreviewLength();
            }
        }
    }
    else {
        Int32 listBottom = GetHeight() - mListPadding->mBottom;
        if (mTempRect->mBottom > listBottom) {
            amountToScroll = mTempRect->mBottom - listBottom;
            if (positionOfNewFocus < mItemCount - 1) {
                amountToScroll += GetArrowScrollPreviewLength();
            }
        }
    }
    return amountToScroll;
}

/**
 * Determine the distance to the nearest edge of a view in a particular
 * direction.
 *
 * @param descendant A descendant of this list.
 * @return The distance, or 0 if the nearest edge is already on screen.
 */
Int32 ListView::DistanceToView(
    /* [in] */ IView* descendant)
{
    Int32 distance = 0;
    descendant->GetDrawingRect(mTempRect);
    OffsetDescendantRectToMyCoords(descendant, mTempRect);
    Int32 listBottom = mBottom - mTop - mListPadding->mBottom;
    if (mTempRect->mBottom < mListPadding->mTop) {
        distance = mListPadding->mTop - mTempRect->mBottom;
    }
    else if (mTempRect->mTop > listBottom) {
        distance = mTempRect->mTop - listBottom;
    }

    return distance;
}


/**
 * Scroll the children by amount, adding a view at the end and removing
 * views that fall off as necessary.
 *
 * @param amount The amount (positive or negative) to scroll.
 */
void ListView::ScrollListItemsBy(
    /* [in] */ Int32 amount)
{
    OffsetChildrenTopAndBottom(amount);

    Int32 listBottom = GetHeight() - mListPadding->mBottom;
    Int32 listTop = mListPadding->mTop;
    RecycleBin* recycleBin = mRecycler;

    if (amount < 0) {
        // shifted items up

        // may need to pan views into the bottom space
        Int32 numChildren = GetChildCount();
        AutoPtr<IView> last = GetChildAt(numChildren - 1);
        Int32 bottom;
        last->GetBottom(&bottom);
        while (bottom < listBottom) {
            Int32 lastVisiblePosition = mFirstPosition + numChildren - 1;
            if (lastVisiblePosition < mItemCount - 1) {
                last = AddViewBelow(last, lastVisiblePosition);
                numChildren++;
            }
            else {
                break;
            }
        }

        // may have brought in the last child of the list that is skinnier
        // than the fading edge, thereby leaving space at the end.  need
        // to shift back
        if (bottom < listBottom) {
            OffsetChildrenTopAndBottom(listBottom - bottom);
        }

        // top views may be panned off screen
        AutoPtr<IView> first = GetChildAt(0);
        while (((View*)first->Probe(EIID_View))->GetBottom() < listTop) {
            AutoPtr<IAbsListViewLayoutParams> layoutParams;
            first->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
            if (recycleBin->ShouldRecycleViewType(
                ((CAbsListViewLayoutParams*)layoutParams.Get())->mViewType)) {
                DetachViewFromParent(first);
                recycleBin->AddScrapView(first);
            }
            else {
                RemoveViewInLayout(first);
            }
            first = GetChildAt(0);
            mFirstPosition++;
        }
    }
    else {
        // shifted items down
        AutoPtr<IView> first = GetChildAt(0);

        Int32 top;
        first->GetTop(&top);
        // may need to pan views into top
        while ((top > listTop) && (mFirstPosition > 0)) {
            first = AddViewAbove(first, mFirstPosition);
            mFirstPosition--;
        }

        // may have brought the very first child of the list in too far and
        // need to shift it back
        if (top > listTop) {
            OffsetChildrenTopAndBottom(listTop - top);
        }

        Int32 lastIndex = GetChildCount() - 1;
        AutoPtr<IView> last = GetChildAt(lastIndex);

        // bottom view may be panned off screen
        while (((View*)last->Probe(EIID_View))->GetTop() > listBottom) {
            AutoPtr<IAbsListViewLayoutParams> layoutParams;
            last->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
            if (recycleBin->ShouldRecycleViewType(
                ((CAbsListViewLayoutParams*)layoutParams.Get())->mViewType)) {
                DetachViewFromParent(last);
                recycleBin->AddScrapView(last);
            }
            else {
                RemoveViewInLayout(last);
            }
            last = GetChildAt(--lastIndex);
        }
    }
}

AutoPtr<IView> ListView::AddViewAbove(
    /* [in] */ IView* theView,
    /* [in] */ Int32 position)
{
    Int32 abovePosition = position - 1;
    AutoPtr<IView> view = ObtainView(abovePosition, mIsScrap);
    Int32 edgeOfNewChild;
    theView->GetTop(&edgeOfNewChild);
    edgeOfNewChild -= mDividerHeight;
    SetupChild(
        view, abovePosition, edgeOfNewChild, FALSE,
        mListPadding->mLeft,FALSE, mIsScrap[0]);

    return view;
}

AutoPtr<IView> ListView::AddViewBelow(
    /* [in] */ IView* theView,
    /* [in] */ Int32 position)
{
    Int32 belowPosition = position + 1;
    AutoPtr<IView> view = ObtainView(belowPosition, mIsScrap);
    Int32 edgeOfNewChild;
    theView->GetBottom(&edgeOfNewChild);
    edgeOfNewChild += mDividerHeight;
    SetupChild(
        view, belowPosition, edgeOfNewChild, TRUE,
        mListPadding->mLeft, FALSE, mIsScrap[0]);

    return view;
}

/**
 * Indicates that the views created by the ListAdapter can contain focusable
 * items.
 *
 * @param itemsCanFocus TRUE if items can Get focus, FALSE otherwise
 */
ECode ListView::SetItemsCanFocus(
    /* [in] */ Boolean itemsCanFocus)
{
    mItemsCanFocus = itemsCanFocus;
    if (!itemsCanFocus) {
        SetDescendantFocusability(ViewGroup::FOCUS_BLOCK_DESCENDANTS);
    }

    return NOERROR;
}

/**
 * @return Whether the views created by the ListAdapter can contain focusable
 * items.
 */
Boolean ListView::GetItemsCanFocus()
{
    return mItemsCanFocus;
}

/**
 * @hide Pending API council approval.
 */
//@Override
Boolean ListView::IsOpaque()
{
    return (mCachingStarted && mIsCacheColorOpaque && mDividerIsOpaque &&
            HasOpaqueScrollbars()) || AbsListView::IsOpaque();
}

//@Override
ECode ListView::SetCacheColorHint(
    /* [in] */ Int32 color)
{
    Boolean opaque = ((color >> 24) & 0xFF) == 0xFF;
    mIsCacheColorOpaque = opaque;
    if (opaque) {
        if (mDividerPaint == NULL) {
            ASSERT_SUCCEEDED(CPaint::New((IPaint**)&mDividerPaint));
        }
        mDividerPaint->SetColor(color);
    }

    return AbsListView::SetCacheColorHint(color);
}

void ListView::DrawOverscrollHeader(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IDrawable* drawable,
    /* [in] */ IRect* bounds)
{
    CRect* boundsRect = (CRect*)bounds;
    Int32 height;
    drawable->GetMinimumHeight(&height);

    Int32 count;
    canvas->Save(&count);
    Boolean result;
    canvas->ClipRectEx3(boundsRect, &result);

    Int32 span = boundsRect->mBottom - boundsRect->mTop;
    if (span < height) {
        boundsRect->mTop = boundsRect->mBottom - height;
    }

    drawable->SetBoundsEx(boundsRect);
    drawable->Draw(canvas);

    canvas->Restore();
}

void ListView::DrawOverscrollFooter(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IDrawable* drawable,
    /* [in] */ IRect* bounds)
{
    CRect* boundsRect = (CRect*)bounds;
    Int32 height;
    drawable->GetMinimumHeight(&height);

    Int32 count;
    canvas->Save(&count);
    Boolean result;
    canvas->ClipRectEx3(boundsRect, &result);

    Int32 span = boundsRect->mBottom - boundsRect->mTop;
    if (span < height) {
        boundsRect->mBottom = boundsRect->mTop + height;
    }

    drawable->SetBoundsEx(boundsRect);
    drawable->Draw(canvas);

    canvas->Restore();
}

//@Override
void ListView::DispatchDraw(
    /* [in] */ ICanvas* canvas)
{
    // Draw the dividers
    Int32 dividerHeight = mDividerHeight;
    AutoPtr<IDrawable> overscrollHeader = mOverScrollHeader;
    AutoPtr<IDrawable> overscrollFooter = mOverScrollFooter;
    Boolean drawOverscrollHeader = overscrollHeader != NULL;
    Boolean drawOverscrollFooter = overscrollFooter != NULL;
    Boolean drawDividers = dividerHeight > 0 && mDivider != NULL;

    if (drawDividers || drawOverscrollHeader || drawOverscrollFooter) {
        // Only modify the top and bottom in the loop, we set the left and right here
        CRect* bounds = mTempRect;
        bounds->mLeft = mPaddingLeft;
        bounds->mRight = mRight - mLeft - mPaddingRight;

        Int32 count = GetChildCount();
        Int32 headerCount = mHeaderViewInfos.GetSize();
        Int32 itemCount = mItemCount;
        Int32 footerLimit = itemCount - mFooterViewInfos.GetSize() - 1;
        Boolean headerDividers = mHeaderDividersEnabled;
        Boolean footerDividers = mFooterDividersEnabled;
        Int32 first = mFirstPosition;
        Boolean areAllItemsSelectable = mAreAllItemsSelectable;
        AutoPtr<IListAdapter> adapter = mAdapter;
        // If the list is opaque *and* the background is not, we want to
        // fill a rect where the dividers would be for non-selectable items
        // If the list is opaque and the background is also opaque, we don't
        // need to draw anything since the background will do it for us
        Boolean fillForMissingDividers = drawDividers && IsOpaque()
            && !AbsListView::IsOpaque();

        if (fillForMissingDividers && mDividerPaint == NULL && mIsCacheColorOpaque) {
            mDividerPaint = NULL;
            ASSERT_SUCCEEDED(CPaint::New((IPaint**)&mDividerPaint));
            mDividerPaint->SetColor(GetCacheColorHint());
        }
        IPaint* paint = mDividerPaint;

        Int32 listBottom = mBottom - mTop - mListPadding->mBottom + mScrollY;
        if (!mStackFromBottom) {
            Int32 bottom = 0;
            // Draw top divider or header for overscroll
            Int32 scrollY = mScrollY;
            if (count > 0 && scrollY < 0) {
                if (drawOverscrollHeader) {
                    bounds->mBottom = 0;
                    bounds->mTop = scrollY;
                    DrawOverscrollHeader(canvas, overscrollHeader, bounds);
                }
                else if (drawDividers) {
                    bounds->mBottom = 0;
                    bounds->mTop = -dividerHeight;
                    DrawDivider(canvas, bounds, -1);
                }
            }

            for (Int32 i = 0; i < count; i++) {
                if ((headerDividers || first + i >= headerCount) &&
                    (footerDividers || first + i < footerLimit)) {
                    AutoPtr<IView> child = GetChildAt(i);
                    child->GetBottom(&bottom);
                    // Don't draw dividers next to items that are not enabled
                    if (drawDividers &&
                        (bottom < listBottom && !(drawOverscrollFooter && i == count - 1))) {
                        Boolean isEnabled1, isEnabled2;
                        adapter->IsEnabled(first + i, &isEnabled1);
                        adapter->IsEnabled(first + i + 1, &isEnabled2);
                        if (areAllItemsSelectable ||
                            (isEnabled1 && (i == count - 1 ||isEnabled2))) {
                            bounds->mTop = bottom;
                            bounds->mBottom = bottom + dividerHeight;
                            DrawDivider(canvas, bounds, i);
                        }
                        else if (fillForMissingDividers) {
                            bounds->mTop = bottom;
                            bounds->mBottom = bottom + dividerHeight;
                            canvas->DrawRectEx(bounds, paint);
                        }
                    }
                }
            }

            Int32 overFooterBottom = mBottom + mScrollY;
            if (drawOverscrollFooter && first + count == itemCount &&
                overFooterBottom > bottom) {
                bounds->mTop = bottom;
                bounds->mBottom = overFooterBottom;
                DrawOverscrollFooter(canvas, overscrollFooter, bounds);
            }
        }
        else {
            Int32 top;
            Int32 listTop = mListPadding->mTop;
            Int32 scrollY = mScrollY;

            if (count > 0 && drawOverscrollHeader) {
                bounds->mTop = scrollY;
                GetChildAt(0)->GetTop(&bounds->mBottom);
                DrawOverscrollHeader(canvas, overscrollHeader, bounds);
            }

            Int32 start = drawOverscrollHeader ? 1 : 0;
            for (Int32 i = start; i < count; i++) {
                if ((headerDividers || first + i >= headerCount) &&
                    (footerDividers || first + i < footerLimit)) {
                    AutoPtr<IView> child = GetChildAt(i);
                    child->GetTop(&top);
                    // Don't draw dividers next to items that are not enabled
                    if (drawDividers && top > listTop) {
                        Boolean isEnabled1, isEnabled2;
                        adapter->IsEnabled(first + i, &isEnabled1);
                        adapter->IsEnabled(first + i + 1, &isEnabled2);
                        if (areAllItemsSelectable ||
                            (isEnabled1 && (i == count - 1 ||isEnabled2))) {
                            bounds->mTop = top - dividerHeight;
                            bounds->mBottom = top;
                            // Give the method the child ABOVE the divider, so we
                            // subtract one from our child
                            // position. Give -1 when there is no child above the
                            // divider.
                            DrawDivider(canvas, bounds, i - 1);
                        }
                        else if (fillForMissingDividers) {
                            bounds->mTop = top - dividerHeight;
                            bounds->mBottom = top;
                            canvas->DrawRectEx(bounds, paint);
                        }
                    }
                }
            }

            if (count > 0 && scrollY > 0) {
                if (drawOverscrollFooter) {
                    Int32 absListBottom = mBottom;
                    bounds->mTop = absListBottom;
                    bounds->mBottom = absListBottom + scrollY;
                    DrawOverscrollFooter(canvas, overscrollFooter, bounds);
                }
                else if (drawDividers) {
                    bounds->mTop = listBottom;
                    bounds->mBottom = listBottom + dividerHeight;
                    DrawDivider(canvas, bounds, -1);
                }
            }
        }
    }

    // Draw the indicators (these should be drawn above the dividers) and children
    AbsListView::DispatchDraw(canvas);
}

/**
 * Draws a divider for the given child in the given bounds.
 *
 * @param canvas The canvas to draw to.
 * @param bounds The bounds of the divider.
 * @param childIndex The index of child (of the View) above the divider.
 *            This will be -1 if there is no child above the divider to be
 *            drawn.
 */
void ListView::DrawDivider(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* bounds,
    /* [in] */ Int32 childIndex)
{
    assert(mDivider != NULL);
    // This widGet draws the same divider for all children
    AutoPtr<IDrawable> divider = mDivider;
    Boolean clipDivider = mClipDivider;

    if (!clipDivider) {
        divider->SetBoundsEx(bounds);
    }
    else {
        Int32 count;
        canvas->Save(&count);
        Boolean result;
        canvas->ClipRectEx3(bounds, &result);
    }

    divider->Draw(canvas);

    if (clipDivider) {
        canvas->Restore();
    }
}

/**
 * Returns the drawable that will be drawn between each item in the list.
 *
 * @return the current drawable drawn between list elements
 */
AutoPtr<IDrawable> ListView::GetDivider()
{
    return mDivider;
}

/**
 * Sets the drawable that will be drawn between each item in the list. If the drawable does
 * not have an intrinsic height, you should also call {@link #setDividerHeight(Int32)}
 *
 * @param divider The drawable to use.
 */
ECode ListView::SetDivider(
    /* [in] */ IDrawable* divider)
{
    if (divider != NULL) {
        divider->GetIntrinsicHeight(&mDividerHeight);
        mClipDivider = divider->Probe(EIID_IColorDrawable) != NULL;
        Int32 opacity;
        divider->GetOpacity(&opacity);
        mDividerIsOpaque = opacity == ElPixelFormat::OPAQUE;
    }
    else {
        mDividerHeight = 0;
        mClipDivider = FALSE;
        mDividerIsOpaque = TRUE;
    }
    mDivider = divider;
    RequestLayoutIfNecessary();

    return NOERROR;
}

/**
 * @return Returns the height of the divider that will be drawn between each item in the list.
 */
Int32 ListView::GetDividerHeight()
{
    return mDividerHeight;
}

/**
 * Sets the height of the divider that will be drawn between each item in the list. Calling
 * this will override the intrinsic height as set by {@link #setDivider(Drawable)}
 *
 * @param height The new height of the divider in pixels.
 */
ECode ListView::SetDividerHeight(
    /* [in] */ Int32 height)
{
    mDividerHeight = height;
    RequestLayoutIfNecessary();

    return NOERROR;
}

/**
 * Enables or disables the drawing of the divider for header views.
 *
 * @param headerDividersEnabled True to draw the headers, FALSE otherwise.
 *
 * @see #setFooterDividersEnabled(Boolean)
 * @see #addHeaderView(android.view.View)
 */
ECode ListView::SetHeaderDividersEnabled(
    /* [in] */ Boolean headerDividersEnabled)
{
    mHeaderDividersEnabled = headerDividersEnabled;
    Invalidate();

    return NOERROR;
}

/**
 * Enables or disables the drawing of the divider for footer views.
 *
 * @param footerDividersEnabled True to draw the footers, FALSE otherwise.
 *
 * @see #setHeaderDividersEnabled(Boolean)
 * @see #addFooterView(android.view.View)
 */
ECode ListView::SetFooterDividersEnabled(
    /* [in] */ Boolean footerDividersEnabled)
{
    mFooterDividersEnabled = footerDividersEnabled;
    Invalidate();

    return NOERROR;
}

/**
 * Sets the drawable that will be drawn above all other list content.
 * This area can become visible when the user overscrolls the list.
 *
 * @param header The drawable to use
 */
ECode ListView::SetOverscrollHeader(
    /* [in] */ IDrawable* header)
{
    mOverScrollHeader = header;
    if (mScrollY < 0) {
        Invalidate();
    }

    return NOERROR;
}

/**
 * @return The drawable that will be drawn above all other list content
 */
AutoPtr<IDrawable> ListView::GetOverscrollHeader()
{
    return mOverScrollHeader;
}

/**
 * Sets the drawable that will be drawn below all other list content.
 * This area can become visible when the user overscrolls the list,
 * or when the list's content does not fully fill the container area.
 *
 * @param footer The drawable to use
 */
ECode ListView::SetOverscrollFooter(
    /* [in] */ IDrawable* footer)
{
    mOverScrollFooter = footer;
    Invalidate();

    return NOERROR;
}

/**
 * @return The drawable that will be drawn below all other list content
 */
AutoPtr<IDrawable> ListView::GetOverscrollFooter()
{
    return mOverScrollFooter;
}

//@Override
void ListView::OnFocusChanged(
    /* [in] */ Boolean gainFocus,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    AbsListView::OnFocusChanged(gainFocus, direction, previouslyFocusedRect);

    Int32 closetChildIndex = -1;
    if (gainFocus && previouslyFocusedRect != NULL) {
        previouslyFocusedRect->Offset(mScrollX, mScrollY);

        AutoPtr<IListAdapter> adapter = mAdapter;
        Int32 count;
        adapter->GetCount(&count);
        // Don't cache the result of GetChildCount or mFirstPosition here,
        // it could change in layoutChildren.
        if (count < GetChildCount() + mFirstPosition) {
            mLayoutMode = LAYOUT_NORMAL;
            LayoutChildren();
        }

        // figure out which item should be selected based on previously
        // focused rect
        CRect* otherRect = mTempRect;
        Int32 minDistance = Math::INT32_MAX_VALUE;
        Int32 childCount = GetChildCount();
        Int32 firstPosition = mFirstPosition;

        for (Int32 i = 0; i < childCount; i++) {
            // only consider selectable views
            Boolean isEnabled;
            adapter->IsEnabled(firstPosition + i, &isEnabled);
            if (!isEnabled) {
                continue;
            }

            AutoPtr<IView> other = GetChildAt(i);
            other->GetDrawingRect(otherRect);
            OffsetDescendantRectToMyCoords(other, otherRect);
            Int32 distance = GetDistance(previouslyFocusedRect, otherRect, direction);

            if (distance < minDistance) {
                minDistance = distance;
                closetChildIndex = i;
            }
        }
    }

    if (closetChildIndex >= 0) {
        SetSelection(closetChildIndex + mFirstPosition);
    }
    else {
        RequestLayout();
    }
}

/*
 * (non-Javadoc)
 *
 * Children specified in XML are assumed to be header views. After we have
 * parsed them move them out of the children list and into mHeaderViews.
 */
//@Override
ECode ListView::OnFinishInflate()
{
    AbsListView::OnFinishInflate();

    Int32 count = GetChildCount();
    if (count > 0) {
        for (Int32 i = 0; i < count; ++i) {
            AddHeaderView(GetChildAt(i));
        }
        RemoveAllViews();
    }

    return NOERROR;
}

/* (non-Javadoc)
 * @see android.view.View#findViewById(Int32)
 * First look in our children, then in any header and footer views that may be scrolled off.
 */
//@Override
AutoPtr<IView> ListView::FindViewTraversal(
    /* [in] */ Int32 id)
{
    AutoPtr<IView> v = AbsListView::FindViewTraversal(id);
    if (v == NULL) {
        v = FindViewInHeadersOrFooters(mHeaderViewInfos, id);
        if (v != NULL) {
            return v;
        }

        v = FindViewInHeadersOrFooters(mFooterViewInfos, id);
        if (v != NULL) {
            return v;
        }
    }
    return v;
}

/* (non-Javadoc)
 *
 * Look in the passed in list of headers or footers for the view.
 */
AutoPtr<IView> ListView::FindViewInHeadersOrFooters(
    /* [in] */ Vector<AutoPtr<FixedViewInfo> >& where,
    /* [in] */ Int32 id)
{
    Vector<AutoPtr<FixedViewInfo> >::Iterator iter;
    for (iter = where.Begin(); iter != where.End(); ++iter) {
        View* v = (View*)(*iter)->mView->Probe(EIID_View);
        if (!v->IsRootNamespace()) {
            AutoPtr<IView> view = v->FindViewById(id);
            if (view != NULL) {
                return view;
            }
        }
    }

    return NULL;
}

/* (non-Javadoc)
 * @see android.view.View#findViewWithTag(String)
 * First look in our children, then in any header and footer views that may be scrolled off.
 */
//@Override
AutoPtr<IView> ListView::FindViewWithTagTraversal(
    /* [in] */ IInterface* tag)
{
    AutoPtr<IView> v = AbsListView::FindViewWithTagTraversal(tag);
    if (v == NULL) {
        v = FindViewTagInHeadersOrFooters(mHeaderViewInfos, tag);
        if (v != NULL) {
            return v;
        }

        v = FindViewTagInHeadersOrFooters(mFooterViewInfos, tag);
        if (v != NULL) {
            return v;
        }
    }
    return v;
}

/* (non-Javadoc)
 *
 * Look in the passed in list of headers or footers for the view with the tag.
 */
AutoPtr<IView> ListView::FindViewTagInHeadersOrFooters(
    /* [in] */ Vector<AutoPtr<FixedViewInfo> >& where,
    /* [in] */ IInterface* tag)
{
    Vector<AutoPtr<FixedViewInfo> >::Iterator iter;
    for (iter = where.Begin(); iter != where.End(); ++iter) {
        View* v = (View*)(*iter)->mView->Probe(EIID_View);
        if (!v->IsRootNamespace()) {
            AutoPtr<IView> view = v->FindViewWithTag(tag);
            if (view != NULL) {
                return view;
            }
        }
    }

    return NULL;
}

//@Override
Boolean ListView::OnTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    Int32 action, edgeFlags;
    ev->GetAction(&action);
    ev->GetEdgeFlags(&edgeFlags);
    if (mItemsCanFocus && action == MotionEvent_ACTION_DOWN && edgeFlags != 0) {
        // Don't handle edge touches immediately -- they may actually belong to one of our
        // descendants.
        return FALSE;
    }

    return AbsListView::OnTouchEvent(ev);
}

/**
 * @see #setChoiceMode(Int32)
 *
 * @return The current choice mode
 */
Int32 ListView::GetChoiceMode()
{
    return mChoiceMode;
}

/**
 * Defines the choice behavior for the List. By default, Lists do not have any choice behavior
 * ({@link #CHOICE_MODE_NONE}). By setting the choiceMode to {@link #CHOICE_MODE_SINGLE}, the
 * List allows up to one item to  be in a chosen state. By setting the choiceMode to
 * {@link #CHOICE_MODE_MULTIPLE}, the list allows any number of items to be chosen.
 *
 * @param choiceMode One of {@link #CHOICE_MODE_NONE}, {@link #CHOICE_MODE_SINGLE}, or
 * {@link #CHOICE_MODE_MULTIPLE}
 */
ECode ListView::SetChoiceMode(
    /* [in] */ Int32 choiceMode)
{
    mChoiceMode = choiceMode;
    if (mChoiceMode != CHOICE_MODE_NONE) {
        if (mCheckStates == NULL) {
            mCheckStates = new HashMap<Int32, Boolean>();
        }

        Boolean hasStableIds = FALSE;
        if (mAdapter != NULL) {
            mAdapter->HasStableIds(&hasStableIds);
        }

        if (mCheckedIdStates == NULL && hasStableIds) {
            mCheckedIdStates = new HashMap<Int64, Boolean>();
        }
    }
    return NOERROR;
}

//@Override
Boolean ListView::PerformItemClick(
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    Boolean handled = FALSE;

    if (mChoiceMode != CHOICE_MODE_NONE) {
        handled = TRUE;

        Boolean newValue = TRUE;
        if (mCheckStates->Find(position) != mCheckStates->End()) {
            newValue = !(*mCheckStates)[position];
        }
        Boolean hasStableIds;
        mAdapter->HasStableIds(&hasStableIds);
        Int64 id;
        mAdapter->GetItemId(position, &id);

        if (mChoiceMode == CHOICE_MODE_MULTIPLE) {
            (*mCheckStates)[position] = newValue;
            if (mCheckedIdStates != NULL && hasStableIds) {
                if (newValue) {
                    (*mCheckedIdStates)[id] = TRUE;
                }
                else {
                    mCheckedIdStates->Erase(id);
                }
            }
        }
        else {
            if (newValue) {
                mCheckStates->Clear();
                (*mCheckStates)[position] = TRUE;
                if (mCheckedIdStates != NULL && hasStableIds) {
                    mCheckedIdStates->Clear();
                    (*mCheckedIdStates)[id] = TRUE;
                }
            }
        }

        mDataChanged = TRUE;
        RememberSyncState();
        RequestLayout();
    }

    handled |= AbsListView::PerformItemClick(view, position, id);

    return handled;
}

/**
 * Sets the checked state of the specified position. The is only valid if
 * the choice mode has been set to {@link #CHOICE_MODE_SINGLE} or
 * {@link #CHOICE_MODE_MULTIPLE}.
 *
 * @param position The item whose checked state is to be checked
 * @param value The new checked state for the item
 */
ECode ListView::SetItemChecked(
    /* [in] */ Int32 position,
    /* [in] */ Boolean value)
{
    if (mChoiceMode == CHOICE_MODE_NONE) {
        return NOERROR;
    }

    Boolean hasStableIds;
    mAdapter->HasStableIds(&hasStableIds);
    Int64 id;
    mAdapter->GetItemId(position, &id);
    if (mChoiceMode == CHOICE_MODE_MULTIPLE) {
        (*mCheckStates)[position] = value;
        if (mCheckedIdStates != NULL && hasStableIds) {
            if (value) {
                (*mCheckedIdStates)[id] = TRUE;
            }
            else {
                mCheckedIdStates->Erase(id);
            }
        }
    }
    else {
        Boolean updateIds = mCheckedIdStates != NULL && hasStableIds;
        // Clear all values if we're checking something, or unchecking the currently
        // selected item
        if (value || IsItemChecked(position)) {
            mCheckStates->Clear();
            if (updateIds) {
                mCheckedIdStates->Clear();
            }
        }
        // this may end up selecting the value we just cleared but this way
        // we ensure length of mCheckStates is 1, a fact GetCheckedItemPosition relies on
        if (value) {
            (*mCheckStates)[position] = TRUE;
            if (updateIds) {
                (*mCheckedIdStates)[id] = TRUE;
            }
        }
    }

    // Do not generate a data change while we are in the layout phase
    if (!mInLayout && !mBlockLayoutRequests) {
        mDataChanged = TRUE;
        RememberSyncState();
        RequestLayout();
    }

    return NOERROR;
}

/**
 * Returns the checked state of the specified position. The result is only
 * valid if the choice mode has been set to {@link #CHOICE_MODE_SINGLE}
 * or {@link #CHOICE_MODE_MULTIPLE}.
 *
 * @param position The item whose checked state to return
 * @return The item's checked state or <code>FALSE</code> if choice mode
 *         is invalid
 *
 * @see #setChoiceMode(Int32)
 */
Boolean ListView::IsItemChecked(
    /* [in] */ Int32 position)
{
    if (mChoiceMode != CHOICE_MODE_NONE && mCheckStates != NULL) {
        if (mCheckStates->Find(position) != mCheckStates->End()) {
            return (*mCheckStates)[position];
        }
    }

    return FALSE;
}

/**
 * Returns the currently checked item. The result is only valid if the choice
 * mode has been set to {@link #CHOICE_MODE_SINGLE}.
 *
 * @return The position of the currently checked item or
 *         {@link #AdapterView_INVALID_POSITION} if nothing is selected
 *
 * @see #setChoiceMode(Int32)
 */
Int32 ListView::GetCheckedItemPosition()
{
    if (mChoiceMode == CHOICE_MODE_SINGLE && mCheckStates != NULL
        && mCheckStates->GetSize() == 1) {
        return mCheckStates->Begin()->mFirst;
    }

    return AdapterView_INVALID_POSITION;
}

/**
 * Returns the set of checked items in the list. The result is only valid if
 * the choice mode has not been set to {@link #CHOICE_MODE_NONE}.
 *
 * @return  A SparseBooleanArray which will return TRUE for each call to
 *          Get(Int32 position) where position is a position in the list,
 *          or <code>NULL</code> if the choice mode is set to
 *          {@link #CHOICE_MODE_NONE}.
 */
//SparseBooleanArray ListView::GetCheckedItemPositions()
//{
//    if (mChoiceMode != CHOICE_MODE_NONE) {
//        return mCheckStates;
//    }
//    return NULL;
//}

/**
 * Returns the set of checked items ids. The result is only valid if the
 * choice mode has not been set to {@link #CHOICE_MODE_NONE} and the adapter
 * has stable IDs. ({@link ListAdapter#hasStableIds()} == {@code TRUE})
 *
 * @return A new array which contains the id of each checked item in the
 *         list.
 */
ECode ListView::GetCheckedItemIds(
    /* [out, callee] */ ArrayOf<Int64>** ids)
{
    assert(ids != NULL);

    if (mChoiceMode == CHOICE_MODE_NONE || mCheckedIdStates == NULL
        || mAdapter == NULL) {
        *ids = NULL;
        return NOERROR;
    }

    ArrayOf<Int64>* _ids = ArrayOf<Int64>::Alloc(mCheckedIdStates->GetSize());
    HashMap<Int64, Boolean>::Iterator iter = mCheckedIdStates->Begin();
    for (Int32 i = 0; iter != mCheckedIdStates->End(); ++iter, ++i) {
        (*_ids)[i] = iter->mFirst;
    }

    *ids = _ids;
    return NOERROR;
}

/**
 * Clear any choices previously set
 */
ECode ListView::ClearChoices()
{
    if (mCheckStates != NULL) {
        mCheckStates->Clear();
    }

    if (mCheckedIdStates != NULL) {
        mCheckedIdStates->Clear();
    }

    return NOERROR;
}

//@Override
AutoPtr<IParcelable> ListView::OnSaveInstanceState()
{
    AutoPtr<IParcelable> superState = AbsListView::OnSaveInstanceState();
    AutoPtr<IListViewSavedState> savedState;
    CListViewSavedState::New(superState, /*mCheckStates, mCheckedIdStates*/
        (IListViewSavedState**)&savedState);
    return (IParcelable*)savedState->Probe(EIID_IParcelable);
}

//@Override
void ListView::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    CListViewSavedState* ss =
        (CListViewSavedState*)IListViewSavedState::Probe(state);

    AbsListView::OnRestoreInstanceState(
        ((ListViewSavedState*)ss)->GetSuperState());

    if (ss->mCheckStates != NULL) {
        if (mCheckStates) {
            delete mCheckStates;
        }
        mCheckStates = ss->mCheckStates;
    }

    if (ss->mCheckIdStates != NULL) {
        if (mCheckedIdStates) {
            delete mCheckedIdStates;
        }
        mCheckedIdStates = ss->mCheckIdStates;
    }
}

ECode ListView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs/* = NULL*/,
    /* [in] */ Int32 defStyle/* = 0x01010074*//*com.android.internal.R.attr.listViewStyle*/)
{
    FAIL_RETURN(AbsListView::Init(context, attrs, defStyle));

    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mTempRect));

    mArrowScrollFocusResult = new ArrowScrollFocusResult();

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(
        attrs, ArrayOf<Int32>(R_Styleable_ListView,
        sizeof(R_Styleable_ListView) / sizeof(Int32)),//com.android.internal.R.styleable.ListView
        defStyle, 0, (ITypedArray**)&a);

    ArrayOf<ICharSequence*>* entries;
    a->GetTextArray(
        0/*com.android.internal.R.styleable.ListView_entries*/, &entries);

    if (entries != NULL) {
//        SetAdapter(new ArrayAdapter<CharSequence>(context,
//                com.android.internal.R.layout.simple_list_item_1, entries));

        for (Int32 i = 0; i < entries->GetLength(); ++i) {
            (*entries)[0]->Release();
        }
        ArrayOf<ICharSequence*>::Free(entries);
    }

    AutoPtr<IDrawable> d;
    a->GetDrawable(
        1/*com.android.internal.R.styleable.ListView_divider*/,
        (IDrawable**)&d);

    //printf("ListView::Init ListView_divider = 0x%08x\n", d.Get());
    if (d != NULL) {
        // If a divider is specified use its intrinsic height for divider height
        SetDivider(d);
    }

    AutoPtr<IDrawable> osHeader;
    a->GetDrawable(
        6/*com.android.internal.R.styleable.ListView_overScrollHeader*/,
        (IDrawable**)&osHeader);
    //printf("ListView::Init ListView_overScrollHeader = 0x%08x\n", osHeader.Get());
    if (osHeader != NULL) {
        SetOverscrollHeader(osHeader);
    }

    AutoPtr<IDrawable> osFooter;
    a->GetDrawable(
        7/*com.android.internal.R.styleable.ListView_overScrollFooter*/,
        (IDrawable**)&osFooter);
    //printf("ListView::Init ListView_overScrollFooter = 0x%08x\n", osFooter.Get());
    if (osFooter != NULL) {
        SetOverscrollFooter(osFooter);
    }

    // Use the height specified, zero being the default
    Int32 dividerHeight;
    a->GetDimensionPixelSize(
        2/*com.android.internal.R.styleable.ListView_dividerHeight*/,
        0, &dividerHeight);

    if (dividerHeight != 0) {
        SetDividerHeight(dividerHeight);
    }

    Int32 choiceMode;
    a->GetInt32(
        3/*R.styleable.ListView_choiceMode*/,
        CHOICE_MODE_NONE, &choiceMode);
    SetChoiceMode(choiceMode);

    a->GetBoolean(
        4/*R.styleable.ListView_headerDividersEnabled*/,
        TRUE, &mHeaderDividersEnabled);
    a->GetBoolean(
        5/*R.styleable.ListView_footerDividersEnabled*/,
        TRUE, &mFooterDividersEnabled);

    a->Recycle();

    return NOERROR;
}

