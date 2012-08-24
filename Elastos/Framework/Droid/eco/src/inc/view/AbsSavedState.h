
#ifndef __ABSSAVEDSTATE_H__
#define __ABSSAVEDSTATE_H__

#include <elastos.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

class AbsSavedState
{
public:
    AbsSavedState();

protected:
    AbsSavedState(
        /* [in] */ IParcelable* superState);

    AbsSavedState(
        /* [in] */ IParcel* source);

public:
    CARAPI_(AutoPtr<IParcelable>) GetSuperState();

    virtual CARAPI_(Int32) DescribeContents();

    virtual CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    virtual CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

protected:
    CARAPI Init(
        /* [in] */ IParcelable* superState);

    CARAPI Init(
        /* [in] */ IParcel* source);

    //public static final Parcelable.Creator<AbsSavedState> CREATOR 
    //    = new Parcelable.Creator<AbsSavedState>() {
    //    
    //    public AbsSavedState createFromParcel(Parcel in) {
    //        Parcelable superState = in.readParcelable(null);
    //        if (superState != null) {
    //            throw new IllegalStateException("superState must be null");
    //        }
    //        return EMPTY_STATE;
    //    }

    //    public AbsSavedState[] newArray(int size) {
    //        return new AbsSavedState[size];
    //    }
    //};
public:
    //static const AbsSavedState* EMPTY_STATE;

private:
    AutoPtr<IParcelable> mSuperState;
};
#endif //__ABSSAVEDSTATE_H__
