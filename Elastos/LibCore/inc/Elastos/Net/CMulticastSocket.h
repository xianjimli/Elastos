
#ifndef __CMULTICASTSOCKET_H__
#define __CMULTICASTSOCKET_H__

#include "_CMulticastSocket.h"
#include "DatagramSocket.h"

/**
 * This class implements a multicast socket for sending and receiving IP
 * multicast datagram packets.
 *
 * @see DatagramSocket
 */
CarClass(CMulticastSocket), public DatagramSocket
{
public:
    /**
     * Constructs a multicast socket, bound to any available port on the
     * localhost.
     *
     * @throws IOException
     *             if an error occurs creating or binding the socket.
     */
    CARAPI constructor();

    /**
     * Constructs a multicast socket, bound to the specified port on the
     * localhost.
     *
     * @param aPort
     *            the port to bind on the localhost.
     * @throws IOException
     *             if an error occurs creating or binding the socket.
     */
    CARAPI constructor(
        /* [in] */ Int32 aPort);

    /**
     * Constructs a {@code MulticastSocket} bound to the host/port specified by
     * the {@code SocketAddress}, or an unbound {@code DatagramSocket} if the
     * {@code SocketAddress} is {@code null}.
     *
     * @param localAddr
     *            the local machine address and port to bind to.
     * @throws IllegalArgumentException
     *             if the {@code SocketAddress} is not supported.
     * @throws IOException
     *             if an error occurs creating or binding the socket.
     * @since 1.4
     */
    CARAPI constructor(
        /* [in] */ ISocketAddress* localAddr);

    /**
     * Gets the network address used by this socket. This is useful on
     * multihomed machines.
     *
     * @return the address of the network interface through which the datagram
     *         packets are sent or received.
     * @throws SocketException
     *                if an error occurs while getting the interface address.
     */
    CARAPI GetInterface(
            /* [out] */ IInetAddress** address);

    /**
     * Gets the network interface used by this socket. This is useful on
     * multihomed machines.
     *
     * @return the network interface used by this socket or {@code null} if no
     *         interface is set.
     * @throws SocketException
     *                if an error occurs while getting the interface.
     * @since 1.4
     */
    CARAPI GetNetworkInterface(
        /* [out] */ INetworkInterface** networkInterface);

    /**
     * Gets the time-to-live (TTL) for multicast packets sent on this socket.
     *
     * @return the default value for the time-to-life field.
     * @throws IOException
     *                if an error occurs reading the default value.
     */
    CARAPI GetTimeToLive(
        /* [out] */ Int32* ttl);

    /**
     * Gets the time-to-live (TTL) for multicast packets sent on this socket.
     *
     * @return the default value for the time-to-life field.
     * @throws IOException
     *                if an error occurs reading the default value.
     * @deprecated Replaced by {@link #getTimeToLive}
     * @see #getTimeToLive()
     */
    //@Deprecated
    CARAPI GetTTL(
        /* [out] */ Byte* ttl);

    /**
     * Adds this socket to the specified multicast group. A socket must join a
     * group before data may be received. A socket may be a member of multiple
     * groups but may join any group only once.
     *
     * @param groupAddr
     *            the multicast group to be joined.
     * @throws IOException
     *                if an error occurs while joining a group.
     */
    CARAPI JoinGroup(
        /* [in] */ IInetAddress* groupAddr);

    /**
     * Adds this socket to the specified multicast group. A socket must join a
     * group before data may be received. A socket may be a member of multiple
     * groups but may join any group only once.
     *
     * @param groupAddress
     *            the multicast group to be joined.
     * @param netInterface
     *            the network interface on which the datagram packets will be
     *            received.
     * @throws IOException
     *                if the specified address is not a multicast address.
     * @throws SecurityException
     *                if the caller is not authorized to join the group.
     * @throws IllegalArgumentException
     *                if no multicast group is specified.
     * @since 1.4
     */
    CARAPI JoinGroupEx(
        /* [in] */ ISocketAddress* groupAddress,
        /* [in] */ INetworkInterface* netInterface);

    /**
     * Removes this socket from the specified multicast group.
     *
     * @param groupAddr
     *            the multicast group to be left.
     * @throws NullPointerException
     *                if {@code groupAddr} is {@code null}.
     * @throws IOException
     *                if the specified group address is not a multicast address.
     * @throws SecurityException
     *                if the caller is not authorized to leave the group.
     */
    CARAPI LeaveGroup(
        /* [in] */ IInetAddress* groupAddr);

    /**
     * Removes this socket from the specified multicast group.
     *
     * @param groupAddress
     *            the multicast group to be left.
     * @param netInterface
     *            the network interface on which the addresses should be
     *            dropped.
     * @throws IOException
     *                if the specified group address is not a multicast address.
     * @throws SecurityException
     *                if the caller is not authorized to leave the group.
     * @throws IllegalArgumentException
     *                if {@code groupAddress} is {@code null}.
     * @since 1.4
     */
    CARAPI LeaveGroupEx(
        /* [in] */ ISocketAddress* groupAddress,
        /* [in] */ INetworkInterface* netInterface);

    /**
     * Send the packet on this socket. The packet must satisfy the security
     * policy before it may be sent.
     *
     * @param pack
     *            the {@code DatagramPacket} to send
     * @param ttl
     *            the TTL setting for this transmission, overriding the socket
     *            default
     * @throws IOException
     *                if an error occurs while sending data or setting options.
     * @deprecated use {@link #setTimeToLive}.
     */
    //@Deprecated
    CARAPI SendEx(
        /* [in] */ IDatagramPacket* pack,
        /* [in] */ Byte ttl);

    /**
     * Sets the interface address used by this socket. This allows to send
     * multicast packets on a different interface than the default interface of
     * the local system. This is useful on multihomed machines.
     *
     * @param addr
     *            the multicast interface network address to set.
     * @throws SocketException
     *                if an error occurs while setting the network interface
     *                address option.
     */
    CARAPI SetInterface(
        /* [in] */ IInetAddress* addr);

    /**
     * Sets the network interface used by this socket. This is useful for
     * multihomed machines.
     *
     * @param netInterface
     *            the multicast network interface to set.
     * @throws SocketException
     *                if an error occurs while setting the network interface
     *                option.
     * @since 1.4
     */
    CARAPI SetNetworkInterface(
        /* [in] */ INetworkInterface* netInterface);

    /**
     * Sets the time-to-live (TTL) for multicast packets sent on this socket.
     * Valid TTL values are between 0 and 255 inclusive.
     *
     * @param ttl
     *            the default time-to-live field value for packets sent on this
     *            socket. {@code 0 <= ttl <= 255}.
     * @throws IOException
     *                if an error occurs while setting the TTL option value.
     */
    CARAPI SetTimeToLive(
        /* [in] */ Int32 ttl);

    /**
     * Sets the time-to-live (TTL) for multicast packets sent on this socket.
     * Valid TTL values are between 0 and 255 inclusive.
     *
     * @param ttl
     *            the default time-to-live field value for packets sent on this
     *            socket: {@code 0 <= ttl <= 255}.
     * @throws IOException
     *                if an error occurs while setting the TTL option value.
     * @deprecated Replaced by {@link #setTimeToLive}
     * @see #setTimeToLive(int)
     */
    // @Deprecated
    CARAPI SetTTL(
        /* [in] */ Byte ttl);

    // @Override
    CARAPI CreateSocket(
        /* [in] */ Int32 aPort,
        /* [in] */ IInetAddress* addr);

    /**
     * Gets the state of the {@code SocketOptions.IP_MULTICAST_LOOP}.
     *
     * @return {@code true} if the IP multicast loop is enabled, {@code false}
     *         otherwise.
     * @throws SocketException
     *             if the socket is closed or the option is invalid.
     * @since 1.4
     */
    CARAPI GetLoopbackMode(
        /* [out] */ Boolean* isDisabled);

    /**
     * Sets the {@link SocketOptions#IP_MULTICAST_LOOP}.
     *
     * @param disable
     *            true to <i>disable</i> loopback
     * @throws SocketException
     *             if the socket is closed or the option is invalid.
     * @since 1.4
     */
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
        /* [in] */ IDatagramPacket* pack);

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

private:
    CARAPI CheckJoinOrLeave(
        /* [in] */ ISocketAddress* groupAddress,
        /* [in] */ INetworkInterface* netInterface);

    CARAPI CheckJoinOrLeave(
        /* [in] */ IInetAddress* groupAddr);

    CARAPI_(Mutex*) GetSelfLock();

private:
    AutoPtr<IInetAddress> mInterfaceSet;
};

#endif //__CMULTICASTSOCKET_H__
