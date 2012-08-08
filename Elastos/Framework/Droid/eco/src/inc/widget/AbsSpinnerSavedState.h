
#ifndef __ABSSPINNERSAVEDSTATE_H__
#define __ABSSPINNERSAVEDSTATE_H__

#include "view/ViewBaseSavedState.h"

class AbsSpinnerSavedState : public ViewBaseSavedState
{
public:
    AbsSpinnerSavedState(
        /* [in] */ IParcelable* superState);

    AbsSpinnerSavedState(
        /* [in] */ IParcel* source);

public:
    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    /*String toString() {
        return "AbsSpinner.SavedState{"
            + Integer.toHexString(System.identityHashCode(this))
            + " selectedId=" + selectedId
            + " position=" + position + "}";
    }*/

    /*public static final Parcelable.Creator<SavedState> CREATOR
        = new Parcelable.Creator<SavedState>() {
            public SavedState createFromParcel(Parcel in) {
                return new SavedState(in);
            }

            public SavedState[] newArray(int size) {
                return new SavedState[size];
            }
    };*/

protected:
    AbsSpinnerSavedState();

    CARAPI Init(
        /* [in] */ IParcelable* superState);

    CARAPI Init(
        /* [in] */ IParcel* source);

public:
    Int64 mSelectedId;
    Int32 mPosition;
};

#endif //__ABSSPINNERSAVEDSTATE_H__
