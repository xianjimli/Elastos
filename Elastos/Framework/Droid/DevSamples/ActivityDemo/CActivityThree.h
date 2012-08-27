
#ifndef __CACTIVITYTHREE_H__
#define __CACTIVITYTHREE_H__

#include "Activity.h"
#include "_CActivityThree.h"
#include <elastos/AutoPtr.h>

class CActivityThree : public Activity
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

#endif // __CACTIVITYTHREE_H__
