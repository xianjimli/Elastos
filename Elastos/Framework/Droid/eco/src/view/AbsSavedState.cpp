
#include "view/AbsSavedState.h"

//const AbsSavedState* AbsSavedState::EMPTY_STATE = new AbsSavedState();

/**
 * Constructor used to make the EMPTY_STATE singleton
 */
AbsSavedState::AbsSavedState()
{
}

/**
 * Constructor called by derived classes when creating their SavedState objects
 *
 * @param superState The state of the superclass of this view
 */
AbsSavedState::AbsSavedState(
    /* [in] */ IParcelable* superState)
{
    //if (superState == null) {
    //    throw new IllegalArgumentException("superState must not be null");
    //}
    //mSuperState = superState != EMPTY_STATE ? superState : null;
    //todo:
    if (superState != NULL) {
        mSuperState = superState;
    }
}

/**
 * Constructor used when reading from a parcel. Reads the state of the superclass.
 *
 * @param source
 */
AbsSavedState::AbsSavedState(
    /* [in] */ IParcel* source)
{
    // FIXME need class loader
    //Parcelable superState = source->ReadParcelable(null);
    //
    //mSuperState = superState != null ? superState : EMPTY_STATE;
}

AutoPtr<IParcelable> AbsSavedState::GetSuperState()
{
    return mSuperState;
}

Int32 AbsSavedState::DescribeContents()
{
    return 0;
}

ECode AbsSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    if (mSuperState != NULL) {
        mSuperState->WriteToParcel(dest);
    }
    //dest.writeParcelable(mSuperState, flags);
    return NOERROR;
}

ECode AbsSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    if (mSuperState != NULL) {
        mSuperState->ReadFromParcel(source);
    }

    return NOERROR;
}

ECode AbsSavedState::Init(
    /* [in] */ IParcelable* superState)
{
    //todo:
    assert(superState);

    if (superState != NULL) {
        mSuperState = superState;
    }

    return NOERROR;
}

ECode AbsSavedState::Init(
    /* [in] */ IParcel* source)
{
    // FIXME need class loader
    //Parcelable superState = source->ReadParcelable(null);
    //
    //mSuperState = superState != null ? superState : EMPTY_STATE;

    return NOERROR;
}
