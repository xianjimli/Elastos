
#include "SystemServer.h"
#include <Elastos.Framework.Server.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

ECode SystemServer::Init()
{
    ECode ec = NOERROR;
    AutoPtr<IServiceManager> serviceManager;
    AutoPtr<IActivityManager> activityManagerService;
    AutoPtr<ICapsuleManager> capsuleManager;
    AutoPtr<IWindowManagerStub> windowManager;

    AutoPtr<IApartment> apartment;
    CApartment::New(FALSE, (IApartment**)&apartment);
    apartment->Start(ApartmentAttr_New);

    CServiceManager::AcquireSingleton((IServiceManager**)&serviceManager);

    CCapsuleManagerService::New((ICapsuleManager**)&capsuleManager);
    ec = serviceManager->AddService(String("capsule"), capsuleManager.Get());
    if (FAILED(ec)) return ec;

    CWindowManagerService::New((IWindowManagerStub**)&windowManager);
    ec = serviceManager->AddService(String("window"), windowManager.Get());
    if (FAILED(ec)) return ec;

    CActivityManagerService::New(windowManager.Get(), (IActivityManager**)&activityManagerService);
    return serviceManager->AddService(String("ActivityManagerService"), activityManagerService.Get());
}
