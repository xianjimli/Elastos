
#include "widget/AbsListView.h"
#include "view/ViewConfiguration.h"
#include "view/HapticFeedbackConstants.h"
#include "view/CKeyEvent.h"
#include "widget/CAbsListViewLayoutParams.h"
#include "widget/CAbsListViewSavedState.h"
#include "widget/OverScroller.h"
#include "widget/FastScroller.h"
#include "utils/CDisplayMetrics.h"
#include <elastos/Math.h>
#include <Logger.h>
#include <StringBuffer.h>

using namespace Elastos::Utility::Logging;
using namespace Elastos::Core;

const Int32 AbsListView::TOUCH_MODE_REST;
const Int32 AbsListView::TOUCH_MODE_DOWN;
const Int32 AbsListView::TOUCH_MODE_TAP;
const Int32 AbsListView::TOUCH_MODE_DONE_WAITING;
const Int32 AbsListView::TOUCH_MODE_SCROLL;
const Int32 AbsListView::TOUCH_MODE_FLING;
const Int32 AbsListView::TOUCH_MODE_OVERSCROLL;
const Int32 AbsListView::TOUCH_MODE_OVERFLING;
const Int32 AbsListView::LAYOUT_NORMAL;
const Int32 AbsListView::LAYOUT_FORCE_TOP;
const Int32 AbsListView::LAYOUT_SET_SELECTION;
const Int32 AbsListView::LAYOUT_FORCE_BOTTOM;
const Int32 AbsListView::LAYOUT_SPECIFIC;
const Int32 AbsListView::LAYOUT_SYNC;
const Int32 AbsListView::LAYOUT_MOVE_SELECTION;
const Int32 AbsListView::OVERSCROLL_LIMIT_DIVISOR;

const Int32 AbsListView::TOUCH_MODE_UNKNOWN;
const Int32 AbsListView::TOUCH_MODE_ON;
const Int32 AbsListView::TOUCH_MODE_OFF;
const Boolean AbsListView::PROFILE_SCROLLING;
const Boolean AbsListView::PROFILE_FLINGING;
const Int32 AbsListView::INVALID_POINTER;

const Int32 AbsListView::PositionScroller::SCROLL_DURATION;
const Int32 AbsListView::PositionScroller::MOVE_DOWN_POS;
const Int32 AbsListView::PositionScroller::MOVE_UP_POS;
const Int32 AbsListView::PositionScroller::MOVE_DOWN_BOUND;
const Int32 AbsListView::PositionScroller::MOVE_UP_BOUND;

AbsListView::CheckForTap::CheckForTap(
    /* [in] */ AbsListView* host)
    : mHost(host)
{}

ECode AbsListView::CheckForTap::Run()
{
    if (mHost->mTouchMode == mHost->TOUCH_MODE_DOWN) {
        mHost->mTouchMode = mHost->TOUCH_MODE_TAP;
        AutoPtr<IView> child = mHost->GetChildAt(
            mHost->mMotionPosition - mHost->mFirstPosition);

        Boolean hasFocusable = TRUE;
        if (child != NULL && (child->HasFocusable(&hasFocusable), !hasFocusable)) {
            mHost->mLayoutMode = mHost->LAYOUT_NORMAL;
            if (!mHost->mDataChanged) {
                mHost->LayoutChildren();
                child->SetPressed(TRUE);
                mHost->PositionSelector(child);
                mHost->SetPressed(TRUE);

                Int32 longPressTimeout = ViewConfiguration::GetLongPressTimeout();
                Boolean longClickable = mHost->IsLongClickable();

                if (mHost->mSelector != NULL) {
                    AutoPtr<IDrawable> d;
                    mHost->mSelector->GetCurrent((IDrawable**)&d);
                    //if (d != NULL && d instanceof TransitionDrawable) {
                    //    if (longClickable) {
                    //        ((TransitionDrawable) d).startTransition(longPressTimeout);
                    //    } else {
                    //        ((TransitionDrawable) d).resetTransition();
                    //    }
                    //}
                }

                if (longClickable) {
                    if (mHost->mPendingCheckForLongPress == NULL) {
                        mHost->mPendingCheckForLongPress =
                            new AbsListView::CheckForLongPress(mHost);
                    }

                    mHost->mPendingCheckForLongPress->RememberWindowAttachCount();
                    mHost->PostDelayed(
                        (IRunnable*)mHost->mPendingCheckForLongPress->Probe(
                        EIID_IRunnable), longPressTimeout);
                }
                else {
                    mHost->mTouchMode = mHost->TOUCH_MODE_DONE_WAITING;
                }
            }
            else {
                mHost->mTouchMode = mHost->TOUCH_MODE_DONE_WAITING;
            }
        }
    }

    return NOERROR;
}

AbsListView::RecycleBin::RecycleBin(
    /* [in] */ AbsListView* host)
    : mActiveViews(NULL)
    , mActiveViewsLength(0)
    , mScrapViews(NULL)
    , mViewTypeCount(0)
    , mHost(host)
{}

AbsListView::RecycleBin::~RecycleBin()
{
    delete[] mActiveViews;
    delete[] mScrapViews;
}

ECode AbsListView::RecycleBin::SetViewTypeCount(
    /* [in] */ Int32 viewTypeCount)
{
    if (viewTypeCount < 1) {
        Logger::E("RecycleBin", "Can't have a viewTypeCount < 1");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    //noinspection unchecked
    Vector<AutoPtr<IView> >* scrapViews =
        new Vector<AutoPtr<IView> >[viewTypeCount];

    if (mScrapViews) {
        delete[] mScrapViews;
    }

    mViewTypeCount = viewTypeCount;
    mCurrentScrap = scrapViews[0];
    mScrapViews = scrapViews;

    return NOERROR;
}

void AbsListView::RecycleBin::MarkChildrenDirty()
{
    if (mViewTypeCount == 1) {
        Vector<AutoPtr<IView> >::Iterator iter = mCurrentScrap.Begin();
        for (; iter!=mCurrentScrap.End(); ++iter) {
            (*iter)->ForceLayout();
        }
    }
    else {
        for (Int32 i = 0; i < mViewTypeCount; i++) {
            Vector<AutoPtr<IView> > scrap = mScrapViews[i];
            Vector<AutoPtr<IView> >::Iterator iter = scrap.Begin();
            for (; iter!=scrap.End(); ++iter) {
                (*iter)->ForceLayout();
            }
        }
    }
}

Boolean AbsListView::RecycleBin::ShouldRecycleViewType(
    /* [in] */ Int32 viewType)
{
    return viewType >= 0;
}

/**
 * Clears the scrap heap.
 */
void AbsListView::RecycleBin::Clear()
{
    if (mViewTypeCount == 1) {
        Vector<AutoPtr<IView> >::ReverseIterator rit = mCurrentScrap.RBegin();
        for (; rit != mCurrentScrap.REnd(); ++rit) {
            mHost->RemoveDetachedView((*rit).Get(), FALSE);
        }
        mCurrentScrap.Clear();
    }
    else {
        for (Int32 i = 0; i < mViewTypeCount; i++) {
            Vector<AutoPtr<IView> >& scrap = mScrapViews[i];
            Vector<AutoPtr<IView> >::ReverseIterator rit = scrap.RBegin();
            for (; rit != scrap.REnd(); ++rit) {
                mHost->RemoveDetachedView((*rit).Get(), FALSE);
            }
            scrap.Clear();
        }
    }
}

/**
 * Fill ActiveViews with all of the children of the AbsListView.
 *
 * @param childCount The minimum number of views mActiveViews should hold
 * @param firstActivePosition The position of the first view that will be stored in
 *        mActiveViews
 */
void AbsListView::RecycleBin::FillActiveViews(
    /* [in] */ Int32 childCount,
    /* [in] */ Int32 firstActivePosition)
{
    if (mActiveViewsLength < childCount) {
        if (mActiveViews) {
            delete[] mActiveViews;
        }
        mActiveViews = new AutoPtr<IView>[childCount];
        mActiveViewsLength = childCount;
    }

    mFirstActivePosition = firstActivePosition;

    for (Int32 i = 0; i < childCount; i++) {
        AutoPtr<IView> child = mHost->GetChildAt(i);
        AutoPtr<IAbsListViewLayoutParams> lp;
        child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        // Don't put header or footer views into the scrap heap
        if (lp != NULL && ((CAbsListViewLayoutParams*)lp.Get())->mViewType
            != AdapterView_ITEM_VIEW_TYPE_HEADER_OR_FOOTER) {
            // Note:  We do place AdapterView.ITEM_VIEW_TYPE_IGNORE in active views.
            //        However, we will NOT place them into scrap views.
            mActiveViews[i] = child;
        }
    }
}

/**
 * Get the view corresponding to the specified position. The view will be removed from
 * mActiveViews if it is found.
 *
 * @param position The position to look up in mActiveViews
 * @return The view if it is found, NULL otherwise
 */
AutoPtr<IView> AbsListView::RecycleBin::GetActiveView(
    /* [in] */ Int32 position)
{
    Int32 index = position - mFirstActivePosition;
    if (index >=0 && index < mActiveViewsLength) {
        AutoPtr<IView> match = mActiveViews[index];
        mActiveViews[index] = NULL;
        return match;
    }

    return NULL;
}

/**
 * @return A view from the ScrapViews collection. These are unordered.
 */
AutoPtr<IView> AbsListView::RecycleBin::GetScrapView(
    /* [in] */ Int32 position)
{
    if (mViewTypeCount == 1) {
        if (mCurrentScrap.GetSize() > 0) {
            AutoPtr<IView> view = mCurrentScrap.GetBack();
            mCurrentScrap.PopBack();
            return view;
        }
        else {
            return NULL;
        }
    }
    else {
        Int32 whichScrap;
        mHost->mAdapter->GetItemViewType(position, &whichScrap);
        if (whichScrap >= 0 && whichScrap < mViewTypeCount) {
            Vector<AutoPtr<IView> >& scrapViews = mScrapViews[whichScrap];
            if (scrapViews.GetSize() > 0) {
                AutoPtr<IView> view = scrapViews.GetBack();
                scrapViews.PopBack();
                return view;
            }
        }
    }

    return NULL;
}

/**
 * Put a view into the ScapViews list. These views are unordered.
 *
 * @param scrap The view to add
 */
void AbsListView::RecycleBin::AddScrapView(
    /* [in ]*/ IView* scrap)
{
    AutoPtr<IAbsListViewLayoutParams> lp;
    scrap->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    if (lp == NULL) {
        return;
    }

    // Don't put header or footer views or views that should be ignored
    // into the scrap heap
    Int32 viewType = ((CAbsListViewLayoutParams*)lp.Get())->mViewType;
    if (!ShouldRecycleViewType(viewType)) {
        if (viewType != AdapterView_ITEM_VIEW_TYPE_HEADER_OR_FOOTER) {
            mHost->RemoveDetachedView(scrap, FALSE);
        }
        return;
    }

    if (mViewTypeCount == 1) {
        scrap->DispatchStartTemporaryDetach();
        mCurrentScrap.PushBack(scrap);
    }
    else {
        scrap->DispatchStartTemporaryDetach();
        mScrapViews[viewType].PushBack(scrap);
    }

    if (mRecyclerListener != NULL) {
        mRecyclerListener->OnMovedToScrapHeap(scrap);
    }
}

/**
 * Move all views remaining in mActiveViews to mScrapViews.
 */
void AbsListView::RecycleBin::ScrapActiveViews()
{
    AutoPtr<IView>* activeViews = mActiveViews;
    Boolean hasListener = mRecyclerListener != NULL;
    Boolean multipleScraps = mViewTypeCount > 1;

    Vector<AutoPtr<IView> >& scrapViews = mCurrentScrap;
    for (Int32 i = mActiveViewsLength - 1; i >= 0; i--) {
        AutoPtr<IView> victim = activeViews[i];
        if (victim != NULL) {
            AutoPtr<IAbsListViewLayoutParams> lp;
            victim->GetLayoutParams((IViewGroupLayoutParams**)&lp);
            Int32 whichScrap = ((CAbsListViewLayoutParams*)lp.Get())->mViewType;

            activeViews[i] = NULL;

            if (!ShouldRecycleViewType(whichScrap)) {
                // Do not move views that should be ignored
                if (whichScrap != AdapterView_ITEM_VIEW_TYPE_HEADER_OR_FOOTER) {
                    mHost->RemoveDetachedView(victim, FALSE);
                }
                continue;
            }

            if (multipleScraps) {
                scrapViews = mScrapViews[whichScrap];
            }

            victim->DispatchStartTemporaryDetach();
            scrapViews.PushBack(victim);

            if (hasListener) {
                mRecyclerListener->OnMovedToScrapHeap(victim);
            }

            //if (ViewDebug.TRACE_RECYCLER) {
            //    ViewDebug.trace(victim,
            //            ViewDebug.RecyclerTraceType.MOVE_FROM_ACTIVE_TO_SCRAP_HEAP,
            //            mFirstActivePosition + i, -1);
            //}
        }
    }

    PruneScrapViews();
}

/**
 * Makes sure that the size of mScrapViews does not exceed the size of mActiveViews.
 * (This can happen if an adapter does not recycle its views).
 */
void AbsListView::RecycleBin::PruneScrapViews()
{
    Int32 maxViews = mActiveViewsLength;
    Int32 viewTypeCount = mViewTypeCount;
    for (Int32 i = 0; i < viewTypeCount; ++i) {
        Vector<AutoPtr<IView> >& scrapPile = mScrapViews[i];
        Int32 size = scrapPile.GetSize();
        Int32 extras = size - maxViews;
        for (Int32 j = 0; j < extras; j++) {
            AutoPtr<IView> view = scrapPile.GetBack();
            scrapPile.PopBack();
            mHost->RemoveDetachedView(view, FALSE);
        }
    }
}

/**
 * Puts all views in the scrap heap into the supplied list.
 */
void AbsListView::RecycleBin::ReclaimScrapViews(
    /* [in] */ IObjectContainer* views)
{
    if (mViewTypeCount == 1) {
        Vector<AutoPtr<IView> >::Iterator iter = mCurrentScrap.Begin();
        for (; iter!=mCurrentScrap.End(); ++iter) {
            views->Add((*iter).Get());
        }
    }
    else {
        for (Int32 i = 0; i < mViewTypeCount; i++) {
            Vector<AutoPtr<IView> >& scrap = mScrapViews[i];
            Vector<AutoPtr<IView> >::Iterator iter = scrap.Begin();
            for (; iter!=scrap.End(); ++iter) {
                views->Add((*iter).Get());
            }
        }
    }
}

/**
 * Updates the cache color hint of all known views.
 *
 * @param color The new cache color hint.
 */
void AbsListView::RecycleBin::SetCacheColorHint(
    /* [in] */ Int32 color)
{
    if (mViewTypeCount == 1) {
        Vector<AutoPtr<IView> >::Iterator iter = mCurrentScrap.Begin();
        for (; iter!=mCurrentScrap.End(); ++iter) {
            (*iter)->SetDrawingCacheBackgroundColor(color);
        }
    }
    else {
        for (Int32 i = 0; i < mViewTypeCount; i++) {
            Vector<AutoPtr<IView> >& scrap = mScrapViews[i];
            Vector<AutoPtr<IView> >::Iterator iter = scrap.Begin();
            for (; iter!=scrap.End(); ++iter) {
                (*iter)->SetDrawingCacheBackgroundColor(color);
            }
        }
    }
    // Just in case this is called during a layout pass
    for (Int32 i = 0; i < mActiveViewsLength; ++i) {
        AutoPtr<IView> victim = mActiveViews[i];
        if (victim != NULL) {
            victim->SetDrawingCacheBackgroundColor(color);
        }
    }
}

AbsListView::WindowRunnnable::WindowRunnnable(
    /* [in] */ AbsListView* host)
    : mHost(host)
    , mOriginalAttachCount(0)
{}

void AbsListView::WindowRunnnable::RememberWindowAttachCount()
{
    mOriginalAttachCount = mHost->GetWindowAttachCount();
}

Boolean AbsListView::WindowRunnnable::SameWindow()
{
    return mHost->HasWindowFocus() &&
        mHost->GetWindowAttachCount() == mOriginalAttachCount;
}

AbsListView::AbsListViewPerformClick::AbsListViewPerformClick (
    /* [in] */ AbsListView* host)
    : WindowRunnnable(host)
{}

ECode AbsListView::AbsListViewPerformClick::Run()
{
    // The data has changed since we posted this action in the event queue,
    // bail out before bad things happen
    if (mHost->mDataChanged) {
        return NOERROR;
    }

    IListAdapter* adapter = mHost->mAdapter;
    Int32 motionPosition = mClickMotionPosition;
    if (adapter != NULL) {
        Int32 count;
        adapter->GetCount(&count);
        if (mHost->mItemCount > 0 && motionPosition < count &&
            motionPosition != AdapterView_INVALID_POSITION &&
            SameWindow()) {
            Int64 itemId;
            adapter->GetItemId(motionPosition, &itemId);
            mHost->PerformItemClick(mChild, motionPosition, itemId);
        }
    }

    return NOERROR;
}

AbsListView::CheckForLongPress::CheckForLongPress (
    /* [in] */ AbsListView* host)
    : WindowRunnnable(host)
{}

ECode AbsListView::CheckForLongPress::Run()
{
    Int32 motionPosition = mHost->mMotionPosition;
    AutoPtr<IView> child = mHost->GetChildAt(
        motionPosition - mHost->mFirstPosition);
    if (child != NULL) {
        Int32 longPressPosition = mHost->mMotionPosition;
        Int64 longPressId;
        mHost->mAdapter->GetItemId(mHost->mMotionPosition, &longPressId);

        Boolean handled = FALSE;
        if (SameWindow() && !mHost->mDataChanged) {
            handled = mHost->PerformLongPress(
                child, longPressPosition, longPressId);
        }

        if (handled) {
            mHost->mTouchMode = TOUCH_MODE_REST;
            mHost->SetPressed(FALSE);
            child->SetPressed(FALSE);
        }
        else {
            mHost->mTouchMode = TOUCH_MODE_DONE_WAITING;
        }

    }

    return NOERROR;
}

AbsListView::CheckForKeyLongPress::CheckForKeyLongPress (
    /* [in] */ AbsListView* host)
    : WindowRunnnable(host)
{}

ECode AbsListView::CheckForKeyLongPress::Run()
{
    if (mHost->IsPressed() && mHost->mSelectedPosition >= 0) {
        Int32 index = mHost->mSelectedPosition - mHost->mFirstPosition;
        AutoPtr<IView> v = mHost->GetChildAt(index);

        if (!mHost->mDataChanged) {
            Boolean handled = FALSE;
            if (SameWindow()) {
                handled = mHost->PerformLongPress(
                    v, mHost->mSelectedPosition, mHost->mSelectedRowId);
            }

            if (handled) {
                mHost->SetPressed(FALSE);
                v->SetPressed(FALSE);
            }
        }
        else {
            mHost->SetPressed(FALSE);
            if (v != NULL)
                v->SetPressed(FALSE);
        }
    }

    return NOERROR;
}

AbsListView::OnTouchEventRunnable::OnTouchEventRunnable(
    /* [in] */ AbsListView* host,
    /* [in] */ IView* child)
    : mHost(host)
    , mChild(child)
{
}

ECode AbsListView::OnTouchEventRunnable::Run()
{
    mChild->SetPressed(FALSE);
    mHost->SetPressed(FALSE);
    if (!mHost->mDataChanged) {
        mHost->Post(
            (IRunnable*)mHost->mPerformClick->Probe(EIID_IRunnable));
    }
    mHost->mTouchMode = mHost->TOUCH_MODE_REST;
    Release();

    return NOERROR;
}

AbsListView::FlingRunnable::FlingRunnable(
    /* [in] */ AbsListView* host)
    : mHost(host)
{
    mScroller = new OverScroller(host->GetContext());
}

AbsListView::FlingRunnable::~FlingRunnable()
{
    delete mScroller;
}

void AbsListView::FlingRunnable::Start(
    /* [in] */ Int32 initialVelocity)
{
    Int32 initialY = initialVelocity < 0 ? Math::INT32_MAX_VALUE : 0;
    mLastFlingY = initialY;
    mScroller->Fling(0, initialY, 0, initialVelocity,
        0, Math::INT32_MAX_VALUE, 0, Math::INT32_MAX_VALUE);
    mHost->mTouchMode = TOUCH_MODE_FLING;
    mHost->Post((IRunnable*)this->Probe(EIID_IRunnable));

    if (mHost->PROFILE_FLINGING) {
        if (!mHost->mFlingProfilingStarted) {
            //Debug.startMethodTracing("AbsListViewFling");
            mHost->mFlingProfilingStarted = TRUE;
        }
    }
}

void AbsListView::FlingRunnable::StartSpringback()
{
    if (mScroller->SpringBack(0, mHost->mScrollY, 0, 0, 0, 0)) {
        mHost->mTouchMode = mHost->TOUCH_MODE_OVERFLING;
        mHost->Invalidate();
        mHost->Post((IRunnable*)this->Probe(EIID_IRunnable));
    }
    else {
        mHost->mTouchMode = mHost->TOUCH_MODE_REST;
    }
}

void AbsListView::FlingRunnable::StartOverfling(
    /* [in] */ Int32 initialVelocity)
{
    Int32 min = mHost->mScrollY > 0 ? Math::INT32_MIN_VALUE : 0;
    Int32 max = mHost->mScrollY > 0 ? 0 : Math::INT32_MAX_VALUE;
    mScroller->Fling(
        0, mHost->mScrollY, 0, initialVelocity, 0, 0,
        min, max, 0, mHost->GetHeight());
    mHost->mTouchMode = mHost->TOUCH_MODE_OVERFLING;
    mHost->Invalidate();
    mHost->Post((IRunnable*)this->Probe(EIID_IRunnable));
}

void AbsListView::FlingRunnable::EdgeReached(
    /* [in] */ Int32 delta)
{
    mScroller->NotifyVerticalEdgeReached(
        mHost->mScrollY, 0, mHost->mOverflingDistance);
    Int32 overscrollMode = mHost->GetOverScrollMode();
    if (overscrollMode == mHost->OVER_SCROLL_ALWAYS ||
        (overscrollMode == mHost->OVER_SCROLL_IF_CONTENT_SCROLLS
        && !mHost->ContentFits())) {
        mHost->mTouchMode = mHost->TOUCH_MODE_OVERFLING;
        Int32 vel = (Int32)mScroller->GetCurrVelocity();
        if (delta > 0) {
            mHost->mEdgeGlowTop->OnAbsorb(vel);
        }
        else {
            mHost->mEdgeGlowBottom->OnAbsorb(vel);
        }
    }
    mHost->Invalidate();
    mHost->Post((IRunnable*)this->Probe(EIID_IRunnable));
}

void AbsListView::FlingRunnable::StartScroll(
    /* [in] */ Int32 distance,
    /* [in] */ Int32 duration)
{
    Int32 initialY = distance < 0 ? Math::INT32_MAX_VALUE : 0;
    mLastFlingY = initialY;
    mScroller->StartScroll(0, initialY, 0, distance, duration);
    mHost->mTouchMode = mHost->TOUCH_MODE_FLING;
    mHost->Post((IRunnable*)this->Probe(EIID_IRunnable));
}

void AbsListView::FlingRunnable::EndFling()
{
    mHost->mTouchMode = mHost->TOUCH_MODE_REST;

    mHost->ReportScrollStateChange(OnScrollListener_SCROLL_STATE_IDLE);
    mHost->ClearScrollingCache();

    mHost->RemoveCallbacks((IRunnable*)this->Probe(EIID_IRunnable));

    if (mHost->mPositionScroller != NULL) {
        mHost->RemoveCallbacks(
            (IRunnable*)mHost->mPositionScroller->Probe(EIID_IRunnable));
    }
}

ECode AbsListView::FlingRunnable::Run()
{
    switch (mHost->mTouchMode)
    {
    case TOUCH_MODE_FLING:
        {
            if (mHost->mItemCount == 0 || mHost->GetChildCount() == 0) {
                EndFling();
                return NOERROR;
            }

            Boolean more = mScroller->ComputeScrollOffset();
            Int32 y = mScroller->GetCurrY();

            // Flip sign to convert finger direction to list items direction
            // (e.g. finger moving down means list is moving towards the top)
            Int32 delta = mLastFlingY - y;

            // Pretend that each frame of a fling scroll is a touch scroll
            if (delta > 0) {
                // List is moving towards the top. Use first view as mMotionPosition
                mHost->mMotionPosition = mHost->mFirstPosition;
                AutoPtr<IView> firstView = mHost->GetChildAt(0);
                firstView->GetTop(&mHost->mMotionViewOriginalTop);

                // Don't fling more than 1 screen
                delta = Math::Min(
                    mHost->GetHeight() - mHost->mPaddingBottom
                    - mHost->mPaddingTop - 1, delta);
            }
            else {
                // List is moving towards the bottom. Use last view as mMotionPosition
                Int32 offsetToLast = mHost->GetChildCount() - 1;
                mHost->mMotionPosition = mHost->mFirstPosition + offsetToLast;

                AutoPtr<IView> lastView = mHost->GetChildAt(offsetToLast);
                lastView->GetTop(&mHost->mMotionViewOriginalTop);

                // Don't fling more than 1 screen
                delta = Math::Max(
                    -(mHost->GetHeight() - mHost->mPaddingBottom
                    - mHost->mPaddingTop - 1), delta);
            }

            // Check to see if we have bumped into the scroll limit
            AutoPtr<IView> motionView = mHost->GetChildAt(
                mHost->mMotionPosition - mHost->mFirstPosition);
            Int32 oldTop = 0;
            if (motionView != NULL) {
                motionView->GetTop(&oldTop);
            }

            Boolean atEnd = mHost->TrackMotionScroll(delta, delta);
            if (atEnd) {
                if (motionView != NULL) {
                    // Tweak the scroll for how far we overshot
                    Int32 top;
                    motionView->GetTop(&top);
                    Int32 overshoot = -(delta - (top - oldTop));
                    mHost->OverScrollBy(
                        0, overshoot, 0, mHost->mScrollY, 0, 0,
                        0, mHost->mOverflingDistance, FALSE);
                }

                if (more) {
                    EdgeReached(delta);
                }
                break;
            }

            if (more && !atEnd) {
                mHost->Invalidate();
                mLastFlingY = y;
                mHost->Post((IRunnable*)this->Probe(EIID_IRunnable));
            }
            else {
                EndFling();

                if (mHost->PROFILE_FLINGING) {
                    if (mHost->mFlingProfilingStarted) {
                        //Debug.stopMethodTracing();
                        mHost->mFlingProfilingStarted = FALSE;
                    }
                }
            }
        }
        break;
    case TOUCH_MODE_OVERFLING:
        {
            if (mScroller->ComputeScrollOffset()) {
                Int32 scrollY = mHost->mScrollY;
                Int32 deltaY = mScroller->GetCurrY() - scrollY;
                if (mHost->OverScrollBy(
                    0, deltaY, 0, scrollY, 0, 0,
                    0, mHost->mOverflingDistance, FALSE)) {
                        StartSpringback();
                }
                else {
                    mHost->Invalidate();
                    mHost->Post((IRunnable*)this->Probe(EIID_IRunnable));
                }
            }
            else {
                EndFling();
            }
        }
        break;
    default:
        break;
    }

    return NOERROR;
}

AbsListView::PositionScroller::PositionScroller(
    /* [in] */ AbsListView* host)
    : mHost(host)
{
    mExtraScroll = ViewConfiguration::Get(mHost->mContext)
        ->GetScaledFadingEdgeLength();
}

void AbsListView::PositionScroller::Start(
    /* [in] */ Int32 position)
{
    Int32 firstPos = mHost->mFirstPosition;
    Int32 lastPos = firstPos + mHost->GetChildCount() - 1;

    Int32 viewTravelCount = 0;
    if (position <= firstPos) {
        viewTravelCount = firstPos - position + 1;
        mMode = MOVE_UP_POS;
    }
    else if (position >= lastPos) {
        viewTravelCount = position - lastPos + 1;
        mMode = MOVE_DOWN_POS;
    }
    else {
        // Already on screen, nothing to do
        return;
    }

    if (viewTravelCount > 0) {
        mScrollDuration = SCROLL_DURATION / viewTravelCount;
    }
    else {
        mScrollDuration = SCROLL_DURATION;
    }
    mTargetPos = position;
    mBoundPos = AdapterView_INVALID_POSITION;
    mLastSeenPos = AdapterView_INVALID_POSITION;

    mHost->Post((IRunnable*)this->Probe(EIID_IRunnable));
}

void AbsListView::PositionScroller::Start(
    /* [in] */ Int32 position,
    /* [in] */ Int32 boundPosition)
{
    if (boundPosition == AdapterView_INVALID_POSITION) {
        Start(position);
        return;
    }

    Int32 firstPos = mHost->mFirstPosition;
    Int32 lastPos = firstPos + mHost->GetChildCount() - 1;

    Int32 viewTravelCount = 0;
    if (position <= firstPos) {
        Int32 boundPosFromLast = lastPos - boundPosition;
        if (boundPosFromLast < 1) {
            // Moving would shift our bound position off the screen. Abort.
            return;
        }

        Int32 posTravel = firstPos - position + 1;
        Int32 boundTravel = boundPosFromLast - 1;
        if (boundTravel < posTravel) {
            viewTravelCount = boundTravel;
            mMode = MOVE_UP_BOUND;
        }
        else {
            viewTravelCount = posTravel;
            mMode = MOVE_UP_POS;
        }
    }
    else if (position >= lastPos) {
        Int32 boundPosFromFirst = boundPosition - firstPos;
        if (boundPosFromFirst < 1) {
            // Moving would shift our bound position off the screen. Abort.
            return;
        }

        Int32 posTravel = position - lastPos + 1;
        Int32 boundTravel = boundPosFromFirst - 1;
        if (boundTravel < posTravel) {
            viewTravelCount = boundTravel;
            mMode = MOVE_DOWN_BOUND;
        }
        else {
            viewTravelCount = posTravel;
            mMode = MOVE_DOWN_POS;
        }
    }
    else {
        // Already on screen, nothing to do
        return;
    }

    if (viewTravelCount > 0) {
        mScrollDuration = SCROLL_DURATION / viewTravelCount;
    }
    else {
        mScrollDuration = SCROLL_DURATION;
    }
    mTargetPos = position;
    mBoundPos = boundPosition;
    mLastSeenPos = AdapterView_INVALID_POSITION;

    mHost->Post((IRunnable*)this->Probe(EIID_IRunnable));
}

void AbsListView::PositionScroller::Stop()
{
    mHost->RemoveCallbacks((IRunnable*)this->Probe(EIID_IRunnable));
}

ECode AbsListView::PositionScroller::Run()
{
    Int32 listHeight = mHost->GetHeight();
    Int32 firstPos = mHost->mFirstPosition;

    switch (mMode) {
    case MOVE_DOWN_POS:
        {
            Int32 lastViewIndex = mHost->GetChildCount() - 1;
            Int32 lastPos = firstPos + lastViewIndex;

            if (lastViewIndex < 0) {
                return NOERROR;
            }

            if (lastPos == mLastSeenPos) {
                // No new views, let things keep going.
                mHost->Post((IRunnable*)this->Probe(EIID_IRunnable));
                return NOERROR;
            }

            AutoPtr<IView> lastView = mHost->GetChildAt(lastViewIndex);
            Int32 lastViewHeight;
            lastView->GetHeight(&lastViewHeight);
            Int32 lastViewTop;
            lastView->GetTop(&lastViewTop);
            Int32 lastViewPixelsShowing = listHeight - lastViewTop;
            Int32 extraScroll = lastPos < mHost->mItemCount - 1
                ? mExtraScroll : mHost->mListPadding->mBottom;

            mHost->SmoothScrollBy(
                lastViewHeight - lastViewPixelsShowing + extraScroll,
                mScrollDuration);

            mLastSeenPos = lastPos;
            if (lastPos < mTargetPos) {
                mHost->Post((IRunnable*)this->Probe(EIID_IRunnable));
            }
        }
        break;
    case MOVE_DOWN_BOUND:
        {
            Int32 nextViewIndex = 1;
            Int32 childCount = mHost->GetChildCount();

            if (firstPos == mBoundPos || childCount <= nextViewIndex
                || firstPos + childCount >= mHost->mItemCount) {
                return NOERROR;
            }

            Int32 nextPos = firstPos + nextViewIndex;
            if (nextPos == mLastSeenPos) {
                // No new views, let things keep going.
                mHost->Post((IRunnable*)this->Probe(EIID_IRunnable));
                return NOERROR;
            }

            AutoPtr<IView> nextView = mHost->GetChildAt(nextViewIndex);
            Int32 nextViewHeight;
            nextView->GetHeight(&nextViewHeight);
            Int32 nextViewTop;
            nextView->GetTop(&nextViewTop);
            Int32 extraScroll = mExtraScroll;
            if (nextPos < mBoundPos) {
                mHost->SmoothScrollBy(
                    Math::Max(0, nextViewHeight + nextViewTop - extraScroll),
                    mScrollDuration);
                mLastSeenPos = nextPos;
                mHost->Post((IRunnable*)this->Probe(EIID_IRunnable));
            }
            else  {
                if (nextViewTop > extraScroll) {
                    mHost->SmoothScrollBy(nextViewTop - extraScroll, mScrollDuration);
                }
            }
        }
        break;
    case MOVE_UP_POS:
        {
            if (firstPos == mLastSeenPos) {
                // No new views, let things keep going.
                mHost->Post((IRunnable*)this->Probe(EIID_IRunnable));
                return NOERROR;
            }

            AutoPtr<IView> firstView = mHost->GetChildAt(0);
            if (firstView == NULL) {
                return NOERROR;
            }
            Int32 firstViewTop;
            firstView->GetTop(&firstViewTop);
            Int32 extraScroll = firstPos > 0
                ? mExtraScroll : mHost->mListPadding->mTop;

            mHost->SmoothScrollBy(
                firstViewTop - extraScroll, mScrollDuration);

            mLastSeenPos = firstPos;

            if (firstPos > mTargetPos) {
                mHost->Post((IRunnable*)this->Probe(EIID_IRunnable));
            }
        }
        break;
    case MOVE_UP_BOUND:
        {
            Int32 lastViewIndex = mHost->GetChildCount() - 2;
            if (lastViewIndex < 0) {
                return NOERROR;
            }

            Int32 lastPos = firstPos + lastViewIndex;
            if (lastPos == mLastSeenPos) {
                // No new views, let things keep going.
                mHost->Post((IRunnable*)this->Probe(EIID_IRunnable));
                return NOERROR;
            }

            AutoPtr<IView> lastView = mHost->GetChildAt(lastViewIndex);
            Int32 lastViewHeight;
            lastView->GetHeight(&lastViewHeight);
            Int32 lastViewTop;
            lastView->GetTop(&lastViewTop);
            Int32 lastViewPixelsShowing = listHeight - lastViewTop;
            mLastSeenPos = lastPos;
            if (lastPos > mBoundPos) {
                mHost->SmoothScrollBy(
                    -(lastViewPixelsShowing - mExtraScroll),
                    mScrollDuration);
                mHost->Post((IRunnable*)this->Probe(EIID_IRunnable));
            }
            else {
                Int32 bottom = listHeight - mExtraScroll;
                Int32 lastViewBottom = lastViewTop + lastViewHeight;
                if (bottom > lastViewBottom) {
                    mHost->SmoothScrollBy(
                        -(bottom - lastViewBottom), mScrollDuration);
                }
            }
        }
        break;
    default:
        break;
    }

    return NOERROR;
}

AbsListView::ClearScrollingCacheRunnable::ClearScrollingCacheRunnable(
    /* [in] */ AbsListView* host)
    : mHost(host)
{}

ECode AbsListView::ClearScrollingCacheRunnable::Run()
{
    if (mHost->mCachingStarted) {
        mHost->mCachingStarted = FALSE;
        mHost->SetChildrenDrawnWithCacheEnabled(FALSE);
        if ((mHost->mPersistentDrawingCache
            & mHost->PERSISTENT_SCROLLING_CACHE) == 0) {
                mHost->SetChildrenDrawingCacheEnabled(FALSE);
        }

        if (!mHost->IsAlwaysDrawnWithCacheEnabled()) {
            mHost->Invalidate();
        }
    }

    return NOERROR;
}

AbsListView::AbsListView()
    : mLayoutMode(LAYOUT_NORMAL)
    , mDrawSelectorOnTop(FALSE)
    , mSelectionLeftPadding(0)
    , mSelectionTopPadding(0)
    , mSelectionRightPadding(0)
    , mSelectionBottomPadding(0)
    , mWidthMeasureSpec(0)
    , mCachingStarted(FALSE)
    , mTouchMode(TOUCH_MODE_REST)
    , mSelectedTop(0)
    , mStackFromBottom(FALSE)
    , mScrollingCacheEnabled(FALSE)
    , mFastScrollEnabled(FALSE)
    , mResurrectToPosition(AdapterView_INVALID_POSITION)
    , mSmoothScrollbarEnabled(TRUE)
    , mLastTouchMode(TOUCH_MODE_UNKNOWN)
    , mScrollProfilingStarted(FALSE)
    , mFlingProfilingStarted(FALSE)
    , mIsChildViewEnabled(FALSE)
    , mLastScrollState(OnScrollListener_SCROLL_STATE_IDLE)
    , mFastScroller(NULL)
    , mGlobalLayoutListenerAddedFilter(FALSE)
    , mPopupHidden(FALSE)
    , mActivePointerId(INVALID_POINTER)
    , mEdgeGlowTop(NULL)
    , mEdgeGlowBottom(NULL)
    , mDirection(0)
{}

AbsListView::AbsListView(
    /* [in] */ IContext* context)
    : mLayoutMode(LAYOUT_NORMAL)
    , mDrawSelectorOnTop(FALSE)
    , mSelectionLeftPadding(0)
    , mSelectionTopPadding(0)
    , mSelectionRightPadding(0)
    , mSelectionBottomPadding(0)
    , mWidthMeasureSpec(0)
    , mCachingStarted(FALSE)
    , mTouchMode(TOUCH_MODE_REST)
    , mSelectedTop(0)
    , mStackFromBottom(FALSE)
    , mScrollingCacheEnabled(FALSE)
    , mFastScrollEnabled(FALSE)
    , mResurrectToPosition(AdapterView_INVALID_POSITION)
    , mSmoothScrollbarEnabled(TRUE)
    , mLastTouchMode(TOUCH_MODE_UNKNOWN)
    , mScrollProfilingStarted(FALSE)
    , mFlingProfilingStarted(FALSE)
    , mIsChildViewEnabled(FALSE)
    , mLastScrollState(OnScrollListener_SCROLL_STATE_IDLE)
    , mFastScroller(NULL)
    , mGlobalLayoutListenerAddedFilter(FALSE)
    , mPopupHidden(FALSE)
    , mActivePointerId(INVALID_POINTER)
    , mEdgeGlowTop(NULL)
    , mEdgeGlowBottom(NULL)
    , mDirection(0)
{
    Init(context);
}

AbsListView::AbsListView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : mLayoutMode(LAYOUT_NORMAL)
    , mDrawSelectorOnTop(FALSE)
    , mSelectionLeftPadding(0)
    , mSelectionTopPadding(0)
    , mSelectionRightPadding(0)
    , mSelectionBottomPadding(0)
    , mWidthMeasureSpec(0)
    , mCachingStarted(FALSE)
    , mTouchMode(TOUCH_MODE_REST)
    , mSelectedTop(0)
    , mStackFromBottom(FALSE)
    , mScrollingCacheEnabled(FALSE)
    , mFastScrollEnabled(FALSE)
    , mResurrectToPosition(AdapterView_INVALID_POSITION)
    , mSmoothScrollbarEnabled(TRUE)
    , mLastTouchMode(TOUCH_MODE_UNKNOWN)
    , mScrollProfilingStarted(FALSE)
    , mFlingProfilingStarted(FALSE)
    , mIsChildViewEnabled(FALSE)
    , mLastScrollState(OnScrollListener_SCROLL_STATE_IDLE)
    , mFastScroller(NULL)
    , mGlobalLayoutListenerAddedFilter(FALSE)
    , mPopupHidden(FALSE)
    , mActivePointerId(INVALID_POINTER)
    , mEdgeGlowTop(NULL)
    , mEdgeGlowBottom(NULL)
    , mDirection(0)
{
    Init(context, attrs, defStyle);
}

AbsListView::~AbsListView()
{
    if (mRecycler != NULL)
        delete mRecycler;

    if (mEdgeGlowTop != NULL)
        delete mEdgeGlowTop;

    if (mEdgeGlowBottom != NULL)
        delete mEdgeGlowBottom;

    if (mFastScroller != NULL)
        delete mFastScroller;
}

void AbsListView::InitAbsListView()
{
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&mSelectorRect)));
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&mListPadding)));
    mRecycler = new RecycleBin(this);

    // Setting focusable in touch mode will set the focusable property to TRUE
    SetClickable(TRUE);
    SetFocusableInTouchMode(TRUE);
    SetWillNotDraw(FALSE);
    SetAlwaysDrawnWithCacheEnabled(FALSE);
    SetScrollingCacheEnabled(TRUE);

    AutoPtr<ViewConfiguration> configuration = ViewConfiguration::Get(mContext);
    mTouchSlop = configuration->GetScaledTouchSlop();
    mMinimumVelocity = configuration->GetScaledMinimumFlingVelocity();
    mMaximumVelocity = configuration->GetScaledMaximumFlingVelocity();
    mOverscrollDistance = configuration->GetScaledOverscrollDistance();
    mOverflingDistance = configuration->GetScaledOverflingDistance();

    AutoPtr<IResources> resources;
    GetContext()->GetResources((IResources**)&resources);
    AutoPtr<CDisplayMetrics> dm;
    resources->GetDisplayMetrics((IDisplayMetrics**)&dm);
    mDensityScale = dm->mDensity;
}

//@Override
ECode AbsListView::SetOverScrollMode(
    /* [in] */ Int32 mode)
{
    if (mode != OVER_SCROLL_NEVER) {
        if (mEdgeGlowTop == NULL) {
            AutoPtr<IResources> res;
            GetContext()->GetResources((IResources**)&res);
            AutoPtr<IDrawable> edge;
            res->GetDrawable(
                0x01080238/*R.drawable.overscroll_edge*/, (IDrawable**)&edge);
            AutoPtr<IDrawable> glow;
            res->GetDrawable(
                0x01080239/*R.drawable.overscroll_glow*/, (IDrawable**)&glow);
            //printf("AbsListView::SetOverScrollMode edge = 0x%08x, glow = 0x%08x\n",
                //edge.Get(), glow.Get());
            mEdgeGlowTop = new EdgeGlow(edge, glow);
            mEdgeGlowBottom = new EdgeGlow(edge, glow);
        }
    }
    else {
        if (mEdgeGlowTop != NULL) delete mEdgeGlowTop;
        if (mEdgeGlowBottom != NULL) delete mEdgeGlowBottom;
        mEdgeGlowTop = NULL;
        mEdgeGlowBottom = NULL;
    }

    return AdapterView::SetOverScrollMode(mode);
}

/**
 * @return TRUE if all list content currently fits within the view boundaries
 */
Boolean AbsListView::ContentFits()
{
    Int32 childCount = GetChildCount();
    if (childCount != mItemCount) {
        return FALSE;
    }
    Int32 top, bottom;
    AutoPtr<IView> item = GetChildAt(0);
    item->GetTop(&top);
    item = GetChildAt(mItemCount - 1);
    item->GetBottom(&bottom);

    return top >= 0 && bottom <= mBottom;
}

/**
 * Enables fast scrolling by letting the user quickly scroll through lists by
 * dragging the fast scroll thumb. The adapter attached to the list may want
 * to implement {@link SectionIndexer} if it wishes to display alphabet preview and
 * jump between sections of the list.
 * @see SectionIndexer
 * @see #isFastScrollEnabled()
 * @param enabled whether or not to enable fast scrolling
 */
ECode AbsListView::SetFastScrollEnabled(
    /* [in] */ Boolean enabled)
{
    mFastScrollEnabled = enabled;
    if (enabled) {
        if (mFastScroller == NULL) {
            mFastScroller = new FastScroller(GetContext(), this);
        }
    }
    else {
        if (mFastScroller != NULL) {
            mFastScroller->Stop();
            delete mFastScroller;
            mFastScroller = NULL;
        }
    }
    return NOERROR;
}

/**
 * Returns the current state of the fast scroll feature.
 * @see #setFastScrollEnabled(Boolean)
 * @return TRUE if fast scroll is enabled, FALSE otherwise
 */
Boolean AbsListView::IsFastScrollEnabled()
{
    return mFastScrollEnabled;
}

/**
 * If fast scroll is visible, then don't draw the vertical scrollbar.
 * @hide
 */
//@Override
Boolean AbsListView::IsVerticalScrollBarHidden()
{
    return mFastScroller != NULL && mFastScroller->IsVisible();
}

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
ECode AbsListView::SetSmoothScrollbarEnabled(
    /* [in] */ Boolean enabled)
{
    mSmoothScrollbarEnabled = enabled;

    return NOERROR;
}

/**
 * Returns the current state of the fast scroll feature.
 *
 * @return True if smooth scrollbar is enabled is enabled, FALSE otherwise.
 *
 * @see #setSmoothScrollbarEnabled(Boolean)
 */
//@ViewDebug.ExportedProperty
Boolean AbsListView::IsSmoothScrollbarEnabled()
{
    return mSmoothScrollbarEnabled;
}

/**
 * Set the listener that will receive notifications every time the list scrolls.
 *
 * @param l the scroll listener
 */
ECode AbsListView::SetOnScrollListener(
    /* [in] */ IOnScrollListener* l)
{
    mOnScrollListener = l;
    InvokeOnItemScrollListener();

    return NOERROR;
}

/**
 * Notify our scroll listener (if there is one) of a change in scroll state
 */
ECode AbsListView::InvokeOnItemScrollListener()
{
    if (mFastScroller != NULL) {
        mFastScroller->OnScroll(
            (IAbsListView*)this->Probe(EIID_IAbsListView),
            mFirstPosition, GetChildCount(), mItemCount);
    }

    if (mOnScrollListener != NULL) {
        mOnScrollListener->OnScroll(
            (IAbsListView*)this->Probe(EIID_IAbsListView),
            mFirstPosition, GetChildCount(), mItemCount);
    }

    return NOERROR;
}

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
Boolean AbsListView::IsScrollingCacheEnabled()
{
    return mScrollingCacheEnabled;
}

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
ECode AbsListView::SetScrollingCacheEnabled(
    /* [in] */ Boolean enabled)
{
    if (mScrollingCacheEnabled && !enabled) {
        ClearScrollingCache();
    }
    mScrollingCacheEnabled = enabled;

    return NOERROR;
}

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
ECode AbsListView::SetTextFilterEnabled(
    /* [in] */ Boolean textFilterEnabled)
{
    mTextFilterEnabled = textFilterEnabled;

    return NOERROR;
}

/**
 * Indicates whether type filtering is enabled for this view
 *
 * @return TRUE if type filtering is enabled, FALSE otherwise
 *
 * @see #setTextFilterEnabled(Boolean)
 * @see Filterable
 */
//@ViewDebug.ExportedProperty
Boolean AbsListView::IsTextFilterEnabled()
{
    return mTextFilterEnabled;
}

//@Override
ECode AbsListView::GetFocusedRect(
    /* [in] */ IRect* r)
{
    AutoPtr<IView> view = GetSelectedView();
    if (view != NULL) {
        AutoPtr<IViewParent> vp;
        view->GetParent((IViewParent**)&vp);
        if (vp && (IView*)vp->Probe(EIID_IView)
            ==(IView*)this->Probe(EIID_IView)) {
            // the focused rectangle of the selected view offset into the
            // coordinate space of this view.
            view->GetFocusedRect(r);
            OffsetDescendantRectToMyCoords(view, r);
        }
    }
    else {
        // otherwise, just the norm
        return AdapterView::GetFocusedRect(r);
    }

    return NOERROR;
}

void AbsListView::UseDefaultSelector()
{
    AutoPtr<IDrawable> drawable;
    GetResources()->GetDrawable(
        0x01080062/*com.android.internal.R.drawable.list_selector_background*/,
        (IDrawable**)&drawable);
    SetSelector(drawable);
}

/**
 * Indicates whether the content of this view is pinned to, or stacked from,
 * the bottom edge.
 *
 * @return TRUE if the content is stacked from the bottom edge, FALSE otherwise
 */
//@ViewDebug.ExportedProperty
Boolean AbsListView::IsStackFromBottom()
{
    return mStackFromBottom;
}

/**
 * When stack from bottom is set to TRUE, the list fills its content starting from
 * the bottom of the view.
 *
 * @param stackFromBottom TRUE to pin the view's content to the bottom edge,
 *        FALSE to pin the view's content to the top edge
 */
ECode AbsListView::SetStackFromBottom(
    /* [in] */ Boolean stackFromBottom)
{
    if (mStackFromBottom != stackFromBottom) {
        mStackFromBottom = stackFromBottom;
        RequestLayoutIfNecessary();
    }

    return NOERROR;
}

ECode AbsListView::RequestLayoutIfNecessary()
{
    if (GetChildCount() > 0) {
        ResetList();
        RequestLayout();
        Invalidate();
    }

    return NOERROR;
}

//@Override
AutoPtr<IParcelable> AbsListView::OnSaveInstanceState()
{
    /*
     * This doesn't really make sense as the place to dismiss the
     * popups, but there don't seem to be any other useful hooks
     * that happen early enough to keep from getting complaints
     * about having leaked the window.
     */
    DismissPopup();

    AutoPtr<IParcelable> superState = AdapterView::OnSaveInstanceState();

    AutoPtr<CAbsListViewSavedState> ss;
    assert(SUCCEEDED(CAbsListViewSavedState::NewByFriend(
        superState, (CAbsListViewSavedState**)&ss)));

    Boolean haveChildren = GetChildCount() > 0 && mItemCount > 0;
    Int64 selectedId = GetSelectedItemId();
    ss->mSelectedId = selectedId;
    ss->mHeight = GetHeight();

    if (selectedId >= 0) {
        // Remember the selection
        ss->mViewTop = mSelectedTop;
        ss->mPosition = GetSelectedItemPosition();
        ss->mFirstId = AdapterView_INVALID_POSITION;
    }
    else {
        if (haveChildren && mFirstPosition > 0) {
            // Remember the position of the first child.
            // We only do this if we are not currently at the top of
            // the list, for two reasons:
            // (1) The list may be in the process of becoming empty, in
            // which case mItemCount may not be 0, but if we try to
            // ask for any information about position 0 we will crash.
            // (2) Being "at the top" seems like a special case, anyway,
            // and the user wouldn't expect to end up somewhere else when
            // they revisit the list even if its content has changed.
            AutoPtr<IView> v = GetChildAt(0);
            v->GetTop(&ss->mViewTop);
            Int32 firstPos = mFirstPosition;
            if (firstPos >= mItemCount) {
                firstPos = mItemCount - 1;
            }
            ss->mPosition = firstPos;
            mAdapter->GetItemId(firstPos, &ss->mFirstId);
        }
        else {
            ss->mViewTop = 0;
            ss->mFirstId = AdapterView_INVALID_POSITION;
            ss->mPosition = 0;
        }
    }

    if (mFiltered) {
        //final EditText textFilter = mTextFilter;
        //if (textFilter != NULL) {
        //    Editable filterText = textFilter.getText();
        //    if (filterText != NULL) {
        //        ss.filter = filterText.toString();
        //    }
        //}
    }

    return (IParcelable*)ss->Probe(EIID_IParcelable);
}

//@Override
void AbsListView::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    CAbsListViewSavedState* ss =
        (CAbsListViewSavedState*)IAbsListViewSavedState::Probe(state);

    AutoPtr<IParcelable> superState;
    ss->GetSuperState((IParcelable**)&superState);

    AdapterView::OnRestoreInstanceState(superState);

    mDataChanged = TRUE;

    mSyncHeight = ss->mHeight;

    if (ss->mSelectedId >= 0) {
        mNeedSync = TRUE;
        mSyncRowId = ss->mSelectedId;
        mSyncPosition = ss->mPosition;
        mSpecificTop = ss->mViewTop;
        mSyncMode = SYNC_SELECTED_POSITION;
    }
    else if (ss->mFirstId >= 0) {
        SetSelectedPositionInt(AdapterView_INVALID_POSITION);
        // Do this before setting mNeedSync since setNextSelectedPosition looks at mNeedSync
        SetNextSelectedPositionInt(AdapterView_INVALID_POSITION);
        mNeedSync = TRUE;
        mSyncRowId = ss->mFirstId;
        mSyncPosition = ss->mPosition;
        mSpecificTop = ss->mViewTop;
        mSyncMode = SYNC_FIRST_POSITION;
    }

    SetFilterText(ss->mFilter);

    RequestLayout();
}

Boolean AbsListView::AcceptFilter()
{
    //return mTextFilterEnabled && getAdapter() instanceof Filterable &&
    //        ((Filterable) getAdapter()).getFilter() != NULL;
    return FALSE;
}

/**
 * Sets the initial value for the text filter.
 * @param filterText The text to use for the filter.
 *
 * @see #setTextFilterEnabled
 */
ECode AbsListView::SetFilterText(
    /* [in] */ const String& filterText)
{
    // TODO: Should we check for acceptFilter()?
    if (mTextFilterEnabled && !filterText.IsNullOrEmpty()) {
        //CreateTextFilter(FALSE);
        //// This is going to call our listener onTextChanged, but we might not
        //// be ready to bring up a window yet
        //mTextFilter.setText(filterText);
        //mTextFilter.setSelection(filterText.length());
        //if (mAdapter instanceof Filterable) {
        //    // if mPopup is non-NULL, then onTextChanged will do the filtering
        //    if (mPopup == NULL) {
        //        Filter f = ((Filterable) mAdapter).getFilter();
        //        f.filter(filterText);
        //    }
        //    // Set filtered to TRUE so we will display the filter window when our main
        //    // window is ready
        //    mFiltered = TRUE;
        //    mDataSetObserver.clearSavedState();
        //}
    }
    return NOERROR;
}

/**
 * Returns the list's text filter, if available.
 * @return the list's text filter or NULL if filtering isn't enabled
 */
AutoPtr<ICharSequence> AbsListView::GetTextFilter()
{
    //if (mTextFilterEnabled && mTextFilter != NULL) {
    //    return mTextFilter.getText();
    //}
    return NULL;
}

//@Override
void AbsListView::OnFocusChanged(
    /* [in] */ Boolean gainFocus,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    AdapterView::OnFocusChanged(gainFocus, direction, previouslyFocusedRect);
    if (gainFocus && mSelectedPosition < 0 && !IsInTouchMode()) {
        ResurrectSelection();
    }
}

//@Override
ECode AbsListView::RequestLayout()
{
    if (!mBlockLayoutRequests && !mInLayout) {
        AdapterView::RequestLayout();
    }

    return NOERROR;
}

/**
 * The list is empty. Clear everything out.
 */
void AbsListView::ResetList()
{
    RemoveAllViewsInLayout();
    mFirstPosition = 0;
    mDataChanged = FALSE;
    mNeedSync = FALSE;
    mOldSelectedPosition = AdapterView_INVALID_POSITION;
    mOldSelectedRowId = AdapterView_INVALID_ROW_ID;
    SetSelectedPositionInt(AdapterView_INVALID_POSITION);
    SetNextSelectedPositionInt(AdapterView_INVALID_POSITION);
    mSelectedTop = 0;
    mSelectorRect->SetEmpty();
    Invalidate();
}

//@Override
Int32 AbsListView::ComputeVerticalScrollExtent()
{
    Int32 count = GetChildCount();
    if (count > 0) {
        if (mSmoothScrollbarEnabled) {
            Int32 extent = count * 100;

            AutoPtr<IView> view = GetChildAt(0);
            Int32 top, height;
            view->GetTop(&top);
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
        else {
            return 1;
        }
    }
    return 0;
}

//@Override
Int32 AbsListView::ComputeVerticalScrollOffset()
{
    Int32 firstPosition = mFirstPosition;
    Int32 childCount = GetChildCount();
    if (firstPosition >= 0 && childCount > 0) {
        if (mSmoothScrollbarEnabled) {
            AutoPtr<IView> view = GetChildAt(0);
            Int32 top, height;
            view->GetTop(&top);
            view->GetHeight(&height);
            if (height > 0) {
                return Math::Max(firstPosition * 100 - (top * 100) / height +
                    (Int32)((Float)mScrollY / GetHeight() * mItemCount * 100), 0);
            }
        }
        else {
            Int32 index;
            Int32 count = mItemCount;
            if (firstPosition == 0) {
                index = 0;
            }
            else if (firstPosition + childCount == count) {
                index = count;
            }
            else {
                index = firstPosition + childCount / 2;
            }
            return (Int32)(firstPosition + childCount * (index / (Float)count));
        }
    }
    return 0;
}

//@Override
Int32 AbsListView::ComputeVerticalScrollRange()
{
    Int32 result;
    if (mSmoothScrollbarEnabled) {
        result = Math::Max(mItemCount * 100, 0);
        if (mScrollY != 0) {
            // Compensate for overscroll
            result += Math::Abs((Int32)((Float) mScrollY / GetHeight() * mItemCount * 100));
        }
    }
    else {
        result = mItemCount;
    }

    return result;
}

//@Override
Float AbsListView::GetTopFadingEdgeStrength()
{
    Int32 count = GetChildCount();
    Float fadeEdge = AdapterView::GetTopFadingEdgeStrength();
    if (count == 0) {
        return fadeEdge;
    }
    else {
        if (mFirstPosition > 0) {
            return 1.0f;
        }

        Int32 top;
        GetChildAt(0)->GetTop(&top);
        Float fadeLength = (Float)GetVerticalFadingEdgeLength();
        return top < mPaddingTop ? (Float)-(top - mPaddingTop) / fadeLength : fadeEdge;
    }
}

//@Override
Float AbsListView::GetBottomFadingEdgeStrength()
{
    Int32 count = GetChildCount();
    Float fadeEdge = AdapterView::GetBottomFadingEdgeStrength();
    if (count == 0) {
        return fadeEdge;
    }
    else {
        if (mFirstPosition + count - 1 < mItemCount - 1) {
            return 1.0f;
        }

        Int32 bottom;
        GetChildAt(count - 1)->GetBottom(&bottom);

        Int32 height = GetHeight();
        Float fadeLength = (Float)GetVerticalFadingEdgeLength();
        return bottom > height - mPaddingBottom ?
            (Float)(bottom - height + mPaddingBottom) / fadeLength : fadeEdge;
    }
}

//@Override
void AbsListView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    if (mSelector == NULL) {
        UseDefaultSelector();
    }
    CRect* listPadding = (CRect*)mListPadding.Get();
    listPadding->mLeft = mSelectionLeftPadding + mPaddingLeft;
    listPadding->mTop = mSelectionTopPadding + mPaddingTop;
    listPadding->mRight = mSelectionRightPadding + mPaddingRight;
    listPadding->mBottom = mSelectionBottomPadding + mPaddingBottom;
}

/**
 * Subclasses should NOT override this method but
 *  {@link #layoutChildren()} instead.
 */
//@Override
void AbsListView::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    AdapterView::OnLayout(changed, l, t, r, b);
    mInLayout = TRUE;
    if (changed) {
        Int32 childCount = GetChildCount();
        for (Int32 i = 0; i < childCount; i++) {
            GetChildAt(i)->ForceLayout();
        }
        mRecycler->MarkChildrenDirty();
    }

    LayoutChildren();
    mInLayout = FALSE;

    mOverscrollMax = (b - t) / OVERSCROLL_LIMIT_DIVISOR;
}

/**
 * @hide
 */
//@Override
Boolean AbsListView::SetFrame(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    Boolean changed = AdapterView::SetFrame(left, top, right, bottom);

    if (changed) {
        // Reposition the popup when the frame has changed. This includes
        // translating the widget, not just changing its dimension. The
        // filter popup needs to follow the widget.
        Boolean visible = GetWindowVisibility() == View::VISIBLE;
        if (mFiltered && visible /*&& mPopup != NULL && mPopup.isShowing()*/) {
            PositionPopup();
        }
    }

    return changed;
}

void AbsListView::LayoutChildren()
{
}

void AbsListView::UpdateScrollIndicators()
{
    if (mScrollUp != NULL) {
        Boolean canScrollUp;
        // 0th element is not visible
        canScrollUp = mFirstPosition > 0;

        // ... Or top of 0th element is not visible
        if (!canScrollUp) {
            if (GetChildCount() > 0) {
                Int32 top;
                GetChildAt(0)->GetTop(&top);
                canScrollUp = top < mListPadding->mTop;
            }
        }

        mScrollUp->SetVisibility(canScrollUp ? View::VISIBLE : View::INVISIBLE);
    }

    if (mScrollDown != NULL) {
        Boolean canScrollDown;
        Int32 count = GetChildCount();

        // Last item is not visible
        canScrollDown = (mFirstPosition + count) < mItemCount;

        // ... Or bottom of the last element is not visible
        if (!canScrollDown && count > 0) {
            Int32 bottom;
            GetChildAt(count - 1)->GetBottom(&bottom);
            canScrollDown = bottom > mBottom - mListPadding->mBottom;
        }

        mScrollDown->SetVisibility(canScrollDown ? View::VISIBLE : View::INVISIBLE);
    }
}

//@Override
//@ViewDebug.ExportedProperty
AutoPtr<IView> AbsListView::GetSelectedView()
{
    if (mItemCount > 0 && mSelectedPosition >= 0) {
        return GetChildAt(mSelectedPosition - mFirstPosition);
    }
    else {
        return AutoPtr<IView>(NULL);
    }
}

/**
 * List padding is the maximum of the normal view's padding and the padding of the selector.
 *
 * @see android.view.View#getPaddingTop()
 * @see #getSelector()
 *
 * @return The top list padding.
 */
Int32 AbsListView::GetListPaddingTop()
{
    return mListPadding->mTop;
}

/**
 * List padding is the maximum of the normal view's padding and the padding of the selector.
 *
 * @see android.view.View#getPaddingBottom()
 * @see #getSelector()
 *
 * @return The bottom list padding.
 */
Int32 AbsListView::GetListPaddingBottom()
{
    return mListPadding->mBottom;
}

/**
 * List padding is the maximum of the normal view's padding and the padding of the selector.
 *
 * @see android.view.View#getPaddingLeft()
 * @see #getSelector()
 *
 * @return The left list padding.
 */
Int32 AbsListView::GetListPaddingLeft()
{
    return mListPadding->mLeft;
}

/**
 * List padding is the maximum of the normal view's padding and the padding of the selector.
 *
 * @see android.view.View#getPaddingRight()
 * @see #getSelector()
 *
 * @return The right list padding.
 */
Int32 AbsListView::GetListPaddingRight()
{
    return mListPadding->mRight;
}

/**
 * Get a view and have it show the data associated with the specified
 * position. This is called when we have already discovered that the view is
 * not available for reuse in the recycle bin. The only choices left are
 * converting an old view or making a new one.
 *
 * @param position The position to display
 * @param isScrap Array of at least 1 Boolean, the first entry will become TRUE if
 *                the returned view was taken from the scrap heap, FALSE if otherwise.
 *
 * @return A view displaying the data associated with the specified position
 */
AutoPtr<IView> AbsListView::ObtainView(
    /* [in] */ Int32 position,
    /* [in] */ Boolean* isScrap)
{
    isScrap[0] = FALSE;
    AutoPtr<IView> scrapView = mRecycler->GetScrapView(position);
    AutoPtr<IView> child;
    if (scrapView != NULL) {
        //if (ViewDebug.TRACE_RECYCLER) {
        //    ViewDebug.trace(scrapView, ViewDebug.RecyclerTraceType.RECYCLE_FROM_SCRAP_HEAP,
        //            position, -1);
        //}

        mAdapter->GetView(
            position, scrapView, (IViewGroup*)this->Probe(EIID_IViewGroup),
            (IView**)&child);

        //if (ViewDebug.TRACE_RECYCLER) {
        //    ViewDebug.trace(child, ViewDebug.RecyclerTraceType.BIND_VIEW,
        //            position, getChildCount());
        //}

        if (child != scrapView) {
            mRecycler->AddScrapView(scrapView);
            if (mCacheColorHint != 0) {
                child->SetDrawingCacheBackgroundColor(mCacheColorHint);
            }

            //if (ViewDebug.TRACE_RECYCLER) {
            //    ViewDebug.trace(scrapView, ViewDebug.RecyclerTraceType.MOVE_TO_SCRAP_HEAP,
            //            position, -1);
            //}
        }
        else {
            isScrap[0] = TRUE;
            child->DispatchFinishTemporaryDetach();
        }
    }
    else {
        mAdapter->GetView(
            position, NULL, (IViewGroup*)this->Probe(EIID_IViewGroup),
            (IView**)&child);
        if (mCacheColorHint != 0) {
            child->SetDrawingCacheBackgroundColor(mCacheColorHint);
        }

        //if (ViewDebug.TRACE_RECYCLER) {
        //    ViewDebug.trace(child, ViewDebug.RecyclerTraceType.NEW_VIEW,
        //            position, getChildCount());
        //}
    }

    return child;
}

void AbsListView::PositionSelector(
    /* [in] */ IView* sel)
{
    assert(sel);
    View* selView = (View*)sel->Probe(EIID_View);

    CRect* selectorRect = (CRect*)mSelectorRect.Get();
    selectorRect->Set(
        selView->GetLeft(), selView->GetTop(),
        selView->GetRight(), selView->GetBottom());

    PositionSelector(
        selectorRect->mLeft, selectorRect->mTop,
        selectorRect->mRight, selectorRect->mBottom);

    if (selView->IsEnabled() != mIsChildViewEnabled) {
        mIsChildViewEnabled = !mIsChildViewEnabled;
        RefreshDrawableState();
    }
}

void AbsListView::PositionSelector(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    mSelectorRect->Set(
        l - mSelectionLeftPadding, t - mSelectionTopPadding,
        r + mSelectionRightPadding, b + mSelectionBottomPadding);
}

//@Override
void AbsListView::DispatchDraw(
    /* [in] */ ICanvas* canvas)
{
    Int32 saveCount = 0;
    Boolean clipToPadding = (mGroupFlags & CLIP_TO_PADDING_MASK) == CLIP_TO_PADDING_MASK;
    if (clipToPadding) {
        canvas->Save(&saveCount);

        Boolean result;
        canvas->ClipRectEx6(
            mScrollX + mPaddingLeft, mScrollY + mPaddingTop,
            mScrollX + mRight - mLeft - mPaddingRight,
            mScrollY + mBottom - mTop - mPaddingBottom,
            &result);
        mGroupFlags &= ~CLIP_TO_PADDING_MASK;
    }

    if (!mDrawSelectorOnTop) {
        DrawSelector(canvas);
    }

    AdapterView::DispatchDraw(canvas);

    if (mDrawSelectorOnTop) {
        DrawSelector(canvas);
    }

    if (clipToPadding) {
        canvas->RestoreToCount(saveCount);
        mGroupFlags |= CLIP_TO_PADDING_MASK;
    }
}

//@Override
void AbsListView::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    if (GetChildCount() > 0) {
        mDataChanged = TRUE;
        RememberSyncState();
    }

    if (mFastScroller != NULL) {
        mFastScroller->OnSizeChanged(w, h, oldw, oldh);
    }
}

/**
 * @return True if the current touch mode requires that we draw the selector in the pressed
 *         state.
 */
Boolean AbsListView::TouchModeDrawsInPressedState()
{
    // FIXME use isPressed for this
    switch (mTouchMode) {
    case TOUCH_MODE_TAP:
    case TOUCH_MODE_DONE_WAITING:
        return TRUE;
    default:
        return FALSE;
    }
}

/**
 * Indicates whether this view is in a state where the selector should be drawn. This will
 * happen if we have focus but are not in touch mode, or we are in the middle of displaying
 * the pressed state for an item.
 *
 * @return True if the selector should be shown
 */
Boolean AbsListView::ShouldShowSelector()
{
    return (HasFocus() && !IsInTouchMode()) || TouchModeDrawsInPressedState();
}

void AbsListView::DrawSelector(
    /* [in] */ ICanvas* canvas)
{
    //TODO:
    //
    if (mSelector == NULL) {
        return;
    }

    Boolean isEmpty = TRUE;
    if (mSelectorRect != NULL) {
        mSelectorRect->IsEmpty(&isEmpty);
    }

    if (ShouldShowSelector() && !isEmpty) {
        mSelector->SetBoundsEx((IRect*)mSelectorRect.Get());
        mSelector->Draw(canvas);
    }
}

/**
 * Controls whether the selection highlight drawable should be drawn on top of the item or
 * behind it.
 *
 * @param onTop If TRUE, the selector will be drawn on the item it is highlighting. The default
 *        is FALSE.
 *
 * @attr ref android.R.styleable#AbsListView_drawSelectorOnTop
 */
ECode AbsListView::SetDrawSelectorOnTop(
    /* [in] */ Boolean onTop)
{
    mDrawSelectorOnTop = onTop;

    return NOERROR;
}

/**
 * Set a Drawable that should be used to highlight the currently selected item.
 *
 * @param resID A Drawable resource to use as the selection highlight.
 *
 * @attr ref android.R.styleable#AbsListView_listSelector
 */
ECode AbsListView::SetSelector(
    /* [in] */ Int32 resID)
{
    AutoPtr<IDrawable> drawable;
    GetResources()->GetDrawable(resID, (IDrawable**)&drawable);

    return SetSelector(drawable);
}

ECode AbsListView::SetSelector(
    /* [in] */ IDrawable* sel)
{
    if (mSelector != NULL) {
        mSelector->SetCallback(NULL);
        UnscheduleDrawable(mSelector);
    }

    mSelector = sel;
    //TODO:
    //
    if (sel == NULL) {
        return NOERROR;
    }

    AutoPtr<CRect> padding;
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&padding)));

    Boolean result;
    sel->GetPadding((IRect*)padding.Get(), &result);

    mSelectionLeftPadding = padding->mLeft;
    mSelectionTopPadding = padding->mTop;
    mSelectionRightPadding = padding->mRight;
    mSelectionBottomPadding = padding->mBottom;
    sel->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    sel->SetState(GetDrawableState(), &result);

    return NOERROR;
}

/**
 * Returns the selector {@link android.graphics.drawable.Drawable} that is used to draw the
 * selection in the list.
 *
 * @return the drawable used to display the selector
 */
AutoPtr<IDrawable> AbsListView::GetSelector()
{
    return mSelector;
}

/**
 * Sets the selector state to "pressed" and posts a CheckForKeyLongPress to see if
 * this is a Int64 press.
 */
void AbsListView::KeyPressed()
{
    if (!IsEnabled() || !IsClickable()) {
        return;
    }

    IDrawable* selector = mSelector.Get();
    CRect* selectorRect = (CRect*)mSelectorRect.Get();
    if (selector != NULL && (IsFocused() || TouchModeDrawsInPressedState())
        && selectorRect != NULL && !selectorRect->IsEmpty()) {
        AutoPtr<IView> v = GetChildAt(mSelectedPosition - mFirstPosition);
        if (v != NULL) {
            Boolean hasfoucsable;
            v->HasFocusable(&hasfoucsable);
            if (hasfoucsable) {
                return;
            }

            v->SetPressed(TRUE);
        }

        SetPressed(TRUE);

        Boolean longClickable = IsLongClickable();
        AutoPtr<IDrawable> d;
        selector->GetCurrent((IDrawable**)&d);
        //if (d != NULL && d instanceof TransitionDrawable) {
        //    if (longClickable) {
        //        ((TransitionDrawable) d).startTransition(
        //                ViewConfiguration.getLongPressTimeout());
        //    } else {
        //        ((TransitionDrawable) d).resetTransition();
        //    }
        //}

        if (longClickable && !mDataChanged) {
            if (mPendingCheckForKeyLongPress == NULL) {
                mPendingCheckForKeyLongPress = new CheckForKeyLongPress(this);
            }

            mPendingCheckForKeyLongPress->RememberWindowAttachCount();
            PostDelayed(mPendingCheckForKeyLongPress, ViewConfiguration::GetLongPressTimeout());
        }
    }
}

ECode AbsListView::SetScrollIndicators(
    /* [in] */ IView* up,
    /* [in] */ IView* down)
{
    mScrollUp = up;
    mScrollDown = down;

    return NOERROR;
}

//@Override
ECode AbsListView::DrawableStateChanged()
{
    AdapterView::DrawableStateChanged();
    if (mSelector != NULL) {
        Boolean result;
        mSelector->SetState(GetDrawableState(), &result);
    }

    return NOERROR;
}

//@Override
ECode AbsListView::OnCreateDrawableState(
    /* [in] */ Int32 extraSpace,
    /* [out] */ ArrayOf<Int32>** drawableState)
{
    VALIDATE_NOT_NULL(drawableState);

    // If the child view is enabled then do the default behavior.
    if (mIsChildViewEnabled) {
        // Common case
        return AdapterView::OnCreateDrawableState(extraSpace, drawableState);
    }

    // The selector uses this View's drawable state. The selected child view
    // is disabled, so we need to remove the enabled state from the drawable
    // states.
    Int32 enabledState = 0;// = ENABLED_STATE_SET[0];

    // If we don't have any extra space, it will return one of the static state arrays,
    // and clearing the enabled state on those arrays is a bad thing!  If we specify
    // we need extra space, it will create+copy into a new array that safely mutable.
    //
    ArrayOf<Int32>* state = NULL;
    AdapterView::OnCreateDrawableState(extraSpace + 1, &state);
    Int32 enabledPos = -1;
    for (Int32 i = state->GetLength() - 1; i >= 0; i--) {
        if ((*state)[i] == enabledState) {
            enabledPos = i;
            break;
        }
    }

    // Remove the enabled state
    if (enabledPos >= 0) {
        //System.arraycopy(state, enabledPos + 1, state, enabledPos,
        //        state.length - enabledPos - 1);
        state->Replace(
            enabledPos, state->GetPayload() + enabledPos + 1,
            state->GetLength() - enabledPos - 1);
    }

    *drawableState = state;

    return NOERROR;
}

//@Override
Boolean AbsListView::VerifyDrawable(
    /* [in] */ IDrawable* dr)
{
    return mSelector.Get() == dr || AdapterView::VerifyDrawable(dr);
}

//@Override
void AbsListView::OnAttachedToWindow()
{
    AdapterView::OnAttachedToWindow();

    AutoPtr<IViewTreeObserver> treeObserver = GetViewTreeObserver();
    if (treeObserver != NULL) {
        //treeObserver->AddOnTouchModeChangeListener(this);
        //if (mTextFilterEnabled && mPopup != NULL && !mGlobalLayoutListenerAddedFilter) {
        //    treeObserver->AddOnGlobalLayoutListener(this);
        //}
    }
}

//@Override
void AbsListView::OnDetachedFromWindow()
{
    AdapterView::OnDetachedFromWindow();

    // Dismiss the popup in case onSaveInstanceState() was not invoked
    DismissPopup();

    // Detach any view left in the scrap heap
    mRecycler->Clear();

    AutoPtr<IViewTreeObserver> treeObserver = GetViewTreeObserver();
    if (treeObserver != NULL) {
        //treeObserver->RemoveOnTouchModeChangeListener(this);
        //if (mTextFilterEnabled && mPopup != NULL) {
        //    treeObserver->RemoveGlobalOnLayoutListener(this);
        //    mGlobalLayoutListenerAddedFilter = FALSE;
        //}
    }
}

//@Override
ECode AbsListView::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    AdapterView::OnWindowFocusChanged(hasWindowFocus);

    Int32 touchMode = IsInTouchMode() ? TOUCH_MODE_ON : TOUCH_MODE_OFF;

    if (!hasWindowFocus) {
        SetChildrenDrawingCacheEnabled(FALSE);
        if (mFlingRunnable != NULL) {
            RemoveCallbacks(mFlingRunnable);
            // let the fling runnable report it's new state which
            // should be idle
            mFlingRunnable->EndFling();
            if (mScrollY != 0) {
                mScrollY = 0;
                FinishGlows();
                Invalidate();
            }
        }
        // Always hide the type filter
        DismissPopup();

        if (touchMode == TOUCH_MODE_OFF) {
            // Remember the last selected element
            mResurrectToPosition = mSelectedPosition;
        }
    }
    else {
        if (mFiltered && !mPopupHidden) {
            // Show the type filter only if a filter is in effect
            ShowPopup();
        }

        // If we changed touch mode since the last time we had focus
        if (touchMode != mLastTouchMode && mLastTouchMode != TOUCH_MODE_UNKNOWN) {
            // If we come back in trackball mode, we bring the selection back
            if (touchMode == TOUCH_MODE_OFF) {
                // This will trigger a layout
                ResurrectSelection();

            // If we come back in touch mode, then we want to hide the selector
            }
            else {
                HideSelector();
                mLayoutMode = LAYOUT_NORMAL;
                LayoutChildren();
            }
        }
    }

    mLastTouchMode = touchMode;

    return NOERROR;
}

/**
 * Creates the ContextMenuInfo returned from {@link #getContextMenuInfo()}. This
 * methods knows the view, position and ID of the item that received the
 * Int64 press.
 *
 * @param view The view that received the Int64 press.
 * @param position The position of the item that received the Int64 press.
 * @param id The ID of the item that received the Int64 press.
 * @return The extra information that should be returned by
 *         {@link #getContextMenuInfo()}.
 */
AutoPtr<IContextMenuInfo> AbsListView::CreateContextMenuInfo(
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    return new AdapterContextMenuInfo(view, position, id);
}

Boolean AbsListView::PerformLongPress(
    /* [in] */ IView* child,
    /* [in] */ Int32 longPressPosition,
    /* [in] */ Int64 longPressId)
{
    Boolean handled = FALSE;

    if (mOnItemLongClickListener != NULL) {
        mOnItemLongClickListener->OnItemLongClick(
            (IAdapterView*)this->Probe(EIID_IAdapterView),
            child, longPressPosition, longPressId, &handled);
    }

    if (!handled) {
        mContextMenuInfo = CreateContextMenuInfo(
            child, longPressPosition, longPressId);
        handled = AdapterView::ShowContextMenuForChild(
            (IView*)this->Probe(EIID_IView));
    }

    if (handled) {
        PerformHapticFeedback(HapticFeedbackConstants::LONG_PRESS);
    }

    return handled;
}

//@Override
AutoPtr<IContextMenuInfo> AbsListView::GetContextMenuInfo()
{
    return mContextMenuInfo;
}

//@Override
Boolean AbsListView::ShowContextMenuForChild(
    /* [in] */ IView* originalView)
{
    Int32 longPressPosition = GetPositionForView(originalView);
    if (longPressPosition >= 0) {
        Int64 longPressId;
        mAdapter->GetItemId(longPressPosition, &longPressId);

        Boolean handled = FALSE;
        if (mOnItemLongClickListener != NULL) {
            mOnItemLongClickListener->OnItemLongClick(
                (IAdapterView*)this->Probe(EIID_IAdapterView),
                originalView, longPressPosition, longPressId, &handled);
        }

        if (!handled) {
            AutoPtr<IView> child = GetChildAt(
                longPressPosition - mFirstPosition);

            mContextMenuInfo = CreateContextMenuInfo(
                child, longPressPosition, longPressId);

            handled = AdapterView::ShowContextMenuForChild(originalView);
        }

        return handled;
    }
    return FALSE;
}

//@Override
Boolean AbsListView::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return FALSE;
}

//@Override
Boolean AbsListView::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    switch (keyCode) {
    case KeyEvent_KEYCODE_DPAD_CENTER:
    case KeyEvent_KEYCODE_ENTER:
        {
            if (!IsEnabled()) {
                return TRUE;
            }

            Int32 count = 0;
            if (mAdapter != NULL) {
                mAdapter->GetCount(&count);
            }

            if (IsClickable() && IsPressed() &&
                mSelectedPosition >= 0 && mSelectedPosition < count) {
                    AutoPtr<IView> view =
                        GetChildAt(mSelectedPosition - mFirstPosition);
                    if (view != NULL) {
                        PerformItemClick(
                            view, mSelectedPosition, mSelectedRowId);
                        view->SetPressed(FALSE);
                    }
                    SetPressed(FALSE);
                    return TRUE;
            }
        }
        break;
    }

    return AdapterView::OnKeyUp(keyCode, event);
}

//@Override
void AbsListView::DispatchSetPressed(
    /* [in] */ Boolean pressed)
{
    // Don't dispatch setPressed to our children. We call setPressed on ourselves to
    // get the selector in the right state, but we don't want to press each child.
}

/**
 * Maps a point to a position in the list.
 *
 * @param x X in local coordinate
 * @param y Y in local coordinate
 * @return The position of the item which contains the specified point, or
 *         {@link #AdapterView_INVALID_POSITION} if the point does not intersect an item.
 */
Int32 AbsListView::PointToPosition(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    CRect* frame = (CRect*)mTouchFrame.Get();
    if (frame == NULL) {
        assert(SUCCEEDED(CRect::New((IRect**)&mTouchFrame)));
        frame = (CRect*)mTouchFrame.Get();
    }

    Int32 count = GetChildCount();
    for (Int32 i = count - 1; i >= 0; i--) {
        AutoPtr<IView> child = GetChildAt(i);
        Int32 visibility;
        child->GetVisibility(&visibility);
        if (visibility == View::VISIBLE) {
            child->GetHitRect(frame);
            Boolean isContains;
            frame->Contains(x, y, &isContains);
            if (isContains) {
                return mFirstPosition + i;
            }
        }
    }

    return AdapterView_INVALID_POSITION;
}

/**
 * Maps a point to a the rowId of the item which intersects that point.
 *
 * @param x X in local coordinate
 * @param y Y in local coordinate
 * @return The rowId of the item which contains the specified point, or {@link #AdapterView_INVALID_ROW_ID}
 *         if the point does not intersect an item.
 */
Int64 AbsListView::PointToRowId(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    Int32 position = PointToPosition(x, y);
    Int64 itemId = AdapterView_INVALID_ROW_ID;

    if (position >= 0) {
        mAdapter->GetItemId(position, &itemId);
    }

    return itemId;
}

Boolean AbsListView::StartScrollIfNeeded(
    /* [in] */ Int32 deltaY)
{
    // Check if we have moved far enough that it looks more like a
    // scroll than a tap
    Int32 distance = Math::Abs(deltaY);
    Boolean overscroll = mScrollY != 0;
    if (overscroll || distance > mTouchSlop) {
        CreateScrollingCache();
        mTouchMode = overscroll ? TOUCH_MODE_OVERSCROLL : TOUCH_MODE_SCROLL;
        mMotionCorrection = deltaY;

        //final Handler handler = getHandler();
        //// Handler should not be NULL unless the AbsListView is not attached to a
        //// window, which would make it very hard to scroll it... but the monkeys
        //// say it's possible.
        //if (handler != NULL) {
        //    handler.removeCallbacks(mPendingCheckForLongPress);
        //}

        SetPressed(FALSE);
        AutoPtr<IView> motionView = GetChildAt(mMotionPosition - mFirstPosition);
        if (motionView != NULL) {
            motionView->SetPressed(FALSE);
        }

        ReportScrollStateChange(OnScrollListener_SCROLL_STATE_TOUCH_SCROLL);
        // Time to start stealing events! Once we've stolen them, don't let anyone
        // steal from us
        RequestDisallowInterceptTouchEvent(TRUE);

        return TRUE;
    }

    return FALSE;
}

ECode AbsListView::OnTouchModeChanged(
    /* [in] */ Boolean isInTouchMode)
{
    if (isInTouchMode) {
        // Get rid of the selection when we enter touch mode
        HideSelector();
        // Layout, but only if we already have done so previously.
        // (Otherwise may clobber a LAYOUT_SYNC layout that was requested to restore
        // state.)
        if (GetHeight() > 0 && GetChildCount() > 0) {
            // We do not lose focus initiating a touch (since AbsListView is focusable in
            // touch mode). Force an initial layout to get rid of the selection.
            LayoutChildren();
        }
    }
    else {
        Int32 touchMode = mTouchMode;
        if (touchMode == TOUCH_MODE_OVERSCROLL || touchMode == TOUCH_MODE_OVERFLING) {
            if (mFlingRunnable != NULL) {
                mFlingRunnable->EndFling();
            }

            if (mScrollY != 0) {
                mScrollY = 0;
                FinishGlows();
                Invalidate();
            }
        }
    }

    return NOERROR;
}

//@Override
Boolean AbsListView::OnTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    //printf("AbsListView::OnTouchEvent--------------1\n");
    if (!IsEnabled()) {
        // A disabled view that is clickable still consumes the touch
        // events, it just doesn't respond to them.
        return IsClickable() || IsLongClickable();
    }

    if (mFastScroller != NULL) {
        Boolean intercepted = mFastScroller->OnTouchEvent(ev);
        if (intercepted) {
            return TRUE;
        }
    }

    Int32 action;
    ev->GetAction(&action);

    AutoPtr<IView> v;
    Int32 deltaY;

    if (mVelocityTracker == NULL) {
        mVelocityTracker = VelocityTracker::Obtain();
    }
    mVelocityTracker->AddMovement(ev);

    //printf("AbsListView::OnTouchEvent action = 0x%08x--------------2\n", action);
    switch (action & MotionEvent_ACTION_MASK) {
    case MotionEvent_ACTION_DOWN:
        {
            //printf("AbsListView::OnTouchEvent--------------3\n");
            switch (mTouchMode) {
            case TOUCH_MODE_OVERFLING:
                {
                    mFlingRunnable->EndFling();
                    mTouchMode = TOUCH_MODE_OVERSCROLL;
                    Float fy;
                    ev->GetY(&fy);
                    mMotionY = mLastY = (Int32)fy;
                    mMotionCorrection = 0;
                    ev->GetPointerId(0, &mActivePointerId);
                }
                break;
            default:
                {
                    ev->GetPointerId(0, &mActivePointerId);
                    Float fx, fy;
                    ev->GetX(&fx);
                    ev->GetY(&fy);
                    Int32 x = (Int32)fx;
                    Int32 y = (Int32)fy;
                    Int32 motionPosition = PointToPosition(x, y);
                    if (!mDataChanged) {
                        Boolean isEnabled;
                        ((IListAdapter*)(GetAdapter()->Probe(EIID_IListAdapter)))
                            ->IsEnabled(motionPosition, &isEnabled);
                        if ((mTouchMode != TOUCH_MODE_FLING)
                            && (motionPosition >= 0) && isEnabled) {
                            // User clicked on an actual view (and was not
                            // stopping a fling). It might be a click or a scroll.
                            // Assume it is a click until proven otherwise
                            //
                            mTouchMode = TOUCH_MODE_DOWN;
                            // FIXME Debounce
                            if (mPendingCheckForTap == NULL) {
                                AutoPtr<CheckForTap> temp = new CheckForTap(this);
                                mPendingCheckForTap =
                                    (IRunnable*)temp->Probe(EIID_IRunnable);
                            }
                            PostDelayed(
                                mPendingCheckForTap,
                                ViewConfiguration::GetTapTimeout());
                        }
                        else {
                            Int32 edgeFlags;
                            ev->GetEdgeFlags(&edgeFlags);
                            if (edgeFlags != 0 && motionPosition < 0) {
                                // If we couldn't find a view to click on,
                                // but the down event was touching
                                // the edge, we will bail out and try again.
                                // This allows the edge correcting
                                // code in ViewRoot to try to find a nearby view to select
                                return FALSE;
                            }

                            if (mTouchMode == TOUCH_MODE_FLING) {
                                // Stopped a fling. It is a scroll.
                                CreateScrollingCache();
                                mTouchMode = TOUCH_MODE_SCROLL;
                                mMotionCorrection = 0;
                                motionPosition = FindMotionRow(y);
                                ReportScrollStateChange(
                                    OnScrollListener_SCROLL_STATE_TOUCH_SCROLL);
                            }
                        }
                    }

                    if (motionPosition >= 0) {
                        // Remember where the motion event started
                        v = GetChildAt(motionPosition - mFirstPosition);
                        v->GetTop(&mMotionViewOriginalTop);
                    }
                    mMotionX = x;
                    mMotionY = y;
                    mMotionPosition = motionPosition;
                    mLastY = Math::INT32_MIN_VALUE;
                }
                break;
            }
        }
        break;
    case MotionEvent_ACTION_MOVE:
        {
            Int32 pointerIndex;
            ev->FindPointerIndex(mActivePointerId, &pointerIndex);
            Float fy;
            ev->GetYEx(pointerIndex, &fy);
            Int32 y = (Int32)fy;
            deltaY = y - mMotionY;

            switch (mTouchMode) {
            case TOUCH_MODE_DOWN:
            case TOUCH_MODE_TAP:
            case TOUCH_MODE_DONE_WAITING:
                // Check if we have moved far enough that it looks more like a
                // scroll than a tap
                StartScrollIfNeeded(deltaY);
                break;
            case TOUCH_MODE_SCROLL:
                if (PROFILE_SCROLLING) {
                    if (!mScrollProfilingStarted) {
                        //Debug.startMethodTracing("AbsListViewScroll");
                        mScrollProfilingStarted = TRUE;
                    }
                }

                if (y != mLastY) {
                    // We may be here after stopping a fling and continuing to scroll.
                    // If so, we haven't disallowed intercepting touch events yet.
                    // Make sure that we do so in case we're in a parent that can intercept.
                    if ((mGroupFlags & FLAG_DISALLOW_INTERCEPT) == 0 &&
                        Math::Abs(deltaY) > mTouchSlop) {
                        RequestDisallowInterceptTouchEvent(TRUE);
                    }

                    Int32 rawDeltaY = deltaY;
                    deltaY -= mMotionCorrection;
                    Int32 incrementalDeltaY = (mLastY != Math::INT32_MIN_VALUE)
                        ? (y - mLastY) : deltaY;

                    Int32 motionIndex;
                    if (mMotionPosition >= 0) {
                        motionIndex = mMotionPosition - mFirstPosition;
                    }
                    else {
                        // If we don't have a motion position that we can reliably track,
                        // pick something in the middle to make a best guess at things below.
                        motionIndex = GetChildCount() / 2;
                    }

                    Int32 motionViewPrevTop = 0;
                    AutoPtr<IView> motionView = GetChildAt(motionIndex);
                    if (motionView != NULL) {
                        motionView->GetTop(&motionViewPrevTop);
                    }

                    // No need to do all this work if we're not going to move anyway
                    Boolean atEdge = FALSE;
                    if (incrementalDeltaY != 0) {
                        atEdge = TrackMotionScroll(deltaY, incrementalDeltaY);
                    }

                    // Check to see if we have bumped into the scroll limit
                    motionView = GetChildAt(motionIndex);
                    if (motionView != NULL) {
                        // Check if the top of the motion view is where it is
                        // supposed to be
                        Int32 motionViewRealTop;
                        motionView->GetTop(&motionViewRealTop);
                        if (atEdge) {
                            // Apply overscroll
                            Int32 overscroll = -incrementalDeltaY -
                                (motionViewRealTop - motionViewPrevTop);
                            OverScrollBy(
                                0, overscroll, 0, mScrollY, 0, 0,
                                0, mOverscrollDistance, TRUE);

                            if (Math::Abs(mOverscrollDistance) == Math::Abs(mScrollY)) {
                                // Don't allow overfling if we're at the edge.
                                mVelocityTracker->Clear();
                            }

                            Int32 overscrollMode = GetOverScrollMode();
                            if (overscrollMode == OVER_SCROLL_ALWAYS ||
                                (overscrollMode == OVER_SCROLL_IF_CONTENT_SCROLLS &&
                                !ContentFits())) {
                                mDirection = 0; // Reset when entering overscroll.
                                mTouchMode = TOUCH_MODE_OVERSCROLL;
                                if (rawDeltaY > 0) {
                                    mEdgeGlowTop->OnPull(
                                        (Float)overscroll / GetHeight());
                                    if (!mEdgeGlowBottom->IsFinished()) {
                                        mEdgeGlowBottom->OnRelease();
                                    }
                                }
                                else if (rawDeltaY < 0) {
                                    mEdgeGlowBottom->OnPull(
                                        (Float)overscroll / GetHeight());
                                    if (!mEdgeGlowTop->IsFinished()) {
                                        mEdgeGlowTop->OnRelease();
                                    }
                                }
                            }
                        }
                        mMotionY = y;
                        Invalidate();
                    }
                    mLastY = y;
                }
                break;

            case TOUCH_MODE_OVERSCROLL:
                if (y != mLastY) {
                    Int32 rawDeltaY = deltaY;
                    deltaY -= mMotionCorrection;
                    Int32 incrementalDeltaY =
                        (mLastY != Math::INT32_MIN_VALUE) ? y - mLastY : deltaY;

                    Int32 oldScroll = mScrollY;
                    Int32 newScroll = oldScroll - incrementalDeltaY;
                    Int32 newDirection = y > mLastY ? 1 : -1;

                    if (mDirection == 0) {
                        mDirection = newDirection;
                    }

                    if (mDirection != newDirection) {
                        // Coming back to 'real' list scrolling
                        incrementalDeltaY = -newScroll;
                        mScrollY = 0;

                        // No need to do all this work if we're not going to move anyway
                        if (incrementalDeltaY != 0) {
                            TrackMotionScroll(incrementalDeltaY, incrementalDeltaY);
                        }

                        // Check to see if we are back in
                        AutoPtr<IView> motionView =
                            GetChildAt(mMotionPosition - mFirstPosition);
                        if (motionView != NULL) {
                            mTouchMode = TOUCH_MODE_SCROLL;

                            // We did not scroll the full amount. Treat this essentially like the
                            // start of a new touch scroll
                            Int32 motionPosition = FindClosestMotionRow(y);

                            mMotionCorrection = 0;
                            motionView = GetChildAt(motionPosition - mFirstPosition);
                            motionView->GetTop(&mMotionViewOriginalTop);
                            mMotionY = y;
                            mMotionPosition = motionPosition;
                        }
                    }
                    else {
                        OverScrollBy(0, -incrementalDeltaY, 0, mScrollY, 0, 0,
                                0, mOverscrollDistance, TRUE);
                        Int32 overscrollMode = GetOverScrollMode();
                        if (overscrollMode == OVER_SCROLL_ALWAYS ||
                            (overscrollMode == OVER_SCROLL_IF_CONTENT_SCROLLS &&
                            !ContentFits())) {
                            if (rawDeltaY > 0) {
                                mEdgeGlowTop->OnPull(
                                    (Float) -incrementalDeltaY / GetHeight());
                                if (!mEdgeGlowBottom->IsFinished()) {
                                    mEdgeGlowBottom->OnRelease();
                                }
                            }
                            else if (rawDeltaY < 0) {
                                mEdgeGlowBottom->OnPull(
                                    (Float) -incrementalDeltaY / GetHeight());
                                if (!mEdgeGlowTop->IsFinished()) {
                                    mEdgeGlowTop->OnRelease();
                                }
                            }
                            Invalidate();
                        }

                        if (Math::Abs(mOverscrollDistance) == Math::Abs(mScrollY)) {
                            // Don't allow overfling if we're at the edge.
                            mVelocityTracker->Clear();
                        }
                    }
                    mLastY = y;
                    mDirection = newDirection;
                }
                break;
            default:
                break;
            }
        }
        break;
    case MotionEvent_ACTION_UP:
        {
            //printf("AbsListView::OnTouchEvent--------------5\n");
            switch (mTouchMode) {
            case TOUCH_MODE_DOWN:
            case TOUCH_MODE_TAP:
            case TOUCH_MODE_DONE_WAITING:
                {
                    //printf("AbsListView::OnTouchEvent--------------6\n");
                    Int32 motionPosition = mMotionPosition;
                    AutoPtr<IView> child = GetChildAt(
                        motionPosition - mFirstPosition);
                    Boolean hasFocusable;
                    if (child != NULL && (child->HasFocusable(&hasFocusable),
                        !hasFocusable)) {
                        if (mTouchMode != TOUCH_MODE_DOWN) {
                            child->SetPressed(FALSE);
                        }

                        if (mPerformClick == NULL) {
                            mPerformClick = new AbsListViewPerformClick(this);
                        }

                        AbsListViewPerformClick* performClick = mPerformClick;
                        performClick->mChild = child;
                        performClick->mClickMotionPosition = motionPosition;
                        performClick->RememberWindowAttachCount();

                        mResurrectToPosition = motionPosition;

                        Boolean isEnabled;
                        mAdapter->IsEnabled(motionPosition, &isEnabled);
                        if (mTouchMode == TOUCH_MODE_DOWN || mTouchMode == TOUCH_MODE_TAP) {
                            //final Handler handler = getHandler();
                            //if (handler != NULL) {
                            //    handler.removeCallbacks(mTouchMode == TOUCH_MODE_DOWN ?
                            //            mPendingCheckForTap : mPendingCheckForLongPress);
                            //}

                            mLayoutMode = LAYOUT_NORMAL;
                            if (!mDataChanged && isEnabled) {
                                mTouchMode = TOUCH_MODE_TAP;
                                SetSelectedPositionInt(mMotionPosition);
                                //printf("mMotionPosition = %d\n", mMotionPosition);
                                LayoutChildren();
                                child->SetPressed(TRUE);
                                PositionSelector(child);
                                SetPressed(TRUE);
                                if (mSelector != NULL) {
                                    AutoPtr<IDrawable> d;
                                    mSelector->GetCurrent((IDrawable**)&d);
                                    //if (d != NULL && d instanceof TransitionDrawable) {
                                    //    ((TransitionDrawable) d).resetTransition();
                                    //}
                                }

                                AutoPtr<OnTouchEventRunnable> runnable =
                                    new OnTouchEventRunnable(this, child);
                                runnable->AddRef();
                                PostDelayed(
                                    (IRunnable*)runnable->Probe(EIID_IRunnable),
                                    ViewConfiguration::GetPressedStateDuration());
                            }
                            else {
                                mTouchMode = TOUCH_MODE_REST;
                            }
                            return TRUE;
                        }
                        else if (!mDataChanged && isEnabled) {
                            Post((IRunnable*)mPerformClick->Probe(EIID_IRunnable));
                        }
                    }
                    mTouchMode = TOUCH_MODE_REST;
                }
                break;
            case TOUCH_MODE_SCROLL:
                {
                    //printf("AbsListView::OnTouchEvent--------------7\n");
                    Int32 childCount = GetChildCount();
                    if (childCount > 0) {
                        Int32 firstChildTop;
                        GetChildAt(0)->GetTop(&firstChildTop);
                        Int32 lastChildBottom;
                        GetChildAt(childCount - 1)->GetBottom(&lastChildBottom);

                        Int32 contentTop = mListPadding->mTop;
                        Int32 contentBottom = GetHeight() - mListPadding->mBottom;
                        if (mFirstPosition == 0 && firstChildTop >= contentTop &&
                            mFirstPosition + childCount < mItemCount &&
                            lastChildBottom <= GetHeight() - contentBottom) {
                                mTouchMode = TOUCH_MODE_REST;
                                ReportScrollStateChange(OnScrollListener_SCROLL_STATE_IDLE);
                        }
                        else {
                            mVelocityTracker->ComputeCurrentVelocity(1000, mMaximumVelocity);
                            Int32 initialVelocity = (Int32)mVelocityTracker->GetYVelocity(mActivePointerId);

                            // Fling if we have enough velocity and we aren't at a boundary.
                            // Since we can potentially overfling more than we can overscroll, don't
                            // allow the weird behavior where you can scroll to a boundary then
                            // fling further.
                            if (Math::Abs(initialVelocity) > mMinimumVelocity &&
                                !((mFirstPosition == 0 &&
                                firstChildTop == contentTop - mOverscrollDistance) ||
                                (mFirstPosition + childCount == mItemCount &&
                                lastChildBottom == contentBottom + mOverscrollDistance))) {
                                    if (mFlingRunnable == NULL) {
                                        mFlingRunnable = new FlingRunnable(this);
                                    }
                                    ReportScrollStateChange(OnScrollListener_SCROLL_STATE_FLING);

                                    mFlingRunnable->Start(-initialVelocity);
                            }
                            else {
                                mTouchMode = TOUCH_MODE_REST;
                                ReportScrollStateChange(OnScrollListener_SCROLL_STATE_IDLE);
                            }
                        }
                    }
                    else {
                        mTouchMode = TOUCH_MODE_REST;
                        ReportScrollStateChange(OnScrollListener_SCROLL_STATE_IDLE);
                    }
                }
                break;
            case TOUCH_MODE_OVERSCROLL:
                {
                    //printf("AbsListView::OnTouchEvent--------------8\n");
                    if (mFlingRunnable == NULL) {
                        mFlingRunnable = new FlingRunnable(this);
                    }

                    mVelocityTracker->ComputeCurrentVelocity(1000, mMaximumVelocity);
                    Int32 initialVelocity = (Int32)mVelocityTracker->GetYVelocity(mActivePointerId);

                    ReportScrollStateChange(OnScrollListener_SCROLL_STATE_FLING);
                    if (Math::Abs(initialVelocity) > mMinimumVelocity) {
                        mFlingRunnable->StartOverfling(-initialVelocity);
                    }
                    else {
                        mFlingRunnable->StartSpringback();
                    }
                }
                break;
            default:
                break;
            }

            //printf("AbsListView::OnTouchEvent--------------9\n");
            SetPressed(FALSE);

            if (mEdgeGlowTop != NULL) {
                mEdgeGlowTop->OnRelease();
                mEdgeGlowBottom->OnRelease();
            }

            // Need to redraw since we probably aren't drawing the selector anymore
            Invalidate();

            //final Handler handler = getHandler();
            //if (handler != NULL) {
            //    handler.removeCallbacks(mPendingCheckForLongPress);
            //}

            if (mVelocityTracker != NULL) {
                mVelocityTracker->Recycle();
                mVelocityTracker = NULL;
            }

            mActivePointerId = INVALID_POINTER;

            if (PROFILE_SCROLLING) {
                if (mScrollProfilingStarted) {
                    //Debug.stopMethodTracing();
                    mScrollProfilingStarted = FALSE;
                }
            }
        }
        break;
    case MotionEvent_ACTION_CANCEL:
        {
            switch (mTouchMode) {
            case TOUCH_MODE_OVERSCROLL:
                if (mFlingRunnable == NULL) {
                    mFlingRunnable = new FlingRunnable(this);
                }
                mFlingRunnable->StartSpringback();
                break;

            case TOUCH_MODE_OVERFLING:
                // Do nothing - let it play out.
                break;

            default:
                mTouchMode = TOUCH_MODE_REST;
                SetPressed(FALSE);
                AutoPtr<IView> motionView = GetChildAt(
                    mMotionPosition - mFirstPosition);
                if (motionView != NULL) {
                    motionView->SetPressed(FALSE);
                }
                ClearScrollingCache();

                //final Handler handler = getHandler();
                //if (handler != NULL) {
                //    handler.removeCallbacks(mPendingCheckForLongPress);
                //}

                if (mVelocityTracker != NULL) {
                    mVelocityTracker->Recycle();
                    mVelocityTracker = NULL;
                }
            }

            if (mEdgeGlowTop != NULL) {
                mEdgeGlowTop->OnRelease();
                mEdgeGlowBottom->OnRelease();
            }
            mActivePointerId = INVALID_POINTER;
        }
        break;

    case MotionEvent_ACTION_POINTER_UP:
        {
            OnSecondaryPointerUp(ev);
            Int32 x = mMotionX;
            Int32 y = mMotionY;
            Int32 motionPosition = PointToPosition(x, y);
            if (motionPosition >= 0) {
                // Remember where the motion event started
                v = GetChildAt(motionPosition - mFirstPosition);
                v->GetTop(&mMotionViewOriginalTop);
                mMotionPosition = motionPosition;
            }
            mLastY = y;
        }
        break;
    default:
        break;
    }

    //printf("AbsListView::OnTouchEvent--------------10\n");
    return TRUE;
}

//@Override
void AbsListView::OnOverScrolled(
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY,
    /* [in] */ Boolean clampedX,
    /* [in] */ Boolean clampedY)
{
    mScrollY = scrollY;

    if (clampedY) {
        // Velocity is broken by hitting the limit; don't start a fling off of this.
        if (mVelocityTracker != NULL) {
            mVelocityTracker->Clear();
        }
    }
    AwakenScrollBars();
}

//@Override
ECode AbsListView::Draw(
    /* [in] */ ICanvas* canvas)
{
    AdapterView::Draw(canvas);
    if (mEdgeGlowTop != NULL) {
        Int32 scrollY = mScrollY;
        if (!mEdgeGlowTop->IsFinished()) {
            Int32 restoreCount;
            canvas->Save(&restoreCount);

            Int32 width = GetWidth();
            canvas->Translate(-width / 2, Math::Min(
                0, scrollY + mFirstPositionDistanceGuess));
            mEdgeGlowTop->SetSize(width * 2, GetHeight());
            if (mEdgeGlowTop->Draw(canvas)) {
                //Invalidate();
                printf("=====%s, %d=====Invalidate()\n", __FILE__, __LINE__);
            }
            canvas->RestoreToCount(restoreCount);
        }

        if (!mEdgeGlowBottom->IsFinished()) {
            Int32 restoreCount;
            canvas->Save(&restoreCount);
            Int32 width = GetWidth();
            Int32 height = GetHeight();

            canvas->Translate(-width / 2,
                Math::Max(height, scrollY + mLastPositionDistanceGuess));
            canvas->RotateEx(180, width, 0);
            mEdgeGlowBottom->SetSize(width * 2, height);
            if (mEdgeGlowBottom->Draw(canvas)) {
                //Invalidate();
            }
            canvas->RestoreToCount(restoreCount);
        }
    }

    if (mFastScroller != NULL) {
        Int32 scrollY = mScrollY;
        if (scrollY != 0) {
            // Pin to the top/bottom during overscroll
            Int32 restoreCount;
            canvas->Save(&restoreCount);
            canvas->Translate(0, (Float)scrollY);
            mFastScroller->Draw(canvas);
            canvas->RestoreToCount(restoreCount);
        }
        else {
            mFastScroller->Draw(canvas);
        }
    }

    return NOERROR;
}

//@Override
Boolean AbsListView::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    Int32 action;
    ev->GetAction(&action);

    AutoPtr<IView> v;
    if (mFastScroller != NULL) {
        Boolean intercepted = mFastScroller->OnInterceptTouchEvent(ev);
        if (intercepted) {
            return TRUE;
        }
    }

    switch (action & MotionEvent_ACTION_MASK) {
    case MotionEvent_ACTION_DOWN:
        {
            Int32 touchMode = mTouchMode;
            if (touchMode == TOUCH_MODE_OVERFLING || touchMode == TOUCH_MODE_OVERSCROLL) {
                mMotionCorrection = 0;
                return TRUE;
            }

            Float fx, fy;
            ev->GetX(&fx);
            ev->GetY(&fy);

            Int32 x = (Int32)fx;
            Int32 y = (Int32)fy;
            ev->GetPointerId(0, &mActivePointerId);

            Int32 motionPosition = FindMotionRow(y);
            if (touchMode != TOUCH_MODE_FLING && motionPosition >= 0) {
                // User clicked on an actual view (and was not stopping a fling).
                // Remember where the motion event started
                //
                v = GetChildAt(motionPosition - mFirstPosition);
                v->GetTop(&mMotionViewOriginalTop);
                mMotionX = x;
                mMotionY = y;
                mMotionPosition = motionPosition;
                mTouchMode = TOUCH_MODE_DOWN;
                ClearScrollingCache();
            }

            mLastY = Math::INT32_MIN_VALUE;
            if (touchMode == TOUCH_MODE_FLING) {
                return TRUE;
            }
        }
        break;
    case MotionEvent_ACTION_MOVE:
        {
            switch (mTouchMode) {
            case TOUCH_MODE_DOWN:
                {
                    Int32 pointerIndex;
                    ev->FindPointerIndex(mActivePointerId, &pointerIndex);
                    Float fy;
                    ev->GetYEx(pointerIndex, &fy);
                    Int32 y = (Int32)fy;
                    if (StartScrollIfNeeded(y - mMotionY)) {
                        return TRUE;
                    }
                }
                break;
            default:
                break;
            }
        }
        break;
    case MotionEvent_ACTION_UP:
        {
            mTouchMode = TOUCH_MODE_REST;
            mActivePointerId = INVALID_POINTER;
            ReportScrollStateChange(OnScrollListener_SCROLL_STATE_IDLE);
        }
        break;
    case MotionEvent_ACTION_POINTER_UP:
        {
            OnSecondaryPointerUp(ev);
        }
        break;
    default:
        break;
    }

    return FALSE;
}

void AbsListView::OnSecondaryPointerUp(
    /* [in] */ IMotionEvent* ev)
{
    Int32 action;
    ev->GetAction(&action);
    Int32 pointerIndex = (action & MotionEvent_ACTION_POINTER_INDEX_MASK) >>
            MotionEvent_ACTION_POINTER_INDEX_SHIFT;
    Int32 pointerId;
    ev->GetPointerId(pointerIndex, &pointerId);
    if (pointerId == mActivePointerId) {
        // This was our active pointer going up. Choose a new
        // active pointer and adjust accordingly.
        // TODO: Make this decision more intelligent.
        Int32 newPointerIndex = pointerIndex == 0 ? 1 : 0;
        Float fx, fy;
        ev->GetXEx(newPointerIndex, &fx);
        ev->GetYEx(newPointerIndex, &fy);
        mMotionX = (Int32)fx;
        mMotionY = (Int32)fy;
        mMotionCorrection = 0;
        ev->GetPointerId(newPointerIndex, &mActivePointerId);
        if (mVelocityTracker != NULL) {
            mVelocityTracker->Clear();
        }
    }
}

/**
 * {@inheritDoc}
 */
//@Override
ECode AbsListView::AddTouchables(
    /* [in] */ IObjectContainer* views)
{
    Int32 count = GetChildCount();
    Int32 firstPosition = mFirstPosition;
    IListAdapter* adapter = mAdapter;

    if (adapter == NULL) {
        return NOERROR;
    }

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child = GetChildAt(i);
        Boolean isEnabled;
        adapter->IsEnabled(firstPosition + i, &isEnabled);
        if (isEnabled) {
            views->Add(child.Get());
        }
        child->AddTouchables(views);
    }

    return NOERROR;
}

/**
 * Fires an "on scroll state changed" event to the registered
 * {@link android.widget.AbsListView.OnScrollListener}, if any. The state change
 * is fired only if the specified state is different from the previously known state.
 *
 * @param newState The new scroll state.
 */
void AbsListView::ReportScrollStateChange(
    /* [in] */ Int32 newState)
{
    if (newState != mLastScrollState) {
        if (mOnScrollListener != NULL) {
            mOnScrollListener->OnScrollStateChanged(
                (IAbsListView*)this->Probe(EIID_IAbsListView), newState);
            mLastScrollState = newState;
        }
    }
}

/**
 * Smoothly scroll to the specified adapter position. The view will
 * scroll such that the indicated position is displayed.
 * @param position Scroll to this adapter position.
 */
ECode AbsListView::SmoothScrollToPosition(
    /* [in] */ Int32 position)
{
    if (mPositionScroller == NULL) {
        mPositionScroller = new PositionScroller(this);
    }
    mPositionScroller->Start(position);

    return NOERROR;
}

/**
 * Smoothly scroll to the specified adapter position. The view will
 * scroll such that the indicated position is displayed, but it will
 * stop early if scrolling further would scroll boundPosition out of
 * view.
 * @param position Scroll to this adapter position.
 * @param boundPosition Do not scroll if it would move this adapter
 *          position out of view.
 */
ECode AbsListView::SmoothScrollToPosition(
    /* [in] */ Int32 position,
    /* [in] */ Int32 boundPosition)
{
    if (mPositionScroller == NULL) {
        mPositionScroller = new PositionScroller(this);
    }
    mPositionScroller->Start(position, boundPosition);

    return NOERROR;
}

/**
 * Smoothly scroll by distance pixels over duration milliseconds.
 * @param distance Distance to scroll in pixels.
 * @param duration Duration of the scroll animation in milliseconds.
 */
ECode AbsListView::SmoothScrollBy(
    /* [in] */ Int32 distance,
    /* [in] */ Int32 duration)
{
    if (mFlingRunnable == NULL) {
        mFlingRunnable = new FlingRunnable(this);
    }
    else {
        mFlingRunnable->EndFling();
    }
    mFlingRunnable->StartScroll(distance, duration);

    return NOERROR;
}

void AbsListView::CreateScrollingCache()
{
    if (mScrollingCacheEnabled && !mCachingStarted) {
        SetChildrenDrawnWithCacheEnabled(TRUE);
        SetChildrenDrawingCacheEnabled(TRUE);
        mCachingStarted = TRUE;
    }
}

void AbsListView::ClearScrollingCache()
{
    if (mClearScrollingCache == NULL) {
        AutoPtr<ClearScrollingCacheRunnable> temp
            = new ClearScrollingCacheRunnable(this);
        mClearScrollingCache = (IRunnable*)temp->Probe(EIID_IRunnable);
    }

    Post(mClearScrollingCache);
}

/**
 * Track a motion scroll
 *
 * @param deltaY Amount to offset mMotionView. This is the accumulated delta since the motion
 *        began. Positive numbers mean the user's finger is moving down the screen.
 * @param incrementalDeltaY Change in deltaY from the previous event.
 * @return TRUE if we're already at the beginning/end of the list and have nothing to do.
 */
Boolean AbsListView::TrackMotionScroll(
    /* [in] */ Int32 deltaY,
    /* [in] */ Int32 incrementalDeltaY)
{
    Int32 childCount = GetChildCount();
    if (childCount == 0) {
        return TRUE;
    }

    Int32 firstTop;
    GetChildAt(0)->GetTop(&firstTop);
    Int32 lastBottom;
    GetChildAt(childCount - 1)->GetBottom(&lastBottom);

    CRect* listPadding = mListPadding.Get();

     // FIXME account for grid vertical spacing too?
    Int32 spaceAbove = listPadding->mTop - firstTop;
    Int32 end = GetHeight() - listPadding->mBottom;
    Int32 spaceBelow = lastBottom - end;

    Int32 height = GetHeight() - mPaddingBottom - mPaddingTop;
    if (deltaY < 0) {
        deltaY = Math::Max(-(height - 1), deltaY);
    }
    else {
        deltaY = Math::Min(height - 1, deltaY);
    }

    if (incrementalDeltaY < 0) {
        incrementalDeltaY = Math::Max(-(height - 1), incrementalDeltaY);
    }
    else {
        incrementalDeltaY = Math::Min(height - 1, incrementalDeltaY);
    }

    Int32 firstPosition = mFirstPosition;

    // Update our guesses for where the first and last views are
    if (firstPosition == 0) {
        mFirstPositionDistanceGuess = firstTop - mListPadding->mTop;
    }
    else {
        mFirstPositionDistanceGuess += incrementalDeltaY;
    }

    if (firstPosition + childCount == mItemCount) {
        mLastPositionDistanceGuess = lastBottom + mListPadding->mBottom;
    }
    else {
        mLastPositionDistanceGuess += incrementalDeltaY;
    }

    if (firstPosition == 0 && firstTop >= listPadding->mTop
        && incrementalDeltaY >= 0) {
        // Don't need to move views down if the top of the first position
        // is already visible
        return incrementalDeltaY != 0;
    }

    if (firstPosition + childCount == mItemCount && lastBottom <= end &&
            incrementalDeltaY <= 0) {
        // Don't need to move views up if the bottom of the last position
        // is already visible
        return incrementalDeltaY != 0;
    }

    Boolean down = incrementalDeltaY < 0;
    Boolean inTouchMode = IsInTouchMode();
    if (inTouchMode) {
        HideSelector();
    }

    Int32 headerViewsCount = GetHeaderViewsCount();
    Int32 footerViewsStart = mItemCount - GetFooterViewsCount();

    Int32 start = 0;
    Int32 count = 0;

    if (down) {
        Int32 top = listPadding->mTop - incrementalDeltaY;
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IView> child = GetChildAt(i);
            Int32 childBottom;
            child->GetBottom(&childBottom);
            if (childBottom >= top) {
                break;
            }
            else {
                count++;
                Int32 position = firstPosition + i;
                if (position >= headerViewsCount && position < footerViewsStart) {
                    mRecycler->AddScrapView(child);
                    //if (ViewDebug.TRACE_RECYCLER) {
                    //    ViewDebug.trace(child,
                    //            ViewDebug.RecyclerTraceType.MOVE_TO_SCRAP_HEAP,
                    //            firstPosition + i, -1);
                    //}
                }
            }
        }
    }
    else {
        Int32 bottom = GetHeight() - listPadding->mBottom - incrementalDeltaY;
        for (Int32 i = childCount - 1; i >= 0; i--) {
            AutoPtr<IView> child = GetChildAt(i);
            Int32 childTop;
            child->GetTop(&childTop);
            if (childTop <= bottom) {
                break;
            }
            else {
                start = i;
                count++;
                Int32 position = firstPosition + i;
                if (position >= headerViewsCount && position < footerViewsStart) {
                    mRecycler->AddScrapView(child);

                    //if (ViewDebug.TRACE_RECYCLER) {
                    //    ViewDebug.trace(child,
                    //            ViewDebug.RecyclerTraceType.MOVE_TO_SCRAP_HEAP,
                    //            firstPosition + i, -1);
                    //}
                }
            }
        }
    }

    mMotionViewNewTop = mMotionViewOriginalTop + deltaY;

    mBlockLayoutRequests = TRUE;

    if (count > 0) {
        DetachViewsFromParent(start, count);
    }

    OffsetChildrenTopAndBottom(incrementalDeltaY);

    if (down) {
        mFirstPosition += count;
    }

    Invalidate();

    Int32 absIncrementalDeltaY = Math::Abs(incrementalDeltaY);
    if (spaceAbove < absIncrementalDeltaY
        || spaceBelow < absIncrementalDeltaY) {
        FillGap(down);
    }

    if (!inTouchMode && mSelectedPosition != AdapterView_INVALID_POSITION) {
        Int32 childIndex = mSelectedPosition - mFirstPosition;
        if (childIndex >= 0 && childIndex < GetChildCount()) {
            PositionSelector(GetChildAt(childIndex));
        }
    }

    mBlockLayoutRequests = FALSE;

    InvokeOnItemScrollListener();
    AwakenScrollBars();

    return FALSE;
}

/**
 * Returns the number of header views in the list. Header views are special views
 * at the top of the list that should not be recycled during a layout.
 *
 * @return The number of header views, 0 in the default implementation.
 */
Int32 AbsListView::GetHeaderViewsCount()
{
    return 0;
}

/**
 * Returns the number of footer views in the list. Footer views are special views
 * at the bottom of the list that should not be recycled during a layout.
 *
 * @return The number of footer views, 0 in the default implementation.
 */
Int32 AbsListView::GetFooterViewsCount()
{
    return 0;
}

void AbsListView::HideSelector()
{
    if (mSelectedPosition != AdapterView_INVALID_POSITION) {
        if (mLayoutMode != LAYOUT_SPECIFIC) {
            mResurrectToPosition = mSelectedPosition;
        }

        if (mNextSelectedPosition >= 0 && mNextSelectedPosition != mSelectedPosition) {
            mResurrectToPosition = mNextSelectedPosition;
        }

        SetSelectedPositionInt(AdapterView_INVALID_POSITION);
        SetNextSelectedPositionInt(AdapterView_INVALID_POSITION);
        mSelectedTop = 0;
        mSelectorRect->SetEmpty();
    }
}

/**
 * @return A position to select. First we try mSelectedPosition. If that has been clobbered by
 * entering touch mode, we then try mResurrectToPosition. Values are pinned to the range
 * of items available in the adapter
 */
Int32 AbsListView::ReconcileSelectedPosition()
{
    Int32 position = mSelectedPosition;
    if (position < 0) {
        position = mResurrectToPosition;
    }
    position = Math::Max(0, position);
    position = Math::Min(position, mItemCount - 1);
    return position;
}

/**
 * Find the row closest to y. This row will be used as the motion row when scrolling.
 *
 * @param y Where the user touched
 * @return The position of the first (or only) item in the row closest to y
 */
Int32 AbsListView::FindClosestMotionRow(
    /* [in] */ Int32 y)
{
    Int32 childCount = GetChildCount();
    if (childCount == 0) {
        return AdapterView_INVALID_POSITION;
    }

    Int32 motionRow = FindMotionRow(y);
    return motionRow != AdapterView_INVALID_POSITION
        ? motionRow : mFirstPosition + childCount - 1;
}

/**
 * Causes all the views to be rebuilt and redrawn.
 */
ECode AbsListView::InvalidateViews()
{
    mDataChanged = TRUE;
    RememberSyncState();
    RequestLayout();
    Invalidate();

    return NOERROR;
}


/**
 * Attempt to bring the selection back if the user is switching from touch
 * to trackball mode
 * @return Whether selection was set to something.
 */
Boolean AbsListView::ResurrectSelection()
{
    Int32 childCount = GetChildCount();

    if (childCount <= 0) {
        return FALSE;
    }

    Int32 selectedTop = 0;
    Int32 selectedPos;
    Int32 childrenTop = mListPadding->mTop;
    Int32 childrenBottom = mBottom - mTop - mListPadding->mBottom;
    Int32 firstPosition = mFirstPosition;
    Int32 toPosition = mResurrectToPosition;
    Boolean down = TRUE;

    if (toPosition >= firstPosition
        && toPosition < firstPosition + childCount) {
        selectedPos = toPosition;

        AutoPtr<IView> selected = GetChildAt(selectedPos - mFirstPosition);
        selected->GetTop(&selectedTop);
        Int32 selectedBottom;
        selected->GetBottom(&selectedBottom);

        // We are scrolled, don't get in the fade
        if (selectedTop < childrenTop) {
            selectedTop = childrenTop + GetVerticalFadingEdgeLength();
        }
        else if (selectedBottom > childrenBottom) {
            Int32 measureHeight;
            selected->GetMeasuredHeight(&measureHeight);
            selectedTop =
                childrenBottom - measureHeight - GetVerticalFadingEdgeLength();
        }
    }
    else {
        if (toPosition < firstPosition) {
            // Default to selecting whatever is first
            selectedPos = firstPosition;
            for (Int32 i = 0; i < childCount; i++) {
                Int32 top;
                GetChildAt(i)->GetTop(&top);

                if (i == 0) {
                    // Remember the position of the first item
                    selectedTop = top;
                    // See if we are scrolled at all
                    if (firstPosition > 0 || top < childrenTop) {
                        // If we are scrolled, don't select anything that is
                        // in the fade region
                        childrenTop += GetVerticalFadingEdgeLength();
                    }
                }

                if (top >= childrenTop) {
                    // Found a view whose top is fully visisble
                    selectedPos = firstPosition + i;
                    selectedTop = top;
                    break;
                }
            }
        }
        else {
            Int32 itemCount = mItemCount;
            down = FALSE;
            selectedPos = firstPosition + childCount - 1;

            for (Int32 i = childCount - 1; i >= 0; i--) {
                AutoPtr<IView> v = GetChildAt(i);
                Int32 top, bottom;
                v->GetTop(&top);
                v->GetBottom(&bottom);

                if (i == childCount - 1) {
                    selectedTop = top;
                    if (firstPosition + childCount < itemCount
                        || bottom > childrenBottom) {
                        childrenBottom -= GetVerticalFadingEdgeLength();
                    }
                }

                if (bottom <= childrenBottom) {
                    selectedPos = firstPosition + i;
                    selectedTop = top;
                    break;
                }
            }
        }
    }

    mResurrectToPosition = AdapterView_INVALID_POSITION;
    RemoveCallbacks((IRunnable*)mFlingRunnable->Probe(EIID_IRunnable));
    mTouchMode = TOUCH_MODE_REST;
    ClearScrollingCache();
    mSpecificTop = selectedTop;
    selectedPos = LookForSelectablePosition(selectedPos, down);
    if (selectedPos >= firstPosition
        && selectedPos <= GetLastVisiblePosition()) {
        mLayoutMode = LAYOUT_SPECIFIC;
        SetSelectionInt(selectedPos);
        InvokeOnItemScrollListener();
    }
    else {
        selectedPos = AdapterView_INVALID_POSITION;
    }
    ReportScrollStateChange(OnScrollListener_SCROLL_STATE_IDLE);

    return selectedPos >= 0;
}

//@Override
void AbsListView::HandleDataChanged()
{
    Int32 count = mItemCount;
    if (count > 0) {
        Int32 newPos;
        Int32 selectablePos;

        // Find the row we are supposed to sync to
        if (mNeedSync) {
            // Update this first, since setNextSelectedPositionInt inspects it
            mNeedSync = FALSE;

            if (mTranscriptMode == AbsListView_TRANSCRIPT_MODE_ALWAYS_SCROLL ||
                (mTranscriptMode == AbsListView_TRANSCRIPT_MODE_NORMAL &&
                mFirstPosition + GetChildCount() >= mOldItemCount)) {
                mLayoutMode = LAYOUT_FORCE_BOTTOM;
                return;
            }

            switch (mSyncMode) {
            case SYNC_SELECTED_POSITION:
                if (IsInTouchMode()) {
                    // We saved our state when not in touch mode. (We know this because
                    // mSyncMode is SYNC_SELECTED_POSITION.) Now we are trying to
                    // restore in touch mode. Just leave mSyncPosition as it is (possibly
                    // adjusting if the available range changed) and return.
                    mLayoutMode = LAYOUT_SYNC;
                    mSyncPosition = Math::Min(Math::Max(0, mSyncPosition), count - 1);

                    return;
                }
                else {
                    // See if we can find a position in the new data with the same
                    // id as the old selection. This will change mSyncPosition.
                    newPos = FindSyncPosition();
                    if (newPos >= 0) {
                        // Found it. Now verify that new selection is still selectable
                        selectablePos = LookForSelectablePosition(newPos, TRUE);
                        if (selectablePos == newPos) {
                            // Same row id is selected
                            mSyncPosition = newPos;

                            if (mSyncHeight == GetHeight()) {
                                // If we are at the same height as when we saved state, try
                                // to restore the scroll position too.
                                mLayoutMode = LAYOUT_SYNC;
                            }
                            else {
                                // We are not the same height as when the selection was saved, so
                                // don't try to restore the exact position
                                mLayoutMode = LAYOUT_SET_SELECTION;
                            }

                            // Restore selection
                            SetNextSelectedPositionInt(newPos);
                            return;
                        }
                    }
                }
                break;
            case SYNC_FIRST_POSITION:
                // Leave mSyncPosition as it is -- just pin to available range
                mLayoutMode = LAYOUT_SYNC;
                mSyncPosition = Math::Min(Math::Max(0, mSyncPosition), count - 1);

                return;
            default:
                break;
            }
        }

        if (!IsInTouchMode()) {
            // We couldn't find matching data -- try to use the same position
            newPos = GetSelectedItemPosition();

            // Pin position to the available range
            if (newPos >= count) {
                newPos = count - 1;
            }

            if (newPos < 0) {
                newPos = 0;
            }

            // Make sure we select something selectable -- first look down
            selectablePos = LookForSelectablePosition(newPos, TRUE);

            if (selectablePos >= 0) {
                SetNextSelectedPositionInt(selectablePos);
                return;
            }
            else {
                // Looking down didn't work -- try looking up
                selectablePos = LookForSelectablePosition(newPos, FALSE);
                if (selectablePos >= 0) {
                    SetNextSelectedPositionInt(selectablePos);
                    return;
                }
            }
        }
        else {
            // We already know where we want to resurrect the selection
            if (mResurrectToPosition >= 0) {
                return;
            }
        }
    }

    // Nothing is selected. Give up and reset everything.
    mLayoutMode = mStackFromBottom ? LAYOUT_FORCE_BOTTOM : LAYOUT_FORCE_TOP;
    mSelectedPosition = AdapterView_INVALID_POSITION;
    mSelectedRowId = AdapterView_INVALID_ROW_ID;
    mNextSelectedPosition = AdapterView_INVALID_POSITION;
    mNextSelectedRowId = AdapterView_INVALID_ROW_ID;
    mNeedSync = FALSE;
    CheckSelectionChanged();
}

//@Override
void AbsListView::OnDisplayHint(
    /* [in] */ Int32 hint)
{
    AdapterView::OnDisplayHint(hint);
    switch (hint) {
    case INVISIBLE:
        //if (mPopup != NULL && mPopup.isShowing()) {
        //    dismissPopup();
        //}
        break;
    case VISIBLE:
        //if (mFiltered && mPopup != NULL && !mPopup.isShowing()) {
        //    showPopup();
        //}
        break;
    }
    mPopupHidden = hint == INVISIBLE;
}

/**
 * Removes the filter window
 */
void AbsListView::DismissPopup()
{
    //if (mPopup != NULL) {
    //    mPopup.dismiss();
    //}
}

/**
 * Shows the filter window
 */
void AbsListView::ShowPopup()
{
    // Make sure we have a window before showing the popup
    if (GetWindowVisibility() == View::VISIBLE) {
        CreateTextFilter(TRUE);
        PositionPopup();
        // Make sure we get focus if we are showing the popup
        CheckFocus();
    }
}

void AbsListView::PositionPopup()
{
    //AutoPtr<CDisplayMetrics> dm;
    //GetResources()->GetDisplayMetrics((IDisplayMetrics**)&dm);
    //Int32 screenHeight = dm->mHeightPixels;
    //Int32 x, y;
    //GetLocationOnScreen(&x, &y);
    //// TODO: The 20 below should come from the theme
    //// TODO: And the gravity should be defined in the theme as well
    ////
    //Int32 bottomGap = screenHeight - y - GetHeight() + (Int32)(mDensityScale * 20);

    //if (!mPopup.isShowing()) {
    //    mPopup.showAtLocation(this, Gravity.BOTTOM | Gravity.CENTER_HORIZONTAL,
    //            x, bottomGap);
    //}
    //else {
    //    mPopup.update(x, bottomGap, -1, -1);
    //}
}

/**
 * What is the distance between the source and destination rectangles given the direction of
 * focus navigation between them? The direction basically helps figure out more quickly what is
 * self evident by the relationship between the rects...
 *
 * @param source the source rectangle
 * @param dest the destination rectangle
 * @param direction the direction
 * @return the distance between the rectangles
 */
Int32 AbsListView::GetDistance(
    /* [in] */ IRect* sourceRect,
    /* [in] */ IRect* destRect,
    /* [in] */ Int32 direction)
{
    assert(sourceRect && destRect);
    CRect* source = (CRect*)sourceRect;
    CRect* dest = (CRect*)destRect;

    Int32 sX, sY; // source x, y
    Int32 dX, dY; // dest x, y
    switch (direction) {
    case View::FOCUS_RIGHT:
        {
            sX = source->mRight;
            sY = source->mTop + source->GetHeight() / 2;
            dX = dest->mLeft;
            dY = dest->mTop + dest->GetHeight() / 2;
        }
        break;
    case View::FOCUS_DOWN:
        {
            sX = source->mLeft + source->GetWidth() / 2;
            sY = source->mBottom;
            dX = dest->mLeft + dest->GetWidth() / 2;
            dY = dest->mTop;
        }
        break;
    case View::FOCUS_LEFT:
        {
            sX = source->mLeft;
            sY = source->mTop + source->GetHeight() / 2;
            dX = dest->mRight;
            dY = dest->mTop + dest->GetHeight() / 2;
        }
        break;
    case View::FOCUS_UP:
        {
            sX = source->mLeft + source->GetWidth() / 2;
            sY = source->mTop;
            dX = dest->mLeft + dest->GetWidth() / 2;
            dY = dest->mBottom;
        }
        break;
    default:
        Logger::E("AbsListView", StringBuffer("direction must be one of ")
            + "{FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}.");
        assert(0);
    }

    Int32 deltaX = dX - sX;
    Int32 deltaY = dY - sY;

    return deltaY * deltaY + deltaX * deltaX;
}

//@Override
Boolean AbsListView::IsInFilterMode()
{
    return mFiltered;
}

/**
 * Sends a key to the text filter window
 *
 * @param keyCode The keycode for the event
 * @param event The actual key event
 *
 * @return True if the text filter handled the event, FALSE otherwise.
 */
Boolean AbsListView::SendToTextFilter(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 count,
    /* [in] */ IKeyEvent* event)
{
    if (!AcceptFilter()) {
        return FALSE;
    }

    Boolean handled = FALSE;
    Boolean okToSend = TRUE;
    switch (keyCode) {
    case KeyEvent_KEYCODE_DPAD_UP:
    case KeyEvent_KEYCODE_DPAD_DOWN:
    case KeyEvent_KEYCODE_DPAD_LEFT:
    case KeyEvent_KEYCODE_DPAD_RIGHT:
    case KeyEvent_KEYCODE_DPAD_CENTER:
    case KeyEvent_KEYCODE_ENTER:
        okToSend = FALSE;
        break;
    case KeyEvent_KEYCODE_BACK:
        //if (mFiltered && mPopup != NULL && mPopup.isShowing()) {
        //    if (event.getAction() == KeyEvent_ACTION_DOWN
        //            && event.getRepeatCount() == 0) {
        //        getKeyDispatcherState().startTracking(event, this);
        //        handled = TRUE;
        //    } else if (event.getAction() == KeyEvent_ACTION_UP
        //            && event.isTracking() && !event.isCanceled()) {
        //        handled = TRUE;
        //        mTextFilter.setText("");
        //    }
        //}
        okToSend = FALSE;
        break;
    case KeyEvent_KEYCODE_SPACE:
        // Only send spaces once we are filtered
        okToSend = mFiltered;
        break;
    }

    if (okToSend) {
        CreateTextFilter(TRUE);

        AutoPtr<IKeyEvent> forwardEvent = event;
        Int32 repeatCount;
        forwardEvent->GetRepeatCount(&repeatCount);
        if (repeatCount > 0) {
            Int64 eventTime;
            event->GetEventTime(&eventTime);
            forwardEvent = NULL;
            CKeyEvent::ChangeTimeRepeat(
                event, eventTime, 0, (IKeyEvent**)&forwardEvent);
        }

        Int32 action;
        event->GetAction(&action);
        //switch (action) {
        //    case KeyEvent_ACTION_DOWN:
        //        handled = mTextFilter.onKeyDown(keyCode, forwardEvent);
        //        break;
        //    case KeyEvent_ACTION_UP:
        //        handled = mTextFilter.onKeyUp(keyCode, forwardEvent);
        //        break;
        //    case KeyEvent_ACTION_MULTIPLE:
        //        handled = mTextFilter.onKeyMultiple(keyCode, count, event);
        //        break;
        //}
    }
    return handled;
}

/**
 * Return an InputConnection for editing of the filter text.
 */
//@Override
//InputConnection onCreateInputConnection(EditorInfo outAttrs) {
//    if (isTextFilterEnabled()) {
//        // XXX we need to have the text filter created, so we can get an
//        // InputConnection to proxy to.  Unfortunately this means we pretty
//        // much need to make it as soon as a list view gets focus.
//        createTextFilter(FALSE);
//        if (mPublicInputConnection == NULL) {
//            mDefInputConnection = new BaseInputConnection(this, FALSE);
//            mPublicInputConnection = new InputConnectionWrapper(
//                    mTextFilter.onCreateInputConnection(outAttrs), TRUE) {
//                @Override
//                public Boolean reportFullscreenMode(Boolean enabled) {
//                    // Use our own input connection, since it is
//                    // the "real" one the IME is talking with.
//                    return mDefInputConnection.reportFullscreenMode(enabled);
//                }

//                @Override
//                public Boolean performEditorAction(Int32 editorAction) {
//                    // The editor is off in its own window; we need to be
//                    // the one that does this.
//                    if (editorAction == EditorInfo.IME_ACTION_DONE) {
//                        InputMethodManager imm = (InputMethodManager)
//                                getContext().getSystemService(
//                                        Context.INPUT_METHOD_SERVICE);
//                        if (imm != NULL) {
//                            imm.hideSoftInputFromWindow(getWindowToken(), 0);
//                        }
//                        return TRUE;
//                    }
//                    return FALSE;
//                }

//                @Override
//                public Boolean sendKeyEvent(KeyEvent event) {
//                    // Use our own input connection, since the filter
//                    // text view may not be shown in a window so has
//                    // no ViewRoot to dispatch events with.
//                    return mDefInputConnection.sendKeyEvent(event);
//                }
//            };
//        }
//        outAttrs.inputType = EditorInfo.TYPE_CLASS_TEXT
//                | EditorInfo.TYPE_TEXT_VARIATION_FILTER;
//        outAttrs.imeOptions = EditorInfo.IME_ACTION_DONE;
//        return mPublicInputConnection;
//    }
//    return NULL;
//}

/**
 * For filtering we proxy an input connection to an internal text editor,
 * and this allows the proxying to happen.
 */
//@Override
Boolean AbsListView::CheckInputConnectionProxy(
    /* [in] */ IView* view)
{
    return FALSE;
    //return view == mTextFilter;
}

/**
 * Creates the window for the text filter and populates it with an EditText field;
 *
 * @param animateEntrance TRUE if the window should appear with an animation
 */
void AbsListView::CreateTextFilter(
    /* [in] */ Boolean animateEntrance)
{
    //if (mPopup == NULL) {
    //    Context c = getContext();
    //    PopupWindow p = new PopupWindow(c);
    //    LayoutInflater layoutInflater = (LayoutInflater)
    //            c.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    //    mTextFilter = (EditText) layoutInflater.inflate(
    //            com.android.internal.R.layout.typing_filter, NULL);
    //    // For some reason setting this as the "real" input type changes
    //    // the text view in some way that it doesn't work, and I don't
    //    // want to figure out why this is.
    //    mTextFilter.setRawInputType(EditorInfo.TYPE_CLASS_TEXT
    //            | EditorInfo.TYPE_TEXT_VARIATION_FILTER);
    //    mTextFilter.setImeOptions(EditorInfo.IME_FLAG_NO_EXTRACT_UI);
    //    mTextFilter.addTextChangedListener(this);
    //    p.setFocusable(FALSE);
    //    p.setTouchable(FALSE);
    //    p.setInputMethodMode(PopupWindow.INPUT_METHOD_NOT_NEEDED);
    //    p.setContentView(mTextFilter);
    //    p.setWidth(LayoutParams.WRAP_CONTENT);
    //    p.setHeight(LayoutParams.WRAP_CONTENT);
    //    p.setBackgroundDrawable(NULL);
    //    mPopup = p;
    //    getViewTreeObserver().addOnGlobalLayoutListener(this);
    //    mGlobalLayoutListenerAddedFilter = TRUE;
    //}
    //if (animateEntrance) {
    //    mPopup.setAnimationStyle(com.android.internal.R.style.Animation_TypingFilter);
    //} else {
    //    mPopup.setAnimationStyle(com.android.internal.R.style.Animation_TypingFilterRestore);
    //}
}

/**
 * Clear the text filter.
 */
ECode AbsListView::ClearTextFilter()
{
    //if (mFiltered) {
    //    mTextFilter.setText("");
    //    mFiltered = FALSE;
    //    if (mPopup != NULL && mPopup.isShowing()) {
    //        dismissPopup();
    //    }
    //}
    return E_NOT_IMPLEMENTED;
}

/**
 * Returns if the ListView currently has a text filter.
 */
Boolean AbsListView::HasTextFilter()
{
    return mFiltered;
}

ECode AbsListView::OnGlobalLayout()
{
    //if (IsShown()) {
    //    // Show the popup if we are filtered
    //    if (mFiltered && mPopup != NULL && !mPopup.isShowing() && !mPopupHidden) {
    //        ShowPopup();
    //    }
    //}
    //else {
    //    // Hide the popup when we are no longer visible
    //    if (mPopup != NULL && mPopup.isShowing()) {
    //        DismissPopup();
    //    }
    //}

    return E_NOT_IMPLEMENTED;
}

/**
 * For our text watcher that is associated with the text filter.  Does
 * nothing.
 */
ECode AbsListView::BeforeTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 after)
{
    return NOERROR;
}

/**
 * For our text watcher that is associated with the text filter. Performs
 * the actual filtering as the text changes, and takes care of hiding and
 * showing the popup displaying the currently entered filter text.
 */
ECode AbsListView::OnTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 count)
{
    //if (mPopup != NULL && isTextFilterEnabled()) {
    //    Int32 length = s.length();
    //    Boolean showing = mPopup.isShowing();
    //    if (!showing && length > 0) {
    //        // Show the filter popup if necessary
    //        showPopup();
    //        mFiltered = TRUE;
    //    } else if (showing && length == 0) {
    //        // Remove the filter popup if the user has cleared all text
    //        dismissPopup();
    //        mFiltered = FALSE;
    //    }
    //    if (mAdapter instanceof Filterable) {
    //        Filter f = ((Filterable) mAdapter).getFilter();
    //        // Filter should not be NULL when we reach this part
    //        if (f != NULL) {
    //            f.filter(s, this);
    //        } else {
    //            throw new IllegalStateException("You cannot call onTextChanged with a non "
    //                    + "filterable adapter");
    //        }
    //    }
    //}
    return E_NOT_IMPLEMENTED;
}

/**
 * For our text watcher that is associated with the text filter.  Does
 * nothing.
 */
//ECode AfterTextChanged(
//    /* [in] */ IEditable* s)
//{
//}

ECode AbsListView::OnFilterComplete(
    /* [in] */ Int32 count)
{
    if (mSelectedPosition < 0 && count > 0) {
        mResurrectToPosition = AdapterView_INVALID_POSITION;
        ResurrectSelection();
    }
    return NOERROR;
}

//@Override
AutoPtr<IViewGroupLayoutParams> AbsListView::GenerateLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    AutoPtr<IViewGroupLayoutParams> params;
    assert(SUCCEEDED(CAbsListViewLayoutParams::New(p,
        (IAbsListViewLayoutParams**)&params)));

    return params;
}

//@Override
AutoPtr<IViewGroupLayoutParams> AbsListView::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<IViewGroupLayoutParams> params;
    assert(SUCCEEDED(CAbsListViewLayoutParams::New(GetContext(), attrs,
        (IAbsListViewLayoutParams**)&params)));

    return params;
}

//@Override
Boolean AbsListView::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return p && (p->Probe(EIID_IAbsListViewLayoutParams) != NULL);
}

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
ECode AbsListView::SetTranscriptMode(
    /* [in] */ Int32 mode)
{
    mTranscriptMode = mode;

    return NOERROR;
}

/**
 * Returns the current transcript mode.
 *
 * @return {@link #AbsListView_TRANSCRIPT_MODE_DISABLED}, {@link #AbsListView_TRANSCRIPT_MODE_NORMAL} or
 *         {@link #AbsListView_TRANSCRIPT_MODE_ALWAYS_SCROLL}
 */
Int32 AbsListView::GetTranscriptMode()
{
    return mTranscriptMode;
}

//@Override
Int32 AbsListView::GetSolidColor()
{
    return mCacheColorHint;
}

/**
 * When set to a non-zero value, the cache color hint indicates that this list is always drawn
 * on top of a solid, single-color, opaque background
 *
 * @param color The background color
 */
ECode AbsListView::SetCacheColorHint(
    /* [in] */ Int32 color)
{
    if (color != mCacheColorHint) {
        mCacheColorHint = color;
        Int32 count = GetChildCount();
        for (Int32 i = 0; i < count; i++) {
            GetChildAt(i)->SetDrawingCacheBackgroundColor(color);
        }
        mRecycler->SetCacheColorHint(color);
    }

    return NOERROR;
}

/**
 * When set to a non-zero value, the cache color hint indicates that this list is always drawn
 * on top of a solid, single-color, opaque background
 *
 * @return The cache color hint
 */
Int32 AbsListView::GetCacheColorHint()
{
    return mCacheColorHint;
}

/**
 * Move all views (excluding headers and footers) held by this AbsListView into the supplied
 * List. This includes views displayed on the screen as well as views stored in AbsListView's
 * internal view recycler.
 *
 * @param views A list into which to put the reclaimed views
 */
ECode AbsListView::ReclaimViews(
    /* [in] */ IObjectContainer* views)
{
    Int32 childCount = GetChildCount();
    AutoPtr<IRecyclerListener> listener = mRecycler->mRecyclerListener;

    // Reclaim views on screen
    for (Int32 i = 0; i < childCount; i++) {
        AutoPtr<IView> child = GetChildAt(i);
        AutoPtr<IAbsListViewLayoutParams> lp;
        child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        // Don't reclaim header or footer views, or views that should be ignored
        if (lp != NULL && mRecycler->ShouldRecycleViewType(
                ((CAbsListViewLayoutParams*)lp.Get())->mViewType)) {
            views->Add(child.Get());
            if (listener != NULL) {
                // Pretend they went through the scrap heap
                listener->OnMovedToScrapHeap(child);
            }
        }
    }
    mRecycler->ReclaimScrapViews(views);
    RemoveAllViewsInLayout();

    return NOERROR;
}

/**
 * @hide
 */
//@Override
Boolean AbsListView::OnConsistencyCheck(
    /* [in] */ Int32 consistency)
{
    Boolean result = AdapterView::OnConsistencyCheck(consistency);

    Boolean checkLayout = TRUE;//(consistency & ViewDebug.CONSISTENCY_LAYOUT) != 0;

    if (checkLayout) {
        // The active recycler must be empty
        AutoPtr<IView>* activeViews = mRecycler->mActiveViews;
        Int32 count = mRecycler->mActiveViewsLength;
        for (Int32 i = 0; i < count; i++) {
            if (activeViews[i] != NULL) {
                result = FALSE;
                //Log.d(ViewDebug.CONSISTENCY_LOG_TAG,
                //        "AbsListView " + this + " has a view in its active recycler: " +
                //                activeViews[i]);
            }
        }

        // All views in the recycler must NOT be on screen and must NOT have a parent
        if (!CheckScrap(mRecycler->mCurrentScrap)) {
            result = FALSE;
        }

        count = mRecycler->mViewTypeCount;
        for (Int32 i = 0; i < count; i++) {
            if (!CheckScrap(mRecycler->mScrapViews[i]))
                result = FALSE;
        }
    }

    return result;
}

Boolean AbsListView::CheckScrap(
    /* [in] */ Vector<AutoPtr<IView> >& scrap)
{
    if (scrap.GetSize() == 0) {
        return TRUE;
    }
    Boolean result = TRUE;

    Vector<AutoPtr<IView> >::Iterator iter = scrap.Begin();
    for (; iter!=scrap.End(); ++iter) {
        IView* view = (*iter).Get();
        assert(view != NULL);
        AutoPtr<IViewParent> parent;
        view->GetParent((IViewParent**)&parent);
        if (parent != NULL) {
            result = FALSE;
            //Log.d(ViewDebug.CONSISTENCY_LOG_TAG, "AbsListView " + this +
            //    " has a view in its scrap heap still attached to a parent: " + view);
            break;
        }

        if (IndexOfChild(view) >= 0) {
            result = FALSE;
            //Log.d(ViewDebug.CONSISTENCY_LOG_TAG, "AbsListView " + this +
            //    " has a view in its scrap heap that is also a direct child: " + view);
            break;
        }
    }

    return result;
}

void AbsListView::FinishGlows()
{
    if (mEdgeGlowTop != NULL) {
        mEdgeGlowTop->Finish();
        mEdgeGlowBottom->Finish();
    }
}

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
ECode AbsListView::SetRecyclerListener(
    /* [in] */ IRecyclerListener* listener)
{
    mRecycler->mRecyclerListener = listener;

    return NOERROR;
}

static Int32 R_Styleable_View[] = {
    0x01010063, 0x01010064, 0x01010065, 0x01010066,
    0x01010067, 0x01010068, 0x01010069, 0x0101007f,
    0x010100d0, 0x010100d1, 0x010100d2, 0x010100d3,
    0x010100d4, 0x010100d5, 0x010100d6, 0x010100d7,
    0x010100d8, 0x010100d9, 0x010100da, 0x010100db,
    0x010100dc, 0x010100dd, 0x010100de, 0x010100df,
    0x010100e0, 0x010100e1, 0x010100e2, 0x010100e3,
    0x010100e4, 0x010100e5, 0x010100e6, 0x010100e7,
    0x010100e8, 0x010100e9, 0x0101013f, 0x01010140,
    0x01010215, 0x01010216, 0x0101024e, 0x0101025e,
    0x0101026f, 0x01010273, 0x010102a8, 0x010102a9,
    0x010102aa, 0x010102c1, 0x010102c4
};

ECode AbsListView::Init(
    /* [in] */ IContext* context)
{
    FAIL_RETURN(AdapterView::Init(context));

    InitAbsListView();

    SetVerticalScrollBarEnabled(TRUE);
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(
        ArrayOf<Int32>(R_Styleable_View,
        sizeof(R_Styleable_View) / sizeof(Int32)), (ITypedArray**)&a);
    InitializeScrollbars(a);
    a->Recycle();

    return NOERROR;
}

static Int32 R_Styleable_AbsListView[] = {
    0x010100fb, 0x010100fc, 0x010100fd, 0x010100fe,
    0x010100ff, 0x01010100, 0x01010101, 0x01010226,
    0x01010231
};

ECode AbsListView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(AdapterView::Init(context, attrs, defStyle));

    InitAbsListView();

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(
        attrs, ArrayOf<Int32>(R_Styleable_AbsListView,
        sizeof(R_Styleable_AbsListView) / sizeof(Int32)),//com.android.internal.R.styleable.AbsListView
        defStyle, 0, (ITypedArray**)&a);

    AutoPtr<IDrawable> d;
    a->GetDrawable(
        0/*com.android.internal.R.styleable.AbsListView_listSelector*/,
        (IDrawable**)&d);
    //printf("AbsListView::Init listSelector = 0x%08x\n", d.Get());
    if (d != NULL) {
        SetSelector(d);
    }

    a->GetBoolean(
        1/*com.android.internal.R.styleable.AbsListView_drawSelectorOnTop*/,
        FALSE, &mDrawSelectorOnTop);

    Boolean stackFromBottom;
    a->GetBoolean(
        2/*R.styleable.AbsListView_stackFromBottom*/,
        FALSE, &stackFromBottom);

    SetStackFromBottom(stackFromBottom);

    Boolean scrollingCacheEnabled;
    a->GetBoolean(
        3/*R.styleable.AbsListView_scrollingCache*/, TRUE,
        &scrollingCacheEnabled);
    SetScrollingCacheEnabled(scrollingCacheEnabled);

    Boolean useTextFilter;
    a->GetBoolean(
        4/*R.styleable.AbsListView_textFilterEnabled*/, FALSE,
        &useTextFilter);
    SetTextFilterEnabled(useTextFilter);

    Int32 transcriptMode;
    a->GetInt32(
        5/*R.styleable.AbsListView_transcriptMode*/,
        AbsListView_TRANSCRIPT_MODE_DISABLED, &transcriptMode);
    SetTranscriptMode(transcriptMode);

    Int32 color;
    a->GetColor(
        6/*R.styleable.AbsListView_cacheColorHint*/,
        0, &color);
    SetCacheColorHint(color);

    Boolean enableFastScroll;
    a->GetBoolean(
        7/*R.styleable.AbsListView_fastScrollEnabled*/,
        FALSE, &enableFastScroll);
    SetFastScrollEnabled(enableFastScroll);

    Boolean smoothScrollbar;
    a->GetBoolean(
        8/*R.styleable.AbsListView_smoothScrollbar*/,
        TRUE, &smoothScrollbar);
    SetSmoothScrollbarEnabled(smoothScrollbar);

    a->Recycle();

    return NOERROR;
}
