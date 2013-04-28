
#include <StringBuffer.h>
#include "content/CSyncResult.h"
#include "content/CSyncStats.h"

const AutoPtr<CSyncResult> CSyncResult::ALREADY_IN_PROGRESS = CSyncResult::CreateSyncResult();

AutoPtr<CSyncResult> CSyncResult::CreateSyncResult()
{
    AutoPtr<CSyncResult> syncResult;

    CSyncResult::NewByFriend((CSyncResult**) &syncResult);

    return syncResult;
}


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
    *hasError = (stats->numParseExceptions > 0) ||
                stats->numConflictDetectedExceptions > 0 ||
                stats->numAuthExceptions > 0 ||
                tooManyDeletions ||
                tooManyRetries ||
                databaseError;

    return NOERROR;
}

ECode CSyncResult::HasSoftError(
    /* [out] */ Boolean* hasError)
{
    CSyncStats* syncStats = (CSyncStats*)stats;

    *hasError = syncAlreadyInProgress ||
               syncStats->numIoExceptions > 0;

    return NOERROR;
}

ECode CSyncResult::HasError(
    /* [out] */ Boolean* hasError)
{
    Boolean hardError = FALSE;
    Boolean softError = FALSE;

    HasSoftError(&softError);
    HasHardError(&hardError);

    *hasError = softError || hardError;

    return NOERROR;
}

ECode CSyncResult::MadeSomeProgress(
    /* [out] */ Boolean* result)
{
    CSyncStats* syncStats = (CSyncStats*)stats;

    *result = ((syncStats->numDeletes > 0) && !tooManyDeletions) ||
               syncStats->numInserts > 0 ||
               syncStats->numUpdates > 0;

    return NOERROR;
}

ECode CSyncResult::DescribeContents(
    /* [out] */ Int32* result)
{
    *result = 0;
    return NOERROR;
}

ECode CSyncResult::Clear()
{
    if (syncAlreadyInProgress) {
//        throw new UnsupportedOperationException(
//                "you are not allowed to clear the ALREADY_IN_PROGRESS SyncStats");
        return E_NOT_SUPPORTED;
    }

    tooManyDeletions = FALSE;
    tooManyRetries = FALSE;
    databaseError = FALSE;
    fullSyncRequested = FALSE;
    partialSyncUnavailable = FALSE;
    moreRecordsToGet = FALSE;
    delayUntil = 0;

    stats->Clear();

    return NOERROR;
}

ECode CSyncResult::ToString(
    /* [out] */ String* str)
{
    StringBuffer sb("");

    sb += "SyncResult:";

    if (syncAlreadyInProgress) {
        sb += " syncAlreadyInProgress: ";
        sb += syncAlreadyInProgress;
    }

    if (tooManyDeletions) {
        sb += " tooManyDeletions: ";
        sb += tooManyDeletions;
    }

    if (tooManyRetries) {
        sb += " tooManyRetries: ";
        sb += tooManyRetries;
    }

    if (databaseError) {
        sb += " databaseError: ";
        sb += databaseError;
    }

    if (fullSyncRequested) {
        sb += " fullSyncRequested: ";
        sb += fullSyncRequested;
    }

    if (partialSyncUnavailable) {
        sb += " partialSyncUnavailable: ";
        sb += partialSyncUnavailable;
    }

    if (moreRecordsToGet) {
        sb += " moreRecordsToGet: ";
        sb += moreRecordsToGet;
    }

    if (delayUntil > 0) {
        sb += " delayUntil: ";
        sb += delayUntil;
    }

    String statsString;

    stats->ToString(&statsString);

    sb += statsString;

    *str = String(sb);

    return NOERROR;
}

ECode CSyncResult::ToDebugString(
    /* [out] */ String* str)
{
    Boolean hasError = FALSE;
    CSyncStats* syncStats = (CSyncStats*)stats;

    StringBuffer sb("");

    if (fullSyncRequested) {
        sb += "f1";
    }

    if (partialSyncUnavailable) {
        sb += "r1";
    }

    HasHardError(&hasError);
    if (hasError) {
        sb += "X1";
    }

    if (syncStats->numParseExceptions > 0) {
        sb += "e";
        sb += syncStats->numParseExceptions;
    }
    if (syncStats->numConflictDetectedExceptions > 0) {
        sb += "c";
        sb += syncStats->numConflictDetectedExceptions;
    }
    if (syncStats->numAuthExceptions > 0) {
        sb += "a";
        sb += syncStats->numAuthExceptions;
    }
    if (tooManyDeletions) {
        sb += "D1";
    }
    if (tooManyRetries) {
        sb += "R1";
    }
    if (databaseError) {
        sb += "b1";
    }

    hasError = FALSE;
    HasSoftError(&hasError);
    if (hasError) {
        sb += "x1";
    }
    if (syncAlreadyInProgress) {
        sb += "l1";
    }
    if (syncStats->numIoExceptions > 0) {
        sb += "I";
        sb += syncStats->numIoExceptions;
    }

    *str = String(sb);

    return NOERROR;
}

ECode CSyncResult::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    Int32 result = 0;

    source->ReadInt32(&result);
    syncAlreadyInProgress = (result == 1 ? TRUE : FALSE);

    result = 0;
    source->ReadInt32(&result);
    tooManyDeletions = (result == 1 ? TRUE : FALSE);

    result = 0;
    source->ReadInt32(&result);
    tooManyRetries = (result == 1 ? TRUE : FALSE);

    result = 0;
    source->ReadInt32(&result);
    databaseError = (result == 1 ? TRUE : FALSE);

    result = 0;
    source->ReadInt32(&result);
    fullSyncRequested = (result == 1 ? TRUE : FALSE);

    result = 0;
    source->ReadInt32(&result);
    partialSyncUnavailable = (result == 1 ? TRUE : FALSE);

    result = 0;
    source->ReadInt32(&result);
    moreRecordsToGet = (result == 1 ? TRUE : FALSE);

    source->ReadInt64(&delayUntil);

    ((CSyncStats*)stats)->WriteToParcel(source);

    return NOERROR;
}

ECode CSyncResult::WriteToParcel(
    /* [in] */ IParcel* parcel)
{
    parcel->WriteInt32(syncAlreadyInProgress ? 1 : 0);
    parcel->WriteInt32(tooManyDeletions ? 1 : 0);
    parcel->WriteInt32(tooManyRetries ? 1 : 0);
    parcel->WriteInt32(databaseError ? 1 : 0);
    parcel->WriteInt32(fullSyncRequested ? 1 : 0);
    parcel->WriteInt32(partialSyncUnavailable ? 1 : 0);
    parcel->WriteInt32(moreRecordsToGet ? 1 : 0);
    parcel->WriteInt64(delayUntil);

    ((CSyncStats*)stats)->WriteToParcel(parcel);

    return NOERROR;
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
    return NOERROR;
}

