
#include "ext/frameworkext.h"
#include "server/CActivityManagerService.h"
#include "server/ActivityState.h"
#include "server/SystemServer.h"
#include "server/CServiceRestarter.h"
#include "server/AttributeCache.h"
#include "content/ContentResolver.h"
#include "content/Context.h"
#include "os/SystemClock.h"
#include "os/Process.h"
#include "os/SystemProperties.h"
#include "os/Binder.h"
#include "utils/EventLogTags.h"
#include "utils/AutoStringArray.h"
#include "view/WindowManagerPolicy.h"
#include <elastos/AutoPtr.h>
#include <elastos/Algorithm.h>
#include <unistd.h>
#include <Slogger.h>
#include <StringBuffer.h>
#include <cutils/properties.h>

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

static ECode GetInt32Prop(
    /* [in] */ CString name,
    /* [in] */ Boolean allowZero,
    /* [out] */ Int32* value)
{
    String str = SystemProperties::Get(name);
    if (str.IsNull()) {
//        throw new IllegalArgumentException("Property not defined: " + name);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 val = str.ToInt32();
    if (val == 0 && !allowZero) {
//        throw new IllegalArgumentException("Property must not be zero: " + name);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *value = val;
    return NOERROR;
}

#define UNUSED(x) (void)x

const char* CActivityManagerService::TAG = "CActivityManagerService";
const Boolean CActivityManagerService::DEBUG;
const Boolean CActivityManagerService::localLOGV ;
const Boolean CActivityManagerService::DEBUG_SWITCH;
const Boolean CActivityManagerService::DEBUG_TASKS;
const Boolean CActivityManagerService::DEBUG_PAUSE;
const Boolean CActivityManagerService::DEBUG_OOM_ADJ;
const Boolean CActivityManagerService::DEBUG_TRANSITION;
const Boolean CActivityManagerService::DEBUG_BROADCAST;
const Boolean CActivityManagerService::DEBUG_BROADCAST_LIGHT;
const Boolean CActivityManagerService::DEBUG_SERVICE;
const Boolean CActivityManagerService::DEBUG_SERVICE_EXECUTING;
const Boolean CActivityManagerService::DEBUG_VISBILITY;
const Boolean CActivityManagerService::DEBUG_PROCESSES;
const Boolean CActivityManagerService::DEBUG_PROVIDER;
const Boolean CActivityManagerService::DEBUG_URI_PERMISSION;
const Boolean CActivityManagerService::DEBUG_USER_LEAVING;
const Boolean CActivityManagerService::DEBUG_RESULTS;
const Boolean CActivityManagerService::DEBUG_BACKUP;
const Boolean CActivityManagerService::DEBUG_CONFIGURATION;
const Boolean CActivityManagerService::VALIDATE_TOKENS;
const Boolean CActivityManagerService::SHOW_ACTIVITY_START_TIME;
const Int32 CActivityManagerService::STOCK_PM_FLAGS;
const Boolean CActivityManagerService::ENFORCE_PROCESS_LIMIT;
const Int32 CActivityManagerService::MAX_ACTIVITIES;
const Int32 CActivityManagerService::MAX_RECENT_TASKS;
const Int32 CActivityManagerService::PROC_START_TIMEOUT;
const Int32 CActivityManagerService::GC_MIN_INTERVAL;
const Int32 CActivityManagerService::BROADCAST_TIMEOUT;
const Int32 CActivityManagerService::SERVICE_TIMEOUT;
const Int32 CActivityManagerService::SERVICE_RESTART_DURATION;
const Int32 CActivityManagerService::SERVICE_RESET_RUN_DURATION;
const Int32 CActivityManagerService::SERVICE_RESTART_DURATION_FACTOR;
const Int32 CActivityManagerService::SERVICE_MIN_RESTART_TIME_BETWEEN;
const Int32 CActivityManagerService::MAX_SERVICE_INACTIVITY;
const Int32 CActivityManagerService::KEY_DISPATCHING_TIMEOUT;
const Int32 CActivityManagerService::INSTRUMENTATION_KEY_DISPATCHING_TIMEOUT;

Int32 CActivityManagerService::EMPTY_APP_ADJ;
Int32 CActivityManagerService::HIDDEN_APP_MAX_ADJ;
Int32 CActivityManagerService::HIDDEN_APP_MIN_ADJ;
Int32 CActivityManagerService::HOME_APP_ADJ;
Int32 CActivityManagerService::BACKUP_APP_ADJ;
Int32 CActivityManagerService::SECONDARY_SERVER_ADJ;
Int32 CActivityManagerService::HEAVY_WEIGHT_APP_ADJ;
Int32 CActivityManagerService::PERCEPTIBLE_APP_ADJ;
Int32 CActivityManagerService::VISIBLE_APP_ADJ;
Int32 CActivityManagerService::FOREGROUND_APP_ADJ;

const Int32 CActivityManagerService::CORE_SERVER_ADJ;
const Int32 CActivityManagerService::SYSTEM_ADJ;
const Int32 CActivityManagerService::PAGE_SIZE;

// Corresponding memory levels for above adjustments.
Int32 CActivityManagerService::EMPTY_APP_MEM;
Int32 CActivityManagerService::HIDDEN_APP_MEM;
Int32 CActivityManagerService::HOME_APP_MEM;
Int32 CActivityManagerService::BACKUP_APP_MEM;
Int32 CActivityManagerService::SECONDARY_SERVER_MEM;
Int32 CActivityManagerService::HEAVY_WEIGHT_APP_MEM;
Int32 CActivityManagerService::PERCEPTIBLE_APP_MEM;
Int32 CActivityManagerService::VISIBLE_APP_MEM;
Int32 CActivityManagerService::FOREGROUND_APP_MEM;
const Int32 CActivityManagerService::MIN_HIDDEN_APPS;
const Int32 CActivityManagerService::MAX_HIDDEN_APPS;
const Int32 CActivityManagerService::CONTENT_APP_IDLE_OFFSET;
const Int32 CActivityManagerService::EMPTY_APP_IDLE_OFFSET;
const Int32 CActivityManagerService::SHOW_ERROR_MSG;
const Int32 CActivityManagerService::SHOW_NOT_RESPONDING_MSG;
const Int32 CActivityManagerService::SHOW_FACTORY_ERROR_MSG;
const Int32 CActivityManagerService::UPDATE_CONFIGURATION_MSG;
const Int32 CActivityManagerService::GC_BACKGROUND_PROCESSES_MSG;
const Int32 CActivityManagerService::WAIT_FOR_DEBUGGER_MSG;
const Int32 CActivityManagerService::BROADCAST_INTENT_MSG;
const Int32 CActivityManagerService::BROADCAST_TIMEOUT_MSG;
const Int32 CActivityManagerService::SERVICE_TIMEOUT_MSG;
const Int32 CActivityManagerService::UPDATE_TIME_ZONE;
const Int32 CActivityManagerService::SHOW_UID_ERROR_MSG;
const Int32 CActivityManagerService::IM_FEELING_LUCKY_MSG;
const Int32 CActivityManagerService::PROC_START_TIMEOUT_MSG;
const Int32 CActivityManagerService::DO_PENDING_ACTIVITY_LAUNCHES_MSG;
const Int32 CActivityManagerService::KILL_APPLICATION_MSG;
const Int32 CActivityManagerService::FINALIZE_PENDING_INTENT_MSG;
const Int32 CActivityManagerService::POST_HEAVY_NOTIFICATION_MSG;
const Int32 CActivityManagerService::CANCEL_HEAVY_NOTIFICATION_MSG;
const Int32 CActivityManagerService::SHOW_STRICT_MODE_VIOLATION_MSG;
const Int32 CActivityManagerService::CHECK_EXCESSIVE_WAKE_LOCKS_MSG;
const Int32 CActivityManagerService::BROADCAST_SUCCESS;
const Int32 CActivityManagerService::BROADCAST_STICKY_CANT_HAVE_PERMISSION;
const Int32 CActivityManagerService::MAX_BROADCAST_HISTORY;
AutoPtr<IContext> CActivityManagerService::sSystemContext;

#define FAIL_RETURN_FALSE(expr) \
    do { \
        ECode ec = expr; \
        if (FAILED(ec)) return FALSE; \
    } while(0);

static Boolean Init()
{
    Int32 value;
    // These values are set in system/rootdir/init.rc on startup.
    ASSERT_SUCCEEDED(GetInt32Prop("ro.FOREGROUND_APP_ADJ", TRUE, &value));
    CActivityManagerService::FOREGROUND_APP_ADJ = value;
    ASSERT_SUCCEEDED(GetInt32Prop("ro.VISIBLE_APP_ADJ", TRUE, &value));
    CActivityManagerService::VISIBLE_APP_ADJ = value;
    ASSERT_SUCCEEDED(GetInt32Prop("ro.PERCEPTIBLE_APP_ADJ", TRUE, &value));
    CActivityManagerService::PERCEPTIBLE_APP_ADJ = value;
    ASSERT_SUCCEEDED(GetInt32Prop("ro.HEAVY_WEIGHT_APP_ADJ", TRUE, &value));
    CActivityManagerService::HEAVY_WEIGHT_APP_ADJ = value;
    ASSERT_SUCCEEDED(GetInt32Prop("ro.SECONDARY_SERVER_ADJ", TRUE, &value));
    CActivityManagerService::SECONDARY_SERVER_ADJ = value;
    ASSERT_SUCCEEDED(GetInt32Prop("ro.BACKUP_APP_ADJ", TRUE, &value));
    CActivityManagerService::BACKUP_APP_ADJ = value;
    ASSERT_SUCCEEDED(GetInt32Prop("ro.HOME_APP_ADJ", TRUE, &value));
    CActivityManagerService::HOME_APP_ADJ = value;
    ASSERT_SUCCEEDED(GetInt32Prop("ro.HIDDEN_APP_MIN_ADJ", TRUE, &value));
    CActivityManagerService::HIDDEN_APP_MIN_ADJ = value;
    ASSERT_SUCCEEDED(GetInt32Prop("ro.EMPTY_APP_ADJ", TRUE, &value));
    CActivityManagerService::EMPTY_APP_ADJ = value;
    // These days we use the last empty slot for hidden apps as well.
    CActivityManagerService::HIDDEN_APP_MAX_ADJ = CActivityManagerService::EMPTY_APP_ADJ;
    ASSERT_SUCCEEDED(GetInt32Prop("ro.FOREGROUND_APP_MEM", FALSE, &value));
    CActivityManagerService::FOREGROUND_APP_MEM = value * CActivityManagerService::PAGE_SIZE;
    ASSERT_SUCCEEDED(GetInt32Prop("ro.VISIBLE_APP_MEM", FALSE, &value));
    CActivityManagerService::VISIBLE_APP_MEM = value * CActivityManagerService::PAGE_SIZE;
    ASSERT_SUCCEEDED(GetInt32Prop("ro.PERCEPTIBLE_APP_MEM", FALSE, &value));
    CActivityManagerService::PERCEPTIBLE_APP_MEM = value * CActivityManagerService::PAGE_SIZE;
    ASSERT_SUCCEEDED(GetInt32Prop("ro.HEAVY_WEIGHT_APP_MEM", FALSE, &value));
    CActivityManagerService::HEAVY_WEIGHT_APP_MEM = value * CActivityManagerService::PAGE_SIZE;
    ASSERT_SUCCEEDED(GetInt32Prop("ro.SECONDARY_SERVER_MEM", FALSE, &value));
    CActivityManagerService::SECONDARY_SERVER_MEM = value * CActivityManagerService::PAGE_SIZE;
    ASSERT_SUCCEEDED(GetInt32Prop("ro.BACKUP_APP_MEM", FALSE, &value));
    CActivityManagerService::BACKUP_APP_MEM = value * CActivityManagerService::PAGE_SIZE;
    ASSERT_SUCCEEDED(GetInt32Prop("ro.HOME_APP_MEM", FALSE, &value));
    CActivityManagerService::HOME_APP_MEM = value * CActivityManagerService::PAGE_SIZE;
    ASSERT_SUCCEEDED(GetInt32Prop("ro.HIDDEN_APP_MEM", FALSE, &value));
    CActivityManagerService::HIDDEN_APP_MEM = value * CActivityManagerService::PAGE_SIZE;
    ASSERT_SUCCEEDED(GetInt32Prop("ro.EMPTY_APP_MEM", FALSE, &value));
    CActivityManagerService::EMPTY_APP_MEM = value * CActivityManagerService::PAGE_SIZE;
    return TRUE;
}

Boolean CActivityManagerService::INIT_SUCCEEDED = Init();


ECode CActivityManagerService::ServiceRestarter::Run()
{
    Mutex::Autolock lock(mActivityManager->_m_syncLock);
    mActivityManager->PerformServiceRestartLocked(mService);
    return NOERROR;
}

CActivityManagerService::CActivityManagerService() :
    mBroadcastsScheduled(FALSE),
    mHeavyWeightProcess(NULL),
    mPidsSelfLocked(11),
    mHomeProcess(NULL),
    mIntentSenderRecords(11),
    mPendingBroadcast(NULL),
    mRegisteredReceivers(11),
    mStickyBroadcasts(11),
    mServices(11),
    mServicesByIntent(11),
    mBackupTarget(NULL),
    mProvidersByName(11),
    mProvidersByClass(11),
    mConfigurationSeq(0),
    mAppBindArgs(NULL),
    mProcessesReady(TRUE),
    mSystemReady(FALSE),
    mWaitingUpdate(FALSE),
    mDidUpdate(FALSE),
    mSleeping(FALSE),
    mShuttingDown(FALSE),
    mCurTask(1),
    mAdjSeq(0),
    mLRUSeq(0),
    mWaitForDebugger(FALSE),
    mDebugTransient(FALSE),
    mOrigWaitForDebugger(FALSE),
    mProcessLimit(0),
    mBooted(FALSE),
    mBooting(FALSE),
    mLaunchWarningShown(FALSE),
    mStringBuffer(256)
{
    CRemoteCallbackList::New((IRemoteCallbackList**)&mWatchers);
    AutoPtr<IApartmentHelper> helper;
    assert(SUCCEEDED(CApartmentHelper::AcquireSingleton((IApartmentHelper**)&helper)));
    assert(SUCCEEDED(helper->GetMainApartment((IApartment**)&mApartment))
            && (mApartment != NULL));

    AutoPtr<IServiceManager> serviceManager;

    mMyPid = getpid();
    mMainStack = new ActivityStack(this, NULL /*context*/, TRUE);
    memset(mBroadcastHistory, 0, MAX_BROADCAST_HISTORY*sizeof(BroadcastRecord*));
    mProcessNames = new ProcessMap<ProcessRecord*>();
    mProcessCrashTimes = new ProcessMap<Millisecond64>();
    mBadProcesses = new ProcessMap<Millisecond64>();
    mReceiverResolver = new IntentResolver<BroadcastFilter,
            BroadcastFilter>();
    CBatteryStatsService::NewByFriend(String("/system/batterystats.bin"),
            (CBatteryStatsService**)&mBatteryStatsService);
    CConfiguration::New((IConfiguration**)&mConfiguration);
    mProcDeaths = new Int32[20];
    memset(mProcDeaths, 0, 20);
}

CActivityManagerService::~CActivityManagerService()
{
    mPidsSelfLocked.Clear();
    delete mProcessNames;
    delete mProcessCrashTimes;
    delete mBadProcesses;
    mStartingProcesses.Clear();
    mStickyBroadcasts.Clear();
    mServices.Clear();
    mServicesByIntent.Clear();
    mProvidersByName.Clear();
    mProvidersByClass.Clear();
    mLaunchingProviders.Clear();
    mGrantedUriPermissions.Clear();
    delete mProcDeaths;
    delete mMainStack;
    if (mAppBindArgs != NULL) {
        mAppBindArgs->Clear();
        delete mAppBindArgs;
    }
}

ECode CActivityManagerService::constructor()
{
    return NOERROR;
}

ECode CActivityManagerService::SetWindowManager(
    /* [in] */ IWindowManager* wm)
{
    assert(wm != NULL);
    mWindowManager = (CWindowManagerService*)wm;
    return NOERROR;
}

ECode CActivityManagerService::GetSystemContext(
    /* [out] */ IContext** ctx)
{
    assert(ctx != NULL);

    if (sSystemContext == NULL) {
        CContextImpl::New(TRUE, (IContext**)&sSystemContext);
    }
    *ctx = sSystemContext;
    (*ctx)->AddRef();

    return NOERROR;
}

PermissionController::PermissionController(
    /* [in] */ CActivityManagerService* activityManagerService)
{
    mActivityManagerService = activityManagerService;
}

Boolean PermissionController::CheckPermission(
    /* [in] */ CString permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid)
{
    Int32 result;
    mActivityManagerService->CheckPermission(permission, pid,
            uid, &result);
    return result == CapsuleManager_PERMISSION_GRANTED;
}

ECode CActivityManagerService::SetSystemProcess()
{
    // try {
    //     ActivityManagerService m = mSelf;

    //     ServiceManager.addService("activity", m);
    //     ServiceManager.addService("meminfo", new MemBinder(m));
    //     if (MONITOR_CPU_USAGE) {
    //         ServiceManager.addService("cpuinfo", new CpuBinder(m));
    //     }
    AutoPtr<IServiceManager> serviceManager;
    Elastos::GetServiceManager((IServiceManager**)&serviceManager);
    AutoPtr<IPermissionController> permissionController;
    CPermissionController::New((IPermissionController**)&permissionController);
    serviceManager->AddService(String("permission"), permissionController.Get());

    //     ApplicationInfo info =
    //         mSelf.mContext.getPackageManager().getApplicationInfo(
    //                 "android", STOCK_PM_FLAGS);
    //     mSystemThread.installSystemApplicationInfo(info);

    //     synchronized (mSelf) {
    //         ProcessRecord app = mSelf.newProcessRecordLocked(
    //                 mSystemThread.getApplicationThread(), info,
    //                 info.processName);
    //         app.persistent = true;
    //         app.pid = MY_PID;
    //         app.maxAdj = SYSTEM_ADJ;
    //         mSelf.mProcessNames.put(app.processName, app.info.uid, app);
    //         synchronized (mSelf.mPidsSelfLocked) {
    //             mSelf.mPidsSelfLocked.put(app.pid, app);
    //         }
    //         mSelf.updateLruProcessLocked(app, true, true);
    //     }
    // } catch (PackageManager.NameNotFoundException e) {
    //     throw new RuntimeException(
    //             "Unable to find android system package", e);
    // }
    assert(sSystemContext != NULL);
    AutoPtr<IApplicationInfo> info;
    ASSERT_SUCCEEDED(sSystemContext->GetApplicationInfo((IApplicationInfo**)&info));
    String processName;
    info->GetProcessName(&processName);

    // ProcessRecord* app = mSelf.NewProcessRecordLocked(
    //         mSystemThread.getApplicationThread(), info,
    //         info.processName);
    AutoPtr<IApplicationApartment> apartment;
    ASSERT_SUCCEEDED(IContextImpl::Probe(sSystemContext)->GetApplicationApartment((IApplicationApartment**)&apartment));
    ProcessRecord* app = NewProcessRecordLocked(apartment, info, processName);
    assert(app != NULL);
    app->mPersistent = TRUE;
    app->mPid = mMyPid;
    app->mMaxAdj = SYSTEM_ADJ;

    Int32 appUid = 0;
    app->mInfo->GetUid(&appUid);
    mProcessNames->Put(app->mProcessName, appUid, app);
    mPidsSelfLocked[app->mPid] = app;

    UpdateLRUProcessLocked(app, TRUE, TRUE);
    return NOERROR;
}

ICapsuleManager* CActivityManagerService::GetCapsuleManager()
{
    AutoPtr<IServiceManager> serviceManager;
    AutoPtr<ICapsuleManager> capsuleManager;
    Elastos::GetServiceManager((IServiceManager**)&serviceManager);
    assert(serviceManager != NULL);
	serviceManager->GetService(String("capsule"),
	        (IInterface**)(ICapsuleManager**)&capsuleManager);
    assert(capsuleManager != NULL);
	return capsuleManager;
}

void CActivityManagerService::UpdateCpuStats()
{
//    final long now = SystemClock.uptimeMillis();
//    if (mLastCpuTime.get() >= now - MONITOR_CPU_MIN_TIME) {
//        return;
//    }
//    if (mProcessStatsMutexFree.compareAndSet(true, false)) {
//        synchronized (mProcessStatsThread) {
//            mProcessStatsThread.notify();
//        }
//    }
}

void CActivityManagerService::UpdateCpuStatsNow()
{
//    synchronized (mProcessStatsThread) {
//        mProcessStatsMutexFree.set(false);
//        final long now = SystemClock.uptimeMillis();
//        boolean haveNewCpuStats = false;
//
//        if (MONITOR_CPU_USAGE &&
//                mLastCpuTime.get() < (now-MONITOR_CPU_MIN_TIME)) {
//            mLastCpuTime.set(now);
//            haveNewCpuStats = true;
//            mProcessStats.update();
//            //Slog.i(TAG, mProcessStats.printCurrentState());
//            //Slog.i(TAG, "Total CPU usage: "
//            //        + mProcessStats.getTotalCpuPercent() + "%");
//
//            // Slog the cpu usage if the property is set.
//            if ("true".equals(SystemProperties.get("events.cpu"))) {
//                int user = mProcessStats.getLastUserTime();
//                int system = mProcessStats.getLastSystemTime();
//                int iowait = mProcessStats.getLastIoWaitTime();
//                int irq = mProcessStats.getLastIrqTime();
//                int softIrq = mProcessStats.getLastSoftIrqTime();
//                int idle = mProcessStats.getLastIdleTime();
//
//                int total = user + system + iowait + irq + softIrq + idle;
//                if (total == 0) total = 1;
//
//                EventLog.writeEvent(EventLogTags.CPU,
//                        ((user+system+iowait+irq+softIrq) * 100) / total,
//                        (user * 100) / total,
//                        (system * 100) / total,
//                        (iowait * 100) / total,
//                        (irq * 100) / total,
//                        (softIrq * 100) / total);
//            }
//        }
//
//        long[] cpuSpeedTimes = mProcessStats.getLastCpuSpeedTimes();
//        final BatteryStatsImpl bstats = mBatteryStatsService.getActiveStatistics();
//        synchronized(bstats) {
//            synchronized(mPidsSelfLocked) {
//                if (haveNewCpuStats) {
//                    if (mOnBattery) {
//                        int perc = bstats.startAddingCpuLocked();
//                        int totalUTime = 0;
//                        int totalSTime = 0;
//                        final int N = mProcessStats.countStats();
//                        for (int i=0; i<N; i++) {
//                            ProcessStats.Stats st = mProcessStats.getStats(i);
//                            if (!st.working) {
//                                continue;
//                            }
//                            ProcessRecord pr = mPidsSelfLocked.get(st.pid);
//                            int otherUTime = (st.rel_utime*perc)/100;
//                            int otherSTime = (st.rel_stime*perc)/100;
//                            totalUTime += otherUTime;
//                            totalSTime += otherSTime;
//                            if (pr != null) {
//                                BatteryStatsImpl.Uid.Proc ps = pr.batteryStats;
//                                ps.addCpuTimeLocked(st.rel_utime-otherUTime,
//                                        st.rel_stime-otherSTime);
//                                ps.addSpeedStepTimes(cpuSpeedTimes);
//                                pr.curCpuTime += (st.rel_utime+st.rel_stime) * 10;
//                            } else {
//                                BatteryStatsImpl.Uid.Proc ps =
//                                        bstats.getProcessStatsLocked(st.name, st.pid);
//                                if (ps != null) {
//                                    ps.addCpuTimeLocked(st.rel_utime-otherUTime,
//                                            st.rel_stime-otherSTime);
//                                    ps.addSpeedStepTimes(cpuSpeedTimes);
//                                }
//                            }
//                        }
//                        bstats.finishAddingCpuLocked(perc, totalUTime,
//                                totalSTime, cpuSpeedTimes);
//                    }
//                }
//            }
//
//            if (mLastWriteTime < (now-BATTERY_STATS_TIME)) {
//                mLastWriteTime = now;
//                mBatteryStatsService.getActiveStatistics().writeAsyncLocked();
//            }
//        }
//    }
}

void CActivityManagerService::BatteryNeedsCpuUpdate()
{
    //updateCpuStatsNow();
}

void CActivityManagerService::BatteryPowerChanged(
    /* [in] */ Boolean onBattery)
{
//    // When plugging in, update the CPU stats first before changing
//    // the plug state.
//    updateCpuStatsNow();
//    synchronized (this) {
//        synchronized(mPidsSelfLocked) {
//            mOnBattery = DEBUG_POWER ? true : onBattery;
//        }
//    }
}

/**
 * Initialize the application bind args. These are passed to each
 * process when the bindApplication() IPC is sent to the process. They're
 * lazily setup to make sure the services are running when they're asked for.
 */
ECode CActivityManagerService::GetCommonServicesLocked(
    /* [out] */ IObjectStringMap** services)
{
    assert(services != NULL);

    if (mAppBindArgs == NULL) {
        mAppBindArgs = new HashMap<String, AutoPtr<IInterface> >(5);

        // Setup the application init args
        AutoPtr<IServiceManager> serviceManager;
        AutoPtr<IInterface> capsuleManager, windowManager, alarmManager;
        Elastos::GetServiceManager((IServiceManager**)&serviceManager);
        serviceManager->GetService(String("capsule"), (IInterface**)&capsuleManager);
        serviceManager->GetService(String("window"), (IInterface**)&windowManager);
        serviceManager->GetService(String(Context_ALARM_SERVICE), (IInterface**)&serviceManager);
        (*mAppBindArgs)[String("capsule")] = capsuleManager;
        (*mAppBindArgs)[String("window")] = windowManager;
        (*mAppBindArgs)[String(Context_ALARM_SERVICE)] = serviceManager;
    }

    CParcelableObjectStringMap::New(services);
    (*services)->Put(String("capsule"), (*mAppBindArgs)[String("capsule")]);
    (*services)->Put(String("window"), (*mAppBindArgs)[String("window")]);
    (*services)->Put(String(Context_ALARM_SERVICE), (*mAppBindArgs)[String(Context_ALARM_SERVICE)]);

    return NOERROR;
}

void CActivityManagerService::SetFocusedActivityLocked(
    /* [in] */ CActivityRecord* r)
{
    if ((CActivityRecord*)mFocusedActivity != r) {
        mFocusedActivity = r;
        mWindowManager->SetFocusedApp(r, TRUE);
    }
}

void CActivityManagerService::UpdateLRUProcessInternalLocked(
    /* [in] */ ProcessRecord* app,
    /* [in] */ Boolean oomAdj,
    /* [in] */ Boolean updateActivityTime,
    /* [in] */ Int32 bestPos)
{
    // put it on the LRU to keep track of when it should be exited.
    List<ProcessRecord*>::Iterator it = Find(
            mLRUProcesses.Begin(), mLRUProcesses.End(), app);
    if (it != mLRUProcesses.End()) mLRUProcesses.Erase(it);

    Int32 i = mLRUProcesses.GetSize() - 1;
    it = --mLRUProcesses.End();
    Int32 skipTop = 0;

    app->mLRUSeq = mLRUSeq;

    // compute the new weight for this process.
    if (updateActivityTime) {
        app->mLastActivityTime = SystemClock::GetUptimeMillis();
    }
    if (app->mActivities.GetSize() > 0) {
        // If this process has activities, we more strongly want to keep
        // it around.
        app->mLRUWeight = app->mLastActivityTime;
    } else if (app->mPubProviders.GetSize() > 0) {
        // If this process contains content providers, we want to keep
        // it a little more strongly.
        app->mLRUWeight = app->mLastActivityTime - CONTENT_APP_IDLE_OFFSET;
        // Also don't let it kick out the first few "real" hidden processes.
        skipTop = MIN_HIDDEN_APPS;
    } else {
        // If this process doesn't have activities, we less strongly
        // want to keep it around, and generally want to avoid getting
        // in front of any very recently used activities.
        app->mLRUWeight = app->mLastActivityTime - EMPTY_APP_IDLE_OFFSET;
        // Also don't let it kick out the first few "real" hidden processes.
        skipTop = MIN_HIDDEN_APPS;
    }

    while (i >= 0) {
        ProcessRecord* p = *it;
        // If this app shouldn't be in front of the first N background
        // apps, then skip over that many that are currently hidden.
        if (skipTop > 0 && p->mSetAdj >= HIDDEN_APP_MIN_ADJ) {
            skipTop--;
        }
        if (p->mLRUWeight <= app->mLRUWeight || i < bestPos) {
            mLRUProcesses.Insert(++it, app);
            break;
        }
        i--;
        it--;
    }
    if (i < 0) {
        mLRUProcesses.PushFront(app);
    }

    // If the app is currently using a content provider or service,
    // bump those processes as well.
    if (app->mConnections.GetSize() > 0) {
        Set<ConnectionRecord*>::Iterator it1;
        for(it1 = app->mConnections.Begin(); it1 != app->mConnections.End(); ++it1) {
            ConnectionRecord* cr = *it1;
            if (cr->mBinding != NULL && cr->mBinding->mService != NULL
                    && cr->mBinding->mService->mApp != NULL
                    && cr->mBinding->mService->mApp->mLRUSeq != mLRUSeq) {
                UpdateLRUProcessInternalLocked(cr->mBinding->mService->mApp, oomAdj,
                        updateActivityTime, i + 1);
            }
        }
    }
    if (app->mConProviders.GetSize() > 0) {
        HashMap<ContentProviderRecord*, Int32,
                Hash<ContentProviderRecord*> >::Iterator it2 = app->mConProviders.Begin();
        for (; it2 != app->mConProviders.End(); ++it2) {
            ContentProviderRecord* cpr = it2->mFirst;
            if (cpr->mApp != NULL && cpr->mApp->mLRUSeq != mLRUSeq) {
                UpdateLRUProcessInternalLocked(cpr->mApp, oomAdj,
                        updateActivityTime, i + 1);
            }
        }
    }

    //Slogger::I(TAG, StringBuffer("Putting proc to front: ") + app->mProcessName);
    if (oomAdj) {
        UpdateOomAdjLocked();
    }
}

void CActivityManagerService::UpdateLRUProcessLocked(
    /* [in] */ ProcessRecord* app,
    /* [in] */ Boolean oomAdj,
    /* [in] */ Boolean updateActivityTime)
{
    mLRUSeq++;
    UpdateLRUProcessInternalLocked(app, oomAdj, updateActivityTime, 0);
}

ProcessRecord* CActivityManagerService::GetProcessRecordLocked(
    /* [in] */ const String& processName,
    /* [in] */ Int32 uid)
{
    if (uid == Process::SYSTEM_UID) {
        // The system gets to run in any process.  If there are multiple
        // processes with the same uid, just pick the first (this
        // should never happen).
        HashMap<String, Map<Int32, ProcessRecord*>*>::Iterator it = \
                mProcessNames->GetMap()->Find(processName);
        if (it != mProcessNames->GetMap()->End()) {
            return it->mSecond->Begin()->mSecond;
        }
        else {
            return NULL;
        }
    }
    ProcessRecord* proc = mProcessNames->Get(processName, uid);
    return proc;
}

void CActivityManagerService::EnsurePackageDexOpt(
    /* [in] */ const String& packageName)
{
    AutoPtr<ICapsuleManager> pm = mMainStack->GetCapsuleManager();  // = AppGlobals.getPackageManager();
//    try {
        Boolean result;
        pm->PerformDexOpt(packageName, &result);
        if (result) {
            mDidDexOpt = TRUE;
        }
//    } catch (RemoteException e) {
//    }
}

Boolean CActivityManagerService::IsNextTransitionForward()
{
    Int32 transit;
    mWindowManager->GetPendingAppTransition(&transit);
    return transit == WindowManagerPolicy::TRANSIT_ACTIVITY_OPEN
            || transit == WindowManagerPolicy::TRANSIT_TASK_OPEN
            || transit == WindowManagerPolicy::TRANSIT_TASK_TO_FRONT;
}

ProcessRecord* CActivityManagerService::StartProcessLocked(
    /* [in] */ const String& processName,
    /* [in] */ IApplicationInfo* info,
    /* [in] */ Boolean knownToBeDead,
    /* [in] */ Int32 intentFlags,
    /* [in] */ const char* hostingType,
    /* [in] */ IComponentName* hostingName,
    /* [in] */ Boolean allowWhileBooting)
{
    Int32 infoUid;
    info->GetUid(&infoUid);
    ProcessRecord* app = GetProcessRecordLocked(processName, infoUid);
    // We don't have to do anything more if:
    // (1) There is an existing application record; and
    // (2) The caller doesn't think it is dead, OR there is no thread
    //     object attached to it so we know it couldn't have crashed; and
    // (3) There is a pid assigned to it, so it is either starting or
    //     already running.
    if (DEBUG_PROCESSES) {
        String appDes;
        app->GetDescription(&appDes);
        StringBuffer sb("startProcess: name=");
        sb = sb + processName + " app=" + appDes;
        sb = sb + " knownToBeDead=" + knownToBeDead;
        if (app != NULL) {
            String appApDes;
            app->mAppApartment->GetDescription(&appApDes);
            sb += " AppApartment=" + appApDes;
        }
        else {
            sb += " AppApartment=NULL";
        }
        sb += " pid=" + (app != NULL ? app->mPid : -1);
        Slogger::V(TAG, sb);
    }
    if (app != NULL && app->mPid > 0) {
        if (!knownToBeDead || app->mAppApartment == NULL) {
            // We already have the app running, or are waiting for it to
            // come up (we have a pid but not yet its thread), so keep it.
            if (DEBUG_PROCESSES) {
                String appDes;
                app->GetDescription(&appDes);
                Slogger::V(TAG, StringBuffer("App already running: ") + appDes);
            }
            return app;
        }
        else {
            // An application record is attached to a previous process,
            // clean it up now.
            if (DEBUG_PROCESSES) {
                String appDes;
                app->GetDescription(&appDes);
                Slogger::V(TAG, StringBuffer("App died: ") + appDes);
            }
            HandleAppDiedLocked(app, TRUE);
        }
    }

    String hostingNameStr;
    if (hostingName != NULL) {
        hostingName->FlattenToShortString(&hostingNameStr);
    }

    String infoPName;
    info->GetProcessName(&infoPName);
    if ((intentFlags & Intent_FLAG_FROM_BACKGROUND) != 0) {
        // If we are in the background, then check to see if this process
        // is bad.  If so, we will just silently fail.
        if (mBadProcesses->Get(infoPName, infoUid) != (Millisecond64)NULL) {
            if (DEBUG_PROCESSES) {
                Slogger::V(TAG, StringBuffer("Bad process: ") + infoUid
                        + "/" + infoPName);
            }
            return NULL;
        }
    }
    else {
        // When the user is explicitly starting a process, then clear its
        // crash count so that we won't make it bad until they see at
        // least one crash dialog again, and make the process good again
        // if it had been bad.
        if (DEBUG_PROCESSES) {
            Slogger::V(TAG, StringBuffer("Clearing bad process: ") + infoUid
                    + "/" + infoPName);
        }
        mProcessCrashTimes->Remove(infoPName, infoUid);
        if (mBadProcesses->Get(infoPName, infoUid) != (Millisecond64)NULL) {
//            EventLog.writeEvent(EventLogTags.AM_PROC_GOOD, info.uid,
//                    info.processName);
            mBadProcesses->Remove(infoPName, infoUid);
            if (app != NULL) {
                app->mBad = FALSE;
            }
        }
    }

    if (app == NULL) {
        app = NewProcessRecordLocked(NULL, info, processName);
        mProcessNames->Put(processName, infoUid, app);
    }
    else {
        String infoCName;
        info->GetCapsuleName(&infoCName);
        // If this is a new package in the process, add the package to the list
        app->AddCapsule(infoCName);
    }

    // If the system is not ready yet, then hold off on starting this
    // process until it is.
    if (!mProcessesReady
            && !IsAllowedWhileBooting(info)
            && !allowWhileBooting) {
        List<ProcessRecord*>::Iterator it = Find(
                mProcessesOnHold.Begin(), mProcessesOnHold.End(), app);
        if (it == mProcessesOnHold.End()) {
            mProcessesOnHold.PushBack(app);
        }
        if (DEBUG_PROCESSES) {
            String appDes;
            app->GetDescription(&appDes);
            Slogger::V(TAG, StringBuffer("System not ready, putting on hold: ")
                    + appDes);
        }
        return app;
    }

    StartProcessLocked(app, hostingType, hostingNameStr);
    return (app->mPid != 0) ? app : NULL;
}

Boolean CActivityManagerService::IsAllowedWhileBooting(
    /* [in] */ IApplicationInfo* ai)
{
    Int32 flags;
    ai->GetFlags(&flags);
    return (flags & ApplicationInfo_FLAG_PERSISTENT) != 0;
}

void CActivityManagerService::StartProcessLocked(
    /* [in] */ ProcessRecord* app,
    /* [in] */ const char* hostingType,
    /* [in] */ const char* hostingNameStr)
{
    if (app->mPid > 0 && app->mPid != mMyPid) {
        Mutex::Autolock lock(mPidsSelfLock);
        mPidsSelfLocked.Erase(app->mPid);
//        mHandler.removeMessages(PROC_START_TIMEOUT_MSG, app);
        app->mPid = 0;
    }

    if (DEBUG_PROCESSES &&
            (Find(mProcessesOnHold.Begin(), mProcessesOnHold.End(), app)
            != mProcessesOnHold.End())) {
        String appDes;
        app->GetDescription(&appDes);
        Slogger::V(TAG, StringBuffer("startProcessLocked removing on hold: ")
                + appDes);
    }
    mProcessesOnHold.Remove(app);

    UpdateCpuStats();

    for(Int32 i = 19; i > 0; i--) {
        mProcDeaths[i] = mProcDeaths[i - 1];
    }
    mProcDeaths[0] = 0;

//    try {
    Int32 uid;
    app->mInfo->GetUid(&uid);
    UNUSED(uid);
//    int[] gids = null;
//    try {
//        gids = mContext.getPackageManager().getPackageGids(
//                app.info.packageName);
//    } catch (PackageManager.NameNotFoundException e) {
//        Slogger::W(TAG, "Unable to retrieve gids", e);
//    }
//    if (mFactoryTest != SystemServer.FACTORY_TEST_OFF) {
//        if (mFactoryTest == SystemServer.FACTORY_TEST_LOW_LEVEL
//                && mTopComponent != null
//                && app.processName.equals(mTopComponent.getPackageName())) {
//            uid = 0;
//        }
//        if (mFactoryTest == SystemServer.FACTORY_TEST_HIGH_LEVEL
//                && (app.info.flags&ApplicationInfo.FLAG_FACTORY_TEST) != 0) {
//            uid = 0;
//        }
//    }
//    int debugFlags = 0;
//    if ((app.info.flags & ApplicationInfo.FLAG_DEBUGGABLE) != 0) {
//        debugFlags |= Zygote.DEBUG_ENABLE_DEBUGGER;
//    }
//    // Run the app in safe mode if its manifest requests so or the
//    // system is booted in safe mode.
//    if ((app.info.flags & ApplicationInfo.FLAG_VM_SAFE_MODE) != 0 ||
//        Zygote.systemInSafeMode == true) {
//        debugFlags |= Zygote.DEBUG_ENABLE_SAFEMODE;
//    }
//    if ("1".equals(SystemProperties.get("debug.checkjni"))) {
//        debugFlags |= Zygote.DEBUG_ENABLE_CHECKJNI;
//    }
//    if ("1".equals(SystemProperties.get("debug.assert"))) {
//        debugFlags |= Zygote.DEBUG_ENABLE_ASSERT;
//    }
//    int pid = Process.start("android.app.ActivityThread",
//            mSimpleProcessManagement ? app.processName : null, uid, uid,
//            gids, debugFlags, null);

    Int32 pid = 0;
    String sourcePath;
    app->mInfo->GetSourceDir(&sourcePath);
    if (!sourcePath.IsNull() && sourcePath.EndWith(".apk")) {
        if (system("dalvikvm -cp /system/framework/android-bootstrap.jar com.elastos.android.Bootstrap") != 0) {
            Slogger::E(TAG, "Execute dalvikvm fail!\n");
        }
        char strPid[10];
        property_get("elastos.pid", strPid, "0");
        pid = atoi(strPid);
    }
    else {
        pid = fork();
        if(pid == 0){
            char *argv[] = {(char *)"host", (char *)"Elastos.Framework.Core.eco",
                            (char *)"CApplicationApartment", NULL};
            if(execv("/data/data/com.elastos.runtime/elastos/host", argv) < 0){
                Slogger::E(TAG, "Execute host.ecx fail!\n");
            }
            exit(0);
        }
    }

//    AutoPtr<BatteryStatsImpl> bs = app->mBatteryStats->GetBatteryStats();
//    bs->Lock();
//    if (bs->IsOnBattery()) {
//        app->mBatteryStats->IncStartsLocked();
//    }
//    bs->Unlock();

//    EventLog.writeEvent(EventLogTags.AM_PROC_START, pid, uid,
//            app.processName, hostingType,
//            hostingNameStr != null ? hostingNameStr : "");

//    if (app.persistent) {
//        Watchdog.getInstance().processStarted(app.processName, pid);
//    }

//    StringBuilder buf = mStringBuilder;
//    buf.setLength(0);
//    buf.append("Start proc ");
//    buf.append(app.processName);
//    buf.append(" for ");
//    buf.append(hostingType);
//    if (hostingNameStr != null) {
//        buf.append(" ");
//        buf.append(hostingNameStr);
//    }
//    buf.append(": pid=");
//    buf.append(pid);
//    buf.append(" uid=");
//    buf.append(uid);
//    buf.append(" gids={");
//    if (gids != null) {
//        for (int gi=0; gi<gids.length; gi++) {
//            if (gi != 0) buf.append(", ");
//            buf.append(gids[gi]);
//
//        }
//    }
//    buf.append("}");
//    Slog.i(TAG, buf.toString());
    if (pid == 0 || pid == mMyPid) {
        // Processes are being emulated with threads.
        app->mPid = mMyPid;
        app->mRemoved = FALSE;
        mStartingProcesses.PushBack(app);
    } else if (pid > 0) {
        app->mPid = pid;
        app->mRemoved = FALSE;
        Mutex::Autolock lock(mPidsSelfLock);
        mPidsSelfLocked[pid] = app;
//        Message msg = mHandler.obtainMessage(PROC_START_TIMEOUT_MSG);
//        msg.obj = app;
//        mHandler.sendMessageDelayed(msg, PROC_START_TIMEOUT);
    } else {
        app->mPid = 0;
//        RuntimeException e = new RuntimeException(
//                "Failure starting process " + app.processName
//                + ": returned pid=" + pid);
//        Slog.e(TAG, e.getMessage(), e);
    }
//    } catch (RuntimeException e) {
//    // XXX do better error recovery.
//    app.pid = 0;
//    Slog.e(TAG, "Failure starting process " + app.processName, e);
//    }
}

void CActivityManagerService::UpdateUsageStats(
    /* [in] */ CActivityRecord* resumedComponent,
    /* [in] */ Boolean resumed)
{
    if (resumed) {
//        mUsageStatsService.noteResumeComponent(resumedComponent.realActivity);
    } else {
//        mUsageStatsService.notePauseComponent(resumedComponent.realActivity);
    }
}

Boolean CActivityManagerService::StartHomeActivityLocked()
{
    if (mFactoryTest == SystemServer::FACTORY_TEST_LOW_LEVEL
            && mTopAction.IsNull()) {
        // We are running in factory test mode, but unable to find
        // the factory test app, so just sit around displaying the
        // error message and don't try to start anything.
        return FALSE;
    }

    AutoPtr<IUri> uri;
    if (!mTopData.IsNull()) {
        Uri::Parse(mTopData, (IUri**)&uri);
    }
    AutoPtr<IIntent> intent;
    CIntent::New(mTopAction, (IUri*)uri, (IIntent**)&intent);
    intent->SetComponent(mTopComponent);
    if (mFactoryTest != SystemServer::FACTORY_TEST_LOW_LEVEL) {
        intent->AddCategory(String(Intent_CATEGORY_HOME));
    }
    AutoPtr<IActivityInfo> aInfo;
    intent->ResolveActivityInfo(GetCapsuleManager(),
            STOCK_PM_FLAGS, (IActivityInfo**)&aInfo);
    if (aInfo != NULL) {
        AutoPtr<IApplicationInfo> appInfo;
        aInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
        String cname, name, pname;
        appInfo->GetCapsuleName(&cname);
        aInfo->GetName(&name);
        aInfo->GetProcessName(&pname);
        AutoPtr<IComponentName> component;
        CComponentName::New(cname, name, (IComponentName**)&component);
        intent->SetComponent((IComponentName*)component);
        // Don't do this if the home app is currently being
        // instrumented.
        Int32 uid;
        appInfo->GetUid(&uid);
        ProcessRecord* app = GetProcessRecordLocked(pname, uid);
        if (app == NULL || app->mInstrumentationClass == NULL) {
            Int32 flags, status;
            intent->GetFlags(&flags);
            intent->SetFlags(flags | Intent_FLAG_ACTIVITY_NEW_TASK);
            mMainStack->StartActivityLocked(NULL, (IIntent*)intent, String(NULL), NULL,
                    0, (IActivityInfo*)aInfo, NULL, String(NULL), 0, 0, 0, FALSE, FALSE, &status);
        }
    }

    return TRUE;
}

/**
 * Starts the "new version setup screen" if appropriate.
 */
void CActivityManagerService::StartSetupActivityLocked()
{
/*    // Only do this once per boot.
    if (mCheckedForSetup) {
        return;
    }

    // We will show this screen if the current one is a different
    // version than the last one shown, and we are not running in
    // low-level factory test mode.
    const AutoPtr<IContentResolver> resolver;
    AutoPtr<IContext> mContext;
    mContext->GetContentResolver((IContentResolver**)&resolver);//final ContentResolver resolver = mContext.getContentResolver();
    Int32 mInt;
    CSettings::Secure::GetInt((IContentResolver*)resolver,CSettings::Secure::DEVICE_PROVISIONED, 0,&mInt);
    if (mFactoryTest != SystemServer::FACTORY_TEST_LOW_LEVEL && mInt != 0) {
        mCheckedForSetup = TRUE;

*/        // See if we should be showing the platform update setup UI.
//        IIntent* intent;
//        CIntent::New(String(Intent_ACTION_UPGRADE_SETUP),(IIntent**)&intent);
//        List<ResolveInfo> ris = mSelf.mContext.getPackageManager()
//                .queryIntentActivities(intent, PackageManager.GET_META_DATA);
//
//        // We don't allow third party apps to replace this.
//        ResolveInfo ri = null;
//        for (int i=0; ris != null && i<ris.size(); i++) {
//            if ((ris.get(i).activityInfo.applicationInfo.flags
//                    & ApplicationInfo.FLAG_SYSTEM) != 0) {
//                ri = ris.get(i);
//                break;
//            }
//        }
//
//        if (ri != null) {
//            String vers = ri.activityInfo.metaData != null
//                    ? ri.activityInfo.metaData.getString(Intent.METADATA_SETUP_VERSION)
//                    : null;
//            if (vers == null && ri.activityInfo.applicationInfo.metaData != null) {
//                vers = ri.activityInfo.applicationInfo.metaData.getString(
//                        Intent.METADATA_SETUP_VERSION);
//            }
//            String lastVers = Settings.Secure.getString(
//                    resolver, Settings.Secure.LAST_SETUP_SHOWN);
//            if (vers != null && !vers.equals(lastVers)) {
//                intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//                intent.setComponent(new ComponentName(
//                        ri.activityInfo.packageName, ri.activityInfo.name));
//                mMainStack.startActivityLocked(null, intent, null, null, 0, ri.activityInfo,
//                        null, null, 0, 0, 0, false, false);
//            }
//        }
//    }
}

void CActivityManagerService::ReportResumedActivityLocked(
    /* [in] */ CActivityRecord* r)
{
    //Slog.i(TAG, "**** REPORT RESUME: " + r);

    const Int32 identHash = 0; //System.identityHashCode(r);
    UpdateUsageStats(r, TRUE);

    Int32 i = 0;
    mWatchers->BeginBroadcast(&i);
    while (i > 0) {
        i--;
        AutoPtr<IActivityWatcher> w;
        mWatchers->GetBroadcastItem(i,(IInterface**)&w);
        if (w != NULL) {
//            try {
            w->ActivityResuming(identHash);
//            } catch (RemoteException e) {
//            }
        }
    }
    mWatchers->FinishBroadcast();
}

void CActivityManagerService::DoPendingActivityLaunchesLocked(
    /* [in] */ Boolean doResume)
{
    if (mPendingActivityLaunches.GetSize() <= 0) {
        return;
    }

    List<PendingActivityLaunch*>::Iterator it = \
            mPendingActivityLaunches.Begin();
    for (; it != mPendingActivityLaunches.End(); ++it) {
        PendingActivityLaunch* pal = *it;
        Int32 status;
        mMainStack->StartActivityUncheckedLocked((CActivityRecord*)pal->mR,
                (CActivityRecord*)pal->mSourceRecord,
                pal->mGrantedUriPermissions, pal->mGrantedMode, pal->mOnlyIfNeeded,
                doResume && it == --mPendingActivityLaunches.End(), &status);
    }
    mPendingActivityLaunches.Clear();
}

ECode CActivityManagerService::StartActivity(
    /* [in] */ IApplicationApartment* caller,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ IObjectContainer* grantedUriPermissions,
    /* [in] */ Int32 grantedMode,
    /* [in] */ IBinder* resultTo,
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 requestCode,
    /* [in] */ Boolean onlyIfNeeded,
    /* [in] */ Boolean debug,
    /* [out] */ Int32* status)
{
    if (status == NULL) return E_INVALID_ARGUMENT;

    return mMainStack->StartActivityMayWait(caller, intent, resolvedType,
            grantedUriPermissions, grantedMode, resultTo, resultWho,
            requestCode, onlyIfNeeded, debug, NULL, NULL, status);
}

ECode CActivityManagerService::StartActivityAndWait(
    /* [in] */ IApplicationApartment* caller,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ IObjectContainer* grantedUriPermissions,
    /* [in] */ Int32 grantedMode,
    /* [in] */ IBinder* resultTo,
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 requestCode,
    /* [in] */ Boolean onlyIfNeeded,
    /* [in] */ Boolean debug,
    /* [out] */ IWaitResult** result)
{
    Int32 status;
    CWaitResult::New(result);
    return mMainStack->StartActivityMayWait(caller, intent, resolvedType,
            grantedUriPermissions, grantedMode, resultTo, resultWho,
            requestCode, onlyIfNeeded, debug, *result, NULL, &status);
}

ECode CActivityManagerService::StartActivityWithConfig(
    /* [in] */ IApplicationApartment* caller,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ IObjectContainer* grantedUriPermissions,
    /* [in] */ Int32 grantedMode,
    /* [in] */ IBinder* resultTo,
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 requestCode,
    /* [in] */ Boolean onlyIfNeeded,
    /* [in] */ Boolean debug,
    /* [in] */ IConfiguration* config,
    /* [out] */ Int32* status)
{
    return mMainStack->StartActivityMayWait(caller, intent, resolvedType,
            grantedUriPermissions, grantedMode, resultTo, resultWho,
            requestCode, onlyIfNeeded, debug, NULL, config, status);
}

ECode CActivityManagerService::StartActivityIntentSender(
    /* [in] */ IApplicationApartment* caller,
    /* [in] */ IIntentSender* sender,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ const String& resolvedType,
    /* [in] */ IBinder* resultTo,
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [out] */ Int32* status)
{
    // Refuse possible leaked file descriptors
    Boolean hasFd;
    if (fillInIntent != NULL &&
            (fillInIntent->HasFileDescriptors(&hasFd), hasFd)) {
        Slogger::E(TAG, "File descriptors passed in Intent");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    ClassID senderClsid;
    IObject* senderObj = (IObject*)sender->Probe(EIID_IObject);
    senderObj->GetClassID(&senderClsid);
    if (senderClsid != ECLSID_CPendingIntentRecord) {
        Slogger::E(TAG, "Bad PendingIntent object");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    CPendingIntentRecord* pir = (CPendingIntentRecord*)sender;

    {
        Mutex::Autolock lock(_m_syncLock);

        // If this is coming from the currently resumed activity, it is
        // effectively saying that app switches are allowed at this point.
        if (mMainStack->mResumedActivity != NULL) {
            AutoPtr<IApplicationInfo> appInfo;
            mMainStack->mResumedActivity->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            Int32 uid;
            appInfo->GetUid(&uid);
            if (uid == Binder::GetCallingUid()) {
                mAppSwitchesAllowedTime = 0;
            }
        }
    }

    return pir->SendInner(0, fillInIntent, resolvedType,
            NULL, resultTo, resultWho, requestCode, flagsMask, flagsValues);
}


ECode CActivityManagerService::StartNextMatchingActivity(
    /* [in] */ IBinder* callingActivity,
    /* [in] */ IIntent* intent,
    /* [out] */ Boolean* result)
{
    // Refuse possible leaked file descriptors
    Boolean hasFD;

    if (intent != NULL && (intent->HasFileDescriptors(&hasFD),hasFD)) {

        Slogger::E(TAG, "File descriptors passed in Intent.\n");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    {
        Mutex::Autolock lock(_m_syncLock);

        Int32 index = mMainStack->GetIndexOfTokenLocked(callingActivity);
        if (index < 0) {
            *result = FALSE;
            return NOERROR;
        }
        AutoPtr<CActivityRecord> r = (CActivityRecord*)mMainStack->mHistory[index];
        if (r->mApp == NULL || r->mApp->mAppApartment== NULL) {
            // The caller is not running...  d'oh!
            *result = FALSE;
            return NOERROR;
        }
        AutoPtr<IIntent> newIntent;
        CIntent::New(intent, (IIntent**)&newIntent);
        // The caller is not allowed to change the data.
        String mGetType;
        r->mIntent->GetType(&mGetType);
        AutoPtr<IUri> mGetData;
        r->mIntent->GetData((IUri**)&mGetData);
        newIntent->SetDataAndType(mGetData, mGetType);
        // And we are resetting to find the next component...
        newIntent->SetComponent(NULL);

        AutoPtr<IActivityInfo> aInfo;
        // try {
        // List<ResolveInfo> resolves =
        //         AppGlobals.getPackageManager().queryIntentActivities(
        //                 intent, r.resolvedType,
        //                 PackageManager.MATCH_DEFAULT_ONLY | STOCK_PM_FLAGS);
        AutoPtr<ICapsuleManager> cm;
        AutoPtr<IObjectContainer> resolves;
        cm->QueryIntentActivities(
                    newIntent, r->mResolvedType,
                    CapsuleManager_MATCH_DEFAULT_ONLY | STOCK_PM_FLAGS,
                    (IObjectContainer**)&resolves);

        // Look for the original activity in the list...
        AutoPtr<IObjectEnumerator> enumerator;
        resolves->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
        Boolean hasNext = FALSE;
        while(enumerator->MoveNext(&hasNext), hasNext){
            AutoPtr<IResolveInfo> rInfo;
            enumerator->Current((IInterface**)&rInfo);
            aInfo = NULL;
            rInfo->GetActivityInfo((IActivityInfo**)&aInfo);
            String cName;
            aInfo->GetCapsuleName(&cName);
            String name;
            aInfo->GetName(&name);
            String rName;
            r->mInfo->GetName(&rName);
            if (cName.Equals(r->mCapsuleName) && name.Equals(rName)) {
                // We found the current one...  the next matching is
                // after it.
                enumerator->MoveNext(&hasNext);
                if (hasNext) {
                    rInfo = NULL;
                    enumerator->Current((IInterface**)&rInfo);
                    aInfo = NULL;
                    rInfo->GetActivityInfo((IActivityInfo**)&aInfo);
                }
                break;
            }
        }
        // } catch (RemoteException e) {
        // }

        if (aInfo == NULL) {
            // Nobody who is next!
            *result = FALSE;
            return NOERROR;
        }

        AutoPtr<IApplicationInfo> appInfo;
        aInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
        String appCName;
        appInfo->GetCapsuleName(&appCName);
        String aName;
        aInfo->GetName(&aName);
        AutoPtr<IComponentName> component;
        CComponentName::New(appCName, aName,(IComponentName**)&component);
        newIntent->SetComponent(component);
        Int32 flags;
        newIntent->GetFlags(&flags);
        newIntent->SetFlags(flags&~(
                Intent_FLAG_ACTIVITY_FORWARD_RESULT|
                Intent_FLAG_ACTIVITY_CLEAR_TOP|
                Intent_FLAG_ACTIVITY_MULTIPLE_TASK|
                Intent_FLAG_ACTIVITY_NEW_TASK));

        // Okay now we need to start the new activity, replacing the
        // currently running activity.  This is a little tricky because
        // we want to start the new one as if the current one is finished,
        // but not finish the current one first so that there is no flicker.
        // And thus...
        const Boolean wasFinishing = r->mFinishing;
        r->mFinishing = TRUE;

        // Propagate reply information over to the new activity.
         AutoPtr<CActivityRecord> resultTo = r->mResultTo;
        const String resultWho = r->mResultWho;
        const Int32 requestCode = r->mRequestCode;
        r->mResultTo = NULL;
        if (resultTo != NULL) {
            resultTo->RemoveResultsLocked(r, resultWho, requestCode);
        }

        const Int64 origId = Binder::ClearCallingIdentity();
        // XXX we are not dealing with propagating grantedUriPermissions...
        // those are not yet exposed to user code, so there is no need.
        Int32 res;
        mMainStack->StartActivityLocked(r->mApp->mAppApartment, newIntent,
                r->mResolvedType, NULL, 0, aInfo, resultTo, resultWho,
                requestCode, -1, r->mLaunchedFromUid, FALSE, FALSE, &res);
        Binder::RestoreCallingIdentity(origId);

        r->mFinishing = wasFinishing;
        if (res != ActivityManager_START_SUCCESS) {
            *result = FALSE;
            return NOERROR;
        }
        *result = TRUE;
        return NOERROR;
    }
}

ECode CActivityManagerService::StartActivityInCapsule(
    /* [in] */ Int32 uid,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ IBinder* resultTo,
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 requestCode,
    /* [in] */ Boolean onlyIfNeeded,
    /* [out] */ Int32* result)
{
    // This is so super not safe, that only the system (or okay root)
    // can do it.
    const Int32 callingUid = Binder::GetCallingUid();
    if (callingUid != 0 && callingUid != Process::MyUid()) {
        Slogger::E(TAG, "startActivityInPackage only available to the system");
        return E_SECURITY_EXCEPTION;
    }

    AutoPtr<IComponentName> component;
    intent->GetComponent((IComponentName**)&component);
    const Boolean componentSpecified = component != NULL;

    // Don't modify the client's object!
    AutoPtr<IIntent> newIntent;
    CIntent::New(intent, (IIntent**)&newIntent);

    // Collect information about the target of the Intent.
    AutoPtr<IActivityInfo> aInfo;
    //try {
    AutoPtr<ICapsuleManager> cm;
    AutoPtr<IResolveInfo> rInfo;
//      AppGlobals.getPackageManager().resolveIntent(
    cm->ResolveIntent(
                newIntent, resolvedType,
                CapsuleManager_MATCH_DEFAULT_ONLY | STOCK_PM_FLAGS, (IResolveInfo**)&rInfo);
    if (rInfo != NULL) {
        rInfo->GetActivityInfo((IActivityInfo**)&aInfo);
    }
    //} catch (RemoteException e) {
    //    aInfo = NULL;
    //}

    if (aInfo != NULL) {
        // Store the found target back into the intent, because now that
        // we have it we never want to do this again.  For example, if the
        // user navigates back to this point in the history, we should
        // always restart the exact same activity.
        AutoPtr<IApplicationInfo> appInfo;
        String cName;
        aInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
        appInfo->GetCapsuleName(&cName);
        String name;
        aInfo->GetName(&name);
        AutoPtr<IComponentName> component;
        CComponentName::New(cName, name, (IComponentName**)&component);
        newIntent->SetComponent(component);
    }

    {
        Mutex::Autolock lock(_m_syncLock);

        return mMainStack->StartActivityLocked(NULL, intent, resolvedType,
                NULL, 0, aInfo, resultTo, resultWho, requestCode, -1, uid,
                onlyIfNeeded, componentSpecified, result);
    }
}

void CActivityManagerService::AddRecentTaskLocked(
    /* [in] */ TaskRecord* task)
{
    // Remove any existing entries that are the same kind of task.
    List<TaskRecord*>::Iterator it = mRecentTasks.Begin();
    while (it != mRecentTasks.End()) {
        Boolean equal;
        TaskRecord* sr = *it;
        if ((!task->mAffinity.IsNull() && !task->mAffinity.Compare(sr->mAffinity))
                || (task->mIntent != NULL &&
                (task->mIntent->FilterEquals(sr->mIntent, &equal), equal))) {
            it = mRecentTasks.Erase(it);
            if (task->mIntent == NULL) {
                // If the new recent task we are adding is not fully
                // specified, then replace it with the existing recent task.
                task = sr;
            }
        }
        else ++it;
    }
    if (mRecentTasks.GetSize() >= (List<TaskRecord*>::SizeType)MAX_RECENT_TASKS) {
        mRecentTasks.PopBack();
    }
    mRecentTasks.PushFront(task);
}

ECode CActivityManagerService::SetRequestedOrientation(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 requestedOrientation)
{
    Mutex::Autolock lock(_m_syncLock);
    Int32 index = mMainStack->GetIndexOfTokenLocked(token);
    if (index < 0) {
        return NOERROR;;
    }
    AutoPtr<CActivityRecord> r = mMainStack->mHistory[index];
    const Int64 origId = Binder::ClearCallingIdentity();
    mWindowManager->SetAppOrientation(r, requestedOrientation);
    AutoPtr<IConfiguration> config;
    mWindowManager->UpdateOrientationFromAppTokens(
            mConfiguration,
            r->MayFreezeScreenLocked(r->mApp) ? r : NULL,
            (IConfiguration**)&config);
    if (config != NULL) {
        r->mFrozenBeforeDestroy = TRUE;
        if (!UpdateConfigurationLocked(config, r)) {
            mMainStack->ResumeTopActivityLocked(NULL);
        }
    }
    Binder::RestoreCallingIdentity(origId);

    return NOERROR;
}

ECode CActivityManagerService::GetRequestedOrientation(
    /* [in] */ IBinder* token,
    /* [out] */ Int32* requestedOrientation)
{
    Mutex::Autolock lock(_m_syncLock);
    Int32 index = mMainStack->GetIndexOfTokenLocked(token);
    if (index < 0) {
        *requestedOrientation = ActivityInfo_SCREEN_ORIENTATION_UNSPECIFIED;
        return NOERROR;
    }
    AutoPtr<CActivityRecord> r = mMainStack->mHistory[index];
    mWindowManager->GetAppOrientation(r, requestedOrientation);
    return NOERROR;
}

/**
 * This is the internal entry point for handling Activity.Finish().
 *
 * @param token The token referencing the Activity we want to finish.
 * @param resultCode Result code, if any, from this Activity.
 * @param resultData Result data (Intent), if any, from this Activity.
 *
 * @result Returns true if the activity successfully finished, or false if it is still running.
 */
ECode CActivityManagerService::FinishActivity(
    /* [in] */ IBinder *token,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *resultData,
    /* [out] */ Boolean* finished)
{
    // Refuse possible leaked file descriptors
    Boolean hasFd;
    if (resultData != NULL &&
            (resultData->HasFileDescriptors(&hasFd), hasFd)) {
        Slogger::E(TAG, "File descriptors passed in Intent");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    {
        Mutex::Autolock lock(_m_syncLock);

        if (mController != NULL) {
            // Find the first activity that is not finishing.
            AutoPtr<CActivityRecord> next = \
                    mMainStack->GetTopRunningActivityLocked(token, 0);
            if (next != NULL) {
                // ask watcher if this is allowed
                Boolean resumeOK = TRUE;
                if (FAILED(mController->ActivityResuming(
                        next->mCapsuleName, &resumeOK))) {
                    mController = NULL;
                }

                if (!resumeOK) {
                    *finished = FALSE;
                    return NOERROR;
                }
            }
        }
        *finished = mMainStack->RequestFinishActivityLocked(token, resultCode,
                resultData, "app-request");
        return NOERROR;
    }
}

ECode CActivityManagerService::FinishHeavyWeightApp()
{
    if (CheckCallingPermission("android.permission.FORCE_STOP_CAPSULES" /*android.Manifest.permission.FORCE_STOP_CAPSULES*/)
           != CapsuleManager_PERMISSION_GRANTED) {
        Slogger::W(TAG, StringBuffer("Permission Denial: finishHeavyWeightApp() from pid=")
               + Binder::GetCallingPid()
               + ", uid=" + Binder::GetCallingUid()
               + " requires " + "android.permission.FORCE_STOP_CAPSULES");
        // throw new SecurityException(msg);
        return E_SECURITY_EXCEPTION;
    }
//
//    synchronized(this) {
//        if (mHeavyWeightProcess == null) {
//            return;
//        }
//
//        ArrayList<ActivityRecord> activities = new ArrayList<ActivityRecord>(
//                mHeavyWeightProcess.activities);
//        for (int i=0; i<activities.size(); i++) {
//            ActivityRecord r = activities.get(i);
//            if (!r.finishing) {
//                int index = mMainStack.indexOfTokenLocked(r);
//                if (index >= 0) {
//                    mMainStack.finishActivityLocked(r, index, Activity.RESULT_CANCELED,
//                            null, "finish-heavy");
//                }
//            }
//        }
//
//        mHeavyWeightProcess = null;
//        mHandler.sendEmptyMessage(CANCEL_HEAVY_NOTIFICATION_MSG);
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::CrashApplication(
    /* [in] */ Int32 uid,
    /* [in] */ Int32 initialPid,
    /* [in] */ CString capsuleName,
    /* [in] */ CString message)
{
    if (CheckCallingPermission("android.permission.FORCE_STOP_CAPSULES" /*android.Manifest.permission.FORCE_STOP_CAPSULES*/)
           != CapsuleManager_PERMISSION_GRANTED) {
        Slogger::W(TAG, StringBuffer("Permission Denial: crashApplication() from pid=")
            + Binder::GetCallingPid()
            + ", uid=" + Binder::GetCallingUid()
            + " requires " + "android.permission.FORCE_STOP_CAPSULES");
        // throw new SecurityException(msg);
        return E_SECURITY_EXCEPTION;
    }
//
//    synchronized(this) {
//        ProcessRecord proc = null;
//
//        // Figure out which process to kill.  We don't trust that initialPid
//        // still has any relation to current pids, so must scan through the
//        // list.
//        synchronized (mPidsSelfLocked) {
//            for (int i=0; i<mPidsSelfLocked.size(); i++) {
//                ProcessRecord p = mPidsSelfLocked.valueAt(i);
//                if (p.info.uid != uid) {
//                    continue;
//                }
//                if (p.pid == initialPid) {
//                    proc = p;
//                    break;
//                }
//                for (String str : p.pkgList) {
//                    if (str.equals(packageName)) {
//                        proc = p;
//                    }
//                }
//            }
//        }
//
//        if (proc == null) {
//            Slogger::W(TAG, "crashApplication: nothing for uid=" + uid
//                    + " initialPid=" + initialPid
//                    + " packageName=" + packageName);
//            return;
//        }
//
//        if (proc.thread != null) {
//            long ident = Binder.clearCallingIdentity();
//            try {
//                proc.thread.scheduleCrash(message);
//            } catch (RemoteException e) {
//            }
//            Binder.restoreCallingIdentity(ident);
//        }
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::FinishSubActivity(
    /* [in] */ IBinder* token,
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 requestCode)
{
    {
        Mutex::Autolock lock(_m_syncLock);
        Int32 index = mMainStack->GetIndexOfTokenLocked(token);
        if (index < 0) {
            return NOERROR;
        }
        AutoPtr<CActivityRecord> self = mMainStack->mHistory[index];

        const Int64 origId = Binder::ClearCallingIdentity();

        Int32 i = mMainStack->mHistory.GetSize()-1;
        List< AutoPtr<CActivityRecord> >::ReverseIterator rit;
        for (rit = mMainStack->mHistory.RBegin(); rit != mMainStack->mHistory.REnd(); ++rit) {
            AutoPtr<CActivityRecord> r = *rit;
            if (r->mResultTo == self && r->mRequestCode == requestCode) {
                if ((r->mResultWho.IsNull() && resultWho.IsNull()) ||
                    (!r->mResultWho.IsNull() && r->mResultWho.Equals(resultWho))) {
                    mMainStack->FinishActivityLocked(r, i,
                            Activity_RESULT_CANCELED, NULL, "request-sub");
                }
            }
            i--;
        }

        Binder::RestoreCallingIdentity(origId);
    }
    return NOERROR;
}

ECode CActivityManagerService::WillActivityBeVisible(
    /* [in] */ IBinder* token,
    /* [out] */ Boolean* visible)
{
    if (visible == NULL) return E_INVALID_ARGUMENT;

    Mutex::Autolock lock(_m_syncLock);

    List<AutoPtr<CActivityRecord> >::ReverseIterator rit
            = mMainStack->mHistory.RBegin();
    for (; rit != mMainStack->mHistory.REnd(); ++rit) {
        AutoPtr<CActivityRecord> r = *rit;
        if ((IBinder*)r.Get() == token) {
            *visible = TRUE;
            return NOERROR;
        }
        if (r->mFullscreen && !r->mFinishing) {
            *visible = FALSE;
            return NOERROR;
        }
    }

    *visible = TRUE;
    return NOERROR;
}

ECode CActivityManagerService::OverridePendingTransition(
    /* [in] */ IBinder* token,
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 enterAnim,
    /* [in] */ Int32 exitAnim)
{
//    synchronized(this) {
//        int index = mMainStack.indexOfTokenLocked(token);
//        if (index < 0) {
//            return;
//        }
//        ActivityRecord self = (ActivityRecord)mMainStack.mHistory.get(index);
//
//        final long origId = Binder.clearCallingIdentity();
//
//        if (self.state == ActivityState.RESUMED
//                || self.state == ActivityState.PAUSING) {
//            mWindowManager.overridePendingAppTransition(packageName,
//                    enterAnim, exitAnim);
//        }
//
//        Binder.restoreCallingIdentity(origId);
//    }
    return E_NOT_IMPLEMENTED;
}

/**
 * Main function for removing an existing process from the activity manager
 * as a result of that process going away.  Clears out all connections
 * to the process.
 */
void CActivityManagerService::HandleAppDiedLocked(
    /* [in] */ ProcessRecord* app,
    /* [in] */ Boolean restarting)
{
    CleanUpApplicationRecordLocked(app, restarting, -1);
    if (!restarting) {
        mLRUProcesses.Remove(app);
    }

    // Just in case...
    if (mMainStack->mPausingActivity != NULL
            && mMainStack->mPausingActivity->mApp == app) {
        if (DEBUG_PAUSE) {
            String actDes;
            mMainStack->mPausingActivity->GetDescription(&actDes);
            Slogger::V(TAG, StringBuffer("App died while pausing: ")
                    + actDes);
        }
        mMainStack->mPausingActivity = NULL;
    }
    if (mMainStack->mLastPausedActivity != NULL
            && mMainStack->mLastPausedActivity->mApp == app) {
        mMainStack->mLastPausedActivity = NULL;
    }

    // Remove this application's activities from active lists.
    mMainStack->RemoveHistoryRecordsForAppLocked(app);

    Boolean atTop = TRUE;
    Boolean hasVisibleActivities = FALSE;

    // Clean out the history list.
    if (localLOGV) {
        Int32 size = mMainStack->mHistory.GetSize();
        String appDes;
        app->GetDescription(&appDes);
        Slogger::V(TAG, StringBuffer("Removing app ") + appDes
                + " from history with " + size + " entries");
    }

    List<AutoPtr<CActivityRecord> >::ReverseIterator rit = mMainStack->mHistory.RBegin();
    for (; rit != mMainStack->mHistory.REnd(); ++rit) {
        AutoPtr<CActivityRecord> r = *rit;
        if (localLOGV) {
            String actDes, appDes;
            r->GetDescription(&actDes);
            r->mApp->GetDescription(&appDes);
            Slogger::V(TAG, StringBuffer("Record ") + actDes + ": app=" + appDes);
        }
        if (r->mApp == app) {
            if ((!r->mHaveState && !r->mStateNotNeeded) || r->mFinishing) {
                if (localLOGV) {
                    Slogger::V(TAG, StringBuffer("Removing this entry!  frozen=")
                            + r->mHaveState + " finishing=" + r->mFinishing);
                }
                mMainStack->mHistory.Erase(--rit.GetBase());

                r->mInHistory = FALSE;
                mWindowManager->RemoveAppToken(r);
                if (VALIDATE_TOKENS) {
                    mWindowManager->ValidateAppTokens(mMainStack->mHistory);
                }
                r->RemoveUriPermissionsLocked();

            }
            else {
                // We have the current state for this activity, so
                // it can be restarted later when needed.
                if (localLOGV) {
                    Slogger::V(TAG, "Keeping entry, setting app to null");
                }
                if (r->mVisible) {
                    hasVisibleActivities = TRUE;
                }
                r->mApp = NULL;
                r->mNowVisible = FALSE;
                if (!r->mHaveState) {
                    r->mIcicle = NULL;
                }
            }

            r->mStack->CleanUpActivityLocked(r, TRUE);
            r->mState = ActivityState_STOPPED;
        }
        atTop = FALSE;
    }

    app->mActivities.Clear();

    if (app->mInstrumentationClass != NULL) {
        String instrStr;
        app->mInstrumentationClass->GetDescription(&instrStr);
        Slogger::W(TAG, StringBuffer("Crash of app ") + app->mProcessName
              + " running instrumentation " + instrStr);

        AutoPtr<IBundle> info;
        CBundle::New((IBundle**)&info);
        info->PutString(String("shortMsg"), String("Process crashed."));
        FinishInstrumentationLocked(app, Activity_RESULT_CANCELED, info);
    }

    if (!restarting) {
        if (!mMainStack->ResumeTopActivityLocked(NULL)) {
            // If there was nothing to resume, and we are not already
            // restarting this process, but there is a visible activity that
            // is hosted by the process...  then make sure all visible
            // activities are running, taking care of restarting this
            // process.
            if (hasVisibleActivities) {
                mMainStack->EnsureActivitiesVisibleLocked(NULL, 0);
            }
        }
    }
}

Int32 CActivityManagerService::GetLRUProcessRecordIndexForAppLocked(
    /* [in] */ const IApplicationApartment* appApartment)
{
    // Find the application record.
    Int32 idx = mLRUProcesses.GetSize() - 1;
    List<ProcessRecord*>::ReverseIterator it;
    for (it = mLRUProcesses.RBegin(); it != mLRUProcesses.REnd(); ++it, --idx) {
        ProcessRecord* rec = *it;
        if (rec->mAppApartment != NULL &&
            rec->mAppApartment == appApartment) {
            return idx;
        }
    }
    return -1;
}

ProcessRecord* CActivityManagerService::GetProcessRecordForAppLocked(
    /* [in] */ const IApplicationApartment* appApartment)
{
    if (appApartment == NULL) return NULL;

    List<ProcessRecord*>::Iterator it;
    for (it = mLRUProcesses.Begin(); it != mLRUProcesses.End(); ++it) {
        ProcessRecord* rec = *it;
        if (rec->mAppApartment != NULL &&
            rec->mAppApartment == appApartment) {
            return rec;
        }
    }
    return NULL;
}

void CActivityManagerService::AppDiedLocked(
    /* [in] */ ProcessRecord* app,
    /* [in] */ Int32 pid,
    /* [in] */ IApplicationApartment* appApartment)
{
//    mProcDeaths[0]++;
//
//    BatteryStatsImpl stats = mBatteryStatsService.getActiveStatistics();
//    synchronized (stats) {
//        stats.noteProcessDiedLocked(app.info.uid, pid);
//    }
//
//    // Clean up already done if the process has been re-started.
//    if (app.pid == pid && app.thread != null &&
//            app.thread.asBinder() == thread.asBinder()) {
//        if (!app.killedBackground) {
//            Slog.i(TAG, "Process " + app.processName + " (pid " + pid
//                    + ") has died.");
//        }
//        EventLog.writeEvent(EventLogTags.AM_PROC_DIED, app.pid, app.processName);
//        if (localLOGV) Slog.v(
//            TAG, "Dying app: " + app + ", pid: " + pid
//            + ", thread: " + thread.asBinder());
//        boolean doLowMem = app.instrumentationClass == null;
//        handleAppDiedLocked(app, false);
//
//        if (doLowMem) {
//            // If there are no longer any background processes running,
//            // and the app that died was not running instrumentation,
//            // then tell everyone we are now low on memory.
//            boolean haveBg = false;
//            for (int i=mLruProcesses.size()-1; i>=0; i--) {
//                ProcessRecord rec = mLruProcesses.get(i);
//                if (rec.thread != null && rec.setAdj >= HIDDEN_APP_MIN_ADJ) {
//                    haveBg = true;
//                    break;
//                }
//            }
//
//            if (!haveBg) {
//                Slog.i(TAG, "Low Memory: No more background processes.");
//                EventLog.writeEvent(EventLogTags.AM_LOW_MEMORY, mLruProcesses.size());
//                long now = SystemClock.uptimeMillis();
//                for (int i=mLruProcesses.size()-1; i>=0; i--) {
//                    ProcessRecord rec = mLruProcesses.get(i);
//                    if (rec != app && rec.thread != null &&
//                            (rec.lastLowMemory+GC_MIN_INTERVAL) <= now) {
//                        // The low memory report is overriding any current
//                        // state for a GC request.  Make sure to do
//                        // heavy/important/visible/foreground processes first.
//                        if (rec.setAdj <= HEAVY_WEIGHT_APP_ADJ) {
//                            rec.lastRequestedGc = 0;
//                        } else {
//                            rec.lastRequestedGc = rec.lastLowMemory;
//                        }
//                        rec.reportLowMemory = true;
//                        rec.lastLowMemory = now;
//                        mProcessesToGc.remove(rec);
//                        addProcessToGcListLocked(rec);
//                    }
//                }
//                scheduleAppGcsLocked();
//            }
//        }
//    } else if (app.pid != pid) {
//        // A new process has already been started.
//        Slog.i(TAG, "Process " + app.processName + " (pid " + pid
//                + ") has died and restarted (pid " + app.pid + ").");
//        EventLog.writeEvent(EventLogTags.AM_PROC_DIED, app.pid, app.processName);
//    } else if (DEBUG_PROCESSES) {
//        Slog.d(TAG, "Received spurious death notification for thread "
//                + thread.asBinder());
//    }
}

void CActivityManagerService::AppNotResponding(
    /* [in] */ ProcessRecord* app,
    /* [in] */ CActivityRecord* activity,
    /* [in] */ CActivityRecord* parent,
    /* [in] */ CString annotation)
{
//ArrayList<Integer> firstPids = new ArrayList<Integer>(5);
//        SparseArray<Boolean> lastPids = new SparseArray<Boolean>(20);
//
//        if (mController != null) {
//            try {
//                // 0 == continue, -1 = kill process immediately
//                int res = mController.appEarlyNotResponding(app.processName, app.pid, annotation);
//                if (res < 0 && app.pid != MY_PID) Process.killProcess(app.pid);
//            } catch (RemoteException e) {
//                mController = null;
//            }
//        }
//
//        long anrTime = SystemClock.uptimeMillis();
//        if (MONITOR_CPU_USAGE) {
//            updateCpuStatsNow();
//        }
//
//        synchronized (this) {
//            // PowerManager.reboot() can block for a long time, so ignore ANRs while shutting down.
//            if (mShuttingDown) {
//                Slog.i(TAG, "During shutdown skipping ANR: " + app + " " + annotation);
//                return;
//            } else if (app.notResponding) {
//                Slog.i(TAG, "Skipping duplicate ANR: " + app + " " + annotation);
//                return;
//            } else if (app.crashing) {
//                Slog.i(TAG, "Crashing app skipping ANR: " + app + " " + annotation);
//                return;
//            }
//
//            // In case we come through here for the same app before completing
//            // this one, mark as anring now so we will bail out.
//            app.notResponding = true;
//
//            // Log the ANR to the event log.
//            EventLog.writeEvent(EventLogTags.AM_ANR, app.pid, app.processName, app.info.flags,
//                    annotation);
//
//            // Dump thread traces as quickly as we can, starting with "interesting" processes.
//            firstPids.add(app.pid);
//
//            int parentPid = app.pid;
//            if (parent != null && parent.app != null && parent.app.pid > 0) parentPid = parent.app.pid;
//            if (parentPid != app.pid) firstPids.add(parentPid);
//
//            if (MY_PID != app.pid && MY_PID != parentPid) firstPids.add(MY_PID);
//
//            for (int i = mLruProcesses.size() - 1; i >= 0; i--) {
//                ProcessRecord r = mLruProcesses.get(i);
//                if (r != null && r.thread != null) {
//                    int pid = r.pid;
//                    if (pid > 0 && pid != app.pid && pid != parentPid && pid != MY_PID) {
//                        if (r.persistent) {
//                            firstPids.add(pid);
//                        } else {
//                            lastPids.put(pid, Boolean.TRUE);
//                        }
//                    }
//                }
//            }
//        }
//
//        // Log the ANR to the main log.
//        StringBuilder info = mStringBuilder;
//        info.setLength(0);
//        info.append("ANR in ").append(app.processName);
//        if (activity != null && activity.shortComponentName != null) {
//            info.append(" (").append(activity.shortComponentName).append(")");
//        }
//        info.append("\n");
//        if (annotation != null) {
//            info.append("Reason: ").append(annotation).append("\n");
//        }
//        if (parent != null && parent != activity) {
//            info.append("Parent: ").append(parent.shortComponentName).append("\n");
//        }
//
//        final ProcessStats processStats = new ProcessStats(true);
//
//        File tracesFile = dumpStackTraces(true, firstPids, processStats, lastPids);
//
//        String cpuInfo = null;
//        if (MONITOR_CPU_USAGE) {
//            updateCpuStatsNow();
//            synchronized (mProcessStatsThread) {
//                cpuInfo = mProcessStats.printCurrentState(anrTime);
//            }
//            info.append(processStats.printCurrentLoad());
//            info.append(cpuInfo);
//        }
//
//        info.append(processStats.printCurrentState(anrTime));
//
//        Slog.e(TAG, info.toString());
//        if (tracesFile == null) {
//            // There is no trace file, so dump (only) the alleged culprit's threads to the log
//            Process.sendSignal(app.pid, Process.SIGNAL_QUIT);
//        }
//
//        addErrorToDropBox("anr", app, activity, parent, annotation, cpuInfo, tracesFile, null);
//
//        if (mController != null) {
//            try {
//                // 0 == show dialog, 1 = keep waiting, -1 = kill process immediately
//                int res = mController.appNotResponding(app.processName, app.pid, info.toString());
//                if (res != 0) {
//                    if (res < 0 && app.pid != MY_PID) Process.killProcess(app.pid);
//                    return;
//                }
//            } catch (RemoteException e) {
//                mController = null;
//            }
//        }
//
//        // Unless configured otherwise, swallow ANRs in background processes & kill the process.
//        boolean showBackground = Settings.Secure.getInt(mContext.getContentResolver(),
//                Settings.Secure.ANR_SHOW_BACKGROUND, 0) != 0;
//
//        synchronized (this) {
//            if (!showBackground && !app.isInterestingToUserLocked() && app.pid != MY_PID) {
//                Process.killProcess(app.pid);
//                return;
//            }
//
//            // Set the app's notResponding state, and look up the errorReportReceiver
//            makeAppNotRespondingLocked(app,
//                    activity != null ? activity.shortComponentName : null,
//                    annotation != null ? "ANR " + annotation : "ANR",
//                    info.toString());
//
//            // Bring up the infamous App Not Responding dialog
//            Message msg = Message.obtain();
//            HashMap map = new HashMap();
//            msg.what = SHOW_NOT_RESPONDING_MSG;
//            msg.obj = map;
//            map.put("app", app);
//            if (activity != null) {
//                map.put("activity", activity);
//            }
//
//            mHandler.sendMessage(msg);
//        }
}

void CActivityManagerService::ShowLaunchWarningLocked(
    /* [in] */ CActivityRecord* cur,
    /* [in] */ CActivityRecord* next)
{
//    if (!mLaunchWarningShown) {
//        mLaunchWarningShown = true;
//        mHandler.post(new Runnable() {
//            @Override
//            public void run() {
//                synchronized (ActivityManagerService.this) {
//                    final Dialog d = new LaunchWarningWindow(mContext, cur, next);
//                    d.show();
//                    mHandler.postDelayed(new Runnable() {
//                        @Override
//                        public void run() {
//                            synchronized (ActivityManagerService.this) {
//                                d.dismiss();
//                                mLaunchWarningShown = false;
//                            }
//                        }
//                    }, 4000);
//                }
//            }
//        });
//    }
}

ECode CActivityManagerService::ClearApplicationUserData(
    /* [in] */ const String& capsuleName,
    /* [in] */ ICapsuleDataObserver* observer,
    /* [out] */ Boolean* result)
{
//    int uid = Binder.getCallingUid();
//    int pid = Binder.getCallingPid();
//    long callingId = Binder.clearCallingIdentity();
//    try {
//        IPackageManager pm = AppGlobals.getPackageManager();
//        int pkgUid = -1;
//        synchronized(this) {
//            try {
//                pkgUid = pm.getPackageUid(packageName);
//            } catch (RemoteException e) {
//            }
//            if (pkgUid == -1) {
//                Slogger::W(TAG, "Invalid packageName:" + packageName);
//                return false;
//            }
//            if (uid == pkgUid || checkComponentPermission(
//                    android.Manifest.permission.CLEAR_APP_USER_DATA,
//                    pid, uid, -1)
//                    == PackageManager.PERMISSION_GRANTED) {
//                forceStopPackageLocked(packageName, pkgUid);
//            } else {
//                throw new SecurityException(pid+" does not have permission:"+
//                        android.Manifest.permission.CLEAR_APP_USER_DATA+" to clear data" +
//                                "for process:"+packageName);
//            }
//        }
//
//        try {
//            //clear application user data
//            pm.clearApplicationUserData(packageName, observer);
//            Intent intent = new Intent(Intent.ACTION_CAPSULE_DATA_CLEARED,
//                    Uri.fromParts("package", packageName, null));
//            intent.putExtra(Intent.EXTRA_UID, pkgUid);
//            broadcastIntentInPackage("android", Process.SYSTEM_UID, intent,
//                    null, null, 0, null, null, null, false, false);
//        } catch (RemoteException e) {
//        }
//    } finally {
//        Binder.restoreCallingIdentity(callingId);
//    }
//    return true;
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::KillBackgroundProcesses(
    /* [in] */ const String& capsuleName)
{
    if (CheckCallingPermission("android.permission.KILL_BACKGROUND_PROCESSES" /*android.Manifest.permission.KILL_BACKGROUND_PROCESSES*/)
            != CapsuleManager_PERMISSION_GRANTED &&
            CheckCallingPermission("android.permission.RESTART_CAPSULES" /*android.Manifest.permission.RESTART_CAPSULES*/)
                != CapsuleManager_PERMISSION_GRANTED) {
        Slogger::W(TAG, StringBuffer("Permission Denial: killBackgroundProcesses() from pid=")
               + Binder::GetCallingPid()
               + ", uid=" + Binder::GetCallingUid()
               + " requires " + "android.permission.KILL_BACKGROUND_PROCESSES");
        // throw new SecurityException(msg);
        return E_SECURITY_EXCEPTION;
    }
//
//    long callingId = Binder.clearCallingIdentity();
//    try {
//        IPackageManager pm = AppGlobals.getPackageManager();
//        int pkgUid = -1;
//        synchronized(this) {
//            try {
//                pkgUid = pm.getPackageUid(packageName);
//            } catch (RemoteException e) {
//            }
//            if (pkgUid == -1) {
//                Slogger::W(TAG, "Invalid packageName: " + packageName);
//                return;
//            }
//            killPackageProcessesLocked(packageName, pkgUid,
//                    SECONDARY_SERVER_ADJ, false, true);
//        }
//    } finally {
//        Binder.restoreCallingIdentity(callingId);
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::ForceStopCapsule(
    /* [in] */ const String& capsuleName)
{
    if (CheckCallingPermission("android.permission.FORCE_STOP_CAPSULES" /*android.Manifest.permission.FORCE_STOP_CAPSULES*/)
           != CapsuleManager_PERMISSION_GRANTED) {
        Slogger::W(TAG, StringBuffer("Permission Denial: forceStopPackage() from pid=")
               + Binder::GetCallingPid()
               + ", uid=" + Binder::GetCallingUid()
               + " requires " + "android.permission.FORCE_STOP_CAPSULES");
        // throw new SecurityException(msg);
        return E_SECURITY_EXCEPTION;
    }
//
//    long callingId = Binder.clearCallingIdentity();
//    try {
//        IPackageManager pm = AppGlobals.getPackageManager();
//        int pkgUid = -1;
//        synchronized(this) {
//            try {
//                pkgUid = pm.getPackageUid(packageName);
//            } catch (RemoteException e) {
//            }
//            if (pkgUid == -1) {
//                Slogger::W(TAG, "Invalid packageName: " + packageName);
//                return;
//            }
//            forceStopPackageLocked(packageName, pkgUid);
//        }
//    } finally {
//        Binder.restoreCallingIdentity(callingId);
//    }
    return E_NOT_IMPLEMENTED;
}

/*
 * The pkg name and uid have to be specified.
 * @see android.app.IActivityManager#killApplicationWithUid(java.lang.String, int)
 */
ECode CActivityManagerService::KillApplicationWithUid(
    /* [in] */ const String& cap,
    /* [in] */ Int32 uid)
{
//    if (pkg == null) {
//        return;
//    }
//    // Make sure the uid is valid.
//    if (uid < 0) {
//        Slogger::W(TAG, "Invalid uid specified for pkg : " + pkg);
//        return;
//    }
//    int callerUid = Binder.getCallingUid();
//    // Only the system server can kill an application
//    if (callerUid == Process.SYSTEM_UID) {
//        // Post an aysnc message to kill the application
//        Message msg = mHandler.obtainMessage(KILL_APPLICATION_MSG);
//        msg.arg1 = uid;
//        msg.arg2 = 0;
//        msg.obj = pkg;
//        mHandler.sendMessage(msg);
//    } else {
//        throw new SecurityException(callerUid + " cannot kill pkg: " +
//                pkg);
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::CloseSystemDialogs(
    /* [in] */ const String& reason)
{
//    Intent intent = new Intent(Intent.ACTION_CLOSE_SYSTEM_DIALOGS);
//    intent.addFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY);
//    if (reason != null) {
//        intent.putExtra("reason", reason);
//    }
//
//    final int uid = Binder.getCallingUid();
//    final long origId = Binder.clearCallingIdentity();
//    synchronized (this) {
//        int i = mWatchers.beginBroadcast();
//        while (i > 0) {
//            i--;
//            IActivityWatcher w = mWatchers.getBroadcastItem(i);
//            if (w != null) {
//                try {
//                    w.closingSystemDialogs(reason);
//                } catch (RemoteException e) {
//                }
//            }
//        }
//        mWatchers.finishBroadcast();
//
//        mWindowManager.closeSystemDialogs(reason);
//
//        for (i=mMainStack.mHistory.size()-1; i>=0; i--) {
//            ActivityRecord r = (ActivityRecord)mMainStack.mHistory.get(i);
//            if ((r.info.flags&ActivityInfo.FLAG_FINISH_ON_CLOSE_SYSTEM_DIALOGS) != 0) {
//                r.stack.finishActivityLocked(r, i,
//                        Activity.RESULT_CANCELED, null, "close-sys");
//            }
//        }
//
//        broadcastIntentLocked(null, null, intent, null,
//                null, 0, null, null, null, false, false, -1, uid);
//    }
//    Binder.restoreCallingIdentity(origId);
    return E_NOT_IMPLEMENTED;
}

//public Debug.MemoryInfo[] getProcessMemoryInfo(int[] pids)
//{
//    Debug.MemoryInfo[] infos = new Debug.MemoryInfo[pids.length];
//    for (int i=pids.length-1; i>=0; i--) {
//        infos[i] = new Debug.MemoryInfo();
//        Debug.getMemoryInfo(pids[i], infos[i]);
//    }
//    return infos;
//}

ECode CActivityManagerService::KillApplicationProcess(
    /* [in] */ const String& processName,
    /* [in] */ Int32 uid)
{
//    if (processName == null) {
//        return;
//    }
//
//    int callerUid = Binder.getCallingUid();
//    // Only the system server can kill an application
//    if (callerUid == Process.SYSTEM_UID) {
//        synchronized (this) {
//            ProcessRecord app = getProcessRecordLocked(processName, uid);
//            if (app != null) {
//                try {
//                    app.thread.scheduleSuicide();
//                } catch (RemoteException e) {
//                    // If the other end already died, then our work here is done.
//                }
//            } else {
//                Slogger::W(TAG, "Process/uid not found attempting kill of "
//                        + processName + " / " + uid);
//            }
//        }
//    } else {
//        throw new SecurityException(callerUid + " cannot kill app process: " +
//                processName);
//    }
    return E_NOT_IMPLEMENTED;
}

void CActivityManagerService::ForceStopCapsuleLocked(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 uid)
{
//    forceStopPackageLocked(packageName, uid, false, false, true);
//    Intent intent = new Intent(Intent.ACTION_CAPSULE_RESTARTED,
//            Uri.fromParts("package", packageName, null));
//    if (!mProcessesReady) {
//        intent.addFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY);
//    }
//    intent.putExtra(Intent.EXTRA_UID, uid);
//    broadcastIntentLocked(null, null, intent,
//            null, null, 0, null, null, null,
//            false, false, MY_PID, Process.SYSTEM_UID);
}

Boolean CActivityManagerService::KillCapsuleProcessesLocked(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 minOomAdj,
    /* [in] */ Boolean callerWillRestart,
    /* [in] */ Boolean doit)
{
    List<ProcessRecord*> procs;

    // Remove all processes this package may have touched: all with the
    // same UID (except for the system or root user), and all whose name
    // matches the package name.
    StringBuffer sb;
    sb = sb + capsuleName + ":";
    HashMap<String, Map<Int32, ProcessRecord*>*>::Iterator it = \
            mProcessNames->GetMap()->Begin();
    for (; it != mProcessNames->GetMap()->End(); ++it) {
        Map<Int32, ProcessRecord*>::Iterator iit = it->mSecond->Begin();
        for (; iit != it->mSecond->End(); ++iit) {
            ProcessRecord* app = iit->mSecond;
            if (app->mRemoved) {
                if (doit) {
                    procs.PushBack(app);
                }
            }
            else {
                Int32 appUid;
                app->mInfo->GetUid(&appUid);
                if ((uid > 0 && uid != Process::SYSTEM_UID && appUid == uid)
                    || !app->mProcessName.Compare(capsuleName)
                    || app->mProcessName.StartWith((const char*)sb)) {
                    if (app->mSetAdj >= minOomAdj) {
                        if (!doit) {
                            return TRUE;
                        }
                        app->mRemoved = TRUE;
                        procs.PushBack(app);
                    }
                }
            }
        }
    }

    List<ProcessRecord*>::Iterator iter = procs.Begin();
    for (; iter != procs.End(); ++iter) {
        RemoveProcessLocked(*iter, callerWillRestart);
    }
    return procs.GetSize() > 0;
}

Boolean CActivityManagerService::ForceStopCapsuleLocked(
    /* [in] */ const String& name,
    /* [in] */ Int32 uid,
    /* [in] */ Boolean callerWillRestart,
    /* [in] */ Boolean purgeCache,
    /* [in] */ Boolean doit)
{
    int i, N;
    UNUSED(i);
    UNUSED(N);

    if (uid < 0) {
        GetCapsuleManager()->GetCapsuleUid(name, &uid);
    }

    if (doit) {
        Slogger::I(TAG, StringBuffer("Force stopping package ") + name + " uid=" + uid);

        HashMap<String, Map<Int32, Millisecond64>*>::Iterator it = \
                mProcessCrashTimes->GetMap()->Begin();
        for (; it != mProcessCrashTimes->GetMap()->End(); ++it) {
            Map<Int32, Millisecond64>* ba = it->mSecond;
            if (ba->Find(uid) != ba->End()) {
                mProcessCrashTimes->GetMap()->Erase(it);
            }
        }
    }

    Boolean didSomething = KillCapsuleProcessesLocked(name, uid, -100,
            callerWillRestart, doit);
    UNUSED(didSomething);
//
//    for (i=mMainStack.mHistory.size()-1; i>=0; i--) {
//          ActivityRecord r = (ActivityRecord)mMainStack.mHistory.get(i);
//        if (r.packageName.equals(name)) {
//            if (!doit) {
//                return true;
//            }
//            didSomething = true;
//            Slog.i(TAG, "  Force finishing activity " + r);
//            if (r.app != null) {
//                r.app.removed = true;
//            }
//            r.app = null;
//            r.stack.finishActivityLocked(r, i, Activity.RESULT_CANCELED, null, "uninstall");
//        }
//    }
//
//    ArrayList<ServiceRecord> services = new ArrayList<ServiceRecord>();
//    for (ServiceRecord service : mServices.values()) {
//        if (service.packageName.equals(name)) {
//            if (!doit) {
//                return true;
//            }
//            didSomething = true;
//            Slog.i(TAG, "  Force stopping service " + service);
//            if (service.app != null) {
//                service.app.removed = true;
//            }
//            service.app = null;
//            services.add(service);
//        }
//    }
//
//    N = services.size();
//    for (i=0; i<N; i++) {
//        bringDownServiceLocked(services.get(i), true);
//    }
//
//    if (doit) {
//        if (purgeCache) {
//            AttributeCache ac = AttributeCache.instance();
//            if (ac != null) {
//                ac.removePackage(name);
//            }
//        }
//        mMainStack.resumeTopActivityLocked(null);
//    }
//
//    return didSomething;
    return TRUE;
}

Boolean CActivityManagerService::RemoveProcessLocked(
    /* [in] */ ProcessRecord* app,
    /* [in] */ Boolean callerWillRestart)
{
    String name = app->mProcessName;
    Int32 uid;
    app->mInfo->GetUid(&uid);
    if (DEBUG_PROCESSES) {
        String appDes;
        app->GetDescription(&appDes);
        Slogger::D(TAG, StringBuffer("Force removing process ")
                + appDes + " (" + name + "/" + uid + ")");
    }

    mProcessNames->Remove(name, uid);
    if (mHeavyWeightProcess == app) {
        mHeavyWeightProcess = NULL;
//        mHandler.sendEmptyMessage(CANCEL_HEAVY_NOTIFICATION_MSG);
    }
    Boolean needRestart = FALSE;
    if (app->mPid > 0 && app->mPid != mMyPid) {
        Int32 pid = app->mPid;
        {
            Mutex::Autolock lock(mPidsSelfLock);
            mPidsSelfLocked.Erase(pid);
//            mHandler.removeMessages(PROC_START_TIMEOUT_MSG, app);
        }
        HandleAppDiedLocked(app, TRUE);
        mLRUProcesses.Remove(app);
        Process::KillProcess(pid);

        if (app->mPersistent) {
            if (!callerWillRestart) {
                AddAppLocked(app->mInfo);
            } else {
                needRestart = TRUE;
            }
        }
    } else {
        mRemovedProcesses.PushBack(app);
    }

    return needRestart;
}

void CActivityManagerService::ProcessStartTimedOutLocked(
    /* [in] */ ProcessRecord* app)
{
//    final int pid = app.pid;
//    boolean gone = false;
//    synchronized (mPidsSelfLocked) {
//        ProcessRecord knownApp = mPidsSelfLocked.get(pid);
//        if (knownApp != null && knownApp.thread == null) {
//            mPidsSelfLocked.remove(pid);
//            gone = true;
//        }
//    }
//
//    if (gone) {
//        Slogger::W(TAG, "Process " + app + " failed to attach");
//        EventLog.writeEvent(EventLogTags.AM_PROCESS_START_TIMEOUT, pid, app.info.uid,
//                app.processName);
//        mProcessNames.remove(app.processName, app.info.uid);
//        if (mHeavyWeightProcess == app) {
//            mHeavyWeightProcess = null;
//            mHandler.sendEmptyMessage(CANCEL_HEAVY_NOTIFICATION_MSG);
//        }
//        // Take care of any launching providers waiting for this process.
//        checkAppInLaunchingProvidersLocked(app, true);
//        // Take care of any services that are waiting for the process.
//        for (int i=0; i<mPendingServices.size(); i++) {
//            ServiceRecord sr = mPendingServices.get(i);
//            if (app.info.uid == sr.appInfo.uid
//                    && app.processName.equals(sr.processName)) {
//                Slogger::W(TAG, "Forcing bringing down service: " + sr);
//                mPendingServices.remove(i);
//                i--;
//                bringDownServiceLocked(sr, true);
//            }
//        }
//        Process.killProcess(pid);
//        if (mBackupTarget != null && mBackupTarget.app.pid == pid) {
//            Slogger::W(TAG, "Unattached app died before backup, skipping");
//            try {
//                IBackupManager bm = IBackupManager.Stub.asInterface(
//                        ServiceManager.getService(Context.BACKUP_SERVICE));
//                bm.agentDisconnected(app.info.packageName);
//            } catch (RemoteException e) {
//                // Can't happen; the backup manager is local
//            }
//        }
//        if (mPendingBroadcast != null && mPendingBroadcast.curApp.pid == pid) {
//            Slogger::W(TAG, "Unattached app died before broadcast acknowledged, skipping");
//            mPendingBroadcast.state = BroadcastRecord.IDLE;
//            mPendingBroadcast.nextReceiver = mPendingBroadcastRecvIndex;
//            mPendingBroadcast = null;
//            scheduleBroadcastsLocked();
//        }
//    } else {
//        Slogger::W(TAG, "Spurious process start timeout - pid not known for " + app);
//    }
}

Boolean CActivityManagerService::AttachApplicationLocked(
    /* [in] */ IApplicationApartment* appApartment,
    /* [in] */ Int32 pid)
{
    // Find the application record that is being attached...  either via
    // the pid if we are running in multiple processes, or just pull the
    // next app record if we are emulating process with anonymous threads.
    ProcessRecord* app;
    if (pid != mMyPid && pid >= 0) {
        Mutex::Autolock lock(mPidsSelfLock);
        app = mPidsSelfLocked[pid];
        assert(app != NULL);
    }
    else if (mStartingProcesses.GetSize() > 0) {
        app = mStartingProcesses.GetFront();
        mStartingProcesses.PopFront();
        app->mPid = pid;
    }
    else {
        app = NULL;
    }

    if (app == NULL) {
        String appApDes;
        appApartment->GetDescription(&appApDes);
        Slogger::W(TAG, StringBuffer("No pending application record for pid ") + pid
                + " (IApplicationApartment " + appApDes + "); dropping process");
//        EventLog.writeEvent(EventLogTags.AM_DROP_PROCESS, pid);
        if (pid > 0 && pid != mMyPid) {
            Process::KillProcess(pid);
        }
        else {
            appApartment->ScheduleExit();
        }
        return FALSE;
    }

    // If this application record is still attached to a previous
    // process, clean it up now.
    if (app->mAppApartment != NULL) {
        HandleAppDiedLocked(app, TRUE);
    }

    // Tell the process all about itself.

    if (localLOGV) {
        String appDes;
        app->GetDescription(&appDes);
        Slogger::V(TAG, StringBuffer("Binding process pid ") + pid
                + " to record " + appDes);
    }

    String processName = app->mProcessName;
//    try {
//        thread.asBinder().linkToDeath(new AppDeathRecipient(
//                app, pid, thread), 0);
//    } catch (RemoteException e) {
//        app.resetPackageList();
//        startProcessLocked(app, "link fail", processName);
//        return false;
//    }

//    EventLog.writeEvent(EventLogTags.AM_PROC_BOUND, app.pid, app.processName);

    app->mAppApartment = appApartment;
    app->mCurAdj = app->mSetAdj = -100;
    app->mCurSchedGroup = Process::THREAD_GROUP_DEFAULT;
    app->mSetSchedGroup = Process::THREAD_GROUP_BG_NONINTERACTIVE;
    app->mForcingToForeground = NULL;
    app->mForegroundServices = FALSE;
    app->mDebugging = FALSE;

//    mHandler.removeMessages(PROC_START_TIMEOUT_MSG, app);

    AutoPtr<IObjectContainer> providers;
    Boolean normalMode = mProcessesReady || IsAllowedWhileBooting(app->mInfo);
    if (normalMode) {
        GenerateApplicationProvidersLocked(app, (IObjectContainer**)&providers);
    }

    if (!normalMode) {
        String appDes;
        app->GetDescription(&appDes);
        Slogger::I(TAG, StringBuffer("Launching preboot mode app: ")
                + appDes);
    }

    if (localLOGV) {
        String appDes, appApDes;
        app->GetDescription(&appDes);
        appApartment->GetDescription(&appApDes);
        Slogger::V(TAG, StringBuffer("New app record ") + appDes
                + " apartment=" + appApDes + " pid=" + pid);
    }
//    try {
    Int32 testMode = ApplicationApartment_DEBUG_OFF;
    if (!mDebugApp.IsNull() && !mDebugApp.Compare(processName)) {
        testMode = mWaitForDebugger
            ? ApplicationApartment_DEBUG_WAIT
            : ApplicationApartment_DEBUG_ON;
        app->mDebugging = TRUE;
        if (mDebugTransient) {
            mDebugApp = mOrigDebugApp;
            mWaitForDebugger = mOrigWaitForDebugger;
        }
    }

    // If the app is being launched for restore or full backup, set it up specially
    Boolean isRestrictedBackupMode = FALSE;
//    if (mBackupTarget != null && mBackupAppName.equals(processName)) {
//        isRestrictedBackupMode = (mBackupTarget.backupMode == BackupRecord.RESTORE)
//                || (mBackupTarget.backupMode == BackupRecord.BACKUP_FULL);
//    }

//    ensurePackageDexOpt(app.instrumentationInfo != null
//            ? app.instrumentationInfo.packageName
//            : app.info.packageName);
//    if (app.instrumentationClass != null) {
//        ensurePackageDexOpt(app.instrumentationClass.getPackageName());
//    }
    if (DEBUG_CONFIGURATION) {
        String confDes;
        mConfiguration->GetDescription(&confDes);
        Slogger::V(TAG, StringBuffer("Binding proc ")
            + processName + " with config " + confDes);
    }
    AutoPtr<IObjectStringMap> services;
    GetCommonServicesLocked((IObjectStringMap**)&services);
    appApartment->BindApplication(processName, app->mInstrumentationInfo != NULL
            ? app->mInstrumentationInfo : app->mInfo, providers,
            app->mInstrumentationClass, app->mInstrumentationProfileFile,
            app->mInstrumentationArguments, app->mInstrumentationWatcher, testMode,
            isRestrictedBackupMode || !normalMode,
            mConfiguration, services);
    UpdateLRUProcessLocked(app, FALSE, TRUE);
    app->mLastRequestedGc = app->mLastLowMemory = SystemClock::GetUptimeMillis();
//    } catch (Exception e) {
//        // todo: Yikes!  What should we do?  For now we will try to
//        // start another process, but that could easily get us in
//        // an infinite loop of restarting processes...
//        Slogger::W(TAG, "Exception thrown during bind!", e);
//
//        app.resetPackageList();
//        startProcessLocked(app, "bind fail", processName);
//        return false;
//    }

    // Remove this record from the list of starting applications.
    mPersistentStartingProcesses.Remove(app);
    if (DEBUG_PROCESSES && (Find(mProcessesOnHold.Begin(),
            mProcessesOnHold.End(), app) != mProcessesOnHold.End())) {
        String appDes;
        app->GetDescription(&appDes);
        Slogger::V(TAG, StringBuffer("Attach application locked removing on hold: ")
                + appDes);
    }
    mProcessesOnHold.Remove(app);

    Boolean badApp = FALSE;
    Boolean didSomething = FALSE;

    // See if the top visible activity is waiting to run in this process...
    AutoPtr<CActivityRecord> hr = mMainStack->GetTopRunningActivityLocked(NULL);
    if (hr != NULL && normalMode) {
        AutoPtr<IApplicationInfo> hrAppInfo;
        hr->mInfo->GetApplicationInfo((IApplicationInfo**)&hrAppInfo);
        Int32 appUid, hrUid;
        app->mInfo->GetUid(&appUid);
        hrAppInfo->GetUid(&hrUid);
        if (hr->mApp == NULL && appUid == hrUid && !processName.Compare(hr->mProcessName)) {
//            try {
            Boolean succeeded;
            if (SUCCEEDED(mMainStack->RealStartActivityLocked(hr, app, TRUE, TRUE, &succeeded))
                    && succeeded) {
                didSomething = TRUE;
            }
//            } catch (Exception e) {
//                Slogger::W(TAG, "Exception in new application when starting activity "
//                      + hr.intent.getComponent().flattenToShortString(), e);
//                badApp = true;
//            }
        }
        else {
            mMainStack->EnsureActivitiesVisibleLocked(
                    (CActivityRecord*)hr, NULL, processName, 0);
        }
    }

    // Find any services that should be running in this process...
    if (!badApp && mPendingServices.GetSize() > 0) {
        AutoPtr<CServiceRecord> sr;
//        try {
        List<AutoPtr<CServiceRecord> >::Iterator it = mPendingServices.Begin();
        while (it != mPendingServices.End()) {
            sr = *it;
            Int32 appUid, srUid;
            app->mInfo->GetUid(&appUid);
            sr->mAppInfo->GetUid(&srUid);
            if (appUid != srUid || processName.Compare(sr->mProcessName)) {
                ++it;
                continue;
            }

            it = mPendingServices.Erase(it);
            RealStartServiceLocked(sr, app);
            didSomething = TRUE;
        }
//        } catch (Exception e) {
//            Slogger::W(TAG, "Exception in new application when starting service "
//                  + sr.shortName, e);
//            badApp = true;
//        }
    }

    // Check if the next broadcast receiver is in this process...
    BroadcastRecord* br = mPendingBroadcast;
    if (!badApp && br != NULL && br->mCurApp == app) {
//        try {
        mPendingBroadcast = NULL;
        ProcessCurBroadcastLocked(br, app);
        didSomething = TRUE;
//        } catch (Exception e) {
//            Slogger::W(TAG, "Exception in new application when starting receiver "
//                  + br.curComponent.flattenToShortString(), e);
//            badApp = true;
//            logBroadcastReceiverDiscardLocked(br);
//            finishReceiverLocked(br.receiver, br.resultCode, br.resultData,
//                    br.resultExtras, br.resultAbort, true);
//            scheduleBroadcastsLocked();
//            // We need to reset the state if we fails to start the receiver.
//            br.state = BroadcastRecord.IDLE;
//        }
    }

//    // Check whether the next backup agent is in this process...
//    if (!badApp && mBackupTarget != null && mBackupTarget.appInfo.uid == app.info.uid) {
//        if (DEBUG_BACKUP) Slog.v(TAG, "New app is backup target, launching agent for " + app);
//        ensurePackageDexOpt(mBackupTarget.appInfo.packageName);
//        try {
//            thread.scheduleCreateBackupAgent(mBackupTarget.appInfo, mBackupTarget.backupMode);
//        } catch (Exception e) {
//            Slogger::W(TAG, "Exception scheduling backup agent creation: ");
//            e.printStackTrace();
//        }
//    }

    if (badApp) {
        // todo: Also need to kill application to deal with all
        // kinds of exceptions.
        HandleAppDiedLocked(app, FALSE);
        return FALSE;
    }

    if (!didSomething) {
        UpdateOomAdjLocked();
    }

    return TRUE;
}

ECode CActivityManagerService::AttachApplication(
    /* [in] */ IApplicationApartment* appApartment,
    /* [in] */ Int32 pid)
{
    SelfLock();
    ECode ec = AttachApplicationLocked(appApartment, pid);
    SelfUnlock();

    return ec;
}

ECode CActivityManagerService::ActivityIdle(
    /* [in] */ IBinder* token,
    /* [in] */ IConfiguration* config)
{
    const Int64 origId = Binder::ClearCallingIdentity();
    mMainStack->ActivityIdleInternal(token, FALSE, config);
    Binder::RestoreCallingIdentity(origId);

    return NOERROR;
}

void CActivityManagerService::EnableScreenAfterBoot()
{
//    EventLog.writeEvent(EventLogTags.BOOT_PROGRESS_ENABLE_SCREEN,
//            SystemClock.uptimeMillis());
//    mWindowManager.enableScreenAfterBoot();
}

void CActivityManagerService::FinishBooting()
{
//    IntentFilter pkgFilter = new IntentFilter();
//    pkgFilter.addAction(Intent.ACTION_QUERY_CAPSULE_RESTART);
//    pkgFilter.addDataScheme("package");
//    mContext.registerReceiver(new BroadcastReceiver() {
//        @Override
//        public void onReceive(Context context, Intent intent) {
//            String[] pkgs = intent.getStringArrayExtra(Intent.EXTRA_CAPSULES);
//            if (pkgs != null) {
//                for (String pkg : pkgs) {
//                    if (forceStopPackageLocked(pkg, -1, false, false, false)) {
//                        setResultCode(Activity.RESULT_OK);
//                        return;
//                    }
//                }
//            }
//        }
//    }, pkgFilter);
//
//    synchronized (this) {
//        // Ensure that any processes we had put on hold are now started
//        // up.
//        final int NP = mProcessesOnHold.size();
//        if (NP > 0) {
//            ArrayList<ProcessRecord> procs =
//                new ArrayList<ProcessRecord>(mProcessesOnHold);
//            for (int ip=0; ip<NP; ip++) {
//                if (DEBUG_PROCESSES) Slog.v(TAG, "Starting process on hold: "
//                        + procs.get(ip));
//                startProcessLocked(procs.get(ip), "on-hold", null);
//            }
//        }
//
//        if (mFactoryTest != SystemServer.FACTORY_TEST_LOW_LEVEL) {
//            // Start looking for apps that are abusing wake locks.
//            Message nmsg = mHandler.obtainMessage(CHECK_EXCESSIVE_WAKE_LOCKS_MSG);
//            mHandler.sendMessageDelayed(nmsg, POWER_CHECK_DELAY);
//            // Tell anyone interested that we are done booting!
//            SystemProperties.set("sys.boot_completed", "1");
//            broadcastIntentLocked(null, null,
//                    new Intent(Intent.ACTION_BOOT_COMPLETED, null),
//                    null, null, 0, null, null,
//                    android.Manifest.permission.RECEIVE_BOOT_COMPLETED,
//                    false, false, MY_PID, Process.SYSTEM_UID);
//        }
//    }
}

void CActivityManagerService::EnsureBootCompleted()
{
//    boolean booting;
//    boolean enableScreen;
//    synchronized (this) {
//        booting = mBooting;
//        mBooting = false;
//        enableScreen = !mBooted;
//        mBooted = true;
//    }
//
//    if (booting) {
//        finishBooting();
//    }
//
//    if (enableScreen) {
//        enableScreenAfterBoot();
//    }
}

ECode CActivityManagerService::ActivityPaused(
    /* [in] */ IBinder* token,
    /* [in] */ IBundle* icicle)
{
    // Refuse possible leaked file descriptors
    Boolean hasFd;
    if (icicle != NULL && (icicle->HasFileDescriptors(&hasFd), hasFd)) {
        Slogger::E(TAG, "File descriptors passed in Bundle");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return mMainStack->ActivityPaused(token, icicle, FALSE);
}

ECode CActivityManagerService::ActivityStopped(
    /* [in] */ IBinder* token,
    /* [in] */ IBitmap* thumbnail, //Bitmap thumbnail
    /* [in] */ const String& description)
{
    if (localLOGV) {
        String tokenDes;
        token->GetDescription(&tokenDes);
        Slogger::V(TAG, StringBuffer("Activity stopped: token=") + tokenDes);
    }

    AutoPtr<CActivityRecord> r;

    SelfLock();
    Int32 index = mMainStack->GetIndexOfTokenLocked(token);
    if (index >= 0) {
        r = mMainStack->mHistory[index];
        r->mThumbnail = thumbnail;
//        r->mDescription = String::Duplicate(description);
        r->mStopped = TRUE;
        r->mState = ActivityState_STOPPED;
        if (!r->mFinishing) {
            if (r->mConfigDestroy) {
                r->mStack->DestroyActivityLocked(r, TRUE);
                r->mStack->ResumeTopActivityLocked(NULL);
            }
        }
    }
    SelfUnlock();

    if (r != NULL) {
        SendPendingThumbnail(r, NULL, NULL, NULL, FALSE);
    }

    TrimApplications();

    return NOERROR;
}

ECode CActivityManagerService::ActivityDestroyed(
    /* [in] */ IBinder* token)
{
    if (DEBUG_SWITCH) {
        String tokenDes;
        token->GetDescription(&tokenDes);
        Slogger::V(TAG, StringBuffer("ACTIVITY DESTROYED: ") + tokenDes);
    }
    mMainStack->ActivityDestroyed(token);

    return NOERROR;
}

ECode CActivityManagerService::GetCallingPackage(
    /* [in] */ IBinder* token,
    /* [out] */ String* capsule)
{
//    synchronized (this) {
//        ActivityRecord r = getCallingRecordLocked(token);
//        return r != null && r.app != null ? r.info.packageName : null;
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::GetCallingActivity(
    /* [in] */ IBinder* token,
    /* [out] */ IComponentName** activity)
{
    VALIDATE_NOT_NULL(activity);

    Mutex::Autolock lock(_m_syncLock);

    AutoPtr<CActivityRecord> r = GetCallingRecordLocked(token);
    if (r != NULL){
        return r->mIntent->GetComponent(activity);
    }
    else {
        *activity = NULL;
        return NOERROR;
    }
}

AutoPtr<CActivityRecord> CActivityManagerService::GetCallingRecordLocked(
    /* [in] */ IBinder* token)
{
    Int32 index = mMainStack->GetIndexOfTokenLocked(token);
    if (index >= 0) {
        AutoPtr<CActivityRecord> r = mMainStack->mHistory[index];
        if (r != NULL) {
            return r->mResultTo;
        }
    }
    return NULL;
}

ECode CActivityManagerService::GetActivityClassForToken(
    /* [in] */ IBinder* token,
    /* [out] */ IComponentName** activityCls)
{
    VALIDATE_NOT_NULL(activityCls);
    Mutex::Autolock lock(_m_syncLock);

    Int32 index = mMainStack->GetIndexOfTokenLocked(token);
    if (index >= 0) {
        AutoPtr<CActivityRecord> r = mMainStack->mHistory[index];
        r->mIntent->GetComponent(activityCls);

    }
    *activityCls = NULL;
    return NOERROR;
}

ECode CActivityManagerService::GetCapsuleForToken(
    /* [in] */ IBinder* token,
    /* [out] */ String* capsule)
{
//    synchronized(this) {
//        int index = mMainStack.indexOfTokenLocked(token);
//        if (index >= 0) {
//            ActivityRecord r = (ActivityRecord)mMainStack.mHistory.get(index);
//            return r.packageName;
//        }
//        return null;
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::GetIntentSender(
    /* [in] */ Int32 type,
    /* [in] */ const String& capsuleName,
    /* [in] */ IBinder* token,
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [out] */ IIntentSender** sender)
{
//    // Refuse possible leaked file descriptors
//    if (intent != null && intent.hasFileDescriptors() == true) {
//        throw new IllegalArgumentException("File descriptors passed in Intent");
//    }
//
//    if (type == INTENT_SENDER_BROADCAST) {
//        if ((intent.getFlags()&Intent.FLAG_RECEIVER_BOOT_UPGRADE) != 0) {
//            throw new IllegalArgumentException(
//                    "Can't use FLAG_RECEIVER_BOOT_UPGRADE here");
//        }
//    }
//
//    synchronized(this) {
//        int callingUid = Binder.getCallingUid();
//        try {
//            if (callingUid != 0 && callingUid != Process.SYSTEM_UID &&
//                    Process.supportsProcesses()) {
//                int uid = AppGlobals.getPackageManager()
//                        .getPackageUid(packageName);
//                if (uid != Binder.getCallingUid()) {
//                    String msg = "Permission Denial: getIntentSender() from pid="
//                        + Binder.getCallingPid()
//                        + ", uid=" + Binder.getCallingUid()
//                        + ", (need uid=" + uid + ")"
//                        + " is not allowed to send as package " + packageName;
//                    Slogger::W(TAG, msg);
//                    throw new SecurityException(msg);
//                }
//            }
//
//            return getIntentSenderLocked(type, packageName, callingUid,
//                    token, resultWho, requestCode, intent, resolvedType, flags);
//
//        } catch (RemoteException e) {
//            throw new SecurityException(e);
//        }
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::GetIntentSenderLocked(
    /* [in] */ Int32 type,
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 callingUid,
    /* [in] */ IBinder* token,
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [out] */ IIntentSender** sender)
{
    AutoPtr<CActivityRecord> activity;
    if (type == ActivityManager_INTENT_SENDER_ACTIVITY_RESULT) {
        Int32 index = mMainStack->GetIndexOfTokenLocked(token);
        if (index < 0) {
            *sender = NULL;
            return NOERROR;
        }
        activity = (CActivityRecord*)mMainStack->mHistory[index];
        if (activity->mFinishing) {
            *sender = NULL;
            return NOERROR;
        }
    }

    Boolean noCreate = (flags & PendingIntent_FLAG_NO_CREATE) != 0;
    Boolean cancelCurrent = (flags & PendingIntent_FLAG_CANCEL_CURRENT) != 0;
    Boolean updateCurrent = (flags & PendingIntent_FLAG_UPDATE_CURRENT) != 0;
    flags &= ~(PendingIntent_FLAG_NO_CREATE | PendingIntent_FLAG_CANCEL_CURRENT
            | PendingIntent_FLAG_UPDATE_CURRENT);

    Key* key = new Key(type, capsuleName, activity, resultWho,
            requestCode, intent, resolvedType, flags);
    HashMap<Key*, CPendingIntentRecord*>::Iterator it = \
            mIntentSenderRecords.Find(key);
    AutoPtr<CPendingIntentRecord> rec = (it != mIntentSenderRecords.End()) ? it->mSecond : NULL;
    if (rec != NULL) {
        if (!cancelCurrent) {
            if (updateCurrent) {
                rec->mKey->mRequestIntent->ReplaceExtras(intent);
            }
            *sender = rec;
            if (*sender != NULL) (*sender)->AddRef();
            return NOERROR;
        }
        rec->mCanceled = TRUE;
        mIntentSenderRecords.Erase(key);
    }
    if (noCreate) {
        *sender = rec;
        if (*sender != NULL) (*sender)->AddRef();
        return NOERROR;
    }
    CPendingIntentRecord::NewByFriend((CPendingIntentRecord**)&rec);
    rec->Initialize(this, key, callingUid);
    mIntentSenderRecords[key] = rec.Get();
    if (type == ActivityManager_INTENT_SENDER_ACTIVITY_RESULT) {
        if (activity->mPendingResults == NULL) {
            activity->mPendingResults = new Set<CPendingIntentRecord*>();
        }
        activity->mPendingResults->Insert(rec.Get());
    }
    *sender = rec;
    if (*sender != NULL) (*sender)->AddRef();
    return NOERROR;
}

ECode CActivityManagerService::CancelIntentSender(
    /* [in] */ IIntentSender* sender)
{
//    if (!(sender instanceof PendingIntentRecord)) {
//        return;
//    }
//    synchronized(this) {
//        PendingIntentRecord rec = (PendingIntentRecord)sender;
//        try {
//            int uid = AppGlobals.getPackageManager()
//                    .getPackageUid(rec.key.packageName);
//            if (uid != Binder.getCallingUid()) {
//                String msg = "Permission Denial: cancelIntentSender() from pid="
//                    + Binder.getCallingPid()
//                    + ", uid=" + Binder.getCallingUid()
//                    + " is not allowed to cancel packges "
//                    + rec.key.packageName;
//                Slogger::W(TAG, msg);
//                throw new SecurityException(msg);
//            }
//        } catch (RemoteException e) {
//            throw new SecurityException(e);
//        }
//        cancelIntentSenderLocked(rec, true);
//    }
    return E_NOT_IMPLEMENTED;
}

void CActivityManagerService::CancelIntentSenderLocked(
    /* [in] */ CPendingIntentRecord* rec,
    /* [in] */ Boolean cleanActivity)
{
    rec->mCanceled = TRUE;
    mIntentSenderRecords.Erase(rec->mKey);
    if (cleanActivity && rec->mKey->mActivity != NULL) {
        rec->mKey->mActivity->mPendingResults->Erase(rec);
    }
}

ECode CActivityManagerService::GetCapsuleForIntentSender(
    /* [in] */ IIntentSender* pendingResult,
    /* [out] */ String* capsule)
{
//    if (!(pendingResult instanceof PendingIntentRecord)) {
//        return null;
//    }
//    try {
//        PendingIntentRecord res = (PendingIntentRecord)pendingResult;
//        return res.key.packageName;
//    } catch (ClassCastException e) {
//    }
//    return null;
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::SetProcessLimit(
    /* [in] */ Int32 max)
{
    ECode ec = EnforceCallingPermission("android.Manifest.permission.SET_PROCESS_LIMIT",
            "setProcessLimit()");
    mProcessLimit = max;
    return ec;
}

ECode CActivityManagerService::GetProcessLimit(
    /* [out] */ Int32* limit)
{
//    return mProcessLimit;
    return E_NOT_IMPLEMENTED;
}

void CActivityManagerService::ForegroundTokenDied(
    /* [in] */ /*ForegroundToken token*/)
{
//    synchronized (ActivityManagerService.this) {
//        synchronized (mPidsSelfLocked) {
//            ForegroundToken cur
//                = mForegroundProcesses.get(token.pid);
//            if (cur != token) {
//                return;
//            }
//            mForegroundProcesses.remove(token.pid);
//            ProcessRecord pr = mPidsSelfLocked.get(token.pid);
//            if (pr == null) {
//                return;
//            }
//            pr.forcingToForeground = null;
//            pr.foregroundServices = false;
//        }
//        updateOomAdjLocked();
//    }
}

ECode CActivityManagerService::SetProcessForeground(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 pid,
    /* [in] */ Boolean isForeground)
{
    EnforceCallingPermission("android.Manifest.permission.SET_PROCESS_LIMIT",
            "setProcessForeground()");
//    synchronized(this) {
//        boolean changed = false;
//
//        synchronized (mPidsSelfLocked) {
//            ProcessRecord pr = mPidsSelfLocked.get(pid);
//            if (pr == null) {
//                Slogger::W(TAG, "setProcessForeground called on unknown pid: " + pid);
//                return;
//            }
//            ForegroundToken oldToken = mForegroundProcesses.get(pid);
//            if (oldToken != null) {
//                oldToken.token.unlinkToDeath(oldToken, 0);
//                mForegroundProcesses.remove(pid);
//                pr.forcingToForeground = null;
//                changed = true;
//            }
//            if (isForeground && token != null) {
//                ForegroundToken newToken = new ForegroundToken() {
//                    public void binderDied() {
//                        foregroundTokenDied(this);
//                    }
//                };
//                newToken.pid = pid;
//                newToken.token = token;
//                try {
//                    token.linkToDeath(newToken, 0);
//                    mForegroundProcesses.put(pid, newToken);
//                    pr.forcingToForeground = token;
//                    changed = true;
//                } catch (RemoteException e) {
//                    // If the process died while doing this, we will later
//                    // do the cleanup with the process death link.
//                }
//            }
//        }
//
//        if (changed) {
//            updateOomAdjLocked();
//        }
//    }
    return E_NOT_IMPLEMENTED;
}

// =========================================================
// PERMISSIONS
// =========================================================

/**
 * This can be called with or without the global lock held.
 */
Int32 CActivityManagerService::CheckComponentPermission(
    /* [in] */ CString permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 reqUid)
{
    // We might be performing an operation on behalf of an indirect binder
    // invocation, e.g. via {@link #openContentUri}.  Check and adjust the
    // client identity accordingly before proceeding.
    Identity* tlsIdentity = NULL; /* sCallerIdentity.get(); */
    if (tlsIdentity != NULL) {
        Slogger::D(TAG, StringBuffer("checkComponentPermission() adjusting {pid,uid} to {")
                + tlsIdentity->mPid + "," + tlsIdentity->mUid + "}");
        uid = tlsIdentity->mUid;
        pid = tlsIdentity->mPid;
    }

    // Root, system server and our own process get to do everything.
    if (uid == 0 || uid == Process::SYSTEM_UID || pid == mMyPid ||
        !Process::SupportsProcesses()) {
        return CapsuleManager_PERMISSION_GRANTED;
    }
    // If the target requires a specific UID, always fail for others.
    if (reqUid >= 0 && uid != reqUid) {
        Slogger::W(TAG, StringBuffer("Permission denied: checkComponentPermission() reqUid=")
                + reqUid);
        return CapsuleManager_PERMISSION_DENIED;
    }
    if (permission.IsNull()) {
        return CapsuleManager_PERMISSION_GRANTED;
    }

    Int32 perm;
    if (SUCCEEDED(GetCapsuleManager()->CheckUidPermission(permission, uid, &perm))) {
        return perm;
    }
    else {
        // Should never happen, but if it does... deny!
        Slogger::E(TAG, "CapsuleManager is dead?!?");
    }
    return CapsuleManager_PERMISSION_DENIED;
}

/**
 * As the only public entry point for permissions checking, this method
 * can enforce the semantic that requesting a check on a null global
 * permission is automatically denied.  (Internally a null permission
 * string is used when calling {@link #checkComponentPermission} in cases
 * when only uid-based security is needed.)
 *
 * This can be called with or without the global lock held.
 */
ECode CActivityManagerService::CheckPermission(
    /* [in] */ CString permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [out] */ Int32* result)
{
    if (permission.IsNull()) {
        *result = CapsuleManager_PERMISSION_GRANTED;
        return NOERROR;
    }

    *result = CheckComponentPermission(permission, pid, uid, -1);
    return NOERROR;
}

/**
 * Binder IPC calls go through the public entry point.
 * This can be called with or without the global lock held.
 */
Int32 CActivityManagerService::CheckCallingPermission(
    /* [in] */ CString permission)
{
    Int32 result;
    CheckPermission(permission,
            Binder::GetCallingPid(),
            Binder::GetCallingUid(),
            &result);
    return result;
}

/**
 * This can be called with or without the global lock held.
 */
ECode CActivityManagerService::EnforceCallingPermission(
    /* [in] */ CString permission,
    /* [in] */ CString func)
{
    if (CheckCallingPermission(permission)
            == CapsuleManager_PERMISSION_GRANTED) {
        return NOERROR;
    }

    Slogger::W(TAG, StringBuffer("Permission Denial: ") + func
                + " from pid=" + Binder::GetCallingPid()
                + ", uid=" + Binder::GetCallingUid()
                + " requires " + permission);
    return E_SECURITY_EXCEPTION;
}

Boolean CActivityManagerService::CheckHoldingPermissionsLocked(
    /* [in] */ ICapsuleManager* pm,
    /* [in] */ IContentProviderInfo* pi,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags)
{
    Boolean readPerm = (modeFlags & Intent_FLAG_GRANT_READ_URI_PERMISSION) == 0;
    Boolean writePerm = (modeFlags & Intent_FLAG_GRANT_WRITE_URI_PERMISSION) == 0;
    if (DEBUG_URI_PERMISSION) {
        String uriDes;
        uri->GetDescription(&uriDes);
        Slogger::V(TAG, StringBuffer("checkHoldingPermissionsLocked: uri=")
                + uriDes + " uid=" + uid);
    }
//    try {
    // Is the component private from the target uid?
    Boolean piExported;
    pi->IsExported(&piExported);
    AutoPtr<IApplicationInfo> piAppInfo;
    pi->GetApplicationInfo((IApplicationInfo**)&piAppInfo);
    Int32 piUid;
    piAppInfo->GetUid(&piUid);

    Boolean prv = !piExported && piUid != uid;

    // Acceptable if the there is no read permission needed from the
    // target or the target is holding the read permission.
    if (!readPerm) {
        String piReadPermission;
        pi->GetReadPermission(&piReadPermission);
        if (!prv && piReadPermission.IsNull()) {
            readPerm = TRUE;
        }

        Int32 perm;
        assert(SUCCEEDED(pm->CheckUidPermission(piReadPermission, uid, &perm)));
        if (perm == CapsuleManager_PERMISSION_GRANTED) {
            readPerm = TRUE;
        }
    }

    // Acceptable if the there is no write permission needed from the
    // target or the target is holding the read permission.
    if (!writePerm) {
        String piWritePermission;
        pi->GetWritePermission(&piWritePermission);
        if (!prv && piWritePermission.IsNull()) {
            writePerm = TRUE;
        }

        Int32 perm;
        assert(SUCCEEDED(pm->CheckUidPermission(piWritePermission, uid, &perm)));
        if (perm == CapsuleManager_PERMISSION_GRANTED) {
            writePerm = TRUE;
        }
    }

    // Acceptable if there is a path permission matching the URI that
    // the target holds the permission on.
    AutoPtr<IObjectContainer> pps;
    pi->GetPathPermissions((IObjectContainer**)&pps);
    if (pps != NULL && (!readPerm || !writePerm)) {
        String path;
        uri->GetPath(&path);
        AutoPtr<IObjectEnumerator> en;
        pps->GetObjectEnumerator((IObjectEnumerator**)&en);
        Boolean hasNext = FALSE;
        AutoPtr<IPathPermission> pp;
        while (en->MoveNext(&hasNext), hasNext && (!readPerm || !writePerm)) {
            en->Current((IInterface**)&pp);
            if (!readPerm) {
                Int32 perm;
                String pprperm;
                pp->GetReadPermission(&pprperm);
                assert(SUCCEEDED(pm->CheckUidPermission(pprperm, uid, &perm)));

                Boolean isMatch;
                pp->Match(path, &isMatch);
                if (DEBUG_URI_PERMISSION) {
                    String pathLog;
                    pp->GetPath(&pathLog);
                    Slogger::V(TAG, StringBuffer("Checking read perm for ")
                        + pprperm + " for " + pathLog
                        + ": match=" + isMatch
                        + " check=" + perm);
                }
                if (!pprperm.IsNull() && isMatch &&
                        (perm == CapsuleManager_PERMISSION_GRANTED)) {
                    readPerm = TRUE;
                }
            }
            if (!writePerm) {
                Int32 perm;
                String ppwperm;
                pp->GetWritePermission(&ppwperm);
                assert(SUCCEEDED(pm->CheckUidPermission(ppwperm, uid, &perm)));

                Boolean isMatch;
                pp->Match(path, &isMatch);
                if (DEBUG_URI_PERMISSION) {
                    String pathLog;
                    pp->GetPath(&pathLog);
                    Slogger::V(TAG, StringBuffer("Checking write perm ")
                        + ppwperm + " for " + pathLog
                        + ": match=" + isMatch
                        + " check=" + perm);
                }
                if (!ppwperm.IsNull() && isMatch &&
                        (perm == CapsuleManager_PERMISSION_GRANTED)) {
                    writePerm = TRUE;
                }
            }
        }
    }
    // } catch (RemoteException e) {
    //     return false;
    // }

    return readPerm && writePerm;
}

Boolean CActivityManagerService::CheckUriPermissionLocked(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags)
{
    // Root gets to do everything.
    if (uid == 0 || !Process::SupportsProcesses()) {
        return TRUE;
    }

    Map<Int32, HashMap<IUri*, UriPermission*, Hash<IUri*> >*>::Iterator it = \
            mGrantedUriPermissions.Find(uid);
    if (it == mGrantedUriPermissions.End()) return FALSE;

    HashMap<IUri*, UriPermission*, Hash<IUri*> >* perms = it->mSecond;
    HashMap<IUri*, UriPermission*, Hash<IUri*> >::Iterator uit = perms->Find(uri);
    if (uit == perms->End()) return FALSE;
    UriPermission* perm = uit->mSecond;
    return (modeFlags & perm->mModeFlags) == modeFlags;
}

ECode CActivityManagerService::CheckUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* permission)
{
    // Another redirected-binder-call permissions check as in
    // {@link checkComponentPermission}.
    //TODO:
    Identity* tlsIdentity = NULL;//sCallerIdentity.get();
    if (tlsIdentity != NULL) {
       uid = tlsIdentity->mUid;
       pid = tlsIdentity->mPid;
    }

    // Our own process gets to do everything.
    if (pid == Binder::GetCallingPid()) {
        *permission = CapsuleManager_PERMISSION_GRANTED;
        return CapsuleManager_PERMISSION_GRANTED;
    }

    Mutex::Autolock lock(_m_syncLock);
    return CheckUriPermissionLocked(uri, uid, modeFlags)
           ? CapsuleManager_PERMISSION_GRANTED
           : CapsuleManager_PERMISSION_DENIED;
}

/**
 * Check if the targetPkg can be granted permission to access uri by
 * the callingUid using the given modeFlags.  Throws a security exception
 * if callingUid is not allowed to do this.  Returns the uid of the target
 * if the URI permission grant should be performed; returns -1 if it is not
 * needed (for example targetPkg already has permission to access the URI).
 */
ECode CActivityManagerService::CheckGrantUriPermissionLocked(
    /* [in] */ Int32 callingUid,
    /* [in] */ const String& targetCap,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* targetUid)
{
    modeFlags &= (Intent_FLAG_GRANT_READ_URI_PERMISSION
            | Intent_FLAG_GRANT_WRITE_URI_PERMISSION);
    if (modeFlags == 0) {
        *targetUid = -1;
        return NOERROR;
    }

    if (DEBUG_URI_PERMISSION) {
        String uriDes;
        uri->GetDescription(&uriDes);
        Slogger::V(TAG, StringBuffer("Checking grant ") + targetCap
                + " permission to " + uriDes);
    }

    AutoPtr<ICapsuleManager> pm = GetCapsuleManager();

    // If this is not a content: uri, we can't do anything with it.
    String scheme;
    uri->GetScheme(&scheme);
    if (!scheme.Equals(ContentResolver::SCHEME_CONTENT)) {
        if (DEBUG_URI_PERMISSION) {
            String uriDes;
            uri->GetDescription(&uriDes);
            Slogger::V(TAG, StringBuffer("Can't grant URI permission for non-content URI: ")
                    + uriDes);
        }
        *targetUid = -1;
        return NOERROR;
    }

    String name;
    uri->GetAuthority(&name);
    AutoPtr<IContentProviderInfo> pi;
    ContentProviderRecord* cpr = mProvidersByName.Find(name)->mSecond;
    if (cpr != NULL) {
        cpr->mHolder->GetContentProviderInfo((IContentProviderInfo**)&pi);
    }
    else {
        pm->ResolveContentProvider(name,
                CapsuleManager_GET_URI_PERMISSION_PATTERNS,
                (IContentProviderInfo**)&pi);
    }
    if (pi == NULL) {
        Slogger::W(TAG, StringBuffer("No content provider found for: ") + name);
        *targetUid = -1;
        return NOERROR;
    }

    if (SUCCEEDED(pm->GetCapsuleUid(targetCap, targetUid))) {
        if (*targetUid < 0) {
            if (DEBUG_URI_PERMISSION) {
                Slogger::V(TAG, StringBuffer("Can't grant URI permission no uid for: ")
                        + targetCap);
            }
            return NOERROR;
        }
    }
    else {
        *targetUid = -1;
        return NOERROR;
    }

    // First...  does the target actually need this permission?
    if (CheckHoldingPermissionsLocked(pm, pi, uri, *targetUid, modeFlags)) {
        // No need to grant the target this permission.
        if (DEBUG_URI_PERMISSION) {
            String uriDes;
            uri->GetDescription(&uriDes);
            Slogger::V(TAG, StringBuffer("Target ") + targetCap
                    + " already has full permission to " + uriDes);
        }
        *targetUid = -1;
        return NOERROR;
    }

    // Second...  is the provider allowing granting of URI permissions?
    Boolean grant;
    pi->GetGrantUriPermissions(&grant);
    if (!grant) {
        String uriDes, piCName, piName;
        uri->GetDescription(&uriDes);
        pi->GetCapsuleName(&piCName);
        pi->GetName(&piName);
        Slogger::E(TAG, StringBuffer("Provider ") + piCName
               + "/" + piName
               + " does not allow granting of Uri permissions (uri "
               + uriDes + ")");
        return E_SECURITY_EXCEPTION;
    }

    AutoPtr<IObjectContainer> patterns = NULL;
    pi->GetUriPermissionPatterns((IObjectContainer**)&patterns);
    if (patterns != NULL) {
        Boolean allowed = FALSE;
        String path;
        uri->GetPath(&path);
        AutoPtr<IObjectEnumerator> en;
        patterns->GetObjectEnumerator((IObjectEnumerator**)&en);
        Boolean isMatch = FALSE;
        Boolean hasNext;
        AutoPtr<IPatternMatcher> it;
        while (en->MoveNext(&hasNext), hasNext) {
            en->Current((IInterface**)&it);
            it->Match(path, &isMatch);
            if (isMatch) {
                allowed = TRUE;
                break;
            }
        }

        if (!allowed) {
            String uriDes, piCName, piName;
            uri->GetDescription(&uriDes);
            pi->GetCapsuleName(&piCName);
            pi->GetName(&piName);
            Slogger::E(TAG, StringBuffer("Provider ") + piCName
                    + "/" + piName
                    + " does not allow granting of permission to path of Uri "
                    + uriDes);
            return E_SECURITY_EXCEPTION;
        }
    }

    // Third...  does the caller itself have permission to access
    // this uri?
    if (!CheckHoldingPermissionsLocked(pm, pi, uri, callingUid, modeFlags)) {
        if (!CheckUriPermissionLocked(uri, callingUid, modeFlags)) {
            String uriDes;
            uri->GetDescription(&uriDes);
            Slogger::E(TAG, StringBuffer("Uid ") + callingUid
                    + " does not have permission to uri " + uriDes);
            return E_SECURITY_EXCEPTION;
        }
    }

    return NOERROR;
}

void CActivityManagerService::GrantUriPermissionUncheckedLocked(
    /* [in] */ Int32 targetUid,
    /* [in] */ const String& targetCap,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [in] */ UriPermissionOwner* owner)
{
    modeFlags &= (Intent_FLAG_GRANT_READ_URI_PERMISSION
            | Intent_FLAG_GRANT_WRITE_URI_PERMISSION);
    if (modeFlags == 0) {
        return;
    }

    // So here we are: the caller has the assumed permission
    // to the uri, and the target doesn't.  Let's now give this to
    // the target.

    if (DEBUG_URI_PERMISSION) {
        String uriDes;
        uri->GetDescription(&uriDes);
        Slogger::V(TAG, StringBuffer("Granting ") + targetCap + "/"
                + targetUid + " permission to " + uriDes);
    }

    HashMap<IUri*, UriPermission*>* targetUris;
    Map<Int32, HashMap<IUri*, UriPermission*>*>::Iterator it = \
            mGrantedUriPermissions.Find(targetUid);
    if (it == mGrantedUriPermissions.End()) {
        targetUris = new HashMap<IUri*, UriPermission*>(5);
        mGrantedUriPermissions[targetUid] = targetUris;
    }

    UriPermission* perm;
    HashMap<IUri*, UriPermission*>::Iterator uit = targetUris->Find(uri);
    if (uit == targetUris->End()) {
        perm = new UriPermission(targetUid, uri);
        (*targetUris)[uri] = perm;
    }

    perm->mModeFlags |= modeFlags;
    if (owner == NULL) {
        perm->mGlobalModeFlags |= modeFlags;
    }
    else if ((modeFlags & Intent_FLAG_GRANT_READ_URI_PERMISSION) != 0) {
        perm->mReadOwners.Insert(owner);
        owner->AddReadPermission(perm);
    }
    else if ((modeFlags & Intent_FLAG_GRANT_WRITE_URI_PERMISSION) != 0) {
        perm->mWriteOwners.Insert(owner);
        owner->AddWritePermission(perm);
    }
}

void CActivityManagerService::GrantUriPermissionLocked(
    /* [in] */ Int32 callingUid,
    /* [in] */ const String& targetCap,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [in] */ UriPermissionOwner* owner)
{
    Int32 targetUid;
    CheckGrantUriPermissionLocked(callingUid, targetCap, uri, modeFlags, &targetUid);
    if (targetUid < 0) {
        return;
    }

    GrantUriPermissionUncheckedLocked(targetUid, targetCap, uri, modeFlags, owner);
}

/**
 * Like checkGrantUriPermissionLocked, but takes an Intent.
 */
Int32 CActivityManagerService::CheckGrantUriPermissionFromIntentLocked(
    /* [in] */ Int32 callingUid,
    /* [in] */ const String& targetCap,
    /* [in] */ IIntent* intent)
{
    if (DEBUG_URI_PERMISSION) {
        AutoPtr<IUri> uri;
        String intDes, uriDes;
        Int32 flags;
        if (intent != NULL) {
            intent->GetDescription(&intDes);
            intent->GetFlags(&flags);
            intent->GetData((IUri**)&uri);
            uri->GetDescription(&uriDes);
        }
        Slogger::V(TAG, StringBuffer("Checking URI perm to ") +
                (intent != NULL ? uriDes : String("NULL")) + " from " +
                (intent != NULL ? intDes : String("NULL")) + "; flags=0x"
                + (intent != NULL ? flags : 0));
    }

    if (intent == NULL) {
        return -1;
    }
    AutoPtr<IUri> data;
    intent->GetData((IUri**)&data);
    if (data == NULL) {
        return -1;
    }
    Int32 flags;
    intent->GetFlags(&flags);

    Int32 targetUid;
    CheckGrantUriPermissionLocked(callingUid, targetCap, data, flags, &targetUid);

    return targetUid;
}

/**
 * Like grantUriPermissionUncheckedLocked, but takes an Intent.
 */
void CActivityManagerService::GrantUriPermissionUncheckedFromIntentLocked(
    /* [in] */ Int32 targetUid,
    /* [in] */ const String& targetPkg,
    /* [in] */ IIntent* intent,
    /* [in] */ UriPermissionOwner* owner)
{
    AutoPtr<IUri> uri;
    intent->GetData((IUri**)&uri);

    Int32 flags;
    intent->GetFlags(&flags);

    GrantUriPermissionUncheckedLocked(targetUid, targetPkg, uri, flags, owner);
}

void CActivityManagerService::GrantUriPermissionFromIntentLocked(
    /* [in] */ Int32 callingUid,
    /* [in] */ const String& targetCap,
    /* [in] */ IIntent* intent,
    /* [in] */ UriPermissionOwner* owner)
{
    Int32 targetUid = CheckGrantUriPermissionFromIntentLocked(callingUid, targetCap, intent);
    if (targetUid < 0) {
        return;
    }

    GrantUriPermissionUncheckedFromIntentLocked(targetUid, targetCap, intent, owner);
}

Int32 CActivityManagerService::GetLRURecordIndexForAppLocked(
    /* [in] */ IApplicationApartment* thread)
{
    //TODO:
    // IBinder threadBinder = thread.asBinder();

    // // Find the application record.
    // for (int i=mLruProcesses.size()-1; i>=0; i--) {
    //     ProcessRecord rec = mLruProcesses.get(i);
    //     if (rec.thread != null && rec.thread.asBinder() == threadBinder) {
    //         return i;
    //     }
    // }
    return -1;
}

ProcessRecord* CActivityManagerService::GetRecordForAppLocked(
    /* [in] */ IApplicationApartment* thread)
{
    if (thread == NULL) {
        return NULL;
    }

    Int32 appIndex = GetLRURecordIndexForAppLocked(thread);
    return appIndex >= 0 ? mLRUProcesses[appIndex] : NULL;
}

ECode CActivityManagerService::GrantUriPermission(
    /* [in] */ IApplicationApartment* caller,
    /* [in] */ const String& targetPkg,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    Mutex::Autolock lock(_m_syncLock);

    ProcessRecord* r = GetRecordForAppLocked(caller);
    if (r == NULL) {
        // throw new SecurityException("Unable to find app for caller "
           // + caller
           // + " when granting permission to uri " + uri);
        return E_SECURITY_EXCEPTION;
    }
    if (targetPkg.IsNull()) {
        // throw new IllegalArgumentException("null target");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (uri == NULL) {
        // throw new IllegalArgumentException("null uri");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 uid;
    r->mInfo->GetUid(&uid);
    GrantUriPermissionLocked(uid, targetPkg, uri, modeFlags, NULL);
    return NOERROR;
}

void CActivityManagerService::RemoveUriPermissionIfNeededLocked(
    /* [in] */ UriPermission* perm)
{
    if ((perm->mModeFlags & (Intent_FLAG_GRANT_READ_URI_PERMISSION
           | Intent_FLAG_GRANT_WRITE_URI_PERMISSION)) == 0) {
        HashMap<IUri*, UriPermission*>* perms
                = mGrantedUriPermissions[perm->mUid];
        if (perms != NULL) {
            if (DEBUG_URI_PERMISSION) {
                Slogger::V(TAG,
                    StringBuffer("Removing ") + perm->mUid + " permission to " + perm->mUri);
            }
            perms->Erase(perm->mUri);
            if (perms->Begin() == perms->End()) {
                mGrantedUriPermissions.Erase(perm->mUid);
            }
        }
    }
}

void CActivityManagerService::RevokeUriPermissionLocked(
    /* [in] */ Int32 callingUid,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    modeFlags &= (Intent_FLAG_GRANT_READ_URI_PERMISSION
           | Intent_FLAG_GRANT_WRITE_URI_PERMISSION);
    if (modeFlags == 0) {
       return;
    }

    if (DEBUG_URI_PERMISSION) {
        String str;
        uri->ToString(&str);
        Slogger::V(TAG, "Revoking all granted permissions to " + str);
    }

    AutoPtr<ICapsuleManager> pm = GetCapsuleManager();
    String authority;
    uri->GetAuthority(&authority);
    AutoPtr<IContentProviderInfo> pi;
    ContentProviderRecord* cpr = mProvidersByName.Find(authority)->mSecond;
    if (cpr != NULL) {
        cpr->mHolder->GetContentProviderInfo((IContentProviderInfo**)&pi);
    }
    else {
        // try {
        pm->ResolveContentProvider(authority,
            CapsuleManager_GET_URI_PERMISSION_PATTERNS, (IContentProviderInfo**)&pi);
        // }
        // catch (RemoteException ex) {
        // }
    }
    if (pi == NULL) {
        Slogger::W(TAG, "No content provider found for: " + authority);
        return;
    }

    // Does the caller have this permission on the URI?
    if (!CheckHoldingPermissionsLocked(pm, pi, uri, callingUid, modeFlags)) {
       // Right now, if you are not the original owner of the permission,
       // you are not allowed to revoke it.
       //if (!checkUriPermissionLocked(uri, callingUid, modeFlags)) {
        // throw new SecurityException("Uid " + callingUid
        //         + " does not have permission to uri " + uri);
        return;
       //}
    }

    // Go through all of the permissions and remove any that match.
    ArrayOf<String>* SEGMENTS;
    uri->GetPathSegments(&SEGMENTS);
    if (SEGMENTS != NULL) {
        Int32 NS = SEGMENTS->GetLength();
        Int32 N = mGrantedUriPermissions.GetSize();
        for (Int32 i=0; i<N; i++) {
            HashMap<IUri*, UriPermission*>* perms
                   = mGrantedUriPermissions.Find(i)->mSecond;
            HashMap<IUri*, UriPermission*>::Iterator it = perms->Begin();
        toploop:
            while (it != perms->End()) {
                UriPermission* perm = it->mSecond;
                IUri* targetUri = perm->mUri;
                String targetAuthority;
                targetUri->GetAuthority(&targetAuthority);
                if (authority != targetAuthority) {
                    it++;
                    continue;
                }
                ArrayOf<String>* targetSegments;
                targetUri->GetPathSegments(&targetSegments);
                if (targetSegments->GetPayload() == NULL) {
                    it++;
                    continue;
                }
                if (targetSegments->GetLength() < NS) {
                    it++;
                    continue;
                }
                for (Int32 j=0; j<NS; j++) {
                    if (SEGMENTS[j] != targetSegments[j]) {
                        it++;
                        goto toploop;
                    }
                }
                if (DEBUG_URI_PERMISSION) {
                    // Slogger::v(TAG,
                    //     "Revoking " + perm.uid + " permission to " + perm.uri);
                }
                perm->ClearModes(modeFlags);
                if (perm->mModeFlags == 0) {
                    HashMap<IUri*, UriPermission*>::Iterator itBackUp = it;
                    it++;
                    perms->Erase(itBackUp);
                }
            }
            if (perms->GetSize() == 0) {
                mGrantedUriPermissions.Erase(i);
                N--;
                i--;
            }
        }
    }
}

ECode CActivityManagerService::RevokeUriPermission(
    /* [in] */ IApplicationApartment* caller,
    /* [in] */ IUri* uri,
    /* [in]*/ Int32 modeFlags)
{
    Mutex::Autolock lock(_m_syncLock);

    ProcessRecord* r = GetRecordForAppLocked(caller);
    if (r == NULL) {
        // throw new SecurityException("Unable to find app for caller "
        //        + caller
        //        + " when revoking permission to uri " + uri);
        return E_SECURITY_EXCEPTION;
    }
    if (uri == NULL) {
        Slogger::W(TAG, "revokeUriPermission: null uri");
        return E_INVALID_ARGUMENT;;
    }

    modeFlags &= (Intent_FLAG_GRANT_READ_URI_PERMISSION
           | Intent_FLAG_GRANT_WRITE_URI_PERMISSION);
    if (modeFlags == 0) {
       return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ICapsuleManager> pm = GetCapsuleManager();

    String authority;
    uri->GetAuthority(&authority);
    ContentProviderRecord* cpr = mProvidersByName.Find(authority)->mSecond;
    AutoPtr<IContentProviderInfo> pi;
    if (cpr != NULL) {
        cpr->mHolder->GetContentProviderInfo((IContentProviderInfo**)&pi);
    }
    else {
        // try {
        pm->ResolveContentProvider(authority,
           CapsuleManager_GET_URI_PERMISSION_PATTERNS, (IContentProviderInfo**)&pi);
        // }
        // catch (RemoteException ex) {
        // }
    }
    if (pi == NULL) {
        Slogger::W(TAG, "No content provider found for: " + authority);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 uid;
    r->mInfo->GetUid(&uid);
    RevokeUriPermissionLocked(uid, uri, modeFlags);

    return NOERROR;
}

ECode CActivityManagerService::NewUriPermissionOwner(
    /* [in] */ const String& name,
    /* [out] */ IBinder** token)
{
    Mutex::Autolock lock(_m_syncLock);
    AutoPtr<UriPermissionOwner> owner = new UriPermissionOwner(this, (Handle32)&name);
    *token = owner->GetExternalTokenLocked();

    return NOERROR;
}

ECode CActivityManagerService::GrantUriPermissionFromOwner(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 fromUid,
    /* [in] */ const String& targetPkg,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    Mutex::Autolock lock(_m_syncLock);
    UriPermissionOwner* owner = UriPermissionOwner::FromExternalToken(token);
    if (owner == NULL) {
        // throw new IllegalArgumentException("Unknown owner: " + token);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (fromUid != Binder::GetCallingUid()) {
       if (Binder::GetCallingUid() != Process::MyUid()) {
            // Only system code can grant URI permissions on behalf
            // of other users.
            // throw new SecurityException("nice try");
            return E_SECURITY_EXCEPTION;
       }
    }
    if (targetPkg.IsNull()) {
        // throw new IllegalArgumentException("null target");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (uri == NULL) {
        // throw new IllegalArgumentException("null uri");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    GrantUriPermissionLocked(fromUid, targetPkg, uri, modeFlags, owner);
    return NOERROR;
}

ECode CActivityManagerService::RevokeUriPermissionFromOwner(
    /* [in] */ IBinder* token,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 mode)
{
    Mutex::Autolock lock(_m_syncLock);
    UriPermissionOwner* owner = UriPermissionOwner::FromExternalToken(token);
    if (owner == NULL) {
        // throw new IllegalArgumentException("Unknown owner: " + token);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (uri == NULL) {
       owner->RemoveUriPermissionsLocked(mode);
    }
    else{
       owner->RemoveUriPermissionLocked(uri, mode);
    }
    return NOERROR;
}

ECode CActivityManagerService::ShowWaitingForDebugger(
    /* [in] */ IApplicationApartment* who,
    /* [in] */ Boolean waiting)
{
//    synchronized (this) {
//        ProcessRecord app =
//            who != null ? getRecordForAppLocked(who) : null;
//        if (app == null) return;
//
//        Message msg = Message.obtain();
//        msg.what = WAIT_FOR_DEBUGGER_MSG;
//        msg.obj = app;
//        msg.arg1 = waiting ? 1 : 0;
//        mHandler.sendMessage(msg);
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::GetMemoryInfo(
    /* [out] */ IMemoryInfo** outInfo)
{
//    outInfo.availMem = Process.getFreeMemory();
//    outInfo.threshold = HOME_APP_MEM;
//    outInfo.lowMemory = outInfo.availMem <
//            (HOME_APP_MEM + ((HIDDEN_APP_MEM-HOME_APP_MEM)/2));
    return E_NOT_IMPLEMENTED;
}

// =========================================================
// TASK MANAGEMENT
// =========================================================

ECode CActivityManagerService::GetTasks(
    /* [in] */ Int32 maxNum,
    /* [in] */ Int32 flags,
    /* [in] */ IThumbnailReceiver* receiver,
    /* [out] */ IObjectContainer* tasks)
{
//    ArrayList list = new ArrayList();
//
//    PendingThumbnailsRecord pending = null;
//    IApplicationThread topThumbnail = null;
//    ActivityRecord topRecord = null;
//
//    synchronized(this) {
//        if (localLOGV) Slog.v(
//            TAG, "getTasks: max=" + maxNum + ", flags=" + flags
//            + ", receiver=" + receiver);
//
        if (CheckCallingPermission("android.permission.GET_TASKS" /*android.Manifest.permission.GET_TASKS*/)
               != CapsuleManager_PERMISSION_GRANTED) {
           if (receiver != NULL) {
               // If the caller wants to wait for pending thumbnails,
               // it ain't gonna get them.
               // try {
               receiver->Finished();
               // } catch (RemoteException ex) {
               // }
           }
            Slogger::W(TAG, StringBuffer("Permission Denial: getTasks() from pid=")
                + Binder::GetCallingPid()
                + ", uid=" + Binder::GetCallingUid()
                + " requires " + "android.permission.GET_TASKS");
            // throw new SecurityException(msg);
            return E_SECURITY_EXCEPTION;
        }
//
//        int pos = mMainStack.mHistory.size()-1;
//        ActivityRecord next =
//            pos >= 0 ? (ActivityRecord)mMainStack.mHistory.get(pos) : null;
//        ActivityRecord top = null;
//        CharSequence topDescription = null;
//        TaskRecord curTask = null;
//        int numActivities = 0;
//        int numRunning = 0;
//        while (pos >= 0 && maxNum > 0) {
//            final ActivityRecord r = next;
//            pos--;
//            next = pos >= 0 ? (ActivityRecord)mMainStack.mHistory.get(pos) : null;
//
//            // Initialize state for next task if needed.
//            if (top == null ||
//                    (top.state == ActivityState.INITIALIZING
//                        && top.task == r.task)) {
//                top = r;
//                topDescription = r.description;
//                curTask = r.task;
//                numActivities = numRunning = 0;
//            }
//
//            // Add 'r' into the current task.
//            numActivities++;
//            if (r.app != null && r.app.thread != null) {
//                numRunning++;
//            }
//            if (topDescription == null) {
//                topDescription = r.description;
//            }
//
//            if (localLOGV) Slog.v(
//                TAG, r.intent.getComponent().flattenToShortString()
//                + ": task=" + r.task);
//
//            // If the next one is a different task, generate a new
//            // TaskInfo entry for what we have.
//            if (next == null || next.task != curTask) {
//                ActivityManager.RunningTaskInfo ci
//                        = new ActivityManager.RunningTaskInfo();
//                ci.id = curTask.taskId;
//                ci.baseActivity = r.intent.getComponent();
//                ci.topActivity = top.intent.getComponent();
//                ci.thumbnail = top.thumbnail;
//                ci.description = topDescription;
//                ci.numActivities = numActivities;
//                ci.numRunning = numRunning;
//                //System.out.println(
//                //    "#" + maxNum + ": " + " descr=" + ci.description);
//                if (ci.thumbnail == null && receiver != null) {
//                    if (localLOGV) Slog.v(
//                        TAG, "State=" + top.state + "Idle=" + top.idle
//                        + " app=" + top.app
//                        + " thr=" + (top.app != null ? top.app.thread : null));
//                    if (top.state == ActivityState.RESUMED
//                            || top.state == ActivityState.PAUSING) {
//                        if (top.idle && top.app != null
//                            && top.app.thread != null) {
//                            topRecord = top;
//                            topThumbnail = top.app.thread;
//                        } else {
//                            top.thumbnailNeeded = true;
//                        }
//                    }
//                    if (pending == null) {
//                        pending = new PendingThumbnailsRecord(receiver);
//                    }
//                    pending.pendingRecords.add(top);
//                }
//                list.add(ci);
//                maxNum--;
//                top = null;
//            }
//        }
//
//        if (pending != null) {
//            mPendingThumbnails.add(pending);
//        }
//    }
//
//    if (localLOGV) Slog.v(TAG, "We have pending thumbnails: " + pending);
//
//    if (topThumbnail != null) {
//        if (localLOGV) Slog.v(TAG, "Requesting top thumbnail");
//        try {
//            topThumbnail.requestThumbnail(topRecord);
//        } catch (Exception e) {
//            Slogger::W(TAG, "Exception thrown when requesting thumbnail", e);
//            sendPendingThumbnail(null, topRecord, null, null, true);
//        }
//    }
//
//    if (pending == null && receiver != null) {
//        // In this case all thumbnails were available and the client
//        // is being asked to be told when the remaining ones come in...
//        // which is unusually, since the top-most currently running
//        // activity should never have a canned thumbnail!  Oh well.
//        try {
//            receiver.finished();
//        } catch (RemoteException ex) {
//        }
//    }
//
//    return list;
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::GetRecentTasks(
    /* [in] */ Int32 maxNum,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** tasks)
{
    Mutex::Autolock lock(_m_syncLock);
    EnforceCallingPermission("android.Manifest.permission.GET_TASKS",
        "getRecentTasks()");
//
//        IPackageManager pm = AppGlobals.getPackageManager();
//
//        final int N = mRecentTasks.size();
//        ArrayList<ActivityManager.RecentTaskInfo> res
//                = new ArrayList<ActivityManager.RecentTaskInfo>(
//                        maxNum < N ? maxNum : N);
//        for (int i=0; i<N && maxNum > 0; i++) {
//            TaskRecord tr = mRecentTasks.get(i);
//            if (((flags&ActivityManager.RECENT_WITH_EXCLUDED) != 0)
//                    || (tr.intent == null)
//                    || ((tr.intent.getFlags()
//                            &Intent.FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS) == 0)) {
//                ActivityManager.RecentTaskInfo rti
//                        = new ActivityManager.RecentTaskInfo();
//                rti.id = tr.numActivities > 0 ? tr.taskId : -1;
//                rti.baseIntent = new Intent(
//                        tr.intent != null ? tr.intent : tr.affinityIntent);
//                rti.origActivity = tr.origActivity;
//
//                if ((flags&ActivityManager.RECENT_IGNORE_UNAVAILABLE) != 0) {
//                    // Check whether this activity is currently available.
//                    try {
//                        if (rti.origActivity != null) {
//                            if (pm.getActivityInfo(rti.origActivity, 0) == null) {
//                                continue;
//                            }
//                        } else if (rti.baseIntent != null) {
//                            if (pm.queryIntentActivities(rti.baseIntent,
//                                    null, 0) == null) {
//                                continue;
//                            }
//                        }
//                    } catch (RemoteException e) {
//                        // Will never happen.
//                    }
//                }
//
//                res.add(rti);
//                maxNum--;
//            }
//        }
//        return res;
//    }
    return E_NOT_IMPLEMENTED;
}

Int32 CActivityManagerService::FindAffinityTaskTopLocked(
    /* [in] */ Int32 startIndex,
    /* [in] */ const String& affinity)
{
//    int j;
//    TaskRecord startTask = ((ActivityRecord)mMainStack.mHistory.get(startIndex)).task;
//    TaskRecord jt = startTask;
//
//    // First look backwards
//    for (j=startIndex-1; j>=0; j--) {
//        ActivityRecord r = (ActivityRecord)mMainStack.mHistory.get(j);
//        if (r.task != jt) {
//            jt = r.task;
//            if (affinity.equals(jt.affinity)) {
//                return j;
//            }
//        }
//    }
//
//    // Now look forwards
//    final int N = mMainStack.mHistory.size();
//    jt = startTask;
//    for (j=startIndex+1; j<N; j++) {
//        ActivityRecord r = (ActivityRecord)mMainStack.mHistory.get(j);
//        if (r.task != jt) {
//            if (affinity.equals(jt.affinity)) {
//                return j;
//            }
//            jt = r.task;
//        }
//    }
//
//    // Might it be at the top?
//    if (affinity.equals(((ActivityRecord)mMainStack.mHistory.get(N-1)).task.affinity)) {
//        return N-1;
//    }
//
//    return -1;
    return 0;
}

/**
 * TODO: Add mController hook
 */
ECode CActivityManagerService::MoveTaskToFront(
    /* [in] */ Int32 task)
{
    EnforceCallingPermission("android.Manifest.permission.REORDER_TASKS",
            "moveTaskToFront()");

//    synchronized(this) {
//        if (!checkAppSwitchAllowedLocked(Binder.getCallingPid(),
//                Binder.getCallingUid(), "Task to front")) {
//            return;
//        }
//        final long origId = Binder.clearCallingIdentity();
//        try {
//            int N = mRecentTasks.size();
//            for (int i=0; i<N; i++) {
//                TaskRecord tr = mRecentTasks.get(i);
//                if (tr.taskId == task) {
//                    mMainStack.moveTaskToFrontLocked(tr, null);
//                    return;
//                }
//            }
//            for (int i=mMainStack.mHistory.size()-1; i>=0; i--) {
//                ActivityRecord hr = (ActivityRecord)mMainStack.mHistory.get(i);
//                if (hr.task.taskId == task) {
//                    mMainStack.moveTaskToFrontLocked(hr.task, null);
//                    return;
//                }
//            }
//        } finally {
//            Binder.restoreCallingIdentity(origId);
//        }
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::MoveTaskToBack(
    /* [in] */ Int32 task)
{
    EnforceCallingPermission("android.Manifest.permission.REORDER_TASKS",
            "moveTaskToBack()");
//
//    synchronized(this) {
//        if (mMainStack.mResumedActivity != null
//                && mMainStack.mResumedActivity.task.taskId == task) {
//            if (!checkAppSwitchAllowedLocked(Binder.getCallingPid(),
//                    Binder.getCallingUid(), "Task to back")) {
//                return;
//            }
//        }
//        final long origId = Binder.clearCallingIdentity();
//        mMainStack.moveTaskToBackLocked(task, null);
//        Binder.restoreCallingIdentity(origId);
//    }
    return E_NOT_IMPLEMENTED;
}

/**
 * Moves an activity, and all of the other activities within the same task, to the bottom
 * of the history stack.  The activity's order within the task is unchanged.
 *
 * @param token A reference to the activity we wish to move
 * @param nonRoot If false then this only works if the activity is the root
 *                of a task; if true it will work for any activity in a task.
 * @return Returns true if the move completed, false if not.
 */
ECode CActivityManagerService::MoveActivityTaskToBack(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean nonRoot,
    /* [out] */ Boolean* result)
{
    {
        Mutex::Autolock lock(_m_syncLock);
        const Int64 origId = Binder::ClearCallingIdentity();
        Int32 taskId = GetTaskForActivityLocked(token, !nonRoot);
        if (taskId >= 0) {
            *result = mMainStack->MoveTaskToBackLocked(taskId, NULL);
            return NOERROR;
        }
        Binder::RestoreCallingIdentity(origId);
    }
    *result = FALSE;

    return NOERROR;
}

ECode CActivityManagerService::MoveTaskBackwards(
    /* [in] */ Int32 task)
{
    EnforceCallingPermission("android.Manifest.permission.REORDER_TASKS",
            "moveTaskBackwards()");
//
//    synchronized(this) {
//        if (!checkAppSwitchAllowedLocked(Binder.getCallingPid(),
//                Binder.getCallingUid(), "Task backwards")) {
//            return;
//        }
//        final long origId = Binder.clearCallingIdentity();
//        moveTaskBackwardsLocked(task);
//        Binder.restoreCallingIdentity(origId);
//    }
    return E_NOT_IMPLEMENTED;
}

void CActivityManagerService::MoveTaskBackwardsLocked(
    /* [in] */ Int32 task)
{
//    Slog.e(TAG, "moveTaskBackwards not yet implemented!");
}

ECode CActivityManagerService::GetTaskForActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean onlyRoot,
    /* [out] */ Int32* taskId)
{
    Mutex::Autolock lock(_m_syncLock);

    return GetTaskForActivityLocked(token, onlyRoot);

}

Int32 CActivityManagerService::GetTaskForActivityLocked(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean onlyRoot)
{
    List<AutoPtr<CActivityRecord> >::Iterator it;
    TaskRecord* lastTask = NULL;
    for (it = mMainStack->mHistory.Begin(); it != mMainStack->mHistory.End(); ++it) {
        AutoPtr<CActivityRecord> r = *it;
        if ((IBinder*)r.Get() == token) {
            if (!onlyRoot || lastTask != r->mTask) {
                return r->mTask->mTaskId;
            }
            return -1;
        }
        lastTask = r->mTask;
    }

    return -1;
}

ECode CActivityManagerService::FinishOtherInstances(
    /* [in] */ IBinder* token,
    /* [in] */ IComponentName* className)
{
//    synchronized(this) {
//        final long origId = Binder.clearCallingIdentity();
//
//        int N = mMainStack.mHistory.size();
//        TaskRecord lastTask = null;
//        for (int i=0; i<N; i++) {
//            ActivityRecord r = (ActivityRecord)mMainStack.mHistory.get(i);
//            if (r.realActivity.equals(className)
//                    && r != token && lastTask != r.task) {
//                if (r.stack.finishActivityLocked(r, i, Activity.RESULT_CANCELED,
//                        null, "others")) {
//                    i--;
//                    N--;
//                }
//            }
//            lastTask = r.task;
//        }
//
//        Binder.restoreCallingIdentity(origId);
//    }
    return E_NOT_IMPLEMENTED;
}

// =========================================================
// THUMBNAILS
// =========================================================

ECode CActivityManagerService::ReportThumbnail(
    /* [in] */ IBinder* token,
    /* [in] */ IBitmap* thumbnail,
    /* [in] */ ICharSequence* description)
{
//    //System.out.println("Report thumbnail for " + token + ": " + thumbnail);
//    final long origId = Binder.clearCallingIdentity();
//    sendPendingThumbnail(null, token, thumbnail, description, true);
//    Binder.restoreCallingIdentity(origId);
    return E_NOT_IMPLEMENTED;
}

void CActivityManagerService::SendPendingThumbnail(
    /* [in] */ CActivityRecord* r,
    /* [in] */ IBinder* token,
    /* [in] */ IBitmap* thumbnail,
    /* [in] */ ICharSequence* description,
    /* [in] */ Boolean always)
{
    TaskRecord* task = NULL;
    List<PendingThumbnailsRecord*>* receivers = NULL;

    {
        Mutex::Autolock lock(_m_syncLock);

        if (r == NULL) {
            Int32 index = mMainStack->GetIndexOfTokenLocked(token);
            if (index < 0) {
                return;
            }
            r = (CActivityRecord*)mMainStack->mHistory[index];
        }
        if (thumbnail == NULL) {
            thumbnail = r->mThumbnail;
            description = r->mDescription;
        }
        if (thumbnail == NULL && !always) {
            // If there is no thumbnail, and this entry is not actually
            // going away, then abort for now and pick up the next
            // thumbnail we get.
            return;
        }
        task = r->mTask;

        List<PendingThumbnailsRecord*>::Iterator it = mPendingThumbnails.Begin();
        for (; it != mPendingThumbnails.End(); ++it) {
            PendingThumbnailsRecord* pr = *it;
            if (pr->mPendingRecords.Erase(r)) {
                if (receivers == NULL) {
                    receivers = new List<PendingThumbnailsRecord*>();
                }
                receivers->PushBack(pr);
                if (pr->mPendingRecords.GetSize() == 0) {
                    pr->mFinished = TRUE;
                    mPendingThumbnails.Erase(it);
                    continue;
                }
            }
        }
    }

    if (receivers != NULL) {
        List<PendingThumbnailsRecord*>::Iterator it = receivers->Begin();
        for (; it != receivers->End(); ++it) {
            PendingThumbnailsRecord* pr = *it;
            if (FAILED(pr->mReceiver->NewThumbnail(
                    task != NULL ? task->mTaskId : -1, thumbnail, description))) {
                goto SendPendingThumbnailException;
            }
            if (pr->mFinished) {
                if (FAILED(pr->mReceiver->Finished())) goto SendPendingThumbnailException;
            }
            continue;
SendPendingThumbnailException:
            Slogger::E(TAG, "Exception thrown when sending thumbnail");
        }
    }
}

// =========================================================
// CONTENT PROVIDERS
// =========================================================

ECode CActivityManagerService::GenerateApplicationProvidersLocked(
    /* [in] */ ProcessRecord* app,
    /* [out] */ IObjectContainer** providers)
{
    assert(providers != NULL);
    ECode ec = GetCapsuleManager()->QueryContentProviders(app->mProcessName,
            -1 /* app.info.uid */,
            0 /* STOCK_PM_FLAGS | PackageManager.GET_URI_PERMISSION_PATTERNS */,
            providers);
    if (FAILED(ec)) {
        return ec;
    }
    if (*providers != NULL) {
        Boolean hasNext = FALSE;
        AutoPtr<IObjectEnumerator> enumerator;
        (*providers)->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
        while(enumerator->MoveNext(&hasNext), hasNext) {
            AutoPtr<IContentProviderInfo> cpi;
            enumerator->Current((IInterface**)&cpi);
            String cpiName;
            cpi->GetName(&cpiName);
            ContentProviderRecord* cpr = mProvidersByClass[cpiName];
            if (cpr == NULL) {
                cpr = new ContentProviderRecord(cpi, app->mInfo);
                mProvidersByClass[cpiName] = cpr;
            }
            (app->mPubProviders)[cpiName] = cpr;
            AutoPtr<IApplicationInfo> cpiAppInfo;
            cpi->GetApplicationInfo((IApplicationInfo**)&cpiAppInfo);
            String cpiCName;
            cpiAppInfo->GetCapsuleName(&cpiCName);
            app->AddCapsule(cpiCName);
//            ensurePackageDexOpt(cpi.applicationInfo.packageName);
        }
    }

    return NOERROR;
}

String CActivityManagerService::CheckContentProviderPermissionLocked(
    /* [in] */ IContentProviderInfo* cpi,
    /* [in] */ ProcessRecord* r)
{
    //ToDo: not implemented according to android
    Int32 callingPid, callingUid;
    if (r != NULL) {
        callingPid = r->mPid;
        r->mInfo->GetUid(&callingUid);
    }
    else {
        callingPid = Binder::GetCallingPid();
        callingUid = Binder::GetCallingUid();
    }
    String cpiReadPerm, cpiWritePerm;
    cpi->GetReadPermission(&cpiReadPerm);
    cpi->GetWritePermission(&cpiWritePerm);
    Boolean cpiExported;
    cpi->IsExported(&cpiExported);
    AutoPtr<IApplicationInfo> cpiAppInfo;
    cpi->GetApplicationInfo((IApplicationInfo**)&cpiAppInfo);
    Int32 cpiUid;
    cpiAppInfo->GetUid(&cpiUid);
    if (CheckComponentPermission(cpiReadPerm, callingPid, callingUid,
            cpiExported ? -1 : cpiUid)
            == CapsuleManager_PERMISSION_GRANTED) {
        return String(NULL);
    }
    if (CheckComponentPermission(cpiWritePerm, callingPid, callingUid,
            cpiExported ? -1 : cpiUid)
            == CapsuleManager_PERMISSION_GRANTED) {
        return String(NULL);
    }

    AutoPtr<IObjectContainer> pps;
    cpi->GetPathPermissions((IObjectContainer**)&pps);
    // List<AutoPtr<CPathPermission> >* pps = cpi->mPathPermissions;
    if (pps != NULL) {
        //List<AutoPtr<CPathPermission> >::ReverseIterator rit = pps->RBegin();
        AutoPtr<IObjectEnumerator> en;
        pps->GetObjectEnumerator((IObjectEnumerator**)&en);
        Boolean hasNext = FALSE;
        AutoPtr<IPathPermission> pp;
        while (en->MoveNext(&hasNext), hasNext) {
            en->Current((IInterface**)&pp);
            String readPerm;
            pp->GetReadPermission(&readPerm);
            if (CheckComponentPermission(readPerm, callingPid, callingUid,
                    cpiExported ? -1 : cpiUid)
                    == CapsuleManager_PERMISSION_GRANTED) {
                return String(NULL);
            }

            String writePerm;
            pp->GetWritePermission(&writePerm);
            if (CheckComponentPermission(writePerm, callingPid, callingUid,
                    cpiExported ? -1 : cpiUid)
                    == CapsuleManager_PERMISSION_GRANTED) {
                return String(NULL);
            }
        }
    }

    Map<Int32, HashMap<IUri*, UriPermission*>*>::Iterator it = \
            mGrantedUriPermissions.Find(callingUid);
    if (it != mGrantedUriPermissions.End()) {
        HashMap<IUri*, UriPermission*>* perms = it->mSecond;
        if (perms != NULL) {
            HashMap<IUri*, UriPermission*>::Iterator pit = perms->Begin();
            for (; pit != perms->End(); ++pit) {
                String auth, cpiAuth;
                pit->mFirst->GetAuthority(&auth);
                cpi->GetAuthority(&cpiAuth);
                if (auth.Compare(cpiAuth)) {
                    return String(NULL);
                }
            }
        }
    }

    String appDes, cpiName;
    cpi->GetName(&cpiName);
    r->GetDescription(&appDes);
    StringBuffer msg;
    msg += "Permission Denial: opening provider " + cpiName
            + " from " + (r != NULL ? appDes : String("(null)")) + " (pid=" + callingPid
            + ", uid=" + callingUid + ") requires "
            + cpiReadPerm + " or " + cpiWritePerm;
    Slogger::W(TAG, msg);;
    return String((const char*)msg);
}

ECode CActivityManagerService::GetContentProviderImpl(
    /* [in] */ IApplicationApartment* caller,
    /* [in] */ const String& name,
    /* [out] */ IContentProviderHolder** providerHolder)
{
    ContentProviderRecord* cpr = NULL;
    AutoPtr<IContentProviderInfo> cpi;

    {
    Mutex::Autolock lock(_m_syncLock);

    ProcessRecord* r = NULL;
    if (caller != NULL) {
        r = GetProcessRecordForAppLocked(caller);
        if (r == NULL) {
            String callerDes;
            caller->GetDescription(&callerDes);
            Slogger::E(TAG, StringBuffer("Unable to find app for caller ") + callerDes
                    + " (pid=" + Binder::GetCallingPid()
                    + ") when getting content provider " + name);
            return E_SECURITY_EXCEPTION;
        }
    }

    // First check if this content provider has been published...
    HashMap<String, ContentProviderRecord*>::Iterator it = mProvidersByName.Find(name);
    if (it != mProvidersByName.End()) {
        cpr = it->mSecond;
    }
    if (cpr != NULL) {
        cpr->mHolder->GetContentProviderInfo((IContentProviderInfo**)&cpi);

        String msg = CheckContentProviderPermissionLocked(cpi, r);
        if (!msg.IsNull()) {
            Slogger::E(TAG, msg);
            return E_SECURITY_EXCEPTION;
        }

        if (r != NULL && cpr->CanRunHere(r)) {
            // This provider has been published or is in the process
            // of being published...  but it is also allowed to run
            // in the caller's process, so don't make a connection
            // and just let the caller instantiate its own instance.
            AutoPtr<IContentProvider> cprProvider;
            cpr->mHolder->GetContentProvider((IContentProvider**)&cprProvider);
            if (cprProvider != NULL) {
                // don't give caller the provider object, it needs
                // to make its own.
                cpr = new ContentProviderRecord(cpr);
            }
            *providerHolder = cpr->mHolder;
            return NOERROR;
        }

//        final long origId = Binder.clearCallingIdentity();

        // In this case the provider instance already exists, so we can
        // return it right away.
        if (r != NULL) {
            if (DEBUG_PROVIDER) {
                AutoPtr<IContentProviderInfo> cprProviderInfo;
                cpr->mHolder->GetContentProviderInfo((IContentProviderInfo**)&cprProviderInfo);
                String cprPName;
                cprProviderInfo->GetProcessName(&cprPName);
                Slogger::V(TAG, StringBuffer("Adding provider requested by ")
                    + r->mProcessName + " from process "
                    + cprPName);
            }
            if (r->mConProviders.Find(cpr) == r->mConProviders.End()) {
                r->mConProviders[cpr] = 1;
            }
            else {
                r->mConProviders[cpr] = r->mConProviders[cpr] + 1;
            }
            cpr->mClients.Insert(r);
            if (cpr->mApp != NULL && r->mSetAdj <= PERCEPTIBLE_APP_ADJ) {
                // If this is a perceptible app accessing the provider,
                // make sure to count it as being accessed and thus
                // back up on the LRU list.  This is good because
                // content providers are often expensive to start.
                UpdateLRUProcessLocked(cpr->mApp, FALSE, TRUE);
            }
        }
        else {
            cpr->mExternals++;
        }

        if (cpr->mApp != NULL) {
            UpdateOomAdjLocked(cpr->mApp);
        }

//        Binder.restoreCallingIdentity(origId);

    }
    else {
        AutoPtr<IServiceManager> serviceManager;
        AutoPtr<ICapsuleManager> capsuleManager;
        Elastos::GetServiceManager((IServiceManager**)&serviceManager);
        serviceManager->GetService(String("capsule"),
                (IInterface**)&capsuleManager);
        ECode ec = capsuleManager->ResolveContentProvider(name,
                -1/*STOCK_PM_FLAGS | PackageManager.GET_URI_PERMISSION_PATTERNS*/,
                (IContentProviderInfo**)&cpi);
        if (FAILED(ec) || cpi == NULL) {
            *providerHolder = NULL;
            return ec;
        }

        String msg = CheckContentProviderPermissionLocked(cpi, r);
        if (!msg.IsNull()) {
            Slogger::E(TAG, msg);
            return E_SECURITY_EXCEPTION;
        }

        String cpiPName, cpiName;
        cpi->GetProcessName(&cpiPName);
        if (!mProcessesReady && !mDidUpdate && !mWaitingUpdate
                && !cpiPName.Equals("system")) {
            // If this content provider does not run in the system
            // process, and the system is not yet ready to run other
            // processes, then fail fast instead of hanging.
            Slogger::E(TAG, "Attempt to launch content provider before system ready");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        cpi->GetName(&cpiName);
        cpr = mProvidersByClass[cpiName];
        Boolean firstClass = cpr == NULL;
        if (firstClass) {
            AutoPtr<IServiceManager> serviceManager;
            AutoPtr<ICapsuleManager> capsuleManager;
            AutoPtr<IApplicationInfo> appInfo;
            Elastos::GetServiceManager((IServiceManager**)&serviceManager);
            serviceManager->GetService(String("capsule"),
                    (IInterface**)&capsuleManager);
            AutoPtr<IApplicationInfo> cpiAppInfo;
            cpi->GetApplicationInfo((IApplicationInfo**)&cpiAppInfo);
            String cpiCName;
            cpiAppInfo->GetCapsuleName(&cpiCName);
            ec = capsuleManager->GetApplicationInfo(
                    cpiCName, -1 /*STOCK_PM_FLAGS*/,
                    (IApplicationInfo**)&appInfo);
            if (FAILED(ec) || (appInfo == NULL)) {
                String cpiName;
                cpi->GetName(&cpiName);
                Slogger::W(TAG, StringBuffer("No package info for content provider ")
                     + cpiName);
                *providerHolder = NULL;
                return ec;
            }
            cpr = new ContentProviderRecord(cpi, appInfo);
        }

        if (r != NULL && cpr->CanRunHere(r)) {
            // If this is a multiprocess provider, then just return its
            // info and allow the caller to instantiate it.  Only do
            // this if the provider is the same user as the caller's
            // process, or can run as root (so can be in any process).
            *providerHolder = cpr->mHolder;
            if (*providerHolder) (*providerHolder)->AddRef();

            return NOERROR;
        }

        if (DEBUG_PROVIDER) {
            Int32 rUid, cprUid;
            r->mInfo->GetUid(&rUid);
            cpr->mAppInfo->GetUid(&cprUid);
            AutoPtr<IContentProviderInfo> cprProviderInfo;
            cpr->mHolder->GetContentProviderInfo((IContentProviderInfo**)&cprProviderInfo);
            String cprName;
            cprProviderInfo->GetName(&cprName);
            Slogger::W(TAG, StringBuffer("LAUNCHING REMOTE PROVIDER (myuid ")
                    + rUid + " pruid " + cprUid + "): " + cprName);
        }

        // This is single process, and our app is now connecting to it.
        // See if we are already in the process of launching this
        // provider.
        List<ContentProviderRecord*>::Iterator it = mLaunchingProviders.Begin();
        for (; it != mLaunchingProviders.End(); ++it) {
            if (*it == cpr) {
                break;
            }
        }

        // If the provider is not already being launched, then get it
        // started.
        if (it == mLaunchingProviders.End()) {
            String cpiCName, cpiName, cpiPName;
            AutoPtr<IApplicationInfo> cpiAppInfo;
            cpi->GetApplicationInfo((IApplicationInfo**)&cpiAppInfo);
            cpiAppInfo->GetCapsuleName(&cpiCName);
            cpi->GetName(&cpiName);
            cpi->GetProcessName(&cpiPName);
            AutoPtr<IComponentName> component;
            CComponentName::New(cpiCName,
                    cpiName, (IComponentName**)&component);
            ProcessRecord* proc = StartProcessLocked(cpiPName,
                    cpr->mAppInfo, TRUE, 0, "activity", component, FALSE);
            if (proc == NULL) {
                Int32 cpiUid;
                cpiAppInfo->GetUid(&cpiUid);
                Slogger::W(TAG, StringBuffer("Unable to launch app ")
                        + cpiCName + "/"
                        + cpiUid + " for provider "
                        + name + ": process is bad");
                *providerHolder = NULL;
                return ec;
            }
            cpr->mLaunchingApp = proc;
            mLaunchingProviders.PushBack(cpr);
        }

        // Make sure the provider is published (the same provider class
        // may be published under multiple names).
        if (firstClass) {
            String cpiName;
            cpi->GetName(&cpiName);
            mProvidersByClass[cpiName] = cpr;
        }
        mProvidersByName[name] = cpr;

        if (r != NULL) {
            if (DEBUG_PROVIDER) {
                AutoPtr<IContentProviderInfo> cprProviderInfo;
                cpr->mHolder->GetContentProviderInfo((IContentProviderInfo**)&cprProviderInfo);
                String cprPName;
                cprProviderInfo->GetProcessName(&cprPName);
                Slogger::V(TAG, StringBuffer("Adding provider requested by ")
                    + r->mProcessName + " from process "
                    + cprPName);
            }
            if (r->mConProviders.Find(cpr) == r->mConProviders.End()) {
                r->mConProviders[cpr] = 1;
            }
            else {
                r->mConProviders[cpr] = r->mConProviders[cpr] + 1;
            }
            cpr->mClients.Insert(r);
        }
        else {
            cpr->mExternals++;
        }
    }
    }

    AutoPtr<IContentProvider> cprProvider;
    cpr->Lock();
    while (cprProvider = NULL, cpr->mHolder->GetContentProvider((IContentProvider**)&cprProvider),
            cprProvider == NULL) {
        if (cpr->mLaunchingApp == NULL) {
            cpr->Unlock();
            AutoPtr<IApplicationInfo> cpiAppInfo;
            cpi->GetApplicationInfo((IApplicationInfo**)&cpiAppInfo);
            String cpiCName;
            cpiAppInfo->GetCapsuleName(&cpiCName);
            Int32 cpiUid;
            cpiAppInfo->GetUid(&cpiUid);
            Slogger::W(TAG, StringBuffer("Unable to launch app ")
                    + cpiCName + "/" + cpiUid + " for provider "
                    + name + ": launching app became null");
//            EventLog.writeEvent(EventLogTags.AM_PROVIDER_LOST_PROCESS,
//                    cpi.applicationInfo.packageName,
//                    cpi.applicationInfo.uid, name);
            *providerHolder = NULL;
            return E_LAUNCH_EXCEPTION;
        }
        cpr->Wait();
    }
    cpr->Unlock();

    *providerHolder = cpr->mHolder;
    if (*providerHolder != NULL) (*providerHolder)->AddRef();
    return NOERROR;
}

ECode CActivityManagerService::GetContentProvider(
    /* [in] */ IApplicationApartment* caller,
    /* [in] */ const String& name,
    /* [out] */ IContentProviderHolder** providerHolder)
{
    if (providerHolder == NULL) return E_INVALID_ARGUMENT;

    if (caller == NULL) {
        Slogger::W(TAG,
            StringBuffer("null IApplicationThread when getting content provider ")
            + name);
        return E_SECURITY_EXCEPTION;
    }

    return GetContentProviderImpl(caller, name, providerHolder);
}

ECode CActivityManagerService::GetContentProviderExternal(
    /* [in] */ const String& name,
    /* [out] */ IContentProviderHolder** providerHolder)
{
    return GetContentProviderImpl(NULL, name, providerHolder);
}

/**
 * Drop a content provider from a ProcessRecord's bookkeeping
 * @param cpr
 */
ECode CActivityManagerService::RemoveContentProvider(
    /* [in] */ IApplicationApartment* caller,
    /* [in] */ const String& name)
{
//    synchronized (this) {
//        ContentProviderRecord cpr = mProvidersByName.get(name);
//        if(cpr == null) {
//            // remove from mProvidersByClass
//            if (DEBUG_PROVIDER) Slog.v(TAG, name +
//                    " provider not found in providers list");
//            return;
//        }
//        final ProcessRecord r = getRecordForAppLocked(caller);
//        if (r == null) {
//            throw new SecurityException(
//                    "Unable to find app for caller " + caller +
//                    " when removing content provider " + name);
//        }
//        //update content provider record entry info
//        ContentProviderRecord localCpr = mProvidersByClass.get(cpr.info.name);
//        if (DEBUG_PROVIDER) Slog.v(TAG, "Removing provider requested by "
//                + r.info.processName + " from process "
//                + localCpr.appInfo.processName);
//        if (localCpr.app == r) {
//            //should not happen. taken care of as a local provider
//            Slogger::W(TAG, "removeContentProvider called on local provider: "
//                    + cpr.info.name + " in process " + r.processName);
//            return;
//        } else {
//            Integer cnt = r.conProviders.get(localCpr);
//            if (cnt == null || cnt.intValue() <= 1) {
//                localCpr.clients.remove(r);
//                r.conProviders.remove(localCpr);
//            } else {
//                r.conProviders.put(localCpr, new Integer(cnt.intValue()-1));
//            }
//        }
//        updateOomAdjLocked();
//    }
    return E_NOT_IMPLEMENTED;
}

void CActivityManagerService::RemoveContentProviderExternal(
    /* [in] */ const String& name)
{
    Mutex::Autolock lock(_m_syncLock);

    HashMap<String, ContentProviderRecord*>::Iterator it = \
            mProvidersByName.Find(name);
    if (it == mProvidersByName.End()) {
        //remove from mProvidersByClass
        if(localLOGV) {
            Slogger::V(TAG, StringBuffer(name)
                    + " content provider not found in providers list");
        }
        return;
    }

    ContentProviderRecord* cpr = it->mSecond;

    //update content provider record entry info
    AutoPtr<IContentProviderInfo> cprProviderInfo;
    cpr->mHolder->GetContentProviderInfo((IContentProviderInfo**)&cprProviderInfo);
    String cprName;
    cprProviderInfo->GetName(&cprName);
    ContentProviderRecord* localCpr = mProvidersByClass[cprName];
    localCpr->mExternals--;
    if (localCpr->mExternals < 0) {
        String localCprDes;
        localCpr->GetDescription(&localCprDes);
        Slogger::E(TAG, StringBuffer("Externals < 0 for content provider ")
                + localCprDes);
    }
    UpdateOomAdjLocked();
}

ECode CActivityManagerService::PublishContentProviders(
    /* [in] */ IApplicationApartment* caller,
    /* [in] */ IObjectContainer* providers)
{
    if (providers == NULL) {
        return NOERROR;
    }

    Mutex::Autolock lock(_m_syncLock);

    ProcessRecord* r = GetProcessRecordForAppLocked(caller);
    if (r == NULL) {
        String callerDes;
        caller->GetDescription(&callerDes);
        Slogger::W(TAG,
                StringBuffer("Unable to find app for caller ") + callerDes
                + " (pid=" + Binder::GetCallingPid()
                + ") when publishing content providers");
        return E_SECURITY_EXCEPTION;
    }

    Boolean hasNext = FALSE;
    AutoPtr<IObjectEnumerator> enumerator = NULL;
    providers->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    while(enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IContentProviderHolder> src;
        enumerator->Current((IInterface**)&src);

        if (src == NULL) {
            continue;
        }
        AutoPtr<IContentProviderInfo> srcProviderInfo;
        src->GetContentProviderInfo((IContentProviderInfo**)&srcProviderInfo);
        AutoPtr<IContentProvider> srcProvider;
        src->GetContentProvider((IContentProvider**)&srcProvider);
        if (srcProviderInfo == NULL || srcProvider == NULL) {
            continue;
        }

        String srcName;
        srcProviderInfo->GetName(&srcName);
        ContentProviderRecord* dst =
                r->mPubProviders[srcName];
        if (dst != NULL) {
            AutoPtr<IContentProviderInfo> dstProviderInfo;
            dst->mHolder->GetContentProviderInfo((IContentProviderInfo**)&dstProviderInfo);
            String dstName;
            dstProviderInfo->GetName(&dstName);
            mProvidersByClass[dstName] = dst;
            String dstAuth;
            dstProviderInfo->GetAuthority(&dstAuth);
            StringTokenizer* tokens = new StringTokenizer(
                    dstAuth, ";");
            while (tokens->HasMoreTokens()) {
                String name = tokens->NextToken();
                mProvidersByName[name] = dst;
            }

            List<ContentProviderRecord*>::Iterator it = \
                    mLaunchingProviders.Begin();
            while (it != mLaunchingProviders.End()) {
                if (*it == dst) {
                    it = mLaunchingProviders.Erase(it);
                }
                else ++it;
            }
            dst->Lock();
            dst->mHolder->SetContentProvider(srcProvider);
            dst->mApp = r;
            dst->Notify();
            dst->Unlock();
            UpdateOomAdjLocked(r);
        }
    }
    return NOERROR;
}

void CActivityManagerService::InstallSystemProviders()
{
//    List providers;
//    synchronized (mSelf) {
//        ProcessRecord app = mSelf.mProcessNames.get("system", Process.SYSTEM_UID);
//        providers = mSelf.generateApplicationProvidersLocked(app);
//        if (providers != null) {
//            for (int i=providers.size()-1; i>=0; i--) {
//                ProviderInfo pi = (ProviderInfo)providers.get(i);
//                if ((pi.applicationInfo.flags&ApplicationInfo.FLAG_SYSTEM) == 0) {
//                    Slogger::W(TAG, "Not installing system proc provider " + pi.name
//                            + ": not system .apk");
//                    providers.remove(i);
//                }
//            }
//        }
//    }
//    if (providers != null) {
//        mSystemThread.installSystemProviders(providers);
//    }
}

/**
 * Allows app to retrieve the MIME type of a URI without having permission
 * to access its content provider.
 *
 * CTS tests for this functionality can be run with "runtest cts-appsecurity".
 *
 * Test cases are at cts/tests/appsecurity-tests/test-apps/UsePermissionDiffCert/
 *     src/com/android/cts/usespermissiondiffcertapp/AccessPermissionWithDiffSigTest.java
 */
ECode CActivityManagerService::GetProviderMimeType(
    /* [in] */ IUri* uri,
    /* [out] */ String* type)
{
    String name;
    uri->GetAuthority(&name);
//    final long ident = Binder.clearCallingIdentity();
    AutoPtr<IContentProviderHolder> holder;

    if (SUCCEEDED(GetContentProviderExternal(name,
            (IContentProviderHolder**)&holder))) {
        if (holder != NULL) {
            AutoPtr<IContentProvider> provider;
            holder->GetContentProvider((IContentProvider**)&provider);
            provider->GetType(uri, type);
            RemoveContentProviderExternal(name);
//        Binder.restoreCallingIdentity(ident);
            return NOERROR;
        }
    }
    else {
        String uriDes;
        uri->GetDescription(&uriDes);
        Slogger::W(TAG, StringBuffer("Content provider dead retrieving ")
                + uriDes);
    }

    if (holder != NULL) {
        RemoveContentProviderExternal(name);
    }

    *type = NULL;
//        Binder.restoreCallingIdentity(ident);
    return NOERROR;
}

// =========================================================
// GLOBAL MANAGEMENT
// =========================================================

ProcessRecord* CActivityManagerService::NewProcessRecordLocked(
    /* [in] */ IApplicationApartment* apartment,
    /* [in] */ IApplicationInfo* info,
    /* [in] */ const String& customProcess)
{
    String proc;
    if (!customProcess.IsNull()) {
        proc = customProcess;
    }
    else {
        info->GetProcessName(&proc);
    }
//    AutoPtr<BatteryStatsImpl::Uid::Proc> ps;
//    AutoPtr<BatteryStatsImpl> stats = mBatteryStatsService->GetActiveStatistics();
//    stats->Lock();
//    ps = stats->GetProcessStatsLocked(info->mUid, proc);
//    stats->Unlock();
    return new ProcessRecord(NULL, apartment, info, proc);
}

ProcessRecord* CActivityManagerService::AddAppLocked(
    /* [in] */ IApplicationInfo* info)
{
    String pname;
    info->GetProcessName(&pname);
    Int32 uid;
    info->GetUid(&uid);
    ProcessRecord* app = GetProcessRecordLocked(pname, uid);

    if (app == NULL) {
        app = NewProcessRecordLocked(NULL, info, String(NULL));
        mProcessNames->Put(pname, uid, app);
        UpdateLRUProcessLocked(app, TRUE, TRUE);
    }

    Int32 flags;
    info->GetFlags(&flags);
    if ((flags & (ApplicationInfo_FLAG_SYSTEM | ApplicationInfo_FLAG_PERSISTENT))
            == (ApplicationInfo_FLAG_SYSTEM | ApplicationInfo_FLAG_PERSISTENT)) {
        app->mPersistent = TRUE;
        app->mMaxAdj = CORE_SERVER_ADJ;
    }
    if (app->mAppApartment == NULL &&
        (Find(mPersistentStartingProcesses.Begin(),
        mPersistentStartingProcesses.End(), app) == mPersistentStartingProcesses.End())) {
        mPersistentStartingProcesses.PushBack(app);
        StartProcessLocked(app, "added application", app->mProcessName);
    }

    return app;
}

ECode CActivityManagerService::UnhandledBack()
{
    EnforceCallingPermission("android.Manifest.permission.FORCE_BACK",
           "unhandledBack()");
//
//    synchronized(this) {
//        int count = mMainStack.mHistory.size();
//        if (DEBUG_SWITCH) Slog.d(
//            TAG, "Performing unhandledBack(): stack size = " + count);
//        if (count > 1) {
//            final long origId = Binder.clearCallingIdentity();
//            mMainStack.finishActivityLocked((ActivityRecord)mMainStack.mHistory.get(count-1),
//                    count-1, Activity.RESULT_CANCELED, null, "unhandled-back");
//            Binder.restoreCallingIdentity(origId);
//        }
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::OpenContentUri(
    /* [in] */ IUri* uri
    /* ParcelFileDescriptor */ )
{
//    String name = uri.getAuthority();
//    ContentProviderHolder cph = getContentProviderExternal(name);
//    ParcelFileDescriptor pfd = null;
//    if (cph != null) {
//        // We record the binder invoker's uid in thread-local storage before
//        // going to the content provider to open the file.  Later, in the code
//        // that handles all permissions checks, we look for this uid and use
//        // that rather than the Activity Manager's own uid.  The effect is that
//        // we do the check against the caller's permissions even though it looks
//        // to the content provider like the Activity Manager itself is making
//        // the request.
//        sCallerIdentity.set(new Identity(
//                Binder.getCallingPid(), Binder.getCallingUid()));
//        try {
//            pfd = cph.provider.openFile(uri, "r");
//        } catch (FileNotFoundException e) {
//            // do nothing; pfd will be returned null
//        } finally {
//            // Ensure that whatever happens, we clean up the identity state
//            sCallerIdentity.remove();
//        }
//
//        // We've got the fd now, so we're done with the provider.
//        removeContentProviderExternal(name);
//    } else {
//        Slog.d(TAG, "Failed to get provider for authority '" + name + "'");
//    }
//    return pfd;
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::GoingToSleep()
{
//    synchronized(this) {
//        mSleeping = true;
//        mWindowManager.setEventDispatching(false);
//
//        if (mMainStack.mResumedActivity != null) {
//            mMainStack.pauseIfSleepingLocked();
//        } else {
//            Slogger::W(TAG, "goingToSleep with no resumed activity!");
//        }
//
//        // Initialize the wake times of all processes.
//        checkExcessivePowerUsageLocked(false);
//        mHandler.removeMessages(CHECK_EXCESSIVE_WAKE_LOCKS_MSG);
//        Message nmsg = mHandler.obtainMessage(CHECK_EXCESSIVE_WAKE_LOCKS_MSG);
//        mHandler.sendMessageDelayed(nmsg, POWER_CHECK_DELAY);
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::Shutdown(
    /* [in]*/ Int32 timeout,
    /* [out] */ Boolean* result)
{
    if (CheckCallingPermission("android.permission.SHUTDOWN" /*android.Manifest.permission.SHUTDOWN)*/)
           != CapsuleManager_PERMISSION_GRANTED) {
        // throw new SecurityException("Requires permission "
        //         + android.Manifest.permission.SHUTDOWN);
        return E_SECURITY_EXCEPTION;
    }
//
//    boolean timedout = false;
//
//    synchronized(this) {
//        mShuttingDown = true;
//        mWindowManager.setEventDispatching(false);
//
//        if (mMainStack.mResumedActivity != null) {
//            mMainStack.pauseIfSleepingLocked();
//            final long endTime = System.currentTimeMillis() + timeout;
//            while (mMainStack.mResumedActivity != null
//                    || mMainStack.mPausingActivity != null) {
//                long delay = endTime - System.currentTimeMillis();
//                if (delay <= 0) {
//                    Slogger::W(TAG, "Activity manager shutdown timed out");
//                    timedout = true;
//                    break;
//                }
//                try {
//                    this.wait();
//                } catch (InterruptedException e) {
//                }
//            }
//        }
//    }
//
//    mUsageStatsService.shutdown();
//    mBatteryStatsService.shutdown();
//
//    return timedout;
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::WakingUp()
{
//    synchronized(this) {
//        if (mMainStack.mGoingToSleep.isHeld()) {
//            mMainStack.mGoingToSleep.release();
//        }
//        mWindowManager.setEventDispatching(true);
//        mSleeping = false;
//        mMainStack.resumeTopActivityLocked(null);
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::StopAppSwitches()
{
    if (CheckCallingPermission("android.permission.STOP_APP_SWITCHES" /*android.Manifest.permission.STOP_APP_SWITCHES*/)
           != CapsuleManager_PERMISSION_GRANTED) {
        // throw new SecurityException("Requires permission "
        //         + android.Manifest.permission.STOP_APP_SWITCHES);
        return E_SECURITY_EXCEPTION;
    }
//
//    synchronized(this) {
//        mAppSwitchesAllowedTime = SystemClock.uptimeMillis()
//                + APP_SWITCH_DELAY_TIME;
//        mDidAppSwitch = false;
//        mHandler.removeMessages(DO_PENDING_ACTIVITY_LAUNCHES_MSG);
//        Message msg = mHandler.obtainMessage(DO_PENDING_ACTIVITY_LAUNCHES_MSG);
//        mHandler.sendMessageDelayed(msg, APP_SWITCH_DELAY_TIME);
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::ResumeAppSwitches()
{
    if (CheckCallingPermission("android.permission.STOP_APP_SWITCHES" /*android.Manifest.permission.STOP_APP_SWITCHES)*/)
           != CapsuleManager_PERMISSION_GRANTED) {
        // throw new SecurityException("Requires permission "
        //        + android.Manifest.permission.STOP_APP_SWITCHES);
        return E_SECURITY_EXCEPTION;
    }
//
//    synchronized(this) {
//        // Note that we don't execute any pending app switches... we will
//        // let those wait until either the timeout, or the next start
//        // activity request.
//        mAppSwitchesAllowedTime = 0;
//    }
    return E_NOT_IMPLEMENTED;
}

Boolean CActivityManagerService::CheckAppSwitchAllowedLocked(
    /* [in] */ Int32 callingPid,
    /* [in] */ Int32 callingUid,
    /* [in] */ const char* name)
{
    if (mAppSwitchesAllowedTime < SystemClock::GetUptimeMillis()) {
        return TRUE;
    }

    Int32 perm = CheckComponentPermission(
            "elastos.permission.STOP_APP_SWITCHES", /*android.Manifest.permission.STOP_APP_SWITCHES*/
            callingPid, callingUid, -1);
    if (perm == CapsuleManager_PERMISSION_GRANTED) {
        return TRUE;
    }

    Slogger::W(TAG, StringBuffer(name) + " request from " + callingUid + " stopped");
    return FALSE;
}

ECode CActivityManagerService::SetDebugApp(
    /* [in] */ const String& capsuleName,
    /* [in] */ Boolean waitForDebugger,
    /* [in] */ Boolean persistent)
{
    EnforceCallingPermission("android.Manifest.permission.SET_DEBUG_APP",
           "setDebugApp()");
//
//    // Note that this is not really thread safe if there are multiple
//    // callers into it at the same time, but that's not a situation we
//    // care about.
//    if (persistent) {
//        final ContentResolver resolver = mContext.getContentResolver();
//        Settings.System.putString(
//            resolver, Settings.System.DEBUG_APP,
//            packageName);
//        Settings.System.putInt(
//            resolver, Settings.System.WAIT_FOR_DEBUGGER,
//            waitForDebugger ? 1 : 0);
//    }
//
//    synchronized (this) {
//        if (!persistent) {
//            mOrigDebugApp = mDebugApp;
//            mOrigWaitForDebugger = mWaitForDebugger;
//        }
//        mDebugApp = packageName;
//        mWaitForDebugger = waitForDebugger;
//        mDebugTransient = !persistent;
//        if (packageName != null) {
//            final long origId = Binder.clearCallingIdentity();
//            forceStopPackageLocked(packageName, -1, false, false, true);
//            Binder.restoreCallingIdentity(origId);
//        }
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::SetAlwaysFinish(
    /* [in] */ Boolean enabled)
{
    EnforceCallingPermission("android.Manifest.permission.SET_ALWAYS_FINISH",
         "setAlwaysFinish()");
//
//    Settings.System.putInt(
//            mContext.getContentResolver(),
//            Settings.System.ALWAYS_FINISH_ACTIVITIES, enabled ? 1 : 0);
//
//    synchronized (this) {
//        mAlwaysFinishActivities = enabled;
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::SetActivityController(
    /* [in] */ IActivityController* controller)
{
    EnforceCallingPermission("elastos.permission.SET_ACTIVITY_WATCHER",/*android.Manifest.permission.SET_ACTIVITY_WATCHER,*/
            "setActivityController()");

    Mutex::Autolock lock(_m_syncLock);
    mController = controller;

    return NOERROR;
}

ECode CActivityManagerService::IsUserAMonkey(
    /* [out] */ Boolean* result)
{
//    // For now the fact that there is a controller implies
//    // we have a monkey.
//    synchronized (this) {
//        return mController != null;
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::RegisterActivityWatcher(
    /* [in] */ IActivityWatcher* watcher)
{
    Mutex::Autolock lock(_m_syncLock);
    Boolean outBoolean;
    mWatchers->Register(watcher, &outBoolean);

    return NOERROR;
}

ECode CActivityManagerService::UnregisterActivityWatcher(
    /* [in] */ IActivityWatcher* watcher)
{
    Mutex::Autolock lock(_m_syncLock);
    Boolean outBoolean;
    mWatchers->Unregister(watcher, &outBoolean);

    return NOERROR;
}

ECode CActivityManagerService::EnterSafeMode()
{
//    synchronized(this) {
//        // It only makes sense to do this before the system is ready
//        // and started launching other packages.
//        if (!mSystemReady) {
//            try {
//                AppGlobals.getPackageManager().enterSafeMode();
//            } catch (RemoteException e) {
//            }
//
//            View v = LayoutInflater.from(mContext).inflate(
//                    com.android.internal.R.layout.safe_mode, null);
//            WindowManager.LayoutParams lp = new WindowManager.LayoutParams();
//            lp.type = WindowManager.LayoutParams.TYPE_SECURE_SYSTEM_OVERLAY;
//            lp.width = WindowManager.LayoutParams.WRAP_CONTENT;
//            lp.height = WindowManager.LayoutParams.WRAP_CONTENT;
//            lp.gravity = Gravity.BOTTOM | Gravity.LEFT;
//            lp.format = v.getBackground().getOpacity();
//            lp.flags = WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE
//                    | WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE;
//            ((WindowManager)mContext.getSystemService(
//                    Context.WINDOW_SERVICE)).addView(v, lp);
//        }
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::NoteWakeupAlarm(
    /* [in] */ IIntentSender* sender)
{
//    if (!(sender instanceof PendingIntentRecord)) {
//        return;
//    }
//    BatteryStatsImpl stats = mBatteryStatsService.getActiveStatistics();
//    synchronized (stats) {
//        if (mBatteryStatsService.isOnBattery()) {
//            mBatteryStatsService.enforceCallingPermission();
//            PendingIntentRecord rec = (PendingIntentRecord)sender;
//            int MY_UID = Binder.getCallingUid();
//            int uid = rec.uid == MY_UID ? Process.SYSTEM_UID : rec.uid;
//            BatteryStatsImpl.Uid.Pkg pkg =
//                stats.getPackageStatsLocked(uid, rec.key.packageName);
//            pkg.incWakeupsLocked();
//        }
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::KillPids(
    /* [in] */ const ArrayOf<Int32>& pids,
    /* [in] */ const String& pReason,
    /* [out] */ Boolean* result)
{
//    if (Binder.getCallingUid() != Process.SYSTEM_UID) {
//        throw new SecurityException("killPids only available to the system");
//    }
//    String reason = (pReason == null) ? "Unknown" : pReason;
//    // XXX Note: don't acquire main activity lock here, because the window
//    // manager calls in with its locks held.
//
//    boolean killed = false;
//    synchronized (mPidsSelfLocked) {
//        int[] types = new int[pids.length];
//        int worstType = 0;
//        for (int i=0; i<pids.length; i++) {
//            ProcessRecord proc = mPidsSelfLocked.get(pids[i]);
//            if (proc != null) {
//                int type = proc.setAdj;
//                types[i] = type;
//                if (type > worstType) {
//                    worstType = type;
//                }
//            }
//        }
//
//        // If the worse oom_adj is somewhere in the hidden proc LRU range,
//        // then constrain it so we will kill all hidden procs.
//        if (worstType < EMPTY_APP_ADJ && worstType > HIDDEN_APP_MIN_ADJ) {
//            worstType = HIDDEN_APP_MIN_ADJ;
//        }
//        Slogger::W(TAG, "Killing processes " + reason + " at adjustment " + worstType);
//        for (int i=0; i<pids.length; i++) {
//            ProcessRecord proc = mPidsSelfLocked.get(pids[i]);
//            if (proc == null) {
//                continue;
//            }
//            int adj = proc.setAdj;
//            if (adj >= worstType && !proc.killedBackground) {
//                Slogger::W(TAG, "Killing " + proc + " (adj " + adj + "): " + reason);
//                EventLog.writeEvent(EventLogTags.AM_KILL, proc.pid,
//                        proc.processName, adj, reason);
//                killed = true;
//                proc.killedBackground = true;
//                Process.killProcessQuiet(pids[i]);
//            }
//        }
//    }
//    return killed;
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::StartRunning(
    /* [in] */ const String& cap,
    /* [in] */ const String& cls,
    /* [in] */ const String& action,
    /* [in] */ const String& data)
{
//    synchronized(this) {
//        if (mStartRunning) {
//            return;
//        }
//        mStartRunning = true;
//        mTopComponent = pkg != null && cls != null
//                ? new ComponentName(pkg, cls) : null;
//        mTopAction = action != null ? action : Intent.ACTION_MAIN;
//        mTopData = data;
//        if (!mSystemReady) {
//            return;
//        }
//    }
//
//    systemReady(null);
    return E_NOT_IMPLEMENTED;
}

void CActivityManagerService::RetrieveSettings()
{
//    final ContentResolver resolver = mContext.getContentResolver();
//    String debugApp = Settings.System.getString(
//        resolver, Settings.System.DEBUG_APP);
//    boolean waitForDebugger = Settings.System.getInt(
//        resolver, Settings.System.WAIT_FOR_DEBUGGER, 0) != 0;
//    boolean alwaysFinishActivities = Settings.System.getInt(
//        resolver, Settings.System.ALWAYS_FINISH_ACTIVITIES, 0) != 0;
//
//    Configuration configuration = new Configuration();
//    Settings.System.getConfiguration(resolver, configuration);
//
//    synchronized (this) {
//        mDebugApp = mOrigDebugApp = debugApp;
//        mWaitForDebugger = mOrigWaitForDebugger = waitForDebugger;
//        mAlwaysFinishActivities = alwaysFinishActivities;
//        // This happens before any activities are started, so we can
//        // change mConfiguration in-place.
//        mConfiguration.updateFrom(configuration);
//        mConfigurationSeq = mConfiguration.seq = 1;
//        if (DEBUG_CONFIGURATION) Slog.v(TAG, "Initial config: " + mConfiguration);
//    }
}

Boolean CActivityManagerService::TestIsSystemReady()
{
    // no need to synchronize(this) just to read & return the value
    return mSystemReady;
}

//private static File getCalledPreBootReceiversFile() {
//    File dataDir = Environment.getDataDirectory();
//    File systemDir = new File(dataDir, "system");
//    File fname = new File(systemDir, "called_pre_boots.dat");
//    return fname;
//}

List<AutoPtr<IComponentName> >* CActivityManagerService::ReadLastDonePreBootReceivers()
{
//    ArrayList<ComponentName> lastDoneReceivers = new ArrayList<ComponentName>();
//    File file = getCalledPreBootReceiversFile();
//    FileInputStream fis = null;
//    try {
//        fis = new FileInputStream(file);
//        DataInputStream dis = new DataInputStream(new BufferedInputStream(fis, 2048));
//        int vers = dis.readInt();
//        String codename = dis.readUTF();
//        if (vers == android.os.Build.VERSION.SDK_INT
//                && codename.equals(android.os.Build.VERSION.CODENAME)) {
//            int num = dis.readInt();
//            while (num > 0) {
//                num--;
//                String pkg = dis.readUTF();
//                String cls = dis.readUTF();
//                lastDoneReceivers.add(new ComponentName(pkg, cls));
//            }
//        }
//    } catch (FileNotFoundException e) {
//    } catch (IOException e) {
//        Slogger::W(TAG, "Failure reading last done pre-boot receivers", e);
//    } finally {
//        if (fis != null) {
//            try {
//                fis.close();
//            } catch (IOException e) {
//            }
//        }
//    }
//    return lastDoneReceivers;
    return NULL;
}

void CActivityManagerService::WriteLastDonePreBootReceivers(
    List<AutoPtr<IComponentName> >& list)
{
//    File file = getCalledPreBootReceiversFile();
//    FileOutputStream fos = null;
//    DataOutputStream dos = null;
//    try {
//        Slog.i(TAG, "Writing new set of last done pre-boot receivers...");
//        fos = new FileOutputStream(file);
//        dos = new DataOutputStream(new BufferedOutputStream(fos, 2048));
//        dos.writeInt(android.os.Build.VERSION.SDK_INT);
//        dos.writeUTF(android.os.Build.VERSION.CODENAME);
//        dos.writeInt(list.size());
//        for (int i=0; i<list.size(); i++) {
//            dos.writeUTF(list.get(i).getPackageName());
//            dos.writeUTF(list.get(i).getClassName());
//        }
//    } catch (IOException e) {
//        Slogger::W(TAG, "Failure writing last done pre-boot receivers", e);
//        file.delete();
//    } finally {
//        FileUtils.sync(fos);
//        if (dos != null) {
//            try {
//                dos.close();
//            } catch (IOException e) {
//                // TODO Auto-generated catch block
//                e.printStackTrace();
//            }
//        }
//    }
}

void CActivityManagerService::SystemReady(
    /* [in] */ IRunnable* goingCallback)
{
//    // In the simulator, startRunning will never have been called, which
//    // normally sets a few crucial variables. Do it here instead.
//    if (!Process.supportsProcesses()) {
//        mStartRunning = true;
//        mTopAction = Intent.ACTION_MAIN;
//    }
//
//    synchronized(this) {
//        if (mSystemReady) {
//            if (goingCallback != null) goingCallback.run();
//            return;
//        }
//
//        // Check to see if there are any update receivers to run.
//        if (!mDidUpdate) {
//            if (mWaitingUpdate) {
//                return;
//            }
//            Intent intent = new Intent(Intent.ACTION_PRE_BOOT_COMPLETED);
//            List<ResolveInfo> ris = null;
//            try {
//                ris = AppGlobals.getPackageManager().queryIntentReceivers(
//                            intent, null, 0);
//            } catch (RemoteException e) {
//            }
//            if (ris != null) {
//                for (int i=ris.size()-1; i>=0; i--) {
//                    if ((ris.get(i).activityInfo.applicationInfo.flags
//                            &ApplicationInfo.FLAG_SYSTEM) == 0) {
//                        ris.remove(i);
//                    }
//                }
//                intent.addFlags(Intent.FLAG_RECEIVER_BOOT_UPGRADE);
//
//                ArrayList<ComponentName> lastDoneReceivers = readLastDonePreBootReceivers();
//
//                final ArrayList<ComponentName> doneReceivers = new ArrayList<ComponentName>();
//                for (int i=0; i<ris.size(); i++) {
//                    ActivityInfo ai = ris.get(i).activityInfo;
//                    ComponentName comp = new ComponentName(ai.packageName, ai.name);
//                    if (lastDoneReceivers.contains(comp)) {
//                        ris.remove(i);
//                        i--;
//                    }
//                }
//
//                for (int i=0; i<ris.size(); i++) {
//                    ActivityInfo ai = ris.get(i).activityInfo;
//                    ComponentName comp = new ComponentName(ai.packageName, ai.name);
//                    doneReceivers.add(comp);
//                    intent.setComponent(comp);
//                    IIntentReceiver finisher = null;
//                    if (i == ris.size()-1) {
//                        finisher = new IIntentReceiver.Stub() {
//                            public void performReceive(Intent intent, int resultCode,
//                                    String data, Bundle extras, boolean ordered,
//                                    boolean sticky) {
//                                // The raw IIntentReceiver interface is called
//                                // with the AM lock held, so redispatch to
//                                // execute our code without the lock.
//                                mHandler.post(new Runnable() {
//                                    public void run() {
//                                        synchronized (ActivityManagerService.this) {
//                                            mDidUpdate = true;
//                                        }
//                                        writeLastDonePreBootReceivers(doneReceivers);
//                                        systemReady(goingCallback);
//                                    }
//                                });
//                            }
//                        };
//                    }
//                    Slog.i(TAG, "Sending system update to: " + intent.getComponent());
//                    broadcastIntentLocked(null, null, intent, null, finisher,
//                            0, null, null, null, true, false, MY_PID, Process.SYSTEM_UID);
//                    if (finisher != null) {
//                        mWaitingUpdate = true;
//                    }
//                }
//            }
//            if (mWaitingUpdate) {
//                return;
//            }
//            mDidUpdate = true;
//        }
//
//        mSystemReady = true;
//        if (!mStartRunning) {
//            return;
//        }
//    }
//
//    ArrayList<ProcessRecord> procsToKill = null;
//    synchronized(mPidsSelfLocked) {
//        for (int i=mPidsSelfLocked.size()-1; i>=0; i--) {
//            ProcessRecord proc = mPidsSelfLocked.valueAt(i);
//            if (!isAllowedWhileBooting(proc.info)){
//                if (procsToKill == null) {
//                    procsToKill = new ArrayList<ProcessRecord>();
//                }
//                procsToKill.add(proc);
//            }
//        }
//    }
//
//    synchronized(this) {
//        if (procsToKill != null) {
//            for (int i=procsToKill.size()-1; i>=0; i--) {
//                ProcessRecord proc = procsToKill.get(i);
//                Slog.i(TAG, "Removing system update proc: " + proc);
//                removeProcessLocked(proc, true);
//            }
//        }
//
//        // Now that we have cleaned up any update processes, we
//        // are ready to start launching real processes and know that
//        // we won't trample on them any more.
//        mProcessesReady = true;
//    }
//
//    Slog.i(TAG, "System now ready");
//    EventLog.writeEvent(EventLogTags.BOOT_PROGRESS_AMS_READY,
//        SystemClock.uptimeMillis());
//
//    synchronized(this) {
//        // Make sure we have no pre-ready processes sitting around.
//
//        if (mFactoryTest == SystemServer.FACTORY_TEST_LOW_LEVEL) {
//            ResolveInfo ri = mContext.getPackageManager()
//                    .resolveActivity(new Intent(Intent.ACTION_FACTORY_TEST),
//                            STOCK_PM_FLAGS);
//            CharSequence errorMsg = null;
//            if (ri != null) {
//                ActivityInfo ai = ri.activityInfo;
//                ApplicationInfo app = ai.applicationInfo;
//                if ((app.flags&ApplicationInfo.FLAG_SYSTEM) != 0) {
//                    mTopAction = Intent.ACTION_FACTORY_TEST;
//                    mTopData = null;
//                    mTopComponent = new ComponentName(app.packageName,
//                            ai.name);
//                } else {
//                    errorMsg = mContext.getResources().getText(
//                            com.android.internal.R.string.factorytest_not_system);
//                }
//            } else {
//                errorMsg = mContext.getResources().getText(
//                        com.android.internal.R.string.factorytest_no_action);
//            }
//            if (errorMsg != null) {
//                mTopAction = null;
//                mTopData = null;
//                mTopComponent = null;
//                Message msg = Message.obtain();
//                msg.what = SHOW_FACTORY_ERROR_MSG;
//                msg.getData().putCharSequence("msg", errorMsg);
//                mHandler.sendMessage(msg);
//            }
//        }
//    }
//
//    retrieveSettings();
//
//    if (goingCallback != null) goingCallback.run();
//
//    synchronized (this) {
//        if (mFactoryTest != SystemServer.FACTORY_TEST_LOW_LEVEL) {
//            try {
//                List apps = AppGlobals.getPackageManager().
//                    getPersistentApplications(STOCK_PM_FLAGS);
//                if (apps != null) {
//                    int N = apps.size();
//                    int i;
//                    for (i=0; i<N; i++) {
//                        ApplicationInfo info
//                            = (ApplicationInfo)apps.get(i);
//                        if (info != null &&
//                                !info.packageName.equals("android")) {
//                            addAppLocked(info);
//                        }
//                    }
//                }
//            } catch (RemoteException ex) {
//                // pm is in same process, this will never happen.
//            }
//        }
//
//        // Start up initial activity.
//        mBooting = true;
//
//        try {
//            if (AppGlobals.getPackageManager().hasSystemUidErrors()) {
//                Message msg = Message.obtain();
//                msg.what = SHOW_UID_ERROR_MSG;
//                mHandler.sendMessage(msg);
//            }
//        } catch (RemoteException e) {
//        }
//
//        mMainStack.resumeTopActivityLocked(null);
//    }
}

Boolean CActivityManagerService::MakeAppCrashingLocked(
    /* [in] */ ProcessRecord* app,
    /* [in] */ const String& shortMsg,
    /* [in] */ const String& longMsg,
    /* [in] */ const String& stackTrace)
{
//    app.crashing = true;
//    app.crashingReport = generateProcessError(app,
//            ActivityManager.ProcessErrorStateInfo.CRASHED, null, shortMsg, longMsg, stackTrace);
//    startAppProblemLocked(app);
//    app.stopFreezingAllLocked();
//    return handleAppCrashLocked(app);
    return TRUE;
}

void CActivityManagerService::MakeAppNotRespondingLocked(
    /* [in] */ ProcessRecord* app,
    /* [in] */ const String& activity,
    /* [in] */ const String& shortMsg,
    /* [in] */ const String& longMsg)
{
//    app.notResponding = true;
//    app.notRespondingReport = generateProcessError(app,
//            ActivityManager.ProcessErrorStateInfo.NOT_RESPONDING,
//            activity, shortMsg, longMsg, null);
//    startAppProblemLocked(app);
//    app.stopFreezingAllLocked();
}

/**
 * Generate a process error record, suitable for attachment to a ProcessRecord.
 *
 * @param app The ProcessRecord in which the error occurred.
 * @param condition Crashing, Application Not Responding, etc.  Values are defined in
 *                      ActivityManager.AppErrorStateInfo
 * @param activity The activity associated with the crash, if known.
 * @param shortMsg Short message describing the crash.
 * @param longMsg Long message describing the crash.
 * @param stackTrace Full crash stack trace, may be null.
 *
 * @return Returns a fully-formed AppErrorStateInfo record.
 */
void CActivityManagerService::GenerateProcessError(
    /* [in] */ ProcessRecord* app,
    /* [in] */ Int32 condition,
    /* [in] */ const String& activity,
    /* [in] */ const String& shortMsg,
    /* [in] */ const String& longMsg,
    /* [in] */ const String& stackTrace,
    /* [out] */ CProcessErrorStateInfo** report)
{
//    ActivityManager.ProcessErrorStateInfo report = new ActivityManager.ProcessErrorStateInfo();
//
//    report.condition = condition;
//    report.processName = app.processName;
//    report.pid = app.pid;
//    report.uid = app.info.uid;
//    report.tag = activity;
//    report.shortMsg = shortMsg;
//    report.longMsg = longMsg;
//    report.stackTrace = stackTrace;
//
//    return report;
}

void CActivityManagerService::KillAppAtUsersRequest(
    /* [in] */ ProcessRecord* app//,
    /* Dialog fromDialog */)
{
//    synchronized (this) {
//        app.crashing = false;
//        app.crashingReport = null;
//        app.notResponding = false;
//        app.notRespondingReport = null;
//        if (app.anrDialog == fromDialog) {
//            app.anrDialog = null;
//        }
//        if (app.waitDialog == fromDialog) {
//            app.waitDialog = null;
//        }
//        if (app.pid > 0 && app.pid != MY_PID) {
//            handleAppCrashLocked(app);
//            Slog.i(ActivityManagerService.TAG, "Killing "
//                    + app.processName + " (pid=" + app.pid + "): user's request");
//            EventLog.writeEvent(EventLogTags.AM_KILL, app.pid,
//                    app.processName, app.setAdj, "user's request after error");
//            Process.killProcess(app.pid);
//        }
//    }
}

Boolean CActivityManagerService::HandleAppCrashLocked(
    /* [in] */ ProcessRecord* app)
{
//    long now = SystemClock.uptimeMillis();
//
//    Long crashTime = mProcessCrashTimes.get(app.info.processName,
//            app.info.uid);
//    if (crashTime != null && now < crashTime+MIN_CRASH_INTERVAL) {
//        // This process loses!
//        Slogger::W(TAG, "Process " + app.info.processName
//                + " has crashed too many times: killing!");
//        EventLog.writeEvent(EventLogTags.AM_PROCESS_CRASHED_TOO_MUCH,
//                app.info.processName, app.info.uid);
//        killServicesLocked(app, false);
//        for (int i=mMainStack.mHistory.size()-1; i>=0; i--) {
//            ActivityRecord r = (ActivityRecord)mMainStack.mHistory.get(i);
//            if (r.app == app) {
//                Slogger::W(TAG, "  Force finishing activity "
//                    + r.intent.getComponent().flattenToShortString());
//                r.stack.finishActivityLocked(r, i, Activity.RESULT_CANCELED, null, "crashed");
//            }
//        }
//        if (!app.persistent) {
//            // We don't want to start this process again until the user
//            // explicitly does so...  but for persistent process, we really
//            // need to keep it running.  If a persistent process is actually
//            // repeatedly crashing, then badness for everyone.
//            EventLog.writeEvent(EventLogTags.AM_PROC_BAD, app.info.uid,
//                    app.info.processName);
//            mBadProcesses.put(app.info.processName, app.info.uid, now);
//            app.bad = true;
//            mProcessCrashTimes.remove(app.info.processName, app.info.uid);
//            app.removed = true;
//            removeProcessLocked(app, false);
//            return false;
//        }
//    } else {
//        ActivityRecord r = mMainStack.topRunningActivityLocked(null);
//        if (r.app == app) {
//            // If the top running activity is from this crashing
//            // process, then terminate it to avoid getting in a loop.
//            Slogger::W(TAG, "  Force finishing activity "
//                    + r.intent.getComponent().flattenToShortString());
//            int index = mMainStack.indexOfTokenLocked(r);
//            r.stack.finishActivityLocked(r, index,
//                    Activity.RESULT_CANCELED, null, "crashed");
//            // Also terminate an activities below it that aren't yet
//            // stopped, to avoid a situation where one will get
//            // re-start our crashing activity once it gets resumed again.
//            index--;
//            if (index >= 0) {
//                r = (ActivityRecord)mMainStack.mHistory.get(index);
//                if (r.state == ActivityState.RESUMED
//                        || r.state == ActivityState.PAUSING
//                        || r.state == ActivityState.PAUSED) {
//                    if (!r.isHomeActivity) {
//                        Slogger::W(TAG, "  Force finishing activity "
//                                + r.intent.getComponent().flattenToShortString());
//                        r.stack.finishActivityLocked(r, index,
//                                Activity.RESULT_CANCELED, null, "crashed");
//                    }
//                }
//            }
//        }
//    }
//
//    // Bump up the crash count of any services currently running in the proc.
//    if (app.services.size() != 0) {
//        // Any services running in the application need to be placed
//        // back in the pending list.
//        Iterator<ServiceRecord> it = app.services.iterator();
//        while (it.hasNext()) {
//            ServiceRecord sr = it.next();
//            sr.crashCount++;
//        }
//    }
//
//    mProcessCrashTimes.put(app.info.processName, app.info.uid, now);
//    return true;
    return TRUE;
}

void CActivityManagerService::StartAppProblemLocked(
    /* [in] */ ProcessRecord* app)
{
//    app.errorReportReceiver = ApplicationErrorReport.getErrorReportReceiver(
//            mContext, app.info.packageName, app.info.flags);
//    skipCurrentReceiverLocked(app);
}

void CActivityManagerService::SkipCurrentReceiverLocked(
    /* [in] */ ProcessRecord* app)
{
    Boolean reschedule = FALSE;
    BroadcastRecord* r = app->mCurReceiver;
    if (r != NULL) {
        // The current broadcast is waiting for this app's receiver
        // to be finished.  Looks like that's not going to happen, so
        // let the broadcast continue.
        LogBroadcastReceiverDiscardLocked(r);
        FinishReceiverLocked(r->mReceiver, r->mResultCode, r->mResultData,
                r->mResultExtras, r->mResultAbort, TRUE);
        reschedule = TRUE;
    }
    r = mPendingBroadcast;
    if (r != NULL && r->mCurApp == app) {
        if (DEBUG_BROADCAST) {
            String brDes;
            r->GetDescription(&brDes);
            Slogger::V(TAG, StringBuffer("skip & discard pending app ") + brDes);
        }
        LogBroadcastReceiverDiscardLocked(r);
        FinishReceiverLocked(r->mReceiver, r->mResultCode, r->mResultData,
                r->mResultExtras, r->mResultAbort, TRUE);
        reschedule = TRUE;
    }
    if (reschedule) {
        ScheduleBroadcastsLocked();
    }
}

/**
 * Used by {@link com.android.internal.os.RuntimeInit} to report when an application crashes.
 * The application process will exit immediately after this call returns.
 * @param app object of the crashing app, null for the system server
 * @param crashInfo describing the exception
 */
ECode CActivityManagerService::HandleApplicationCrash(
    /* [in] */ IBinder* app,
    /* [in] */ CCrashInfo* crashInfo)
{
//    ProcessRecord r = findAppProcess(app);
//
//    EventLog.writeEvent(EventLogTags.AM_CRASH, Binder.getCallingPid(),
//            app == null ? "system" : (r == null ? "unknown" : r.processName),
//            r == null ? -1 : r.info.flags,
//            crashInfo.exceptionClassName,
//            crashInfo.exceptionMessage,
//            crashInfo.throwFileName,
//            crashInfo.throwLineNumber);
//
//    addErrorToDropBox("crash", r, null, null, null, null, null, crashInfo);
//
//    crashApplication(r, crashInfo);
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::HandleApplicationStrictModeViolation(
    /* [in] */ IBinder* app,
    /* [in] */ Int32 violationMask//,
    /* StrictMode.ViolationInfo info */)
{
//    ProcessRecord r = findAppProcess(app);
//
//    if ((violationMask & StrictMode.PENALTY_DROPBOX) != 0) {
//        Integer stackFingerprint = info.crashInfo.stackTrace.hashCode();
//        boolean logIt = true;
//        synchronized (mAlreadyLoggedViolatedStacks) {
//            if (mAlreadyLoggedViolatedStacks.contains(stackFingerprint)) {
//                logIt = false;
//                // TODO: sub-sample into EventLog for these, with
//                // the info.durationMillis?  Then we'd get
//                // the relative pain numbers, without logging all
//                // the stack traces repeatedly.  We'd want to do
//                // likewise in the client code, which also does
//                // dup suppression, before the Binder call.
//            } else {
//                if (mAlreadyLoggedViolatedStacks.size() >= MAX_DUP_SUPPRESSED_STACKS) {
//                    mAlreadyLoggedViolatedStacks.clear();
//                }
//                mAlreadyLoggedViolatedStacks.add(stackFingerprint);
//            }
//        }
//        if (logIt) {
//            logStrictModeViolationToDropBox(r, info);
//        }
//    }
//
//    if ((violationMask & StrictMode.PENALTY_DIALOG) != 0) {
//        AppErrorResult result = new AppErrorResult();
//        synchronized (this) {
//            final long origId = Binder.clearCallingIdentity();
//
//            Message msg = Message.obtain();
//            msg.what = SHOW_STRICT_MODE_VIOLATION_MSG;
//            HashMap<String, Object> data = new HashMap<String, Object>();
//            data.put("result", result);
//            data.put("app", r);
//            data.put("violationMask", violationMask);
//            data.put("info", info);
//            msg.obj = data;
//            mHandler.sendMessage(msg);
//
//            Binder.restoreCallingIdentity(origId);
//        }
//        int res = result.get();
//        Slogger::W(TAG, "handleApplicationStrictModeViolation; res=" + res);
//    }
    return E_NOT_IMPLEMENTED;
}

// Depending on the policy in effect, there could be a bunch of
// these in quick succession so we try to batch these together to
// minimize disk writes, number of dropbox entries, and maximize
// compression, by having more fewer, larger records.
void CActivityManagerService::LogStrictModeViolationToDropBox(
    /* [in] */ ProcessRecord* process//,
    /* StrictMode.ViolationInfo info */)
{
//    if (info == null) {
//        return;
//    }
//    final boolean isSystemApp = process == null ||
//            (process.info.flags & (ApplicationInfo.FLAG_SYSTEM |
//                                   ApplicationInfo.FLAG_UPDATED_SYSTEM_APP)) != 0;
//    final String dropboxTag = isSystemApp ? "system_app_strictmode" : "data_app_strictmode";
//    final DropBoxManager dbox = (DropBoxManager)
//            mContext.getSystemService(Context.DROPBOX_SERVICE);
//
//    // Exit early if the dropbox isn't configured to accept this report type.
//    if (dbox == null || !dbox.isTagEnabled(dropboxTag)) return;
//
//    boolean bufferWasEmpty;
//    boolean needsFlush;
//    final StringBuilder sb = isSystemApp ? mStrictModeBuffer : new StringBuilder(1024);
//    synchronized (sb) {
//        bufferWasEmpty = sb.length() == 0;
//        appendDropBoxProcessHeaders(process, sb);
//        sb.append("Build: ").append(Build.FINGERPRINT).append("\n");
//        sb.append("System-App: ").append(isSystemApp).append("\n");
//        sb.append("Uptime-Millis: ").append(info.violationUptimeMillis).append("\n");
//        if (info.violationNumThisLoop != 0) {
//            sb.append("Loop-Violation-Number: ").append(info.violationNumThisLoop).append("\n");
//        }
//        if (info != null && info.durationMillis != -1) {
//            sb.append("Duration-Millis: ").append(info.durationMillis).append("\n");
//        }
//        sb.append("\n");
//        if (info.crashInfo != null && info.crashInfo.stackTrace != null) {
//            sb.append(info.crashInfo.stackTrace);
//        }
//        sb.append("\n");
//
//        // Only buffer up to ~64k.  Various logging bits truncate
//        // things at 128k.
//        needsFlush = (sb.length() > 64 * 1024);
//    }
//
//    // Flush immediately if the buffer's grown too large, or this
//    // is a non-system app.  Non-system apps are isolated with a
//    // different tag & policy and not batched.
//    //
//    // Batching is useful during internal testing with
//    // StrictMode settings turned up high.  Without batching,
//    // thousands of separate files could be created on boot.
//    if (!isSystemApp || needsFlush) {
//        new Thread("Error dump: " + dropboxTag) {
//            @Override
//            public void run() {
//                String report;
//                synchronized (sb) {
//                    report = sb.toString();
//                    sb.delete(0, sb.length());
//                    sb.trimToSize();
//                }
//                if (report.length() != 0) {
//                    dbox.addText(dropboxTag, report);
//                }
//            }
//        }.start();
//        return;
//    }
//
//    // System app batching:
//    if (!bufferWasEmpty) {
//        // An existing dropbox-writing thread is outstanding, so
//        // we don't need to start it up.  The existing thread will
//        // catch the buffer appends we just did.
//        return;
//    }
//
//    // Worker thread to both batch writes and to avoid blocking the caller on I/O.
//    // (After this point, we shouldn't access AMS internal data structures.)
//    new Thread("Error dump: " + dropboxTag) {
//        @Override
//        public void run() {
//            // 5 second sleep to let stacks arrive and be batched together
//            try {
//                Thread.sleep(5000);  // 5 seconds
//            } catch (InterruptedException e) {}
//
//            String errorReport;
//            synchronized (mStrictModeBuffer) {
//                errorReport = mStrictModeBuffer.toString();
//                if (errorReport.length() == 0) {
//                    return;
//                }
//                mStrictModeBuffer.delete(0, mStrictModeBuffer.length());
//                mStrictModeBuffer.trimToSize();
//            }
//            dbox.addText(dropboxTag, errorReport);
//        }
//    }.start();
}

/**
 * Used by {@link Log} via {@link com.android.internal.os.RuntimeInit} to report serious errors.
 * @param app object of the crashing app, null for the system server
 * @param tag reported by the caller
 * @param crashInfo describing the context of the error
 * @return true if the process should exit immediately (WTF is fatal)
 */
ECode CActivityManagerService::HandleApplicationWtf(
    /* [in] */ IBinder* app,
    /* [in] */ const String& tag,
    /* [in] */ CCrashInfo* crashInfo,
    /* [out] */ Boolean* result)
{
//    ProcessRecord r = findAppProcess(app);
//
//    EventLog.writeEvent(EventLogTags.AM_WTF, Binder.getCallingPid(),
//            app == null ? "system" : (r == null ? "unknown" : r.processName),
//            r == null ? -1 : r.info.flags,
//            tag, crashInfo.exceptionMessage);
//
//    addErrorToDropBox("wtf", r, null, null, tag, null, null, crashInfo);
//
//    if (r != null && r.pid != Process.myPid() &&
//            Settings.Secure.getInt(mContext.getContentResolver(),
//                    Settings.Secure.WTF_IS_FATAL, 0) != 0) {
//        crashApplication(r, crashInfo);
//        return true;
//    } else {
//        return false;
//    }
    return E_NOT_IMPLEMENTED;
}

/**
 * @param app object of some object (as stored in {@link com.android.internal.os.RuntimeInit})
 * @return the corresponding {@link ProcessRecord} object, or null if none could be found
 */
ProcessRecord* CActivityManagerService::FindAppProcess(
    /* [in] */ IBinder* app)
{
//    if (app == null) {
//        return null;
//    }
//
//    synchronized (this) {
//        for (SparseArray<ProcessRecord> apps : mProcessNames.getMap().values()) {
//            final int NA = apps.size();
//            for (int ia=0; ia<NA; ia++) {
//                ProcessRecord p = apps.valueAt(ia);
//                if (p.thread != null && p.thread.asBinder() == app) {
//                    return p;
//                }
//            }
//        }
//
//        Slogger::W(TAG, "Can't find mystery application: " + app);
//        return null;
//    }
    return NULL;
}

/**
 * Utility function for addErrorToDropBox and handleStrictModeViolation's logging
 * to append various headers to the dropbox log text.
 */
void CActivityManagerService::AppendDropBoxProcessHeaders(
    /* [in] */ ProcessRecord* process,
    /* [in] */ StringBuffer& sb)
{
//    // Note: ProcessRecord 'process' is guarded by the service
//    // instance.  (notably process.pkgList, which could otherwise change
//    // concurrently during execution of this method)
//    synchronized (this) {
//        if (process == null || process.pid == MY_PID) {
//            sb.append("Process: system_server\n");
//        } else {
//            sb.append("Process: ").append(process.processName).append("\n");
//        }
//        if (process == null) {
//            return;
//        }
//        int flags = process.info.flags;
//        IPackageManager pm = AppGlobals.getPackageManager();
//        sb.append("Flags: 0x").append(Integer.toString(flags, 16)).append("\n");
//        for (String pkg : process.pkgList) {
//            sb.append("Package: ").append(pkg);
//            try {
//                PackageInfo pi = pm.getPackageInfo(pkg, 0);
//                if (pi != null) {
//                    sb.append(" v").append(pi.versionCode);
//                    if (pi.versionName != null) {
//                        sb.append(" (").append(pi.versionName).append(")");
//                    }
//                }
//            } catch (RemoteException e) {
//                Slog.e(TAG, "Error getting package info: " + pkg, e);
//            }
//            sb.append("\n");
//        }
//    }
}

/**
 * Write a description of an error (crash, WTF, ANR) to the drop box.
 * @param eventType to include in the drop box tag ("crash", "wtf", etc.)
 * @param process which caused the error, null means the system server
 * @param activity which triggered the error, null if unknown
 * @param parent activity related to the error, null if unknown
 * @param subject line related to the error, null if absent
 * @param report in long form describing the error, null if absent
 * @param logFile to include in the report, null if none
 * @param crashInfo giving an application stack trace, null if absent
 */
void CActivityManagerService::AddErrorToDropBox(
    /* [in] */ const String& eventType,
    /* [in] */ ProcessRecord* process,
    /* [in] */ CActivityRecord* activity,
    /* [in] */ CActivityRecord* parent,
    /* [in] */ const String& subject,
    /* [in] */ const String& report,
//        final File logFile,
    /* [in] */ CCrashInfo* crashInfo)
{
//    // NOTE -- this must never acquire the ActivityManagerService lock,
//    // otherwise the watchdog may be prevented from resetting the system.
//
//    final String dropboxTag = processClass(process) + "_" + eventType;
//    final DropBoxManager dbox = (DropBoxManager)
//            mContext.getSystemService(Context.DROPBOX_SERVICE);
//
//    // Exit early if the dropbox isn't configured to accept this report type.
//    if (dbox == null || !dbox.isTagEnabled(dropboxTag)) return;
//
//    final StringBuilder sb = new StringBuilder(1024);
//    appendDropBoxProcessHeaders(process, sb);
//    if (activity != null) {
//        sb.append("Activity: ").append(activity.shortComponentName).append("\n");
//    }
//    if (parent != null && parent.app != null && parent.app.pid != process.pid) {
//        sb.append("Parent-Process: ").append(parent.app.processName).append("\n");
//    }
//    if (parent != null && parent != activity) {
//        sb.append("Parent-Activity: ").append(parent.shortComponentName).append("\n");
//    }
//    if (subject != null) {
//        sb.append("Subject: ").append(subject).append("\n");
//    }
//    sb.append("Build: ").append(Build.FINGERPRINT).append("\n");
//    sb.append("\n");
//
//    // Do the rest in a worker thread to avoid blocking the caller on I/O
//    // (After this point, we shouldn't access AMS internal data structures.)
//    Thread worker = new Thread("Error dump: " + dropboxTag) {
//        @Override
//        public void run() {
//            if (report != null) {
//                sb.append(report);
//            }
//            if (logFile != null) {
//                try {
//                    sb.append(FileUtils.readTextFile(logFile, 128 * 1024, "\n\n[[TRUNCATED]]"));
//                } catch (IOException e) {
//                    Slog.e(TAG, "Error reading " + logFile, e);
//                }
//            }
//            if (crashInfo != null && crashInfo.stackTrace != null) {
//                sb.append(crashInfo.stackTrace);
//            }
//
//            String setting = Settings.Secure.ERROR_LOGCAT_PREFIX + dropboxTag;
//            int lines = Settings.Secure.getInt(mContext.getContentResolver(), setting, 0);
//            if (lines > 0) {
//                sb.append("\n");
//
//                // Merge several logcat streams, and take the last N lines
//                InputStreamReader input = null;
//                try {
//                    java.lang.Process logcat = new ProcessBuilder("/system/bin/logcat",
//                            "-v", "time", "-b", "events", "-b", "system", "-b", "main",
//                            "-t", String.valueOf(lines)).redirectErrorStream(true).start();
//
//                    try { logcat.getOutputStream().close(); } catch (IOException e) {}
//                    try { logcat.getErrorStream().close(); } catch (IOException e) {}
//                    input = new InputStreamReader(logcat.getInputStream());
//
//                    int num;
//                    char[] buf = new char[8192];
//                    while ((num = input.read(buf)) > 0) sb.append(buf, 0, num);
//                } catch (IOException e) {
//                    Slog.e(TAG, "Error running logcat", e);
//                } finally {
//                    if (input != null) try { input.close(); } catch (IOException e) {}
//                }
//            }
//
//            dbox.addText(dropboxTag, sb.toString());
//        }
//    };
//
//    if (process == null || process.pid == MY_PID) {
//        worker.run();  // We may be about to die -- need to run this synchronously
//    } else {
//        worker.start();
//    }
}

/**
 * Bring up the "unexpected error" dialog box for a crashing app.
 * Deal with edge cases (intercepts from instrumented applications,
 * ActivityController, error intent receivers, that sort of thing).
 * @param r the application crashing
 * @param crashInfo describing the failure
 */
void CActivityManagerService::CrashApplication(
    /* [in] */ ProcessRecord* r,
    /* [in] */ CCrashInfo* crashInfo)
{
//    long timeMillis = System.currentTimeMillis();
//    String shortMsg = crashInfo.exceptionClassName;
//    String longMsg = crashInfo.exceptionMessage;
//    String stackTrace = crashInfo.stackTrace;
//    if (shortMsg != null && longMsg != null) {
//        longMsg = shortMsg + ": " + longMsg;
//    } else if (shortMsg != null) {
//        longMsg = shortMsg;
//    }
//
//    AppErrorResult result = new AppErrorResult();
//    synchronized (this) {
//        if (mController != null) {
//            try {
//                String name = r != null ? r.processName : null;
//                int pid = r != null ? r.pid : Binder.getCallingPid();
//                if (!mController.appCrashed(name, pid,
//                        shortMsg, longMsg, timeMillis, crashInfo.stackTrace)) {
//                    Slogger::W(TAG, "Force-killing crashed app " + name
//                            + " at watcher's request");
//                    Process.killProcess(pid);
//                    return;
//                }
//            } catch (RemoteException e) {
//                mController = null;
//            }
//        }
//
//        final long origId = Binder.clearCallingIdentity();
//
//        // If this process is running instrumentation, finish it.
//        if (r != null && r.instrumentationClass != null) {
//            Slogger::W(TAG, "Error in app " + r.processName
//                  + " running instrumentation " + r.instrumentationClass + ":");
//            if (shortMsg != null) Slogger::W(TAG, "  " + shortMsg);
//            if (longMsg != null) Slogger::W(TAG, "  " + longMsg);
//            Bundle info = new Bundle();
//            info.putString("shortMsg", shortMsg);
//            info.putString("longMsg", longMsg);
//            finishInstrumentationLocked(r, Activity.RESULT_CANCELED, info);
//            Binder.restoreCallingIdentity(origId);
//            return;
//        }
//
//        // If we can't identify the process or it's already exceeded its crash quota,
//        // quit right away without showing a crash dialog.
//        if (r == null || !makeAppCrashingLocked(r, shortMsg, longMsg, stackTrace)) {
//            Binder.restoreCallingIdentity(origId);
//            return;
//        }
//
//        Message msg = Message.obtain();
//        msg.what = SHOW_ERROR_MSG;
//        HashMap data = new HashMap();
//        data.put("result", result);
//        data.put("app", r);
//        msg.obj = data;
//        mHandler.sendMessage(msg);
//
//        Binder.restoreCallingIdentity(origId);
//    }
//
//    int res = result.get();
//
//    Intent appErrorIntent = null;
//    synchronized (this) {
//        if (r != null) {
//            mProcessCrashTimes.put(r.info.processName, r.info.uid,
//                    SystemClock.uptimeMillis());
//        }
//        if (res == AppErrorDialog.FORCE_QUIT_AND_REPORT) {
//            appErrorIntent = createAppErrorIntentLocked(r, timeMillis, crashInfo);
//        }
//    }
//
//    if (appErrorIntent != null) {
//        try {
//            mContext.startActivity(appErrorIntent);
//        } catch (ActivityNotFoundException e) {
//            Slogger::W(TAG, "bug report receiver dissappeared", e);
//        }
//    }
}

ECode CActivityManagerService::CreateAppErrorIntentLocked(
    /* [in] */ ProcessRecord* r,
    /* [in] */ Millisecond64 timeMillis,
    /* [in] */ CCrashInfo* crashInfo,
    /* [out] */ IIntent** intent)
{
//    ApplicationErrorReport report = createAppErrorReportLocked(r, timeMillis, crashInfo);
//    if (report == null) {
//        return null;
//    }
//    Intent result = new Intent(Intent.ACTION_APP_ERROR);
//    result.setComponent(r.errorReportReceiver);
//    result.putExtra(Intent.EXTRA_BUG_REPORT, report);
//    result.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//    return result;
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::CreateAppErrorReportLocked(
    /* [in] */ ProcessRecord* r,
    /* [in] */ Millisecond64 timeMillis,
    /* [in] */ CCrashInfo* crashInfo,
    /* [out] */ CApplicationErrorReport** report)
{
//    if (r.errorReportReceiver == null) {
//        return null;
//    }
//
//    if (!r.crashing && !r.notResponding) {
//        return null;
//    }
//
//    ApplicationErrorReport report = new ApplicationErrorReport();
//    report.packageName = r.info.packageName;
//    report.installerPackageName = r.errorReportReceiver.getPackageName();
//    report.processName = r.processName;
//    report.time = timeMillis;
//    report.systemApp = (r.info.flags & ApplicationInfo.FLAG_SYSTEM) != 0;
//
//    if (r.crashing) {
//        report.type = ApplicationErrorReport.TYPE_CRASH;
//        report.crashInfo = crashInfo;
//    } else if (r.notResponding) {
//        report.type = ApplicationErrorReport.TYPE_ANR;
//        report.anrInfo = new ApplicationErrorReport.AnrInfo();
//
//        report.anrInfo.activity = r.notRespondingReport.tag;
//        report.anrInfo.cause = r.notRespondingReport.shortMsg;
//        report.anrInfo.info = r.notRespondingReport.longMsg;
//    }
//
//    return report;
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::GetProcessesInErrorState(
    /* [out] */ IObjectContainer** procs)
{
//    // assume our apps are happy - lazy create the list
//    List<ActivityManager.ProcessErrorStateInfo> errList = null;
//
//    synchronized (this) {
//
//        // iterate across all processes
//        for (int i=mLruProcesses.size()-1; i>=0; i--) {
//            ProcessRecord app = mLruProcesses.get(i);
//            if ((app.thread != null) && (app.crashing || app.notResponding)) {
//                // This one's in trouble, so we'll generate a report for it
//                // crashes are higher priority (in case there's a crash *and* an anr)
//                ActivityManager.ProcessErrorStateInfo report = null;
//                if (app.crashing) {
//                    report = app.crashingReport;
//                } else if (app.notResponding) {
//                    report = app.notRespondingReport;
//                }
//
//                if (report != null) {
//                    if (errList == null) {
//                        errList = new ArrayList<ActivityManager.ProcessErrorStateInfo>(1);
//                    }
//                    errList.add(report);
//                } else {
//                    Slogger::W(TAG, "Missing app error report, app = " + app.processName +
//                            " crashing = " + app.crashing +
//                            " notResponding = " + app.notResponding);
//                }
//            }
//        }
//    }
//
//    return errList;
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::GetRunningAppProcesses(
    /* [out] */ IObjectContainer** appProcs)
{
//    // Lazy instantiation of list
//    List<ActivityManager.RunningAppProcessInfo> runList = null;
//    synchronized (this) {
//        // Iterate across all processes
//        for (int i=mLruProcesses.size()-1; i>=0; i--) {
//            ProcessRecord app = mLruProcesses.get(i);
//            if ((app.thread != null) && (!app.crashing && !app.notResponding)) {
//                // Generate process state info for running application
//                ActivityManager.RunningAppProcessInfo currApp =
//                    new ActivityManager.RunningAppProcessInfo(app.processName,
//                            app.pid, app.getPackageList());
//                currApp.uid = app.info.uid;
//                if (mHeavyWeightProcess == app) {
//                    currApp.flags |= ActivityManager.RunningAppProcessInfo.FLAG_CANT_SAVE_STATE;
//                }
//                if (app.persistent) {
//                    currApp.flags |= ActivityManager.RunningAppProcessInfo.FLAG_PERSISTENT;
//                }
//                int adj = app.curAdj;
//                if (adj >= EMPTY_APP_ADJ) {
//                    currApp.importance = ActivityManager.RunningAppProcessInfo.IMPORTANCE_EMPTY;
//                } else if (adj >= HIDDEN_APP_MIN_ADJ) {
//                    currApp.importance = ActivityManager.RunningAppProcessInfo.IMPORTANCE_BACKGROUND;
//                    currApp.lru = adj - HIDDEN_APP_MIN_ADJ + 1;
//                } else if (adj >= HOME_APP_ADJ) {
//                    currApp.importance = ActivityManager.RunningAppProcessInfo.IMPORTANCE_BACKGROUND;
//                    currApp.lru = 0;
//                } else if (adj >= SECONDARY_SERVER_ADJ) {
//                    currApp.importance = ActivityManager.RunningAppProcessInfo.IMPORTANCE_SERVICE;
//                } else if (adj >= HEAVY_WEIGHT_APP_ADJ) {
//                    currApp.importance = ActivityManager.RunningAppProcessInfo.IMPORTANCE_CANT_SAVE_STATE;
//                } else if (adj >= PERCEPTIBLE_APP_ADJ) {
//                    currApp.importance = ActivityManager.RunningAppProcessInfo.IMPORTANCE_PERCEPTIBLE;
//                } else if (adj >= VISIBLE_APP_ADJ) {
//                    currApp.importance = ActivityManager.RunningAppProcessInfo.IMPORTANCE_VISIBLE;
//                } else {
//                    currApp.importance = ActivityManager.RunningAppProcessInfo.IMPORTANCE_FOREGROUND;
//                }
//                currApp.importanceReasonCode = app.adjTypeCode;
//                if (app.adjSource instanceof ProcessRecord) {
//                    currApp.importanceReasonPid = ((ProcessRecord)app.adjSource).pid;
//                } else if (app.adjSource instanceof ActivityRecord) {
//                    ActivityRecord r = (ActivityRecord)app.adjSource;
//                    if (r.app != null) currApp.importanceReasonPid = r.app.pid;
//                }
//                if (app.adjTarget instanceof ComponentName) {
//                    currApp.importanceReasonComponent = (ComponentName)app.adjTarget;
//                }
//                //Slog.v(TAG, "Proc " + app.processName + ": imp=" + currApp.importance
//                //        + " lru=" + currApp.lru);
//                if (runList == null) {
//                    runList = new ArrayList<ActivityManager.RunningAppProcessInfo>();
//                }
//                runList.add(currApp);
//            }
//        }
//    }
//    return runList;
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::GetRunningExternalApplications(
    /* [out] */ IObjectContainer** apps)
{
//    List<ActivityManager.RunningAppProcessInfo> runningApps = getRunningAppProcesses();
//    List<ApplicationInfo> retList = new ArrayList<ApplicationInfo>();
//    if (runningApps != null && runningApps.size() > 0) {
//        Set<String> extList = new HashSet<String>();
//        for (ActivityManager.RunningAppProcessInfo app : runningApps) {
//            if (app.pkgList != null) {
//                for (String pkg : app.pkgList) {
//                    extList.add(pkg);
//                }
//            }
//        }
//        IPackageManager pm = AppGlobals.getPackageManager();
//        for (String pkg : extList) {
//            try {
//                ApplicationInfo info = pm.getApplicationInfo(pkg, 0);
//                if ((info.flags & ApplicationInfo.FLAG_EXTERNAL_STORAGE) != 0) {
//                    retList.add(info);
//                }
//            } catch (RemoteException e) {
//            }
//        }
//    }
//    return retList;
    return NOERROR;
}

void CActivityManagerService::KillServicesLocked(
    /* [in] */ ProcessRecord* app,
    /* [in] */ Boolean allowRestart)
{
    // Report disconnected services.
//    if (false) {
//        // XXX we are letting the client link to the service for
//        // death notifications.
//        if (app.services.size() > 0) {
//            Iterator<ServiceRecord> it = app.services.iterator();
//            while (it.hasNext()) {
//                ServiceRecord r = it.next();
//                if (r.connections.size() > 0) {
//                    Iterator<ArrayList<ConnectionRecord>> jt
//                            = r.connections.values().iterator();
//                    while (jt.hasNext()) {
//                        ArrayList<ConnectionRecord> cl = jt.next();
//                        for (int i=0; i<cl.size(); i++) {
//                            ConnectionRecord c = cl.get(i);
//                            if (c.binding.client != app) {
//                                try {
//                                    //c.conn.connected(r.className, null);
//                                } catch (Exception e) {
//                                    // todo: this should be asynchronous!
//                                    Slogger::W(TAG, "Exception thrown disconnected servce "
//                                          + r.shortName
//                                          + " from app " + app.processName, e);
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }

    // Clean up any connections this application has to other services.
    if (app->mConnections.GetSize() > 0) {
        Set<ConnectionRecord*>::Iterator it = app->mConnections.Begin();
        for (; it != app->mConnections.End(); ++it) {
            RemoveConnectionLocked(*it, app, NULL);
        }
    }
    app->mConnections.Clear();

    if (app->mServices.GetSize() != 0) {
        // Any services running in the application need to be placed
        // back in the pending list.
        Set<AutoPtr<CServiceRecord> >::Iterator it = app->mServices.Begin();
        for (; it != app->mServices.End(); ++it) {
            AutoPtr<CServiceRecord> sr = *it;
//            sr->mStats->GetBatteryStats()->Lock();
//            sr->mStats->StopLaunchedLocked();
//            sr->mStats->GetBatteryStats()->Unlock();
            sr->mApp = NULL;
            sr->mExecuteNesting = 0;
            List<AutoPtr<CServiceRecord> >::Iterator it = \
                    Find(mStoppingServices.Begin(), mStoppingServices.End(), sr);
            if (it != mStoppingServices.End()) {
                mStoppingServices.Erase(it);
                if (DEBUG_SERVICE) {
                    String srDes;
                    sr->GetDescription(&srDes);
                    Slogger::V(TAG, StringBuffer("killServices remove stopping ")
                            + srDes);
                }
            }

            Boolean hasClients = sr->mBindings.GetSize() > 0;
            if (hasClients) {
                HashMap<AutoPtr<IIntentFilterComparison>, IntentBindRecord*>::Iterator it = \
                        sr->mBindings.Begin();
                for (; it != sr->mBindings.End(); ++it) {
                    IntentBindRecord* b = it->mSecond;
                    if (DEBUG_SERVICE) {
                        String ibDes;
                        b->GetDescription(&ibDes);
                        Slogger::V(TAG, StringBuffer("Killing binding ") + ibDes
                            + ": shouldUnbind=" + b->mHasBound);
                    }
                    b->mBinder = NULL;
                    b->mRequested = b->mReceived = b->mHasBound = FALSE;
                }
            }

            if (sr->mCrashCount >= 2) {
                String srDes;
                sr->GetDescription(&srDes);
                Slogger::W(TAG, StringBuffer("Service crashed ") + sr->mCrashCount
                        + " times, stopping: " + srDes);
//                EventLog.writeEvent(EventLogTags.AM_SERVICE_CRASHED_TOO_MUCH,
//                        sr.crashCount, sr.shortName, app.pid);
                BringDownServiceLocked(sr, TRUE);
            }
            else if (!allowRestart) {
                BringDownServiceLocked(sr, TRUE);
            }
            else {
                Boolean canceled = ScheduleServiceRestartLocked(sr, TRUE);

                // Should the service remain running?  Note that in the
                // extreme case of so many attempts to deliver a command
                // that it failed, that we also will stop it here.
                if (sr->mStartRequested && (sr->mStopIfKilled || canceled)) {
                    if (sr->mPendingStarts.GetSize() == 0) {
                        sr->mStartRequested = FALSE;
                        if (!hasClients) {
                            // Whoops, no reason to restart!
                            BringDownServiceLocked(sr, TRUE);
                        }
                    }
                }
            }
        }

        if (!allowRestart) {
            app->mServices.Clear();
        }
    }

    // Make sure we have no more records on the stopping list.
    List<AutoPtr<CServiceRecord> >::ReverseIterator rit = mStoppingServices.RBegin();
    for (; rit != mStoppingServices.REnd(); ++rit) {
        if ((*rit)->mApp == app) {
            mStoppingServices.Erase(--rit.GetBase());
            if (DEBUG_SERVICE) {
                String srDes;
                (*rit)->GetDescription(&srDes);
                Slogger::V(TAG, StringBuffer("killServices remove stopping ") + srDes);
            }
        }
    }

    app->mExecutingServices.Clear();
}

void CActivityManagerService::RemoveDyingProviderLocked(
    /* [in] */ ProcessRecord* proc,
    /* [in] */ ContentProviderRecord* cpr)
{
//    synchronized (cpr) {
//        cpr.launchingApp = null;
//        cpr.notifyAll();
//    }
    AutoPtr<IContentProviderInfo> cprProviderInfo;
    cpr->mHolder->GetContentProviderInfo((IContentProviderInfo**)&cprProviderInfo);
    String cprName;
    cprProviderInfo->GetName(&cprName);
    mProvidersByClass.Erase(cprName);

    String cprAuth;
    cprProviderInfo->GetAuthority(&cprAuth);
    StringTokenizer st(cprAuth, String(";"));
    while (st.HasMoreTokens()) {
        mProvidersByName.Erase(st.NextToken());
    }

    Set<ProcessRecord*>::Iterator cit = cpr->mClients.Begin();
    for (; cit != cpr->mClients.End(); ++cit) {
        ProcessRecord* capp = *cit;
        if (!capp->mPersistent && capp->mAppApartment != NULL
                && capp->mPid != 0
                && capp->mPid != mMyPid) {
            Slogger::I(TAG, StringBuffer("Kill ") + capp->mProcessName
                    + " (pid " + capp->mPid + "): provider "
                    + cprName + " in dying process " + proc->mProcessName);
//            EventLog.writeEvent(EventLogTags.AM_KILL, capp.pid,
//                    capp.processName, capp.setAdj, "dying provider " + proc.processName);
            Process::KillProcess(capp->mPid);
        }
    }

    mLaunchingProviders.Remove(cpr);
}

/**
 * Main code for cleaning up a process when it has gone away.  This is
 * called both as a result of the process dying, or directly when stopping
 * a process when running in single process mode.
 */
void CActivityManagerService::CleanUpApplicationRecordLocked(
    /* [in] */ ProcessRecord* app,
    /* [in] */ Boolean restarting,
    /* [in] */ Int32 index)
{
    if (index >= 0) {
        mLRUProcesses.Remove(index);
    }

    mProcessesToGc.Remove(app);

    // Dismiss any open dialogs.
//    if (app.crashDialog != null) {
//        app.crashDialog.dismiss();
//        app.crashDialog = null;
//    }
//    if (app.anrDialog != null) {
//        app.anrDialog.dismiss();
//        app.anrDialog = null;
//    }
//    if (app.waitDialog != null) {
//        app.waitDialog.dismiss();
//        app.waitDialog = null;
//    }

    app->mCrashing = FALSE;
    app->mNotResponding = FALSE;

    app->ResetCapsuleList();
    app->mAppApartment = NULL;
    app->mForcingToForeground = NULL;
    app->mForegroundServices = FALSE;

    KillServicesLocked(app, TRUE);

    Boolean restart = FALSE;

    // Remove published content providers.
    if (!app->mPubProviders.IsEmpty()) {
        HashMap<String, ContentProviderRecord*>::Iterator it = \
                app->mPubProviders.Begin();
        for (; it != app->mPubProviders.End(); ++it) {
            ContentProviderRecord* cpr = it->mSecond;
            cpr->mHolder->SetContentProvider(NULL);
            cpr->mApp = NULL;

            // See if someone is waiting for this provider...  in which
            // case we don't remove it, but just let it restart.
            if (!app->mBad) {
                List<ContentProviderRecord*>::Iterator it = \
                        mLaunchingProviders.Begin();
                for (; it != mLaunchingProviders.End(); ++it) {
                    if (*it == cpr) {
                        restart = TRUE;
                        break;
                    }
                }
            }

            if (!restart) {
                RemoveDyingProviderLocked(app, cpr);
            }
        }
        app->mPubProviders.Clear();
    }

    // Take care of any launching providers waiting for this process.
    if (CheckAppInLaunchingProvidersLocked(app, FALSE)) {
        restart = TRUE;
    }

    // Unregister from connected content providers.
    if (!app->mConProviders.IsEmpty()) {
        HashMap<ContentProviderRecord*, Int32,
                Hash<ContentProviderRecord*> >::Iterator it = app->mConProviders.Begin();
        for (; it != app->mConProviders.End(); ++it) {
            it->mFirst->mClients.Erase(app);
        }
        app->mConProviders.Clear();
    }

//    // At this point there may be remaining entries in mLaunchingProviders
//    // where we were the only one waiting, so they are no longer of use.
//    // Look for these and clean up if found.
//    // XXX Commented out for now.  Trying to figure out a way to reproduce
//    // the actual situation to identify what is actually going on.
//    if (false) {
//        for (int i=0; i<NL; i++) {
//            ContentProviderRecord cpr = (ContentProviderRecord)
//                    mLaunchingProviders.get(i);
//            if (cpr.clients.size() <= 0 && cpr.externals <= 0) {
//                synchronized (cpr) {
//                    cpr.launchingApp = null;
//                    cpr.notifyAll();
//                }
//            }
//        }
//    }

    SkipCurrentReceiverLocked(app);

    // Unregister any receivers.
    if (app->mReceivers.GetSize() > 0) {
        Set<ReceiverList*>::Iterator it = app->mReceivers.Begin();
        for (; it != app->mReceivers.End(); ++it) {
            RemoveReceiverLocked(*it);
        }
        app->mReceivers.Clear();
    }

    // If the app is undergoing backup, tell the backup manager about it
    if (mBackupTarget != NULL && app->mPid == mBackupTarget->mApp->mPid) {
        if (DEBUG_BACKUP) {
            String aiDes;
            mBackupTarget->mAppInfo->GetDescription(&aiDes);
            Slogger::D(TAG, StringBuffer("App ") + aiDes + " died during backup");
        }
//        try {
            AutoPtr<IServiceManager> sm;
            AutoPtr<IBackupManager> bm;
            Elastos::GetServiceManager((IServiceManager**)&sm);
            assert(sm != NULL);
	        sm->GetService(String("Context.BACKUP_SERVICE"),
	                (IInterface**)(IBackupManager**)&bm);
            String appCName;
            app->mInfo->GetCapsuleName(&appCName);
            bm->AgentDisconnected(appCName);
//        } catch (RemoteException e) {
//            // can't happen; backup manager is local
//        }
    }

    // If the caller is restarting this app, then leave it in its
    // current lists and let the caller take care of it.
    if (restarting) {
        return;
    }

    if (!app->mPersistent) {
        if (DEBUG_PROCESSES) {
            String appDes;
            app->GetDescription(&appDes);
            Slogger::V(TAG, StringBuffer("Removing non-persistent process during cleanup: ")
                    + appDes);
        }
        Int32 appUid;
        app->mInfo->GetUid(&appUid);
        mProcessNames->Remove(app->mProcessName, appUid);
        if (mHeavyWeightProcess == app) {
            mHeavyWeightProcess = NULL;
//            mHandler.sendEmptyMessage(CANCEL_HEAVY_NOTIFICATION_MSG);
        }
    }
    else if (!app->mRemoved) {
        // This app is persistent, so we need to keep its record around.
        // If it is not already on the pending app list, add it there
        // and start a new process for it.
        app->mAppApartment = NULL;
        app->mForcingToForeground = NULL;
        app->mForegroundServices = FALSE;
        if (Find(mPersistentStartingProcesses.Begin(), mPersistentStartingProcesses.End(), app)
                == mPersistentStartingProcesses.End()) {
            mPersistentStartingProcesses.PushBack(app);
            restart = TRUE;
        }
    }
    if (DEBUG_PROCESSES && (Find(mProcessesOnHold.Begin(), mProcessesOnHold.End(), app)
            != mProcessesOnHold.End())) {
        String appDes;
        app->GetDescription(&appDes);
        Slogger::V(TAG, StringBuffer("Clean-up removing on hold: ") + appDes);
    }
    mProcessesOnHold.Remove(app);

    if (app == mHomeProcess) {
        mHomeProcess = NULL;
    }

    if (restart) {
        // We have components that still need to be running in the
        // process, so re-launch it.
        Int32 appUid;
        app->mInfo->GetUid(&appUid);
        mProcessNames->Put(app->mProcessName, appUid, app);
        StartProcessLocked(app, "restart", app->mProcessName);
    }
    else if (app->mPid > 0 && app->mPid != mMyPid) {
        // Goodbye!
        {
            Mutex::Autolock lock(mPidsSelfLock);

            mPidsSelfLocked.Erase(app->mPid);
//            mHandler.removeMessages(PROC_START_TIMEOUT_MSG, app);
        }
        app->SetPid(0);
    }
}

Boolean CActivityManagerService::CheckAppInLaunchingProvidersLocked(
    /* [in] */ ProcessRecord* app,
    /* [in] */ Boolean alwaysBad)
{
    // Look through the content providers we are waiting to have launched,
    // and if any run in this process then either schedule a restart of
    // the process or kill the client waiting for it if this process has
    // gone bad.
    Boolean restart = FALSE;
    List<ContentProviderRecord*>::Iterator it = mLaunchingProviders.Begin();
    for (; it != mLaunchingProviders.End(); ++it) {
        ContentProviderRecord* cpr = *it;
        if (cpr->mLaunchingApp == app) {
            if (!alwaysBad && !app->mBad) {
                restart = TRUE;
            }
            else {
                RemoveDyingProviderLocked(app, cpr);
            }
        }
    }
    return restart;
}

// =========================================================
// SERVICE
// =========================================================
CRunningServiceInfo* CActivityManagerService::MakeRunningServiceInfoLocked(
    /* [in] */ CServiceRecord* r)
{
//    ActivityManager.RunningServiceInfo info =
//        new ActivityManager.RunningServiceInfo();
//    info.service = r.name;
//    if (r.app != null) {
//        info.pid = r.app.pid;
//    }
//    info.uid = r.appInfo.uid;
//    info.process = r.processName;
//    info.foreground = r.isForeground;
//    info.activeSince = r.createTime;
//    info.started = r.startRequested;
//    info.clientCount = r.connections.size();
//    info.crashCount = r.crashCount;
//    info.lastActivityTime = r.lastActivity;
//    if (r.isForeground) {
//        info.flags |= ActivityManager.RunningServiceInfo.FLAG_FOREGROUND;
//    }
//    if (r.startRequested) {
//        info.flags |= ActivityManager.RunningServiceInfo.FLAG_STARTED;
//    }
//    if (r.app != null && r.app.pid == MY_PID) {
//        info.flags |= ActivityManager.RunningServiceInfo.FLAG_SYSTEM_PROCESS;
//    }
//    if (r.app != null && r.app.persistent) {
//        info.flags |= ActivityManager.RunningServiceInfo.FLAG_PERSISTENT_PROCESS;
//    }
//
//    for (ArrayList<ConnectionRecord> connl : r.connections.values()) {
//        for (int i=0; i<connl.size(); i++) {
//            ConnectionRecord conn = connl.get(i);
//            if (conn.clientLabel != 0) {
//                info.clientPackage = conn.binding.client.info.packageName;
//                info.clientLabel = conn.clientLabel;
//                return info;
//            }
//        }
//    }
//    return info;
    return NULL;
}

ECode CActivityManagerService::GetServices(
    /* [in] */ Int32 maxNum,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** services)
{
//    synchronized (this) {
//        ArrayList<ActivityManager.RunningServiceInfo> res
//                = new ArrayList<ActivityManager.RunningServiceInfo>();
//
//        if (mServices.size() > 0) {
//            Iterator<ServiceRecord> it = mServices.values().iterator();
//            while (it.hasNext() && res.size() < maxNum) {
//                res.add(makeRunningServiceInfoLocked(it.next()));
//            }
//        }
//
//        for (int i=0; i<mRestartingServices.size() && res.size() < maxNum; i++) {
//            ServiceRecord r = mRestartingServices.get(i);
//            ActivityManager.RunningServiceInfo info =
//                    makeRunningServiceInfoLocked(r);
//            info.restarting = r.nextRestartTime;
//            res.add(info);
//        }
//
//        return res;
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::GetRunningServiceControlPanel(
    /* [in] */ IComponentName* name,
    /* [out] */ IPendingIntent** intent)
{
//    synchronized (this) {
//        ServiceRecord r = mServices.get(name);
//        if (r != null) {
//            for (ArrayList<ConnectionRecord> conn : r.connections.values()) {
//                for (int i=0; i<conn.size(); i++) {
//                    if (conn.get(i).clientIntent != null) {
//                        return conn.get(i).clientIntent;
//                    }
//                }
//            }
//        }
//    }
//    return null;
    return E_NOT_IMPLEMENTED;
}

CServiceRecord* CActivityManagerService::FindServiceLocked(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* token)
{
     CServiceRecord* r = mServices[name];
     return (IBinder*)r == token ? r : NULL;
}

CActivityManagerService::ServiceLookupResult*
CActivityManagerService::FindServiceLocked(
    /* [in] */ IIntent* service,
    /* [in] */ const String& resolvedType)
{
    AutoPtr<CServiceRecord> r;
    AutoPtr<IComponentName> comp;
    service->GetComponent((IComponentName**)&comp);
    if (comp != NULL) {
        r = mServices[comp];
    }
    if (r == NULL) {
        AutoPtr<IIntentFilterComparison> filter;
        CIntentFilterComparison::New(service, (IIntentFilterComparison**)&filter);
        r = mServicesByIntent[filter];
    }

    if (r == NULL) {
        AutoPtr<IResolveInfo> rInfo;
        GetCapsuleManager()->ResolveService(
                service, resolvedType, 0, (IResolveInfo**)&rInfo);
        AutoPtr<IServiceInfo> sInfo;
        if (rInfo != NULL) {
            rInfo->GetServiceInfo((IServiceInfo**)&sInfo);
        }
        if (sInfo == NULL) {
            return NULL;
        }

        AutoPtr<IApplicationInfo> appInfo;
        sInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
        String scname, sname;
        appInfo->GetCapsuleName(&scname);
        sInfo->GetName(&sname);
        AutoPtr<IComponentName> name;
        CComponentName::New(scname, sname, (IComponentName**)&name);
        r = mServices[name];
    }
    if (r != NULL) {
        Int32 callingPid = 0; /*Binder.getCallingPid();*/
        Int32 callingUid = 0; /*Binder.getCallingUid();*/
        Int32 rUid;
        r->mAppInfo->GetUid(&rUid);
        if (CheckComponentPermission(r->mPermission,
                callingPid, callingUid, r->mExported ? -1 : rUid)
                != CapsuleManager_PERMISSION_GRANTED) {
            String srDes;
            r->mName->GetDescription(&srDes);
            Slogger::W(TAG, StringBuffer("Permission Denial: Accessing service ")
                    + srDes + " from pid=" + callingPid
                    + ", uid=" + callingUid
                    + " requires " + r->mPermission);
            return new ServiceLookupResult(NULL, r->mPermission);
        }
        return new ServiceLookupResult(r, String(NULL));
    }
    return NULL;
}

CActivityManagerService::ServiceLookupResult*
CActivityManagerService::RetrieveServiceLocked(
    /* [in] */ IIntent* service,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 callingPid,
    /* [in] */ Int32 callingUid)
{
    AutoPtr<CServiceRecord> r;
    AutoPtr<IComponentName> name;
    service->GetComponent((IComponentName**)&name);
    if (name != NULL) {
        HashMap<AutoPtr<IComponentName>, AutoPtr<CServiceRecord> >::Iterator it =
                mServices.Find(name);
        if (it != mServices.End()) {
            r = it->mSecond;
        }
    }
    if (r == NULL) {
        AutoPtr<IIntentFilterComparison> filter;
        CIntentFilterComparison::New(service, (IIntentFilterComparison**)&filter);
        HashMap<AutoPtr<IIntentFilterComparison>, AutoPtr<CServiceRecord> >::Iterator it =
                mServicesByIntent.Find(filter);
        if (it != mServicesByIntent.End()) {
            r = it->mSecond;
        }
        if (r == NULL) {
            AutoPtr<IResolveInfo> rInfo;
            GetCapsuleManager()->ResolveService(service, resolvedType,
                    STOCK_PM_FLAGS, (IResolveInfo**)&rInfo);
            AutoPtr<IServiceInfo> sInfo;
            if (rInfo != NULL) {
                rInfo->GetServiceInfo((IServiceInfo**)&sInfo);
            }
            if (sInfo == NULL) {
                String intDes;
                service->GetDescription(&intDes);
                Slogger::W(TAG, StringBuffer("Unable to start service ") + intDes +
                      ": not found");
                return NULL;
            }

            AutoPtr<IApplicationInfo> appInfo;
            sInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            String scname, sname;
            appInfo->GetCapsuleName(&scname);
            sInfo->GetName(&sname);
            AutoPtr<IComponentName> name;
            CComponentName::New(scname, sname, (IComponentName**)&name);
            HashMap<AutoPtr<IComponentName>, AutoPtr<CServiceRecord> >::Iterator it =
                    mServices.Find(name);
            if (it != mServices.End()) {
                r = it->mSecond;
            }
            if (r == NULL) {
                AutoPtr<IIntent> ci;
                service->CloneFilter((IIntent**)&ci);
                filter = NULL;
                CIntentFilterComparison::New(ci, (IIntentFilterComparison**)&filter);
                ServiceRestarter* res = new ServiceRestarter();
                AutoPtr<BatteryStatsImpl::Uid::Cap::Serv> ss;
    //            AutoPtr<BatteryStatsImpl> stats = mBatteryStatsService->GetActiveStatistics();
    //            stats->Lock();
    //            ss = stats->GetServiceStatsLocked(
    //                    ((CServiceInfo*)(IServiceInfo*)sInfo)->mApplicationInfo->mUid,
    //                    ((CServiceInfo*)(IServiceInfo*)sInfo)->mCapsuleName,
    //                    ((CServiceInfo*)(IServiceInfo*)sInfo)->mName);
    //            stats->Unlock();
                CServiceRecord::NewByFriend((CServiceRecord**)&r);
                r->Init(this, ss, name, filter, sInfo, res);
                res->SetService(r);
                mServices[name] = r;
                mServicesByIntent[filter] = r;

                // Make sure this component isn't in the pending list.
                List<AutoPtr<CServiceRecord> >::Iterator it = mPendingServices.Begin();
                while (it != mPendingServices.End()) {
                    AutoPtr<CServiceRecord> pr = *it;
                    Boolean isEqual;
                    if (pr->mName->Equals(name, &isEqual), isEqual) {
                        it = mPendingServices.Erase(it);
                    }
                    else ++it;
                }
            }
        }
    }
    if (r != NULL) {
        Int32 rUid;
        r->mAppInfo->GetUid(&rUid);
        if (CheckComponentPermission(r->mPermission,
                callingPid, callingUid, r->mExported ? -1 : rUid)
                != CapsuleManager_PERMISSION_GRANTED) {
            String srDes;
            r->mName->GetDescription(&srDes);
            Slogger::W(TAG, StringBuffer("Permission Denial: Accessing service ") + srDes
                    + " from pid=" + Binder::GetCallingPid()
                    + ", uid=" + Binder::GetCallingUid()
                    + " requires " + r->mPermission);
            return new ServiceLookupResult(NULL, r->mPermission);
        }
        return new ServiceLookupResult(r, String(NULL));
    }
    return NULL;
}

void CActivityManagerService::BumpServiceExecutingLocked(
    /* [in] */ CServiceRecord* r,
    /* [in] */ const char* why)
{
    if (DEBUG_SERVICE) {
        String srDes, appDes;
        r->GetDescription(&srDes);
        r->mApp->GetDescription(&appDes);
        Slogger::W(TAG, StringBuffer(">>> EXECUTING ")
                + why + " of " + srDes + " in app " + appDes);
    }
    else if (DEBUG_SERVICE_EXECUTING) {
        Slogger::V(TAG, StringBuffer(">>> EXECUTING ")
                + why + " of " + r->mShortName);
    }
    Millisecond64 now = SystemClock::GetUptimeMillis();
    if (r->mExecuteNesting == 0 && r->mApp != NULL) {
        if (r->mApp->mExecutingServices.GetSize() == 0) {
            void (STDCALL CActivityManagerService::*pHandlerFunc)(
                ProcessRecord* proc);
            pHandlerFunc = &CActivityManagerService::HandleServiceTimeout;
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInt32((Handle32)r->mApp);
            mApartment->PostCppCallbackAtTime((Handle32)this,
                    *(Handle32*)&pHandlerFunc, params, 0, now + SERVICE_TIMEOUT);
        }
        r->mApp->mExecutingServices.Insert(r);
    }
    r->mExecuteNesting++;
    r->mExecutingStart = now;
}

void CActivityManagerService::SendServiceArgsLocked(
    /* [in] */ CServiceRecord* r,
    /* [in] */ Boolean oomAdjusted)
{
    Int32 N = r->mPendingStarts.GetSize();
    if (N == 0) {
        return;
    }

    while (r->mPendingStarts.GetSize() > 0) {
//        try {
        List<CServiceRecord::StartItem*>::Iterator it = r->mPendingStarts.Begin();
        CServiceRecord::StartItem* si = *it;
        r->mPendingStarts.Erase(it);
        if (DEBUG_SERVICE) {
            String srDes, filDes, intDes;
            r->mName->GetDescription(&srDes);
            r->mIntent->GetDescription(&filDes);
            si->mIntent->GetDescription(&intDes);
            Slogger::V(TAG, "Sending arguments to service: "
                    + srDes + " " + filDes + " args=" + intDes);
        }
        if (si->mIntent == NULL) {
            // If somehow we got a dummy start at the front, then
            // just drop it here.
            continue;
        }
        si->mDeliveredTime = SystemClock::GetUptimeMillis();
        r->mDeliveredStarts.PushBack(si);
        si->mDeliveryCount++;
        if (si->mTargetPermissionUid >= 0) {
            GrantUriPermissionUncheckedFromIntentLocked(si->mTargetPermissionUid,
                    r->mCapsuleName, si->mIntent, si->GetUriPermissionsLocked());
        }
        BumpServiceExecutingLocked(r, "start");
        if (!oomAdjusted) {
            oomAdjusted = TRUE;
            UpdateOomAdjLocked(r->mApp);
        }
        Int32 flags = 0;
        if (si->mDeliveryCount > 0) {
            flags |= Service_START_FLAG_RETRY;
        }
        if (si->mDoneExecutingCount > 0) {
            flags |= Service_START_FLAG_REDELIVERY;
        }
        r->mApp->mAppApartment->ScheduleServiceArgs(r, si->mId, flags, si->mIntent);
//        } catch (RemoteException e) {
//            // Remote process gone...  we'll let the normal cleanup take
//            // care of this.
//            if (DEBUG_SERVICE) Slog.v(TAG, "Crashed while scheduling start: " + r);
//            break;
//        } catch (Exception e) {
//            Slogger::W(TAG, "Unexpected exception", e);
//            break;
//        }
    }
}

Boolean CActivityManagerService::RequestServiceBindingLocked(
    /* [in] */ CServiceRecord* r,
    /* [in] */ IntentBindRecord* i,
    /* [in] */ Boolean rebind)
{
    if (r->mApp == NULL || r->mApp->mAppApartment == NULL) {
        // If service is not currently running, can't yet bind.
        return FALSE;
    }
    if ((!i->mRequested || rebind) && i->mApps.GetSize() > 0) {
        BumpServiceExecutingLocked(r, "bind");
        AutoPtr<IIntent> intent;
        i->mIntent->GetIntent((IIntent**)&intent);
        ECode ec = r->mApp->mAppApartment->ScheduleBindService(r, intent, rebind);
        if (SUCCEEDED(ec)) {
            if (!rebind) {
                i->mRequested = TRUE;
            }
            i->mHasBound = TRUE;
            i->mDoRebind = FALSE;
        }
        else {
            if (DEBUG_SERVICE) {
                String srDes;
                r->GetDescription(&srDes);
                Slogger::V(TAG, StringBuffer("Crashed while binding ") + srDes);
            }
            return FALSE;
        }
    }
    return TRUE;
}

void CActivityManagerService::RequestServiceBindingsLocked(
    /* [in] */ CServiceRecord* r)
{
    HashMap<AutoPtr<IIntentFilterComparison>, IntentBindRecord*>::Iterator it;
    for (it = r->mBindings.Begin(); it != r->mBindings.End(); ++it) {
        IntentBindRecord* i = it->mSecond;
        if (!RequestServiceBindingLocked(r, i, FALSE)) {
            break;
        }
    }
}

ECode CActivityManagerService::RealStartServiceLocked(
    /* [in] */ CServiceRecord* r,
    /* [in] */ ProcessRecord* app)
{
    if (app->mAppApartment == NULL) {
        return E_REMOTE_EXCEPTION;
    }

    r->mApp = app;
    r->mRestartTime = r->mLastActivity = SystemClock::GetUptimeMillis();

    app->mServices.Insert(r);
    BumpServiceExecutingLocked(r, "create");
    UpdateLRUProcessLocked(app, TRUE, TRUE);

    Boolean created = FALSE;
//    try {
//    mStringBuilder.setLength(0);
//    r.intent.getIntent().toShortString(mStringBuilder, false, true);
//    EventLog.writeEvent(EventLogTags.AM_CREATE_SERVICE,
//            System.identityHashCode(r), r.shortName,
//            mStringBuilder.toString(), r.app.pid);
//    r->mStats->GetBatteryStats()->Lock();
//    r->mStats->StartLaunchedLocked();
//    r->mStats->GetBatteryStats()->Unlock();
//    ensurePackageDexOpt(r.serviceInfo.packageName);
    app->mAppApartment->ScheduleCreateService(r, r->mServiceInfo);
    r->PostNotification();
    created = TRUE;
//    } finally {
//        if (!created) {
//            app.services.remove(r);
//            scheduleServiceRestartLocked(r. false);
//        }
//    }

    RequestServiceBindingsLocked(r);

    // If the service is in the started state, and there are no
    // pending arguments, then fake up one so its onStartCommand() will
    // be called.
    if (r->mStartRequested && r->mCallStart && r->mPendingStarts.GetSize() == 0) {
        r->mLastStartId++;
        if (r->mLastStartId < 1) {
            r->mLastStartId = 1;
        }
        r->mPendingStarts.PushBack(new CServiceRecord::StartItem(r, r->mLastStartId, NULL, -1));
    }

    SendServiceArgsLocked(r, TRUE);
    return NOERROR;
}

Boolean CActivityManagerService::ScheduleServiceRestartLocked(
    /* [in] */ CServiceRecord* r,
    /* [in] */ Boolean allowCancel)
{
    Boolean canceled = FALSE;

    Millisecond64 now = SystemClock::GetUptimeMillis();
    Millisecond64 minDuration = SERVICE_RESTART_DURATION;
    Millisecond64 resetTime = SERVICE_RESET_RUN_DURATION;

    // Any delivered but not yet finished starts should be put back
    // on the pending list.
    if (r->mDeliveredStarts.GetSize() > 0) {
        List<CServiceRecord::StartItem*>::ReverseIterator rit = r->mDeliveredStarts.RBegin();
        for(; rit != r->mDeliveredStarts.REnd(); ++rit) {
            CServiceRecord::StartItem* si = *rit;
            si->RemoveUriPermissionsLocked();
            if (si->mIntent == NULL) {
                // We'll generate this again if needed.
            } else if (!allowCancel || (si->mDeliveryCount < CServiceRecord::MAX_DELIVERY_COUNT
                    && si->mDoneExecutingCount < CServiceRecord::MAX_DONE_EXECUTING_COUNT)) {
                r->mPendingStarts.PushFront(si);
                Millisecond64 dur = SystemClock::GetUptimeMillis() - si->mDeliveredTime;
                dur *= 2;
                if (minDuration < dur) minDuration = dur;
                if (resetTime < dur) resetTime = dur;
            } else {
                String intDes, srDes;
                si->mIntent->GetDescription(&intDes);
                r->mName->GetDescription(&srDes);
                Slogger::W(TAG, StringBuffer("Canceling start item ") + intDes
                        + " in service " + srDes);
                canceled = TRUE;
            }
        }
        r->mDeliveredStarts.Clear();
    }

    r->mTotalRestartCount++;
    if (r->mRestartDelay == 0) {
        r->mRestartCount++;
        r->mRestartDelay = minDuration;
    } else {
        // If it has been a "reasonably long time" since the service
        // was started, then reset our restart duration back to
        // the beginning, so we don't infinitely increase the duration
        // on a service that just occasionally gets killed (which is
        // a normal case, due to process being killed to reclaim memory).
        if (now > (r->mRestartTime + resetTime)) {
            r->mRestartCount = 1;
            r->mRestartDelay = minDuration;
        } else {
            r->mRestartDelay *= SERVICE_RESTART_DURATION_FACTOR;
            if (r->mRestartDelay < minDuration) {
                r->mRestartDelay = minDuration;
            }
        }
    }

    r->mNextRestartTime = now + r->mRestartDelay;

    // Make sure that we don't end up restarting a bunch of services
    // all at the same time.
    Boolean repeat;
    do {
        repeat = FALSE;
        List<AutoPtr<CServiceRecord> >::ReverseIterator it;
        for (it = mRestartingServices.RBegin(); it != mRestartingServices.REnd(); ++it) {
            AutoPtr<CServiceRecord> r2 = *it;
            if ((CServiceRecord*)r2 != r && r->mNextRestartTime
                    >= (r2->mNextRestartTime - SERVICE_MIN_RESTART_TIME_BETWEEN)
                    && r->mNextRestartTime
                    < (r2->mNextRestartTime + SERVICE_MIN_RESTART_TIME_BETWEEN)) {
                r->mNextRestartTime = r2->mNextRestartTime + SERVICE_MIN_RESTART_TIME_BETWEEN;
                r->mRestartDelay = r->mNextRestartTime - now;
                repeat = TRUE;
                break;
            }
        }
    } while (repeat);

    if (Find(mRestartingServices.Begin(),
            mRestartingServices.End(),
            (const CServiceRecord*)r) == mRestartingServices.End()) {
        mRestartingServices.PushBack(r);
    }

    r->CancelNotification();

//    mHandler.removeCallbacks(r.restarter);
//    mHandler.postAtTime(r.restarter, r.nextRestartTime);
//    r.nextRestartTime = SystemClock.uptimeMillis() + r.restartDelay;
//    Slogger::W(TAG, "Scheduling restart of crashed service "
//            + r.shortName + " in " + r.restartDelay + "ms");
//    EventLog.writeEvent(EventLogTags.AM_SCHEDULE_SERVICE_RESTART,
//            r.shortName, r.restartDelay);

    return canceled;
}

void CActivityManagerService::PerformServiceRestartLocked(
    /* [in] */ CServiceRecord* r)
{
    if (Find(mRestartingServices.Begin(),
            mRestartingServices.End(),
            (const CServiceRecord*)r) == mRestartingServices.End()) {
        return;
    }
    AutoPtr<IIntent> intent;
    r->mIntent->GetIntent((IIntent**)&intent);
    Int32 flags;
    intent->GetFlags(&flags);
    BringUpServiceLocked(r, flags, TRUE);
}

Boolean CActivityManagerService::UnscheduleServiceRestartLocked(
    /* [in] */ CServiceRecord* r)
{
    if (r->mRestartDelay == 0) {
        return FALSE;
    }
    r->ResetRestartCounter();
    mRestartingServices.Remove(r);
//    mHandler.removeCallbacks(r.restarter);
    return TRUE;
}

Boolean CActivityManagerService::BringUpServiceLocked(
    /* [in] */ CServiceRecord* r,
    /* [in] */ Int32 intentFlags,
    /* [in] */ Boolean whileRestarting)
{
    if (r->mApp != NULL && r->mApp->mAppApartment != NULL) {
        SendServiceArgsLocked(r, FALSE);
        return TRUE;
    }

    if (!whileRestarting && r->mRestartDelay > 0) {
        // If waiting for a restart, then do nothing.
        return TRUE;
    }

    if (DEBUG_SERVICE) {
        String srDes, filDes;
        r->GetDescription(&srDes);
        r->mIntent->GetDescription(&filDes);
        Slogger::V(TAG, StringBuffer("Bringing up ") + srDes
                + " " + filDes);

    }

    // We are now bringing the service up, so no longer in the
    // restarting state.
    mRestartingServices.Remove(r);

    String appName = r->mProcessName;
    Int32 rUid;
    r->mAppInfo->GetUid(&rUid);
    ProcessRecord* app = GetProcessRecordLocked(appName, rUid);
    if (app != NULL && app->mAppApartment != NULL) {
        if (SUCCEEDED(RealStartServiceLocked(r, app))) {
            return TRUE;
        }
        else {
            Slogger::W(TAG, StringBuffer("Exception when starting service ") + r->mShortName);
        }

        // If a dead object exception was thrown -- fall through to
        // restart the application.
    }

    // Not running -- get it started, and enqueue this service record
    // to be executed when the app comes up.
    if (StartProcessLocked(appName, r->mAppInfo, TRUE, intentFlags,
            "service", r->mName, FALSE) == NULL) {
        AutoPtr<IIntent> rIntent;
        r->mIntent->GetIntent((IIntent**)&rIntent);
        String intDes;
        rIntent->GetDescription(&intDes);
        String rCName;
        r->mAppInfo->GetCapsuleName(&rCName);
        Slogger::W(TAG, StringBuffer("Unable to launch app ")
                + rCName + "/"
                + rUid + " for service "
                + intDes + ": process is bad");
        BringDownServiceLocked(r, TRUE);

        return FALSE;
    }

    if (Find(mPendingServices.Begin(),
            mPendingServices.End(),
            (const CServiceRecord*)r) == mPendingServices.End()) {
        mPendingServices.PushBack(r);
    }
    return TRUE;
}

ECode CActivityManagerService::BringDownServiceLocked(
    /* [in] */ CServiceRecord* r,
    /* [in] */ Boolean force)
{
    // Does it still need to run?
    if (!force && r->mStartRequested) {
        return NOERROR;
    }
    if (r->mConnections.GetSize() > 0) {
        if (!force) {
            // XXX should probably keep a count of the number of auto-create
            // connections directly in the service.
            Map<AutoPtr<IServiceConnectionInner>, List<ConnectionRecord*>*>::Iterator it;
            for (it = r->mConnections.Begin(); it != r->mConnections.End(); ++it) {
                List<ConnectionRecord*>* crlist = it->mSecond;
                List<ConnectionRecord*>::Iterator crit = crlist->Begin();
                for (; crit != crlist->End(); ++crit) {
                    ConnectionRecord* cr = *crit;
                    if ((cr->mFlags & Context_BIND_AUTO_CREATE) != 0) {
                        return NOERROR;
                    }
                }
            }
        }

        // Report to all of the connections that the service is no longer
        // available.
        Map<AutoPtr<IServiceConnectionInner>, List<ConnectionRecord*>*>::Iterator it;
        for (it = r->mConnections.Begin(); it != r->mConnections.End(); ++it) {
            List<ConnectionRecord*>* crlist = it->mSecond;
            List<ConnectionRecord*>::Iterator crit = crlist->Begin();
            for (; crit != crlist->End(); ++crit) {
                // todo: shouldn't be a synchronous call!
                ConnectionRecord* c = *crit;
                if (FAILED(c->mConn->Connected(r->mName, NULL))) {
                    String srDes, connDes;
                    r->mName->GetDescription(&srDes);
                    c->mConn->GetDescription(&connDes);
                    Slogger::W(TAG, StringBuffer("Failure disconnecting service ") + srDes +
                              " to connection " + connDes +
                              " (in " + c->mBinding->mClient->mProcessName + ")");
                }
            }
        }
    }

    // Tell the service that it has been unbound.
    if (r->mBindings.GetSize() > 0 && r->mApp != NULL && r->mApp->mAppApartment != NULL) {
        HashMap<AutoPtr<IIntentFilterComparison>, IntentBindRecord*>::Iterator it;
        for (it = r->mBindings.Begin(); it != r->mBindings.End(); ++it) {
            IntentBindRecord* ibr = it->mSecond;
            if (DEBUG_SERVICE) {
                String ibrDes;
                ibr->GetDescription(&ibrDes);
                Slogger::V(TAG, StringBuffer("Bringing down binding ") + ibrDes
                    + ": hasBound=" + ibr->mHasBound);
            }
            if (r->mApp != NULL && r->mApp->mAppApartment != NULL && ibr->mHasBound) {
//                try {
                BumpServiceExecutingLocked(r, "bring down unbind");
                UpdateOomAdjLocked(r->mApp);
                ibr->mHasBound = FALSE;
                AutoPtr<IIntent> ibrIntent;
                ibr->mIntent->GetIntent((IIntent**)&ibrIntent);
                r->mApp->mAppApartment->ScheduleUnbindService(r, ibrIntent);
//                } catch (Exception e) {
//                    Slogger::W(TAG, "Exception when unbinding service "
//                            + r.shortName, e);
//                    serviceDoneExecutingLocked(r, true);
//                }
            }
        }
    }

    if (DEBUG_SERVICE) {
        String srDes, filDes;
        r->GetDescription(&srDes);
        r->mIntent->GetDescription(&filDes);
        Slogger::V(TAG, StringBuffer("Bringing down ") + srDes + " " + filDes);

    }
//    EventLog.writeEvent(EventLogTags.AM_DESTROY_SERVICE,
//            System.identityHashCode(r), r.shortName,
//            (r.app != null) ? r.app.pid : -1);

    mServices.Erase(r->mName);
    mServicesByIntent.Erase(r->mIntent);
    r->mTotalRestartCount = 0;
    UnscheduleServiceRestartLocked(r);

    // Also make sure it is not on the pending list.
    List<AutoPtr<CServiceRecord> >::Iterator it = mPendingServices.Begin();
    while (it != mPendingServices.End()) {
        if ((CServiceRecord*)*it == r) {
            it = mPendingServices.Erase(it);
            if (DEBUG_SERVICE) {
                String srDes;
                r->GetDescription(&srDes);
                Slogger::V(TAG, StringBuffer("Removed pending: ") + srDes);
            }
        }
        else ++it;
    }

    r->CancelNotification();
    r->mIsForeground = FALSE;
    r->mForegroundId = 0;
    r->mForegroundNoti = NULL;

    // Clear start entries.
    r->ClearDeliveredStartsLocked();
    r->mPendingStarts.Clear();

    if (r->mApp != NULL) {
//        r->mStats->GetBatteryStats()->Lock();
//        r->mStats->StopLaunchedLocked();
//        r->mStats->GetBatteryStats()->Unlock();
        r->mApp->mServices.Erase(r);
        if (r->mApp->mAppApartment != NULL) {
            UpdateServiceForegroundLocked(r->mApp, FALSE);
//            try {
            BumpServiceExecutingLocked(r, "stop");
            mStoppingServices.PushBack(r);
            UpdateOomAdjLocked(r->mApp);
            r->mApp->mAppApartment->ScheduleStopService(r);
//            } catch (Exception e) {
//                Slogger::W(TAG, "Exception when stopping service "
//                        + r.shortName, e);
//                serviceDoneExecutingLocked(r, true);
//            }
            UpdateServiceForegroundLocked(r->mApp, FALSE);
        } else {
            if (DEBUG_SERVICE) {
                String srDes;
                r->GetDescription(&srDes);
                Slogger::V(TAG, StringBuffer("Removed service that has no process: ")
                        + srDes);
            }
        }
    } else {
        if (DEBUG_SERVICE) {
            String srDes;
            r->GetDescription(&srDes);
            Slogger::V(TAG, StringBuffer("Removed service that is not running: ")
                    + srDes);
        }
    }

    if (r->mBindings.GetSize() > 0) {
        r->mBindings.Clear();
    }

    // ClassID clsid;
    // AutoPtr<IObject> obj = (IObject*)r->mRestarter->Probe(EIID_IObject);
    // obj->GetClassID(&clsid);
    // if (clsid == ECLSID_CServiceRestarter) {
    //    ((CServiceRestarter*)(IRunnable*)(r->mRestarter))->SetService(NULL);
    // }
    return NOERROR;
}

ECode CActivityManagerService::StartServiceLocked(
    /* [in] */ IApplicationApartment* caller,
    /* [in] */ IIntent* service,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 callingPid,
    /* [in] */ Int32 callingUid,
    /* [out] */ IComponentName** name)
{
    if (DEBUG_SERVICE) {
        String intDes, bdDes;
        service->GetDescription(&intDes);
        AutoPtr<IBundle> b;
        service->GetExtras((IBundle**)&b);
        b->GetDescription(&bdDes);
        Slogger::V(TAG, StringBuffer("startService: ") + intDes
            + " type=" + resolvedType + " args=" + bdDes);
    }

    if (caller != NULL) {
        ProcessRecord* callerApp = GetProcessRecordForAppLocked(caller);
        if (callerApp == NULL) {
            String appApDes, intDes;
            caller->GetDescription(&appApDes);
            service->GetDescription(&intDes);
            Slogger::E(TAG, StringBuffer("Unable to find app for caller ")
                    + appApDes + " (pid=" + Binder::GetCallingPid()
                    + ") when starting service " + intDes);
            return E_SECURITY_EXCEPTION;
        }
    }

    ServiceLookupResult* res =
            RetrieveServiceLocked(service, resolvedType,
                callingPid, callingUid);
    if (res == NULL) {
        *name = NULL;
        return E_DOES_NOT_EXIST;
    }
    if (res->mRecord == NULL) {
        CComponentName::New(String("!"), !res->mPermission.IsNull()
                ? res->mPermission : String("private to package"), name);
        return E_FAIL;
    }
    AutoPtr<CServiceRecord> r = res->mRecord;
    Int32 targetPermissionUid = CheckGrantUriPermissionFromIntentLocked(
                    callingUid, r->mCapsuleName, service);
    if (UnscheduleServiceRestartLocked(r)) {
        if (DEBUG_SERVICE) {
            String srDes;
            r->GetDescription(&srDes);
            Slogger::V(TAG, StringBuffer("START SERVICE WHILE RESTART PENDING: ")
                    + srDes);
        }
    }
    r->mStartRequested = TRUE;
    r->mCallStart = FALSE;
    r->mLastStartId++;
    if (r->mLastStartId < 1) {
        r->mLastStartId = 1;
    }
    r->mPendingStarts.PushBack(new CServiceRecord::StartItem(r,
            r->mLastStartId, service, targetPermissionUid));
    r->mLastActivity = SystemClock::GetUptimeMillis();
//    r->mStats->GetBatteryStats()->Lock();
//    r->mStats->StartRunningLocked();
//    r->mStats->GetBatteryStats()->Unlock();

    Int32 flags;
    service->GetFlags(&flags);
    if (!BringUpServiceLocked(r, flags, FALSE)) {
        CComponentName::New(String("!"), String("Service process is bad"), name);
        return NOERROR;
    }
    *name = r->mName;
    SelfUnlock();
    return NOERROR;
}

ECode CActivityManagerService::StartService(
    /* [in] */ IApplicationApartment* caller,
    /* [in] */ IIntent* service,
    /* [in] */ const String& resolvedType,
    /* [out] */ IComponentName** name)
{
    if (name == NULL) return E_INVALID_ARGUMENT;

    // Refuse possible leaked file descriptors
    Boolean result;
    if (service != NULL &&
        (service->HasFileDescriptors(&result), result == TRUE)) {
        Slogger::E(TAG, "File descriptors passed in Intent");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Mutex::Autolock lock(_m_syncLock);

    const Int32 callingPid = 0; /*Binder.getCallingPid();*/
    const Int32 callingUid = 0; /*Binder.getCallingUid();*/
//    final long origId = Binder.clearCallingIdentity();
    ECode ec = StartServiceLocked(caller, service, resolvedType,
            callingPid, callingUid, name);
//    Binder.restoreCallingIdentity(origId);

    return ec;
}

ECode CActivityManagerService::StartServiceInCapsule(
    /* [in] */ Int32 uid,
    /* [in] */ IIntent* service,
    /* [in] */ const String& resolvedType,
    /* [out] */ IComponentName** name)
{
    Mutex::Autolock lock(_m_syncLock);

//    final long origId = Binder.clearCallingIdentity();
    ECode ec = StartServiceLocked(NULL, service,
            resolvedType, -1, uid, name);
//    Binder.restoreCallingIdentity(origId);

    return ec;
}

ECode CActivityManagerService::StopService(
    /* [in] */ IApplicationApartment* caller,
    /* [in] */ IIntent* service,
    /* [in] */ const String& resolvedType,
    /* [out] */ Int32* result)
{
    // Refuse possible leaked file descriptors
    Boolean hasFD;
    if (service != NULL && (service->HasFileDescriptors(&hasFD), hasFD == TRUE)) {
        Slogger::E(TAG, "File descriptors passed in Intent");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Mutex::Autolock lock(_m_syncLock);

    if (DEBUG_SERVICE) {
        String intDes;
        service->GetDescription(&intDes);
        Slogger::V(TAG, StringBuffer("stopService: ") + intDes
            + " type=" + resolvedType);
    }

    ProcessRecord* callerApp = GetProcessRecordForAppLocked(caller);
    if (caller != NULL && callerApp == NULL) {
        String appApDes, intDes;
        caller->GetDescription(&appApDes);
        service->GetDescription(&intDes);
        Slogger::E(TAG, StringBuffer("Unable to find app for caller ") + appApDes
                + " (pid=" + Binder::GetCallingPid()
                + ") when stopping service " + intDes);
        return E_SECURITY_EXCEPTION;
    }

    // If this service is active, make sure it is stopped.
    ServiceLookupResult* r = FindServiceLocked(service,
            resolvedType);
    if (r != NULL) {
        if (r->mRecord != NULL) {
//            r->mRecord->mStats->GetBatteryStats()->Lock();
//            r->mRecord->mStats->StopRunningLocked();
//            r->mRecord->mStats->GetBatteryStats()->Unlock();
            r->mRecord->mStartRequested = FALSE;
            r->mRecord->mCallStart = FALSE;
//            final long origId = Binder.clearCallingIdentity();
            BringDownServiceLocked(r->mRecord, FALSE);
//            Binder.restoreCallingIdentity(origId);
            *result = 1;
        }
        else {
            *result = -1;
        }
    }
    else {
        *result = 0;
    }
    return NOERROR;
}

ECode CActivityManagerService::PeekService(
    /* [in] */ IIntent* service,
    /* [in] */ const String& resolvedType,
    /* [out] */ IBinder** token)
{
    if (token == NULL) return E_INVALID_ARGUMENT;

    *token = NULL;

    // Refuse possible leaked file descriptors
    Boolean hasFD;
    if (service != NULL && (service->HasFileDescriptors(&hasFD), hasFD == TRUE)) {
        Slogger::E(TAG, "File descriptors passed in Intent");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Mutex::Autolock lock(_m_syncLock);

    ServiceLookupResult* r = FindServiceLocked(service,
            resolvedType);

    if (r != NULL) {
        // r.record is null if findServiceLocked() failed the caller permission check
        if (r->mRecord == NULL) {
            Slogger::E(TAG, StringBuffer("Permission Denial: Accessing service ")
                    + " from pid=" + Binder::GetCallingPid()
                    + ", uid=" + Binder::GetCallingUid()
                    + " requires " + r->mPermission);
            return E_SECURITY_EXCEPTION;
        }
        IntentBindRecord* ib = r->mRecord->mBindings[r->mRecord->mIntent];
        if (ib != NULL) {
            *token = ib->mBinder;
        }
    }
    return NOERROR;
}

ECode CActivityManagerService::StopServiceToken(
    /* [in] */ IComponentName* className,
    /* [in] */ IBinder* token,
    /* [in] */ Int32 startId,
    /* [out] */ Boolean* succeeded)
{
    if (succeeded == NULL) return E_INVALID_ARGUMENT;

    Mutex::Autolock lock(_m_syncLock);

    if (DEBUG_SERVICE) {
        String clsDes, tokenDes;
        className->GetDescription(&clsDes);
        token->GetDescription(&tokenDes);
        Slogger::V(TAG, StringBuffer("stopServiceToken: ") + clsDes
            + " " + tokenDes + " startId=" + startId);
    }
    AutoPtr<CServiceRecord> r = FindServiceLocked(className, token);
    if (r != NULL) {
        if (startId >= 0) {
            // Asked to only stop if done with all work.  Note that
            // to avoid leaks, we will take this as dropping all
            // start items up to and including this one.
            CServiceRecord::StartItem* si = r->FindDeliveredStart(startId, FALSE);
            if (si != NULL) {
                while (r->mDeliveredStarts.GetSize() > 0) {
                    CServiceRecord::StartItem* cur = r->mDeliveredStarts.GetFront();
                    r->mDeliveredStarts.PopFront();
                    cur->RemoveUriPermissionsLocked();
                    if (cur == si) {
                        break;
                    }
                }
            }

            if (r->mLastStartId != startId) {
                *succeeded = FALSE;
                return NOERROR;
            }

            if (r->mDeliveredStarts.GetSize() > 0) {
                Slogger::W(TAG, StringBuffer("stopServiceToken startId ") + startId
                        + " is last, but have " + (Int32)r->mDeliveredStarts.GetSize()
                        + " remaining args");
            }
        }
//        r->mStats->GetBatteryStats()->Lock();
//        r->mStats->StopRunningLocked();
        r->mStartRequested = FALSE;
        r->mCallStart = FALSE;
//        r->mStats->GetBatteryStats()->Unlock();
//        final long origId = Binder.clearCallingIdentity();
        BringDownServiceLocked(r, FALSE);
//        Binder.restoreCallingIdentity(origId);
        *succeeded = TRUE;
        return NOERROR;
    }
    *succeeded = FALSE;
    return NOERROR;
}

ECode CActivityManagerService::SetServiceForeground(
    /* [in] */ IComponentName* className,
    /* [in] */ IBinder* token,
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification,
    /* [in] */ Boolean removeNotification)
{
//    final long origId = Binder.clearCallingIdentity();
//    try {
    Mutex::Autolock lock(_m_syncLock);

    AutoPtr<CServiceRecord> r = FindServiceLocked(className, token);
    if (r != NULL) {
        if (id != 0) {
            if (notification == NULL) {
//                throw new IllegalArgumentException("null notification");
                Slogger::E(TAG, "null notification");
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            if (r->mForegroundId != id) {
                r->CancelNotification();
                r->mForegroundId = id;
            }
            Int32 flags;
            notification->GetFlags(&flags);
            flags |= Notification_FLAG_FOREGROUND_SERVICE;
            notification->SetFlags(flags);
            r->mForegroundNoti = notification;
            r->mIsForeground = TRUE;
            r->PostNotification();
            if (r->mApp != NULL) {
                UpdateServiceForegroundLocked(r->mApp, TRUE);
            }
        } else {
            if (r->mIsForeground) {
                r->mIsForeground = FALSE;
                if (r->mApp != NULL) {
                    UpdateLRUProcessLocked(r->mApp, FALSE, TRUE);
                    UpdateServiceForegroundLocked(r->mApp, TRUE);
                }
            }
            if (removeNotification) {
                r->CancelNotification();
                r->mForegroundId = 0;
                r->mForegroundNoti = NULL;
            }
        }
    }
    return NOERROR;
//    } finally {
//        Binder.restoreCallingIdentity(origId);
//    }
}

ECode CActivityManagerService::UpdateServiceForegroundLocked(
    /* [in] */ ProcessRecord* proc,
    /* [in] */ Boolean oomAdj)
{
    Boolean anyForeground = FALSE;
    Set<AutoPtr<CServiceRecord> >::Iterator it;
    for (it = proc->mServices.Begin(); it != proc->mServices.End(); ++it) {
        if ((*it)->mIsForeground) {
            anyForeground = TRUE;
            break;
        }
    }
    if (anyForeground != proc->mForegroundServices) {
        proc->mForegroundServices = anyForeground;
        if (oomAdj) {
            UpdateOomAdjLocked();
        }
    }
    return NOERROR;
}

ECode CActivityManagerService::BindService(
    /* [in] */ IApplicationApartment* caller,
    /* [in] */ IBinder* token,
    /* [in] */ IIntent* _service,
    /* [in] */ const String& resolvedType,
    /* [in] */ IServiceConnectionInner* connection,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 callingPid,
    /* [in] */ Int32 callingUid,
    /* [out] */ Int32* result)
{
    if (result == NULL) return E_INVALID_ARGUMENT;

    AutoPtr<IIntent> service = _service;
    // Refuse possible leaked file descriptors
    Boolean hasFD;
    if (service != NULL && service->HasFileDescriptors(&hasFD), (hasFD == TRUE)) {
        Slogger::E(TAG, "File descriptors passed in Intent");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Mutex::Autolock lock(_m_syncLock);

    if (DEBUG_SERVICE) {
        String intDes, connDes;
        service->GetDescription(&intDes);
        connection->GetDescription(&connDes);
        Slogger::V(TAG, "bindService: " + intDes
            + " type=" + resolvedType + " conn=" + connDes
            + " flags=" + flags);
    }
    ProcessRecord* callerApp = GetProcessRecordForAppLocked(caller);
    if (callerApp == NULL) {
        String appApDes, intDes;
        caller->GetDescription(&appApDes);
        service->GetDescription(&intDes);
        Slogger::E(TAG, StringBuffer("Unable to find app for caller ") + appApDes
                + " (pid=" + Binder::GetCallingPid()
                + ") when binding service " + intDes);
        return E_SECURITY_EXCEPTION;
    }

    AutoPtr<CActivityRecord> activity;
    if (token != NULL) {
        Int32 aindex = mMainStack->GetIndexOfTokenLocked(token);
        if (aindex < 0) {
            String tokenDes;
            token->GetDescription(&tokenDes);
            Slogger::W(TAG, "Binding with unknown activity: " + tokenDes);
            *result = 0;
            return NOERROR;
        }
        activity = mMainStack->mHistory[aindex];
    }

    Int32 clientLabel = 0;
    AutoPtr<IPendingIntent> clientIntent;

    Int32 callerAppUid;
    callerApp->mInfo->GetUid(&callerAppUid);
    if (callerAppUid == Process::SYSTEM_UID) {
        // Hacky kind of thing -- allow system stuff to tell us
        // what they are, so we can report this elsewhere for
        // others to know why certain services are running.
        AutoPtr<IParcelable> value;
        service->GetParcelableExtra(String(Intent_EXTRA_CLIENT_INTENT), (IParcelable**)&value);
        clientIntent = IPendingIntent::Probe(value);
        if (clientIntent != NULL) {
            service->GetInt32Extra(String(Intent_EXTRA_CLIENT_LABEL), 0, &clientLabel);
            if (clientLabel != 0) {
                // There are no useful extras in the intent, trash them.
                // System code calling with this stuff just needs to know
                // this will happen.
                service->CloneFilter((IIntent**)&service);
            }
        }
    }

    ServiceLookupResult* res =
        RetrieveServiceLocked(service, resolvedType,
                callingPid, callingUid);
    if (res == NULL) {
        *result = 0;
        return NOERROR;
    }
    if (res->mRecord == NULL) {
        *result = -1;
        return NOERROR;
    }
    AutoPtr<CServiceRecord> s = res->mRecord;

//    final long origId = Binder.clearCallingIdentity();

    if (UnscheduleServiceRestartLocked(s)) {
        if (DEBUG_SERVICE) {
            String srDes;
            s->GetDescription(&srDes);
            Slogger::V(TAG, StringBuffer("BIND SERVICE WHILE RESTART PENDING: ")
                + srDes);
        }
    }

    AppBindRecord* b = s->RetrieveAppBindingLocked(service, callerApp);
    ConnectionRecord* c = new ConnectionRecord(b, activity,
            connection, flags, clientLabel, clientIntent);

    List<ConnectionRecord*>* clist = s->mConnections[connection];
    if (clist == NULL) {
        clist = new List<ConnectionRecord*>();
        s->mConnections[connection] = clist;
    }
    clist->PushBack(c);
    b->mConnections.Insert(c);
    if (activity != NULL) {
        if (activity->mConnections == NULL) {
            activity->mConnections = new Set<ConnectionRecord*>();
        }
        activity->mConnections->Insert(c);
    }
    b->mClient->mConnections.Insert(c);
    clist = mServiceConnections[connection];
    if (clist == NULL) {
        clist = new List<ConnectionRecord*>();
        mServiceConnections[connection] = clist;
    }
    clist->PushBack(c);
    if ((flags & Context_BIND_AUTO_CREATE) != 0) {
        s->mLastActivity = SystemClock::GetUptimeMillis();
        Int32 flags;
        service->GetFlags(&flags);
        if (!BringUpServiceLocked(s, flags, FALSE)) {
            *result = 0;
            return NOERROR;
        }
    }

    if (s->mApp != NULL) {
        // This could have made the service more important.
        UpdateOomAdjLocked(s->mApp);
    }

    if (DEBUG_SERVICE) {
        String srDes, abDes;
        s->GetDescription(&srDes);
        b->GetDescription(&abDes);
        Slogger::V(TAG, StringBuffer("Bind ") + srDes
                + " with " + abDes
                + ": received=" + b->mIntent->mReceived
                + " apps=" + (Int32)b->mIntent->mApps.GetSize()
                + " doRebind=" + b->mIntent->mDoRebind);
    }

    if (s->mApp != NULL && b->mIntent->mReceived) {
        // Service is already running, so we can immediately
        // publish the connection.
//        try {
        c->mConn->Connected(s->mName, b->mIntent->mBinder);
//        } catch (Exception e) {
//            Slogger::W(TAG, "Failure sending service " + s.shortName
//                    + " to connection " + c.conn.asBinder()
//                    + " (in " + c.binding.client.processName + ")", e);
//        }

        // If this is the first app connected back to this binding,
        // and the service had previously asked to be told when
        // rebound, then do so.
        if (b->mIntent->mApps.GetSize() == 1 && b->mIntent->mDoRebind) {
            RequestServiceBindingLocked(s, b->mIntent, TRUE);
        }
    }
    else if (!b->mIntent->mRequested) {
        RequestServiceBindingLocked(s, b->mIntent, FALSE);
    }

//        Binder.restoreCallingIdentity(origId);
    *result = 1;
    return NOERROR;
}

ECode CActivityManagerService::RemoveConnectionLocked(
    /* [in] */ ConnectionRecord* c,
    /* [in] */ ProcessRecord* skipApp,
    /* [in] */ CActivityRecord* skipAct)
{
    AppBindRecord* b = c->mBinding;
    AutoPtr<CServiceRecord> s = b->mService;
    List<ConnectionRecord*>* clist = s->mConnections[c->mConn];
    if (clist != NULL) {
        clist->Remove(c);
        if (clist->GetSize() == 0) {
            s->mConnections.Erase(c->mConn);
        }
    }
    b->mConnections.Erase(c);
    if (c->mActivity != NULL && (CActivityRecord*)(c->mActivity) != skipAct) {
        if (c->mActivity->mConnections != NULL) {
            c->mActivity->mConnections->Erase(c);
        }
    }
    if (b->mClient != skipApp) {
        b->mClient->mConnections.Erase(c);
    }
    clist = mServiceConnections[c->mConn];
    if (clist != NULL) {
        clist->Remove(c);
        if (clist->GetSize() == 0) {
            mServiceConnections.Erase(c->mConn);
        }
    }

    if (b->mConnections.GetSize() == 0) {
        b->mIntent->mApps.Erase(b->mClient);
    }

    if (DEBUG_SERVICE) {
        String ibrDes;
        b->mIntent->GetDescription(&ibrDes);
        Slogger::V(TAG, StringBuffer("Disconnecting binding ") + ibrDes
            + ": shouldUnbind=" + b->mIntent->mHasBound);
    }
    if (s->mApp != NULL && s->mApp->mAppApartment != NULL
            && b->mIntent->mApps.GetSize() == 0 && b->mIntent->mHasBound) {
//        try {
        BumpServiceExecutingLocked(s, "unbind");
        UpdateOomAdjLocked(s->mApp);
        b->mIntent->mHasBound = FALSE;
        // Assume the client doesn't want to know about a rebind;
        // we will deal with that later if it asks for one.
        b->mIntent->mDoRebind = FALSE;
        AutoPtr<IIntent> bIntent;
        b->mIntent->mIntent->GetIntent((IIntent**)&bIntent);
        s->mApp->mAppApartment->ScheduleUnbindService(
                s, bIntent);
//        } catch (Exception e) {
//            Slogger::W(TAG, "Exception when unbinding service " + s.shortName, e);
//            serviceDoneExecutingLocked(s, true);
//        }
    }

    if ((c->mFlags & Context_BIND_AUTO_CREATE) != 0) {
        BringDownServiceLocked(s, FALSE);
    }

    return NOERROR;
}

ECode CActivityManagerService::UnbindService(
    /* [in] */ IServiceConnectionInner* connection,
    /* [out] */ Boolean* succeeded)
{
    if (succeeded == NULL) return E_INVALID_ARGUMENT;

    Mutex::Autolock lock(_m_syncLock);

    if (DEBUG_SERVICE) {
        String connDes;
        connection->GetDescription(&connDes);
        Slogger::V(TAG, StringBuffer("unbindService: conn=") + connDes);
    }
    List<ConnectionRecord*>* clist = mServiceConnections[connection];
    if (clist == NULL) {
        String connDes;
        connection->GetDescription(&connDes);
        Slogger::W(TAG,
            StringBuffer("Unbind failed: could not find connection for ") + connDes);
        *succeeded = FALSE;
        return NOERROR;
    }

//    final long origId = Binder.clearCallingIdentity();
    while (clist->GetSize() > 0) {
        ConnectionRecord* r = clist->GetFront();
        RemoveConnectionLocked(r, NULL, NULL);
        if (r->mBinding->mService->mApp != NULL) {
            // This could have made the service less important.
            UpdateOomAdjLocked(r->mBinding->mService->mApp);
        }
    }

//    Binder.restoreCallingIdentity(origId);
    *succeeded = TRUE;
    return NOERROR;
}

ECode CActivityManagerService::PublishService(
    /* [in] */ IBinder* token,
    /* [in] */ IIntent* intent,
    /* [in] */ IBinder* service)
{
    // Refuse possible leaked file descriptors
    Boolean hasFD;
    if (intent != NULL && (intent->HasFileDescriptors(&hasFD), hasFD == TRUE)) {
        Slogger::E(TAG, "File descriptors passed in Intent");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Mutex::Autolock lock(_m_syncLock);

    AutoPtr<IObject> tokenObj = (IObject*)token->Probe(EIID_IObject);
    assert(tokenObj != NULL);
    ClassID clsid;
    tokenObj->GetClassID(&clsid);
    if (clsid != ECLSID_CServiceRecord) {
        Slogger::E(TAG, "Invalid service token");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<CServiceRecord> r = (CServiceRecord*)token;

//    final long origId = Binder.clearCallingIdentity();

    if (DEBUG_SERVICE) {
        String srDes, intDes, servDes;
        r->GetDescription(&srDes);
        intent->GetDescription(&intDes);
        service->GetDescription(&servDes);
        Slogger::V(TAG, StringBuffer("PUBLISHING ") + srDes
            + " " + intDes + ": " + servDes);
    }
    if (r != NULL) {
        AutoPtr<IIntentFilterComparison> filter;
        CIntentFilterComparison::New(intent, (IIntentFilterComparison**)&filter);
        IntentBindRecord* b = NULL;
        HashMap<AutoPtr<IIntentFilterComparison>, IntentBindRecord*>::Iterator it =
                r->mBindings.Find(filter);
        if (it != r->mBindings.End()) {
            b = it->mSecond;
        }
        if (b != NULL && !b->mReceived) {
            b->mBinder = service;
            b->mRequested = TRUE;
            b->mReceived = TRUE;
            if (r->mConnections.GetSize() > 0) {
                Map<AutoPtr<IServiceConnectionInner>, List<ConnectionRecord*>*>::Iterator it;
                for (it = r->mConnections.Begin(); it != r->mConnections.End(); ++it) {
                    List<ConnectionRecord*>::Iterator crit = it->mSecond->Begin();
                    for(; crit != it->mSecond->End(); ++crit) {
                        ConnectionRecord* c = *crit;
                        Boolean isEqual;
                        filter->Equals(c->mBinding->mIntent->mIntent, &isEqual);
                        if (!isEqual) {
                            if (DEBUG_SERVICE) {
                                String crDes, filDes, intDes;
                                c->GetDescription(&crDes);
                                c->mBinding->mIntent->mIntent->GetDescription(&filDes);
                                intent->GetDescription(&intDes);
                                Slogger::V(TAG,
                                    StringBuffer("Not publishing to: ") + crDes);
                                Slogger::V(TAG, StringBuffer("Bound intent: ") + filDes);
                                Slogger::V(TAG, StringBuffer("Published intent: ") + intDes);
                            }
                            continue;
                        }
                        if (DEBUG_SERVICE) {
                            String crDes;
                            c->GetDescription(&crDes);
                            Slogger::V(TAG, StringBuffer("Publishing to: ") + crDes);
                        }
                        if (FAILED(c->mConn->Connected(r->mName, service))) {
                            String srDes, connDes;
                            r->mName->GetDescription(&srDes);
                            c->mConn->GetDescription(&connDes);
                            Slogger::W(TAG, StringBuffer("Failure sending service ")
                                + srDes + " to connection " + connDes +
                                " (in " + c->mBinding->mClient->mProcessName + ")");
                        }
                    }
                }
            }
        }

        List<AutoPtr<CServiceRecord> >::Iterator it2 =
                Find(mStoppingServices.Begin(), mStoppingServices.End(), r);
        ServiceDoneExecutingLocked(r, it2 == NULL? FALSE : TRUE);

//        Binder.restoreCallingIdentity(origId);
    }
    return NOERROR;
}

ECode CActivityManagerService::UnbindFinished(
    /* [in] */ IBinder* token,
    /* [in] */ IIntent* intent,
    /* [in] */ Boolean doRebind)
{
    // Refuse possible leaked file descriptors
    Boolean hasFD;
    if (intent != NULL && (intent->HasFileDescriptors(&hasFD), hasFD == TRUE)) {
        Slogger::E(TAG, "File descriptors passed in Intent");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Mutex::Autolock lock(_m_syncLock);

    AutoPtr<IObject> tokenObj = (IObject*)token->Probe(EIID_IObject);
    assert(tokenObj != NULL);
    ClassID clsid;
    tokenObj->GetClassID(&clsid);
    if (clsid != ECLSID_CServiceRecord) {
        Slogger::E(TAG, "Invalid service token");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<CServiceRecord> r = (CServiceRecord*)token;

//    final long origId = Binder.clearCallingIdentity();

    if (r != NULL) {
        AutoPtr<IIntentFilterComparison> filter;
        CIntentFilterComparison::New(intent, (IIntentFilterComparison**)&filter);
        IntentBindRecord* b = r->mBindings[filter];
        if (DEBUG_SERVICE) {
            String srDes, ibrDes;
            r->GetDescription(&srDes);
            b->GetDescription(&ibrDes);
            Slogger::V(TAG, StringBuffer("unbindFinished in ") + srDes
                + " at " + ibrDes + ": apps="
                + (Int32)(b != NULL ? b->mApps.GetSize() : 0));
        }
        if (b != NULL) {
            if (b->mApps.GetSize() > 0) {
                // Applications have already bound since the last
                // unbind, so just rebind right here.
                RequestServiceBindingLocked(r, b, TRUE);
            } else {
                // Note to tell the service the next time there is
                // a new client.
                b->mDoRebind = TRUE;
            }
        }

        List<AutoPtr<CServiceRecord> >::Iterator it =
                Find(mStoppingServices.Begin(), mStoppingServices.End(), r);
        ServiceDoneExecutingLocked(r, it == NULL? FALSE : TRUE);

//        Binder.restoreCallingIdentity(origId);
    }
    return NOERROR;
}

ECode CActivityManagerService::ServiceDoneExecuting(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 type,
    /* [in] */ Int32 startId,
    /* [in] */ Int32 res)
{
    Mutex::Autolock lock(_m_syncLock);

    AutoPtr<IObject> tokenObj = (IObject*)token->Probe(EIID_IObject);
    assert(tokenObj != NULL);
    ClassID clsid;
    tokenObj->GetClassID(&clsid);
    if (clsid != ECLSID_CServiceRecord) {
        Slogger::E(TAG, "Invalid service token");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<CServiceRecord> r = (CServiceRecord*)token;
    if (r != NULL) {
        if (type == 1) {
            // This is a call from a service start...  take care of
            // book-keeping.
            r->mCallStart = TRUE;
            switch (res) {
                case Service_START_STICKY_COMPATIBILITY:
                case Service_START_STICKY: {
                    // We are done with the associated start arguments.
                    r->FindDeliveredStart(startId, TRUE);
                    // Don't stop if killed.
                    r->mStopIfKilled = FALSE;
                    break;
                }
                case Service_START_NOT_STICKY: {
                    // We are done with the associated start arguments.
                    r->FindDeliveredStart(startId, TRUE);
                    if (r->mLastStartId == startId) {
                        // There is no more work, and this service
                        // doesn't want to hang around if killed.
                        r->mStopIfKilled = TRUE;
                    }
                    break;
                }
                case Service_START_REDELIVER_INTENT: {
                    // We'll keep this item until they explicitly
                    // call stop for it, but keep track of the fact
                    // that it was delivered.
                    CServiceRecord::StartItem* si = r->FindDeliveredStart(startId, FALSE);
                    if (si != NULL) {
                        si->mDeliveryCount = 0;
                        si->mDoneExecutingCount++;
                        // Don't stop if killed.
                        r->mStopIfKilled = TRUE;
                    }
                    break;
                }
                default:
                    Slogger::E(TAG,
                        StringBuffer("Unknown service start result: ") + res);
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            if (res == Service_START_STICKY_COMPATIBILITY) {
                r->mCallStart = FALSE;
            }
        }

//        final long origId = Binder.clearCallingIdentity();
        List<AutoPtr<CServiceRecord> >::Iterator it =
                Find(mStoppingServices.Begin(), mStoppingServices.End(), r);
        ServiceDoneExecutingLocked(r, it == NULL? FALSE : TRUE);
//        Binder.restoreCallingIdentity(origId);
    } else {
        Slogger::W(TAG, StringBuffer("Done executing unknown service from pid ")
                        + Binder::GetCallingPid());
    }
    return NOERROR;
}

ECode CActivityManagerService::ServiceDoneExecutingLocked(
    /* [in] */ CServiceRecord* r,
    /* [in] */ Boolean inStopping)
{
    if (DEBUG_SERVICE) {
        String srDes, appDes;
        r->GetDescription(&srDes);
        r->mApp->GetDescription(&appDes);
        Slogger::V(TAG, StringBuffer("<<< DONE EXECUTING ") + srDes
                + ": nesting=" + r->mExecuteNesting
                + ", inStopping=" + inStopping + ", app=" + appDes);
    }
    else if (DEBUG_SERVICE_EXECUTING) {
        Slogger::V(TAG, StringBuffer("<<< DONE EXECUTING ") + r->mShortName);
    }
    r->mExecuteNesting--;
    if (r->mExecuteNesting <= 0 && r->mApp != NULL) {
        if (DEBUG_SERVICE) {
            Slogger::V(TAG, StringBuffer("Nesting at 0 of ") + r->mShortName);
        }
        r->mApp->mExecutingServices.Erase(r);
        if (r->mApp->mExecutingServices.GetSize() == 0) {
            if (DEBUG_SERVICE || DEBUG_SERVICE_EXECUTING) {
                Slogger::V(TAG, StringBuffer("No more executingServices of ")
                    + r->mShortName);
            }
//            mHandler.removeMessages(SERVICE_TIMEOUT_MSG, r.app);
        }
        if (inStopping) {
            if (DEBUG_SERVICE) {
                String srDes;
                r->GetDescription(&srDes);
                Slogger::V(TAG, StringBuffer("doneExecuting remove stopping ")
                    + srDes);
            }
            mStoppingServices.Remove(r);
            r->mBindings.Clear();
        }
        UpdateOomAdjLocked(r->mApp);
    }
    return NOERROR;
}

void CActivityManagerService::ServiceTimeout(
    /* [in] */ ProcessRecord* proc)
{
    StringBuffer anrMessage;

    {
        Mutex::Autolock lock(_m_syncLock);

        if (proc->mExecutingServices.GetSize() == 0 || proc->mAppApartment == NULL) {
            return;
        }
        Millisecond64 maxTime = SystemClock::GetUptimeMillis() - SERVICE_TIMEOUT;
        Millisecond64 nextTime = 0;
        AutoPtr<CServiceRecord> timeout;
        Set<AutoPtr<CServiceRecord> >::Iterator it;
        for (it = proc->mExecutingServices.Begin();
             it != proc->mExecutingServices.End(); ++it) {
            AutoPtr<CServiceRecord> sr = *it;
            if (sr->mExecutingStart < maxTime) {
                timeout = sr;
                break;
            }
            if (sr->mExecutingStart > nextTime) {
                nextTime = sr->mExecutingStart;
            }
        }
        if (timeout != NULL && (Find(mLRUProcesses.Begin(),
                mLRUProcesses.End(), proc) != mLRUProcesses.End())) {
            String srDes;
            timeout->GetDescription(&srDes);
            Slogger::W(TAG, StringBuffer("Timeout executing service: ") + srDes);
            anrMessage += "Executing service " + timeout->mShortName;
        }
        else {
//            Message msg = mHandler.obtainMessage(SERVICE_TIMEOUT_MSG);
//            msg.obj = proc;
//            mHandler.sendMessageAtTime(msg, nextTime+SERVICE_TIMEOUT);
        }
    }

    if (!anrMessage.IsNull()) {
        AppNotResponding(proc, NULL, NULL, anrMessage);
    }
}

void CActivityManagerService::HandleServiceTimeout(
    /* [in] */ ProcessRecord* proc)
{
//    if (mDidDexOpt) {
//        mDidDexOpt = false;
//        Message nmsg = mHandler.obtainMessage(SERVICE_TIMEOUT_MSG);
//        nmsg.obj = msg.obj;
//        mHandler.sendMessageDelayed(nmsg, SERVICE_TIMEOUT);
//        return;
//    }
    ServiceTimeout(proc);
}

// =========================================================
// BACKUP AND RESTORE
// =========================================================

// Cause the target app to be launched if necessary and its backup agent
// instantiated.  The backup agent will invoke backupAgentCreated() on the
// activity manager to announce its creation.
ECode CActivityManagerService::BindBackupAgent(
        /* [in] */ IApplicationInfo* app,
        /* [in] */ Int32 backupMode,
        /* [out] */ Boolean* result)
{
//    if (DEBUG_BACKUP) Slog.v(TAG, "startBackupAgent: app=" + app + " mode=" + backupMode);
    EnforceCallingPermission("android.permission.BACKUP", "startBackupAgent");
//
//    synchronized(this) {
//        // !!! TODO: currently no check here that we're already bound
//        BatteryStatsImpl.Uid.Pkg.Serv ss = null;
//        BatteryStatsImpl stats = mBatteryStatsService.getActiveStatistics();
//        synchronized (stats) {
//            ss = stats.getServiceStatsLocked(app.uid, app.packageName, app.name);
//        }
//
//        BackupRecord r = new BackupRecord(ss, app, backupMode);
//        ComponentName hostingName = new ComponentName(app.packageName, app.backupAgentName);
//        // startProcessLocked() returns existing proc's record if it's already running
//        ProcessRecord proc = startProcessLocked(app.processName, app,
//                false, 0, "backup", hostingName, false);
//        if (proc == null) {
//            Slog.e(TAG, "Unable to start backup agent process " + r);
//            return false;
//        }
//
//        r.app = proc;
//        mBackupTarget = r;
//        mBackupAppName = app.packageName;
//
//        // Try not to kill the process during backup
//        updateOomAdjLocked(proc);
//
//        // If the process is already attached, schedule the creation of the backup agent now.
//        // If it is not yet live, this will be done when it attaches to the framework.
//        if (proc.thread != null) {
//            if (DEBUG_BACKUP) Slog.v(TAG, "Agent proc already running: " + proc);
//            try {
//                proc.thread.scheduleCreateBackupAgent(app, backupMode);
//            } catch (RemoteException e) {
//                // Will time out on the backup manager side
//            }
//        } else {
//            if (DEBUG_BACKUP) Slog.v(TAG, "Agent proc not running, waiting for attach");
//        }
//        // Invariants: at this point, the target app process exists and the application
//        // is either already running or in the process of coming up.  mBackupTarget and
//        // mBackupAppName describe the app, so that when it binds back to the AM we
//        // know that it's scheduled for a backup-agent operation.
//    }
//
//    return true;
    return E_NOT_IMPLEMENTED;
}

// A backup agent has just come up
ECode CActivityManagerService::BackupAgentCreated(
    /* [in] */ const String& agentCapsuleName,
    /* [in] */ IBinder* agent)
{
//    if (DEBUG_BACKUP) Slog.v(TAG, "backupAgentCreated: " + agentPackageName
//            + " = " + agent);
//
//    synchronized(this) {
//        if (!agentPackageName.equals(mBackupAppName)) {
//            Slog.e(TAG, "Backup agent created for " + agentPackageName + " but not requested!");
//            return;
//        }
//    }
//
//    long oldIdent = Binder.clearCallingIdentity();
//    try {
//        IBackupManager bm = IBackupManager.Stub.asInterface(
//                ServiceManager.getService(Context.BACKUP_SERVICE));
//        bm.agentConnected(agentPackageName, agent);
//    } catch (RemoteException e) {
//        // can't happen; the backup manager service is local
//    } catch (Exception e) {
//        Slogger::W(TAG, "Exception trying to deliver BackupAgent binding: ");
//        e.printStackTrace();
//    } finally {
//        Binder.restoreCallingIdentity(oldIdent);
//    }
    return E_NOT_IMPLEMENTED;
}

// done with this agent
ECode CActivityManagerService::UnbindBackupAgent(
    /* [in] */ IApplicationInfo* appInfo)
{
//    if (DEBUG_BACKUP) Slog.v(TAG, "unbindBackupAgent: " + appInfo);
//    if (appInfo == null) {
//        Slogger::W(TAG, "unbind backup agent for null app");
//        return;
//    }
//
//    synchronized(this) {
//        if (mBackupAppName == null) {
//            Slogger::W(TAG, "Unbinding backup agent with no active backup");
//            return;
//        }
//
//        if (!mBackupAppName.equals(appInfo.packageName)) {
//            Slog.e(TAG, "Unbind of " + appInfo + " but is not the current backup target");
//            return;
//        }
//
//        ProcessRecord proc = mBackupTarget.app;
//        mBackupTarget = null;
//        mBackupAppName = null;
//
//        // Not backing this app up any more; reset its OOM adjustment
//        updateOomAdjLocked(proc);
//
//        // If the app crashed during backup, 'thread' will be null here
//        if (proc.thread != null) {
//            try {
//                proc.thread.scheduleDestroyBackupAgent(appInfo);
//            } catch (Exception e) {
//                Slog.e(TAG, "Exception when unbinding backup agent:");
//                e.printStackTrace();
//            }
//        }
//    }
    return E_NOT_IMPLEMENTED;
}

// =========================================================
// BROADCAST RECEIVER
// =========================================================

List<AutoPtr<IIntent> >* CActivityManagerService::GetStickiesLocked(
    /* [in] */ const String& action,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ List<AutoPtr<IIntent> >* cur)
{
//    final ContentResolver resolver = mContext.getContentResolver();
//    final ArrayList<Intent> list = mStickyBroadcasts.get(action);
//    if (list == null) {
//        return cur;
//    }
//    int N = list.size();
//    for (int i=0; i<N; i++) {
//        Intent intent = list.get(i);
//        if (filter.match(resolver, intent, true, TAG) >= 0) {
//            if (cur == null) {
//                cur = new ArrayList<Intent>();
//            }
//            cur.add(intent);
//        }
//    }
//    return cur;
    return NULL;
}

ECode CActivityManagerService::ScheduleBroadcastsLocked()
{
    ECode ec;

    if (DEBUG_BROADCAST) {
        Slogger::V(TAG, StringBuffer("Schedule broadcasts: current=")
                + mBroadcastsScheduled);
    }

    if (mBroadcastsScheduled) return NOERROR;

    //send message
    ECode (STDCALL CActivityManagerService::*pHandlerFunc)(Boolean fromMsg);
    pHandlerFunc = &CActivityManagerService::ProcessNextBroadcast;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteBoolean(TRUE);
    ec = mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);

    mBroadcastsScheduled = TRUE;
    return ec;
}

ECode CActivityManagerService::RegisterReceiver(
    /* [in] */ IApplicationApartment* caller,
    /* [in] */ IIntentReceiver* receiver,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ const String& permission,
    /* [out] */ IIntent** intent)
{
//    synchronized(this) {
//        ProcessRecord callerApp = null;
//        if (caller != null) {
//            callerApp = getRecordForAppLocked(caller);
//            if (callerApp == null) {
//                throw new SecurityException(
//                        "Unable to find app for caller " + caller
//                        + " (pid=" + Binder.getCallingPid()
//                        + ") when registering receiver " + receiver);
//            }
//        }
//
//        List allSticky = null;
//
//        // Look for any matching sticky broadcasts...
//        Iterator actions = filter.actionsIterator();
//        if (actions != null) {
//            while (actions.hasNext()) {
//                String action = (String)actions.next();
//                allSticky = getStickiesLocked(action, filter, allSticky);
//            }
//        } else {
//            allSticky = getStickiesLocked(null, filter, allSticky);
//        }
//
//        // The first sticky in the list is returned directly back to
//        // the client.
//        Intent sticky = allSticky != null ? (Intent)allSticky.get(0) : null;
//
//        if (DEBUG_BROADCAST) Slog.v(TAG, "Register receiver " + filter
//                + ": " + sticky);
//
//        if (receiver == null) {
//            return sticky;
//        }
//
//        ReceiverList rl
//            = (ReceiverList)mRegisteredReceivers.get(receiver.asBinder());
//        if (rl == null) {
//            rl = new ReceiverList(this, callerApp,
//                    Binder.getCallingPid(),
//                    Binder.getCallingUid(), receiver);
//            if (rl.app != null) {
//                rl.app.receivers.add(rl);
//            } else {
//                try {
//                    receiver.asBinder().linkToDeath(rl, 0);
//                } catch (RemoteException e) {
//                    return sticky;
//                }
//                rl.linkedToDeath = true;
//            }
//            mRegisteredReceivers.put(receiver.asBinder(), rl);
//        }
//        BroadcastFilter bf = new BroadcastFilter(filter, rl, permission);
//        rl.add(bf);
//        if (!bf.debugCheck()) {
//            Slogger::W(TAG, "==> For Dynamic broadast");
//        }
//        mReceiverResolver.addFilter(bf);
//
//        // Enqueue broadcasts for all existing stickies that match
//        // this filter.
//        if (allSticky != null) {
//            ArrayList receivers = new ArrayList();
//            receivers.add(bf);
//
//            int N = allSticky.size();
//            for (int i=0; i<N; i++) {
//                Intent intent = (Intent)allSticky.get(i);
//                BroadcastRecord r = new BroadcastRecord(intent, null,
//                        null, -1, -1, null, receivers, null, 0, null, null,
//                        false, true, true);
//                if (mParallelBroadcasts.size() == 0) {
//                    scheduleBroadcastsLocked();
//                }
//                mParallelBroadcasts.add(r);
//            }
//        }
//
//        return sticky;
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::UnregisterReceiver(
    /* [in] */ IIntentReceiver* receiver)
{
//    if (DEBUG_BROADCAST) Slog.v(TAG, "Unregister receiver: " + receiver);
//
//    boolean doNext = false;
//
//    synchronized(this) {
//        ReceiverList rl
//            = (ReceiverList)mRegisteredReceivers.get(receiver.asBinder());
//        if (rl != null) {
//            if (rl.curBroadcast != null) {
//                BroadcastRecord r = rl.curBroadcast;
//                doNext = finishReceiverLocked(
//                    receiver.asBinder(), r.resultCode, r.resultData,
//                    r.resultExtras, r.resultAbort, true);
//            }
//
//            if (rl.app != null) {
//                rl.app.receivers.remove(rl);
//            }
//            removeReceiverLocked(rl);
//            if (rl.linkedToDeath) {
//                rl.linkedToDeath = false;
//                rl.receiver.asBinder().unlinkToDeath(rl, 0);
//            }
//        }
//    }
//
//    if (!doNext) {
//        return;
//    }
//
//    final long origId = Binder.clearCallingIdentity();
//    processNextBroadcast(false);
//    trimApplications();
//    Binder.restoreCallingIdentity(origId);
    return E_NOT_IMPLEMENTED;
}

void CActivityManagerService::RemoveReceiverLocked(
    /* [in] */ ReceiverList* rl)
{
    mRegisteredReceivers.Erase(rl->mReceiver);
    List<BroadcastFilter*>::Iterator it = rl->Begin();
    for (; it != rl->End(); ++it) {
        mReceiverResolver->RemoveFilter(*it);
    }
}

void CActivityManagerService::SendCapsuleBroadcastLocked(
    /* [in] */ Int32 cmd,
    /* [in] */ ArrayOf<String>& capsules)
{
    List<ProcessRecord*>::ReverseIterator rit = mLRUProcesses.RBegin();
    for (; rit != mLRUProcesses.REnd(); ++rit) {
        ProcessRecord* r = *rit;
        if (r->mAppApartment != NULL) {
            r->mAppApartment->DispatchCapsuleBroadcast(cmd, capsules);
        }
    }
}

ECode CActivityManagerService::BroadcastIntentLocked(
    /* [in] */ ProcessRecord* callerApp,
    /* [in] */ const String& callerCapsule,
    /* [in] */ IIntent* _intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ IIntentReceiver* resultTo,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& resultData,
    /* [in] */ IBundle* map,
    /* [in] */ const String& requiredPermission,
    /* [in] */ Boolean ordered,
    /* [in] */ Boolean sticky,
    /* [in] */ Int32 callingPid,
    /* [in] */ Int32 callingUid,
    /* [out] */ Int32* result)
{
    AutoPtr<IIntent> intent;
    CIntent::New(_intent, (IIntent**)&intent);

    if (DEBUG_BROADCAST_LIGHT) {
        String intDes;
        intent->GetDescription(&intDes);
        Slogger::V(TAG, StringBuffer(sticky ? "Broadcast sticky: ": "Broadcast: ")
                + intDes + " ordered=" + ordered);
    }
    if ((resultTo != NULL) && !ordered) {
        String intDes;
        intent->GetDescription(&intDes);
        Slogger::W(TAG, StringBuffer("Broadcast ") + intDes
                + " not ordered but result callback requested!");
    }

    // Handle special intents: if this broadcast is from the package
    // manager about a package being removed, we need to remove all of
    // its activities from the history stack.
    String action;
    intent->GetAction(&action);
    Boolean uidRemoved = !action.Compare(Intent_ACTION_UID_REMOVED);
    if (!action.Compare(Intent_ACTION_CAPSULE_REMOVED)
            || !action.Compare(Intent_ACTION_CAPSULE_CHANGED)
            || !action.Compare(Intent_ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE)
            || uidRemoved) {
        if (CheckComponentPermission(
                "elastos.permission.BROADCAST_CAPSULE_REMOVED"/*android.Manifest.permission.BROADCAST_CAPSULE_REMOVED*/,
                callingPid, callingUid, -1)
                == CapsuleManager_PERMISSION_GRANTED) {
            if (uidRemoved) {
                AutoPtr<IBundle> intentExtras;
                intent->GetExtras((IBundle**)&intentExtras);
                Int32 uid = -1;
                if (intentExtras != NULL) {
                    intentExtras->GetInt32(String(Intent_EXTRA_UID), &uid);
                }
//                if (uid >= 0) {
//                    AutoPtr<BatteryStatsImpl> bs = mBatteryStatsService->GetActiveStatistics();
//                    bs->Lock();
//                    bs->RemoveUidStatsLocked(uid);
//                    bs->Unlock();
//                }
            } else {
                // If resources are unvailble just force stop all
                // those packages and flush the attribute cache as well.
                if (!action.Compare(Intent_ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE)) {
                    ArrayOf<String>* list;
                    intent->GetStringArrayExtra(String(Intent_EXTRA_CHANGED_CAPSULE_LIST), &list);
//                    String list[] = intent.getStringArrayExtra(Intent.EXTRA_CHANGED_CAPSULE_LIST);
                    if (list != NULL && (list->GetLength() > 0)) {
                        for (Int32 i = 0; i < list->GetLength(); i++) {
                            ForceStopCapsuleLocked((*list)[i], -1, FALSE, TRUE, TRUE);
                        }
                        SendCapsuleBroadcastLocked(
                                ApplicationApartment_EXTERNAL_STORAGE_UNAVAILABLE, *list);
                    }
                } else {
                    AutoPtr<IUri> data;
                    intent->GetData((IUri**)&data);
                    String ssp;
                    if (data != NULL &&
                            (data->GetSchemeSpecificPart(&ssp), !ssp.IsNull())) {
                        Boolean dontKillApp = FALSE;
                        intent->GetBooleanExtra(String(Intent_EXTRA_DONT_KILL_APP), FALSE, &dontKillApp);
                        if (!dontKillApp) {
                            Int32 uid = -1;
                            intent->GetInt32Extra(String(Intent_EXTRA_UID), -1, &uid);
                            ForceStopCapsuleLocked(ssp, uid, FALSE, TRUE, TRUE);
                        }
                        if (!action.Compare(Intent_ACTION_CAPSULE_REMOVED)) {
                            ArrayOf_<String, 1> sa;
                            sa[0] = ssp;
                            SendCapsuleBroadcastLocked(ApplicationApartment_CAPSULE_REMOVED,
                                    sa);
                        }
                    }
                }
            }
        } else {
            String action;
            intent->GetAction(&action);
            StringBuffer msg;
            msg += "Permission Denial: " + action + " broadcast from " + callerCapsule
                    + " (pid=" + callingPid + ", uid=" + callingUid + ")" + " requires "
                    + "elastos.permission.BROADCAST_CAPSULE_REMOVED"\
                    /*android.Manifest.permission.BROADCAST_CAPSULE_REMOVED*/;
            Slogger::W(TAG, msg);
            return E_SECURITY_EXCEPTION;
        }
    }

    /*
     * If this is the time zone changed action, queue up a message that will reset the timezone
     * of all currently running processes. This message will get queued up before the broadcast
     * happens.
     */
    if (!action.Compare(Intent_ACTION_TIMEZONE_CHANGED)) {
//        mHandler.sendEmptyMessage(UPDATE_TIME_ZONE);
    }

    /*
     * Prevent non-system code (defined here to be non-persistent
     * processes) from sending protected broadcasts.
     */
    if (callingUid == Process::SYSTEM_UID || callingUid == Process::PHONE_UID
            || callingUid == Process::SHELL_UID || callingUid == 0) {
        // Always okay.
    } else if (callerApp == NULL || !callerApp->mPersistent) {
        String action;
        intent->GetAction(&action);
        Boolean res;
        ECode ec = GetCapsuleManager()->IsProtectedBroadcast(action, &res);
        if (SUCCEEDED(ec)) {
            if (res) {
                StringBuffer msg;
                msg += "Permission Denial: not allowed to send broadcast "
                    + action + " from pid=" + callingPid + ", uid=" + callingUid;
                Slogger::W(TAG, msg);
                return E_SECURITY_EXCEPTION;
            }
        }
        else {
            assert(ec == E_REMOTE_EXCEPTION);
            Slogger::W(TAG, "Remote exception");
            *result = BROADCAST_SUCCESS;
            return NOERROR;
        }
    }

    // Add to the sticky list if requested.
    if (sticky) {
	Int32 permission;
	CheckPermission("elastos.permission.BROADCAST_STICKY"/*android.Manifest.permission.BROADCAST_STICKY*/,
                callingPid, callingUid, &permission);
        if (permission != CapsuleManager_PERMISSION_GRANTED) {
            StringBuffer msg;
            msg = msg + "Permission Denial: broadcastIntent() requesting a sticky broadcast from pid="
                    + callingPid + ", uid=" + callingUid + " requires "
                    + "elastos.permission.BROADCAST_STICKY"\
                    /*android.Manifest.permission.BROADCAST_STICKY*/;
            Slogger::W(TAG, msg);
            return E_SECURITY_EXCEPTION;
        }
        if (!requiredPermission.IsNull()) {
            String intDes;
            intent->GetDescription(&intDes);
            Slogger::W(TAG, StringBuffer("Can't broadcast sticky intent ") + intDes
                    + " and enforce permission " + requiredPermission);
            *result = BROADCAST_STICKY_CANT_HAVE_PERMISSION;
            return NOERROR;
        }
        AutoPtr<IComponentName> component;
        intent->GetComponent((IComponentName**)&component);
        if (component != NULL) {
            Slogger::E(TAG, "Sticky broadcasts can't target a specific component");
            return E_SECURITY_EXCEPTION;
        }
        List<AutoPtr<IIntent> >* list = mStickyBroadcasts[action];
        if (list == NULL) {
            list = new List<AutoPtr<IIntent> >();
            mStickyBroadcasts[action] = list;
        }
        List<AutoPtr<IIntent> >::Iterator it = list->Begin();
        for (; it != list->End(); ++it) {
            Boolean equals;
            intent->FilterEquals((IIntent*)*it, &equals);
            if (equals) {
                // This sticky already exists, replace it.
                AutoPtr<IIntent> newIntent;
                CIntent::New(intent, (IIntent**)&newIntent);
                list->Insert(it, newIntent);
                break;
            }
        }
        if (it == list->End()) {
            AutoPtr<IIntent> newIntent;
            CIntent::New(intent, (IIntent**)&newIntent);
            list->PushBack(newIntent);
        }
    }

    // Figure out who all will receive this broadcast.
    //List<ResolveInfo>
    AutoPtr<IObjectContainer> receivers = NULL;
    List<BroadcastFilter*>* registeredReceivers = NULL;

    AutoPtr<IComponentName> component;
    intent->GetComponent((IComponentName**)&component);
    if (component != NULL) {
        // Broadcast is going to one specific receiver class...
        AutoPtr<IActivityInfo> ai;
        GetCapsuleManager()->GetReceiverInfo(component,
                STOCK_PM_FLAGS, (IActivityInfo**)&ai);
        if (ai != NULL) {
            CParcelableObjectContainer::New((IObjectContainer**)&receivers);
            AutoPtr<IResolveInfo> ri;
            CResolveInfo::New((IResolveInfo**)&ri);
            ri->SetActivityInfo(ai);
            receivers->Add(ri);
        }
    }
    else {
        // Need to resolve the intent to interested receivers...
        Int32 flags;
        intent->GetFlags(&flags);
        if ((flags & Intent_FLAG_RECEIVER_REGISTERED_ONLY) == 0) {
            GetCapsuleManager()->QueryIntentReceivers(intent, resolvedType, flags,
                    (IObjectContainer**)&receivers);
        }
        registeredReceivers = mReceiverResolver->QueryIntent(
                intent, resolvedType, FALSE);
    }

    Int32 flags;
    intent->GetFlags(&flags);

    Boolean replacePending =
            (flags & Intent_FLAG_RECEIVER_REPLACE_PENDING) !=  0;

    if (DEBUG_BROADCAST) {
        String action;
        intent->GetAction(&action);
        Slogger::V(TAG, StringBuffer("Enqueing broadcast: ") + action
                + " replacePending=" + replacePending);
    }

    Int32 NR = registeredReceivers != NULL ? registeredReceivers->GetSize() : 0;
    if (!ordered && NR > 0) {
        List<AutoPtr<IObject> >* rlist = new List<AutoPtr<IObject> >();
        List<BroadcastFilter*>::Iterator it = registeredReceivers->Begin();
        for (; it != registeredReceivers->End(); ++it) {
            rlist->PushBack((IObject*)*it);
        }
        // If we are not serializing this broadcast, then send the
        // registered receivers separately so they don't wait for the
        // components to be launched.
        BroadcastRecord* r = new BroadcastRecord(intent, callerApp,
                callerCapsule, callingPid, callingUid, requiredPermission,
                rlist, resultTo, resultCode, resultData, map,
                ordered, sticky, FALSE);
        if (DEBUG_BROADCAST) {
            String brDes;
            r->GetDescription(&brDes);
            Slogger::V(TAG, StringBuffer("Enqueueing parallel broadcast ") + brDes
                    + ": prev had " + (Int32)mParallelBroadcasts.GetSize());
        }
        Boolean replaced = FALSE;
        if (replacePending) {
            List<BroadcastRecord*>::ReverseIterator it = mParallelBroadcasts.RBegin();
            for (; it != mParallelBroadcasts.REnd(); ++it) {
                Boolean equals;
                intent->FilterEquals((IIntent*)*it, &equals);
                if (equals) {
                    if (DEBUG_BROADCAST) {
                        String intDes;
                        intent->GetDescription(&intDes);
                        Slogger::V(TAG, StringBuffer("***** DROPPING PARALLEL: ") + intDes);
                    }
                    mParallelBroadcasts.Insert(it.GetBase(), r);
                    replaced = TRUE;
                    break;
                }
            }
        }
        if (!replaced) {
            mParallelBroadcasts.PushBack(r);
            ScheduleBroadcastsLocked();
        }
        registeredReceivers = NULL;
        NR = 0;
    }

    // Merge into one list.
    List<BroadcastFilter*>::Iterator ir = registeredReceivers->Begin();
    if (receivers != NULL) {
        // A special case for CAPSULE_ADDED: do not allow the package
        // being added to see this broadcast.  This prevents them from
        // using this as a back door to get run as soon as they are
        // installed.  Maybe in the future we want to have a special install
        // broadcast or such for apps, but we'd like to deliberately make
        // this decision.
        AutoStringArray skipCapsules;
        if (!action.Compare(Intent_ACTION_CAPSULE_ADDED)
                || !action.Compare(Intent_ACTION_CAPSULE_RESTARTED)
                || !action.Compare(Intent_ACTION_CAPSULE_DATA_CLEARED)) {
            AutoPtr<IUri> data;
            intent->GetData((IUri**)&data);
            if (data != NULL) {
                String capName;
                data->GetSchemeSpecificPart(&capName);
                if (!capName.IsNull()) {
                    skipCapsules = ArrayOf<String>::Alloc(1);
                    (*skipCapsules)[0] = capName;
                }
            }
        } else if (!action.Compare(Intent_ACTION_EXTERNAL_APPLICATIONS_AVAILABLE)) {
            intent->GetStringArrayExtra(String(Intent_EXTRA_CHANGED_CAPSULE_LIST),
                    (ArrayOf<String>**)&skipCapsules);
        }
        if (skipCapsules != NULL && (skipCapsules->GetLength() > 0)) {
            for (Int32 i = 0; i < skipCapsules->GetLength(); ++i) {
                String skipCapsule = (*skipCapsules)[i];
                if (!skipCapsule.IsNull()) {
                    AutoPtr<IObjectEnumerator> en;
                    receivers->GetObjectEnumerator((IObjectEnumerator**)&en);
                    Boolean hasNext;
                    while (en->MoveNext(&hasNext), hasNext) {
                        AutoPtr<IInterface> itf;
                        IResolveInfo* curt;
                        en->Current((IInterface**)&itf);
                        curt = IResolveInfo::Probe(itf);
                        AutoPtr<IActivityInfo> aInfo;
                        curt->GetActivityInfo((IActivityInfo**)&aInfo);
                        String cname;
                        aInfo->GetCapsuleName(&cname);
                        if (!cname.Compare(skipCapsule)) {
                            receivers->Remove(curt);
                        }
                    }
                }
            }
        }

        IResolveInfo* curt = NULL;
        BroadcastFilter* curr = NULL;
        AutoPtr<IObjectEnumerator> en;
        receivers->GetObjectEnumerator((IObjectEnumerator**)&en);
        Boolean hasNext;
        en->MoveNext(&hasNext);
        while (hasNext && ir != registeredReceivers->End()) {
            if (curt == NULL) {
                AutoPtr<IInterface> itf;
                en->Current((IInterface**)&itf);
                curt = IResolveInfo::Probe(itf);
            }
            if (curr == NULL) {
                curr = *ir;
            }
            Int32 curtPriority;
            curt->GetPriority(&curtPriority);
            if (curr->GetPriority() >= curtPriority) {
//                // Insert this broadcast record into the final list.
//                receivers->Add(curr);
//                ++ir;
//                curr = NULL;
//                en->MoveNext(&hasNext);
            }
            else {
                // Skip to the next ResolveInfo in the final list.
                en->MoveNext(&hasNext);
                curt = NULL;
            }
        }
    }
    while (ir != registeredReceivers->End()) {
        if (receivers == NULL) {
            CParcelableObjectContainer::New((IObjectContainer**)&receivers);
        }
        receivers->Add(*ir);
        ir++;
    }

    Int32 count;
    if ((receivers != NULL && (receivers->GetObjectCount(&count), count > 0))
            || resultTo != NULL) {
        List<AutoPtr<IObject> >* rlist = new List<AutoPtr<IObject> >();
        Boolean hasNext = FALSE;
        AutoPtr<IObjectEnumerator> enumerator = NULL;
        receivers->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
        while(enumerator->MoveNext(&hasNext), hasNext) {
            AutoPtr<IInterface> itf;
            AutoPtr<IObject> obj;
            enumerator->Current((IInterface**)&itf);
            obj = (IObject*)itf->Probe(EIID_IObject);
            rlist->PushBack(obj);
        }
        BroadcastRecord* r = new BroadcastRecord(intent, callerApp,
                callerCapsule, callingPid, callingUid, requiredPermission,
                rlist, resultTo, resultCode, resultData, map, ordered,
                sticky, FALSE);
        if (DEBUG_BROADCAST) {
            String brDes;
            r->GetDescription(&brDes);
            Slogger::V(TAG, StringBuffer("Enqueueing ordered broadcast ") + brDes
                + ": prev had " + (Int32)mOrderedBroadcasts.GetSize());
        }
        if (DEBUG_BROADCAST) {
            Int32 seq = -1;
            r->mIntent->GetInt32Extra(String("seq"), -1, &seq);
            String action;
            r->mIntent->GetAction(&action);
            Slogger::I(TAG, StringBuffer("Enqueueing broadcast ") + action + " seq=" + seq);
        }
        Boolean replaced = FALSE;
        if (replacePending) {
            List<BroadcastRecord*>::ReverseIterator it = mOrderedBroadcasts.RBegin();
            for (; it != --mOrderedBroadcasts.REnd(); ++it) {
                Boolean equals;
                intent->FilterEquals((IIntent*)*it, &equals);
                if (equals) {
                    if (DEBUG_BROADCAST) {
                        String intDes;
                        intent->GetDescription(&intDes);
                        Slogger::V(TAG, StringBuffer("***** DROPPING ORDERED: ") + intDes);
                    }
                    mOrderedBroadcasts.Insert(it.GetBase(), r);
                    replaced = TRUE;
                    break;
                }
            }
        }
        if (!replaced) {
            mOrderedBroadcasts.PushBack(r);
            ScheduleBroadcastsLocked();
        }
    }

    *result = BROADCAST_SUCCESS;
    return NOERROR;
}

ECode CActivityManagerService::VerifyBroadcastLocked(
    /* [in] */ IIntent* intent,
    /* [out] */ IIntent** newIntent)
{
    // Refuse possible leaked file descriptors
    *newIntent = intent;
    if (*newIntent != NULL) {
        (*newIntent)->AddRef();

        Boolean hasFD;
        if ((*newIntent)->HasFileDescriptors(&hasFD), hasFD) {
            Slogger::E(TAG, "File descriptors passed in Intent.\n");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    Int32 flags;
    (*newIntent)->GetFlags(&flags);

    if (!mProcessesReady) {
        // if the caller really truly claims to know what they're doing, go
        // ahead and allow the broadcast without launching any receivers
        if ((flags & Intent_FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT) != 0) {
            if (*newIntent != NULL) (*newIntent)->Release();
            CIntent::New(intent, newIntent);
            (*newIntent)->AddFlags(Intent_FLAG_RECEIVER_REGISTERED_ONLY);
        }
        else if ((flags & Intent_FLAG_RECEIVER_REGISTERED_ONLY) == 0){
            String intDes;
            intent->GetDescription(&intDes);
            Slogger::E(TAG, "Attempt to launch receivers of broadcast intent " + intDes
                    + " before boot completion");
            Slogger::E(TAG, "Cannot broadcast before boot completed");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
    }

    if ((flags & Intent_FLAG_RECEIVER_BOOT_UPGRADE) != 0) {
        Slogger::E(TAG, "Can't use FLAG_RECEIVER_BOOT_UPGRADE here");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

ECode CActivityManagerService::BroadcastIntent(
    /* [in] */ IApplicationApartment* caller,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ IIntentReceiver* resultTo,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& resultData,
    /* [in] */ IBundle* map,
    /* [in] */ const String& requiredPermission,
    /* [in] */ Boolean serialized,
    /* [in] */ Boolean sticky,
    /* [in] */ Int32 callingPid,
    /* [in] */ Int32 callingUid,
    /* [out] */ Int32* result)
{
    if (result == NULL) return E_INVALID_ARGUMENT;

    SelfLock();
    AutoPtr<IIntent> newIntent;
    VerifyBroadcastLocked(intent, (IIntent**)&newIntent);
    ProcessRecord* callerApp = GetProcessRecordForAppLocked(caller);
    String callerCapsule;
    if (callerApp != NULL) {
        callerApp->mInfo->GetCapsuleName(&callerCapsule);
    }
    ECode ec = BroadcastIntentLocked(callerApp,
            callerCapsule, intent, resolvedType, resultTo,
            resultCode, resultData, map, requiredPermission, serialized,
            sticky, callingPid, callingUid, result);
    SelfUnlock();

    return ec;
}

ECode CActivityManagerService::BroadcastIntentInCapsuel(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 uid,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ IIntentReceiver* resultTo,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& resultData,
    /* [in] */ IBundle* map,
    /* [in] */ const String& requiredPermission,
    /* [in] */ Boolean serialized,
    /* [in] */ Boolean sticky,
    /* [out] */ Int32* result)
{
//    synchronized(this) {
//        intent = verifyBroadcastLocked(intent);
//
//        final long origId = Binder.clearCallingIdentity();
//        int res = broadcastIntentLocked(null, packageName, intent, resolvedType,
//                resultTo, resultCode, resultData, map, requiredPermission,
//                serialized, sticky, -1, uid);
//        Binder.restoreCallingIdentity(origId);
//        return res;
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::UnbroadcastIntent(
    /* [in] */ IApplicationApartment* caller,
    /* [in] */ IIntent* intent)
{
//    // Refuse possible leaked file descriptors
//    if (intent != null && intent.hasFileDescriptors() == true) {
//        throw new IllegalArgumentException("File descriptors passed in Intent");
//    }
//

    Mutex::Autolock lock(_m_syncLock);
    if (CheckCallingPermission("android.permission.BROADCAST_STICKY" /*android.Manifest.permission.BROADCAST_STICKY)*/)
            != CapsuleManager_PERMISSION_GRANTED) {
        Slogger::V(TAG, StringBuffer("Permission Denial: unbroadcastIntent() from pid=") + Binder::GetCallingPid()
            + ", uid=" + Binder::GetCallingUid() + " requires " + "android.permission.BROADCAST_STICKY");
        // throw new SecurityException(msg);
        return E_SECURITY_EXCEPTION;
    }
//        ArrayList<Intent> list = mStickyBroadcasts.get(intent.getAction());
//        if (list != null) {
//            int N = list.size();
//            int i;
//            for (i=0; i<N; i++) {
//                if (intent.filterEquals(list.get(i))) {
//                    list.remove(i);
//                    break;
//                }
//            }
//        }
//    }
    return E_NOT_IMPLEMENTED;
}

Boolean CActivityManagerService::FinishReceiverLocked(
    /* [in] */ IBinder* receiver,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& resultData,
    /* [in] */ IBundle* resultExtras,
    /* [in] */ Boolean resultAbort,
    /* [in] */ Boolean _explicit)
{
    if (mOrderedBroadcasts.GetSize() == 0) {
        if (_explicit) {
            Slogger::W(TAG, "finishReceiver called but no pending broadcasts");
        }
        return FALSE;
    }
    BroadcastRecord* r = mOrderedBroadcasts[0];
    if (r->mReceiver == NULL) {
        if (_explicit) {
            Slogger::W(TAG, "finishReceiver called but none active");
        }
        return FALSE;
    }
    if ((IInterface*)r->mReceiver != receiver) {
        Slogger::W(TAG, "finishReceiver called but active receiver is different");
        return FALSE;
    }
    Int32 state = r->mState;
    r->mState = r->IDLE;
    if (state == r->IDLE) {
        if (_explicit) {
            Slogger::W(TAG, "finishReceiver called but state is IDLE");
        }
    }
    r->mReceiver = NULL;
    r->mIntent->SetComponent(NULL);
    if (r->mCurApp != NULL) {
        r->mCurApp->mCurReceiver = NULL;
    }
    if (r->mCurFilter != NULL) {
        r->mCurFilter->mReceiverList->mCurBroadcast = NULL;
    }
    r->mCurFilter = NULL;
    r->mCurApp = NULL;
    r->mCurComponent = NULL;
    r->mCurReceiver = NULL;
    mPendingBroadcast = NULL;

    r->mResultCode = resultCode;
    r->mResultData = resultData;
    r->mResultExtras = resultExtras;
    r->mResultAbort = resultAbort;

    // We will process the next receiver right now if this is finishing
    // an app receiver (which is always asynchronous) or after we have
    // come back from calling a receiver.
    return state == BroadcastRecord::APP_RECEIVE
            || state == BroadcastRecord::CALL_DONE_RECEIVE;
}

ECode CActivityManagerService::FinishReceiver(
    /* [in] */ IBinder* who,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& resultData,
    /* [in] */ IBundle* resultExtras,
    /* [in] */ Boolean resultAbort)
{
    if (DEBUG_BROADCAST) {
        String whoDes;
        who->GetDescription(&whoDes);
        Slogger::V(TAG, "Finish receiver: " + whoDes);
    }

    // Refuse possible leaked file descriptors
    Boolean hasFD;
    if (resultExtras != NULL && (resultExtras->HasFileDescriptors(&hasFD), hasFD)) {
        Slogger::E(TAG, "File descriptors passed in Bundle");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Boolean doNext;

//    final long origId = Binder.clearCallingIdentity();

    {
        Mutex::Autolock lock(_m_syncLock);
        doNext = FinishReceiverLocked(
                who, resultCode, resultData, resultExtras, resultAbort, TRUE);
    }

    if (doNext) {
        ProcessNextBroadcast(FALSE);
    }
    TrimApplications();

//    Binder.restoreCallingIdentity(origId);
    return NOERROR;
}

void CActivityManagerService::LogBroadcastReceiverDiscardLocked(
    /* [in] */ BroadcastRecord* r)
{
//    if (r.nextReceiver > 0) {
//        Object curReceiver = r.receivers.get(r.nextReceiver-1);
//        if (curReceiver instanceof BroadcastFilter) {
//            BroadcastFilter bf = (BroadcastFilter) curReceiver;
//            EventLog.writeEvent(EventLogTags.AM_BROADCAST_DISCARD_FILTER,
//                    System.identityHashCode(r),
//                    r.intent.getAction(),
//                    r.nextReceiver - 1,
//                    System.identityHashCode(bf));
//        } else {
//            EventLog.writeEvent(EventLogTags.AM_BROADCAST_DISCARD_APP,
//                    System.identityHashCode(r),
//                    r.intent.getAction(),
//                    r.nextReceiver - 1,
//                    ((ResolveInfo)curReceiver).toString());
//        }
//    } else {
//        Slogger::W(TAG, "Discarding broadcast before first receiver is invoked: "
//                + r);
//        EventLog.writeEvent(EventLogTags.AM_BROADCAST_DISCARD_APP,
//                System.identityHashCode(r),
//                r.intent.getAction(),
//                r.nextReceiver,
//                "NONE");
//    }
}

void CActivityManagerService::SetBroadcastTimeoutLocked(
    /* [in] */ Millisecond64 timeoutTime)
{
//    if (! mPendingBroadcastTimeoutMessage) {
//        Message msg = mHandler.obtainMessage(BROADCAST_TIMEOUT_MSG);
//        mHandler.sendMessageAtTime(msg, timeoutTime);
//        mPendingBroadcastTimeoutMessage = true;
//    }
}

void CActivityManagerService::CancelBroadcastTimeoutLocked()
{
//    if (mPendingBroadcastTimeoutMessage) {
//        mHandler.removeMessages(BROADCAST_TIMEOUT_MSG);
//        mPendingBroadcastTimeoutMessage = false;
//    }
}

void CActivityManagerService::BroadcastTimeoutLocked(
    /* [in] */ Boolean fromMsg)
{
    if (fromMsg) {
        mPendingBroadcastTimeoutMessage = FALSE;
    }

    if (mOrderedBroadcasts.GetSize() == 0) {
        return;
    }

    Millisecond64 now = SystemClock::GetUptimeMillis();
    BroadcastRecord* r = mOrderedBroadcasts.GetFront();
    if (fromMsg) {
//        if (mDidDexOpt) {
//            // Delay timeouts until dexopt finishes.
//            mDidDexOpt = false;
//            long timeoutTime = SystemClock.uptimeMillis() + BROADCAST_TIMEOUT;
//            setBroadcastTimeoutLocked(timeoutTime);
//            return;
//        }
        if (! mProcessesReady) {
            // Only process broadcast timeouts if the system is ready. That way
            // PRE_BOOT_COMPLETED broadcasts can't timeout as they are intended
            // to do heavy lifting for system up.
            return;
        }

        Millisecond64 timeoutTime = r->mReceiverTime + BROADCAST_TIMEOUT;
        if (timeoutTime > now) {
            // We can observe premature timeouts because we do not cancel and reset the
            // broadcast timeout message after each receiver finishes.  Instead, we set up
            // an initial timeout then kick it down the road a little further as needed
            // when it expires.
            if (DEBUG_BROADCAST) {
                Slogger::V(TAG, StringBuffer("Premature timeout @ ") + now
                    + ": resetting BROADCAST_TIMEOUT_MSG for " + timeoutTime);
            }
            SetBroadcastTimeoutLocked(timeoutTime);
            return;
        }
    }

    String brDes;
    r->GetDescription(&brDes);
    Slogger::W(TAG, StringBuffer("Timeout of broadcast ") + brDes
//            + " - receiver=" + r.receiver
            + ", started " + (now - r->mReceiverTime) + "ms ago");
    r->mReceiverTime = now;
    r->mAnrCount++;

//    // Current receiver has passed its expiration date.
//    if (r.nextReceiver <= 0) {
//        Slogger::W(TAG, "Timeout on receiver with nextReceiver <= 0");
//        return;
//    }

    ProcessRecord* app = NULL;
    StringBuffer anrMessage;

    AutoPtr<IObject> curReceiver = *(--r->mNextReceiver);
    ++r->mNextReceiver;
//    Slogger::W(TAG, "Receiver during timeout: " + curReceiver);
    LogBroadcastReceiverDiscardLocked(r);
    ClassID clsid;
    curReceiver->GetClassID(&clsid);
    if (clsid == ECLSID_BroadcastFilter) {
        BroadcastFilter* bf = (BroadcastFilter*)(IObject*)curReceiver;
        if (bf->mReceiverList->mPid != 0
                && bf->mReceiverList->mPid != mMyPid) {
            mPidsSelfLock.Lock();
            app = mPidsSelfLocked[bf->mReceiverList->mPid];
            mPidsSelfLock.Unlock();
        }
    }
    else {
        app = r->mCurApp;
    }

    if (app != NULL) {
        String intentDes;
        r->mIntent->GetDescription(&intentDes);
        anrMessage += "Broadcast of " + intentDes;
    }

    if (mPendingBroadcast == r) {
        mPendingBroadcast = NULL;
    }

    // Move on to the next receiver.
    FinishReceiverLocked(r->mReceiver, r->mResultCode, r->mResultData,
            r->mResultExtras, r->mResultAbort, TRUE);
    ScheduleBroadcastsLocked();

    if (!anrMessage.IsNull()) {
        // Post the ANR to the handler since we do not want to process ANRs while
        // potentially holding our lock.
//        mHandler.post(new AppNotResponding(app, anrMessage));
    }
}

ECode CActivityManagerService::ProcessCurBroadcastLocked(
    /* [in] */ BroadcastRecord* r,
    /* [in] */ ProcessRecord* app)
{
    if (DEBUG_BROADCAST)  {
        String brDes, appDes;
        r->GetDescription(&brDes);
        app->GetDescription(&appDes);
        Slogger::V(TAG, StringBuffer("Process cur broadcast ")
                + brDes + " for app " + appDes);
    }
    if (app->mAppApartment == NULL) return E_REMOTE_EXCEPTION;

    r->mReceiver = app->mAppApartment;
    r->mCurApp = app;
    app->mCurReceiver = r;
    UpdateLRUProcessLocked(app, TRUE, TRUE);

    // Tell the application to launch this receiver.
    String capsuleName, className;
    r->mCurComponent->GetCapsuleName(&capsuleName);
    r->mCurComponent->GetClassName(&className);
    r->mIntent->SetClassNameEx(capsuleName, className);

    if (DEBUG_BROADCAST_LIGHT) {
        String compDes, brDes;
        r->mCurComponent->GetDescription(&compDes);
        r->GetDescription(&brDes);
        Slogger::V(TAG, StringBuffer("Delivering to component ") + compDes
                + ": " + brDes);
    }
//    ensurePackageDexOpt(r.intent.getComponent().getPackageName());
    AutoPtr<IIntent> newIntent;
    CIntent::New(r->mIntent, (IIntent**)&newIntent);
    if (FAILED(app->mAppApartment->ScheduleReceiver(newIntent, r->mCurReceiver,
            r->mResultCode, r->mResultData, r->mResultExtras, r->mOrdered))) {
        r->mReceiver = NULL;
        r->mCurApp = NULL;
        app->mCurReceiver = NULL;
        if (DEBUG_BROADCAST) {
            String brDes;
            r->GetDescription(&brDes);
            Slogger::V(TAG, StringBuffer("Process cur broadcast ")
                    + brDes + ": NOT STARTED!");
        }
        return NOERROR;
    }
    if (DEBUG_BROADCAST) {
        String brDes, appDes;
        r->GetDescription(&brDes);
        app->GetDescription(&appDes);
        Slogger::V(TAG, StringBuffer("Process cur broadcast ")
                + brDes + " DELIVERED for app " + appDes);
    }
    return NOERROR;
}

ECode CActivityManagerService::PerformReceiveLocked(
    /* [in] */ ProcessRecord* app,
    /* [in] */ IIntentReceiver* receiver,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean ordered,
    /* [in] */ Boolean sticky)
{
    // Send the intent to the receiver asynchronously using one-way binder calls.
    if (app != NULL && app->mAppApartment != NULL) {
        // If we have an app thread, do the call through that so it is
        // correctly ordered with other one-way calls.
        app->mAppApartment->ScheduleRegisteredReceiver(receiver, intent,
                resultCode, data, extras, ordered, sticky);
    }
    else {
        receiver->PerformReceive(intent, resultCode, data, extras, ordered, sticky);
    }

    return NOERROR;
}

ECode CActivityManagerService::DeliverToRegisteredReceiverLocked(
    /* [in] */ BroadcastRecord* r,
    /* [in] */ BroadcastFilter* filter,
    /* [in] */ Boolean ordered)
{
    Boolean skip = FALSE;
    if (!filter->mRequiredPermission.IsNull()) {
        Int32 perm = CheckComponentPermission(filter->mRequiredPermission,
                r->mCallingPid, r->mCallingUid, -1);
        if (perm != CapsuleManager_PERMISSION_GRANTED) {
            String intentDes, bfDes;
            r->mIntent->GetDescription(&intentDes);
            filter->GetDescription(&bfDes);
            Slogger::W(TAG, StringBuffer("Permission Denial: broadcasting ")
                    + intentDes
                    + " from " + r->mCallerCapsule + " (pid="
                    + r->mCallingPid + ", uid=" + r->mCallingUid + ")"
                    + " requires " + filter->mRequiredPermission
                    + " due to registered receiver " + bfDes);
            skip = TRUE;
        }
    }
    if (!r->mRequiredPermission.IsNull()) {
        Int32 perm = CheckComponentPermission(r->mRequiredPermission,
                filter->mReceiverList->mPid, filter->mReceiverList->mUid, -1);
        if (perm != CapsuleManager_PERMISSION_GRANTED) {
            String intentDes, appDes;
            r->mIntent->GetDescription(&intentDes);
            filter->mReceiverList->mApp->GetDescription(&appDes);
            Slogger::W(TAG, StringBuffer("Permission Denial: receiving ")
                    + intentDes
                    + " to " + appDes
                    + " (pid=" + filter->mReceiverList->mPid
                    + ", uid=" + filter->mReceiverList->mUid + ")"
                    + " requires " + r->mRequiredPermission
                    + " due to sender " + r->mCallerCapsule
                    + " (uid " + r->mCallingUid + ")");
            skip = TRUE;
        }
    }

    if (!skip) {
        // If this is not being sent as an ordered broadcast, then we
        // don't want to touch the fields that keep track of the current
        // state of ordered broadcasts.
        if (ordered) {
            r->mReceiver = (IIntentReceiver*)filter->mReceiverList->mReceiver;
            r->mCurFilter = filter;
            filter->mReceiverList->mCurBroadcast = r;
            r->mState = BroadcastRecord::CALL_IN_RECEIVE;
            if (filter->mReceiverList->mApp != NULL) {
                // Bump hosting application to no longer be in background
                // scheduling class.  Note that we can't do that if there
                // isn't an app...  but we can only be in that case for
                // things that directly call the IActivityManager API, which
                // are already core system stuff so don't matter for this.
                r->mCurApp = filter->mReceiverList->mApp;
                filter->mReceiverList->mApp->mCurReceiver = r;
                UpdateOomAdjLocked();
            }
        }
        if (DEBUG_BROADCAST_LIGHT) {
            Int32 seq = -1;
            r->mIntent->GetInt32Extra(String("seq"), -1, &seq);
            String bfDes, brDes;
            filter->GetDescription(&bfDes);
            r->GetDescription(&brDes);
            Slogger::I(TAG, StringBuffer("Delivering to ") + bfDes
                    + " (seq=" + seq + "): " + brDes);
        }
        AutoPtr<IIntent> newIntent;
        CIntent::New(r->mIntent, (IIntent**)&newIntent);
        if (SUCCEEDED(PerformReceiveLocked(filter->mReceiverList->mApp, filter->mReceiverList->mReceiver,
                newIntent, r->mResultCode, r->mResultData, r->mResultExtras,
                r->mOrdered, r->mInitialSticky))) {
            if (ordered) {
                r->mState = BroadcastRecord::CALL_DONE_RECEIVE;
            }
        }
        else {
            String intentDes;
            r->mIntent->GetDescription(&intentDes);
            Slogger::W(TAG, StringBuffer("Failure sending broadcast ") + intentDes);
            if (ordered) {
                r->mReceiver = NULL;
                r->mCurFilter = NULL;
                filter->mReceiverList->mCurBroadcast = NULL;
                if (filter->mReceiverList->mApp != NULL) {
                    filter->mReceiverList->mApp->mCurReceiver = NULL;
                }
            }
        }
    }

    return NOERROR;
}

void CActivityManagerService::AddBroadcastToHistoryLocked(
    /* [in] */ BroadcastRecord* r)
{
    if (r->mCallingUid < 0) {
        // This was from a registerReceiver() call; ignore it.
        return;
    }
    for (Int32 i = MAX_BROADCAST_HISTORY - 1; i >= 1; --i) {
        mBroadcastHistory[i] = mBroadcastHistory[i - 1];
    }
    r->mFinishTime = SystemClock::GetUptimeMillis();
    mBroadcastHistory[0] = r;
}

ECode CActivityManagerService::ProcessNextBroadcast(
    /* [in] */ Boolean fromMsg)
{
    Mutex::Autolock lock(_m_syncLock);

    BroadcastRecord* r;

    if (DEBUG_BROADCAST) {
        Slogger::V(TAG, StringBuffer("processNextBroadcast: ")
                    + (Int32)mParallelBroadcasts.GetSize() + " broadcasts, "
                    + (Int32)mOrderedBroadcasts.GetSize() + " ordered broadcasts");
    }

    UpdateCpuStats();

    if (fromMsg) {
        mBroadcastsScheduled = FALSE;
    }

    // First, deliver any non-serialized broadcasts right away.
    while (mParallelBroadcasts.GetSize() > 0) {
        List<BroadcastRecord*>::Iterator it = mParallelBroadcasts.Begin();
        r = *it;
        r->mDispatchTime = SystemClock::GetUptimeMillis();
        mParallelBroadcasts.Erase(it);
        Int32 size = r->mReceivers->GetSize();
        UNUSED(size);
        if (DEBUG_BROADCAST_LIGHT) {
            String brDes;
            r->GetDescription(&brDes);
            Slogger::V(TAG, StringBuffer("Processing parallel broadcast ")
                    + brDes);
        }
        List<AutoPtr<IObject> >::Iterator it2 = r->mReceivers->Begin();
        for (; it2 != r->mReceivers->End(); ++it2) {
            BroadcastFilter* f = (BroadcastFilter*)(IObject*)(*it2);
            if (DEBUG_BROADCAST) {
                String bfDes, brDes;
                f->GetDescription(&bfDes);
                r->GetDescription(&brDes);
                Slogger::V(TAG, StringBuffer("Delivering non-serialized to registered ")
                    + bfDes + ": " + brDes);
            }
            DeliverToRegisteredReceiverLocked(r, f, FALSE);
        }
        AddBroadcastToHistoryLocked(r);
        if (DEBUG_BROADCAST_LIGHT) {
            String brDes;
            r->GetDescription(&brDes);
            Slogger::V(TAG, StringBuffer("Done with parallel broadcast ")
                + brDes);
        }
    }

    // Now take care of the next serialized one...

    // If we are waiting for a process to come up to handle the next
    // broadcast, then do nothing at this point.  Just in case, we
    // check that the process we're waiting for still exists.
    if (mPendingBroadcast != NULL) {
        if (DEBUG_BROADCAST_LIGHT) {
            String appDes;
            mPendingBroadcast->mCurApp->GetDescription(&appDes);
            Slogger::V(TAG, StringBuffer("processNextBroadcast: waiting for ")
                    + appDes);
        }

        Boolean isDead =
               (mPidsSelfLocked[mPendingBroadcast->mCurApp->mPid] == NULL);

        if (!isDead) {
            // It's still alive, so keep waiting
            return NOERROR;
        } else {
            String appDes;
            mPendingBroadcast->mCurApp->GetDescription(&appDes);
            Slogger::W(TAG, StringBuffer("pending app ")
                    + appDes
                    + " died before responding to broadcast");
            mPendingBroadcast->mState = BroadcastRecord::IDLE;
            mPendingBroadcast->mNextReceiver = mPendingBroadcastRecvIndex;
            mPendingBroadcast = NULL;
        }
    }

    Boolean looped = FALSE;

    do {
        if (mOrderedBroadcasts.GetSize() == 0) {
            // No more broadcasts pending, so all done!
            ScheduleAppGcsLocked();
            if (looped) {
                // If we had finished the last ordered broadcast, then
                // make sure all processes have correct oom and sched
                // adjustments.
                UpdateOomAdjLocked();
            }
            return NOERROR;
        }
        r = *(mOrderedBroadcasts.Begin());
        Boolean forceReceive = FALSE;

        // Ensure that even if something goes awry with the timeout
        // detection, we catch "hung" broadcasts here, discard them,
        // and continue to make progress.
        //
        // This is only done if the system is ready so that PRE_BOOT_COMPLETED
        // receivers don't get executed with with timeouts. They're intended for
        // one time heavy lifting after system upgrades and can take
        // significant amounts of time.
        Int32 numReceivers = r->mReceivers->GetSize();
        if (mProcessesReady && r->mDispatchTime > 0) {
            Millisecond64 now = SystemClock::GetUptimeMillis();
            if ((numReceivers > 0) &&
                    (now > r->mDispatchTime + (2 * BROADCAST_TIMEOUT * numReceivers))) {
                String intentDes;
                r->mIntent->GetDescription(&intentDes);
                Slogger::W(TAG, StringBuffer("Hung broadcast discarded after timeout failure:")
                        + " now=" + now
                        + " dispatchTime=" + r->mDispatchTime
                        + " startTime=" + r->mReceiverTime
                        + " intent=" + intentDes
                        + " numReceivers=" + numReceivers
//                        + " nextReceiver=" + r->mNextReceiver
                        + " state=" + (Int32)r->mState);
                BroadcastTimeoutLocked(FALSE); // forcibly finish this broadcast
                forceReceive = TRUE;
                r->mState = BroadcastRecord::IDLE;
            }
        }

        if (r->mState != BroadcastRecord::IDLE) {
            if (DEBUG_BROADCAST) {
                Slogger::D(TAG,
                    StringBuffer("processNextBroadcast() called when not idle (state=")
                    + r->mState + ")");
            }
            return NOERROR;
        }

        if (r->mReceivers == NULL || r->mNextReceiver == r->mReceivers->End()/*r.nextReceiver >= numReceivers*/
                || r->mResultAbort || forceReceive) {
            // No more receivers for this broadcast!  Send the final
            // result if requested...
            if (r->mResultTo != NULL) {
                if (DEBUG_BROADCAST) {
                    Int32 seq = -1;
                    r->mIntent->GetInt32Extra(String("seq"), -1, &seq);
                    String action, appDes;
                    r->mIntent->GetAction(&action);
                    r->mCallerApp->GetDescription(&appDes);
                    Slogger::I(TAG, StringBuffer("Finishing broadcast ") + action
                            + " seq=" + seq + " app=" + appDes);
                }
                AutoPtr<IIntent> intent;
                CIntent::New(r->mIntent, (IIntent**)&intent);
                if (SUCCEEDED(PerformReceiveLocked(r->mCallerApp, r->mResultTo, intent,
                        r->mResultCode, r->mResultData, r->mResultExtras, FALSE, FALSE))) {
                    // Set this to null so that the reference
                    // (local and remote) isnt kept in the mBroadcastHistory.
                    r->mResultTo = NULL;
                }
                else {
                    String intentDes;
                    r->mIntent->GetDescription(&intentDes);
                    Slogger::W(TAG, StringBuffer("Failure sending broadcast result of ") +intentDes);
                }
            }

            if (DEBUG_BROADCAST) {
                Slogger::V(TAG, "Cancelling BROADCAST_TIMEOUT_MSG");
            }
            CancelBroadcastTimeoutLocked();

            if (DEBUG_BROADCAST_LIGHT) {
                String brDes;
                r->GetDescription(&brDes);
                Slogger::V(TAG, StringBuffer("Finished with ordered broadcast ")
                        + brDes);
            }

            // ... and on to the next...
            AddBroadcastToHistoryLocked(r);
            mOrderedBroadcasts.PopFront();
            r = NULL;
            looped = TRUE;
            continue;
        }
    } while (r == NULL);

    // Get the next receiver...
    List<AutoPtr<IObject> >::Iterator recIt = r->mNextReceiver++;

    // Keep track of when this receiver started, and make sure there
    // is a timeout message pending to kill it if need be.
    r->mReceiverTime = SystemClock::GetUptimeMillis();
//    if (recIdx == 0) {
//        r.dispatchTime = r.receiverTime;
//
//        if (DEBUG_BROADCAST_LIGHT) Slog.v(TAG, "Processing ordered broadcast "
//                + r);
//    }
    if (! mPendingBroadcastTimeoutMessage) {
        Millisecond64 timeoutTime = r->mReceiverTime + BROADCAST_TIMEOUT;
        if (DEBUG_BROADCAST) {
            String brDes;
            r->GetDescription(&brDes);
            Slogger::V(TAG, StringBuffer("Submitting BROADCAST_TIMEOUT_MSG for ")
                    + brDes + " at " + timeoutTime);
        }
        SetBroadcastTimeoutLocked(timeoutTime);
    }

    ClassID clsid;
    AutoPtr<IObject> nextReceiver = *recIt;
    nextReceiver->GetClassID(&clsid);
    if (clsid == ECLSID_BroadcastFilter) {
        // Simple case: this is a registered receiver who gets
        // a direct call.
        BroadcastFilter* filter = (BroadcastFilter*)(IObject*)nextReceiver;
        if (DEBUG_BROADCAST)  {
            String bfDes, brDes;
            filter->GetDescription(&bfDes);
            r->GetDescription(&brDes);
            Slogger::V(TAG, StringBuffer("Delivering ordered to registered ")
                + bfDes + ": " + brDes);
        }
        DeliverToRegisteredReceiverLocked(r, filter, r->mOrdered);
        if (r->mReceiver == NULL || !r->mOrdered) {
            // The receiver has already finished, so schedule to
            // process the next one.
            if (DEBUG_BROADCAST) {
                Slogger::V(TAG, StringBuffer("Quick finishing: ordered=")
                    + r->mOrdered /*+ " receiver=" + r.receiver*/);
            }
            r->mState = BroadcastRecord::IDLE;
            ScheduleBroadcastsLocked();
        }
        return NOERROR;
    }

    // Hard case: need to instantiate the receiver, possibly
    // starting its application process to host it.

    IResolveInfo* info = IResolveInfo::Probe(nextReceiver);

    Boolean skip = FALSE;
    AutoPtr<IActivityInfo> infoAInfo;
    info->GetActivityInfo((IActivityInfo**)&infoAInfo);
    AutoPtr<IApplicationInfo> infoAppInfo;
    infoAInfo->GetApplicationInfo((IApplicationInfo**)&infoAppInfo);
    String infoPerm;
    infoAInfo->GetPermission(&infoPerm);
    Int32 reqUid = -1;
    Boolean infoExported;
    infoAInfo->IsExported(&infoExported);
    if (!infoExported) {
        infoAppInfo->GetUid(&reqUid);
    }
    Int32 perm = CheckComponentPermission(infoPerm,
            r->mCallingPid, r->mCallingUid, reqUid);
    if (perm != CapsuleManager_PERMISSION_GRANTED) {
        String intentDes, infoCName, infoName;
        r->mIntent->GetDescription(&intentDes);
        infoAInfo->GetCapsuleName(&infoCName);
        infoAInfo->GetName(&infoName);
        Slogger::W(TAG, StringBuffer("Permission Denial: broadcasting ")
                + intentDes
                + " from " + r->mCallerCapsule + " (pid=" + r->mCallingPid
                + ", uid=" + r->mCallingUid + ")"
                + " requires " + infoPerm
                + " due to receiver " + infoCName
                + "/" + infoName);
        skip = TRUE;
    }
    if (r->mCallingUid != Process::SYSTEM_UID &&
        !r->mRequiredPermission.IsNull()) {
        String infoCName;
        infoAppInfo->GetCapsuleName(&infoCName);
        Int32 perm;
        if (FAILED(GetCapsuleManager()->CheckPermission(r->mRequiredPermission,
                infoCName, &perm))) {
            perm = CapsuleManager_PERMISSION_DENIED;
        }
        if (perm != CapsuleManager_PERMISSION_GRANTED) {
            String intentDes;
            r->mIntent->GetDescription(&intentDes);
            Slogger::W(TAG, StringBuffer("Permission Denial: receiving ")
                    + intentDes + " to "
                    + infoCName
                    + " requires " + r->mRequiredPermission
                    + " due to sender " + r->mCallerCapsule
                    + " (uid " + r->mCallingUid + ")");
            skip = TRUE;
        }
    }
    if (r->mCurApp != NULL && r->mCurApp->mCrashing) {
        // If the target process is crashing, just skip it.
        if (DEBUG_BROADCAST) {
            String brDes, appDes;
            r->GetDescription(&brDes);
            r->mCurApp->GetDescription(&appDes);
            Slogger::V(TAG, StringBuffer("Skipping deliver ordered ")
                    + brDes + " to " + appDes + ": process crashing");
        }
        skip = TRUE;
    }

    if (skip) {
        if (DEBUG_BROADCAST) {
            String brDes;
            r->GetDescription(&brDes);
            Slogger::V(TAG, StringBuffer("Skipping delivery of ordered ")
                    + brDes + " for whatever reason");
        }
        r->mReceiver = NULL;
        r->mCurFilter = NULL;
        r->mState = BroadcastRecord::IDLE;
        ScheduleBroadcastsLocked();
        return NOERROR;
    }

    r->mState = BroadcastRecord::APP_RECEIVE;
    String targetProcess, infoCName, infoName;
    infoAInfo->GetProcessName(&targetProcess);
    infoAppInfo->GetCapsuleName(&infoCName);
    infoAInfo->GetName(&infoName);
    CComponentName::New(infoCName, infoName, (IComponentName**)&(r->mCurComponent));
    r->mCurReceiver = infoAInfo;

    // Is this receiver's application already running?
    ProcessRecord* app = GetProcessRecordLocked(targetProcess, -1);
    if (app != NULL && app->mAppApartment != NULL) {
        if (SUCCEEDED(ProcessCurBroadcastLocked(r, app))) {
            return NOERROR;
        }
        else {
            String compDes;
            r->mCurComponent->GetDescription(&compDes);
            Slogger::W(TAG, StringBuffer("Exception when sending broadcast to ")
                  + compDes);
        }
        // If a dead object exception was thrown -- fall through to
        // restart the application.
    }

    // Not running -- get it started, to be executed when the app comes up.
    if (DEBUG_BROADCAST) {
        String brDes;
        r->GetDescription(&brDes);
        Slogger::V(TAG, StringBuffer("Need to start app ") + targetProcess
                + " for broadcast " + brDes);
    }
    Int32 flags;
    r->mIntent->GetFlags(&flags);
    if ((r->mCurApp = StartProcessLocked(targetProcess,
            infoAppInfo, TRUE, 0, "activity", NULL,
            (flags & Intent_FLAG_RECEIVER_BOOT_UPGRADE) != 0)) == NULL) {
        // Ah, this recipient is unavailable.  Finish it if necessary,
        // and mark the broadcast record as ready for the next.
        String intentDes, infoCName;
        r->mIntent->GetDescription(&intentDes);
        infoAppInfo->GetCapsuleName(&infoCName);
        Int32 infoUid;
        infoAppInfo->GetUid(&infoUid);
        Slogger::W(TAG, StringBuffer("Unable to launch app ")
                + infoCName + "/" + infoUid + " for broadcast "
                + intentDes + ": process is bad");
        LogBroadcastReceiverDiscardLocked(r);
        FinishReceiverLocked(r->mReceiver, r->mResultCode, r->mResultData,
                r->mResultExtras, r->mResultAbort, TRUE);
        ScheduleBroadcastsLocked();
        r->mState = BroadcastRecord::IDLE;
        return NOERROR;
    }

    mPendingBroadcast = r;
    mPendingBroadcastRecvIndex = recIt;

    return NOERROR;
}

// =========================================================
// INSTRUMENTATION
// =========================================================

ECode CActivityManagerService::StartInstrumentation(
    /* [in] */ IComponentName* className,
    /* [in] */ const String& profileFile,
    /* [in] */ Int32 flags,
    /* [in] */ IBundle* arguments,
    /* [in] */ IInstrumentationWatcher* watcher,
    /* [out] */ Boolean* result)
{
//    // Refuse possible leaked file descriptors
//    if (arguments != null && arguments.hasFileDescriptors()) {
//        throw new IllegalArgumentException("File descriptors passed in Bundle");
//    }
//
//    synchronized(this) {
//        InstrumentationInfo ii = null;
//        ApplicationInfo ai = null;
//        try {
//            ii = mContext.getPackageManager().getInstrumentationInfo(
//                className, STOCK_PM_FLAGS);
//            ai = mContext.getPackageManager().getApplicationInfo(
//                ii.targetPackage, STOCK_PM_FLAGS);
//        } catch (PackageManager.NameNotFoundException e) {
//        }
//        if (ii == null) {
//            reportStartInstrumentationFailure(watcher, className,
//                    "Unable to find instrumentation info for: " + className);
//            return false;
//        }
//        if (ai == null) {
//            reportStartInstrumentationFailure(watcher, className,
//                    "Unable to find instrumentation target package: " + ii.targetPackage);
//            return false;
//        }
//
//        int match = mContext.getPackageManager().checkSignatures(
//                ii.targetPackage, ii.packageName);
//        if (match < 0 && match != PackageManager.SIGNATURE_FIRST_NOT_SIGNED) {
//            String msg = "Permission Denial: starting instrumentation "
//                    + className + " from pid="
//                    + Binder.getCallingPid()
//                    + ", uid=" + Binder.getCallingPid()
//                    + " not allowed because package " + ii.packageName
//                    + " does not have a signature matching the target "
//                    + ii.targetPackage;
//            reportStartInstrumentationFailure(watcher, className, msg);
//            throw new SecurityException(msg);
//        }
//
//        final long origId = Binder.clearCallingIdentity();
//        forceStopPackageLocked(ii.targetPackage, -1, true, false, true);
//        ProcessRecord app = addAppLocked(ai);
//        app.instrumentationClass = className;
//        app.instrumentationInfo = ai;
//        app.instrumentationProfileFile = profileFile;
//        app.instrumentationArguments = arguments;
//        app.instrumentationWatcher = watcher;
//        app.instrumentationResultClass = className;
//        Binder.restoreCallingIdentity(origId);
//    }
//
//    return true;
    return TRUE;
}


/**
 * Report errors that occur while attempting to start Instrumentation.  Always writes the
 * error to the logs, but if somebody is watching, send the report there too.  This enables
 * the "am" command to report errors with more information.
 *
 * @param watcher The IInstrumentationWatcher.  Null if there isn't one.
 * @param cn The component name of the instrumentation.
 * @param report The error report.
 */
void CActivityManagerService::ReportStartInstrumentationFailure(
    /* [in] */ IInstrumentationWatcher* watcher,
    /* [in] */ IComponentName* cn,
    /* [in] */ const String& report)
{
//    Slogger::W(TAG, report);
//    try {
//        if (watcher != null) {
//            Bundle results = new Bundle();
//            results.putString(Instrumentation.REPORT_KEY_IDENTIFIER, "ActivityManagerService");
//            results.putString("Error", report);
//            watcher.instrumentationStatus(cn, -1, results);
//        }
//    } catch (RemoteException e) {
//        Slogger::W(TAG, e);
//    }
}

void CActivityManagerService::FinishInstrumentationLocked(
    /* [in] */ ProcessRecord* app,
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* results)
{
    if (app->mInstrumentationWatcher != NULL) {
        // NOTE:  IInstrumentationWatcher *must* be oneway here
        app->mInstrumentationWatcher->InstrumentationFinished(
                app->mInstrumentationClass,
                resultCode,
                results);
    }
    app->mInstrumentationWatcher = NULL;
    app->mInstrumentationClass = NULL;
    app->mInstrumentationInfo = NULL;
    app->mInstrumentationProfileFile = NULL;
    app->mInstrumentationArguments = NULL;

    ForceStopCapsuleLocked(app->mProcessName, -1, FALSE, FALSE, TRUE);
}

ECode CActivityManagerService::FinishInstrumentation(
    /* [in] */ IApplicationApartment* target,
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* results)
{
//    // Refuse possible leaked file descriptors
//    if (results != null && results.hasFileDescriptors()) {
//        throw new IllegalArgumentException("File descriptors passed in Intent");
//    }
//
//    synchronized(this) {
//        ProcessRecord app = getRecordForAppLocked(target);
//        if (app == null) {
//            Slogger::W(TAG, "finishInstrumentation: no app for " + target);
//            return;
//        }
//        final long origId = Binder.clearCallingIdentity();
//        finishInstrumentationLocked(app, resultCode, results);
//        Binder.restoreCallingIdentity(origId);
//    }
    return E_NOT_IMPLEMENTED;
}

// =========================================================
// CONFIGURATION
// =========================================================

ECode CActivityManagerService::GetDeviceConfigurationInfo(
    /* [out] */ IConfigurationInfo** config)
{
//    ConfigurationInfo config = new ConfigurationInfo();
//    synchronized (this) {
//        config.reqTouchScreen = mConfiguration.touchscreen;
//        config.reqKeyboardType = mConfiguration.keyboard;
//        config.reqNavigation = mConfiguration.navigation;
//        if (mConfiguration.navigation == Configuration.NAVIGATION_DPAD
//                || mConfiguration.navigation == Configuration.NAVIGATION_TRACKBALL) {
//            config.reqInputFeatures |= ConfigurationInfo.INPUT_FEATURE_FIVE_WAY_NAV;
//        }
//        if (mConfiguration.keyboard != Configuration.KEYBOARD_UNDEFINED
//                && mConfiguration.keyboard != Configuration.KEYBOARD_NOKEYS) {
//            config.reqInputFeatures |= ConfigurationInfo.INPUT_FEATURE_HARD_KEYBOARD;
//        }
//        config.reqGlEsVersion = GL_ES_VERSION;
//    }
//    return config;
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::GetConfiguration(
    /* [out] */ IConfiguration** values)
{
//    Configuration ci;
//    synchronized(this) {
//        ci = new Configuration(mConfiguration);
//    }
//    return ci;
    return E_NOT_IMPLEMENTED;
}

ECode CActivityManagerService::UpdateConfiguration(
    /* [in] */ IConfiguration* values)
{
    EnforceCallingPermission("android.permission.CHANGE_CONFIGURATION", /*android.Manifest.permission.CHANGE_CONFIGURATION,*/
            "UpdateConfiguration()");
//
//    synchronized(this) {
//        if (values == null && mWindowManager != null) {
//            // sentinel: fetch the current configuration from the window manager
//            values = mWindowManager.computeNewConfiguration();
//        }
//
//        final long origId = Binder.clearCallingIdentity();
//        updateConfigurationLocked(values, null);
//        Binder.restoreCallingIdentity(origId);
//    }
    return E_NOT_IMPLEMENTED;
}

/**
 * Do either or both things: (1) change the current configuration, and (2)
 * make sure the given activity is running with the (now) current
 * configuration.  Returns true if the activity has been left running, or
 * false if <var>starting</var> is being destroyed to match the new
 * configuration.
 */
Boolean CActivityManagerService::UpdateConfigurationLocked(
    /* [in] */ IConfiguration* values,
    /* [in] */ CActivityRecord* starting)
{
    Int32 changes = 0;

    Boolean kept = TRUE;

    if (values != NULL) {
        AutoPtr<IConfiguration> newConfig;
        CConfiguration::New(mConfiguration, (IConfiguration**)&newConfig);
        newConfig->UpdateFrom(values, &changes);
        if (FALSE/*changes != 0*/) {
            if (DEBUG_SWITCH || DEBUG_CONFIGURATION) {
                String confDes;
                values->GetDescription(&confDes);
                Slogger::I(TAG, StringBuffer("Updating configuration to: ") + confDes);
            }

//            EventLog.writeEvent(EventLogTags.CONFIGURATION_CHANGED, changes);

//            if (values.locale != null) {
//                saveLocaleLocked(values.locale,
//                                 !values.locale.equals(mConfiguration.locale),
//                                 values.userSetLocale);
//            }

            mConfigurationSeq++;
            if (mConfigurationSeq <= 0) {
                mConfigurationSeq = 1;
            }
            newConfig->SetSeq(mConfigurationSeq);
            mConfiguration = newConfig;
            String confDes;
            newConfig->GetDescription(&confDes);
            Slogger::I(TAG, StringBuffer("Config changed: ") + confDes);

            AttributeCache* ac = AttributeCache::GetInstance();
            if (ac != NULL) {
                ac->UpdateConfiguration(mConfiguration);
            }

//            if (Settings.System.hasInterestingConfigurationChanges(changes)) {
//                Message msg = mHandler.obtainMessage(UPDATE_CONFIGURATION_MSG);
//                msg.obj = new Configuration(mConfiguration);
//                mHandler.sendMessage(msg);
//            }

            List<ProcessRecord*>::ReverseIterator rit = mLRUProcesses.RBegin();
            for (; rit != mLRUProcesses.REnd(); ++rit) {
                ProcessRecord* app = *rit;
                if (app->mAppApartment != NULL) {
                    if (DEBUG_CONFIGURATION) {
                        String confDes;
                        mConfiguration->GetDescription(&confDes);
                        Slogger::V(TAG, StringBuffer("Sending to proc ")
                                + app->mProcessName + " new config " + confDes);
                    }
                    app->mAppApartment->ScheduleConfigurationChanged(mConfiguration);
                }
            }
            AutoPtr<IIntent> intent;
            CIntent::New(String(Intent_ACTION_CONFIGURATION_CHANGED), (IIntent**)&intent);
            intent->AddFlags(Intent_FLAG_RECEIVER_REGISTERED_ONLY
                    | Intent_FLAG_RECEIVER_REPLACE_PENDING);
            Int32 result;
            BroadcastIntentLocked(NULL, String(NULL), intent, String(NULL), NULL, 0,
                    String(NULL), NULL, String(NULL), FALSE, FALSE, mMyPid, Process::SYSTEM_UID, &result);
            if ((changes & ActivityInfo_CONFIG_LOCALE) != 0) {
                AutoPtr<IIntent> newIntent;
                CIntent::New(String(Intent_ACTION_LOCALE_CHANGED), (IIntent**)&intent);
                BroadcastIntentLocked(NULL, String(NULL),
                        newIntent, String(NULL), NULL, 0, String(NULL), NULL,
                        String(NULL), FALSE, FALSE, mMyPid, Process::SYSTEM_UID, &result);
            }
        }
    }

    if (changes != 0 && starting == NULL) {
        // If the configuration changed, and the caller is not already
        // in the process of starting an activity, then find the top
        // activity to check if its configuration needs to change.
        starting = mMainStack->GetTopRunningActivityLocked(NULL);
    }

    if (starting != NULL) {
        kept = mMainStack->EnsureActivityConfigurationLocked(starting, changes);
        if (kept) {
            // If this didn't result in the starting activity being
            // destroyed, then we need to make sure at this point that all
            // other activities are made visible.
            if (DEBUG_SWITCH) {
                String acDes;
                starting->GetDescription(&acDes);
                Slogger::I(TAG, StringBuffer("Config didn't destroy ") + acDes
                    + ", ensuring others are correct.");
            }
            mMainStack->EnsureActivitiesVisibleLocked(starting, changes);
        }
    }

    if (values != NULL && mWindowManager != NULL) {
        mWindowManager->SetNewConfiguration(mConfiguration);
    }

    return kept;
}

// =========================================================
// LIFETIME MANAGEMENT
// =========================================================

Int32 CActivityManagerService::ComputeOomAdjLocked(
    /* [in] */ ProcessRecord* app,
    /* [in] */ Int32 hiddenAdj,
    /* [in] */ ProcessRecord* TOP_APP,
    /* [in] */ Boolean recursed)
{
    if (mAdjSeq == app->mAdjSeq) {
        // This adjustment has already been computed.  If we are calling
        // from the top, we may have already computed our adjustment with
        // an earlier hidden adjustment that isn't really for us... if
        // so, use the new hidden adjustment.
        if (!recursed && app->mHidden) {
            app->mCurAdj = hiddenAdj;
        }
        return app->mCurAdj;
    }

    if (app->mAppApartment == NULL) {
        app->mAdjSeq = mAdjSeq;
        app->mCurSchedGroup = Process::THREAD_GROUP_BG_NONINTERACTIVE;
        app->mCurAdj = EMPTY_APP_ADJ;
        return app->mCurAdj;
    }

    if (app->mMaxAdj <= FOREGROUND_APP_ADJ) {
        // The max adjustment doesn't allow this app to be anything
        // below foreground, so it is not worth doing work for it.
        app->mAdjType = "fixed";
        app->mAdjSeq = mAdjSeq;
        app->mCurRawAdj = app->mMaxAdj;
        app->mKeeping = TRUE;
        app->mCurSchedGroup = Process::THREAD_GROUP_DEFAULT;
        app->mCurAdj = app->mMaxAdj;
        return app->mCurAdj;
    }

    app->mAdjTypeCode = RunningAppProcessInfo_REASON_UNKNOWN;
    app->mAdjSource = NULL;
    app->mAdjTarget = NULL;
    app->mKeeping = FALSE;
    app->mEmpty = FALSE;
    app->mHidden = FALSE;

    // Determine the importance of the process, starting with most
    // important to least, and assign an appropriate OOM adjustment.
    Int32 adj;
    Int32 schedGroup;
    Int32 N;
    UNUSED(N);
    if (app == TOP_APP) {
        // The last app on the list is the foreground app.
        adj = FOREGROUND_APP_ADJ;
        schedGroup = Process::THREAD_GROUP_DEFAULT;
        app->mAdjType = "top-activity";
    } else if (app->mInstrumentationClass != NULL) {
        // Don't want to kill running instrumentation.
        adj = FOREGROUND_APP_ADJ;
        schedGroup = Process::THREAD_GROUP_DEFAULT;
        app->mAdjType = "instrumentation";
    } else if (app->mCurReceiver != NULL ||
            (mPendingBroadcast != NULL && mPendingBroadcast->mCurApp == app)) {
        // An app that is currently receiving a broadcast also
        // counts as being in the foreground.
        adj = FOREGROUND_APP_ADJ;
        schedGroup = Process::THREAD_GROUP_DEFAULT;
        app->mAdjType = "broadcast";
    } else if (app->mExecutingServices.GetSize() > 0) {
        // An app that is currently executing a service callback also
        // counts as being in the foreground.
        adj = FOREGROUND_APP_ADJ;
        schedGroup = Process::THREAD_GROUP_DEFAULT;
        app->mAdjType = "exec-service";
    } else if (app->mForegroundServices) {
        // The user is aware of this app, so make it visible.
        adj = PERCEPTIBLE_APP_ADJ;
        schedGroup = Process::THREAD_GROUP_DEFAULT;
        app->mAdjType = "foreground-service";
    } else if (app->mForcingToForeground != NULL) {
        // The user is aware of this app, so make it visible.
        adj = PERCEPTIBLE_APP_ADJ;
        schedGroup = Process::THREAD_GROUP_DEFAULT;
        app->mAdjType = "force-foreground";
        app->mAdjSource = (IObject*)app->mForcingToForeground->Probe(EIID_IObject);
    } else if (app == mHeavyWeightProcess) {
        // We don't want to kill the current heavy-weight process.
        adj = HEAVY_WEIGHT_APP_ADJ;
        schedGroup = Process::THREAD_GROUP_DEFAULT;
        app->mAdjType = "heavy";
    } else if (app == mHomeProcess) {
        // This process is hosting what we currently consider to be the
        // home app, so we don't want to let it go into the background.
        adj = HOME_APP_ADJ;
        schedGroup = Process::THREAD_GROUP_BG_NONINTERACTIVE;
        app->mAdjType = "home";
    } else if (app->mActivities.GetSize() != 0) {
        // This app is in the background with paused activities.
        app->mHidden = TRUE;
        adj = hiddenAdj;
        schedGroup = Process::THREAD_GROUP_BG_NONINTERACTIVE;
        app->mAdjType = "bg-activities";
        List<AutoPtr<CActivityRecord> >::Iterator it = app->mActivities.Begin();
        for (; it != app->mActivities.End(); ++it) {
            if ((*it)->mVisible) {
                // This app has a visible activity!
                app->mHidden = FALSE;
                adj = VISIBLE_APP_ADJ;
                schedGroup = Process::THREAD_GROUP_DEFAULT;
                app->mAdjType = "visible";
                break;
            }
        }
    } else {
        // A very not-needed process.  If this is lower in the lru list,
        // we will push it in to the empty bucket.
        app->mHidden = TRUE;
        app->mEmpty = TRUE;
        schedGroup = Process::THREAD_GROUP_BG_NONINTERACTIVE;
        adj = hiddenAdj;
        app->mAdjType = "bg-empty";
    }

    //Slog.i(TAG, "OOM " + app + ": initial adj=" + adj);

    // By default, we use the computed adjustment.  It may be changed if
    // there are applications dependent on our services or providers, but
    // this gives us a baseline and makes sure we don't get into an
    // infinite recursion.
    app->mAdjSeq = mAdjSeq;
    app->mCurRawAdj = adj;

    if (mBackupTarget != NULL && app == mBackupTarget->mApp) {
        // If possible we want to avoid killing apps while they're being backed up
        if (adj > BACKUP_APP_ADJ) {
            if (DEBUG_BACKUP) {
                String appDes;
                app->GetDescription(&appDes);
                Slogger::V(TAG, "oom BACKUP_APP_ADJ for " + appDes);
            }
            adj = BACKUP_APP_ADJ;
            app->mAdjType = "backup";
            app->mHidden = FALSE;
        }
    }

    if (app->mServices.GetSize() != 0 && (adj > FOREGROUND_APP_ADJ
            || schedGroup == Process::THREAD_GROUP_BG_NONINTERACTIVE)) {
        Millisecond64 now = SystemClock::GetUptimeMillis();
        // This process is more important if the top activity is
        // bound to the service.
        Set<AutoPtr<CServiceRecord> >::Iterator it = app->mServices.Begin();
        for (; it != app->mServices.End() && adj > FOREGROUND_APP_ADJ; ++it) {
            AutoPtr<CServiceRecord> s = *it;
            if (s->mStartRequested) {
                if (now < (s->mLastActivity + MAX_SERVICE_INACTIVITY)) {
                    // This service has seen some activity within
                    // recent memory, so we will keep its process ahead
                    // of the background processes.
                    if (adj > SECONDARY_SERVER_ADJ) {
                        adj = SECONDARY_SERVER_ADJ;
                        app->mAdjType = "started-services";
                        app->mHidden = FALSE;
                    }
                }
                // If we have let the service slide into the background
                // state, still have some text describing what it is doing
                // even though the service no longer has an impact.
                if (adj > SECONDARY_SERVER_ADJ) {
                    app->mAdjType = "started-bg-services";
                }
                // Don't kill this process because it is doing work; it
                // has said it is doing work.
                app->mKeeping = TRUE;
            }
            if (s->mConnections.GetSize() > 0 && (adj > FOREGROUND_APP_ADJ
                    || schedGroup == Process::THREAD_GROUP_BG_NONINTERACTIVE)) {
                Map<AutoPtr<IServiceConnectionInner>, List<ConnectionRecord*>*>:: Iterator kt = \
                        s->mConnections.Begin();
                for (; kt != s->mConnections.End() && adj > FOREGROUND_APP_ADJ; ++kt) {
                    List<ConnectionRecord*>* clist = kt->mSecond;
                    List<ConnectionRecord*>::Iterator lt = clist->Begin();
                    for (; lt != clist->End() && adj > FOREGROUND_APP_ADJ; ++lt) {
                        // XXX should compute this based on the max of
                        // all connected clients.
                        ConnectionRecord* cr = *lt;
                        if (cr->mBinding->mClient == app) {
                            // Binding to ourself is not interesting.
                            continue;
                        }
                        if ((cr->mFlags & Context_BIND_AUTO_CREATE) != 0) {
                            ProcessRecord* client = cr->mBinding->mClient;
                            Int32 myHiddenAdj = hiddenAdj;
                            if (myHiddenAdj > client->mHiddenAdj) {
                                if (client->mHiddenAdj >= VISIBLE_APP_ADJ) {
                                    myHiddenAdj = client->mHiddenAdj;
                                } else {
                                    myHiddenAdj = VISIBLE_APP_ADJ;
                                }
                            }
                            Int32 clientAdj = ComputeOomAdjLocked(
                                client, myHiddenAdj, TOP_APP, TRUE);
                            if (adj > clientAdj) {
                                adj = clientAdj >= VISIBLE_APP_ADJ
                                        ? clientAdj : VISIBLE_APP_ADJ;
                                if (!client->mHiddenAdj) {
                                    app->mHidden = FALSE;
                                }
                                if (client->mKeeping) {
                                    app->mKeeping = TRUE;
                                }
                                app->mAdjType = "service";
                                app->mAdjTypeCode = RunningAppProcessInfo_REASON_SERVICE_IN_USE;
//                                app->mAdjSource = cr->mBinding->mClient;
                                app->mAdjTarget = (IObject*)s->mName->Probe(EIID_IObject);
                            }
                            if ((cr->mFlags & Context_BIND_NOT_FOREGROUND) == 0) {
                                if (client->mCurSchedGroup == Process::THREAD_GROUP_DEFAULT) {
                                    schedGroup = Process::THREAD_GROUP_DEFAULT;
                                }
                            }
                        }
                        AutoPtr<CActivityRecord> a = cr->mActivity;
                        //if (a != NULL) {
                        //    Slog.i(TAG, "Connection to " + a ": state=" + a.state);
                        //}
                        if (a != NULL && adj > FOREGROUND_APP_ADJ &&
                                (a->mState == ActivityState_RESUMED
                                 || a->mState == ActivityState_PAUSING)) {
                            adj = FOREGROUND_APP_ADJ;
                            schedGroup = Process::THREAD_GROUP_DEFAULT;
                            app->mHidden = FALSE;
                            app->mAdjType = "service";
                            app->mAdjTypeCode = RunningAppProcessInfo_REASON_SERVICE_IN_USE;
                            app->mAdjSource = (IObject*)a->Probe(EIID_IObject);
                            app->mAdjTarget = (IObject*)s->mName->Probe(EIID_IObject);
                        }
                    }
                }
            }
        }

        // Finally, if this process has active services running in it, we
        // would like to avoid killing it unless it would prevent the current
        // application from running.  By default we put the process in
        // with the rest of the background processes; as we scan through
        // its services we may bump it up from there.
        if (adj > hiddenAdj) {
            adj = hiddenAdj;
            app->mHidden = FALSE;
            app->mAdjType = "bg-services";
        }
    }

    if (app->mPubProviders.GetSize() != 0 && (adj > FOREGROUND_APP_ADJ
            || schedGroup == Process::THREAD_GROUP_BG_NONINTERACTIVE)) {
        HashMap<String, ContentProviderRecord*>::Iterator jt = \
                app->mPubProviders.Begin();
        for (; jt != app->mPubProviders.End() && (adj > FOREGROUND_APP_ADJ
                || schedGroup == Process::THREAD_GROUP_BG_NONINTERACTIVE); ++jt) {
            ContentProviderRecord* cpr = jt->mSecond;
            if (cpr->mClients.GetSize() != 0) {
                Set<ProcessRecord*>::Iterator kt = cpr->mClients.Begin();
                for (; kt != cpr->mClients.End() && adj > FOREGROUND_APP_ADJ; ++kt) {
                    ProcessRecord* client = *kt;
                    if (client == app) {
                        // Being our own client is not interesting.
                        continue;
                    }
                    Int32 myHiddenAdj = hiddenAdj;
                    if (myHiddenAdj > client->mHiddenAdj) {
                        if (client->mHiddenAdj > FOREGROUND_APP_ADJ) {
                            myHiddenAdj = client->mHiddenAdj;
                        } else {
                            myHiddenAdj = FOREGROUND_APP_ADJ;
                        }
                    }
                    Int32 clientAdj = ComputeOomAdjLocked(
                        client, myHiddenAdj, TOP_APP, TRUE);
                    if (adj > clientAdj) {
                        adj = clientAdj > FOREGROUND_APP_ADJ
                                ? clientAdj : FOREGROUND_APP_ADJ;
                        if (!client->mHiddenAdj) {
                            app->mHidden = FALSE;
                        }
                        if (client->mKeeping) {
                            app->mKeeping = TRUE;
                        }
                        app->mAdjType = "provider";
                        app->mAdjTypeCode = RunningAppProcessInfo_REASON_PROVIDER_IN_USE;
//                        app->mAdjSource = client;
                        app->mAdjTarget = (IObject*)cpr->mName->Probe(EIID_IObject);
                    }
                    if (client->mCurSchedGroup == Process::THREAD_GROUP_DEFAULT) {
                        schedGroup = Process::THREAD_GROUP_DEFAULT;
                    }
                }
            }
            // If the provider has external (non-framework) process
            // dependencies, ensure that its adjustment is at least
            // FOREGROUND_APP_ADJ.
            if (cpr->mExternals != 0) {
                if (adj > FOREGROUND_APP_ADJ) {
                    adj = FOREGROUND_APP_ADJ;
                    schedGroup = Process::THREAD_GROUP_DEFAULT;
                    app->mHidden = FALSE;
                    app->mKeeping = TRUE;
                    app->mAdjType = "provider";
                    app->mAdjTarget = (IObject*)cpr->mName->Probe(EIID_IObject);
                }
            }
        }
    }

    app->mCurRawAdj = adj;

    //Slog.i(TAG, "OOM ADJ " + app + ": pid=" + app.pid +
    //      " adj=" + adj + " curAdj=" + app.curAdj + " maxAdj=" + app.maxAdj);
    if (adj > app->mMaxAdj) {
        adj = app->mMaxAdj;
        if (app->mMaxAdj <= PERCEPTIBLE_APP_ADJ) {
            schedGroup = Process::THREAD_GROUP_DEFAULT;
        }
    }
    if (adj < HIDDEN_APP_MIN_ADJ) {
        app->mKeeping = TRUE;
    }

    app->mCurAdj = adj;
    app->mCurSchedGroup = schedGroup;

    return adj;
}

/**
 * Ask a given process to GC right now.
 */
void CActivityManagerService::PerformAppGcLocked(
    /* [in] */ ProcessRecord* app)
{
//    try {
//        app.lastRequestedGc = SystemClock.uptimeMillis();
//        if (app.thread != null) {
//            if (app.reportLowMemory) {
//                app.reportLowMemory = false;
//                app.thread.scheduleLowMemory();
//            } else {
//                app.thread.processInBackground();
//            }
//        }
//    } catch (Exception e) {
//        // whatever.
//    }
}

/**
 * Returns true if things are idle enough to perform GCs.
 */
Boolean CActivityManagerService::CanGcNowLocked()
{
//    return mParallelBroadcasts.size() == 0
//            && mOrderedBroadcasts.size() == 0
//            && (mSleeping || (mMainStack.mResumedActivity != null &&
//            mMainStack.mResumedActivity.idle));
    return TRUE;
}

/**
 * Perform GCs on all processes that are waiting for it, but only
 * if things are idle.
 */
void CActivityManagerService::PerformAppGcsLocked()
{
//    final int N = mProcessesToGc.size();
//    if (N <= 0) {
//        return;
//    }
//    if (canGcNowLocked()) {
//        while (mProcessesToGc.size() > 0) {
//            ProcessRecord proc = mProcessesToGc.remove(0);
//            if (proc.curRawAdj > PERCEPTIBLE_APP_ADJ || proc.reportLowMemory) {
//                if ((proc.lastRequestedGc+GC_MIN_INTERVAL)
//                        <= SystemClock.uptimeMillis()) {
//                    // To avoid spamming the system, we will GC processes one
//                    // at a time, waiting a few seconds between each.
//                    performAppGcLocked(proc);
//                    scheduleAppGcsLocked();
//                    return;
//                } else {
//                    // It hasn't been long enough since we last GCed this
//                    // process...  put it in the list to wait for its time.
//                    addProcessToGcListLocked(proc);
//                    break;
//                }
//            }
//        }
//
//        scheduleAppGcsLocked();
//    }
}

/**
 * If all looks good, perform GCs on all processes waiting for them.
 */
void CActivityManagerService::PerformAppGcsIfAppropriateLocked()
{
//    if (canGcNowLocked()) {
//        performAppGcsLocked();
//        return;
//    }
//    // Still not idle, wait some more.
//    scheduleAppGcsLocked();
}

/**
 * Schedule the execution of all pending app GCs.
 */
void CActivityManagerService::ScheduleAppGcsLocked()
{
//    mHandler.removeMessages(GC_BACKGROUND_PROCESSES_MSG);
//
//    if (mProcessesToGc.size() > 0) {
//        // Schedule a GC for the time to the next process.
//        ProcessRecord proc = mProcessesToGc.get(0);
//        Message msg = mHandler.obtainMessage(GC_BACKGROUND_PROCESSES_MSG);
//
//        long when = mProcessesToGc.get(0).lastRequestedGc + GC_MIN_INTERVAL;
//        long now = SystemClock.uptimeMillis();
//        if (when < (now+GC_TIMEOUT)) {
//            when = now + GC_TIMEOUT;
//        }
//        mHandler.sendMessageAtTime(msg, when);
//    }
}

/**
 * Add a process to the array of processes waiting to be GCed.  Keeps the
 * list in sorted order by the last GC time.  The process can't already be
 * on the list.
 */
void CActivityManagerService::AddProcessToGcListLocked(
    /* [in] */ ProcessRecord* proc)
{
    Boolean added = FALSE;
    List<ProcessRecord*>::ReverseIterator rit = mProcessesToGc.RBegin();
    for (; rit != mProcessesToGc.REnd(); ++rit) {
        if ((*rit)->mLastRequestedGc < proc->mLastRequestedGc) {
            added = TRUE;
            mProcessesToGc.Insert(rit.GetBase(), proc);
            break;
        }
    }
    if (!added) {
        mProcessesToGc.PushFront(proc);
    }
}

/**
 * Set up to ask a process to GC itself.  This will either do it
 * immediately, or put it on the list of processes to gc the next
 * time things are idle.
 */
void CActivityManagerService::ScheduleAppGcLocked(
    /* [in] */ ProcessRecord* app)
{
    Millisecond64 now = SystemClock::GetUptimeMillis();
    if ((app->mLastRequestedGc + GC_MIN_INTERVAL) > now) {
        return;
    }
    if (Find(mProcessesToGc.Begin(), mProcessesToGc.End(), app)
            == mProcessesToGc.End()) {
        AddProcessToGcListLocked(app);
        ScheduleAppGcsLocked();
    }
}

void CActivityManagerService::CheckExcessivePowerUsageLocked(
    /* [in] */ Boolean doKills)
{
//    updateCpuStatsNow();
//
//    BatteryStatsImpl stats = mBatteryStatsService.getActiveStatistics();
//    boolean doWakeKills = doKills;
//    boolean doCpuKills = doKills;
//    if (mLastPowerCheckRealtime == 0) {
//        doWakeKills = false;
//    }
//    if (mLastPowerCheckUptime == 0) {
//        doCpuKills = false;
//    }
//    if (stats.isScreenOn()) {
//        doWakeKills = false;
//    }
//    final long curRealtime = SystemClock.elapsedRealtime();
//    final long realtimeSince = curRealtime - mLastPowerCheckRealtime;
//    final long curUptime = SystemClock.uptimeMillis();
//    final long uptimeSince = curUptime - mLastPowerCheckUptime;
//    mLastPowerCheckRealtime = curRealtime;
//    mLastPowerCheckUptime = curUptime;
//    if (realtimeSince < WAKE_LOCK_MIN_CHECK_DURATION) {
//        doWakeKills = false;
//    }
//    if (uptimeSince < CPU_MIN_CHECK_DURATION) {
//        doCpuKills = false;
//    }
//    int i = mLruProcesses.size();
//    while (i > 0) {
//        i--;
//        ProcessRecord app = mLruProcesses.get(i);
//        if (!app.keeping) {
//            long wtime;
//            synchronized (stats) {
//                wtime = stats.getProcessWakeTime(app.info.uid,
//                        app.pid, curRealtime);
//            }
//            long wtimeUsed = wtime - app.lastWakeTime;
//            long cputimeUsed = app.curCpuTime - app.lastCpuTime;
//            if (DEBUG_POWER) {
//                StringBuilder sb = new StringBuilder(128);
//                sb.append("Wake for ");
//                app.toShortString(sb);
//                sb.append(": over ");
//                TimeUtils.formatDuration(realtimeSince, sb);
//                sb.append(" used ");
//                TimeUtils.formatDuration(wtimeUsed, sb);
//                sb.append(" (");
//                sb.append((wtimeUsed*100)/realtimeSince);
//                sb.append("%)");
//                Slog.i(TAG, sb.toString());
//                sb.setLength(0);
//                sb.append("CPU for ");
//                app.toShortString(sb);
//                sb.append(": over ");
//                TimeUtils.formatDuration(uptimeSince, sb);
//                sb.append(" used ");
//                TimeUtils.formatDuration(cputimeUsed, sb);
//                sb.append(" (");
//                sb.append((cputimeUsed*100)/uptimeSince);
//                sb.append("%)");
//                Slog.i(TAG, sb.toString());
//            }
//            // If a process has held a wake lock for more
//            // than 50% of the time during this period,
//            // that sounds pad.  Kill!
//            if (doWakeKills && realtimeSince > 0
//                    && ((wtimeUsed*100)/realtimeSince) >= 50) {
//                synchronized (stats) {
//                    stats.reportExcessiveWakeLocked(app.info.uid, app.processName,
//                            realtimeSince, wtimeUsed);
//                }
//                Slogger::W(TAG, "Excessive wake lock in " + app.processName
//                        + " (pid " + app.pid + "): held " + wtimeUsed
//                        + " during " + realtimeSince);
//                EventLog.writeEvent(EventLogTags.AM_KILL, app.pid,
//                        app.processName, app.setAdj, "excessive wake lock");
//                Process.killProcessQuiet(app.pid);
//            } else if (doCpuKills && uptimeSince > 0
//                    && ((cputimeUsed*100)/uptimeSince) >= 50) {
//                synchronized (stats) {
//                    stats.reportExcessiveCpuLocked(app.info.uid, app.processName,
//                            uptimeSince, cputimeUsed);
//                }
//                Slogger::W(TAG, "Excessive CPU in " + app.processName
//                        + " (pid " + app.pid + "): used " + cputimeUsed
//                        + " during " + uptimeSince);
//                EventLog.writeEvent(EventLogTags.AM_KILL, app.pid,
//                        app.processName, app.setAdj, "excessive cpu");
//                Process.killProcessQuiet(app.pid);
//            } else {
//                app.lastWakeTime = wtime;
//                app.lastCpuTime = app.curCpuTime;
//            }
//        }
//    }
}

Boolean CActivityManagerService::UpdateOomAdjLocked(
    /* [in] */ ProcessRecord* app,
    /* [in] */ Int32 hiddenAdj,
    /* [in] */ ProcessRecord* TOP_APP)
{
    app->mHiddenAdj = hiddenAdj;

    if (app->mAppApartment == NULL) {
        return TRUE;
    }

    Boolean wasKeeping = app->mKeeping;

    Int32 adj = ComputeOomAdjLocked(app, hiddenAdj, TOP_APP, FALSE);

    if ((app->mPid != 0 && app->mPid != mMyPid) || Process::SupportsProcesses()) {
        if (app->mCurRawAdj != app->mSetRawAdj) {
            if (app->mCurRawAdj > FOREGROUND_APP_ADJ
                    && app->mSetRawAdj <= FOREGROUND_APP_ADJ) {
                // If this app is transitioning from foreground to
                // non-foreground, have it do a gc.
                ScheduleAppGcLocked(app);
            } else if (app->mCurRawAdj >= HIDDEN_APP_MIN_ADJ
                    && app->mSetRawAdj < HIDDEN_APP_MIN_ADJ) {
                // Likewise do a gc when an app is moving in to the
                // background (such as a service stopping).
                ScheduleAppGcLocked(app);
            }

            if (wasKeeping && !app->mKeeping) {
                // This app is no longer something we want to keep.  Note
                // its current wake lock time to later know to kill it if
                // it is not behaving well.
//                AutoPtr<BatteryStatsImpl> stats = mBatteryStatsService->GetActiveStatistics();
//                stats->Lock();
//                app->mLastWakeTime = stats->GetProcessWakeTime(app->mInfo->mUid,
//                            app->mPid, SystemClock::GetElapsedRealtime());
//                stats->Unlock();
                app->mLastCpuTime = app->mCurCpuTime;
            }

            app->mSetRawAdj = app->mCurRawAdj;
        }
        if (adj != app->mSetAdj) {
            if (Process::SetOomAdj(app->mPid, adj)) {
                if (DEBUG_SWITCH || DEBUG_OOM_ADJ) {
                    Slogger::V(TAG, StringBuffer("Set app ") + app->mProcessName +
                            " oom adj to " + adj);
                }
                app->mSetAdj = adj;
            } else {
                return FALSE;
            }
        }
        if (app->mSetSchedGroup != app->mCurSchedGroup) {
            app->mSetSchedGroup = app->mCurSchedGroup;
            if (DEBUG_SWITCH || DEBUG_OOM_ADJ) {
                Slogger::V(TAG, StringBuffer("Setting process group of ")
                        + app->mProcessName + " to " + app->mCurSchedGroup);
            }
            if (true) {
//                long oldId = Binder.clearCallingIdentity();
                if (FAILED(Process::SetProcessGroup(app->mPid, app->mCurSchedGroup))) {
                    Slogger::W(TAG, StringBuffer("Failed setting process group of ")
                            + app->mPid + " to " + app->mCurSchedGroup);
                }
//                Binder.restoreCallingIdentity(oldId);
            }
//            if (false) {
//                if (app.thread != null) {
//                    try {
//                        app.thread.setSchedulingGroup(app.curSchedGroup);
//                    } catch (RemoteException e) {
//                    }
//                }
//            }
        }
    }

    return TRUE;
}

CActivityRecord* CActivityManagerService::ResumedAppLocked()
{
    CActivityRecord* resumedActivity = mMainStack->mResumedActivity;
    if (resumedActivity == NULL || resumedActivity->mApp == NULL) {
        resumedActivity = mMainStack->mPausingActivity;
        if (resumedActivity == NULL || resumedActivity->mApp == NULL) {
            resumedActivity = mMainStack->GetTopRunningActivityLocked(NULL);
        }
    }
    return resumedActivity;
}

Boolean CActivityManagerService::UpdateOomAdjLocked(
    /* [in] */ ProcessRecord* app)
{
    CActivityRecord* TOP_ACT = ResumedAppLocked();
    ProcessRecord* TOP_APP = TOP_ACT != NULL ? TOP_ACT->mApp : NULL;
    Int32 curAdj = app->mCurAdj;
    UNUSED(curAdj);
    Boolean wasHidden = app->mCurAdj >= HIDDEN_APP_MIN_ADJ
            && app->mCurAdj <= HIDDEN_APP_MAX_ADJ;

    mAdjSeq++;

    Boolean res = UpdateOomAdjLocked(app, app->mHiddenAdj, TOP_APP);
    if (res) {
        Boolean nowHidden = app->mCurAdj >= HIDDEN_APP_MIN_ADJ
                && app->mCurAdj <= HIDDEN_APP_MAX_ADJ;
        if (nowHidden != wasHidden) {
            // Changed to/from hidden state, so apps after it in the LRU
            // list may also be changed.
            UpdateOomAdjLocked();
        }
    }
    return res;
}

Boolean CActivityManagerService::UpdateOomAdjLocked()
{
    Boolean didOomAdj = TRUE;
    CActivityRecord* TOP_ACT = ResumedAppLocked();
    ProcessRecord* TOP_APP = TOP_ACT != NULL ? TOP_ACT->mApp : NULL;

//    if (false) {
//        RuntimeException e = new RuntimeException();
//        e.fillInStackTrace();
//        Slog.i(TAG, "updateOomAdj: top=" + TOP_ACT, e);
//    }

    mAdjSeq++;

    // Let's determine how many processes we have running vs.
    // how many slots we have for background processes; we may want
    // to put multiple processes in a slot of there are enough of
    // them.
    Int32 numSlots = HIDDEN_APP_MAX_ADJ - HIDDEN_APP_MIN_ADJ + 1;
    Int32 factor = (mLRUProcesses.GetSize() - 4) / numSlots;
    if (factor < 1) factor = 1;
    Int32 step = 0;
    Int32 numHidden = 0;

    // First try updating the OOM adjustment for each of the
    // application processes based on their current state.
    Int32 i = mLRUProcesses.GetSize();
    UNUSED(i);
    Int32 curHiddenAdj = HIDDEN_APP_MIN_ADJ;
    List<ProcessRecord*>::ReverseIterator rit = mLRUProcesses.RBegin();
    for (; rit != mLRUProcesses.REnd(); ++rit) {
        ProcessRecord* app = *rit;
        //Slog.i(TAG, "OOM " + app + ": cur hidden=" + curHiddenAdj);
        if (UpdateOomAdjLocked(app, curHiddenAdj, TOP_APP)) {
            if (curHiddenAdj < EMPTY_APP_ADJ
                && app->mCurAdj == curHiddenAdj) {
                step++;
                if (step >= factor) {
                    step = 0;
                    curHiddenAdj++;
                }
            }
            if (app->mCurAdj >= HIDDEN_APP_MIN_ADJ) {
                if (!app->mKilledBackground) {
                    numHidden++;
                    if (numHidden > MAX_HIDDEN_APPS) {
                        Slogger::I(TAG, "No longer want " + app->mProcessName
                                + " (pid " + app->mPid + "): hidden #" + numHidden);
//                        EventLog.writeEvent(EventLogTags.AM_KILL, app.pid,
//                                app.processName, app.setAdj, "too many background");
                        app->mKilledBackground = TRUE;
                        Process::KillProcessQuiet(app->mPid);
                    }
                }
            }
        } else {
            didOomAdj = FALSE;
        }
    }

    // If we return false, we will fall back on killing processes to
    // have a fixed limit.  Do this if a limit has been requested; else
    // only return false if one of the adjustments failed.
    return ENFORCE_PROCESS_LIMIT || mProcessLimit > 0 ? FALSE : didOomAdj;
}

void CActivityManagerService::TrimApplications()
{
//    synchronized (this) {
//        int i;
//
//        // First remove any unused application processes whose package
//        // has been removed.
//        for (i=mRemovedProcesses.size()-1; i>=0; i--) {
//            final ProcessRecord app = mRemovedProcesses.get(i);
//            if (app.activities.size() == 0
//                    && app.curReceiver == null && app.services.size() == 0) {
//                Slog.i(
//                    TAG, "Exiting empty application process "
//                    + app.processName + " ("
//                    + (app.thread != null ? app.thread.asBinder() : null)
//                    + ")\n");
//                if (app.pid > 0 && app.pid != MY_PID) {
//                    Process.killProcess(app.pid);
//                } else {
//                    try {
//                        app.thread.scheduleExit();
//                    } catch (Exception e) {
//                        // Ignore exceptions.
//                    }
//                }
//                cleanUpApplicationRecordLocked(app, false, -1);
//                mRemovedProcesses.remove(i);
//
//                if (app.persistent) {
//                    if (app.persistent) {
//                        addAppLocked(app.info);
//                    }
//                }
//            }
//        }
//
//        // Now try updating the OOM adjustment for each of the
//        // application processes based on their current state.
//        // If the setOomAdj() API is not supported, then go with our
//        // back-up plan...
//        if (!updateOomAdjLocked()) {
//
//            // Count how many processes are running services.
//            int numServiceProcs = 0;
//            for (i=mLruProcesses.size()-1; i>=0; i--) {
//                final ProcessRecord app = mLruProcesses.get(i);
//
//                if (app.persistent || app.services.size() != 0
//                        || app.curReceiver != null) {
//                    // Don't count processes holding services against our
//                    // maximum process count.
//                    if (localLOGV) Slog.v(
//                        TAG, "Not trimming app " + app + " with services: "
//                        + app.services);
//                    numServiceProcs++;
//                }
//            }
//
//            int curMaxProcs = mProcessLimit;
//            if (curMaxProcs <= 0) curMaxProcs = MAX_PROCESSES;
//            if (mAlwaysFinishActivities) {
//                curMaxProcs = 1;
//            }
//            curMaxProcs += numServiceProcs;
//
//            // Quit as many processes as we can to get down to the desired
//            // process count.  First remove any processes that no longer
//            // have activites running in them.
//            for (   i=0;
//                    i<mLruProcesses.size()
//                        && mLruProcesses.size() > curMaxProcs;
//                    i++) {
//                final ProcessRecord app = mLruProcesses.get(i);
//                // Quit an application only if it is not currently
//                // running any activities.
//                if (!app.persistent && app.activities.size() == 0
//                        && app.curReceiver == null && app.services.size() == 0) {
//                    Slog.i(
//                        TAG, "Exiting empty application process "
//                        + app.processName + " ("
//                        + (app.thread != null ? app.thread.asBinder() : null)
//                        + ")\n");
//                    if (app.pid > 0 && app.pid != MY_PID) {
//                        Process.killProcess(app.pid);
//                    } else {
//                        try {
//                            app.thread.scheduleExit();
//                        } catch (Exception e) {
//                            // Ignore exceptions.
//                        }
//                    }
//                    // todo: For now we assume the application is not buggy
//                    // or evil, and will quit as a result of our request.
//                    // Eventually we need to drive this off of the death
//                    // notification, and kill the process if it takes too long.
//                    cleanUpApplicationRecordLocked(app, false, i);
//                    i--;
//                }
//            }
//
//            // If we still have too many processes, now from the least
//            // recently used process we start finishing activities.
//            if (Config.LOGV) Slog.v(
//                TAG, "*** NOW HAVE " + mLruProcesses.size() +
//                " of " + curMaxProcs + " processes");
//            for (   i=0;
//                    i<mLruProcesses.size()
//                        && mLruProcesses.size() > curMaxProcs;
//                    i++) {
//                final ProcessRecord app = mLruProcesses.get(i);
//                // Quit the application only if we have a state saved for
//                // all of its activities.
//                boolean canQuit = !app.persistent && app.curReceiver == null
//                    && app.services.size() == 0;
//                int NUMA = app.activities.size();
//                int j;
//                if (Config.LOGV) Slog.v(
//                    TAG, "Looking to quit " + app.processName);
//                for (j=0; j<NUMA && canQuit; j++) {
//                    ActivityRecord r = app.activities.get(j);
//                    if (Config.LOGV) Slog.v(
//                        TAG, "  " + r.intent.getComponent().flattenToShortString()
//                        + ": frozen=" + r.haveState + ", visible=" + r.visible);
//                    canQuit = (r.haveState || !r.stateNotNeeded)
//                            && !r.visible && r.stopped;
//                }
//                if (canQuit) {
//                    // Finish all of the activities, and then the app itself.
//                    for (j=0; j<NUMA; j++) {
//                        ActivityRecord r = app.activities.get(j);
//                        if (!r.finishing) {
//                            r.stack.destroyActivityLocked(r, false);
//                        }
//                        r.resultTo = null;
//                    }
//                    Slog.i(TAG, "Exiting application process "
//                          + app.processName + " ("
//                          + (app.thread != null ? app.thread.asBinder() : null)
//                          + ")\n");
//                    if (app.pid > 0 && app.pid != MY_PID) {
//                        Process.killProcess(app.pid);
//                    } else {
//                        try {
//                            app.thread.scheduleExit();
//                        } catch (Exception e) {
//                            // Ignore exceptions.
//                        }
//                    }
//                    // todo: For now we assume the application is not buggy
//                    // or evil, and will quit as a result of our request.
//                    // Eventually we need to drive this off of the death
//                    // notification, and kill the process if it takes too long.
//                    cleanUpApplicationRecordLocked(app, false, i);
//                    i--;
//                    //dump();
//                }
//            }
//
//        }
//
//        int curMaxActivities = MAX_ACTIVITIES;
//        if (mAlwaysFinishActivities) {
//            curMaxActivities = 1;
//        }
//
//        // Finally, if there are too many activities now running, try to
//        // finish as many as we can to get back down to the limit.
//        for (   i=0;
//                i<mMainStack.mLRUActivities.size()
//                    && mMainStack.mLRUActivities.size() > curMaxActivities;
//                i++) {
//            final ActivityRecord r
//                = (ActivityRecord)mMainStack.mLRUActivities.get(i);
//
//            // We can finish this one if we have its icicle saved and
//            // it is not persistent.
//            if ((r.haveState || !r.stateNotNeeded) && !r.visible
//                    && r.stopped && !r.finishing) {
//                final int origSize = mMainStack.mLRUActivities.size();
//                r.stack.destroyActivityLocked(r, true);
//
//                // This will remove it from the LRU list, so keep
//                // our index at the same value.  Note that this check to
//                // see if the size changes is just paranoia -- if
//                // something unexpected happens, we don't want to end up
//                // in an infinite loop.
//                if (origSize > mMainStack.mLRUActivities.size()) {
//                    i--;
//                }
//            }
//        }
//    }
}

void CActivityManagerService::Lock()
{
    SelfLock();
}

void CActivityManagerService::Unlock()
{
    SelfUnlock();
}

ECode CActivityManagerService::GetStringBuffer(
    /* [out] */ StringBuffer* stringbuffer)
{
    VALIDATE_NOT_NULL(stringbuffer);
    *stringbuffer = mStringBuffer;
    return NOERROR;
}

ECode CActivityManagerService::GetDidDexOpt(
    /* [out] */ Boolean* opt)
{
    VALIDATE_NOT_NULL(opt);
    *opt = mDidDexOpt;
    return NOERROR;
}

ECode CActivityManagerService::SetDidDexOpt(
    /* [in] */ Boolean dexopt)
{
    mDidDexOpt = dexopt;
    return NOERROR;
}
