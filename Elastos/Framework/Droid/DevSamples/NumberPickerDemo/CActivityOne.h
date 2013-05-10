
#ifndef __CACTIVITYONE_H__
#define __CACTIVITYONE_H__

#include "Activity.h"
#include "_CActivityOne.h"

class CActivityOne : public Activity
{

protected:
    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnStart();

    CARAPI OnResume();

    CARAPI OnPause();

    CARAPI OnStop();

    CARAPI OnDestroy();

private:
    CARAPI OnActivityResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent *data);

private:
};

#endif // __CACTIVITYONE_H__
