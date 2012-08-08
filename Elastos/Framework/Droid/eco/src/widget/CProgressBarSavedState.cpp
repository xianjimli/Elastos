
#include "ext/frameworkdef.h"
#include "widget/CProgressBarSavedState.h"

ECode CProgressBarSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    VALIDATE_NOT_NULL(superState);

    AutoPtr<IParcelable> temp = ProgressBarSavedState::GetSuperState();
    *superState = temp;
    if (*superState) {
        (*superState)->AddRef();
    }

    return NOERROR;
}

ECode CProgressBarSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return ProgressBarSavedState::ReadFromParcel(source);
}

ECode CProgressBarSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return ProgressBarSavedState::WriteToParcel(dest);
}

ECode CProgressBarSavedState::constructor(
    /* [in] */ IParcelable* superState)
{
    return ViewBaseSavedState::Init(superState);
}

ECode CProgressBarSavedState::constructor(
    /* [in] */ IParcel* parcel)
{
    return ViewBaseSavedState::Init(parcel);
}

