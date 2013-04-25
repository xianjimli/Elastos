
#include "ext/frameworkdef.h"
#include "text/CClipboardManager.h"
#include "os/CServiceManager.h"

AutoPtr<IClipboard> CClipboardManager::sService;

AutoPtr<IClipboard> CClipboardManager::GetService()
{
    if (sService != NULL) {
        return sService;
    }

    AutoPtr<IServiceManager> serviceManager;
    ASSERT_SUCCEEDED((CServiceManager::AcquireSingleton(
            (IServiceManager**)&serviceManager)));
    serviceManager->GetService(String("clipboard"), (IInterface**)&sService);
    return sService;
}

ECode CClipboardManager::GetText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    // try {
    ECode ec = GetService()->GetClipboardText(text);
    if (FAILED(ec)) {
        *text = NULL;
    }
    return ec;
    // } catch (RemoteException e) {
    //     return null;
    // }
}

ECode CClipboardManager::SetText(
    /* [in] */ ICharSequence* text)
{
    // try {
    return GetService()->SetClipboardText(text);
    // } catch (RemoteException e) {
    // }
}

ECode CClipboardManager::HasText(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // try {
    ECode ec = GetService()->HasClipboardText(result);
    if (FAILED(ec)) {
        *result = FALSE;
    }
    return ec;
    // } catch (RemoteException e) {
    //     return false;
    // }
}

ECode CClipboardManager::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;

    return NOERROR;
}

