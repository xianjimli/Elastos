
#ifndef __SERVERSOCKET_H__
#define __SERVERSOCKET_H__

#include "Elastos.Net_server.h"
#include "CSocket.h"

using namespace Elastos;
using namespace Elastos::Core::Threading;

/**
 * This class represents a server-side socket that waits for incoming client
 * connections. A {@code ServerSocket} handles the requests and sends back an
 * appropriate reply. The actual tasks that a server socket must accomplish are
 * implemented by an internal {@code SocketImpl} instance.
 */
class ServerSocket
{
public:
    /**
     * Waits for an incoming request and blocks until the connection is opened.
     * This method returns a socket object representing the just opened
     * connection.
     *
     * @return the connection representing socket.
     * @throws IOException
     *             if an error occurs while accepting a new connection.
     */
    virtual CARAPI Accept(
        /* [out] */ ISocket** socket);

    /**
     * Checks whether the server may listen for connection requests on {@code
     * aport}. Throws an exception if the port is outside the valid range
     * {@code 0 <= aport <= 65535 }or does not satisfy the security policy.
     *
     * @param aPort
     *            the candidate port to listen on.
     */
    CARAPI CheckListen(
        /* [in] */ Int32 aPort);

    /**
     * Closes this server socket and its implementation. Any attempt to connect
     * to this socket thereafter will fail.
     *
     * @throws IOException
     *             if an error occurs while closing this socket.
     */
    virtual CARAPI Close();

    /**
     * Returns the default number of pending connections on a server socket. If
     * the backlog value maximum is reached, any subsequent incoming request is
     * rejected.
     *
     * @return int the default number of pending connection requests
     */
    static CARAPI_(Int32) DefaultBacklog();

    /**
     * Gets the local IP address of this server socket or {@code null} if the
     * socket is unbound. This is useful for multihomed hosts.
     *
     * @return the local address of this server socket.
     */
    virtual CARAPI GetInetAddress(
        /* [out] */ IInetAddress** address);

    /**
     * Gets the local port of this server socket or {@code -1} if the socket is
     * unbound.
     *
     * @return the local port this server is listening on.
     */
    virtual CARAPI GetLocalPort(
        /* [out] */ Int32* port);

    /**
     * Gets the socket {@link SocketOptions#SO_TIMEOUT accept timeout}.
     *
     * @throws IOException
     *             if the option cannot be retrieved.
     */
    virtual CARAPI GetSoTimeout(
        /* [out] */ Int32* timeout);

    /**
     * Sets the server socket implementation factory of this instance. This
     * method may only be invoked with sufficient security privilege and only
     * once during the application lifetime.
     *
     * @param aFactory
     *            the streaming socket factory to be used for further socket
     *            instantiations.
     * @throws IOException
     *             if the factory could not be set or is already set.
     */
    static CARAPI SetSocketFactory(
        /* [in] */ ISocketImplFactory* aFactory);

    /**
     * Sets the {@link SocketOptions#SO_TIMEOUT accept timeout} in milliseconds for this socket.
     * This accept timeout defines the period the socket will block waiting to
     * accept a connection before throwing an {@code InterruptedIOException}. The value
     * {@code 0} (default) is used to set an infinite timeout. To have effect
     * this option must be set before the blocking method was called.
     *
     * @param timeout the timeout in milliseconds or 0 for no timeout.
     * @throws SocketException
     *             if an error occurs while setting the option.
     */
    virtual CARAPI SetSoTimeout(
        /* [in] */ Int32 timeout);

    /**
     * Binds this server socket to the given local socket address with a maximum
     * backlog of 50 unaccepted connections. If the {@code localAddr} is set to
     * {@code null} the socket will be bound to an available local address on
     * any free port of the system.
     *
     * @param localAddr
     *            the local address and port to bind on.
     * @throws IllegalArgumentException
     *             if the {@code SocketAddress} is not supported.
     * @throws IOException
     *             if the socket is already bound or a problem occurs during
     *             binding.
     */
    virtual CARAPI Bind(
        /* [in] */ ISocketAddress* localAddr);

    /**
     * Binds this server socket to the given local socket address. If the
     * {@code localAddr} is set to {@code null} the socket will be bound to an
     * available local address on any free port of the system.
     *
     * @param localAddr the local machine address and port to bind on.
     * @param backlog the maximum number of unaccepted connections. Passing 0 or
     *     a negative value yields the default backlog of 50.
     * @throws IllegalArgumentException if the {@code SocketAddress} is not
     *     supported.
     * @throws IOException if the socket is already bound or a problem occurs
     *     during binding.
     */
    virtual CARAPI BindEx(
        /* [in] */ ISocketAddress* localAddr,
        /* [in] */ Int32 backlog);

    /**
     * Gets the local socket address of this server socket or {@code null} if
     * the socket is unbound. This is useful on multihomed hosts.
     *
     * @return the local socket address and port this socket is bound to.
     */
    virtual CARAPI GetLocalSocketAddress(
        /* [out] */ ISocketAddress** address);

    /**
     * Returns whether this server socket is bound to a local address and port
     * or not.
     *
     * @return {@code true} if this socket is bound, {@code false} otherwise.
     */
    virtual CARAPI IsBound(
        /* [out] */ Boolean* isBound);

    /**
     * Returns whether this server socket is closed or not.
     *
     * @return {@code true} if this socket is closed, {@code false} otherwise.
     */
    virtual CARAPI IsClosed(
        /* [out] */ Boolean* isClosed);

    /**
     * Sets the value for the socket option {@code SocketOptions.SO_REUSEADDR}.
     *
     * @param reuse
     *            the socket option setting.
     * @throws SocketException
     *             if an error occurs while setting the option value.
     */
    virtual CARAPI SetReuseAddress(
        /* [in] */ Boolean reuse);

    /**
     * Gets the value of the socket option {@code SocketOptions.SO_REUSEADDR}.
     *
     * @return {@code true} if the option is enabled, {@code false} otherwise.
     * @throws SocketException
     *             if an error occurs while reading the option value.
     */
    virtual CARAPI GetReuseAddress(
        /* [out] */ Boolean* reuse);

    /**
     * Sets the server socket receive buffer size {@code
     * SocketOptions.SO_RCVBUF}.
     *
     * @param size
     *            the buffer size in bytes.
     * @throws SocketException
     *             if an error occurs while setting the size or the size is
     *             invalid.
     */
    virtual CARAPI SetReceiveBufferSize(
        /* [in] */ Int32 size);

    /**
     * Gets the value for the receive buffer size socket option {@code
     * SocketOptions.SO_RCVBUF}.
     *
     * @return the receive buffer size of this socket.
     * @throws SocketException
     *             if an error occurs while reading the option value.
     */
    virtual CARAPI GetReceiveBufferSize(
        /* [out] */ Int32* size);

    /**
     * Gets the related channel if this instance was created by a
     * {@code ServerSocketChannel}. The current implementation returns always {@code
     * null}.
     *
     * @return the related {@code ServerSocketChannel} if any.
     */
    virtual CARAPI GetChannel(
        /* [out] */ IServerSocketChannel** channel);

    /**
     * Sets performance preferences for connection time, latency and bandwidth.
     * <p>
     * This method does currently nothing.
     *
     * @param connectionTime
     *            the value representing the importance of a short connecting
     *            time.
     * @param latency
     *            the value representing the importance of low latency.
     * @param bandwidth
     *            the value representing the importance of high bandwidth.
     */
    virtual CARAPI SetPerformancePreferences(
        /* [in] */ Int32 connectionTime,
        /* [in] */ Int32 latency,
        /* [in] */ Int32 bandwidth);

protected:
    ServerSocket();

    /**
     * Unspecified constructor needed by ServerSocketChannelImpl.ServerSocketAdapter.
     *
     * @hide
     */
    CARAPI Init(
        /* [in] */ ISocketImpl* impl);

    /**
     * Constructs a new {@code ServerSocket} instance which is not bound to any
     * port. The default number of pending connections may be backlogged.
     *
     * @throws IOException
     *             if an error occurs while creating the server socket.
     */
    CARAPI Init();

    /**
     * Constructs a new {@code ServerSocket} instance bound to the nominated
     * port on the localhost. The default number of pending connections may be
     * backlogged. If {@code aport} is 0 a free port is assigned to the socket.
     *
     * @param aport
     *            the port number to listen for connection requests on.
     * @throws IOException
     *             if an error occurs while creating the server socket.
     */
    CARAPI Init(
        /* [in] */ Int32 aPort);

    /**
     * Constructs a new {@code ServerSocket} instance bound to the nominated
     * port on the localhost. The number of pending connections that may be
     * backlogged is specified by {@code backlog}. If {@code aport} is 0 a free
     * port is assigned to the socket.
     *
     * @param aport
     *            the port number to listen for connection requests on.
     * @param backlog
     *            the number of pending connection requests, before requests
     *            will be rejected.
     * @throws IOException
     *             if an error occurs while creating the server socket.
     */
    CARAPI Init(
        /* [in] */ Int32 aPort,
        /* [in] */ Int32 backlog);

    /**
     * Constructs a new {@code ServerSocket} instance bound to the nominated
     * local host address and port. The number of pending connections that may
     * be backlogged is specified by {@code backlog}. If {@code aport} is 0 a
     * free port is assigned to the socket.
     *
     * @param aport
     *            the port number to listen for connection requests on.
     * @param localAddr
     *            the local machine address to bind on.
     * @param backlog
     *            the number of pending connection requests, before requests
     *            will be rejected.
     * @throws IOException
     *             if an error occurs while creating the server socket.
     */
    CARAPI Init(
        /* [in] */ Int32 aPort,
        /* [in] */ Int32 backlog,
        /* [in] */ IInetAddress* localAddr);

    /**
     * Invokes the server socket implementation to accept a connection on the
     * given socket {@code aSocket}.
     *
     * @param aSocket
     *            the concrete {@code SocketImpl} to accept the connection
     *            request on.
     * @throws IOException
     *             if the connection cannot be accepted.
     */
    CARAPI ImplAccept(
        /* [in] */ CSocket* aSocket);

private:
    /**
     * Checks whether the socket is closed, and throws an exception.
     */
    CARAPI CheckClosedAndCreate(
        /* [in] */ Boolean create);

public:
    AutoPtr<ISocketImpl> mImpl;

    static AutoPtr<ISocketImplFactory> sFactory;

private:
    Boolean mIsCreated;

    Boolean mIsBound;

    Boolean mIsClosed;

    Mutex mLock;
    static Mutex sLock;
};

#endif //__SERVERSOCKET_H__
