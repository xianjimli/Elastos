
#include "cmdef.h"
#include "CInetSocketAddress.h"
#include "CInet4Address.h"
#include "InetAddress.h"

const Int64 CInetSocketAddress::sSerialVersionUID = 5076001401234631237L;

CInetSocketAddress::CInetSocketAddress()
    : mPort(0)
{}

ECode CInetSocketAddress::Init(
    /* [in] */ const String& hostname,
    /* [in] */ Int32 port,
    /* [in] */ Boolean needResolved)
{
    if (hostname.IsNull() || port < 0 || port > 65535) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("host=" + hostname + ", port=" + port);
    }
    mHostname = hostname;
    mPort = port;

    if (needResolved) {
        // SecurityManager smgr = System.getSecurityManager();
        // if (smgr != null) {
        //     smgr.checkConnect(hostname, port);
        // }
//        try {
        InetAddress::GetByName(hostname, (IInetAddress**)&mAddr);
        mHostname = String(NULL);
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
    CInetSocketAddress* sa = new CInetSocketAddress();
    sa->Init(host, port, FALSE);
    if (sa != NULL) {
        *addr = (IInetSocketAddress*)sa->Probe(EIID_IInetSocketAddress);
    }
    else {
        *addr = NULL;
    }

    return NOERROR;
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

    return NOERROR;
}

ECode CInetSocketAddress::GetHostName(
    /* [out] */ String* hostname)
{
    VALIDATE_NOT_NULL(hostname);
    if (mAddr != NULL) {
        mAddr->GetHostName(hostname);
    }
    else {
        *hostname = mHostname;
    }

    return NOERROR;
}

ECode CInetSocketAddress::IsUnresolved(
    /* [out] */ Boolean* isUnresolved)
{
    VALIDATE_NOT_NULL(isUnresolved);
    *isUnresolved = mAddr == NULL? TRUE: FALSE;

    return NOERROR;
}

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
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
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
    /* [in] */ String host,
    /* [in] */ Int32 port)
{
    return Init(host, port, TRUE);
}
