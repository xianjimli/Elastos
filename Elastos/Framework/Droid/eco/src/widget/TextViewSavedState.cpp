
#include "widget/TextViewSavedState.h"

TextViewSavedState::TextViewSavedState()
    : mSelStart(-1)
    , mSelEnd(-1)
    , mFrozenWithFocus(FALSE)
{
}

TextViewSavedState::TextViewSavedState(
    /* [in] */ IParcelable* superState)
    : ViewBaseSavedState(superState)
    , mSelStart(-1)
    , mSelEnd(-1)
    , mFrozenWithFocus(FALSE)
{
}

TextViewSavedState::TextViewSavedState(
    /* [in] */ IParcel* source)
    : ViewBaseSavedState(source)
{
    source->ReadInt32(&mSelStart);
    source->ReadInt32(&mSelEnd);
    Int32 data;
    source->ReadInt32(&data);
    mFrozenWithFocus = data != 0;
    // text = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(source);

    // if (source.readInt() != 0) {
    //    error = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(source);
    // }
}

ECode TextViewSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    ViewBaseSavedState::WriteToParcel(dest);

    dest->WriteInt32(mSelStart);
    dest->WriteInt32(mSelEnd);
    dest->WriteInt32(mFrozenWithFocus ? 1 : 0);
    // TextUtils.writeToParcel(text, out, flags);

    // if (error == NULL) {
    //    out.writeInt(0);
    // } else {
    //    out.writeInt(1);
    //    TextUtils.writeToParcel(error, out, flags);
    // }

    return NOERROR;
}

ECode TextViewSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    ViewBaseSavedState::ReadFromParcel(source);

    source->ReadInt32(&mSelStart);
    source->ReadInt32(&mSelEnd);
    Int32 data;
    source->ReadInt32(&data);
    mFrozenWithFocus = data != 0;
    // text = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(source);

    // if (source.readInt() != 0) {
    //    error = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(source);
    // }

    return NOERROR;
}

ECode TextViewSavedState::Init(
    /* [in] */ IParcelable* superState)
{
    return ViewBaseSavedState::Init(superState);
}

ECode TextViewSavedState::Init(
    /* [in] */ IParcel* source)
{
    ViewBaseSavedState::Init(source);

    source->ReadInt32(&mSelStart);
    source->ReadInt32(&mSelEnd);
    Int32 data;
    source->ReadInt32(&data);
    mFrozenWithFocus = data != 0;
    // text = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(source);

    // if (source.readInt() != 0) {
    //    error = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(source);
    // }

    return NOERROR;
}
