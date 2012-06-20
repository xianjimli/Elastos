
#ifndef __CLISTVIEWSAVEDSTATE_H__
#define __CLISTVIEWSAVEDSTATE_H__

#include "_CListViewSavedState.h"
#include "widget/ListViewSavedState.h"

CarClass(CListViewSavedState), public ListViewSavedState
{
    friend class ListView;
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

#endif // __CLISTVIEWSAVEDSTATE_H__
