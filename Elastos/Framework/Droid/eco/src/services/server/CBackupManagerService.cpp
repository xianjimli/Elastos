
#include "server/CBackupManagerService.h"
#include <Slogger.h>
#include <StringBuffer.h>
#include <BackupConstants.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

CString CBackupManagerService::PerformBackupTask::TAG = "PerformBackupThread";
CString CBackupManagerService::ActiveRestoreSession::TAG = "RestoreSession";


CString CBackupManagerService::TAG = "BackupManagerService";
const Boolean CBackupManagerService::DEBUG;
const Int64 CBackupManagerService::BACKUP_INTERVAL;
const Int32 CBackupManagerService::FUZZ_MILLIS;
const Int64 CBackupManagerService::FIRST_BACKUP_INTERVAL;
CString CBackupManagerService::RUN_BACKUP_ACTION = "android.app.backup.intent.RUN";
CString CBackupManagerService::RUN_INITIALIZE_ACTION = "android.app.backup.intent.INIT";
CString CBackupManagerService::RUN_CLEAR_ACTION = "android.app.backup.intent.CLEAR";
const Int32 CBackupManagerService::MSG_RUN_BACKUP;
const Int32 CBackupManagerService::MSG_RUN_FULL_BACKUP;
const Int32 CBackupManagerService::MSG_RUN_RESTORE;
const Int32 CBackupManagerService::MSG_RUN_CLEAR;
const Int32 CBackupManagerService::MSG_RUN_INITIALIZE;
const Int32 CBackupManagerService::MSG_RUN_GET_RESTORE_SETS;
const Int32 CBackupManagerService::MSG_TIMEOUT;
const Int64 CBackupManagerService::TIMEOUT_INTERVAL;
const Int64 CBackupManagerService::TIMEOUT_BACKUP_INTERVAL;
const Int64 CBackupManagerService::TIMEOUT_RESTORE_INTERVAL;
CString CBackupManagerService::PACKAGE_MANAGER_SENTINEL = "@pm@";
const Int32 CBackupManagerService::OP_PENDING;
const Int32 CBackupManagerService::OP_ACKNOWLEDGED;
const Int32 CBackupManagerService::OP_TIMEOUT;
const Int32 CBackupManagerService::CURRENT_ANCESTRAL_RECORD_VERSION;
CString CBackupManagerService::INIT_SENTINEL_FILE_NAME = "_need_init_";


CBackupManagerService::BackupRequest::BackupRequest(
    /* [in] */ IApplicationInfo* app,
    /* [in] */ Boolean isFull)
    : mAppInfo(app)
    , mFullBackup(isFull)
{
}

CBackupManagerService::RestoreGetSetsParams::RestoreGetSetsParams(
    /* [in] */ IBackupTransport* _transport,
    /* [in] */ ActiveRestoreSession* _session,
    /* [in] */ IRestoreObserver* _observer)
    : mTransport(_transport)
    , mSession(_session)
    , mObserver(_observer)
{
}

CBackupManagerService::RestoreParams::RestoreParams(
    /* [in] */ IBackupTransport* _transport,
    /* [in] */ IRestoreObserver* _obs,
    /* [in] */ Int64 _token,
    /* [in] */ ICapsuleInfo* _cap,
    /* [in] */ Int32 _pmToken,
    /* [in] */ Boolean _needFullBackup)
    : mTransport(_transport)
    , mObserver(_obs)
    , mToken(_token)
    , mCapInfo(_cap)
    , mPmToken(_pmToken)
    , mNeedFullBackup(_needFullBackup)
{
}

CBackupManagerService::RestoreParams::RestoreParams(
    /* [in] */ IBackupTransport* _transport,
    /* [in] */ IRestoreObserver* _obs,
    /* [in] */ Int64 _token,
    /* [in] */ Boolean _needFullBackup)
    : mTransport(_transport)
    , mObserver(_obs)
    , mToken(_token)
    , mCapInfo(NULL)
    , mPmToken(0)
    , mNeedFullBackup(_needFullBackup)
{
}

CBackupManagerService::ClearParams::ClearParams(
    /* [in] */ IBackupTransport* _transport,
    /* [in] */ ICapsuleInfo* _info)
    : mTransport(_transport)
    , mCapInfo(_info)
{
}

CBackupManagerService::RunBackupReceiver::RunBackupReceiver(
    /* [in] */ CBackupManagerService* host)
    : mHost(host)
{
}

ECode CBackupManagerService::RunBackupReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    assert(intent != NULL);
    intent->GetAction(&action);

    if (RUN_BACKUP_ACTION.Equals(action)) {
        {
            Mutex::Autolock lock(mHost->mQueueLock);
            if (mHost->mPendingInits.GetSize() > 0) {
                // If there are pending init operations, we process those
                // and then settle into the usual periodic backup schedule.
                //if (DEBUG) Slog.v(TAG, "Init pending at scheduled backup");

                //TODO
                //mAlarmManager.cancel(mRunInitIntent);
                assert(mHost->mRunInitIntent != NULL);
                mHost->mRunInitIntent->Send();
                // } catch (PendingIntent.CanceledException ce) {
                //     Slog.e(TAG, "Run init intent cancelled");
                //     // can't really do more than bail here
                // }
            } else {
                // Don't run backups now if we're disabled or not yet
                // fully set up.
                if (mHost->mEnabled && mHost->mProvisioned) {
                    if (DEBUG) Slogger::V(TAG, StringBuffer("Running a backup pass"));

                    // Acquire the wakelock and pass it to the backup thread.  it will
                    // be released once backup concludes.
                    //TODO
                    //mWakelock.acquire();

#if 0
                    void (STDCALL CBackupManagerService::*pHandlerFunc)();

                    pHandlerFunc = &CBackupManagerService::HandleRunBackup;
                    mHost->mBackupHandler->PostCppCallbackDelayed(
                        (Handle32)mHost, *(Handle32*)&pHandlerFunc,
                        NULL, 0, 0);
#endif
                } else {
                    Slogger::W(TAG, StringBuffer("Backup pass but e=") + mHost->mEnabled +
                        StringBuffer(" p=") + mHost->mProvisioned);
                }
            }
        }
    }

    return NOERROR;
}





//TODO: could't porting.
ECode CBackupManagerService::RunInitializeReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // String action;
    // assert(intent != NULL);
    // intent->GetAction(&action);

    // if (RUN_INITIALIZE_ACTION.Equals(action)) {
    //     {
    //         Mutex::Autolock lock(mQueueLock);
    //         if (DEBUG) Slog.v(TAG, "Running a device init");

    //         // Acquire the wakelock and pass it to the init thread.  it will
    //         // be released once init concludes.
    //         //TODO
    //         //mWakelock.acquire();

    //         void (STDCALL CBackupManagerService::*pHandlerFunc)();

    //         pHandlerFunc = &CBackupManagerService::HandleRunInitilize;
    //         mHost->mBackupHandler->PostCppCallbackDelayed(
    //             (Handle32)mHost, *(Handle32*)&pHandlerFunc,
    //             NULL, 0, 0);
    //     }
    // }

    return NOERROR;
}

CBackupManagerService::ClearDataObserver::ClearDataObserver(
    /* [in] */ CBackupManagerService* host)
    : mHost(host)
{
}

ECode CBackupManagerService::ClearDataObserver::OnRemoveCompleted(
    /* [in] */ const String& packageName,
    /* [in] */ Boolean succeeded)
{
    // {
    //     Mutex::Autolock lock(mHost->mClearDataLock);
    //     mClearingData = FALSE;
    //     mClearDataLock.notifyAll();
    // }

    return E_NOT_IMPLEMENTED;
}

UInt32 CBackupManagerService::ClearDataObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CBackupManagerService::ClearDataObserver::Release()
{
    return ElRefBase::Release();
}

CBackupManagerService::PerformBackupTask::PerformBackupTask(
    /* [in] */ IBackupTransport* transport,
    /* [in] */ ArrayOf<BackupRequest*>* queue,
    /* [in] */ IFile* journal,
    /* [in] */ CBackupManagerService* host)
    : mTransport(transport)
    , mQueue(queue)
    , mJournal(journal)
    , mHost(host)
{
    // String dirName;
    // assert(transport != NULL);
    // transport->TransportDirName(&dirName);
    // CFile::New(mBaseStateDir, dirName, (IFile**) &mStateDir);
    // // } catch (RemoteException e) {
    // //     // can't happen; the transport is local
    // // }
}

ECode CBackupManagerService::PerformBackupTask::Run()
{
    // Int32 status = BackupConstants_TRANSPORT_OK;
    // Int64 startRealtime = SystemClock.elapsedRealtime();
    // if (DEBUG) Slog.v(TAG, "Beginning backup of " + mQueue.size() + " targets");

    // // Backups run at background priority
    // Process.setThreadPriority(Process.THREAD_PRIORITY_BACKGROUND);

    // try {
    //     EventLog.writeEvent(EventLogTags.BACKUP_START, mTransport.transportDirName());

    //     // If we haven't stored package manager metadata yet, we must init the transport.
    //     File pmState = new File(mStateDir, PACKAGE_MANAGER_SENTINEL);
    //     if (status == BackupConstants.TRANSPORT_OK && pmState.length() <= 0) {
    //         Slog.i(TAG, "Initializing (wiping) backup state and transport storage");
    //         resetBackupState(mStateDir);  // Just to make sure.
    //         status = mTransport.initializeDevice();
    //         if (status == BackupConstants.TRANSPORT_OK) {
    //             EventLog.writeEvent(EventLogTags.BACKUP_INITIALIZE);
    //         } else {
    //             EventLog.writeEvent(EventLogTags.BACKUP_TRANSPORT_FAILURE, "(initialize)");
    //             Slog.e(TAG, "Transport error in initializeDevice()");
    //         }
    //     }

    //     // The package manager doesn't have a proper <application> etc, but since
    //     // it's running here in the system process we can just set up its agent
    //     // directly and use a synthetic BackupRequest.  We always run this pass
    //     // because it's cheap and this way we guarantee that we don't get out of
    //     // step even if we're selecting among various transports at run time.
    //     if (status == BackupConstants.TRANSPORT_OK) {
    //         PackageManagerBackupAgent pmAgent = new PackageManagerBackupAgent(
    //                 mPackageManager, allAgentPackages());
    //         BackupRequest pmRequest = new BackupRequest(new ApplicationInfo(), FALSE);
    //         pmRequest.appInfo.packageName = PACKAGE_MANAGER_SENTINEL;
    //         status = processOneBackup(pmRequest,
    //                 IBackupAgent.Stub.asInterface(pmAgent.onBind()), mTransport);
    //     }

    //     if (status == BackupConstants.TRANSPORT_OK) {
    //         // Now run all the backups in our queue
    //         status = doQueuedBackups(mTransport);
    //     }

    //     if (status == BackupConstants.TRANSPORT_OK) {
    //         // Tell the transport to finish everything it has buffered
    //         status = mTransport.finishBackup();
    //         if (status == BackupConstants.TRANSPORT_OK) {
    //             Int32 millis = (Int32) (SystemClock.elapsedRealtime() - startRealtime);
    //             EventLog.writeEvent(EventLogTags.BACKUP_SUCCESS, mQueue.size(), millis);
    //         } else {
    //             EventLog.writeEvent(EventLogTags.BACKUP_TRANSPORT_FAILURE, "(finish)");
    //             Slog.e(TAG, "Transport error in finishBackup()");
    //         }
    //     }

    //     if (status == BackupConstants.TRANSPORT_NOT_INITIALIZED) {
    //         // The backend reports that our dataset has been wiped.  We need to
    //         // reset all of our bookkeeping and instead run a new backup pass for
    //         // everything.
    //         EventLog.writeEvent(EventLogTags.BACKUP_RESET, mTransport.transportDirName());
    //         resetBackupState(mStateDir);
    //     }
    // } catch (Exception e) {
    //     Slog.e(TAG, "Error in backup thread", e);
    //     status = BackupConstants.TRANSPORT_ERROR;
    // } finally {
    //     // If everything actually went through and this is the first time we've
    //     // done a backup, we can now record what the current backup dataset token
    //     // is.
    //     if ((mCurrentToken == 0) && (status == BackupConstants.TRANSPORT_OK)) {
    //         try {
    //             mCurrentToken = mTransport.getCurrentRestoreSet();
    //         } catch (RemoteException e) { /* cannot happen */ }
    //         writeRestoreTokens();
    //     }

    //     // If things went wrong, we need to re-stage the apps we had expected
    //     // to be backing up in this pass.  This journals the package names in
    //     // the current active pending-backup file, not in the we are holding
    //     // here in mJournal.
    //     if (status != BackupConstants.TRANSPORT_OK) {
    //         Slog.w(TAG, "Backup pass unsuccessful, restaging");
    //         for (BackupRequest req : mQueue) {
    //             dataChangedImpl(req.appInfo.packageName);
    //         }

    //         // We also want to reset the backup schedule based on whatever
    //         // the transport suggests by way of retry/backoff time.
    //         try {
    //             startBackupAlarmsLocked(mTransport.requestBackupTime());
    //         } catch (RemoteException e) { /* cannot happen */ }
    //     }

    //     // Either backup was successful, in which case we of course do not need
    //     // this pass's journal any more; or it failed, in which case we just
    //     // re-enqueued all of these packages in the current active journal.
    //     // Either way, we no longer need this pass's journal.
    //     if (mJournal != NULL && !mJournal.delete()) {
    //         Slog.e(TAG, "Unable to remove backup journal file " + mJournal);
    //     }

    //     // Only once we're entirely finished do we release the wakelock
    //     if (status == BackupConstants.TRANSPORT_NOT_INITIALIZED) {
    //         backupNow();
    //     }

    //     mWakelock.release();
    // }

    return E_NOT_IMPLEMENTED;
}

Int32 CBackupManagerService::PerformBackupTask::DoQueuedBackups(
    /* [in] */ IBackupTransport* transport)
{
    // for (BackupRequest request : mQueue) {
    //     Slog.d(TAG, "starting agent for backup of " + request);

    //     IBackupAgent agent = NULL;
    //     Int32 mode = (request.fullBackup)
    //             ? IApplicationThread.BACKUP_MODE_FULL
    //             : IApplicationThread.BACKUP_MODE_INCREMENTAL;
    //     try {
    //         mWakelock.setWorkSource(new WorkSource(request.appInfo.uid));
    //         agent = bindToAgentSynchronous(request.appInfo, mode);
    //         if (agent != NULL) {
    //             Int32 result = processOneBackup(request, agent, transport);
    //             if (result != BackupConstants.TRANSPORT_OK) return result;
    //         }
    //     } catch (SecurityException ex) {
    //         // Try for the next one.
    //         Slog.d(TAG, "error in bind/backup", ex);
    //     } finally {
    //         try {  // unbind even on timeout, just in case
    //             mActivityManager.unbindBackupAgent(request.appInfo);
    //         } catch (RemoteException e) {}
    //     }
    // }

    // mWakelock.setWorkSource(NULL);

    return BackupConstants::TRANSPORT_OK;
}

Int32 CBackupManagerService::PerformBackupTask::ProcessOneBackup(
    /* [in] */ BackupRequest* request,
    /* [in] */ IBackupAgent* agent,
    /* [in] */ IBackupTransport* transport)
{
    // final String packageName = request.appInfo.packageName;
    // if (DEBUG) Slog.d(TAG, "processOneBackup doBackup() on " + packageName);

    // File savedStateName = new File(mStateDir, packageName);
    // File backupDataName = new File(mDataDir, packageName + ".data");
    // File newStateName = new File(mStateDir, packageName + ".new");

    // ParcelFileDescriptor savedState = NULL;
    // ParcelFileDescriptor backupData = NULL;
    // ParcelFileDescriptor newState = NULL;

    // PackageInfo packInfo;

    // srand(time(NULL));
    // Int32 token = rand();
    // try {
    //     // Look up the package info & signatures.  This is first so that if it
    //     // throws an exception, there's no file setup yet that would need to
    //     // be unraveled.
    //     if (packageName.equals(PACKAGE_MANAGER_SENTINEL)) {
    //         // The metadata 'package' is synthetic
    //         packInfo = new PackageInfo();
    //         packInfo.packageName = packageName;
    //     } else {
    //         packInfo = mPackageManager.getPackageInfo(packageName,
    //             PackageManager.GET_SIGNATURES);
    //     }

    //     // In a full backup, we pass a NULL ParcelFileDescriptor as
    //     // the saved-state "file"
    //     if (!request.fullBackup) {
    //         savedState = ParcelFileDescriptor.open(savedStateName,
    //                 ParcelFileDescriptor.MODE_READ_ONLY |
    //                 ParcelFileDescriptor.MODE_CREATE);  // Make an empty file if necessary
    //     }

    //     backupData = ParcelFileDescriptor.open(backupDataName,
    //             ParcelFileDescriptor.MODE_READ_WRITE |
    //             ParcelFileDescriptor.MODE_CREATE |
    //             ParcelFileDescriptor.MODE_TRUNCATE);

    //     newState = ParcelFileDescriptor.open(newStateName,
    //             ParcelFileDescriptor.MODE_READ_WRITE |
    //             ParcelFileDescriptor.MODE_CREATE |
    //             ParcelFileDescriptor.MODE_TRUNCATE);

    //     // Initiate the target's backup pass
    //     prepareOperationTimeout(token, TIMEOUT_BACKUP_INTERVAL);
    //     agent.doBackup(savedState, backupData, newState, token, mBackupManagerBinder);
    //     boolean success = waitUntilOperationComplete(token);

    //     if (!success) {
    //         // timeout -- bail out into the failed-transaction logic
    //         throw new RuntimeException("Backup timeout");
    //     }

    //     logBackupComplete(packageName);
    //     if (DEBUG) Slog.v(TAG, "doBackup() success");
    // } catch (Exception e) {
    //     Slog.e(TAG, "Error backing up " + packageName, e);
    //     EventLog.writeEvent(EventLogTags.BACKUP_AGENT_FAILURE, packageName, e.toString());
    //     backupDataName.delete();
    //     newStateName.delete();
    //     return BackupConstants.TRANSPORT_ERROR;
    // } finally {
    //     try { if (savedState != NULL) savedState.close(); } catch (IOException e) {}
    //     try { if (backupData != NULL) backupData.close(); } catch (IOException e) {}
    //     try { if (newState != NULL) newState.close(); } catch (IOException e) {}
    //     savedState = backupData = newState = NULL;
    //     synchronized (mCurrentOpLock) {
    //         mCurrentOperations.clear();
    //     }
    // }

    // // Now propagate the newly-backed-up data to the transport
    // Int32 result = BackupConstants.TRANSPORT_OK;
    // try {
    //     Int32 size = (Int32) backupDataName.length();
    //     if (size > 0) {
    //         if (result == BackupConstants.TRANSPORT_OK) {
    //             backupData = ParcelFileDescriptor.open(backupDataName,
    //                     ParcelFileDescriptor.MODE_READ_ONLY);
    //             result = transport.performBackup(packInfo, backupData);
    //         }

    //         // TODO - We call finishBackup() for each application backed up, because
    //         // we need to know now whether it succeeded or failed.  Instead, we should
    //         // hold off on finishBackup() until the end, which implies holding off on
    //         // renaming *all* the output state files (see below) until that happens.

    //         if (result == BackupConstants.TRANSPORT_OK) {
    //             result = transport.finishBackup();
    //         }
    //     } else {
    //         if (DEBUG) Slog.i(TAG, "no backup data written; not calling transport");
    //     }

    //     // After successful transport, delete the now-stale data
    //     // and juggle the files so that next time we supply the agent
    //     // with the new state file it just created.
    //     if (result == BackupConstants.TRANSPORT_OK) {
    //         backupDataName.delete();
    //         newStateName.renameTo(savedStateName);
    //         EventLog.writeEvent(EventLogTags.BACKUP_PACKAGE, packageName, size);
    //     } else {
    //         EventLog.writeEvent(EventLogTags.BACKUP_TRANSPORT_FAILURE, packageName);
    //     }
    // } catch (Exception e) {
    //     Slog.e(TAG, "Transport error backing up " + packageName, e);
    //     EventLog.writeEvent(EventLogTags.BACKUP_TRANSPORT_FAILURE, packageName);
    //     result = BackupConstants.TRANSPORT_ERROR;
    // } finally {
    //     try { if (backupData != NULL) backupData.close(); } catch (IOException e) {}
    // }

    // return result;

    //TODO
    return 0xffff;
}

UInt32 CBackupManagerService::PerformBackupTask::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CBackupManagerService::PerformBackupTask::Release()
{
    return ElRefBase::Release();
}

CBackupManagerService::PerformRestoreTask::RestoreRequest::RestoreRequest(
    /* [in] */ ICapsuleInfo* _app,
    /* [in] */ Int32 _version)
    : mApp(_app)
    , mStoredAppVersion(_version)
{
}

CBackupManagerService::PerformRestoreTask::PerformRestoreTask(
    /* [in] */ IBackupTransport* transport,
    /* [in] */ IRestoreObserver* observer,
    /* [in] */ Int64 restoreSetToken,
    /* [in] */ ICapsuleInfo* targetPackage,
    /* [in] */ Int32 pmToken,
    /* [in] */ Boolean needFullBackup)
    : mTransport(transport)
    , mObserver(observer)
    , mToken(restoreSetToken)
    , mTargetPackage(targetPackage)
    , mPmToken(pmToken)
    , mNeedFullBackup(needFullBackup)
{
    // try {
    //     mStateDir = new File(mBaseStateDir, transport.transportDirName());
    // } catch (RemoteException e) {
    //     // can't happen; the transport is local
    // }
}

ECode CBackupManagerService::PerformRestoreTask::Run()
{
    // Int64 startRealtime = SystemClock.elapsedRealtime();
    // if (DEBUG) Slog.v(TAG, "Beginning restore process mTransport=" + mTransport
    //         + " mObserver=" + mObserver + " mToken=" + Long.toHexString(mToken)
    //         + " mTargetPackage=" + mTargetPackage + " mPmToken=" + mPmToken);

    // PackageManagerBackupAgent pmAgent = NULL;
    // Int32 error = -1; // assume error

    // // build the set of apps to restore
    // try {
    //     // TODO: Log this before getAvailableRestoreSets, somehow
    //     EventLog.writeEvent(EventLogTags.RESTORE_START, mTransport.transportDirName(), mToken);

    //     // Get the list of all packages which have backup enabled.
    //     // (Include the Package Manager metadata pseudo-package first.)
    //     ArrayList<PackageInfo> restorePackages = new ArrayList<PackageInfo>();
    //     PackageInfo omPackage = new PackageInfo();
    //     omPackage.packageName = PACKAGE_MANAGER_SENTINEL;
    //     restorePackages.add(omPackage);

    //     List<PackageInfo> agentPackages = allAgentPackages();
    //     if (mTargetPackage == NULL) {
    //         restorePackages.addAll(agentPackages);
    //     } else {
    //         // Just one package to attempt restore of
    //         restorePackages.add(mTargetPackage);
    //     }

    //     // let the observer know that we're running
    //     if (mObserver != NULL) {
    //         try {
    //             // !!! TODO: get an actual count from the transport after
    //             // its startRestore() runs?
    //             mObserver.restoreStarting(restorePackages.size());
    //         } catch (RemoteException e) {
    //             Slog.d(TAG, "Restore observer died at restoreStarting");
    //             mObserver = NULL;
    //         }
    //     }

    //     if (mTransport.startRestore(mToken, restorePackages.toArray(new PackageInfo[0])) !=
    //             BackupConstants.TRANSPORT_OK) {
    //         Slog.e(TAG, "Error starting restore operation");
    //         EventLog.writeEvent(EventLogTags.RESTORE_TRANSPORT_FAILURE);
    //         return;
    //     }

    //     String packageName = mTransport.nextRestorePackage();
    //     if (packageName == NULL) {
    //         Slog.e(TAG, "Error getting first restore package");
    //         EventLog.writeEvent(EventLogTags.RESTORE_TRANSPORT_FAILURE);
    //         return;
    //     } else if (packageName.equals("")) {
    //         Slog.i(TAG, "No restore data available");
    //         Int32 millis = (Int32) (SystemClock.elapsedRealtime() - startRealtime);
    //         EventLog.writeEvent(EventLogTags.RESTORE_SUCCESS, 0, millis);
    //         return;
    //     } else if (!packageName.equals(PACKAGE_MANAGER_SENTINEL)) {
    //         Slog.e(TAG, "Expected restore data for \"" + PACKAGE_MANAGER_SENTINEL
    //               + "\", found only \"" + packageName + "\"");
    //         EventLog.writeEvent(EventLogTags.RESTORE_AGENT_FAILURE, PACKAGE_MANAGER_SENTINEL,
    //                 "Package manager data missing");
    //         return;
    //     }

    //     // Pull the Package Manager metadata from the restore set first
    //     pmAgent = new PackageManagerBackupAgent(
    //             mPackageManager, agentPackages);
    //     processOneRestore(omPackage, 0, IBackupAgent.Stub.asInterface(pmAgent.onBind()),
    //             mNeedFullBackup);

    //     // Verify that the backup set includes metadata.  If not, we can't do
    //     // signature/version verification etc, so we simply do not proceed with
    //     // the restore operation.
    //     if (!pmAgent.hasMetadata()) {
    //         Slog.e(TAG, "No restore metadata available, so not restoring settings");
    //         EventLog.writeEvent(EventLogTags.RESTORE_AGENT_FAILURE, PACKAGE_MANAGER_SENTINEL,
    //                 "Package manager restore metadata missing");
    //         return;
    //     }

    //     Int32 count = 0;
    //     for (;;) {
    //         packageName = mTransport.nextRestorePackage();

    //         if (packageName == NULL) {
    //             Slog.e(TAG, "Error getting next restore package");
    //             EventLog.writeEvent(EventLogTags.RESTORE_TRANSPORT_FAILURE);
    //             return;
    //         } else if (packageName.equals("")) {
    //             if (DEBUG) Slog.v(TAG, "No next package, finishing restore");
    //             break;
    //         }

    //         if (mObserver != NULL) {
    //             try {
    //                 mObserver.onUpdate(count, packageName);
    //             } catch (RemoteException e) {
    //                 Slog.d(TAG, "Restore observer died in onUpdate");
    //                 mObserver = NULL;
    //             }
    //         }

    //         Metadata metaInfo = pmAgent.getRestoredMetadata(packageName);
    //         if (metaInfo == NULL) {
    //             Slog.e(TAG, "Missing metadata for " + packageName);
    //             EventLog.writeEvent(EventLogTags.RESTORE_AGENT_FAILURE, packageName,
    //                     "Package metadata missing");
    //             continue;
    //         }

    //         PackageInfo packageInfo;
    //         try {
    //             Int32 flags = PackageManager.GET_SIGNATURES;
    //             packageInfo = mPackageManager.getPackageInfo(packageName, flags);
    //         } catch (NameNotFoundException e) {
    //             Slog.e(TAG, "Invalid package restoring data", e);
    //             EventLog.writeEvent(EventLogTags.RESTORE_AGENT_FAILURE, packageName,
    //                     "Package missing on device");
    //             continue;
    //         }

    //         if (metaInfo.versionCode > packageInfo.versionCode) {
    //             // Data is from a "newer" version of the app than we have currently
    //             // installed.  If the app has not declared that it is prepared to
    //             // handle this case, we do not attempt the restore.
    //             if ((packageInfo.applicationInfo.flags
    //                     & ApplicationInfo_FLAG_RESTORE_ANY_VERSION) == 0) {
    //                 String message = "Version " + metaInfo.versionCode
    //                         + " > installed version " + packageInfo.versionCode;
    //                 Slog.w(TAG, "Package " + packageName + ": " + message);
    //                 EventLog.writeEvent(EventLogTags.RESTORE_AGENT_FAILURE,
    //                         packageName, message);
    //                 continue;
    //             } else {
    //                 if (DEBUG) Slog.v(TAG, "Version " + metaInfo.versionCode
    //                         + " > installed " + packageInfo.versionCode
    //                         + " but restoreAnyVersion");
    //             }
    //         }

    //         if (!signaturesMatch(metaInfo.signatures, packageInfo)) {
    //             Slog.w(TAG, "Signature mismatch restoring " + packageName);
    //             EventLog.writeEvent(EventLogTags.RESTORE_AGENT_FAILURE, packageName,
    //                     "Signature mismatch");
    //             continue;
    //         }

    //         if (DEBUG) Slog.v(TAG, "Package " + packageName
    //                 + " restore version [" + metaInfo.versionCode
    //                 + "] is compatible with installed version ["
    //                 + packageInfo.versionCode + "]");

    //         // Then set up and bind the agent
    //         IBackupAgent agent = bindToAgentSynchronous(
    //                 packageInfo.applicationInfo,
    //                 IApplicationThread.BACKUP_MODE_INCREMENTAL);
    //         if (agent == NULL) {
    //             Slog.w(TAG, "Can't find backup agent for " + packageName);
    //             EventLog.writeEvent(EventLogTags.RESTORE_AGENT_FAILURE, packageName,
    //                     "Restore agent missing");
    //             continue;
    //         }

    //         // And then finally run the restore on this agent
    //         try {
    //             processOneRestore(packageInfo, metaInfo.versionCode, agent,
    //                     mNeedFullBackup);
    //             ++count;
    //         } finally {
    //             // unbind and tidy up even on timeout or failure, just in case
    //             mActivityManager.unbindBackupAgent(packageInfo.applicationInfo);

    //             // The agent was probably running with a stub Application object,
    //             // which isn't a valid run mode for the main app logic.  Shut
    //             // down the app so that next time it's launched, it gets the
    //             // usual full initialization.  Note that this is only done for
    //             // full-system restores: when a single app has requested a restore,
    //             // it is explicitly not killed following that operation.
    //             if (mTargetPackage == NULL && (packageInfo.applicationInfo.flags
    //                     & ApplicationInfo.FLAG_KILL_AFTER_RESTORE) != 0) {
    //                 if (DEBUG) Slog.d(TAG, "Restore complete, killing host process of "
    //                         + packageInfo.applicationInfo.processName);
    //                 mActivityManager.killApplicationProcess(
    //                         packageInfo.applicationInfo.processName,
    //                         packageInfo.applicationInfo.uid);
    //             }
    //         }
    //     }

    //     // if we get this far, report success to the observer
    //     error = 0;
    //     Int32 millis = (Int32) (SystemClock.elapsedRealtime() - startRealtime);
    //     EventLog.writeEvent(EventLogTags.RESTORE_SUCCESS, count, millis);
    // } catch (Exception e) {
    //     Slog.e(TAG, "Error in restore thread", e);
    // } finally {
    //     if (DEBUG) Slog.d(TAG, "finishing restore mObserver=" + mObserver);

    //     try {
    //         mTransport.finishRestore();
    //     } catch (RemoteException e) {
    //         Slog.e(TAG, "Error finishing restore", e);
    //     }

    //     if (mObserver != NULL) {
    //         try {
    //             mObserver.restoreFinished(error);
    //         } catch (RemoteException e) {
    //             Slog.d(TAG, "Restore observer died at restoreFinished");
    //         }
    //     }

    //     // If this was a restoreAll operation, record that this was our
    //     // ancestral dataset, as well as the set of apps that are possibly
    //     // restoreable from the dataset
    //     if (mTargetPackage == NULL && pmAgent != NULL) {
    //         mAncestralPackages = pmAgent.getRestoredPackages();
    //         mAncestralToken = mToken;
    //         writeRestoreTokens();
    //     }

    //     // We must under all circumstances tell the Package Manager to
    //     // proceed with install notifications if it's waiting for us.
    //     if (mPmToken > 0) {
    //         if (DEBUG) Slog.v(TAG, "finishing PM token " + mPmToken);
    //         try {
    //             mPackageManagerBinder.finishPackageInstall(mPmToken);
    //         } catch (RemoteException e) { /* can't happen */ }
    //     }

    //     // done; we can finally release the wakelock
    //     mWakelock.release();
    // }

    return E_NOT_IMPLEMENTED;
}

void CBackupManagerService::PerformRestoreTask::ProcessOneRestore(
    /* [in] */ ICapsuleInfo* app,
    /* [in] */ Int32 appVersionCode,
    /* [in] */ IBackupAgent* agent,
    /* [in] */ Boolean needFullBackup)
{
    // // !!! TODO: actually run the restore through mTransport
    // final String packageName = app.packageName;

    // if (DEBUG) Slog.d(TAG, "processOneRestore packageName=" + packageName);

    // // !!! TODO: get the dirs from the transport
    // File backupDataName = new File(mDataDir, packageName + ".restore");
    // File newStateName = new File(mStateDir, packageName + ".new");
    // File savedStateName = new File(mStateDir, packageName);

    // ParcelFileDescriptor backupData = NULL;
    // ParcelFileDescriptor newState = NULL;

    // srand(time(NULL));
    // Int32 token = rand();
    // try {
    //     // Run the transport's restore pass
    //     backupData = ParcelFileDescriptor.open(backupDataName,
    //                 ParcelFileDescriptor.MODE_READ_WRITE |
    //                 ParcelFileDescriptor.MODE_CREATE |
    //                 ParcelFileDescriptor.MODE_TRUNCATE);

    //     if (mTransport.getRestoreData(backupData) != BackupConstants.TRANSPORT_OK) {
    //         Slog.e(TAG, "Error getting restore data for " + packageName);
    //         EventLog.writeEvent(EventLogTags.RESTORE_TRANSPORT_FAILURE);
    //         return;
    //     }

    //     // Okay, we have the data.  Now have the agent do the restore.
    //     backupData.close();
    //     backupData = ParcelFileDescriptor.open(backupDataName,
    //                 ParcelFileDescriptor.MODE_READ_ONLY);

    //     newState = ParcelFileDescriptor.open(newStateName,
    //                 ParcelFileDescriptor.MODE_READ_WRITE |
    //                 ParcelFileDescriptor.MODE_CREATE |
    //                 ParcelFileDescriptor.MODE_TRUNCATE);

    //     // Kick off the restore, checking for hung agents
    //     prepareOperationTimeout(token, TIMEOUT_RESTORE_INTERVAL);
    //     agent.doRestore(backupData, appVersionCode, newState, token, mBackupManagerBinder);
    //     boolean success = waitUntilOperationComplete(token);

    //     if (!success) {
    //         throw new RuntimeException("restore timeout");
    //     }

    //     // if everything went okay, remember the recorded state now
    //     //
    //     // !!! TODO: the restored data should be migrated on the server
    //     // side into the current dataset.  In that case the new state file
    //     // we just created would reflect the data already extant in the
    //     // backend, so there'd be nothing more to do.  Until that happens,
    //     // however, we need to make sure that we record the data to the
    //     // current backend dataset.  (Yes, this means shipping the data over
    //     // the wire in both directions.  That's bad, but consistency comes
    //     // first, then efficiency.)  Once we introduce server-side data
    //     // migration to the newly-restored device's dataset, we will change
    //     // the following from a discard of the newly-written state to the
    //     // "correct" operation of renaming into the canonical state blob.
    //     newStateName.delete();                      // TODO: remove; see above comment
    //     //newStateName.renameTo(savedStateName);    // TODO: replace with this

    //     Int32 size = (Int32) backupDataName.length();
    //     EventLog.writeEvent(EventLogTags.RESTORE_PACKAGE, packageName, size);
    // } catch (Exception e) {
    //     Slog.e(TAG, "Error restoring data for " + packageName, e);
    //     EventLog.writeEvent(EventLogTags.RESTORE_AGENT_FAILURE, packageName, e.toString());

    //     // If the agent fails restore, it might have put the app's data
    //     // into an incoherent state.  For consistency we wipe its data
    //     // again in this case before propagating the exception
    //     clearApplicationDataSynchronous(packageName);
    // } finally {
    //     backupDataName.delete();
    //     try { if (backupData != NULL) backupData.close(); } catch (IOException e) {}
    //     try { if (newState != NULL) newState.close(); } catch (IOException e) {}
    //     backupData = newState = NULL;
    //     mCurrentOperations.delete(token);

    //     // If we know a priori that we'll need to perform a full post-restore backup
    //     // pass, clear the new state file data.  This means we're discarding work that
    //     // was just done by the app's agent, but this way the agent doesn't need to
    //     // take any special action based on global device state.
    //     if (needFullBackup) {
    //         newStateName.delete();
    //     }
    // }
}

UInt32 CBackupManagerService::PerformRestoreTask::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CBackupManagerService::PerformRestoreTask::Release()
{
    return ElRefBase::Release();
}

CBackupManagerService::PerformClearTask::PerformClearTask(
    /* [in] */ IBackupTransport* transport,
    /* [in] */ ICapsuleInfo* capInfo)
    : mTransport(transport)
    , mCapsule(capInfo)
{
}

ECode CBackupManagerService::PerformClearTask::Run()
{
    // try {
    //     // Clear the on-device backup state to ensure a full backup next time
    //     File stateDir = new File(mBaseStateDir, mTransport.transportDirName());
    //     File stateFile = new File(stateDir, mPackage.packageName);
    //     stateFile.delete();

    //     // Tell the transport to remove all the persistent storage for the app
    //     // TODO - need to handle failures
    //     mTransport.clearBackupData(mPackage);
    // } catch (RemoteException e) {
    //     // can't happen; the transport is local
    // } finally {
    //     try {
    //         // TODO - need to handle failures
    //         mTransport.finishBackup();
    //     } catch (RemoteException e) {
    //         // can't happen; the transport is local
    //     }

    //     // Last but not least, release the cpu
    //     mWakelock.release();
    // }

    return E_NOT_IMPLEMENTED;
}

UInt32 CBackupManagerService::PerformClearTask::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CBackupManagerService::PerformClearTask::Release()
{
    return ElRefBase::Release();
}

CBackupManagerService::PerformInitializeTask::PerformInitializeTask(
    /* [in] */ HashSet<String>* transportNames)
    : mQueue(transportNames)
{
}

ECode CBackupManagerService::PerformInitializeTask::Run()
{
    // try {
    //     for (String transportName : mQueue) {
    //         IBackupTransport transport = getTransport(transportName);
    //         if (transport == NULL) {
    //             Slog.e(TAG, "Requested init for " + transportName + " but not found");
    //             continue;
    //         }

    //         Slog.i(TAG, "Initializing (wiping) backup transport storage: " + transportName);
    //         EventLog.writeEvent(EventLogTags.BACKUP_START, transport.transportDirName());
    //         Int64 startRealtime = SystemClock.elapsedRealtime();
    //         Int32 status = transport.initializeDevice();

    //         if (status == BackupConstants.TRANSPORT_OK) {
    //             status = transport.finishBackup();
    //         }

    //         // Okay, the wipe really happened.  Clean up our local bookkeeping.
    //         if (status == BackupConstants.TRANSPORT_OK) {
    //             Slog.i(TAG, "Device init successful");
    //             Int32 millis = (Int32) (SystemClock.elapsedRealtime() - startRealtime);
    //             EventLog.writeEvent(EventLogTags.BACKUP_INITIALIZE);
    //             resetBackupState(new File(mBaseStateDir, transport.transportDirName()));
    //             EventLog.writeEvent(EventLogTags.BACKUP_SUCCESS, 0, millis);
    //             {
    //                 Mutex::Autolock lock(mQueueLock);
    //                 recordInitPendingLocked(FALSE, transportName);
    //             }
    //         } else {
    //             // If this didn't work, requeue this one and try again
    //             // after a suitable interval
    //             Slog.e(TAG, "Transport error in initializeDevice()");
    //             EventLog.writeEvent(EventLogTags.BACKUP_TRANSPORT_FAILURE, "(initialize)");
    //             {
    //                 Mutex::Autolock lock(mQueueLock);
    //                 recordInitPendingLocked(TRUE, transportName);
    //             }
    //             // do this via another alarm to make sure of the wakelock states
    //             Int64 delay = transport.requestBackupTime();
    //             if (DEBUG) Slog.w(TAG, "init failed on "
    //                     + transportName + " resched in " + delay);
    //             mAlarmManager.set(AlarmManager.RTC_WAKEUP,
    //                     System.currentTimeMillis() + delay, mRunInitIntent);
    //         }
    //     }
    // } catch (RemoteException e) {
    //     // can't happen; the transports are local
    // } catch (Exception e) {
    //     Slog.e(TAG, "Unexpected error performing init", e);
    // } finally {
    //     // Done; release the wakelock
    //     mWakelock.release();
    // }

    return E_NOT_IMPLEMENTED;
}

UInt32 CBackupManagerService::PerformInitializeTask::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CBackupManagerService::PerformInitializeTask::Release()
{
    return ElRefBase::Release();
}

CBackupManagerService::ActiveRestoreSession::ActiveRestoreSession(
    /* [in] */ const String& packageName,
    /* [in] */ const String& transport,
    /* [in] */ CBackupManagerService* host)
    : mPackageName(packageName)
    , mRestoreTransport(NULL)
    , mRestoreSets(NULL)
    , mHost(host)
{
    assert(mHost != NULL);
    mRestoreTransport = mHost->GetTransport(transport);
}

// --- Binder interface ---
ECode CBackupManagerService::ActiveRestoreSession::GetAvailableRestoreSets(
    /* [in] */ IRestoreObserverStub* observer,
    /* [out] */ Int32* error)
{
    // synchronized(this) {
    //     mContext.enforceCallingOrSelfPermission(android.Manifest.permission.BACKUP,
    //             "getAvailableRestoreSets");
    //     if (observer == NULL) {
    //         throw new IllegalArgumentException("Observer must not be NULL");
    //     }

    //     Int64 oldId = Binder.clearCallingIdentity();
    //     try {
    //         if (mRestoreTransport == NULL) {
    //             Slog.w(TAG, "Null transport getting restore sets");
    //             return -1;
    //         }
    //         // spin off the transport request to our service thread
    //         mWakelock.acquire();
    //         Message msg = mBackupHandler.obtainMessage(MSG_RUN_GET_RESTORE_SETS,
    //                 new RestoreGetSetsParams(mRestoreTransport, this, observer));
    //         mBackupHandler.sendMessage(msg);
    //         return 0;
    //     } catch (Exception e) {
    //         Slog.e(TAG, "Error in getAvailableRestoreSets", e);
    //         return -1;
    //     } finally {
    //         Binder.restoreCallingIdentity(oldId);
    //     }
    // }

    return E_NOT_IMPLEMENTED;
}

ECode CBackupManagerService::ActiveRestoreSession::RestoreAll(
    /* [in] */ Int64 token,
    /* [in] */ IRestoreObserverStub* observer,
    /* [out] */ Int32* error)
{
    // synchronized(this) {
    //     mContext.enforceCallingOrSelfPermission(android.Manifest.permission.BACKUP,
    //             "performRestore");

    //     if (DEBUG) Slog.d(TAG, "restoreAll token=" + Long.toHexString(token)
    //             + " observer=" + observer);

    //     if (mRestoreTransport == NULL || mRestoreSets == NULL) {
    //         Slog.e(TAG, "Ignoring restoreAll() with no restore set");
    //         return -1;
    //     }

    //     if (mPackageName != NULL) {
    //         Slog.e(TAG, "Ignoring restoreAll() on single-package session");
    //         return -1;
    //     }

    //     {
    //         Mutex::Autolock lock(mQueueLock);
    //         for (Int32 i = 0; i < mRestoreSets.length; i++) {
    //             if (token == mRestoreSets[i].token) {
    //                 Int64 oldId = Binder.clearCallingIdentity();
    //                 mWakelock.acquire();
    //                 Message msg = mBackupHandler.obtainMessage(MSG_RUN_RESTORE);
    //                 msg.obj = new RestoreParams(mRestoreTransport, observer, token, TRUE);
    //                 mBackupHandler.sendMessage(msg);
    //                 Binder.restoreCallingIdentity(oldId);
    //                 return 0;
    //             }
    //         }
    //     }

    //     Slog.w(TAG, "Restore token " + Long.toHexString(token) + " not found");
    //     return -1;
    // }

    return E_NOT_IMPLEMENTED;
}

ECode CBackupManagerService::ActiveRestoreSession::RestorePackage(
    /* [in] */ const String& packageName,
    /* [in] */ IRestoreObserverStub* observer,
    /* [out] */ Int32* error)
{
    // synchronized(this) {
    //     if (DEBUG) Slog.v(TAG, "restorePackage pkg=" + packageName + " obs=" + observer);

    //     if (mPackageName != NULL) {
    //         if (! mPackageName.equals(packageName)) {
    //             Slog.e(TAG, "Ignoring attempt to restore pkg=" + packageName
    //                     + " on session for package " + mPackageName);
    //             return -1;
    //         }
    //     }

    //     PackageInfo app = NULL;
    //     try {
    //         app = mPackageManager.getPackageInfo(packageName, 0);
    //     } catch (NameNotFoundException nnf) {
    //         Slog.w(TAG, "Asked to restore nonexistent pkg " + packageName);
    //         return -1;
    //     }

    //     // If the caller is not privileged and is not coming from the target
    //     // app's uid, throw a permission exception back to the caller.
    //     Int32 perm = mContext.checkPermission(android.Manifest.permission.BACKUP,
    //             Binder.getCallingPid(), Binder.getCallingUid());
    //     if ((perm == PackageManager.PERMISSION_DENIED) &&
    //             (app.applicationInfo.uid != Binder.getCallingUid())) {
    //         Slog.w(TAG, "restorePackage: bad packageName=" + packageName
    //                 + " or calling uid=" + Binder.getCallingUid());
    //         throw new SecurityException("No permission to restore other packages");
    //     }

    //     // If the package has no backup agent, we obviously cannot proceed
    //     if (app.applicationInfo.backupAgentName == NULL) {
    //         Slog.w(TAG, "Asked to restore package " + packageName + " with no agent");
    //         return -1;
    //     }

    //     // So far so good; we're allowed to try to restore this package.  Now
    //     // check whether there is data for it in the current dataset, falling back
    //     // to the ancestral dataset if not.
    //     Int64 token = getAvailableRestoreToken(packageName);

    //     // If we didn't come up with a place to look -- no ancestral dataset and
    //     // the app has never been backed up from this device -- there's nothing
    //     // to do but return failure.
    //     if (token == 0) {
    //         if (DEBUG) Slog.w(TAG, "No data available for this package; not restoring");
    //         return -1;
    //     }

    //     // Ready to go:  enqueue the restore request and claim success
    //     Int64 oldId = Binder.clearCallingIdentity();
    //     mWakelock.acquire();
    //     Message msg = mBackupHandler.obtainMessage(MSG_RUN_RESTORE);
    //     msg.obj = new RestoreParams(mRestoreTransport, observer, token, app, 0, FALSE);
    //     mBackupHandler.sendMessage(msg);
    //     Binder.restoreCallingIdentity(oldId);
    //     return 0;
    // }

    return E_NOT_IMPLEMENTED;
}

ECode CBackupManagerService::ActiveRestoreSession::EndRestoreSession()
{
    // synchronized(this) {
    //     if (DEBUG) Slog.d(TAG, "endRestoreSession");

    //     synchronized (this) {
    //         Int64 oldId = Binder.clearCallingIdentity();
    //         try {
    //             if (mRestoreTransport != NULL) mRestoreTransport.finishRestore();
    //         } catch (Exception e) {
    //             Slog.e(TAG, "Error in finishRestore", e);
    //         } finally {
    //             mRestoreTransport = NULL;
    //             Binder.restoreCallingIdentity(oldId);
    //         }
    //     }

    //     synchronized (BackupManagerService.this) {
    //         if (BackupManagerService.this.mActiveRestoreSession == this) {
    //             BackupManagerService.this.mActiveRestoreSession = NULL;
    //         } else {
    //             Slog.e(TAG, "ending non-current restore session");
    //         }
    //     }
    // }

    return E_NOT_IMPLEMENTED;
}

UInt32 CBackupManagerService::ActiveRestoreSession::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CBackupManagerService::ActiveRestoreSession::Release()
{
    return ElRefBase::Release();
}

ECode CBackupManagerService::GroupServiceConnection::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    // if (DEBUG) Slog.v(TAG, "Connected to Google transport");
    // mGoogleTransport = IBackupTransport.Stub.asInterface(service);
    // RegisterTransport(name.flattenToShortString(), mGoogleTransport);

    return E_NOT_IMPLEMENTED;
}

ECode CBackupManagerService::GroupServiceConnection::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    // if (DEBUG) Slog.v(TAG, "Disconnected from Google transport");
    // mGoogleTransport = NULL;
    // RgisterTransport(name.flattenToShortString(), NULL);

    return E_NOT_IMPLEMENTED;
}

UInt32 CBackupManagerService::GroupServiceConnection::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CBackupManagerService::GroupServiceConnection::Release()
{
    return ElRefBase::Release();
}

ECode CBackupManagerService::TrackCapsuleInstallAndRemoval::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // if (DEBUG) Slog.d(TAG, "Received broadcast " + intent);

    // String action = intent.getAction();
    // boolean replacing = FALSE;
    // boolean added = FALSE;
    // Bundle extras = intent.getExtras();
    // String pkgList[] = NULL;
    // if (Intent.ACTION_PACKAGE_ADDED.equals(action) ||
    //         Intent.ACTION_PACKAGE_REMOVED.equals(action)) {
    //     Uri uri = intent.getData();
    //     if (uri == NULL) {
    //         return;
    //     }
    //     String pkgName = uri.getSchemeSpecificPart();
    //     if (pkgName != NULL) {
    //         pkgList = new String[] { pkgName };
    //     }
    //     added = Intent.ACTION_PACKAGE_ADDED.equals(action);
    //     replacing = extras.getBoolean(Intent.EXTRA_REPLACING, FALSE);
    // } else if (Intent.ACTION_EXTERNAL_APPLICATIONS_AVAILABLE.equals(action)) {
    //     added = TRUE;
    //     pkgList = intent.getStringArrayExtra(Intent.EXTRA_CHANGED_PACKAGE_LIST);
    // } else if (Intent.ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE.equals(action)) {
    //     added = FALSE;
    //     pkgList = intent.getStringArrayExtra(Intent.EXTRA_CHANGED_PACKAGE_LIST);
    // }
    // if (pkgList == NULL || pkgList.length == 0) {
    //     return;
    // }
    // if (added) {
    //     synchronized (mBackupParticipants) {
    //         for (String pkgName : pkgList) {
    //             if (replacing) {
    //                 // The package was just upgraded
    //                 updatePackageParticipantsLocked(pkgName);
    //             } else {
    //                 // The package was just added
    //                 addPackageParticipantsLocked(pkgName);
    //             }
    //         }
    //     }
    // } else {
    //     if (replacing) {
    //         // The package is being updated.  We'll receive a PACKAGE_ADDED shortly.
    //     } else {
    //         synchronized (mBackupParticipants) {
    //             for (String pkgName : pkgList) {
    //                 removePackageParticipantsLocked(pkgName);
    //             }
    //         }
    //     }
    // }

    return E_NOT_IMPLEMENTED;
}

CBackupManagerService::CBackupManagerService()
    : mPackageManager(NULL)
    , mEnabled(FALSE)
    , mProvisioned(FALSE)
    , mAutoRestore(FALSE)
    , mConnecting(FALSE)
    , mLastBackupPass(0)
    , mNextBackupPass(0)
    , mClearingData(FALSE)
    , mCurrentOperations(NULL)
    , mAncestralToken(0)
    , mCurrentToken(0)
    , mBroadcastReceiver(NULL)
{
    mCurrentOperations = new SparseInt32Array();
}

CBackupManagerService::~CBackupManagerService()
{
    if (mCurrentOperations != NULL) {
        delete mCurrentOperations;
        mCurrentOperations = NULL;
    }
}

ECode CBackupManagerService::constructor(
    /* [in] */ IContext* ctx)
{
    // mContext = context;
    // mPackageManager = context.getPackageManager();
    // mPackageManagerBinder = AppGlobals.getPackageManager();
    // mActivityManager = ActivityManagerNative.getDefault();

    // mAlarmManager = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
    // mPowerManager = (PowerManager) context.getSystemService(Context.POWER_SERVICE);

    // mBackupManagerBinder = asInterface(asBinder());

    // // spin up the backup/restore handler thread
    // mHandlerThread = new HandlerThread("backup", Process.THREAD_PRIORITY_BACKGROUND);
    // mHandlerThread.start();
    // mBackupHandler = new BackupHandler(mHandlerThread.getLooper());

    // mBroadcastReceiver = new TrackCapsuleInstallAndRemoval();

    // // Set up our bookkeeping
    // boolean areEnabled = Settings.Secure.getInt(context.getContentResolver(),
    //         Settings.Secure.BACKUP_ENABLED, 0) != 0;
    // mProvisioned = Settings.Secure.getInt(context.getContentResolver(),
    //         Settings.Secure.BACKUP_PROVISIONED, 0) != 0;
    // mAutoRestore = Settings.Secure.getInt(context.getContentResolver(),
    //         Settings.Secure.BACKUP_AUTO_RESTORE, 1) != 0;
    // // If Encrypted file systems is enabled or disabled, this call will return the
    // // correct directory.
    // mBaseStateDir = new File(Environment.getSecureDataDirectory(), "backup");
    // mBaseStateDir.mkdirs();
    // mDataDir = Environment.getDownloadCacheDirectory();

    // // Alarm receivers for scheduled backups & initialization operations
    // mRunBackupReceiver = new RunBackupReceiver();
    // IntentFilter filter = new IntentFilter();
    // filter.addAction(RUN_BACKUP_ACTION);
    // context.registerReceiver(mRunBackupReceiver, filter,
    //         android.Manifest.permission.BACKUP, NULL);

    // mRunInitReceiver = new RunInitializeReceiver();
    // filter = new IntentFilter();
    // filter.addAction(RUN_INITIALIZE_ACTION);
    // context.registerReceiver(mRunInitReceiver, filter,
    //         android.Manifest.permission.BACKUP, NULL);

    // Intent backupIntent = new Intent(RUN_BACKUP_ACTION);
    // backupIntent.addFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY);
    // mRunBackupIntent = PendingIntent.getBroadcast(context, MSG_RUN_BACKUP, backupIntent, 0);

    // Intent initIntent = new Intent(RUN_INITIALIZE_ACTION);
    // backupIntent.addFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY);
    // mRunInitIntent = PendingIntent.getBroadcast(context, MSG_RUN_INITIALIZE, initIntent, 0);

    // // Set up the backup-request journaling
    // mJournalDir = new File(mBaseStateDir, "pending");
    // mJournalDir.mkdirs();   // creates mBaseStateDir along the way
    // mJournal = NULL;        // will be created on first use

    // // Set up the various sorts of package tracking we do
    // initPackageTracking();

    // // Build our mapping of uid to backup client services.  This implicitly
    // // schedules a backup pass on the Package Manager metadata the first
    // // time anything needs to be backed up.
    // synchronized (mBackupParticipants) {
    //     addPackageParticipantsLocked(NULL);
    // }

    // // Set up our transport options and initialize the default transport
    // // TODO: Have transports register themselves somehow?
    // // TODO: Don't create transports that we don't need to?
    // mLocalTransport = new LocalTransport(context);  // This is actually pretty cheap
    // ComponentName localName = new ComponentName(context, LocalTransport.class);
    // registerTransport(localName.flattenToShortString(), mLocalTransport);

    // mGoogleTransport = NULL;
    // mCurrentTransport = Settings.Secure.getString(context.getContentResolver(),
    //         Settings.Secure.BACKUP_TRANSPORT);
    // if ("".equals(mCurrentTransport)) {
    //     mCurrentTransport = NULL;
    // }
    // if (DEBUG) Slog.v(TAG, "Starting with transport " + mCurrentTransport);

    // // Attach to the Google backup transport.  When this comes up, it will set
    // // itself as the current transport because we explicitly reset mCurrentTransport
    // // to NULL.
    // ComponentName transportComponent = new ComponentName("com.google.android.backup",
    //         "com.google.android.backup.BackupTransportService");
    // try {
    //     // If there's something out there that is supposed to be the Google
    //     // backup transport, make sure it's legitimately part of the OS build
    //     // and not an app lying about its package name.
    //     AutoPtr<IApplicationInfo> info = mPackageManager.getApplicationInfo(
    //             transportComponent.getPackageName(), 0);
    //     if ((info.flags & ApplicationInfo_FLAG_SYSTEM) != 0) {
    //         if (DEBUG) Slog.v(TAG, "Binding to Google transport");
    //         Intent intent = new Intent().setComponent(transportComponent);
    //         mGoogleConnection = new GroupServiceConnection();
    //         context.bindService(intent, mGoogleConnection, Context.BIND_AUTO_CREATE);
    //     } else {
    //         Slog.w(TAG, "Possible Google transport spoof: ignoring " + info);
    //     }
    // } catch (PackageManager.NameNotFoundException nnf) {
    //     // No such package?  No binding.
    //     if (DEBUG) Slog.v(TAG, "Google transport not present");
    // }

    // // Now that we know about valid backup participants, parse any
    // // leftover journal files into the pending backup set
    // parseLeftoverJournals();

    // // Power management
    // mWakelock = mPowerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "*backup*");

    // // Start the backup passes going
    // setBackupEnabled(areEnabled);

    return E_NOT_IMPLEMENTED;
}

ECode CBackupManagerService::DataChanged(
    /* [in] */ const String& packageName)
{
    // final HashSet<AutoPtr<IApplicationInfo> > targets = dataChangedTargets(packageName);
    // if (targets == NULL) {
    //     Slog.w(TAG, "dataChanged but no participant pkg='" + packageName + "'"
    //            + " uid=" + Binder.getCallingUid());
    //     return;
    // }

    // mBackupHandler.post(new Runnable() {
    //         public void run() {
    //             dataChangedImpl(packageName, targets);
    //         }
    //     });

    return E_NOT_IMPLEMENTED;
}

// Clear the given package's backup data from the current transport
ECode CBackupManagerService::ClearBackupData(
    /* [in] */ const String& packageName)
{
    // if (DEBUG) Slog.v(TAG, "clearBackupData() of " + packageName);
    // PackageInfo info;
    // try {
    //     info = mPackageManager.getPackageInfo(packageName, PackageManager.GET_SIGNATURES);
    // } catch (NameNotFoundException e) {
    //     Slog.d(TAG, "No such package '" + packageName + "' - not clearing backup data");
    //     return;
    // }

    // // If the caller does not hold the BACKUP permission, it can only request a
    // // wipe of its own backed-up data.
    // HashSet<AutoPtr<IApplicationInfo> > apps;
    // if ((mContext.checkPermission(android.Manifest.permission.BACKUP, Binder.getCallingPid(),
    //         Binder.getCallingUid())) == PackageManager.PERMISSION_DENIED) {
    //     apps = mBackupParticipants.get(Binder.getCallingUid());
    // } else {
    //     // a caller with full permission can ask to back up any participating app
    //     // !!! TODO: allow data-clear of ANY app?
    //     if (DEBUG) Slog.v(TAG, "Privileged caller, allowing clear of other apps");
    //     apps = new HashSet<AutoPtr<IApplicationInfo>>();
    //     Int32 N = mBackupParticipants.size();
    //     for (Int32 i = 0; i < N; i++) {
    //         HashSet<AutoPtr<IApplicationInfo>> s = mBackupParticipants.valueAt(i);
    //         if (s != NULL) {
    //             apps.addAll(s);
    //         }
    //     }
    // }

    // // now find the given package in the set of candidate apps
    // for (IApplicationInfo app : apps) {
    //     if (app.packageName.equals(packageName)) {
    //         if (DEBUG) Slog.v(TAG, "Found the app - running clear process");
    //         // found it; fire off the clear request
    //         {
    //             Mutex::Autolock lock(mQueueLock);
    //             Int64 oldId = Binder.clearCallingIdentity();
    //             mWakelock.acquire();
    //             Message msg = mBackupHandler.obtainMessage(MSG_RUN_CLEAR,
    //                     new ClearParams(getTransport(mCurrentTransport), info));
    //             mBackupHandler.sendMessage(msg);
    //             Binder.restoreCallingIdentity(oldId);
    //         }
    //         break;
    //     }
    // }

    return E_NOT_IMPLEMENTED;
}

// Run a backup pass immediately for any applications that have declared
// that they have pending updates.
ECode CBackupManagerService::BackupNow()
{
    // mContext.enforceCallingOrSelfPermission(android.Manifest.permission.BACKUP, "backupNow");

    // if (DEBUG) Slog.v(TAG, "Scheduling immediate backup pass");
    // {
    //     Mutex::Autolock lock(mQueueLock);
    //     // Because the alarms we are using can jitter, and we want an *immediate*
    //     // backup pass to happen, we restart the timer beginning with "next time,"
    //     // then manually fire the backup trigger intent ourselves.
    //     startBackupAlarmsLocked(BACKUP_INTERVAL);
    //     try {
    //         mRunBackupIntent.send();
    //     } catch (PendingIntent.CanceledException e) {
    //         // should never happen
    //         Slog.e(TAG, "run-backup intent cancelled!");
    //     }
    // }

    return E_NOT_IMPLEMENTED;
}

// Enable/disable the backup service
ECode CBackupManagerService::SetBackupEnabled(
    /* [in] */ Boolean enable)
{
    // mContext.enforceCallingOrSelfPermission(android.Manifest.permission.BACKUP,
    //         "setBackupEnabled");

    // Slog.i(TAG, "Backup enabled => " + enable);

    // boolean wasEnabled = mEnabled;
    // synchronized (this) {
    //     Settings.Secure.putInt(mContext.getContentResolver(),
    //             Settings.Secure.BACKUP_ENABLED, enable ? 1 : 0);
    //     mEnabled = enable;
    // }

    // {
    //     Mutex::Autolock lock(mQueueLock);
    //     if (enable && !wasEnabled && mProvisioned) {
    //         // if we've just been enabled, start scheduling backup passes
    //         startBackupAlarmsLocked(BACKUP_INTERVAL);
    //     } else if (!enable) {
    //         // No longer enabled, so stop running backups
    //         if (DEBUG) Slog.i(TAG, "Opting out of backup");

    //         mAlarmManager.cancel(mRunBackupIntent);

    //         // This also constitutes an opt-out, so we wipe any data for
    //         // this device from the backend.  We start that process with
    //         // an alarm in order to guarantee wakelock states.
    //         if (wasEnabled && mProvisioned) {
    //             // NOTE: we currently flush every registered transport, not just
    //             // the currently-active one.
    //             HashSet<String> allTransports;
    //             synchronized (mTransports) {
    //                 allTransports = new HashSet<String>(mTransports.keySet());
    //             }
    //             // build the set of transports for which we are posting an init
    //             for (String transport : allTransports) {
    //                 recordInitPendingLocked(TRUE, transport);
    //             }
    //             mAlarmManager.set(AlarmManager.RTC_WAKEUP, System.currentTimeMillis(),
    //                     mRunInitIntent);
    //         }
    //     }
    // }

    return E_NOT_IMPLEMENTED;
}

// Enable/disable automatic restore of app data at install time
ECode CBackupManagerService::SetAutoRestore(
    /* [in] */ Boolean doAutoRestore)
{
    // mContext.enforceCallingOrSelfPermission(android.Manifest.permission.BACKUP,
    // "setBackupEnabled");

    // Slog.i(TAG, "Auto restore => " + doAutoRestore);

    // synchronized (this) {
    //     Settings.Secure.putInt(mContext.getContentResolver(),
    //             Settings.Secure.BACKUP_AUTO_RESTORE, doAutoRestore ? 1 : 0);
    //     mAutoRestore = doAutoRestore;
    // }

    return E_NOT_IMPLEMENTED;
}

// Mark the backup service as having been provisioned
ECode CBackupManagerService::SetBackupProvisioned(
    /* [in] */ Boolean available)
{
    // mContext.enforceCallingOrSelfPermission(android.Manifest.permission.BACKUP,
    //         "setBackupProvisioned");

    // boolean wasProvisioned = mProvisioned;
    // synchronized (this) {
    //     Settings.Secure.putInt(mContext.getContentResolver(),
    //             Settings.Secure.BACKUP_PROVISIONED, available ? 1 : 0);
    //     mProvisioned = available;
    // }

    // {
    //     Mutex::Autolock lock(mQueueLock);
    //     if (available && !wasProvisioned && mEnabled) {
    //         // we're now good to go, so start the backup alarms
    //         startBackupAlarmsLocked(FIRST_BACKUP_INTERVAL);
    //     } else if (!available) {
    //         // No longer enabled, so stop running backups
    //         Slog.w(TAG, "Backup service no longer provisioned");
    //         mAlarmManager.cancel(mRunBackupIntent);
    //     }
    // }

    return E_NOT_IMPLEMENTED;
}

// Report whether the backup mechanism is currently enabled
ECode CBackupManagerService::IsBackupEnabled(
    /* [out] */ Boolean* enabled)
{
    //TODO
    // mContext.enforceCallingOrSelfPermission(android.Manifest.permission.BACKUP, "isBackupEnabled");
    return mEnabled;    // no need to synchronize just to read it
}

// Report the name of the currently active transport
ECode CBackupManagerService::GetCurrentTransport(
    /* [out] */ String* transport)
{
    assert(transport != NULL);

    //TODO
    // mContext.enforceCallingOrSelfPermission(android.Manifest.permission.BACKUP,
    //         "getCurrentTransport");
    if (DEBUG) Slogger::V(TAG, "... getCurrentTransport() returning " + mCurrentTransport);

    *transport = mCurrentTransport;
    return NOERROR;
}

// Report all known, available backup transports
ECode CBackupManagerService::ListAllTransports(
    /*[out, callee]*/ BufferOf<String>** transports)
{
    // mContext.enforceCallingOrSelfPermission(android.Manifest.permission.BACKUP, "listAllTransports");

    // String[] list = NULL;
    // ArrayList<String> known = new ArrayList<String>();
    // for (Map.Entry<String, IBackupTransport> entry : mTransports.entrySet()) {
    //     if (entry.getValue() != NULL) {
    //         known.add(entry.getKey());
    //     }
    // }

    // if (known.size() > 0) {
    //     list = new String[known.size()];
    //     known.toArray(list);
    // }
    // return list;

    return E_NOT_IMPLEMENTED;
}

// Select which transport to use for the next backup operation.  If the given
// name is not one of the available transports, no action is taken and the method
// returns NULL.
ECode CBackupManagerService::SelectBackupTransport(
    /* [in] */ const String& transport,
    /* [out] */ String* port)
{
    // mContext.enforceCallingOrSelfPermission(android.Manifest.permission.BACKUP, "selectBackupTransport");

    // synchronized (mTransports) {
    //     String prevTransport = NULL;
    //     if (mTransports.get(transport) != NULL) {
    //         prevTransport = mCurrentTransport;
    //         mCurrentTransport = transport;
    //         Settings.Secure.putString(mContext.getContentResolver(),
    //                 Settings.Secure.BACKUP_TRANSPORT, transport);
    //         Slog.v(TAG, "selectBackupTransport() set " + mCurrentTransport
    //                 + " returning " + prevTransport);
    //     } else {
    //         Slog.w(TAG, "Attempt to select unavailable transport " + transport);
    //     }
    //     return prevTransport;
    // }

    return E_NOT_IMPLEMENTED;
}

// Callback: a requested backup agent has been instantiated.  This should only
// be called from the Activity Manager.
ECode CBackupManagerService::AgentConnected(
    /* [in] */ const String& packageName,
    /* [in] */ IBinder* agentBinder)
{
    // synchronized(mAgentConnectLock) {
    //     if (Binder.getCallingUid() == Process.SYSTEM_UID) {
    //         Slog.d(TAG, "agentConnected pkg=" + packageName + " agent=" + agentBinder);
    //         IBackupAgent agent = IBackupAgent.Stub.asInterface(agentBinder);
    //         mConnectedAgent = agent;
    //         mConnecting = FALSE;
    //     } else {
    //         Slog.w(TAG, "Non-system process uid=" + Binder.getCallingUid()
    //                 + " claiming agent connected");
    //     }
    //     mAgentConnectLock.notifyAll();
    // }

    return E_NOT_IMPLEMENTED;
}

// Callback: a backup agent has failed to come up, or has unexpectedly quit.
// If the agent failed to come up in the first place, the agentBinder argument
// will be NULL.  This should only be called from the Activity Manager.
ECode CBackupManagerService::AgentDisconnected(
    /* [in] */ const String& packageName)
{
    // // TODO: handle backup being interrupted
    // synchronized(mAgentConnectLock) {
    //     if (Binder.getCallingUid() == Process.SYSTEM_UID) {
    //         mConnectedAgent = NULL;
    //         mConnecting = FALSE;
    //     } else {
    //         Slog.w(TAG, "Non-system process uid=" + Binder.getCallingUid()
    //                 + " claiming agent disconnected");
    //     }
    //     mAgentConnectLock.notifyAll();
    // }

    return E_NOT_IMPLEMENTED;
}

// An application being installed will need a restore pass, then the Package Manager
// will need to be told when the restore is finished.
ECode CBackupManagerService::RestoreAtInstall(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 token)
{
    // if (Binder.getCallingUid() != Process.SYSTEM_UID) {
    //     Slog.w(TAG, "Non-system process uid=" + Binder.getCallingUid()
    //             + " attemping install-time restore");
    //     return;
    // }

    // Int64 restoreSet = getAvailableRestoreToken(packageName);
    // if (DEBUG) Slog.v(TAG, "restoreAtInstall pkg=" + packageName
    //         + " token=" + Integer.toHexString(token));

    // if (mAutoRestore && mProvisioned && restoreSet != 0) {
    //     // okay, we're going to attempt a restore of this package from this restore set.
    //     // The eventual message back into the Package Manager to run the post-install
    //     // steps for 'token' will be issued from the restore handling code.

    //     // We can use a synthetic PackageInfo here because:
    //     //   1. We know it's valid, since the Package Manager supplied the name
    //     //   2. Only the packageName field will be used by the restore code
    //     PackageInfo pkg = new PackageInfo();
    //     pkg.packageName = packageName;

    //     mWakelock.acquire();
    //     Message msg = mBackupHandler.obtainMessage(MSG_RUN_RESTORE);
    //     msg.obj = new RestoreParams(getTransport(mCurrentTransport), NULL,
    //             restoreSet, pkg, token, TRUE);
    //     mBackupHandler.sendMessage(msg);
    // } else {
    //     // Auto-restore disabled or no way to attempt a restore; just tell the Package
    //     // Manager to proceed with the post-install handling for this package.
    //     if (DEBUG) Slog.v(TAG, "No restore set -- skipping restore");
    //     try {
    //         mPackageManagerBinder.finishPackageInstall(token);
    //     } catch (RemoteException e) { /* can't happen */ }
    // }

    return E_NOT_IMPLEMENTED;
}

// Hand off a restore session
ECode CBackupManagerService::BeginRestoreSession(
    /* [in] */ const String& packageName,
    /* [in] */ const String& transport,
    /* [out] */ IRestoreSessionStub** session)
{
    // if (DEBUG) Slog.v(TAG, "beginRestoreSession: pkg=" + packageName
    //         + " transport=" + transport);

    // boolean needPermission = TRUE;
    // if (transport == NULL) {
    //     transport = mCurrentTransport;

    //     if (packageName != NULL) {
    //         PackageInfo app = NULL;
    //         try {
    //             app = mPackageManager.getPackageInfo(packageName, 0);
    //         } catch (NameNotFoundException nnf) {
    //             Slog.w(TAG, "Asked to restore nonexistent pkg " + packageName);
    //             throw new IllegalArgumentException("Package " + packageName + " not found");
    //         }

    //         if (app.applicationInfo.uid == Binder.getCallingUid()) {
    //             // So: using the current active transport, and the caller has asked
    //             // that its own package will be restored.  In this narrow use case
    //             // we do not require the caller to hold the permission.
    //             needPermission = FALSE;
    //         }
    //     }
    // }

    // if (needPermission) {
    //     mContext.enforceCallingOrSelfPermission(android.Manifest.permission.BACKUP,
    //             "beginRestoreSession");
    // } else {
    //     if (DEBUG) Slog.d(TAG, "restoring self on current transport; no permission needed");
    // }

    // synchronized(this) {
    //     if (mActiveRestoreSession != NULL) {
    //         Slog.d(TAG, "Restore session requested but one already active");
    //         return NULL;
    //     }
    //     mActiveRestoreSession = new ActiveRestoreSession(packageName, transport);
    // }
    // return mActiveRestoreSession;

    return E_NOT_IMPLEMENTED;
}

// Note that a currently-active backup agent has notified us that it has
// completed the given outstanding asynchronous backup/restore operation.
ECode CBackupManagerService::OpComplete(
    /* [in] */ Int32 token)
{
    // synchronized (mCurrentOpLock) {
    //     if (DEBUG) Slog.v(TAG, "opComplete: " + Integer.toHexString(token));
    //     mCurrentOperations.put(token, OP_ACKNOWLEDGED);
    //     mCurrentOpLock.notifyAll();
    // }

    return E_NOT_IMPLEMENTED;
}

void CBackupManagerService::InitPackageTracking()
{
    // if (DEBUG) Slog.v(TAG, "Initializing package tracking");

    // // Remember our ancestral dataset
    // mTokenFile = new File(mBaseStateDir, "ancestral");
    // try {
    //     RandomAccessFile tf = new RandomAccessFile(mTokenFile, "r");
    //     Int32 version = tf.readInt();
    //     if (version == CURRENT_ANCESTRAL_RECORD_VERSION) {
    //         mAncestralToken = tf.readLong();
    //         mCurrentToken = tf.readLong();

    //         Int32 numPackages = tf.readInt();
    //         if (numPackages >= 0) {
    //             mAncestralPackages = new HashSet<String>();
    //             for (Int32 i = 0; i < numPackages; i++) {
    //                 String pkgName = tf.readUTF();
    //                 mAncestralPackages.add(pkgName);
    //             }
    //         }
    //     }
    // } catch (FileNotFoundException fnf) {
    //     // Probably innocuous
    //     Slog.v(TAG, "No ancestral data");
    // } catch (IOException e) {
    //     Slog.w(TAG, "Unable to read token file", e);
    // }

    // // Keep a log of what apps we've ever backed up.  Because we might have
    // // rebooted in the middle of an operation that was removing something from
    // // this log, we sanity-check its contents here and reconstruct it.
    // mEverStored = new File(mBaseStateDir, "processed");
    // File tempProcessedFile = new File(mBaseStateDir, "processed.new");

    // // If we were in the middle of removing something from the ever-backed-up
    // // file, there might be a transient "processed.new" file still present.
    // // Ignore it -- we'll validate "processed" against the current package set.
    // if (tempProcessedFile.exists()) {
    //     tempProcessedFile.delete();
    // }

    // // If there are previous contents, parse them out then start a new
    // // file to continue the recordkeeping.
    // if (mEverStored.exists()) {
    //     RandomAccessFile temp = NULL;
    //     RandomAccessFile in = NULL;

    //     try {
    //         temp = new RandomAccessFile(tempProcessedFile, "rws");
    //         in = new RandomAccessFile(mEverStored, "r");

    //         while (TRUE) {
    //             PackageInfo info;
    //             String pkg = in.readUTF();
    //             try {
    //                 info = mPackageManager.getPackageInfo(pkg, 0);
    //                 mEverStoredApps.add(pkg);
    //                 temp.writeUTF(pkg);
    //                 if (DEBUG) Slog.v(TAG, "   + " + pkg);
    //             } catch (NameNotFoundException e) {
    //                 // nope, this package was uninstalled; don't include it
    //                 if (DEBUG) Slog.v(TAG, "   - " + pkg);
    //             }
    //         }
    //     } catch (EOFException e) {
    //         // Once we've rewritten the backup history log, atomically replace the
    //         // old one with the new one then reopen the file for continuing use.
    //         if (!tempProcessedFile.renameTo(mEverStored)) {
    //             Slog.e(TAG, "Error renaming " + tempProcessedFile + " to " + mEverStored);
    //         }
    //     } catch (IOException e) {
    //         Slog.e(TAG, "Error in processed file", e);
    //     } finally {
    //         try { if (temp != NULL) temp.close(); } catch (IOException e) {}
    //         try { if (in != NULL) in.close(); } catch (IOException e) {}
    //     }
    // }

    // // Register for broadcasts about package install, etc., so we can
    // // update the provider list.
    // IntentFilter filter = new IntentFilter();
    // filter.addAction(Intent.ACTION_PACKAGE_ADDED);
    // filter.addAction(Intent.ACTION_PACKAGE_REMOVED);
    // filter.addDataScheme("package");
    // mContext.registerReceiver(mBroadcastReceiver, filter);
    // // Register for events related to sdcard installation.
    // IntentFilter sdFilter = new IntentFilter();
    // sdFilter.addAction(Intent.ACTION_EXTERNAL_APPLICATIONS_AVAILABLE);
    // sdFilter.addAction(Intent.ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE);
    // mContext.registerReceiver(mBroadcastReceiver, sdFilter);
}

void CBackupManagerService::ParseLeftoverJournals()
{
    // for (File f : mJournalDir.listFiles()) {
    //     if (mJournal == NULL || f.compareTo(mJournal) != 0) {
    //         // This isn't the current journal, so it must be a leftover.  Read
    //         // out the package names mentioned there and schedule them for
    //         // backup.
    //         RandomAccessFile in = NULL;
    //         try {
    //             Slog.i(TAG, "Found stale backup journal, scheduling");
    //             in = new RandomAccessFile(f, "r");
    //             while (TRUE) {
    //                 String packageName = in.readUTF();
    //                 Slog.i(TAG, "  " + packageName);
    //                 dataChangedImpl(packageName);
    //             }
    //         } catch (EOFException e) {
    //             // no more data; we're done
    //         } catch (Exception e) {
    //             Slog.e(TAG, "Can't read " + f, e);
    //         } finally {
    //             // close/delete the file
    //             try { if (in != NULL) in.close(); } catch (IOException e) {}
    //             f.delete();
    //         }
    //     }
    // }
}

void CBackupManagerService::RecordInitPendingLocked(
    /* [in] */ Boolean isPending,
    /* [in] */ const String& transportName)
{
    // if (DEBUG) Slog.i(TAG, "recordInitPendingLocked: " + isPending
    //         + " on transport " + transportName);
    // try {
    //     IBackupTransport transport = getTransport(transportName);
    //     String transportDirName = transport.transportDirName();
    //     File stateDir = new File(mBaseStateDir, transportDirName);
    //     File initPendingFile = new File(stateDir, INIT_SENTINEL_FILE_NAME);

    //     if (isPending) {
    //         // We need an init before we can proceed with sending backup data.
    //         // Record that with an entry in our set of pending inits, as well as
    //         // journaling it via creation of a sentinel file.
    //         mPendingInits.add(transportName);
    //         try {
    //             (new FileOutputStream(initPendingFile)).close();
    //         } catch (IOException ioe) {
    //             // Something is badly wrong with our permissions; just try to move on
    //         }
    //     } else {
    //         // No more initialization needed; wipe the journal and reset our state.
    //         initPendingFile.delete();
    //         mPendingInits.remove(transportName);
    //     }
    // } catch (RemoteException e) {
    //     // can't happen; the transport is local
    // }
}

void CBackupManagerService::ResetBackupState(
    /* [in] */ IFile* stateFileDir)
{
    // {
    //     Mutex::Autolock lock(mQueueLock);
    //     // Wipe the "what we've ever backed up" tracking
    //     mEverStoredApps.clear();
    //     mEverStored.delete();

    //     mCurrentToken = 0;
    //     writeRestoreTokens();

    //     // Remove all the state files
    //     for (File sf : stateFileDir.listFiles()) {
    //         // ... but don't touch the needs-init sentinel
    //         if (!sf.getName().equals(INIT_SENTINEL_FILE_NAME)) {
    //             sf.delete();
    //         }
    //     }

    //     // Enqueue a new backup of every participant
    //     Int32 N = mBackupParticipants.size();
    //     for (Int32 i=0; i<N; i++) {
    //         Int32 uid = mBackupParticipants.keyAt(i);
    //         HashSet<AutoPtr<IApplicationInfo> > participants = mBackupParticipants.valueAt(i);
    //         for (IApplicationInfo app: participants) {
    //             dataChangedImpl(app.packageName);
    //         }
    //     }
    // }
}

void CBackupManagerService::RegisterTransport(
    /* [in] */ const String& name,
    /* [in] */ IBackupTransport* transport)
{
    // synchronized (mTransports) {
    //     if (DEBUG) Slog.v(TAG, "Registering transport " + name + " = " + transport);
    //     if (transport != NULL) {
    //         mTransports.put(name, transport);
    //     } else {
    //         mTransports.remove(name);
    //         if ((mCurrentTransport != NULL) && mCurrentTransport.equals(name)) {
    //             mCurrentTransport = NULL;
    //         }
    //         // Nothing further to do in the unregistration case
    //         return;
    //     }
    // }

    // // If the init sentinel file exists, we need to be sure to perform the init
    // // as soon as practical.  We also create the state directory at registration
    // // time to ensure it's present from the outset.
    // try {
    //     String transportName = transport.transportDirName();
    //     File stateDir = new File(mBaseStateDir, transportName);
    //     stateDir.mkdirs();

    //     File initSentinel = new File(stateDir, INIT_SENTINEL_FILE_NAME);
    //     if (initSentinel.exists()) {
    //         {
    //             Mutex::Autolock lock(mQueueLock);
    //             mPendingInits.add(transportName);

    //             // TODO: pick a better starting time than now + 1 minute
    //             Int64 delay = 1000 * 60; // one minute, in milliseconds
    //             mAlarmManager.set(AlarmManager.RTC_WAKEUP,
    //                     System.currentTimeMillis() + delay, mRunInitIntent);
    //         }
    //     }
    // } catch (RemoteException e) {
    //     // can't happen, the transport is local
    // }
}

List<AutoPtr<ICapsuleInfo> >* CBackupManagerService::AllAgentPackages()
{
    // // !!! TODO: cache this and regenerate only when necessary
    // Int32 flags = PackageManager.GET_SIGNATURES;
    // List<PackageInfo> packages = mPackageManager.getInstalledPackages(flags);
    // Int32 N = packages.size();
    // for (Int32 a = N-1; a >= 0; a--) {
    //     PackageInfo pkg = packages.get(a);
    //     try {
    //         AutoPtr<IApplicationInfo> app = pkg.applicationInfo;
    //         if (((app.flags & ApplicationInfo_FLAG_ALLOW_BACKUP) == 0)
    //                 || app.backupAgentName == NULL) {
    //             packages.remove(a);
    //         }
    //         else {
    //             // we will need the shared library path, so look that up and store it here
    //             app = mPackageManager.getApplicationInfo(pkg.packageName,
    //                     PackageManager.GET_SHARED_LIBRARY_FILES);
    //             pkg.applicationInfo.sharedLibraryFiles = app.sharedLibraryFiles;
    //         }
    //     } catch (NameNotFoundException e) {
    //         packages.remove(a);
    //     }
    // }
    // return packages;

    return NULL;
}

// Reset the given package's known backup participants.  Unlike add/remove, the update
// action cannot be passed a NULL package name.
void CBackupManagerService::UpdatePackageParticipantsLocked(
    /* [in] */ const String& packageName)
{
    // if (packageName == NULL) {
    //     Slog.e(TAG, "updatePackageParticipants called with NULL package name");
    //     return;
    // }
    // if (DEBUG) Slog.v(TAG, "updatePackageParticipantsLocked: " + packageName);

    // // brute force but small code size
    // List<PackageInfo> allApps = allAgentPackages();
    // removePackageParticipantsLockedInner(packageName, allApps);
    // addPackageParticipantsLockedInner(packageName, allApps);
}

// Called from the backup task: record that the given app has been successfully
// backed up at least once
void CBackupManagerService::LogBackupComplete(
    /* [in] */ const String& packageName)
{
    // if (packageName.equals(PACKAGE_MANAGER_SENTINEL)) return;

    // synchronized (mEverStoredApps) {
    //     if (!mEverStoredApps.add(packageName)) return;

    //     RandomAccessFile out = NULL;
    //     try {
    //         out = new RandomAccessFile(mEverStored, "rws");
    //         out.seek(out.length());
    //         out.writeUTF(packageName);
    //     } catch (IOException e) {
    //         Slog.e(TAG, "Can't log backup of " + packageName + " to " + mEverStored);
    //     } finally {
    //         try { if (out != NULL) out.close(); } catch (IOException e) {}
    //     }
    // }
}

// Remove our awareness of having ever backed up the given package
void CBackupManagerService::RemoveEverBackedUp(
    /* [in] */ const String& packageName)
{
    // if (DEBUG) Slog.v(TAG, "Removing backed-up knowledge of " + packageName + ", new set:");

    // synchronized (mEverStoredApps) {
    //     // Rewrite the file and rename to overwrite.  If we reboot in the middle,
    //     // we'll recognize on initialization time that the package no longer
    //     // exists and fix it up then.
    //     File tempKnownFile = new File(mBaseStateDir, "processed.new");
    //     RandomAccessFile known = NULL;
    //     try {
    //         known = new RandomAccessFile(tempKnownFile, "rws");
    //         mEverStoredApps.remove(packageName);
    //         for (String s : mEverStoredApps) {
    //             known.writeUTF(s);
    //             if (DEBUG) Slog.v(TAG, "    " + s);
    //         }
    //         known.close();
    //         known = NULL;
    //         if (!tempKnownFile.renameTo(mEverStored)) {
    //             throw new IOException("Can't rename " + tempKnownFile + " to " + mEverStored);
    //         }
    //     } catch (IOException e) {
    //         // Bad: we couldn't create the new copy.  For safety's sake we
    //         // abandon the whole process and remove all what's-backed-up
    //         // state entirely, meaning we'll force a backup pass for every
    //         // participant on the next boot or [re]install.
    //         Slog.w(TAG, "Error rewriting " + mEverStored, e);
    //         mEverStoredApps.clear();
    //         tempKnownFile.delete();
    //         mEverStored.delete();
    //     } finally {
    //         try { if (known != NULL) known.close(); } catch (IOException e) {}
    //     }
    // }
}

// Persistently record the current and ancestral backup tokens as well
// as the set of packages with data [supposedly] available in the
// ancestral dataset.
void CBackupManagerService::WriteRestoreTokens()
{
    // try {
    //     RandomAccessFile af = new RandomAccessFile(mTokenFile, "rwd");

    //     // First, the version number of this record, for futureproofing
    //     af.writeInt(CURRENT_ANCESTRAL_RECORD_VERSION);

    //     // Write the ancestral and current tokens
    //     af.writeLong(mAncestralToken);
    //     af.writeLong(mCurrentToken);

    //     // Now write the set of ancestral packages
    //     if (mAncestralPackages == NULL) {
    //         af.writeInt(-1);
    //     } else {
    //         af.writeInt(mAncestralPackages.size());
    //         if (DEBUG) Slog.v(TAG, "Ancestral packages:  " + mAncestralPackages.size());
    //         for (String pkgName : mAncestralPackages) {
    //             af.writeUTF(pkgName);
    //             if (DEBUG) Slog.v(TAG, "   " + pkgName);
    //         }
    //     }
    //     af.close();
    // } catch (IOException e) {
    //     Slog.w(TAG, "Unable to write token file:", e);
    // }
}

// Return the given transport
AutoPtr<IBackupTransport> CBackupManagerService::GetTransport(
    /* [in] */ const String& transportName)
{
    // synchronized (mTransports) {
    //     IBackupTransport transport = mTransports.get(transportName);
    //     if (transport == NULL) {
    //         Slog.w(TAG, "Requested unavailable transport: " + transportName);
    //     }
    //     return transport;
    // }

    return NULL;
}

// fire off a backup agent, blocking until it attaches or times out
AutoPtr<IBackupAgent> CBackupManagerService::BindToAgentSynchronous(
    /* [in] */ IApplicationInfo* app,
    /* [in] */ Int32 mode)
{
    // IBackupAgent agent = NULL;
    // synchronized(mAgentConnectLock) {
    //     mConnecting = TRUE;
    //     mConnectedAgent = NULL;
    //     try {
    //         if (mActivityManager.bindBackupAgent(app, mode)) {
    //             Slog.d(TAG, "awaiting agent for " + app);

    //             // success; wait for the agent to arrive
    //             // only wait 10 seconds for the clear data to happen
    //             Int64 timeoutMark = System.currentTimeMillis() + TIMEOUT_INTERVAL;
    //             while (mConnecting && mConnectedAgent == NULL
    //                     && (System.currentTimeMillis() < timeoutMark)) {
    //                 try {
    //                     mAgentConnectLock.wait(5000);
    //                 } catch (InterruptedException e) {
    //                     // just bail
    //                     return NULL;
    //                 }
    //             }

    //             // if we timed out with no connect, abort and move on
    //             if (mConnecting == TRUE) {
    //                 Slog.w(TAG, "Timeout waiting for agent " + app);
    //                 return NULL;
    //             }
    //             agent = mConnectedAgent;
    //         }
    //     } catch (RemoteException e) {
    //         // can't happen
    //     }
    // }
    // return agent;

    return NULL;
}

// clear an application's data, blocking until the operation completes or times out
void CBackupManagerService::ClearApplicationDataSynchronous(
    /* [in] */ const String& packageName)
{
    // // Don't wipe packages marked allowClearUserData=FALSE
    // try {
    //     PackageInfo info = mPackageManager.getPackageInfo(packageName, 0);
    //     if ((info.applicationInfo.flags & ApplicationInfo_FLAG_ALLOW_CLEAR_USER_DATA) == 0) {
    //         if (DEBUG) Slog.i(TAG, "allowClearUserData=FALSE so not wiping "
    //                 + packageName);
    //         return;
    //     }
    // } catch (NameNotFoundException e) {
    //     Slog.w(TAG, "Tried to clear data for " + packageName + " but not found");
    //     return;
    // }

    // ClearDataObserver observer = new ClearDataObserver();

    // synchronized(mClearDataLock) {
    //     mClearingData = TRUE;
    //     try {
    //         mActivityManager.clearApplicationUserData(packageName, observer);
    //     } catch (RemoteException e) {
    //         // can't happen because the activity manager is in this process
    //     }

    //     // only wait 10 seconds for the clear data to happen
    //     Int64 timeoutMark = System.currentTimeMillis() + TIMEOUT_INTERVAL;
    //     while (mClearingData && (System.currentTimeMillis() < timeoutMark)) {
    //         try {
    //             mClearDataLock.wait(5000);
    //         } catch (InterruptedException e) {
    //             // won't happen, but still.
    //             mClearingData = FALSE;
    //         }
    //     }
    // }
}

Int64 CBackupManagerService::GetAvailableRestoreToken(
    /* [in] */ const String& packageName)
{
    // Int64 token = mAncestralToken;
    // {
    //     Mutex::Autolock lock(mQueueLock);
    //     if (mEverStoredApps.contains(packageName)) {
    //         token = mCurrentToken;
    //     }
    // }
    // return token;

    return 0xffff;
}

// -----
// Utility methods used by the asynchronous-with-timeout backup/restore operations
Boolean CBackupManagerService::WaitUntilOperationComplete(
    /* [in] */ Int32 token)
{
    // Int32 finalState = OP_PENDING;
    // synchronized (mCurrentOpLock) {
    //     try {
    //         while ((finalState = mCurrentOperations.get(token, OP_TIMEOUT)) == OP_PENDING) {
    //             try {
    //                 mCurrentOpLock.wait();
    //             } catch (InterruptedException e) {}
    //         }
    //     } catch (IndexOutOfBoundsException e) {
    //         // the operation has been mysteriously cleared from our
    //         // bookkeeping -- consider this a success and ignore it.
    //     }
    // }
    // mBackupHandler.removeMessages(MSG_TIMEOUT);
    // if (DEBUG) Slog.v(TAG, "operation " + Integer.toHexString(token)
    //         + " complete: finalState=" + finalState);
    // return finalState == OP_ACKNOWLEDGED;

    return FALSE;
}

Int64 CBackupManagerService::PrepareOperationTimeout(
    /* [in] */ Int32 token,
    /* [in] */ Int64 interval)
{
    // if (DEBUG) Slog.v(TAG, "starting timeout: token=" + Integer.toHexString(token)
    //         + " interval=" + interval);
    // mCurrentOperations.put(token, OP_PENDING);
    // Message msg = mBackupHandler.obtainMessage(MSG_TIMEOUT, token, 0);
    // mBackupHandler.sendMessageDelayed(msg, interval);

    return 0xffff;
}

Boolean CBackupManagerService::SignaturesMatch(
    /* [in] */ ArrayOf<AutoPtr<ISignature> >* storedSigs,
    /* [in] */ ICapsuleInfo* target)
{
    // // If the target resides on the system partition, we allow it to restore
    // // data from the like-named package in a restore set even if the signatures
    // // do not match.  (Unlike general applications, those flashed to the system
    // // partition will be signed with the device's platform certificate, so on
    // // different phones the same system app will have different signatures.)
    // if ((target.applicationInfo.flags & ApplicationInfo_FLAG_SYSTEM) != 0) {
    //     if (DEBUG) Slog.v(TAG, "System app " + target.packageName + " - skipping sig check");
    //     return TRUE;
    // }

    // // Allow unsigned apps, but not signed on one device and unsigned on the other
    // // !!! TODO: is this the right policy?
    // Signature[] deviceSigs = target.signatures;
    // if (DEBUG) Slog.v(TAG, "signaturesMatch(): stored=" + storedSigs
    //         + " device=" + deviceSigs);
    // if ((storedSigs == NULL || storedSigs.length == 0)
    //         && (deviceSigs == NULL || deviceSigs.length == 0)) {
    //     return TRUE;
    // }
    // if (storedSigs == NULL || deviceSigs == NULL) {
    //     return FALSE;
    // }

    // // !!! TODO: this demands that every stored signature match one
    // // that is present on device, and does not demand the converse.
    // // Is this this right policy?
    // Int32 nStored = storedSigs.length;
    // Int32 nDevice = deviceSigs.length;

    // for (Int32 i=0; i < nStored; i++) {
    //     boolean match = FALSE;
    //     for (Int32 j=0; j < nDevice; j++) {
    //         if (storedSigs[i].equals(deviceSigs[j])) {
    //             match = TRUE;
    //             break;
    //         }
    //     }
    //     if (!match) {
    //         return FALSE;
    //     }
    // }

    return TRUE;
}

void CBackupManagerService::DataChangedImpl(
    /* [in] */ const String& packageName)
{
    // HashSet<AutoPtr<IApplicationInfo> > targets = dataChangedTargets(packageName);
    // dataChangedImpl(packageName, targets);
}

void CBackupManagerService::DataChangedImpl(
    /* [in] */ const String& packageName,
    /* [in] */ HashSet<AutoPtr<IApplicationInfo> >* targets)
{
    // // Record that we need a backup pass for the caller.  Since multiple callers
    // // may share a uid, we need to note all candidates within that uid and schedule
    // // a backup pass for each of them.
    // EventLog.writeEvent(EventLogTags.BACKUP_DATA_CHANGED, packageName);

    // if (targets == NULL) {
    //     Slog.w(TAG, "dataChanged but no participant pkg='" + packageName + "'"
    //            + " uid=" + Binder.getCallingUid());
    //     return;
    // }

    // {
    //     Mutex::Autolock lock(mQueueLock);
    //     // Note that this client has made data changes that need to be backed up
    //     for (ApplicationInfo app : targets) {
    //         // validate the caller-supplied package name against the known set of
    //         // packages associated with this uid
    //         if (app.packageName.equals(packageName)) {
    //             // Add the caller to the set of pending backups.  If there is
    //             // one already there, then overwrite it, but no harm done.
    //             BackupRequest req = new BackupRequest(app, FALSE);
    //             if (mPendingBackups.put(app, req) == NULL) {
    //                 // Journal this request in case of crash.  The put()
    //                 // operation returned NULL when this package was not already
    //                 // in the set; we want to avoid touching the disk redundantly.
    //                 writeToJournalLocked(packageName);

    //                 if (DEBUG) {
    //                     Int32 numKeys = mPendingBackups.size();
    //                     Slog.d(TAG, "Now awaiting backup for " + numKeys + " participants:");
    //                     for (BackupRequest b : mPendingBackups.values()) {
    //                         Slog.d(TAG, "    + " + b + " agent=" + b.appInfo.backupAgentName);
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }
}

// Note: packageName is currently unused, but may be in the future
HashSet<AutoPtr<IApplicationInfo> >* CBackupManagerService::DataChangedTargets(
    /* [in] */ const String& packageName)
{
    // // If the caller does not hold the BACKUP permission, it can only request a
    // // backup of its own data.
    // if ((mContext.checkPermission(android.Manifest.permission.BACKUP, Binder.getCallingPid(),
    //         Binder.getCallingUid())) == PackageManager.PERMISSION_DENIED) {
    //     synchronized (mBackupParticipants) {
    //         return mBackupParticipants.get(Binder.getCallingUid());
    //     }
    // }

    // // a caller with full permission can ask to back up any participating app
    // // !!! TODO: allow backup of ANY app?
    // HashSet<AutoPtr<IApplicationInfo> > targets = new HashSet<AutoPtr<IApplicationInfo> >();
    // synchronized (mBackupParticipants) {
    //     Int32 N = mBackupParticipants.size();
    //     for (Int32 i = 0; i < N; i++) {
    //         HashSet<AutoPtr<IApplicationInfo> > s = mBackupParticipants.valueAt(i);
    //         if (s != NULL) {
    //             targets.addAll(s);
    //         }
    //     }
    // }
    // return targets;

    return NULL;
}

void CBackupManagerService::WriteToJournalLocked(
    /* [in] */ const String& str)
{
    // RandomAccessFile out = NULL;
    // try {
    //     if (mJournal == NULL) mJournal = File.createTempFile("journal", NULL, mJournalDir);
    //     out = new RandomAccessFile(mJournal, "rws");
    //     out.seek(out.length());
    //     out.writeUTF(str);
    // } catch (IOException e) {
    //     Slog.e(TAG, "Can't write " + str + " to backup journal", e);
    //     mJournal = NULL;
    // } finally {
    //     try { if (out != NULL) out.close(); } catch (IOException e) {}
    // }
}

void CBackupManagerService::StartBackupAlarmsLocked(
    /* [in] */ Int64 delayBeforeFirstBackup)
{
    // // We used to use setInexactRepeating(), but that may be linked to
    // // backups running at :00 more often than not, creating load spikes.
    // // Schedule at an exact time for now, and also add a bit of "fuzz".

    // srand(time(NULL));
    // Int64 when = System.currentTimeMillis() + delayBeforeFirstBackup + rand() % FUZZ_MILLIS;
    // mAlarmManager.setRepeating(AlarmManager.RTC_WAKEUP, when,
    //         BACKUP_INTERVAL + rand() % FUZZ_MILLIS, mRunBackupIntent);
    // mNextBackupPass = when;
}

void CBackupManagerService::HandleRunBackup()
{
    // mLastBackupPass = System.currentTimeMillis();
    // mNextBackupPass = mLastBackupPass + BACKUP_INTERVAL;

    // IBackupTransport transport = getTransport(mCurrentTransport);
    // if (transport == NULL) {
    //     Slog.v(TAG, "Backup requested but no transport available");
    //     mWakelock.release();
    //     return;
    // }

    // // snapshot the pending-backup set and work on that
    // ArrayList<BackupRequest> queue = new ArrayList<BackupRequest>();
    // File oldJournal = mJournal;
    // synchronized (mQueueLock) {
    //     // Do we have any work to do?  Construct the work queue
    //     // then release the synchronization lock to actually run
    //     // the backup.
    //     if (mPendingBackups.size() > 0) {
    //         for (BackupRequest b: mPendingBackups.values()) {
    //             queue.add(b);
    //         }
    //         if (DEBUG) Slog.v(TAG, "clearing pending backups");
    //         mPendingBackups.clear();

    //         // Start a new backup-queue journal file too
    //         mJournal = NULL;

    //     }
    // }

    // if (queue.size() > 0) {
    //     // At this point, we have started a new journal file, and the old
    //     // file identity is being passed to the backup processing thread.
    //     // When it completes successfully, that old journal file will be
    //     // deleted.  If we crash prior to that, the old journal is parsed
    //     // at next boot and the journaled requests fulfilled.
    //     (new PerformBackupTask(transport, queue, oldJournal)).run();
    // } else {
    //     Slog.v(TAG, "Backup requested but nothing pending");
    //     mWakelock.release();
    // }
}

void CBackupManagerService::HandleRunInitilize()
{
    // HashSet<String> queue;

    // // Snapshot the pending-init queue and work on that
    // synchronized (mQueueLock) {
    //     queue = new HashSet<String>(mPendingInits);
    //     mPendingInits.clear();
    // }

    // (new PerformInitializeTask(queue)).run();
}
