
#include "cmdef.h"
#include "ServerSocket.h"
#include "CInet4Address.h"
#include "CInetSocketAddress.h"

AutoPtr<ISocketImplFactory> ServerSocket::mFactory;
Mutex* ServerSocket::mLock;

ServerSocket::ServerSocket()
    : mIsCreated(FALSE)
    , mIsBound(FALSE)
    , mIsClosed(FALSE)
{}

ECode ServerSocket::Accept(
    /* [out] */ ISocket** socket)
{
    VALIDATE_NOT_NULL(socket);

    CheckClosedAndCreate(FALSE);

    Boolean isBound;
    IsBound(&isBound);
    if (!isBound) {
        return E_SOCKET_EXCEPTION;
//        throw new SocketException("Socket is not bound");
    }

    AutoPtr<CSocket> socketCls;
    ASSERT_SUCCEEDED(CSocket::NewByFriend((CSocket**)&socketCls));
//    try {
    ECode ec = ImplAccept(socketCls);
    if (FAILED(ec)) {
        socketCls->Close();
        return ec;
    }
//    } catch (SecurityException e) {
//        aSocket.close();
//        throw e;
//    } catch (IOException e) {
//        aSocket.close();
//        throw e;
//    }
    *socket = (ISocket*)socketCls->Probe(EIID_ISocket);
    return NOERROR;
}

ECode ServerSocket::CheckListen(
    /* [in] */ Int32 aPort)
{
    if (aPort < 0 || aPort > 65535) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("Port out of range: " + aPort);
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkListen(aPort);
//    }

    return NOERROR;
}

ECode ServerSocket::Close()
{
    mIsClosed = TRUE;
    return mImpl->Close();
}

Int32 ServerSocket::DefaultBacklog()
{
    return 50;
}

ECode ServerSocket::GetInetAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    Boolean isBound;
    IsBound(&isBound);
    if (!isBound) {
        *address = NULL;
        return NOERROR;
    }
    return mImpl->GetInetAddress(address);
}

ECode ServerSocket::GetLocalPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);

    Boolean isBound;
    IsBound(&isBound);
    if (!isBound) {
        *port = -1;
        return NOERROR;
    }
    return mImpl->GetLocalPort(port);
}

ECode ServerSocket::GetSoTimeout(
    /* [out] */ Int32* timeout)
{
    VALIDATE_NOT_NULL(timeout);

    if (!mIsCreated) {
        Mutex::Autolock lock(mLock);
        if (!mIsCreated) {
//            try {
            ECode ec = mImpl->Create(TRUE);
            if (ec == E_SOCKET_EXCEPTION) {
                return ec;
            }
            else if (ec == E_IO_EXCEPTION) {
                return E_SOCKET_EXCEPTION;
            }
//            } catch (SocketException e) {
//                throw e;
//            } catch (IOException e) {
//                throw new SocketException(e.toString());
//            }
            mIsCreated = TRUE;
        }
    }
    return mImpl->GetOption(SocketOption_SO_TIMEOUT, timeout);
}

ECode ServerSocket::ImplAccept(
    /* [in] */ CSocket* aSocket)
{
    {
        Mutex::Autolock lock(mLock);
        mImpl->Accept(aSocket->mImpl);
        aSocket->Accepted();
    }

    return NOERROR;

//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkAccept(aSocket.getInetAddress().getHostAddress(),
//                aSocket.getPort());
//    }
}

ECode ServerSocket::SetSocketFactory(
    /* [in] */ ISocketImplFactory* aFactory)
{
    Mutex::Autolock lock(mLock);

//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkSetFactory();
//    }
    if (mFactory != NULL) {
        return E_SOCKET_EXCEPTION;
//        throw new SocketException("Factory already set");
    }
    mFactory = aFactory;

    return NOERROR;
}

ECode ServerSocket::SetSoTimeout(
    /* [in] */ Int32 timeout)
{
    CheckClosedAndCreate(TRUE);
    if (timeout < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("timeout < 0");
    }
    return mImpl->SetOption(SocketOption_SO_TIMEOUT, timeout);
}

ECode ServerSocket::Bind(
    /* [in] */ ISocketAddress* localAddr)
{
    return BindEx(localAddr, DefaultBacklog());
}

ECode ServerSocket::BindEx(
    /* [in] */ ISocketAddress* localAddr,
    /* [in] */ Int32 backlog)
{
    CheckClosedAndCreate(TRUE);
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
        AutoPtr<IInetSocketAddress> inetAddr =
                (IInetSocketAddress*)localAddr->Probe(EIID_IInetSocketAddress);
        inetAddr->GetAddress((IInetAddress**)&addr);
        if (addr == NULL) {
            return E_SOCKET_EXCEPTION;
//            throw new SocketException("Host is unresolved: " + inetAddr.getHostName());
        }
        inetAddr->GetPort(&port);
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkListen(port);
//    }

    Mutex::Autolock lock(mLock);
//    try {
    ECode ec = mImpl->Bind(addr, port);
    if (FAILED(ec)) {
        Close();
        return ec;
    }
    mIsBound = TRUE;
    ec = mImpl->Listen(backlog > 0 ? backlog : DefaultBacklog());
    if (FAILED(ec)) {
        Close();
        return ec;
    }
    // } catch (IOException e) {
    //     close();
    //     throw e;
    // }

    return NOERROR;
}

ECode ServerSocket::GetLocalSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    VALIDATE_NOT_NULL(address);

    Boolean isBound;
    IsBound(&isBound);
    if (!isBound) {
        *address = NULL;
        return NOERROR;
    }

    AutoPtr<IInetAddress> addr;
    ASSERT_SUCCEEDED(GetInetAddress((IInetAddress**)&addr));
    Int32 port;
    ASSERT_SUCCEEDED(GetLocalPort(&port));
    return CInetSocketAddress::New(addr, port, (IInetSocketAddress**)address);
}

ECode ServerSocket::IsBound(
    /* [out] */ Boolean* isBound)
{
    VALIDATE_NOT_NULL(isBound);
    *isBound = mIsBound;
    return NOERROR;
}

ECode ServerSocket::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    VALIDATE_NOT_NULL(isClosed);
    *isClosed = mIsClosed;
    return NOERROR;
}

ECode ServerSocket::CheckClosedAndCreate(
    /* [in] */ Boolean create)
{
    Boolean isClosed;
    IsClosed(&isClosed);
    if (isClosed) {
        return E_SOCKET_EXCEPTION;
//        throw new SocketException("Socket is closed");
    }

    if (!create || mIsCreated) {
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
    // } catch (SocketException e) {
    //     throw e;
    // } catch (IOException e) {
    //     throw new SocketException(e.toString());
    // }
    mIsCreated = TRUE;

    return NOERROR;
}

ECode ServerSocket::SetReuseAddress(
    /* [in] */ Boolean reuse)
{
    CheckClosedAndCreate(TRUE);
    return mImpl->SetOption(SocketOption_SO_REUSEADDR, (Int32)reuse);
}

ECode ServerSocket::GetReuseAddress(
    /* [out] */ Boolean* reuse)
{
    VALIDATE_NOT_NULL(reuse);

    CheckClosedAndCreate(TRUE);
    return mImpl->GetOption(SocketOption_SO_REUSEADDR, (Int32*)reuse);
}

ECode ServerSocket::SetReceiveBufferSize(
    /* [in] */ Int32 size)
{
    CheckClosedAndCreate(TRUE);
    if (size < 1) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("size < 1");
    }
    return mImpl->SetOption(SocketOption_SO_RCVBUF, size);
}

ECode ServerSocket::GetReceiveBufferSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    CheckClosedAndCreate(TRUE);
    return mImpl->GetOption(SocketOption_SO_RCVBUF, size);
}

ECode ServerSocket::GetChannel(
    /* [out] */ IServerSocketChannel** channel)
{
    VALIDATE_NOT_NULL(channel);

    *channel = NULL;
    return NOERROR;
}

ECode ServerSocket::SetPerformancePreferences(
    /* [in] */ Int32 connectionTime,
    /* [in] */ Int32 latency,
    /* [in] */ Int32 bandwidth)
{
    // Our socket implementation only provide one protocol: TCP/IP, so
    // we do nothing for this method
    return E_NOT_IMPLEMENTED;
}

ECode ServerSocket::Init()
{
    if (mFactory != NULL) {
        return mFactory->CreateSocketImpl((ISocketImpl**)&mImpl);
    }
    else {
        return E_NOT_IMPLEMENTED;
    }
}

ECode ServerSocket::Init(
    /* [in] */ ISocketImpl* impl)
{
    mImpl = impl;

    return NOERROR;
}

ECode ServerSocket::Init(
    /* [in] */ Int32 aPort)
{
    return Init(aPort, DefaultBacklog(), CInet4Address::ANY);
}

ECode ServerSocket::Init(
    /* [in] */ Int32 aPort,
    /* [in] */ Int32 backlog)
{
    return Init(aPort, backlog, CInet4Address::ANY);
}

ECode ServerSocket::Init(
    /* [in] */ Int32 aPort,
    /* [in] */ Int32 backlog,
    /* [in] */ IInetAddress* localAddr)
{
//    super();
    CheckListen(aPort);

    if (mFactory != NULL) {
        return mFactory->CreateSocketImpl((ISocketImpl**)&mImpl);
    }
    else {
        return E_NOT_IMPLEMENTED;
    }

    AutoPtr<IInetAddress> addr = localAddr == NULL ?
            CInet4Address::ANY : AutoPtr<IInetAddress>(localAddr);

    Mutex::Autolock lock(mLock);
    mImpl->Create(TRUE);
    mIsCreated = TRUE;
//    try {
    ECode ec = mImpl->Bind(addr, aPort);
    if (FAILED(ec)) {
        Close();
        return ec;
    }
    mIsBound = TRUE;
    ec = mImpl->Listen(backlog > 0 ? backlog : DefaultBacklog());
    if (FAILED(ec)) {
        Close();
        return ec;
    }
//    } catch (IOException e) {
//        close();
//        throw e;
//    }

    return NOERROR;
}
