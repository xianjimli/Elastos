
#include "widget/CAbsSpinnerSavedState.h"
#include "ext/frameworkdef.h"

ECode CAbsSpinnerSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return AbsSpinnerSavedState::ReadFromParcel(source);
}

ECode CAbsSpinnerSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return AbsSpinnerSavedState::WriteToParcel(dest);
}

ECode CAbsSpinnerSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    VALIDATE_NOT_NULL(superState);
    AutoPtr<IParcelable> temp = AbsSpinnerSavedState::GetSuperState();
    *superState = temp;
    if (*superState) {
        (*superState)->AddRef();
    }

    return NOERROR;
}

ECode CAbsSpinnerSavedState::constructor(
    /* [in] */ IParcelable* superState)
{
    return AbsSpinnerSavedState::Init(superState);
}

ECode CAbsSpinnerSavedState::constructor(
    /* [in] */ IParcel* source)
{
    return AbsSpinnerSavedState::Init(source);
}

