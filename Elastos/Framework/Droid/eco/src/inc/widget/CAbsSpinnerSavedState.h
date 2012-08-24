
#ifndef __CABSSPINNERSAVEDSTATE_H__
#define __CABSSPINNERSAVEDSTATE_H__

#include "_CAbsSpinnerSavedState.h"
#include "widget/AbsSpinnerSavedState.h"

CarClass(CAbsSpinnerSavedState), public AbsSpinnerSavedState
{
    friend class AbsSpanner;

public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);

    CARAPI constructor(
        /* [in] */ IParcelable* superState);

    CARAPI constructor(
        /* [in] */ IParcel* source);

private:
    // TODO: Add your private member variables here.
};

#endif // __CABSSPANNERWSAVEDSTATE_H__
