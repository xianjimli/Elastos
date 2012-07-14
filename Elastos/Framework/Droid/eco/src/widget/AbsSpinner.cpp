
#include "widget/AbsSpinner.h"
#include "view/CViewGroupLayoutParams.h"
#include "widget/CAbsSpinnerSavedState.h"
#include "widget/CArrayAdapter.h"
#include "utils/AutoStringArray.h"
#include "utils/CParcelableObjectContainer.h"
#include <elastos/Math.h>

static Int32 R_Styleable_AbsSpinner[] = {
    0x010100b2
};

static const Int32 R_Styleable_AbsSpinner_Entries = 0;

static const Int32 R_Layout_Simple_Spinner_Item = 0x01090008;

static const Int32 R_Layout_Simple_Spinner_Dropdown_Item = 0x01090009;

AbsSpinner::AbsSpinner(
    /* [in] */ IContext* context) : AdapterView(context)
{
    InitAbsSpinner();
}

AbsSpinner::AbsSpinner(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs) : AdapterView(context, attrs, 0)
{
    InitAbsSpinner();

    Init(context, attrs);
}

AbsSpinner::AbsSpinner(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle) : AdapterView(context, attrs, defStyle)
{
    InitAbsSpinner();

    Init(context, attrs, defStyle);
}

AbsSpinner::~AbsSpinner()
{
    delete mRecycler;
}

void AbsSpinner::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(attrs,
        ArrayOf<Int32>(R_Styleable_AbsSpinner, 1), defStyle, 0, (ITypedArray**)&a);

    AutoStringArray entries;
    a->GetTextArray(
        R_Styleable_AbsSpinner_Entries, (ArrayOf<String>**)&entries);

    if (entries != NULL) {

        AutoPtr<IObjectContainer> strs;
        CParcelableObjectContainer::New((IObjectContainer**)&strs);

        Int32 size = entries->GetLength();
        for (Int32 i = 0; i < size; i++) {
            AutoPtr<ICharSequence> cs;
            CStringWrapper::New((*entries)[i], (ICharSequence**)&cs);
            strs->Add(cs);
        }

        AutoPtr<IArrayAdapter> adapter;
        CArrayAdapter::New(context, R_Layout_Simple_Spinner_Item, strs, (IArrayAdapter**)&adapter);


        adapter->SetDropDownViewResource(R_Layout_Simple_Spinner_Dropdown_Item);
        SetAdapter(adapter);
    }

    a->Recycle();
}

/**
 * Common code for different constructor flavors
 */
void AbsSpinner::InitAbsSpinner()
{
    mSelectionLeftPadding = 0;
    mSelectionTopPadding = 0;
    mSelectionRightPadding = 0;
    mSelectionBottomPadding = 0;

    CRect::NewByFriend((CRect**)&mSpinnerPadding);

    mRecycler = new RecycleBin(this);

    SetFocusable(TRUE);
    SetWillNotDraw(FALSE);
}

/**
 * The Adapter is used to provide the data which backs this Spinner.
 * It also provides methods to transform spinner items based on their position
 * relative to the selected item.
 * @param adapter The SpinnerAdapter to use for this Spinner
 */
ECode AbsSpinner::SetAdapter(
    /* [in] */ IAdapter* adapter)
{
    if (NULL != mAdapter.Get()) {
        mAdapter->UnregisterDataSetObserver(mDataSetObserver);
        ResetList();
    }

    mAdapter = adapter;

    mOldSelectedPosition = AdapterView_INVALID_POSITION;
    mOldSelectedRowId = AdapterView_INVALID_ROW_ID;

    if (mAdapter.Get() != NULL) {
        mOldItemCount = mItemCount;
        mAdapter->GetCount(&mItemCount);
        CheckFocus();

        mDataSetObserver = new AdapterDataSetObserver(this);
        mAdapter->RegisterDataSetObserver(mDataSetObserver);

        Int32 position = mItemCount > 0 ? 0 : AdapterView_INVALID_POSITION;

        SetSelectedPositionInt(position);
        SetNextSelectedPositionInt(position);

        if (mItemCount == 0) {
            // Nothing selected
            CheckSelectionChanged();
        }

    } else {
        CheckFocus();
        ResetList();
        // Nothing selected
        CheckSelectionChanged();
    }

    RequestLayout();

    return NOERROR;
}

/**
 * Clear out all children from the list
 */
void AbsSpinner::ResetList()
{
    mDataChanged = FALSE;
    mNeedSync = FALSE;

    RemoveAllViewsInLayout();
    mOldSelectedPosition = AdapterView_INVALID_POSITION;
    mOldSelectedRowId = AdapterView_INVALID_ROW_ID;

    SetSelectedPositionInt(AdapterView_INVALID_POSITION);
    SetNextSelectedPositionInt(AdapterView_INVALID_POSITION);
    Invalidate();
}

/**
 * @see android.view.View#measure(Int32, Int32)
 *
 * Figure out the dimensions of this Spinner. The width comes from
 * the widthMeasureSpec as Spinnners can't have their width set to
 * UNSPECIFIED. The height is based on the height of the selected item
 * plus padding.
 */
void AbsSpinner::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 widthMode = MeasureSpec::GetMode(widthMeasureSpec);
    Int32 widthSize;
    Int32 heightSize;

    mSpinnerPadding->mLeft = mPaddingLeft > mSelectionLeftPadding ? mPaddingLeft
            : mSelectionLeftPadding;
    mSpinnerPadding->mTop = mPaddingTop > mSelectionTopPadding ? mPaddingTop
            : mSelectionTopPadding;
    mSpinnerPadding->mRight = mPaddingRight > mSelectionRightPadding ? mPaddingRight
            : mSelectionRightPadding;
    mSpinnerPadding->mBottom = mPaddingBottom > mSelectionBottomPadding ? mPaddingBottom
            : mSelectionBottomPadding;

    if (mDataChanged) {
        HandleDataChanged();
    }

    Int32 preferredHeight = 0;
    Int32 preferredWidth = 0;
    Boolean needsMeasuring = TRUE;

    Int32 selectedPosition = GetSelectedItemPosition();

    Int32 count;
    if (mAdapter.Get()) {
        mAdapter->GetCount(&count);
    }

    if (selectedPosition >= 0 && mAdapter.Get() != NULL && selectedPosition < count) {
        // Try looking in the recycler. (Maybe we were measured once already)
        AutoPtr<IView> view = mRecycler->Get(selectedPosition);
        if (view.Get() == NULL) {
            // Make a new one
            mAdapter->GetView(selectedPosition, NULL, (IViewGroup*)this->Probe(EIID_IViewGroup), (IView**)&view);
        }

        if (view.Get() != NULL) {
            // Put in recycler for re-measuring and/or layout
            mRecycler->Put(selectedPosition, view);
        }

        if (view.Get() != NULL) {
            AutoPtr<IViewGroupLayoutParams> params;
            view->GetLayoutParams((IViewGroupLayoutParams**)&params);
            if (params.Get() == NULL) {
                mBlockLayoutRequests = TRUE;
                GenerateDefaultLayoutParams((IViewGroupLayoutParams**)&params);
                view->SetLayoutParams(params);
                mBlockLayoutRequests = FALSE;
            }
            MeasureChild(view, widthMeasureSpec, heightMeasureSpec);

            preferredHeight = GetChildHeight(view) + mSpinnerPadding->mTop + mSpinnerPadding->mBottom;
            preferredWidth = GetChildWidth(view) + mSpinnerPadding->mLeft + mSpinnerPadding->mRight;

            needsMeasuring = FALSE;
        }
    }

    if (needsMeasuring) {
        // No views -- just use padding
        preferredHeight = mSpinnerPadding->mTop + mSpinnerPadding->mBottom;
        if (widthMode == MeasureSpec::UNSPECIFIED) {
            preferredWidth = mSpinnerPadding->mLeft + mSpinnerPadding->mRight;
        }
    }

    preferredHeight = Math::Max(preferredHeight, GetSuggestedMinimumHeight());
    preferredWidth = Math::Max(preferredWidth, GetSuggestedMinimumWidth());

    heightSize = ResolveSize(preferredHeight, heightMeasureSpec);
    widthSize = ResolveSize(preferredWidth, widthMeasureSpec);

    SetMeasuredDimension(widthSize, heightSize);
    mHeightMeasureSpec = heightMeasureSpec;
    mWidthMeasureSpec = widthMeasureSpec;
}

Int32 AbsSpinner::GetChildHeight(
    /* [in] */ IView* child)
{
    Int32 height;
    child->GetMeasuredHeight(&height);

    return height;
}

Int32 AbsSpinner::GetChildWidth(
    /* [in] */ IView* child)
{
    Int32 width;
    child->GetMeasuredWidth(&width);

    return width;
}

ECode AbsSpinner::GenerateDefaultLayoutParams(
    /* [out] */ IViewGroupLayoutParams** params)
{
    return CViewGroupLayoutParams::New(ViewGroupLayoutParams_MATCH_PARENT,
            ViewGroupLayoutParams_WRAP_CONTENT, params);
}

void AbsSpinner::RecycleAllViews()
{
    Int32 childCount = GetChildCount();
    RecycleBin* recycleBin = mRecycler;
    Int32 position = mFirstPosition;

    // All views go in recycler
    for (Int32 i = 0; i < childCount; i++) {
        AutoPtr<IView> v = GetChildAt(i);
        Int32 index = position + i;
        recycleBin->Put(index, v);
    }
}

/**
 * Jump directly to a specific item in the adapter data.
 */
ECode AbsSpinner::SetSelection(
    /* [in] */ Int32 position,
    /* [in] */ Boolean animate)
{
    // Animate only if requested position is already on screen somewhere
    Boolean shouldAnimate = animate && mFirstPosition <= position &&
            position <= mFirstPosition + GetChildCount() - 1;
    SetSelectionInt(position, shouldAnimate);

    return NOERROR;
}

ECode AbsSpinner::SetSelection(
    /* [in] */ Int32 position)
{
    SetNextSelectedPositionInt(position);
    RequestLayout();
    Invalidate();

    return NOERROR;
}


/**
 * Makes the item at the supplied position selected.
 *
 * @param position Position to select
 * @param animate Should the transition be animated
 *
 */
void AbsSpinner::SetSelectionInt(
    /* [in] */ Int32 position,
    /* [in] */ Boolean animate)
{
    if (position != mOldSelectedPosition) {
        mBlockLayoutRequests = TRUE;
        Int32 delta  = position - mSelectedPosition;
        SetNextSelectedPositionInt(position);
        LayoutEx(delta, animate);
        mBlockLayoutRequests = FALSE;
    }
}

//abstract void layout(Int32 delta, Boolean animate);

AutoPtr<IView> AbsSpinner::GetSelectedView()
{
    if (mItemCount > 0 && mSelectedPosition >= 0) {
        return GetChildAt(mSelectedPosition - mFirstPosition);
    } else {
        return NULL;
    }
}

/**
 * Override to prevent spamming ourselves with layout requests
 * as we place views
 *
 * @see android.view.View#requestLayout()
 */
ECode AbsSpinner::RequestLayout()
{
    if (!mBlockLayoutRequests) {
        AdapterView::RequestLayout();
    }

    return NOERROR;
}

AutoPtr<IAdapter> AbsSpinner::GetAdapter()
{
    return mAdapter;
}

Int32 AbsSpinner::GetCount()
{
    return mItemCount;
}

/**
 * Maps a point to a position in the list.
 *
 * @param x X in local coordinate
 * @param y Y in local coordinate
 * @return The position of the item which contains the specified point, or
 *         {@link #INVALID_POSITION} if the point does not intersect an item.
 */
Int32 AbsSpinner::PointToPosition(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    AutoPtr<IRect> frame = mTouchFrame;
    if (frame.Get() == NULL) {
        CRect::New((IRect**)&mTouchFrame);
        frame = mTouchFrame;
    }

    Int32 count = GetChildCount();
    for (Int32 i = count - 1; i >= 0; i--) {
        AutoPtr<IView> child = GetChildAt(i);
        Int32 visible;
        child->GetVisibility(&visible);
        if (visible == View::VISIBLE) {
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

AutoPtr<IParcelable> AbsSpinner::OnSaveInstanceState()
{
    AutoPtr<IParcelable> superState = AdapterView::OnSaveInstanceState();
    AutoPtr<IAbsSpinnerSavedState> ss;
    CAbsSpinnerSavedState::New(superState, (IAbsSpinnerSavedState**)&ss);

    CAbsSpinnerSavedState* saveState = (CAbsSpinnerSavedState*)ss.Get();
    saveState->mSelectedId = GetSelectedItemId();
    if (saveState->mSelectedId >= 0) {
        saveState->mPosition = GetSelectedItemPosition();
    } else {
        saveState->mPosition = AdapterView_INVALID_POSITION;
    }
    return (IParcelable*)ss.Get();
}

void AbsSpinner::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    IAbsSpinnerSavedState* ss = (IAbsSpinnerSavedState*) state;

    AutoPtr<IParcelable> p;
    ss->GetSuperState((IParcelable**)&p);
    AdapterView::OnRestoreInstanceState(p);

    CAbsSpinnerSavedState* saveState = (CAbsSpinnerSavedState*)ss;
    if (saveState->mSelectedId >= 0) {
        mDataChanged = TRUE;
        mNeedSync = TRUE;
        mSyncRowId = saveState->mSelectedId;
        mSyncPosition = saveState->mPosition;
        mSyncMode = SYNC_SELECTED_POSITION;
        RequestLayout();
    }
}

AbsSpinner::RecycleBin::RecycleBin(
    /* [in] */ AbsSpinner* host) : mHost(host)
{
    assert(host);
}

void AbsSpinner::RecycleBin::Put(
    /* [in] */ Int32 position,
    /* [in] */ IView* v)
{
    mScrapHeap.Put(position, v);
}

IView* AbsSpinner::RecycleBin::Get(
    /* [in] */ Int32 position)
{
    // System.out.print("Looking for " + position);
    IView* result = (IView*)mScrapHeap.Get(position);
    if (result != NULL) {
        // System.out.println(" HIT");
        mScrapHeap.Delete(position);
    } else {
        // System.out.println(" MISS");
    }
    return result;
}

void AbsSpinner::RecycleBin::Clear()
{
    Int32 count = mScrapHeap.Size();
    for (Int32 i = 0; i < count; i++) {
        IView* view = (IView*)mScrapHeap.ValueAt(i);
        if (view != NULL) {
            mHost->RemoveDetachedView(view, TRUE);
        }
    }
    mScrapHeap.Clear();
}

