
#ifndef __CABSLISTVIEWSAVEDSTATE_H__
#define __CABSLISTVIEWSAVEDSTATE_H__

#include "_CAbsListViewSavedState.h"
#include "widget/AbsListViewSavedState.h"

CarClass(CAbsListViewSavedState), public AbsListViewSavedState
{
    friend class AbsListView;
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

#endif // __CABSLISTVIEWSAVEDSTATE_H__
