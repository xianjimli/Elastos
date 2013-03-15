
#include "view/inputmethod/CIInputMethodClient.h"


ECode CIInputMethodClient::constructor(
    /* [in] */ ILocalInputMethodManager* host)
{
	mHost = (CLocalInputMethodManager*)host;
	return NOERROR;
}

ECode CIInputMethodClient::SetUsingInputMethod(
    /* [in] */ Boolean state)
{
	return NOERROR;
}

ECode CIInputMethodClient::OnBindMethod(
    /* [in] */ IInputBindResult* res)
{
    void (STDCALL CLocalInputMethodManager::*pHandlerFunc)(IInputBindResult*);
    pHandlerFunc = &CLocalInputMethodManager::HandleMsgBind;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(res);
    return mHost->mMainLooper->PostCppCallbackDelayed((Handle32)mHost.Get(), *(Handle32*)&pHandlerFunc,
        params, 0, 0);
}

ECode CIInputMethodClient::OnUnbindMethod(
    /* [in] */ Int32 sequence)
{
    void (STDCALL CLocalInputMethodManager::*pHandlerFunc)(Int32);
    pHandlerFunc = &CLocalInputMethodManager::HandleMsgUnBind;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(sequence);
    return mHost->mMainLooper->PostCppCallbackDelayed((Handle32)mHost.Get(), *(Handle32*)&pHandlerFunc,
        params, 0, 0);
}

ECode CIInputMethodClient::SetActive(
    /* [in] */ Boolean active)
{
    void (STDCALL CLocalInputMethodManager::*pHandlerFunc)(Boolean);
    pHandlerFunc = &CLocalInputMethodManager::HandleMsgSetActive;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteBoolean(active);
    return mHost->mMainLooper->PostCppCallbackDelayed((Handle32)mHost.Get(), *(Handle32*)&pHandlerFunc,
        params, 0, 0);
}

ECode CIInputMethodClient::GetDescription(
    /* [out] */ String* str)
{
    return E_NOT_IMPLEMENTED;
}
