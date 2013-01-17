
#ifndef __DATAGRAMSOCKETIMPL_H__
#define __DATAGRAMSOCKETIMPL_H__

#include "Elastos.Net_server.h"
#include <elastos/AutoPtr.h>


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

    /**
     * Binds the datagram socket to the given localhost/port. Sockets must be
     * bound prior to attempting to send or receive data.
     *
     * @param port
     *            the port on the localhost to bind.
     * @param addr
     *            the address on the multihomed localhost to bind.
     * @throws SocketException
     *                if an error occurs while binding, for example, if the port
     *                has been already bound.
     */
    virtual CARAPI Bind(
        /* [in] */ Int32 port,
        /* [in] */ IInetAddress* addr) = 0;

    /**
     * Closes this socket.
     */
    virtual CARAPI Close() = 0;

    /**
     * This method allocates the socket descriptor in the underlying operating
     * system.
     *
     * @throws SocketException
     *             if an error occurs while creating the socket.
     */
    virtual CARAPI Create() = 0;

    /**
     * Gets the {@code FileDescriptor} of this datagram socket, which is invalid
     * if the socket is closed or not bound.
     *
     * @return the current file descriptor of this socket.
     */
    virtual CARAPI GetFileDescriptor(
        /* [out] */ IFileDescriptor** fileDescriptor);

    /**
     * Gets the local address to which the socket is bound.
     *
     * @return the local address to which the socket is bound.
     */
    virtual CARAPI GetLocalAddress(
        /* [out] */ IInetAddress** address);

    /**
     * Gets the local port of this socket.
     *
     * @return the local port to which this socket is bound.
     */
    virtual CARAPI GetLocalPort(
        /* [out] */ Int32* port);

    /**
     * Gets the time-to-live (TTL) for multicast packets sent on this socket.
     *
     * @return the time-to-live option as a byte value.
     * @throws IOException
     *             if an error occurs while getting the time-to-live option
     *             value.
     * @deprecated Replaced by {@link #getTimeToLive}
     * @see #getTimeToLive()
     */
    // @Deprecated
    virtual CARAPI GetTTL(
        /* [out] */ Byte* ttl) = 0;

    /**
     * Gets the time-to-live (TTL) for multicast packets sent on this socket.
     * The TTL option defines how many routers a packet may be pass before it is
     * discarded.
     *
     * @return the time-to-live option as an integer value.
     * @throws IOException
     *             if an error occurs while getting the time-to-live option
     *             value.
     */
    virtual CARAPI GetTimeToLive(
        /* [out] */ Int32* option) = 0;

    /**
     * Adds this socket to the multicast group {@code addr}. A socket must join
     * a group before being able to receive data. Further, a socket may be a
     * member of multiple groups but may join any group only once.
     *
     * @param addr
     *            the multicast group to which this socket has to be joined.
     * @throws IOException
     *             if an error occurs while joining the specified multicast
     *             group.
     */
    virtual CARAPI Join(
        /* [in] */ IInetAddress* addr) = 0;

    /**
     * Adds this socket to the multicast group {@code addr}. A socket must join
     * a group before being able to receive data. Further, a socket may be a
     * member of multiple groups but may join any group only once.
     *
     * @param addr
     *            the multicast group to which this socket has to be joined.
     * @param netInterface
     *            the local network interface which will receive the multicast
     *            datagram packets.
     * @throws IOException
     *             if an error occurs while joining the specified multicast
     *             group.
     */
    virtual CARAPI JoinGroup(
        /* [in] */ ISocketAddress* addr,
        /* [in] */ INetworkInterface* netInterface) = 0;

    /**
     * Removes this socket from the multicast group {@code addr}.
     *
     * @param addr
     *            the multicast group to be left.
     * @throws IOException
     *             if an error occurs while leaving the group or no multicast
     *             address was assigned.
     */
    virtual CARAPI Leave(
        /* [in] */ IInetAddress* addr) = 0;

    /**
     * Removes this socket from the multicast group {@code addr}.
     *
     * @param addr
     *            the multicast group to be left.
     * @param netInterface
     *            the local network interface on which this socket has to be
     *            removed.
     * @throws IOException
     *             if an error occurs while leaving the group.
     */
    virtual CARAPI LeaveGroup(
        /* [in] */ ISocketAddress* addr,
        /* [in] */ INetworkInterface* netInterface) = 0;

    /**
     * Peeks at the incoming packet to this socket and returns the address of
     * the {@code sender}. The method will block until a packet is received or
     * timeout expires.
     *
     * @param sender
     *            the origin address of a packet.
     * @return the address of {@code sender} as an integer value.
     * @throws IOException
     *                if an error or a timeout occurs while reading the address.
     */
    virtual CARAPI Peek(
        /* [in] */ IInetAddress* sender,
        /* [out] */ Int32* addr) = 0;

    /**
     * Receives data and stores it in the supplied datagram packet {@code pack}.
     * This call will block until either data has been received or, if a timeout
     * is set, the timeout has expired. If the timeout expires an {@code
     * InterruptedIOException} is thrown.
     *
     * @param pack
     *            the datagram packet container to fill in the received data.
     * @throws IOException
     *                if an error or timeout occurs while receiving data.
     */
    virtual CARAPI Receive(
        /* [in] */ IDatagramPacket* pack) = 0;

    /**
     * Sends the given datagram packet {@code pack}. The packet contains the
     * data and the address and port information of the target host as well.
     *
     * @param pack
     *            the datagram packet to be sent.
     * @throws IOException
     *                if an error occurs while sending the packet.
     */
    virtual CARAPI Send(
        /* [in] */ IDatagramPacket* pack) = 0;

    /**
     * Sets the time-to-live (TTL) option for multicast packets sent on this
     * socket.
     *
     * @param ttl
     *            the time-to-live option value. Valid values are 0 &lt; ttl
     *            &lt;= 255.
     * @throws IOException
     *             if an error occurs while setting the option.
     */
    virtual CARAPI SetTimeToLive(
        /* [in] */ Int32 ttl) = 0;

    /**
     * Sets the time-to-live (TTL) option for multicast packets sent on this
     * socket.
     *
     * @param ttl
     *            the time-to-live option value. Valid values are 0 &lt; ttl
     *            &lt;= 255.
     * @throws IOException
     *             if an error occurs while setting the option.
     * @deprecated Replaced by {@link #setTimeToLive}
     * @see #setTimeToLive(int)
     */
    // @Deprecated
    virtual CARAPI SetTTL(
        /* [in] */ Byte ttl) = 0;

    /**
     * Connects this socket to the specified remote address and port.
     *
     * @param inetAddr
     *            the address of the target host which has to be connected.
     * @param port
     *            the port on the target host which has to be connected.
     * @throws SocketException
     *                if the datagram socket cannot be connected to the
     *                specified remote address and port.
     */
    virtual CARAPI Connect(
        /* [in] */ IInetAddress* inetAddr,
        /* [in] */ Int32 port);

    /**
     * Disconnects this socket from the remote host.
     */
    virtual CARAPI Disconnect();

    /**
     * Receives data into the supplied datagram packet by peeking. The data is
     * not removed from socket buffer and can be received again by another
     * {@code peekData()} or {@code receive()} call. This call blocks until
     * either data has been received or, if a timeout is set, the timeout has
     * been expired.
     *
     * @param pack
     *            the datagram packet used to store the data.
     * @return the port the packet was received from.
     * @throws IOException
     *                if an error occurs while peeking at the data.
     */
    virtual CARAPI PeekData(
        /* [in] */ IDatagramPacket* pack,
        /* [out] */ Int32* port) = 0;

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
