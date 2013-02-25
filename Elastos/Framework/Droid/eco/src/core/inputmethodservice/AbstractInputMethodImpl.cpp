
#include "inputmethodservice/AbstractInputMethodImpl.h"
#include "inputmethodservice/AbstractInputMethodSessionImpl.h"


AbstractInputMethodImpl::AbstractInputMethodImpl(
	/* [in] */ AbstractInputMethodService* host):
	mHost(host)
{}

AbstractInputMethodImpl::~AbstractInputMethodImpl()
{}

PInterface AbstractInputMethodImpl::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IAbstractInputMethodImpl == riid) {
        return (IAbstractInputMethodImpl *)this;
    }
    if (EIID_ILocalInputMethod == riid) {
        return (ILocalInputMethod *)this;
    }

    return NULL;
}

UInt32 AbstractInputMethodImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AbstractInputMethodImpl::Release()
{
    return ElRefBase::Release();
}

ECode AbstractInputMethodImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)(ILocalInputMethod*)this) {
        *pIID = EIID_ILocalInputMethod;
    }
    else if (pObject == (IInterface*)(IAbstractInputMethodImpl*)this) {
        *pIID = EIID_IAbstractInputMethodImpl;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}


ECode AbstractInputMethodImpl::AttachToken(
    /* [in] */ IBinder* token)
{
	assert(0);
	return NOERROR;
}

ECode AbstractInputMethodImpl::BindInput(
    /* [in] */ IInputBinding* binding)
{
	assert(0);
	return NOERROR;
}

ECode AbstractInputMethodImpl::UnbindInput()
{
	assert(0);
	return NOERROR;
}

ECode AbstractInputMethodImpl::StartInput(
    /* [in] */ IInputConnection* inputConnection,
    /* [in] */ IEditorInfo* attribute)
{
	assert(0);
	return NOERROR;
}

ECode AbstractInputMethodImpl::RestartInput(
    /* [in] */ IInputConnection* inputConnection,
    /* [in] */ IEditorInfo* attribute)
{
	assert(0);
	return NOERROR;
}

ECode AbstractInputMethodImpl::CreateSession(
    /* [in] */ ILocalInputMethodSessionCallback* callback)
{
	assert(mHost != NULL);
	AutoPtr<AbstractInputMethodSessionImpl> impl;
	mHost->OnCreateInputMethodSessionInterface((IAbstractInputMethodSessionImpl**) &impl);

	return callback->SessionCreated(
			(ILocalInputMethodSession*)impl->Probe(EIID_ILocalInputMethodSession));
}

ECode AbstractInputMethodImpl::SetSessionEnabled(
    /* [in] */ ILocalInputMethodSession* session,
    /* [in] */ Boolean enabled)
{
	return ((AbstractInputMethodSessionImpl*)session)->SetEnabled(enabled);
}

ECode AbstractInputMethodImpl::RevokeSession(
    /* [in] */ ILocalInputMethodSession* session)
{
	return ((AbstractInputMethodSessionImpl*)session)->RevokeSelf();
}

ECode AbstractInputMethodImpl::ShowSoftInput(
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver)
{
	assert(0);
	return NOERROR;
}

ECode AbstractInputMethodImpl::HideSoftInput(
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver)
{
	assert(0);
	return NOERROR;
}
