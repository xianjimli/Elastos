
#include "content/CSyncResult.h"
#include "content/CSyncStats.h"

const CSyncResult* CSyncResult::ALREADY_IN_PROGRESS = new CSyncResult(true);

CSyncResult::CSyncResult()
{

}

CSyncResult::CSyncResult(
    /* [in] */ Boolean syncAlreadyInProgress )
{
    constructor(syncAlreadyInProgress);
}

ECode CSyncResult::HasHardError(
    /* [out] */ Boolean* hasError)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncResult::HasSoftError(
    /* [out] */ Boolean* hasError)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncResult::HasError(
    /* [out] */ Boolean* hasError)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncResult::MadeSomeProgress(
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncResult::DescribeContents(
    /* [out] */ Int32* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncResult::Clear()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncResult::ToString(
    /* [out] */ String* str)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncResult::ToDebugString(
    /* [out] */ String* str)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncResult::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncResult::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSyncResult::constructor()
{
    constructor(FALSE);
    return NOERROR;
}

ECode CSyncResult::constructor(
    /* [in] */ Boolean syncAlreadyInProgress)
{
    syncAlreadyInProgress = syncAlreadyInProgress;
    tooManyDeletions = FALSE;
    tooManyRetries = FALSE;
    fullSyncRequested = FALSE;
    partialSyncUnavailable = FALSE;
    moreRecordsToGet = FALSE;
    delayUntil = 0;

    CSyncStats::New((ISyncStats**)&stats);

    return NOERROR;
}

ECode CSyncResult::constructor(
    /* [in] */ IParcel* parcel)
{
    Int32 value = 0;

    parcel->ReadInt32(&value);
    syncAlreadyInProgress = (value != 0);

    parcel->ReadInt32(&value);
    tooManyDeletions = (value != 0);

    parcel->ReadInt32(&value);
    tooManyRetries = (value != 0);

    parcel->ReadInt32(&value);
    databaseError = (value != 0);

    parcel->ReadInt32(&value);
    fullSyncRequested = (value != 0);

    parcel->ReadInt32(&value);
    partialSyncUnavailable = (value != 0);

    parcel->ReadInt32(&value);
    moreRecordsToGet = (value != 0);

    delayUntil = parcel->ReadInt64(&delayUntil);

    CSyncStats::New(parcel, (ISyncStats**)&stats);
    return E_NOT_IMPLEMENTED;
}

