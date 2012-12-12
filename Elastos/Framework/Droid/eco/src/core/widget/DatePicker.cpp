
#include "widget/DatePicker.h"

const Int32 DatePicker::DEFAULT_START_YEAR;
const Int32 DatePicker::DEFAULT_END_YEAR;

// This ignores Undecimber, but we only support real Gregorian calendars.
const Int32 DatePicker::NUMBER_OF_MONTHS;

DatePicker::DatePicker()
{

}

DatePicker::DatePicker(
    /* [in] */ IContext* context) : FrameLayout(context, NULL, 0)
{
    Init(context, NULL);
}

DatePicker::DatePicker(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs) : FrameLayout(context, attrs, 0)
{
    Init(context, attrs, 0);
}

DatePicker::DatePicker(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs, 
    /* [in] */ Int32 defStyle) : FrameLayout(context, attrs, defStyle)
{
    Init(context, attrs, defStyle);
}

void DatePicker::Init(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs, 
    /* [in] */ Int32 defStyle)
{
    //LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    //inflater.inflate(R.layout.date_picker, this, TRUE);

    /*mDayPicker = (NumberPicker) findViewById(R.id.day);
    mDayPicker.setFormatter(NumberPicker.TWO_DIGIT_FORMATTER);
    mDayPicker.setSpeed(100);
    mDayPicker.setOnChangeListener(new OnChangedListener() {
        public void onChanged(NumberPicker picker, Int32 oldVal, Int32 newVal) {
            mDay = newVal;
            notifyDateChanged();
        }
    });
    mMonthPicker = (NumberPicker) findViewById(R.id.month);
    mMonthPicker.setFormatter(NumberPicker.TWO_DIGIT_FORMATTER);*/
    //final String[] months = getShortMonths();

    /*
     * If the user is in a locale where the month names are numeric,
     * use just the number instead of the "month" character for
     * consistency with the other fields.
     */
    //if (months[0].startsWith("1")) {
    //    for (Int32 i = 0; i < months.length; i++) {
    //        months[i] = String.valueOf(i + 1);
    //    }
    //    //mMonthPicker.setRange(1, NUMBER_OF_MONTHS);
    //} else {
    //    //mMonthPicker.setRange(1, NUMBER_OF_MONTHS, months);
    //}

    //mMonthPicker.setSpeed(200);
    //mMonthPicker.setOnChangeListener(new OnChangedListener() {
    //    public void onChanged(NumberPicker picker, Int32 oldVal, Int32 newVal) {
    //        
    //        /* We display the month 1-12 but store it 0-11 so always
    //         * subtract by one to ensure our internal state is always 0-11
    //         */
    //        mMonth = newVal - 1;
    //        // Adjust max day of the month
    //        adjustMaxDay();
    //        notifyDateChanged();
    //        updateDaySpinner();
    //    }
    //});
    //mYearPicker = (NumberPicker) findViewById(R.id.year);
    //mYearPicker.setSpeed(100);
    //mYearPicker.setOnChangeListener(new OnChangedListener() {
    //    public void onChanged(NumberPicker picker, Int32 oldVal, Int32 newVal) {
    //        mYear = newVal;
    //        // Adjust max day for leap years if needed
    //        adjustMaxDay();
    //        notifyDateChanged();
    //        updateDaySpinner();
    //    }
    //});
    
    // attributes
    //TypedArray a = context.obtainStyledAttributes(attrs, R.styleable.DatePicker);

    //Int32 mStartYear = a.getInt(R.styleable.DatePicker_startYear, DEFAULT_START_YEAR);
    //Int32 mEndYear = a.getInt(R.styleable.DatePicker_endYear, DEFAULT_END_YEAR);
    ////mYearPicker.setRange(mStartYear, mEndYear);
    //
    //a.recycle();
    //
    //// initialize to current date
    //Calendar cal = Calendar.getInstance();
    //init(cal.get(Calendar.YEAR), cal.get(Calendar.MONTH), cal.get(Calendar.DAY_OF_MONTH), NULL);
    //
    //// re-order the number pickers to match the current date format
    //reorderPickers(months);
    //
    //if (!isEnabled()) {
    //    setEnabled(FALSE);
    //}
}

ECode DatePicker::SetEnabled(
    /* [in] */ Boolean enabled) 
{
    FrameLayout::SetEnabled(enabled);
    /*mDayPicker.setEnabled(enabled);
    mMonthPicker.setEnabled(enabled);
    mYearPicker.setEnabled(enabled);*/
}

void DatePicker::ReorderPickers(
    /* [in] */ ArrayOf<String>* months)
{
    //java.text.DateFormat format;
    //String order;

    ///*
    // * If the user is in a locale where the medium date format is
    // * still numeric (Japanese and Czech, for example), respect
    // * the date format order setting.  Otherwise, use the order
    // * that the locale says is appropriate for a spelled-out date.
    // */

    //if (months[0].startsWith("1")) {
    //    format = DateFormat.getDateFormat(getContext());
    //} else {
    //    format = DateFormat.getMediumDateFormat(getContext());
    //}

    //if (format instanceof SimpleDateFormat) {
    //    order = ((SimpleDateFormat) format).toPattern();
    //} else {
    //    // Shouldn't happen, but just in case.
    //    order = new String(DateFormat.getDateFormatOrder(getContext()));
    //}

    ///* Remove the 3 pickers from their parent and then add them back in the
    // * required order.
    // */
    //LinearLayout parent = (LinearLayout) findViewById(R.id.parent);
    //parent.removeAllViews();

    //Boolean quoted = FALSE;
    //Boolean didDay = FALSE, didMonth = FALSE, didYear = FALSE;

    //for (Int32 i = 0; i < order.length(); i++) {
    //    char c = order.charAt(i);

    //    if (c == '\'') {
    //        quoted = !quoted;
    //    }

    //    if (!quoted) {
    //        if (c == DateFormat.DATE && !didDay) {
    //            //parent.addView(mDayPicker);
    //            didDay = TRUE;
    //        } else if ((c == DateFormat.MONTH || c == 'L') && !didMonth) {
    //            //parent.addView(mMonthPicker);
    //            didMonth = TRUE;
    //        } else if (c == DateFormat.YEAR && !didYear) {
    //            //parent.addView (mYearPicker);
    //            didYear = TRUE;
    //        }
    //    }
    //}

    //// Shouldn't happen, but just in case.
    //if (!didMonth) {
    //    //parent.addView(mMonthPicker);
    //}
    //if (!didDay) {
    //    //parent.addView(mDayPicker);
    //}
    //if (!didYear) {
    //    //parent.addView(mYearPicker);
    //}
}

ECode DatePicker::UpdateDate(
    /* [in] */ Int32 year, 
    /* [in] */ Int32 monthOfYear, 
    /* [in] */ Int32 dayOfMonth)
{
    if (mYear != year || mMonth != monthOfYear || mDay != dayOfMonth) {
        mYear = year;
        mMonth = monthOfYear;
        mDay = dayOfMonth;
        UpdateSpinners();
        ReorderPickers(GetShortMonths());
        NotifyDateChanged();
    }
}

ArrayOf<String>* DatePicker::GetShortMonths()
{
    /*AutoPtr<ILocale> currentLocale = Locale.getDefault();
    if (currentLocale.equals(mMonthLocale) && mShortMonths != NULL) {
        return mShortMonths;
    } else {
        synchronized (mMonthUpdateLock) {
            if (!currentLocale.equals(mMonthLocale)) {
                mShortMonths = new String[NUMBER_OF_MONTHS];
                for (Int32 i = 0; i < NUMBER_OF_MONTHS; i++) {
                    mShortMonths[i] = DateUtils.getMonthString(Calendar.JANUARY + i,
                            DateUtils.LENGTH_MEDIUM);
                }
                mMonthLocale = currentLocale;
            }
        }
        return mShortMonths;
    }*/

    return NULL;
}

DatePicker::DatePickerSavedState::DatePickerSavedState(
    /* [in] */ IParcelable* superState, 
    /* [in] */ Int32 year, 
    /* [in] */ Int32 month, 
    /* [in] */ Int32 day)
{
    /*super(superState);
    mYear = year;
    mMonth = month;
    mDay = day;*/
}

/**
 * Constructor called from {@link #CREATOR}
 */
DatePicker::DatePickerSavedState::DatePickerSavedState(
    /* [in] */ IParcel* in)
{
    /*super(in);
    mYear = in.readInt();
    mMonth = in.readInt();
    mDay = in.readInt();*/
}

Int32 DatePicker::DatePickerSavedState::GetYear() 
{
    return mYear;
}

Int32 DatePicker::DatePickerSavedState::GetMonth()
{
    return mMonth;
}

Int32 DatePicker::DatePickerSavedState::GetDay()
{
    return mDay;
}

ECode DatePicker::DatePickerSavedState::WriteToParcel(
    /* [in] */ IParcel* dest, 
    /* [in] */ Int32 flags)
{
    /*super.writeToParcel(dest, flags);
    dest.writeInt(mYear);
    dest.writeInt(mMonth);
    dest.writeInt(mDay);*/
}

//public static final Parcelable.Creator<SavedState> CREATOR =
//        new Creator<SavedState>() {
//
//            public SavedState createFromParcel(Parcel in) {
//                return new SavedState(in);
//            }
//
//            public SavedState[] newArray(Int32 size) {
//                return new SavedState[size];
//            }
//        };



/**
 * Override so we are in complete control of save / restore for this widget.
 */
void DatePicker::DispatchRestoreInstanceState(
    /* [in] */ SparseArray* container) 
{
    //DispatchThawSelfOnly(container);
}

AutoPtr<IParcelable> DatePicker::OnSaveInstanceState()
{
    /*Parcelable superState = super.onSaveInstanceState();
    
    return new SavedState(superState, mYear, mMonth, mDay);*/

    return NULL;
}

void DatePicker::OnRestoreInstanceState(
    /* [in] */ IParcelable* state) 
{
    /*SavedState ss = (SavedState) state;
    super.onRestoreInstanceState(ss.getSuperState());
    mYear = ss.getYear();
    mMonth = ss.getMonth();
    mDay = ss.getDay();
    UpdateSpinners();*/
}

/**
 * Initialize the state.
 * @param year The initial year.
 * @param monthOfYear The initial month.
 * @param dayOfMonth The initial day of the month.
 * @param onDateChangedListener How user is notified date is changed by user, can be NULL.
 */
ECode DatePicker::Init(
    /* [in] */ Int32 year, 
    /* [in] */ Int32 monthOfYear, 
    /* [in] */ Int32 dayOfMonth,
    /* [in] */ IOnDateChangedListener* onDateChangedListener)
{
    mYear = year;
    mMonth = monthOfYear;
    mDay = dayOfMonth;
    mOnDateChangedListener = onDateChangedListener;
    UpdateSpinners();

    return NOERROR;
}

void DatePicker::UpdateSpinners()
{
    UpdateDaySpinner();
    //mYearPicker->SetCurrent(mYear);
    
    /* The month display uses 1-12 but our internal state stores it
     * 0-11 so add one when setting the display.
     */
    //mMonthPicker->SetCurrent(mMonth + 1);
}

void DatePicker::UpdateDaySpinner()
{
    /*Calendar cal = Calendar.getInstance();
    cal.set(mYear, mMonth, mDay);
    Int32 max = cal.getActualMaximum(Calendar.DAY_OF_MONTH);*/
    //mDayPicker.setRange(1, max);
    //mDayPicker.setCurrent(mDay);
}

Int32 DatePicker::GetYear() 
{
    return mYear;
}

Int32 DatePicker::GetMonth()
{
    return mMonth;
}

Int32 DatePicker::GetDayOfMonth()
{
    return mDay;
}

void DatePicker::AdjustMaxDay()
{
    /*Calendar cal = Calendar.getInstance();
    cal.set(Calendar.YEAR, mYear);
    cal.set(Calendar.MONTH, mMonth);
    Int32 max = cal.getActualMaximum(Calendar.DAY_OF_MONTH);
    if (mDay > max) {
        mDay = max;
    }*/
}

void DatePicker::NotifyDateChanged() 
{
    if (mOnDateChangedListener != NULL) {
        //mOnDateChangedListener->OnDateChanged(DatePicker.this, mYear, mMonth, mDay);
    }
}

