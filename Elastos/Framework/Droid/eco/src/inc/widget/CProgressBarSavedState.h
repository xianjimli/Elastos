
#ifndef __CPROGRESSBARSAVEDSTATE_H__
#define __CPROGRESSBARSAVEDSTATE_H__

#include "_CProgressBarSavedState.h"
#include "widget/ProgressBarSavedState.h"

CarClass(CProgressBarSavedState), public ProgressBarSavedState
{
    friend class ProgressBar;
public:
    CARAPI GetSuperState(
        /* [out] */ IParcelable ** ppSuperState);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel * pSource);

    CARAPI WriteToParcel(
        /* [in] */ IParcel * pDest);

    CARAPI constructor(
        /* [in] */ IParcelable * pSuperState);

    CARAPI constructor(
        /* [in] */ IParcel * pParcel);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPROGRESSBARSAVEDSTATE_H__
