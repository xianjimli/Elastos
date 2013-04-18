
#include "cmdef.h"
#include "ServerSocket.h"
#include "CInet4Address.h"
#include "CInetSocketAddress.h"
#include <stdio.h>

AutoPtr<ISocketImplFactory> ServerSocket::sFactory;
Mutex ServerSocket::sLock;

ServerSocket::ServerSocket()
    : mIsCreated(FALSE)
    , mIsBound(FALSE)
    , mIsClosed(FALSE)
{}

ECode ServerSocket::Init()
{
    if (sFactory != NULL) {
        return sFactory->CreateSocketImpl((ISocketImpl**)&mImpl);
    }
    else {
        // new PlainServerSocketImpl();
        assert(0);
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
    printf("%s, %d\n", __FILE__, __LINE__);
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
    printf("%s, %d\n", __FILE__, __LINE__);
    FAIL_RETURN(CheckListen(aPort));

    printf("%s, %d\n", __FILE__, __LINE__);
    if (sFactory != NULL) {
            printf("%s, %d\n", __FILE__, __LINE__);
        return sFactory->CreateSocketImpl((ISocketImpl**)&mImpl);
    }
    else {
        // new PlainServerSocketImpl();
        assert(0);
        return E_NOT_IMPLEMENTED;
    }
    printf("%s, %d\n", __FILE__, __LINE__);
    IInetAddress* addr = localAddr == NULL ? CInet4Address::ANY.Get() : localAddr;
    printf("%s, %d\n", __FILE__, __LINE__);
    Mutex::Autolock lock(&mLock);
    printf("%s, %d\n", __FILE__, __LINE__);
    mImpl->Create(TRUE);
    mIsCreated = TRUE;
//    try {
    ECode ec = mImpl->Bind(addr, aPort);
    if (FAILED(ec)) {
        Close();
        return ec;
    }
    printf("%s, %d\n", __FILE__, __LINE__);
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

ECode ServerSocket::Accept(
    /* [out] */ ISocket** socket)
{
    VALIDATE_NOT_NULL(socket);

    FAIL_RETURN(CheckClosedAndCreate(FALSE));

    Boolean isBound;
    if (IsBound(&isBound), !isBound) {
//        throw new SocketException("Socket is not bound");
        return E_SOCKET_EXCEPTION;
    }

    AutoPtr<CSocket> socketObj;
    FAIL_RETURN(CSocket::NewByFriend((CSocket**)&socketObj));
//    try {
    ECode ec = ImplAccept(socketObj);
    if (FAILED(ec)) {
        socketObj->Close();
        *socket = NULL;
        return ec;
    }
//    } catch (SecurityException e) {
//        aSocket.close();
//        throw e;
//    } catch (IOException e) {
//        aSocket.close();
//        throw e;
//    }
    *socket = (ISocket*)socketObj.Get();
    (*socket)->AddRef();
    return NOERROR;
}

ECode ServerSocket::CheckListen(
    /* [in] */ Int32 aPort)
{
    if (aPort < 0 || aPort > 65535) {
//        throw new IllegalArgumentException("Port out of range: " + aPort);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
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
    if (IsBound(&isBound), !isBound) {
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
    if (IsBound(&isBound), !isBound) {
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
        Mutex::Autolock lock(&mLock);

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
    AutoPtr<IInteger32> optVal;
    FAIL_RETURN(mImpl->GetOption(SocketOption_SO_TIMEOUT, (IInterface**)&optVal));
    return optVal->GetValue(timeout);
}

ECode ServerSocket::ImplAccept(
    /* [in] */ CSocket* aSocket)
{
    {
        Mutex::Autolock lock(&mLock);

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
    Mutex::Autolock lock(&sLock);

//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkSetFactory();
//    }
    if (sFactory != NULL) {
//        throw new SocketException("Factory already set");
        return E_SOCKET_EXCEPTION;
    }
    sFactory = aFactory;
    return NOERROR;
}

ECode ServerSocket::SetSoTimeout(
    /* [in] */ Int32 timeout)
{
    Mutex::Autolock lock(&mLock);

    FAIL_RETURN(CheckClosedAndCreate(TRUE));
    if (timeout < 0) {
//        throw new IllegalArgumentException("timeout < 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IInteger32> optVal;
    CInteger32::New(timeout, (IInteger32**)&optVal);
    return mImpl->SetOption(SocketOption_SO_TIMEOUT, optVal);
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
    FAIL_RETURN(CheckClosedAndCreate(TRUE));
    Boolean isBound;
    if (IsBound(&isBound), isBound) {
//        throw new BindException("Socket is already bound");
        return E_BIND_EXCEPTION;
    }
    Int32 port = 0;
    IInetAddress* addr = CInet4Address::ANY;
    if (localAddr != NULL) {
        if (IInetSocketAddress::Probe(localAddr) == NULL) {
//            throw new IllegalArgumentException("Local address not an InetSocketAddress: " +
//                    localAddr.getClass());
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        IInetSocketAddress* inetAddr = IInetSocketAddress::Probe(localAddr);
        inetAddr->GetAddress((IInetAddress**)&addr);
        if (addr == NULL) {
//            throw new SocketException("Host is unresolved: " + inetAddr.getHostName());
            return E_SOCKET_EXCEPTION;
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
    if (IsBound(&isBound), !isBound) {
        *address = NULL;
        return NOERROR;
    }

    AutoPtr<IInetAddress> addr;
    FAIL_RETURN(GetInetAddress((IInetAddress**)&addr));
    Int32 port;
    FAIL_RETURN(GetLocalPort(&port));
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
    if (IsClosed(&isClosed), isClosed) {
//        throw new SocketException("Socket is closed");
        return E_SOCKET_EXCEPTION;
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
    AutoPtr<IBoolean> optVal;
    CBoolean::New(reuse, (IBoolean**)&optVal);
    return mImpl->SetOption(SocketOption_SO_REUSEADDR, optVal);
}

ECode ServerSocket::GetReuseAddress(
    /* [out] */ Boolean* reuse)
{
    VALIDATE_NOT_NULL(reuse);

    FAIL_RETURN(CheckClosedAndCreate(TRUE));
    AutoPtr<IBoolean> optVal;
    FAIL_RETURN(mImpl->GetOption(SocketOption_SO_REUSEADDR, (IInterface**)&optVal));
    return optVal->GetValue(reuse);
}

ECode ServerSocket::SetReceiveBufferSize(
    /* [in] */ Int32 size)
{
    FAIL_RETURN(CheckClosedAndCreate(TRUE));
    if (size < 1) {
//        throw new IllegalArgumentException("size < 1");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IInteger32> optVal;
    CInteger32::New(size, (IInteger32**)&optVal);
    return mImpl->SetOption(SocketOption_SO_RCVBUF, optVal);
}

ECode ServerSocket::GetReceiveBufferSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    FAIL_RETURN(CheckClosedAndCreate(TRUE));
    AutoPtr<IInteger32> optVal;
    FAIL_RETURN(mImpl->GetOption(SocketOption_SO_RCVBUF, (IInterface**)&optVal));
    return optVal->GetValue(size);
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
    return NOERROR;
}
