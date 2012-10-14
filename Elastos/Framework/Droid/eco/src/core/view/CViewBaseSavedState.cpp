
#include "view/CViewBaseSavedState.h"
#include "ext/frameworkdef.h"

ECode CViewBaseSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return ViewBaseSavedState::ReadFromParcel(source);
}

ECode CViewBaseSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return ViewBaseSavedState::WriteToParcel(dest);
}

ECode CViewBaseSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    VALIDATE_NOT_NULL(superState);
    AutoPtr<IParcelable> temp = ViewBaseSavedState::GetSuperState();
    *superState = temp;
    if (*superState) {
        (*superState)->AddRef();
    }

    return NOERROR;
}

ECode CViewBaseSavedState::constructor(
    /* [in] */ IParcelable* superState)
{
    return ViewBaseSavedState::Init(superState);
}

ECode CViewBaseSavedState::constructor(
    /* [in] */ IParcel* source)
{
    return ViewBaseSavedState::Init(source);
}
