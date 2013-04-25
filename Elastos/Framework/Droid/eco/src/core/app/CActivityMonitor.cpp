
#include "app/CActivityMonitor.h"


ECode CActivityMonitor::constructor(
    /* [in] */ IIntentFilter* which,
    /* [in] */ IActivityResult* result,
    /* [in] */ Boolean block)
{
    mWhich = which;
    mClass = String(NULL);
    mResult = result;
    mBlock = block;
    return NOERROR;
}

ECode CActivityMonitor::constructor(
    /* [in] */ const String& cls,
    /* [in] */ IActivityResult* result,
    /* [in] */ Boolean block)
{
    mWhich = NULL;
    mClass = cls;
    mResult = result;
    mBlock = block;
    return NOERROR;
}

ECode CActivityMonitor::GetFilter(
    /* [out] */ IIntentFilter** filter)
{
    *filter = mWhich;
    return NOERROR;
}

ECode CActivityMonitor::GetResult(
    /* [out] */ IActivityResult** result)
{
    *result = mResult;
    return NOERROR;
}

ECode CActivityMonitor::IsBlocking(
    /* [out] */ Boolean* result)
{
    *result = mBlock;
    return NOERROR;
}

ECode CActivityMonitor::GetHits(
    /* [out] */ Int32* time)
{
    *time = mHits;
    return NOERROR;
}

ECode CActivityMonitor::GetLastActivity(
    /* [out] */ IActivity** activity)
{
    *activity = mLastActivity;
    return NOERROR;
}

ECode CActivityMonitor::WaitForActivity(
    /* [out] */ IActivity** activity)
{
    Mutex::Autolock lock(mLock);
    while (mLastActivity == NULL) {
//        try {
//                wait();
//            } catch (InterruptedException e) {
//            }
    }
    AutoPtr<IActivity> res = mLastActivity;
    mLastActivity = NULL;
    *activity = res;
    return NOERROR;
}

ECode CActivityMonitor::WaitForActivityWithTimeout(
    /* [in] */ Int64 timeOut,
    /* [out] */ IActivity** activity)
{
    Mutex::Autolock lock(mLock);
//    try {
//        wait(timeOut);
//    } catch (InterruptedException e) {
//    }
    if (mLastActivity == NULL) {
        *activity = NULL;
        return NOERROR;
    } else {
        AutoPtr<IActivity> res = mLastActivity;
        mLastActivity = NULL;
        *activity = res;
        return NOERROR;
    }
}

Boolean CActivityMonitor::Match(
    /* [in] */ IContext* who,
    /* [in] */ IActivity* activity,
    /* [in] */ IIntent* intent)
{
    Mutex::Autolock lock(mLock);
    AutoPtr<IContentResolver> resolver;
    who->GetContentResolver((IContentResolver**)&resolver);
    Int32 match;
    mWhich->MatchEx(resolver, intent, TRUE, String("Instrumentation"), &match);
    if (mWhich != NULL && match < 0) {
        return FALSE;
    }
    if (mClass != NULL) {
        String cls = String(NULL);
        AutoPtr<IComponentName> component;
        intent->GetComponent((IComponentName**)&component);
        if (activity != NULL) {
//            cls = activity.getClass().getName();
        } else if (component != NULL) {
            component->GetClassName(&cls);
        }
        if (cls == NULL || !mClass.Equals(cls)) {
            return FALSE;
        }
    }
    if (activity != NULL) {
        mLastActivity = activity;
//        notifyAll();
    }
    return TRUE;
}
