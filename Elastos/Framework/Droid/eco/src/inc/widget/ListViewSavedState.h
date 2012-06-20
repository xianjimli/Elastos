
#ifndef __LISTVIEWSAVEDSTATE_H__
#define __LISTVIEWSAVEDSTATE_H__

#include "view/ViewBaseSavedState.h"
#include "ext/frameworkext.h"
#include <elastos/HashMap.h>

class ListViewSavedState : public ViewBaseSavedState
{
public:
    ListViewSavedState();

protected:
    ListViewSavedState(
        /* [in] */ IParcelable* superState,
        /* [in] */ HashMap<Int32, Boolean>* checkState,
        /* [in] */ HashMap<Int64, Boolean>* checkIdState);

private:
    ListViewSavedState(
        /* [in] */ IParcel* source);

public:
    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

protected:
    CARAPI Init(
        /* [in] */ IParcelable* superState,
        /* [in] */ HashMap<Int32, Boolean>* checkState,
        /* [in] */ HashMap<Int64, Boolean>* checkIdState);

    CARAPI Init(
        /* [in] */ IParcel* source);

    //@Override
    //    public String toString() {
    //        return "ListView.SavedState{"
    //            + Integer.toHexString(System.identityHashCode(this))
    //            + " checkState=" + checkState + "}";
    //}

    //public static const Parcelable.Creator<SavedState> CREATOR
    //    = new Parcelable.Creator<SavedState>() {
    //        public SavedState createFromParcel(Parcel in) {
    //            return new SavedState(in);
    //        }

    //        public SavedState[] newArray(Int32 size) {
    //            return new SavedState[size];
    //        }
    //};

protected:
    HashMap<Int32, Boolean>* mCheckStates;
    HashMap<Int64, Boolean>* mCheckIdStates;
};
#endif //__LISTVIEWSAVEDSTATE_H__
