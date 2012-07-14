
#ifndef __SERVICEMANAGER_H__
#define __SERVICEMANAGER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>

class ServiceManager
{
public:
    /**
     * Returns a reference to a service with the given name.
     *
     * @param name the name of the service to get
     * @return a reference to the service, or <code>null</code> if the service doesn't exist
     */
    static CARAPI_(AutoPtr<IInterface>) GetService(
        /* [in] */ const String& name);

    /**
     * Place a new @a service called @a name into the service
     * manager.
     *
     * @param name the name of the new service
     * @param service the service object
     */
    static CARAPI_(void) AddService(
        /* [in] */ const String& name,
        /* [in] */ IInterface* service);

    /**
     * Retrieve an existing service called @a name from the
     * service manager.  Non-blocking.
     */
    static CARAPI_(AutoPtr<IInterface>) CheckService(
        /* [in] */ const String& name);

    /**
     * Return a list of all currently running services.
     */
    static CARAPI_(void) ListServices(
        /* [out, callee] */ ArrayOf<String>** services);

    /**
     * This is only intended to be called when the process is first being brought
     * up and bound by the activity manager. There is only one thread in the process
     * at that time, so no locking is done.
     *
     * @param cache the cache of service references
     * @hide
     */
    static CARAPI InitServiceCache(
        /* [in] */ IObjectStringMap* cache);

private:
    static CARAPI_(AutoPtr<IServiceManager>) GetIServiceManager();

private:
    static const CString TAG;

    static AutoPtr<IServiceManager> sServiceManager;
    static HashMap<String, AutoPtr<IInterface> > sCache;
};

#endif //__SERVICEMANAGER_H__
