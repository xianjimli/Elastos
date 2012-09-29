
#ifndef __TEXTVIEWSAVEDSTATE_H__
#define __TEXTVIEWSAVEDSTATE_H__

#include "ext/frameworkext.h"
#include "view/ViewBaseSavedState.h"
#include <elastos/AutoPtr.h>

class TextViewSavedState : public ViewBaseSavedState
{
public:
    TextViewSavedState();

protected:
    TextViewSavedState(
        /* [in] */ IParcelable* superState);

    TextViewSavedState(
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

    //    @Override
    //    public String toString() {
    //        String str = "TextView.SavedState{"
    //                + Integer.toHexString(System.identityHashCode(this))
    //                + " start=" + selStart + " end=" + selEnd;
    //        if (text != NULL) {
    //            str += " text=" + text;
    //        }
    //        return str + "}";
    //    }

    //    @SuppressWarnings("hiding")
    //    public static final Parcelable.Creator<SavedState> CREATOR
    //            = new Parcelable.Creator<SavedState>() {
    //        public SavedState createFromParcel(Parcel in) {
    //            return new SavedState(in);
    //        }

    //        public SavedState[] newArray(Int32 size) {
    //            return new SavedState[size];
    //        }
    //    };

protected:
    Int32 mSelStart;
    Int32 mSelEnd;
    AutoPtr<ICharSequence> mText;
    Boolean mFrozenWithFocus;
    AutoPtr<ICharSequence> mError;
};
#endif //__TEXTVIEWSAVEDSTATE_H__
