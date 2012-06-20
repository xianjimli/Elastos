
#ifndef __CACTIVITYMANAGERSERVICE_H__
#define __CACTIVITYMANAGERSERVICE_H__

#include "_CActivityManagerService.h"
#include "server/ActivityStack.h"
#include "server/CActivityRecord.h"
#include "server/CServiceRecord.h"
#include "server/ProcessRecord.h"
#include "server/ProcessMap.h"
#include "server/CBatteryStatsService.h"
#include "utils/CApartment.h"
#include "server/BroadcastFilter.h"
#include "server/IntentResolver.h"
#include "server/TaskRecord.h"
#include "server/IntentBindRecord.h"
#include "server/ConnectionRecord.h"
#include "server/ContentProviderRecord.h"
#include "server/BackupRecord.h"
#include "server/UriPermission.h"
#include "content/CComponentName.h"
#include "content/CIntent.h"
#include "content/CConfiguration.h"
#include "capsule/CContentProviderInfo.h"
#include "server/CWindowManagerService.h"
#include "server/PendingThumbnailsRecord.h"
#include "server/UriPermissionOwner.h"
#include "server/CPendingIntentRecord.h"
#include "server/Key.h"
#include "server/ReceiverList.h"
#include "net/Uri.h"
#include "app/CCrashInfo.h"
#include "app/CProcessErrorStateInfo.h"
#include "app/CRunningServiceInfo.h"
#include "app/CApplicationErrorReport.h"
#include "ext/frameworkdef.h"
#include "os/Runnable.h"
#include <elastos/HashMap.h>
#include <elastos/Vector.h>
#include <elastos/List.h>
#include <elastos/Mutex.h>
#include <StringBuffer.h>

using namespace Elastos;
using namespace Elastos::System;
using namespace Elastos::System::Threading;

class ActivityStack;
class CServiceRecord;
class ProcessRecord;
class ContentProviderRecord;
class PendingThumbnailsRecord;
class UriPermissionOwner;
class ReceiverList;

CarClass(CActivityManagerService)
{
public:
    static const String TAG;
    static const Boolean DEBUG = FALSE;
    static const Boolean localLOGV = DEBUG;
    static const Boolean DEBUG_SWITCH = localLOGV || FALSE;
    static const Boolean DEBUG_TASKS = localLOGV || FALSE;
    static const Boolean DEBUG_PAUSE = localLOGV || FALSE;
    static const Boolean DEBUG_OOM_ADJ = localLOGV || FALSE;
    static const Boolean DEBUG_TRANSITION = localLOGV || FALSE;
    static const Boolean DEBUG_BROADCAST = localLOGV || FALSE;
    static const Boolean DEBUG_BROADCAST_LIGHT = DEBUG_BROADCAST || FALSE;
    static const Boolean DEBUG_SERVICE = localLOGV || FALSE;
    static const Boolean DEBUG_SERVICE_EXECUTING = localLOGV || FALSE;
    static const Boolean DEBUG_VISBILITY = localLOGV || FALSE;
    static const Boolean DEBUG_PROCESSES = localLOGV || FALSE;
    static const Boolean DEBUG_PROVIDER = localLOGV || FALSE;
    static const Boolean DEBUG_URI_PERMISSION = localLOGV || FALSE;
    static const Boolean DEBUG_USER_LEAVING = localLOGV || FALSE;
    static const Boolean DEBUG_RESULTS = localLOGV || FALSE;
    static const Boolean DEBUG_BACKUP = localLOGV || FALSE;
    static const Boolean DEBUG_CONFIGURATION = localLOGV || FALSE;
    static const Boolean VALIDATE_TOKENS = FALSE;

    // The flags that are set for all calls we make to the package manager.
    static const Int32 STOCK_PM_FLAGS = CapsuleManager_GET_SHARED_LIBRARY_FILES;

    // Set to false to leave processes running indefinitely, relying on
    // the kernel killing them as resources are required.
    static const Boolean ENFORCE_PROCESS_LIMIT = FALSE;

    // This is the maximum number of activities that we would like to have
    // running at a given time.
    static const Int32 MAX_ACTIVITIES = 20;

    // Maximum number of recent tasks that we can remember.
    static const Int32 MAX_RECENT_TASKS = 20;

    // How long we wait for a launched process to attach to the activity manager
    // before we decide it's never going to come up for real.
    static const Int32 PROC_START_TIMEOUT = 10*1000;

    // The minimum amount of time between successive GC requests for a process.
    static const Int32 GC_MIN_INTERVAL = 60*1000;

    // How long we allow a receiver to run before giving up on it.
    static const Int32 BROADCAST_TIMEOUT = 10*1000;

    // How long we wait for a service to finish executing.
    static const Int32 SERVICE_TIMEOUT = 20*1000;

    // How long a service needs to be running until restarting its process
    // is no longer considered to be a relaunch of the service.
    static const Int32 SERVICE_RESTART_DURATION = 5*1000;

    // How long a service needs to be running until it will start back at
    // SERVICE_RESTART_DURATION after being killed.
    static const Int32 SERVICE_RESET_RUN_DURATION = 60*1000;

    // Multiplying factor to increase restart duration time by, for each time
    // a service is killed before it has run for SERVICE_RESET_RUN_DURATION.
    static const Int32 SERVICE_RESTART_DURATION_FACTOR = 4;

    // The minimum amount of time between restarting services that we allow.
    // That is, when multiple services are restarting, we won't allow each
    // to restart less than this amount of time from the last one.
    static const Int32 SERVICE_MIN_RESTART_TIME_BETWEEN = 10*1000;

    // Maximum amount of time for there to be no activity on a service before
    // we consider it non-essential and allow its process to go on the
    // LRU background list.
    static const Int32 MAX_SERVICE_INACTIVITY = 30*60*1000;

    // OOM adjustments for processes in various states:

    // This is a process without anything currently running in it.  Definitely
    // the first to go! Value set in system/rootdir/init.rc on startup.
    // This value is initalized in the constructor, careful when refering to
    // this static variable externally.
    static const Int32 EMPTY_APP_ADJ;

    // This is a process only hosting activities that are not visible,
    // so it can be killed without any disruption. Value set in
    // system/rootdir/init.rc on startup.
    static const Int32 HIDDEN_APP_MAX_ADJ;
    static const Int32 HIDDEN_APP_MIN_ADJ;

    // This is a process holding the home application -- we want to try
    // avoiding killing it, even if it would normally be in the background,
    // because the user interacts with it so much.
    static const Int32 HOME_APP_ADJ;

    // This is a process currently hosting a backup operation.  Killing it
    // is not entirely fatal but is generally a bad idea.
    static const Int32 BACKUP_APP_ADJ;

    // This is a process holding a secondary server -- killing it will not
    // have much of an impact as far as the user is concerned. Value set in
    // system/rootdir/init.rc on startup.
    static const Int32 SECONDARY_SERVER_ADJ;

    // This is a process with a heavy-weight application.  It is in the
    // background, but we want to try to avoid killing it.  Value set in
    // system/rootdir/init.rc on startup.
    static const Int32 HEAVY_WEIGHT_APP_ADJ;

    // This is a process only hosting components that are perceptible to the
    // user, and we really want to avoid killing them, but they are not
    // immediately visible. An example is background music playback.  Value set in
    // system/rootdir/init.rc on startup.
    static const Int32 PERCEPTIBLE_APP_ADJ;

    // This is a process only hosting activities that are visible to the
    // user, so we'd prefer they don't disappear. Value set in
    // system/rootdir/init.rc on startup.
    static const Int32 VISIBLE_APP_ADJ;

    // This is the process running the current foreground app.  We'd really
    // rather not kill it! Value set in system/rootdir/init.rc on startup.
    static const Int32 FOREGROUND_APP_ADJ;

    // This is a process running a core server, such as telephony.  Definitely
    // don't want to kill it, but doing so is not completely fatal.
    static const Int32 CORE_SERVER_ADJ = -12;

    // The minimum number of hidden apps we want to be able to keep around,
    // without empty apps being able to push them out of memory.
    static const Int32 MIN_HIDDEN_APPS = 2;

    // The maximum number of hidden processes we will keep around before
    // killing them; this is just a control to not let us go too crazy with
    // keeping around processes on devices with large amounts of RAM.
    static const Int32 MAX_HIDDEN_APPS = 15;

    // We put empty content processes after any hidden processes that have
    // been idle for less than 15 seconds.
    static const Int32 CONTENT_APP_IDLE_OFFSET = 15*1000;

    // We put empty content processes after any hidden processes that have
    // been idle for less than 120 seconds.
    static const Int32 EMPTY_APP_IDLE_OFFSET = 120*1000;

    static const Int32 SHOW_ERROR_MSG = 1;
    static const Int32 SHOW_NOT_RESPONDING_MSG = 2;
    static const Int32 SHOW_FACTORY_ERROR_MSG = 3;
    static const Int32 UPDATE_CONFIGURATION_MSG = 4;
    static const Int32 GC_BACKGROUND_PROCESSES_MSG = 5;
    static const Int32 WAIT_FOR_DEBUGGER_MSG = 6;
    static const Int32 BROADCAST_INTENT_MSG = 7;
    static const Int32 BROADCAST_TIMEOUT_MSG = 8;
    static const Int32 SERVICE_TIMEOUT_MSG = 12;
    static const Int32 UPDATE_TIME_ZONE = 13;
    static const Int32 SHOW_UID_ERROR_MSG = 14;
    static const Int32 IM_FEELING_LUCKY_MSG = 15;
    static const Int32 PROC_START_TIMEOUT_MSG = 20;
    static const Int32 DO_PENDING_ACTIVITY_LAUNCHES_MSG = 21;
    static const Int32 KILL_APPLICATION_MSG = 22;
    static const Int32 FINALIZE_PENDING_INTENT_MSG = 23;
    static const Int32 POST_HEAVY_NOTIFICATION_MSG = 24;
    static const Int32 CANCEL_HEAVY_NOTIFICATION_MSG = 25;
    static const Int32 SHOW_STRICT_MODE_VIOLATION_MSG = 26;
    static const Int32 CHECK_EXCESSIVE_WAKE_LOCKS_MSG = 27;

    //from IActivityManager.java
    static const Int32 BROADCAST_SUCCESS = 0;
    static const Int32 BROADCAST_STICKY_CANT_HAVE_PERMISSION = -1;

public:
    class ServiceLookupResult
    {
    public:
        ServiceLookupResult(
            /* [in] */ CServiceRecord* record,
            /* [in] */ String permission)
        {
            mRecord = record;
            mPermission = String::Duplicate(permission);
        }

        ~ServiceLookupResult()
        {
            String::Free(mPermission);
        }

    public:
        AutoPtr<CServiceRecord> mRecord;
        String mPermission;
    };

    class ServiceRestarter : public Runnable
    {
    public:
        CARAPI Run();

    public:
        CARAPI_(void) SetService(
            /* [in] */ CServiceRecord* service)
        {
            mService = service;
        }

    private:
        AutoPtr<CServiceRecord> mService;
        AutoPtr<CActivityManagerService> mActivityManager;
    };

public:
    CActivityManagerService();

    ~CActivityManagerService();

    CARAPI_(void) BatteryNeedsCpuUpdate();

    CARAPI_(void) BatteryPowerChanged(
        /* [in] */ Boolean onBattery);

    CARAPI StartActivity(
        /* [in] */ IApplicationApartment* caller,
        /* [in] */ IIntent* intent,
        /* [in] */ String resolvedType,
        /* [in] */ IObjectContainer* grantedUriPermissions,
        /* [in] */ Int32 grantedMode,
        /* [in] */ IBinder* resultTo,
        /* [in] */ String resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Boolean onlyIfNeeded,
        /* [in] */ Boolean debug,
        /* [out] */ Int32* status);

    CARAPI StartActivityAndWait(
        /* [in] */ IApplicationApartment* caller,
        /* [in] */ IIntent* intent,
        /* [in] */ String resolvedType,
        /* [in] */ IObjectContainer* grantedUriPermissions,
        /* [in] */ Int32 grantedMode,
        /* [in] */ IBinder* resultTo,
        /* [in] */ String resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Boolean onlyIfNeeded,
        /* [in] */ Boolean debug,
        /* [out] */ IWaitResult** result);

    CARAPI StartActivityWithConfig(
        /* [in] */ IApplicationApartment* caller,
        /* [in] */ IIntent* intent,
        /* [in] */ String resolvedType,
        /* [in] */ IObjectContainer* grantedUriPermissions,
        /* [in] */ Int32 grantedMode,
        /* [in] */ IBinder* resultTo,
        /* [in] */ String resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Boolean onlyIfNeeded,
        /* [in] */ Boolean debug,
        /* [in] */ IConfiguration* config,
        /* [out] */ Int32* status);

    CARAPI StartActivityIntentSender(
        /* [in] */ IApplicationApartment* caller,
        /* [in] */ IIntentSender* sender,
        /* [in] */ IIntent* fillInIntent,
        /* [in] */ String resolvedType,
        /* [in] */ IBinder* resultTo,
        /* [in] */ String resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 flagsMask,
        /* [in] */ Int32 flagsValues,
        /* [out] */ Int32* status);

    CARAPI StartNextMatchingActivity(
        /* [in] */ IBinder* callingActivity,
        /* [in] */ IIntent* intent,
        /* [out] */ Boolean* result);

    CARAPI StartActivityInCapsule(
        /* [in] */ Int32 uid,
        /* [in] */ IIntent* intent,
        /* [in] */ String resolvedType,
        /* [in] */ IBinder* resultTo,
        /* [in] */ String resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Boolean onlyIfNeeded,
        /* [out] */ Int32* result);

    CARAPI SetRequestedOrientation(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 requestedOrientation);

    CARAPI GetRequestedOrientation(
        /* [in] */ IBinder* token,
        /* [out] */ Int32* requestedOrientation);

    CARAPI FinishActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* resultData,
        /* [out] */ Boolean* finished);

    CARAPI FinishHeavyWeightApp();

    CARAPI CrashApplication(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 initialPid,
        /* [in] */ String capsuleName,
        /* [in] */ String message);

    CARAPI FinishSubActivity(
        /* [in] */ IBinder* token,
        /* [in] */ String resultWho,
        /* [in] */ Int32 requestCode);

    CARAPI WillActivityBeVisible(
        /* [in] */ IBinder* token,
        /* [out] */ Boolean* visible);

    CARAPI OverridePendingTransition(
        /* [in] */ IBinder* token,
        /* [in] */ String capsuleName,
        /* [in] */ Int32 enterAnim,
        /* [in] */ Int32 exitAnim);

    CARAPI ClearApplicationUserData(
        /* [in] */ String capsuleName,
        /* [in] */ ICapsuleDataObserver* observer,
        /* [out] */ Boolean* result);

    CARAPI KillBackgroundProcesses(
        /* [in] */ String capsuleName);

    CARAPI ForceStopCapsule(
        /* [in] */ String capsuleName);

    CARAPI KillApplicationWithUid(
        /* [in] */ String cap,
        /* [in] */ Int32 uid);

    CARAPI CloseSystemDialogs(
        /* [in] */ String reason);

    CARAPI KillApplicationProcess(
        /* [in] */ String processName,
        /* [in] */ Int32 uid);

    CARAPI AttachApplication(
        /* [in] */ IApplicationApartment* appApartment,
        /* [in] */ Int32 pid);

    CARAPI ActivityIdle(
        /* [in] */ IBinder* token,
        /* [in] */ IConfiguration* config);

    CARAPI ActivityPaused(
        /* [in] */ IBinder* token,
        /* [in] */ IBundle* icicle);

    CARAPI ActivityStopped(
        /* [in] */ IBinder* token,
        /* [in] */ IBitmap* thumbnail, //Bitmap thumbnail
        /* [in] */ String description);

    CARAPI ActivityDestroyed(
        /* [in] */ IBinder* token);

    CARAPI GetCallingPackage(
        /* [in] */ IBinder* token,
        /* [out] */ String* capsule);

    CARAPI GetCallingActivity(
        /* [in] */ IBinder* token,
        /* [out] */ IComponentName** activity);

    CARAPI GetActivityClassForToken(
        /* [in] */ IBinder* token,
        /* [out] */ IComponentName** activityCls);

    CARAPI GetCapsuleForToken(
        /* [in] */ IBinder* token,
        /* [out] */ String* capsule);

    CARAPI GetIntentSender(
        /* [in] */ Int32 type,
        /* [in] */ String capsuleName,
        /* [in] */ IBinder* token,
        /* [in] */ String resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ IIntent* intent,
        /* [in] */ String resolvedType,
        /* [in] */ Int32 flags,
        /* [out] */ IIntentSender** sender);

    CARAPI CancelIntentSender(
        /* [in] */ IIntentSender* sender);

    CARAPI GetCapsuleForIntentSender(
        /* [in] */ IIntentSender* pendingResult,
        /* [out] */ String* capsule);

    CARAPI SetProcessLimit(
        /* [in] */ Int32 max);

    CARAPI GetProcessLimit(
        /* [out] */ Int32* limit);

    CARAPI SetProcessForeground(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 pid,
        /* [in] */ Boolean isForeground);

    CARAPI CheckUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32* permission);

    CARAPI GrantUriPermission(
        /* [in] */ IApplicationApartment* caller,
        /* [in] */ String targetPkg,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags);

    CARAPI RevokeUriPermission(
        /* [in] */ IApplicationApartment* caller,
        /* [in] */ IUri* uri,
        /* [in]*/ Int32 modeFlags);

    CARAPI NewUriPermissionOwner(
        /* [in] */ String name,
        /* [out] */ IBinder** token);

    CARAPI GrantUriPermissionFromOwner(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 fromUid,
        /* [in] */ String targetPkg,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags);

    CARAPI RevokeUriPermissionFromOwner(
        /* [in] */ IBinder* token,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 mode);

    CARAPI ShowWaitingForDebugger(
        /* [in] */ IApplicationApartment* who,
        /* [in] */ Boolean waiting);

    CARAPI GetMemoryInfo(
        /* [out] */ IMemoryInfo** outInfo);

    CARAPI GetTasks(
        /* [in] */ Int32 maxNum,
        /* [in] */ Int32 flags,
        /* [in] */ IThumbnailReceiver* receiver,
        /* [out] */ IObjectContainer* tasks);

    CARAPI GetRecentTasks(
        /* [in] */ Int32 maxNum,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** tasks);

    CARAPI MoveTaskToFront(
        /* [in] */ Int32 task);

    CARAPI MoveTaskToBack(
        /* [in] */ Int32 task);

    CARAPI MoveActivityTaskToBack(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean nonRoot,
        /* [out] */ Boolean* result);

    CARAPI MoveTaskBackwards(
        /* [in] */ Int32 task);

    CARAPI GetTaskForActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean onlyRoot,
        /* [out] */ Int32* taskId);

    CARAPI FinishOtherInstances(
        /* [in] */ IBinder* token,
        /* [in] */ IComponentName* className);

    CARAPI ReportThumbnail(
        /* [in] */ IBinder* token,
        /* [in] */ IBitmap* thumbnail,
        /* [in] */ const ArrayOf<Char8>& description);

    CARAPI GetContentProvider(
        /* [in] */ IApplicationApartment* caller,
        /* [in] */ String name,
        /* [out] */ IContentProviderHolder** providerHolder);

    CARAPI RemoveContentProvider(
        /* [in] */ IApplicationApartment* caller,
        /* [in] */ String name);

    CARAPI PublishContentProviders(
        /* [in] */ IApplicationApartment* caller,
        /* [in] */ IObjectContainer* providers);

    static CARAPI_(void) InstallSystemProviders();

    CARAPI GetProviderMimeType(
        /* [in] */ IUri* uri,
        /* [out] */ String* type);

    CARAPI UnhandledBack();

    CARAPI OpenContentUri(
        /* [in] */ IUri* uri
        /* ParcelFileDescriptor */ );

    CARAPI GoingToSleep();

    CARAPI Shutdown(
        /* [in]*/ Int32 timeout,
        /* [out] */ Boolean* result);

    CARAPI WakingUp();

    CARAPI StopAppSwitches();

    CARAPI ResumeAppSwitches();

    CARAPI SetDebugApp(
        /* [in] */ String capsuleName,
        /* [in] */ Boolean waitForDebugger,
        /* [in] */ Boolean persistent);

    CARAPI SetAlwaysFinish(
        /* [in] */ Boolean enabled);

    CARAPI SetActivityController(
        /* [in] */ IActivityController* controller);

    CARAPI IsUserAMonkey(
        /* [out] */ Boolean* result);

    CARAPI RegisterActivityWatcher(
        /* [in] */ IActivityWatcher* watcher);

    CARAPI UnregisterActivityWatcher(
        /* [in] */ IActivityWatcher* watcher);

    CARAPI EnterSafeMode();

    CARAPI NoteWakeupAlarm(
        /* [in] */ IIntentSender* sender);

    CARAPI KillPids(
        /* [in] */ const ArrayOf<Int32>& pids,
        /* [in] */ String pReason,
        /* [out] */ Boolean* result);

    CARAPI StartRunning(
        /* [in] */ String cap,
        /* [in] */ String cls,
        /* [in] */ String action,
        /* [in] */ String data);

    CARAPI_(Boolean) TestIsSystemReady();

    CARAPI_(void) SystemReady(
        /* [in] */ IRunnable* goingCallback);

    CARAPI HandleApplicationCrash(
        /* [in] */ IBinder* app,
        /* [in] */ CCrashInfo* crashInfo);

    CARAPI HandleApplicationStrictModeViolation(
        /* [in] */ IBinder* app,
        /* [in] */ Int32 violationMask//,
        /* StrictMode.ViolationInfo info */);

    CARAPI HandleApplicationWtf(
        /* [in] */ IBinder* app,
        /* [in] */ String tag,
        /* [in] */ CCrashInfo* crashInfo,
        /* [out] */ Boolean* result);

    CARAPI_(void) AddErrorToDropBox(
        /* [in] */ String eventType,
        /* [in] */ ProcessRecord* process,
        /* [in] */ CActivityRecord* activity,
        /* [in] */ CActivityRecord* parent,
        /* [in] */ String subject,
        /* [in] */ String report,
//        final File logFile,
        /* [in] */ CCrashInfo* crashInfo);

    CARAPI GetProcessesInErrorState(
        /* [out] */ IObjectContainer** procs);

    CARAPI GetRunningAppProcesses(
        /* [out] */ IObjectContainer** appProcs);

    CARAPI GetRunningExternalApplications(
        /* [out] */ IObjectContainer** apps);

    CARAPI GetServices(
        /* [in] */ Int32 maxNum,
        /* [in] */ Int32 flags,
        /* [out] */ IObjectContainer** services);

    CARAPI GetRunningServiceControlPanel(
        /* [in] */ IComponentName* name,
        /* [out] */ IPendingIntent** intent);

    CARAPI StartService(
        /* [in] */ IApplicationApartment* caller,
        /* [in] */ IIntent* service,
        /* [in] */ String resolvedType,
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [out] */ IComponentName** name);

    CARAPI StopService(
        /* [in] */ IApplicationApartment* caller,
        /* [in] */ IIntent* service,
        /* [in] */ String resolvedType,
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [out] */ Int32* result);

    CARAPI PeekService(
        /* [in] */ IIntent* service,
        /* [in] */ String resolvedType,
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [out] */ IBinder** token);

    CARAPI StopServiceToken(
        /* [in] */ IComponentName* className,
        /* [in] */ IBinder* token,
        /* [in] */ Int32 startId,
        /* [out] */ Boolean* succeeded);

    CARAPI SetServiceForeground(
        /* [in] */ IComponentName* className,
        /* [in] */ IBinder* token,
        /* [in] */ Int32 id,
        /* [in] */ INotification* notification,
        /* [in] */ Boolean removeNotification);

    CARAPI BindService(
        /* [in] */ IApplicationApartment* caller,
        /* [in] */ IBinder* token,
        /* [in] */ IIntent* service,
        /* [in] */ String resolvedType,
        /* [in] */ IServiceConnectionInner* connection,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [out] */ Int32* result);

    CARAPI UnbindService(
        /* [in] */ IServiceConnectionInner* connection,
        /* [out] */ Boolean* succeeded);

    CARAPI PublishService(
        /* [in] */ IBinder* token,
        /* [in] */ IIntent* intent,
        /* [in] */ IBinder* service);

    CARAPI UnbindFinished(
        /* [in] */ IBinder* token,
        /* [in] */ IIntent* intent,
        /* [in] */ Boolean doRebind);

    CARAPI ServiceDoneExecuting(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 type,
        /* [in] */ Int32 startId,
        /* [in] */ Int32 res);

    CARAPI BindBackupAgent(
        /* [in] */ IApplicationInfo* app,
        /* [in] */ Int32 backupMode,
        /* [out] */ Boolean* result);

    CARAPI BackupAgentCreated(
        /* [in] */ String agentCapsuleName,
        /* [in] */ IBinder* agent);

    CARAPI UnbindBackupAgent(
        /* [in] */ IApplicationInfo* appInfo);

    CARAPI RegisterReceiver(
        /* [in] */ IApplicationApartment* caller,
        /* [in] */ IIntentReceiver* receiver,
        /* [in] */ IIntentFilter* filter,
        /* [in] */ String permission,
        /* [out] */ IIntent** intent);

    CARAPI UnregisterReceiver(
        /* [in] */ IIntentReceiver* receiver);

    CARAPI BroadcastIntent(
        /* [in] */ IApplicationApartment* caller,
        /* [in] */ IIntent* intent,
        /* [in] */ String resolvedType,
        /* [in] */ IIntentReceiver* resultTo,
        /* [in] */ Int32 resultCode,
        /* [in] */ String resultData,
        /* [in] */ IBundle* map,
        /* [in] */ String requiredPermission,
        /* [in] */ Boolean serialized,
        /* [in] */ Boolean sticky,
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [out] */ Int32* result);

    CARAPI BroadcastIntentInCapsuel(
        /* [in] */ String capsuleName,
        /* [in] */ Int32 uid,
        /* [in] */ IIntent* intent,
        /* [in] */ String resolvedType,
        /* [in] */ IIntentReceiver* resultTo,
        /* [in] */ Int32 resultCode,
        /* [in] */ String resultData,
        /* [in] */ IBundle* map,
        /* [in] */ String requiredPermission,
        /* [in] */ Boolean serialized,
        /* [in] */ Boolean sticky,
        /* [out] */ Int32* result);

    CARAPI UnbroadcastIntent(
        /* [in] */ IApplicationApartment* caller,
        /* [in] */ IIntent* intent);

    CARAPI FinishReceiver(
        /* [in] */ IBinder* who,
        /* [in] */ Int32 resultCode,
        /* [in] */ String resultData,
        /* [in] */ IBundle* resultExtras,
        /* [in] */ Boolean resultAbort);

    CARAPI StartInstrumentation(
        /* [in] */ IComponentName* className,
        /* [in] */ String profileFile,
        /* [in] */ Int32 flags,
        /* [in] */ IBundle* arguments,
        /* [in] */ IInstrumentationWatcher* watcher,
        /* [out] */ Boolean* result);

    CARAPI FinishInstrumentation(
        /* [in] */ IApplicationApartment* target,
        /* [in] */ Int32 resultCode,
        /* [in] */ IBundle* results);

    CARAPI GetDeviceConfigurationInfo(
        /* [out] */ IConfigurationInfo** config);

    CARAPI GetConfiguration(
        /* [out] */ IConfiguration** values);

    CARAPI UpdateConfiguration(
        /* [in] */ IConfiguration* values);

    CARAPI constructor(
        /* [in] */ IWindowManagerEx* wm);

private:
    CARAPI_(void) UpdateCpuStats();

    CARAPI_(void) UpdateCpuStatsNow();

    CARAPI GetCommonServicesLocked(
        /* [out] */ IObjectStringMap** services);

    CARAPI_(void) SetFocusedActivityLocked(
        /* [in] */ CActivityRecord* r);

    CARAPI_(void) UpdateLRUProcessInternalLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Boolean oomAdj,
        /* [in] */ Boolean updateActivityTime,
        /* [in] */ Int32 bestPos);

    CARAPI_(void) UpdateLRUProcessLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Boolean oomAdj,
        /* [in] */ Boolean updateActivityTime);

    CARAPI_(ProcessRecord*) GetProcessRecordLocked(
        /* [in] */ String processName,
        /* [in] */ Int32 uid);

    CARAPI_(Boolean) IsNextTransitionForward();

    CARAPI_(ProcessRecord*) StartProcessLocked(
        /* [in] */ String processName,
        /* [in] */ CApplicationInfo* info,
        /* [in] */ Boolean knownToBeDead,
        /* [in] */ Int32 intentFlags,
        /* [in] */ String hostingType,
        /* [in] */ IComponentName* hostingName,
        /* [in] */ Boolean allowWhileBooting);

    CARAPI_(Boolean) IsAllowedWhileBooting(
        /* [in] */ CApplicationInfo* ai);

    CARAPI_(void) StartProcessLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ String hostingType,
        /* [in] */ String hostingNameStr);

    CARAPI_(void) UpdateUsageStats(
        /* [in] */ CActivityRecord* resumedComponent,
        /* [in] */ Boolean resumed);

    CARAPI_(Boolean) StartHomeActivityLocked();

    CARAPI_(void) StartSetupActivityLocked();

    CARAPI_(void) ReportResumedActivityLocked(
        /* [in] */ CActivityRecord* r);

    CARAPI_(void) DoPendingActivityLaunchesLocked(
        /* [in] */ Boolean doResume);

    CARAPI_(void) AddRecentTaskLocked(
        /* [in] */ TaskRecord* task);

    CARAPI_(void) HandleAppDiedLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Boolean restarting);

    CARAPI_(Int32) GetLRUProcessRecordIndexForAppLocked(
        /* [in] */ const IApplicationApartment* appApartment);

    CARAPI_(ProcessRecord*) GetProcessRecordForAppLocked(
        /* [in] */ const IApplicationApartment* appApartment);

    CARAPI_(void) AppDiedLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Int32 pid,
        /* [in] */ IApplicationApartment* appApartment);

    CARAPI_(void) AppNotResponding(
        /* [in] */ ProcessRecord* app,
        /* [in] */ CActivityRecord* activity,
        /* [in] */ CActivityRecord* parent,
        /* [in] */ String annotation);

    CARAPI_(void) ShowLaunchWarningLocked(
        /* [in] */ CActivityRecord* cur,
        /* [in] */ CActivityRecord* next);

    CARAPI_(void) ForceStopCapsuleLocked(
        /* [in] */ String capsuleName,
        /* [in] */ Int32 uid);

    CARAPI_(Boolean) KillCapsuleProcessesLocked(
        /* [in] */ String capsuleName,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 minOomAdj,
        /* [in] */ Boolean callerWillRestart,
        /* [in] */ Boolean doit);

    CARAPI_(Boolean) ForceStopCapsuleLocked(
        /* [in] */ String name,
        /* [in] */ Int32 uid,
        /* [in] */ Boolean callerWillRestart,
        /* [in] */ Boolean purgeCache,
        /* [in] */ Boolean doit);

    CARAPI_(Boolean) RemoveProcessLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Boolean callerWillRestart);

    CARAPI_(void) ProcessStartTimedOutLocked(
        /* [in] */ ProcessRecord* app);

    CARAPI_(Boolean) AttachApplicationLocked(
        /* [in] */ IApplicationApartment* appApartment,
        /* [in] */ Int32 pid);

    CARAPI_(void) EnableScreenAfterBoot();

    CARAPI_(void) FinishBooting();

    CARAPI_(void) EnsureBootCompleted();

    CARAPI_(CActivityRecord*) GetCallingRecordLocked(
        /* [in] */ IBinder* token);

    CARAPI GetIntentSenderLocked(
        /* [in] */ Int32 type,
        /* [in] */ String capsuleName,
        /* [in] */ Int32 callingUid,
        /* [in] */ IBinder* token,
        /* [in] */ String resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ IIntent* intent,
        /* [in] */ String resolvedType,
        /* [in] */ Int32 flags,
        /* [out] */ IIntentSender** sender);

    CARAPI_(void) CancelIntentSenderLocked(
        /* [in] */ CPendingIntentRecord* rec,
        /* [in] */ Boolean cleanActivity);

    CARAPI_(void) ForegroundTokenDied(
        /* [in] */ /*ForegroundToken token*/);

    CARAPI_(Int32) CheckComponentPermission(
        /* [in] */ String permission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 reqUid);

    CARAPI_(Int32) CheckPermission(
        /* [in] */ String permission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid);

    CARAPI_(Int32) CheckCallingPermission(
        /* [in] */ String permission);

    CARAPI EnforceCallingPermission(
        /* [in] */ String permission,
        /* [in] */ String func);

    CARAPI_(Boolean) CheckHoldingPermissionsLocked(
        /* [in] */ ICapsuleManager* pm,
        /* [in] */ CContentProviderInfo* pi,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags);

    CARAPI_(Boolean) CheckUriPermissionLocked(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags);

    CARAPI CheckGrantUriPermissionLocked(
        /* [in] */ Int32 callingUid,
        /* [in] */ String targetCap,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32* permission);

    CARAPI_(void) GrantUriPermissionUncheckedLocked(
        /* [in] */ Int32 targetUid,
        /* [in] */ String targetCap,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [in] */ UriPermissionOwner* owner);

    CARAPI_(void) GrantUriPermissionLocked(
        /* [in] */ Int32 callingUid,
        /* [in] */ String targetCap,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [in] */ UriPermissionOwner* owner);

    CARAPI_(Int32) CheckGrantUriPermissionFromIntentLocked(
        /* [in] */ Int32 callingUid,
        /* [in] */ String targetCap,
        /* [in] */ IIntent* intent);

    CARAPI_(void) GrantUriPermissionUncheckedFromIntentLocked(
        /* [in] */ Int32 targetUid,
        /* [in] */ String targetCap,
        /* [in] */ IIntent* intent,
        /* [in] */ UriPermissionOwner* owner);

    CARAPI_(void) GrantUriPermissionFromIntentLocked(
        /* [in] */ Int32 callingUid,
        /* [in] */ String targetCap,
        /* [in] */ IIntent* intent,
        /* [in] */ UriPermissionOwner* owner);

    CARAPI_(void) RemoveUriPermissionIfNeededLocked(
        /* [in] */ UriPermission* perm);

    CARAPI_(void) RevokeUriPermissionLocked(
        /* [in] */ Int32 callingUid,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags);

    CARAPI_(Int32) FindAffinityTaskTopLocked(
        /* [in] */ Int32 startIndex,
        /* [in] */ String affinity);

    CARAPI_(void) MoveTaskBackwardsLocked(
        /* [in] */ Int32 task);

    CARAPI_(Int32) GetTaskForActivityLocked(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean onlyRoot);

    CARAPI_(void) SendPendingThumbnail(
        /* [in] */ CActivityRecord* r,
        /* [in] */ IBinder* token,
        /* [in] */ IBitmap* thumbnail,
        /* [in] */ ArrayOf<Char8>* description,
        /* [in] */ Boolean always);

    CARAPI GenerateApplicationProvidersLocked(
        /* [in] */ ProcessRecord* app,
        /* [out] */ IObjectContainer** providers);

    CARAPI_(String) CheckContentProviderPermissionLocked(
        /* [in] */ CContentProviderInfo* cpi,
        /* [in] */ ProcessRecord* r);

    CARAPI GetContentProviderImpl(
        /* [in] */ IApplicationApartment* caller,
        /* [in] */ String name,
        /* [out] */ IContentProviderHolder** providerHolder);

    CARAPI GetContentProviderExternal(
        /* [in] */ String name,
        /* [out] */ IContentProviderHolder** providerHolder);

    CARAPI_(void) RemoveContentProviderExternal(
        /* [in] */ String name);

    CARAPI_(ProcessRecord*) NewProcessRecordLocked(
        /* [in] */ IApplicationApartment* apartment,
        /* [in] */ CApplicationInfo* info,
        /* [in] */ String customProcess);

    CARAPI_(ProcessRecord*) AddAppLocked(
        /* [in] */ CApplicationInfo* info);

    CARAPI_(Boolean) CheckAppSwitchAllowedLocked(
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [in] */ String name);

    CARAPI_(void) RetrieveSettings();

    static CARAPI_(List<AutoPtr<IComponentName> >*) ReadLastDonePreBootReceivers();

    static CARAPI_(void) WriteLastDonePreBootReceivers(
        /* [in] */ List<AutoPtr<IComponentName> >& list);

    CARAPI_(Boolean) MakeAppCrashingLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ String shortMsg,
        /* [in] */ String longMsg,
        /* [in] */ String stackTrace);

    CARAPI_(void) MakeAppNotRespondingLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ String activity,
        /* [in] */ String shortMsg,
        /* [in] */ String longMsg);

    CARAPI_(void) GenerateProcessError(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Int32 condition,
        /* [in] */ String activity,
        /* [in] */ String shortMsg,
        /* [in] */ String longMsg,
        /* [in] */ String stackTrace,
        /* [out] */ CProcessErrorStateInfo** report);

    CARAPI_(void) KillAppAtUsersRequest(
        /* [in] */ ProcessRecord* app//,
        /* Dialog fromDialog */);

    CARAPI_(Boolean) HandleAppCrashLocked(
        /* [in] */ ProcessRecord* app);

    CARAPI_(void) StartAppProblemLocked(
        /* [in] */ ProcessRecord* app);

    CARAPI_(void) SkipCurrentReceiverLocked(
        /* [in] */ ProcessRecord* app);

    CARAPI_(void) LogStrictModeViolationToDropBox(
        /* [in] */ ProcessRecord* process//,
        /* StrictMode.ViolationInfo info */);

    CARAPI_(ProcessRecord*) FindAppProcess(
        /* [in] */ IBinder* app);

    CARAPI_(void) AppendDropBoxProcessHeaders(
        /* [in] */ ProcessRecord* process,
        /* [in] */ StringBuffer& sb);

    CARAPI_(void) CrashApplication(
        /* [in] */ ProcessRecord* r,
        /* [in] */ CCrashInfo* crashInfo);

    CARAPI CreateAppErrorIntentLocked(
        /* [in] */ ProcessRecord* r,
        /* [in] */ Millisecond64 timeMillis,
        /* [in] */ CCrashInfo* crashInfo,
        /* [out] */ IIntent** intent);

    CARAPI CreateAppErrorReportLocked(
        /* [in] */ ProcessRecord* r,
        /* [in] */ Millisecond64 timeMillis,
        /* [in] */ CCrashInfo* crashInfo,
        /* [out] */ CApplicationErrorReport** report);

    CARAPI_(void) KillServicesLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Boolean allowRestart);

    CARAPI_(void) RemoveDyingProviderLocked(
        /* [in] */ ProcessRecord* proc,
        /* [in] */ ContentProviderRecord* cpr);

    CARAPI_(void) CleanUpApplicationRecordLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Boolean restarting,
        /* [in] */ Int32 index);

    CARAPI_(Boolean) CheckAppInLaunchingProvidersLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Boolean alwaysBad);

    CARAPI_(CRunningServiceInfo*) MakeRunningServiceInfoLocked(
        /* [in] */ CServiceRecord* r);

    CARAPI_(CServiceRecord*) FindServiceLocked(
        /* [in] */ IComponentName* name,
        /* [in] */ IBinder* token);

    CARAPI_(ServiceLookupResult*) FindServiceLocked(
        /* [in] */ IIntent* service,
        /* [in] */ String resolvedType,
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid);

    CARAPI_(ServiceLookupResult*) RetrieveServiceLocked(
        /* [in] */ IIntent* service,
        /* [in] */ String resolvedType,
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid);

    CARAPI_(void) BumpServiceExecutingLocked(
        /* [in] */ CServiceRecord* r,
        /* [in] */ String why);

    CARAPI_(void) SendServiceArgsLocked(
        /* [in] */ CServiceRecord* r,
        /* [in] */ Boolean oomAdjusted);

    CARAPI_(Boolean) RequestServiceBindingLocked(
        /* [in] */ CServiceRecord* r,
        /* [in] */ IntentBindRecord* i,
        /* [in] */ Boolean rebind);

    CARAPI_(void) RequestServiceBindingsLocked(
        /* [in] */ CServiceRecord* r);

    CARAPI RealStartServiceLocked(
        /* [in] */ CServiceRecord* r,
        /* [in] */ ProcessRecord* app);

    CARAPI_(Boolean) ScheduleServiceRestartLocked(
        /* [in] */ CServiceRecord* r,
        /* [in] */ Boolean allowCancel);

    CARAPI_(void) PerformServiceRestartLocked(
        /* [in] */ CServiceRecord* r);

    CARAPI_(Boolean) UnscheduleServiceRestartLocked(
        /* [in] */ CServiceRecord* r);

    CARAPI_(Boolean) BringUpServiceLocked(
        /* [in] */ CServiceRecord* r,
        /* [in] */ Int32 intentFlags,
        /* [in] */ Boolean whileRestarting);

    CARAPI BringDownServiceLocked(
        /* [in] */ CServiceRecord* r,
        /* [in] */ Boolean force);

    CARAPI StartServiceLocked(
        /* [in] */ IApplicationApartment* caller,
        /* [in] */ IIntent* service,
        /* [in] */ String resolvedType,
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [out] */ IComponentName** name);

    CARAPI StartServiceInCapsule(
        /* [in] */ Int32 uid,
        /* [in] */ IIntent* service,
        /* [in] */ String resolvedType,
        /* [out] */ IComponentName** name);

    CARAPI UpdateServiceForegroundLocked(
        /* [in] */ ProcessRecord* proc,
        /* [in] */ Boolean oomAdj);

    CARAPI RemoveConnectionLocked(
        /* [in] */ ConnectionRecord* c,
        /* [in] */ ProcessRecord* skipApp,
        /* [in] */ CActivityRecord* skipAct);

    CARAPI ServiceDoneExecutingLocked(
        /* [in] */ CServiceRecord* r,
        /* [in] */ Boolean inStopping);

    CARAPI_(void) ServiceTimeout(
        /* [in] */ ProcessRecord* proc);

    CARAPI_(void) HandleServiceTimeout(
        /* [in] */ ProcessRecord* proc);

    CARAPI_(List<AutoPtr<IIntent> >*) GetStickiesLocked(
        /* [in] */ String action,
        /* [in] */ IIntentFilter* filter,
        /* [in] */ List<AutoPtr<IIntent> >* cur);

    CARAPI ScheduleBroadcastsLocked();

    CARAPI_(void) RemoveReceiverLocked(
        /* [in] */ ReceiverList* rl);

    CARAPI_(void) SendCapsuleBroadcastLocked(
        /* [in] */ Int32 cmd,
        /* [in] */ ArrayOf<String>& capsules);

    CARAPI VerifyBroadcastLocked(
        /* [in] */ IIntent* intent,
        /* [out] */ IIntent** newIntent);

    CARAPI BroadcastIntentLocked(
        /* [in] */ ProcessRecord* callerApp,
        /* [in] */ String callerCapsule,
        /* [in] */ IIntent *intent,
        /* [in] */ String resolvedType,
        /* [in] */ IIntentReceiver* resultTo,
        /* [in] */ Int32 resultCode,
        /* [in] */ String resultData,
        /* [in] */ IBundle* map,
        /* [in] */ String requiredPermission,
        /* [in] */ Boolean ordered,
        /* [in] */ Boolean sticky,
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [out] */ Int32* result);

    CARAPI_(Boolean) FinishReceiverLocked(
        /* [in] */ IBinder* receiver,
        /* [in] */ Int32 resultCode,
        /* [in] */ String resultData,
        /* [in] */ IBundle* resultExtras,
        /* [in] */ Boolean resultAbort,
        /* [in] */ Boolean _explicit);

    CARAPI_(void) LogBroadcastReceiverDiscardLocked(
        /* [in] */ BroadcastRecord* r);

    CARAPI_(void) SetBroadcastTimeoutLocked(
        /* [in] */ Millisecond64 timeoutTime);

    CARAPI_(void) CancelBroadcastTimeoutLocked();

    CARAPI_(void) BroadcastTimeoutLocked(
        /* [in] */ Boolean fromMsg);

    CARAPI ProcessCurBroadcastLocked(
        /* [in] */ BroadcastRecord* r,
        /* [in] */ ProcessRecord* app);

    CARAPI PerformReceiveLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ IIntentReceiver* receiver,
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 resultCode,
        /* [in] */ String data,
        /* [in] */ IBundle* extras,
        /* [in] */ Boolean ordered,
        /* [in] */ Boolean sticky);

    CARAPI DeliverToRegisteredReceiverLocked(
        /* [in] */ BroadcastRecord* r,
        /* [in] */ BroadcastFilter* filter,
        /* [in] */ Boolean ordered);

    CARAPI_(void) AddBroadcastToHistoryLocked(
        /* [in] */ BroadcastRecord* r);

    CARAPI ProcessNextBroadcast(
        /* [in] */ Boolean fromMsg);

    CARAPI_(void) ReportStartInstrumentationFailure(
        /* [in] */ IInstrumentationWatcher* watcher,
        /* [in] */ IComponentName* cn,
        /* [in] */ String report);

    CARAPI_(void) FinishInstrumentationLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Int32 resultCode,
        /* [in] */ IBundle* results);

    CARAPI_(Boolean) UpdateConfigurationLocked(
        /* [in] */ IConfiguration* values,
        /* [in] */ CActivityRecord* starting);

    CARAPI_(Int32) ComputeOomAdjLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Int32 hiddenAdj,
        /* [in] */ ProcessRecord* TOP_APP,
        /* [in] */ Boolean recursed);

    CARAPI_(void) PerformAppGcLocked(
        /* [in] */ ProcessRecord* app);

    CARAPI_(Boolean) CanGcNowLocked();

    CARAPI_(void) PerformAppGcsLocked();

    CARAPI_(void) PerformAppGcsIfAppropriateLocked();

    CARAPI_(void) ScheduleAppGcsLocked();

    CARAPI_(void) AddProcessToGcListLocked(
        /* [in] */ ProcessRecord* proc);

    CARAPI_(void) ScheduleAppGcLocked(
        /* [in] */ ProcessRecord* app);

    CARAPI_(void) CheckExcessivePowerUsageLocked(
        /* [in] */ Boolean doKills);

    CARAPI_(Boolean) UpdateOomAdjLocked(
        /* [in] */ ProcessRecord* app,
        /* [in] */ Int32 hiddenAdj,
        /* [in] */ ProcessRecord* TOP_APP);

    CARAPI_(CActivityRecord*) ResumedAppLocked();

    CARAPI_(Boolean) UpdateOomAdjLocked(
        /* [in] */ ProcessRecord* app);

    CARAPI_(Boolean) UpdateOomAdjLocked();

    CARAPI_(void) TrimApplications();

    CARAPI_(ICapsuleManager*) GetCapsuleManager();

    CARAPI_(void) Lock();

    CARAPI_(void) Unlock();

private:
    friend class ActivityStack;
    friend class CActivityRecord;

    AutoPtr<IApartment> mApartment;

    Int32 mMyPid;

    ActivityStack* mMainStack;

    /**
     * Description of a request to start a new activity, which has been held
     * due to app switches being disabled.
     */
    class PendingActivityLaunch
    {
    public:
        PendingActivityLaunch() : mGrantedUriPermissions(0) {}

    public:
        AutoPtr<CActivityRecord> mR;
        AutoPtr<CActivityRecord> mSourceRecord;
        List<AutoPtr<IUri> >* mGrantedUriPermissions;
        Int32 mGrantedMode;
        Boolean mOnlyIfNeeded;
    };

    List<PendingActivityLaunch*> mPendingActivityLaunches;

    /**
     * List of all active broadcasts that are to be executed immediately
     * (without waiting for another broadcast to finish).  Currently this only
     * contains broadcasts to registered receivers, to avoid spinning up
     * a bunch of processes to execute IntentReceiver components.
     */
    List<BroadcastRecord*> mParallelBroadcasts;

     /**
     * List of all active broadcasts that are to be executed one at a time.
     * The object at the top of the list is the currently activity broadcasts;
     * those after it are waiting for the top to finish..
     */
    List<BroadcastRecord*> mOrderedBroadcasts;

    /**
     * Historical data of past broadcasts, for debugging.
     */
    static const Int32 MAX_BROADCAST_HISTORY = 100;
    BroadcastRecord* mBroadcastHistory[MAX_BROADCAST_HISTORY];

    /**
     * Set when we current have a BROADCAST_INTENT_MSG in flight.
     */
    Boolean mBroadcastsScheduled;

    /**
     * Activity we have told the window manager to have key focus.
     */
    AutoPtr<CActivityRecord> mFocusedActivity;

    /**
     * List of intents that were used to start the most recent tasks.
     */
    List<TaskRecord*> mRecentTasks;

    /**
     * All of the applications we currently have running organized by name.
     * The keys are strings of the application capsule name (as
     * returned by the capsule manager), and the keys are ApplicationRecord
     * objects.
     */
    ProcessMap<ProcessRecord*>* mProcessNames;

    /**
     * The currently running heavy-weight process, if any.
     */
    ProcessRecord* mHeavyWeightProcess;

    /**
     * The last time that various processes have crashed.
     */
    ProcessMap<Millisecond64>* mProcessCrashTimes;

    /**
     * Set of applications that we consider to be bad, and will reject
     * incoming broadcasts from (which the user has no control over).
     * Processes are added to this set when they have crashed twice within
     * a minimum amount of time; they are removed from it when they are
     * later restarted (hopefully due to some user action).  The value is the
     * time it was added to the list.
     */
    ProcessMap<Millisecond64>* mBadProcesses;

    Mutex mPidsSelfLock;

    /**
     * All of the processes we currently have running organized by pid.
     * The keys are the pid running the application.
     *
     * <p>NOTE: This object is protected by its own lock, NOT the global
     * activity manager lock!
     */
    HashMap<Int32, ProcessRecord*> mPidsSelfLocked;

    /**
     * List of records for processes that someone had tried to start before the
     * system was ready.  We don't start them at that point, but ensure they
     * are started by the time booting is complete.
     */
    List<ProcessRecord*> mProcessesOnHold;

    /**
     * List of records for processes that we have started and are waiting
     * for them to call back.  This is really only needed when running in
     * single processes mode, in which case we do not have a unique pid for
     * each process.
     */
    List<ProcessRecord*> mStartingProcesses;

    /**
     * List of persistent applications that are in the process
     * of being started.
     */
    List<ProcessRecord*> mPersistentStartingProcesses;

    /**
     * Processes that are being forcibly torn down.
     */
    List<ProcessRecord*> mRemovedProcesses;

    /**
     * List of running applications, sorted by recent usage.
     * The first entry in the list is the least recently used.
     * It contains ApplicationRecord objects.  This list does NOT include
     * any persistent application records (since we never want to exit them).
     */
    List<ProcessRecord*> mLRUProcesses;

    /**
     * List of processes that should gc as soon as things are idle.
     */
    List<ProcessRecord*> mProcessesToGc;

    /**
     * This is the process holding what we currently consider to be
     * the "home" activity.
     */
    ProcessRecord* mHomeProcess;

    /**
     * Set of IntentSenderRecord objects that are currently active.
     */
    HashMap<Key*, CPendingIntentRecord*> mIntentSenderRecords;

    /**
     * True if we have a pending unexpired BROADCAST_TIMEOUT_MSG posted to our handler.
     */
    Boolean mPendingBroadcastTimeoutMessage;

    /**
     * Intent broadcast that we have tried to start, but are
     * waiting for its application's process to be created.  We only
     * need one (instead of a list) because we always process broadcasts
     * one at a time, so no others can be started while waiting for this
     * one.
     */
    BroadcastRecord* mPendingBroadcast;

    /**
     * The receiver index that is pending, to restart the broadcast if needed.
     */
    List<AutoPtr<IObject> >::Iterator mPendingBroadcastRecvIndex;

     /**
     * Keeps track of all IIntentReceivers that have been registered for
     * broadcasts.  Hash keys are the receiver IBinder, hash value is
     * a ReceiverList.
     */
    HashMap<AutoPtr<IIntentReceiver>, ReceiverList*> mRegisteredReceivers;


    /**
     * Resolver for broadcast intents to registered receivers.
     * Holds BroadcastFilter (subclass of IntentFilter).
     */
    IntentResolver<BroadcastFilter, BroadcastFilter>* mReceiverResolver;

    /**
     * State of all active sticky broadcasts.  Keys are the action of the
     * sticky Intent, values are an ArrayList of all broadcasted intents with
     * that action (which should usually be one).
     */
    HashMap<String, List<AutoPtr<IIntent> >*> mStickyBroadcasts;

    /**
     * All currently running services.
     */
    HashMap<AutoPtr<IComponentName>, AutoPtr<CServiceRecord> > mServices;

    /**
     * All currently running services indexed by the Intent used to start them.
     */
    HashMap<CIntent::FilterComparison*, AutoPtr<CServiceRecord> > mServicesByIntent;

    /**
     * All currently bound service connections.  Keys are the IBinder of
     * the client's IServiceConnection.
     */
    Map<AutoPtr<IServiceConnectionInner>, List<ConnectionRecord*>*> mServiceConnections;

    /**
     * List of services that we have been asked to start,
     * but haven't yet been able to.  It is used to hold start requests
     * while waiting for their corresponding application thread to get
     * going.
     */
    List<AutoPtr<CServiceRecord> > mPendingServices;

     /**
     * List of services that are scheduled to restart following a crash.
     */
    List<AutoPtr<CServiceRecord> > mRestartingServices;

    /**
     * List of services that are in the process of being stopped.
     */
    List<AutoPtr<CServiceRecord> > mStoppingServices;

    /**
     * Backup/restore process management
     */
    String mBackupAppName;
    BackupRecord* mBackupTarget;

    /**
     * List of PendingThumbnailsRecord objects of clients who are still
     * waiting to receive all of the thumbnails for a task.
     */
    List<PendingThumbnailsRecord*> mPendingThumbnails;

    /**
     * List of HistoryRecord objects that have been finished and must
     * still report back to a pending thumbnail receiver.
     */
    List<AutoPtr<CActivityRecord> > mCancelledThumbnails;

    /**
     * All of the currently running global content providers.  Keys are a
     * string containing the provider name and values are a
     * ContentProviderRecord object containing the data about it.  Note
     * that a single provider may be published under multiple names, so
     * there may be multiple entries here for a single one in mProvidersByClass.
     */
    HashMap<String, ContentProviderRecord*> mProvidersByName;

    /**
     * All of the currently running global content providers.  Keys are a
     * string containing the provider's implementation class and values are a
     * ContentProviderRecord object containing the data about it.
     */
    HashMap<String, ContentProviderRecord*> mProvidersByClass;

    /**
     * List of content providers who have clients waiting for them.  The
     * application is currently being launched and the provider will be
     * removed from this list once it is published.
     */
    List<ContentProviderRecord*> mLaunchingProviders;

    /**
     * Global set of specific Uri permissions that have been granted.
     */
    Map<Int32, HashMap<IUri*, UriPermission*>*> mGrantedUriPermissions;

    /**
     * Thread-local storage used to carry caller permissions over through
     * indirect content-provider access.
     * @see #ActivityManagerService.openContentUri()
     */
    class Identity
    {
    public:
        Identity(Int32 pid, Int32 uid)
        {
            mPid = pid;
            mUid = uid;
        }

    public:
        Int32 mPid;
        Int32 mUid;
    };

    /**
     * All information we have collected about the runtime performance of
     * any user id that can impact battery performance.
     */
    AutoPtr<CBatteryStatsService> mBatteryStatsService;

    /**
     * Current configuration information.  HistoryRecord objects are given
     * a reference to this object to indicate which configuration they are
     * currently running in, so this object must be kept immutable.
     */
    AutoPtr<CConfiguration> mConfiguration;

    /**
     * Current sequencing integer of the configuration, for skipping old
     * configurations.
     */
    Int32 mConfigurationSeq;

    /**
     * List of initialization arguments to pass to all processes when binding applications to them.
     * For example, references to the commonly used services.
     */
    HashMap<String, AutoPtr<IInterface> >* mAppBindArgs;

    AutoPtr<IComponentName> mTopComponent;
    String mTopAction;
    String mTopData;
    Boolean mProcessesReady;
    Boolean mSystemReady;
    Boolean mWaitingUpdate;
    Boolean mDidUpdate;

    Int32 mFactoryTest;

    /**
     * The time at which we will allow normal application switches again,
     * after a call to {@link #stopAppSwitches()}.
     */
    Millisecond64 mAppSwitchesAllowedTime;

    /**
     * This is set to true after the first switch after mAppSwitchesAllowedTime
     * is set; any switches after that will clear the time.
     */
    Boolean mDidAppSwitch;

    /**
     * Set while we are wanting to sleep, to prevent any
     * activities from being started/resumed.
     */
    Boolean mSleeping;

    /**
     * Set if we are shutting down the system, similar to sleeping.
     */
    Boolean mShuttingDown;

    /**
     * Task identifier that activities are currently being started
     * in.  Incremented each time a new task is created.
     * todo: Replace this with a TokenSpace class that generates non-repeating
     * integers that won't wrap.
     */
    Int32 mCurTask;

    /**
     * Current sequence id for oom_adj computation traversal.
     */
    Int32 mAdjSeq;

    /**
     * Current sequence id for process LRU updating.
     */
    Int32 mLRUSeq;

    /**
     * System monitoring: number of processes that died since the last
     * N procs were started.
     */
    Int32* mProcDeaths;

    String mDebugApp;
    Boolean mWaitForDebugger;
    Boolean mDebugTransient;
    String mOrigDebugApp;
    Boolean mOrigWaitForDebugger;

    AutoPtr<IActivityController> mController;

    Int32 mProcessLimit;

    AutoPtr<CWindowManagerService> mWindowManager;
};

#endif // __CACTIVITYMANAGERSERVICE_H__
