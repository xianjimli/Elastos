
#include "cmdef.h"
#include "CMulticastSocket.h"
#include "InetAddress.h"
#include "CInet6Address.h"
#include "CInet4Address.h"
#include "NetworkInterface.h"

CMulticastSocket::CMulticastSocket()
    : DatagramSocket()
{}

ECode CMulticastSocket::GetInterface(
        /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    CheckClosedAndBind(FALSE);
    if (mInterfaceSet == NULL) {
        Int32 addr;
        mImpl->GetOption(SocketOption_IP_MULTICAST_IF, &addr);
        AutoPtr<IInetAddress> ipvXaddress = (IInetAddress*)addr;
        Boolean isAnyLocalAddress;
        ipvXaddress->IsAnyLocalAddress(&isAnyLocalAddress);
        if (isAnyLocalAddress) {
            // the address was not set at the IPV4 level so check the IPV6
            // level
            AutoPtr<INetworkInterface> theInterface;
            GetNetworkInterface((INetworkInterface**)&theInterface);
            if (theInterface != NULL) {
                AutoPtr<IObjectContainer> addresses;
                theInterface->GetInetAddresses((IObjectContainer**)&addresses);
                if (addresses != NULL) {
                    AutoPtr<IObjectEnumerator> enumerator;
                    addresses->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
                    Boolean hasNext;
                    while(enumerator->MoveNext(&hasNext), hasNext) {
                        AutoPtr<IInetAddress> nextAddress;
                        enumerator->Current((IInterface**)&nextAddress);
                        if (nextAddress != NULL &&
                                nextAddress->Probe(EIID_IInet6Address) != NULL) {
                            *address = nextAddress;
                            return NOERROR;
                        }
                    }
                }
            }
        }

        *address = ipvXaddress;
        return NOERROR;
    }

    *address = mInterfaceSet;
    return NOERROR;
}

ECode CMulticastSocket::GetNetworkInterface(
    /* [out] */ INetworkInterface** networkInterface)
{
    VALIDATE_NOT_NULL(networkInterface);

    CheckClosedAndBind(FALSE);

    // check if it is set at the IPV6 level. If so then use that. Otherwise
    // do it at the IPV4 level
//    Integer theIndex = Integer.valueOf(0);
//    try {
    Int32 theIndex;
    mImpl->GetOption(SocketOption_IP_MULTICAST_IF2, &theIndex);
//    } catch (SocketException e) {
        // we may get an exception if IPV6 is not enabled.
//    }

    if (theIndex != 0) {
        AutoPtr<IObjectContainer> theInterfaces;
        NetworkInterface::GetNetworkInterfaces((IObjectContainer**)&theInterfaces);
        if (theInterfaces != NULL) {
            AutoPtr<IObjectEnumerator> enumerator;
            theInterfaces->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
            Boolean hasNext;
            while(enumerator->MoveNext(&hasNext), hasNext) {
                AutoPtr<INetworkInterface> nextInterface;
                enumerator->Current((IInterface**)&nextInterface);
                if (((NetworkInterface*)nextInterface.Get())->GetIndex() == theIndex) {
                    *networkInterface = nextInterface;
                    return NOERROR;
                }
            }
        }
    }

    // ok it was not set at the IPV6 level so try at the IPV4 level
    Int32 addr;
    mImpl->GetOption(SocketOption_IP_MULTICAST_IF, &addr);
    AutoPtr<IInetAddress> theAddress = (IInetAddress*)addr;
    if (theAddress != NULL) {
        Boolean isAnyLocalAddress;
        theAddress->IsAnyLocalAddress(&isAnyLocalAddress);
        if (!isAnyLocalAddress) {
            return NetworkInterface::GetByInetAddress(theAddress, networkInterface);
        }

        // not set as we got the any address so return a dummy network
        // interface with only the any address. We do this to be
        // compatible
        ArrayOf<AutoPtr<IInetAddress> >* theAddresses = ArrayOf<AutoPtr<IInetAddress> >::Alloc(1);
        if (InetAddress::PreferIPv6Addresses()) {
            (*theAddresses)[0] = CInet6Address::ANY;
        }
        else {
            (*theAddresses)[0] = CInet4Address::ANY;
        }
        NetworkInterface* networkInt = new NetworkInterface(String(NULL), String(NULL), theAddresses,
                NetworkInterface_UNSET_INTERFACE_INDEX);
        if (networkInt->Probe(EIID_INetworkInterface) != NULL) {
            *networkInterface = (INetworkInterface*)networkInt->Probe(EIID_INetworkInterface);
            return NOERROR;
        }
    }

    // ok not set at all so return null
    *networkInterface = NULL;
    return NOERROR;
}

ECode CMulticastSocket::GetTimeToLive(
    /* [out] */ Int32* ttl)
{
    VALIDATE_NOT_NULL(ttl);

    CheckClosedAndBind(FALSE);
    return mImpl->GetTimeToLive(ttl);
}

ECode CMulticastSocket::GetTTL(
    /* [out] */ Byte* ttl)
{
    VALIDATE_NOT_NULL(ttl);

    CheckClosedAndBind(FALSE);
    return mImpl->GetTTL(ttl);
}

ECode CMulticastSocket::JoinGroup(
    /* [in] */ IInetAddress* groupAddr)
{
    CheckJoinOrLeaveEx(groupAddr);
    return mImpl->Join(groupAddr);
}

ECode CMulticastSocket::JoinGroupEx(
    /* [in] */ ISocketAddress* groupAddress,
    /* [in] */ INetworkInterface* netInterface)
{
    CheckJoinOrLeave(groupAddress, netInterface);
    return mImpl->JoinGroup(groupAddress, netInterface);
}

ECode CMulticastSocket::LeaveGroup(
    /* [in] */ IInetAddress* groupAddr)
{
    CheckJoinOrLeaveEx(groupAddr);
    return mImpl->Leave(groupAddr);
}

ECode CMulticastSocket::LeaveGroupEx(
    /* [in] */ ISocketAddress* groupAddress,
    /* [in] */ INetworkInterface* netInterface)
{
    CheckJoinOrLeave(groupAddress, netInterface);
    return mImpl->LeaveGroup(groupAddress, netInterface);
}

ECode CMulticastSocket::CheckJoinOrLeave(
    /* [in] */ ISocketAddress* groupAddress,
    /* [in] */ INetworkInterface* netInterface)
{
    CheckClosedAndBind(FALSE);
    if (groupAddress == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("groupAddress == null");
    }


    if (netInterface != NULL) {
        AutoPtr<IInetAddress> addr;
        ((NetworkInterface*)netInterface)->GetFirstAddress((IInetAddress**)&addr);
        if (addr == NULL) {
            return E_SOCKET_EXCEPTION;
//            throw new SocketException("No address associated with interface: " + netInterface);
        }
    }

    if (groupAddress->Probe(EIID_IInetSocketAddress) == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("Group address not an InetSocketAddress: " +
//                groupAddress.getClass());
    }

    AutoPtr<IInetSocketAddress> socketAddr =
            (IInetSocketAddress*)groupAddress->Probe(EIID_IInetSocketAddress);
    AutoPtr<IInetAddress> groupAddr;
    socketAddr->GetAddress((IInetAddress**)&groupAddr);
    if (groupAddr == NULL) {
        return E_SOCKET_EXCEPTION;
//        throw new SocketException("Group address has no address: " + groupAddress);
    }

    Boolean isMulticastAddress;
    groupAddr->IsMulticastAddress(&isMulticastAddress);
    if (!isMulticastAddress) {
        return E_IO_EXCEPTION;
//        throw new IOException("Not a multicast group: " + groupAddr);
    }
//    SecurityManager security = System.getSecurityManager();
//    if (security != NULL) {
//        security.checkMulticast(groupAddr);
//    }

    return NOERROR;
}

ECode CMulticastSocket::CheckJoinOrLeaveEx(
    /* [in] */ IInetAddress* groupAddr)
{
    CheckClosedAndBind(FALSE);
    Boolean isMulticastAddress;
    groupAddr->IsMulticastAddress(&isMulticastAddress);
    if (!isMulticastAddress) {
        return E_IO_EXCEPTION;
//        throw new IOException("Not a multicast group: " + groupAddr);
    }
    // SecurityManager security = System.getSecurityManager();
    // if (security != null) {
    //     security.checkMulticast(groupAddr);
    // }

    return NOERROR;
}

ECode CMulticastSocket::SendEx(
    /* [in] */ IDatagramPacket* pack,
    /* [in] */ Byte ttl)
{
    CheckClosedAndBind(FALSE);
    AutoPtr<IInetAddress> packAddr;
    pack->GetAddress((IInetAddress**)&packAddr);
//    SecurityManager security = System.getSecurityManager();
    // if (security != NULL) {
    //     Boolean isMulticastAddress;
    //     groupAddr->IsMulticastAddress(&isMulticastAddress);
    //     if (isMulticastAddress) {
    //         security.checkMulticast(packAddr, ttl);
    //     }
    //     else {
    //         String hostName;
    //         Int32 port;
    //         packAddr->GetHostName(&hostName);
    //         pack->GetPort(&port);
    //         security.checkConnect(hostName, port);
    //     }
    // }
    Int32 currTTL;
    GetTimeToLive(&currTTL);
    Boolean isMulticastAddress;
    packAddr->IsMulticastAddress(&isMulticastAddress);
    if (isMulticastAddress && currTTL != ttl) {
//        try {
        ECode ec = SetTimeToLive(ttl & 0xff);
        if (FAILED(ec)) {
            goto finally;
        }

        mImpl->Send(pack);
finally:
        SetTimeToLive(currTTL);
    }
    else {
        mImpl->Send(pack);
    }

    return NOERROR;
}

ECode CMulticastSocket::SetInterface(
    /* [in] */ IInetAddress* addr)
{
    CheckClosedAndBind(FALSE);
    if (addr == NULL) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException();
    }

    Boolean isAnyLocalAddress;
    addr->IsAnyLocalAddress(&isAnyLocalAddress);
    if (isAnyLocalAddress) {
        mImpl->SetOption(SocketOption_IP_MULTICAST_IF, (Handle32)CInet4Address::ANY.Get());
    }
    else if (addr->Probe(EIID_IInet4Address) != NULL) {
        mImpl->SetOption(SocketOption_IP_MULTICAST_IF, (Handle32)addr);
        // keep the address used to do the set as we must return the same
        // value and for IPv6 we may not be able to get it back uniquely
        mInterfaceSet = addr;
    }

    /*
     * now we should also make sure this works for IPV6 get the network
     * interface for the address and set the interface using its index
     * however if IPV6 is not enabled then we may get an exception. if IPV6
     * is not enabled
     */
    AutoPtr<INetworkInterface> theInterface;
    NetworkInterface::GetByInetAddress(addr, (INetworkInterface**)&theInterface);
    if ((theInterface != NULL) && (((NetworkInterface*)theInterface.Get())->GetIndex() != 0)) {
//        try {
        mImpl->SetOption(SocketOption_IP_MULTICAST_IF2,
                ((NetworkInterface*)theInterface.Get())->GetIndex());
//        } catch (SocketException e) {
            // Ignored
//        }
    }
    else if (isAnyLocalAddress) {
//        try {
        mImpl->SetOption(SocketOption_IP_MULTICAST_IF2, 0);
//        } catch (SocketException e) {
            // Ignored
//        }
    }
    else if (addr->Probe(EIID_IInet6Address) != NULL) {
        return E_SOCKET_EXCEPTION;
//        throw new SocketException("Address not associated with an interface: " + addr);
    }

    return NOERROR;
}

ECode CMulticastSocket::SetNetworkInterface(
    /* [in] */ INetworkInterface* netInterface)
{
    CheckClosedAndBind(FALSE);

    if (netInterface == NULL) {
        // throw a socket exception indicating that we do not support this
        return E_SOCKET_EXCEPTION;
//        throw new SocketException("netInterface == null");
    }

    AutoPtr<IInetAddress> firstAddress;
    ((NetworkInterface*)netInterface)->GetFirstAddress((IInetAddress**)&firstAddress);
    if (firstAddress == NULL) {
        return E_SOCKET_EXCEPTION;
//        throw new SocketException("No address associated with interface: " + netInterface);
    }

    if (((NetworkInterface*)netInterface)->GetIndex() == NetworkInterface_UNSET_INTERFACE_INDEX) {
        // set the address using IP_MULTICAST_IF to make sure this
        // works for both IPV4 and IPV6
        mImpl->SetOption(SocketOption_IP_MULTICAST_IF, (Handle32)CInet4Address::ANY.Get());

//        try {
            // we have the index so now we pass set the interface
            // using IP_MULTICAST_IF2. This is what is used to set
            // the interface on systems which support IPV6
        mImpl->SetOption(SocketOption_IP_MULTICAST_IF2, NetworkInterface_NO_INTERFACE_INDEX);
//        } catch (SocketException e) {
            // for now just do this, -- could be narrowed?
//        }
    }

    /*
     * Now try to set using IPV4 way. However, if interface passed in has no
     * IP addresses associated with it then we cannot do it. first we have
     * to make sure there is an IPV4 address that we can use to call set
     * interface otherwise we will not set it
     */
    AutoPtr<IObjectContainer> theAddresses;
    netInterface->GetInetAddresses((IObjectContainer**)&theAddresses);
    if (theAddresses != NULL) {
        Boolean found = FALSE;
        firstAddress = NULL;
        AutoPtr<IObjectEnumerator> enumerator;
        theAddresses->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
        Boolean hasNext;
        while(enumerator->MoveNext(&hasNext), hasNext) {
            AutoPtr<IInetAddress> theAddress;
            enumerator->Current((IInterface**)&theAddress);
            if (theAddress != NULL &&
                    theAddress->Probe(EIID_IInet4Address) != NULL) {
                found = TRUE;
            }
        }
    }

    if (((NetworkInterface*)netInterface)->GetIndex() == NetworkInterface_NO_INTERFACE_INDEX) {
        // the system does not support IPV6 and does not provide
        // indexes for the network interfaces. Just pass in the
        // first address for the network interface
        if (firstAddress != NULL) {
            mImpl->SetOption(SocketOption_IP_MULTICAST_IF, (Handle32)firstAddress.Get());
        }
        else {
            /*
             * we should never get here as there should not be any network
             * interfaces which have no IPV4 address and which does not have
             * the network interface index not set correctly
             */
            return E_SOCKET_EXCEPTION;
//            throw new SocketException("No address associated with interface: " + netInterface);
        }
    }
    else {
        // set the address using IP_MULTICAST_IF to make sure this
        // works for both IPV4 and IPV6
        if (firstAddress != NULL) {
            mImpl->SetOption(SocketOption_IP_MULTICAST_IF, (Handle32)firstAddress.Get());
        }

//        try {
        // we have the index so now we pass set the interface
        // using IP_MULTICAST_IF2. This is what is used to set
        // the interface on systems which support IPV6
        mImpl->SetOption(SocketOption_IP_MULTICAST_IF2,
                ((NetworkInterface*)netInterface)->GetIndex());
//        } catch (SocketException e) {
            // for now just do this -- could be narrowed?
//        }
    }

    mInterfaceSet = NULL;

    return NOERROR;
}

ECode CMulticastSocket::SetTimeToLive(
    /* [in] */ Int32 ttl)
{
    CheckClosedAndBind(FALSE);
    if (ttl < 0 || ttl > 255) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("TimeToLive out of bounds: " + ttl);
    }

    return mImpl->SetTimeToLive(ttl);
}

ECode CMulticastSocket::SetTTL(
    /* [in] */ Byte ttl)
{
    CheckClosedAndBind(FALSE);
    return mImpl->SetTTL(ttl);
}

ECode CMulticastSocket::CreateSocket(
    /* [in] */ Int32 aPort,
    /* [in] */ IInetAddress* addr)
{
//    mImpl = mFactory != NULL ? factory.createDatagramSocketImpl()
//            : new PlainDatagramSocketImpl();
    if (mFactory != NULL) {
        mFactory->CreateDatagramSocketImpl((IDatagramSocketImpl**)&mImpl);
    } else {
        return E_NOT_IMPLEMENTED;
//        new PlainDatagramSocketImpl();
    }

    mImpl->Create();
//    try {
    ECode ec = mImpl->Bind(aPort, addr);
    if (FAILED(ec)) {
        Close();
        return ec;
    }

    mIsBound = TRUE;
    return NOERROR;
//    } catch (SocketException e) {
//        close();
//        throw e;
//    }
}

ECode CMulticastSocket::GetLoopbackMode(
    /* [out] */ Boolean* isDisabled)
{
    VALIDATE_NOT_NULL(isDisabled);

    CheckClosedAndBind(FALSE);
    Int32 value;
    mImpl->GetOption(SocketOption_IP_MULTICAST_LOOP, &value);
    *isDisabled = !(Boolean)value;

    return NOERROR;
}

ECode CMulticastSocket::SetLoopbackMode(
    /* [in] */ Boolean isDisabled)
{
    CheckClosedAndBind(FALSE);
    mImpl->SetOption(SocketOption_IP_MULTICAST_LOOP, (Int32)!isDisabled);

    return NOERROR;
}

ECode CMulticastSocket::Close()
{
    return DatagramSocket::Close();
}

ECode CMulticastSocket::Connect(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 aPort)
{
    return DatagramSocket::Connect(address, aPort);
}

ECode CMulticastSocket::Disconnect()
{
    return DatagramSocket::Disconnect();
}

ECode CMulticastSocket::GetInetAddress(
    /* [out] */ IInetAddress** address)
{
    return DatagramSocket::GetInetAddress(address);
}

ECode CMulticastSocket::GetLocalAddress(
    /* [out] */ IInetAddress** address)
{
    return DatagramSocket::GetLocalAddress(address);
}

ECode CMulticastSocket::GetLocalPort(
    /* [out] */ Int32* port)
{
    return DatagramSocket::GetLocalPort(port);
}

ECode CMulticastSocket::GetPort(
    /* [out] */ Int32* port)
{
    return DatagramSocket::GetPort(port);
}

ECode CMulticastSocket::GetReceiveBufferSize(
    /* [out] */ Int32* size)
{
    return DatagramSocket::GetReceiveBufferSize(size);
}

ECode CMulticastSocket::GetSendBufferSize(
    /* [out] */ Int32* size)
{
    return DatagramSocket::GetSendBufferSize(size);
}

ECode CMulticastSocket::GetSoTimeout(
    /* [out] */ Int32* timeout)
{
    return DatagramSocket::GetSoTimeout(timeout);
}

ECode CMulticastSocket::Receive(
    /* [in, out] */ IDatagramPacket* pack)
{
    return DatagramSocket::Receive(pack);
}

ECode CMulticastSocket::Send(
    /* [in] */ IDatagramPacket* pack)
{
    return DatagramSocket::Send(pack);
}

ECode CMulticastSocket::SetSendBufferSize(
    /* [in] */ Int32 size)
{
    return DatagramSocket::SetSendBufferSize(size);
}

ECode CMulticastSocket::SetReceiveBufferSize(
    /* [in] */ Int32 size)
{
    return DatagramSocket::SetReceiveBufferSize(size);
}

ECode CMulticastSocket::SetSoTimeout(
    /* [in] */ Int32 timeout)
{
    return DatagramSocket::SetSoTimeout(timeout);
}

ECode CMulticastSocket::Bind(
    /* [in] */ ISocketAddress* localAddr)
{
    return DatagramSocket::Bind(localAddr);
}

ECode CMulticastSocket::ConnectEx(
    /* [in] */ ISocketAddress* remoteAddr)
{
    return DatagramSocket::ConnectEx(remoteAddr);
}

ECode CMulticastSocket::IsBound(
    /* [out] */ Boolean* isBound)
{
    return DatagramSocket::IsBound(isBound);
}

ECode CMulticastSocket::IsConnected(
    /* [out] */ Boolean* isConnected)
{
    return DatagramSocket::IsConnected(isConnected);
}

ECode CMulticastSocket::GetRemoteSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    return DatagramSocket::GetRemoteSocketAddress(address);
}

ECode CMulticastSocket::GetLocalSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    return DatagramSocket::GetLocalSocketAddress(address);
}

ECode CMulticastSocket::SetReuseAddress(
    /* [in] */ Boolean reuse)
{
    return DatagramSocket::SetReuseAddress(reuse);
}

ECode CMulticastSocket::GetReuseAddress(
    /* [out] */ Boolean* reuse)
{
    return DatagramSocket::GetReuseAddress(reuse);
}

ECode CMulticastSocket::SetBroadcast(
    /* [in] */ Boolean broadcast)
{
    return DatagramSocket::SetBroadcast(broadcast);
}

ECode CMulticastSocket::GetBroadcast(
    /* [out] */ Boolean* broadcast)
{
    return DatagramSocket::GetBroadcast(broadcast);
}

ECode CMulticastSocket::SetTrafficClass(
    /* [in] */ Int32 value)
{
    return DatagramSocket::SetTrafficClass(value);
}

ECode CMulticastSocket::GetTrafficClass(
    /* [out] */ Int32* value)
{
    return DatagramSocket::GetTrafficClass(value);
}

ECode CMulticastSocket::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    return DatagramSocket::IsClosed(isClosed);
}

ECode CMulticastSocket::GetChannel(
    /* [out] */ IDatagramChannel** channel)
{
    return DatagramSocket::GetChannel(channel);
}

ECode CMulticastSocket::constructor()
{
    FAIL_RETURN(DatagramSocket::Init());
    FAIL_RETURN(SetReuseAddress(TRUE));

    return NOERROR;
}

ECode CMulticastSocket::constructor(
    /* [in] */ Int32 aPort)
{
    FAIL_RETURN(DatagramSocket::Init(aPort));
    FAIL_RETURN(SetReuseAddress(TRUE));

    return NOERROR;
}

ECode CMulticastSocket::constructor(
    /* [in] */ ISocketAddress* localAddr)
{
    FAIL_RETURN(DatagramSocket::Init(localAddr));
    FAIL_RETURN(SetReuseAddress(TRUE));

    return NOERROR;
}

