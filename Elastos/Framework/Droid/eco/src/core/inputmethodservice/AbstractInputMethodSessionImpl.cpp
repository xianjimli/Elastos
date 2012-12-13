
#include "inputmethodservice/AbstractInputMethodSessionImpl.h"

AbstractInputMethodSessionImpl::AbstractInputMethodSessionImpl(
	/* [in] */ CAbstractInputMethodService* host):
    mEnabled(TRUE),
    mRevoked(FALSE),
    mHost(host)
{
}

AbstractInputMethodSessionImpl::~AbstractInputMethodSessionImpl()
{
}

PInterface AbstractInputMethodSessionImpl::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_ILocalInputMethodSession == riid) {
        return (ILocalInputMethodSession *)this;
    }

    if (EIID_IAbstractInputMethodSessionImpl == riid) {
        return (IAbstractInputMethodSessionImpl *)this;
    }

    return NULL;
}

UInt32 AbstractInputMethodSessionImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AbstractInputMethodSessionImpl::Release()
{
    return ElRefBase::Release();
}

ECode AbstractInputMethodSessionImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(ILocalInputMethodSession*)this) {
        *pIID = EIID_ILocalInputMethodSession;
    }
    else if (pObject == (IInterface*)(IAbstractInputMethodSessionImpl*)this) {
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
	assert(event != NULL && mHost != NULL);

    Boolean handled = FALSE;
    event->DispatchEx((IKeyEventCallback*)mHost->Probe(EIID_IKeyEventCallback),
    	mHost->mDispatcherState, this->Probe(EIID_IInputMethodSessionStub), &handled);

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
	assert(mHost != NULL);
    Boolean handled = FALSE;
    mHost->OnTrackballEvent(event, &handled);

    if (callback != NULL) {
        return callback->FinishedEvent(seq, handled);
    }

    return NOERROR;
}

ECode AbstractInputMethodSessionImpl::FinishInput()
{
	assert(0);
	return NOERROR;
}

ECode AbstractInputMethodSessionImpl::UpdateExtractedText(
    /* [in] */ Int32 token,
    /* [in] */ IExtractedText* text)
{
	assert(0);
	return NOERROR;
}

ECode AbstractInputMethodSessionImpl::UpdateSelection(
    /* [in] */ Int32 oldSelStart,
    /* [in] */ Int32 oldSelEnd,
    /* [in] */ Int32 newSelStart,
    /* [in] */ Int32 newSelEnd,
    /* [in] */ Int32 candidatesStart,
    /* [in] */ Int32 candidatesEnd)
{
	assert(0);
	return NOERROR;
}

ECode AbstractInputMethodSessionImpl::UpdateCursor(
    /* [in] */ IRect* newCursor)
{
	assert(0);
	return NOERROR;
}

ECode AbstractInputMethodSessionImpl::DisplayCompletions(
    /* [in] */ ArrayOf<ICompletionInfo*>* completions)
{
	assert(0);
	return NOERROR;
}

ECode AbstractInputMethodSessionImpl::AppPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
	assert(0);
	return NOERROR;
}

ECode AbstractInputMethodSessionImpl::ToggleSoftInput(
    /* [in] */ Int32 showFlags,
    /* [in] */ Int32 hideFlags)
{
	assert(0);
	return NOERROR;
}

ECode AbstractInputMethodSessionImpl::FinishSession()
{
	assert(0);
	return NOERROR;
}
