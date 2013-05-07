
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
    NativeMessageQueue* mMessageQueue;
    List<ActivityWaiter*>* mWaitingActivities;
    List<AutoPtr<IActivityMonitor> >* mActivityMonitors;
    Mutex mSync;

};

#endif // __CDEFAULTINSTRUMENTATION_H__
