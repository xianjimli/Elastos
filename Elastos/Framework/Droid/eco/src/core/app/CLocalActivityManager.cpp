
#include "ext/frameworkext.h"
#include "app/CLocalActivityManager.h"
#include "utils/Config.h"
#include <elastos/HashMap.h>
#include <Slogger.h>

using namespace Elastos::Utility::Logging;


const CString  CLocalActivityManager::TAG = "CLocalActivityManager";

const Boolean CLocalActivityManager::localLOGV = FALSE || Config::LOGV;

CLocalActivityManager::LocalActivityRecord::LocalActivityRecord(
    /* [in] */ const String& id,
    /* [in] */ IIntent* intent)
    : mId(id)
    , mIntent(intent)
{}

CLocalActivityManager::LocalActivityRecord::~LocalActivityRecord()
{}

ECode CLocalActivityManager::constructor(
    /* [in] */ IActivity* parent,
    /* [in] */ Boolean singleMode)
{
    //mActivityThread = ActivityThread.currentActivityThread();
    mParent = parent;
    mSingleMode = singleMode;
    return NOERROR;
}

void CLocalActivityManager::MoveToState(
    /* [in] */ LocalActivityRecord* r,
    /* [in] */ Int32 desiredState)
{
    if (r->mCurState == RESTORED 
        || r->mCurState == DESTROYED) {
        // startActivity() has not yet been called, so nothing to do.
        return;
    }

    if (r->mCurState == INITIALIZING) {
        // Get the lastNonConfigurationInstance for the activity
        AutoPtr<IObjectStringMap> lastNonConfigurationInstances;
        mParent->GetLastNonConfigurationChildInstances((IObjectStringMap**)&lastNonConfigurationInstances);
        AutoPtr<IInterface> instance = NULL;
        if (lastNonConfigurationInstances != NULL) {
            lastNonConfigurationInstances->Get(r->mId, (IInterface**)&instance);
        }

        // We need to have always created the activity.
        if (localLOGV) Slogger::V(TAG, r->mId + ": starting " /*+ r->mIntent*/);
        if (r->mActivityInfo == NULL) {
            //r->mActivityInfo = mActivityThread.resolveActivityInfo(r.intent);
        }
//        r->mActivity = mActivityThread.startActivityNow(
//                mParent, r.id, r.intent, r.activityInfo, r, r.instanceState, instance);
        if (r->mActivity == NULL) {
            return;
        }
        r->mWindow = r->mActivity->GetWindow();
        r->mInstanceState = NULL;
        r->mCurState = STARTED;

        if (desiredState == RESUMED) {
            if (localLOGV) Slogger::V(TAG, r->mId + ": resuming");
//            mActivityThread.performResumeActivity(r, TRUE);
            r->mCurState = RESUMED;
        }

        // Don't do anything more here.  There is an important case:
        // if this is being done as part of onCreate() of the group, then
        // the launching of the activity gets its state a little ahead
        // of our own (it is now STARTED, while we are only CREATED).
        // If we just leave things as-is, we'll deal with it as the
        // group's state catches up.
        return;
    }
        
    switch (r->mCurState) {
        case CREATED:
            if (desiredState == STARTED) {
                if (localLOGV) Slogger::V(TAG, r->mId + ": restarting");
//                mActivityThread.performRestartActivity(r);
                r->mCurState = STARTED;
            }
            if (desiredState == RESUMED) {
                if (localLOGV) Slogger::V(TAG, r->mId + ": restarting and resuming");
//                mActivityThread.performRestartActivity(r);
//                mActivityThread.performResumeActivity(r, true);
                r->mCurState = RESUMED;
            }
            return;
  
        case STARTED:
            if (desiredState == RESUMED) {
                // Need to resume it...
                if (localLOGV) Slogger::V(TAG, r->mId + ": resuming");
//                mActivityThread.performResumeActivity(r, true);
                r->mInstanceState = NULL;
                r->mCurState = RESUMED;
            }
            if (desiredState == CREATED) {
                if (localLOGV) Slogger::V(TAG, r->mId + ": stopping");
//                mActivityThread.performStopActivity(r);
                r->mCurState = CREATED;
            }
                return;

        case RESUMED:
            if (desiredState == STARTED) {
                if (localLOGV) Slogger::V(TAG, r->mId + ": pausing");
                PerformPause(r, mFinishing);
                r->mCurState = STARTED;
            }
            if (desiredState == CREATED) {
                if (localLOGV) Slogger::V(TAG, r->mId + ": pausing");
                PerformPause(r, mFinishing);
                if (localLOGV) Slogger::V(TAG, r->mId + ": stopping");
//                mActivityThread.performStopActivity(r);
                r->mCurState = CREATED;
                }
            return;
    }
}

void CLocalActivityManager::PerformPause(
    /* [in] */ LocalActivityRecord* r,
    /* [in] */ Boolean finishing)
{
    Boolean needState = r->mInstanceState == NULL;
//    AutoPtr<IBundle> instanceState = mActivityThread.performPauseActivity(r,
//                finishing, needState);
    if (needState) {
//        r->mInstanceState = instanceState;
    }
}

ECode CLocalActivityManager::StartActivity(
    /* [in] */ const String& id,
    /* [in] */ IIntent* intent,
    /* [out] */ IWindow** window)
{
    VALIDATE_NOT_NULL(window);

    if (mCurState == INITIALIZING) {
//            throw new IllegalStateException(
//                    "Activities can't be added until the containing group has been created.");
    }

    Boolean adding = FALSE;
    Boolean sameIntent = FALSE;

    AutoPtr<IActivityInfo> aInfo = NULL;

        // Already have information about the new activity id?
    Map<String, LocalActivityRecord* >::Iterator it = mActivities.Find(id);
    LocalActivityRecord* r = it->mSecond;
    if (r == NULL) {
        // Need to create it...
        r = new LocalActivityRecord(id, intent);
        adding = TRUE;
    } 
    else if (r->mIntent != NULL) {
        r->mIntent->FilterEquals(intent, &sameIntent); 
        if (sameIntent) {
            // We are starting the same activity.
            aInfo = r->mActivityInfo;
            }
        }
    if (aInfo == NULL) {
//        aInfo = mActivityThread.resolveActivityInfo(intent);
    }

    // Pause the currently running activity if there is one and only a single
    // activity is allowed to be running at a time.
    if (mSingleMode) {
        LocalActivityRecord* old = mResumed;

        // If there was a previous activity, and it is not the current
        // activity, we need to stop it.
        if (old != NULL && old != r && mCurState == RESUMED) {
            MoveToState(old, STARTED);
        }
    }

    if (adding) {
        // It's a brand new world.
        mActivities.Insert(Map<String, LocalActivityRecord* >::ValueType(id, r));
        mActivityArray.PushBack(r);
    } 
    else if (r->mActivityInfo != NULL) {
        // If the new activity is the same as the current one, then
        // we may be able to reuse it.
        String name, rName;
        aInfo->GetName(&name);
        String capName, rCapName;
        aInfo->GetCapsuleName(&capName);
        r->mActivityInfo->GetCapsuleName(&rCapName);
        r->mActivityInfo->GetName(&rName);
        Int32 lmode;
        aInfo->GetLaunchMode(&lmode);
        Int32 flags;
        aInfo->GetFlags(&flags);
        if (aInfo == r->mActivityInfo ||
                (name.Equals(rName) && capName.Equals(rCapName))) {
            if (lmode != ActivityInfo_LAUNCH_MULTIPLE ||
                    (flags & Intent_FLAG_ACTIVITY_SINGLE_TOP) != 0) {
                // The activity wants onNewIntent() called.
                List<AutoPtr<IIntent> > intents(1);
                intents.PushBack(intent);
                if (localLOGV) Slogger::V(TAG, r->mId + ": new intent");
//                mActivityThread.performNewIntents(r, intents);
                r->mIntent = intent;
                MoveToState(r, mCurState);
                if (mSingleMode) {
                    mResumed = r;
                }
                *window = r->mWindow;
                return NOERROR;
            }
            Int32 iflags;
            intent->GetFlags(&iflags);
            if (sameIntent &&
                    (iflags & Intent_FLAG_ACTIVITY_CLEAR_TOP) == 0) {
                // We are showing the same thing, so this activity is
                // just resumed and stays as-is.
                r->mIntent = intent;
                MoveToState(r, mCurState);
                if (mSingleMode) {
                    mResumed = r;
                }
                *window = r->mWindow;
                return NOERROR;
            }
        }

        // The new activity is different than the current one, or it
        // is a multiple launch activity, so we need to destroy what
        // is currently there.
        PerformDestroy(r, TRUE);
    }

    r->mIntent = intent;
    r->mCurState = INITIALIZING;
    r->mActivityInfo = aInfo;

    MoveToState(r, mCurState);

    // When in single mode keep track of the current activity
    if (mSingleMode) {
        mResumed = r;
    }
    *window = r->mWindow;
    return NOERROR;
}

AutoPtr<IWindow> CLocalActivityManager::PerformDestroy(
    /* [in] */ LocalActivityRecord* r,
    /* [in] */ Boolean finish)
{
    AutoPtr<IWindow> win = NULL;
    win = r->mWindow;
    if (r->mCurState == RESUMED && !finish) {
        PerformPause(r, finish);
    }
    if (localLOGV) Slogger::V(TAG, r->mId + ": destroying");
//    mActivityThread.performDestroyActivity(r, finish);
    r->mActivity = NULL;
    r->mWindow = NULL;
    if (finish) {
        r->mInstanceState = NULL;
    }
    r->mCurState = DESTROYED;
    return win;
}

ECode CLocalActivityManager::DestroyActivity(
    /* [in] */ const String& id,
    /* [in] */ Boolean finish,
    /* [out] */ IWindow** window)
{
    VALIDATE_NOT_NULL(window);

    Map<String, LocalActivityRecord* >::Iterator it = mActivities.Find(id);
    LocalActivityRecord* r = it->mSecond;
    AutoPtr<IWindow> win = NULL;
    if (r != NULL) {
        win = PerformDestroy(r, finish);
        if (finish) {
            mActivities.Erase(it);
        }
    }
    *window = win;
    return NOERROR;
}

ECode CLocalActivityManager::GetCurrentActivity(
    /* [out] */ IActivity** activity)
{
    VALIDATE_NOT_NULL(activity);

    if (mResumed != NULL) {
        *activity = mResumed->mActivity;
        return NOERROR;
    }
    else {
       *activity = NULL;
       return NOERROR;
    }
}

ECode CLocalActivityManager::GetCurrentId(
    /* [out] */ String* id)
{
    VALIDATE_NOT_NULL(id);

    if (mResumed != NULL) {
        *id = mResumed->mId;
        return NOERROR;
    }
    else {
        *id = String(NULL);
        return NOERROR;
    }
}

ECode CLocalActivityManager::GetActivity(
    /* [in] */ const String& id,
    /* [out] */ IActivity** activity)
{
    VALIDATE_NOT_NULL(activity);

    Map<String, LocalActivityRecord* >::Iterator it = mActivities.Find(id);
    LocalActivityRecord* r = it->mSecond;
    if (r != NULL) {
        *activity = r->mActivity;
        return NOERROR;
    }
    else {
        *activity = NULL;
        return NOERROR;
    }
}

ECode CLocalActivityManager::DispatchCreate(
    /* [in] */ IBundle* state)
{
    if (state != NULL) {
        AutoPtr<IObjectEnumerator> i;
        AutoPtr<IObjectContainer>  container;
        state->KeySet((IObjectContainer**)&container);
        container->GetObjectEnumerator((IObjectEnumerator**)&i);
        Boolean hasNext = FALSE;
        while (i->MoveNext(&hasNext), hasNext) {
//            try {
            String id;
            i->Current((PInterface*)&id);
            AutoPtr<IBundle> astate;
            state->GetBundle(id, (IBundle**)&astate);
            Map<String, LocalActivityRecord* >::Iterator it = mActivities.Find(id);
            LocalActivityRecord* r = it->mSecond;
            if (r != NULL) {
                r->mInstanceState = astate;
            }
            else {
                r = new LocalActivityRecord(id, NULL);
                r->mInstanceState = astate;
                mActivities.Insert(Map<String, LocalActivityRecord* >::ValueType(id, r));
                mActivityArray.PushBack(r);
            }
//        } catch (Exception e) {
//                    // Recover from -all- app errors.
//                    Log.e(TAG,
//                          "Exception thrown when restoring LocalActivityManager state",
//                          e);
//            }
        }
    }
        
    mCurState = CREATED;
    return NOERROR;
}

ECode CLocalActivityManager::SaveInstanceState(
    /* [out] */ IBundle** st)
{
    VALIDATE_NOT_NULL(st);

    AutoPtr<IBundle> state = NULL;

    // FIXME: child activities will freeze as part of onPaused. Do we
    // need to do this here?
    List<LocalActivityRecord*>::Iterator it;
    for (it = mActivityArray.Begin(); it != mActivityArray.End(); ++it) {
        LocalActivityRecord* r = *it;
        if (state == NULL) {
            CBundle::New((IBundle**)&state);
        }
        if ((r->mInstanceState != NULL || r->mCurState == RESUMED)
                && r->mActivity != NULL) {
            // We need to save the state now, if we don't currently
            // already have it or the activity is currently resumed.
            AutoPtr<IBundle> childState;
            CBundle::New((IBundle**)&childState);
            r->mActivity->OnSaveInstanceState(childState);
            r->mInstanceState = childState;
        }
        if (r->mInstanceState != NULL) {
            state->PutBundle(r->mId, r->mInstanceState);
        }
    }

    *st = state;
    return NOERROR;
}

ECode CLocalActivityManager::DispatchResume()
{
    mCurState = RESUMED;
    if (mSingleMode) {
        if (mResumed != NULL) {
            MoveToState(mResumed, RESUMED);
        }
    }
    else {
        List<LocalActivityRecord*>::Iterator it;
        for (it = mActivityArray.Begin(); it != mActivityArray.End(); ++it) {
            LocalActivityRecord* r = *it;
            MoveToState(r, RESUMED);
        }
    }
    return NOERROR;
}

ECode CLocalActivityManager::DispatchPause(
    /* [in] */ Boolean finishing)
{
    if (finishing) {
        mFinishing = TRUE;
    }
    mCurState = STARTED;
    if (mSingleMode) {
        if (mResumed != NULL) {
            MoveToState(mResumed, STARTED);
        }
    }
    else {
        List<LocalActivityRecord*>::Iterator it;
        for (it = mActivityArray.Begin(); it != mActivityArray.End(); ++it) {
            LocalActivityRecord* r = *it;
            if (r->mCurState == RESUMED) {
                MoveToState(r, STARTED);
            }
        }
    }
    return NOERROR;
}

ECode CLocalActivityManager::DispatchStop()
{
    mCurState = CREATED;
    List<LocalActivityRecord*>::Iterator it;
    for (it = mActivityArray.Begin(); it != mActivityArray.End(); ++it) {
        LocalActivityRecord* r = *it;
        MoveToState(r, CREATED);
    }
    return NOERROR;
}

ECode CLocalActivityManager::DispatchRetainNonConfigurationInstance(
    /* [out] */ IObjectStringMap** results)
{
    VALIDATE_NOT_NULL(results);

    AutoPtr<IObjectStringMap> instanceMap = NULL;

    List<LocalActivityRecord*>::Iterator it;
    for (it = mActivityArray.Begin(); it != mActivityArray.End(); ++it) {
        LocalActivityRecord* r = *it;
        if ((r != NULL) && (r->mActivity != NULL)) {
            AutoPtr<IInterface> instance;
            r->mActivity->OnRetainNonConfigurationInstance((IInterface**)&instance);
            if (instance != NULL) {
                if (instanceMap == NULL) {
                    instanceMap = AutoPtr<IObjectStringMap>();
                }
                instanceMap->Put(r->mId, instance);
            }
        }
    }
    *results = instanceMap;
    return NOERROR;
}

ECode CLocalActivityManager::RemoveAllActivities()
{
    return DispatchDestroy(TRUE);
}

ECode CLocalActivityManager::DispatchDestroy(
    /* [in] */ Boolean finishing)
{
    List<LocalActivityRecord*>::Iterator it;
    for (it = mActivityArray.Begin(); it != mActivityArray.End(); ++it) {
        LocalActivityRecord* r = *it;
        if (localLOGV) Slogger::V(TAG, r->mId + ": destroying");
//        mActivityThread.performDestroyActivity(r, finishing);
    }
    mActivities.Clear();
    mActivityArray.Clear();
    return NOERROR;
}
