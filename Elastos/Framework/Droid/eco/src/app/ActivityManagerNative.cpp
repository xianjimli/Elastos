
#include "app/ActivityManagerNative.h"
#include <elastos/AutoPtr.h>

ECode ActivityManagerNative::GetDefault(
    /* [out] */ IActivityManager** service)
{
    if (service == NULL) return E_INVALID_ARGUMENT;

    AutoPtr<IServiceManager> sm;
    Elastos::GetServiceManager((IServiceManager**)&sm);
    return sm->GetService(String("ActivityManagerService"), (IInterface**)service);
}
