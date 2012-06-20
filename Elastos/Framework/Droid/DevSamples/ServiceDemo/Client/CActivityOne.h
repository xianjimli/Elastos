
#ifndef __CACTIVITYONE_H__
#define __CACTIVITYONE_H__

#include "CActivity.h"
#include "CActivityOneServiceConnection.h"
#include "_CActivityOne.h"

class CActivityOne : public CActivity
{
public:

protected:
    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnStart();

    CARAPI OnResume();

    CARAPI OnPause();

    CARAPI OnStop();

    CARAPI OnDestroy();

private:
    ECode GFXCALLBACKTYPE OnClick(
    	/* [in] */ Handle32 serverHandle);

    ECode GFXCALLBACKTYPE OnClick1(
    	/* [in] */ Handle32 serverHandle);

    ECode GFXCALLBACKTYPE OnClick2(
    	/* [in] */ Handle32 serverHandle);

    CARAPI OnActivityResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent *data);
};

#endif // __CACTIVITYONE_H__
