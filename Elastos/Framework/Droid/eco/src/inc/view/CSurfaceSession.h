
#ifndef __CSURFACESESSION_H__
#define __CSURFACESESSION_H__

#include "_CSurfaceSession.h"
#include "ext/frameworkext.h"
#ifdef _linux
#include <surfaceflinger/SurfaceComposerClient.h>
#endif

CarClass(CSurfaceSession)
{
    friend class CSurface;

public:
    CSurfaceSession();

    /** Forcibly detach native resources associated with this object.
     *  Unlike destroy(), after this call any surfaces that were created
     *  from the session will no longer work. The session itself is destroyed.
     */
    CARAPI Kill();

private:
    /* no user serviceable parts here ... */
    CARAPI Finalize();
    CARAPI Init();
    CARAPI Destroy();

private:
#ifdef _linux
    android::sp<android::SurfaceComposerClient> mClient;
#endif
};

#endif //__CSURFACESESSION_H__
