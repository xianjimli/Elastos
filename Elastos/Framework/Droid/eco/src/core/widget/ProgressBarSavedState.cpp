
#include "widget/ProgressBarSavedState.h"

ProgressBarSavedState::ProgressBarSavedState()
{}

ProgressBarSavedState::ProgressBarSavedState(
    /* [in] */ IParcelable* superState)
    : ViewBaseSavedState(superState)
{}

ProgressBarSavedState::ProgressBarSavedState(
    /* [in] */ IParcel* in)
    : ViewBaseSavedState(in)
{
    in->ReadInt32(&mProgress);
    in->ReadInt32(&mSecondaryProgress);
}

ECode ProgressBarSavedState::WriteToParcel(
    /* [in] */ IParcel* out)
{
    ViewBaseSavedState::WriteToParcel(out);
    out->WriteInt32(mProgress);
    out->WriteInt32(mSecondaryProgress);

    return NOERROR;
}
