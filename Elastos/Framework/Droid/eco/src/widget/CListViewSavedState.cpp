
#include "widget/CListViewSavedState.h"
#include "ext/frameworkdef.h"

ECode CListViewSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return ListViewSavedState::ReadFromParcel(source);
}

ECode CListViewSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return ListViewSavedState::WriteToParcel(dest);
}

ECode CListViewSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    VALIDATE_NOT_NULL(superState);
    AutoPtr<IParcelable> temp = ListViewSavedState::GetSuperState();
    *superState = temp;
    if (*superState) {
        (*superState)->AddRef();
    }

    return NOERROR;
}

ECode CListViewSavedState::constructor(
    /* [in] */ IParcelable* superState)
{
    //TODO:
    return ListViewSavedState::Init(superState, NULL, NULL);
}

ECode CListViewSavedState::constructor(
    /* [in] */ IParcel* source)
{
    return ListViewSavedState::Init(source);
}

