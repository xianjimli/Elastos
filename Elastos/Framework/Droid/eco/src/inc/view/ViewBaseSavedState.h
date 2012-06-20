
#ifndef __VIEWBASESAVEDSTATE_H__
#define __VIEWBASESAVEDSTATE_H__

#include "view/AbsSavedState.h"

class ViewBaseSavedState : public AbsSavedState
{
public:
    ViewBaseSavedState();

protected:
    ViewBaseSavedState(
        /* [in] */ IParcelable* superState);

    ViewBaseSavedState(
        /* [in] */ IParcel* source);

    //public static final Parcelable.Creator<BaseSavedState> CREATOR =
    //    new Parcelable.Creator<BaseSavedState>() {
    //        public BaseSavedState createFromParcel(Parcel in) {
    //            return new BaseSavedState(in);
    //        }

    //        public BaseSavedState[] newArray(int size) {
    //            return new BaseSavedState[size];
    //        }
    //};
};
#endif //__VIEWBASESAVEDSTATE_H__
