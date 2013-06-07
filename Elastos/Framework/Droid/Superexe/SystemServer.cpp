
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
    AutoPtr<IWindowManager> windowManager;
    AutoPtr<IInputMethodManager> inputmethodService;

    AutoPtr<IApartment> apartment;
    CApartment::New(FALSE, (IApartment**)&apartment);
    apartment->Start(ApartmentAttr_New);

    CServiceManager::AcquireSingleton((IServiceManager**)&serviceManager);

    CActivityManagerService::New((IActivityManager**)&activityManagerService);
    ec = serviceManager->AddService(String("ActivityManagerService"), activityManagerService.Get());
    if (FAILED(ec)) return ec;

    AutoPtr<IContext> ctx;
    IActivityManagerService::Probe(activityManagerService)->GetSystemContext((IContext**)&ctx);
    IActivityManagerService::Probe(activityManagerService)->SetSystemProcess();

    CWindowManagerService::New(ctx, FALSE, (IWindowManager**)&windowManager);
    ec = serviceManager->AddService(String("window"), windowManager.Get());
    if (FAILED(ec)) return ec;

    ec = IActivityManagerService::Probe(activityManagerService)->SetWindowManager(windowManager.Get());
    if (FAILED(ec)) return ec;

    CCapsuleManagerService::New(ctx, FALSE, (ICapsuleManager**)&capsuleManager);
    ec = serviceManager->AddService(String("capsule"), capsuleManager.Get());
    if (FAILED(ec)) return ec;

    AutoPtr<IClipboard> clipboardService;
    CClipboardService::New(ctx, (IClipboard**)&clipboardService);
    ec = serviceManager->AddService(String("clipboard"), clipboardService.Get());
    if (FAILED(ec)) return ec;

    CInputMethodManagerService::New(ctx, NULL, (IInputMethodManager**)&inputmethodService);
    IInputMethodManagerService::Probe(inputmethodService)->SystemReady();
    serviceManager->AddService(String(Context_INPUT_METHOD_SERVICE), inputmethodService.Get());

    AutoPtr<ILocationManagerService> locationService;
    CLocationManagerService::New(ctx, (ILocationManagerService**)&locationService);
    ec = serviceManager->AddService(String(Context_LOCATION_SERVICE), locationService.Get());
    if (FAILED(ec)) return ec;
    locationService->SystemReady();

    // BEGIN privacy-added
    AutoPtr<IPrivacySettingsManager> privacyService;
    CPrivacySettingsManagerService::New(ctx, (IPrivacySettingsManager**)&privacyService);
    ec = serviceManager->AddService(String("privacy"), privacyService.Get());
    if (FAILED(ec)) return ec;
    // END privacy-added

    AutoPtr<IContentService> contentService;
    Boolean factoryTest = TRUE;
    CContentService::New(ctx, factoryTest, (IContentService**)&contentService);
    ec = serviceManager->AddService(String("content"), contentService.Get());
    if (FAILED(ec)) return ec;

    return NOERROR;
}
