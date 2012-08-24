
#include "widget/CAbsListViewSavedState.h"
#include "ext/frameworkdef.h"

ECode CAbsListViewSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return AbsListViewSavedState::ReadFromParcel(source);
}

ECode CAbsListViewSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return AbsListViewSavedState::WriteToParcel(dest);
}

ECode CAbsListViewSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    VALIDATE_NOT_NULL(superState);
    AutoPtr<IParcelable> temp = AbsListViewSavedState::GetSuperState();
    *superState = temp;
    if (*superState) {
        (*superState)->AddRef();
    }

    return NOERROR;
}

ECode CAbsListViewSavedState::constructor(
    /* [in] */ IParcelable* superState)
{
    return AbsListViewSavedState::Init(superState);
}

ECode CAbsListViewSavedState::constructor(
    /* [in] */ IParcel* source)
{
    return AbsListViewSavedState::Init(source);
}

