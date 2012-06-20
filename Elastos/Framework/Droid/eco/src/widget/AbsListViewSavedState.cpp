
#include "widget/AbsListViewSavedState.h"

AbsListViewSavedState::AbsListViewSavedState()
    : mSelectedId(-1)
    , mFirstId(-1)
    , mViewTop(0)
    , mPosition(0)
    , mHeight(0)
{
}

AbsListViewSavedState::AbsListViewSavedState(
    /* [in] */ IParcelable* superState)
    : ViewBaseSavedState(superState)
    , mSelectedId(-1)
    , mFirstId(-1)
    , mViewTop(0)
    , mPosition(0)
    , mHeight(0)
{
}

AbsListViewSavedState::AbsListViewSavedState(
    /* [in] */ IParcel* source)
    : ViewBaseSavedState(source)
{
    source->ReadInt64(&mSelectedId);
    source->ReadInt64(&mFirstId);
    source->ReadInt32(&mViewTop);
    source->ReadInt32(&mPosition);
    source->ReadInt32(&mHeight);
    source->ReadString(&mFilter);
}

ECode AbsListViewSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    ViewBaseSavedState::WriteToParcel(dest);

    dest->WriteInt64(mSelectedId);
    dest->WriteInt64(mFirstId);
    dest->WriteInt32(mViewTop);
    dest->WriteInt32(mPosition);
    dest->WriteInt32(mHeight);
    dest->WriteString(mFilter);

    return NOERROR;
}

ECode AbsListViewSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    ViewBaseSavedState::ReadFromParcel(source);

    source->ReadInt64(&mSelectedId);
    source->ReadInt64(&mFirstId);
    source->ReadInt32(&mViewTop);
    source->ReadInt32(&mPosition);
    source->ReadInt32(&mHeight);
    source->ReadString(&mFilter);

    return NOERROR;
}

ECode AbsListViewSavedState::Init(
    /* [in] */ IParcelable* superState)
{
    return ViewBaseSavedState::Init(superState);
}

ECode AbsListViewSavedState::Init(
    /* [in] */ IParcel* source)
{
    ViewBaseSavedState::Init(source);

    source->ReadInt64(&mSelectedId);
    source->ReadInt64(&mFirstId);
    source->ReadInt32(&mViewTop);
    source->ReadInt32(&mPosition);
    source->ReadInt32(&mHeight);
    source->ReadString(&mFilter);

    return NOERROR;
}
