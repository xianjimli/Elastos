
#include "cmdef.h"
#include "CInetSocketAddress.h"
#include "CInet4Address.h"
#include "InetAddress.h"


const Int64 CInetSocketAddress::sSerialVersionUID = 5076001401234631237ll;

ECode CInetSocketAddress::constructor(
    /* [in] */ Int32 port)
{
    return constructor(NULL, port);
}

ECode CInetSocketAddress::constructor(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 port)
{
    if (port < 0 || port > 65535) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (address == NULL) {
        mAddr = CInet4Address::ANY;
    }
    else {
        mAddr = address;
    }
    mAddr->GetHostName(&mHostname);
    mPort = port;

    return NOERROR;
}

ECode CInetSocketAddress::constructor(
    /* [in] */ const String& host,
    /* [in] */ Int32 port)
{
    return constructor(host, port, TRUE);
}

ECode CInetSocketAddress::constructor(
    /* [in] */ const String& hostname,
    /* [in] */ Int32 port,
    /* [in] */ Boolean needResolved)
{
    if (hostname.IsNull() || port < 0 || port > 65535) {
//        throw new IllegalArgumentException("host=" + hostname + ", port=" + port);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mHostname = hostname;
    mPort = port;

    if (needResolved) {
        // SecurityManager smgr = System.getSecurityManager();
        // if (smgr != null) {
        //     smgr.checkConnect(hostname, port);
        // }
//        try {
        FAIL_RETURN(InetAddress::GetByName(hostname, (IInetAddress**)&mAddr));
        mHostname = NULL;
//        } catch (UnknownHostException ignored) {
//        }
    }
    return NOERROR;
}

ECode CInetSocketAddress::CreateUnresolved(
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [out] */ IInetSocketAddress** addr)
{
    VALIDATE_NOT_NULL(addr);

    return CInetSocketAddress::New(host, port, FALSE, addr);
}

ECode CInetSocketAddress::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    *port = mPort;

    return NOERROR;
}

ECode CInetSocketAddress::GetAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);
    *address = mAddr;
    if (*address != NULL) (*address)->AddRef();

    return NOERROR;
}

ECode CInetSocketAddress::GetHostName(
    /* [out] */ String* hostname)
{
    VALIDATE_NOT_NULL(hostname);
    if (mAddr != NULL) {
        return mAddr->GetHostName(hostname);
    }
    else {
        *hostname = mHostname;
        return NOERROR;
    }
}

ECode CInetSocketAddress::IsUnresolved(
    /* [out] */ Boolean* isUnresolved)
{
    VALIDATE_NOT_NULL(isUnresolved);
    *isUnresolved = mAddr == NULL;

    return NOERROR;
}
