
#include "view/accessibility/CAccessibilityManagerClient.h"
#include "os/CApartment.h"
#include "os/CServiceManager.h"

ECode CAccessibilityManagerClient::SetEnabled(
    /* [in] */ Boolean enabled)
{
    void (STDCALL CAccessibilityManager::*pHandlerFunc)(Boolean);
    pHandlerFunc = &CAccessibilityManager::HandleSetEnabled;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteBoolean(enabled);

    mHost->mHandler->PostCppCallback(
        (Handle32)mHost.Get(), *(Handle32*)&pHandlerFunc, params, 0);

    return NOERROR;
}

ECode CAccessibilityManagerClient::constructor(
    /* [in] */ ILocalAccessibilityManager* host)
{
    assert(host);
    mHost = (CAccessibilityManager*)host;

    return NOERROR;
}
