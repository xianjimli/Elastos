
#ifndef __CSYNCRESULT_H__
#define __CSYNCRESULT_H__

#include "_CSyncResult.h"
#include "CSyncStats.h"
#include <ext/frameworkdef.h>
#include <elastos/AutoPtr.h>

CarClass(CSyncResult)
{
public:
    CSyncResult();
    CSyncResult(Boolean);

    static AutoPtr<CSyncResult> CreateSyncResult();

public:
    CARAPI HasHardError(
        /* [out] */ Boolean* hasError);

    CARAPI HasSoftError(
        /* [out] */ Boolean* hasError);

    CARAPI HasError(
        /* [out] */ Boolean* hasError);

    CARAPI MadeSomeProgress(
        /* [out] */ Boolean* result);

    CARAPI DescribeContents(
        /* [out] */ Int32* result);

    CARAPI Clear();

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI ToDebugString(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Boolean syncAlreadyInProgress);

    CARAPI constructor(
        /* [in] */ IParcel* parcel);

public:
    /**
     * Used to indicate that the SyncAdapter is already performing a sync operation, though
     * not necessarily for the requested account and authority and that it wasn't able to
     * process this request. The SyncManager will reschedule the request to run later.
     */
    /*const*/ Boolean syncAlreadyInProgress;

    /**
     * Used to indicate that the SyncAdapter determined that it would need to issue
     * too many delete operations to the server in order to satisfy the request
     * (as defined by the SyncAdapter). The SyncManager will record
     * that the sync request failed and will cause a System Notification to be created
     * asking the user what they want to do about this. It will give the user a chance to
     * choose between (1) go ahead even with those deletes, (2) revert the deletes,
     * or (3) take no action. If the user decides (1) or (2) the SyncManager will issue another
     * sync request with either {@link ContentResolver#SYNC_EXTRAS_OVERRIDE_TOO_MANY_DELETIONS}
     * or {@link ContentResolver#SYNC_EXTRAS_DISCARD_LOCAL_DELETIONS} set in the extras.
     * It is then up to the SyncAdapter to decide how to honor that request.
     */
    Boolean tooManyDeletions;

    /**
     * Used to indicate that the SyncAdapter experienced a hard error due to trying the same
     * operation too many times (as defined by the SyncAdapter). The SyncManager will record
     * that the sync request failed and it will not reschedule the request.
     */
   Boolean tooManyRetries;

    /**
     * Used to indicate that the SyncAdapter experienced a hard error due to an error it
     * received from interacting with the storage later. The SyncManager will record that
     * the sync request failed and it will not reschedule the request.
     */
    Boolean databaseError;

    /**
     * If set the SyncManager will request an immediate sync with the same Account and authority
     * (but empty extras Bundle) as was used in the sync request.
     */
    Boolean fullSyncRequested;

    /**
     * This field is ignored by the SyncManager.
     */
    Boolean partialSyncUnavailable;

    /**
     * This field is ignored by the SyncManager.
     */
    Boolean moreRecordsToGet;

    /**
     * Used to indicate to the SyncManager that future sync requests that match the request's
     * Account and authority should be delayed at least this many seconds.
     */
    Int64 delayUntil;

    /**
     * Used to hold extras statistics about the sync operation. Some of these indicate that
     * the sync request resulted in a hard or soft error, others are for purely informational
     * purposes.
     */
    /*const*/ AutoPtr<CSyncStats> stats;

    /**
     * This instance of a SyncResult is returned by the SyncAdapter in response to a
     * sync request when a sync is already underway. The SyncManager will reschedule the
     * sync request to try again later.
     */
    const static AutoPtr<CSyncResult> ALREADY_IN_PROGRESS;

private:
    // TODO: Add your private member variables here.
};

#endif // __CSYNCRESULT_H__
