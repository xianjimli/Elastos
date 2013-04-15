
#include "server/CBackupManagerService.h"
#include <Slogger.h>
#include <StringBuffer.h>
#include <BackupConstants.h>
#include "app/ActivityManagerNative.h"
#include <Settings.h>
#include "os/Environment.h"
#include <elastos/System.h>
#include "os/SystemClock.h"
#include "os/Binder.h"
#include "os/Process.h"

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
    /* [in] */ IRestoreObserverStub* _observer)
    : mTransport(_transport)
    , mSession(_session)
    , mObserver(_observer)
{
}

CBackupManagerService::RestoreParams::RestoreParams(
    /* [in] */ IBackupTransport* _transport,
    /* [in] */ IRestoreObserverStub* _obs,
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
    /* [in] */ IRestoreObserverStub* _obs,
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

                    void (STDCALL CBackupManagerService::*pHandlerFunc)();

                    pHandlerFunc = &CBackupManagerService::HandleRunBackup;
                    mHost->mBackupHandler->PostCppCallbackDelayed(
                        (Handle32)mHost, *(Handle32*)&pHandlerFunc,
                        NULL, 0, 0);
                } else {
                    Slogger::W(TAG, StringBuffer("Backup pass but e=") + mHost->mEnabled +
                        StringBuffer(" p=") + mHost->mProvisioned);
                }
            }
        }
    }

    return NOERROR;
}

CBackupManagerService::RunInitializeReceiver::RunInitializeReceiver(
    /* [in] */ CBackupManagerService* host)
    : mHost(host)
{}

ECode CBackupManagerService::RunInitializeReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    assert(intent != NULL);
    intent->GetAction(&action);

    if (RUN_INITIALIZE_ACTION.Equals(action)) {
        {
            Mutex::Autolock lock(mHost->mQueueLock);
            if (DEBUG) Slogger::V(TAG, "Running a device init");

            // Acquire the wakelock and pass it to the init thread.  it will
            // be released once init concludes.
            //TODO
            //mWakelock.acquire();

            void (STDCALL CBackupManagerService::*pHandlerFunc)();

            pHandlerFunc = &CBackupManagerService::HandleRunInitilize;
            mHost->mBackupHandler->PostCppCallbackDelayed(
                (Handle32)mHost, *(Handle32*)&pHandlerFunc,
                NULL, 0, 0);
        }
    }

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
    {
        Mutex::Autolock lock(mHost->mClearDataLock);
        mHost->mClearingData = FALSE;

        // TODO
        //mClearDataLock.notifyAll();
    }

    return NOERROR;
}

UInt32 CBackupManagerService::ClearDataObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CBackupManagerService::ClearDataObserver::Release()
{
    return ElRefBase::Release();
}

PInterface CBackupManagerService::ClearDataObserver::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_ICapsuleDataObserver) {
        return (PInterface)this;
    }

    return NULL;
}

ECode CBackupManagerService::ClearDataObserver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(ICapsuleDataObserver*)this) {
        *pIID = EIID_ICapsuleDataObserver;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CBackupManagerService::PerformBackupTask::PerformBackupTask(
    /* [in] */ IBackupTransport* transport,
    /* [in] */ List<BackupRequest*>* queue,
    /* [in] */ IFile* journal,
    /* [in] */ CBackupManagerService* host)
    : mTransport(transport)
    , mQueue(queue)
    , mJournal(journal)
    , mHost(host)
{
    String dirName;
    assert(transport != NULL);
    transport->TransportDirName(&dirName);
    CFile::New(mHost->mBaseStateDir, dirName, (IFile**) &mStateDir);
    // } catch (RemoteException e) {
    //     // can't happen; the transport is local
    // }
}

ECode CBackupManagerService::PerformBackupTask::Run()
{
    Int32 status = BackupConstants::TRANSPORT_OK;
    //Int64 startRealtime = SystemClock::GetElapsedRealtime();
    assert(mQueue != NULL);
    if (DEBUG) Slogger::V(TAG, StringBuffer("Beginning backup of ") + mQueue->GetSize() + " targets");

    // Backups run at background priority
    Process::SetThreadPriority(Process::THREAD_PRIORITY_BACKGROUND);

    //EventLog.writeEvent(EventLogTags.BACKUP_START, mTransport.transportDirName());

    // If we haven't stored package manager metadata yet, we must init the transport.
    AutoPtr<IFile> pmState;
    CFile::New(mStateDir, String((const char*)PACKAGE_MANAGER_SENTINEL),
            (IFile**) &pmState);
    assert(pmState != NULL);

    Int64 len = 0;
    pmState->GetLength(&len);
    if (status == BackupConstants::TRANSPORT_OK && len <= 0) {
        Slogger::I(TAG, "Initializing (wiping) backup state and transport storage");
        mHost->ResetBackupState(mStateDir);  // Just to make sure.

        Int32 error = 0;
        status = mTransport->InitializeDevice(&error);
        if (status == BackupConstants::TRANSPORT_OK) {
            //EventLog.writeEvent(EventLogTags.BACKUP_INITIALIZE);
        } else {
            //EventLog.writeEvent(EventLogTags.BACKUP_TRANSPORT_FAILURE, "(initialize)");
            Slogger::E(TAG, "Transport error in initializeDevice()");
        }
    }

    // The package manager doesn't have a proper <application> etc, but since
    // it's running here in the system process we can just set up its agent
    // directly and use a synthetic BackupRequest.  We always run this pass
    // because it's cheap and this way we guarantee that we don't get out of
    // step even if we're selecting among various transports at run time.
    if (status == BackupConstants::TRANSPORT_OK) {
        assert(0);
        //TODO
        // PackageManagerBackupAgent pmAgent = new PackageManagerBackupAgent(
        //         mPackageManager, allAgentPackages());
        // BackupRequest pmRequest = new BackupRequest(new ApplicationInfo(), FALSE);
        // pmRequest.appInfo.packageName = PACKAGE_MANAGER_SENTINEL;
        // status = processOneBackup(pmRequest,
        //         IBackupAgent.Stub.asInterface(pmAgent.onBind()), mTransport);
    }

    if (status == BackupConstants::TRANSPORT_OK) {
        // Now run all the backups in our queue
        status = DoQueuedBackups(mTransport);
    }

    if (status == BackupConstants::TRANSPORT_OK) {
        // Tell the transport to finish everything it has buffered
        mTransport->FinishBackup(&status);
        if (status == BackupConstants::TRANSPORT_OK) {
            //Int32 millis = (Int32) (SystemClock::GetElapsedRealtime() - startRealtime);
            //EventLog.writeEvent(EventLogTags.BACKUP_SUCCESS, mQueue.size(), millis);
        } else {
            //EventLog.writeEvent(EventLogTags.BACKUP_TRANSPORT_FAILURE, "(finish)");
            Slogger::E(TAG, "Transport error in finishBackup()");
        }
    }

    if (status == BackupConstants::TRANSPORT_NOT_INITIALIZED) {
        // The backend reports that our dataset has been wiped.  We need to
        // reset all of our bookkeeping and instead run a new backup pass for
        // everything.
        //EventLog.writeEvent(EventLogTags.BACKUP_RESET, mTransport.transportDirName());
        mHost->ResetBackupState(mStateDir);
    }

    // catch (Exception e) {
    //     Slog.e(TAG, "Error in backup thread", e);
    //     status = BackupConstants.TRANSPORT_ERROR;
    // }
    //finally {
    // If everything actually went through and this is the first time we've
    // done a backup, we can now record what the current backup dataset token
    // is.
    if ((mHost->mCurrentToken == 0) && (status == BackupConstants::TRANSPORT_OK)) {
        //try {
        mTransport->GetCurrentRestoreSet(&(mHost->mCurrentToken));
        //} catch (RemoteException e) { /* cannot happen */ }
        mHost->WriteRestoreTokens();
    }

    // If things went wrong, we need to re-stage the apps we had expected
    // to be backing up in this pass.  This journals the package names in
    // the current active pending-backup file, not in the we are holding
    // here in mJournal.
    if (status != BackupConstants::TRANSPORT_OK) {
        Slogger::W(TAG, "Backup pass unsuccessful, restaging");
        if (mQueue != NULL) {
            List<BackupRequest*>::Iterator req = mQueue->Begin();
            for (; req != mQueue->End(); ++req) {
                assert((*req)->mAppInfo != NULL);

                String name;
                (*req)->mAppInfo->GetCapsuleName(&name);
                mHost->DataChangedImpl(name);
            }
        }

        // We also want to reset the backup schedule based on whatever
        // the transport suggests by way of retry/backoff time.
        //try {
        Int64 t = 0;
        mTransport->RequestBackupTime(&t);
        mHost->StartBackupAlarmsLocked(t);
        //} catch (RemoteException e) { /* cannot happen */ }
    }

    // Either backup was successful, in which case we of course do not need
    // this pass's journal any more; or it failed, in which case we just
    // re-enqueued all of these packages in the current active journal.
    // Either way, we no longer need this pass's journal.
    Boolean df = FALSE;
    if (mJournal != NULL && (mJournal->Delete(&df), !df)) {
        Slogger::E(TAG, "Unable to remove backup journal file " + mJournal);
    }

    // Only once we're entirely finished do we release the wakelock
    if (status == BackupConstants::TRANSPORT_NOT_INITIALIZED) {
        mHost->BackupNow();
    }

    //TODO
    //mWakelock.release();

    return NOERROR;
}

Int32 CBackupManagerService::PerformBackupTask::DoQueuedBackups(
    /* [in] */ IBackupTransport* transport)
{
    assert(mQueue != NULL);
    List<BackupRequest*>::Iterator request = mQueue->Begin();
    for (; request != mQueue->End(); ++ request) {
        //Slogger::D(TAG, StringBuffer("starting agent for backup of ") + request);
        AutoPtr<IBackupAgent> agent;
        Int32 mode = ((*request)->mFullBackup)
                ? ApplicationApartment_BACKUP_MODE_FULL
                : ApplicationApartment_BACKUP_MODE_INCREMENTAL;
        //try {
        //TODO
        //mWakelock.setWorkSource(new WorkSource((*request)->mAppInfo.uid));
        agent = mHost->BindToAgentSynchronous((*request)->mAppInfo, mode);
        if (agent != NULL) {
            Int32 result = ProcessOneBackup((*request), agent, transport);
            if (result != BackupConstants::TRANSPORT_OK) {
                return result;
            }
        }
        // } catch (SecurityException ex) {
        //     // Try for the next one.
        //     Slog.d(TAG, "error in bind/backup", ex);
        // } finally {
        //try {  // unbind even on timeout, just in case
        mHost->mActivityManager->UnbindBackupAgent((*request)->mAppInfo);
        //} catch (RemoteException e) {}
        //}
    }

    //TODO
    //mWakelock.setWorkSource(NULL);

    return BackupConstants::TRANSPORT_OK;
}

Int32 CBackupManagerService::PerformBackupTask::ProcessOneBackup(
    /* [in] */ BackupRequest* request,
    /* [in] */ IBackupAgent* agent,
    /* [in] */ IBackupTransport* transport)
{
    assert(request != NULL && request->mAppInfo != NULL);
    String packageName;

    request->mAppInfo->GetCapsuleName(&packageName);
    if (DEBUG) Slogger::D(TAG, "processOneBackup doBackup() on " + packageName);

    AutoPtr<IFile> savedStateName;
    CFile::New(mStateDir, packageName, (IFile**) &savedStateName);

    AutoPtr<IFile> backupDataName;
    CFile::New(mHost->mDataDir, packageName + ".data", (IFile**) &backupDataName);
    AutoPtr<IFile> newStateName;
    CFile::New(mStateDir, packageName + ".new", (IFile**) &newStateName);

    AutoPtr<IParcelFileDescriptorHelper> helper;
    FAIL_RETURN(CParcelFileDescriptorHelper::AcquireSingleton(
        (IParcelFileDescriptorHelper**)&helper));

    AutoPtr<IParcelFileDescriptor> savedState;
    AutoPtr<IParcelFileDescriptor> backupData;
    AutoPtr<IParcelFileDescriptor> newState;

    AutoPtr<ICapsuleInfo> packInfo;

    srand(time(NULL));
    Int32 token = rand();
    //try {
    // Look up the package info & signatures.  This is first so that if it
    // throws an exception, there's no file setup yet that would need to
    // be unraveled.
    if (packageName.Equals((const char*)PACKAGE_MANAGER_SENTINEL)) {
        // The metadata 'package' is synthetic
        CCapsuleInfo::New((ICapsuleInfo**) &packInfo);
        assert(packInfo != NULL);
        packInfo->SetCapsuleName(packageName);
    } else {
        mHost->mPackageManager->GetCapsuleInfo(packageName, CapsuleManager_GET_SIGNATURES,
            (ICapsuleInfo**) &packInfo);
    }

    // In a full backup, we pass a NULL ParcelFileDescriptor as
    // the saved-state "file"
    if (!request->mFullBackup) {
        helper->Open(savedStateName,
                ParcelFileDescriptor_MODE_READ_ONLY |
                ParcelFileDescriptor_MODE_CREATE,
                (IParcelFileDescriptor**)&savedState);  // Make an empty file if necessary
    }

    helper->Open(backupDataName,
            ParcelFileDescriptor_MODE_READ_WRITE |
            ParcelFileDescriptor_MODE_CREATE |
            ParcelFileDescriptor_MODE_TRUNCATE,
            (IParcelFileDescriptor**) &backupData);

    helper->Open(newStateName,
            ParcelFileDescriptor_MODE_READ_WRITE |
            ParcelFileDescriptor_MODE_CREATE |
            ParcelFileDescriptor_MODE_TRUNCATE,
            (IParcelFileDescriptor**) &newState);

    // Initiate the target's backup pass
    mHost->PrepareOperationTimeout(token, TIMEOUT_BACKUP_INTERVAL);
    agent->DoBackup(savedState, backupData, newState, token, mHost->mBackupManagerBinder);
    Boolean success = mHost->WaitUntilOperationComplete(token);

    if (!success) {
        // timeout -- bail out into the failed-transaction logic
        //throw new RuntimeException("Backup timeout");
        assert(0);
    }

    mHost->LogBackupComplete(packageName);
    if (DEBUG) Slogger::V(TAG, "doBackup() success");
    // } catch (Exception e) {
    //     Slog.e(TAG, "Error backing up " + packageName, e);
    //     EventLog.writeEvent(EventLogTags.BACKUP_AGENT_FAILURE, packageName, e.toString());
    //     backupDataName.delete();
    //     newStateName.delete();
    //     return BackupConstants.TRANSPORT_ERROR;
    // } finally {
    //try {
    if (savedState != NULL) {
        savedState->Close();
    }
    //} catch (IOException e) {}

    //try {
    if (backupData != NULL) {
        backupData->Close();
    }
    //} catch (IOException e) {}

    //try {
    if (newState != NULL) {
        newState->Close();
    }
    //} catch (IOException e) {}

    savedState = backupData = newState = NULL;

    {
        Mutex::Autolock lock(mHost->mCurrentOpLock);
        mHost->mCurrentOperations->Clear();
    }
    //}

    // Now propagate the newly-backed-up data to the transport
    Int32 result = BackupConstants::TRANSPORT_OK;
    //try {
    Int32 size = 0;
    backupDataName->GetLength((Int64*) &size);
    if (size > 0) {
        if (result == BackupConstants::TRANSPORT_OK) {
            helper->Open(backupDataName,
                    ParcelFileDescriptor_MODE_READ_ONLY,
                    (IParcelFileDescriptor**) &backupData);

            transport->PerformBackup(packInfo, backupData, &result);
        }

        // TODO - We call finishBackup() for each application backed up, because
        // we need to know now whether it succeeded or failed.  Instead, we should
        // hold off on finishBackup() until the end, which implies holding off on
        // renaming *all* the output state files (see below) until that happens.

        if (result == BackupConstants::TRANSPORT_OK) {
            transport->FinishBackup(&result);
        }
    } else {
        if (DEBUG) Slogger::I(TAG, "no backup data written; not calling transport");
    }

    // After successful transport, delete the now-stale data
    // and juggle the files so that next time we supply the agent
    // with the new state file it just created.
    if (result == BackupConstants::TRANSPORT_OK) {
        Boolean t = FALSE;
        backupDataName->Delete(&t);
        newStateName->RenameTo(savedStateName, &t);
        //EventLog.writeEvent(EventLogTags.BACKUP_PACKAGE, packageName, size);
    } else {
        //EventLog.writeEvent(EventLogTags.BACKUP_TRANSPORT_FAILURE, packageName);
    }
    // } catch (Exception e) {
    //     Slog.e(TAG, "Transport error backing up " + packageName, e);
    //     EventLog.writeEvent(EventLogTags.BACKUP_TRANSPORT_FAILURE, packageName);
    //     result = BackupConstants.TRANSPORT_ERROR;
    // } finally {
    //try {
    if (backupData != NULL) {
        backupData->Close();
    }
   // } catch (IOException e) {}
    //}

    return result;
}

UInt32 CBackupManagerService::PerformBackupTask::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CBackupManagerService::PerformBackupTask::Release()
{
    return ElRefBase::Release();
}

PInterface CBackupManagerService::PerformBackupTask::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IRunnable) {
        return (PInterface)this;
    }

    return NULL;
}

ECode CBackupManagerService::PerformBackupTask::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IRunnable*)this) {
        *pIID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
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
    /* [in] */ IRestoreObserverStub* observer,
    /* [in] */ Int64 restoreSetToken,
    /* [in] */ ICapsuleInfo* targetPackage,
    /* [in] */ Int32 pmToken,
    /* [in] */ Boolean needFullBackup,
    /* [in] */ CBackupManagerService* host)
    : mTransport(transport)
    , mObserver(observer)
    , mToken(restoreSetToken)
    , mTargetPackage(targetPackage)
    , mPmToken(pmToken)
    , mNeedFullBackup(needFullBackup)
    , mHost(host)
{
    String name;
    transport->TransportDirName(&name);
    //try {
    CFile::New(mHost->mBaseStateDir, name, (IFile**) &mStateDir);
    // } catch (RemoteException e) {
    //     // can't happen; the transport is local
    // }
}

ECode CBackupManagerService::PerformRestoreTask::Run()
{
    assert(0);
    // Int64 startRealtime = SystemClock::GetElapsedRealtime();
    // // if (DEBUG) Slogger::V(TAG, "Beginning restore process mTransport=" + mTransport
    // //         + " mObserver=" + mObserver + " mToken=" + Long.toHexString(mToken)
    // //         + " mTargetPackage=" + mTargetPackage + " mPmToken=" + mPmToken);

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
    // !!! TODO: actually run the restore through mTransport
    assert(app != NULL);
    String packageName;
    app->GetCapsuleName(&packageName);

    if (DEBUG) Slogger::D(TAG, "processOneRestore packageName=" + packageName);

    // !!! TODO: get the dirs from the transport
    AutoPtr<IFile> backupDataName;
    CFile::New(mHost->mDataDir, packageName + ".restore", (IFile**) &backupDataName);

    AutoPtr<IFile> newStateName;
    CFile::New(mStateDir, packageName + ".new", (IFile**) &newStateName);

    AutoPtr<IFile> savedStateName;
    CFile::New(mStateDir, packageName, (IFile**) &savedStateName);

    AutoPtr<IParcelFileDescriptor> backupData;
    AutoPtr<IParcelFileDescriptor> newState;

    AutoPtr<IParcelFileDescriptorHelper> helper;
    CParcelFileDescriptorHelper::AcquireSingleton(
        (IParcelFileDescriptorHelper**)&helper);
    assert(helper != NULL);

    srand(time(NULL));
    Int32 token = rand();
    // try {
    // Run the transport's restore pass
    helper->Open(backupDataName,
                ParcelFileDescriptor_MODE_READ_WRITE |
                ParcelFileDescriptor_MODE_CREATE |
                ParcelFileDescriptor_MODE_TRUNCATE,
                (IParcelFileDescriptor**) &backupData);

    Int32 data = 0, error = 0;
    mTransport->GetRestoreData(backupData, &error);
    if (data != BackupConstants::TRANSPORT_OK) {
        Slogger::E(TAG, "Error getting restore data for " + packageName);
        //EventLog.writeEvent(EventLogTags.RESTORE_TRANSPORT_FAILURE);
        return;
    }

    // Okay, we have the data.  Now have the agent do the restore.
    backupData->Close();
    helper->Open(backupDataName,
                ParcelFileDescriptor_MODE_READ_ONLY,
                (IParcelFileDescriptor**) &backupData);

    helper->Open(newStateName,
                ParcelFileDescriptor_MODE_READ_WRITE |
                ParcelFileDescriptor_MODE_CREATE |
                ParcelFileDescriptor_MODE_TRUNCATE,
                (IParcelFileDescriptor**) &newState);

    // Kick off the restore, checking for hung agents
    mHost->PrepareOperationTimeout(token, TIMEOUT_RESTORE_INTERVAL);
    agent->DoRestore(backupData, appVersionCode, newState, token,
            mHost->mBackupManagerBinder);

    Boolean success = mHost->WaitUntilOperationComplete(token);

    if (!success) {
        assert(0);
        //throw new RuntimeException("restore timeout");
    }

    // if everything went okay, remember the recorded state now
    //
    // !!! TODO: the restored data should be migrated on the server
    // side into the current dataset.  In that case the new state file
    // we just created would reflect the data already extant in the
    // backend, so there'd be nothing more to do.  Until that happens,
    // however, we need to make sure that we record the data to the
    // current backend dataset.  (Yes, this means shipping the data over
    // the wire in both directions.  That's bad, but consistency comes
    // first, then efficiency.)  Once we introduce server-side data
    // migration to the newly-restored device's dataset, we will change
    // the following from a discard of the newly-written state to the
    // "correct" operation of renaming into the canonical state blob.
    Boolean tmpFlag = FALSE;
    newStateName->Delete(&tmpFlag);                      // TODO: remove; see above comment
    //newStateName.renameTo(savedStateName);    // TODO: replace with this

    Int32 size = 0;
    backupDataName->GetLength((Int64*) &size);
    //EventLog.writeEvent(EventLogTags.RESTORE_PACKAGE, packageName, size);
    // } catch (Exception e) {
    //     Slog.e(TAG, "Error restoring data for " + packageName, e);
    //     EventLog.writeEvent(EventLogTags.RESTORE_AGENT_FAILURE, packageName, e.toString());

    //     // If the agent fails restore, it might have put the app's data
    //     // into an incoherent state.  For consistency we wipe its data
    //     // again in this case before propagating the exception
    //     clearApplicationDataSynchronous(packageName);
    // } finally {
    backupDataName->Delete(&tmpFlag);
    //try {
    if (backupData != NULL) {
        backupData->Close();
    }
    //} catch (IOException e) {}

    //try {
    if (newState != NULL) {
        newState->Close();
    }
    //} catch (IOException e) {}

    backupData = newState = NULL;
    mHost->mCurrentOperations->Delete(token);

    // If we know a priori that we'll need to perform a full post-restore backup
    // pass, clear the new state file data.  This means we're discarding work that
    // was just done by the app's agent, but this way the agent doesn't need to
    // take any special action based on global device state.
    if (needFullBackup) {
        newStateName->Delete(&tmpFlag);
    }
    //}
}

UInt32 CBackupManagerService::PerformRestoreTask::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CBackupManagerService::PerformRestoreTask::Release()
{
    return ElRefBase::Release();
}

PInterface CBackupManagerService::PerformRestoreTask::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IRunnable) {
        return (PInterface)this;
    }

    return NULL;
}

ECode CBackupManagerService::PerformRestoreTask::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IRunnable*)this) {
        *pIID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CBackupManagerService::PerformClearTask::PerformClearTask(
    /* [in] */ IBackupTransport* transport,
    /* [in] */ ICapsuleInfo* capInfo,
    /* [in] */ CBackupManagerService* host)
    : mTransport(transport)
    , mCapsule(capInfo)
    , mHost(host)
{
}

ECode CBackupManagerService::PerformClearTask::Run()
{
    // /try {
    // Clear the on-device backup state to ensure a full backup next time
    AutoPtr<IFile> stateDir;
    String name;
    mTransport->TransportDirName(&name);
    CFile::New(mHost->mBaseStateDir, name, (IFile**) &stateDir);

    AutoPtr<IFile> stateFile;
    mCapsule->GetCapsuleName(&name);
    CFile::New(stateDir, name, (IFile**) &stateFile);
    Boolean tmpFlag = FALSE;
    stateFile->Delete(&tmpFlag);

    // Tell the transport to remove all the persistent storage for the app
    // TODO - need to handle failures
    Int32 error = 0;
    mTransport->ClearBackupData(mCapsule, &error);
    // } catch (RemoteException e) {
    //     // can't happen; the transport is local
    // } finally {
        // try {
            // TODO - need to handle failures
    mTransport->FinishBackup(&error);
        // } catch (RemoteException e) {
        //     // can't happen; the transport is local
        // }

    // Last but not least, release the cpu
    //TODO
    //mWakelock.release();
    //}

    return NOERROR;
}

UInt32 CBackupManagerService::PerformClearTask::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CBackupManagerService::PerformClearTask::Release()
{
    return ElRefBase::Release();
}

PInterface CBackupManagerService::PerformClearTask::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IRunnable) {
        return (PInterface)this;
    }

    return NULL;
}

ECode CBackupManagerService::PerformClearTask::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IRunnable*)this) {
        *pIID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CBackupManagerService::PerformInitializeTask::PerformInitializeTask(
    /* [in] */ HashSet<String>* transportNames,
    /* [in] */ CBackupManagerService* host)
    : mQueue(transportNames)
    , mHost(host)
{
}

ECode CBackupManagerService::PerformInitializeTask::Run()
{
    if (mQueue != NULL) {
        HashSet<String>::Iterator transportName = mQueue->Begin();
        //try {
        for (; transportName != mQueue->End(); ++transportName) {
            AutoPtr<IBackupTransport> transport = mHost->GetTransport(*transportName);
            if (transport == NULL) {
                Slogger::E(TAG, StringBuffer("Requested init for ")
                     + *transportName + " but not found");
                continue;
            }

            Slogger::I(TAG, StringBuffer("Initializing (wiping) backup transport storage: ")
                 + *transportName);

            //EventLog.writeEvent(EventLogTags.BACKUP_START, transport.transportDirName());
            //Int64 startRealtime = SystemClock::GetElapsedRealtime();
            Int32 status = 0;
            transport->InitializeDevice(&status);

            if (status == BackupConstants::TRANSPORT_OK) {
                transport->FinishBackup(&status);
            }

            // Okay, the wipe really happened.  Clean up our local bookkeeping.
            if (status == BackupConstants::TRANSPORT_OK) {
                Slogger::I(TAG, "Device init successful");
                //Int32 millis = (Int32) (SystemClock::GetElapsedRealtime() - startRealtime);
                //EventLog.writeEvent(EventLogTags.BACKUP_INITIALIZE);
                AutoPtr<IFile> tmpF;
                String name;
                transport->TransportDirName(&name);
                CFile::New(mHost->mBaseStateDir, name, (IFile**) &tmpF);
                mHost->ResetBackupState(tmpF);
                //EventLog.writeEvent(EventLogTags.BACKUP_SUCCESS, 0, millis);
                {
                    Mutex::Autolock lock(mHost->mQueueLock);
                    mHost->RecordInitPendingLocked(FALSE, *transportName);
                }
            } else {
                // If this didn't work, requeue this one and try again
                // after a suitable interval
                Slogger::E(TAG, "Transport error in initializeDevice()");
                //EventLog.writeEvent(EventLogTags.BACKUP_TRANSPORT_FAILURE, "(initialize)");
                {
                    Mutex::Autolock lock(mHost->mQueueLock);
                    mHost->RecordInitPendingLocked(TRUE, *transportName);
                }
                // do this via another alarm to make sure of the wakelock states
                Int64 delay = 0;
                transport->RequestBackupTime(&delay);
                if (DEBUG) Slogger::W(TAG, StringBuffer("init failed on ")
                        + *transportName + " resched in " + delay);

                //TODO
                // mAlarmManager.set(AlarmManager.RTC_WAKEUP,
                //         System.currentTimeMillis() + delay, mRunInitIntent);
            }
        }
    }
    // } catch (RemoteException e) {
    //     // can't happen; the transports are local
    // } catch (Exception e) {
    //     Slog.e(TAG, "Unexpected error performing init", e);
    // } finally {
    //     // Done; release the wakelock
    //     //TODO
    //     mWakelock.release();
    // }

    return NOERROR;
}

UInt32 CBackupManagerService::PerformInitializeTask::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CBackupManagerService::PerformInitializeTask::Release()
{
    return ElRefBase::Release();
}

PInterface CBackupManagerService::PerformInitializeTask::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IRunnable) {
        return (PInterface)this;
    }

    return NULL;
}

ECode CBackupManagerService::PerformInitializeTask::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IRunnable*)this) {
        *pIID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
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
    {
        Mutex::Autolock lock(mActivieRSLock);
        assert(error != NULL);

        *error = 0;
        mHost->mContext->EnforceCallingOrSelfPermission(
                String("android.Manifest.permission.BACKUP"),
                "getAvailableRestoreSets");
        if (observer == NULL) {
            //throw new IllegalArgumentException("Observer must not be NULL");
            return E_INVALID_ARGUMENT;
        }

        Int64 oldId = Binder::ClearCallingIdentity();
        //try {
        if (mRestoreTransport == NULL) {
            Slogger::W(TAG, "Null transport getting restore sets");
            *error = -1;
            return NOERROR;
        }

        // spin off the transport request to our service thread
        //TODO
        //mWakelock.acquire();


        void (STDCALL CBackupManagerService::*pHandlerFunc)(RestoreGetSetsParams*);
        pHandlerFunc = &CBackupManagerService::HandleGetRestoreSets;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        CBackupManagerService::RestoreGetSetsParams data(mRestoreTransport, this, observer);
        params->WriteInt32((Handle32)(Handle32*) &data);

        mHost->mBackupHandler->PostCppCallbackDelayed(
            (Handle32)mHost, *(Handle32*)&pHandlerFunc,
            params, 0, 0);

        // } catch (Exception e) {
        //     Slog.e(TAG, "Error in getAvailableRestoreSets", e);
        //     return -1;
        // } finally {
        Binder::RestoreCallingIdentity(oldId);
        // }
    }

    return NOERROR;
}

ECode CBackupManagerService::ActiveRestoreSession::RestoreAll(
    /* [in] */ Int64 token,
    /* [in] */ IRestoreObserverStub* observer,
    /* [out] */ Int32* error)
{
    Mutex::Autolock lock(mActivieRSLock);
    assert(error != NULL);
    *error = 0;
    mHost->mContext->EnforceCallingOrSelfPermission(
            String("android.Manifest.permission.BACKUP"),
            "performRestore");

    if (DEBUG) Slogger::D(TAG, StringBuffer("restoreAll token=")
            + token
            + " observer=" + observer);

    if (mRestoreTransport == NULL || mRestoreSets == NULL) {
        Slogger::E(TAG, "Ignoring restoreAll() with no restore set");
        *error = -1;
        return NOERROR;
    }

    if (mPackageName != NULL) {
        Slogger::E(TAG, "Ignoring restoreAll() on single-package session");
        *error = -1;
        return NOERROR;
    }

    {
        Mutex::Autolock lock(mHost->mQueueLock);
        if (mRestoreSets != NULL) {
            for (Int32 i = 0; i < mRestoreSets->GetLength(); i++) {
                AutoPtr<IRestoreSet> set = (*mRestoreSets)[i];
                assert(set != NULL);
                Int64 token = 0;
                set->GetToken(&token);
                if (token == token) {
                    Int64 oldId = Binder::ClearCallingIdentity();

                    //TODO
                    //mWakelock.acquire();
                    void (STDCALL CBackupManagerService::*pHandlerFunc)(RestoreParams*);
                    pHandlerFunc = &CBackupManagerService::HandleRestore;

                    AutoPtr<IParcel> params;
                    CCallbackParcel::New((IParcel**)&params);
                    CBackupManagerService::RestoreParams data(mRestoreTransport, observer
                            , token, TRUE);

                    params->WriteInt32((Handle32)(Handle32*) &data);

                    mHost->mBackupHandler->PostCppCallbackDelayed(
                        (Handle32)mHost, *(Handle32*)&pHandlerFunc,
                        params, 0, 0);

                    Binder::RestoreCallingIdentity(oldId);
                    return NOERROR;
                }
            }
        }
    }

    Slogger::W(TAG, StringBuffer("Restore token ") + token + " not found");
    *error = -1;
    return NOERROR;
}

ECode CBackupManagerService::ActiveRestoreSession::RestorePackage(
    /* [in] */ const String& packageName,
    /* [in] */ IRestoreObserverStub* observer,
    /* [out] */ Int32* error)
{
    {
        Mutex::Autolock lock(mActivieRSLock);
        assert(error != NULL);
        *error = 0;

        if (DEBUG) Slogger::V(TAG, StringBuffer("restorePackage pkg=")
             + packageName + " obs=" + observer);

        if (!mPackageName.IsNull()) {
            if (! mPackageName.Equals(packageName)) {
                Slogger::E(TAG, StringBuffer("Ignoring attempt to restore pkg=") + packageName
                        + " on session for package " + mPackageName);
                *error = -1;
                return NOERROR;
            }
        }

        AutoPtr<ICapsuleInfo> app;
        //try {
        mHost->mPackageManager->GetCapsuleInfo(packageName, 0, (ICapsuleInfo**) &app);
        assert(app != NULL);
        // } catch (NameNotFoundException nnf) {
        //     Slog.w(TAG, "Asked to restore nonexistent pkg " + packageName);
        //     *error = -1;
        //     return NOERROR;
        // }

        // If the caller is not privileged and is not coming from the target
        // app's uid, throw a permission exception back to the caller.
        Int32 perm = 0;
        mHost->mContext->CheckPermission(String("android.Manifest.permission.BACKUP"),
                Binder::GetCallingPid(), Binder::GetCallingUid(), &perm);

        AutoPtr<IApplicationInfo> info;
        app->GetApplicationInfo((IApplicationInfo**) &info);
        assert(info != NULL);
        Int32 uid = 0;
        info->GetUid(&uid);

        if ((perm == CapsuleManager_PERMISSION_DENIED) &&
                (uid != Binder::GetCallingUid())) {
            Slogger::W(TAG, StringBuffer("restorePackage: bad packageName=") + packageName
                    + " or calling uid=" + Binder::GetCallingUid());
            //throw new SecurityException("No permission to restore other packages");
            assert(0);
        }

        String backupAgentName;
        info->GetBackupAgentName(&backupAgentName);
        // If the package has no backup agent, we obviously cannot proceed
        if (backupAgentName.IsNull()) {
            Slogger::W(TAG, StringBuffer("Asked to restore package ")
                    + packageName + " with no agent");

            *error = -1;
            return NOERROR;
        }

        // So far so good; we're allowed to try to restore this package.  Now
        // check whether there is data for it in the current dataset, falling back
        // to the ancestral dataset if not.
        Int64 token = mHost->GetAvailableRestoreToken(packageName);

        // If we didn't come up with a place to look -- no ancestral dataset and
        // the app has never been backed up from this device -- there's nothing
        // to do but return failure.
        if (token == 0) {
            if (DEBUG) Slogger::W(TAG, "No data available for this package; not restoring");
            *error = -1;
            return NOERROR;
        }

        // Ready to go:  enqueue the restore request and claim success
        Int64 oldId = Binder::ClearCallingIdentity();

        //TODO
        //mWakelock.acquire();
        void (STDCALL CBackupManagerService::*pHandlerFunc)(RestoreParams*);
        pHandlerFunc = &CBackupManagerService::HandleRestore;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        CBackupManagerService::RestoreParams data(mRestoreTransport, observer
                , token, app, 0, FALSE);

        params->WriteInt32((Handle32)(Handle32*) &data);

        mHost->mBackupHandler->PostCppCallbackDelayed(
            (Handle32)mHost, *(Handle32*)&pHandlerFunc,
            params, 0, 0);

        Binder::RestoreCallingIdentity(oldId);
    }

    return NOERROR;
}

ECode CBackupManagerService::ActiveRestoreSession::EndRestoreSession()
{
    {
        Mutex::Autolock lock(mActivieRSLock);
        if (DEBUG) Slogger::D(TAG, "endRestoreSession");

        //synchronized (this) {
        Int64 oldId = Binder::ClearCallingIdentity();
        // /try {
        if (mRestoreTransport != NULL) {
            mRestoreTransport->FinishRestore();
        }
        // } catch (Exception e) {
        //     Slog.e(TAG, "Error in finishRestore", e);
        // } finally {
        mRestoreTransport = NULL;
        Binder::RestoreCallingIdentity(oldId);
        //}
        //}

        //synchronized (BackupManagerService.this)
        {
            Mutex::Autolock lock(mHost->mBackupServiceLock);
            if (mHost->mActiveRestoreSession.Get() == this) {
                mHost->mActiveRestoreSession = NULL;
            } else {
                Slogger::E(TAG, "ending non-current restore session");
            }
        }
    }

    return NOERROR;
}

ECode CBackupManagerService::ActiveRestoreSession::RestoreCapsule(
    /* [in] */ const String& capsuleName,
    /* [in] */ IRestoreObserverStub* observer,
    /* [out] */ Int32* error)
{
    assert(0);
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

PInterface CBackupManagerService::ActiveRestoreSession::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IRestoreSessionStub) {
        return (PInterface)this;
    }

    return NULL;
}

ECode CBackupManagerService::ActiveRestoreSession::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IRestoreSessionStub*)this) {
        *pIID = EIID_IRestoreSessionStub;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}


CBackupManagerService::GroupServiceConnection::GroupServiceConnection(
    /* [in] */ CBackupManagerService* host)
    : mHost(host)
{
}

ECode CBackupManagerService::GroupServiceConnection::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    if (DEBUG) Slogger::V(TAG, "Connected to Google transport");
    mHost->mGoogleTransport = IBackupTransport::Probe(service);

    assert(name != NULL);
    String str;
    name->FlattenToShortString(&str);
    mHost->RegisterTransport(str, mHost->mGoogleTransport);

    return NOERROR;
}

ECode CBackupManagerService::GroupServiceConnection::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    if (DEBUG) Slogger::V(TAG, "Disconnected from Google transport");
    mHost->mGoogleTransport = NULL;

    String str;
    name->FlattenToShortString(&str);
    mHost->RegisterTransport(str, NULL);

    return NOERROR;
}

UInt32 CBackupManagerService::GroupServiceConnection::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CBackupManagerService::GroupServiceConnection::Release()
{
    return ElRefBase::Release();
}

PInterface CBackupManagerService::GroupServiceConnection::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IServiceConnection) {
        return (PInterface)this;
    }

    return NULL;
}

ECode CBackupManagerService::GroupServiceConnection::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IServiceConnection*)this) {
        *pIID = EIID_IServiceConnection;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CBackupManagerService::DataChangedRunable::DataChangedRunable(
    /* [in] */ const String& name,
    /* [in] */ HashSet<AutoPtr<IApplicationInfo> >* targets,
    /* [in] */ CBackupManagerService* host)
    : mCapsuleName(name)
    , mTargets(targets)
    , mHost(host)
{
}

ECode CBackupManagerService::DataChangedRunable::Run()
{
    assert(mTargets != NULL);
    mHost->DataChangedImpl(mCapsuleName, mTargets);
    delete mTargets;
    mTargets = NULL;

    return NOERROR;
}

UInt32 CBackupManagerService::DataChangedRunable::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CBackupManagerService::DataChangedRunable::Release()
{
    return ElRefBase::Release();
}

PInterface CBackupManagerService::DataChangedRunable::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IServiceConnection) {
        return (PInterface)this;
    }

    return NULL;
}

ECode CBackupManagerService::DataChangedRunable::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IServiceConnection*)this) {
        *pIID = EIID_IServiceConnection;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CBackupManagerService::TrackCapsuleInstallAndRemoval::TrackCapsuleInstallAndRemoval(
    /* [in] */ CBackupManagerService* host)
    : mHost(host)
{
}

ECode CBackupManagerService::TrackCapsuleInstallAndRemoval::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    if (DEBUG) Slogger::D(TAG, StringBuffer("Received broadcast ") + intent);

    assert(intent != NULL);
    String action;
    intent->GetAction(&action);

    Boolean replacing = FALSE;
    Boolean added = FALSE;
    AutoPtr<IBundle> extras;
    intent->GetExtras((IBundle**) &extras);

    ArrayOf<String>* pkgList = NULL;
    if (String(Intent_ACTION_CAPSULE_ADDED).Equals(action) ||
            String(Intent_ACTION_CAPSULE_REMOVED).Equals(action)) {
        AutoPtr<IUri> uri;
        intent->GetData((IUri**)&uri);
        if (uri == NULL) {
            return NOERROR;
        }

        assert(uri != NULL);
        String pkgName;
        uri->GetSchemeSpecificPart(&pkgName);
        if (pkgName != NULL) {
            pkgList = ArrayOf<String>::Alloc(1);
            (*pkgList)[0] = pkgName;
        }

        added = String(Intent_ACTION_CAPSULE_ADDED).Equals(action);
        extras->GetBooleanEx(String(Intent_EXTRA_REPLACING), FALSE, &replacing);
    } else if (String(Intent_ACTION_EXTERNAL_APPLICATIONS_AVAILABLE).Equals(action)) {
        added = TRUE;
        intent->GetStringArrayExtra(String(Intent_EXTRA_CHANGED_CAPSULE_LIST), &pkgList);
    } else if (String(Intent_ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE).Equals(action)) {
        added = FALSE;
        intent->GetStringArrayExtra(String(Intent_EXTRA_CHANGED_CAPSULE_LIST), &pkgList);
    }

    if (pkgList == NULL || pkgList->GetLength() == 0) {
        return NOERROR;
    }

    if (added) {
        {
            Mutex::Autolock lock(mHost->mBackupParticipantsLock);
            for (Int32 i = 0; i < pkgList->GetLength(); i++) {
                if (replacing) {
                    // The package was just upgraded
                    mHost->UpdatePackageParticipantsLocked((*pkgList)[i]);
                } else {
                    // The package was just added
                    mHost->AddPackageParticipantsLocked((*pkgList)[i]);
                }
            }
        }
    } else {
        if (replacing) {
            // The package is being updated.  We'll receive a PACKAGE_ADDED shortly.
        } else {
            {
                Mutex::Autolock lock(mHost->mBackupParticipantsLock);
                for (Int32 i = 0; i < pkgList->GetLength(); i++) {
                    mHost->RemovePackageParticipantsLocked((*pkgList)[i]);
                }
            }
        }
    }

    if (pkgList != NULL) {
        ArrayOf<String>::Free(pkgList);
        pkgList = NULL;
    }

    return NOERROR;
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

    Map<Int32, HashSet<AutoPtr<IApplicationInfo> >*>::Iterator it;
    for (it = mBackupParticipants.Begin(); it != mBackupParticipants.End(); ++ it) {
        HashSet<AutoPtr<IApplicationInfo> >* set = it->mSecond;
        if (set != NULL) {
            delete set;
            set = NULL;
        }
    }

    HashMap<AutoPtr<IApplicationInfo>, BackupRequest* >::Iterator it2 = mPendingBackups.Begin();
    for (; it2 != mPendingBackups.End(); ++it2) {
        BackupRequest* d = it2->mSecond;
        if (d != NULL) {
            delete d;
            d = NULL;
        }
    }

    mBackupParticipants.Clear();
}

ECode CBackupManagerService::constructor(
    /* [in] */ IContext* ctx)
{
    assert(ctx != NULL);
    mContext = ctx;
    ctx->GetCapsuleManager((ILocalCapsuleManager**) &mPackageManager);

    AutoPtr<IServiceManager> serviceManager;
    Elastos::GetServiceManager((IServiceManager**)&serviceManager);
    assert(serviceManager != NULL);
    serviceManager->GetService(String("capsule"),
            (IInterface**)(ICapsuleManager**)&mPackageManagerBinder);
    assert(mPackageManagerBinder != NULL);

    ActivityManagerNative::GetDefault((IActivityManager**)&mActivityManager);

    //TODO
    // mAlarmManager = (AlarmManager) ctx.getSystemService(Context.ALARM_SERVICE);
    // mPowerManager = (PowerManager) ctx.getSystemService(Context.POWER_SERVICE);

    mBackupManagerBinder = this;

    // spin up the backup/restore handler thread
    //TODO
    // mHandlerThread = new HandlerThread("backup", Process.THREAD_PRIORITY_BACKGROUND);
    // mHandlerThread.start();
    //mBackupHandler = new BackupHandler(mHandlerThread.getLooper());
    CApartment::New(FALSE, (IApartment**) &mBackupHandler);
    assert(mBackupHandler != NULL);
    mBackupHandler->Start(ApartmentAttr_New);


    mBroadcastReceiver = new TrackCapsuleInstallAndRemoval(this);

    // Set up our bookkeeping
    AutoPtr<IContentResolver> res;
    ctx->GetContentResolver((IContentResolver**) &res);

    Int32 tmpValue = 0;
    Settings::Secure::GetInt32(res, String(SettingsSecure_BACKUP_ENABLED), 0, &tmpValue);
    Boolean areEnabled = tmpValue != 0;

    Settings::Secure::GetInt32(res, String(SettingsSecure_BACKUP_PROVISIONED), 0, &tmpValue);
    mProvisioned = tmpValue != 0;

    Settings::Secure::GetInt32(res, String(SettingsSecure_BACKUP_AUTO_RESTORE), 1, &tmpValue);
    mAutoRestore = tmpValue != 0;
    // If Encrypted file systems is enabled or disabled, this call will return the
    // correct directory.
    CFile::New(Environment::GetSecureDataDirectory(), String("backup"), (IFile**) &mBaseStateDir);
    assert(mBaseStateDir != NULL);

    Boolean succeeded = FALSE;
    mBaseStateDir->Mkdirs(&succeeded);
    mDataDir = Environment::GetDownloadCacheDirectory();

    // Alarm receivers for scheduled backups & initialization operations
    mRunBackupReceiver = new RunBackupReceiver(this);

    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New((IIntentFilter**) &filter);
    assert(filter != NULL);

    filter->AddAction(String(RUN_BACKUP_ACTION));

    //TODO
    // ctx->RegisterReceiver(mRunBackupReceiver, filter,
    //         android.Manifest.permission.BACKUP, NULL);

    mRunInitReceiver = new RunInitializeReceiver(this);
    CIntentFilter::New((IIntentFilter**) &filter);
    assert(filter != NULL);

    filter->AddAction(String(RUN_INITIALIZE_ACTION));

    //TODO
    // ctx->RegisterReceiver(mRunInitReceiver, filter,
    //         android.Manifest.permission.BACKUP, NULL);

    AutoPtr<IIntent> backupIntent;
    CIntent::New(String(RUN_BACKUP_ACTION), (IIntent**) &backupIntent);
    assert(backupIntent != NULL);

    backupIntent->AddFlags(Intent_FLAG_RECEIVER_REGISTERED_ONLY);

    AutoPtr<IPendingIntentHelper> helper;
    CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&helper);
    assert(helper != NULL);

    helper->GetBroadcast(ctx, MSG_RUN_BACKUP, backupIntent, 0, (IPendingIntent**) &mRunBackupIntent);

    AutoPtr<IIntent> initIntent;
    CIntent::New(String(RUN_INITIALIZE_ACTION), (IIntent**) &initIntent);
    backupIntent->AddFlags(Intent_FLAG_RECEIVER_REGISTERED_ONLY);
    helper->GetBroadcast(ctx, MSG_RUN_INITIALIZE, initIntent, 0, (IPendingIntent**) &mRunInitIntent);

    // Set up the backup-request journaling
    CFile::New(mBaseStateDir, String("pending"), (IFile**) &mJournalDir);
    assert(mJournalDir != NULL);
    mJournalDir->Mkdirs(&succeeded);   // creates mBaseStateDir along the way
    mJournal = NULL;        // will be created on first use

    // Set up the various sorts of package tracking we do
    InitPackageTracking();

    // Build our mapping of uid to backup client services.  This implicitly
    // schedules a backup pass on the Package Manager metadata the first
    // time anything needs to be backed up.
    {
        Mutex::Autolock lock(mBackupParticipantsLock);
        AddPackageParticipantsLocked(String(""));
    }

    // Set up our transport options and initialize the default transport
    // TODO: Have transports register themselves somehow?
    // TODO: Don't create transports that we don't need to?
    CLocalTransport::New(ctx, (IBackupTransport**) &mLocalTransport);  // This is actually pretty cheap

    //TODO
    //ComponentName localName = new ComponentName(ctx, LocalTransport.class);
    AutoPtr<IComponentName> localName;
    CComponentName::New(ctx, String("app.backup.LocalTransport"), (IComponentName**) &localName);
    assert(localName != NULL);

    String shortName;
    localName->FlattenToShortString(&shortName);
    RegisterTransport(shortName, mLocalTransport);

    mGoogleTransport = NULL;
    Settings::Secure::GetString(res,
            String(SettingsSecure_BACKUP_TRANSPORT), &mCurrentTransport);

    if (String("").Equals(mCurrentTransport)) {
        mCurrentTransport = NULL;
    }

    if (DEBUG) Slogger::V(TAG, StringBuffer("Starting with transport ") + mCurrentTransport);

    // Attach to the Google backup transport.  When this comes up, it will set
    // itself as the current transport because we explicitly reset mCurrentTransport
    // to NULL.

    //TODO: using google's backup????????????
    // ComponentName transportComponent = new ComponentName("com.google.android.backup",
    //         "com.google.android.backup.BackupTransportService");
    AutoPtr<IComponentName> transportComponent;
    CComponentName::New(String("com.google.android.backup"),
            String("com.google.android.backup.BackupTransportService"), (IComponentName**) &transportComponent);
    assert(transportComponent != NULL);

    //try {
    // If there's something out there that is supposed to be the Google
    // backup transport, make sure it's legitimately part of the OS build
    // and not an app lying about its package name.
    AutoPtr<IApplicationInfo> info;
    String capName;
    transportComponent->GetCapsuleName(&capName);
    mPackageManager->GetApplicationInfo(capName, 0,
        (IApplicationInfo**) &info);

    assert(info != NULL);

    Int32 flags = 0;
    info->GetFlags(&flags);
    if ((flags & ApplicationInfo_FLAG_SYSTEM) != 0) {
        if (DEBUG) Slogger::V(TAG, StringBuffer("Binding to Google transport"));
        AutoPtr<IIntent> intent;
        CIntent::New((IIntent**) &intent);
        assert(intent != NULL);

        intent->SetComponent(transportComponent);
        mGoogleConnection = new GroupServiceConnection(this);
        ctx->BindService(intent, mGoogleConnection, Context_BIND_AUTO_CREATE, &succeeded);
    } else {
        Slogger::W(TAG, StringBuffer("Possible Google transport spoof: ignoring ") + info);
    }
    // } catch (PackageManager.NameNotFoundException nnf) {
    //     // No such package?  No binding.
    //     if (DEBUG) Slogger::V(TAG, StringBuffer("Google transport not present"));
    // }

    // Now that we know about valid backup participants, parse any
    // leftover journal files into the pending backup set
    ParseLeftoverJournals();

    // Power management
    //TODO
    //mWakelock = mPowerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "*backup*");

    // Start the backup passes going
    SetBackupEnabled(areEnabled);

    return NOERROR;
}

ECode CBackupManagerService::DataChanged(
    /* [in] */ const String& packageName)
{
    HashSet<AutoPtr<IApplicationInfo> >* targets = DataChangedTargets(packageName);
    if (targets == NULL) {
        Slogger::W(TAG, StringBuffer("dataChanged but no participant pkg='") + packageName + "'"
               + " uid=" + Binder::GetCallingUid());
        return NOERROR;
    }

    // mBackupHandler.post(new Runnable() {
    //         public void run() {
    //             dataChangedImpl(packageName, targets);
    //         }
    //     });

    ECode (STDCALL CBackupManagerService::DataChangedRunable::*pHandlerFunc)();
    pHandlerFunc = &CBackupManagerService::DataChangedRunable::Run;

    mBackupHandler->PostCppCallbackDelayed(
        (Handle32)(Handle32*)(new DataChangedRunable(packageName, targets, this)),
        *(Handle32*)&pHandlerFunc,
        NULL, 0, 0);

    return NOERROR;
}

// Clear the given package's backup data from the current transport
ECode CBackupManagerService::ClearBackupData(
    /* [in] */ const String& packageName)
{
    if (DEBUG) Slogger::V(TAG, StringBuffer("clearBackupData() of ") + packageName);
    AutoPtr<ICapsuleInfo> info;
    //try {
    mPackageManager->GetCapsuleInfo(packageName, CapsuleManager_GET_SIGNATURES,
            (ICapsuleInfo**) &info);

    // } catch (NameNotFoundException e) {
    //     Slog.d(TAG, "No such package '" + packageName + "' - not clearing backup data");
    //     return;
    // }

    // If the caller does not hold the BACKUP permission, it can only request a
    // wipe of its own backed-up data.
    HashSet<AutoPtr<IApplicationInfo> >* apps = NULL;

    Int32 perm = 0;
    mContext->CheckPermission(String("android.Manifest.permission.BACKUP"),
                Binder::GetCallingPid(),
                Binder::GetCallingUid(), &perm);

    if (perm == CapsuleManager_PERMISSION_DENIED) {
        HashSet<AutoPtr<IApplicationInfo> >* s = mBackupParticipants[Binder::GetCallingUid()];
        if (s != NULL) {
            HashSet<AutoPtr<IApplicationInfo> >::Iterator it = s->Begin();
            for (; it != s->End(); ++it) {
                apps->Insert(*it);
            }
        }
    } else {
        // a caller with full permission can ask to back up any participating app
        // !!! TODO: allow data-clear of ANY app?
        if (DEBUG) Slogger::V(TAG, "Privileged caller, allowing clear of other apps");
        apps = new HashSet<AutoPtr<IApplicationInfo> >();
        Int32 N = mBackupParticipants.GetSize();
        for (Int32 i = 0; i < N; i++) {
            HashSet<AutoPtr<IApplicationInfo> >* s = mBackupParticipants[i];
            if (s != NULL) {
                HashSet<AutoPtr<IApplicationInfo> >::Iterator it = s->Begin();
                for (; it != s->End(); ++it) {
                    apps->Insert(*it);
                }
            }
        }
    }

    // now find the given package in the set of candidate apps
    if (apps != NULL) {
        HashSet<AutoPtr<IApplicationInfo> >::Iterator app = apps->Begin();
        for (; app != apps->End(); ++ app) {
            String name;
            (*app)->GetCapsuleName(&name);
            if (name.Equals(packageName)) {
                if (DEBUG) Slogger::V(TAG, "Found the app - running clear process");
                // found it; fire off the clear request
                {
                    Mutex::Autolock lock(mQueueLock);
                    Int64 oldId = Binder::ClearCallingIdentity();

                    //TODO
                    //mWakelock.acquire();
                    // Message msg = mBackupHandler.obtainMessage(MSG_RUN_CLEAR,
                    //         new ClearParams(GetTransport(mCurrentTransport), info));
                    // mBackupHandler.sendMessage(msg);

                    void (STDCALL CBackupManagerService::*pHandlerFunc)(ClearParams*);
                    pHandlerFunc = &CBackupManagerService::HandleClear;

                    AutoPtr<IParcel> params;
                    CCallbackParcel::New((IParcel**)&params);
                    params->WriteInt32((Handle32)(Handle32*)(new ClearParams(GetTransport(mCurrentTransport), info)));

                    mBackupHandler->PostCppCallbackDelayed(
                        (Handle32)this,
                        *(Handle32*)&pHandlerFunc,
                        params, 0, 0);

                    Binder::RestoreCallingIdentity(oldId);
                }
                break;
            }
        }

        delete apps;
        apps = NULL;
    }

    return NOERROR;
}

// Run a backup pass immediately for any applications that have declared
// that they have pending updates.
ECode CBackupManagerService::BackupNow()
{
    mContext->EnforceCallingOrSelfPermission(String("android.Manifest.permission.BACKUP"),
        String("backupNow"));

    if (DEBUG) Slogger::V(TAG, "Scheduling immediate backup pass");
    {
        Mutex::Autolock lock(mQueueLock);
        // Because the alarms we are using can jitter, and we want an *immediate*
        // backup pass to happen, we restart the timer beginning with "next time,"
        // then manually fire the backup trigger intent ourselves.
        StartBackupAlarmsLocked(BACKUP_INTERVAL);
        //try {
        assert(mRunBackupIntent != NULL);
        mRunBackupIntent->Send();
        // } catch (PendingIntent.CanceledException e) {
        //     // should never happen
        //     Slog.e(TAG, "run-backup intent cancelled!");
        // }
    }

    return NOERROR;
}

// Enable/disable the backup service
ECode CBackupManagerService::SetBackupEnabled(
    /* [in] */ Boolean enable)
{
    mContext->EnforceCallingOrSelfPermission(String("android.Manifest.permission.BACKUP"),
            String("setBackupEnabled"));

    Slogger::I(TAG, StringBuffer("Backup enabled => ") + enable);

    Boolean wasEnabled = mEnabled;
    {
        Mutex::Autolock lock(mBackupServiceLock);

        AutoPtr<IContentResolver> res;
        mContext->GetContentResolver((IContentResolver**) &res);
        Boolean result = FALSE;
        Settings::Secure::PutInt32(res, String(SettingsSecure_BACKUP_ENABLED),
                enable ? 1 : 0, &result);
        mEnabled = enable;
    }

    {
        Mutex::Autolock lock(mQueueLock);
        if (enable && !wasEnabled && mProvisioned) {
            // if we've just been enabled, start scheduling backup passes
            StartBackupAlarmsLocked(BACKUP_INTERVAL);
        } else if (!enable) {
            // No longer enabled, so stop running backups
            if (DEBUG) Slogger::I(TAG, "Opting out of backup");

            //TODO
            //mAlarmManager.cancel(mRunBackupIntent);

            // This also constitutes an opt-out, so we wipe any data for
            // this device from the backend.  We start that process with
            // an alarm in order to guarantee wakelock states.
            if (wasEnabled && mProvisioned) {
                // NOTE: we currently flush every registered transport, not just
                // the currently-active one.
                // HashSet<String> allTransports;
                // {
                //     Mutex::Autolock lock(mTransportsLock);
                //     allTransports = new HashSet<String>(mTransports.keySet());
                // }

                // build the set of transports for which we are posting an init
                HashMap<String, AutoPtr<IBackupTransport> >::Iterator transport;

                for (transport = mTransports.Begin(); transport != mTransports.End(); ++transport) {
                    RecordInitPendingLocked(TRUE, transport->mFirst);
                }

                //TODO
                // mAlarmManager.set(AlarmManager.RTC_WAKEUP, System.currentTimeMillis(),
                //         mRunInitIntent);
            }
        }
    }

    return NOERROR;
}

// Enable/disable automatic restore of app data at install time
ECode CBackupManagerService::SetAutoRestore(
    /* [in] */ Boolean doAutoRestore)
{
    mContext->EnforceCallingOrSelfPermission(String("android.Manifest.permission.BACKUP"),
        String("setBackupEnabled"));

    Slogger::I(TAG, StringBuffer("Auto restore => ") + doAutoRestore);

    {
        Mutex::Autolock lock(mBackupServiceLock);

        AutoPtr<IContentResolver> res;
        mContext->GetContentResolver((IContentResolver**) &res);
        Boolean result = FALSE;
        Settings::Secure::PutInt32(res, String(SettingsSecure_BACKUP_AUTO_RESTORE),
                doAutoRestore ? 1 : 0, &result);
        mAutoRestore = doAutoRestore;
    }

    return NOERROR;
}

// Mark the backup service as having been provisioned
ECode CBackupManagerService::SetBackupProvisioned(
    /* [in] */ Boolean available)
{
    mContext->EnforceCallingOrSelfPermission(String("android.Manifest.permission.BACKUP"),
            String("setBackupProvisioned"));

    Boolean wasProvisioned = mProvisioned;
    {
        Mutex::Autolock lock(mBackupServiceLock);

        AutoPtr<IContentResolver> res;
        mContext->GetContentResolver((IContentResolver**) &res);
        Boolean result = FALSE;
        Settings::Secure::PutInt32(res, String(SettingsSecure_BACKUP_PROVISIONED),
                available ? 1 : 0, &result);
        mProvisioned = available;
    }

    {
        Mutex::Autolock lock(mQueueLock);
        if (available && !wasProvisioned && mEnabled) {
            // we're now good to go, so start the backup alarms
            StartBackupAlarmsLocked(FIRST_BACKUP_INTERVAL);
        } else if (!available) {
            // No longer enabled, so stop running backups
            Slogger::W(TAG, "Backup service no longer provisioned");

            //TODO
            //mAlarmManager.cancel(mRunBackupIntent);
        }
    }

    return NOERROR;
}

// Report whether the backup mechanism is currently enabled
ECode CBackupManagerService::IsBackupEnabled(
    /* [out] */ Boolean* enabled)
{
    mContext->EnforceCallingOrSelfPermission(
        String("android.Manifest.permission.BACKUP"), String("isBackupEnabled"));
    return mEnabled;    // no need to synchronize just to read it
}

// Report the name of the currently active transport
ECode CBackupManagerService::GetCurrentTransport(
    /* [out] */ String* transport)
{
    assert(transport != NULL);

    mContext->EnforceCallingOrSelfPermission(String("android.Manifest.permission.BACKUP"),
            String("getCurrentTransport"));
    if (DEBUG) Slogger::V(TAG, StringBuffer("... getCurrentTransport() returning ") + mCurrentTransport);

    *transport = mCurrentTransport;
    return NOERROR;
}

// Report all known, available backup transports
ECode CBackupManagerService::ListAllTransports(
    /*[out, callee]*/ ArrayOf<String>** transports)
{
    assert(transports != NULL);
    *transports = NULL;

    mContext->EnforceCallingOrSelfPermission(
        String("android.Manifest.permission.BACKUP"),
        String("listAllTransports"));

    if (mTransports.GetSize() <= 0) {
        return NOERROR;
    }

    HashMap<String, AutoPtr<IBackupTransport> >::Iterator entry;
    Int32 index = 0, count = 0;
    for (entry = mTransports.Begin(); entry != mTransports.End(); ++entry) {
        if (entry->mSecond != NULL) {
            ++ count;
        }
    }

    if (count <= 0) {
        return NOERROR;
    }

    *transports = ArrayOf<String>::Alloc(count);
    for (entry = mTransports.Begin(); entry != mTransports.End(); ++entry) {
        if (entry->mSecond != NULL) {
            (**transports)[index] = entry->mFirst;

            index ++;
        }
    }

    return NOERROR;
}

// Select which transport to use for the next backup operation.  If the given
// name is not one of the available transports, no action is taken and the method
// returns NULL.
ECode CBackupManagerService::SelectBackupTransport(
    /* [in] */ const String& transport,
    /* [out] */ String* port)
{
    assert(port != NULL);
    mContext->EnforceCallingOrSelfPermission(
        String("android.Manifest.permission.BACKUP"),
        String("selectBackupTransport"));

    {
        Mutex::Autolock lock(mTransportsLock);
        String prevTransport;
        if (mTransports[transport] != NULL) {
            prevTransport = mCurrentTransport;
            mCurrentTransport = transport;

            AutoPtr<IContentResolver> res;
            mContext->GetContentResolver((IContentResolver**) &res);
            Boolean result = FALSE;
            Settings::Secure::PutString(res,
                    String(SettingsSecure_BACKUP_TRANSPORT), transport, &result);
            Slogger::V(TAG, StringBuffer("selectBackupTransport() set ") + mCurrentTransport
                    + " returning " + prevTransport);
        } else {
            Slogger::W(TAG, StringBuffer("Attempt to select unavailable transport ") + transport);
        }

        *port = prevTransport;
        return NOERROR;
    }

    return NOERROR;
}

// Callback: a requested backup agent has been instantiated.  This should only
// be called from the Activity Manager.
ECode CBackupManagerService::AgentConnected(
    /* [in] */ const String& packageName,
    /* [in] */ IBinder* agentBinder)
{
    {
        Mutex::Autolock lock(mAgentConnectLock);
        if (Binder::GetCallingUid() == Process::SYSTEM_UID) {
            Slogger::D(TAG, StringBuffer("agentConnected pkg=")
                    + packageName + " agent=" + agentBinder);

            AutoPtr<IBackupAgent> agent = IBackupAgent::Probe(agentBinder);
            mConnectedAgent = agent;
            mConnecting = FALSE;
        } else {
            Slogger::W(TAG, StringBuffer("Non-system process uid=")+ Binder::GetCallingUid()
                    + " claiming agent connected");
        }

        //TODO
        //mAgentConnectLock.notifyAll();
    }

    return NOERROR;
}

// Callback: a backup agent has failed to come up, or has unexpectedly quit.
// If the agent failed to come up in the first place, the agentBinder argument
// will be NULL.  This should only be called from the Activity Manager.
ECode CBackupManagerService::AgentDisconnected(
    /* [in] */ const String& packageName)
{
    // TODO: handle backup being interrupted
    {
        Mutex::Autolock lock(mAgentConnectLock);
        if (Binder::GetCallingUid() == Process::SYSTEM_UID) {
            mConnectedAgent = NULL;
            mConnecting = FALSE;
        } else {
            Slogger::W(TAG, StringBuffer("Non-system process uid=")
                    + Binder::GetCallingUid()
                    + " claiming agent disconnected");
        }

        //TODO
        //mAgentConnectLock.notifyAll();
    }

    return NOERROR;
}

// An application being installed will need a restore pass, then the Package Manager
// will need to be told when the restore is finished.
ECode CBackupManagerService::RestoreAtInstall(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 token)
{
    if (Binder::GetCallingUid() != Process::SYSTEM_UID) {
        Slogger::W(TAG, StringBuffer("Non-system process uid=") + Binder::GetCallingUid()
                + " attemping install-time restore");
        return NOERROR;
    }

    Int64 restoreSet = GetAvailableRestoreToken(packageName);
    if (DEBUG) Slogger::V(TAG, StringBuffer("restoreAtInstall pkg=") + packageName
            + " token=" + token);

    if (mAutoRestore && mProvisioned && restoreSet != 0) {
        // okay, we're going to attempt a restore of this package from this restore set.
        // The eventual message back into the Package Manager to run the post-install
        // steps for 'token' will be issued from the restore handling code.

        // We can use a synthetic PackageInfo here because:
        //   1. We know it's valid, since the Package Manager supplied the name
        //   2. Only the packageName field will be used by the restore code
        AutoPtr<ICapsuleInfo> pkg;
        CCapsuleInfo::New((ICapsuleInfo**) &pkg);
        pkg->SetCapsuleName(packageName);

        //TODO
        //mWakelock.acquire();
        // Message msg = mBackupHandler.obtainMessage(MSG_RUN_RESTORE);
        // msg.obj = new RestoreParams(getTransport(mCurrentTransport), NULL,
        //         restoreSet, pkg, token, TRUE);
        // mBackupHandler.sendMessage(msg);

        void (STDCALL CBackupManagerService::*pHandlerFunc)(RestoreParams*);
        pHandlerFunc = &CBackupManagerService::HandleRestore;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        CBackupManagerService::RestoreParams data(GetTransport(mCurrentTransport), NULL,
                restoreSet, pkg, token, TRUE);
        params->WriteInt32((Handle32)(Handle32*) &data);

        mBackupHandler->PostCppCallbackDelayed(
            (Handle32)this, *(Handle32*)&pHandlerFunc,
            params, 0, 0);
    } else {
        // Auto-restore disabled or no way to attempt a restore; just tell the Package
        // Manager to proceed with the post-install handling for this package.
        if (DEBUG) Slogger::V(TAG, "No restore set -- skipping restore");
        //try {
        mPackageManagerBinder->FinishCapsuleInstall(token);
        //} catch (RemoteException e) { /* can't happen */ }
    }

    return NOERROR;
}

// Hand off a restore session
ECode CBackupManagerService::BeginRestoreSession(
    /* [in] */ const String& packageName,
    /* [in] */ const String& transport,
    /* [out] */ IRestoreSessionStub** session)
{
    assert(session != NULL);
    if (DEBUG) Slogger::V(TAG, StringBuffer("beginRestoreSession: pkg=") + packageName
            + " transport=" + transport);

    Boolean needPermission = TRUE;

    String tmpPort = transport;
    if (tmpPort == NULL) {
        tmpPort = mCurrentTransport;

        if (!packageName.IsNull()) {
            AutoPtr<ICapsuleInfo> app;
            //try {
            mPackageManager->GetCapsuleInfo(packageName, 0, (ICapsuleInfo**) &app);
            // } catch (NameNotFoundException nnf) {
            //     Slog.w(TAG, "Asked to restore nonexistent pkg " + packageName);
            //     throw new IllegalArgumentException("Package " + packageName + " not found");
            // }

            assert(app != NULL);
            AutoPtr<IApplicationInfo> appInfo;
            app->GetApplicationInfo((IApplicationInfo**) &appInfo);
            assert(appInfo != NULL);
            Int32 uid = 0;
            appInfo->GetUid(&uid);
            if (uid == Binder::GetCallingUid()) {
                // So: using the current active transport, and the caller has asked
                // that its own package will be restored.  In this narrow use case
                // we do not require the caller to hold the permission.
                needPermission = FALSE;
            }
        }
    }

    if (needPermission) {
        mContext->EnforceCallingOrSelfPermission(
                String("android.Manifest.permission.BACKUP"),
                String("beginRestoreSession"));
    } else {
        if (DEBUG) Slogger::D(TAG, "restoring self on current transport; no permission needed");
    }

    {
        Mutex::Autolock lock(mBackupServiceLock);
        if (mActiveRestoreSession != NULL) {
            Slogger::D(TAG, "Restore session requested but one already active");
            *session = NULL;
            return NOERROR;
        }

        mActiveRestoreSession = new ActiveRestoreSession(packageName, tmpPort, this);
    }

    *session = mActiveRestoreSession;
    if (*session != NULL) {
        (*session)->AddRef();
    }

    return NOERROR;
}

// Note that a currently-active backup agent has notified us that it has
// completed the given outstanding asynchronous backup/restore operation.
ECode CBackupManagerService::OpComplete(
    /* [in] */ Int32 token)
{
    {
        Mutex::Autolock lock(mCurrentOpLock);
        if (DEBUG) Slogger::V(TAG, StringBuffer("opComplete: ") + token);
        mCurrentOperations->Put(token, OP_ACKNOWLEDGED);

        //TODO
        //mCurrentOpLock.notifyAll();
    }

    return NOERROR;
}

void CBackupManagerService::InitPackageTracking()
{
    if (DEBUG) Slogger::V(TAG, "Initializing package tracking");

    // Remember our ancestral dataset
    CFile::New(mBaseStateDir, String("ancestral"), (IFile**) &mTokenFile);

    //try {
    AutoPtr<IRandomAccessFile> tf;
    CRandomAccessFile::New(mTokenFile, String("r"), (IRandomAccessFile**) &tf);
    assert(tf != NULL);
    AutoPtr<IDataInput> dataInput = IDataInput::Probe(tf);

    Int32 version = 0;
    dataInput->ReadInt32(&version);
    if (version == CURRENT_ANCESTRAL_RECORD_VERSION) {
        dataInput->ReadInt64(&mAncestralToken);
        dataInput->ReadInt64(&mCurrentToken);

        Int32 numPackages = 0;
        dataInput->ReadInt32(&numPackages);
        if (numPackages >= 0) {
            for (Int32 i = 0; i < numPackages; i++) {
                String pkgName;
                //TODO
                // tf->ReadUTF(&pkgName);
                assert(0);
                mAncestralPackages.Insert(pkgName);
            }
        }
    }
    // } catch (FileNotFoundException fnf) {
    //     // Probably innocuous
    //     Slog.v(TAG, "No ancestral data");
    // } catch (IOException e) {
    //     Slog.w(TAG, "Unable to read token file", e);
    // }

    // Keep a log of what apps we've ever backed up.  Because we might have
    // rebooted in the middle of an operation that was removing something from
    // this log, we sanity-check its contents here and reconstruct it.
    CFile::New(mBaseStateDir, String("processed"), (IFile**) &mEverStored);
    AutoPtr<IFile> tempProcessedFile;
    CFile::New(mBaseStateDir, String("processed.new"), (IFile**) &tempProcessedFile);

    // If we were in the middle of removing something from the ever-backed-up
    // file, there might be a transient "processed.new" file still present.
    // Ignore it -- we'll validate "processed" against the current package set.
    Boolean flag = FALSE;
    if (tempProcessedFile->Exists(&flag), flag) {
        tempProcessedFile->Delete(&flag);
    }

    // If there are previous contents, parse them out then start a new
    // file to continue the recordkeeping.
    if (mEverStored->Exists(&flag), flag) {
        AutoPtr<IRandomAccessFile> temp;
        AutoPtr<IRandomAccessFile> in;

        //try {
        CRandomAccessFile::New(tempProcessedFile, String("rws"), (IRandomAccessFile**) &temp);
        CRandomAccessFile::New(mEverStored, String("r"), (IRandomAccessFile**) &in);

        while (TRUE) {
            AutoPtr<ICapsuleInfo> info;
            String pkg;

            //TODO
            assert(0);
            //in->ReadUTF(&pkg);

            //try {
            mPackageManager->GetCapsuleInfo(pkg, 0, (ICapsuleInfo**) &info);
            mEverStoredApps.Insert(pkg);


            //temp.WriteUTF(pkg);
            ArrayOf<Byte>* utf = StringToByteArray(pkg);
            //TODO ??no WriteUTF api
            temp->WriteBuffer(*utf);
            ArrayOf<Byte>::Free(utf);

            if (DEBUG) Slogger::V(TAG, StringBuffer("   + ") + pkg);
            // } catch (NameNotFoundException e) {
            //     // nope, this package was uninstalled; don't include it
            //     if (DEBUG) Slog.v(TAG, "   - " + pkg);
            // }
        }
        // } catch (EOFException e) {
        //     // Once we've rewritten the backup history log, atomically replace the
        //     // old one with the new one then reopen the file for continuing use.
        //     if (!tempProcessedFile.renameTo(mEverStored)) {
        //         Slog.e(TAG, "Error renaming " + tempProcessedFile + " to " + mEverStored);
        //     }
        // } catch (IOException e) {
        //     Slog.e(TAG, "Error in processed file", e);
        // } finally {
        //try {
        if (temp != NULL) temp->Close();
        //} catch (IOException e) {}

        //try {
        if (in != NULL) in->Close();
        //} catch (IOException e) {}
        //}
    }

    // Register for broadcasts about package install, etc., so we can
    // update the provider list.
    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New((IIntentFilter**) &filter);
    filter->AddAction(String(Intent_ACTION_CAPSULE_ADDED));
    filter->AddAction(String(Intent_ACTION_CAPSULE_REMOVED));
    filter->AddDataScheme(String("package"));

    //TODO
    assert(0);
    //mContext->RegisterReceiver(mBroadcastReceiver, filter);

    // Register for events related to sdcard installation.
    AutoPtr<IIntentFilter> sdFilter;
    CIntentFilter::New((IIntentFilter**) &sdFilter);
    sdFilter->AddAction(String(Intent_ACTION_EXTERNAL_APPLICATIONS_AVAILABLE));
    sdFilter->AddAction(String(Intent_ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE));

    //TODO
    assert(0);
    //mContext->RegisterReceiver(mBroadcastReceiver, sdFilter);
}

void CBackupManagerService::ParseLeftoverJournals()
{
    assert(mJournalDir != NULL);
    AutoPtr<IObjectContainer> fileList;
    CObjectContainer::New((IObjectContainer**)&fileList);
    mJournalDir->ListFiles((IObjectContainer**) &fileList);

    AutoPtr<IObjectEnumerator> fileIt;
    fileList->GetObjectEnumerator((IObjectEnumerator**)&fileIt);
    Boolean hasNext = FALSE;
    while(fileIt->MoveNext(&hasNext), hasNext) {
        AutoPtr<IFile> f;
        fileIt->Current((IInterface**)&f);
        assert(f != NULL);

        Int32 result = 0;
        if (mJournal == NULL || (f->CompareTo(mJournal, &result), result != 0)) {
            // This isn't the current journal, so it must be a leftover.  Read
            // out the package names mentioned there and schedule them for
            // backup.
            AutoPtr<IRandomAccessFile> in;
            //try {
            Slogger::I(TAG, "Found stale backup journal, scheduling");
            CRandomAccessFile::New(f, String("r"), (IRandomAccessFile**) &in);
            while (TRUE) {
                String packageName;

                //TODO
                assert(0);
                //in->ReadUTF(&packageName);

                Slogger::I(TAG, StringBuffer("  ") + packageName);
                DataChangedImpl(packageName);
            }
            // } catch (EOFException e) {
            //     // no more data; we're done
            // } catch (Exception e) {
            //     Slog.e(TAG, "Can't read " + f, e);
            // } finally {
            // close/delete the file
            //try {
            if (in != NULL) in->Close();
            //} catch (IOException e) {}

            Boolean successed = FALSE;
            f->Delete(&successed);
            //}
        }
    }

}

void CBackupManagerService::RecordInitPendingLocked(
    /* [in] */ Boolean isPending,
    /* [in] */ const String& transportName)
{
    if (DEBUG) Slogger::I(TAG, StringBuffer("recordInitPendingLocked: ") + isPending
            + " on transport " + transportName);
    //try {
    AutoPtr<IBackupTransport> transport = GetTransport(transportName);
    String transportDirName;
    transport->TransportDirName(&transportDirName);
    AutoPtr<IFile> stateDir;
    CFile::New(mBaseStateDir, transportDirName, (IFile**) &stateDir);

    AutoPtr<IFile> initPendingFile;
    CFile::New(stateDir, String((const char*)INIT_SENTINEL_FILE_NAME), (IFile**) &initPendingFile);

    if (isPending) {
        // We need an init before we can proceed with sending backup data.
        // Record that with an entry in our set of pending inits, as well as
        // journaling it via creation of a sentinel file.
        mPendingInits.Insert(transportName);
        //try {
        AutoPtr<IFileOutputStream> tmpFOS;

        CFileOutputStream::New(initPendingFile, (IFileOutputStream**) &tmpFOS);
        tmpFOS->Close();
        // } catch (IOException ioe) {
        //     // Something is badly wrong with our permissions; just try to move on
        // }
    } else {
        // No more initialization needed; wipe the journal and reset our state.
        Boolean successed = FALSE;
        initPendingFile->Delete(&successed);
        mPendingInits.Erase(transportName);
    }
    // } catch (RemoteException e) {
    //     // can't happen; the transport is local
    // }
}

void CBackupManagerService::ResetBackupState(
    /* [in] */ IFile* stateFileDir)
{
    {
        Mutex::Autolock lock(mQueueLock);
        // Wipe the "what we've ever backed up" tracking
        mEverStoredApps.Clear();

        Boolean successed = FALSE;
        mEverStored->Delete(&successed);

        mCurrentToken = 0;
        WriteRestoreTokens();

        // Remove all the state files
        assert(stateFileDir != NULL);
        AutoPtr<IObjectContainer> fileList;
        CObjectContainer::New((IObjectContainer**)&fileList);
        stateFileDir->ListFiles((IObjectContainer**) &fileList);

        AutoPtr<IObjectEnumerator> fileIt;
        fileList->GetObjectEnumerator((IObjectEnumerator**)&fileIt);
        Boolean hasNext = FALSE;
        while(fileIt->MoveNext(&hasNext), hasNext) {
            AutoPtr<IFile> sf;
            fileIt->Current((IInterface**)&sf);
            assert(sf != NULL);

            // ... but don't touch the needs-init sentinel
            String name;
            sf->GetName(&name);
            if (!name.Equals((const char*)INIT_SENTINEL_FILE_NAME)) {
                sf->Delete(&successed);
            }
        }

        // Enqueue a new backup of every participant
        Map<Int32, HashSet<AutoPtr<IApplicationInfo> >*>::Iterator it;
        for (it = mBackupParticipants.Begin(); it != mBackupParticipants.End(); ++it) {
            //Int32 uid = it->mFirst;
            HashSet<AutoPtr<IApplicationInfo> >* participants = it->mSecond;
            assert(participants != NULL);
            HashSet<AutoPtr<IApplicationInfo> >::Iterator app = participants->Begin();
            for (; app != participants->End(); ++app) {
                String name;
                assert(*app != NULL);
                (*app)->GetCapsuleName(&name);
                DataChangedImpl(name);
            }
        }
    }
}

void CBackupManagerService::RegisterTransport(
    /* [in] */ const String& name,
    /* [in] */ IBackupTransport* transport)
{
    {
        Mutex::Autolock lock(mTransportsLock);
        if (DEBUG) Slogger::V(TAG, StringBuffer("Registering transport ") + name + " = " + transport);
        if (transport != NULL) {
            mTransports[name] = transport;
        } else {
            mTransports.Erase(name);
            if ((mCurrentTransport != NULL) && mCurrentTransport.Equals(name)) {
                mCurrentTransport = NULL;
            }
            // Nothing further to do in the unregistration case
            return;
        }
    }

    // If the init sentinel file exists, we need to be sure to perform the init
    // as soon as practical.  We also create the state directory at registration
    // time to ensure it's present from the outset.
    //try {
    String transportName;
    transport->TransportDirName(&transportName);
    AutoPtr<IFile> stateDir;
    CFile::New(mBaseStateDir, transportName, (IFile**) &stateDir);
    Boolean tmpFlag = FALSE;
    stateDir->Mkdirs(&tmpFlag);

    AutoPtr<IFile> initSentinel;
    CFile::New(stateDir, String(INIT_SENTINEL_FILE_NAME), (IFile**) &initSentinel);

    initSentinel->Exists(&tmpFlag);
    if (tmpFlag) {
        {
            Mutex::Autolock lock(mQueueLock);
            mPendingInits.Insert(transportName);

            // TODO: pick a better starting time than now + 1 minute
            // TODO
            //Int64 delay = 1000 * 60; // one minute, in milliseconds
            // mAlarmManager.set(AlarmManager.RTC_WAKEUP,
            //         System.currentTimeMillis() + delay, mRunInitIntent);
        }
    }
    // } catch (RemoteException e) {
    //     // can't happen, the transport is local
    // }
}

void CBackupManagerService::AddPackageParticipantsLocked(
    /* [in] */ const String& packageName)
{
    // Look for apps that define the android:backupAgent attribute
    if (DEBUG) Slogger::V(TAG, StringBuffer("addPackageParticipantsLocked: ") + packageName);
    List<AutoPtr<ICapsuleInfo> >* targetApps = AllAgentPackages();
    AddPackageParticipantsLockedInner(packageName, targetApps);

    if (targetApps != NULL) {
        delete targetApps;
        targetApps = NULL;
    }
}

void CBackupManagerService::AddPackageParticipantsLockedInner(
    /* [in] */ const String& packageName,
    /* [in] */ List<AutoPtr<ICapsuleInfo> >* targetPkgs)
{
    assert(targetPkgs != NULL);

    // if (DEBUG) {
    //     Slogger::V(TAG, StringBuffer("Adding ") + targetPkgs.size() + " backup participants:");
    //     for (PackageInfo p : targetPkgs) {
    //         Slog.v(TAG, "    " + p + " agent=" + p.applicationInfo.backupAgentName
    //                 + " uid=" + p.applicationInfo.uid
    //                 + " killAfterRestore="
    //                 + (((p.applicationInfo.flags & ApplicationInfo.FLAG_KILL_AFTER_RESTORE) != 0) ? "true" : "false")
    //                 );
    //     }
    // }

    List<AutoPtr<ICapsuleInfo> >::Iterator pkg;
    for(pkg = targetPkgs->Begin(); pkg != targetPkgs->End(); ++ pkg) {
        String pkName;
        assert((*pkg) != NULL);
        (*pkg)->GetCapsuleName(&pkName);

        if (packageName.IsNull() || pkName.Equals(packageName)) {
            AutoPtr<IApplicationInfo> appInfo;
            (*pkg)->GetApplicationInfo((IApplicationInfo**) &appInfo);
            assert(appInfo != NULL);

            Int32 uid = 0;
            appInfo->GetUid(&uid);
            HashSet<AutoPtr<IApplicationInfo> >* set = mBackupParticipants[uid];
            if (set == NULL) {
                set = new HashSet<AutoPtr<IApplicationInfo> >();
                mBackupParticipants[uid] = set;
            }

            set->Insert(appInfo);

            // If we've never seen this app before, schedule a backup for it
            Boolean has = FALSE;
            HashSet<String>::Iterator item = mEverStoredApps.Begin();
            for (; item != mEverStoredApps.End(); ++ item) {
                if ((*item).Equals(pkName)) {
                    has = TRUE;
                    break;
                }
            }

            if (!has) {
                if (DEBUG) Slogger::I(TAG, StringBuffer("New app ") + (const char*)pkName
                        + " never backed up; scheduling");
                DataChangedImpl(pkName);
            }
        }
    }
}

// Remove the given package's entry from our known active set.  If
// 'packageName' is NULL, *all* participating apps will be removed.
void CBackupManagerService::RemovePackageParticipantsLocked(
    /* [in] */ const String& packageName)
{
    if (DEBUG) Slogger::V(TAG, StringBuffer("removePackageParticipantsLocked: ") + packageName);
    List<AutoPtr<ICapsuleInfo> >* allApps = NULL;
    if (!packageName.IsNull()) {
        //try {
        Int32 flags = CapsuleManager_GET_SIGNATURES;
        AutoPtr<ICapsuleInfo> capInfo;
        mPackageManager->GetCapsuleInfo(packageName, flags, (ICapsuleInfo**) &capInfo);
        allApps->PushBack(capInfo);
        // } catch (Exception e) {
        //     // just skip it (???)
        // }
    } else {
        // all apps with agents
        allApps = AllAgentPackages();
    }

    RemovePackageParticipantsLockedInner(packageName, allApps);

    if (allApps != NULL) {
        delete allApps;
        allApps = NULL;
    }
}

void CBackupManagerService::RemovePackageParticipantsLockedInner(
    /* [in] */ const String& packageName,
    /* [in] */ List<AutoPtr<ICapsuleInfo> >* agents)
{
    // if (DEBUG) {
    //     Slog.v(TAG, "removePackageParticipantsLockedInner (" + packageName
    //             + ") removing " + agents.size() + " entries");
    //     for (PackageInfo p : agents) {
    //         Slog.v(TAG, "    - " + p);
    //     }
    // }
    assert(agents != NULL);
    List<AutoPtr<ICapsuleInfo> >::Iterator pkg = agents->Begin();
    for (; pkg != agents->End(); ++ pkg) {
        String pkName;
        assert((*pkg) != NULL);
        (*pkg)->GetCapsuleName(&pkName);

        if (packageName.IsNull() || pkName.Equals(packageName)) {
            AutoPtr<IApplicationInfo> appInfo;
            (*pkg)->GetApplicationInfo((IApplicationInfo**) &appInfo);
            assert(appInfo != NULL);

            Int32 uid = 0;
            appInfo->GetUid(&uid);

            HashSet<AutoPtr<IApplicationInfo> >* set = mBackupParticipants[uid];
            if (set != NULL) {
                // Find the existing entry with the same package name, and remove it.
                // We can't just remove(app) because the instances are different.
                HashSet<AutoPtr<IApplicationInfo> >::Iterator entry = set->Begin();
                for (; entry != set->End(); ++ entry) {
                    String name;
                    assert((*entry) != NULL);
                    (*entry)->GetCapsuleName(&name);

                    if (name.Equals(pkName)) {
                        set->Erase(entry);
                        RemoveEverBackedUp(pkName);
                        break;
                    }
                }

                if (set->GetSize() == 0) {
                    mBackupParticipants.Erase(uid);
                }
            }
        }
    }
}

List<AutoPtr<ICapsuleInfo> >* CBackupManagerService::AllAgentPackages()
{
    // !!! TODO: cache this and regenerate only when necessary
    Int32 flags = CapsuleManager_GET_SIGNATURES;

    AutoPtr<IObjectContainer> capsules;
    CObjectContainer::New((IObjectContainer**)&capsules);
    mPackageManager->GetInstalledCapsules(flags, (IObjectContainer**)&capsules);

    List<AutoPtr<ICapsuleInfo> >* packages = new List<AutoPtr<ICapsuleInfo> >();
    AutoPtr<IObjectEnumerator> pkgIt;
    capsules->GetObjectEnumerator((IObjectEnumerator**)&pkgIt);
    Boolean hasNext = FALSE;
    while(pkgIt->MoveNext(&hasNext), hasNext) {
        AutoPtr<ICapsuleInfo> pkg;
        pkgIt->Current((IInterface**)&pkg);
        packages->PushBack(pkg);
    }

    Int32 N = packages->GetSize();
    for (Int32 a = N-1; a >= 0; a--) {
        AutoPtr<ICapsuleInfo> pkg = (*packages)[a];
        assert(pkg != NULL);
        //try {
        AutoPtr<IApplicationInfo> app;
        pkg->GetApplicationInfo((IApplicationInfo**) &app);
        assert(app != NULL);

        Int32 flags = 0;
        app->GetFlags(&flags);
        String agentName;
        app->GetBackupAgentName(&agentName);
        if (((flags & ApplicationInfo_FLAG_ALLOW_BACKUP) == 0)
                || agentName.IsNull()) {
            packages->Remove(a);
        }
        else {
            // we will need the shared library path, so look that up and store it here
            String name;
            pkg->GetCapsuleName(&name);

            AutoPtr<IApplicationInfo> tmpApp;
            mPackageManager->GetApplicationInfo(name,
                    CapsuleManager_GET_SHARED_LIBRARY_FILES,
                    (IApplicationInfo**) &tmpApp);

            ArrayOf<String>* sharedLibraryFiles = NULL;
            tmpApp->GetSharedLibraryFiles(&sharedLibraryFiles);
            app->SetSharedLibraryFiles(sharedLibraryFiles);

            if (sharedLibraryFiles != NULL) {
                for (Int32 i = 0; i < sharedLibraryFiles->GetLength(); ++i) {
                    (*sharedLibraryFiles)[i].~String();
                }
                ArrayOf<String>::Free(sharedLibraryFiles);
                sharedLibraryFiles = NULL;
            }
        }
        // } catch (NameNotFoundException e) {
        //     (*packages)->Remove(a);
        // }
    }

    return packages;
}

// Reset the given package's known backup participants.  Unlike add/remove, the update
// action cannot be passed a NULL package name.
void CBackupManagerService::UpdatePackageParticipantsLocked(
    /* [in] */ const String& packageName)
{
    if (packageName.IsNull()) {
        Slogger::E(TAG, "updatePackageParticipants called with NULL package name");
        return;
    }

    if (DEBUG) Slogger::V(TAG, StringBuffer("updatePackageParticipantsLocked: ") + packageName);

    // brute force but small code size
    List<AutoPtr<ICapsuleInfo> >* allApps = AllAgentPackages();
    RemovePackageParticipantsLockedInner(packageName, allApps);
    AddPackageParticipantsLockedInner(packageName, allApps);

    if (allApps != NULL) {
        delete allApps;
        allApps = NULL;
    }
}

// Called from the backup task: record that the given app has been successfully
// backed up at least once
void CBackupManagerService::LogBackupComplete(
    /* [in] */ const String& packageName)
{
    if (packageName.Equals((const char*)PACKAGE_MANAGER_SENTINEL)) return;

    {
        Mutex::Autolock lock(mEverStoredAppsLock);

        HashSet<String>::Iterator item = mEverStoredApps.Find(packageName);
        if (item != mEverStoredApps.End()) {
            return;
        }

        mEverStoredApps.Insert(packageName);
        //if (!mEverStoredApps.add(packageName)) return;

        AutoPtr<IRandomAccessFile> out;
        //try {
        CRandomAccessFile::New(mEverStored, String("rws"), (IRandomAccessFile**) &out);
        Int64 len = 0;
        out->GetLength(&len);
        out->Seek(len);

        //out->WriteUTF(packageName);
        ArrayOf<Byte>* utf = StringToByteArray(packageName);
        out->WriteBuffer(*utf);
        ArrayOf<Byte>::Free(utf);


        // } catch (IOException e) {
        //     Slog.e(TAG, "Can't log backup of " + packageName + " to " + mEverStored);
        // } finally {
        //try {
        if (out != NULL) out->Close();
        //} catch (IOException e) {}
        //}
    }
}

// Remove our awareness of having ever backed up the given package
void CBackupManagerService::RemoveEverBackedUp(
    /* [in] */ const String& packageName)
{
    if (DEBUG) Slogger::V(TAG, StringBuffer("Removing backed-up knowledge of ")
        + packageName + ", new set:");

    Mutex::Autolock lock(mEverStoredAppsLock);
    // Rewrite the file and rename to overwrite.  If we reboot in the middle,
    // we'll recognize on initialization time that the package no longer
    // exists and fix it up then.
    AutoPtr<IFile> tempKnownFile;
    CFile::New(mBaseStateDir, String("processed.new"), (IFile**) &tempKnownFile);
    AutoPtr<IRandomAccessFile> known;
    //try {
    CRandomAccessFile::New(tempKnownFile, String("rws"), (IRandomAccessFile**) &known);
    mEverStoredApps.Erase(packageName);

    HashSet<String>::Iterator s = mEverStoredApps.Begin();
    for (; s != mEverStoredApps.End(); ++s) {
        //known.writeUTF(s);
        ArrayOf<Byte>* utf = StringToByteArray(*s);
        known->WriteBuffer(*utf);
        ArrayOf<Byte>::Free(utf);

        //if (DEBUG) Slog.v(TAG, "    " + s);
    }

    known->Close();
    known = NULL;

    Boolean successed = FALSE;
    if (tempKnownFile->RenameTo(mEverStored, &successed), !successed) {
        //throw new IOException("Can't rename " + tempKnownFile + " to " + mEverStored);
        assert(0);
    }
    // } catch (IOException e) {
    //     // Bad: we couldn't create the new copy.  For safety's sake we
    //     // abandon the whole process and remove all what's-backed-up
    //     // state entirely, meaning we'll force a backup pass for every
    //     // participant on the next boot or [re]install.
    //     Slog.w(TAG, "Error rewriting " + mEverStored, e);
    //     mEverStoredApps.clear();
    //     tempKnownFile.delete();
    //     mEverStored.delete();
    // } finally {
    //try {
    if (known != NULL) known->Close();
    //} catch (IOException e) {}
    //}
}

// Persistently record the current and ancestral backup tokens as well
// as the set of packages with data [supposedly] available in the
// ancestral dataset.
void CBackupManagerService::WriteRestoreTokens()
{
    //try {
    AutoPtr<IRandomAccessFile> af;
    CRandomAccessFile::New(mTokenFile, String("rwd"), (IRandomAccessFile**) &af);

    AutoPtr<IDataOutput> afOutput;
    afOutput = IDataOutput::Probe(af);
    assert(afOutput != NULL);
    // First, the version number of this record, for futureproofing
    afOutput->WriteInt32(CURRENT_ANCESTRAL_RECORD_VERSION);

    // Write the ancestral and current tokens
    afOutput->WriteInt64(mAncestralToken);
    afOutput->WriteInt64(mCurrentToken);

    // Now write the set of ancestral packages
    if (mAncestralPackages.GetSize() == 0) {
        afOutput->WriteInt32(-1);
    } else {
        afOutput->WriteInt32(mAncestralPackages.GetSize());
        if (DEBUG) Slogger::V(TAG, StringBuffer("Ancestral packages:  ") + mAncestralPackages.GetSize());

        Set<String>::Iterator pkgName = mAncestralPackages.Begin();
        for (; pkgName != mAncestralPackages.End(); ++pkgName) {
            //af.writeUTF(pkgName);
            ArrayOf<Byte>* utf = StringToByteArray(*pkgName);
            af->WriteBuffer(*utf);
            ArrayOf<Byte>::Free(utf);

            if (DEBUG) Slogger::V(TAG, StringBuffer("   ") + *pkgName);
        }
    }

    af->Close();
    // } catch (IOException e) {
    //     Slog.w(TAG, "Unable to write token file:", e);
    // }
}

// Return the given transport
AutoPtr<IBackupTransport> CBackupManagerService::GetTransport(
    /* [in] */ const String& transportName)
{
    Mutex::Autolock lock(mTransportsLock);
    AutoPtr<IBackupTransport> transport = mTransports[transportName];
    if (transport == NULL) {
        Slogger::W(TAG, StringBuffer("Requested unavailable transport: ") + transportName);
    }

    return transport;
}

// fire off a backup agent, blocking until it attaches or times out
AutoPtr<IBackupAgent> CBackupManagerService::BindToAgentSynchronous(
    /* [in] */ IApplicationInfo* app,
    /* [in] */ Int32 mode)
{
    AutoPtr<IBackupAgent> agent = NULL;
    {
        Mutex::Autolock lock(mAgentConnectLock);
        mConnecting = TRUE;
        mConnectedAgent = NULL;
        //try {
        Boolean ok = FALSE;
        mActivityManager->BindBackupAgent(app, mode, &ok);
        if (ok) {
            Slogger::D(TAG, StringBuffer("awaiting agent for ") + app);

            // success; wait for the agent to arrive
            // only wait 10 seconds for the clear data to happen
            Int64 timeoutMark = System::GetCurrentTimeMillis() + TIMEOUT_INTERVAL;
            while (mConnecting && mConnectedAgent == NULL
                    && (System::GetCurrentTimeMillis() < timeoutMark)) {
                //try {

                //TODO
                //mAgentConnectLock.wait(5000);
                // } catch (InterruptedException e) {
                //     // just bail
                //     return NULL;
                // }
            }

            // if we timed out with no connect, abort and move on
            if (mConnecting == TRUE) {
                Slogger::W(TAG, StringBuffer("Timeout waiting for agent ") + app);
                return NULL;
            }

            agent = mConnectedAgent;
        }
        // } catch (RemoteException e) {
        //     // can't happen
        // }
    }

    return agent;
}

// clear an application's data, blocking until the operation completes or times out
void CBackupManagerService::ClearApplicationDataSynchronous(
    /* [in] */ const String& packageName)
{
    // Don't wipe packages marked allowClearUserData=FALSE
    // try {
    AutoPtr<ICapsuleInfo> info;
    mPackageManager->GetCapsuleInfo(packageName, 0, (ICapsuleInfo**) &info);
    assert(info != NULL);

    AutoPtr<IApplicationInfo> appInfo;
    info->GetApplicationInfo((IApplicationInfo**) &appInfo);
    assert(appInfo != NULL);

    Int32 flags = 0;
    appInfo->GetFlags(&flags);
    if ((flags & ApplicationInfo_FLAG_ALLOW_CLEAR_USER_DATA) == 0) {
        if (DEBUG) Slogger::I(TAG, StringBuffer("allowClearUserData=FALSE so not wiping ")
                + packageName);
        return;
    }
    // } catch (NameNotFoundException e) {
    //     Slog.w(TAG, "Tried to clear data for " + packageName + " but not found");
    //     return;
    // }

    ClearDataObserver* observer = new ClearDataObserver(this);

    Mutex::Autolock lock(mClearDataLock);
    mClearingData = TRUE;
    //try {
    Boolean res = FALSE;
    mActivityManager->ClearApplicationUserData(packageName, observer, &res);
    // } catch (RemoteException e) {
    //     // can't happen because the activity manager is in this process
    // }

    // only wait 10 seconds for the clear data to happen
    Int64 timeoutMark = System::GetCurrentTimeMillis() + TIMEOUT_INTERVAL;
    while (mClearingData && (System::GetCurrentTimeMillis() < timeoutMark)) {
        // try {
        //     //TODO
        //     mClearDataLock.wait(5000);
        // } catch (InterruptedException e) {
        //     // won't happen, but still.
        //     mClearingData = FALSE;
        // }
    }
}

Int64 CBackupManagerService::GetAvailableRestoreToken(
    /* [in] */ const String& packageName)
{
    Int64 token = mAncestralToken;
    {
        Mutex::Autolock lock(mQueueLock);

        Boolean has = FALSE;
        HashSet<String>::Iterator it = mEverStoredApps.Begin();
        if (it != mEverStoredApps.End()) {
            has = TRUE;
        }

        if (has) {
            token = mCurrentToken;
        }
    }

    return token;
}

// -----
// Utility methods used by the asynchronous-with-timeout backup/restore operations
Boolean CBackupManagerService::WaitUntilOperationComplete(
    /* [in] */ Int32 token)
{
    Int32 finalState = OP_PENDING;
    {
        Mutex::Autolock lock(mCurrentOpLock);
        //try {
        while ((finalState = mCurrentOperations->Get(token, OP_TIMEOUT)) == OP_PENDING) {
            // try {
            //     //TODO
            //     mCurrentOpLock.wait();
            // } catch (InterruptedException e) {}
        }
        // } catch (IndexOutOfBoundsException e) {
        //     // the operation has been mysteriously cleared from our
        //     // bookkeeping -- consider this a success and ignore it.
        // }
    }

    void (STDCALL CBackupManagerService::*pHandlerFunc)(Int32);
    pHandlerFunc = &CBackupManagerService::HandleMSGTimeout;
    mBackupHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);

    if (DEBUG) Slogger::V(TAG, StringBuffer("operation ") + token
            + " complete: finalState=" + finalState);
    return finalState == OP_ACKNOWLEDGED;
}

void CBackupManagerService::PrepareOperationTimeout(
    /* [in] */ Int32 token,
    /* [in] */ Int64 interval)
{
    if (DEBUG) Slogger::V(TAG, StringBuffer("starting timeout: token=") + token
            + " interval=" + interval);

    assert(mCurrentOperations != NULL);
    mCurrentOperations->Put(token, OP_PENDING);

    void (STDCALL CBackupManagerService::*pHandlerFunc)(Int32);
    pHandlerFunc = &CBackupManagerService::HandleMSGTimeout;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(token);
    mBackupHandler->PostCppCallbackDelayed(
        (Handle32)this, *(Handle32*)&pHandlerFunc,
        params, 0, 0);
}

Boolean CBackupManagerService::SignaturesMatch(
    /* [in] */ List<AutoPtr<ISignature> >* storedSigs,
    /* [in] */ ICapsuleInfo* target)
{
    assert(target != NULL);
    // If the target resides on the system partition, we allow it to restore
    // data from the like-named package in a restore set even if the signatures
    // do not match.  (Unlike general applications, those flashed to the system
    // partition will be signed with the device's platform certificate, so on
    // different phones the same system app will have different signatures.)
    AutoPtr<IApplicationInfo> appInfo;
    target->GetApplicationInfo((IApplicationInfo**) &appInfo);
    assert(appInfo != NULL);
    Int32 flags = 0;
    appInfo->GetFlags(&flags);
    if ((flags & ApplicationInfo_FLAG_SYSTEM) != 0) {
        String name;
        target->GetCapsuleName(&name);
        if (DEBUG) Slogger::V(TAG, StringBuffer("System app ") + name + " - skipping sig check");
        return TRUE;
    }

    // Allow unsigned apps, but not signed on one device and unsigned on the other
    // !!! TODO: is this the right policy?
    AutoPtr<IObjectContainer> deviceSigs;
    FAIL_RETURN(CObjectContainer::New((IObjectContainer**)&deviceSigs);
    target->GetSignatures((IObjectContainer**)&deviceSigs));

    Int32 dLen = 0;
    if (deviceSigs != NULL) {
        deviceSigs->GetObjectCount(&dLen);
    }

    // if (DEBUG) Slog.v(TAG, "signaturesMatch(): stored=" + storedSigs
    //         + " device=" + deviceSigs);
    if ((storedSigs == NULL || storedSigs->GetSize() == 0)
            && (deviceSigs == NULL || dLen == 0)) {
        return TRUE;
    }

    if (storedSigs == NULL || deviceSigs == NULL) {
        return FALSE;
    }

    // !!! TODO: this demands that every stored signature match one
    // that is present on device, and does not demand the converse.
    // Is this this right policy?
    List<AutoPtr<ISignature> >::Iterator storeIt = storedSigs->Begin();

    for (; storeIt != storedSigs->End(); ++storeIt) {
        Boolean match = FALSE;

        AutoPtr<IObjectEnumerator> deviceIt;
        deviceSigs->GetObjectEnumerator((IObjectEnumerator**)&deviceIt);
        Boolean hasNext = FALSE;
        while(deviceIt->MoveNext(&hasNext), hasNext) {
            AutoPtr<ISignature> s;
            deviceIt->Current((IInterface**)&s);

            Boolean isEquals = FALSE;
            if ((*storeIt)->Equals(s, &isEquals), isEquals) {
                match = TRUE;
                break;
            }
        }

        if (!match) {
            return FALSE;
        }
    }

    return TRUE;
}

void CBackupManagerService::DataChangedImpl(
    /* [in] */ const String& packageName)
{
    HashSet<AutoPtr<IApplicationInfo> >* targets = DataChangedTargets(packageName);
    DataChangedImpl(packageName, targets);

    //TODO : Need to delete??
    //delete targets;
}

void CBackupManagerService::DataChangedImpl(
    /* [in] */ const String& packageName,
    /* [in] */ HashSet<AutoPtr<IApplicationInfo> >* targets)
{
    // Record that we need a backup pass for the caller.  Since multiple callers
    // may share a uid, we need to note all candidates within that uid and schedule
    // a backup pass for each of them.
    //EventLog.writeEvent(EventLogTags.BACKUP_DATA_CHANGED, packageName);

    if (targets == NULL) {
        Slogger::W(TAG, StringBuffer("dataChanged but no participant pkg='") + packageName + "'"
               + " uid=" + Binder::GetCallingUid());
        return;
    }

    {
        Mutex::Autolock lock(mQueueLock);
        // Note that this client has made data changes that need to be backed up
        HashSet<AutoPtr<IApplicationInfo> >::Iterator app = targets->Begin();

        for (; app != targets->End(); ++app) {
            // validate the caller-supplied package name against the known set of
            // packages associated with this uid
            String capName;
            (*app)->GetCapsuleName(&capName);
            if (capName.Equals(packageName)) {
                // Add the caller to the set of pending backups.  If there is
                // one already there, then overwrite it, but no harm done.
                BackupRequest* old = mPendingBackups[*app];
                BackupRequest* req = new BackupRequest(*app, FALSE);

                mPendingBackups[*app] = req;
                if (old == NULL) {
                    // Journal this request in case of crash.  The put()
                    // operation returned NULL when this package was not already
                    // in the set; we want to avoid touching the disk redundantly.
                    WriteToJournalLocked(packageName);

                    // if (DEBUG) {
                    //     Int32 numKeys = mPendingBackups.size();
                    //     Slog.d(TAG, "Now awaiting backup for " + numKeys + " participants:");
                    //     for (BackupRequest b : mPendingBackups.values()) {
                    //         Slog.d(TAG, "    + " + b + " agent=" + b.appInfo.backupAgentName);
                    //     }
                    // }
                }
            }
        }
    }
}

// Note: packageName is currently unused, but may be in the future
HashSet<AutoPtr<IApplicationInfo> >* CBackupManagerService::DataChangedTargets(
    /* [in] */ const String& packageName)
{
    // If the caller does not hold the BACKUP permission, it can only request a
    // backup of its own data.
    Int32 res = 0;
    mContext->CheckPermission(String("android.Manifest.permission.BACKUP"), Binder::GetCallingPid(),
            Binder::GetCallingUid(), &res);

    HashSet<AutoPtr<IApplicationInfo> >* targets = new HashSet<AutoPtr<IApplicationInfo> >();
    if (res == CapsuleManager_PERMISSION_DENIED) {
        {
            Mutex::Autolock lock(mBackupParticipantsLock);

            HashSet<AutoPtr<IApplicationInfo> >* s = mBackupParticipants[Binder::GetCallingUid()];
            if (s != NULL) {
                HashSet<AutoPtr<IApplicationInfo> >::Iterator it = s->Begin();
                for (; it != s->End(); ++it) {
                    targets->Insert(*it);
                }
            }

            return targets;
        }
    }

    // a caller with full permission can ask to back up any participating app
    // !!! TODO: allow backup of ANY app?
    {
        Mutex::Autolock lock(mBackupParticipantsLock);
        Int32 N = mBackupParticipants.GetSize();
        for (Int32 i = 0; i < N; i++) {
            HashSet<AutoPtr<IApplicationInfo> >* s = mBackupParticipants[i];
            if (s != NULL) {
                HashSet<AutoPtr<IApplicationInfo> >::Iterator it = s->Begin();
                for (; it != s->End(); ++it) {
                    targets->Insert(*it);
                }
            }
        }
    }

    return targets;
}

void CBackupManagerService::WriteToJournalLocked(
    /* [in] */ const String& str)
{
    AutoPtr<IRandomAccessFile> out;

//    try {
    if (mJournal == NULL) {
        AutoPtr<IFileHelper> helper;
        CFileHelper::AcquireSingleton((IFileHelper**)&helper);
        helper->CreateTempFileEx(String("journal"), String(""), mJournalDir, (IFile**) &mJournal);
    }

    CRandomAccessFile::New(mJournal, String("rws"), (IRandomAccessFile**) &out);
    assert(out != NULL);

    Int64 len = 0;
    out->GetLength(&len);
    out->Seek(len);

    ArrayOf<Byte>* utf = StringToByteArray(str);

    //TODO ??no WriteUTF api
    out->WriteBuffer(*utf);
    ArrayOf<Byte>::Free(utf);

    //out->WriteUTF(str);
    // } catch (IOException e) {
    //     Slog.e(TAG, "Can't write " + str + " to backup journal", e);
    //     mJournal = NULL;
    // } finally {
        //try {
    if (out != NULL) {
        out->Close();
    }
        //} catch (IOException e) {}
    //}
}

void CBackupManagerService::StartBackupAlarmsLocked(
    /* [in] */ Int64 delayBeforeFirstBackup)
{
    // We used to use setInexactRepeating(), but that may be linked to
    // backups running at :00 more often than not, creating load spikes.
    // Schedule at an exact time for now, and also add a bit of "fuzz".

    srand(time(NULL));
    Int64 when = System::GetCurrentTimeMillis() + delayBeforeFirstBackup + rand() % FUZZ_MILLIS;

    //TODO
    // mAlarmManager.setRepeating(AlarmManager.RTC_WAKEUP, when,
    //         BACKUP_INTERVAL + rand() % FUZZ_MILLIS, mRunBackupIntent);
    mNextBackupPass = when;
}

void CBackupManagerService::HandleRunBackup()
{
    mLastBackupPass = System::GetCurrentTimeMillis();
    mNextBackupPass = mLastBackupPass + BACKUP_INTERVAL;

    AutoPtr<IBackupTransport> transport = GetTransport(mCurrentTransport);
    if (transport == NULL) {
        Slogger::V(TAG, StringBuffer("Backup requested but no transport available"));
        //TODO
        //mWakelock.release();
        return;
    }

    // snapshot the pending-backup set and work on that
    List<BackupRequest*>* queue = new List<BackupRequest*>();
    {
        Mutex::Autolock lock(mQueueLock);
        // Do we have any work to do?  Construct the work queue
        // then release the synchronization lock to actually run
        // the backup.
        if (mPendingBackups.GetSize() > 0) {
            HashMap<AutoPtr<IApplicationInfo>, BackupRequest* >::Iterator b = mPendingBackups.Begin();
            for (; b != mPendingBackups.End(); ++b) {
                queue->PushBack(b->mSecond);
            }

            if (DEBUG) Slogger::V(TAG, StringBuffer("clearing pending backups"));
            mPendingBackups.Clear();

            // Start a new backup-queue journal file too
            mJournal = NULL;
        }
    }

    if (queue->GetSize() > 0) {
        // At this point, we have started a new journal file, and the old
        // file identity is being passed to the backup processing thread.
        // When it completes successfully, that old journal file will be
        // deleted.  If we crash prior to that, the old journal is parsed
        // at next boot and the journaled requests fulfilled.
        PerformBackupTask t(transport, queue, mJournal, this);
        t.Run();
    } else {
        Slogger::V(TAG, StringBuffer("Backup requested but nothing pending"));

        //TODO
        //mWakelock.release();
    }

    delete queue;
    queue = NULL;
}

void CBackupManagerService::HandleRunInitilize()
{
    HashSet<String>* queue;

    // Snapshot the pending-init queue and work on that
    {
        Mutex::Autolock lock(mQueueLock);
        queue = new HashSet<String>(mPendingInits);
        mPendingInits.Clear();
    }

    PerformInitializeTask t(queue ,this);

    t.Run();
    if (queue != NULL) {
        delete queue;
        queue = NULL;
    }
}

void CBackupManagerService::HandleMSGTimeout(
    /* [in] */ Int32 token)
{
    {
        Mutex::Autolock lock(mCurrentOpLock);
        Int32 state = mCurrentOperations->Get(token, OP_TIMEOUT);
        if (state == OP_PENDING) {
            if (DEBUG) Slogger::V(TAG, StringBuffer("TIMEOUT: token=") + token);
            mCurrentOperations->Put(token, OP_TIMEOUT);
        }

        //TODO
        //mCurrentOpLock.notifyAll();
    }
}

void CBackupManagerService::HandleGetRestoreSets(
    /* [in] */ RestoreGetSetsParams* params)
{
    assert(params != NULL);
    // Like other async operations, this is entered with the wakelock held
    ArrayOf<IRestoreSet*>* sets = NULL;
    //try {
    params->mTransport->GetAvailableRestoreSets(&sets);
    // cache the result in the active session
    {
        Mutex::Autolock lock(params->mSessionLock);
        params->mSession->mRestoreSets = sets;
    }

    //if (sets == NULL) EventLog.writeEvent(EventLogTags.RESTORE_TRANSPORT_FAILURE);
    // } catch (Exception e) {
    //     Slogger::E(TAG, "Error from transport getting set list");
    // } finally {
    if (params->mObserver != NULL) {
        //try {
        params->mObserver->RestoreSetsAvailable(sets);
        // } catch (RemoteException re) {
        //     Slog.e(TAG, "Unable to report listing to observer");
        // } catch (Exception e) {
        //     Slog.e(TAG, "Restore observer threw", e);
        // }
    }

    //TODO
    //mWakelock.release();
    //}
}

void CBackupManagerService::HandleRestore(
    /* [in] */ RestoreParams* params)
{
    assert(params != NULL);
    Slogger::D(TAG, StringBuffer("MSG_RUN_RESTORE observer=") + params->mObserver);

    PerformRestoreTask p(params->mTransport, params->mObserver,
        params->mToken, params->mCapInfo, params->mPmToken,
        params->mNeedFullBackup, this);

    p.Run();
}

void CBackupManagerService::HandleClear(
    /* [in] */ ClearParams* params)
{
    assert(params != NULL);
    PerformClearTask p(params->mTransport, params->mCapInfo, this);
    p.Run();

    delete params;
}

ArrayOf<Byte>* CBackupManagerService::StringToByteArray(
    /* [in] */ const String& str)
{
    Int32 len = str.GetLength();
    ArrayOf<Byte>* ret = ArrayOf<Byte>::Alloc(len);
    for (Int32 i = 0; i < len; i++) {
        (*ret)[i] = str[i];
    }
    return ret;
}
