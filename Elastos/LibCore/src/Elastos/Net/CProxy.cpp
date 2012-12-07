
#include "cmdef.h"
#include "CProxy.h"

AutoPtr<IProxy> CProxy::InitProxy()
{
    CProxy* proxy = new CProxy();
    proxy->Init();
    AutoPtr<IProxy> ip = (IProxy*)proxy->Probe(EIID_IProxy);
    ip->AddRef();
    return ip;
}

AutoPtr<IProxy> CProxy::NO_PROXY = CProxy::InitProxy();

void CProxy::Init()
{
    mType = ProxyType_DIRECT;
    mAddress = NULL;
}

ECode CProxy::GetType(
    /* [out] */ ProxyType* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mType;

    return NOERROR;
}

ECode CProxy::GetAddress(
    /* [out] */ ISocketAddress** address)
{
    VALIDATE_NOT_NULL(address);
    *address = mAddress;

    return NOERROR;
}

ECode CProxy::constructor(
    /* [in] */ ProxyType type,
    /* [in] */ ISocketAddress* sa)
{
    /*
     * Don't use DIRECT type to construct a proxy instance directly.
     * SocketAddress must NOT be null.
     */
    if (type == ProxyType_DIRECT || sa == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("Illegal Proxy.Type or SocketAddress argument");
    }
    mType = type;
    mAddress = sa;

    return NOERROR;
}
