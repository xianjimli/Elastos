
#include "widget/CProgressBarSavedState.h"

ECode CProgressBarSavedState::GetSuperState(
    /* [out] */ IParcelable ** superState)
{
    if (!superState) {
        return E_INVALID_ARGUMENT;
    }

    AutoPtr<IParcelable> temp = ProgressBarSavedState::GetSuperState();
    *superState = temp;
    if (*superState) {
        (*superState)->AddRef();
    }

    return NOERROR;
}

ECode CProgressBarSavedState::ReadFromParcel(
    /* [in] */ IParcel * pSource)
{
    return ProgressBarSavedState::ReadFromParcel(pSource);
}

ECode CProgressBarSavedState::WriteToParcel(
    /* [in] */ IParcel * pDest)
{
    return ProgressBarSavedState::WriteToParcel(pDest);
}

ECode CProgressBarSavedState::constructor(
    /* [in] */ IParcelable * pSuperState)
{
    return ViewBaseSavedState::Init(pSuperState);
}

ECode CProgressBarSavedState::constructor(
    /* [in] */ IParcel * pParcel)
{
    return ViewBaseSavedState::Init(pParcel);
}

