
#ifdef _FRAMEWORK_CORE
#include "inputmethodservice/AbstractInputMethodSessionImpl.h"
#else
#include "AbstractInputMethodSessionImpl.h"
#endif

AbstractInputMethodSessionImpl::AbstractInputMethodSessionImpl(
    /* [in] */ AbstractInputMethodService* host)
    : mEnabled(TRUE)
    , mRevoked(FALSE)
    , mAbstractHost(host)
{}

PInterface AbstractInputMethodSessionImpl::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_ILocalInputMethodSession == riid) {
        return (ILocalInputMethodSession *)this;
    }
    else if (EIID_IAbstractInputMethodSessionImpl == riid) {
        return (IAbstractInputMethodSessionImpl *)this;
    }

    return NULL;
}

ECode AbstractInputMethodSessionImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    assert(pIID != NULL);

    if (pObject == (IInterface*)(IAbstractInputMethodSessionImpl*)this) {
        *pIID = EIID_IAbstractInputMethodSessionImpl;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode AbstractInputMethodSessionImpl::IsEnabled(
    /* [out] */ Boolean* enabled)
{
    assert(enabled != NULL);
    *enabled = mEnabled;

    return NOERROR;
}

ECode AbstractInputMethodSessionImpl::IsRevoked(
    /* [out] */ Boolean* revoked)
{
    assert(revoked != NULL);
    *revoked = mRevoked;

    return NOERROR;
}

ECode AbstractInputMethodSessionImpl::SetEnabled(
    /* [in] */ Boolean enabled)
{
    if (!mRevoked) {
        mEnabled = enabled;
    }

    return NOERROR;
}

ECode AbstractInputMethodSessionImpl::RevokeSelf()
{
    mRevoked = TRUE;
    mEnabled = FALSE;

    return NOERROR;
}

ECode AbstractInputMethodSessionImpl::DispatchKeyEvent(
    /* [in] */ Int32 seq,
    /* [in] */ IKeyEvent* event,
    /* [in] */ ILocalInputMethodSessionEventCallback* callback)
{
    assert(event != NULL && mAbstractHost != NULL);

    Boolean handled = FALSE;
    event->DispatchEx((IKeyEventCallback*)mAbstractHost->Probe(EIID_IKeyEventCallback),
            mAbstractHost->mDispatcherState, this->Probe(EIID_IInputMethodSession), &handled);
    if (callback != NULL) {
        return callback->FinishedEvent(seq, handled);
    }

    return NOERROR;
}

ECode AbstractInputMethodSessionImpl::DispatchTrackballEvent(
    /* [in] */ Int32 seq,
    /* [in] */ IMotionEvent* event,
    /* [in] */ ILocalInputMethodSessionEventCallback* callback)
{
    assert(mAbstractHost != NULL);
    Boolean handled = FALSE;
    mAbstractHost->OnTrackballEvent(event, &handled);
    if (callback != NULL) {
        return callback->FinishedEvent(seq, handled);
    }

    return NOERROR;
}
