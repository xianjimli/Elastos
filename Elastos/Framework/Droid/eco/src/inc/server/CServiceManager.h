
#ifndef __CSERVICEMANAGER_H__
#define __CSERVICEMANAGER_H__

#include "_CServiceManager.h"
#include "ext/frameworkdef.h"
#include <elastos/HashMap.h>
#include <elastos/AutoPtr.h>
#include <binder/Binder.h>

using namespace Elastos;

CarClass(CServiceManager)
{
public:
    CServiceManager();

    ~CServiceManager();

    CARAPI AddService(
        /* [in] */ const String& name,
        /* [in] */ IInterface * service);

    CARAPI GetService(
        /* [in] */ const String& name,
        /* [out] */ IInterface ** service);

    CARAPI RemoveService(
        /* [in] */ const String& name);

public:
    CARAPI InitServiceCache(
        /* [in] */ IObjectStringMap* services);

private:
    enum {
        ADD_SERVICE = android::IBinder::FIRST_CALL_TRANSACTION,
        GET_SERVICE,
        REMOVE_SERVICE,
    };

    HashMap<String, AutoPtr<IInterface> > mServiceCache;
};

#endif // __CSERVICEMANAGER_H__
