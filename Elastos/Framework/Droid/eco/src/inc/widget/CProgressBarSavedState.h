
#ifndef __CPROGRESSBARSAVEDSTATE_H__
#define __CPROGRESSBARSAVEDSTATE_H__

#include "_CProgressBarSavedState.h"
#include "widget/ProgressBarSavedState.h"

CarClass(CProgressBarSavedState), public ProgressBarSavedState
{
    friend class ProgressBar;

public:
    CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ IParcelable* superState);

    CARAPI constructor(
        /* [in] */ IParcel* parcel);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPROGRESSBARSAVEDSTATE_H__
