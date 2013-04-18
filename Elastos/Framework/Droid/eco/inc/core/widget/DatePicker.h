
#ifndef __DATEPICKER_H__
#define __DATEPICKER_H__

#include "widget/FrameLayout.h"
#include "utils/SparseArray.h"
#include "view/ViewBaseSavedState.h"

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
public:
    DatePicker();

    DatePicker(
        /* [in] */ IContext* context);
    
    DatePicker(
        /* [in] */ IContext* context, 
        /* [in] */ IAttributeSet* attrs);

    DatePicker(
        /* [in] */ IContext* context, 
        /* [in] */ IAttributeSet* attrs, 
        /* [in] */ Int32 defStyle);
    
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
    virtual CARAPI_(void) DispatchRestoreInstanceState(
        /* [in] */ SparseArray* container);

    virtual CARAPI_(AutoPtr<IParcelable>) OnSaveInstanceState();

    virtual CARAPI_(void) OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

private:
    CARAPI_(void) Init(
        /* [in] */ IContext* context, 
        /* [in] */ IAttributeSet* attrs = NULL, 
        /* [in] */ Int32 defStyle = 0);

    CARAPI_(void) ReorderPickers(
        /* [in] */ ArrayOf<String>* months);

    CARAPI_(ArrayOf<String>*) GetShortMonths();

    class DatePickerSavedState : public ViewBaseSavedState
    {
    public:
        virtual CARAPI_(Int32) GetYear();

        virtual CARAPI_(Int32) GetMonth();

        virtual CARAPI_(Int32) GetDay();

        virtual CARAPI WriteToParcel(
            /* [in] */ IParcel* dest, 
            /* [in] */ Int32 flags);

        /*static const Parcelable.Creator<SavedState> CREATOR =
                new Creator<SavedState>() {

                    public SavedState createFromParcel(Parcel in) {
                        return new SavedState(in);
                    }

                    public SavedState[] newArray(Int32 size) {
                        return new SavedState[size];
                    }
                };*/

    private:
        /**
         * Constructor called from {@link DatePicker#onSaveInstanceState()}
         */
        DatePickerSavedState(
            /* [in] */ IParcelable* superState, 
            /* [in] */ Int32 year, 
            /* [in] */ Int32 month, 
            /* [in] */ Int32 day);
        
        /**
         * Constructor called from {@link #CREATOR}
         */
        DatePickerSavedState(
            /* [in] */ IParcel* in);

        Int32 mYear;
        Int32 mMonth;
        Int32 mDay;
    };

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
    /*NumberPicker mDayPicker;
    NumberPicker mMonthPicker;
    NumberPicker mYearPicker;*/

    /**
     * How we notify users the date has changed.
     */
    AutoPtr<IOnDateChangedListener> mOnDateChangedListener;
    
    Int32 mDay;
    Int32 mMonth;
    Int32 mYear;

    AutoPtr<IInterface> mMonthUpdateLock;// = new Object();
    AutoPtr<ILocale> mMonthLocale;
    ArrayOf<String>* mShortMonths;
};

#endif
