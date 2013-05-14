
#ifndef __CDATEPICKERSAVEDSTATE_H__
#define __CDATEPICKERSAVEDSTATE_H__

#include "_CDatePickerSavedState.h"
#include "view/ViewBaseSavedState.h"

CarClass(CDatePickerSavedState), public ViewBaseSavedState
{
public:
    CDatePickerSavedState();

    CARAPI_(Int32) GetYear();

    CARAPI_(Int32) GetMonth();

    CARAPI_(Int32) GetDay();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);

    CARAPI constructor(
        /* [in] */ IParcelable* superState,
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day);

private:
    Int32 mYear;
    Int32 mMonth;
    Int32 mDay;
};

#endif // __CDATEPICKERSAVEDSTATE_H__
