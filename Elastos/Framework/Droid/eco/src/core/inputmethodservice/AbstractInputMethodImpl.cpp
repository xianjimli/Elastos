
#ifdef _FRAMEWORK_CORE
#include "inputmethodservice/AbstractInputMethodImpl.h"
#include "inputmethodservice/AbstractInputMethodSessionImpl.h"
#else
#include "AbstractInputMethodImpl.h"
#include "AbstractInputMethodSessionImpl.h"
#endif


AbstractInputMethodImpl::AbstractInputMethodImpl(
    /* [in] */ AbstractInputMethodService* host)
    : mHost(host)
{}

PInterface AbstractInputMethodImpl::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_ILocalInputMethod) {
        return (ILocalInputMethod*)this;
    }
    else if (riid == EIID_IAbstractInputMethodImpl) {
        return (IAbstractInputMethodImpl*)this;
    }

    return NULL;
}

ECode AbstractInputMethodImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IAbstractInputMethodImpl*)this) {
        *pIID = EIID_IAbstractInputMethodImpl;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode AbstractInputMethodImpl::CreateSession(
    /* [in] */ ILocalInputMethodSessionCallback* callback)
{
    assert(mHost != NULL);
    AutoPtr<IAbstractInputMethodSessionImpl> impl;
    mHost->OnCreateInputMethodSessionInterface((IAbstractInputMethodSessionImpl**)&impl);
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
