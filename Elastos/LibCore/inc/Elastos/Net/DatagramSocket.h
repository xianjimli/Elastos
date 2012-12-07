
#ifndef __DATAGRAMSOCKET_H__
#define __DATAGRAMSOCKET_H__

#include "Elastos.Net_server.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

class DatagramSocket
{
public:
    DatagramSocket();

    /**
     * Sends prior to attempting to bind the socket, checks whether the port is
     * within the valid port range and verifies with the security manager that
     * the port may be bound by the current context.
     *
     * @param aPort
     *            the port on the localhost that is to be bound.
     */
    CARAPI CheckListen(
        /* [in] */ Int32 aPort);

    virtual CARAPI Close();

    virtual CARAPI Connect(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 aPort);

    virtual CARAPI Disconnect();

    virtual CARAPI CreateSocket(
        /* [in] */ Int32 aPort,
        /* [in] */ IInetAddress* addr);

    virtual CARAPI GetInetAddress(
        /* [out] */ IInetAddress** address);

    virtual CARAPI GetLocalAddress(
        /* [out] */ IInetAddress** address);

    virtual CARAPI GetLocalPort(
        /* [out] */ Int32* port);

    virtual CARAPI GetPort(
        /* [out] */ Int32* port);

    /**
     * Indicates whether this socket is multicast or not.
     *
     * @return the return value is always {@code false}.
     */
    CARAPI_(Boolean) IsMulticastSocket();

    virtual CARAPI GetReceiveBufferSize(
        /* [out] */ Int32* size);

    virtual CARAPI GetSendBufferSize(
        /* [out] */ Int32* size);

    virtual CARAPI GetSoTimeout(
        /* [out] */ Int32* timeout);

    virtual CARAPI Receive(
        /* [in, out] */ IDatagramPacket* pack);

    virtual CARAPI Send(
        /* [in] */ IDatagramPacket* pack);

    virtual CARAPI SetSendBufferSize(
        /* [in] */ Int32 size);

    virtual CARAPI SetReceiveBufferSize(
        /* [in] */ Int32 size);

    virtual CARAPI SetSoTimeout(
        /* [in] */ Int32 timeout);

    /**
     * Sets the socket implementation factory. This may only be invoked once
     * over the lifetime of the application. This factory is used to create
     * a new datagram socket implementation. If a security manager is set its
     * method {@code checkSetFactory()} is called to check if the operation is
     * allowed. A {@code SecurityException} is thrown if the operation is not
     * allowed.
     *
     * @param fac
     *            the socket factory to use.
     * @throws IOException
     *                if the factory has already been set.
     * @see DatagramSocketImplFactory
     */
    static CARAPI SetDatagramSocketImplFactory(
        /* [in] */ IDatagramSocketImplFactory* fac);

    CARAPI CheckClosedAndBind(
        /* [in] */ Boolean bind);

    virtual CARAPI Bind(
        /* [in] */ ISocketAddress* localAddr);

    virtual CARAPI ConnectEx(
        /* [in] */ ISocketAddress* remoteAddr);

    virtual CARAPI IsBound(
        /* [out] */ Boolean* isBound);

    virtual CARAPI IsConnected(
        /* [out] */ Boolean* isConnected);

    virtual CARAPI GetRemoteSocketAddress(
        /* [out] */ ISocketAddress** address);

    virtual CARAPI GetLocalSocketAddress(
        /* [out] */ ISocketAddress** address);

    virtual CARAPI SetReuseAddress(
        /* [in] */ Boolean reuse);

    virtual CARAPI GetReuseAddress(
        /* [out] */ Boolean* reuse);

    virtual CARAPI SetBroadcast(
        /* [in] */ Boolean broadcast);

    virtual CARAPI GetBroadcast(
        /* [out] */ Boolean* broadcast);

    virtual CARAPI SetTrafficClass(
        /* [in] */ Int32 value);

    virtual CARAPI GetTrafficClass(
        /* [out] */ Int32* value);

    virtual CARAPI IsClosed(
        /* [out] */ Boolean* isClosed);

    virtual CARAPI GetChannel(
        /* [out] */ IDatagramChannel** channel);

    CARAPI Init();

    CARAPI Init(
        /* [in] */ Int32 aPort);

    CARAPI Init(
        /* [in] */ Int32 aport,
        /* [in] */ IInetAddress* addr);

    CARAPI Init(
        /* [in] */ ISocketAddress* localAddr);

public:
    AutoPtr<IDatagramSocketImpl> mImpl;

    AutoPtr<IInetAddress> mAddress;

    Int32 mPort;

    static AutoPtr<IDatagramSocketImplFactory> mFactory;

    Boolean mIsBound;

private:
    Boolean mIsConnected;

    Boolean mIsClosed;

    Mutex* mLock;
};

#endif //__DATAGRAMSOCKET_H__
