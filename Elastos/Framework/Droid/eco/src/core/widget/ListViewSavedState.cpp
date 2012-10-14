
#include "widget/ListViewSavedState.h"

ListViewSavedState::ListViewSavedState()
    : mCheckStates(NULL)
    , mCheckIdStates(NULL)
{
}

ListViewSavedState::ListViewSavedState(
    /* [in] */ IParcelable* superState,
    /* [in] */ HashMap<Int32, Boolean>* checkState,
    /* [in] */ HashMap<Int64, Boolean>* checkIdState)
    : ViewBaseSavedState(superState)
    , mCheckStates(checkState)
    , mCheckIdStates(checkIdState)
{
}

ListViewSavedState::ListViewSavedState(
    /* [in] */ IParcel* source)
    : ViewBaseSavedState(source)
{
    //checkState = in.readSparseBooleanArray();
    //Int64[] idState = in.createLongArray();

    //if (idState.length > 0) {
    //    checkIdState = new LongSparseArray<Boolean>();
    //    checkIdState.setValues(idState, Boolean.TRUE);
    //}
}

ECode ListViewSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    ViewBaseSavedState::WriteToParcel(dest);

    //out.writeSparseBooleanArray(checkState);
    //out.writeLongArray(checkIdState != NULL ? checkIdState.GetKeys() : new Int64[0]);

    return NOERROR;
}

ECode ListViewSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    ViewBaseSavedState::ReadFromParcel(source);

    //checkState = in.readSparseBooleanArray();
    //Int64[] idState = in.createLongArray();

    //if (idState.length > 0) {
    //    checkIdState = new LongSparseArray<Boolean>();
    //    checkIdState.setValues(idState, Boolean.TRUE);
    //}

    return NOERROR;
}

ECode ListViewSavedState::Init(
    /* [in] */ IParcelable* superState,
    /* [in] */ HashMap<Int32, Boolean>* checkState,
    /* [in] */ HashMap<Int64, Boolean>* checkIdState)
{
    FAIL_RETURN(ViewBaseSavedState::Init(superState));

    mCheckStates = checkState;
    mCheckIdStates = checkIdState;

    return NOERROR;
}

ECode ListViewSavedState::Init(
    /* [in] */ IParcel* source)
{
    FAIL_RETURN(ViewBaseSavedState::Init(source));

    //checkState = in.readSparseBooleanArray();
    //Int64[] idState = in.createLongArray();

    //if (idState.length > 0) {
    //    checkIdState = new LongSparseArray<Boolean>();
    //    checkIdState.setValues(idState, Boolean.TRUE);
    //}

    return NOERROR;
}
