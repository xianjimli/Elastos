
#include "widget/ProgressBarSavedState.h"

ProgressBarSavedState::ProgressBarSavedState()
{

}

ProgressBarSavedState::ProgressBarSavedState(
    /* [in] */ IParcelable* superState) : ViewBaseSavedState(superState)

{

}
    
ProgressBarSavedState::ProgressBarSavedState(
    /* [in] */ IParcel* in) : ViewBaseSavedState(in)
{
    in->ReadInt32(&progress);
    in->ReadInt32(&secondaryProgress);
}

ECode ProgressBarSavedState::WriteToParcel(
    /* [in] */ IParcel* out)
{
    ViewBaseSavedState::WriteToParcel(out);
    out->WriteInt32(progress);
    out->WriteInt32(secondaryProgress);

    return NOERROR;
}
