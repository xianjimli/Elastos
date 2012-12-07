
#ifndef __DATAGRAMSOCKETIMPL_H__
#define __DATAGRAMSOCKETIMPL_H__

#include "Elastos.Net_server.h"
#include <Com.Kortide.Platform.h>
#include <elastos/AutoPtr.h>

//extern "C" const InterfaceID EIID_ProxySelectorImpl;

/**
 * The abstract superclass for datagram and multicast socket implementations.
 */
class DatagramSocketImpl
{
public:
    /**
     * Constructs an unbound datagram socket implementation.
     */
    DatagramSocketImpl();

    virtual CARAPI Bind(
        /* [in] */ Int32 port,
        /* [in] */ IInetAddress* addr) = 0;

    virtual CARAPI Close() = 0;

    virtual CARAPI Create() = 0;

    virtual CARAPI GetFileDescriptor(
        /* [out] */ IFileDescriptor** fileDescriptor);

    virtual CARAPI GetLocalAddress(
        /* [out] */ IInetAddress** address);

    virtual CARAPI GetLocalPort(
        /* [out] */ Int32* port);

    virtual CARAPI GetTTL(
        /* [out] */ Byte* ttl) = 0;

    virtual CARAPI GetTimeToLive(
        /* [out] */ Int32* option) = 0;

    virtual CARAPI Join(
        /* [in] */ IInetAddress* addr) = 0;

    virtual CARAPI JoinGroup(
        /* [in] */ ISocketAddress* addr,
        /* [in] */ INetworkInterface* netInterface) = 0;

    virtual CARAPI Leave(
        /* [in] */ IInetAddress* addr) = 0;

    virtual CARAPI LeaveGroup(
        /* [in] */ ISocketAddress* addr,
        /* [in] */ INetworkInterface* netInterface) = 0;

    virtual CARAPI Peek(
        /* [in] */ IInetAddress* sender,
        /* [out] */ Int32* addr) = 0;

    virtual CARAPI Receive(
        /* [in] */ IDatagramPacket* pack) = 0;

    virtual CARAPI Send(
        /* [in] */ IDatagramPacket* pack) = 0;

    virtual CARAPI SetTimeToLive(
        /* [in] */ Int32 ttl) = 0;

    virtual CARAPI SetTTL(
        /* [in] */ Byte ttl) = 0;

    virtual CARAPI Connect(
        /* [in] */ IInetAddress* inetAddr,
        /* [in] */ Int32 port);

    virtual CARAPI Disconnect();

    virtual CARAPI PeekData(
        /* [in] */ IDatagramPacket* pack,
        /* [out] */ Int32* port) = 0;

    virtual CARAPI GetOption(
        /* [in] */ Int32 optID,
        /* [out] */ Int32* optVal) = 0;

    virtual CARAPI SetOption(
        /* [in] */ Int32 optID,
        /* [in] */ Int32 optVal) = 0;

protected:
    /**
     * File descriptor that is used to address this socket.
     */
    AutoPtr<IFileDescriptor> mFd;

    /**
     * The number of the local port to which this socket is bound.
     */
    Int32 mLocalPort;
};

#endif //__DATAGRAMSOCKETIMPL_H__
