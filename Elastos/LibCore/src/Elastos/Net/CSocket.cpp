
#include "cmdef.h"
#include "CSocket.h"
#include "CInet4Address.h"
#include "InetAddress.h"
#include "CInetSocketAddress.h"

AutoPtr<ISocketImplFactory> CSocket::mFactory;

CSocket::CSocket()
    : mIsCreated(FALSE)
    , mIsBound(FALSE)
    , mIsConnected(FALSE)
    , mIsClosed(FALSE)
    , mIsInputShutdown(FALSE)
    , mIsOutputShutdown(FALSE)
{
    mLocalAddress = CInet4Address::ANY;
}

void CSocket::TryAllAddresses(
    /* [in] */ const String& dstName,
    /* [in] */ Int32 dstPort,
    /* [in] */ IInetAddress* localAddress,
    /* [in] */ Int32 localPort,
    /* [in] */ Boolean streaming)
{
    ArrayOf<IInetAddress*>* dstAddresses;
    InetAddress::GetAllByName(dstName, &dstAddresses);
    // Loop through all the destination addresses except the last, trying to
    // connect to each one and ignoring errors. There must be at least one
    // address, or getAllByName would have thrown UnknownHostException.
    IInetAddress* dstAddress;
    for (Int32 i = 0; i < dstAddresses->GetLength() - 1; i++) {
        dstAddress = (*dstAddresses)[i];
//        try {
        CheckDestination(dstAddress, dstPort);
        StartupSocket(dstAddress, dstPort, localAddress, localPort, streaming);
        return;
//        } catch (SecurityException e1) {
//        } catch (IOException e2) {
//        }
    }

    // Now try to connect to the last address in the array, handing back to
    // the caller any exceptions that are thrown.
    dstAddress = (*dstAddresses)[dstAddresses->GetLength() - 1];
    CheckDestination(dstAddress, dstPort);
    StartupSocket(dstAddress, dstPort, localAddress, localPort, streaming);
}

ECode CSocket::CheckDestination(
    /* [in] */ IInetAddress* destAddr,
    /* [in] */ Int32 dstPort)
{
    if (dstPort < 0 || dstPort > 65535) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("Port out of range: " + dstPort);
    }
    String address;
    destAddr->GetHostAddress(&address);
    CheckConnectPermission(address, dstPort);

    return NOERROR;
}

void CSocket::CheckConnectPermission(
    /* [in] */ const String& hostname,
    /* [in] */ Int32 dstPort)
{
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkConnect(hostname, dstPort);
//    }
}

ECode CSocket::Close()
{
    mIsClosed = true;
    // RI compatibility: the RI returns the any address (but the original local port) after close.
    mLocalAddress = CInet4Address::ANY;
    return mImpl->Close();
}

ECode CSocket::GetInetAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    Boolean isConnected;
    IsConnected(&isConnected);
    if (!isConnected) {
        *address = NULL;
        return NOERROR;
    }
    return mImpl->GetInetAddress(address);
}

ECode CSocket::GetInputStream(
    /* [out] */ IInputStream** is)
{
    VALIDATE_NOT_NULL(is);

    CheckOpenAndCreate(FALSE);
    Boolean isInputShutdown;
    IsInputShutdown(&isInputShutdown);
    if (isInputShutdown) {
        return E_SOCKET_EXCEPTION;
//        throw new SocketException("Socket input is shutdown");
    }
    return mImpl->GetInputStream(is);
}

ECode CSocket::GetKeepAlive(
    /* [out] */ Boolean* keepAlive)
{
    VALIDATE_NOT_NULL(keepAlive);

    CheckOpenAndCreate(TRUE);
    return mImpl->GetOption(SocketOption_SO_KEEPALIVE, (Int32*)keepAlive);
}

ECode CSocket::GetLocalAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    *address = mLocalAddress;
    return NOERROR;
}

ECode CSocket::GetLocalPort(
    /* [out] */ Int32* localPort)
{
    VALIDATE_NOT_NULL(localPort);

    Boolean isBound;
    IsBound(&isBound);
    if (!isBound) {
        *localPort = -1;
        return NOERROR;
    }
    return mImpl->GetLocalPort(localPort);
}

ECode CSocket::GetOutputStream(
    /* [out] */ IOutputStream** os)
{
    VALIDATE_NOT_NULL(os);

    CheckOpenAndCreate(FALSE);
    Boolean isOutputShutdown;
    IsOutputShutdown(&isOutputShutdown);
    if (isOutputShutdown) {
        return E_SOCKET_EXCEPTION;
//        throw new SocketException("Socket output is shutdown");
    }
    return mImpl->GetOutputStream(os);
}

ECode CSocket::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);

    Boolean isConnected;
    IsConnected(&isConnected);
    if (!isConnected) {
        *port = 0;
        return NOERROR;
    }
    return mImpl->GetPort(port);
}

ECode CSocket::GetSoLinger(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    CheckOpenAndCreate(TRUE);
    // The RI explicitly guarantees this idiocy in the SocketOptions.setOption documentation.
    return mImpl->GetOption(SocketOption_SO_LINGER, value);
//    if (value instanceof Integer) {
//        return (Integer) value;
//    } else {
//        return -1;
//    }
}

ECode CSocket::GetReceiveBufferSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    CheckOpenAndCreate(TRUE);
    return mImpl->GetOption(SocketOption_SO_RCVBUF, size);
}

ECode CSocket::GetSendBufferSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    CheckOpenAndCreate(TRUE);
    return mImpl->GetOption(SocketOption_SO_SNDBUF, size);
}

ECode CSocket::GetSoTimeout(
    /* [out] */ Int32* timeout)
{
    VALIDATE_NOT_NULL(timeout);

    CheckOpenAndCreate(TRUE);
    return mImpl->GetOption(SocketOption_SO_TIMEOUT, timeout);
}

ECode CSocket::GetTcpNoDelay(
    /* [out] */ Boolean* noDelay)
{
    VALIDATE_NOT_NULL(noDelay);

    CheckOpenAndCreate(TRUE);
    return mImpl->GetOption(SocketOption_TCP_NODELAY, (Int32*)noDelay);
}

ECode CSocket::SetKeepAlive(
    /* [in] */ Boolean keepAlive)
{
    if (mImpl != NULL) {
        CheckOpenAndCreate(TRUE);
        return mImpl->SetOption(SocketOption_SO_KEEPALIVE, (Int32)keepAlive);
    }

    return NOERROR;
}

ECode CSocket::SetSocketImplFactory(
    /* [in] */ ISocketImplFactory* fac)
{
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkSetFactory();
//    }
    if (mFactory != NULL) {
        return E_SOCKET_EXCEPTION;
//        throw new SocketException("Factory already set");
    }
    mFactory = fac;

    return NOERROR;
}

ECode CSocket::SetSendBufferSize(
    /* [in] */ Int32 size)
{
    CheckOpenAndCreate(TRUE);
    if (size < 1) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("size < 1");
    }
    return mImpl->SetOption(SocketOption_SO_SNDBUF, size);
}

ECode CSocket::SetReceiveBufferSize(
    /* [in] */ Int32 size)
{
    CheckOpenAndCreate(TRUE);
    if (size < 1) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("size < 1");
    }
    return mImpl->SetOption(SocketOption_SO_RCVBUF, size);
}

ECode CSocket::SetSoLinger(
    /* [in] */ Boolean on,
    /* [in] */ Int32 timeout)
{
    CheckOpenAndCreate(TRUE);
    if (on && timeout < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("timeout < 0");
    }
    if (on) {
        return mImpl->SetOption(SocketOption_SO_LINGER, timeout);
    }
    else {
        return mImpl->SetOption(SocketOption_SO_LINGER, 0);
    }
}

ECode CSocket::SetSoTimeout(
    /* [in] */ Int32 timeout)
{
    CheckOpenAndCreate(TRUE);
    if (timeout < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("timeout < 0");
    }
    return mImpl->SetOption(SocketOption_SO_TIMEOUT, timeout);
}

ECode CSocket::SetTcpNoDelay(
    /* [in] */ Boolean on)
{
    CheckOpenAndCreate(TRUE);
    return mImpl->SetOption(SocketOption_TCP_NODELAY, (Int32)on);
}

ECode CSocket::StartupSocket(
    /* [in] */ IInetAddress* dstAddress,
    /* [in] */ Int32 dstPort,
    /* [in] */ IInetAddress* localAddress,
    /* [in] */ Int32 localPort,
    /* [in] */ Boolean streaming)
{
    if (localPort < 0 || localPort > 65535) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("Local port out of range: " + localPort);
    }

    AutoPtr<IInetAddress> addr = localAddress == NULL ?
            CInet4Address::ANY : AutoPtr<IInetAddress>(localAddress);

    Mutex::Autolock lock(mLock);
    mImpl->Create(streaming);
    mIsCreated = TRUE;
//    try {
    ECode ec = NOERROR;
    if (!streaming || !UsingSocks()) {
        ec = mImpl->Bind(addr, localPort);
        if (FAILED(ec)) {
            mImpl->Close();
            return ec;
        }
    }
    mIsBound = TRUE;
    ec = mImpl->ConnectEx(dstAddress, dstPort);
    if (FAILED(ec)) {
        mImpl->Close();
        return ec;
    }

    mIsConnected = TRUE;
    CacheLocalAddress();
//    } catch (IOException e) {
//        impl.close();
//        throw e;
//    }

    return ec;
}

Boolean CSocket::UsingSocks()
{
    Int32 type;
    mProxy->GetType(&type);
    return mProxy != NULL && type == ProxyType_SOCKS;
}

ECode CSocket::ShutdownInput()
{
    Boolean isInputShutdown;
    IsInputShutdown(&isInputShutdown);
    if (isInputShutdown) {
        return E_SOCKET_EXCEPTION;
//        throw new SocketException("Socket input is shutdown");
    }
    CheckOpenAndCreate(FALSE);
    ECode ec = mImpl->ShutdownInput();
    if (FAILED(ec)) {
        return ec;
    }
    mIsInputShutdown = TRUE;

    return NOERROR;
}

ECode CSocket::ShutdownOutput()
{
    Boolean isOutputShutdown;
    IsOutputShutdown(&isOutputShutdown);
    if (isOutputShutdown) {
        return E_SOCKET_EXCEPTION;
//        throw new SocketException("Socket output is shutdown");
    }
    CheckOpenAndCreate(FALSE);
    ECode ec = mImpl->ShutdownOutput();
    if (FAILED(ec)) {
        return ec;
    }
    mIsOutputShutdown = TRUE;

    return NOERROR;
}

ECode CSocket::CheckOpenAndCreate(
    /* [in] */ Boolean create)
{
    Boolean isClosed;
    IsClosed(&isClosed);
    if (isClosed) {
        return E_SOCKET_EXCEPTION;
//        throw new SocketException("Socket is closed");
    }
    if (!create) {
        Boolean isConnected;
        IsConnected(&isConnected);
        if (!isConnected) {
            return E_SOCKET_EXCEPTION;
//            throw new SocketException("Socket is not connected");
            // a connected socket must be created
        }

        /*
         * return directly to fix a possible bug, if !create, should return
         * here
         */
        return NOERROR;
    }
    if (mIsCreated) {
        return NOERROR;
    }

    Mutex::Autolock lock(mLock);
    if (mIsCreated) {
        return NOERROR;
    }
//    try {
    ECode ec = mImpl->Create(TRUE);
    if (FAILED(ec)) {
        return E_SOCKET_EXCEPTION;
    }
//    } catch (SocketException e) {
//        throw e;
//    } catch (IOException e) {
//        throw new SocketException(e.toString());
//    }
    mIsCreated = TRUE;

    return NOERROR;
}

ECode CSocket::GetLocalSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    VALIDATE_NOT_NULL(address);

    Boolean isBound;
    IsBound(&isBound);
    if (!isBound) {
        *address = NULL;
        return NOERROR;
    }
    AutoPtr<IInetAddress> localAddress;
    Int32 localPort;
    GetLocalAddress((IInetAddress**)&localAddress);
    GetLocalPort(&localPort);
    return CInetSocketAddress::New(localAddress, localPort, (IInetSocketAddress**)address);
}

ECode CSocket::GetRemoteSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    VALIDATE_NOT_NULL(address);

    Boolean isConnected;
    IsConnected(&isConnected);
    if (!isConnected) {
        *address = NULL;
        return NOERROR;
    }
    AutoPtr<IInetAddress> netAddress;
    Int32 port;
    GetInetAddress((IInetAddress**)&netAddress);
    GetPort(&port);
    return CInetSocketAddress::New(netAddress, port, (IInetSocketAddress**)address);
}

ECode CSocket::IsBound(
    /* [out] */ Boolean* isBound)
{
    VALIDATE_NOT_NULL(isBound);

    *isBound = mIsBound;
    return NOERROR;
}

ECode CSocket::IsConnected(
    /* [out] */ Boolean* isConnected)
{
    VALIDATE_NOT_NULL(isConnected);

    *isConnected = mIsConnected;
    return NOERROR;
}

ECode CSocket::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    VALIDATE_NOT_NULL(isClosed);

    *isClosed = mIsClosed;
    return NOERROR;
}

ECode CSocket::Bind(
    /* [in] */ ISocketAddress* localAddr)
{
    CheckOpenAndCreate(TRUE);
    Boolean isBound;
    IsBound(&isBound);
    if (isBound) {
        return E_BIND_EXCEPTION;
//        throw new BindException("Socket is already bound");
    }

    Int32 port = 0;
    AutoPtr<IInetAddress> addr = CInet4Address::ANY;
    if (localAddr != NULL) {
        if (localAddr->Probe(EIID_IInetSocketAddress) == NULL) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
//            throw new IllegalArgumentException("Local address not an InetSocketAddress: " +
//                    localAddr.getClass());
        }
        AutoPtr<IInetSocketAddress> inetAddr = (IInetSocketAddress*)localAddr->Probe(EIID_IInetSocketAddress);
        inetAddr->GetAddress((IInetAddress**)&addr);
        if (addr == NULL) {
            return E_SOCKET_EXCEPTION;
//            throw new SocketException("Host is unresolved: " + inetAddr.getHostName());
        }
        inetAddr->GetPort(&port);
    }

    Mutex::Autolock lock(mLock);
//    try {
    ECode ec= mImpl->Bind(addr, port);
    if (FAILED(ec)) {
        mImpl->Close();
        return ec;
    }
    mIsBound = TRUE;
    CacheLocalAddress();

    return NOERROR;
//    } catch (IOException e) {
//        impl.close();
//        throw e;
//    }
}

ECode CSocket::Connect(
    /* [in] */ ISocketAddress* remoteAddr)
{
    return ConnectEx(remoteAddr, 0);
}

ECode CSocket::ConnectEx(
    /* [in] */ ISocketAddress* remoteAddr,
    /* [in] */ Int32 timeout)
{
    CheckOpenAndCreate(TRUE);
    if (timeout < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("timeout < 0");
    }
    Boolean isConnected;
    IsConnected(&isConnected);
    if (isConnected) {
        return E_SOCKET_EXCEPTION;
//        throw new SocketException("Already connected");
    }
    if (remoteAddr == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("remoteAddr == null");
    }

    if (remoteAddr->Probe(EIID_IInetSocketAddress) == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;;
    }
    AutoPtr<IInetSocketAddress> inetAddr =
            (IInetSocketAddress*)remoteAddr->Probe(EIID_IInetSocketAddress);
    AutoPtr<IInetAddress> addr;
    inetAddr->GetAddress((IInetAddress**)&addr);
    if (addr == NULL) {
        return E_SOCKET_EXCEPTION;
//        throw new SocketException("Host is unresolved: " + inetAddr.getHostName());
    }
    Int32 port;
    inetAddr->GetPort(&port);

    CheckDestination(addr, port);

    Mutex::Autolock lock(mLock);
//    synchronized (connectLock) {
//    try {
    ECode ec = NOERROR;
    Boolean isBound;
    IsBound(&isBound);
    if (!isBound) {
        // socket already created at this point by earlier call or
        // checkOpenAndCreate this caused us to lose socket
        // options on create
        // impl.create(true);
        if (!UsingSocks()) {
            ec = mImpl->Bind(CInet4Address::ANY, 0);
            if (FAILED(ec)) {
                mImpl->Close();
                return ec;
            }
        }
        mIsBound = TRUE;
    }
    ec = mImpl->ConnectEx2(remoteAddr, timeout);
    if (FAILED(ec)) {
        mImpl->Close();
        return ec;
    }
    mIsConnected = TRUE;
    CacheLocalAddress();
//    } catch (IOException e) {
//        impl.close();
//        throw e;
//    }

    return ec;
}

ECode CSocket::IsInputShutdown(
    /* [out] */ Boolean* isInputShutdown)
{
    VALIDATE_NOT_NULL(isInputShutdown);

    *isInputShutdown = mIsInputShutdown;
    return NOERROR;
}

ECode CSocket::IsOutputShutdown(
    /* [out] */ Boolean* isOutputShutdown)
{
    VALIDATE_NOT_NULL(isOutputShutdown);

    *isOutputShutdown = mIsOutputShutdown;
    return NOERROR;
}

ECode CSocket::SetReuseAddress(
    /* [in] */ Boolean reuse)
{
    CheckOpenAndCreate(TRUE);
    return mImpl->SetOption(SocketOption_SO_REUSEADDR, (Int32)reuse);
}

ECode CSocket::GetReuseAddress(
    /* [out] */ Boolean* reuse)
{
    VALIDATE_NOT_NULL(reuse);

    CheckOpenAndCreate(TRUE);
    return mImpl->GetOption(SocketOption_SO_REUSEADDR, (Int32*)reuse);
}

ECode CSocket::SetOOBInline(
    /* [in] */ Boolean oobinline)
{
    CheckOpenAndCreate(TRUE);
    return mImpl->SetOption(SocketOption_SO_OOBINLINE, (Int32)oobinline);
}

ECode CSocket::GetOOBInline(
    /* [out] */ Boolean* oobinline)
{
    VALIDATE_NOT_NULL(oobinline);

    CheckOpenAndCreate(TRUE);
    return mImpl->GetOption(SocketOption_SO_OOBINLINE, (Int32*)oobinline);
}

ECode CSocket::SetTrafficClass(
    /* [in] */ Int32 value)
{
    CheckOpenAndCreate(TRUE);
    if (value < 0 || value > 255) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }
    return mImpl->SetOption(SocketOption_IP_TOS, value);
}

ECode CSocket::GetTrafficClass(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    CheckOpenAndCreate(TRUE);
    return mImpl->GetOption(SocketOption_IP_TOS, value);
}

ECode CSocket::SendUrgentData(
    /* [in] */ Int32 value)
{
    return mImpl->SendUrgentData(value);
}

void CSocket::Accepted()
{
    mIsCreated = mIsBound = mIsConnected = TRUE;
    CacheLocalAddress();
}

void CSocket::CacheLocalAddress()
{
    AutoPtr<IPlatform> platform;
    ASSERT_SUCCEEDED(CPlatform::AcquireSingleton((IPlatform**)&platform));
    AutoPtr<INetworkSystem> networkSystem;
    platform->GetNetworkSystem((INetworkSystem**)&networkSystem);
//    mLocalAddress = networkSystem->GetSocketLocalAddress(impl.fd);
}

ECode CSocket::GetChannel(
    /* [out] */ ISocketChannel** channel)
{
    VALIDATE_NOT_NULL(channel);
    *channel = NULL;
    return NOERROR;
}

ECode CSocket::SetPerformancePreferences(
    /* [in] */ Int32 connectionTime,
    /* [in] */ Int32 latency,
    /* [in] */ Int32 bandwidth)
{
    // Our socket implementation only provide one protocol: TCP/IP, so
    // we do nothing for this method
    return NOERROR;
}

ECode CSocket::constructor()
{
    if (mFactory != NULL) {
        mFactory->CreateSocketImpl((ISocketImpl**)&mImpl);
    }
    else {
        mImpl = NULL;//new PlainSocketImpl();
    }
    mProxy = NULL;

    return NOERROR;
}

ECode CSocket::constructor(
    /* [in] */ IProxy* proxy)
{
    mProxy = proxy;
    ProxyType type;
    proxy->GetType(&type);
    if (proxy == NULL || type == ProxyType_HTTP) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("Proxy is null or invalid type");
    }
    AutoPtr<IInetSocketAddress> address;
    AutoPtr<ISocketAddress> socketAddress;
    proxy->GetAddress((ISocketAddress**)&socketAddress);
    address = (IInetSocketAddress*)socketAddress->Probe(EIID_IInetSocketAddress);
    if (NULL != address) {
        AutoPtr<IInetAddress> addr;
        address->GetAddress((IInetAddress**)&addr);
        String host;
        if (NULL != addr) {
            addr->GetHostAddress(&host);
        }
        else {
            address->GetHostName(&host);
        }
        Int32 port;
        address->GetPort(&port);
        CheckConnectPermission(host, port);
    }

    if (mFactory != NULL) {
        mFactory->CreateSocketImpl((ISocketImpl**)&mImpl);
    }
    else {
        mImpl = NULL;//new PlainSocketImpl();
    }

    return NOERROR;
}

ECode CSocket::constructor(
    /* [in] */ const String& dstName,
    /* [in] */ Int32 dstPort)
{
    return constructor(dstName, dstPort, NULL, 0);
}

ECode CSocket::constructor(
    /* [in] */ const String& dstName,
    /* [in] */ Int32 dstPort,
    /* [in] */ IInetAddress* localAddress,
    /* [in] */ Int32 localPort)
{
    constructor();
    TryAllAddresses(dstName, dstPort, localAddress, localPort, TRUE);

    return NOERROR;
}

ECode CSocket::constructor(
    /* [in] */ const String& hostName,
    /* [in] */ Int32 port,
    /* [in] */ Boolean streaming)
{
    constructor();
    TryAllAddresses(hostName, port, NULL, 0, streaming);

    return NOERROR;
}

ECode CSocket::constructor(
    /* [in] */ IInetAddress* dstAddress,
    /* [in] */ Int32 dstPort)
{
    constructor();
    CheckDestination(dstAddress, dstPort);
    return StartupSocket(dstAddress, dstPort, NULL, 0, TRUE);
}

ECode CSocket::constructor(
    /* [in] */ IInetAddress* dstAddress,
    /* [in] */ Int32 dstPort,
    /* [in] */ IInetAddress* localAddress,
    /* [in] */ Int32 localPort)
{
    constructor();
    CheckDestination(dstAddress, dstPort);
    return StartupSocket(dstAddress, dstPort, localAddress, localPort, TRUE);
}

ECode CSocket::constructor(
    /* [in] */ IInetAddress* addr,
    /* [in] */ Int32 port,
    /* [in] */ Boolean streaming)
{
    constructor();
    CheckDestination(addr, port);
    return StartupSocket(addr, port, NULL, 0, streaming);
}
