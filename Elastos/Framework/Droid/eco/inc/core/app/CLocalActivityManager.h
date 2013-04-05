
#ifndef __CLOCALACTIVITYMANAGER_H__
#define __CLOCALACTIVITYMANAGER_H__

#include "_CLocalActivityManager.h"
#include "os/Binder.h"
#include "capsule/CActivityInfo.h"
#include "app/CApplicationApartment.h"
#include "app/Activity.h"
#include <elastos/AutoPtr.h>


CarClass(CLocalActivityManager)
{
private:
    class LocalActivityRecord : public Binder
    {
    public:
        LocalActivityRecord(
            /* [in] */ const String& id,
            /* [in] */ IIntent* intent);

        ~LocalActivityRecord();

    public:
        const String mId;                          // Unique name of this record.
        AutoPtr<IIntent> mIntent;                  // Which activity to run here.
        AutoPtr<IActivityInfo> mActivityInfo;      // Package manager info about activity.
        AutoPtr<Activity> mActivity;              // Currently instantiated activity.
        AutoPtr<IWindow> mWindow;                   // Activity's top-level window.
        AutoPtr<IBundle> mInstanceState;           // Last retrieved freeze state.
        Int32 mCurState;                           // Current state the activity is in.
    };

public:
    /** String ids of running activities starting with least recently used. */
    // TODO: put back in stopping of activities.
    //private List<LocalActivityRecord>  mLRU = new ArrayList();

    /**
     * Create a new LocalActivityManager for holding activities running within
     * the given <var>parent</var>.
     * 
     * @param parent the host of the embedded activities
     * @param singleMode True if the LocalActivityManger should keep a maximum
     * of one activity resumed
     */
    CARAPI constructor(
        /* [in] */ IActivity* parent,
        /* [in] */ Boolean singleMode);

    /**
     * Start a new activity running in the group.  Every activity you start
     * must have a unique string ID associated with it -- this is used to keep
     * track of the activity, so that if you later call startActivity() again
     * on it the same activity object will be retained.
     * 
     * <p>When there had previously been an activity started under this id,
     * it may either be destroyed and a new one started, or the current
     * one re-used, based on these conditions, in order:</p>
     * 
     * <ul>
     * <li> If the Intent maps to a different activity component than is
     * currently running, the current activity is finished and a new one
     * started.
     * <li> If the current activity uses a non-multiple launch mode (such
     * as singleTop), or the Intent has the
     * {@link Intent#FLAG_ACTIVITY_SINGLE_TOP} flag set, then the current
     * activity will remain running and its
     * {@link Activity#onNewIntent(Intent) Activity.onNewIntent()} method
     * called.
     * <li> If the new Intent is the same (excluding extras) as the previous
     * one, and the new Intent does not have the
     * {@link Intent#FLAG_ACTIVITY_CLEAR_TOP} set, then the current activity
     * will remain running as-is.
     * <li> Otherwise, the current activity will be finished and a new
     * one started.
     * </ul>
     * 
     * <p>If the given Intent can not be resolved to an available Activity,
     * this method throws {@link android.content.ActivityNotFoundException}.
     * 
     * <p>Warning: There is an issue where, if the Intent does not
     * include an explicit component, we can restore the state for a different
     * activity class than was previously running when the state was saved (if
     * the set of available activities changes between those points).
     * 
     * @param id Unique identifier of the activity to be started
     * @param intent The Intent describing the activity to be started
     * 
     * @return Returns the window of the activity.  The caller needs to take
     * care of adding this window to a view hierarchy, and likewise dealing
     * with removing the old window if the activity has changed.
     * 
     * @throws android.content.ActivityNotFoundException
     */
    CARAPI StartActivity(
        /* [in] */ const String& id,
        /* [in] */ IIntent* intent,
        /* [out] */ IWindow** window);

    /**
     * Destroy the activity associated with a particular id.  This activity
     * will go through the normal lifecycle events and fine onDestroy(), and
     * then the id removed from the group.
     * 
     * @param id Unique identifier of the activity to be destroyed
     * @param finish If true, this activity will be finished, so its id and
     * all state are removed from the group.
     * 
     * @return Returns the window that was used to display the activity, or
     * null if there was none.
     */
    CARAPI DestroyActivity(
        /* [in] */ const String& id,
        /* [in] */ Boolean finish,
        /* [out] */ IWindow** window);

    /**
     * Retrieve the Activity that is currently running.
     * 
     * @return the currently running (resumed) Activity, or null if there is
     *         not one
     * 
     * @see #startActivity
     * @see #getCurrentId
     */
    CARAPI GetCurrentActivity(
        /* [out] */ IActivity** activity);

    /**
     * Retrieve the ID of the activity that is currently running.
     * 
     * @return the ID of the currently running (resumed) Activity, or null if
     *         there is not one
     * 
     * @see #startActivity
     * @see #getCurrentActivity
     */
    CARAPI GetCurrentId(
        /* [out] */ String* id);

    /**
     * Return the Activity object associated with a string ID.
     * 
     * @see #startActivity
     * 
     * @return the associated Activity object, or null if the id is unknown or
     *         its activity is not currently instantiated
     */
    CARAPI GetActivity(
        /* [in] */ const String& id,
        /* [out] */ IActivity** activity);

    /**
     * Restore a state that was previously returned by {@link #saveInstanceState}.  This
     * adds to the activity group information about all activity IDs that had
     * previously been saved, even if they have not been started yet, so if the
     * user later navigates to them the correct state will be restored.
     * 
     * <p>Note: This does <b>not</b> change the current running activity, or
     * start whatever activity was previously running when the state was saved.
     * That is up to the client to do, in whatever way it thinks is best.
     * 
     * @param state a previously saved state; does nothing if this is null
     * 
     * @see #saveInstanceState
     */
    CARAPI DispatchCreate(
        /* [in] */ IBundle* state);

    /**
     * Retrieve the state of all activities known by the group.  For
     * activities that have previously run and are now stopped or finished, the
     * last saved state is used.  For the current running activity, its
     * {@link Activity#onSaveInstanceState} is called to retrieve its current state.
     * 
     * @return a Bundle holding the newly created state of all known activities
     * 
     * @see #dispatchCreate
     */
    CARAPI SaveInstanceState(
        /* [out] */ IBundle** st);

    /**
     * Called by the container activity in its {@link Activity#onResume} so
     * that LocalActivityManager can perform the corresponding action on the
     * activities it holds.
     * 
     * @see Activity#onResume
     */
    CARAPI DispatchResume();

    /**
     * Called by the container activity in its {@link Activity#onPause} so
     * that LocalActivityManager can perform the corresponding action on the
     * activities it holds.
     * 
     * @param finishing set to true if the parent activity has been finished;
     *                  this can be determined by calling
     *                  Activity.isFinishing()
     * 
     * @see Activity#onPause
     * @see Activity#isFinishing
     */
    CARAPI DispatchPause(
        /* [in] */ Boolean finishing);

    /**
     * Called by the container activity in its {@link Activity#onStop} so
     * that LocalActivityManager can perform the corresponding action on the
     * activities it holds.
     * 
     * @see Activity#onStop
     */
    CARAPI DispatchStop();

    /**
     * Call onRetainNonConfigurationInstance on each child activity and store the
     * results in a HashMap by id.  Only construct the HashMap if there is a non-null
     * object to store.  Note that this does not support nested ActivityGroups.
     * 
     * {@hide}
     */
    CARAPI DispatchRetainNonConfigurationInstance(
        /* [out] */ IObjectStringMap** results);

    /**
     * Remove all activities from this LocalActivityManager, performing an
     * {@link Activity#onDestroy} on any that are currently instantiated.
     */
    CARAPI RemoveAllActivities();

    /**
     * Called by the container activity in its {@link Activity#onDestroy} so
     * that LocalActivityManager can perform the corresponding action on the
     * activities it holds.
     * 
     * @see Activity#onDestroy
     */
    CARAPI DispatchDestroy(
        /* [in] */ Boolean finishing);

private:
    CARAPI_(void) MoveToState(
        /* [in] */ LocalActivityRecord* r,
        /* [in] */ Int32 desiredState);

    CARAPI_(void) PerformPause(
        /* [in] */ LocalActivityRecord* r,
        /* [in] */ Boolean finishing);

    CARAPI_(AutoPtr<IWindow>) PerformDestroy(
        /* [in] */ LocalActivityRecord* r,
        /* [in] */ Boolean finish);

private:
    static const CString TAG;
    static const Boolean localLOGV;

    const static Int32 RESTORED = 0;      // State restored, but no startActivity().

    const static Int32 INITIALIZING = 1;  // Ready to launch (after startActivity()).

    const static Int32 CREATED = 2;       // Created, not started or resumed.

    const static Int32 STARTED = 3;       // Created and started, not resumed.

    const static Int32 RESUMED = 4;       // Created started and resumed.

    const static Int32 DESTROYED = 5;     // No longer with us.


    /** Thread our activities are running in. */
    const AutoPtr<CApplicationApartment> mApplicationApartment;

    /** The containing activity that owns the activities we create. */
    AutoPtr<IActivity> mParent;

    /** The activity that is currently resumed. */
    LocalActivityRecord* mResumed;

    /** id -> record of all known activities. */
    Map<String, LocalActivityRecord* > mActivities;

    /** array of all known activities for easy iterating. */
    List<LocalActivityRecord*> mActivityArray;

    /** True if only one activity can be resumed at a time */
    Boolean mSingleMode;
    
    /** Set to true once we find out the container is finishing. */
    Boolean mFinishing;
    
    /** Current state the owner (ActivityGroup) is in */
    Int32 mCurState;
};

#endif //__CLOCALACTIVITYMANAGER_H__