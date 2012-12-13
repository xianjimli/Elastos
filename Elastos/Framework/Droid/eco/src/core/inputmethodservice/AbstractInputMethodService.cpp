
#include "inputmethodservice/AbstractInputMethodService.h"
#include "view/CDispatcherState.h"


AbstractInputMethodService::AbstractInputMethodService()
{
	assert(SUCCEEDED(CDispatcherState::New((IDispatcherState**)&mDispatcherState)));
}

AbstractInputMethodService::~AbstractInputMethodService()
{}

UInt32 AbstractInputMethodService::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AbstractInputMethodService::Release()
{
    return ElRefBase::Release();
}

PInterface AbstractInputMethodService::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IObject) {
        return (PInterface)(IObject*)this;
    }
    if (riid == EIID_IInterface) {
        return (PInterface)(IService*)this;
    }
    if (riid == EIID_IService) {
        return (IService*)this;
    }
    else if (riid == EIID_IContextThemeWrapper) {
       return (IContextThemeWrapper*)this;
    }
    else if (riid == EIID_IContextWrapper) {
       return (IContextWrapper*)this;
    }
    else if (riid == EIID_IContext) {
       return (IContext*)this;
    }
    else if (riid == EIID_IKeyEventCallback) {
       return (IKeyEventCallback*)this;
    }

    return NULL;
}

ECode AbstractInputMethodService::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IService*)this) {
        *pIID = EIID_IService;
    }
    else if (pObject == (IInterface*)(IKeyEventCallback*)this) {
        *pIID = EIID_IKeyEventCallback;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode AbstractInputMethodService::GetKeyDispatcherState(
    /* [out] */ IDispatcherState** dispatcherState)
{
    assert(dispatcherState != NULL && mDispatcherState != NULL);
    *dispatcherState = mDispatcherState;
    if (*dispatcherState != NULL) {
        (*dispatcherState)->AddRef();
    }

    return NOERROR;
}

ECode AbstractInputMethodService::OnBind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    if (mInputMethod == NULL) {
        return OnCreateInputMethodInterface((IAbstractInputMethodImpl**) &mInputMethod);
    }

    //TODO
    assert(0);
    //return new IInputMethodWrapper(this, mInputMethod);
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
