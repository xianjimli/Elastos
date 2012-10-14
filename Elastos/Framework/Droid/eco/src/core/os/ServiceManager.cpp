
#include "os/ServiceManager.h"
#ifdef _FRAMEWORK_CORE
#include "os/CServiceManager.h"
#elif defined(_FRAMEWORK_SERVER)
#include "Elastos.Framework.Core.h"
#endif

const CString ServiceManager::TAG = "ServiceManager";
AutoPtr<IServiceManager> ServiceManager::sServiceManager;
HashMap<String, AutoPtr<IInterface> > ServiceManager::sCache(11);

AutoPtr<IServiceManager> ServiceManager::GetIServiceManager()
{
    if (sServiceManager != NULL) {
        return sServiceManager;
    }

    // Find the service manager
    ASSERT_SUCCEEDED(CServiceManager::AcquireSingleton((IServiceManager**)&sServiceManager));
    return sServiceManager;
}

AutoPtr<IInterface> ServiceManager::GetService(
    /* [in] */ const String& name)
{
//    try {
    HashMap<String, AutoPtr<IInterface> >::Iterator it = sCache.Find(name);
    if (it != sCache.End()) {
        return it->mSecond;
    }
    else {
        AutoPtr<IInterface> obj;
        ECode ec = GetIServiceManager()->GetService(name, (IInterface**)&obj);
        if (SUCCEEDED(ec)) {
            return obj;
        }
        else return NULL;
    }
//    } catch (RemoteException e) {
//        Log.e(TAG, "error in getService", e);
//    }
}

void ServiceManager::AddService(
    /* [in] */ const String& name,
    /* [in] */ IInterface* service)
{
//    try {
    GetIServiceManager()->AddService(name, service);
//    } catch (RemoteException e) {
//        Log.e(TAG, "error in addService", e);
//    }
}

AutoPtr<IInterface> ServiceManager::CheckService(
    /* [in] */ const String& name)
{
//    try {
//        IBinder service = sCache.get(name);
//        if (service != null) {
//            return service;
//        } else {
//            return getIServiceManager().checkService(name);
//        }
//    } catch (RemoteException e) {
//        Log.e(TAG, "error in checkService", e);
//        return null;
//    }
    return NULL;
}

void ServiceManager::ListServices(
    /* [out, callee] */ ArrayOf<String>** services)
{
//    try {
//        return getIServiceManager().listServices();
//    } catch (RemoteException e) {
//        Log.e(TAG, "error in listServices", e);
//        return null;
//    }
}

ECode ServiceManager::InitServiceCache(
    /* [in] */ IObjectStringMap* cache)
{
//    if (sCache.size() != 0 && Process.supportsProcesses()) {
//        throw new IllegalStateException("setServiceCache may only be called once");
//    }
//    sCache.putAll(cache);
    return E_NOT_IMPLEMENTED;
}
