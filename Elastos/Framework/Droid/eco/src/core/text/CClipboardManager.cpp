
#include "ext/frameworkdef.h"
#include "text/CClipboardManager.h"
#include "os/CServiceManager.h"

#if 0
static AutoPtr<IClipboard> mService;
#endif

#if 0
static IClipboard* GetService()
{
    if (mService != NULL)
        return mService;

    AutoPtr<IServiceManager> serviceManager;

    if (FAILED(CServiceManager::AcquireSingleton(
        (IServiceManager**)&serviceManager))) {
        return NULL;
    }

    if (FAILED(serviceManager->GetService(
        String("clipboard"), (IInterface**)&mService))) {
        return NULL;
    }

    return mService;
}
#endif

ECode CClipboardManager::GetText(
    /* [out] */ ICharSequence** text)
{
#if 0
    VALIDATE_NOT_NULL(result);
    // try {
    IClipboard* service = GetService();
    if (service) {
        return service->GetClipboardText(text);
    }
    else {
        *text = NULL;
        return NOERROR;
    }
    // } catch (RemoteException e) {
    //     return null;
    // }
#else
    return E_NOT_IMPLEMENTED;
#endif
}

ECode CClipboardManager::SetText(
    /* [in] */ ICharSequence* text)
{
#if 0
    // try {
    IClipboard* service = GetService();
    if (service) {
        return service->SetClipboardText(text);
    }
    else {
        return NOERROR;
    }
    // } catch (RemoteException e) {
    // }
#else
    return E_NOT_IMPLEMENTED;
#endif
}

ECode CClipboardManager::HasText(
    /* [out] */ Boolean* result)
{
#if 0
    VALIDATE_NOT_NULL(result);
    // try {
    IClipboard* service = GetService();
    if (service) {
        return service->HasClipboardText(result);
    }
    else {
        *result = FALSE;
        return NOERROR;
    }
    // } catch (RemoteException e) {
    //     return false;
    // }
#else
    return E_NOT_IMPLEMENTED;
#endif
}

ECode CClipboardManager::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;

    return NOERROR;
}

