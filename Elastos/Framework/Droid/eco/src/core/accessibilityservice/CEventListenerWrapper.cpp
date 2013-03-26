

#include "accessibilityservice/CEventListenerWrapper.h"
#include "os/CApartment.h"
#include "ext/frameworkdef.h"

CEventListenerWrapper::CEventListenerWrapper()
{
    ASSERT_SUCCEEDED(CApartment::GetMainApartment((IApartment**)&mApartment)
            && (mApartment != NULL));
}

ECode CEventListenerWrapper::SetConnection(
    /* [in] */ IAccessibilityServiceConnection* connection)
{
    void (STDCALL CEventListenerWrapper::*pHandlerFunc)(IAccessibilityServiceConnection*);
    pHandlerFunc = &CEventListenerWrapper::HandleDoSetConnection;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(connection);

    mApartment->PostCppCallback(
        (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    return NOERROR;
}

ECode CEventListenerWrapper::OnAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    void (STDCALL CEventListenerWrapper::*pHandlerFunc)(IAccessibilityEvent*);
    pHandlerFunc = &CEventListenerWrapper::HandleDoOnAccessibilityEvent;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(event);

    mApartment->PostCppCallback(
        (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);

    return NOERROR;
}

ECode CEventListenerWrapper::OnInterrupt()
{
    void (STDCALL CEventListenerWrapper::*pHandlerFunc)();
    pHandlerFunc = &CEventListenerWrapper::HandleDoOnInterrupt;

    mApartment->PostCppCallback(
        (Handle32)this, *(Handle32*)&pHandlerFunc, NULL, 0);

    return NOERROR;
}

ECode CEventListenerWrapper::GetDescription(
    /* [out] */ String* description)
{
    return NOERROR;
}

ECode CEventListenerWrapper::constructor(
    /* [in] */ IAccessibilityService* context)
{
    mTarget = context;
    return NOERROR;
}

void CEventListenerWrapper::HandleDoSetConnection(
    /* [in] */ IAccessibilityServiceConnection* connection)
{
    mTarget->SetConnection(connection);
    mTarget->OnServiceConnected();
}

void CEventListenerWrapper::HandleDoOnInterrupt()
{
    mTarget->OnInterrupt();
}

void CEventListenerWrapper::HandleDoOnAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    if (event != NULL) {
        mTarget->OnAccessibilityEvent(event);
        event->Recycle();
    }
}
