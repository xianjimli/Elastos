
#include "CPAcceptAll.h"

PInterface CPAcceptAll::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_ICookiePolicy) {
        return (ICookiePolicy*)this;
    }

    return NULL;
}

UInt32 CPAcceptAll::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CPAcceptAll::Release()
{
    return ElRefBase::Release();
}

ECode CPAcceptAll::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPAcceptAll::ShouldAccept(
    /* [in] */ IURI* uri,
    /* [in] */ IHttpCookie* cookie,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}
