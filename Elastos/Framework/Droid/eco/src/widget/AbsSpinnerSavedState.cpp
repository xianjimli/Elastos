
#include "ext/frameworkdef.h"
#include "widget/AbsSpinnerSavedState.h"

AbsSpinnerSavedState::AbsSpinnerSavedState()
    : mSelectedId(-1)
    , mPosition(0)
{
}

AbsSpinnerSavedState::AbsSpinnerSavedState(
    /* [in] */ IParcelable* superState)
    : ViewBaseSavedState(superState)
    , mSelectedId(-1)
    , mPosition(0)
{
}

AbsSpinnerSavedState::AbsSpinnerSavedState(
    /* [in] */ IParcel* source)
    : ViewBaseSavedState(source)
{
    source->ReadInt64(&mSelectedId);
    source->ReadInt32(&mPosition);
}

ECode AbsSpinnerSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    ViewBaseSavedState::WriteToParcel(dest);

    dest->WriteInt64(mSelectedId);
    dest->WriteInt32(mPosition);

    return NOERROR;
}

ECode AbsSpinnerSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    ViewBaseSavedState::ReadFromParcel(source);

    source->ReadInt64(&mSelectedId);
    source->ReadInt32(&mPosition);

    return NOERROR;
}

ECode AbsSpinnerSavedState::Init(
    /* [in] */ IParcelable* superState)
{
    return ViewBaseSavedState::Init(superState);
}

ECode AbsSpinnerSavedState::Init(
    /* [in] */ IParcel* source)
{
    ASSERT_SUCCEEDED(ViewBaseSavedState::Init(source));

    source->ReadInt64(&mSelectedId);
    source->ReadInt32(&mPosition);

    return NOERROR;
}
