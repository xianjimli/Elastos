
#ifndef __CACTIVITYONE_H__
#define __CACTIVITYONE_H__

#include "Activity.h"
#include "_CActivityOne.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

class CActivityOne : public Activity
{
public:
    class CMySurfaceHolderCallback:
        public ElRefBase,
        public ISurfaceHolderCallback
    {
    public:
        CMySurfaceHolderCallback(
            /* [in] */ CActivityOne* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI SurfaceCreated(
            /* [in] */ ISurfaceHolder* holder);

        CARAPI SurfaceChanged(
            /* [in] */ ISurfaceHolder* holder,
            /* [in] */ Int32 format,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        CARAPI SurfaceDestroyed(
            /* [in] */ ISurfaceHolder* holder);

    private:
        CActivityOne*     mHost;
    };

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
    AutoPtr<ISurfaceView>   mSurfaceView;
    AutoPtr<ISurfaceHolder>  mSurfaceHolder;
    AutoPtr<CMySurfaceHolderCallback>  mSurfaceHolderCallback;
};

#endif // __CACTIVITYONE_H__
