
#include "app/CDefaultInstrumentation.h"
#include "app/ActivityManagerNative.h"
#include <elastos/AutoPtr.h>

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
//    (*app)->Attach(context);

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
//    if (mWaitingActivities != null) {
//        synchronized (mSync) {
//            final int N = mWaitingActivities.size();
//            for (int i=0; i<N; i++) {
//                final ActivityWaiter aw = mWaitingActivities.get(i);
//                final Intent intent = aw.intent;
//                if (intent.filterEquals(activity.getIntent())) {
//                    aw.activity = activity;
//                    mMessageQueue.addIdleHandler(new ActivityGoing(aw));
//                }
//            }
//        }
//    }

    activity->Create(icicle);

//    if (mActivityMonitors != null) {
//        synchronized (mSync) {
//            final int N = mActivityMonitors.size();
//            for (int i=0; i<N; i++) {
//                final ActivityMonitor am = mActivityMonitors.get(i);
//                am.match(activity, activity, activity.getIntent());
//            }
//        }
//    }
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
//    activity.mResumed = true;
    activity->Resume();

//    if (mActivityMonitors != null) {
//        synchronized (mSync) {
//            final int N = mActivityMonitors.size();
//            for (int i=0; i<N; i++) {
//                final ActivityMonitor am = mActivityMonitors.get(i);
//                am.match(activity, activity, activity.getIntent());
//            }
//        }
//    }
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
//    if (mActivityMonitors != null) {
//        synchronized (mSync) {
//            final int N = mActivityMonitors.size();
//            for (int i=0; i<N; i++) {
//                final ActivityMonitor am = mActivityMonitors.get(i);
//                if (am.match(who, null, intent)) {
//                    am.mHits++;
//                    if (am.isBlocking()) {
//                        return requestCode >= 0 ? am.getResult() : null;
//                    }
//                    break;
//                }
//            }
//        }
//    }
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

ECode CDefaultInstrumentation::CheckStartActivityResult(
    /* [in] */ Int32 res,
    /* [in] */ IIntent* intent)
{
    return E_NOT_IMPLEMENTED;
}
