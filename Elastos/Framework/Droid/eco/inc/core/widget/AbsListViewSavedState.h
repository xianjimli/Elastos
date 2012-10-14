
#ifndef __ABSLISTVIEWSAVEDSTATE_H__
#define __ABSLISTVIEWSAVEDSTATE_H__

#include "view/ViewBaseSavedState.h"

class AbsListViewSavedState : public ViewBaseSavedState
{
public:
    AbsListViewSavedState();

protected:
    AbsListViewSavedState(
        /* [in] */ IParcelable* superState);

    AbsListViewSavedState(
        /* [in] */ IParcel* source);

public:
    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

protected:
    CARAPI Init(
        /* [in] */ IParcelable* superState);

    CARAPI Init(
        /* [in] */ IParcel* source);

    //@Override
    //    public String toString() {
    //        return "AbsListView.SavedState{"
    //            + Integer.toHexString(System.identityHashCode(this))
    //            + " selectedId=" + selectedId
    //            + " firstId=" + firstId
    //            + " viewTop=" + viewTop
    //            + " position=" + position
    //            + " height=" + height
    //            + " filter=" + filter + "}";
    //}

    //public static final Parcelable.Creator<SavedState> CREATOR
    //    = new Parcelable.Creator<SavedState>() {
    //        public SavedState createFromParcel(Parcel in) {
    //            return new SavedState(in);
    //        }

    //        public SavedState[] newArray(int size) {
    //            return new SavedState[size];
    //        }
    //};

protected:
    Int64 mSelectedId;
    Int64 mFirstId;
    Int32 mViewTop;
    Int32 mPosition;
    Int32 mHeight;
    String mFilter;
};
#endif //__ABSLISTVIEWSAVEDSTATE_H__
