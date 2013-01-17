
#ifndef __DATAGRAMSOCKET_H__
#define __DATAGRAMSOCKET_H__

#include "Elastos.Net_server.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;


/**
 * This class implements a UDP socket for sending and receiving {@code
 * DatagramPacket}. A {@code DatagramSocket} object can be used for both
 * endpoints of a connection for a packet delivery service.
 *
 * @see DatagramPacket
 * @see DatagramSocketImplFactory
 */
class DatagramSocket
{
public:
    /**
     * Sends prior to attempting to bind the socket, checks whether the port is
     * within the valid port range and verifies with the security manager that
     * the port may be bound by the current context.
     *
     * @param aPort
     *            the port on the localhost that is to be bound.
     */
    virtual CARAPI CheckListen(
        /* [in] */ Int32 aPort);

    /**
     * Closes this UDP datagram socket and all possibly associated channels.
     */
    // In the documentation jdk1.1.7a/guide/net/miscNet.html, this method is
    // noted as not being synchronized.
    virtual CARAPI Close();

    /**
     * Connects this UDP datagram socket to the specific target host with the
     * address {@code anAdress} on port {@code aPort}. The host and port are
     * validated, thereafter the only validation on {@code send()} and {@code
     * receive()} is to check whether the packet address/port matches the
     * connected target.
     *
     * @param anAddress
     *            the target address of this socket.
     * @param aPort
     *            the target port of this socket.
     */
    virtual CARAPI Connect(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 aPort);

    /**
     * Disconnects this UDP datagram socket from the remote host. This method
     * called on an unconnected socket does nothing.
     */
    virtual CARAPI Disconnect();

    virtual CARAPI CreateSocket(
        /* [in] */ Int32 aPort,
        /* [in] */ IInetAddress* addr);

    /**
     * Gets the {@code InetAddress} instance representing the remote address to
     * which this UDP datagram socket is connected.
     *
     * @return the remote address this socket is connected to or {@code null} if
     *         this socket is not connected.
     */
    virtual CARAPI GetInetAddress(
        /* [out] */ IInetAddress** address);

    /**
     * Gets the {@code InetAddress} instance representing the bound local
     * address of this UDP datagram socket.
     *
     * @return the local address to which this socket is bound to or {@code
     *         null} if this socket is closed.
     */
    virtual CARAPI GetLocalAddress(
        /* [out] */ IInetAddress** address);

    /**
     * Gets the local port which this socket is bound to.
     *
     * @return the local port of this socket or {@code -1} if this socket is
     *         closed and {@code 0} if it is unbound.
     */
    virtual CARAPI GetLocalPort(
        /* [out] */ Int32* port);

    /**
     * Gets the remote port which this socket is connected to.
     *
     * @return the remote port of this socket. The return value {@code -1}
     *         indicates that this socket is not connected.
     */
    virtual CARAPI GetPort(
        /* [out] */ Int32* port);

    /**
     * Indicates whether this socket is multicast or not.
     *
     * @return the return value is always {@code false}.
     */
    virtual CARAPI_(Boolean) IsMulticastSocket();

    /**
     * Gets the socket receive buffer size. ( {@code SocketOptions.SO_RCVBUF} )
     *
     * @return the input buffer size.
     * @throws SocketException
     *                if an error occurs while getting the option value.
     */
    virtual CARAPI GetReceiveBufferSize(
        /* [out] */ Int32* size);

    /**
     * Gets the socket send buffer size. ( {@code SocketOptions.SO_SNDBUF} )
     *
     * @return the output buffer size.
     * @throws SocketException
     *                if an error occurs while getting the option value.
     */
    virtual CARAPI GetSendBufferSize(
        /* [out] */ Int32* size);

    /**
     * Gets the socket {@link SocketOptions#SO_TIMEOUT receive timeout}.
     *
     * @throws SocketException
     *                if an error occurs while getting the option value.
     */
    virtual CARAPI GetSoTimeout(
        /* [out] */ Int32* timeout);

    /**
     * Receives a packet from this socket and stores it in the argument {@code
     * pack}. All fields of {@code pack} must be set according to the data
     * received. If the received data is longer than the packet buffer size it
     * is truncated. This method blocks until a packet is received or a timeout
     * has expired. If a security manager exists, its {@code checkAccept} method
     * determines whether or not a packet is discarded. Any packets from
     * unacceptable origins are silently discarded.
     *
     * @param pack
     *            the {@code DatagramPacket} to store the received data.
     * @throws IOException
     *                if an error occurs while receiving the packet.
     */
    virtual CARAPI Receive(
        /* [in, out] */ IDatagramPacket* pack);

    /**
     * Sends a packet over this socket. The packet must satisfy the security
     * policy before it may be sent. If a security manager is installed, this
     * method checks whether it is allowed to send this packet to the specified
     * address.
     *
     * @param pack
     *            the {@code DatagramPacket} which has to be sent.
     * @throws IOException
     *                if an error occurs while sending the packet.
     */
    virtual CARAPI Send(
        /* [in] */ IDatagramPacket* pack);

    /**
     * Sets the socket send buffer size. This buffer size determines which the
     * maximum packet size is that can be sent over this socket. It depends on
     * the network implementation what will happen if the packet is bigger than
     * the buffer size. ( {@code SocketOptions.SO_SNDBUF} )
     *
     * @param size
     *            the buffer size in bytes. The size must be at least one byte.
     * @throws SocketException
     *                if an error occurs while setting the option.
     */
    virtual CARAPI SetSendBufferSize(
        /* [in] */ Int32 size);

    /**
     * Sets the socket receive buffer size. This buffer size determines which
     * the maximum packet size is that can be received over this socket. It
     * depends on the network implementation what will happen if the packet is
     * bigger than the buffer size. ( {@code SocketOptions.SO_RCVBUF} )
     *
     * @param size
     *            the buffer size in bytes. The size must be at least one byte.
     * @throws SocketException
     *                if an error occurs while setting the option.
     */
    virtual CARAPI SetReceiveBufferSize(
        /* [in] */ Int32 size);

    /**
     * Sets the {@link SocketOptions#SO_TIMEOUT read timeout} in milliseconds for this socket.
     * This receive timeout defines the period the socket will block waiting to
     * receive data before throwing an {@code InterruptedIOException}. The value
     * {@code 0} (default) is used to set an infinite timeout. To have effect
     * this option must be set before the blocking method was called.
     *
     * @param timeout the timeout in milliseconds or 0 for no timeout.
     * @throws SocketException
     *                if an error occurs while setting the option.
     */
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

    virtual CARAPI CheckClosedAndBind(
        /* [in] */ Boolean bind);

    /**
     * Binds this socket to the local address and port specified by {@code
     * localAddr}. If this value is {@code null} any free port on a valid local
     * address is used.
     *
     * @param localAddr
     *            the local machine address and port to bind on.
     * @throws IllegalArgumentException
     *             if the SocketAddress is not supported
     * @throws SocketException
     *             if the socket is already bound or a problem occurs during
     *             binding.
     */
    virtual CARAPI Bind(
        /* [in] */ ISocketAddress* localAddr);

    /**
     * Connects this datagram socket to the remote host and port specified by
     * {@code remoteAddr}. The host and port are validated, thereafter the only
     * validation on {@code send()} and {@code receive()} is that the packet
     * address/port matches the connected target.
     *
     * @param remoteAddr
     *            the address and port of the target host.
     * @throws SocketException
     *                if an error occurs during connecting.
     */
    virtual CARAPI ConnectEx(
        /* [in] */ ISocketAddress* remoteAddr);

    /**
     * Determines whether the socket is bound to an address or not.
     *
     * @return {@code true} if the socket is bound, {@code false} otherwise.
     */
    virtual CARAPI IsBound(
        /* [out] */ Boolean* isBound);

    /**
     * Determines whether the socket is connected to a target host.
     *
     * @return {@code true} if the socket is connected, {@code false} otherwise.
     */
    virtual CARAPI IsConnected(
        /* [out] */ Boolean* isConnected);

    /**
     * Gets the address and port of the connected remote host. If this socket is
     * not connected yet, {@code null} is returned.
     *
     * @return the remote socket address.
     */
    virtual CARAPI GetRemoteSocketAddress(
        /* [out] */ ISocketAddress** address);

    /**
     * Gets the bound local address and port of this socket. If the socket is
     * unbound, {@code null} is returned.
     *
     * @return the local socket address.
     */
    virtual CARAPI GetLocalSocketAddress(
        /* [out] */ ISocketAddress** address);

    /**
     * Sets the socket option {@code SocketOptions.SO_REUSEADDR}. This option
     * has to be enabled if more than one UDP socket wants to be bound to the
     * same address. That could be needed for receiving multicast packets.
     * <p>
     * There is an undefined behavior if this option is set after the socket is
     * already bound.
     *
     * @param reuse
     *            the socket option value to enable or disable this option.
     * @throws SocketException
     *             if the socket is closed or the option could not be set.
     */
    virtual CARAPI SetReuseAddress(
        /* [in] */ Boolean reuse);

    /**
     * Gets the state of the socket option {@code SocketOptions.SO_REUSEADDR}.
     *
     * @return {@code true} if the option is enabled, {@code false} otherwise.
     * @throws SocketException
     *             if the socket is closed or the option is invalid.
     */
    virtual CARAPI GetReuseAddress(
        /* [out] */ Boolean* reuse);

    /**
     * Sets the socket option {@code SocketOptions.SO_BROADCAST}. This option
     * must be enabled to send broadcast messages.
     *
     * @param broadcast
     *            the socket option value to enable or disable this option.
     * @throws SocketException
     *             if the socket is closed or the option could not be set.
     */
    virtual CARAPI SetBroadcast(
        /* [in] */ Boolean broadcast);

    /**
     * Gets the state of the socket option {@code SocketOptions.SO_BROADCAST}.
     *
     * @return {@code true} if the option is enabled, {@code false} otherwise.
     * @throws SocketException
     *             if the socket is closed or the option is invalid.
     */
    virtual CARAPI GetBroadcast(
        /* [out] */ Boolean* broadcast);

    /**
     * Sets the {@see SocketOptions#IP_TOS} value for every packet sent by this socket.
     *
     * @throws SocketException
     *             if the socket is closed or the option could not be set.
     */
    virtual CARAPI SetTrafficClass(
        /* [in] */ Int32 value);

    /**
     * Returns this socket's {@see SocketOptions#IP_TOS} setting.
     *
     * @throws SocketException
     *             if the socket is closed or the option is invalid.
     */
    virtual CARAPI GetTrafficClass(
        /* [out] */ Int32* value);

    /**
     * Gets the state of this socket.
     *
     * @return {@code true} if the socket is closed, {@code false} otherwise.
     */
    virtual CARAPI IsClosed(
        /* [out] */ Boolean* isClosed);

    /**
     * Gets the related DatagramChannel of this socket. This implementation
     * returns always {@code null}.
     *
     * @return the related DatagramChannel or {@code null} if this socket was
     *         not created by a {@code DatagramChannel} object.
     */
    virtual CARAPI GetChannel(
        /* [out] */ IDatagramChannel** channel);

protected:
    DatagramSocket();

    /**
     * Constructs a UDP datagram socket which is bound to any available port on
     * the localhost.
     *
     * @throws SocketException
     *             if an error occurs while creating or binding the socket.
     */
    CARAPI Init();

    /**
     * Constructs a UDP datagram socket which is bound to the specific port
     * {@code aPort} on the localhost. Valid values for {@code aPort} are
     * between 0 and 65535 inclusive.
     *
     * @param aPort
     *            the port to bind on the localhost.
     * @throws SocketException
     *             if an error occurs while creating or binding the socket.
     */
    CARAPI Init(
        /* [in] */ Int32 aPort);

    /**
     * Constructs a UDP datagram socket which is bound to the specific local
     * address {@code addr} on port {@code aPort}. Valid values for {@code
     * aPort} are between 0 and 65535 inclusive.
     *
     * @param aPort
     *            the port to bind on the localhost.
     * @param addr
     *            the address to bind on the localhost.
     * @throws SocketException
     *             if an error occurs while creating or binding the socket.
     */
    CARAPI Init(
        /* [in] */ Int32 aport,
        /* [in] */ IInetAddress* addr);

    /**
     * Constructs a new {@code DatagramSocket} bound to the host/port specified
     * by the {@code SocketAddress} {@code localAddr} or an unbound {@code
     * DatagramSocket} if the {@code SocketAddress} is {@code null}.
     *
     * @param localAddr
     *            the local machine address and port to bind to.
     * @throws IllegalArgumentException
     *             if the SocketAddress is not supported
     * @throws SocketException
     *             if a problem occurs creating or binding the socket.
     */
    CARAPI Init(
        /* [in] */ ISocketAddress* localAddr);

    virtual CARAPI_(Mutex*) GetSelfLock() = 0;

public:
    AutoPtr<IDatagramSocketImpl> mImpl;

    AutoPtr<IInetAddress> mAddress;

    Int32 mPort;

    static AutoPtr<IDatagramSocketImplFactory> mFactory;

    Boolean mIsBound;

private:
    Boolean mIsConnected;

    Boolean mIsClosed;

    Mutex mLock;
    static Mutex sLock;
};

#endif //__DATAGRAMSOCKET_H__
