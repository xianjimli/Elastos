
#include "widget/NumberPicker.h"

/**
 * Create a new number picker
 * @param context the application environment
 */
NumberPicker::NumberPicker(
    /* [in] */ IContext* context) : LinearLayout(context, NULL)
{
    Init(context, NULL);
}

/**
 * Create a new number picker
 * @param context the application environment
 * @param attrs a collection of attributes
 */
NumberPicker::NumberPicker(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs) : LinearLayout(context, attrs)
{   
    Init(context, attrs);
}

void NumberPicker::Init(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs)
{
    //SetOrientation(VERTICAL);
    //LayoutInflater inflater =
    //        (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    //inflater.inflate(R.layout.number_picker, this, TRUE);
    //mHandler = new Handler();

    //OnClickListener clickListener = new OnClickListener() {
    //    public void onClick(View v) {
    //        validateInput(mText);
    //        if (!mText.hasFocus()) mText.requestFocus();

    //        // now perform the increment/decrement
    //        if (R.id.increment == v.getId()) {
    //            changeCurrent(mCurrent + 1);
    //        } else if (R.id.decrement == v.getId()) {
    //            changeCurrent(mCurrent - 1);
    //        }
    //    }
    //};

    //OnFocusChangeListener focusListener = new OnFocusChangeListener() {
    //    public void onFocusChange(View v, Boolean hasFocus) {

    //        /* When focus is lost check that the text field
    //         * has valid values.
    //         */
    //        if (!hasFocus) {
    //            validateInput(v);
    //        }
    //    }
    //};

    //OnLongClickListener longClickListener = new OnLongClickListener() {
    //    /**
    //     * We start the Int64 click here but rely on the {@link NumberPickerButton}
    //     * to inform us when the Int64 click has ended.
    //     */
    //    public Boolean onLongClick(View v) {
    //        /* The text view may still have focus so clear it's focus which will
    //         * trigger the on focus changed and any typed values to be pulled.
    //         */
    //        mText.clearFocus();

    //        if (R.id.increment == v.getId()) {
    //            mIncrement = TRUE;
    //            mHandler.post(mRunnable);
    //        } else if (R.id.decrement == v.getId()) {
    //            mDecrement = TRUE;
    //            mHandler.post(mRunnable);
    //        }
    //        return TRUE;
    //    }
    //};

    //InputFilter inputFilter = new NumberPickerInputFilter();
    //mNumberInputFilter = new NumberRangeKeyListener();
    //mIncrementButton = (NumberPickerButton) findViewById(R.id.increment);
    //mIncrementButton.setOnClickListener(clickListener);
    //mIncrementButton.setOnLongClickListener(longClickListener);
    //mIncrementButton.setNumberPicker(this);

    //mDecrementButton = (NumberPickerButton) findViewById(R.id.decrement);
    //mDecrementButton.setOnClickListener(clickListener);
    //mDecrementButton.setOnLongClickListener(longClickListener);
    //mDecrementButton.setNumberPicker(this);

    //mText = (EditText) findViewById(R.id.timepicker_input);
    //mText.setOnFocusChangeListener(focusListener);
    //mText.setFilters(new InputFilter[] {inputFilter});
    //mText.setRawInputType(InputType.TYPE_CLASS_NUMBER);

    //if (!isEnabled()) {
    //    setEnabled(FALSE);
    //}
}

/**
 * Set the enabled state of this view. The interpretation of the enabled
 * state varies by subclass.
 *
 * @param enabled True if this view is enabled, FALSE otherwise.
 */
ECode NumberPicker::SetEnabled(
    /* [in] */ Boolean enabled) 
{
    LinearLayout::SetEnabled(enabled);
    mIncrementButton->SetEnabled(enabled);
    mDecrementButton->SetEnabled(enabled);
    mText->SetEnabled(enabled);

    return NOERROR;
}

/**
 * Set the callback that indicates the number has been adjusted by the user.
 * @param listener the callback, should not be NULL.
 */
ECode NumberPicker::SetOnChangeListener(
    /* [in] */ IOnChangedListener* listener) 
{
    mListener = listener;

    return NOERROR;
}

/**
 * Set the formatter that will be used to format the number for presentation
 * @param formatter the formatter object.  If formatter is NULL, String.valueOf()
 * will be used
 */
ECode NumberPicker::SetFormatter(
    /* [in] */ IFormatter* formatter) 
{
    mFormatter = formatter;

    return NOERROR;
}

/**
 * Set the range of numbers allowed for the number picker. The current
 * value will be automatically set to the start.
 *
 * @param start the start of the range (inclusive)
 * @param end the end of the range (inclusive)
 */
ECode NumberPicker::SetRange(
    /* [in] */ Int32 start, 
    /* [in] */ Int32 end) 
{
    return SetRange(start, end, NULL/*displayedValues*/);
}

/**
 * Set the range of numbers allowed for the number picker. The current
 * value will be automatically set to the start. Also provide a mapping
 * for values used to display to the user.
 *
 * @param start the start of the range (inclusive)
 * @param end the end of the range (inclusive)
 * @param displayedValues the values displayed to the user.
 */
ECode NumberPicker::SetRange(
    /* [in] */ Int32 start, 
    /* [in] */ Int32 end, 
    /* [in] */ ArrayOf<String>* displayedValues)
{
    mDisplayedValues = displayedValues;
    mStart = start;
    mEnd = end;
    mCurrent = start;
    UpdateView();

    if (displayedValues != NULL) {
        // Allow text entry rather than strictly numeric entry.
        mText->SetRawInputType(InputType_TYPE_CLASS_TEXT |
                InputType_TYPE_TEXT_FLAG_NO_SUGGESTIONS);
    }

    return NOERROR;
}

/**
 * Set the current value for the number picker.
 *
 * @param current the current value the start of the range (inclusive)
 * @throws IllegalArgumentException when current is not within the range
 *         of of the number picker
 */
ECode NumberPicker::SetCurrent(
    /* [in] */ Int32 current) 
{
    if (current < mStart || current > mEnd) {
        /*throw new IllegalArgumentException(
                "current should be >= start and <= end");*/
    }
    mCurrent = current;
    UpdateView();

    return NOERROR;
}

/**
 * Sets the speed at which the numbers will scroll when the +/-
 * buttons are longpressed
 *
 * @param speed The speed (in milliseconds) at which the numbers will scroll
 * default 300ms
 */
ECode NumberPicker::SetSpeed(
    /* [in] */ Int64 speed)
{
    mSpeed = speed;

    return NOERROR;
}

String NumberPicker::FormatNumber(
    /* [in] */ Int32 value)
{
    /*return (mFormatter != NULL)
            ? mFormatter.toString(value)
            : String.valueOf(value);*/

    return String("");
}

/**
 * Sets the current value of this NumberPicker, and sets mPrevious to the previous
 * value.  If current is greater than mEnd less than mStart, the value of mCurrent
 * is wrapped around.
 *
 * Subclasses can override this to change the wrapping behavior
 *
 * @param current the new value of the NumberPicker
 */
void NumberPicker::ChangeCurrent(
    /* [in] */ Int32 current) 
{
    // Wrap around the values if we go past the start or end
    if (current > mEnd) {
        current = mStart;
    } else if (current < mStart) {
        current = mEnd;
    }
    mPrevious = mCurrent;
    mCurrent = current;
    NotifyChange();
    UpdateView();
}

/**
 * Notifies the listener, if registered, of a change of the value of this
 * NumberPicker.
 */
void NumberPicker::NotifyChange()
{
    if (mListener != NULL) {
        //mListener->OnChanged(this, mPrevious, mCurrent);
    }
}

/**
 * Updates the view of this NumberPicker.  If displayValues were specified
 * in {@link #setRange}, the string corresponding to the index specified by
 * the current value will be returned.  Otherwise, the formatter specified
 * in {@link setFormatter} will be used to format the number.
 */
void NumberPicker::UpdateView() 
{
    /* If we don't have displayed values then use the
     * current number else find the correct value in the
     * displayed values for the current number.
     */
    /*if (mDisplayedValues == NULL) {
        mText->SetText(FormatNumber(mCurrent));
    } else {
        mText->SetText((*mDisplayedValues)[mCurrent - mStart]);
    }*/

    /*Int32 length;
    AutoPtr<ICharSequence> cs;mText->GetText();
    cs->GetLength(&length);
    mText->SetSelection(length);*/
}

void NumberPicker::ValidateCurrentView(
    /* [in] */ ICharSequence* str) 
{
   /* Int32 val = GetSelectedPos(str->ToString());
    if ((val >= mStart) && (val <= mEnd)) {
        if (mCurrent != val) {
            mPrevious = mCurrent;
            mCurrent = val;
            NotifyChange();
        }
    }
    UpdateView();*/
}

void NumberPicker::ValidateInput(
    /* [in] */ IView* v) 
{
    //String str = String.valueOf(((TextView) v).getText());
    //if ("".equals(str)) {

    //    // Restore to the old value as we don't allow empty values
    //    updateView();
    //} else {

    //    // Check the new value and ensure it's in range
    //    validateCurrentView(str);
    //}
}

/**
 * @hide
 */
ECode NumberPicker::CancelIncrement()
{
    mIncrement = FALSE;
}

/**
 * @hide
 */
ECode NumberPicker::CancelDecrement()
{
    mDecrement = FALSE;
}

AutoPtr<ICharSequence> NumberPicker::NumberPickerInputFilter::Filter(
    /* [in] */ ICharSequence* source, 
    /* [in] */ Int32 start, 
    /* [in] */ Int32 end,
    /* [in] */ ISpanned* dest, 
    /* [in] */ Int32 dstart, 
    /* [in] */ Int32 dend)
{
    /*if (mDisplayedValues == NULL) {
        return mNumberInputFilter->Filter(source, start, end, dest, dstart, dend);
    }
    CharSequence filtered = String.valueOf(source.subSequence(start, end));
    String result = String.valueOf(dest.subSequence(0, dstart))
            + filtered
            + dest.subSequence(dend, dest.length());
    String str = String.valueOf(result).toLowerCase();
    for (String val : mDisplayedValues) {
        val = val.toLowerCase();
        if (val.startsWith(str)) {
            return filtered;
        }
    }*/
    return NULL;
}

// XXX This doesn't allow for range limits when controlled by a
// soft input method!
Int32 NumberPicker::NumberRangeKeyListener::GetInputType()
{
    return InputType_TYPE_CLASS_NUMBER;
}

Char16* NumberPicker::NumberRangeKeyListener::GetAcceptedChars() 
{
    return DIGIT_CHARACTERS;
}

AutoPtr<ICharSequence> NumberPicker::NumberRangeKeyListener::Filter(
    /* [in] */ ICharSequence* source, 
    /* [in] */ Int32 start, 
    /* [in] */ Int32 end,
    /* [in] */ ISpanned* dest, 
    /* [in] */ Int32 dstart, 
    /* [in] */ Int32 dend)
{
    AutoPtr<ICharSequence> filtered = NumberKeyListener::Filter(source, start, end, dest, dstart, dend);
    if (filtered == NULL) {
        source->SubSequence(start, end, (ICharSequence**)&filtered);
    }

    /*String result = String.valueOf(dest.subSequence(0, dstart))
            + filtered
            + dest.subSequence(dend, dest.length());

    if ("".equals(result)) {
        return result;
    }*/
    //Int32 val = GetSelectedPos(result);

    /* Ensure the user can't type in a value greater
     * than the max allowed. We have to allow less than min
     * as the user might want to delete some numbers
     * and then type a new number.
     */
    /*if (val > mEnd) {
        return "";
    } else {
        return filtered;
    }*/

    return filtered;
}

Int32 NumberPicker::GetSelectedPos(
    /* [in] */ String str)
{
    //if (mDisplayedValues == NULL) {
    //    try {
    //        return Integer.parseInt(str);
    //    } catch (NumberFormatException e) {
    //        /* Ignore as if it's not a number we don't care */
    //    }
    //} else {
    //    for (Int32 i = 0; i < mDisplayedValues.length; i++) {
    //        /* Don't force the user to type in jan when ja will do */
    //        str = str.toLowerCase();
    //        if (mDisplayedValues[i].toLowerCase().startsWith(str)) {
    //            return mStart + i;
    //        }
    //    }

    //    /* The user might have typed in a number into the month field i.e.
    //     * 10 instead of OCT so support that too.
    //     */
    //    try {
    //        return Integer.parseInt(str);
    //    } catch (NumberFormatException e) {

    //        /* Ignore as if it's not a number we don't care */
    //    }
    //}
    return mStart;
}

/**
 * Returns the current value of the NumberPicker
 * @return the current value.
 */
Int32 NumberPicker::GetCurrent()
{
    return mCurrent;
}

/**
 * Returns the upper value of the range of the NumberPicker
 * @return the uppper number of the range.
 */
Int32 NumberPicker::GetEndRange()
{
    return mEnd;
}

/**
 * Returns the lower value of the range of the NumberPicker
 * @return the lower number of the range.
 */
Int32 NumberPicker::GetBeginRange()
{
    return mStart;
}
