
#include <unistd.h>
#include <new>
#include "ext/frameworkext.h"
#include "utils/CParcelableObjectContainer.h"
#include "app/CApplicationApartment.h"
#include "app/LoadedCap.h"
#include "app/CContextImpl.h"
#include "app/CDefaultInstrumentation.h"
#include "app/ActivityManagerNative.h"
#include "app/CContentProviderHolder.h"
#include "app/CResultInfo.h"
#include "content/CApplicationInfo.h"
#include "content/CInstrumentationInfo.h"
#include "capsule/CContentProviderInfo.h"
#include "os/CApartment.h"
#include "os/CServiceManager.h"
#include "view/CWindowManagerLayoutParams.h"
#include "view/CWindowManagerImpl.h"
#include <Slogger.h>
#include <StringBuffer.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

#define UNUSED(x) (void)x

const char* CApplicationApartment::TAG = "CApplicationApartment";
const Boolean CApplicationApartment::DEBUG;
const Boolean CApplicationApartment::localLOGV;
const Boolean CApplicationApartment::DEBUG_MESSAGES;
const Boolean CApplicationApartment::DEBUG_BROADCAST;
const Boolean CApplicationApartment::DEBUG_RESULTS;

ECode CApplicationApartment::
ActivityClientRecord::GetDescription(
    /* [out] */ String* description)
{
    String tkDes, nmDes;

    if (description == NULL) return E_INVALID_ARGUMENT;

    AutoPtr<IComponentName> componentName;
    mIntent->GetComponent((IComponentName**)&componentName);
    mToken->GetDescription(&tkDes);
    if (componentName != NULL) componentName->GetShortDescription(&nmDes);
    else nmDes = "no component name";

    StringBuffer sb;
    sb += "ActivityRecord{ token=" + tkDes + " " + nmDes + "}";
    *description = (const char*)sb;
    return NOERROR;
}

CApplicationApartment::
ProviderClientRecord::ProviderClientRecord(
    /* [in] */ const String& name,
    /* [in] */ IContentProvider* provider,
    /* [in] */ IContentProvider* localProvider)
    : mName(name)
    , mProvider(provider)
    , mLocalProvider(localProvider)
{}

CApplicationApartment::CApplicationApartment() :
    mCapsules(11),
    mResourceCapsules(5),
    mActiveResources(5),
    mProviderMap(5)
{
    mProviderRefCountMap = new HashMap<AutoPtr<IContentProvider>,
            Int32, HashICP, ICPEq>(5);

    CApartment::New(TRUE, (IApartment**)&mApartment);
}

CApplicationApartment::~CApplicationApartment()
{
    mCapsules.Clear();
    mResourceCapsules.Clear();
    mActiveResources.Clear();
    mProviderMap.Clear();

    mProviderRefCountMap->Clear();
    delete mProviderRefCountMap;

    mActivities.Clear();
}

AutoPtr<ICapsuleManager> CApplicationApartment::GetCapsuleManager()
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

ECode CApplicationApartment::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplicationApartment::BindApplication(
    /* [in] */ const String& processName,
    /* [in] */ IApplicationInfo* appInfo,
    /* [in] */ IObjectContainer* providers,
    /* [in] */ IComponentName* instrumentationName,
    /* [in] */ const String& profileFile,
    /* [in] */ IBundle* instrumentationArgs,
    /* [in] */ IInstrumentationWatcher* instrumentationWatcher,
    /* [in] */ Int32 debugMode,
    /* [in] */ Boolean isRestrictedBackupMode,
    /* [in] */ IConfiguration* config,
    /* [in] */ IObjectStringMap* services)
{
    ECode (STDCALL CApplicationApartment::*pHandlerFunc)(
            AppBindData* data);
    pHandlerFunc = &CApplicationApartment::HandleBindApplication;

    if (services != NULL) {
        // Setup the service cache in the ServiceManager
        AutoPtr<CServiceManager> serviceManager;
        CServiceManager::AcquireSingletonByFriend((CServiceManager**)&serviceManager);
        serviceManager->InitServiceCache(services);
    }
    AppBindData* data = new AppBindData();
    data->mProcessName = processName;
    data->mAppInfo = (CApplicationInfo*)appInfo;
    data->mProviders = providers;
    data->mInstrumentationName = instrumentationName;
    data->mProfileFile = profileFile;
    data->mInstrumentationArgs = instrumentationArgs;
    data->mInstrumentationWatcher = instrumentationWatcher;
    data->mDebugMode = debugMode;
    data->mRestrictedBackupMode = isRestrictedBackupMode;
    data->mConfig = config;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)data);

    return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CApplicationApartment::ScheduleLaunchActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ IBinder* token,
    /* [in] */ Int32 ident,
    /* [in] */ IActivityInfo* info,
    /* [in] */ IBundle* state,
    /* [in] */ IObjectContainer* pendingResults,
    /* [in] */ IObjectContainer* pendingNewIntents,
    /* [in] */ Boolean notResumed,
    /* [in] */ Boolean isForward)
{
    ECode (STDCALL CApplicationApartment::*pHandlerFunc)(
        ActivityClientRecord* r, IIntent* customIntent);
    pHandlerFunc = &CApplicationApartment::HandleLaunchActivity;

    ActivityClientRecord* r = new ActivityClientRecord();
    r->mToken = token;
    r->mIdent = ident;
    r->mIntent = intent;
    r->mActivityInfo = (CActivityInfo*)info;
    r->mState = (CBundle*)state;

    if (pendingResults != NULL) {
        Int32 count;
        pendingResults->GetObjectCount(&count);
        if (count > 0) {
            r->mPendingResults = new List<AutoPtr<CResultInfo> >();
            AutoPtr<IObjectEnumerator> enumerator;
            pendingResults->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
            Boolean hasNext;
            while(enumerator->MoveNext(&hasNext), hasNext) {
                AutoPtr<IParcelable> obj;
                enumerator->Current((IInterface**)&obj);
                r->mPendingResults->PushBack((CResultInfo*)(IParcelable*)obj);
            }
        }
    }

    if (pendingNewIntents != NULL) {
        Int32 count;
        pendingNewIntents->GetObjectCount(&count);
        if (count > 0) {
            r->mPendingIntents = new List<AutoPtr<IIntent> >();
            AutoPtr<IObjectEnumerator> enumerator;
            pendingNewIntents->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
            Boolean hasNext;
            while(enumerator->MoveNext(&hasNext), hasNext) {
                AutoPtr<IIntent> obj;
                enumerator->Current((IInterface**)&obj);
                r->mPendingIntents->PushBack((IIntent*)obj);
            }
        }
    }

    r->mStartsNotResumed = notResumed;
    r->mIsForward = isForward;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)r);
    params->WriteInterfacePtr(NULL);

    return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CApplicationApartment::ScheduleRelaunchActivity(
    /* [in] */ IBinder* token,
    /* [in] */ IObjectContainer* pendingResults,
    /* [in] */ IObjectContainer* pendingNewIntents,
    /* [in] */ Int32 configChanges,
    /* [in] */ Boolean notResumed,
    /* [in] */ IConfiguration* config)
{
    ECode (STDCALL CApplicationApartment::*pHandlerFunc)(
        ActivityClientRecord* r, Int32 configChanges);
    pHandlerFunc = &CApplicationApartment::HandleRelaunchActivity;

    ActivityClientRecord* r = new ActivityClientRecord();
    r->mToken = token;

    if (pendingResults != NULL) {
        Int32 count;
        pendingResults->GetObjectCount(&count);
        if (count > 0) {
            r->mPendingResults = new List<AutoPtr<CResultInfo> >();
            AutoPtr<IObjectEnumerator> enumerator;
            pendingResults->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
            Boolean hasNext;
            while(enumerator->MoveNext(&hasNext), hasNext) {
                AutoPtr<IParcelable> obj;
                enumerator->Current((IInterface**)&obj);
                r->mPendingResults->PushBack((CResultInfo*)(IParcelable*)obj);
            }
        }
    }

    if (pendingNewIntents != NULL) {
        Int32 count;
        pendingNewIntents->GetObjectCount(&count);
        if (count > 0) {
            r->mPendingIntents = new List<AutoPtr<IIntent> >();
            AutoPtr<IObjectEnumerator> enumerator;
            pendingNewIntents->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
            Boolean hasNext;
            while(enumerator->MoveNext(&hasNext), hasNext) {
                AutoPtr<IIntent> obj;
                enumerator->Current((IInterface**)&obj);
                r->mPendingIntents->PushBack((IIntent*)obj);
            }
        }
    }

    r->mStartsNotResumed = notResumed;
    r->mCreatedConfig = config;

    mCapsulesLock.Lock();
    mRelaunchingActivities.PushBack(r);
    mCapsulesLock.Unlock();

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)r);
    params->WriteInt32(configChanges);

    return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CApplicationApartment::ScheduleSendResult(
    /* [in] */ IBinder* token,
    /* [in] */ IObjectContainer* results)
{
    ECode (STDCALL CApplicationApartment::*pHandlerFunc)(
        ResultData* res);
    pHandlerFunc = &CApplicationApartment::HandleSendResult;

    ResultData* res = new ResultData();
    res->mToken = token;
    AutoPtr<IObjectEnumerator> enumerator;
    results->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext;
    while(enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IParcelable> obj;
        enumerator->Current((IInterface**)&obj);
        res->mResults.PushBack((CResultInfo*)(IParcelable*)obj);
    }

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)res);

    return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CApplicationApartment::ScheduleResumeActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean isForward)
{
    ECode (STDCALL CApplicationApartment::*pHandlerFunc)(
        IBinder* token, Boolean clearHide, Boolean isForward);
    pHandlerFunc = &CApplicationApartment::HandleResumeActivity;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(token);
    params->WriteBoolean(TRUE);
    params->WriteBoolean(isForward);

    return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CApplicationApartment::SchedulePauseActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean finished,
    /* [in] */ Boolean userLeaving,
    /* [in] */ Int32 configChanges)
{
    ECode (STDCALL CApplicationApartment::*pHandlerFunc)(
        IBinder* token, Boolean finished,
        Boolean userLeaving, Int32 configChanges);
    pHandlerFunc = &CApplicationApartment::HandlePauseActivity;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(token);
    params->WriteBoolean(finished);
    params->WriteBoolean(userLeaving);
    params->WriteInt32(configChanges);

    return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CApplicationApartment::ScheduleStopActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean showWindow,
    /* [in] */ Int32 configChanges)
{
    ECode (STDCALL CApplicationApartment::*pHandlerFunc)(
        IBinder* token, Boolean show, Int32 configChanges);
    pHandlerFunc = &CApplicationApartment::HandleStopActivity;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(token);
    params->WriteBoolean(showWindow);
    params->WriteInt32(configChanges);

    return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CApplicationApartment::ScheduleWindowVisibility(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean showWindow)
{
    ECode (STDCALL CApplicationApartment::*pHandlerFunc)(
        IBinder* token, Boolean show);
    pHandlerFunc = &CApplicationApartment::HandleWindowVisibility;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(token);
    params->WriteBoolean(showWindow);

    return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CApplicationApartment::ScheduleDestroyActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean finishing,
        /* [in] */ Int32 configChanges)
{
    ECode (STDCALL CApplicationApartment::*pHandlerFunc)(
        IBinder* token, Boolean finishing,
        Int32 configChanges, Boolean getNonConfigInstance);
    pHandlerFunc = &CApplicationApartment::HandleDestroyActivity;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(token);
    params->WriteBoolean(finishing);
    params->WriteInt32(configChanges);
    params->WriteBoolean(FALSE);

    return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CApplicationApartment::ScheduleReceiver(
    /* [in] */ IIntent* intent,
    /* [in] */ IActivityInfo* info,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean sync)
{
    ECode (STDCALL CApplicationApartment::*pHandlerFunc)(
            ReceiverData* data);
    pHandlerFunc = &CApplicationApartment::HandleReceiver;

    ReceiverData* r = new ReceiverData();
    r->mIntent = intent;
    r->mInfo = (CActivityInfo*)info;
    r->mResultCode = resultCode;
    r->mResultData = data;
    r->mResultExtras = extras;
    r->mSync = sync;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)r);
    return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CApplicationApartment::ScheduleRegisteredReceiver(
    /* [in] */ IIntentReceiver* receiver,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& dataStr,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean ordered,
    /* [in] */ Boolean sticky)
{
    return receiver->PerformReceive(intent, resultCode, dataStr, extras, ordered, sticky);
}

ECode CApplicationApartment::ScheduleNewIntent(
    /* [in] */ IObjectContainer* intents,
    /* [in] */ IBinder *token)
{
    ECode (STDCALL CApplicationApartment::*pHandlerFunc)(
        NewIntentData* data);
    pHandlerFunc = &CApplicationApartment::HandleNewIntent;

    NewIntentData* data = new NewIntentData();
    AutoPtr<IObjectEnumerator> enumerator;
    intents->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext;
    while(enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IIntent> obj;
        enumerator->Current((IInterface**)&obj);
        data->mIntents.PushBack((IIntent*)obj);
    }
    data->mToken = token;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)data);
    return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CApplicationApartment::SendActivityResult(
    /* [in] */ IBinder* token,
    /* [in] */ const String& id,
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* data)
{
    if (DEBUG_RESULTS) {
        Slogger::V(TAG, StringBuffer("sendActivityResult: id=") + id +
                " req=" + requestCode + " res=" + resultCode + " data=" + (Int32)data);
    }
    AutoPtr<IObjectContainer> list;
    CParcelableObjectContainer::New((IObjectContainer**)&list);
    AutoPtr<CResultInfo> info;
    CResultInfo::NewByFriend(id, requestCode, resultCode, data, (CResultInfo**)&info);
    list->Add((IParcelable*)(CResultInfo*)info);
    return ScheduleSendResult(token, list);
}

ECode CApplicationApartment::ScheduleCreateService(
    /* [in] */ IBinder* token,
    /* [in] */ IServiceInfo* info)
{
    ECode (STDCALL CApplicationApartment::*pHandlerFunc)(
        CreateServiceData* data);
    pHandlerFunc = &CApplicationApartment::HandleCreateService;

    CreateServiceData* s = new CreateServiceData();
    s->mToken = token;
    s->mInfo = (CServiceInfo*)info;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)s);
    return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CApplicationApartment::ScheduleBindService(
    /* [in] */ IBinder* token,
    /* [in] */ IIntent* intent,
    /* [in] */ Boolean rebind)
{
    ECode (STDCALL CApplicationApartment::*pHandlerFunc)(
        BindServiceData* data);
    pHandlerFunc = &CApplicationApartment::HandleBindService;

    BindServiceData* s = new BindServiceData();
    s->mToken = token;
    s->mIntent = intent;
    s->mRebind = rebind;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)s);
    return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CApplicationApartment::ScheduleUnbindService(
    /* [in] */ IBinder* token,
    /* [in] */ IIntent* intent)
{
    ECode (STDCALL CApplicationApartment::*pHandlerFunc)(
        BindServiceData* data);
    pHandlerFunc = &CApplicationApartment::HandleUnbindService;

    BindServiceData* s = new BindServiceData();
    s->mToken = token;
    s->mIntent = intent;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)s);
    return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CApplicationApartment::ScheduleServiceArgs(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 startId,
    /* [in] */ Int32 flags,
    /* [in] */ IIntent* args)
{
    ECode (STDCALL CApplicationApartment::*pHandlerFunc)(
        ServiceArgsData* data);
    pHandlerFunc = &CApplicationApartment::HandleServiceArgs;

    ServiceArgsData* s = new ServiceArgsData();
    s->mToken = token;
    s->mStartId = startId;
    s->mFlags = flags;
    s->mArgs = args;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)s);
    return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CApplicationApartment::ScheduleStopService(
    /* [in] */ IBinder* token)
{
    ECode (STDCALL CApplicationApartment::*pHandlerFunc)(
        IBinder* token);
    pHandlerFunc = &CApplicationApartment::HandleStopService;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(token);
    return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CApplicationApartment::ScheduleConfigurationChanged(
    /* [in] */ IConfiguration* config)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplicationApartment::ScheduleActivityConfigurationChanged(
    /* [in] */ IBinder* token)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplicationApartment::DispatchCapsuleBroadcast(
    /* [in] */ Int32 cmd,
    /* [in] */ const ArrayOf<String>& capsules)
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplicationApartment::ScheduleContextCleanup(
    /* [in] */ IContext* context,
    /* [in] */ const String& who,
    /* [in] */ const String& what)
{
    ECode (STDCALL CApplicationApartment::*pHandlerFunc)(
        ContextCleanupInfo* cci);
    pHandlerFunc = &CApplicationApartment::HandleContextCleanup;

    ContextCleanupInfo* cci = new ContextCleanupInfo();
    cci->mContext = context;
    cci->mWho = who;
    cci->mWhat = what;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)cci);
    return mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CApplicationApartment::HandleBindApplication(
    /* [in] */ AppBindData* data)
{
    String wdir;
    data->mAppInfo->GetSourceDir(&wdir);
    chdir((const char*)wdir);

//    mBoundApplication = data;
//    mConfiguration = new Configuration(data.config);
//
//    // send up app name; do this *before* waiting for debugger
//    Process.setArgV0(data.processName);
//    android.ddm.DdmHandleAppName.setAppName(data.processName);
//
//    /*
//     * Before spawning a new process, reset the time zone to be the system time zone.
//     * This needs to be done because the system time zone could have changed after the
//     * the spawning of this process. Without doing this this process would have the incorrect
//     * system time zone.
//     */
//    TimeZone.setDefault(null);
//
//    /*
//     * Initialize the default locale in this process for the reasons we set the time zone.
//     */
//    Locale.setDefault(data.config.locale);
//
//    /*
//     * Update the system configuration since its preloaded and might not
//     * reflect configuration changes. The configuration object passed
//     * in AppBindData can be safely assumed to be up to date
//     */
//    Resources.getSystem().updateConfiguration(mConfiguration, null);

    data->mInfo = GetCapsuleInfoNoCheck(data->mAppInfo);

//    /**
//     * For system applications on userdebug/eng builds, log stack
//     * traces of disk and network access to dropbox for analysis.
//     */
//    if ((data.appInfo.flags &
//         (ApplicationInfo.FLAG_SYSTEM |
//          ApplicationInfo.FLAG_UPDATED_SYSTEM_APP)) != 0) {
//        StrictMode.conditionallyEnableDebugLogging();
//    }

//    /**
//     * Switch this process to density compatibility mode if needed.
//     */
//    if ((data.appInfo.flags&ApplicationInfo.FLAG_SUPPORTS_SCREEN_DENSITIES)
//            == 0) {
//        Bitmap.setDefaultDensity(DisplayMetrics.DENSITY_DEFAULT);
//    }
//
//    if (data.debugMode != IApplicationThread.DEBUG_OFF) {
//        // XXX should have option to change the port.
//        Debug.changeDebugPort(8100);
//        if (data.debugMode == IApplicationThread.DEBUG_WAIT) {
//            Slog.w(TAG, "Application " + data.info.getPackageName()
//                  + " is waiting for the debugger on port 8100...");
//
//            IActivityManager mgr = ActivityManagerNative.getDefault();
//            try {
//                mgr.showWaitingForDebugger(mAppThread, true);
//            } catch (RemoteException ex) {
//            }
//
//            Debug.waitForDebugger();
//
//            try {
//                mgr.showWaitingForDebugger(mAppThread, false);
//            } catch (RemoteException ex) {
//            }
//
//        } else {
//            Slog.w(TAG, "Application " + data.info.getPackageName()
//                  + " can be debugged on port 8100...");
//        }
//    }

    if (data->mInstrumentationName != NULL) {
        AutoPtr<CContextImpl> appContext;
        CContextImpl::NewByFriend((CContextImpl**)&appContext);
        appContext->Init(data->mInfo, NULL, this);
        AutoPtr<IInstrumentationInfo> ii;
        GetCapsuleManager()->GetInstrumentationInfo(
                data->mInstrumentationName, 0, (IInstrumentationInfo**)&ii);
        if (ii == NULL) {
            if (DEBUG) {
                String instName;
                data->mInstrumentationName->FlattenToShortString(&instName);
                Slogger::E(TAG,
                        StringBuffer("Unable to find instrumentation info for: ") + instName);
            }
            return E_RUNTIME_EXCEPTION;
        }

        CInstrumentationInfo* ci = (CInstrumentationInfo*)(IInstrumentationInfo*)ii;
        mInstrumentationAppDir = ci->mSourceDir;
        mInstrumentationAppPackage = ci->mCapsuleName;
        data->mInfo->GetAppDir(&mInstrumentedAppDir);

        AutoPtr<CApplicationInfo> instrApp;
        CApplicationInfo::NewByFriend((CApplicationInfo**)&instrApp);
        instrApp->mCapsuleName = ci->mCapsuleName;
        instrApp->mSourceDir = ci->mSourceDir;
        instrApp->mPublicSourceDir = ci->mPublicSourceDir;
        instrApp->mDataDir = ci->mDataDir;
        LoadedCap* pi = GetCapsuleInfo(instrApp, FALSE, TRUE);
        UNUSED(pi);
//        ContextImpl instrContext = new ContextImpl();
//        instrContext.init(pi, null, this);
//
//        try {
//            java.lang.ClassLoader cl = instrContext.getClassLoader();
//            mInstrumentation = (Instrumentation)
//                cl.loadClass(data.instrumentationName.getClassName()).newInstance();
//        } catch (Exception e) {
//            throw new RuntimeException(
//                "Unable to instantiate instrumentation "
//                + data.instrumentationName + ": " + e.toString(), e);
//        }
//
//        mInstrumentation.init(this, instrContext, appContext,
//                new ComponentName(ii.packageName, ii.name), data.instrumentationWatcher);
//
//        if (data.profileFile != null && !ii.handleProfiling) {
//            data.handlingProfiling = true;
//            File file = new File(data.profileFile);
//            file.getParentFile().mkdirs();
//            Debug.startMethodTracing(file.toString(), 8 * 1024 * 1024);
//        }
//
//        try {
//            mInstrumentation.onCreate(data.instrumentationArgs);
//        }
//        catch (Exception e) {
//            throw new RuntimeException(
//                "Exception thrown in onCreate() of "
//                + data.instrumentationName + ": " + e.toString(), e);
//        }

    } else {
        CDefaultInstrumentation::New((IInstrumentation**)&mInstrumentation);
    }

    // If the app is being launched for full backup or restore, bring it up in
    // a restricted environment with the base application class.
    AutoPtr<IApplication> app;
    data->mInfo->MakeApplication(FALSE /* data.restrictedBackupMode */, NULL,
            (IApplication**)&app);
    mInitialApplication = app;

    if (data->mProviders != NULL) {
        InstallContentProviders((IContext*)(IApplication*)app, data->mProviders);
//        // For process that contain content providers, we want to
//        // ensure that the JIT is enabled "at some point".
//        mH.sendEmptyMessageDelayed(H.ENABLE_JIT, 10*1000);
    }

    ECode ec = mInstrumentation->CallApplicationOnCreate(app);
    if (FAILED(ec)) {
        if (DEBUG) {
//            Slog.e(TAG,
//                "Unable to create application " + app.getClass().getName()
//                + ": " + e.toString(), e);
        }
        return E_RUNTIME_EXCEPTION;
    }

    return NOERROR;
}

ECode CApplicationApartment::HandleLaunchActivity(
    /* [in] */ ActivityClientRecord* r,
    /* [in] */ IIntent* customIntent)
{
    r->mCapsuleInfo = GetCapsuleInfoNoCheck(
            r->mActivityInfo->mApplicationInfo);

//    // If we are getting ready to gc after going to the background, well
//    // we are back active so skip it.
//    unscheduleGcIdler();

    if (localLOGV) {
        String acrDes;
        r->GetDescription(&acrDes);
        Slogger::V(TAG, StringBuffer("Handling launch of ") + acrDes);
    }
    AutoPtr<IActivity> a;
    PerformLaunchActivity(r, customIntent, (IActivity**)&a);

    if (a != NULL) {
//        r.createdConfig = new Configuration(mConfiguration);
//        Bundle oldState = r.state;
        if (localLOGV) {
            Slogger::V(TAG, StringBuffer("Handling resume of activity."));
        }

        HandleResumeActivity(r->mToken, FALSE, r->mIsForward);

        Boolean finished;
        if ((r->mActivity->IsFinished(&finished), !finished) && r->mStartsNotResumed) {
            // The activity manager actually wants this one to start out
            // paused, because it needs to be visible but isn't in the
            // foreground.  We accomplish this by going through the
            // normal startup (because activities expect to go through
            // onResume() the first time they run, before their window
            // is displayed), and then pausing it.  However, in this case
            // we do -not- need to do the full pause cycle (of freezing
            // and such) because the activity manager assumes it can just
            // retain the current state it has.
            r->mActivity->SetCalled(FALSE);
            mInstrumentation->CallActivityOnPause(r->mActivity);
//            // We need to keep around the original state, in case
//            // we need to be created again.
//            r.state = oldState;
//            if (!r.activity.mCalled) {
//                throw new SuperNotCalledException(
//                    "Activity " + r.intent.getComponent().toShortString() +
//                    " did not call through to super.onPause()");
//            }

            r->mPaused = TRUE;
        }
    }
    else {
//        // If there was an error, for any reason, tell the activity
//        // manager to stop us.
//        try {
//            ActivityManagerNative.getDefault()
//                .finishActivity(r.token, Activity.RESULT_CANCELED, null);
//        } catch (RemoteException ex) {
//        }
    }
    return NOERROR;
}

ECode CApplicationApartment::PerformLaunchActivity(
    /* [in] */ ActivityClientRecord* r,
    /* [in] */ IIntent* customIntent,
    /* [out] */ IActivity** activity)
{
    ECode ec = NOERROR;
    AutoPtr<CActivityInfo> aInfo = r->mActivityInfo;
    if (r->mCapsuleInfo == NULL) {
        assert(0);
//        r->mCapsuleInfo = GetCapsuleInfo(aInfo->mApplicationInfo,
//                Context.CONTEXT_INCLUDE_CODE);
    }

    AutoPtr<IComponentName> component;
    r->mIntent->GetComponent((IComponentName**)&component);
    if (component == NULL) {
        assert(0);
//        component = r.intent.resolveActivity(
//            mInitialApplication.getPackageManager());
//        r.intent.setComponent(component);
    }

//    if (r.activityInfo.targetActivity != null) {
//        component = new ComponentName(r.activityInfo.packageName,
//                r.activityInfo.targetActivity);
//    }

    String capsuleName;
    component->GetCapsuleName(&capsuleName);
    String className;
    component->GetClassName(&className);

    assert(!capsuleName.IsNull());
    assert(!className.IsNull());

    char path[256];
    strcpy(path, "/data/data/com.elastos.runtime/elastos/");
    strcat(path, capsuleName);
    strcat(path, "/");
    strcat(path, capsuleName);
    strcat(path, ".eco");

    AutoPtr<IModuleInfo> moduleInfo;
    ec = CReflector::AcquireModuleInfo(path, (IModuleInfo**)&moduleInfo);
    if (FAILED(ec)) {
        if (localLOGV) {
            Slogger::E(TAG, StringBuffer("Cann't Find the path is ") + path);
        }
    }

    AutoPtr<IClassInfo> classInfo;
    moduleInfo->GetClassInfo(className, (IClassInfo**)&classInfo);
    AutoPtr<IInterface> object;
    classInfo->CreateObject((IInterface**)&object);
    AutoPtr<IActivity> a;
    a = (IActivity*)object->Probe(EIID_IActivity);

//    try {
    AutoPtr<IApplication> app;// = r.packageInfo.makeApplication(false, mInstrumentation);
//
//    if (localLOGV) Slog.v(TAG, "Performing launch of " + r);
//    if (localLOGV) Slog.v(
//            TAG, r + ": app=" + app
//            + ", appName=" + app.getPackageName()
//            + ", pkg=" + r.packageInfo.getPackageName()
//            + ", comp=" + r.intent.getComponent().toShortString()
//            + ", dir=" + r.packageInfo.getAppDir());

    if (a != NULL) {
        AutoPtr<CContextImpl> appContext;
        CContextImpl::NewByFriend((CContextImpl**)&appContext);
        appContext->Init(r->mCapsuleInfo, r->mToken, this);
        appContext->SetOuterContext((IContext*)(IActivity*)a);
        AutoPtr<ICharSequence> title;
        r->mActivityInfo->LoadLabel(NULL/*appContext.getPackageManager()*/, (ICharSequence**)&title);
        AutoPtr<IConfiguration> config;// = new Configuration(mConfiguration);
//        if (DEBUG_CONFIGURATION) Slog.v(TAG, "Launching activity "
//                + r.activityInfo.name + " with config " + config);
//        activity.attach(appContext, this, getInstrumentation(), r.token,
//                r.ident, app, r.intent, r.activityInfo, title, r.parent,
//                r.embeddedID, r.lastNonConfigurationInstance,
//                r.lastNonConfigurationChildInstances, config);
        a->AttachEx((IContext*)(CContextImpl*)appContext,
                this, mInstrumentation, r->mToken, r->mIdent, app, r->mIntent,
                r->mActivityInfo, title, r->mParent, r->mEmbeddedID,
                r->mLastNonConfigurationInstance,
                r->mLastNonConfigurationChildInstances, config);

//        if (customIntent != null) {
//            activity.mIntent = customIntent;
//        }
//        r.lastNonConfigurationInstance = null;
//        r.lastNonConfigurationChildInstances = null;
//        activity.mStartedActivity = false;
//        int theme = r.activityInfo.getThemeResource();
//        if (theme != 0) {
//            activity.setTheme(theme);
//        }

        a->SetCalled(FALSE);
        mInstrumentation->CallActivityOnCreate(a, r->mState);
        Boolean called;
        a->IsCalled(&called);
        if (!called) {
//            throw new SuperNotCalledException(
//                "Activity " + r.intent.getComponent().toShortString() +
//                " did not call through to super.onCreate()");
        }
        r->mActivity = a;
        r->mStopped = TRUE;
        Boolean finished;
        if (r->mActivity->IsFinished(&finished), !finished) {
            a->PerformStart();
            r->mStopped = FALSE;
        }
        r->mActivity->IsFinished(&finished);
        if (!finished) {
            if (r->mState != NULL) {
//                mInstrumentation.callActivityOnRestoreInstanceState(activity, r.state);
            }
        }
        r->mActivity->IsFinished(&finished);
        if (!finished) {
            a->SetCalled(FALSE);
            mInstrumentation->CallActivityOnPostCreate(a, r->mState);
//            if (!activity.mCalled) {
//                throw new SuperNotCalledException(
//                    "Activity " + r.intent.getComponent().toShortString() +
//                    " did not call through to super.onPostCreate()");
//            }
        }
    }
    r->mPaused = TRUE;
    mActivities[r->mToken] = r;

//    } catch (SuperNotCalledException e) {
//        throw e;
//
//    } catch (Exception e) {
//        if (!mInstrumentation.onException(activity, e)) {
//            throw new RuntimeException(
//                "Unable to start activity " + component
//                + ": " + e.toString(), e);
//        }
//    }

    *activity = a;
    if (*activity != NULL) (*activity)->AddRef();

    return NOERROR;
}

void CApplicationApartment::DeliverNewIntents(
    /* [in] */ ActivityClientRecord* r,
    /* [in] */ List<AutoPtr<IIntent> >* intents)
{
    List<AutoPtr<IIntent> >::Iterator it1 = intents->Begin();
    List<AutoPtr<IIntent> >::Iterator it2 = intents->End();
    for (; it1 != it2; ++it1) {
        AutoPtr<IIntent> intent = *it1;
//        intent.setExtrasClassLoader(r.activity.getClassLoader());
        mInstrumentation->CallActivityOnNewIntent(r->mActivity, intent);
    }
}

//ECode CApplicationApartment::PerformNewIntent(
//    /* [in] */ IIntent *intent,
//    /* [in] */ IBinder *token)
//{
//    ActivityClientRecord* r = mActivities[token];
//    if (r->mActivity != NULL) {
//        return mInstrumentation->CallActivityOnNewIntent(r->mActivity, intent);
//    }
//    else {
//        return E_DOES_NOT_EXIST;
//    }
//}

ECode CApplicationApartment::DeliverResults(
    /* [in] */ ActivityClientRecord* r,
    /* [in] */ List<AutoPtr<CResultInfo> >* results)
{
    List<AutoPtr<CResultInfo> >::Iterator it1 = results->Begin();
    List<AutoPtr<CResultInfo> >::Iterator it2 = results->End();
    for (; it1 != it2; ++it1) {
//        try {
        AutoPtr<CResultInfo> ri = *it1;
//        if (ri.mData != null) {
//            ri.mData.setExtrasClassLoader(r.activity.getClassLoader());
//        }
//        if (DEBUG_RESULTS) Slog.v(TAG,
//                "Delivering result to activity " + r + " : " + ri);
        r->mActivity->DispatchActivityResult(ri->mResultWho,
                ri->mRequestCode, ri->mResultCode, ri->mData);
//        } catch (Exception e) {
//            if (!mInstrumentation.onException(r.activity, e)) {
//                throw new RuntimeException(
//                        "Failure delivering result " + ri + " to activity "
//                        + r.intent.getComponent().toShortString()
//                        + ": " + e.toString(), e);
//            }
//        }
    }

    return NOERROR;
}

ECode CApplicationApartment::HandleSendResult(
    /* [in] */ ResultData* res)
{
    ActivityClientRecord* r = mActivities[res->mToken];
//    if (DEBUG_RESULTS) Slog.v(TAG, "Handling send result to " + r);
    if (r != NULL) {
        Boolean resumed = !r->mPaused;
//        if (!r->mActivity.mFinished && r.activity.mDecor != null
//                && r.hideForNow && resumed) {
//            // We had hidden the activity because it started another
//            // one...  we have gotten a result back and we are not
//            // paused, so make sure our window is visible.
//            updateVisibility(r, true);
//        }
        if (resumed) {
//            try {
                // Now we are idle.
            r->mActivity->SetCalled(FALSE);
            mInstrumentation->CallActivityOnPause(r->mActivity);
            Boolean called;
            if (r->mActivity->IsCalled(&called), !called) {
//                    throw new SuperNotCalledException(
//                        "Activity " + r.intent.getComponent().toShortString()
//                        + " did not call through to super.onPause()");
            }
//            } catch (SuperNotCalledException e) {
//                throw e;
//            } catch (Exception e) {
//                if (!mInstrumentation.onException(r.activity, e)) {
//                    throw new RuntimeException(
//                            "Unable to pause activity "
//                            + r.intent.getComponent().toShortString()
//                            + ": " + e.toString(), e);
//                }
//            }
        }
        DeliverResults(r, &res->mResults);
        if (resumed) {
            mInstrumentation->CallActivityOnResume(r->mActivity);
        }
    }

    return NOERROR;
}

ECode CApplicationApartment::HandleResumeActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean clearHide,
    /* [in] */ Boolean isForward)
{
//    // If we are getting ready to gc after going to the background, well
//    // we are back active so skip it.
//    unscheduleGcIdler();

    ActivityClientRecord* r = PerformResumeActivity(token, clearHide);
    if (r != NULL) {
        AutoPtr<IActivity> a = r->mActivity;

        if (localLOGV) {
            String rdes;
            r->GetDescription(&rdes);
            Slogger::V(TAG, StringBuffer("Resume ") + rdes + " started activity: "
                    /*+ a.mStartedActivity*/ + ", hideForNow: " + r->mHideForNow
                    + ", finished: " /*+ a.mFinished*/);
        }

        Int32 forwardBit = isForward ?
                WindowManagerLayoutParams_SOFT_INPUT_IS_FORWARD_NAVIGATION : 0;

        // If the window hasn't yet been added to the window manager,
        // and this guy didn't finish itself or start another activity,
        // then go ahead and add the window.
        Boolean willBeVisible = FALSE;
        a->IsStartedActivity(&willBeVisible);
        if (!willBeVisible) {
                AutoPtr<IActivityManager> service;
                ActivityManagerNative::GetDefault((IActivityManager**)&service);
                AutoPtr<IBinder> t;
                a->GetActivityToken((IBinder**)&t);
                service->WillActivityBeVisible(t.Get(), &willBeVisible);
        }
        Boolean finished = FALSE;
        a->IsFinished(&finished);
        if (r->mWindow == NULL && !finished && willBeVisible) {
            r->mActivity->GetWindowEx((IWindow**)&r->mWindow);
            AutoPtr<IView> decor;
            r->mWindow->GetDecorView((IView**)&decor);
            decor->SetVisibility(View_INVISIBLE);
            AutoPtr<IViewManager> wm;
            a->GetWindowManagerEx((IWindowManager**)&wm);
            AutoPtr<IWindowManagerLayoutParams> l;
            r->mWindow->GetAttributes((IWindowManagerLayoutParams**)&l);
//	            WindowManager.LayoutParams l = r.window.getAttributes();
            a->SetDecorView(decor);
            ((CWindowManagerLayoutParams*)l.Get())->mType = WindowManagerLayoutParams_TYPE_BASE_APPLICATION;
//	            l.type = WindowManager.LayoutParams.TYPE_BASE_APPLICATION;
            ((CWindowManagerLayoutParams*)l.Get())->mSoftInputMode |= forwardBit;
            Boolean visibleFromClient = FALSE;
            a->IsVisibleFromClient(&visibleFromClient);
            if (visibleFromClient) {
                a->SetWindowAdded(TRUE);
                wm->AddViewEx5(decor, l);
            }

        // If the window has already been added, but during resume
        // we started another activity, then don't yet make the
        // window visisble.
        } else if (!willBeVisible) {
//	            if (localLOGV) Slog.v(
//	                TAG, "Launch " + r + " mStartedActivity set");
            r->mHideForNow = TRUE;
        }

        // The window is now visible if it has been added, we are not
        // simply finishing, and we are not starting another activity.
        r->mActivity->IsFinished(&finished);
        AutoPtr<IView> decor;
        r->mActivity->GetDecorView((IView**)&decor);
        if (!finished && willBeVisible
                  && decor != NULL && !r->mHideForNow) {
//	            if (r.newConfig != null) {
//	                if (DEBUG_CONFIGURATION) Slog.v(TAG, "Resuming activity "
//	                            + r.activityInfo.name + " with newConfig " + r.newConfig);
//	                performConfigurationChanged(r.activity, r.newConfig);
//	                r.newConfig = null;
//	            }
//	            if (localLOGV) Slog.v(TAG, "Resuming " + r + " with isForward="
//	                    + isForward);
//	            WindowManager.LayoutParams l = r.window.getAttributes();
//	            if ((l.softInputMode
//	                    & WindowManager.LayoutParams.SOFT_INPUT_IS_FORWARD_NAVIGATION)
//	                    != forwardBit) {
//	                l.softInputMode = (l.softInputMode
//	                        & (~WindowManager.LayoutParams.SOFT_INPUT_IS_FORWARD_NAVIGATION))
//	                        | forwardBit;
//	                if (r.activity.mVisibleFromClient) {
//	                    ViewManager wm = a.getWindowManager();
//	                    View decor = r.window.getDecorView();
//	                    wm.updateViewLayout(decor, l);
//	                }
//	            }
//	            r.activity.mVisibleFromServer = true;
//	            mNumVisibleActivities++;
//	            if (r.activity.mVisibleFromClient) {
                r->mActivity->MakeVisible();
//	            }
        }

//	        r.nextIdle = mNewActivities;
//	        mNewActivities = r;
//	        if (localLOGV) Slog.v(
//	            TAG, "Scheduling idle handler for " + r);
//	        Looper.myQueue().addIdleHandler(new Idler());

    } else {
        // If an exception was thrown when trying to resume, then
        // just end this activity.
//	        try {
//	            ActivityManagerNative.getDefault()
//	                .finishActivity(token, Activity.RESULT_CANCELED, null);
//	        } catch (RemoteException ex) {
//	        }
    }
    return NOERROR;
}

CApplicationApartment::ActivityClientRecord*
CApplicationApartment::PerformResumeActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean clearHide)
{
    ActivityClientRecord* r = mActivities[token];
//    if (localLOGV) Slog.v(TAG, "Performing resume of " + r
//            + " finished=" + r.activity.mFinished);
    Boolean finished;
    if (r != NULL && (r->mActivity->IsFinished(&finished), !finished)) {
//        if (clearHide) {
//            r.hideForNow = false;
//            r.activity.mStartedActivity = false;
//        }
//        try {
        if (r->mPendingIntents != NULL) {
            DeliverNewIntents(r, r->mPendingIntents);
            r->mPendingIntents->Clear();
            r->mPendingIntents = NULL;
        }
        if (r->mPendingResults != NULL) {
            DeliverResults(r, r->mPendingResults);
            r->mPendingResults->Clear();
            r->mPendingResults = NULL;
        }
        r->mActivity->PerformResume();

//        EventLog.writeEvent(LOG_ON_RESUME_CALLED,
//                r.activity.getComponentName().getClassName());

        r->mPaused = FALSE;
        r->mStopped = FALSE;
        r->mState = NULL;
//        } catch (Exception e) {
//            if (!mInstrumentation.onException(r.activity, e)) {
//                throw new RuntimeException(
//                    "Unable to resume activity "
//                    + r.intent.getComponent().toShortString()
//                    + ": " + e.toString(), e);
//            }
//        }
    }
    return r;
}

ECode CApplicationApartment::HandlePauseActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean finished,
    /* [in] */ Boolean userLeaving,
    /* [in] */ Int32 configChanges)
{
    ActivityClientRecord* r = mActivities[token];
    if (r != NULL) {
        //Slog.v(TAG, "userLeaving=" + userLeaving + " handling pause of " + r);
        if (userLeaving) {
            PerformUserLeavingActivity(r);
        }

//        r.activity.mConfigChangeFlags |= configChanges;
        AutoPtr<IBundle> state;
        PerformPauseActivity(token, finished, TRUE, (IBundle**)&state);

//        // Make sure any pending writes are now committed.
//        QueuedWork.waitToFinish();

        // Tell the activity manager we have paused.
        AutoPtr<IActivityManager> amService;
        ActivityManagerNative::GetDefault((IActivityManager**)&amService);
        amService->ActivityPaused(token, state);
    }
    return NOERROR;
}

ECode CApplicationApartment::PerformUserLeavingActivity(
    /* [in] */ ActivityClientRecord* r)
{
    return mInstrumentation->CallActivityOnUserLeaving(r->mActivity);
}

ECode CApplicationApartment::PerformPauseActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean finished,
    /* [in] */ Boolean saveState,
    /* [out] */ IBundle** state)
{
    ActivityClientRecord* r = mActivities[token];
    if (r != NULL) {
        return PerformPauseActivity(r, finished, saveState, state);
    }
    else {
        *state = NULL;
        return NOERROR;
    }
}

ECode CApplicationApartment::PerformPauseActivity(
    /* [in] */ ActivityClientRecord* r,
    /* [in] */ Boolean finished,
    /* [in] */ Boolean saveState,
    /* [out] */ IBundle** state)
{
    Boolean isFinished;
    if (r->mPaused) {
        if (r->mActivity->IsFinished(&isFinished), isFinished) {
            // If we are finishing, we won't call onResume() in certain cases.
            // So here we likewise don't want to call onPause() if the activity
            // isn't resumed.
            *state = NULL;
            return NOERROR;
        }
//        RuntimeException e = new RuntimeException(
//                "Performing pause of activity that is not resumed: "
//                + r.intent.getComponent().toShortString());
//        Slog.e(TAG, e.getMessage(), e);
    }
    AutoPtr<CBundle> st;
    if (finished) {
        r->mActivity->SetFinished(TRUE);
    }
//    try {
    // Next have the activity save its current state and managed dialogs...
    if ((r->mActivity->IsFinished(&isFinished), !isFinished) && saveState) {
        CBundle::NewByFriend((CBundle**)&st);
        mInstrumentation->CallActivityOnSaveInstanceState(
                r->mActivity, (IBundle*)(CBundle*)st);
        r->mState = st;
    }
    // Now we are idle.
    r->mActivity->SetCalled(FALSE);
    mInstrumentation->CallActivityOnPause(r->mActivity);
//    EventLog.writeEvent(LOG_ON_PAUSE_CALLED, r.activity.getComponentName().getClassName());
    Boolean called;
    if (r->mActivity->IsCalled(&called), !called) {
//        throw new SuperNotCalledException(
//            "Activity " + r.intent.getComponent().toShortString() +
//            " did not call through to super.onPause()");
    }

//    } catch (SuperNotCalledException e) {
//        throw e;
//
//    } catch (Exception e) {
//        if (!mInstrumentation.onException(r.activity, e)) {
//            throw new RuntimeException(
//                    "Unable to pause activity "
//                    + r.intent.getComponent().toShortString()
//                    + ": " + e.toString(), e);
//        }
//    }
    r->mPaused = TRUE;

//    // Notify any outstanding on paused listeners
//    ArrayList<OnActivityPausedListener> listeners;
//    synchronized (mOnPauseListeners) {
//        listeners = mOnPauseListeners.remove(r.activity);
//    }
//    int size = (listeners != null ? listeners.size() : 0);
//    for (int i = 0; i < size; i++) {
//        listeners.get(i).onPaused(r.activity);
//    }

    *state = st;
    if (*state != NULL) {
        (*state)->AddRef();
    }

    return NOERROR;
}

ECode CApplicationApartment::HandleStopActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean show,
    /* [in] */ Int32 configChanges)
{
    ECode ec;

    ActivityClientRecord* r = mActivities[token];
//    r.activity.mConfigChangeFlags |= configChanges;

    StopInfo* info = new StopInfo();
    PerformStopActivityInner(r, info, show);

//    if (localLOGV) Slog.v(
//        TAG, "Finishing stop of " + r + ": show=" + show
//        + " win=" + r.window);

//    updateVisibility(r, show);

    // Tell activity manager we have been stopped.c
    AutoPtr<IActivityManager> amService;
    ActivityManagerNative::GetDefault((IActivityManager**)&amService);
    ec = amService->ActivityStopped(r->mToken, NULL /*info.thumbnail*/, String(NULL) /*info.description*/);

    return NOERROR;
}

ECode CApplicationApartment::PerformStopActivity(
    /* [in] */ IBinder* token)
{
    ActivityClientRecord* r = mActivities[token];
    return PerformStopActivityInner(r, NULL, FALSE);
}

ECode CApplicationApartment::PerformStopActivityInner(
    /* [in] */ ActivityClientRecord* r,
    /* [in] */ StopInfo* info,
    /* [in] */ Boolean keepShown)
{
//    if (localLOGV) Slog.v(TAG, "Performing stop of " + r);
    if (r != NULL) {
        if (!keepShown && r->mStopped) {
            Boolean finished;
            if (r->mActivity->IsFinished(&finished), finished) {
                // If we are finishing, we won't call onResume() in certain
                // cases.  So here we likewise don't want to call onStop()
                // if the activity isn't resumed.
                return NOERROR;
            }
//            RuntimeException e = new RuntimeException(
//                    "Performing stop of activity that is not resumed: "
//                    + r.intent.getComponent().toShortString());
//            Slog.e(TAG, e.getMessage(), e);
        }

        if (info != NULL) {
//            try {
//                // First create a thumbnail for the activity...
//                info.thumbnail = createThumbnailBitmap(r);
//                info.description = r.activity.onCreateDescription();
//            } catch (Exception e) {
//                if (!mInstrumentation.onException(r.activity, e)) {
//                    throw new RuntimeException(
//                            "Unable to save state of activity "
//                            + r.intent.getComponent().toShortString()
//                            + ": " + e.toString(), e);
//                }
//            }
        }

        if (!keepShown) {
//            try {
                // Now we are idle.
                r->mActivity->PerformStop();
//            } catch (Exception e) {
//                if (!mInstrumentation.onException(r.activity, e)) {
//                    throw new RuntimeException(
//                            "Unable to stop activity "
//                            + r.intent.getComponent().toShortString()
//                            + ": " + e.toString(), e);
//                }
//            }
            r->mStopped = TRUE;
        }

        r->mPaused = TRUE;
    }
    return NOERROR;
}

ECode CApplicationApartment::HandleWindowVisibility(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean show)
{
    ActivityClientRecord* r = mActivities[token];
    if (!show && !r->mStopped) {
        PerformStopActivityInner(r, NULL, show);
    } else if (show && r->mStopped) {
        r->mActivity->PerformRestart();
        r->mStopped = FALSE;
    }
//    if (r.activity.mDecor != null) {
//        if (Config.LOGV) Slog.v(
//            TAG, "Handle window " + r + " visibility: " + show);
//        updateVisibility(r, show);
//    }
    return NOERROR;
}

CApplicationApartment::ActivityClientRecord*
CApplicationApartment::PerformDestroyActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean finishing)
{
    return PerformDestroyActivity(token, finishing, 0, FALSE);
}

CApplicationApartment::ActivityClientRecord*
CApplicationApartment::PerformDestroyActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean finishing,
    /* [in] */ Int32 configChanges,
    /* [in] */ Boolean getNonConfigInstance)
{
    ActivityClientRecord* r = mActivities[token];
//    if (localLOGV) Slog.v(TAG, "Performing finish of " + r);
    if (r != NULL) {
//        r.activity.mConfigChangeFlags |= configChanges;
        if (finishing) {
            r->mActivity->SetFinished(TRUE);
        }
        if (!r->mPaused) {
//            try {
            r->mActivity->SetCalled(FALSE);
            mInstrumentation->CallActivityOnPause(r->mActivity);
//            EventLog.writeEvent(LOG_ON_PAUSE_CALLED,
//                    r.activity.getComponentName().getClassName());
//            if (!r.activity.mCalled) {
//                throw new SuperNotCalledException(
//                    "Activity " + safeToComponentShortString(r.intent)
//                    + " did not call through to super.onPause()");
//            }
//            } catch (SuperNotCalledException e) {
//                throw e;
//            } catch (Exception e) {
//                if (!mInstrumentation.onException(r.activity, e)) {
//                    throw new RuntimeException(
//                            "Unable to pause activity "
//                            + safeToComponentShortString(r.intent)
//                            + ": " + e.toString(), e);
//                }
//            }
            r->mPaused = TRUE;
        }
        if (!r->mStopped) {
//            try {
            r->mActivity->PerformStop();
//            } catch (SuperNotCalledException e) {
//                throw e;
//            } catch (Exception e) {
//                if (!mInstrumentation.onException(r.activity, e)) {
//                    throw new RuntimeException(
//                            "Unable to stop activity "
//                            + safeToComponentShortString(r.intent)
//                            + ": " + e.toString(), e);
//                }
//            }
            r->mStopped = TRUE;
        }
//        if (getNonConfigInstance) {
//            try {
//                r.lastNonConfigurationInstance
//                        = r.activity.onRetainNonConfigurationInstance();
//            } catch (Exception e) {
//                if (!mInstrumentation.onException(r.activity, e)) {
//                    throw new RuntimeException(
//                            "Unable to retain activity "
//                            + r.intent.getComponent().toShortString()
//                            + ": " + e.toString(), e);
//                }
//            }
//            try {
//                r.lastNonConfigurationChildInstances
//                        = r.activity.onRetainNonConfigurationChildInstances();
//            } catch (Exception e) {
//                if (!mInstrumentation.onException(r.activity, e)) {
//                    throw new RuntimeException(
//                            "Unable to retain child activities "
//                            + safeToComponentShortString(r.intent)
//                            + ": " + e.toString(), e);
//                }
//            }
//
//        }
//        try {
        r->mActivity->SetCalled(FALSE);
        r->mActivity->Destroy();
//        if (!r.activity.mCalled) {
//            throw new SuperNotCalledException(
//                "Activity " + safeToComponentShortString(r.intent) +
//                " did not call through to super.onDestroy()");
//        }
//        if (r.window != null) {
//            r.window.closeAllPanels();
//        }
//        } catch (SuperNotCalledException e) {
//            throw e;
//        } catch (Exception e) {
//            if (!mInstrumentation.onException(r.activity, e)) {
//                throw new RuntimeException(
//                        "Unable to destroy activity " + safeToComponentShortString(r.intent)
//                        + ": " + e.toString(), e);
//            }
//        }
    }
    mActivities[token] = NULL;

    return r;
}

ECode CApplicationApartment::HandleDestroyActivity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean finishing,
    /* [in] */ Int32 configChanges,
    /* [in] */ Boolean getNonConfigInstance)
{
    ActivityClientRecord* r = PerformDestroyActivity(token, finishing,
            configChanges, getNonConfigInstance);
    UNUSED(r);
//    if (r != NULL) {
//        WindowManager wm = r.activity.getWindowManager();
//        View v = r.activity.mDecor;
//        if (v != null) {
//            if (r.activity.mVisibleFromServer) {
//                mNumVisibleActivities--;
//            }
//            IBinder wtoken = v.getWindowToken();
//            if (r.activity.mWindowAdded) {
//                wm.removeViewImmediate(v);
//            }
//            if (wtoken != null) {
//                WindowManagerImpl.getDefault().closeAll(wtoken,
//                        r.activity.getClass().getName(), "Activity");
//            }
//            r.activity.mDecor = null;
//        }
//        WindowManagerImpl.getDefault().closeAll(token,
//                r.activity.getClass().getName(), "Activity");
//
//        // Mocked out contexts won't be participating in the normal
//        // process lifecycle, but if we're running with a proper
//        // ApplicationContext we need to have it tear down things
//        // cleanly.
//        Context c = r.activity.getBaseContext();
//        if (c instanceof ContextImpl) {
//            ((ContextImpl) c).scheduleFinalCleanup(
//                    r.activity.getClass().getName(), "Activity");
//        }
//    }
    if (finishing) {
        AutoPtr<IActivityManager> amService;
        ActivityManagerNative::GetDefault((IActivityManager**)&amService);
        ECode ec = amService->ActivityDestroyed(token);
        UNUSED(ec);
    }
    return NOERROR;
}

ECode CApplicationApartment::HandleRelaunchActivity(
    /* [in] */ ActivityClientRecord* tmp,
    /* [in] */ Int32 configChanges)
{
    return E_NOT_IMPLEMENTED;
//    // If we are getting ready to gc after going to the background, well
//        // we are back active so skip it.
//        unscheduleGcIdler();
//
//        Configuration changedConfig = null;
//
//        if (DEBUG_CONFIGURATION) Slog.v(TAG, "Relaunching activity "
//                + tmp.token + " with configChanges=0x"
//                + Integer.toHexString(configChanges));
//
//        // First: make sure we have the most recent configuration and most
//        // recent version of the activity, or skip it if some previous call
//        // had taken a more recent version.
//        synchronized (mPackages) {
//            int N = mRelaunchingActivities.size();
//            IBinder token = tmp.token;
//            tmp = null;
//            for (int i=0; i<N; i++) {
//                ActivityClientRecord r = mRelaunchingActivities.get(i);
//                if (r.token == token) {
//                    tmp = r;
//                    mRelaunchingActivities.remove(i);
//                    i--;
//                    N--;
//                }
//            }
//
//            if (tmp == null) {
//                if (DEBUG_CONFIGURATION) Slog.v(TAG, "Abort, activity not relaunching!");
//                return;
//            }
//
//            if (mPendingConfiguration != null) {
//                changedConfig = mPendingConfiguration;
//                mPendingConfiguration = null;
//            }
//        }
//
//        if (tmp.createdConfig != null) {
//            // If the activity manager is passing us its current config,
//            // assume that is really what we want regardless of what we
//            // may have pending.
//            if (mConfiguration == null
//                    || (tmp.createdConfig.isOtherSeqNewer(mConfiguration)
//                            && mConfiguration.diff(tmp.createdConfig) != 0)) {
//                if (changedConfig == null
//                        || tmp.createdConfig.isOtherSeqNewer(changedConfig)) {
//                    changedConfig = tmp.createdConfig;
//                }
//            }
//        }
//
//        if (DEBUG_CONFIGURATION) Slog.v(TAG, "Relaunching activity "
//                + tmp.token + ": changedConfig=" + changedConfig);
//
//        // If there was a pending configuration change, execute it first.
//        if (changedConfig != null) {
//            handleConfigurationChanged(changedConfig);
//        }
//
//        ActivityClientRecord r = mActivities.get(tmp.token);
//        if (DEBUG_CONFIGURATION) Slog.v(TAG, "Handling relaunch of " + r);
//        if (r == null) {
//            return;
//        }
//
//        r.activity.mConfigChangeFlags |= configChanges;
//        Intent currentIntent = r.activity.mIntent;
//
//        Bundle savedState = null;
//        if (!r.paused) {
//            savedState = performPauseActivity(r.token, false, true);
//        }
//
//        handleDestroyActivity(r.token, false, configChanges, true);
//
//        r.activity = null;
//        r.window = null;
//        r.hideForNow = false;
//        r.nextIdle = null;
//        // Merge any pending results and pending intents; don't just replace them
//        if (tmp.pendingResults != null) {
//            if (r.pendingResults == null) {
//                r.pendingResults = tmp.pendingResults;
//            } else {
//                r.pendingResults.addAll(tmp.pendingResults);
//            }
//        }
//        if (tmp.pendingIntents != null) {
//            if (r.pendingIntents == null) {
//                r.pendingIntents = tmp.pendingIntents;
//            } else {
//                r.pendingIntents.addAll(tmp.pendingIntents);
//            }
//        }
//        r.startsNotResumed = tmp.startsNotResumed;
//        if (savedState != null) {
//            r.state = savedState;
//        }
//
//        handleLaunchActivity(r, currentIntent);
}

void CApplicationApartment::PerformNewIntents(
    /* [in] */ IBinder* token,
    /* [in] */ List<AutoPtr<IIntent> >* intents)
{
    ActivityClientRecord* r = mActivities[token];
    if (r != NULL) {
        Boolean resumed = !r->mPaused;
        if (resumed) {
            mInstrumentation->CallActivityOnPause(r->mActivity);
        }
        DeliverNewIntents(r, intents);
        if (resumed) {
            mInstrumentation->CallActivityOnResume(r->mActivity);
        }
    }
}

ECode CApplicationApartment::HandleNewIntent(
    /* [in] */ NewIntentData* data)
{
    PerformNewIntents(data->mToken, &data->mIntents);
    return NOERROR;
}

ECode CApplicationApartment::HandleReceiver(
    /* [in] */ ReceiverData* data)
{
//    // If we are getting ready to gc after going to the background, well
//    // we are back active so skip it.
//    unscheduleGcIdler();

    AutoPtr<IComponentName> component;
    String capsuleName, className;

    data->mIntent->GetComponent((IComponentName**)&component);
    component->GetCapsuleName(&capsuleName);
    component->GetClassName(&className);

    LoadedCap* capsuleInfo = GetCapsuleInfoNoCheck(
            data->mInfo->mApplicationInfo);

    AutoPtr<IActivityManager> mgr;
    ActivityManagerNative::GetDefault((IActivityManager**)&mgr);

    AutoPtr<IBroadcastReceiver> receiver;
//    try {
    String path = capsuleName + ".eco";

    AutoPtr<IModuleInfo> moduleInfo;
    AutoPtr<IClassInfo> classInfo;
    AutoPtr<IInterface> object;
    CReflector::AcquireModuleInfo(path, (IModuleInfo**)&moduleInfo);
    moduleInfo->GetClassInfo(className, (IClassInfo**)&classInfo);
    classInfo->CreateObject((IInterface**)&object);

    receiver = (IBroadcastReceiver*)object->Probe(EIID_IBroadcastReceiver);
//    } catch (Exception e) {
//        try {
//            if (DEBUG_BROADCAST) Slog.i(TAG,
//                    "Finishing failed broadcast to " + data.intent.getComponent());
//            mgr.finishReceiver(mAppThread.asBinder(), data.resultCode,
//                               data.resultData, data.resultExtras, data.resultAbort);
//        } catch (RemoteException ex) {
//        }
//        throw new RuntimeException(
//            "Unable to instantiate receiver " + component
//            + ": " + e.toString(), e);
//    }

//        try {
    AutoPtr<IApplication> app;
    capsuleInfo->MakeApplication(FALSE, mInstrumentation, (IApplication**)&app);

//    if (localLOGV) Slog.v(
//        TAG, "Performing receive of " + data.intent
//        + ": app=" + app
//        + ", appName=" + app.getPackageName()
//        + ", pkg=" + packageInfo.getPackageName()
//        + ", comp=" + data.intent.getComponent().toShortString()
//        + ", dir=" + packageInfo.getAppDir());

    AutoPtr<CContextImpl> context;
//    ContextImpl context = (ContextImpl)app.getBaseContext();
    app->GetBaseContext((IContext**)(CContextImpl**)&context);
    receiver->SetOrderedHint(TRUE);
    receiver->SetResult(data->mResultCode, data->mResultData,
            data->mResultExtras);
    receiver->SetOrderedHint(data->mSync);
    receiver->ReceiveBroadcast(context->GetReceiverRestrictedContext(),
            data->mIntent);
//        } catch (Exception e) {
//            try {
//                if (DEBUG_BROADCAST) Slog.i(TAG,
//                        "Finishing failed broadcast to " + data.intent.getComponent());
//                mgr.finishReceiver(mAppThread.asBinder(), data.resultCode,
//                    data.resultData, data.resultExtras, data.resultAbort);
//            } catch (RemoteException ex) {
//            }
//            if (!mInstrumentation.onException(receiver, e)) {
//                throw new RuntimeException(
//                    "Unable to start receiver " + component
//                    + ": " + e.toString(), e);
//            }
//        }
//    QueuedWork.waitToFinish();
//    try {
    if (data->mSync) {
//        if (DEBUG_BROADCAST) Slog.i(TAG,
//                "Finishing ordered broadcast to " + data.intent.getComponent());
        Int32 rc;
        String rd;
        AutoPtr<IBundle> re;
        Boolean ab;
        receiver->GetResultCode(&rc);
        receiver->GetResultData(&rd);
        receiver->GetResultExtras(FALSE, (IBundle**)&re);
        receiver->GetAbortBroadcast(&ab);
        mgr->FinishReceiver((IApplicationApartment*)this, rc, rd, re, ab);
    }
    else {
//        if (DEBUG_BROADCAST) Slog.i(TAG,
//                "Finishing broadcast to " + data.intent.getComponent());
        mgr->FinishReceiver((IApplicationApartment*)this, 0, String(NULL), NULL, FALSE);
    }
//    } catch (RemoteException ex) {
//    }

    return NOERROR;
}

ECode CApplicationApartment::HandleCreateService(
    /* [in] */ CreateServiceData* data)
{
    // If we are getting ready to gc after going to the background, well
    // we are back active so skip it.
//    unscheduleGcIdler();

    LoadedCap* capsuleInfo = GetCapsuleInfoNoCheck(
            data->mInfo->mApplicationInfo);

    char path[256];
    strcpy(path, "/data/data/com.elastos.runtime/elastos/");
    strcat(path, data->mInfo->mCapsuleName);
    strcat(path, "/");
    strcat(path, data->mInfo->mCapsuleName);
    strcat(path, ".eco");

    AutoPtr<IModuleInfo> moduleInfo;
    ECode ec = CReflector::AcquireModuleInfo(path, (IModuleInfo**)&moduleInfo);
    if (FAILED(ec)) {
        Slogger::D(TAG, StringBuffer("Cann't Find the path is ") + path);
        return E_RUNTIME_EXCEPTION;
    }
    printf("==== File: %s, Line: %d ====, pid: %d, FUNC : %s.\n", __FILE__, __LINE__, getpid(), __FUNCTION__);
    AutoPtr<IClassInfo> classInfo;
    moduleInfo->GetClassInfo(data->mInfo->mName, (IClassInfo**)&classInfo);
    assert(classInfo != NULL);
    AutoPtr<IInterface> object;
    classInfo->CreateObject((IInterface**)&object);
    assert(object != NULL);
    AutoPtr<IService> service;
    service = (IService*)object->Probe(EIID_IService);

//    try {
//    if (localLOGV) Slog.v(TAG, "Creating service " + data.info.name);

    AutoPtr<CContextImpl> appContext;
    CContextImpl::NewByFriend((CContextImpl**)&appContext);
    appContext->Init(capsuleInfo, NULL, this);

    AutoPtr<IApplication> app;
    capsuleInfo->MakeApplication(FALSE, mInstrumentation, (IApplication**)&app);
    appContext->SetOuterContext((IContext*)(IService*)service);
    AutoPtr<IActivityManager> activityManager;
    ActivityManagerNative::GetDefault((IActivityManager**)&activityManager);
    service->Attach(appContext, this, data->mInfo->mName, data->mToken, app,
            activityManager);
    service->Create();
    mServices[data->mToken] = service;
//    try {
    activityManager->ServiceDoneExecuting(data->mToken, 0, 0, 0);
//    } catch (RemoteException e) {
//        // nothing to do.
//    }
//    } catch (Exception e) {
//        if (!mInstrumentation.onException(service, e)) {
//            throw new RuntimeException(
//                "Unable to create service " + data.info.name
//                + ": " + e.toString(), e);
//        }
//    }
    return NOERROR;
}

ECode CApplicationApartment::HandleBindService(
    /* [in] */ BindServiceData* data)
{
    AutoPtr<IService> s = mServices[data->mToken];
    if (s != NULL) {
//        try {
//        data.intent.setExtrasClassLoader(s.getClassLoader());
//        try {
        AutoPtr<IActivityManager> activityManager;
        ActivityManagerNative::GetDefault((IActivityManager**)&activityManager);
        if (!data->mRebind) {
            AutoPtr<IBinder> binder;
            s->Bind(data->mIntent, (IBinder**)&binder);
            activityManager->PublishService(
                    data->mToken, data->mIntent, binder);
        } else {
            s->Rebind(data->mIntent);
            activityManager->ServiceDoneExecuting(
                    data->mToken, 0, 0, 0);
        }
//        ensureJitEnabled();
//        } catch (RemoteException ex) {
//        }
//        } catch (Exception e) {
//            if (!mInstrumentation.onException(s, e)) {
//                throw new RuntimeException(
//                        "Unable to bind to service " + s
//                        + " with " + data.intent + ": " + e.toString(), e);
//            }
//        }
    }
    return NOERROR;
}

ECode CApplicationApartment::HandleUnbindService(
    /* [in] */ BindServiceData* data)
{
    AutoPtr<IService> s = mServices[data->mToken];
    if (s != NULL) {
//        try {
//            data.intent.setExtrasClassLoader(s.getClassLoader());
//            try {
        AutoPtr<IActivityManager> activityManager;
        ActivityManagerNative::GetDefault((IActivityManager**)&activityManager);
        if (!data->mRebind) {
            AutoPtr<IBinder> binder;
            s->Bind(data->mIntent, (IBinder**)&binder);
            activityManager->PublishService(
                    data->mToken, data->mIntent, binder);
        } else {
            s->Rebind(data->mIntent);
            activityManager->ServiceDoneExecuting(data->mToken, 0, 0, 0);
        }
//            } catch (RemoteException ex) {
//            }
//        } catch (Exception e) {
//            if (!mInstrumentation.onException(s, e)) {
//                throw new RuntimeException(
//                        "Unable to bind to service " + s
//                        + " with " + data.intent + ": " + e.toString(), e);
//            }
//        }
    }
    return NOERROR;
}

ECode CApplicationApartment::HandleServiceArgs(
    /* [in] */ ServiceArgsData* data)
{
    AutoPtr<IService> s = mServices[data->mToken];
    if (s != NULL) {
//        try {
//        if (data.args != null) {
//            data.args.setExtrasClassLoader(s.getClassLoader());
//        }
        Int32 res;
        s->StartCommand(data->mArgs, data->mFlags, data->mStartId, &res);
//        QueuedWork.waitToFinish();
//        try {
        AutoPtr<IActivityManager> activityManager;
        ActivityManagerNative::GetDefault((IActivityManager**)&activityManager);
        activityManager->ServiceDoneExecuting(data->mToken, 1, data->mStartId, res);
//        } catch (RemoteException e) {
//            // nothing to do.
//        }
//        ensureJitEnabled();
//        } catch (Exception e) {
//            if (!mInstrumentation.onException(s, e)) {
//                throw new RuntimeException(
//                        "Unable to start service " + s
//                        + " with " + data.args + ": " + e.toString(), e);
//            }
//        }
    }
    return NOERROR;
}

ECode CApplicationApartment::HandleStopService(
    /* [in] */ IBinder* token)
{
    AutoPtr<IService> s = mServices[token];
    if (s != NULL) {
//        try {
//        if (localLOGV) Slog.v(TAG, "Destroying service " + s);
        s->Destroy();
        AutoPtr<IContext> context;
        s->GetBaseContext((IContext**)&context);
        ClassID clsid;
        AutoPtr<IObject> ctxObj = (IObject*)context->Probe(EIID_IObject);
        ctxObj->GetClassID(&clsid);
        if (clsid == ECLSID_CContextImpl) {
            String who;
            s->GetClassName(&who);
            ((CContextImpl*)(IContext*)context)->ScheduleFinalCleanup(
                    who, String("Service"));
        }
//        QueuedWork.waitToFinish();
//        try {
        AutoPtr<IActivityManager> activityManager;
        ActivityManagerNative::GetDefault((IActivityManager**)&activityManager);
        activityManager->ServiceDoneExecuting(token, 0, 0, 0);
//        } catch (RemoteException e) {
//            // nothing to do.
//        }
//        } catch (Exception e) {
//            if (!mInstrumentation.onException(s, e)) {
//                throw new RuntimeException(
//                        "Unable to stop service " + s
//                        + ": " + e.toString(), e);
//            }
//        }
        mServices.Erase(token);
    }
    //Slog.i(TAG, "Running services: " + mServices);
    return NOERROR;
}

ECode CApplicationApartment::GetDisplayMetricsLocked(
    /* [in] */ Boolean forceUpdate,
    /* [out] */ IDisplayMetrics** dm)
{
    assert(dm != NULL);

    if (mDisplayMetrics != NULL && !forceUpdate) {
        *dm = mDisplayMetrics.Get();
        (*dm)->AddRef();
        return NOERROR;
    }
    if (mDisplay == NULL) {
        AutoPtr<IWindowManager> wm = (IWindowManager*)CWindowManagerImpl::GetDefault();
        wm->GetDefaultDisplay((IDisplay**)&mDisplay);
    }
    CDisplayMetrics::New((IDisplayMetrics**)&mDisplayMetrics);
    mDisplay->GetMetrics(mDisplayMetrics.Get());

    //Slog.i("foo", "New metrics: w=" + metrics.widthPixels + " h="
    //        + metrics.heightPixels + " den=" + metrics.density
    //        + " xdpi=" + metrics.xdpi + " ydpi=" + metrics.ydpi);
    *dm = mDisplayMetrics.Get();
    (*dm)->AddRef();
    return NOERROR;
}

ECode CApplicationApartment::GetTopLevelResources(
    /* [in] */ const String& resDir,
    /* [in] */ CCompatibilityInfo* compInfo,
    /* [out] */ CResources** res)
{
    assert(res != NULL);
    assert(compInfo != NULL);
    ResourcesKey* key = new ResourcesKey(resDir, compInfo->mApplicationScale);
    AutoPtr<CResources> r;
    {
        Mutex::Autolock lock(mCapsulesLock);

        // Resources is app scale dependent.
//        if (false) {
//            Slog.w(TAG, "getTopLevelResources: " + resDir + " / "
//                    + compInfo.applicationScale);
//        }
        HashMap<ResourcesKey*, AutoPtr<CResources>, HashRK, RKEq>::Iterator it =
                mActiveResources.Find(key);
        if (it != mActiveResources.End()) {
            r = (it->mSecond).Get();
        }
        //if (r != null) Slog.i(TAG, "isUpToDate " + resDir + ": " + r.getAssets().isUpToDate());
        if (r != NULL) {
            Boolean updated;
            AutoPtr<IAssetManager> assets;
            r->GetAssets((IAssetManager**)&assets);
            assets->IsUpToDate(&updated);
            if (updated) {
//            if (false) {
//                Slog.w(TAG, "Returning cached resources " + r + " " + resDir
//                        + ": appScale=" + r.getCompatibilityInfo().applicationScale);
//            }
                *res = r.Get();
                (*res)->AddRef();
                return NOERROR;
            }
        }
    }

    //if (r != null) {
    //    Slog.w(TAG, "Throwing away out-of-date resources!!!! "
    //            + r + " " + resDir);
    //}

    AutoPtr<CAssetManager> assets;
    CAssetManager::NewByFriend((CAssetManager**)&assets);
    Int32 cookie;
    assets->AddAssetPath(resDir, &cookie);
    if (cookie == 0) {
        *res = NULL;
        return E_FAIL;
    }

    //Slog.i(TAG, "Resource: key=" + key + ", display metrics=" + metrics);
    AutoPtr<IDisplayMetrics> metrics;
    GetDisplayMetricsLocked(FALSE, (IDisplayMetrics**)&metrics);
    AutoPtr<IConfiguration> config = GetConfiguration();
    CResources::NewByFriend((IAssetManager*)assets.Get(), metrics.Get(),
            config.Get(), (ICompatibilityInfo*)compInfo, (CResources**)&r);
//    if (false) {
//        Slog.i(TAG, "Created app resources " + resDir + " " + r + ": "
//                + r.getConfiguration() + " appScale="
//                + r.getCompatibilityInfo().applicationScale);
//    }

    {
        Mutex::Autolock lock(mCapsulesLock);

        AutoPtr<CResources> existing;
        HashMap<ResourcesKey*, AutoPtr<CResources>, HashRK, RKEq>::Iterator it =
                mActiveResources.Find(key);
        if (it != mActiveResources.End()) {
            existing = (it->mSecond).Get();
        }
        if (existing != NULL) {
            Boolean updated;
            AutoPtr<IAssetManager> assets;
            existing->GetAssets((IAssetManager**)&assets);
            assets->IsUpToDate(&updated);
            if (updated) {
                // Someone else already created the resources while we were
                // unlocked; go ahead and use theirs.
                AutoPtr<IAssetManager> assets1;
                r->GetAssets((IAssetManager**)&assets1);
                assets1->Close();
                *res = existing.Get();
                (*res)->AddRef();
                return NOERROR;
            }
        }

        // XXX need to remove entries when weak references go away
        mActiveResources[key] = r;
        *res = r.Get();
        (*res)->AddRef();
        return NOERROR;
    }
}

ECode CApplicationApartment::GetTopLevelResources(
    /* [in] */ const String& resDir,
    /* [in] */ LoadedCap* capInfo,
    /* [out] */ CResources** res)
{
    return GetTopLevelResources(resDir, capInfo->mCompatibilityInfo.Get(), res);
}

LoadedCap* CApplicationApartment::GetCapsuleInfoNoCheck(
    /* [in] */ CApplicationInfo* ai)
{
    return GetCapsuleInfo(ai, FALSE, TRUE);
}

LoadedCap* CApplicationApartment::GetCapsuleInfo(
    /* [in] */ CApplicationInfo* aInfo,
    /* [in] */ Boolean securityViolation,
    /* [in] */ Boolean includeCode)
{
    LoadedCap* capsuleInfo;
    Mutex::Autolock lock(mCapsulesLock);

    if (includeCode) {
        capsuleInfo = mCapsules[aInfo->mCapsuleName];
    }
    else {
        capsuleInfo = mResourceCapsules[aInfo->mCapsuleName];
    }
    if (capsuleInfo == NULL /*|| (packageInfo.mResources != null
            && !packageInfo.mResources.getAssets().isUpToDate()) */ ) {
//        if (localLOGV) Slog.v(TAG, (includeCode ? "Loading code package "
//                : "Loading resource-only package ") + aInfo.packageName
//                + " (in " + (mBoundApplication != null
//                        ? mBoundApplication.processName : null)
//                + ")");
        capsuleInfo = new LoadedCap(this,
                aInfo, this, securityViolation, includeCode /* &&
                (aInfo.flags&ApplicationInfo.FLAG_HAS_CODE) != 0 */);
        if (includeCode) {
            mCapsules[aInfo->mCapsuleName] = capsuleInfo;
        } else {
            mResourceCapsules[aInfo->mCapsuleName] = capsuleInfo;
        }
    }

    return capsuleInfo;
}

LoadedCap* CApplicationApartment::GetCapsuleInfo(
    /* [in] */ CApplicationInfo* ai,
    /* [in] */ Int32 flags)
{
    Boolean includeCode = (flags & Context_CONTEXT_INCLUDE_CODE) != 0;
    Boolean securityViolation = includeCode && ai->mUid != 0
            /*&& ai->mUid != Process.SYSTEM_UID*/ && (mBoundApplication != NULL
                    ? ai->mUid != mBoundApplication->mAppInfo->mUid : TRUE);
    if ((flags & (Context_CONTEXT_INCLUDE_CODE
            | Context_CONTEXT_IGNORE_SECURITY))
            == Context_CONTEXT_INCLUDE_CODE) {
        if (securityViolation) {
//            String msg = "Requesting code from " + ai.packageName
//                    + " (with uid " + ai.uid + ")";
//            if (mBoundApplication != null) {
//                msg = msg + " to be run in process "
//                    + mBoundApplication.processName + " (with uid "
//                    + mBoundApplication.appInfo.uid + ")";
//            }
//            throw new SecurityException(msg);
        }
    }
    return GetCapsuleInfo(ai, securityViolation, includeCode);
}

LoadedCap* CApplicationApartment::GetCapsuleInfo(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 flags)
{
    {
        Mutex::Autolock lock(mCapsulesLock);
        LoadedCap* capsuleInfo;
        if ((flags & Context_CONTEXT_INCLUDE_CODE) != 0) {
            capsuleInfo = mCapsules[capsuleName];
        } else {
            capsuleInfo = mResourceCapsules[capsuleName];
        }
        //Slog.i(TAG, "getPackageInfo " + packageName + ": " + packageInfo);
        //if (packageInfo != null) Slog.i(TAG, "isUptoDate " + packageInfo.mResDir
        //        + ": " + packageInfo.mResources.getAssets().isUpToDate());
        if (capsuleInfo != NULL /*&& (packageInfo.mResources == null
                || packageInfo.mResources.getAssets().isUpToDate()) */ ) {
            if (capsuleInfo->IsSecurityViolation()
                    && (flags & Context_CONTEXT_IGNORE_SECURITY) == 0) {
    //            throw new SecurityException(
    //                    "Requesting code from " + packageName
    //                    + " to be run in process "
    //                    + mBoundApplication.processName
    //                    + "/" + mBoundApplication.appInfo.uid);
            }
            return capsuleInfo;
        }
    }

    AutoPtr<CApplicationInfo> ai;
//    try {
    GetCapsuleManager()->GetApplicationInfo(capsuleName,
            CapsuleManager_GET_SHARED_LIBRARY_FILES,
            (IApplicationInfo**)(CApplicationInfo**)&ai);
//    } catch (RemoteException e) {
//    }

    if (ai != NULL) {
        return GetCapsuleInfo(ai, flags);
    }

    return NULL;
}

ECode CApplicationApartment::InstallContentProviders(
    /* [in] */ IContext* context,
    /* [in] */ IObjectContainer* providers)
{
    ECode ec;
    AutoPtr<IObjectContainer> results;
    CParcelableObjectContainer::New((IObjectContainer**)&results);

    Boolean hasNext = FALSE;
    AutoPtr<IObjectEnumerator> enumerator;
    providers->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    while(enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IInterface> itf;
        enumerator->Current((IInterface**)&itf);
        AutoPtr<CContentProviderInfo> cpi;
        cpi = (CContentProviderInfo*)
                (IContentProviderInfo*)(IInterface*)itf;
//        StringBuilder buf = new StringBuilder(128);
//        buf.append("Pub ");
//        buf.append(cpi.authority);
//        buf.append(": ");
//        buf.append(cpi.name);
//        Slog.i(TAG, buf.toString());
        AutoPtr<IContentProvider> cp;
        ec = InstallProvider(context, NULL, cpi, TRUE, (IContentProvider**)&cp);
        if (SUCCEEDED(ec) && (cp != NULL)) {
            AutoPtr<CContentProviderHolder> cph;
            CContentProviderHolder::NewByFriend((CContentProviderHolder**)&cph);
            cph->mProvider = cp;
            results->Add((IContentProviderHolder*)(CContentProviderHolder*)cph);
//            // Don't ever unload this provider from the process.
//            synchronized(mProviderMap) {
//                mProviderRefCountMap.put(cp.asBinder(), new ProviderRefCount(10000));
//            }
        }
    }

    AutoPtr<IActivityManager> amService;
    ActivityManagerNative::GetDefault((IActivityManager**)&amService);
    ec = amService->PublishContentProviders(this, results);

    return ec;
}

ECode CApplicationApartment::GetExistingProvider(
    /* [in] */ IContext* context,
    /* [in] */ const String& name,
    /* [out] */ IContentProvider** provider)
{
    Mutex::Autolock lock(mProviderMapLock);
    ProviderClientRecord* pr = mProviderMap[name];
    if (pr != NULL) {
        *provider = pr->mProvider;
        if (*provider != NULL) (*provider)->AddRef();
        return NOERROR;
    }

    return E_DOES_NOT_EXIST;
}

ECode CApplicationApartment::GetProvider(
    /* [in] */ IContext* context,
    /* [in] */ const String& name,
    /* [out] */ IContentProvider** provider)
{
    ECode ec = GetExistingProvider(context, name, provider);
    if (SUCCEEDED(ec)) {
        return ec;
    }

    AutoPtr<IContentProviderHolder> holder;
    AutoPtr<IActivityManager> amService;
    ActivityManagerNative::GetDefault((IActivityManager**)&amService);
    ec = amService->GetContentProvider((IApplicationApartment*)this, name,
            (IContentProviderHolder**)&holder);
    if (FAILED(ec) || holder == NULL) {
        Slogger::E(TAG, StringBuffer("Failed to find provider info for ") + name);
        *provider = NULL;
        return ec;
    }

    AutoPtr<IContentProviderInfo> info;
    AutoPtr<IContentProvider> prov;
    holder->GetContentProviderInfo((IContentProviderInfo**)&info);
    holder->GetContentProvider((IContentProvider**)&prov);
    InstallProvider(context, prov, info, TRUE, provider);
//    //Slog.i(TAG, "noReleaseNeeded=" + holder.noReleaseNeeded);
//    if (holder.noReleaseNeeded || holder.provider == null) {
//        // We are not going to release the provider if it is an external
//        // provider that doesn't care about being released, or if it is
//        // a local provider running in this process.
//        //Slog.i(TAG, "*** NO RELEASE NEEDED");
//        synchronized(mProviderMap) {
//            mProviderRefCountMap.put(prov.asBinder(), new ProviderRefCount(10000));
//        }
//    }

    return NOERROR;
}

ECode CApplicationApartment::AcquireProvider(
    /* [in] */ IContext* context,
    /* [in] */ const String& name,
    /* [out] */ IContentProvider** provider)
{
    if (provider == NULL) return E_INVALID_ARGUMENT;

    ECode ec = GetProvider(context, name, provider);
    if (FAILED(ec) || provider == NULL) {
        *provider = NULL;
        return ec;
    }
//    IBinder jBinder = provider.asBinder();
//    synchronized(mProviderMap) {
//        ProviderRefCount prc = mProviderRefCountMap.get(jBinder);
//        if(prc == null) {
//            mProviderRefCountMap.put(jBinder, new ProviderRefCount(1));
//        } else {
//            prc.count++;
//        } //end else
//    } //end synchronized
    return ec;
}

ECode CApplicationApartment::AcquireExistingProvider(
    /* [in] */ IContext* context,
    /* [in] */ const String& name,
    /* [out] */ IContentProvider** provider)
{
    if (provider == NULL) return E_INVALID_ARGUMENT;

    ECode ec = GetExistingProvider(context, name, provider);
    if (FAILED(ec)) {
        return ec;
    }

    Mutex::Autolock lock(mProviderMapLock);
    HashMap<AutoPtr<IContentProvider>, Int32, HashICP, ICPEq>::Iterator
            it = mProviderRefCountMap->Find(AutoPtr<IContentProvider>(*provider));
    if (it != mProviderRefCountMap->End()) {
        it->mSecond++;
    }
    else {
        (*mProviderRefCountMap)[AutoPtr<IContentProvider>(*provider)] = 1;
    }

    return ec;
}

ECode CApplicationApartment::InstallProvider(
    /* [in] */ IContext* context,
    /* [in] */ IContentProvider* provider,
    /* [in] */ IContentProviderInfo* info,
    /* [in] */ Boolean noisy,
    /* [out] */ IContentProvider** retProvider)
{
    AutoPtr<IContentProvider> localProvider;
    if (provider == NULL) {
        if (noisy) {
//            Slog.d(TAG, "Loading provider " + info.authority + ": "
//                    + info.name);
        }
        AutoPtr<IContext> c;
        AutoPtr<IApplicationInfo> ai;
        info->GetApplicationInfo((IApplicationInfo**)&ai);
        String capName;
        context->GetCapsuleName(&capName);
        if (!capName.Compare(
                ((CApplicationInfo*)(IApplicationInfo*)ai)->mCapsuleName)) {
            c = context;
        }
        else if (mInitialApplication != NULL) {
            mInitialApplication->GetCapsuleName(&capName);
            if (!capName.Compare(((CApplicationInfo*)
                    (IApplicationInfo*)ai)->mCapsuleName)) {
                c = (IContext*)mInitialApplication;
            }
        }
        else {
            context->CreateCapsuleContext(
                    ((CApplicationInfo*)(IApplicationInfo*)ai)->mCapsuleName,
                    -1 /* Context.CONTEXT_INCLUDE_CODE */, (IContext**)&c);
        }
        if (c == NULL) {
//            Slog.w(TAG, "Unable to get context for package " +
//                  ai.packageName +
//                  " while loading content provider " +
//                  info.name);
            *retProvider = NULL;
            return E_DOES_NOT_EXIST;
        }

        AutoPtr<IModuleInfo> moduleInfo;
        AutoPtr<IClassInfo> classInfo;
        AutoPtr<IInterface> object;
        String className = ((CContentProviderInfo*)info)->mName;
        String capsuleName = ((CContentProviderInfo*)info)->mCapsuleName;
        String path = capsuleName + ".eco";

        CReflector::AcquireModuleInfo(path /* info.name */, (IModuleInfo**)&moduleInfo);
        moduleInfo->GetClassInfo(className, (IClassInfo**)&classInfo);
        ECode ec = classInfo->CreateObject((IInterface**)&object);
        if (FAILED(ec) || object == NULL) {
//            Slog.e(TAG, "Failed to instantiate class " +
//                          info.name + " from sourceDir " +
//                          info.applicationInfo.sourceDir);
            *retProvider = NULL;
            return ec;
        }
        localProvider = (IContentProvider*)object->Probe(EIID_IContentProvider);

//        provider = localProvider.getIContentProvider();
        Int32 size;
        void* buf;
        _CObject_MarshalInterface(localProvider, &buf, &size);
        _CObject_UnmarshalInterface(UnmarshalFlag_Coexisting,
                buf, (IInterface**)&provider, &size);

//        if (Config.LOGV) Slog.v(
//                    TAG, "Instantiating local provider " + info.name);
        localProvider->AttachInfo(c, info);
    }
//    else if (localLOGV) {
//        Slog.v(TAG, "Installing external provider " + info.authority + ": "
//                + info.name);
//    }

    {
        Mutex::Autolock lock(mProviderMapLock);
        String auth = ((CContentProviderInfo*)info)->mAuthority;
        StringTokenizer* tokens = new StringTokenizer(auth, ";");
        while (tokens->HasMoreTokens()) {
            String name = tokens->NextToken();
            ProviderClientRecord* pr = new ProviderClientRecord(name,
                    provider, localProvider);
            mProviderMap[name] = pr;
        }
        if (localProvider != NULL) {
    //        mLocalProviders.put(provider.asBinder(),
    //                        new ProviderClientRecord(null, provider, localProvider));
        }
    }

    *retProvider = provider;
    return NOERROR;
}

ECode CApplicationApartment::ReleaseProvider(
    /* [in] */ IContentProvider* provider)
{
//    if(provider == null) {
//            return false;
//        }
//        IBinder jBinder = provider.asBinder();
//        synchronized(mProviderMap) {
//            ProviderRefCount prc = mProviderRefCountMap.get(jBinder);
//            if(prc == null) {
//                if(localLOGV) Slog.v(TAG, "releaseProvider::Weird shouldn't be here");
//                return false;
//            } else {
//                prc.count--;
//                if(prc.count == 0) {
//                    // Schedule the actual remove asynchronously, since we
//                    // don't know the context this will be called in.
//                    // TODO: it would be nice to post a delayed message, so
//                    // if we come back and need the same provider quickly
//                    // we will still have it available.
//                    Message msg = mH.obtainMessage(H.REMOVE_PROVIDER, provider);
//                    mH.sendMessage(msg);
//                } //end if
//            } //end else
//        } //end synchronized
        return true;
    return E_NOT_IMPLEMENTED;
}

ECode CApplicationApartment::ScheduleExit()
{
    return E_NOT_IMPLEMENTED;
}

ECode CApplicationApartment::RequestThumbnail(
	/* [in] */ IBinder* token)
{
    return E_NOT_IMPLEMENTED;
}


ECode CApplicationApartment::HandleContextCleanup(
    /* [in] */ ContextCleanupInfo* cci)
{
    return ((CContextImpl*)(IContext*)cci->mContext)->PerformFinalCleanup(
            cci->mWho, cci->mWhat);
}

ECode CApplicationApartment::Startup(
    /* [in] */ ApartmentAttr attr)
{
    AutoPtr<IInterface> obj;
    AutoPtr<IServiceManager> serviceManager;

    Elastos::GetServiceManager((IServiceManager**)&serviceManager);
    serviceManager->GetService(String("ActivityManagerService"), (IInterface**)&obj);

    AutoPtr<IActivityManager> ams;
    ams = (IActivityManager*)obj->Probe(EIID_IActivityManager);

    ams->AttachApplication((IApplicationApartment*)this, getpid());

    mApartment->Start(attr);

    return NOERROR;
}

IInstrumentation* CApplicationApartment::GetInstrumentation()
{
    return mInstrumentation;
}

IConfiguration* CApplicationApartment::GetConfiguration()
{
    return (IConfiguration*)mConfiguration.Get();
}
