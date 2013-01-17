
#include "cmdef.h"
#include "DatagramSocket.h"
#include "CInet4Address.h"
#include "CDatagramPacket.h"
#include "CInetSocketAddress.h"


AutoPtr<IDatagramSocketImplFactory> DatagramSocket::mFactory;
Mutex DatagramSocket::sLock;

DatagramSocket::DatagramSocket()
    : mPort(-1)
    , mIsBound(FALSE)
    , mIsConnected(FALSE)
    , mIsClosed(FALSE)
{}

ECode DatagramSocket::Init()
{
    return Init(0);
}

ECode DatagramSocket::Init(
    /* [in] */ Int32 aPort)
{
    FAIL_RETURN(CheckListen(aPort));
    return CreateSocket(aPort, CInet4Address::ANY.Get());
}

ECode DatagramSocket::Init(
    /* [in] */ Int32 aport,
    /* [in] */ IInetAddress* addr)
{
    FAIL_RETURN(CheckListen(aport));
    return CreateSocket(aport, NULL == addr ? CInet4Address::ANY.Get() : addr);
}

ECode DatagramSocket::Init(
    /* [in] */ ISocketAddress* localAddr)
{
    if (localAddr != NULL) {
        if (IInetSocketAddress::Probe(localAddr) == NULL) {
//            throw new IllegalArgumentException("Local address not an InetSocketAddress: " +
//                    localAddr.getClass());
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        IInetSocketAddress* netSocketAddr = IInetSocketAddress::Probe(localAddr);
        Int32 port;
        netSocketAddr->GetPort(&port);
        FAIL_RETURN(CheckListen(port));
    }
    if (mFactory != NULL) {
        mFactory->CreateDatagramSocketImpl((IDatagramSocketImpl**)&mImpl);
    }
    else {
//        new PlainDatagramSocketImpl();
        assert(0);
        return E_NOT_IMPLEMENTED;
    }
    mImpl->Create();
    if (localAddr != NULL) {
//        try {
        ECode ec = Bind(localAddr);
        if (FAILED(ec)) {
            Close();
            return ec;
        }
//        } catch (SocketException e) {
//            close();
//            throw e;
//        }
    }
    // SocketOptions.SO_BROADCAST is set by default for DatagramSocket
    return SetBroadcast(TRUE);
}

ECode DatagramSocket::CheckListen(
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

ECode DatagramSocket::Close()
{
    mIsClosed = TRUE;
    return mImpl->Close();
}

ECode DatagramSocket::Connect(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 aPort)
{
    if (address == NULL || aPort < 0 || aPort > 65535) {
//        throw new IllegalArgumentException("Address null or destination port out of range");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Mutex::Autolock lock(mLock);

    Boolean isClosed;
    if (IsClosed(&isClosed), isClosed) {
        return NOERROR;
    }
//    try {
    CheckClosedAndBind(TRUE);
//    } catch (SocketException e) {
        // Ignored
//    }

//    SecurityManager security = System.getSecurityManager();
//    if (security != NULL) {
//        Boolean isMulticastAddress;
//        address->IsMulticastAddress(&isMulticastAddress);
//        if (isMulticastAddress) {
//            security.checkMulticast(address);
//        }
//        else {
//            String hostName;
//            address->GetHostName(&hostName);
//            security.checkConnect(hostName, aPort);
//        }
//    }

//    try {
    mImpl->Connect(address, aPort);
//    } catch (SocketException e) {
        // not connected at the native level just do what we did before
//    }
    mAddress = address;
    mPort = aPort;
    mIsConnected = TRUE;

    return NOERROR;
}

ECode DatagramSocket::Disconnect()
{
    Boolean isClosed, isConnected;
    if ((IsClosed(&isClosed), isClosed) || (IsConnected(&isConnected), !isConnected)) {
        return NOERROR;
    }
    mImpl->Disconnect();
    mAddress = NULL;
    mPort = -1;
    mIsConnected = FALSE;

    return NOERROR;
}

ECode DatagramSocket::CreateSocket(
    /* [in] */ Int32 aPort,
    /* [in] */ IInetAddress* addr)
{
    Mutex::Autolock lock(GetSelfLock());

    if (mFactory != NULL) {
        mImpl = NULL;
        mFactory->CreateDatagramSocketImpl((IDatagramSocketImpl**)&mImpl);
    }
    else {
//        new PlainDatagramSocketImpl();
        assert(0);
        return E_NOT_IMPLEMENTED;
    }
    mImpl->Create();

//    try {
    ECode ec = mImpl->Bind(aPort, addr);
    if (FAILED(ec)) {
        Close();
        return ec;
    }
    mIsBound = TRUE;
//    } catch (SocketException e) {
//        close();
//        throw e;
//    }

    return NOERROR;
}

ECode DatagramSocket::GetInetAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);
    *address = mAddress;
    if (*address != NULL) (*address)->AddRef();
    return NOERROR;
}

ECode DatagramSocket::GetLocalAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    Boolean isClosed;
    if (IsClosed(&isClosed), isClosed) {
        *address = NULL;
        return NOERROR;
    }

    Boolean isBound;
    if (IsBound(&isBound), !isBound) {
        *address = CInet4Address::ANY;
        (*address)->AddRef();
        return NOERROR;
    }
    AutoPtr<IInetAddress> anAddr;
    mImpl->GetLocalAddress((IInetAddress**)&anAddr);
//    try {
//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        String hostName;
//        anAddr->GetHostName(&hostName);
//        security.checkConnect(hostName, -1);
//    }
//    } catch (SecurityException e) {
//        return Inet4Address.ANY;
//    }
    *address = anAddr;
    if (*address != NULL) (*address)->AddRef();

    return NOERROR;
}

ECode DatagramSocket::GetLocalPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);

    Boolean isClosed;
    if (IsClosed(&isClosed), isClosed) {
        *port = -1;
        return NOERROR;
    }

    Boolean isBound;
    if (IsBound(&isBound), !isBound) {
        *port = 0;
        return NOERROR;
    }

    return mImpl->GetLocalPort(port);
}

ECode DatagramSocket::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);

    *port = mPort;
    return NOERROR;
}

Boolean DatagramSocket::IsMulticastSocket()
{
    return FALSE;
}

ECode DatagramSocket::GetReceiveBufferSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    Mutex::Autolock lock(GetSelfLock());

    FAIL_RETURN(CheckClosedAndBind(FALSE));
    AutoPtr<IInteger32> optVal;
    FAIL_RETURN(mImpl->GetOption(SocketOption_SO_RCVBUF, (IInterface**)&optVal));
    return optVal->GetValue(size);
}

ECode DatagramSocket::GetSendBufferSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    Mutex::Autolock lock(GetSelfLock());

    FAIL_RETURN(CheckClosedAndBind(FALSE));
    AutoPtr<IInteger32> optVal;
    FAIL_RETURN(mImpl->GetOption(SocketOption_SO_SNDBUF, (IInterface**)&optVal));
    return optVal->GetValue(size);
}

ECode DatagramSocket::GetSoTimeout(
    /* [out] */ Int32* timeout)
{
    VALIDATE_NOT_NULL(timeout);

    Mutex::Autolock lock(GetSelfLock());

    FAIL_RETURN(CheckClosedAndBind(FALSE));
    AutoPtr<IInteger32> optVal;
    FAIL_RETURN(mImpl->GetOption(SocketOption_SO_TIMEOUT, (IInterface**)&optVal));
    return optVal->GetValue(timeout);
}

ECode DatagramSocket::Receive(
    /* [in] */ IDatagramPacket* pack)
{
    Mutex::Autolock lock(GetSelfLock());

    FAIL_RETURN(CheckClosedAndBind(TRUE));

    AutoPtr<IInetAddress> senderAddr;
    Int32 senderPort;
    AutoPtr<IDatagramPacket> tempPack;
    FAIL_RETURN(CDatagramPacket::New(ArrayOf<Byte>::Alloc(1), 1, (IDatagramPacket**)&tempPack));

    // means that we have received the packet into the temporary buffer
    Boolean copy = FALSE;

//    SecurityManager security = System.getSecurityManager();

    if (mAddress != NULL/* || security != NULL*/) {
        // The socket is connected or we need to check security permissions

        // Check pack before peeking
        if (pack == NULL) {
//            throw new NullPointerException();
            return E_NULL_POINTER_EXCEPTION;
        }

        // iterate over incoming packets
        while (TRUE) {
            copy = FALSE;

            // let's get sender's address and port
//            try {
            mImpl->PeekData(tempPack, &senderPort);
            tempPack->GetAddress((IInetAddress**)&senderAddr);
            // } catch (SocketException e) {
            //     if (e.getMessage().equals(
            //             "The socket does not support the operation")) {
            //         // receive packet to temporary buffer
            //         tempPack = new DatagramPacket(new byte[pack.getCapacity()],
            //                 pack.getCapacity());
            //         impl.receive(tempPack);
            //         // tempPack's length field is now updated, capacity is unchanged
            //         // let's extract address & port
            //         senderAddr = tempPack.getAddress();
            //         senderPort = tempPack.getPort();
            //         copy = true;
            //     } else {
            //         throw e;
            //     }
            // }

            if (mAddress == NULL) {
                // if we are not connected let's check if we are allowed to
                // receive packets from sender's address and port
//                try {
//                security.checkAccept(senderAddr.getHostName(),
//                        senderPort);
                    // address & port are valid
                break;
//                } catch (SecurityException e) {
//                    if (!copy) {
                        // drop this packet and continue
//                        impl.receive(tempPack);
//                    }
//                }
            }
            else if (mPort == senderPort/* && mAddress->Equals(senderAddr)*/) {
                // we are connected and the packet came
                // from the address & port we are connected to
                break;
            }
            else if (!copy) {
                // drop packet and continue
                mImpl->Receive(tempPack);
            }
        }
    }

    if (copy) {
//        System.arraycopy(tempPack.getData(), 0, pack.getData(), pack
//                .getOffset(), tempPack.getLength());
        // we shouldn't update the pack's capacity field in order to be
        // compatible with RI
        Int32 length, port;
        AutoPtr<IInetAddress> inetAddress;
        tempPack->GetLength(&length);
        tempPack->GetAddress((IInetAddress**)&inetAddress);
        tempPack->GetPort(&port);
        ((CDatagramPacket*)pack)->SetLengthOnly(length);
        pack->SetAddress(inetAddress);
        pack->SetPort(port);
    }
    else {
        pack->SetLength(((CDatagramPacket*)pack)->GetCapacity());
        mImpl->Receive(pack);
        // pack's length field is now updated by native code call;
        // pack's capacity field is unchanged
    }
    return NOERROR;
}

ECode DatagramSocket::Send(
    /* [in] */ IDatagramPacket* pack)
{
    FAIL_RETURN(CheckClosedAndBind(TRUE));

    AutoPtr<IInetAddress> packAddr;
    pack->GetAddress((IInetAddress**)&packAddr);
    if (mAddress != NULL) { // The socket is connected
        if (packAddr != NULL) {
            Int32 port;
            pack->GetPort(&port);
            if (mAddress != packAddr/*!address.equals(packAddr)*/ || mPort != port) {
//                throw new IllegalArgumentException("Packet address mismatch with connected address");
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
        }
        else {
            pack->SetAddress(mAddress);
            pack->SetPort(mPort);
        }
    }
    else {
        // not connected so the target address is not allowed to be null
        if (packAddr == NULL) {
            Int32 port;
            pack->GetPort(&port);
            if (port == -1) {
//                throw new NullPointerException("Destination address is null");
                return E_NULL_POINTER_EXCEPTION;
            }
            return NOERROR;
        }
//        SecurityManager security = System.getSecurityManager();
//        if (security != NULL) {
//            Boolean isMulticastAddress;
//            packAddr->IsMulticastAddress(&isMulticastAddress);
//            if (isMulticastAddress) {
//                security.checkMulticast(packAddr);
//            }
//            else {
//                String hostName;
//                Int32 port;
//                packAddr->GetHostName(&hostName);
//                pack->GetPort(&port);
//                security.checkConnect(hostName, port);
//            }
//        }
    }
    return mImpl->Send(pack);
}

ECode DatagramSocket::SetSendBufferSize(
    /* [in] */ Int32 size)
{
    Mutex::Autolock lock(GetSelfLock());

    if (size < 1) {
//        throw new IllegalArgumentException("size < 1");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    FAIL_RETURN(CheckClosedAndBind(FALSE));
    AutoPtr<IInteger32> optVal;
    CInteger32::New(size, (IInteger32**)&optVal);
    return mImpl->SetOption(SocketOption_SO_SNDBUF, optVal);
}

ECode DatagramSocket::SetReceiveBufferSize(
    /* [in] */ Int32 size)
{
    Mutex::Autolock lock(GetSelfLock());

    if (size < 1) {
//        throw new IllegalArgumentException("size < 1");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    FAIL_RETURN(CheckClosedAndBind(FALSE));
    AutoPtr<IInteger32> optVal;
    CInteger32::New(size, (IInteger32**)&optVal);
    return mImpl->SetOption(SocketOption_SO_RCVBUF, optVal);
}

ECode DatagramSocket::SetSoTimeout(
    /* [in] */ Int32 timeout)
{
    Mutex::Autolock lock(GetSelfLock());

    if (timeout < 0) {
//        throw new IllegalArgumentException("timeout < 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    FAIL_RETURN(CheckClosedAndBind(FALSE));
    AutoPtr<IInteger32> optVal;
    CInteger32::New(timeout, (IInteger32**)&optVal);
    return mImpl->SetOption(SocketOption_SO_TIMEOUT, optVal);
}

ECode DatagramSocket::SetDatagramSocketImplFactory(
    /* [in] */ IDatagramSocketImplFactory* fac)
{
    Mutex::Autolock lock(&sLock);

//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkSetFactory();
//    }
    if (mFactory != NULL) {
//        throw new SocketException("Factory already set");
        return E_SOCKET_EXCEPTION;
    }
    mFactory = fac;
    return NOERROR;
}

ECode DatagramSocket::CheckClosedAndBind(
    /* [in] */ Boolean bind)
{
    Boolean isClosed;
    if (IsClosed(&isClosed), isClosed) {
//        throw new SocketException("Socket is closed");
        return E_SOCKET_EXCEPTION;
    }
    Boolean isBound;
    if (bind && (IsBound(&isBound), !isBound)) {
        FAIL_RETURN(CheckListen(0));
        FAIL_RETURN(mImpl->Bind(0, CInet4Address::ANY));
        mIsBound = TRUE;
    }

    return NOERROR;
}

ECode DatagramSocket::Bind(
    /* [in] */ ISocketAddress* localAddr)
{
    FAIL_RETURN(CheckClosedAndBind(FALSE));
    Int32 localPort = 0;
    AutoPtr<IInetAddress> addr = CInet4Address::ANY;
    if (localAddr != NULL) {
        if (IInetSocketAddress::Probe(localAddr) == NULL) {
//            throw new IllegalArgumentException("Local address not an InetSocketAddress: " +
//                    localAddr.getClass());
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        IInetSocketAddress* inetAddr = IInetSocketAddress::Probe(localAddr);
        addr = NULL;
        inetAddr->GetAddress((IInetAddress**)&addr);
        if (addr == NULL) {
//            throw new SocketException("Host is unresolved: " + inetAddr.getHostName());
            return E_SOCKET_EXCEPTION;
        }
        inetAddr->GetPort(&localPort);
        FAIL_RETURN(CheckListen(localPort));
    }
    FAIL_RETURN(mImpl->Bind(localPort, addr));
    mIsBound = TRUE;
    return NOERROR;
}

ECode DatagramSocket::ConnectEx(
    /* [in] */ ISocketAddress* remoteAddr)
{
    if (remoteAddr == NULL) {
//        throw new IllegalArgumentException("remoteAddr == null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (IInetSocketAddress::Probe(remoteAddr) == NULL) {
//        throw new IllegalArgumentException("Remote address not an InetSocketAddress: " +
//                remoteAddr.getClass());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    IInetSocketAddress* inetAddr = IInetSocketAddress::Probe(remoteAddr);
    AutoPtr<IInetAddress> addr;
    inetAddr->GetAddress((IInetAddress**)&addr);
    if (addr == NULL) {
//        throw new SocketException("Host is unresolved: " + inetAddr.getHostName());
        return E_SOCKET_EXCEPTION;
    }

    Mutex::Autolock lock(mLock);
    // make sure the socket is open
    FAIL_RETURN(CheckClosedAndBind(TRUE));

//   SecurityManager security = System.getSecurityManager();
//   if (security != null) {
//        Boolean isMulticastAddress;
//        addr->IsMulticastAddress(&isMulticastAddress);
//        if (isMulticastAddress) {
//            security.checkMulticast(inetAddr.getAddress());
//        }
//        else {
//            String hostName;
//            Int32 port;
//            addr->GetHostName(&hostName);
//            inetAddr->GetPort(&port);
//            security.checkConnect(hostName, port);
//        }
//    }

    // now try to do the connection at the native level. To be
    // compatible for the case when the address is inaddr_any we just
    // eat the exception an act as if we are connected at the java level
//    try {
    Int32 port;
    inetAddr->GetPort(&port);
    mImpl->Connect(addr, port);
//    } catch (Exception e) {
//        // not connected at the native level just do what we did before
//    }

    // if we get here then we connected ok
    mAddress = addr;
    mPort = port;
    mIsConnected = TRUE;

    return NOERROR;
}

ECode DatagramSocket::IsBound(
    /* [out] */ Boolean* isBound)
{
    VALIDATE_NOT_NULL(isBound);
    *isBound = mIsBound;

    return NOERROR;
}

ECode DatagramSocket::IsConnected(
    /* [out] */ Boolean* isConnected)
{
    VALIDATE_NOT_NULL(isConnected);
    *isConnected = mIsConnected;

    return NOERROR;
}

ECode DatagramSocket::GetRemoteSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    VALIDATE_NOT_NULL(address);

    Boolean isConnected;
    if (IsConnected(&isConnected), !isConnected) {
        *address = NULL;
        return NOERROR;
    }

    AutoPtr<IInetAddress> addr;
    Int32 port;
    GetInetAddress((IInetAddress**)&addr);
    GetPort(&port);
    return CInetSocketAddress::New(addr, port, (IInetSocketAddress**)address);
}

ECode DatagramSocket::GetLocalSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    VALIDATE_NOT_NULL(address);

    Boolean isBound;
    if (IsBound(&isBound), !isBound) {
        *address = NULL;
        return NOERROR;
    }

    AutoPtr<IInetAddress> addr;
    Int32 port;
    GetLocalAddress((IInetAddress**)&addr);
    GetLocalPort(&port);
    return CInetSocketAddress::New(addr, port, (IInetSocketAddress**)address);
}

ECode DatagramSocket::SetReuseAddress(
    /* [in] */ Boolean reuse)
{
    FAIL_RETURN(CheckClosedAndBind(FALSE));
    AutoPtr<IBoolean> optVal;
    CBoolean::New(reuse, (IBoolean**)&optVal);
    return mImpl->SetOption(SocketOption_SO_REUSEADDR, optVal);
}

ECode DatagramSocket::GetReuseAddress(
    /* [out] */ Boolean* reuse)
{
    VALIDATE_NOT_NULL(reuse);

    FAIL_RETURN(CheckClosedAndBind(FALSE));
    AutoPtr<IBoolean> optVal;
    FAIL_RETURN(mImpl->GetOption(SocketOption_SO_REUSEADDR, (IInterface**)&optVal));
    return optVal->GetValue(reuse);
}

ECode DatagramSocket::SetBroadcast(
    /* [in] */ Boolean broadcast)
{
    FAIL_RETURN(CheckClosedAndBind(FALSE));
    AutoPtr<IBoolean> optVal;
    CBoolean::New(broadcast, (IBoolean**)&optVal);
    return mImpl->SetOption(SocketOption_SO_BROADCAST, optVal);
}

ECode DatagramSocket::GetBroadcast(
    /* [out] */ Boolean* broadcast)
{
    VALIDATE_NOT_NULL(broadcast);

    FAIL_RETURN(CheckClosedAndBind(FALSE));
    AutoPtr<IBoolean> optVal;
    FAIL_RETURN(mImpl->GetOption(SocketOption_SO_BROADCAST, (IInterface**)&optVal));
    return optVal->GetValue(broadcast);
}

ECode DatagramSocket::SetTrafficClass(
    /* [in] */ Int32 value)
{
    FAIL_RETURN(CheckClosedAndBind(FALSE));
    if (value < 0 || value > 255) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IInteger32> optVal;
    CInteger32::New(value, (IInteger32**)&optVal);
    return mImpl->SetOption(SocketOption_IP_TOS, optVal);
}

ECode DatagramSocket::GetTrafficClass(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    FAIL_RETURN(CheckClosedAndBind(FALSE));
    AutoPtr<IInteger32> optVal;
    FAIL_RETURN(mImpl->GetOption(SocketOption_IP_TOS, (IInterface**)&optVal));
    return optVal->GetValue(value);
}

ECode DatagramSocket::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    VALIDATE_NOT_NULL(isClosed);
    *isClosed = mIsClosed;

    return NOERROR;
}

ECode DatagramSocket::GetChannel(
    /* [out] */ IDatagramChannel** channel)
{
    VALIDATE_NOT_NULL(channel);
    *channel = NULL;

    return NOERROR;
}

