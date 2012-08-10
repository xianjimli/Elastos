
#include "app/CAlertDialogBuilder.h"
#include "app/CAlertDialog.h"

/**
 * Constructor using a context for this builder and the {@link AlertDialog} it creates.
 */
ECode CAlertDialogBuilder::constructor(
    /* [in] */ IContext* context)
{
    mP = new AlertController::AlertParams(context);

    return NOERROR;
}

/**
 * Set the title using the given resource id.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetTitle(
    /* [in] */ Int32 titleId)
{
    mP->mTitle = NULL;
    return mP->mContext->GetText(titleId, (ICharSequence**)&mP->mTitle);
}

/**
 * Set the title displayed in the {@link Dialog}.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetTitleEx(
    /* [in] */ ICharSequence* title)
{
    mP->mTitle = title;
    return NOERROR;
}

/**
 * Set the title using the custom view {@code customTitleView}. The
 * methods {@link #setTitle(Int32)} and {@link #setIcon(Int32)} should be
 * sufficient for most titles, but this is provided if the title needs
 * more customization. Using this will replace the title and icon Set
 * via the other methods.
 *
 * @param customTitleView The custom view to use as the title.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetCustomTitle(
    /* [in] */ IView* customTitleView)
{
    mP->mCustomTitleView = customTitleView;
    return NOERROR;
}

/**
 * Set the message to display using the given resource id.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetMessage(
    /* [in] */ Int32 messageId)
{
    mP->mMessage = NULL;
    return mP->mContext->GetText(messageId, (ICharSequence**)&mP->mMessage);
}

/**
 * Set the message to display.
  *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetMessageEx(
    /* [in] */ ICharSequence* message)
{
    mP->mMessage = message;
    return NOERROR;
}

/**
 * Set the resource id of the {@link Drawable} to be used in the title.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetIcon(
    /* [in] */ Int32 iconId)
{
    mP->mIconId = iconId;
    return NOERROR;
}

/**
 * Set the {@link Drawable} to be used in the title.
  *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetIconEx(
    /* [in] */ IDrawable* icon)
{
    mP->mIcon = icon;
    return NOERROR;
}

/**
 * Set a listener to be invoked when the positive button of the dialog is pressed.
 * @param textId The resource id of the text to display in the positive button
 * @param listener The {@link DialogInterface.OnClickListener} to use.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetPositiveButton(
    /* [in] */ Int32 textId,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mP->mPositiveButtonText = NULL;
    mP->mContext->GetText(textId, (ICharSequence**)&mP->mPositiveButtonText);
    mP->mPositiveButtonListener = listener;
    return NOERROR;
}

/**
 * Set a listener to be invoked when the positive button of the dialog is pressed.
 * @param text The text to display in the positive button
 * @param listener The {@link DialogInterface.OnClickListener} to use.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetPositiveButtonEx(
    /* [in] */ ICharSequence* text,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mP->mPositiveButtonText = text;
    mP->mPositiveButtonListener = listener;
    return NOERROR;
}

/**
 * Set a listener to be invoked when the negative button of the dialog is pressed.
 * @param textId The resource id of the text to display in the negative button
 * @param listener The {@link DialogInterface.OnClickListener} to use.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetNegativeButton(
    /* [in] */ Int32 textId,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mP->mNegativeButtonText = NULL;
    mP->mContext->GetText(textId, (ICharSequence**)&mP->mNegativeButtonText);
    mP->mNegativeButtonListener = listener;
    return NOERROR;
}

/**
 * Set a listener to be invoked when the negative button of the dialog is pressed.
 * @param text The text to display in the negative button
 * @param listener The {@link DialogInterface.OnClickListener} to use.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetNegativeButtonEx(
    /* [in] */ ICharSequence* text,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mP->mNegativeButtonText = text;
    mP->mNegativeButtonListener = listener;
    return NOERROR;
}

/**
 * Set a listener to be invoked when the neutral button of the dialog is pressed.
 * @param textId The resource id of the text to display in the neutral button
 * @param listener The {@link DialogInterface.OnClickListener} to use.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetNeutralButton(
    /* [in] */ Int32 textId,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mP->mNeutralButtonText = NULL;
    mP->mContext->GetText(textId, (ICharSequence**)&mP->mNeutralButtonText);
    mP->mNeutralButtonListener = listener;
    return NOERROR;
}

/**
 * Set a listener to be invoked when the neutral button of the dialog is pressed.
 * @param text The text to display in the neutral button
 * @param listener The {@link DialogInterface.OnClickListener} to use.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetNeutralButtonEx(
    /* [in] */ ICharSequence* text,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mP->mNeutralButtonText = text;
    mP->mNeutralButtonListener = listener;
    return NOERROR;
}

/**
 * Sets whether the dialog is cancelable or not.  Default is TRUE.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetCancelable(
    /* [in] */ Boolean cancelable)
{
    mP->mCancelable = cancelable;
    return NOERROR;
}

/**
 * Sets the callback that will be called if the dialog is canceled.
 * @see #setCancelable(Boolean)
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetOnCancelListener(
    /* [in] */ IDialogInterfaceOnCancelListener* onCancelListener)
{
    mP->mOnCancelListener = onCancelListener;
    return NOERROR;
}

/**
 * Sets the callback that will be called if a key is dispatched to the dialog.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetOnKeyListener(
    /* [in] */ IDialogInterfaceOnKeyListener* onKeyListener)
{
    mP->mOnKeyListener = onKeyListener;
    return NOERROR;
}

/**
 * Set a list of items to be displayed in the dialog as the content, you will be notified of the
 * selected item via the supplied listener. This should be an array type i.e. R.array.foo
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetItems(
    /* [in] */ Int32 itemsId,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    AutoPtr<IResources> resources;
    mP->mContext->GetResources((IResources**)&resources);
    //todo: check whether mP->mItems has memory leak
    resources->GetTextArray(itemsId, &mP->mItems);
    mP->mOnClickListener = listener;
    return NOERROR;
}

/**
 * Set a list of items to be displayed in the dialog as the content, you will be notified of the
 * selected item via the supplied listener.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetItemsEx(
    /* [in] */ const ArrayOf<ICharSequence*>& items,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mP->mItems = items.Clone();
    for (Int32 i = 0; i < items.GetLength(); ++i) {
        if (items[i] != NULL) items[i]->AddRef();
    }
    mP->mOnClickListener = listener;
    return NOERROR;
}

/**
 * Set a list of items, which are supplied by the given {@link ListAdapter}, to be
 * displayed in the dialog as the content, you will be notified of the
 * selected item via the supplied listener.
 *
 * @param adapter The {@link ListAdapter} to supply the list of items
 * @param listener The listener that will be called when an item is clicked.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetAdapter(
    /* [in] */ IListAdapter* adapter,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mP->mAdapter = adapter;
    mP->mOnClickListener = listener;
    return NOERROR;
}

/**
 * Set a list of items, which are supplied by the given {@link Cursor}, to be
 * displayed in the dialog as the content, you will be notified of the
 * selected item via the supplied listener.
 *
 * @param cursor The {@link Cursor} to supply the list of items
 * @param listener The listener that will be called when an item is clicked.
 * @param labelColumn The column name on the cursor containing the string to display
 *          in the label.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetCursor(
    /* [in] */ ICursor* cursor,
    /* [in] */ IDialogInterfaceOnClickListener* listener,
    /* [in] */ const String& labelColumn)
{
    mP->mCursor = cursor;
    mP->mLabelColumn = labelColumn;
    mP->mOnClickListener = listener;
    return NOERROR;
}

/**
 * Set a list of items to be displayed in the dialog as the content,
 * you will be notified of the selected item via the supplied listener.
 * This should be an array type, e.g. R.array.foo. The list will have
 * a check mark displayed to the right of the text for each checked
 * item. Clicking on an item in the list will not dismiss the dialog.
 * Clicking on a button will dismiss the dialog.
 *
 * @param itemsId the resource id of an array i.e. R.array.foo
 * @param checkedItems specifies which items are checked. It should be null in which case no
 *        items are checked. If non null it must be exactly the same length as the array of
 *        items.
 * @param listener notified when an item on the list is clicked. The dialog will not be
 *        dismissed when an item is clicked. It will only be dismissed if clicked on a
 *        button, if no buttons are supplied it's up to the user to dismiss the dialog.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetMultiChoiceItems(
    /* [in] */ Int32 itemsId,
    /* [in] */ const ArrayOf<Boolean>& checkedItems,
    /* [in] */ IDialogInterfaceOnMultiChoiceClickListener* listener)
{
    AutoPtr<IResources> resources;
    mP->mContext->GetResources((IResources**)&resources);
    //todo: check whether mP->mItems has memory leak
    resources->GetTextArray(itemsId, &mP->mItems);
    mP->mOnCheckboxClickListener = listener;
    mP->mCheckedItems = checkedItems.Clone();
    mP->mIsMultiChoice = TRUE;
    return NOERROR;
}

/**
 * Set a list of items to be displayed in the dialog as the content,
 * you will be notified of the selected item via the supplied listener.
 * The list will have a check mark displayed to the right of the text
 * for each checked item. Clicking on an item in the list will not
 * dismiss the dialog. Clicking on a button will dismiss the dialog.
 *
 * @param items the text of the items to be displayed in the list.
 * @param checkedItems specifies which items are checked. It should be null in which case no
 *        items are checked. If non null it must be exactly the same length as the array of
 *        items.
 * @param listener notified when an item on the list is clicked. The dialog will not be
 *        dismissed when an item is clicked. It will only be dismissed if clicked on a
 *        button, if no buttons are supplied it's up to the user to dismiss the dialog.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetMultiChoiceItemsEx(
    /* [in] */ const ArrayOf<ICharSequence*>& items,
    /* [in] */ const ArrayOf<Boolean>& checkedItems,
    /* [in] */ IDialogInterfaceOnMultiChoiceClickListener* listener)
{
    mP->mItems = items.Clone();
    for (Int32 i = 0; i < items.GetLength(); ++i) {
        if (items[i] != NULL) items[i]->AddRef();
    }
    mP->mOnCheckboxClickListener = listener;
    mP->mCheckedItems = checkedItems.Clone();
    mP->mIsMultiChoice = TRUE;
    return NOERROR;
}

/**
 * Set a list of items to be displayed in the dialog as the content,
 * you will be notified of the selected item via the supplied listener.
 * The list will have a check mark displayed to the right of the text
 * for each checked item. Clicking on an item in the list will not
 * dismiss the dialog. Clicking on a button will dismiss the dialog.
 *
 * @param cursor the cursor used to provide the items.
 * @param isCheckedColumn specifies the column name on the cursor to use to determine
 *        whether a checkbox is checked or not. It must return an Int32eger value where 1
 *        means checked and 0 means unchecked.
 * @param labelColumn The column name on the cursor containing the string to display in the
 *        label.
 * @param listener notified when an item on the list is clicked. The dialog will not be
 *        dismissed when an item is clicked. It will only be dismissed if clicked on a
 *        button, if no buttons are supplied it's up to the user to dismiss the dialog.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetMultiChoiceItemsEx2(
    /* [in] */ ICursor* cursor,
    /* [in] */ const String& isCheckedColumn,
    /* [in] */ const String& labelColumn,
    /* [in] */ IDialogInterfaceOnMultiChoiceClickListener* listener)
{
    mP->mCursor = cursor;
    mP->mOnCheckboxClickListener = listener;
    mP->mIsCheckedColumn = isCheckedColumn;
    mP->mLabelColumn = labelColumn;
    mP->mIsMultiChoice = TRUE;
    return NOERROR;
}

/**
 * Set a list of items to be displayed in the dialog as the content, you will be notified of
 * the selected item via the supplied listener. This should be an array type i.e.
 * R.array.foo The list will have a check mark displayed to the right of the text for the
 * checked item. Clicking on an item in the list will not dismiss the dialog. Clicking on a
 * button will dismiss the dialog.
 *
 * @param itemsId the resource id of an array i.e. R.array.foo
 * @param checkedItem specifies which item is checked. If -1 no items are checked.
 * @param listener notified when an item on the list is clicked. The dialog will not be
 *        dismissed when an item is clicked. It will only be dismissed if clicked on a
 *        button, if no buttons are supplied it's up to the user to dismiss the dialog.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetSingleChoiceItems(
    /* [in] */ Int32 itemsId,
    /* [in] */ Int32 checkedItem,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    AutoPtr<IResources> resources;
    mP->mContext->GetResources((IResources**)&resources);
    //todo: check whether mP->mItems has memory leak
    resources->GetTextArray(itemsId, &mP->mItems);
    mP->mOnClickListener = listener;
    mP->mCheckedItem = checkedItem;
    mP->mIsSingleChoice = TRUE;
    return NOERROR;
}

/**
 * Set a list of items to be displayed in the dialog as the content, you will be notified of
 * the selected item via the supplied listener. The list will have a check mark displayed to
 * the right of the text for the checked item. Clicking on an item in the list will not
 * dismiss the dialog. Clicking on a button will dismiss the dialog.
 *
 * @param cursor the cursor to retrieve the items from.
 * @param checkedItem specifies which item is checked. If -1 no items are checked.
 * @param labelColumn The column name on the cursor containing the string to display in the
 *        label.
 * @param listener notified when an item on the list is clicked. The dialog will not be
 *        dismissed when an item is clicked. It will only be dismissed if clicked on a
 *        button, if no buttons are supplied it's up to the user to dismiss the dialog.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetSingleChoiceItemsEx(
    /* [in] */ ICursor* cursor,
    /* [in] */ Int32 checkedItem,
    /* [in] */ const String& labelColumn,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mP->mCursor = cursor;
    mP->mOnClickListener = listener;
    mP->mCheckedItem = checkedItem;
    mP->mLabelColumn = labelColumn;
    mP->mIsSingleChoice = TRUE;
    return NOERROR;
}

/**
 * Set a list of items to be displayed in the dialog as the content, you will be notified of
 * the selected item via the supplied listener. The list will have a check mark displayed to
 * the right of the text for the checked item. Clicking on an item in the list will not
 * dismiss the dialog. Clicking on a button will dismiss the dialog.
 *
 * @param items the items to be displayed.
 * @param checkedItem specifies which item is checked. If -1 no items are checked.
 * @param listener notified when an item on the list is clicked. The dialog will not be
 *        dismissed when an item is clicked. It will only be dismissed if clicked on a
 *        button, if no buttons are supplied it's up to the user to dismiss the dialog.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetSingleChoiceItemsEx2(
    /* [in] */ const ArrayOf<ICharSequence*>& items,
    /* [in] */ Int32 checkedItem,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mP->mItems = items.Clone();
    for (Int32 i = 0; i < items.GetLength(); ++i) {
        if (items[i] != NULL) items[i]->AddRef();
    }
    mP->mOnClickListener = listener;
    mP->mCheckedItem = checkedItem;
    mP->mIsSingleChoice = TRUE;
    return NOERROR;
}

/**
 * Set a list of items to be displayed in the dialog as the content, you will be notified of
 * the selected item via the supplied listener. The list will have a check mark displayed to
 * the right of the text for the checked item. Clicking on an item in the list will not
 * dismiss the dialog. Clicking on a button will dismiss the dialog.
 *
 * @param adapter The {@link ListAdapter} to supply the list of items
 * @param checkedItem specifies which item is checked. If -1 no items are checked.
 * @param listener notified when an item on the list is clicked. The dialog will not be
 *        dismissed when an item is clicked. It will only be dismissed if clicked on a
 *        button, if no buttons are supplied it's up to the user to dismiss the dialog.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetSingleChoiceItemsEx3(
    /* [in] */ IListAdapter* adapter,
    /* [in] */ Int32 checkedItem,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mP->mAdapter = adapter;
    mP->mOnClickListener = listener;
    mP->mCheckedItem = checkedItem;
    mP->mIsSingleChoice = TRUE;
    return NOERROR;
}

/**
 * Sets a listener to be invoked when an item in the list is selected.
 *
 * @param listener The listener to be invoked.
 * @see AdapterView#setOnItemSelectedListener(android.widget.AdapterView.OnItemSelectedListener)
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetOnItemSelectedListener(
    /* [in] */ IOnItemSelectedListener* listener)
{
    mP->mOnItemSelectedListener = listener;
    return NOERROR;
}

/**
 * Set a custom view to be the contents of the Dialog. If the supplied view is an instance
 * of a {@link ListView} the light background will be used.
 *
 * @param view The view to use as the contents of the Dialog.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetView(
    /* [in] */ IView* view)
{
    mP->mView = view;
    mP->mViewSpacingSpecified = FALSE;
    return NOERROR;
}

/**
 * Set a custom view to be the contents of the Dialog, specifying the
 * spacing to appear around that view. If the supplied view is an
 * instance of a {@link ListView} the light background will be used.
 *
 * @param view The view to use as the contents of the Dialog.
 * @param viewSpacingLeft Spacing between the left edge of the view and
 *        the dialog frame
 * @param viewSpacingTop Spacing between the top edge of the view and
 *        the dialog frame
 * @param viewSpacingRight Spacing between the right edge of the view
 *        and the dialog frame
 * @param viewSpacingBottom Spacing between the bottom edge of the view
 *        and the dialog frame
 * @return This Builder object to allow for chaining of calls to Set
 *         methods
 *
 *
 * This is currently hidden because it seems like people should just
 * be able to put padding around the view.
 * @hide
 */
ECode CAlertDialogBuilder::SetViewEx(
    /* [in] */ IView* view,
    /* [in] */ Int32 viewSpacingLeft,
    /* [in] */ Int32 viewSpacingTop,
    /* [in] */ Int32 viewSpacingRight,
    /* [in] */ Int32 viewSpacingBottom)
{
    mP->mView = view;
    mP->mViewSpacingSpecified = TRUE;
    mP->mViewSpacingLeft = viewSpacingLeft;
    mP->mViewSpacingTop = viewSpacingTop;
    mP->mViewSpacingRight = viewSpacingRight;
    mP->mViewSpacingBottom = viewSpacingBottom;
    return NOERROR;
}

/**
 * Sets the Dialog to use the inverse background, regardless of what the
 * contents is.
 *
 * @param useInverseBackground Whether to use the inverse background
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetInverseBackgroundForced(
    /* [in] */ Boolean useInverseBackground)
{
    mP->mForceInverseBackground = useInverseBackground;
    return NOERROR;
}

/**
 * @hide
 */
ECode CAlertDialogBuilder::SetRecycleOnMeasureEnabled(
    /* [in] */ Boolean enabled)
{
    mP->mRecycleOnMeasure = enabled;
    return NOERROR;
}


/**
 * Creates a {@link AlertDialog} with the arguments supplied to this builder. It does not
 * {@link Dialog#show()} the dialog. This allows the user to do any extra processing
 * before displaying the dialog. Use {@link #show()} if you don't have any other processing
 * to do and want this to be created and displayed.
 */
ECode CAlertDialogBuilder::Create(
    /* [out] */ IAlertDialog** dialog)
{
    VALIDATE_NOT_NULL(dialog);

    AutoPtr<CAlertDialog> cdialog;
    CAlertDialog::NewByFriend(mP->mContext, (CAlertDialog**)&cdialog);
    mP->Apply(cdialog->mAlert);
    cdialog->SetCancelable(mP->mCancelable);
    cdialog->SetOnCancelListener(mP->mOnCancelListener);
    if (mP->mOnKeyListener != NULL) {
        cdialog->SetOnKeyListener(mP->mOnKeyListener);
    }

    *dialog = cdialog.Get();
    if (*dialog) {
        (*dialog)->AddRef();
    }

    return NOERROR;
}

/**
 * Creates a {@link AlertDialog} with the arguments supplied to this builder and
 * {@link Dialog#show()}'s the dialog.
 */
ECode CAlertDialogBuilder::Show(
    /* [out] */ IAlertDialog** dialog)
{
    FAIL_RETURN(Create(dialog));
    (*dialog)->Show();

    return NOERROR;
}
