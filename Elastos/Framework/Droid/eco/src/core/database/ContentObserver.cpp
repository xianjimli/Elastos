
#include "database/ContentObserver.h"


ContentObserver::Transport::Transport(
    /* [in] */ ILocalContentObserver* contentObserver)
    : mContentObserver(contentObserver)
{}

PInterface ContentObserver::Transport::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IContentObserver*)this;
    }
    else if (riid == EIID_IContentObserver) {
        return (IContentObserver*)this;
    }

    return NULL;
}

UInt32 ContentObserver::Transport::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ContentObserver::Transport::Release()
{
    return ElRefBase::Release();
}

ECode ContentObserver::Transport::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IContentObserver*)this) {
        *pIID = EIID_IContentObserver;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode ContentObserver::Transport::DeliverSelfNotifications(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (mContentObserver != NULL) {
        return mContentObserver->DeliverSelfNotifications(result);
    }
    *result = FALSE;
    return NOERROR;
}

ECode ContentObserver::Transport::OnChange(
    /* [in] */ Boolean selfChange)
{
    if (mContentObserver != NULL) {
        return mContentObserver->DispatchChange(selfChange);
    }
    return NOERROR;
}

void ContentObserver::Transport::ReleaseContentObserver()
{
    mContentObserver = NULL;
}


ContentObserver::~ContentObserver()
{}

AutoPtr<IContentObserver> ContentObserver::GetContentObserver()
{
    Mutex::Autolock lock(mTransportLock);

    if (mTransport == NULL) {
       mTransport = new Transport(
            (ILocalContentObserver*)this->Probe(EIID_ILocalContentObserver));
    }
    return mTransport;
}

AutoPtr<IContentObserver> ContentObserver::ReleaseContentObserver()
{
    Mutex::Autolock lock(mTransportLock);

    AutoPtr<Transport> oldTransport = mTransport;
    if (oldTransport != NULL) {
        oldTransport->ReleaseContentObserver();
        mTransport = NULL;
    }
    return oldTransport;
}

Boolean ContentObserver::DeliverSelfNotifications()
{
    return FALSE;
}

ECode ContentObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    return NOERROR;
}

ECode ContentObserver::DispatchChange(
    /* [in] */ Boolean selfChange)
{
    // if (mHandler == null) {
    //     onChange(selfChange);
    // } else {
    //     mHandler.post(new NotificationRunnable(selfChange));
    // }
    return E_NOT_IMPLEMENTED;
}
