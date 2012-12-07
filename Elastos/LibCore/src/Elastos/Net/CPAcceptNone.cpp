
#include "CPAcceptNone.h"

PInterface CPAcceptNone::Probe(
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

UInt32 CPAcceptNone::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CPAcceptNone::Release()
{
    return ElRefBase::Release();
}

ECode CPAcceptNone::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPAcceptNone::ShouldAccept(
    /* [in] */ IURI* uri,
    /* [in] */ IHttpCookie* cookie,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}
