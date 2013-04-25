
#ifndef __CACTIVITYMONITOR_H__
#define __CACTIVITYMONITOR_H__

#include "_CActivityMonitor.h"
#include <elastos/AutoPtr.h>


CarClass(CActivityMonitor)
{
public:
    /**
     * Create a new ActivityMonitor that looks for a particular kind of 
     * intent to be started.
     *  
     * @param which The set of intents this monitor is responsible for.
     * @param result A canned result to return if the monitor is hit; can 
     *               be null.
     * @param block Controls whether the monitor should block the activity 
     *              start (returning its canned result) or let the call
     *              proceed.
     *  
     * @see Instrumentation#addMonitor 
     */
    CARAPI constructor(
        /* [in] */ IIntentFilter* which,
        /* [in] */ IActivityResult* result,
        /* [in] */ Boolean block);

    /**
     * Create a new ActivityMonitor that looks for a specific activity 
     * class to be started. 
     *  
     * @param cls The activity class this monitor is responsible for.
     * @param result A canned result to return if the monitor is hit; can 
     *               be null.
     * @param block Controls whether the monitor should block the activity 
     *              start (returning its canned result) or let the call
     *              proceed.
     *  
     * @see Instrumentation#addMonitor 
     */
    CARAPI constructor(
        /* [in] */ const String& cls,
        /* [in] */ IActivityResult* result,
        /* [in] */ Boolean block);

    /**
     * Retrieve the filter associated with this ActivityMonitor.
     */
    CARAPI GetFilter(
        /* [out] */ IIntentFilter** filter);

    /**
     * Retrieve the result associated with this ActivityMonitor, or null if 
     * none. 
     */
    CARAPI GetResult(
        /* [out] */ IActivityResult** result);

    /**
     * Check whether this monitor blocks activity starts (not allowing the 
     * actual activity to run) or allows them to execute normally. 
     */
    CARAPI IsBlocking(
        /* [out] */ Boolean* result);

    /**
     * Retrieve the number of times the monitor has been hit so far.
     */
    CARAPI GetHits(
        /* [out] */ Int32* time);

    /**
     * Retrieve the most recent activity class that was seen by this 
     * monitor. 
     */
    CARAPI GetLastActivity(
        /* [out] */ IActivity** activity);

    /**
     * Block until an Activity is created that matches this monitor, 
     * returning the resulting activity. 
     * 
     * @return Activity
     */
    CARAPI WaitForActivity(
        /* [out] */ IActivity** activity);

    /**
     * Block until an Activity is created that matches this monitor, 
     * returning the resulting activity or till the timeOut period expires.
     * If the timeOut expires before the activity is started, return null. 
     * 
     * @param timeOut Time to wait before the activity is created.
     * 
     * @return Activity
     */
    CARAPI WaitForActivityWithTimeout(
        /* [in] */ Int64 timeOut,
        /* [out] */ IActivity** activity);

public:
    CARAPI_(Boolean) Match(
        /* [in] */ IContext* who,
        /* [in] */ IActivity* activity,
        /* [in] */ IIntent* intent);

public:
    // This is protected by 'Instrumentation.this.mSync'.
    /*package*/
    Int32 mHits;

    // This is protected by 'this'.
    /*package*/
    AutoPtr<IActivity> mLastActivity;

private:
    AutoPtr<IIntentFilter> mWhich;
    String mClass;
    AutoPtr<IActivityResult> mResult;
    Boolean mBlock;
    Mutex mLock;
};

#endif //__CDEFAULTINSTRUMENTATIONACTIVITYMONITOR_H__