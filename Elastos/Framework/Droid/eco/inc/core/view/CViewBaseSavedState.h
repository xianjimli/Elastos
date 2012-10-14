
#ifndef __CVIEWBASESAVEDSTATE_H__
#define __CVIEWBASESAVEDSTATE_H__

#include "_CViewBaseSavedState.h"
#include "view/ViewBaseSavedState.h"

CarClass(CViewBaseSavedState), public ViewBaseSavedState
{
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

#endif // __CVIEWBASESAVEDSTATE_H__
