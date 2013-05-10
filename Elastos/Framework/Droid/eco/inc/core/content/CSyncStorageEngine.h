
#ifndef __CSYNCSTORAGEENGINE_H__
#define __CSYNCSTORAGEENGINE_H__
#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include "_CSyncStorageEngine.h"
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>
#include <elastos/List.h>
#include <elastos/ElRefBase.h>
#include <elastos/etl_pair.h>
#include "utils/SparseArray.h"

CarClass(CSyncStorageEngine)
{
    class AccountInfo: public ElRefBase
    {
        public:
            AccountInfo(
                /* [in] */ IAccount* account);

            ~AccountInfo();

            CARAPI_(PInterface) Probe(
                /* [in] */ REIID riid);

            CARAPI_(UInt32) AddRef();

            CARAPI_(UInt32) Release();

            CARAPI GetInterfaceID(
                /* [in] */ IInterface *pObject,
                /* [out] */ InterfaceID *pIID);

        public:
            AutoPtr<IAccount> mAccount;
            HashMap<String, AutoPtr<ISyncStorageEngineAuthorityInfo> >* mAuthorities;

    };

public:
    CSyncStorageEngine();

    ~CSyncStorageEngine();

    CARAPI HandleMessage(
        /* [in] */ IMessage* msg);

    CARAPI AddStatusChangeListener(
        /* [in] */ Int32 mask,
        /* [in] */ ISyncStatusObserver* syncStatusObserver);

    CARAPI RemoveStatusChangeListener(
        /* [in] */ ISyncStatusObserver* syncStatusObserver);

    CARAPI GetSyncAutomatically(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [out] */ Boolean* result);

    CARAPI SetSyncAutomatically(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [in] */ Boolean sync);

    CARAPI GetIsSyncable(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [out] */ Int32* result);

    CARAPI SetIsSyncable(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [in] */ Int32 syncable);

    CARAPI SetBackoff(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [in] */ Int64 nextSyncTime,
        /* [in] */ Int64 nextDelay);

    CARAPI ClearAllBackoffs();

    CARAPI SetDelayUntilTime(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [in] */ Int64 delayUntil);

    CARAPI GetDelayUntilTime(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [out] */ Int64* delayUntilTime);

    CARAPI AddPeriodicSync(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [in] */ IBundle* extras,
        /* [in] */ Int64 pollFrequency);

    CARAPI RemovePeriodicSync(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [in] */ IBundle* extras);

    CARAPI GetPeriodicSyncs(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [out] */ IObjectContainer** periodicSyncList);

    CARAPI SetMasterSyncAutomatically(
        /* [in] */ Boolean flag);

    CARAPI GetMasterSyncAutomatically(
        /* [out] */ Boolean* result);

    CARAPI GetOrCreateAuthority(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo);

    CARAPI RemoveAuthority(
        /* [in] */ IAccount * account,
        /* [in] */ const String& authority);

    CARAPI GetAuthority(
        /* [in] */ Int32 authoriyId,
        /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo);

    CARAPI IsSyncActive(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [out] */ Boolean* isActive);

    CARAPI InsertIntoPending(
        /* [in] */ ISyncStorageEnginePendingOperation* op,
        /* [out] */ ISyncStorageEnginePendingOperation** pendingOperation);

    CARAPI DeleteFromPending(
        /* [in] */ ISyncStorageEnginePendingOperation* op,
        /* [out] */ Boolean* result);

    CARAPI ClearPending(
        /* [out] */ Int32* result);

    CARAPI GetPendingOperations(
        /* [out] */ IObjectContainer** pendingOperationList);

    CARAPI GetPendingOperationCount(
        /* [out] */ Int32* pCount);

    CARAPI DoDatabaseCleanup(
        /* [in] */ ArrayOf<IAccount *>* accounts);

    CARAPI ReportActiveChange();

    CARAPI InsertStartSyncEvent(
        /* [in] */ IAccount* accountName,
        /* [in] */ const String& authorityName,
        /* [in] */ Int64 now,
        /* [in] */ Int32 source,
        /* [out] */ Int64* result);

    CARAPI StopSyncEvent(
        /* [in] */ Int64 historyId,
        /* [in] */ Int64 elapsedTime,
        /* [in] */ const String& resultMessage,
        /* [in] */ Int64 downstreamActivity,
        /* [in] */ Int64 upstreamActivity);

    CARAPI GetCurrentSync(
        /* [out] */ ISyncInfo** syncInfo);

    CARAPI GetSyncStatus(
        /* [out] */ IObjectContainer** syncStatusInfoList);

    CARAPI GetAuthorities(
        /* [out] */ IObjectContainer** authorityInfoList);

    CARAPI GetStatusByAccountAndAuthority(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [out] */ ISyncStatusInfo** result);

    CARAPI IsSyncPending(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [out] */ Boolean* isPending);

    CARAPI GetSyncHistory(
        /* [out] */ IObjectContainer** syncHistoryItemList);

    CARAPI GetDayStatistics(
        /* [out, callee] */ ArrayOf<ISyncStorageEngineDayStats *>** dayStats);

    CARAPI GetInitialSyncFailureTime(
        /* [out] */ Int64* failureTime);

    CARAPI GetOrCreateSyncStatus(
        /* [in] */ ISyncStorageEngineAuthorityInfo* authority,
        /* [out] */ ISyncStatusInfo** syncStatus);

    CARAPI WriteAllState();

    CARAPI ClearAndReadState();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IFile* dataDir);

    CARAPI_(void) GetBackoff(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [out] */ Pair<Int64, Int64>** pairs);

public:

    static CARAPI GetIntColumn(
        /* [in] */ ICursor* cursor,
        /* [in] */ const String& name,
        /* [out] */ Int32* result);

    static CARAPI GetLongColumn(
        /* [in] */ ICursor* cursor,
        /* [in] */ const String& name,
        /* [out] */ Int64* result);

private:

    static CARAPI FlattenBundle(
        /* [in] */ IBundle* bundle,
        /* [out, callee] */ ArrayOf<Byte>** flattenBundleArray);

    static CARAPI UnflattenBundle(
        /* [in] */ ArrayOf<Byte>* flatData,
        /* [out] */ IBundle** bundle);

    static CARAPI DoStaticInitialization();

private:

    CARAPI ReportChange(
        /* [in] */ Int32 which);

    CARAPI UpdateOrRemovePeriodicSync(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [in] */ IBundle* extras,
        /* [in] */ Int64 period,
        /* [in] */ Boolean add);

    CARAPI GetCurrentDayLocked(
        /* [out] */ Int32* result);

    /**
     * Retrieve an authority, returning null if one does not exist.
     *
     * @param accountName The name of the account for the authority.
     * @param authorityName The name of the authority itself.
     * @param tag If non-null, this will be used in a log message if the
     * requested authority does not exist.
     */
    CARAPI GetAuthorityLocked(
        /* [in] */ IAccount* accountName,
        /* [in] */ const String& authorityName,
        /* [in] */ const String& tag,
        /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo);

    CARAPI GetOrCreateAuthorityLocked(
        /* [in] */ IAccount* accountName,
        /* [in] */ const String& authorityName,
        /* [in] */ Int32 ident,
        /* [in] */ Boolean doWrite,
        /* [out] */ ISyncStorageEngineAuthorityInfo** authorityInfo);

    CARAPI RemoveAuthorityLocked(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authorityName,
        /* [in] */ Boolean doWrite);

    CARAPI GetOrCreateSyncStatusLocked(
        /* [in] */ Int32 authorityId,
        /* [out] */ ISyncStatusInfo** syncStatusInfo);

    /**
     * Read all account information back in to the initial engine state.
     */
    CARAPI ReadAccountInfoLocked();

    /**
     * some authority names have changed. copy over their settings and delete the old ones
     * @return true if a change was made
     */
    CARAPI MaybeMigrateSettingsForRenamedAuthorities(
        /* [out] */ Boolean* result);

    //TODO: private AuthorityInfo parseAuthority(XmlPullParser parser, int version)
    //TODO: private Pair<Bundle, Long> parsePeriodicSync(XmlPullParser parser, AuthorityInfo authority)
    //TODO: private void parseExtra(XmlPullParser parser, Pair<Bundle, Long> periodicSync)

    /**
     * Write all account information to the account file.
     */
    CARAPI WriteAccountInfoLocked();

    /**
     * Load sync engine state from the old syncmanager database, and then
     * erase it.  Note that we don't deal with pending operations, active
     * sync, or history.
     */
    CARAPI ReadAndDeleteLegacyAccountInfoLocked();

    /**
     * Read all sync status back in to the initial engine state.
     */
    CARAPI ReadStatusLocked();

    /**
     * Write all sync status to the sync status file.
     */
    CARAPI WriteStatusLocked();

    /**
     * Read all pending operations back in to the initial engine state.
     */
    CARAPI ReadPendingOperationsLocked();

    CARAPI WritePendingOperationLocked(
        /* [in] */ ISyncStorageEnginePendingOperation* op,
        /* [in] */ IParcel* out);

    /**
     * Write all currently pending ops to the pending ops file.
     */
    CARAPI WritePendingOperationsLocked();

    /**
     * Append the given operation to the pending ops file; if unable to,
     * write all pending ops.
     */
    CARAPI AppendPendingOperationLocked(
        /* [in] */ ISyncStorageEnginePendingOperation* op);

    /**
     * Read all sync statistics back in to the initial engine state.
     */
    CARAPI ReadStatisticsLocked();

    /**
     * Write all sync statistics to the sync status file.
     */
    CARAPI WriteStatisticsLocked();

private:
    static const CString TAG;

    static const Boolean DEBUG_FILE;

    static const Int64 DEFAULT_POLL_FREQUENCY_SECONDS;

    // @VisibleForTesting
    static const Int64 MILLIS_IN_4WEEKS;

    static const Int32 MSG_WRITE_STATUS;

    static const Int64 WRITE_STATUS_DELAY; // 10 minutes

    static const Int32 MSG_WRITE_STATISTICS;

    static const Int64 WRITE_STATISTICS_DELAY; // 1/2 hour

    static const Boolean SYNC_ENABLED_DEFAULT;

    // the version of the accounts xml file format
    static const Int32 ACCOUNTS_VERSION;

    static HashMap<String, String>* sAuthorityRenames;

    static const ECode staticECode;

    static volatile ISyncStorageEngine* sSyncStorageEngine;

    static const Int32 PENDING_FINISH_TO_WRITE;

private:
    // Primary list of all syncable authorities.  Also our global lock.
    SparseArray* mAuthorities;

    HashMap<IAccount*, CSyncStorageEngine::AccountInfo*>* mAccounts;
    List<ISyncStorageEnginePendingOperation*>* mPendingOperations;
    AutoPtr<ISyncInfo> mCurrentSync;
    SparseArray* mSyncStatus;
    List<ISyncStorageEngineSyncHistoryItem*>* mSyncHistory;
    AutoPtr<IRemoteCallbackList> mChangeListeners;
    Int32 mNextAuthorityId;

    // We keep 4 weeks of stats.
    ArrayOf<AutoPtr<ISyncStorageEngineDayStats> >* mDayStats;

    //TODO: private final Calendar mCal;

    Int32 mYear;
    Int32 mYearInDays;
    AutoPtr<IContext> mContext;

    /**
     * This file contains the core engine state: all accounts and the
     * settings for them.  It must never be lost, and should be changed
     * infrequently, so it is stored as an XML file.
     */
    //TODO: private final AtomicFile mAccountInfoFile;

    /**
     * This file contains the current sync status.  We would like to retain
     * it across boots, but its loss is not the end of the world, so we store
     * this information as binary data.
     */
    //TODO: private final AtomicFile mStatusFile;

    /**
     * This file contains sync statistics.  This is purely debugging information
     * so is written infrequently and can be thrown away at any time.
     */
    //TODO: private final AtomicFile mStatisticsFile;

    /**
     * This file contains the pending sync operations.  It is a binary file,
     * which must be updated every time an operation is added or removed,
     * so we have special handling of it.
     */
    //TODO: private final AtomicFile mPendingFile;
    
    Int32 mNumPendingFinished;
    Int32 mNextHistoryId;
    Boolean mMasterSyncAutomatically;

};

#endif // __CSYNCSTORAGEENGINE_H__
