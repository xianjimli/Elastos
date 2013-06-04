
#ifndef __CDEFAULTINSTRUMENTATION_H__
#define __CDEFAULTINSTRUMENTATION_H__

#include "_CDefaultInstrumentation.h"
#include "ext/frameworkext.h"
#include "os/NativeMessageQueue.h"
#include <elastos/Mutex.h>
#include <elastos/List.h>
#include <elastos/AutoPtr.h>

CarClass(CDefaultInstrumentation)
{
private:
    class ActivityWaiter
    {
    public:
        ActivityWaiter(
            /* [in] */ IIntent* intent);

    public:
        AutoPtr<IIntent> mIntent;
        AutoPtr<IActivity> mActivity;
    };

public:
    CARAPI constructor();

       /**
     * Called when the instrumentation is starting, before any application code
     * has been loaded.  Usually this will be implemented to simply call
     * {@link #start} to begin the instrumentation thread, which will then
     * continue execution in {@link #onStart}.
     * 
     * <p>If you do not need your own thread -- that is you are writing your
     * instrumentation to be completely asynchronous (returning to the event
     * loop so that the application can run), you can simply begin your
     * instrumentation here, for example call {@link Context#startActivity} to
     * begin the appropriate first activity of the application. 
     *  
     * @param arguments Any additional arguments that were supplied when the 
     *                  instrumentation was started.
     */
    CARAPI OnCreate(
        /* [in] */ IBundle* arguments);

       /**
     * Method where the instrumentation thread enters execution.  This allows
     * you to run your instrumentation code in a separate thread than the
     * application, so that it can perform blocking operation such as
     * {@link #sendKeySync} or {@link #startActivitySync}.
     * 
     * <p>You will typically want to call finish() when this function is done,
     * to end your instrumentation.
     */
    CARAPI OnStart();

       /**
     * Called when the instrumented application is stopping, after all of the
     * normal application cleanup has occurred.
     */
    CARAPI OnDestroy();

    /*package*/
    CARAPI Init(
        /* [in] */ IApplicationApartment* apartment,
        /* [in] */ IContext* instrContext,
        /* [in] */ IContext* appContext,
        /* [in] */ IComponentName* component, 
        /* [in] */ IInstrumentationWatcher* watcher);

    CARAPI NewApplication(
        /* [in] */ const String& moduleName,
        /* [in] */ const String& className,
        /* [in] */ IContext* context,
        /* [out] */ IApplication** app);

    CARAPI CallApplicationOnCreate(
        /* [in] */ IApplication* app);

    CARAPI CallActivityOnCreate(
        /* [in] */ IActivity* activity,
        /* [in] */ IBundle* icicle);

    /**
     * Perform calling of an activity's {@link Activity#onPostCreate} method.
     * The default implementation simply calls through to that method.
     *
     * @param activity The activity being created.
     * @param icicle The previously frozen state (or null) to pass through to
     *               onPostCreate().
     */
    CARAPI CallActivityOnPostCreate(
        /* [in] */ IActivity* activity,
        /* [in] */ IBundle* icicle);

    CARAPI CallActivityOnNewIntent(
        /* [in] */ IActivity* activity,
        /* [in] */ IIntent* intent);

    CARAPI CallActivityOnStart(
        /* [in] */ IActivity* activity);

    CARAPI CallActivityOnRestart(
        /* [in] */ IActivity* activity);

    CARAPI CallActivityOnResume(
        /* [in] */ IActivity* activity);

    CARAPI CallActivityOnStop(
        /* [in] */ IActivity* activity);

    CARAPI CallActivityOnSaveInstanceState(
        /* [in] */ IActivity* activity,
        /* [in] */ IBundle* outState);

    CARAPI CallActivityOnPause(
        /* [in] */ IActivity* activity);

    CARAPI CallActivityOnUserLeaving(
        /* [in] */ IActivity* activity);

    CARAPI ExecStartActivity(
        /* [in] */ IContext* who,
        /* [in] */ IApplicationApartment* apartment,
        /* [in] */ IBinder* token,
        /* [in] */ IActivity* target,
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 requestCode,
        /* [out] */ IActivityResult** result);

public:
    static CARAPI CheckStartActivityResult(
        /* [in] */ Int32 res,
        /* [in] */ IIntent* intent);

private:
    AutoPtr<IApplicationApartment> mApartment;
    NativeMessageQueue* mMessageQueue;
    AutoPtr<IContext> mInstrContext;
    AutoPtr<IContext> mAppContext;
    AutoPtr<IComponentName> mComponent;
    List<ActivityWaiter*>* mWaitingActivities;
    List<AutoPtr<IActivityMonitor> >* mActivityMonitors;
    AutoPtr<IInstrumentationWatcher> mWatcher;
    Mutex mSync;
};

#endif // __CDEFAULTINSTRUMENTATION_H__
