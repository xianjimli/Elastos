
#include "widget/NumberPicker.h"
#include "os/CApartment.h"

const ArrayOf<Char32>* NumberPicker::InitDigitCharacters()
{
    ArrayOf<Char32>* charArray = ArrayOf<Char32>::Alloc(10);
    (*charArray)[0] = '0';
    (*charArray)[1] = '1';
    (*charArray)[2] = '2';
    (*charArray)[3] = '3';
    (*charArray)[4] = '4';
    (*charArray)[5] = '5';
    (*charArray)[6] = '6';
    (*charArray)[7] = '7';
    (*charArray)[8] = '8';
    (*charArray)[9] = '9';

    return charArray;
}

const ArrayOf<Char32>* NumberPicker::DIGIT_CHARACTERS = InitDigitCharacters();

NumberPicker::OnLongClickRunnable::OnLongClickRunnable(
    /* [in] */ NumberPicker* host)
    : mHost(host)
{
}

PInterface NumberPicker::OnLongClickRunnable::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IRunnable*)this;
    }
    else if (riid == EIID_IRunnable) {
        return (IRunnable*)this;
    }

    return NULL;
}

UInt32 NumberPicker::OnLongClickRunnable::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 NumberPicker::OnLongClickRunnable::Release()
{
    return ElRefBase::Release();
}

ECode NumberPicker::OnLongClickRunnable::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode NumberPicker::OnLongClickRunnable::Run()
{
    if (mHost->mIncrement) {
        mHost->ChangeCurrent(mHost->mCurrent + 1);
        ECode (STDCALL IRunnable::*pHandlerFunc)();
        pHandlerFunc = &IRunnable::Run;
        mHost->mHandler->PostCppCallbackDelayed(
                (Handle32)this, *(Handle32*)&pHandlerFunc, NULL, 0, mHost->mSpeed);
    }
    else if (mHost->mDecrement) {
        mHost->ChangeCurrent(mHost->mCurrent - 1);
        ECode (STDCALL IRunnable::*pHandlerFunc)();
        pHandlerFunc = &IRunnable::Run;
        mHost->mHandler->PostCppCallbackDelayed(
                (Handle32)this, *(Handle32*)&pHandlerFunc, NULL, 0, mHost->mSpeed);
    }

    return NOERROR;
}

NumberPicker::ClickListener::ClickListener(
    /* [in] */ NumberPicker* host)
    : mHost(host)
{
}

PInterface NumberPicker::ClickListener::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IViewOnClickListener*)this;
    }
    else if (riid == EIID_IViewOnClickListener) {
        return (IViewOnClickListener*)this;
    }

    return NULL;
}

UInt32 NumberPicker::ClickListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 NumberPicker::ClickListener::Release()
{
    return ElRefBase::Release();
}

ECode NumberPicker::ClickListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode NumberPicker::ClickListener::OnClick(
    /* [in] */ IView* v)
{
    mHost->ValidateInput(mHost->mText);
    Boolean hasFocus;
    mHost->mText->HasFocus(&hasFocus);
    if (!hasFocus) {
        Boolean result;
        mHost->mText->RequestFocus(&result);
    }

    // now perform the increment/decrement
    Int32 id;
    v->GetId(&id);
    if (0x010201fa/*R.id.increment*/ == id) {
        mHost->ChangeCurrent(mHost->mCurrent + 1);
    }
    else if (0x010201fc/*R.id.decrement*/ == id) {
        mHost->ChangeCurrent(mHost->mCurrent - 1);
    }

    return NOERROR;
}

NumberPicker::FocusListener::FocusListener(
    /* [in] */ NumberPicker* host)
    : mHost(host)
{
}

PInterface NumberPicker::FocusListener::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IViewOnFocusChangeListener*)this;
    }
    else if (riid == EIID_IViewOnFocusChangeListener) {
        return (IViewOnFocusChangeListener*)this;
    }

    return NULL;
}

UInt32 NumberPicker::FocusListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 NumberPicker::FocusListener::Release()
{
    return ElRefBase::Release();
}

ECode NumberPicker::FocusListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode NumberPicker::FocusListener::OnFocusChange(
    /* [in] */ IView* v,
    /* [in] */ Boolean hasFocus)
{
    /* When focus is lost check that the text field
     * has valid values.
     */
    if (!hasFocus) {
        mHost->ValidateInput(v);
    }

    return NOERROR;
}

NumberPicker::LongClickListener::LongClickListener(
    /* [in] */ NumberPicker* host)
    : mHost(host)
{
}

PInterface NumberPicker::LongClickListener::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IViewOnLongClickListener*)this;
    }
    else if (riid == EIID_IViewOnLongClickListener) {
        return (IViewOnLongClickListener*)this;
    }

    return NULL;
}

UInt32 NumberPicker::LongClickListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 NumberPicker::LongClickListener::Release()
{
    return ElRefBase::Release();
}

ECode NumberPicker::LongClickListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode NumberPicker::LongClickListener::OnLongClick(
    /* [in] */ IView* v,
    /* [out] */ Boolean* result)
{
    /* The text view may still have focus so clear it's focus which will
     * trigger the on focus changed and any typed values to be pulled.
     */
    VALIDATE_NOT_NULL(result);
    mHost->mText->ClearFocus();

    Int32 id;
    v->GetId(&id);
    if (0x010201fa/*R.id.increment*/ == id) {
        mHost->mIncrement = TRUE;
        ECode (STDCALL IRunnable::*pHandlerFunc)();
        pHandlerFunc = &IRunnable::Run;
        mHost->mHandler->PostCppCallback(
                    (Handle32)mHost->mRunnable.Get(), *(Handle32*)&pHandlerFunc
                    , NULL, 0);
    }
    else if (0x010201fc/*R.id.decrement*/ == id) {
        mHost->mDecrement = TRUE;
        ECode (STDCALL IRunnable::*pHandlerFunc)();
        pHandlerFunc = &IRunnable::Run;
        mHost->mHandler->PostCppCallback(
                    (Handle32)mHost->mRunnable.Get(), *(Handle32*)&pHandlerFunc
                    , NULL, 0);
    }

    *result = TRUE;
    return NOERROR;
}

NumberPicker::NumberPicker()
    : mStart(0)
    , mEnd(0)
    , mCurrent(0)
    , mPrevious(0)
    , mSpeed(300)
    , mIncrement(FALSE)
    , mDecrement(FALSE)
{
    mRunnable = (IRunnable*)new OnLongClickRunnable(this);
}

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

NumberPicker::~NumberPicker()
{
    ArrayOf<String>::Free(mDisplayedValues);
}

void NumberPicker::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    LinearLayout::Init(context, attrs);
    SetOrientation(VERTICAL);
    AutoPtr<ILayoutInflater> inflater;
    mContext->GetSystemService(Context_LAYOUT_INFLATER_SERVICE,
            (IInterface**)&inflater);
    AutoPtr<IView> v;
    inflater->InflateEx2(0x01090043/*R.layout.number_picker*/,
            (IViewGroup*)this->Probe(EIID_IViewGroup), TRUE, (IView**)&v);
    CApartment::New(TRUE, (IApartment**)&mHandler);
    mHandler->Start(ApartmentAttr_New);

    ClickListener* clickListener = new ClickListener(this);

    FocusListener* focusListener = new FocusListener(this);

    LongClickListener* longClickListener = new LongClickListener(this);

    //InputFilter inputFilter = new NumberPickerInputFilter();
    //mNumberInputFilter = new NumberRangeKeyListener();

    mIncrementButton = (INumberPickerButton*)
            FindViewById(0x010201fa/*R.id.increment*/).Get();
    assert(mIncrementButton != NULL);
    mIncrementButton->SetOnClickListener((IViewOnClickListener*)clickListener);
    mIncrementButton->SetOnLongClickListener(
                (IViewOnLongClickListener*)longClickListener);
    mIncrementButton->SetNumberPicker(
            (INumberPicker*)this->Probe(EIID_INumberPicker));

    mDecrementButton = (INumberPickerButton*)
            FindViewById(0x010201fc/*R.id.decrement*/).Get();
    mDecrementButton->SetOnClickListener((IViewOnClickListener*)clickListener);
    mDecrementButton->SetOnLongClickListener(
                (IViewOnLongClickListener*)longClickListener);
    mDecrementButton->SetNumberPicker(
            (INumberPicker*)this->Probe(EIID_INumberPicker));

    mText = (IEditText*)FindViewById(0x010201fb/*R.id.timepicker_input*/).Get();
    mText->SetOnFocusChangeListener((IViewOnFocusChangeListener*)focusListener);
    //mText->SetFilters(new InputFilter[] {inputFilter});
    mText->SetRawInputType(InputType_TYPE_CLASS_NUMBER);

    if (!IsEnabled()) {
       SetEnabled(FALSE);
    }
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
    return SetRangeEx(start, end, NULL/*displayedValues*/);
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
ECode NumberPicker::SetRangeEx(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<String>* displayedValues)
{
    mDisplayedValues = displayedValues->Clone();
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
    String s;
    if (mFormatter != NULL) {
        mFormatter->ToString(value, &s);
    }
    else {
        s = String::FromInt32(value);
    }

    return s;
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
    }
    else if (current < mStart) {
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
        mListener->OnChanged((INumberPicker*)Probe(EIID_INumberPicker),
                mPrevious, mCurrent);
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
    AutoPtr<ICharSequence> cs;
    if (mDisplayedValues == NULL) {
        String s = FormatNumber(mCurrent);
        CStringWrapper::New(FormatNumber(mCurrent), (ICharSequence**)&cs);
        mText->SetText(cs);
    }
    else {
        String s = (*mDisplayedValues)[mCurrent - mStart];
        CStringWrapper::New((*mDisplayedValues)[mCurrent - mStart],
                (ICharSequence**)&cs);
        mText->SetText(cs);
    }

    Int32 length;
    mText->GetText((ICharSequence**)&cs);
    cs->GetLength(&length);
    mText->SetSelectionEx(length);
}

void NumberPicker::ValidateCurrentView(
    /* [in] */ ICharSequence* str)
{
    String s;
    str->ToString(&s);
    Int32 val = GetSelectedPos(s);
    if ((val >= mStart) && (val <= mEnd)) {
        if (mCurrent != val) {
            mPrevious = mCurrent;
            mCurrent = val;
            NotifyChange();
        }
    }
    UpdateView();
}

void NumberPicker::ValidateInput(
    /* [in] */ IView* v)
{
    AutoPtr<ICharSequence> cs;
    ((ITextView*)v)->GetText((ICharSequence**)&cs);
    String str;
    cs->ToString(&str);
    if (String("").Equals(str)) {
        // Restore to the old value as we don't allow empty values
        UpdateView();
    }
    else {
        // Check the new value and ensure it's in range
        ValidateCurrentView(cs);
    }
}

/**
 * @hide
 */
ECode NumberPicker::CancelIncrement()
{
    mIncrement = FALSE;
    return NOERROR;
}

/**
 * @hide
 */
ECode NumberPicker::CancelDecrement()
{
    mDecrement = FALSE;
    return NOERROR;
}

NumberPicker::NumberPickerInputFilter::NumberPickerInputFilter(
    /* [in] */ NumberPicker* host)
        : mHost(host)
{}

ECode NumberPicker::NumberPickerInputFilter::Filter(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ISpanned* dest,
    /* [in] */ Int32 dstart,
    /* [in] */ Int32 dend,
    /*[out] */ ICharSequence** cs)
{
    VALIDATE_NOT_NULL(cs);

    // if (mHost->mDisplayedValues == NULL) {
    //     return mHost->mNumberInputFilter->Filter(
    //                 source, start, end, dest, dstart, dend, cs);
    // }
    AutoPtr<ICharSequence> filtered, destCs1, destCs2;
    source->SubSequence(start, end, (ICharSequence**)&filtered);
    dest->SubSequence(0, dstart, (ICharSequence**)&destCs1);
    Int32 len;
    dest->GetLength(&len);
    dest->SubSequence(dend, len, (ICharSequence**)&destCs2);
    String filteredStr, destStr1, destStr2;
    filtered->ToString(&filteredStr);
    destCs1->ToString(&destStr1);
    destCs2->ToString(&destStr2);
    String result = filteredStr + destStr1+ destStr2;
    result.ToLowerCase();
    for(Int32 i = 0; i < mHost->mDisplayedValues->GetLength(); ++i) {
        String val = (*(mHost->mDisplayedValues))[i];
        val.ToLowerCase();
        if (val.StartWith(result)) {
            *cs = filtered;
            (*cs)->AddRef();
            return NOERROR;
        }
    }

    return CStringWrapper::New(String(""), cs);
}

NumberPicker::NumberRangeKeyListener::NumberRangeKeyListener(
    /* [in] */ NumberPicker* host)
        : mHost(host)
{}

// XXX This doesn't allow for range limits when controlled by a
// soft input method!
Int32 NumberPicker::NumberRangeKeyListener::GetInputType()
{
    return InputType_TYPE_CLASS_NUMBER;
}

Int32 NumberPicker::NumberRangeKeyListener::Lookup(
    /* [in] */ IKeyEvent* event,
    /* [in] */ ISpannable* content)
{
    return NumberKeyListener::Lookup(event, content);
}

ArrayOf<Char32>* NumberPicker::NumberRangeKeyListener::GetAcceptedChars()
{
    return const_cast<ArrayOf<Char32>*>(DIGIT_CHARACTERS);
}

ECode NumberPicker::NumberRangeKeyListener::Filter(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ISpanned* dest,
    /* [in] */ Int32 dstart,
    /* [in] */ Int32 dend,
    /*[out] */ ICharSequence** cs)
{
    AutoPtr<ICharSequence> filtered;
    NumberKeyListener::Filter(
            source, start, end, dest, dstart, dend, (ICharSequence**)&filtered);
    if (filtered == NULL) {
        source->SubSequence(start, end, (ICharSequence**)&filtered);
    }

    AutoPtr<ICharSequence> destCs1, destCs2;
    dest->SubSequence(0, dstart, (ICharSequence**)&destCs1);
    Int32 len;
    dest->GetLength(&len);
    dest->SubSequence(dend, len, (ICharSequence**)&destCs2);
    String filteredStr, destStr1, destStr2;
    filtered->ToString(&filteredStr);
    destCs1->ToString(&destStr1);
    destCs2->ToString(&destStr2);
    String result = destStr1 + filteredStr + destStr2;


    if (String("").Equals(result)) {
        return CStringWrapper::New(result, cs);
    }
    Int32 val = mHost->GetSelectedPos(result);

    /* Ensure the user can't type in a value greater
     * than the max allowed. We have to allow less than min
     * as the user might want to delete some numbers
     * and then type a new number.
     */
    if (val > mHost->mEnd) {
        return CStringWrapper::New(String(""), cs);
    }
    else {
        *cs = filtered;
        (*cs)->AddRef();
        return NOERROR;
    }
}

Boolean NumberPicker::NumberRangeKeyListener::OnKeyDown(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return NumberKeyListener::OnKeyDown(view, content, keyCode, event);
}

Int32 NumberPicker::GetSelectedPos(
    /* [in] */ String str)
{
    if (mDisplayedValues == NULL) {
        //try {
        return str.ToInt32();
        // } catch (NumberFormatException e) {
        //      Ignore as if it's not a number we don't care
        // }
    }
    else {
        Int32 len = mDisplayedValues->GetLength();
        for (Int32 i = 0; i < len; i++) {
            /* Don't force the user to type in jan when ja will do */
            str.ToLowerCase();
            String val = (*mDisplayedValues)[i];
            val.ToLowerCase();
            if (val.StartWith(str)) {
                return mStart + i;
            }
        }

        /* The user might have typed in a number into the month field i.e.
         * 10 instead of OCT so support that too.
         */
        //try {
        return str.ToInt32();
        // } catch (NumberFormatException e) {

        //      Ignore as if it's not a number we don't care
        // }
    }
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
