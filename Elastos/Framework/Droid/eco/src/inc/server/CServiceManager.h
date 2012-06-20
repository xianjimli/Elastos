
#ifndef __CSERVICEMANAGER_H__
#define __CSERVICEMANAGER_H__

#include "_CServiceManager.h"
#include "ext/frameworkdef.h"
#include <elastos/HashMap.h>
#include <elastos/AutoPtr.h>
#ifdef _linux
#include <binder/Binder.h>
#endif

using namespace Elastos;

CarClass(CServiceManager)
{
public:
    CServiceManager();

    ~CServiceManager();

    CARAPI AddService(
        /* [in] */ String name,
        /* [in] */ IInterface * service);

    CARAPI GetService(
        /* [in] */ String name,
        /* [out] */ IInterface ** service);

    CARAPI RemoveService(
        /* [in] */ String name);

public:
    CARAPI InitServiceCache(
        /* [in] */ IObjectStringMap* services);

private:
#ifdef _linux
    enum {
        ADD_SERVICE = android::IBinder::FIRST_CALL_TRANSACTION,
        GET_SERVICE,
        REMOVE_SERVICE,
    };
#endif
    HashMap<String, AutoPtr<IInterface> > mServiceCache;
};

#endif // __CSERVICEMANAGER_H__
