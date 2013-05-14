
#include "widget/DatePicker.h"
#include "widget/CDatePickerSavedState.h"
#include "widget/NumberPicker.h"

const Int32 DatePicker::DEFAULT_START_YEAR;
const Int32 DatePicker::DEFAULT_END_YEAR;

// This ignores Undecimber, but we only support real Gregorian calendars.
const Int32 DatePicker::NUMBER_OF_MONTHS;

static Int32 R_Styleable_DatePicker[] = { 0x0101017c, 0x0101017d };

DatePicker::OnDayChangedListener::OnDayChangedListener(
    /* [in] */ DatePicker* host)
    : mHost(host)
{
}

PInterface DatePicker::OnDayChangedListener::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IOnChangedListener*)this;
    }
    else if (riid == EIID_IOnChangedListener) {
        return (IOnChangedListener*)this;
    }

    return NULL;
}

UInt32 DatePicker::OnDayChangedListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 DatePicker::OnDayChangedListener::Release()
{
    return ElRefBase::Release();
}

ECode DatePicker::OnDayChangedListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode DatePicker::OnDayChangedListener::OnChanged(
    /* [in] */ INumberPicker* picker,
    /* [in] */ Int32 oldVal,
    /* [in] */ Int32 newVal)
{
    mHost->mDay = newVal;
    mHost->NotifyDateChanged();
    return NOERROR;
}

DatePicker::OnMonthChangedListener::OnMonthChangedListener(
    /* [in] */ DatePicker* host)
    : mHost(host)
{
}

PInterface DatePicker::OnMonthChangedListener::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IOnChangedListener*)this;
    }
    else if (riid == EIID_IOnChangedListener) {
        return (IOnChangedListener*)this;
    }

    return NULL;
}

UInt32 DatePicker::OnMonthChangedListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 DatePicker::OnMonthChangedListener::Release()
{
    return ElRefBase::Release();
}

ECode DatePicker::OnMonthChangedListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode DatePicker::OnMonthChangedListener::OnChanged(
    /* [in] */ INumberPicker* picker,
    /* [in] */ Int32 oldVal,
    /* [in] */ Int32 newVal)
{
    /* We display the month 1-12 but store it 0-11 so always
     * subtract by one to ensure our internal state is always 0-11
     */
    mHost->mMonth = newVal - 1;
    // Adjust max day of the month
    mHost->AdjustMaxDay();
    mHost->NotifyDateChanged();
    mHost->UpdateDaySpinner();
    return NOERROR;
}

DatePicker::OnYearChangedListener::OnYearChangedListener(
    /* [in] */ DatePicker* host)
    : mHost(host)
{
}

PInterface DatePicker::OnYearChangedListener::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IOnChangedListener*)this;
    }
    else if (riid == EIID_IOnChangedListener) {
        return (IOnChangedListener*)this;
    }

    return NULL;
}

UInt32 DatePicker::OnYearChangedListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 DatePicker::OnYearChangedListener::Release()
{
    return ElRefBase::Release();
}

ECode DatePicker::OnYearChangedListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode DatePicker::OnYearChangedListener::OnChanged(
    /* [in] */ INumberPicker* picker,
    /* [in] */ Int32 oldVal,
    /* [in] */ Int32 newVal)
{
    mHost->mYear = newVal - 1;
    // Adjust max day for leap years if needed
    mHost->AdjustMaxDay();
    mHost->NotifyDateChanged();
    mHost->UpdateDaySpinner();
    return NOERROR;
}

DatePicker::DatePicker()
    : mDay(0)
    , mMonth(0)
    , mYear(0)
{
}

DatePicker::~DatePicker()
{
    ArrayOf<String>::Free(mShortMonths);
}

void DatePicker::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FrameLayout::Init(context, attrs, defStyle);

    AutoPtr<ILayoutInflater> inflater;
    context->GetSystemService(Context_LAYOUT_INFLATER_SERVICE,
            (IInterface**)&inflater);
    AutoPtr<IView> v;
    inflater->InflateEx2(0x0109001f/*R.layout.date_picker*/,
            (IViewGroup*)this->Probe(EIID_IViewGroup), TRUE, (IView**)&v);

    mDayPicker = (INumberPicker*)FindViewById(0x0102019f/*R.id.day*/).Get();
    //mDayPicker->SetFormatter(NumberPicker::TWO_DIGIT_FORMATTER);
    mDayPicker->SetSpeed(100);
    OnDayChangedListener* dayListener = new OnDayChangedListener(this);
    mDayPicker->SetOnChangeListener((IOnChangedListener*)dayListener);
    mMonthPicker = (INumberPicker*)FindViewById(0x0102019e/*R.id.month*/).Get();
    //mMonthPicker->SetFormatter(NumberPicker::TWO_DIGIT_FORMATTER);
    ArrayOf<String>* months = GetShortMonths();

    /*
     * If the user is in a locale where the month names are numeric,
     * use just the number instead of the "month" character for
     * consistency with the other fields.
     */
    if ((*months)[0].StartWith(String("1"))) {
        for (Int32 i = 0; i < months->GetLength(); i++) {
            (*months)[i] = String::FromInt32(i + 1);
        }
        mMonthPicker->SetRange(1, NUMBER_OF_MONTHS);
    }
    else {
        mMonthPicker->SetRangeEx(1, NUMBER_OF_MONTHS, *months);
    }

    mMonthPicker->SetSpeed(200);
    OnMonthChangedListener* monthListener = new OnMonthChangedListener(this);
    mMonthPicker->SetOnChangeListener((IOnChangedListener*)monthListener);
    mYearPicker = (INumberPicker*)FindViewById(0x010201a0/*R.id.year*/).Get();
    mYearPicker->SetSpeed(100);
    OnYearChangedListener* yearListener = new OnYearChangedListener(this);
    mYearPicker->SetOnChangeListener((IOnChangedListener*)yearListener);

    // attributes
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributesEx2(attrs,
            ArrayOf<Int32>(R_Styleable_DatePicker, 2)/*R.styleable.DatePicker*/,
            (ITypedArray**)&a));

    Int32 startYear, endYear;
    a->GetInt32(0/*R.styleable.DatePicker_startYear*/, DEFAULT_START_YEAR, &startYear);
    a->GetInt32(1/*R.styleable.DatePicker_endYear*/, DEFAULT_END_YEAR, &endYear);
    mYearPicker->SetRange(startYear, endYear);

    a->Recycle();

    // initialize to current date
    AutoPtr<ICalendar> cal;
    AutoPtr<ICalendarHelper> calHelper;
    ASSERT_SUCCEEDED(CCalendarHelper::AcquireSingleton((ICalendarHelper**)&calHelper));
    calHelper->GetInstance((ICalendar**)&cal);
    Int32 year, month, day;
    cal->Get(Calendar_YEAR, &year);
    cal->Get(Calendar_MONTH, &month);
    cal->Get(Calendar_DAY_OF_MONTH, &day);

    ASSERT_SUCCEEDED(Init(year, month, day, NULL));

    // re-order the number pickers to match the current date format
    ReorderPickers(months);

    if (!IsEnabled()) {
        SetEnabled(FALSE);
    }
}

ECode DatePicker::SetEnabled(
    /* [in] */ Boolean enabled)
{
    FrameLayout::SetEnabled(enabled);
    mDayPicker->SetEnabled(enabled);
    mMonthPicker->SetEnabled(enabled);
    mYearPicker->SetEnabled(enabled);
    return NOERROR;
}

void DatePicker::ReorderPickers(
    /* [in] */ ArrayOf<String>* months)
{
    AutoPtr<IDateFormat> format;
    String order;

    /*
     * If the user is in a locale where the medium date format is
     * still numeric (Japanese and Czech, for example), respect
     * the date format order setting.  Otherwise, use the order
     * that the locale says is appropriate for a spelled-out date.
     */
    // AutoPtr<IDateFormatHelper> formatHelper;//should define in framework
    // CDateFormatHelper::AcquireSingleton((IDateFormatHelper**)&formatHelper);
    // if ((*months)[0].StartWith(String("1"))) {
    //     format = DateFormat.getDateFormat(getContext());
    // }
    // else {
    //     format = DateFormat.getMediumDateFormat(getContext());
    // }

    if (format && format->Probe(EIID_ISimpleDateFormat) != NULL) {
        ((ISimpleDateFormat*)format->Probe(EIID_ISimpleDateFormat))->ToPattern(&order);
    }
    else {
        // Shouldn't happen, but just in case.
        //order = new String(DateFormat.getDateFormatOrder(getContext()));
    }

    /* Remove the 3 pickers from their parent and then add them back in the
     * required order.
     */
    AutoPtr<ILinearLayout> parent =
            (ILinearLayout*)FindViewById(0x0102019d/*R.id.parent*/).Get();
    parent->RemoveAllViews();

    //Boolean quoted = FALSE;
    Boolean didDay = FALSE, didMonth = FALSE, didYear = FALSE;

    // for (Int32 i = 0; i < order.GetLength(); i++) {
    //     char c = order[i];

    //     if (c == '\'') {
    //         quoted = !quoted;
    //     }

    //     if (!quoted) {
    //         if (c == DateFormat_DATE && !didDay) {
    //             parent->AddView((IView*)mDayPicker);
    //             didDay = TRUE;
    //         }
    //         else if ((c == DateFormat_MONTH || c == 'L') && !didMonth) {
    //             parent->AddView((IView*)mMonthPicker);
    //             didMonth = TRUE;
    //         }
    //         else if (c == DateFormat_YEAR && !didYear) {
    //             parent->AddView((IView*)mYearPicker);
    //             didYear = TRUE;
    //         }
    //     }
    // }

    // Shouldn't happen, but just in case.
    if (!didMonth) {
        parent->AddView((IView*)mMonthPicker);
    }
    if (!didDay) {
        parent->AddView((IView*)mDayPicker);
    }
    if (!didYear) {
        parent->AddView((IView*)mYearPicker);
    }
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

    return NOERROR;
}

ArrayOf<String>* DatePicker::GetShortMonths()
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> currentLocale;
    localeHelper->GetDefault((ILocale**)&currentLocale);
    if (currentLocale == mMonthLocale && mShortMonths != NULL) {
        return mShortMonths;
    }
    else {
        Mutex::Autolock lock(mMonthUpdateLock);
        if (currentLocale != mMonthLocale) {
            mShortMonths = ArrayOf<String>::Alloc(NUMBER_OF_MONTHS);
            for (Int32 i = 0; i < NUMBER_OF_MONTHS; i++) {
                (*mShortMonths)[i] = String::FromInt32(i+1);
                // (*mShortMonths)[i] = DateUtils.getMonthString(Calendar.JANUARY + i,
                //         DateUtils.LENGTH_MEDIUM);
            }
            mMonthLocale = currentLocale;
        }
        return mShortMonths;
    }
}

/**
 * Override so we are in complete control of save / restore for this widget.
 */
ECode DatePicker::DispatchRestoreInstanceState(
    /* [in] */ IObjectIntegerMap* container)
{
    return DispatchThawSelfOnly(container);
}

AutoPtr<IParcelable> DatePicker::OnSaveInstanceState()
{
    AutoPtr<IParcelable> superState = FrameLayout::OnSaveInstanceState();

    AutoPtr<IViewBaseSavedState> savedState;
    ASSERT_SUCCEEDED(CDatePickerSavedState::New(
            superState, mYear, mMonth, mDay, (IViewBaseSavedState**)&savedState));
    return (IParcelable*)savedState->Probe(EIID_IParcelable);
}

void DatePicker::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    AutoPtr<IViewBaseSavedState> ss =
            (IViewBaseSavedState*)state->Probe(EIID_IViewBaseSavedState);
    assert(ss != NULL);
    AutoPtr<IParcelable> superState;
    ss->GetSuperState((IParcelable**)&superState);
    FrameLayout::OnRestoreInstanceState(superState);
    mYear = ((CDatePickerSavedState*)ss.Get())->GetYear();
    mMonth = ((CDatePickerSavedState*)ss.Get())->GetMonth();
    mDay = ((CDatePickerSavedState*)ss.Get())->GetDay();
    UpdateSpinners();
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
    mYearPicker->SetCurrent(mYear);

    /* The month display uses 1-12 but our internal state stores it
     * 0-11 so add one when setting the display.
     */
    mMonthPicker->SetCurrent(mMonth + 1);
}

void DatePicker::UpdateDaySpinner()
{
    AutoPtr<ICalendar> cal;
    AutoPtr<ICalendarHelper> calHelper;
    ASSERT_SUCCEEDED(CCalendarHelper::AcquireSingleton((ICalendarHelper**)&calHelper));
    calHelper->GetInstance((ICalendar**)&cal);
    cal->SetEx(mYear, mMonth, mDay);
    Int32 max;
    cal->GetActualMaximum(Calendar_DAY_OF_MONTH, &max);
    mDayPicker->SetRange(1, max);
    mDayPicker->SetCurrent(mDay);
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
    AutoPtr<ICalendar> cal;
    AutoPtr<ICalendarHelper> calHelper;
    ASSERT_SUCCEEDED(CCalendarHelper::AcquireSingleton((ICalendarHelper**)&calHelper));
    calHelper->GetInstance((ICalendar**)&cal);
    cal->Set(Calendar_YEAR, mYear);
    cal->Set(Calendar_MONTH, mMonth);
    Int32 max;
    cal->GetActualMaximum(Calendar_DAY_OF_MONTH, &max);
    if (mDay > max) {
        mDay = max;
    }
}

void DatePicker::NotifyDateChanged()
{
    if (mOnDateChangedListener != NULL) {
        mOnDateChangedListener->OnDateChanged(
                (IDatePicker*)this->Probe(EIID_IDatePicker), mYear, mMonth, mDay);
    }
}

