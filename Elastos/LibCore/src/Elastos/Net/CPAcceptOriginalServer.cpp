
#include "CPAcceptOriginalServer.h"
#include "CHttpCookie.h"

PInterface CPAcceptOriginalServer::Probe(
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

UInt32 CPAcceptOriginalServer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CPAcceptOriginalServer::Release()
{
    return ElRefBase::Release();
}

ECode CPAcceptOriginalServer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CPAcceptOriginalServer::ShouldAccept(
    /* [in] */ IURI* uri,
    /* [in] */ IHttpCookie* cookie,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    String domain, host;
    cookie->GetDomainAttr(&domain);
    uri->GetHost(&host);
    *result = CHttpCookie::DomainMatches(domain, host);
    return NOERROR;
}
