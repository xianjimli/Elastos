
#include "app/CDefaultInstrumentation.h"
#include "app/ActivityManagerNative.h"
#include "app/CActivityMonitor.h"
#include "app/Activity.h"
#include "app/CApplication.h"


ECode CDefaultInstrumentation::constructor()
{
    mApartment = NULL;
    mMessageQueue = NULL;
    mActivityMonitors = NULL;
    return NOERROR;
}

ECode CDefaultInstrumentation::OnCreate(
    /* [in] */ IBundle* arguments)
{
    return NOERROR;
}

ECode CDefaultInstrumentation::OnStart()
{
    return NOERROR;
}

ECode CDefaultInstrumentation::OnDestroy()
{
    return NOERROR;
}

/**
 * Perform instantiation of the process's {@link Application} object.  The
 * default implementation provides the normal system behavior.
 *
 * @param cl The ClassLoader with which to instantiate the object.
 * @param className The name of the class implementing the Application
 *                  object.
 * @param context The context to initialize the application with
 *
 * @return The newly instantiated Application object.
 */
ECode CDefaultInstrumentation::NewApplication(
    /* [in] */ const String& moduleName,
    /* [in] */ const String& className,
    /* [in] */ IContext* context,
    /* [out] */ IApplication** app)
{
    if (app == NULL) return E_INVALID_ARGUMENT;

//    Int32 idx = className.LastIndexOf('.');
//    char *path = (char*)malloc(idx + 10 + 1);
//    strcpy(path, "/data/");
//    strncat(path, className, idx);
//    strcat(path, ".eco");

    AutoPtr<IModuleInfo> moduleInfo;
    AutoPtr<IClassInfo> classInfo;
    AutoPtr<IInterface> object;
    CReflector::AcquireModuleInfo(moduleName, (IModuleInfo**)&moduleInfo);
    moduleInfo->GetClassInfo(className, (IClassInfo**)&classInfo);
    classInfo->CreateObject((IInterface**)&object);
    *app = (IApplication*)object->Probe(EIID_IApplication);
    (*app)->AddRef();
    ((CApplication*)(*app))->Attach(context);

    return NOERROR;
}

/**
 * Perform calling of the application's {@link Application#onCreate}
 * method.  The default implementation simply calls through to that method.
 *
 * @param app The application being created.
 */
ECode CDefaultInstrumentation::CallApplicationOnCreate(
    /* [in] */ IApplication* app)
{
    return app->Create();
}

/**
 * Perform calling of an activity's {@link Activity#onCreate}
 * method.  The default implementation simply calls through to that method.
 *
 * @param activity The activity being created.
 * @param icicle The previously frozen state (or null) to pass through to
 *               onCreate().
 */
ECode CDefaultInstrumentation::CallActivityOnCreate(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* icicle)
{
    if (mWaitingActivities != NULL) {
        Mutex::Autolock lock(mSync);
        List<ActivityWaiter*>::Iterator it;
        for (it = mWaitingActivities->Begin(); it != mWaitingActivities->End(); ++it) {
            ActivityWaiter* aw = *it;
            const AutoPtr<IIntent> intent = aw->mIntent;
            AutoPtr<IIntent> aIntent;
            activity->GetIntent((IIntent**)&aIntent);
            Boolean eq;
            intent->FilterEquals(aIntent, &eq);
            if (eq) {
                aw->mActivity = activity;
//                    mMessageQueue.addIdleHandler(new ActivityGoing(aw));
            }
        }
    }

    activity->Create(icicle);

    if (mActivityMonitors != NULL) {
        Mutex::Autolock lock(mSync);
        List<AutoPtr<IActivityMonitor> >::Iterator it;
        for (it = mActivityMonitors->Begin(); it != mActivityMonitors->End(); ++it) {
            CActivityMonitor* am = (CActivityMonitor*)(IActivityMonitor*)*it;
            AutoPtr<IIntent> intent;
            activity->GetIntent((IIntent**)&intent);
            am->Match(activity, activity, intent);
        }
    }
    return NOERROR;
}

ECode CDefaultInstrumentation::CallActivityOnPostCreate(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* icicle)
{
    return activity->PostCreate(icicle);
}

ECode CDefaultInstrumentation::CallActivityOnNewIntent(
    /* [in] */ IActivity *activity,
    /* [in] */ IIntent *intent)
{
    activity->DispatchNewIntent(intent);
    return NOERROR;
}

ECode CDefaultInstrumentation::CallActivityOnStart(
    /* [in] */ IActivity* activity)
{
    activity->Start();
    return NOERROR;
}

/**
 * Perform calling of an activity's {@link Activity#onRestart}
 * method.  The default implementation simply calls through to that method.
 *
 * @param activity The activity being restarted.
 */
ECode CDefaultInstrumentation::CallActivityOnRestart(
    /* [in] */ IActivity* activity)
{
    activity->Restart();
    return NOERROR;
}

/**
 * Perform calling of an activity's {@link Activity#onResume} method.  The
 * default implementation simply calls through to that method.
 *
 * @param activity The activity being resumed.
 */
ECode CDefaultInstrumentation::CallActivityOnResume(
    /* [in] */ IActivity* activity)
{
    ((Activity*)activity)->SetResumed(TRUE);
    activity->Resume();

    if (mActivityMonitors != NULL) {
        Mutex::Autolock lock(mSync);
        List<AutoPtr<IActivityMonitor> >::Iterator it;
        for(it = mActivityMonitors->Begin(); it != mActivityMonitors->End(); ++it) {
            CActivityMonitor* am = (CActivityMonitor*)(IActivityMonitor*)*it;
            AutoPtr<IIntent> intent;
            activity->GetIntent((IIntent**)&intent);
            am->Match(activity, activity, intent);
        }
    }
    return NOERROR;
}

ECode CDefaultInstrumentation::CallActivityOnStop(
    /* [in] */ IActivity* activity)
{
    activity->Stop();
    return NOERROR;
}

ECode CDefaultInstrumentation::CallActivityOnSaveInstanceState(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* outState)
{
    activity->PerformSaveInstanceState(outState);
    return NOERROR;
}

ECode CDefaultInstrumentation::CallActivityOnPause(
    /* [in] */ IActivity* activity)
{
    activity->PerformPause();
    return NOERROR;
}

ECode CDefaultInstrumentation::CallActivityOnUserLeaving(
    /* [in] */ IActivity* activity)
{
    activity->PerformUserLeaving();
    return NOERROR;
}

ECode CDefaultInstrumentation::ExecStartActivity(
    /* [in] */ IContext* who,
    /* [in] */ IApplicationApartment* apartment,
    /* [in] */ IBinder* token,
    /* [in] */ IActivity* target,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode,
    /* [out] */ IActivityResult** result)
{
    if (result == NULL) return E_INVALID_ARGUMENT;
    if (mActivityMonitors != NULL) {
        Mutex::Autolock lock(mSync);
        List<AutoPtr<IActivityMonitor> >::Iterator it;
        for(it = mActivityMonitors->Begin(); it != mActivityMonitors->End(); ++it) {
            CActivityMonitor* am = (CActivityMonitor*)(IActivityMonitor*)*it;
            if (am->Match(who, NULL, intent)) {
                am->mHits++;
                Boolean isBlock;
                am->IsBlocking(&isBlock);
                if (isBlock) {
                    AutoPtr<IActivityResult> aResult;
                    am->GetResult((IActivityResult**)&aResult);
                    if(requestCode >= 0 ) {
                        *result = aResult;
                        return NOERROR;
                    }
                    else {
                        *result = NULL;
                        return NOERROR;
                    }
                }
                break;
            }
        }
    }
    AutoPtr<IActivityManager> amService;
    ECode ec = ActivityManagerNative::GetDefault(
            (IActivityManager**)&amService);
    AutoPtr<IContentResolver> resolver;
    who->GetContentResolver((IContentResolver**)&resolver);
    String type;
    intent->ResolveTypeIfNeeded(resolver, &type);
    String resultWho;
    if (target != NULL) target->GetID(&resultWho);
    Int32 res;
    ec = amService->StartActivity(apartment, intent, type, NULL,
            0, token, resultWho, requestCode, FALSE, FALSE, &res);
    CheckStartActivityResult(res, intent);
    *result = NULL;
    return ec;
}

ECode CDefaultInstrumentation::Init(
    /* [in] */ IApplicationApartment* apartment,
    /* [in] */ IContext* instrContext,
    /* [in] */ IContext* appContext,
    /* [in] */ IComponentName* component, 
    /* [in] */ IInstrumentationWatcher* watcher)
{
    mApartment = apartment;
    //mMessageQueue = mApartment.getLooper().myQueue();
    mInstrContext = instrContext;
    mAppContext = appContext;
    mComponent = component;
    mWatcher = watcher;
    return NOERROR;
}

ECode CDefaultInstrumentation::CheckStartActivityResult(
    /* [in] */ Int32 res,
    /* [in] */ IIntent* intent)
{
    if (res >= ActivityManager_START_SUCCESS) {
             return NOERROR;
    }

    AutoPtr<IComponentName> component;
    intent->GetComponent((IComponentName**)&component);
    switch (res) {
        case ActivityManager_START_INTENT_NOT_RESOLVED:
        case ActivityManager_START_CLASS_NOT_FOUND:
            if (IIntent::Probe(intent) && component != NULL) return E_NOT_FOUND_EXCEPTION;
                //throw new ActivityNotFoundException(
                //        "Unable to find explicit activity class "
                //        + ((Intent)intent).getComponent().toShortString()
                //        + "; have you declared this activity in your AndroidManifest.xml?");
            //throw new ActivityNotFoundException(
            //        "No Activity found to handle " + intent);
            return E_NOT_FOUND_EXCEPTION;
        case ActivityManager_START_PERMISSION_DENIED:
            //throw new SecurityException("Not allowed to start activity "
            //         + intent);
            return E_SECURITY_EXCEPTION;
        case ActivityManager_START_FORWARD_AND_REQUEST_CONFLICT:
            // throw new AndroidRuntimeException(
            //         "FORWARD_RESULT_FLAG used while also requesting a result");
            return E_RUNTIME_EXCEPTION;
        case ActivityManager_START_NOT_ACTIVITY:
            //throw new IllegalArgumentException(
            //        "PendingIntent is not an activity");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        default:
            //throw new AndroidRuntimeException("Unknown error code "
            //        + res + " when starting " + intent);
            return E_RUNTIME_EXCEPTION;
    }
}

CDefaultInstrumentation::ActivityWaiter::ActivityWaiter(
    /* [in] */ IIntent* intent)
    : mIntent(intent)
{}
