
#include "inputmethodservice/IInputMethodWrapper.h"
#include "inputmethodservice/IInputMethodSessionWrapper.h"
#include "view/LocalInputConnectionWrapper.h"
#include "view/inputmethod/CInputBinding.h"


CString IInputMethodWrapper::TAG = "IInputMethodWrapper";
const Int32 IInputMethodWrapper::DO_DUMP;
const Int32 IInputMethodWrapper::DO_ATTACH_TOKEN;
const Int32 IInputMethodWrapper::DO_SET_INPUT_CONTEXT;
const Int32 IInputMethodWrapper::DO_UNSET_INPUT_CONTEXT;
const Int32 IInputMethodWrapper::DO_START_INPUT;
const Int32 IInputMethodWrapper::DO_RESTART_INPUT;
const Int32 IInputMethodWrapper::DO_CREATE_SESSION;
const Int32 IInputMethodWrapper::DO_SET_SESSION_ENABLED;
const Int32 IInputMethodWrapper::DO_REVOKE_SESSION;
const Int32 IInputMethodWrapper::DO_SHOW_SOFT_INPUT;
const Int32 IInputMethodWrapper::DO_HIDE_SOFT_INPUT;


IInputMethodWrapper::InputMethodSessionCallbackWrapper::InputMethodSessionCallbackWrapper(
    /* [in] */ IContext* context,
    /* [in] */ IInputMethodCallbackStub* cb):
    mContext(context),
    mCb(cb)
{
}

PInterface IInputMethodWrapper::InputMethodSessionCallbackWrapper::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_ILocalInputMethodSessionCallback == riid) {
        return (ILocalInputMethodSessionCallback *)this;
    }

    return NULL;
}

UInt32 IInputMethodWrapper::InputMethodSessionCallbackWrapper::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 IInputMethodWrapper::InputMethodSessionCallbackWrapper::Release()
{
    return ElRefBase::Release();
}

ECode IInputMethodWrapper::InputMethodSessionCallbackWrapper::GetInterfaceID(
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

ECode IInputMethodWrapper::InputMethodSessionCallbackWrapper::SessionCreated(
    /* [in] */ ILocalInputMethodSession* session)
{
    if (session != NULL) {
        AutoPtr<IInputMethodSessionWrapper> wrap =
                new IInputMethodSessionWrapper(mContext, session);
        return mCb->SessionCreated(wrap);
    } else {
        return mCb->SessionCreated(NULL);
    }
    // } catch (RemoteException e) {
    // }
    return NOERROR;
}

IInputMethodWrapper::IInputMethodWrapper(
    /* [in] */ IAbstractInputMethodService* context,
    /* [in] */ ILocalInputMethod* inputMethod):
    mTarget(context),
    mInputMethod(inputMethod)
{
    // mTarget = new WeakReference<AbstractInputMethodService>(context);
    // mCaller = new HandlerCaller(context.getApplicationContext(), this);
    assert(context != NULL);
    context->GetApplicationContext((IContext**) &mContext);
    // mInputMethod = new WeakReference<ILocalInputMethod>(inputMethod);
}

PInterface IInputMethodWrapper::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInputMethodStub == riid) {
        return (IInputMethodStub *)this;
    }

    return NULL;
}

UInt32 IInputMethodWrapper::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 IInputMethodWrapper::Release()
{
    return ElRefBase::Release();
}

ECode IInputMethodWrapper::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)(IInputMethodStub*)this) {
        *pIID = EIID_IInputMethodStub;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

AutoPtr<ILocalInputMethod> IInputMethodWrapper::GetInternalInputMethod()
{
    return mInputMethod;
}

ECode IInputMethodWrapper::ExecuteMessage(
    /* [in] */ /*Message msg*/)
{
    // ILocalInputMethod inputMethod = mInputMethod.get();
    // // Need a valid reference to the inputMethod for everything except a dump.
    // if (inputMethod == NULL && msg.what != DO_DUMP) {
    //     Log.w(TAG, "Input method reference was NULL, ignoring message: " + msg.what);
    //     return;
    // }

    // switch (msg.what) {
    //     case DO_ATTACH_TOKEN: {
    //         inputMethod.attachToken((IBinder)msg.obj);
    //         return;
    //     }
    //     case DO_SET_INPUT_CONTEXT: {
    //         inputMethod.bindInput((InputBinding)msg.obj);
    //         return;
    //     }
    //     case DO_UNSET_INPUT_CONTEXT:
    //         inputMethod.unbindInput();
    //         return;
    //     case DO_START_INPUT: {
    //         HandlerCaller.SomeArgs args = (HandlerCaller.SomeArgs)msg.obj;
    //         IInputContext inputContext = (IInputContext)args.arg1;
    //         InputConnection ic = inputContext != NULL
    //                 ? new LocalInputConnectionWrapper(inputContext) : NULL;
    //         inputMethod.startInput(ic, (EditorInfo)args.arg2);
    //         return;
    //     }
    //     case DO_RESTART_INPUT: {
    //         HandlerCaller.SomeArgs args = (HandlerCaller.SomeArgs)msg.obj;
    //         IInputContext inputContext = (IInputContext)args.arg1;
    //         InputConnection ic = inputContext != NULL
    //                 ? new LocalInputConnectionWrapper(inputContext) : NULL;
    //         inputMethod.restartInput(ic, (EditorInfo)args.arg2);
    //         return;
    //     }
    //     case DO_CREATE_SESSION: {
    //         inputMethod.createSession(new InputMethodSessionCallbackWrapper(
    //                 mCaller.mContext, (IInputMethodCallbackStub)msg.obj));
    //         return;
    //     }
    //     case DO_SET_SESSION_ENABLED:
    //         inputMethod.setSessionEnabled((ILocalInputMethodSession)msg.obj,
    //                 msg.arg1 != 0);
    //         return;
    //     case DO_REVOKE_SESSION:
    //         inputMethod.revokeSession((ILocalInputMethodSession)msg.obj);
    //         return;
    //     case DO_SHOW_SOFT_INPUT:
    //         inputMethod.showSoftInput(msg.arg1, (ResultReceiver)msg.obj);
    //         return;
    //     case DO_HIDE_SOFT_INPUT:
    //         inputMethod.hideSoftInput(msg.arg1, (ResultReceiver)msg.obj);
    //         return;
    // }
    // Log.w(TAG, "Unhandled message code: " + msg.what);

    return NOERROR;
}

ECode IInputMethodWrapper::AttachToken(
    /* [in] */ IBinder* token)
{
	//TODO: calling.
    //mCaller.executeOrSendMessage(mCaller.obtainMessageO(DO_ATTACH_TOKEN, token));
    assert(mInputMethod != NULL);
    return mInputMethod->AttachToken(token);
}

ECode IInputMethodWrapper::BindInput(
    /* [in] */ IInputBinding* binding)
{
	AutoPtr<IBinder> binder;
	binding->GetConnectionToken((IBinder**) &binder);
	assert(binder != NULL);

    // InputConnection ic = new LocalInputConnectionWrapper(
    //         IInputContext.Stub.asInterface(binder));

    AutoPtr<IInputConnection> ic = new LocalInputConnectionWrapper(IInputContextStub::Probe(binder));

    AutoPtr<IInputBinding> nu;
    CInputBinding::New(ic, binding, (IInputBinding**) &nu);
    assert(nu != NULL);

    //TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageO(DO_SET_INPUT_CONTEXT, nu));

    assert(mInputMethod != NULL);
    return mInputMethod->BindInput(nu);
}

ECode IInputMethodWrapper::UnbindInput()
{
	//TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessage(DO_UNSET_INPUT_CONTEXT));
    assert(mInputMethod != NULL);
    return mInputMethod->UnbindInput();
}

ECode IInputMethodWrapper::StartInput(
    /* [in] */ IInputContextStub* inputContext,
    /* [in] */ IEditorInfo* attribute)
{
	//TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageOO(DO_START_INPUT,
    //         inputContext, attribute));

    AutoPtr<IInputConnection> ic = inputContext != NULL
            ? new LocalInputConnectionWrapper(inputContext) : NULL;

    assert(mInputMethod != NULL);
    return mInputMethod->StartInput(ic, attribute);
}

ECode IInputMethodWrapper::RestartInput(
    /* [in] */ IInputContextStub* inputContext,
    /* [in] */ IEditorInfo* attribute)
{
	//TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageOO(DO_RESTART_INPUT,
    //         inputContext, attribute));

    AutoPtr<IInputConnection> ic = inputContext != NULL
            ? new LocalInputConnectionWrapper(inputContext) : NULL;

    assert(mInputMethod != NULL);
    return mInputMethod->RestartInput(ic, attribute);
}

ECode IInputMethodWrapper::CreateSession(
    /* [in] */ IInputMethodCallbackStub* callback)
{
	//TODO
    //mCaller.executeOrSendMessage(mCaller.obtainMessageO(DO_CREATE_SESSION, callback));

    assert(mInputMethod != NULL);
    return mInputMethod->CreateSession(new InputMethodSessionCallbackWrapper(
                mContext, callback));
}

ECode IInputMethodWrapper::SetSessionEnabled(
    /* [in] */ IInputMethodSessionStub* session,
    /* [in] */ Boolean enabled)
{
    AutoPtr<ILocalInputMethodSession> ls = ((IInputMethodSessionWrapper*)
            session)->GetInternalInputMethodSession();

    //TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageIO(
    //         DO_SET_SESSION_ENABLED, enabled ? 1 : 0, ls));
    // } catch (ClassCastException e) {
    //     Log.w(TAG, "Incoming session not of correct type: " + session, e);
    // }

    assert(mInputMethod != NULL);
    return mInputMethod->SetSessionEnabled(ls, enabled);
}

ECode IInputMethodWrapper::RevokeSession(
    /* [in] */ IInputMethodSessionStub* session)
{
    AutoPtr<ILocalInputMethodSession> ls = ((IInputMethodSessionWrapper*)
            session)->GetInternalInputMethodSession();

    //TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageO(DO_REVOKE_SESSION, ls));
    // } catch (ClassCastException e) {
    //     Log.w(TAG, "Incoming session not of correct type: " + session, e);
    // }

    assert(mInputMethod != NULL);
    return mInputMethod->RevokeSession(ls);
}

ECode IInputMethodWrapper::ShowSoftInput(
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver)
{
	//TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageIO(DO_SHOW_SOFT_INPUT,
    //         flags, resultReceiver));

    assert(mInputMethod != NULL);
    return mInputMethod->ShowSoftInput(flags, resultReceiver);
}

ECode IInputMethodWrapper::HideSoftInput(
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver)
{
	//TODO
    // mCaller.executeOrSendMessage(mCaller.obtainMessageIO(DO_HIDE_SOFT_INPUT,
    //         flags, resultReceiver));

    assert(mInputMethod != NULL);
    return mInputMethod->HideSoftInput(flags, resultReceiver);
}
