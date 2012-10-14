
#ifndef __CTEXTVIEWSAVEDSTATE_H__
#define __CTEXTVIEWSAVEDSTATE_H__

#include "_CTextViewSavedState.h"
#include "widget/TextViewSavedState.h"

CarClass(CTextViewSavedState), public TextViewSavedState
{
    friend class TextView;
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

#endif // __CTEXTVIEWSAVEDSTATE_H__
