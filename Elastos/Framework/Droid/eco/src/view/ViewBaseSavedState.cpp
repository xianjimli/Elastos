
#include "view/ViewBaseSavedState.h"

ViewBaseSavedState::ViewBaseSavedState()
{
}

ViewBaseSavedState::ViewBaseSavedState(
    /* [in] */ IParcelable* superState)
    : AbsSavedState(superState)
{
}

ViewBaseSavedState::ViewBaseSavedState(
    /* [in] */ IParcel* source)
    : AbsSavedState(source)
{
}
