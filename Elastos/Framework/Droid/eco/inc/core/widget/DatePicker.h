
#ifndef __DATEPICKER_H__
#define __DATEPICKER_H__

#include "widget/FrameLayout.h"
#include "utils/SparseArray.h"
#include "view/ViewBaseSavedState.h"
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

/**
 * A view for selecting a month / year / day based on a calendar like layout.
 *
 * <p>See the <a href="{@docRoot}resources/tutorials/views/hello-datepicker.html">Date Picker
 * tutorial</a>.</p>
 *
 * For a dialog using this view, see {@link android.app.DatePickerDialog}.
 */
class DatePicker : public FrameLayout
{
private:
    class OnDayChangedListener
            : public IOnChangedListener
            , public ElRefBase
    {
    public:
        OnDayChangedListener(
            /* [in] */ DatePicker* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnChanged(
            /* [in] */ INumberPicker* picker,
            /* [in] */ Int32 oldVal,
            /* [in] */ Int32 newval);

    private:
        DatePicker* mHost;
    };

    class OnMonthChangedListener
            : public IOnChangedListener
            , public ElRefBase
    {
    public:
        OnMonthChangedListener(
            /* [in] */ DatePicker* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnChanged(
            /* [in] */ INumberPicker* picker,
            /* [in] */ Int32 oldVal,
            /* [in] */ Int32 newval);

    private:
        DatePicker* mHost;
    };

    class OnYearChangedListener
            : public IOnChangedListener
            , public ElRefBase
    {
    public:
        OnYearChangedListener(
            /* [in] */ DatePicker* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnChanged(
            /* [in] */ INumberPicker* picker,
            /* [in] */ Int32 oldVal,
            /* [in] */ Int32 newval);

    private:
        DatePicker* mHost;
    };

public:
    DatePicker();

    virtual ~DatePicker();

    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle = 0);

    virtual CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    virtual CARAPI UpdateDate(
        /* [in] */ Int32 year,
        /* [in] */ Int32 monthOfYear,
        /* [in] */ Int32 dayOfMonth);

    /**
     * Initialize the state.
     * @param year The initial year.
     * @param monthOfYear The initial month.
     * @param dayOfMonth The initial day of the month.
     * @param onDateChangedListener How user is notified date is changed by user, can be NULL.
     */
    virtual CARAPI Init(
        /* [in] */ Int32 year,
        /* [in] */ Int32 monthOfYear,
        /* [in] */ Int32 dayOfMonth,
        /* [in] */ IOnDateChangedListener* onDateChangedListener);

    virtual CARAPI_(Int32) GetYear();

    virtual CARAPI_(Int32) GetMonth();

    virtual CARAPI_(Int32) GetDayOfMonth();

protected:
    /**
     * Override so we are in complete control of save / restore for this widget.
     */
    virtual CARAPI DispatchRestoreInstanceState(
        /* [in] */ IObjectIntegerMap* container);

    virtual CARAPI_(AutoPtr<IParcelable>) OnSaveInstanceState();

    virtual CARAPI_(void) OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

private:
    CARAPI_(void) ReorderPickers(
        /* [in] */ ArrayOf<String>* months);

    CARAPI_(ArrayOf<String>*) GetShortMonths();
    CARAPI_(void) UpdateSpinners();

    CARAPI_(void) UpdateDaySpinner();

    CARAPI_(void) AdjustMaxDay();

    CARAPI_(void) NotifyDateChanged();

private:
    static const Int32 DEFAULT_START_YEAR = 1900;
    static const Int32 DEFAULT_END_YEAR = 2100;

    // This ignores Undecimber, but we only support real Gregorian calendars.
    static const Int32 NUMBER_OF_MONTHS = 12;

    /* UI Components */
    AutoPtr<INumberPicker> mDayPicker;
    AutoPtr<INumberPicker> mMonthPicker;
    AutoPtr<INumberPicker> mYearPicker;

    /**
     * How we notify users the date has changed.
     */
    AutoPtr<IOnDateChangedListener> mOnDateChangedListener;

    Int32 mDay;
    Int32 mMonth;
    Int32 mYear;

    Mutex mMonthUpdateLock;
    AutoPtr<ILocale> mMonthLocale;
    ArrayOf<String>* mShortMonths;
};

#endif
