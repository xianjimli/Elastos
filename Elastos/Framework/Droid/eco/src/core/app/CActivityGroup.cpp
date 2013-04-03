
#include "app/CActivityGroup.h"
#include "app/CLocalActivityManager.h"


const CString CActivityGroup::TAG = "ActivityGroup";
const String CActivityGroup::STATES_KEY = String("android:states");
const String CActivityGroup::PARENT_NON_CONFIG_INSTANCE_KEY = String("android:parent_non_config_instance");

ECode CActivityGroup::constructor()
{
    return constructor(TRUE);
}

ECode CActivityGroup::constructor(
    /* [in] */ Boolean singleActivityMode)
{
    return CLocalActivityManager::New(this, singleActivityMode, (ILocalActivityManager**)&mLocalActivityManager);
}

ECode CActivityGroup::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    Activity::OnCreate(savedInstanceState);
    AutoPtr<IBundle> bundle;
    savedInstanceState->GetBundle(STATES_KEY, (IBundle**)&bundle);
    AutoPtr<IBundle> states = savedInstanceState != NULL ? bundle : NULL;
    mLocalActivityManager->DispatchCreate(states);
    return NOERROR;
}

ECode CActivityGroup::OnResume()
{
    Activity::OnResume();
    mLocalActivityManager->DispatchResume();
    return NOERROR;
}

ECode CActivityGroup::OnSaveInstanceState(
    /* [in] */ IBundle* outState)
{
    Activity::OnSaveInstanceState(outState);
    AutoPtr<IBundle> state;
    mLocalActivityManager->SaveInstanceState((IBundle**)&state);
    if (state != NULL) {
        outState->PutBundle(STATES_KEY, state);
    }
    return NOERROR;
}

ECode CActivityGroup::OnPause()
{
    Activity::OnPause();
    Boolean finished;
    IsFinishing(&finished);
    mLocalActivityManager->DispatchPause(finished);
    return NOERROR;
}

ECode CActivityGroup::OnStop()
{
    Activity::OnStop();
    mLocalActivityManager->DispatchStop();
    return NOERROR;
}

ECode CActivityGroup::OnDestroy()
{
    Activity::OnDestroy();
    Boolean finished;
    IsFinishing(&finished);
    mLocalActivityManager->DispatchDestroy(finished);
    return NOERROR;
}

ECode CActivityGroup::OnRetainNonConfigurationChildInstances(
    /* [out] */ IObjectStringMap** stringmap)
{
    VALIDATE_NOT_NULL(stringmap);

    return mLocalActivityManager->DispatchRetainNonConfigurationInstance(stringmap);
}

ECode CActivityGroup::GetCurrentActivity(
    /* [out] */ IActivity** activity)
{
    VALIDATE_NOT_NULL(activity);

    return mLocalActivityManager->GetCurrentActivity(activity);
}

ECode CActivityGroup::GetLocalActivityManager(
    /* [out] */ ILocalActivityManager** lmananger)
{
    VALIDATE_NOT_NULL(lmananger);

    *lmananger = mLocalActivityManager;
    return NOERROR;
}

ECode CActivityGroup::DispatchActivityResult(
    /* [in] */ const String& who,
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* data)
{
    if (who != String(NULL)) {
        AutoPtr<Activity> act;
        mLocalActivityManager->GetActivity(who, (IActivity**)&act);
        /*
        if (Config.LOGV) Log.v(
            TAG, "Dispatching result: who=" + who + ", reqCode=" + requestCode
            + ", resCode=" + resultCode + ", data=" + data
            + ", rec=" + rec);
        */
        if (act != NULL) {
            act->OnActivityResult(requestCode, resultCode, data);
            return NOERROR;
        }
    }
    Activity::DispatchActivityResult(who, requestCode, resultCode, data);
    return NOERROR;
}