
#include "SystemServer.h"
#include <frameworkext.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

ECode SystemServer::Init()
{
    ECode ec = NOERROR;
    AutoPtr<IServiceManager> serviceManager;
    AutoPtr<IActivityManager> activityManagerService;
    AutoPtr<ICapsuleManager> capsuleManager;
    AutoPtr<IWindowManagerEx> windowManager;

    AutoPtr<IApartment> apartment;
    CApartment::New(FALSE, (IApartment**)&apartment);
    apartment->Start(ApartmentAttr_New);

    Elastos::GetServiceManager((IServiceManager**)&serviceManager);

    CCapsuleManagerService::New((ICapsuleManager**)&capsuleManager);
    ec = serviceManager->AddService("capsule", capsuleManager.Get());
    if (FAILED(ec)) return ec;

    CWindowManagerService::New((IWindowManagerEx**)&windowManager);
    ec = serviceManager->AddService("window", windowManager.Get());
    if (FAILED(ec)) return ec;

    CActivityManagerService::New(windowManager.Get(), (IActivityManager**)&activityManagerService);
    return serviceManager->AddService("ActivityManagerService", activityManagerService.Get());
}
