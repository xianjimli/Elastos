
#ifndef __CAPPLICATION_H__
#define __CAPPLICATION_H__

#include "_CApplication.h"
#include "content/ContextWrapper.h"
#include "content/ContextMacro.h"

CarClass(CApplication), public ContextWrapper
{
public:
    ICONTEXT_METHODS_DECL();

    CARAPI constructor();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetBaseContext(
        /* [out] */ IContext** ctx);

    CARAPI Create();

    /**
     * Called when the application is starting, before any other application
     * objects have been created.  Implementations should be as quick as
     * possible (for example using lazy initialization of state) since the time
     * spent in this function directly impacts the performance of starting the
     * first activity, service, or receiver in a process.
     * If you override this method, be sure to call super.onCreate().
     */
    CARAPI OnCreate();

    /**
     * This method is for use in emulated process environments.  It will
     * never be called on a production Android device, where processes are
     * removed by simply killing them; no user code (including this callback)
     * is executed when doing so.
     */
    CARAPI OnTerminate();

    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI OnLowMemory();

    // ------------------ Internal API ------------------

    /**
     * @hide
     */
    /* package */ CARAPI_(Void) Attach(
        /* [in] */ IContext* context);
};

#endif //__CAPPLICATION_H__
