
#include "widget/AdapterView.h"
#include "os/SystemClock.h"
#include <Logger.h>
#include <elastos/Math.h>

using namespace Elastos::Utility;
using namespace Elastos::Utility::Logging;
using namespace Elastos::Core;

const Int32 AdapterView::SYNC_SELECTED_POSITION;
const Int32 AdapterView::SYNC_FIRST_POSITION;
const Int32 AdapterView::SYNC_MAX_DURATION_MILLIS;

AdapterView::AdapterContextMenuInfo::AdapterContextMenuInfo(
    /* [in] */ IView* targetView,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
    : mTargetView(targetView)
    , mPosition(position)
    , mId(id)
{
}

PInterface AdapterView::AdapterContextMenuInfo::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IContextMenuInfo) {
        return (IContextMenuInfo*)this;
    }

    return NULL;
}

UInt32 AdapterView::AdapterContextMenuInfo::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AdapterView::AdapterContextMenuInfo::Release()
{
    return ElRefBase::Release();
}

ECode AdapterView::AdapterContextMenuInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IContextMenuInfo*)this) {
        *pIID = EIID_IContextMenuInfo;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

PInterface AdapterView::AdapterDataSetObserver::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IDataSetObserver) {
        return (IDataSetObserver*)this;
    }

    return NULL;
}

UInt32 AdapterView::AdapterDataSetObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AdapterView::AdapterDataSetObserver::Release()
{
    return ElRefBase::Release();
}

ECode AdapterView::AdapterDataSetObserver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IDataSetObserver*)this) {
        *pIID = EIID_IDataSetObserver;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode AdapterView::AdapterDataSetObserver::OnChanged()
{
    mHost->mDataChanged = TRUE;
    mHost->mOldItemCount = mHost->mItemCount;

    AutoPtr<IAdapter> adapter = mHost->GetAdapter();
    assert(adapter.Get());
    adapter->GetCount(&mHost->mItemCount);

    // Detect the case where a cursor that was previously invalidated has
    // been repopulated with new data.
    Boolean hasStableIds;
    adapter->HasStableIds(&hasStableIds);
    if (hasStableIds && mInstanceState != NULL
        && mHost->mOldItemCount == 0 && mHost->mItemCount > 0)
    {
        mHost->OnRestoreInstanceState(mInstanceState);
        mInstanceState = NULL;
    }
    else {
        mHost->RememberSyncState();
    }
    mHost->CheckFocus();
    mHost->RequestLayout();

    return NOERROR;
}

//@Override
ECode AdapterView::AdapterDataSetObserver::OnInvalidated()
{
    mHost->mDataChanged = TRUE;

    AutoPtr<IAdapter> adapter = mHost->GetAdapter();
    assert(adapter.Get());
    Boolean hasStableIds;
    adapter->HasStableIds(&hasStableIds);
    if (hasStableIds) {
        // Remember the current state for the case where our hosting activity is being
        // stopped and later restarted
        mInstanceState = mHost->OnSaveInstanceState();
    }

    // Data is invalid so we should reset our state
    mHost->mOldItemCount = mHost->mItemCount;
    mHost->mItemCount = 0;
    mHost->mSelectedPosition = AdapterView_INVALID_POSITION;
    mHost->mSelectedRowId = AdapterView_INVALID_ROW_ID;
    mHost->mNextSelectedPosition = AdapterView_INVALID_POSITION;
    mHost->mNextSelectedRowId = AdapterView_INVALID_ROW_ID;
    mHost->mNeedSync = FALSE;

    mHost->CheckFocus();
    mHost->RequestLayout();

    return NOERROR;
}

ECode AdapterView::AdapterDataSetObserver::ClearSavedState()
{
    mInstanceState = NULL;

    return NOERROR;
}

AdapterView::SelectionNotifier::SelectionNotifier(
    /* [in] */ AdapterView* host)
    : mHost(host)
{
    assert(host);
}

ECode AdapterView::SelectionNotifier::Run()
{
    if (mHost->mDataChanged) {
        // Data has changed between when this SelectionNotifier
        // was posted and now. We need to wait until the AdapterView
        // has been synched to the new data.
        if (mHost->GetAdapter() != NULL) {
            mHost->Post((IRunnable*)this->Probe(EIID_IRunnable));
        }
    }
    else {
        mHost->FireOnSelected();
    }

    return NOERROR;
}

AdapterView::AdapterView()
    : mFirstPosition(0)
    , mSyncRowId(AdapterView_INVALID_ROW_ID)
    , mNeedSync(FALSE)
    , mInLayout(FALSE)
    , mDataChanged(FALSE)
    , mNextSelectedPosition(AdapterView_INVALID_POSITION)
    , mNextSelectedRowId(AdapterView_INVALID_ROW_ID)
    , mSelectedPosition(AdapterView_INVALID_POSITION)
    , mSelectedRowId(AdapterView_INVALID_ROW_ID)
    , mOldSelectedPosition(AdapterView_INVALID_POSITION)
    , mOldSelectedRowId(AdapterView_INVALID_ROW_ID)
    , mBlockLayoutRequests(FALSE)
    , mDesiredFocusableState(FALSE)
    , mDesiredFocusableInTouchModeState(FALSE)
{
}

AdapterView::AdapterView(
    /* [in] */ IContext* context)
    : ViewGroup(context)
    , mFirstPosition(0)
    , mSyncRowId(AdapterView_INVALID_ROW_ID)
    , mNeedSync(FALSE)
    , mInLayout(FALSE)
    , mDataChanged(FALSE)
    , mNextSelectedPosition(AdapterView_INVALID_POSITION)
    , mNextSelectedRowId(AdapterView_INVALID_ROW_ID)
    , mSelectedPosition(AdapterView_INVALID_POSITION)
    , mSelectedRowId(AdapterView_INVALID_ROW_ID)
    , mOldSelectedPosition(AdapterView_INVALID_POSITION)
    , mOldSelectedRowId(AdapterView_INVALID_ROW_ID)
    , mBlockLayoutRequests(FALSE)
    , mDesiredFocusableState(FALSE)
    , mDesiredFocusableInTouchModeState(FALSE)
{
}

AdapterView::AdapterView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : ViewGroup(context, attrs)
    , mFirstPosition(0)
    , mSyncRowId(AdapterView_INVALID_ROW_ID)
    , mNeedSync(FALSE)
    , mInLayout(FALSE)
    , mDataChanged(FALSE)
    , mNextSelectedPosition(AdapterView_INVALID_POSITION)
    , mNextSelectedRowId(AdapterView_INVALID_ROW_ID)
    , mSelectedPosition(AdapterView_INVALID_POSITION)
    , mSelectedRowId(AdapterView_INVALID_ROW_ID)
    , mOldSelectedPosition(AdapterView_INVALID_POSITION)
    , mOldSelectedRowId(AdapterView_INVALID_ROW_ID)
    , mBlockLayoutRequests(FALSE)
    , mDesiredFocusableState(FALSE)
    , mDesiredFocusableInTouchModeState(FALSE)
{
}

AdapterView::AdapterView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : ViewGroup(context, attrs, defStyle)
    , mFirstPosition(0)
    , mSyncRowId(AdapterView_INVALID_ROW_ID)
    , mNeedSync(FALSE)
    , mInLayout(FALSE)
    , mDataChanged(FALSE)
    , mNextSelectedPosition(AdapterView_INVALID_POSITION)
    , mNextSelectedRowId(AdapterView_INVALID_ROW_ID)
    , mSelectedPosition(AdapterView_INVALID_POSITION)
    , mSelectedRowId(AdapterView_INVALID_ROW_ID)
    , mOldSelectedPosition(AdapterView_INVALID_POSITION)
    , mOldSelectedRowId(AdapterView_INVALID_ROW_ID)
    , mBlockLayoutRequests(FALSE)
    , mDesiredFocusableState(FALSE)
    , mDesiredFocusableInTouchModeState(FALSE)
{
}

/**
 * Register a callback to be invoked when an item in this AdapterView has
 * been clicked.
 *
 * @param listener The callback that will be invoked.
 */
ECode AdapterView::SetOnItemClickListener(
    /* [in] */ IOnItemClickListener* listener)
{
    mOnItemClickListener = listener;

    return NOERROR;
}

/**
 * @return The callback to be invoked with an item in this AdapterView has
 *         been clicked, or NULL id no callback has been set.
 */
AutoPtr<IOnItemClickListener> AdapterView::GetOnItemClickListener()
{
    return mOnItemClickListener;
}

/**
 * Call the OnItemClickListener, if it is defined.
 *
 * @param view The view within the AdapterView that was clicked.
 * @param position The position of the view in the adapter.
 * @param id The row id of the item that was clicked.
 * @return True if there was an assigned OnItemClickListener that was
 *         called, FALSE otherwise is returned.
 */
Boolean AdapterView::PerformItemClick(
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    if (mOnItemClickListener != NULL) {
        //PlaySoundEffect(SoundEffectConstants.CLICK);
        mOnItemClickListener->OnItemClick(
            (IAdapterView*)this->Probe(EIID_IAdapterView), view, position, id);
        return TRUE;
    }

    return FALSE;
}

/**
 * Register a callback to be invoked when an item in this AdapterView has
 * been clicked and held
 *
 * @param listener The callback that will run
 */
ECode AdapterView::SetOnItemLongClickListener(
    /* [in] */ IOnItemLongClickListener* listener)
{
    if (!IsLongClickable()) {
        SetLongClickable(TRUE);
    }
    mOnItemLongClickListener = listener;

    return NOERROR;
}

/**
 * @return The callback to be invoked with an item in this AdapterView has
 *         been clicked and held, or NULL id no callback as been set.
 */
AutoPtr<IOnItemLongClickListener> AdapterView::GetOnItemLongClickListener()
{
    return mOnItemLongClickListener;
}

/**
 * Register a callback to be invoked when an item in this AdapterView has
 * been selected.
 *
 * @param listener The callback that will run
 */
ECode AdapterView::SetOnItemSelectedListener(
    /* [in] */ IOnItemSelectedListener* listener)
{
    mOnItemSelectedListener = listener;

    return NOERROR;
}

AutoPtr<IOnItemSelectedListener> AdapterView::GetOnItemSelectedListener()
{
    return mOnItemSelectedListener;
}

/**
 * This method is not supported and throws an UnsupportedOperationException when called.
 *
 * @param child Ignored.
 *
 * @throws UnsupportedOperationException Every time this method is invoked.
 */
//@Override
ECode AdapterView::AddView(
    /* [in] */ IView* child)
{
    Logger::E("AdapterView", "addView(View) is not supported in AdapterView");

    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

/**
 * This method is not supported and throws an UnsupportedOperationException when called.
 *
 * @param child Ignored.
 * @param index Ignored.
 *
 * @throws UnsupportedOperationException Every time this method is invoked.
 */
//@Override
ECode AdapterView::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index)
{
    Logger::E("AdapterView",
        "addView(View, Int32) is not supported in AdapterView");

    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode AdapterView::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return ViewGroup::AddView(child, width, height);
}

/**
 * This method is not supported and throws an UnsupportedOperationException when called.
 *
 * @param child Ignored.
 * @param params Ignored.
 *
 * @throws UnsupportedOperationException Every time this method is invoked.
 */
//@Override
ECode AdapterView::AddView(
    /* [in] */ IView* child,
    /* [in] */ IViewGroupLayoutParams* params)
{
    Logger::E("AdapterView",
        "addView(View, LayoutParams) is not supported in AdapterView");

    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

/**
 * This method is not supported and throws an UnsupportedOperationException when called.
 *
 * @param child Ignored.
 * @param index Ignored.
 * @param params Ignored.
 *
 * @throws UnsupportedOperationException Every time this method is invoked.
 */
//@Override
ECode AdapterView::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    Logger::E("AdapterView",
        "addView(View, Int32, LayoutParams) is not supported in AdapterView");

    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

/**
 * This method is not supported and throws an UnsupportedOperationException when called.
 *
 * @param child Ignored.
 *
 * @throws UnsupportedOperationException Every time this method is invoked.
 */
//@Override
ECode AdapterView::RemoveView(
    /* [in] */ IView* child)
{
    Logger::E("AdapterView",
        "removeView(View) is not supported in AdapterView");

    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

/**
 * This method is not supported and throws an UnsupportedOperationException when called.
 *
 * @param index Ignored.
 *
 * @throws UnsupportedOperationException Every time this method is invoked.
 */
//@Override
ECode AdapterView::RemoveViewAt(
    /* [in] */ Int32 index)
{
    Logger::E("AdapterView",
        "removeViewAt(Int32) is not supported in AdapterView");

    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

/**
 * This method is not supported and throws an UnsupportedOperationException when called.
 *
 * @throws UnsupportedOperationException Every time this method is invoked.
 */
//@Override
ECode AdapterView::RemoveAllViews()
{
    Logger::E("AdapterView",
        "removeAllViews() is not supported in AdapterView");

    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//@Override
void AdapterView::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    mLayoutHeight = GetHeight();
}

/**
 * Return the position of the currently selected item within the adapter's data set
 *
 * @return Int32 Position (starting at 0), or {@link #AdapterView_INVALID_POSITION} if there is nothing selected.
 */
//@ViewDebug.CapturedViewProperty
Int32 AdapterView::GetSelectedItemPosition()
{
    return mNextSelectedPosition;
}

/**
 * @return The id corresponding to the currently selected item, or {@link #AdapterView_INVALID_ROW_ID}
 * if nothing is selected.
 */
//@ViewDebug.CapturedViewProperty
Int64 AdapterView::GetSelectedItemId()
{
    return mNextSelectedRowId;
}

/**
 * @return The data corresponding to the currently selected item, or
 * NULL if there is nothing selected.
 */
AutoPtr<IInterface> AdapterView::GetSelectedItem()
{
    AutoPtr<IAdapter> adapter = GetAdapter();
    Int32 selection = GetSelectedItemPosition();
    if (adapter != NULL) {
        Int32 count;
        adapter->GetCount(&count);
        if (count && selection >= 0) {
            AutoPtr<IInterface> item;
            adapter->GetItem(selection, (IInterface**)&item);
            return item;
        }
    }

    return AutoPtr<IInterface>(NULL);
}

/**
 * @return The number of items owned by the Adapter associated with this
 *         AdapterView. (This is the number of data items, which may be
 *         larger than the number of visible view.)
 */
//@ViewDebug.CapturedViewProperty
Int32 AdapterView::GetCount()
{
    return mItemCount;
}

/**
 * Get the position within the adapter's data set for the view, where view is a an adapter item
 * or a descendant of an adapter item.
 *
 * @param view an adapter item, or a descendant of an adapter item. This must be visible in this
 *        AdapterView at the time of the call.
 * @return the position within the adapter's data set of the view, or {@link #AdapterView_INVALID_POSITION}
 *         if the view does not correspond to a list item (or it is not currently visible).
 */
Int32 AdapterView::GetPositionForView(
    /* [in] */ IView* view)
{
    assert(view);
    IView* listItem = view;
    AutoPtr<IViewParent> vp;
    listItem->GetParent((IViewParent**)&vp);
    IView* v = (IView*)vp->Probe(EIID_IView);
    while (v != (IView*)this->Probe(EIID_IView)) {
        if (v == NULL) {
            return AdapterView_INVALID_POSITION;
        }
        listItem = v;
        vp = NULL;
        listItem->GetParent((IViewParent**)&vp);
        v = (IView*)vp->Probe(EIID_IView);
    }

    // Search the children for the list item
    Int32 childCount = GetChildCount();
    for (Int32 i = 0; i < childCount; i++) {
        AutoPtr<IView> child = GetChildAt(i);
        if (child.Get() == listItem) {
            return mFirstPosition + i;
        }
    }

    // Child not found!
    return AdapterView_INVALID_POSITION;
}

/**
 * Returns the position within the adapter's data set for the first item
 * displayed on screen.
 *
 * @return The position within the adapter's data set
 */
Int32 AdapterView::GetFirstVisiblePosition()
{
    return mFirstPosition;
}

/**
 * Returns the position within the adapter's data set for the last item
 * displayed on screen.
 *
 * @return The position within the adapter's data set
 */
Int32 AdapterView::GetLastVisiblePosition()
{
    return mFirstPosition + GetChildCount() - 1;
}

/**
 * Sets the view to show if the adapter is empty
 */
ECode AdapterView::SetEmptyView(
    /* [in] */ IView* emptyView)
{
    mEmptyView = emptyView;

    AutoPtr<IAdapter> adapter = GetAdapter();
    Boolean empty = TRUE;
    if (adapter != NULL) {
        adapter->IsEmpty(&empty);
    }
    UpdateEmptyStatus(empty);

    return NOERROR;
}

/**
 * When the current adapter is empty, the AdapterView can display a special view
 * call the empty view. The empty view is used to provide feedback to the user
 * that no data is available in this AdapterView.
 *
 * @return The view to show if the adapter is empty.
 */
AutoPtr<IView> AdapterView::GetEmptyView()
{
    return mEmptyView;
}

/**
 * Indicates whether this view is in filter mode. Filter mode can for instance
 * be enabled by a user when typing on the keyboard.
 *
 * @return True if the view is in filter mode, FALSE otherwise.
 */
Boolean AdapterView::IsInFilterMode() {
    return FALSE;
}

//@Override
ECode AdapterView::SetFocusable(
    /* [in] */ Boolean focusable)
{
    AutoPtr<IAdapter> adapter = GetAdapter();
    Boolean empty = TRUE;
    if (adapter != NULL) {
        Int32 count;
        adapter->GetCount(&count);
        empty = (count == 0);
    }

    mDesiredFocusableState = focusable;
    if (!focusable) {
        mDesiredFocusableInTouchModeState = FALSE;
    }

    return ViewGroup::SetFocusable(focusable && (!empty || IsInFilterMode()));
}

//@Override
ECode AdapterView::SetFocusableInTouchMode(
    /* [in] */ Boolean focusable)
{
    AutoPtr<IAdapter> adapter = GetAdapter();
    Boolean empty = TRUE;
    if (adapter != NULL) {
        Int32 count;
        adapter->GetCount(&count);
        empty = (count == 0);
    }

    mDesiredFocusableInTouchModeState = focusable;
    if (focusable) {
        mDesiredFocusableState = TRUE;
    }

    return ViewGroup::SetFocusableInTouchMode(
        focusable && (!empty || IsInFilterMode()));
}

ECode AdapterView::CheckFocus()
{
    AutoPtr<IAdapter> adapter = GetAdapter();
    Boolean empty = TRUE;
    if (adapter != NULL) {
        Int32 count;
        adapter->GetCount(&count);
        empty = (count == 0);
    }

    Boolean focusable = !empty || IsInFilterMode();
    // The order in which we set focusable in touch mode/focusable may matter
    // for the client, see View.setFocusableInTouchMode() comments for more
    // details
    ViewGroup::SetFocusableInTouchMode(focusable && mDesiredFocusableInTouchModeState);
    ViewGroup::SetFocusable(focusable && mDesiredFocusableState);
    if (mEmptyView != NULL) {
        Boolean isEmpty = TRUE;
        if (adapter != NULL) {
            adapter->IsEmpty(&isEmpty);
        }
        UpdateEmptyStatus(isEmpty);
    }

    return NOERROR;
}

/**
 * Update the status of the list based on the empty parameter.  If empty is TRUE and
 * we have an empty view, display it.  In all the other cases, make sure that the listview
 * is VISIBLE and that the empty view is GONE (if it's not NULL).
 */
void AdapterView::UpdateEmptyStatus(
    /* [in] */ Boolean empty)
{
    if (IsInFilterMode()) {
        empty = FALSE;
    }

    if (empty) {
        if (mEmptyView != NULL) {
            mEmptyView->SetVisibility(View::VISIBLE);
            SetVisibility(View::GONE);
        }
        else {
            // If the caller just removed our empty view, make sure the list view is visible
            SetVisibility(View::VISIBLE);
        }

        // We are now GONE, so pending layouts will not be dispatched.
        // Force one here to make sure that the state of the list matches
        // the state of the adapter.
        if (mDataChanged) {
            OnLayout(FALSE, mLeft, mTop, mRight, mBottom);
        }
    }
    else {
        if (mEmptyView != NULL) {
            mEmptyView->SetVisibility(View::GONE);
        }
        SetVisibility(View::VISIBLE);
    }
}

/**
 * Gets the data associated with the specified position in the list.
 *
 * @param position Which data to get
 * @return The data associated with the specified position in the list
 */
AutoPtr<IInterface> AdapterView::GetItemAtPosition(
    /* [in] */ Int32 position)
{
    AutoPtr<IAdapter> adapter = GetAdapter();
    if (adapter == NULL || position < 0) {
        return AutoPtr<IInterface>(NULL);
    }
    else {
        AutoPtr<IInterface> item;
        adapter->GetItem(position, (IInterface**)&item);
        return item;
    }
}

Int64 AdapterView::GetItemIdAtPosition(
    /* [in] */ Int32 position)
{
    AutoPtr<IAdapter> adapter = GetAdapter();
    if (adapter == NULL || position < 0) {
        return AdapterView_INVALID_ROW_ID;
    }
    else {
        Int64 itemId;
        adapter->GetItemId(position, &itemId);
        return itemId;
    }
}

//@Override
ECode AdapterView::SetOnClickListener(
    /* [in] */ IViewOnClickListener* l)
{
    Logger::E("AdapterView",
        StringBuffer("Don't call setOnClickListener for an AdapterView. ")
        + "You probably want setOnItemClickListener instead");

    return E_RUNTIME_EXCEPTION;
}

/**
 * Override to prevent freezing of any views created by the adapter.
 */
//@Override
ECode AdapterView::DispatchSaveInstanceState(
    /* [in] */ IObjectIntegerMap* container)
{
    return DispatchFreezeSelfOnly(container);
}

/**
 * Override to prevent thawing of any views created by the adapter.
 */
//@Override
ECode AdapterView::DispatchRestoreInstanceState(
    /* [in] */ IObjectIntegerMap* container)
{
    return DispatchThawSelfOnly(container);
}

//@Override
void AdapterView::OnDetachedFromWindow()
{
    ViewGroup::OnDetachedFromWindow();
    RemoveCallbacks(mSelectionNotifier);
}

void AdapterView::SelectionChanged()
{
    if (mOnItemSelectedListener != NULL) {
        if (mInLayout || mBlockLayoutRequests) {
            // If we are in a layout traversal, defer notification
            // by posting. This ensures that the view tree is
            // in a consistent state and is able to accomodate
            // new layout or invalidate requests.
            if (mSelectionNotifier == NULL) {
                mSelectionNotifier = new SelectionNotifier(this);
            }
            Post(mSelectionNotifier);
        }
        else {
            FireOnSelected();
        }
    }

    // we fire selection events here not in View
    if (mSelectedPosition != AdapterView_INVALID_POSITION/*ListView::AdapterView_INVALID_POSITION*/
        && IsShown() && !IsInTouchMode()) {
        SendAccessibilityEvent(AccessibilityEvent_TYPE_VIEW_SELECTED);
    }
}

void AdapterView::FireOnSelected()
{
    if (mOnItemSelectedListener == NULL) {
        return;
    }

    Int32 selection = GetSelectedItemPosition();
    if (selection >= 0) {
        AutoPtr<IView> v = GetSelectedView();
        AutoPtr<IAdapter> adapter = GetAdapter();
        Int64 itemId;
        adapter->GetItemId(selection, &itemId);

        mOnItemSelectedListener->OnItemSelected(
            (IAdapterView*)this->Probe(EIID_IAdapterView),
            v, selection, itemId);
    }
    else {
        mOnItemSelectedListener->OnNothingSelected(
            (IAdapterView*)this->Probe(EIID_IAdapterView));
    }
}

Boolean AdapterView::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    Boolean populated = FALSE;
    // This is an exceptional case which occurs when a window gets the
    // focus and sends a focus event via its focused child to announce
    // current focus/selection. AdapterView fires selection but not focus
    // events so we change the event type here.
    //
    //if (event.getEventType() == AccessibilityEvent.TYPE_VIEW_FOCUSED) {
    //    event.setEventType(AccessibilityEvent.TYPE_VIEW_SELECTED);
    //}

    //// we send selection events only from AdapterView to avoid
    //// generation of such event for each child
    //View selectedView = getSelectedView();
    //if (selectedView != NULL) {
    //    populated = selectedView.dispatchPopulateAccessibilityEvent(event);
    //}

    //if (!populated) {
    //    if (selectedView != NULL) {
    //        event.setEnabled(selectedView.isEnabled());
    //    }
    //    event.setItemCount(getCount());
    //    event.setCurrentItemIndex(getSelectedItemPosition());
    //}

    return populated;
}

Boolean AdapterView::CanAnimate()
{
    return ViewGroup::CanAnimate() && mItemCount > 0;
}

void AdapterView::HandleDataChanged()
{
    Int32 count = mItemCount;
    Boolean found = FALSE;

    if (count > 0) {
        Int32 newPos;

        // Find the row we are supposed to sync to
        if (mNeedSync) {
            // Update this first, since setNextSelectedPositionInt inspects
            // it
            mNeedSync = FALSE;

            // See if we can find a position in the new data with the same
            // id as the old selection
            //
            newPos = FindSyncPosition();
            if (newPos >= 0) {
                // Verify that new selection is selectable
                Int32 selectablePos = LookForSelectablePosition(newPos, TRUE);
                if (selectablePos == newPos) {
                    // Same row id is selected
                    SetNextSelectedPositionInt(newPos);
                    found = TRUE;
                }
            }
        }

        if (!found) {
            // Try to use the same position if we can't find matching data
            newPos = GetSelectedItemPosition();

            // Pin position to the available range
            if (newPos >= count) {
                newPos = count - 1;
            }

            if (newPos < 0) {
                newPos = 0;
            }

            // Make sure we select something selectable -- first look down
            Int32 selectablePos = LookForSelectablePosition(newPos, TRUE);
            if (selectablePos < 0) {
                // Looking down didn't work -- try looking up
                selectablePos = LookForSelectablePosition(newPos, FALSE);
            }

            if (selectablePos >= 0) {
                SetNextSelectedPositionInt(selectablePos);
                CheckSelectionChanged();
                found = TRUE;
            }
        }
    }

    if (!found) {
        // Nothing is selected
        mSelectedPosition = AdapterView_INVALID_POSITION;
        mSelectedRowId = AdapterView_INVALID_ROW_ID;
        mNextSelectedPosition = AdapterView_INVALID_POSITION;
        mNextSelectedRowId = AdapterView_INVALID_ROW_ID;
        mNeedSync = FALSE;
        CheckSelectionChanged();
    }
}

void AdapterView::CheckSelectionChanged()
{
    if ((mSelectedPosition != mOldSelectedPosition)
        || (mSelectedRowId != mOldSelectedRowId)) {
        SelectionChanged();
        mOldSelectedPosition = mSelectedPosition;
        mOldSelectedRowId = mSelectedRowId;
    }
}

/**
 * Searches the adapter for a position matching mSyncRowId. The search starts at mSyncPosition
 * and then alternates between moving up and moving down until 1) we find the right position, or
 * 2) we run out of time, or 3) we have looked at every position
 *
 * @return Position of the row that matches mSyncRowId, or {@link #AdapterView_INVALID_POSITION} if it can't
 *         be found
 */
Int32 AdapterView::FindSyncPosition()
{
    Int32 count = mItemCount;

    if (count == 0) {
        return AdapterView_INVALID_POSITION;
    }

    Int64 idToMatch = mSyncRowId;
    Int32 seed = mSyncPosition;

    // If there isn't a selection don't hunt for it
    if (idToMatch == AdapterView_INVALID_ROW_ID) {
        return AdapterView_INVALID_POSITION;
    }

    // Pin seed to reasonable values
    seed = Math::Max(0, seed);
    seed = Math::Min(count - 1, seed);

    Int64 endTime = SystemClock::GetUptimeMillis() + SYNC_MAX_DURATION_MILLIS;
    Int64 rowId;

    // first position scanned so far
    Int32 first = seed;

    // last position scanned so far
    Int32 last = seed;

    // True if we should move down on the next iteration
    Boolean next = FALSE;

    // True when we have looked at the first item in the data
    Boolean hitFirst;

    // True when we have looked at the last item in the data
    Boolean hitLast;

    // Get the item ID locally (instead of getItemIdAtPosition), so
    // we need the adapter
    AutoPtr<IAdapter> adapter = GetAdapter();
    if (adapter == NULL) {
        return AdapterView_INVALID_POSITION;
    }

    while (SystemClock::GetUptimeMillis() <= endTime) {
        adapter->GetItemId(seed, &rowId);
        if (rowId == idToMatch) {
            // Found it!
            return seed;
        }

        hitLast = last == count - 1;
        hitFirst = first == 0;

        if (hitLast && hitFirst) {
            // Looked at everything
            break;
        }

        if (hitFirst || (next && !hitLast)) {
            // Either we hit the top, or we are trying to move down
            last++;
            seed = last;
            // Try going up next time
            next = FALSE;
        }
        else if (hitLast || (!next && !hitFirst)) {
            // Either we hit the bottom, or we are trying to move up
            first--;
            seed = first;
            // Try going down next time
            next = TRUE;
        }
    }

    return AdapterView_INVALID_POSITION;
}

/**
 * Find a position that can be selected (i.e., is not a separator).
 *
 * @param position The starting position to look at.
 * @param lookDown Whether to look down for other positions.
 * @return The next selectable position starting at position and then searching either up or
 *         down. Returns {@link #AdapterView_INVALID_POSITION} if nothing can be found.
 */
Int32 AdapterView::LookForSelectablePosition(
    /* [in] */ Int32 position,
    /* [in] */ Boolean lookDown)
{
    return position;
}

/**
 * Utility to keep mSelectedPosition and mSelectedRowId in sync
 * @param position Our current position
 */
void AdapterView::SetSelectedPositionInt(
    /* [in] */ Int32 position)
{
    mSelectedPosition = position;
    mSelectedRowId = GetItemIdAtPosition(position);
}

/**
 * Utility to keep mNextSelectedPosition and mNextSelectedRowId in sync
 * @param position Intended value for mSelectedPosition the next time we go
 * through layout
 */
void AdapterView::SetNextSelectedPositionInt(
    /* [in] */ Int32 position)
{
    mNextSelectedPosition = position;
    mNextSelectedRowId = GetItemIdAtPosition(position);
    // If we are trying to sync to the selection, update that too
    if (mNeedSync && mSyncMode == SYNC_SELECTED_POSITION && position >= 0) {
        mSyncPosition = position;
        mSyncRowId = mNextSelectedRowId;
    }
}

/**
 * Remember enough information to restore the screen state when the data has
 * changed.
 *
 */
void AdapterView::RememberSyncState()
{
    if (GetChildCount() > 0) {
        mNeedSync = TRUE;
        mSyncHeight = mLayoutHeight;
        if (mSelectedPosition >= 0) {
            // Sync the selection state
            AutoPtr<IView> v = GetChildAt(mSelectedPosition - mFirstPosition);
            mSyncRowId = mNextSelectedRowId;
            mSyncPosition = mNextSelectedPosition;
            if (v != NULL) {
                v->GetTop(&mSpecificTop);
            }
            mSyncMode = SYNC_SELECTED_POSITION;
        }
        else {
            // Sync the based on the offset of the first view
            AutoPtr<IView> v = GetChildAt(0);
            AutoPtr<IAdapter> adapter = GetAdapter();
            Int32 count;
            adapter->GetCount(&count);

            if (mFirstPosition >= 0 && mFirstPosition < count) {
                adapter->GetItemId(mFirstPosition, &mSyncRowId);
            }
            else {
                mSyncRowId = NO_ID;
            }

            mSyncPosition = mFirstPosition;
            if (v != NULL) {
                v->GetTop(&mSpecificTop);
            }
            mSyncMode = SYNC_FIRST_POSITION;
        }
    }
}
