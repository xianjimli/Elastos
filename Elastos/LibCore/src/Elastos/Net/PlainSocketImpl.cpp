#include "PlainSocketImpl.h"
#include "InetAddress.h"
#include "CSocketInputStream.h"
#include "CSocketOutputStream.h"


AutoPtr<IInetAddress> PlainSocketImpl::sLastConnectedAddress;
Int32 PlainSocketImpl::sLastConnectedPort;

PlainSocketImpl::PlainSocketImpl(
    /* [in] */ IFileDescriptor* fd)
    : SocketImpl()
{
    mFd = fd;
    mFd->GetDescriptor(&mIfd);
}

//    AutoPtr<IInetAddress> mAddress;
//
//    /**
//     * The remote port this socket is connected to.
//     */
//    Int32 mPort;
//
//    /**
//     * The file descriptor of this socket.
//     */
//    AutoPtr<IFileDescriptor> mFd;
//
//    /**
//     * The local port this socket is connected to.
//     */
//    Int32 mLocalport;

PlainSocketImpl::PlainSocketImpl(
    /* [in] */ IProxy* proxy)
    : SocketImpl()
{
    AutoPtr<IFileDescriptor> fd;
    CFileDescriptor::New((IFileDescriptor**) &fd);
    mFd = fd;
    mFd->GetDescriptor(&mIfd);
    mProxy = proxy;
}

PlainSocketImpl::PlainSocketImpl()
    : SocketImpl()
{
    AutoPtr<IFileDescriptor> fd;
    CFileDescriptor::New((IFileDescriptor**) &fd);
    mFd = fd;
    mFd->GetDescriptor(&mIfd);
}

PlainSocketImpl::PlainSocketImpl(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 localport,
    /* [in] */ IInetAddress* addr,
    /* [in] */ Int32 port)
    : SocketImpl()
{
    mFd = fd;
    mFd->GetDescriptor(&mIfd);

    mLocalport = localport;
    mAddress = addr;
    mPort = port;

}

ECode PlainSocketImpl::InitLocalPort(
    /* [in] */ Int32 localPort)
{
    return E_NOT_IMPLEMENTED;
}

ECode PlainSocketImpl::InitRemoteAddressAndPort(
    /* [in] */ IInetAddress* remoteAddress,
    /* [in] */ Int32 remotePort)
{
    return E_NOT_IMPLEMENTED;
}
    //    ECode GetOption(int optID)
ECode PlainSocketImpl::SetOption(
    /* [in] */ Int32 optID,
    /* [in] */ Boolean val)
{
    return E_NOT_IMPLEMENTED;
}

ECode PlainSocketImpl::SocksAccept()
{
    return E_NOT_IMPLEMENTED;
}

ECode PlainSocketImpl::GetFD(
        /* [out] */ IFileDescriptor** ppFd)
{
    return E_NOT_IMPLEMENTED;
}

ECode PlainSocketImpl::Read(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ Int32* value)
{
    if (mShutdownInput) {
        return E_FAIL;
    }

    Int32 read;
    mNetImpl->Read(mIfd, offset, count, buffer, &read);
    // Return of zero bytes for a blocking socket means a timeout occurred
    if (read == 0) {
        return E_SOCKET_EXCEPTION;
    }
    // Return of -1 indicates the peer was closed
    if (read == -1) {
        mShutdownInput = true;
    }
    return read;
}

ECode PlainSocketImpl::Write(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ Int32* value)
{
    ECode ec = NOERROR;
    if (mStreaming) {
        ec = mNetImpl->Write(mIfd, buffer, offset, count, value);
    } else {
        ArrayOf<Byte>* array = NULL;
        mAddress->GetAddress(&array);
        ec = mNetImpl->Send(mIfd, buffer, offset, count, mPort, array, value);
        if (array != NULL) {
            ArrayOf<Byte>::Free(array);
        }
    }
    return ec;
}

//---------protected-----------------------
ECode PlainSocketImpl::Accept(
    /* [in] */ ISocketImpl* newImpl)
{
    if (UsingSocks()) {
        ((PlainSocketImpl*) newImpl)->SocksBind();
        ((PlainSocketImpl*) newImpl)->SocksAccept();
        return NOERROR;
    }

//    if (newImpl instanceof PlainSocketImpl) {
//        PlainSocketImpl newPlainSocketImpl = (PlainSocketImpl) newImpl;
//        netImpl.accept(fd, newImpl, newPlainSocketImpl.getFileDescriptor());
//    } else {
//        // if newImpl is not an instance of PlainSocketImpl, use
//        // reflection to get/set protected fields.
//        if (fdField == null) {
//            fdField = getSocketImplField("fd");
//        }
//        FileDescriptor newFd = (FileDescriptor) fdField.get(newImpl);
//        netImpl.accept(fd, newImpl, newFd);
//    } catch (IllegalAccessException e) {
//        // empty
//    }
}

ECode PlainSocketImpl::Available(
    /* [out] */ Int32* value)
{
}

ECode PlainSocketImpl::Bind(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 port)
{
    ECode ec = NOERROR;
    ArrayOf<Byte> *array = NULL;
    address->GetAddress(&array);
    ec = mNetImpl->Bind(mIfd, *array, port);
    mAddress = address;
    ArrayOf<Byte>::Free(array);
    if (port != 0) {
        mLocalport = port;
    } else {
        mNetImpl->GetSocketLocalPort(mIfd, &mLocalport);
    }
    return ec;
}

ECode PlainSocketImpl::Close()
{
//    Mutex
//    synchronized (fd) {
//        if (fd.valid()) {
//            netImpl.close(fd);
//            fd = new FileDescriptor();
//        }
//    }
}

ECode PlainSocketImpl::Connect(
    /* [in] */ const String& aHost,
    /* [in] */ Int32 aPort)
{
    AutoPtr<IInetAddress> addr;
    InetAddress::GetByName(aHost, (IInetAddress**)&addr);
    return Connect((IInetAddress*)addr, aPort);
}

ECode PlainSocketImpl::Connect(
    /* [in] */ IInetAddress* anAddr,
    /* [in] */ Int32 aPort)
{
    return Connect(anAddr, aPort, 0);
}

ECode PlainSocketImpl::Create(
    /* [in] */ Boolean streaming)
{
    mStreaming = streaming;
    return mNetImpl->Socket(streaming, &mIfd);
}

ECode PlainSocketImpl::Finalize()
{
    ECode ec = NOERROR;
    ec = Close();
    if (FAILED(ec)) {
        //return SocketImpl::Finalize();
    }
    return ec;
}

ECode PlainSocketImpl::GetInputStream(
    /* [out] */ IInputStream** in)
{
    CheckNotClosed();
    CSocketInputStream::NewByFriend((ISocketImpl*)this, (CSocketInputStream**)in);
    return NOERROR;
}

ECode PlainSocketImpl::GetOutputStream(
    /* [out] */ IOutputStream** out)
{
    CheckNotClosed();
    CSocketOutputStream::NewByFriend((ISocketImpl*)this, (CSocketOutputStream**)out);
    return NOERROR;
}

ECode PlainSocketImpl::Listen(
    /* [in] */ Int32 backlog)
{
    if (UsingSocks()) {
        // Do nothing for a SOCKS connection. The listen occurs on the
        // server during the bind.
        return E_SOCKET_EXCEPTION;
    }
    return mNetImpl->Listen(mIfd, backlog);
}

ECode PlainSocketImpl::ShutdownInput()
{
    mShutdownInput = TRUE;
    return mNetImpl->ShutdownInput(mIfd);
}

ECode PlainSocketImpl::ShutdownOutput()
{
    return mNetImpl->ShutdownOutput(mIfd);
}

ECode PlainSocketImpl::Connect(
    /* [in] */ ISocketAddress* remoteAddr,
    /* [in] */ Int32 timeout)
{
    AutoPtr<IInetSocketAddress> inetAddr = (IInetSocketAddress*) remoteAddr;
    AutoPtr<IInetAddress> addr;
    inetAddr->GetAddress((IInetAddress**)&addr);
    Int32 port;
    inetAddr->GetPort(&port);
    return Connect(addr, port, timeout);
}

ECode PlainSocketImpl::SupportsUrgentData(
    /* [out] */ Boolean* value)
{
    *value = TRUE;
    return NOERROR;
}

ECode PlainSocketImpl::SendUrgentData(
    /* [in] */ Int32 value)
{
    return mNetImpl->SendUrgentData(mIfd, (Byte) value);
}

//--------------------------------------------------------private
Boolean PlainSocketImpl::UsingSocks()
{
    if (mProxy != NULL) {
        ProxyType type;
        mProxy->GetType(&type);
        if (type) {
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
}

//    Field getSocketImplField(final String fieldName)

ECode PlainSocketImpl::CheckNotClosed()
{
    Boolean value;
    mFd->Valid(&value);
    if (!value) {
        return E_SOCKET_CLOSE_EXCEPTION;
    }
    return NOERROR;
}

ECode PlainSocketImpl::Connect(
        /* [in] */ IInetAddress* anAddr,
        /* [in] */ Int32 aPort,
        /* [in] */ Int32 timeout)
{
    AutoPtr<IInetAddress> normalAddr;
    Boolean isAny;
    ECode ec = NOERROR;

    anAddr->IsAnyLocalAddress(&isAny);
    if (isAny) {
        InetAddress::GetLocalHost((IInetAddress **)&normalAddr);
    } else {
        normalAddr = anAddr;
    }

    if (mStreaming && UsingSocks()) {
        ec = SocksConnect(anAddr, aPort, 0);
    } else {
        ArrayOf<Byte> *array = NULL;
        normalAddr->GetAddress(&array);
        ec = mNetImpl->Connect(mIfd, *array, aPort, timeout);
        if (array == NULL) {
            ArrayOf<Byte>::Free(array);
        }
    }

    mAddress = normalAddr;
    mPort = aPort;
    return ec;
}

Int32 PlainSocketImpl::SocksGetServerPort()
{
    AutoPtr<IInetSocketAddress> addr;
    mProxy->GetAddress((ISocketAddress**) &addr);
    Int32 port;
    addr->GetPort(&port);
    return port;
}

AutoPtr<IInetAddress> PlainSocketImpl::SocksGetServerAddress()
{
    String proxyName;
    // get socks server address from proxy. It is unnecessary to check
    // "socksProxyHost" property, since all proxy setting should be
    // determined by ProxySelector.
    AutoPtr<IInetSocketAddress> addr;
    mProxy->GetAddress((ISocketAddress**) &addr);

    addr->GetHostName(&proxyName);

    if (proxyName.IsNull()) {
        AutoPtr<IInetAddress> iaddr;
        addr->GetAddress((IInetAddress**) &iaddr);

        iaddr->GetHostAddress(&proxyName);
    }

    IInetAddress* retAddr;
    InetAddress::GetByName(proxyName, (IInetAddress**)&retAddr);
    return retAddr;
}

ECode PlainSocketImpl::SocksConnect(
    /* [in] */ IInetAddress* applicationServerAddress,
    /* [in] */ Int32 applicationServerPort,
    /* [in] */ Int32 timeout)
{
    ECode ec = NOERROR;
    AutoPtr<IInetAddress> addr;
    addr = SocksGetServerAddress();
    ArrayOf<Byte>* array = NULL;
    addr->GetAddress(&array);
    ec = mNetImpl->Connect(mIfd, *array, SocksGetServerPort(), timeout);
    if (array != NULL) {
        ArrayOf<Byte>::Free(array);
    }
    if (FAILED(ec)) return ec;

    ec = SocksRequestConnection(applicationServerAddress, applicationServerPort);

    sLastConnectedAddress = applicationServerAddress;
    sLastConnectedPort = applicationServerPort;
    return ec;
}

ECode PlainSocketImpl::SocksRequestConnection(
    /* [in] */ IInetAddress* applicationServerAddress,
    /* [in] */ Int32 applicationServerPort)
{
    SocksSendRequest(Socks4Message::COMMAND_CONNECT,
            applicationServerAddress, applicationServerPort);

    Socks4Message* reply = NULL;
    SocksReadReply((Socks4Message **)&reply);
    if (reply != NULL) {
        if (reply->GetCommandOrResult() != Socks4Message::RETURN_SUCCESS) {
            delete reply;
            return E_SOCKET_EXCEPTION;
        }
        delete reply;
    }

    return NOERROR;
}

ECode PlainSocketImpl::SocksBind()
{
    ECode ec = NOERROR;
    AutoPtr<IInetAddress> addr;
    addr = SocksGetServerAddress();
    ArrayOf<Byte>* array = NULL;
    addr->GetAddress(&array);
    ec = mNetImpl->Connect(mIfd, *array, SocksGetServerPort(), 0);
    if (array != NULL) {
        ArrayOf<Byte>::Free(array);
    }
    if (FAILED(ec)) {
        return ec;
    }

    // There must be a connection to an application host for the bind to
    // work.
    if (sLastConnectedAddress == NULL) {
        return E_INVALID_SOCKET_EXCEPTION;
    }

    // Use the last connected address and port in the bind request.
    SocksSendRequest(Socks4Message::COMMAND_BIND, sLastConnectedAddress,
            sLastConnectedPort);

    Socks4Message* reply = NULL;
    SocksReadReply((Socks4Message**)&reply);

    if (reply == NULL) {
        return E_SOCKET_EXCEPTION;
    }
    if (reply->GetCommandOrResult() != Socks4Message::RETURN_SUCCESS) {
        return E_SOCKET_EXCEPTION;
    }

    // A peculiarity of socks 4 - if the address returned is 0, use the
    // original socks server address.
    if (reply->GetIP() == 0) {
        mAddress = (IInetAddress*)SocksGetServerAddress();
    } else {
        // IPv6 support not yet required as
        // currently the Socks4Message.getIP() only returns int,
        // so only works with IPv4 4byte addresses
        ArrayOf<Byte> *replyBytes = ArrayOf<Byte>::Alloc(4);
        IntToBytes(reply->GetIP(), replyBytes, 0);
        InetAddress::GetByAddress(*replyBytes, (IInetAddress**)&mAddress);
    }

    mLocalport = reply->GetPort();
    delete reply;
    return NOERROR;
}

//static
ECode PlainSocketImpl::IntToBytes(
    /* [in] */ Int32 value,
    /* [in] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 start)
{
    (*bytes)[start] = (Byte) ((value >> 24) & 255);
    (*bytes)[start + 1] = (Byte) ((value >> 16) & 255);
    (*bytes)[start + 2] = (Byte) ((value >> 8) & 255);
    (*bytes)[start + 3] = (Byte) (value & 255);
    return NOERROR;
}

ECode PlainSocketImpl::SocksSendRequest(
    /* [in] */ Int32 command,
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 port)
{
    ECode ec = NOERROR;
    Socks4Message *request = new Socks4Message();

    request->SetCommandOrResult(command);
    request->SetPort(port);
    ArrayOf<Byte>* array = NULL;
    mAddress->GetAddress(&array);
    request->SetIP(*array);
    if (array != NULL) {
        ArrayOf<Byte>::Free(array);
    }
    request->SetUserId(String("default"));

    AutoPtr<IOutputStream> out;
    GetOutputStream((IOutputStream**)&out);

    ec = out->WriteBufferEx(0, request->GetLength(), *(request->GetBytes()));
    delete request;
    return ec;
}

ECode PlainSocketImpl::SocksReadReply(
    /* [out] */ Socks4Message** ppMessage)
{
    Socks4Message* reply = new Socks4Message();

    Int32 bytesRead = 0;

    while (bytesRead < Socks4Message::REPLY_LENGTH) {
        Int32 count;
        AutoPtr<IInputStream> in;
        GetInputStream((IInputStream**)&in);
        in->ReadBufferEx(bytesRead, Socks4Message::REPLY_LENGTH - bytesRead, reply->GetBytes(), &count);
        if (-1 == count) {
            break;
        }
        bytesRead += count;
    }
    if (Socks4Message::REPLY_LENGTH != bytesRead) {
        return E_MALFORMED_REPLY_EXCEPTION;
    }

    *ppMessage = reply;
    return NOERROR;
}