
#ifndef __CACTIVITYONE_H__
#define __CACTIVITYONE_H__

#include "_CActivityOne.h"
#include "CBroadcastReceiverOne.h"
#include "Activity.h"

CarClass(CActivityOne), public Activity
{
public:

protected:
    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnStart();

    CARAPI OnRestart();

    CARAPI OnResume();

    CARAPI OnPause();

    CARAPI OnStop();

    CARAPI OnDestroy();

private:
    IBroadcastReceiver* mReceiver;
    IIntentFilter* mFilter;
};

#endif // __CACTIVITYONE_H__
