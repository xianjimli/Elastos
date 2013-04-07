
#ifndef  __CBACKUPMANAGERSERVICE_H__
#define  __CBACKUPMANAGERSERVICE_H__

#include "_CBackupManagerService.h"
#include "ext/frameworkdef.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <elastos/HashMap.h>
#include <elastos/HashSet.h>
#include <elastos/List.h>
#include <elastos/Set.h>
#include "BroadcastReceiver.h"
#include "SparseInt32Array.h"
#include <elastos/Map.h>

using namespace Elastos;
using namespace Elastos::Core;

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<IApplicationInfo> >
{
    size_t operator()(AutoPtr<IApplicationInfo> name) const
    {
        assert(name != NULL);
        return (size_t)name.Get();
    }
};

template<> struct EqualTo<AutoPtr<IApplicationInfo> >
{
    Boolean operator()(const AutoPtr<IApplicationInfo>& x,
                       const AutoPtr<IApplicationInfo>& y) const
    {
        return x.Get() == y.Get();
    }
};

_ELASTOS_NAMESPACE_END


CarClass(CBackupManagerService)
{
private:
    // set of backup services that have pending changes
    class BackupRequest {
    public:
        BackupRequest(
            /* [in] */ IApplicationInfo* app,
            /* [in] */ Boolean isFull);

        // public String ToString() {
        //     return "BackupRequest{app=" + mAppInfo + " full=" + mFullBackup + "}";
        // }

    public:
        AutoPtr<IApplicationInfo> mAppInfo;
        Boolean mFullBackup;
    };

    class ActiveRestoreSession;

    class RestoreGetSetsParams {
    public:
        RestoreGetSetsParams(
            /* [in] */ IBackupTransport* _transport,
            /* [in] */ ActiveRestoreSession* _session,
            /* [in] */ IRestoreObserver* _observer);

    public:
        AutoPtr<IBackupTransport> mTransport;
        ActiveRestoreSession* mSession;
        AutoPtr<IRestoreObserver> mObserver;
    };

    class RestoreParams {
    public:
        RestoreParams(
            /* [in] */ IBackupTransport* _transport,
            /* [in] */ IRestoreObserver* _obs,
            /* [in] */ Int64 _token,
            /* [in] */ ICapsuleInfo* _cap,
            /* [in] */ Int32 _pmToken,
            /* [in] */ Boolean _needFullBackup);

        RestoreParams(
            /* [in] */ IBackupTransport* _transport,
            /* [in] */ IRestoreObserver* _obs,
            /* [in] */ Int64 _token,
            /* [in] */ Boolean _needFullBackup);

    public:
        AutoPtr<IBackupTransport> mTransport;
        AutoPtr<IRestoreObserver> mObserver;
        Int64 mToken;
        AutoPtr<ICapsuleInfo>  mCapInfo;
        Int32 mPmToken; // in post-install restore, the PM's token for this transaction
        Boolean mNeedFullBackup;
    };

    class ClearParams {
    public:
        ClearParams(
            /* [in] */ IBackupTransport* _transport,
            /* [in] */ ICapsuleInfo* _info);

    public:
        AutoPtr<IBackupTransport> mTransport;
        AutoPtr<ICapsuleInfo>  mCapInfo;
    };

    class RunBackupReceiver
        : public BroadcastReceiver
    {
    public:
        RunBackupReceiver(
            /* [in] */ CBackupManagerService* host);
    protected:
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        CBackupManagerService* mHost;
    };

    class RunInitializeReceiver
        : public BroadcastReceiver
    {
    protected:
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);
    };

    class ClearDataObserver
        : public ElRefBase
        , public ICapsuleDataObserver
    {
    public:
        ClearDataObserver(
            /* [in] */ CBackupManagerService* host);

        CARAPI OnRemoveCompleted(
            /* [in] */ const String& packageName,
            /* [in] */ Boolean succeeded);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

    private:
        CBackupManagerService*  mHost;
    };

    // ----- Back up a set of applications via a worker thread -----

    class PerformBackupTask
        : public ElRefBase
        , public IRunnable
    {
    public:
        PerformBackupTask(
            /* [in] */ IBackupTransport* transport,
            /* [in] */ ArrayOf<BackupRequest*>* queue,
            /* [in] */ IFile* journal,
            /* [in] */ CBackupManagerService* host);

        CARAPI Run();

        CARAPI_(Int32) DoQueuedBackups(
            /* [in] */ IBackupTransport* transport);

        CARAPI_(Int32) ProcessOneBackup(
            /* [in] */ BackupRequest* request,
            /* [in] */ IBackupAgent* agent,
            /* [in] */ IBackupTransport* transport);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

    private:
        static CString TAG;
        AutoPtr<IBackupTransport> mTransport;
        ArrayOf<BackupRequest*>* mQueue;
        AutoPtr<IFile> mStateDir;
        AutoPtr<IFile> mJournal;
        CBackupManagerService*  mHost;
    };

    class PerformRestoreTask
        : public ElRefBase
        , public IRunnable
    {
    private:
        class RestoreRequest {
        public:
            RestoreRequest(
                /* [in] */ ICapsuleInfo* _app,
                /* [in] */ Int32 _version);

        public:
            AutoPtr<ICapsuleInfo> mApp;
            Int32 mStoredAppVersion;
        };

    public:
        PerformRestoreTask(
            /* [in] */ IBackupTransport* transport,
            /* [in] */ IRestoreObserver* observer,
            /* [in] */ Int64 restoreSetToken,
            /* [in] */ ICapsuleInfo* targetPackage,
            /* [in] */ Int32 pmToken,
            /* [in] */ Boolean needFullBackup);

        CARAPI Run();

        // Do the guts of a restore of one application, using mTransport.getRestoreData().
        CARAPI_(void) ProcessOneRestore(
            /* [in] */ ICapsuleInfo* app,
            /* [in] */ Int32 appVersionCode,
            /* [in] */ IBackupAgent* agent,
            /* [in] */ Boolean needFullBackup);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

    private:
        AutoPtr<IBackupTransport> mTransport;
        AutoPtr<IRestoreObserver> mObserver;
        Int64 mToken;
        AutoPtr<ICapsuleInfo> mTargetPackage;
        AutoPtr<IFile> mStateDir;
        Int32 mPmToken;
        Boolean mNeedFullBackup;
    };

    class PerformClearTask
        : public ElRefBase
        , public IRunnable
    {
    public:
        PerformClearTask(
            /* [in] */ IBackupTransport* transport,
            /* [in] */ ICapsuleInfo* capInfo);

        CARAPI Run();

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

    private:
        AutoPtr<IBackupTransport> mTransport;
        AutoPtr<ICapsuleInfo> mCapsule;
    };

    class PerformInitializeTask
        : public ElRefBase
        , public IRunnable
    {
    public:
        PerformInitializeTask(
            /* [in] */ HashSet<String>* transportNames);

        CARAPI Run();

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

    private:
        HashSet<String>* mQueue;
    };

    // ----- Restore session -----

    class ActiveRestoreSession
        : public ElRefBase
        , public IRestoreSessionStub
    {
    public:
        ActiveRestoreSession(
            /* [in] */ const String& packageName,
            /* [in] */ const String& transport,
            /* [in] */ CBackupManagerService* host);

        // --- Binder interface ---
        CARAPI GetAvailableRestoreSets(
            /* [in] */ IRestoreObserverStub* observer,
            /* [out] */ Int32* error);

        CARAPI RestoreAll(
            /* [in] */ Int64 token,
            /* [in] */ IRestoreObserverStub* observer,
            /* [out] */ Int32* error);

        CARAPI RestorePackage(
            /* [in] */ const String& packageName,
            /* [in] */ IRestoreObserverStub* observer,
            /* [out] */ Int32* error);

        CARAPI EndRestoreSession();

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

    private:
        static CString TAG;

        String mPackageName;
        AutoPtr<IBackupTransport> mRestoreTransport;
        ArrayOf<IRestoreSet>* mRestoreSets;
        CBackupManagerService* mHost;
    };

    class TrackCapsuleInstallAndRemoval
        : public BroadcastReceiver
    {
    protected:
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);
    };

    // ----- Track connection to GoogleBackupTransport service -----
    class GroupServiceConnection
        : public ElRefBase
        , public IServiceConnection
    {
    public:
        CARAPI OnServiceConnected(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service);

        CARAPI OnServiceDisconnected(
            /* [in] */ IComponentName* name);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();
    };

public:
    CBackupManagerService();

    ~CBackupManagerService();

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    // ----- IBackupManager binder interface -----
    CARAPI DataChanged(
        /* [in] */ const String& packageName);

    // Clear the given package's backup data from the current transport
    CARAPI ClearBackupData(
        /* [in] */ const String& packageName);

    // Run a backup pass immediately for any applications that have declared
    // that they have pending updates.
    CARAPI BackupNow();

    // Enable/disable the backup service
    CARAPI SetBackupEnabled(
        /* [in] */ Boolean enable);

    // Enable/disable automatic restore of app data at install time
    CARAPI SetAutoRestore(
        /* [in] */ Boolean doAutoRestore);

    // Mark the backup service as having been provisioned
    CARAPI SetBackupProvisioned(
        /* [in] */ Boolean available);

    // Report whether the backup mechanism is currently enabled
    CARAPI IsBackupEnabled(
        /* [out] */ Boolean* enabled);

    // Report the name of the currently active transport
    CARAPI GetCurrentTransport(
        /* [out] */ String* transport);

    // Report all known, available backup transports
    CARAPI ListAllTransports(
        /*[out, callee]*/ BufferOf<String>** transports);

    // Select which transport to use for the next backup operation.  If the given
    // name is not one of the available transports, no action is taken and the method
    // returns null.
    CARAPI SelectBackupTransport(
        /* [in] */ const String& transport,
        /* [out] */ String* port);

    // Callback: a requested backup agent has been instantiated.  This should only
    // be called from the Activity Manager.
    CARAPI AgentConnected(
        /* [in] */ const String& packageName,
        /* [in] */ IBinder* agentBinder);

    // Callback: a backup agent has failed to come up, or has unexpectedly quit.
    // If the agent failed to come up in the first place, the agentBinder argument
    // will be null.  This should only be called from the Activity Manager.
    CARAPI AgentDisconnected(
        /* [in] */ const String& packageName);

    // An application being installed will need a restore pass, then the Package Manager
    // will need to be told when the restore is finished.
    CARAPI RestoreAtInstall(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 token);

    // Hand off a restore session
    CARAPI BeginRestoreSession(
        /* [in] */ const String& packageName,
        /* [in] */ const String& transport,
        /* [out] */ IRestoreSessionStub** session);

    // Note that a currently-active backup agent has notified us that it has
    // completed the given outstanding asynchronous backup/restore operation.
    CARAPI OpComplete(
        /* [in] */ Int32 token);


private:
    CARAPI_(void) InitPackageTracking();

    CARAPI_(void) ParseLeftoverJournals();


    // Maintain persistent state around whether need to do an initialize operation.
    // Must be called with the queue lock held.
    CARAPI_(void) RecordInitPendingLocked(
        /* [in] */ Boolean isPending,
        /* [in] */ const String& transportName);

    // Reset all of our bookkeeping, in response to having been told that
    // the backend data has been wiped [due to idle expiry, for example],
    // so we must re-upload all saved settings.
    CARAPI_(void) ResetBackupState(
        /* [in] */ IFile* stateFileDir);

    // Add a transport to our set of available backends.  If 'transport' is null, this
    // is an unregistration, and the transport's entry is removed from our bookkeeping.
    CARAPI_(void) RegisterTransport(
        /* [in] */ const String& name,
        /* [in] */ IBackupTransport* transport);

    // Returns the set of all applications that define an android:backupAgent attribute
    CARAPI_(List<AutoPtr<ICapsuleInfo> > *) AllAgentPackages();

    // Reset the given package's known backup participants.  Unlike add/remove, the update
    // action cannot be passed a null package name.
    CARAPI_(void) UpdatePackageParticipantsLocked(
        /* [in] */ const String& packageName);

    // Called from the backup task: record that the given app has been successfully
    // backed up at least once
    CARAPI_(void) LogBackupComplete(
        /* [in] */ const String& packageName);

    // Remove our awareness of having ever backed up the given package
    CARAPI_(void) RemoveEverBackedUp(
        /* [in] */ const String& packageName);

    // Persistently record the current and ancestral backup tokens as well
    // as the set of packages with data [supposedly] available in the
    // ancestral dataset.
    CARAPI_(void) WriteRestoreTokens();

    // Return the given transport
    CARAPI_(AutoPtr<IBackupTransport>) GetTransport(
        /* [in] */ const String& transportName);

    // fire off a backup agent, blocking until it attaches or times out
    CARAPI_(AutoPtr<IBackupAgent>) BindToAgentSynchronous(
        /* [in] */ IApplicationInfo* app,
        /* [in] */ Int32 mode);

    // clear an application's data, blocking until the operation completes or times out
    CARAPI_(void) ClearApplicationDataSynchronous(
        /* [in] */ const String& packageName);

    // Get the restore-set token for the best-available restore set for this package:
    // the active set if possible, else the ancestral one.  Returns zero if none available.
    CARAPI_(Int64) GetAvailableRestoreToken(
        /* [in] */ const String& packageName);

    // -----
    // Utility methods used by the asynchronous-with-timeout backup/restore operations
    CARAPI_(Boolean) WaitUntilOperationComplete(
        /* [in] */ Int32 token);

    CARAPI_(Int64) PrepareOperationTimeout(
        /* [in] */ Int32 token,
        /* [in] */ Int64 interval);

    // ----- Restore handling -----

    CARAPI_(Boolean) SignaturesMatch(
        /* [in] */ ArrayOf<AutoPtr<ISignature> >* storedSigs,
        /* [in] */ ICapsuleInfo* target);

    CARAPI_(void) DataChangedImpl(
        /* [in] */ const String& packageName);

    CARAPI_(void) DataChangedImpl(
        /* [in] */ const String& packageName,
        /* [in] */ HashSet<AutoPtr<IApplicationInfo> >* targets);

    // Note: packageName is currently unused, but may be in the future
    CARAPI_(HashSet<AutoPtr<IApplicationInfo> >*) DataChangedTargets(
        /* [in] */ const String& packageName);

    CARAPI_(void) WriteToJournalLocked(
        /* [in] */ const String& str);

    CARAPI_(void) StartBackupAlarmsLocked(
        /* [in] */ Int64 delayBeforeFirstBackup);

    //Callback function.
    CARAPI_(void) HandleRunBackup();

    CARAPI_(void) HandleRunInitilize();


private:
    static CString TAG;
    static const Boolean DEBUG = FALSE;

    // How often we perform a backup pass.  Privileged external callers can
    // trigger an immediate pass.
    static const Int64 BACKUP_INTERVAL = 2 * 2 * 15 * 60 * 1000; //AlarmManager.INTERVAL_HOUR;

    // Random variation in backup scheduling time to avoid server load spikes
    static const Int32 FUZZ_MILLIS = 5 * 60 * 1000;

    // The amount of time between the initial provisioning of the device and
    // the first backup pass.
    static const Int64 FIRST_BACKUP_INTERVAL = 12 * (2 * 2 * 15 * 60 * 1000) ;//AlarmManager.INTERVAL_HOUR;

    static CString RUN_BACKUP_ACTION;
    static CString RUN_INITIALIZE_ACTION;
    static CString RUN_CLEAR_ACTION;
    static const Int32 MSG_RUN_BACKUP = 1;
    static const Int32 MSG_RUN_FULL_BACKUP = 2;
    static const Int32 MSG_RUN_RESTORE = 3;
    static const Int32 MSG_RUN_CLEAR = 4;
    static const Int32 MSG_RUN_INITIALIZE = 5;
    static const Int32 MSG_RUN_GET_RESTORE_SETS = 6;
    static const Int32 MSG_TIMEOUT = 7;

    // Timeout interval for deciding that a bind or clear-data has taken too Int64
    static const Int64 TIMEOUT_INTERVAL = 10 * 1000;

    // Timeout intervals for agent backup & restore operations
    static const Int64 TIMEOUT_BACKUP_INTERVAL = 30 * 1000;
    static const Int64 TIMEOUT_RESTORE_INTERVAL = 60 * 1000;

    AutoPtr<IContext> mContext;
    AutoPtr<ICapsuleManager> mPackageManager;
    AutoPtr<ICapsuleManager> mPackageManagerBinder;
    AutoPtr<IActivityManager> mActivityManager;

    //TODO
    //PowerManager mPowerManager;
    //AlarmManager mAlarmManager;
    AutoPtr<IBackupManager> mBackupManagerBinder;

    Boolean mEnabled;   // access to this is synchronized on 'this'
    Boolean mProvisioned;
    Boolean mAutoRestore;
    //PowerManager.WakeLock mWakelock;

    //TODO
    // HandlerThread mHandlerThread = new HandlerThread("backup", Process.THREAD_PRIORITY_BACKGROUND);
    // BackupHandler mBackupHandler;


    AutoPtr<IPendingIntent> mRunBackupIntent, mRunInitIntent;
    AutoPtr<IBroadcastReceiver> mRunBackupReceiver, mRunInitReceiver;
    // map UIDs to the set of backup client services within that UID's app set
    // final SparseArray<HashSet<ApplicationInfo>> mBackupParticipants
    //     = new SparseArray<HashSet<ApplicationInfo>>();
    Map<Int32, HashSet<AutoPtr<IApplicationInfo> >*> mBackupParticipants;


    // Backups that we haven't started yet.
    HashMap<AutoPtr<IApplicationInfo>, BackupRequest* > mPendingBackups;
    // HashMap<ApplicationInfo,BackupRequest> mPendingBackups
    //         = new HashMap<ApplicationInfo,BackupRequest>();

    // Pseudoname that we use for the Package Manager metadata "package"
    static CString PACKAGE_MANAGER_SENTINEL;

    // locking around the pending-backup management
    Mutex mQueueLock;

    // The thread performing the sequence of queued backups binds to each app's agent
    // in succession.  Bind notifications are asynchronously delivered through the
    // Activity Manager; use this lock object to signal when a requested binding has
    // completed.
    Mutex mAgentConnectLock;
    AutoPtr<IBackupAgent> mConnectedAgent;
    volatile Boolean mConnecting;
    volatile Int64 mLastBackupPass;
    volatile Int64 mNextBackupPass;

    // A similar synchronization mechanism around clearing apps' data for restore
    Mutex mClearDataLock;
    volatile Boolean mClearingData;

    // Transport bookkeeping
    HashMap<String, AutoPtr<IBackupTransport> > mTransports;

    String mCurrentTransport;
    AutoPtr<IBackupTransport> mLocalTransport, mGoogleTransport;
    AutoPtr<ActiveRestoreSession> mActiveRestoreSession;

    // Bookkeeping of in-flight operations for timeout etc. purposes.  The operation
    // token is the index of the entry in the pending-operations list.
    static const Int32 OP_PENDING = 0;
    static const Int32 OP_ACKNOWLEDGED = 1;
    static const Int32 OP_TIMEOUT = -1;

    SparseInt32Array* mCurrentOperations;
    Mutex mCurrentOpLock;

    //TODO
    //Random mTokenGenerator = new Random();

    // Where we keep our journal files and other bookkeeping
    AutoPtr<IFile> mBaseStateDir;
    AutoPtr<IFile> mDataDir;
    AutoPtr<IFile> mJournalDir;
    AutoPtr<IFile> mJournal;

    // Keep a log of all the apps we've ever backed up, and what the
    // dataset tokens are for both the current backup dataset and
    // the ancestral dataset.
    AutoPtr<IFile> mEverStored;
    HashSet<String> mEverStoredApps;

    static const Int32 CURRENT_ANCESTRAL_RECORD_VERSION = 1;  // increment when the schema changes
    AutoPtr<IFile> mTokenFile;
    Set<String> mAncestralPackages;
    Int64 mAncestralToken;
    Int64 mCurrentToken;

    // Persistently track the need to do a full init
    static CString INIT_SENTINEL_FILE_NAME;
    HashSet<String> mPendingInits;  // transport names

    // ----- Track installation/removal of packages -----
    BroadcastReceiver* mBroadcastReceiver;

    // ----- Track connection to GoogleBackupTransport service -----
    AutoPtr<IServiceConnection> mGoogleConnection;

private:
    friend class RunBackupReceiver;
};

#endif  //__CBACKUPMANAGERSERVICE_H__
