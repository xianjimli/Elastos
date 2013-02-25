
#include "inputmethodservice/AbstractInputMethodService.h"
#include "view/CDispatcherState.h"


AbstractInputMethodService::AbstractInputMethodService()
{
	assert(SUCCEEDED(CDispatcherState::New((IDispatcherState**)&mDispatcherState)));
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
