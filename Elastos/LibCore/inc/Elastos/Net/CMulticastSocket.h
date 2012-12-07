
#ifndef __CMULTICASTSOCKET_H__
#define __CMULTICASTSOCKET_H__

#include "_CMulticastSocket.h"
#include "DatagramSocket.h"

CarClass(CMulticastSocket), public DatagramSocket
{
public:
    CMulticastSocket();

    CARAPI GetInterface(
            /* [out] */ IInetAddress** address);

    CARAPI GetNetworkInterface(
        /* [out] */ INetworkInterface** networkInterface);

    CARAPI GetTimeToLive(
        /* [out] */ Int32* ttl);

    CARAPI GetTTL(
        /* [out] */ Byte* ttl);

    CARAPI JoinGroup(
        /* [in] */ IInetAddress* groupAddr);

    CARAPI JoinGroupEx(
        /* [in] */ ISocketAddress* groupAddress,
        /* [in] */ INetworkInterface* netInterface);

    CARAPI LeaveGroup(
        /* [in] */ IInetAddress* groupAddr);

    CARAPI LeaveGroupEx(
        /* [in] */ ISocketAddress* groupAddress,
        /* [in] */ INetworkInterface* netInterface);

    CARAPI SendEx(
        /* [in] */ IDatagramPacket* pack,
        /* [in] */ Byte ttl);

    CARAPI SetInterface(
        /* [in] */ IInetAddress* addr);

    CARAPI SetNetworkInterface(
        /* [in] */ INetworkInterface* netInterface);

    CARAPI SetTimeToLive(
        /* [in] */ Int32 ttl);

    CARAPI SetTTL(
        /* [in] */ Byte ttl);

    CARAPI CreateSocket(
        /* [in] */ Int32 aPort,
        /* [in] */ IInetAddress* addr);

    CARAPI GetLoopbackMode(
        /* [out] */ Boolean* isDisabled);

    CARAPI SetLoopbackMode(
        /* [in] */ Boolean isDisabled);

    CARAPI Close();

    CARAPI Connect(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 aPort);

    CARAPI Disconnect();

    CARAPI GetInetAddress(
        /* [out] */ IInetAddress** address);

    CARAPI GetLocalAddress(
        /* [out] */ IInetAddress** address);

    CARAPI GetLocalPort(
        /* [out] */ Int32* port);

    CARAPI GetPort(
        /* [out] */ Int32* port);

    CARAPI GetReceiveBufferSize(
        /* [out] */ Int32* size);

    CARAPI GetSendBufferSize(
        /* [out] */ Int32* size);

    CARAPI GetSoTimeout(
        /* [out] */ Int32* timeout);

    CARAPI Receive(
        /* [in, out] */ IDatagramPacket* pack);

    CARAPI Send(
        /* [in] */ IDatagramPacket* pack);

    CARAPI SetSendBufferSize(
        /* [in] */ Int32 size);

    CARAPI SetReceiveBufferSize(
        /* [in] */ Int32 size);

    CARAPI SetSoTimeout(
        /* [in] */ Int32 timeout);

    CARAPI Bind(
        /* [in] */ ISocketAddress* localAddr);

    CARAPI ConnectEx(
        /* [in] */ ISocketAddress* remoteAddr);

    CARAPI IsBound(
        /* [out] */ Boolean* isBound);

    CARAPI IsConnected(
        /* [out] */ Boolean* isConnected);

    CARAPI GetRemoteSocketAddress(
        /* [out] */ ISocketAddress** address);

    CARAPI GetLocalSocketAddress(
        /* [out] */ ISocketAddress** address);

    CARAPI SetReuseAddress(
        /* [in] */ Boolean reuse);

    CARAPI GetReuseAddress(
        /* [out] */ Boolean* reuse);

    CARAPI SetBroadcast(
        /* [in] */ Boolean broadcast);

    CARAPI GetBroadcast(
        /* [out] */ Boolean* broadcast);

    CARAPI SetTrafficClass(
        /* [in] */ Int32 value);

    CARAPI GetTrafficClass(
        /* [out] */ Int32* value);

    CARAPI IsClosed(
        /* [out] */ Boolean* isClosed);

    CARAPI GetChannel(
        /* [out] */ IDatagramChannel** channel);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 aPort);

    CARAPI constructor(
        /* [in] */ ISocketAddress* localAddr);

private:
    CARAPI CheckJoinOrLeave(
        /* [in] */ ISocketAddress* groupAddress,
        /* [in] */ INetworkInterface* netInterface);

    CARAPI CheckJoinOrLeaveEx(
        /* [in] */ IInetAddress* groupAddr);

private:
    AutoPtr<IInetAddress> mInterfaceSet;
};

#endif //__CMULTICASTSOCKET_H__
