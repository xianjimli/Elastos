
#ifdef _FRAMEWORK_CORE
#include "ext/frameworkdef.h"
#include "inputmethodservice/AbstractInputMethodService.h"
#include "inputmethodservice/CIInputMethodWrapper.h"
#include "view/CDispatcherState.h"

#include "os/CApartment.h"
#else
#include "AbstractInputMethodService.h"
#endif


AbstractInputMethodService::AbstractInputMethodService()
{
	assert(CDispatcherState::New((IDispatcherState**)&mDispatcherState) == NOERROR);
}

PInterface AbstractInputMethodService::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IKeyEventCallback) {
       return (IKeyEventCallback*)this;
    }
    else return Service::Probe(riid);
}

ECode AbstractInputMethodService::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) return E_ILLEGAL_ARGUMENT_EXCEPTION;

    if (pObject == (IInterface*)(IKeyEventCallback*)this) {
        *pIID = EIID_IKeyEventCallback;
    }
    else return Service::GetInterfaceID(pObject, pIID);
}

ECode AbstractInputMethodService::GetKeyDispatcherState(
    /* [out] */ IDispatcherState** dispatcherState)
{
    assert(dispatcherState != NULL);
    *dispatcherState = mDispatcherState;
    (*dispatcherState)->AddRef();

    return NOERROR;
}

ECode AbstractInputMethodService::OnBind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    assert(binder != NULL);

    if (mInputMethod == NULL) {
        AutoPtr<IAbstractInputMethodImpl> mtdImpl;
        OnCreateInputMethodInterface((IAbstractInputMethodImpl**)&mtdImpl);
        mInputMethod = ILocalInputMethod::Probe(mtdImpl.Get());
    }

    AutoPtr<IInputMethod> method;
    CIInputMethodWrapper::New(
            (IAbstractInputMethodService*)this->Probe(EIID_IAbstractInputMethodService),
            mInputMethod, (IInputMethod**)&method);
    *binder = (IBinder*)method.Get();
    (*binder)->AddRef();
    return NOERROR;
}

ECode AbstractInputMethodService::OnTrackballEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    *state = FALSE;

    return NOERROR;
}
