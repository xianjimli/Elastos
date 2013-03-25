
#include "inputmethodservice/CIInputMethodWrapper.h"
#include "inputmethodservice/CIInputMethodSessionWrapper.h"
#include "view/LocalInputConnectionWrapper.h"
#include "view/inputmethod/CInputBinding.h"
#include "os/CApartment.h"

#include <stdio.h>
#include <unistd.h>



const CString CIInputMethodWrapper::TAG = "CIInputMethodWrapper";
const Int32 CIInputMethodWrapper::DO_DUMP;
const Int32 CIInputMethodWrapper::DO_ATTACH_TOKEN;
const Int32 CIInputMethodWrapper::DO_SET_INPUT_CONTEXT;
const Int32 CIInputMethodWrapper::DO_UNSET_INPUT_CONTEXT;
const Int32 CIInputMethodWrapper::DO_START_INPUT;
const Int32 CIInputMethodWrapper::DO_RESTART_INPUT;
const Int32 CIInputMethodWrapper::DO_CREATE_SESSION;
const Int32 CIInputMethodWrapper::DO_SET_SESSION_ENABLED;
const Int32 CIInputMethodWrapper::DO_REVOKE_SESSION;
const Int32 CIInputMethodWrapper::DO_SHOW_SOFT_INPUT;
const Int32 CIInputMethodWrapper::DO_HIDE_SOFT_INPUT;


CIInputMethodWrapper::InputMethodSessionCallbackWrapper::InputMethodSessionCallbackWrapper(
    /* [in] */ IContext* context,
    /* [in] */ IInputMethodCallback* cb)
    : mContext(context)
    , mCb(cb)
{}

PInterface CIInputMethodWrapper::InputMethodSessionCallbackWrapper::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_ILocalInputMethodSessionCallback == riid) {
        return (ILocalInputMethodSessionCallback *)this;
    }

    return NULL;
}

UInt32 CIInputMethodWrapper::InputMethodSessionCallbackWrapper::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CIInputMethodWrapper::InputMethodSessionCallbackWrapper::Release()
{
    return ElRefBase::Release();
}

ECode CIInputMethodWrapper::InputMethodSessionCallbackWrapper::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)(ILocalInputMethodSessionCallback*)this) {
        *pIID = EIID_ILocalInputMethodSessionCallback;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CIInputMethodWrapper::InputMethodSessionCallbackWrapper::SessionCreated(
    /* [in] */ ILocalInputMethodSession* session)
{
    // try {
    if (session != NULL) {
        AutoPtr<IInputMethodSession> wrap;
        CIInputMethodSessionWrapper::New(mContext, session, (IInputMethodSession**)&wrap);
        return mCb->SessionCreated(wrap);
    }
    else {
        return mCb->SessionCreated(NULL);
    }
    // } catch (RemoteException e) {
    // }
}

ECode CIInputMethodWrapper::constructor(
    /* [in] */ IAbstractInputMethodService* context,
    /* [in] */ ILocalInputMethod* inputMethod)
{
    // mTarget = new WeakReference<AbstractInputMethodService>(context);
    mTarget = context;
    // mCaller = new HandlerCaller(context.getApplicationContext(), this);
    FAIL_RETURN(CApartment::GetMainApartment((IApartment**)&mCaller));
    context->GetApplicationContext((IContext**)&mContext);
    // mInputMethod = new WeakReference<ILocalInputMethod>(inputMethod);
    mInputMethod = inputMethod;
    return NOERROR;
}

AutoPtr<ILocalInputMethod> CIInputMethodWrapper::GetInternalInputMethod()
{
    return mInputMethod;
}

ECode CIInputMethodWrapper::AttachToken(
    /* [in] */ IBinder* token)
{
	//TODO: calling.
    //mCaller.executeOrSendMessage(mCaller.obtainMessageO(DO_ATTACH_TOKEN, token));
    ECode (STDCALL CIInputMethodWrapper::*pHandlerFunc)(IBinder* token);
    pHandlerFunc = &CIInputMethodWrapper::HandleAttachToken;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(token);
    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CIInputMethodWrapper::BindInput(
    /* [in] */ IInputBinding* binding)
{
    AutoPtr<IBinder> binder;
    FAIL_RETURN(binding->GetConnectionToken((IBinder**)&binder));
    AutoPtr<IInputConnection> ic = new LocalInputConnectionWrapper(IInputContext::Probe(binder));
    AutoPtr<IInputBinding> nu;
    FAIL_RETURN(CInputBinding::New(ic, binding, (IInputBinding**)&nu));
    //TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageO(DO_SET_INPUT_CONTEXT, nu));
    ECode (STDCALL CIInputMethodWrapper::*pHandlerFunc)(IInputBinding* nu);
    pHandlerFunc = &CIInputMethodWrapper::HandleSetInputContext;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(nu.Get());
    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CIInputMethodWrapper::UnbindInput()
{
	//TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessage(DO_UNSET_INPUT_CONTEXT));
    ECode (STDCALL CIInputMethodWrapper::*pHandlerFunc)();
    pHandlerFunc = &CIInputMethodWrapper::HandleUnsetInputContext;

    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, NULL, 0);
}

ECode CIInputMethodWrapper::StartInput(
    /* [in] */ IInputContext* inputContext,
    /* [in] */ IEditorInfo* attribute)
{
	//TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageOO(DO_START_INPUT,
    //         inputContext, attribute));
    ECode (STDCALL CIInputMethodWrapper::*pHandlerFunc)(IInputContext* inputContext, IEditorInfo* attribute);
    pHandlerFunc = &CIInputMethodWrapper::HandleStartInput;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(inputContext);
    params->WriteInterfacePtr(attribute);
    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CIInputMethodWrapper::RestartInput(
    /* [in] */ IInputContext* inputContext,
    /* [in] */ IEditorInfo* attribute)
{
	//TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageOO(DO_RESTART_INPUT,
    //         inputContext, attribute));
    ECode (STDCALL CIInputMethodWrapper::*pHandlerFunc)(IInputContext* inputContext, IEditorInfo* attribute);
    pHandlerFunc = &CIInputMethodWrapper::HandleRestartInput;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(inputContext);
    params->WriteInterfacePtr(attribute);
    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CIInputMethodWrapper::CreateSession(
    /* [in] */ IInputMethodCallback* callback)
{
	//TODO
    //mCaller.executeOrSendMessage(mCaller.obtainMessageO(DO_CREATE_SESSION, callback));
    ECode (STDCALL CIInputMethodWrapper::*pHandlerFunc)(IInputMethodCallback* callback);
    pHandlerFunc = &CIInputMethodWrapper::HandleCreateSession;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(callback);
    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CIInputMethodWrapper::SetSessionEnabled(
    /* [in] */ IInputMethodSession* session,
    /* [in] */ Boolean enabled)
{
    // try {
    AutoPtr<ILocalInputMethodSession> ls = ((CIInputMethodSessionWrapper*)
            session)->GetInternalInputMethodSession();
    //TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageIO(
    //         DO_SET_SESSION_ENABLED, enabled ? 1 : 0, ls));
    ECode (STDCALL CIInputMethodWrapper::*pHandlerFunc)(Boolean enabled, ILocalInputMethodSession* ls);
    pHandlerFunc = &CIInputMethodWrapper::HandleSetSessionEnabled;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteBoolean(enabled);
    params->WriteInterfacePtr(ls.Get());
    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    // } catch (ClassCastException e) {
    //     Log.w(TAG, "Incoming session not of correct type: " + session, e);
    // }
}

ECode CIInputMethodWrapper::RevokeSession(
    /* [in] */ IInputMethodSession* session)
{
    // try {
    AutoPtr<ILocalInputMethodSession> ls = ((CIInputMethodSessionWrapper*)
            session)->GetInternalInputMethodSession();
    //TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageO(DO_REVOKE_SESSION, ls));
    ECode (STDCALL CIInputMethodWrapper::*pHandlerFunc)(ILocalInputMethodSession* ls);
    pHandlerFunc = &CIInputMethodWrapper::HandleRevokeSession;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(ls.Get());
    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    // } catch (ClassCastException e) {
    //     Log.w(TAG, "Incoming session not of correct type: " + session, e);
    // }
}

ECode CIInputMethodWrapper::ShowSoftInput(
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver)
{
	//TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageIO(DO_SHOW_SOFT_INPUT,
    //         flags, resultReceiver));
    ECode (STDCALL CIInputMethodWrapper::*pHandlerFunc)(Int32 flags, IResultReceiver* resultReceiver);
    pHandlerFunc = &CIInputMethodWrapper::HandleShowSoftInput;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(flags);
    params->WriteInterfacePtr(resultReceiver);
    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CIInputMethodWrapper::HideSoftInput(
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver)
{
	//TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageIO(DO_HIDE_SOFT_INPUT,
    //         flags, resultReceiver));
    ECode (STDCALL CIInputMethodWrapper::*pHandlerFunc)(Int32 flags, IResultReceiver* resultReceiver);
    pHandlerFunc = &CIInputMethodWrapper::HandleHideSoftInput;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(flags);
    params->WriteInterfacePtr(resultReceiver);
    return mCaller->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CIInputMethodWrapper::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

ECode CIInputMethodWrapper::HandleAttachToken(
    /* [in] */ IBinder* token)
{
    assert(mInputMethod != NULL);
    return mInputMethod->AttachToken(token);
}

ECode CIInputMethodWrapper::HandleSetInputContext(
    /* [in] */ IInputBinding* nu)
{
    assert(mInputMethod != NULL);
    return mInputMethod->BindInput(nu);
}

ECode CIInputMethodWrapper::HandleUnsetInputContext()
{
    assert(mInputMethod != NULL);
    return mInputMethod->UnbindInput();
}

ECode CIInputMethodWrapper::HandleStartInput(
    /* [in] */ IInputContext* inputContext,
    /* [in] */ IEditorInfo* attribute)
{
    AutoPtr<IInputConnection> ic = inputContext != NULL
            ? new LocalInputConnectionWrapper(inputContext) : NULL;
    assert(mInputMethod != NULL);
    return mInputMethod->StartInput(ic, attribute);
}

ECode CIInputMethodWrapper::HandleRestartInput(
    /* [in] */ IInputContext* inputContext,
    /* [in] */ IEditorInfo* attribute)
{
    AutoPtr<IInputConnection> ic = inputContext != NULL
            ? new LocalInputConnectionWrapper(inputContext) : NULL;
    assert(mInputMethod != NULL);
    return mInputMethod->RestartInput(ic, attribute);
}

ECode CIInputMethodWrapper::HandleCreateSession(
    /* [in] */ IInputMethodCallback* callback)
{
    AutoPtr<ILocalInputMethodSessionCallback> cb =
            new InputMethodSessionCallbackWrapper(mContext, callback);
    assert(mInputMethod != NULL);
    return mInputMethod->CreateSession(cb);
}

ECode CIInputMethodWrapper::HandleSetSessionEnabled(
    /* [in] */ Boolean enabled,
    /* [in] */ ILocalInputMethodSession* ls)
{
    assert(mInputMethod != NULL);
    return mInputMethod->SetSessionEnabled(ls, enabled);
}

ECode CIInputMethodWrapper::HandleRevokeSession(
    /* [in] */ ILocalInputMethodSession* ls)
{
    assert(mInputMethod != NULL);
    return mInputMethod->RevokeSession(ls);
}

ECode CIInputMethodWrapper::HandleShowSoftInput(
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver)
{
    assert(mInputMethod != NULL);
    return mInputMethod->ShowSoftInput(flags, resultReceiver);
}

ECode CIInputMethodWrapper::HandleHideSoftInput(
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver)
{
    assert(mInputMethod != NULL);
    return mInputMethod->HideSoftInput(flags, resultReceiver);
}
