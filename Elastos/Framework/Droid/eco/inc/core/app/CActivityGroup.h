
#ifndef __CACTIVITYGROUP_H__
#define __CACTIVITYGROUP_H__

#include "_CActivityGroup.h"
#include "app/Activity.h"
#include "app/CLocalActivityManager.h"


CarClass(CActivityGroup), public Activity
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Boolean singleActivityMode);

    virtual CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI OnResume();

    virtual CARAPI OnSaveInstanceState(
        /* [in] */ IBundle* outState);

    virtual CARAPI OnPause();

    virtual CARAPI OnStop();

    virtual CARAPI OnDestroy();

    virtual CARAPI OnRetainNonConfigurationChildInstances(
        /* [out] */ IObjectStringMap** stringmap);

    CARAPI GetCurrentActivity(
        /* [out] */ IActivity** activity);

    CARAPI GetLocalActivityManager(
        /* [out] */ ILocalActivityManager** lmananger);

    virtual CARAPI DispatchActivityResult(
        /* [in] */ const String& who,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* data);

public:
    static const String PARENT_NON_CONFIG_INSTANCE_KEY;

    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    AutoPtr<CLocalActivityManager> mLocalActivityManager;

private:
    static const CString TAG;

    static const String STATES_KEY;
};

#endif //__CACTIVITYGROUP_H__

