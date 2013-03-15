
#include "widget/AutoCompleteTextView.h"
#include "widget/CPopupWindow.h"
#include "widget/CLinearLayout.h"
#include "widget/CLinearLayoutLayoutParams.h"
#include "view/LayoutInflater.h"
#include "view/inputmethod/CLocalInputMethodManager.h"
#include "view/inputmethod/CCompletionInfo.h"
#include "text/TextUtils.h"
#include "text/Selection.h"
#include <elastos/Math.h>


const Boolean AutoCompleteTextView::DEBUG;
//const String AutoCompleteTextView::TAG;

const Int32 AutoCompleteTextView::HINT_VIEW_ID;

/**
 * This value controls the length of time that the user
 * must leave a pointer down without scrolling to expand
 * the autocomplete dropdown list to cover the IME.
 */
const Int32 AutoCompleteTextView::EXPAND_LIST_TIMEOUT;

static const Int32 R_Attr_AutoCompleteTextViewStyle = 0x0101006b;

static Int32 R_Styleable_AutoCompleteTextView[] = {
    0x01010172, 0x01010173, 0x01010174, 0x01010175,
    0x01010220, 0x01010262, 0x01010263, 0x01010283,
    0x010102ac, 0x010102ad
};

static const Int32 R_Styleable_AutoCompleteTextView_completionHint = 0;
static const Int32 R_Styleable_AutoCompleteTextView_completionHintView = 1;
static const Int32 R_Styleable_AutoCompleteTextView_completionThreshold = 2;
static const Int32 R_Styleable_AutoCompleteTextView_dropDownSelector = 3;
static const Int32 R_Styleable_AutoCompleteTextView_dropDownWidth = 5;
static const Int32 R_Styleable_AutoCompleteTextView_dropDownAnchor = 6;
static const Int32 R_Styleable_AutoCompleteTextView_dropDownHeight = 7;
static const Int32 R_Styleable_AutoCompleteTextView_dropDownHorizontalOffset = 8;
static const Int32 R_Styleable_AutoCompleteTextView_dropDownVerticalOffset = 9;

static const Int32 R_Id_Text1 = 0x01020014;

static const Int32 R_Layout_Simple_dropdown_hint = 0x0109005f;

static const Int32 R_Attr_DropDownListViewStyle = 0x0101006d;

AutoCompleteTextView::AutoCompleteTextView()
    : mDropDownAlwaysVisible(FALSE)
    , mDropDownDismissedOnCompletion(TRUE)
    , mForceIgnoreOutsideTouch(FALSE)
    , mLastKeyCode(KeyEvent_KEYCODE_UNKNOWN)
    , mOpenBefore(FALSE)
    , mBlockCompletion(FALSE)
{
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mTempRect));
    mDropDownItemClickListener = new DropDownItemClickListener(this);
    mResizePopupRunnable = new ResizePopupRunnable(this);
}

AutoCompleteTextView::AutoCompleteTextView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : mDropDownAlwaysVisible(FALSE)
    , mDropDownDismissedOnCompletion(TRUE)
    , mForceIgnoreOutsideTouch(FALSE)
    , mLastKeyCode(KeyEvent_KEYCODE_UNKNOWN)
    , mOpenBefore(FALSE)
    , mBlockCompletion(FALSE)
{
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mTempRect));
    mDropDownItemClickListener = new DropDownItemClickListener(this);
    mResizePopupRunnable = new ResizePopupRunnable(this);
    Init(context, attrs, defStyle);
}

ECode AutoCompleteTextView::SetOnClickListener(
    /* [in] */ IViewOnClickListener* listener)
{
    mPassThroughClickListener->mWrapped = listener;

    return NOERROR;
}

/**
 * Private hook into the on click event, dispatched from {@link PassThroughClickListener}
 */
void AutoCompleteTextView::OnClickImpl()
{
    // If the dropdown is showing, bring the keyboard to the front
    // when the user touches the text field.

    Boolean isShowing;
    mPopup->IsShowing(&isShowing);

    if (isShowing) {
        EnsureImeVisible(TRUE);
    }
}

/**
 * <p>Sets the optional hint text that is displayed at the bottom of the
 * the matching list.  This can be used as a cue to the user on how to
 * best use the list, or to provide extra information.</p>
 *
 * @param hint the text to be displayed to the user
 *
 * @attr ref android.R.styleable#AutoCompleteTextView_completionHint
 */
ECode AutoCompleteTextView::SetCompletionHint(
    /* [in] */ ICharSequence* hint)
{
    mHintText = hint;

    return NOERROR;
}

/**
 * <p>Returns the current width for the auto-complete drop down list. This can
 * be a fixed width, or {@link ViewGroup.LayoutParams#MATCH_PARENT} to fill the screen, or
 * {@link ViewGroup.LayoutParams#WRAP_CONTENT} to fit the width of its anchor view.</p>
 *
 * @return the width for the drop down list
 *
 * @attr ref android.R.styleable#AutoCompleteTextView_dropDownWidth
 */
Int32 AutoCompleteTextView::GetDropDownWidth()
{
    return mDropDownWidth;
}

/**
 * <p>Sets the current width for the auto-complete drop down list. This can
 * be a fixed width, or {@link ViewGroup.LayoutParams#MATCH_PARENT} to fill the screen, or
 * {@link ViewGroup.LayoutParams#WRAP_CONTENT} to fit the width of its anchor view.</p>
 *
 * @param width the width to use
 *
 * @attr ref android.R.styleable#AutoCompleteTextView_dropDownWidth
 */
ECode AutoCompleteTextView::SetDropDownWidth(
    /* [in] */ Int32 width)
{
    mDropDownWidth = width;

    return NOERROR;
}

/**
 * <p>Returns the current height for the auto-complete drop down list. This can
 * be a fixed height, or {@link ViewGroup.LayoutParams#MATCH_PARENT} to fill
 * the screen, or {@link ViewGroup.LayoutParams#WRAP_CONTENT} to fit the height
 * of the drop down's content.</p>
 *
 * @return the height for the drop down list
 *
 * @attr ref android.R.styleable#AutoCompleteTextView_dropDownHeight
 */
Int32 AutoCompleteTextView::GetDropDownHeight()
{
    return mDropDownHeight;
}

/**
 * <p>Sets the current height for the auto-complete drop down list. This can
 * be a fixed height, or {@link ViewGroup.LayoutParams#MATCH_PARENT} to fill
 * the screen, or {@link ViewGroup.LayoutParams#WRAP_CONTENT} to fit the height
 * of the drop down's content.</p>
 *
 * @param height the height to use
 *
 * @attr ref android.R.styleable#AutoCompleteTextView_dropDownHeight
 */
ECode AutoCompleteTextView::SetDropDownHeight(
    /* [in] */ Int32 height)
{
    mDropDownHeight = height;

    return NOERROR;
}

/**
 * <p>Returns the id for the view that the auto-complete drop down list is anchored to.</p>
 *
 * @return the view's id, or {@link View#NO_ID} if none specified
 *
 * @attr ref android.R.styleable#AutoCompleteTextView_dropDownAnchor
 */
Int32 AutoCompleteTextView::GetDropDownAnchor()
{
    return mDropDownAnchorId;
}

/**
 * <p>Sets the view to which the auto-complete drop down list should anchor. The view
 * corresponding to this id will not be loaded until the next time it is needed to avoid
 * loading a view which is not yet instantiated.</p>
 *
 * @param id the id to anchor the drop down list view to
 *
 * @attr ref android.R.styleable#AutoCompleteTextView_dropDownAnchor
 */
ECode AutoCompleteTextView::SetDropDownAnchor(
    /* [in] */ Int32 id)
{
    mDropDownAnchorId = id;
    mDropDownAnchorView = NULL;

    return NOERROR;
}

/**
 * <p>Gets the background of the auto-complete drop-down list.</p>
 *
 * @return the background drawable
 *
 * @attr ref android.R.styleable#PopupWindow_popupBackground
 */
AutoPtr<IDrawable> AutoCompleteTextView::GetDropDownBackground()
{
    AutoPtr<IDrawable> drawable;
    mPopup->GetBackground((IDrawable**)&drawable);

    return drawable;
}

/**
 * <p>Sets the background of the auto-complete drop-down list.</p>
 *
 * @param d the drawable to set as the background
 *
 * @attr ref android.R.styleable#PopupWindow_popupBackground
 */
ECode AutoCompleteTextView::SetDropDownBackgroundDrawable(
    /* [in] */ IDrawable* d)
{
    return mPopup->SetBackgroundDrawable(d);
}

/**
 * <p>Sets the background of the auto-complete drop-down list.</p>
 *
 * @param id the id of the drawable to set as the background
 *
 * @attr ref android.R.styleable#PopupWindow_popupBackground
 */
ECode AutoCompleteTextView::SetDropDownBackgroundResource(
    /* [in] */ Int32 id)
{
    AutoPtr<IDrawable> drawable;
    GetResources()->GetDrawable(id, (IDrawable**)&drawable);

    return mPopup->SetBackgroundDrawable(drawable);
}

/**
 * <p>Sets the vertical offset used for the auto-complete drop-down list.</p>
 *
 * @param offset the vertical offset
 */
ECode AutoCompleteTextView::SetDropDownVerticalOffset(
    /* [in] */ Int32 offset)
{
    mDropDownVerticalOffset = offset;

    return NOERROR;
}

/**
 * <p>Gets the vertical offset used for the auto-complete drop-down list.</p>
 *
 * @return the vertical offset
 */
Int32 AutoCompleteTextView::GetDropDownVerticalOffset()
{
    return mDropDownVerticalOffset;
}

/**
 * <p>Sets the horizontal offset used for the auto-complete drop-down list.</p>
 *
 * @param offset the horizontal offset
 */
ECode AutoCompleteTextView::SetDropDownHorizontalOffset(
    /* [in] */ Int32 offset)
{
    mDropDownHorizontalOffset = offset;

    return NOERROR;
}

/**
 * <p>Gets the horizontal offset used for the auto-complete drop-down list.</p>
 *
 * @return the horizontal offset
 */
Int32 AutoCompleteTextView::GetDropDownHorizontalOffset()
{
    return mDropDownHorizontalOffset;
}

/**
 * <p>Sets the animation style of the auto-complete drop-down list.</p>
 *
 * <p>If the drop-down is showing, calling this method will take effect only
 * the next time the drop-down is shown.</p>
 *
 * @param animationStyle animation style to use when the drop-down appears
 *      and disappears.  Set to -1 for the default animation, 0 for no
 *      animation, or a resource identifier for an explicit animation.
 *
 * @hide Pending API council approval
 */
ECode AutoCompleteTextView::SetDropDownAnimationStyle(
    /* [in] */ Int32 animationStyle)
{
    mPopup->SetAnimationStyle(animationStyle);

    return NOERROR;
}

/**
 * <p>Returns the animation style that is used when the drop-down list appears and disappears
 * </p>
 *
 * @return the animation style that is used when the drop-down list appears and disappears
 *
 * @hide Pending API council approval
 */
Int32 AutoCompleteTextView::GetDropDownAnimationStyle()
{
    Int32 animationStyle;
    mPopup->GetAnimationStyle(&animationStyle);

    return animationStyle;
}

/**
 * @return Whether the drop-down is visible as long as there is {@link #enoughToFilter()}
 *
 * @hide Pending API council approval
 */
Boolean AutoCompleteTextView::IsDropDownAlwaysVisible()
{
    return mDropDownAlwaysVisible;
}

/**
 * Sets whether the drop-down should remain visible as long as there is there is
 * {@link #enoughToFilter()}.  This is useful if an unknown number of results are expected
 * to show up in the adapter sometime in the future.
 *
 * The drop-down will occupy the entire screen below {@link #getDropDownAnchor} regardless
 * of the size or content of the list.  {@link #getDropDownBackground()} will fill any space
 * that is not used by the list.
 *
 * @param dropDownAlwaysVisible Whether to keep the drop-down visible.
 *
 * @hide Pending API council approval
 */
ECode AutoCompleteTextView::SetDropDownAlwaysVisible(
    /* [in] */ Boolean dropDownAlwaysVisible)
{
    mDropDownAlwaysVisible = dropDownAlwaysVisible;

    return NOERROR;
}

/**
 * Checks whether the drop-down is dismissed when a suggestion is clicked.
 *
 * @hide Pending API council approval
 */
Boolean AutoCompleteTextView::IsDropDownDismissedOnCompletion()
{
    return mDropDownDismissedOnCompletion;
}

/**
 * Sets whether the drop-down is dismissed when a suggestion is clicked. This is
 * TRUE by default.
 *
 * @param dropDownDismissedOnCompletion Whether to dismiss the drop-down.
 *
 * @hide Pending API council approval
 */
ECode AutoCompleteTextView::SetDropDownDismissedOnCompletion(
    /* [in] */ Boolean dropDownDismissedOnCompletion)
{
    mDropDownDismissedOnCompletion = dropDownDismissedOnCompletion;

    return NOERROR;
}

/**
 * <p>Returns the number of characters the user must type before the drop
 * down list is shown.</p>
 *
 * @return the minimum number of characters to type to show the drop down
 *
 * @see #setThreshold(Int32)
 */
Int32 AutoCompleteTextView::GetThreshold()
{
    return mThreshold;
}

/**
 * <p>Specifies the minimum number of characters the user has to type in the
 * edit box before the drop down list is shown.</p>
 *
 * <p>When <code>threshold</code> is less than or equals 0, a threshold of
 * 1 is applied.</p>
 *
 * @param threshold the number of characters to type before the drop down
 *                  is shown
 *
 * @see #getThreshold()
 *
 * @attr ref android.R.styleable#AutoCompleteTextView_completionThreshold
 */
ECode AutoCompleteTextView::SetThreshold(
    /* [in] */ Int32 threshold)
{
    if (threshold <= 0) {
        threshold = 1;
    }

    mThreshold = threshold;

    return NOERROR;
}

/**
 * <p>Sets the listener that will be notified when the user clicks an item
 * in the drop down list.</p>
 *
 * @param l the item click listener
 */
ECode AutoCompleteTextView::SetOnItemClickListener(
    /* [in] */ IOnItemClickListener* l)
{
    mItemClickListener = l;

    return NOERROR;
}

/**
 * <p>Sets the listener that will be notified when the user selects an item
 * in the drop down list.</p>
 *
 * @param l the item selected listener
 */
ECode AutoCompleteTextView::SetOnItemSelectedListener(
    /* [in] */ IOnItemSelectedListener* l)
{
    mItemSelectedListener = l;

    return NOERROR;
}

/**
 * <p>Returns the listener that is notified whenever the user clicks an item
 * in the drop down list.</p>
 *
 * @return the item click listener
 */
AutoPtr<IOnItemClickListener> AutoCompleteTextView::GetOnItemClickListener()
{
    return mItemClickListener;
}

/**
 * <p>Returns the listener that is notified whenever the user selects an
 * item in the drop down list.</p>
 *
 * @return the item selected listener
 */
AutoPtr<IOnItemSelectedListener> AutoCompleteTextView::GetOnItemSelectedListener()
{
    return mItemSelectedListener;
}

/**
 * <p>Returns a filterable list adapter used for auto completion.</p>
 *
 * @return a data adapter used for auto completion
 */
AutoPtr<IListAdapter> AutoCompleteTextView::GetAdapter()
{
    return mAdapter;
}

/**
 * <p>Changes the list of data used for auto completion. The provided list
 * must be a filterable list adapter.</p>
 *
 * <p>The caller is still responsible for managing any resources used by the adapter.
 * Notably, when the AutoCompleteTextView is closed or released, the adapter is not notified.
 * A common case is the use of {@link android.widget.CursorAdapter}, which
 * contains a {@link android.database.Cursor} that must be closed.  This can be done
 * automatically (see
 * {@link android.app.Activity#startManagingCursor(android.database.Cursor)
 * startManagingCursor()}),
 * or by manually closing the cursor when the AutoCompleteTextView is dismissed.</p>
 *
 * @param adapter the adapter holding the auto completion data
 *
 * @see #getAdapter()
 * @see android.widget.Filterable
 * @see android.widget.ListAdapter
 */
ECode AutoCompleteTextView::SetAdapter(
    /* [in] */ IListAdapter* adapter)
{
    if (mObserver.Get() == NULL) {
        mObserver = new PopupDataSetObserver(this);
    }
    else if (mAdapter.Get() != NULL) {
        mAdapter->UnregisterDataSetObserver(mObserver);
    }
    mAdapter = adapter;
    if (mAdapter != NULL) {
        //noinspection unchecked
        IFilterable::Probe(mAdapter.Get())->GetFilter((IFilter**)&mFilter);
        adapter->RegisterDataSetObserver(mObserver);
    }
    else {
        mFilter = NULL;
    }

    if (mDropDownList.Get() != NULL) {
        mDropDownList->SetAdapter(mAdapter);
    }

    return NOERROR;
}

Boolean AutoCompleteTextView::OnKeyPreIme(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (keyCode == KeyEvent_KEYCODE_BACK && IsPopupShowing()
            && !mDropDownAlwaysVisible) {
        // special case for the back key, we do not even try to send it
        // to the drop down list but instead, consume it immediately

        Int32 action;
        event->GetAction(&action);

        Int32 count;
        event->GetRepeatCount(&count);

        AutoPtr<IDispatcherState> dispatcherState;
        GetKeyDispatcherState((IDispatcherState**)&dispatcherState);

        if (action == KeyEvent_ACTION_DOWN && count == 0) {
            dispatcherState->StartTracking(event, this->Probe(EIID_IInterface));
            return TRUE;
        }
        else if (action == KeyEvent_ACTION_UP) {
            dispatcherState->HandleUpEvent(event);

            Boolean tracking, canceled;
            event->IsTracking(&tracking);
            event->IsCanceled(&canceled);
            if (tracking && !canceled) {
                DismissDropDown();
                return TRUE;
            }
        }
    }
    return EditText::OnKeyPreIme(keyCode, event);
}

Boolean AutoCompleteTextView::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Int32 position;
    if (IsPopupShowing() &&
        (mDropDownList->GetSelectedItemPosition(&position), position) >= 0) {
        Boolean consumed;
        mDropDownList->OnKeyUp(keyCode, event, &consumed);
        if (consumed) {
            switch (keyCode) {
                // if the list accepts the key events and the key event
                // was a click, the text view gets the selected item
                // from the drop down as its content
                case KeyEvent_KEYCODE_ENTER:
                case KeyEvent_KEYCODE_DPAD_CENTER:
                    PerformCompletion();
                    return TRUE;
            }
        }
    }

    return EditText::OnKeyUp(keyCode, event);
}

Boolean AutoCompleteTextView::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    // when the drop down is shown, we drive it directly
    if (IsPopupShowing()) {
        // the key events are forwarded to the list in the drop down view
        // note that ListView handles space but we don't want that to happen
        // also if selection is not currently in the drop down, then don't
        // let center or enter presses go there since that would cause it
        // to select one of its items

        Int32 position;
        mDropDownList->GetSelectedItemPosition(&position);
        if (keyCode != KeyEvent_KEYCODE_SPACE
                && (position >= 0
                        || (keyCode != KeyEvent_KEYCODE_ENTER
                                && keyCode != KeyEvent_KEYCODE_DPAD_CENTER))) {
            Int32 curIndex;
            mDropDownList->GetSelectedItemPosition(&curIndex);

            Boolean consumed;

            Boolean below;
            mPopup->IsAboveAnchor(&below);
            below = !below;

            AutoPtr<IListAdapter> adapter = mAdapter;

            Boolean allEnabled;
            Int32 firstItem = Math::INT32_MAX_VALUE;
            Int32 lastItem = Math::INT32_MIN_VALUE;

            if (adapter.Get() != NULL) {
                adapter->AreAllItemsEnabled(&allEnabled);

                if (allEnabled) {
                    firstItem = 0;
                }
                else {
                    firstItem = mDropDownList->LookForSelectablePosition(0, TRUE);
                }

                if (allEnabled) {
                    adapter->GetCount(&lastItem);
                    lastItem--;
                }
                else {
                    Int32 count;
                    adapter->GetCount(&count);
                    lastItem = mDropDownList->LookForSelectablePosition(count - 1, FALSE);
                }
            }

            if ((below && keyCode == KeyEvent_KEYCODE_DPAD_UP && curIndex <= firstItem) ||
                    (!below && keyCode == KeyEvent_KEYCODE_DPAD_DOWN && curIndex >= lastItem)) {
                // When the selection is at the top, we block the key
                // event to prevent focus from moving.
                ClearListSelection();
                mPopup->SetInputMethodMode(PopupWindow_INPUT_METHOD_NEEDED);
                ShowDropDown();
                return TRUE;
            }
            else {
                // WARNING: Please read the comment where mListSelectionHidden
                //          is declared
                mDropDownList->mListSelectionHidden = FALSE;
            }

            mDropDownList->OnKeyDown(keyCode, event, &consumed);
            //if (DEBUG) Log.v(TAG, "Key down: code=" + keyCode + " list consumed=" + consumed);

            if (consumed) {
                // If it handled the key event, then the user is
                // navigating in the list, so we should put it in front.
                mPopup->SetInputMethodMode(PopupWindow_INPUT_METHOD_NOT_NEEDED);
                // Here's a little trick we need to do to make sure that
                // the list view is actually showing its focus indicator,
                // by ensuring it has focus and getting its window out
                // of touch mode.
                //mDropDownList->RequestFocusFromTouch();
                ShowDropDown();

                switch (keyCode) {
                    // avoid passing the focus from the text view to the
                    // next component
                    case KeyEvent_KEYCODE_ENTER:
                    case KeyEvent_KEYCODE_DPAD_CENTER:
                    case KeyEvent_KEYCODE_DPAD_DOWN:
                    case KeyEvent_KEYCODE_DPAD_UP:
                        return TRUE;
                }
            }
            else {
                if (below && keyCode == KeyEvent_KEYCODE_DPAD_DOWN) {
                    // when the selection is at the bottom, we block the
                    // event to avoid going to the next focusable widget
                    if (curIndex == lastItem) {
                        return TRUE;
                    }
                }
                else if (!below && keyCode == KeyEvent_KEYCODE_DPAD_UP &&
                        curIndex == firstItem) {
                    return TRUE;
                }
            }
        }
    }
    else {
        switch(keyCode) {
        case KeyEvent_KEYCODE_DPAD_DOWN:
            PerformValidation();
        }
    }
    mLastKeyCode = keyCode;
    Boolean handled = EditText::OnKeyDown(keyCode, event);
    mLastKeyCode = KeyEvent_KEYCODE_UNKNOWN;

    if (handled && IsPopupShowing() && mDropDownList.Get() != NULL) {
        ClearListSelection();
    }

    return handled;
}

/**
 * Returns <code>TRUE</code> if the amount of text in the field meets
 * or exceeds the {@link #getThreshold} requirement.  You can override
 * this to impose a different standard for when filtering will be
 * triggered.
 */
Boolean AutoCompleteTextView::EnoughToFilter()
{
    /*if (DEBUG) Log.v(TAG, "Enough to filter: len=" + getText().length()
            + " threshold=" + mThreshold);*/

    Int32 len;
    GetText()->GetLength(&len);
    return len >= mThreshold;
}

/**
 * This is used to watch for edits to the text view.  Note that we call
 * to methods on the auto complete text view class so that we can access
 * private vars without going through thunks.
 */

AutoCompleteTextView::MyWatcher::MyWatcher(
    /* [in] */ AutoCompleteTextView* host)
    : mHost(host)
{}

PInterface AutoCompleteTextView::MyWatcher::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_INoCopySpan) {
        return (INoCopySpan*)this;
    }
    else if (riid == EIID_ITextWatcher) {
        return (ITextWatcher*)this;
    }

    return NULL;
}

UInt32 AutoCompleteTextView::MyWatcher::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AutoCompleteTextView::MyWatcher::Release()
{
    return ElRefBase::Release();
}

ECode AutoCompleteTextView::MyWatcher::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode AutoCompleteTextView::MyWatcher::AfterTextChanged(
    /* [in] */ IEditable* s)
{
    mHost->DoAfterTextChanged();

    return NOERROR;
}

ECode AutoCompleteTextView::MyWatcher::BeforeTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 after)
{
    mHost->DoBeforeTextChanged();

    return NOERROR;
}

ECode AutoCompleteTextView::MyWatcher::OnTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 count)
{
    return NOERROR;
}

void AutoCompleteTextView::DoBeforeTextChanged()
{
    if (mBlockCompletion) return;

    // when text is changed, inserted or deleted, we attempt to show
    // the drop down
    mOpenBefore = IsPopupShowing();
    //if (DEBUG) Log.v(TAG, "before text changed: open=" + mOpenBefore);
}

void AutoCompleteTextView::DoAfterTextChanged()
{
    if (mBlockCompletion) return;

    // if the list was open before the keystroke, but closed afterwards,
    // then something in the keystroke processing (an input filter perhaps)
    // called performCompletion() and we shouldn't do any more processing.
    /*if (DEBUG) Log.v(TAG, "after text changed: openBefore=" + mOpenBefore
            + " open=" + isPopupShowing());*/
    if (mOpenBefore && !IsPopupShowing()) {
        return;
    }

    // the drop down is shown only when a minimum number of characters
    // was typed in the text view
    if (EnoughToFilter()) {
        if (mFilter.Get() != NULL) {
            PerformFiltering(GetText(), mLastKeyCode);
        }
    }
    else {
        // drop down is automatically dismissed when enough characters
        // are deleted from the text view
        if (!mDropDownAlwaysVisible)
            DismissDropDown();
        if (mFilter.Get() != NULL) {
            mFilter->DoFilter(NULL);
        }
    }
}

/**
 * <p>Indicates whether the popup menu is showing.</p>
 *
 * @return TRUE if the popup menu is showing, FALSE otherwise
 */
Boolean AutoCompleteTextView::IsPopupShowing()
{
    Boolean showing;
    mPopup->IsShowing(&showing);

    return showing;
}

/**
 * <p>Converts the selected item from the drop down list into a sequence
 * of character that can be used in the edit box.</p>
 *
 * @param selectedItem the item selected by the user for completion
 *
 * @return a sequence of characters representing the selected suggestion
 */
AutoPtr<ICharSequence> AutoCompleteTextView::ConvertSelectionToString(
    /* [in] */ IInterface* selectedItem)
{
    AutoPtr<ICharSequence> cs;
    mFilter->ConvertResultToString(selectedItem, (ICharSequence**)&cs);
    return cs;
}

/**
 * <p>Clear the list selection.  This may only be temporary, as user input will often bring
 * it back.
 */
ECode AutoCompleteTextView::ClearListSelection()
{
    AutoPtr<DropDownListView> list = mDropDownList;
    if (list != NULL) {
        // WARNING: Please read the comment where mListSelectionHidden is declared
        list->mListSelectionHidden = TRUE;
        list->HideSelector();
        list->RequestLayout();
    }

    return NOERROR;
}

/**
 * Set the position of the dropdown view selection.
 *
 * @param position The position to move the selector to.
 */
ECode AutoCompleteTextView::SetListSelection(
    /* [in] */ Int32 position)
{
    Boolean showing;
    mPopup->IsShowing(&showing);
    if (showing && (mDropDownList.Get() != NULL)) {
        mDropDownList->mListSelectionHidden = FALSE;
        mDropDownList->SetSelection(position);
        // ListView.setSelection() will call requestLayout()
    }

    return NOERROR;
}

/**
 * Get the position of the dropdown view selection, if there is one.  Returns
 * {@link ListView#INVALID_POSITION ListView.INVALID_POSITION} if there is no dropdown or if
 * there is no selection.
 *
 * @return the position of the current selection, if there is one, or
 * {@link ListView#INVALID_POSITION ListView.INVALID_POSITION} if not.
 *
 * @see ListView#getSelectedItemPosition()
 */
Int32 AutoCompleteTextView::GetListSelection()
{
    Boolean showing;
    mPopup->IsShowing(&showing);
    if (showing && (mDropDownList.Get() != NULL)) {

        Int32 pos;
        mDropDownList->GetSelectedItemPosition(&pos);
        return pos;
    }
    return AdapterView_INVALID_POSITION;
}

/**
 * <p>Starts filtering the content of the drop down list. The filtering
 * pattern is the content of the edit box. Subclasses should override this
 * method to filter with a different pattern, for instance a substring of
 * <code>text</code>.</p>
 *
 * @param text the filtering pattern
 * @param keyCode the last character inserted in the edit box; beware that
 * this will be NULL when text is being added through a soft input method.
 */
void AutoCompleteTextView::PerformFiltering(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 keyCode)
{
    mFilter->DoFilterEx(text, (IFilterListener*)this->Probe(EIID_IFilterListener));
}

/**
 * <p>Performs the text completion by converting the selected item from
 * the drop down list into a string, replacing the text box's content with
 * this string and finally dismissing the drop down menu.</p>
 */
ECode AutoCompleteTextView::PerformCompletion()
{
    PerformCompletion(NULL, -1, -1);

    return NOERROR;
}

ECode AutoCompleteTextView::OnCommitCompletion(
    /* [in] */ ICompletionInfo* completion)
{
    //if (IsPopupShowing()) {
    //    mBlockCompletion = TRUE;
    //    ReplaceText(completion.getText());
    //    mBlockCompletion = FALSE;

    //    if (mItemClickListener != NULL) {
    //        AutoPtr<DropDownListView> list = mDropDownList;
    //        // Note that we don't have a View here, so we will need to
    //        // supply NULL.  Hopefully no existing apps crash...

    //        Int32 position, id;
    //        completion->GetPosition(&position);
    //        completion->GetId(&id);

    //        mItemClickListener->OnItemClick(list, NULL, position, id);
    //    }
    //}
    return NOERROR;
}

void AutoCompleteTextView::PerformCompletion(
    /* [in] */ IView* selectedView,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    if (IsPopupShowing()) {
        AutoPtr<IInterface> selectedItem;
        if (position < 0) {
            mDropDownList->GetSelectedItem((IInterface**)&selectedItem);
        } else {
            mAdapter->GetItem(position, (IInterface**)&selectedItem);
        }
        if (selectedItem == NULL) {
            //Log.w(TAG, "performCompletion: no selected item");
            return;
        }

        mBlockCompletion = TRUE;
        ReplaceText(ConvertSelectionToString(selectedItem));
        mBlockCompletion = FALSE;

        if (mItemClickListener.Get() != NULL) {
            AutoPtr<DropDownListView> list = mDropDownList;

            if (selectedView == NULL || position < 0) {
                list->GetSelectedView(&selectedView);
                list->GetSelectedItemPosition(&position);
                list->GetSelectedItemId(&id);
            }
            mItemClickListener->OnItemClick(list, selectedView, position, id);
        }
    }

    if (mDropDownDismissedOnCompletion && !mDropDownAlwaysVisible) {
        DismissDropDown();
    }
}

/**
 * Identifies whether the view is currently performing a text completion, so subclasses
 * can decide whether to respond to text changed events.
 */
Boolean AutoCompleteTextView::IsPerformingCompletion()
{
    return mBlockCompletion;
}

/**
 * Like {@link #setText(CharSequence)}, except that it can disable filtering.
 *
 * @param filter If <code>FALSE</code>, no filtering will be performed
 *        as a result of this call.
 *
 * @hide Pending API council approval.
 */
ECode AutoCompleteTextView::SetText(
   /* [in] */ ICharSequence* text,
   /* [in] */ Boolean filter)
{
    if (filter) {
        SetText(text);
    }
    else {
        mBlockCompletion = TRUE;
        SetText(text);
        mBlockCompletion = FALSE;
    }

    return NOERROR;
}

/**
 * <p>Performs the text completion by replacing the current text by the
 * selected item. Subclasses should override this method to avoid replacing
 * the whole content of the edit box.</p>
 *
 * @param text the selected suggestion in the drop down list
 */
void AutoCompleteTextView::ReplaceText(
    /* [in] */ ICharSequence* text)
{
    ClearComposingText();

    SetText(text);
    // make sure we keep the caret at the end of the text view
    AutoPtr<IEditable> spannable = IEditable::Probe(GetText());
    assert(spannable != NULL);
    Int32 len = 0;
    spannable->GetLength(&len);
    Selection::SetSelection(spannable, len);
}

/** {@inheritDoc} */
ECode AutoCompleteTextView::OnFilterComplete(
    /* [in] */ Int32 count)
{
    UpdateDropDownForFilter(count);

    return NOERROR;
}

void AutoCompleteTextView::UpdateDropDownForFilter(
    /* [in] */ Int32 count)
{
    // Not attached to window, don't update drop-down
    if (GetWindowVisibility() == View::GONE) return;

    /*
     * This checks enoughToFilter() again because filtering requests
     * are asynchronous, so the result may come back after enough text
     * has since been deleted to make it no longer appropriate
     * to filter.
     */

    if ((count > 0 || mDropDownAlwaysVisible) && EnoughToFilter()) {
        if (HasFocus() && HasWindowFocus()) {
            ShowDropDown();
        }
    }
    else if (!mDropDownAlwaysVisible) {
        DismissDropDown();
    }
}

ECode AutoCompleteTextView::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    EditText::OnWindowFocusChanged(hasWindowFocus);
    if (!hasWindowFocus && !mDropDownAlwaysVisible) {
        DismissDropDown();
    }

    return NOERROR;
}

void AutoCompleteTextView::OnDisplayHint(
    /* [in] */ Int32 hint)
{
    EditText::OnDisplayHint(hint);
    switch (hint) {
        case INVISIBLE:
            if (!mDropDownAlwaysVisible) {
                DismissDropDown();
            }
            break;
    }
}

void AutoCompleteTextView::OnFocusChanged(
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    EditText::OnFocusChanged(focused, direction, previouslyFocusedRect);
    // Perform validation if the view is losing focus.
    if (!focused) {
        PerformValidation();
    }
    if (!focused && !mDropDownAlwaysVisible) {
        DismissDropDown();
    }
}

void AutoCompleteTextView::OnAttachedToWindow()
{
    EditText::OnAttachedToWindow();
}

void AutoCompleteTextView::OnDetachedFromWindow()
{
    DismissDropDown();
    EditText::OnDetachedFromWindow();
}

/**
 * <p>Closes the drop down if present on screen.</p>
 */
ECode AutoCompleteTextView::DismissDropDown()
{
    // AutoPtr<ILocalInputMethodManager> imm = CLocalInputMethodManager::PeekInstance();
    // if (imm != NULL) {
    //     imm->DisplayCompletions((IView*)this->Probe(EIID_IView), NULL);
    // }
    mPopup->Dismiss();
    mPopup->SetContentView(NULL);
    mDropDownList = NULL;
    return NOERROR;
}

Boolean AutoCompleteTextView::SetFrame(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    Boolean result = EditText::SetFrame(l, t, r, b);

    Boolean isShowing;
    mPopup->IsShowing(&isShowing);
    if (isShowing) {
        ShowDropDown();
    }

    return result;
}

/**
 * <p>Used for lazy instantiation of the anchor view from the id we have. If the value of
 * the id is NO_ID or we can't find a view for the given id, we return this TextView as
 * the default anchoring point.</p>
 */
AutoPtr<IView> AutoCompleteTextView::GetDropDownAnchorView()
{
    if (mDropDownAnchorView == NULL && mDropDownAnchorId != View::NO_ID) {
        GetRootView()->FindViewById(mDropDownAnchorId, (IView**)&mDropDownAnchorView);
    }
    return mDropDownAnchorView == NULL ? AutoPtr<IView>((IView*)this->Probe(EIID_IView)) : mDropDownAnchorView;
}

/**
 * Issues a runnable to show the dropdown as soon as possible.
 *
 * @hide internal used only by SearchDialog
 */
ECode AutoCompleteTextView::ShowDropDownAfterLayout()
{
    Post(mShowDropDownRunnable);

    return NOERROR;
}

/**
 * Ensures that the drop down is not obscuring the IME.
 * @param visible whether the ime should be in front. If FALSE, the ime is pushed to
 * the background.
 * @hide internal used only here and SearchDialog
 */
ECode AutoCompleteTextView::EnsureImeVisible(
    /* [in] */ Boolean visible)
{
    mPopup->SetInputMethodMode(visible
            ? PopupWindow_INPUT_METHOD_NEEDED : PopupWindow_INPUT_METHOD_NOT_NEEDED);
    ShowDropDown();

    return NOERROR;
}

/**
 * @hide internal used only here and SearchDialog
 */
Boolean AutoCompleteTextView::IsInputMethodNotNeeded()
{
    Int32 mode;
    mPopup->GetInputMethodMode(&mode);
    return mode == PopupWindow_INPUT_METHOD_NOT_NEEDED;
}

/**
 * <p>Displays the drop down on screen.</p>
 */
ECode AutoCompleteTextView::ShowDropDown()
{
    Int32 height = BuildDropDown();

    Int32 widthSpec = 0;
    Int32 heightSpec = 0;

    Boolean noInputMethod = IsInputMethodNotNeeded();

    Boolean showing;
    mPopup->IsShowing(&showing);
    if (showing) {
        if (mDropDownWidth == ViewGroupLayoutParams_MATCH_PARENT) {
            // The call to PopupWindow's update method below can accept -1 for any
            // value you do not want to update.
            widthSpec = -1;
        }
        else if (mDropDownWidth == ViewGroupLayoutParams_WRAP_CONTENT) {
            GetDropDownAnchorView()->GetWidth(&widthSpec);
        }
        else {
            widthSpec = mDropDownWidth;
        }
        if (mDropDownHeight == ViewGroupLayoutParams_MATCH_PARENT) {
            // The call to PopupWindow's update method below can accept -1 for any
            // value you do not want to update.
            heightSpec = noInputMethod ? height : ViewGroupLayoutParams_MATCH_PARENT;
            if (noInputMethod) {
                mPopup->SetWindowLayoutMode(
                        mDropDownWidth == ViewGroupLayoutParams_MATCH_PARENT ?
                                ViewGroupLayoutParams_MATCH_PARENT : 0, 0);
            }
            else {
                mPopup->SetWindowLayoutMode(
                        mDropDownWidth == ViewGroupLayoutParams_MATCH_PARENT ?
                                ViewGroupLayoutParams_MATCH_PARENT : 0,
                        ViewGroupLayoutParams_MATCH_PARENT);
            }
        }
        else if (mDropDownHeight == ViewGroupLayoutParams_WRAP_CONTENT) {
            heightSpec = height;
        }
        else {
            heightSpec = mDropDownHeight;
        }

        mPopup->SetOutsideTouchable(!mForceIgnoreOutsideTouch && !mDropDownAlwaysVisible);
        mPopup->UpdateEx5(GetDropDownAnchorView(), mDropDownHorizontalOffset,
                mDropDownVerticalOffset, widthSpec, heightSpec);
    }
    else {
        if (mDropDownWidth == ViewGroupLayoutParams_MATCH_PARENT) {
            widthSpec = ViewGroupLayoutParams_MATCH_PARENT;
        }
        else {
            if (mDropDownWidth == ViewGroupLayoutParams_WRAP_CONTENT) {

                Int32 width;
                GetDropDownAnchorView()->GetWidth(&width);

                mPopup->SetWidth(width);
            }
            else {
                mPopup->SetWidth(mDropDownWidth);
            }
        }

        if (mDropDownHeight == ViewGroupLayoutParams_MATCH_PARENT) {
            heightSpec = ViewGroupLayoutParams_MATCH_PARENT;
        }
        else {
            if (mDropDownHeight == ViewGroupLayoutParams_WRAP_CONTENT) {
                mPopup->SetHeight(height);
            }
            else {
                mPopup->SetHeight(mDropDownHeight);
            }
        }

        mPopup->SetWindowLayoutMode(widthSpec, heightSpec);
        mPopup->SetInputMethodMode(PopupWindow_INPUT_METHOD_NEEDED);

        // use outside touchable to dismiss drop down when touching outside of it, so
        // only set this if the dropdown is not always visible
        mPopup->SetOutsideTouchable(!mForceIgnoreOutsideTouch && !mDropDownAlwaysVisible);
        mPopup->SetTouchInterceptor(new PopupTouchInterceptor(this));
        mPopup->ShowAsDropDownEx(GetDropDownAnchorView(),
                mDropDownHorizontalOffset, mDropDownVerticalOffset);
        mDropDownList->SetSelection(AdapterView_INVALID_POSITION);
        ClearListSelection();
        Post(mHideSelector);
    }

    return NOERROR;
}

/**
 * Forces outside touches to be ignored. Normally if {@link #isDropDownAlwaysVisible()} is
 * FALSE, we allow outside touch to dismiss the dropdown. If this is set to TRUE, then we
 * ignore outside touch even when the drop down is not set to always visible.
 *
 * @hide used only by SearchDialog
 */
ECode AutoCompleteTextView::SetForceIgnoreOutsideTouch(
    /* [in] */ Boolean forceIgnoreOutsideTouch)
{
    mForceIgnoreOutsideTouch = forceIgnoreOutsideTouch;

    return NOERROR;
}

/**
 * <p>Builds the popup window's content and returns the height the popup
 * should have. Returns -1 when the content already exists.</p>
 *
 * @return the content's height or -1 if content already exists
 */
Int32 AutoCompleteTextView::BuildDropDown()
{
    AutoPtr<IViewGroup> dropDownView;
    Int32 otherHeights = 0;

    AutoPtr<IListAdapter> adapter = mAdapter;
    if (adapter != NULL) {
        AutoPtr<ILocalInputMethodManager> imm = CLocalInputMethodManager::PeekInstance();
        if (imm != NULL) {
            Int32 count = 0;
            adapter->GetCount(&count);
            count = Math::Min(count, 20);
            ArrayOf< AutoPtr<ICompletionInfo> >* completions = ArrayOf< AutoPtr<ICompletionInfo> >::Alloc(count);
            Int32 realCount = 0;

            for (Int32 i = 0; i < count; i++) {
                Boolean enabled = FALSE;
                adapter->IsEnabled(i, &enabled);
                if (enabled) {
                    realCount++;
                    AutoPtr<IInterface> item;
                    adapter->GetItem(i, (IInterface**)&item);
                    Int64 id = 0;
                    adapter->GetItemId(i, &id);
                    AutoPtr<ICompletionInfo> tmpInfo;
                    CCompletionInfo::New(id, i,
                           ConvertSelectionToString(item), (ICompletionInfo**)&tmpInfo);
                    (*completions)[i] = tmpInfo.Get();
                }
            }

            if (realCount != count) {
                ArrayOf< AutoPtr<ICompletionInfo> >* tmp = ArrayOf< AutoPtr<ICompletionInfo> >::Alloc(realCount);
                //System.arraycopy(completions, 0, tmp, 0, realCount);
                memcpy(tmp->GetPayload(), completions->GetPayload(), realCount * sizeof(AutoPtr<ICompletionInfo>));
                ArrayOf< AutoPtr<ICompletionInfo> >::Free(completions);
                completions = tmp;
            }

            imm->DisplayCompletions((IView*)this->Probe(EIID_IView), (ArrayOf<ICompletionInfo*>*)completions);

            for (Int32 i = 0; i < completions->GetLength(); ++i) {
                (*completions)[i] = NULL;
            }
            ArrayOf< AutoPtr<ICompletionInfo> >::Free(completions);
        }
    }

    if (mDropDownList == NULL) {
        AutoPtr<IContext> context = GetContext();
        mHideSelector = new ListSelectorHider(this);

        /**
         * This Runnable exists for the sole purpose of checking if the view layout has got
         * completed and if so call showDropDown to display the drop down. This is used to show
         * the drop down as soon as possible after user opens up the search dialog, without
         * waiting for the normal UI pipeline to do it's job which is slower than this method.
         */
        mShowDropDownRunnable = new ShowDropDownRunnable(this);
        mDropDownList = new DropDownListView(context);
        mDropDownList->SetSelectorEx(mDropDownListHighlight);
        mDropDownList->SetAdapter(adapter);
        mDropDownList->SetVerticalFadingEdgeEnabled(TRUE);
        mDropDownList->SetOnItemClickListener(mDropDownItemClickListener);
        mDropDownList->SetFocusable(TRUE);
        mDropDownList->SetFocusableInTouchMode(TRUE);
        mDropDownList->SetOnItemSelectedListener(new DropDownItemSelectedListener(this));
        mDropDownList->SetOnScrollListener(new PopupScrollListener(this));

        if (mItemSelectedListener != NULL) {
            mDropDownList->SetOnItemSelectedListener(mItemSelectedListener);
        }

        dropDownView = (IViewGroup*)mDropDownList->Probe(EIID_IViewGroup);

        AutoPtr<IView> hintView = GetHintView(context);
        if (hintView != NULL) {
            // if an hint has been specified, we accomodate more space for it and
            // add a text view in the drop down menu, at the bottom of the list
            AutoPtr<ILinearLayout> hintContainer;
            CLinearLayout::New(context, (ILinearLayout**)&hintContainer);
            hintContainer->SetOrientation(LinearLayout::VERTICAL);

            AutoPtr<ILinearLayoutLayoutParams> hintParams;
            CLinearLayoutLayoutParams::New(
                ViewGroupLayoutParams_MATCH_PARENT, 0, 1.0f,
                (ILinearLayoutLayoutParams**)&hintParams);
            hintContainer->AddViewEx3(dropDownView, hintParams);
            hintContainer->AddView(hintView);

            // measure the hint's height to find how much more vertical space
            // we need to add to the drop down's height
            Int32 widthSpec = MeasureSpec::MakeMeasureSpec(GetWidth(), MeasureSpec::AT_MOST);
            Int32 heightSpec = MeasureSpec::UNSPECIFIED;
            hintView->Measure(widthSpec, heightSpec);

            hintView->GetLayoutParams((IViewGroupLayoutParams**)&hintParams);
            CLinearLayoutLayoutParams* lp = (CLinearLayoutLayoutParams*)hintParams.Get();
            hintView->GetMeasuredHeight(&otherHeights);
            otherHeights += lp->mTopMargin + lp->mBottomMargin;

            dropDownView = IViewGroup::Probe(hintContainer.Get());
        }

        mPopup->SetContentView(dropDownView);
    }
    else {
        AutoPtr<IView> view;
        mPopup->GetContentView((IView**)&view);
        dropDownView = IViewGroup::Probe(view);
        view = NULL;
        dropDownView->FindViewById(HINT_VIEW_ID, (IView**)&view);
        if (view != NULL) {
            AutoPtr<ILinearLayoutLayoutParams> hintParams;
            view->GetLayoutParams((IViewGroupLayoutParams**)&hintParams);
            CLinearLayoutLayoutParams* lp = (CLinearLayoutLayoutParams*)hintParams.Get();
            view->GetMeasuredHeight(&otherHeights);
            otherHeights += lp->mTopMargin + lp->mBottomMargin;
        }
    }
    // Max height available on the screen for a popup.
    Int32 inputMethodMode;
    mPopup->GetInputMethodMode(&inputMethodMode);
    Boolean ignoreBottomDecorations =
           inputMethodMode == PopupWindow_INPUT_METHOD_NOT_NEEDED;
    Int32 maxHeight;
    mPopup->GetMaxAvailableHeightEx2(
           GetDropDownAnchorView(), mDropDownVerticalOffset,
           ignoreBottomDecorations, &maxHeight);
    // getMaxAvailableHeight() subtracts the padding, so we put it back,
    // to get the available height for the whole window
    Int32 padding = 0;
    AutoPtr<IDrawable> background;
    mPopup->GetBackground((IDrawable**)&background);
    if (background != NULL) {
        Boolean res;
        background->GetPadding((IRect*)mTempRect.Get(), &res);
        padding = mTempRect->mTop + mTempRect->mBottom;
    }
   if (mDropDownAlwaysVisible || mDropDownHeight == ViewGroupLayoutParams_MATCH_PARENT) {
        return maxHeight + padding;
    }

    Int32 listContent = mDropDownList->MeasureHeightOfChildren(MeasureSpec::UNSPECIFIED,
           0, ListView::NO_POSITION, maxHeight - otherHeights, 2);
    // add padding only if the list has items in it, that way we don't show
    // the popup if it is not needed
    if (listContent > 0)
        otherHeights += padding;

    return listContent + otherHeights;
}

AutoPtr<IView> AutoCompleteTextView::GetHintView(
    /* [in] */ IContext* context)
{
    Int32 len;
    if (mHintText != NULL && (mHintText->GetLength(&len), len) > 0) {
        AutoPtr<ILayoutInflater> lf;
        LayoutInflater::From(context, (ILayoutInflater**)&lf);
        AutoPtr<IView> view;
        lf->Inflate(mHintResource, NULL, (IView**)&view);
        AutoPtr<IView> view2;
        view->FindViewById(R_Id_Text1, (IView**)&view2);
        AutoPtr<ITextView> hintView = ITextView::Probe(view2.Get());
        hintView->SetText(mHintText);
        hintView->SetId(HINT_VIEW_ID);
        return view2;
    }
    else {
        return NULL;
    }
}

/**
 * Sets the validator used to perform text validation.
 *
 * @param validator The validator used to validate the text entered in this widget.
 *
 * @see #getValidator()
 * @see #performValidation()
 */
ECode AutoCompleteTextView::SetValidator(
    /* [in] */ IValidator* validator)
{
    mValidator = validator;

    return NOERROR;
}

/**
 * Returns the Validator set with {@link #setValidator},
 * or <code>NULL</code> if it was not set.
 *
 * @see #setValidator(android.widget.AutoCompleteTextView.Validator)
 * @see #performValidation()
 */
AutoPtr<IValidator> AutoCompleteTextView::GetValidator()
{
    return mValidator;
}

/**
 * If a validator was set on this view and the current string is not valid,
 * ask the validator to fix it.
 *
 * @see #getValidator()
 * @see #setValidator(android.widget.AutoCompleteTextView.Validator)
 */
ECode AutoCompleteTextView::PerformValidation()
{
    if (mValidator == NULL) return NOERROR;

    AutoPtr<ICharSequence> text = GetText();

    Boolean res;
    mValidator->IsValid(text, &res);

    if (!TextUtils::IsEmpty(text) && !res) {
        AutoPtr<ICharSequence> str;
        mValidator->FixText(text, (ICharSequence**)&str);
        SetText(str);
    }

    return NOERROR;
}

/**
 * Returns the Filter obtained from {@link Filterable#getFilter},
 * or <code>NULL</code> if {@link #setAdapter} was not called with
 * a Filterable.
 */
AutoPtr<IFilter> AutoCompleteTextView::GetFilter()
{
    return mFilter;
}

ECode AutoCompleteTextView::ListSelectorHider::Run()
{
    mHost->ClearListSelection();

    return NOERROR;
}

ECode AutoCompleteTextView::ShowDropDownRunnable::Run()
{
    // View layout should be all done before displaying the drop down.
    AutoPtr<IView> view = mHost->GetDropDownAnchorView();
    AutoPtr<IBinder> token;
    view->GetWindowToken((IBinder**)&token);
    if (view != NULL && token != NULL) {
        mHost->ShowDropDown();
    }

    return NOERROR;
}

ECode AutoCompleteTextView::ResizePopupRunnable::Run()
{
    mHost->mPopup->SetInputMethodMode(PopupWindow_INPUT_METHOD_NOT_NEEDED);
    mHost->ShowDropDown();

    return NOERROR;
}

PInterface AutoCompleteTextView::PopupTouchInterceptor::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IViewOnTouchListener) {
        return (IViewOnTouchListener*)this;
    }

    return NULL;
}

UInt32 AutoCompleteTextView::PopupTouchInterceptor::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AutoCompleteTextView::PopupTouchInterceptor::Release()
{
    return ElRefBase::Release();
}

ECode AutoCompleteTextView::PopupTouchInterceptor::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode AutoCompleteTextView::PopupTouchInterceptor::OnTouch(
    /* [in] */ IView* v,
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* res)
{
    Int32 action;
    event->GetAction(&action);

    Boolean isShowing;
    mHost->mPopup->IsShowing(&isShowing);
    if (action == MotionEvent_ACTION_DOWN &&
            mHost->mPopup != NULL && isShowing) {
        mHost->PostDelayed(mHost->mResizePopupRunnable, EXPAND_LIST_TIMEOUT);
    }
    else if (action == MotionEvent_ACTION_UP) {
        mHost->RemoveCallbacks(mHost->mResizePopupRunnable);
    }
    *res = FALSE;
    return NOERROR;
}

PInterface AutoCompleteTextView::PopupScrollListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IOnScrollListener) {
        return (IOnScrollListener*)this;
    }

    return NULL;
}

UInt32 AutoCompleteTextView::PopupScrollListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AutoCompleteTextView::PopupScrollListener::Release()
{
    return ElRefBase::Release();
}

ECode AutoCompleteTextView::PopupScrollListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode AutoCompleteTextView::PopupScrollListener::OnScroll(
    /* [in] */ IAbsListView* view,
    /* [in] */ Int32 firstVisibleItem,
    /* [in] */ Int32 visibleItemCount,
    /* [in] */ Int32 totalItemCount)
{
    return NOERROR;
}

ECode AutoCompleteTextView::PopupScrollListener::OnScrollStateChanged(
    /* [in] */ IAbsListView* view,
    /* [in] */ Int32 scrollState)
{
    AutoPtr<IView> v;
    mHost->mPopup->GetContentView((IView**)&v);
    if (scrollState == OnScrollListener_SCROLL_STATE_TOUCH_SCROLL &&
        !mHost->IsInputMethodNotNeeded() && v.Get() != NULL) {
        mHost->RemoveCallbacks(mHost->mResizePopupRunnable);
        mHost->mResizePopupRunnable->Run();
    }

    return NOERROR;
}

PInterface AutoCompleteTextView::DropDownItemSelectedListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IOnItemSelectedListener) {
        return (IOnItemSelectedListener*)this;
    }

    return NULL;
}

UInt32 AutoCompleteTextView::DropDownItemSelectedListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AutoCompleteTextView::DropDownItemSelectedListener::Release()
{
    return ElRefBase::Release();
}

ECode AutoCompleteTextView::DropDownItemSelectedListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode AutoCompleteTextView::DropDownItemSelectedListener::OnItemSelected(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    if (position != -1) {
        if (mHost->mDropDownList != NULL) {
            mHost->mDropDownList->mListSelectionHidden = FALSE;
        }
    }

    return NOERROR;
}

ECode AutoCompleteTextView::DropDownItemSelectedListener::OnNothingSelected(
    /* [in] */ IAdapterView* parent)
{
    return NOERROR;
}

PInterface AutoCompleteTextView::DropDownItemClickListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IOnItemClickListener) {
        return (IOnItemClickListener*)this;
    }

    return NULL;
}

UInt32 AutoCompleteTextView::DropDownItemClickListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AutoCompleteTextView::DropDownItemClickListener::Release()
{
    return ElRefBase::Release();
}

ECode AutoCompleteTextView::DropDownItemClickListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode AutoCompleteTextView::DropDownItemClickListener::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    mHost->PerformCompletion(v, position, id);

    return NOERROR;
}

/**
 * <p>Avoids jarring scrolling effect by ensuring that list elements
 * made of a text view fit on a single line.</p>
 *
 * @param position the item index in the list to get a view for
 * @return the view for the specified item
 */
AutoPtr<IView> AutoCompleteTextView::_DropDownListView::ObtainView(
    /* [in] */ Int32 position,
    /* [in] */ Boolean* isScrap)
{
    AutoPtr<IView> view = ListView::ObtainView(position, isScrap);

    if (ITextView::Probe(view)) {
        ITextView::Probe(view)->SetHorizontallyScrolling(TRUE);
    }

    return view;
}

Boolean AutoCompleteTextView::_DropDownListView::IsInTouchMode()
{
    // WARNING: Please read the comment where mListSelectionHidden is declared
    return mListSelectionHidden || ListView::IsInTouchMode();
}

/**
 * <p>Returns the focus state in the drop down.</p>
 *
 * @return TRUE always
 */
Boolean AutoCompleteTextView::_DropDownListView::HasWindowFocus()
{
    return TRUE;
}

/**
 * <p>Returns the focus state in the drop down.</p>
 *
 * @return TRUE always
 */
Boolean AutoCompleteTextView::_DropDownListView::IsFocused()
{
    return TRUE;
}

/**
 * <p>Returns the focus state in the drop down.</p>
 *
 * @return TRUE always
 */
Boolean AutoCompleteTextView::_DropDownListView::HasFocus()
{
    return TRUE;
}

ArrayOf<Int32>* AutoCompleteTextView::_DropDownListView::OnCreateDrawableState(
    /* [in] */ Int32 extraSpace)
{
    ArrayOf<Int32>* res;
    ListView::OnCreateDrawableState(extraSpace, &res);
    // noinspection ConstantIfStatement
    // if (FALSE) {
    //     StringBuilder sb = new StringBuilder("Created drawable state: [");
    //     for (Int32 i=0; i<res.length; i++) {
    //         if (i > 0) sb.append(", ");
    //         sb.append("0x");
    //         sb.append(Integer.toHexString(res[i]));
    //     }
    //     sb.append("]");
    //     Log.i(TAG, sb.toString());
    // }
    return res;
}

IVIEW_METHODS_IMPL(AutoCompleteTextView::DropDownListView,
    AutoCompleteTextView::_DropDownListView, AutoCompleteTextView::_DropDownListView);

IVIEWGROUP_METHODS_IMPL(AutoCompleteTextView::DropDownListView,
    AutoCompleteTextView::_DropDownListView, AutoCompleteTextView::_DropDownListView);

IVIEWPARENT_METHODS_IMPL(AutoCompleteTextView::DropDownListView,
    AutoCompleteTextView::_DropDownListView, AutoCompleteTextView::_DropDownListView);

IVIEWMANAGER_METHODS_IMPL(AutoCompleteTextView::DropDownListView,
    AutoCompleteTextView::_DropDownListView, AutoCompleteTextView::_DropDownListView);

IDrawableCallback_METHODS_IMPL(AutoCompleteTextView::DropDownListView,
    AutoCompleteTextView::_DropDownListView, AutoCompleteTextView::_DropDownListView);

IKeyEventCallback_METHODS_IMPL(AutoCompleteTextView::DropDownListView,
    AutoCompleteTextView::_DropDownListView, AutoCompleteTextView::_DropDownListView);

IAccessibilityEventSource_METHODS_IMPL(AutoCompleteTextView::DropDownListView,
    AutoCompleteTextView::_DropDownListView, AutoCompleteTextView::_DropDownListView);

IADAPTERVIEW_METHODS_IMPL(AutoCompleteTextView::DropDownListView,
    AutoCompleteTextView::_DropDownListView, AutoCompleteTextView::_DropDownListView);

IABSLISTVIEW_METHODS_IMPL(AutoCompleteTextView::DropDownListView,
    AutoCompleteTextView::_DropDownListView, AutoCompleteTextView::_DropDownListView);

ILISTVIEW_METHODS_IMPL(AutoCompleteTextView::DropDownListView,
    AutoCompleteTextView::_DropDownListView, AutoCompleteTextView::_DropDownListView);

AutoCompleteTextView::DropDownListView::DropDownListView(
    /* [in] */ IContext* context)
{
    Init(context, NULL, R_Attr_DropDownListViewStyle);
}

PInterface AutoCompleteTextView::DropDownListView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IView*)(IListView*)this;
    }
    else if (EIID_IView == riid) {
        return (IView*)(IListView*)this;
    }
    else if (EIID_IViewGroup == riid) {
        return (IViewGroup*)(IListView*)this;
    }
    else if (EIID_IAdapterView == riid) {
        return (IAdapterView*)(IListView*)this;
    }
    else if (EIID_IAbsListView == riid) {
        return (IAbsListView*)(IListView*)this;
    }
    else if (EIID_IListView == riid) {
        return (IListView*)this;
    }
    else if (EIID_ITextWatcher == riid) {
        return (ITextWatcher*)this;
    }
    else if (EIID_IOnGlobalLayoutListener == riid) {
        return (IOnGlobalLayoutListener*)this;
    }
    else if (EIID_IOnTouchModeChangeListener == riid) {
        return (IOnTouchModeChangeListener*)this;
    }
    else if (EIID_IFilterListener == riid) {
        return (IFilterListener*)this;
    }
    else if (EIID_IViewParent == riid) {
        return (IViewParent*)this;
    }
    else if (EIID_IViewManager == riid) {
        return (IViewManager*)this;
    }
    else if (EIID_IDrawableCallback == riid) {
        return (IDrawableCallback*)this;
    }
    else if (EIID_IKeyEventCallback == riid) {
        return (IKeyEventCallback*)this;
    }
    else if (EIID_IAccessibilityEventSource == riid) {
        return (IAccessibilityEventSource*)this;
    }
    else if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }

    return NULL;
}

UInt32 AutoCompleteTextView::DropDownListView::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AutoCompleteTextView::DropDownListView::Release()
{
    return ElRefBase::Release();
}

ECode AutoCompleteTextView::DropDownListView::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode AutoCompleteTextView::DropDownListView::OnTouchModeChanged(
    /* [in] */ Boolean isInTouchMode)
{
    return _DropDownListView::OnTouchModeChanged(isInTouchMode);
}

ECode AutoCompleteTextView::DropDownListView::OnGlobalLayout()
{
    return _DropDownListView::OnGlobalLayout();
}

ECode AutoCompleteTextView::DropDownListView::BeforeTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 after)
{
    return _DropDownListView::BeforeTextChanged(s, start, count, after);
}

ECode AutoCompleteTextView::DropDownListView::OnTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 count)
{
    return _DropDownListView::OnTextChanged(s, start, before, count);
}

ECode AutoCompleteTextView::DropDownListView::AfterTextChanged(
    /* [in] */ IEditable* s)
{
    return _DropDownListView::AfterTextChanged(s);
}

ECode AutoCompleteTextView::DropDownListView::OnFilterComplete(
    /* [in] */ Int32 count)
{
    return _DropDownListView::OnFilterComplete(count);
}

PInterface AutoCompleteTextView::PassThroughClickListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IViewOnClickListener) {
        return (IViewOnClickListener*)this;
    }

    return NULL;
}

UInt32 AutoCompleteTextView::PassThroughClickListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AutoCompleteTextView::PassThroughClickListener::Release()
{
    return ElRefBase::Release();
}

ECode AutoCompleteTextView::PassThroughClickListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

/** {@inheritDoc} */
ECode AutoCompleteTextView::PassThroughClickListener::OnClick(
    /* [in] */ IView* v)
{
    mHost->OnClickImpl();

    if (mWrapped != NULL)
        mWrapped->OnClick(v);

    return NOERROR;
}

ECode AutoCompleteTextView::UpdateDropDownForFilterRunnable::Run()
{
    if (mHost->mAdapter != NULL) {
        Int32 count;
        mHost->mAdapter->GetCount(&count);
        mHost->UpdateDropDownForFilter(count);
    }

    return NOERROR;
}

PInterface AutoCompleteTextView::PopupDataSetObserver::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IDataSetObserver) {
        return (IDataSetObserver*)this;
    }

    return NULL;
}

UInt32 AutoCompleteTextView::PopupDataSetObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AutoCompleteTextView::PopupDataSetObserver::Release()
{
    return ElRefBase::Release();
}

ECode AutoCompleteTextView::PopupDataSetObserver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode AutoCompleteTextView::PopupDataSetObserver::OnChanged()
{
    if (mHost->IsPopupShowing()) {
       // This will resize the popup to fit the new adapter's content
       mHost->ShowDropDown();
    }
    else if (mHost->mAdapter != NULL) {
       // If the popup is not showing already, showing it will cause
       // the list of data set observers attached to the adapter to
       // change. We can't do it from here, because we are in the middle
       // of iterating throught he list of observers.
       mHost->Post(new UpdateDropDownForFilterRunnable(mHost));
    }

    return NOERROR;
}

ECode AutoCompleteTextView::PopupDataSetObserver::OnInvalidated()
{
    if (!mHost->mDropDownAlwaysVisible) {
        // There's no data to display so make sure we're not showing
        // the drop down and its list
        mHost->DismissDropDown();
    }

    return NOERROR;
}

ECode AutoCompleteTextView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(EditText::Init(context, attrs, defStyle));

    CPopupWindow::New(
        context, attrs, R_Attr_AutoCompleteTextViewStyle,
        (IPopupWindow**)&mPopup);

    mPopup->SetSoftInputMode(WindowManagerLayoutParams_SOFT_INPUT_ADJUST_RESIZE);

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(attrs,
            ArrayOf<Int32>(R_Styleable_AutoCompleteTextView, 10),
            defStyle, 0, (ITypedArray**)&a);

    a->GetInt32(R_Styleable_AutoCompleteTextView_completionThreshold, 2, &mThreshold);

    a->GetText(R_Styleable_AutoCompleteTextView_completionHint,
        (ICharSequence**)&mHintText);

    a->GetDrawable(R_Styleable_AutoCompleteTextView_dropDownSelector,
        (IDrawable**)&mDropDownListHighlight);

    Float fvalue;
    a->GetDimension(R_Styleable_AutoCompleteTextView_dropDownVerticalOffset, 0.0f, &fvalue);
    mDropDownVerticalOffset = fvalue;

    a->GetDimension(R_Styleable_AutoCompleteTextView_dropDownHorizontalOffset, 0.0f, &fvalue);
    mDropDownHorizontalOffset = fvalue;

    // Get the anchor's id now, but the view won't be ready, so wait to actually get the
    // view and store it in mDropDownAnchorView lazily in getDropDownAnchorView later.
    // Defaults to NO_ID, in which case the getDropDownAnchorView method will simply return
    // this TextView, as a default anchoring point.
    a->GetResourceId(R_Styleable_AutoCompleteTextView_dropDownAnchor,
            View_NO_ID, &mDropDownAnchorId);

    // For dropdown width, the developer can specify a specific width, or MATCH_PARENT
    // (for full screen width) or WRAP_CONTENT (to match the width of the anchored view).
    a->GetLayoutDimensionEx(R_Styleable_AutoCompleteTextView_dropDownWidth,
            ViewGroupLayoutParams_WRAP_CONTENT, &mDropDownWidth);
    a->GetLayoutDimensionEx(R_Styleable_AutoCompleteTextView_dropDownHeight,
            ViewGroupLayoutParams_WRAP_CONTENT, &mDropDownHeight);

    a->GetResourceId(R_Styleable_AutoCompleteTextView_completionHintView,
            R_Layout_Simple_dropdown_hint, &mHintResource);

    // Always turn on the auto complete input type flag, since it
    // makes no sense to use this widget without it.
    Int32 inputType = GetInputType();
    if ((inputType&InputType_TYPE_MASK_CLASS)
            == InputType_TYPE_CLASS_TEXT) {
        inputType |= InputType_TYPE_TEXT_FLAG_AUTO_COMPLETE;
        SetRawInputType(inputType);
    }

    a->Recycle();

    SetFocusable(TRUE);

    AddTextChangedListener(new MyWatcher(this));

    mPassThroughClickListener = new PassThroughClickListener(this);
    EditText::SetOnClickListener(mPassThroughClickListener);

    return NOERROR;
}
