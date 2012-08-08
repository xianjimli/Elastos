
#ifndef __PROGRESSBARSAVEDSTATE_H__
#define __PROGRESSBARSAVEDSTATE_H__

#include "view/ViewBaseSavedState.h"

class ProgressBarSavedState : public ViewBaseSavedState
{
public:
    /**
     * Constructor called from {@link ProgressBar#onSaveInstanceState()}
     */
    ProgressBarSavedState(
        /* [in] */ IParcelable* superState);

    /**
     * Constructor called from {@link #CREATOR}
     */
    ProgressBarSavedState(
        /* [in] */ IParcel* in);

    virtual CARAPI WriteToParcel(
        /* [in] */ IParcel* out);

    /*public static final Parcelable.Creator<SavedState> CREATOR
            = new Parcelable.Creator<SavedState>() {
        public SavedState createFromParcel(Parcel in) {
            return new SavedState(in);
        }

        public SavedState[] newArray(Int32 size) {
            return new SavedState[size];
        }
    };*/

protected:
    ProgressBarSavedState();

public:
    Int32 mProgress;
    Int32 mSecondaryProgress;
};

#endif
