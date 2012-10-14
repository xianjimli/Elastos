
#include "widget/CTextViewSavedState.h"

ECode CTextViewSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return TextViewSavedState::ReadFromParcel(source);
}

ECode CTextViewSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return TextViewSavedState::WriteToParcel(dest);
}

ECode CTextViewSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    VALIDATE_NOT_NULL(superState);
    AutoPtr<IParcelable> temp = TextViewSavedState::GetSuperState();
    *superState = temp;
    if (*superState) {
        (*superState)->AddRef();
    }

    return NOERROR;
}

ECode CTextViewSavedState::constructor(
    /* [in] */ IParcelable* superState)
{
    return TextViewSavedState::Init(superState);
}

ECode CTextViewSavedState::constructor(
    /* [in] */ IParcel* source)
{
    return TextViewSavedState::Init(source);
}

