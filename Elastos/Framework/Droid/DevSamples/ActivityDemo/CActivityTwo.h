
#ifndef __CACTIVITYTWO_H__
#define __CACTIVITYTWO_H__

#include "Activity.h"
#include "_CActivityTwo.h"
#include <elastos/AutoPtr.h>

class CActivityTwo : public Activity
{
protected:
    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnStart();

    CARAPI OnResume();

    CARAPI OnPause();

    CARAPI OnStop();

    CARAPI OnDestroy();
};

#endif // __CACTIVITYTWO_H__
