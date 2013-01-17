
#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "Elastos.Net_server.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

class Socket
{
public:
    /**
     * Closes the socket. It is not possible to reconnect or rebind to this
     * socket thereafter which means a new socket instance has to be created.
     *
     * @throws IOException
     *             if an error occurs while closing the socket.
     */
    CARAPI Close();

    /**
     * Gets the IP address of the target host this socket is connected to.
     *
     * @return the IP address of the connected target host or {@code null} if
     *         this socket is not yet connected.
     */
    CARAPI GetInetAddress(
        /* [out] */ IInetAddress** address);

    /**
     * Gets an input stream to read data from this socket.
     *
     * @return the byte-oriented input stream.
     * @throws IOException
     *             if an error occurs while creating the input stream or the
     *             socket is in an invalid state.
     */
    CARAPI GetInputStream(
        /* [out] */ IInputStream** is);

    /**
     * Gets the setting of the socket option {@code SocketOptions.SO_KEEPALIVE}.
     *
     * @return {@code true} if the {@code SocketOptions.SO_KEEPALIVE} is
     *         enabled, {@code false} otherwise.
     * @throws SocketException
     *             if an error occurs while reading the socket option.
     * @see SocketOptions#SO_KEEPALIVE
     */
    CARAPI GetKeepAlive(
        /* [out] */ Boolean* keepAlive);

    /**
     * Returns the local IP address this socket is bound to, or {@code InetAddress.ANY} if
     * the socket is unbound.
     */
    CARAPI GetLocalAddress(
        /* [out] */ IInetAddress** address);

    /**
     * Returns the local port this socket is bound to, or -1 if the socket is unbound.
     */
    CARAPI GetLocalPort(
        /* [out] */ Int32* localPort);

    /**
     * Gets an output stream to write data into this socket.
     *
     * @return the byte-oriented output stream.
     * @throws IOException
     *             if an error occurs while creating the output stream or the
     *             socket is in an invalid state.
     */
    CARAPI GetOutputStream(
        /* [out] */ IOutputStream** os);

    /**
     * Gets the port number of the target host this socket is connected to.
     *
     * @return the port number of the connected target host or {@code 0} if this
     *         socket is not yet connected.
     */
    CARAPI GetPort(
        /* [out] */ Int32* port);

    /**
     * Gets the value of the socket option {@link SocketOptions#SO_LINGER}.
     *
     * @return the current value of the option {@code SocketOptions.SO_LINGER}
     *         or {@code -1} if this option is disabled.
     * @throws SocketException
     *             if an error occurs while reading the socket option.
     * @see SocketOptions#SO_LINGER
     */
    CARAPI GetSoLinger(
        /* [out] */ Int32* value);

    /**
     * Gets the receive buffer size of this socket.
     *
     * @return the current value of the option {@code SocketOptions.SO_RCVBUF}.
     * @throws SocketException
     *             if an error occurs while reading the socket option.
     * @see SocketOptions#SO_RCVBUF
     */
    CARAPI GetReceiveBufferSize(
        /* [out] */ Int32* size);

    /**
     * Gets the send buffer size of this socket.
     *
     * @return the current value of the option {@code SocketOptions.SO_SNDBUF}.
     * @throws SocketException
     *             if an error occurs while reading the socket option.
     * @see SocketOptions#SO_SNDBUF
     */
    CARAPI GetSendBufferSize(
        /* [out] */ Int32* size);

    /**
     * Gets the socket {@link SocketOptions#SO_TIMEOUT receive timeout}.
     *
     * @throws SocketException
     *             if an error occurs while reading the socket option.
     */
    CARAPI GetSoTimeout(
        /* [out] */ Int32* timeout);

    /**
     * Gets the setting of the socket option {@code SocketOptions.TCP_NODELAY}.
     *
     * @return {@code true} if the {@code SocketOptions.TCP_NODELAY} is enabled,
     *         {@code false} otherwise.
     * @throws SocketException
     *             if an error occurs while reading the socket option.
     * @see SocketOptions#TCP_NODELAY
     */
    CARAPI GetTcpNoDelay(
        /* [out] */ Boolean* noDelay);

    /**
     * Sets the state of the {@code SocketOptions.SO_KEEPALIVE} for this socket.
     *
     * @param keepAlive
     *            the state whether this option is enabled or not.
     * @throws SocketException
     *             if an error occurs while setting the option.
     * @see SocketOptions#SO_KEEPALIVE
     */
    CARAPI SetKeepAlive(
        /* [in] */ Boolean keepAlive);

    /**
     * Sets the internal factory for creating socket implementations. This may
     * only be executed once during the lifetime of the application.
     *
     * @param fac
     *            the socket implementation factory to be set.
     * @throws IOException
     *             if the factory has been already set.
     */
    static CARAPI SetSocketImplFactory(
        /* [in] */ ISocketImplFactory* fac);

    /**
     * Sets the send buffer size of this socket.
     *
     * @param size
     *            the buffer size in bytes. This value must be a positive number
     *            greater than {@code 0}.
     * @throws SocketException
     *             if an error occurs while setting the size or the given value
     *             is an invalid size.
     * @see SocketOptions#SO_SNDBUF
     */
    CARAPI SetSendBufferSize(
        /* [in] */ Int32 size);

    /**
     * Sets the receive buffer size of this socket.
     *
     * @param size
     *            the buffer size in bytes. This value must be a positive number
     *            greater than {@code 0}.
     * @throws SocketException
     *             if an error occurs while setting the size or the given value
     *             is an invalid size.
     * @see SocketOptions#SO_RCVBUF
     */
    CARAPI SetReceiveBufferSize(
        /* [in] */ Int32 size);

    /**
     * Sets the {@link SocketOptions#SO_LINGER} timeout in seconds.
     *
     * @param on
     *            the state whether this option is enabled or not.
     * @param timeout
     *            the linger timeout value in seconds.
     * @throws SocketException
     *             if an error occurs while setting the option.
     * @see SocketOptions#SO_LINGER
     */
    CARAPI SetSoLinger(
        /* [in] */ Boolean on,
        /* [in] */ Int32 timeout);

    /**
     * Sets the {@link SocketOptions#SO_TIMEOUT read timeout} in milliseconds for this socket.
     * This receive timeout defines the period the socket will block waiting to
     * receive data before throwing an {@code InterruptedIOException}. The value
     * {@code 0} (default) is used to set an infinite timeout. To have effect
     * this option must be set before the blocking method was called.
     *
     * @param timeout the timeout in milliseconds or 0 for no timeout.
     * @throws SocketException
     *             if an error occurs while setting the option.
     */
    CARAPI SetSoTimeout(
        /* [in] */ Int32 timeout);

    /**
     * Sets the state of the {@code SocketOptions.TCP_NODELAY} for this socket.
     *
     * @param on
     *            the state whether this option is enabled or not.
     * @throws SocketException
     *             if an error occurs while setting the option.
     * @see SocketOptions#TCP_NODELAY
     */
    CARAPI SetTcpNoDelay(
        /* [in] */ Boolean on);

    /**
     * Closes the input stream of this socket. Any further data sent to this
     * socket will be discarded. Reading from this socket after this method has
     * been called will return the value {@code EOF}.
     *
     * @throws IOException
     *             if an error occurs while closing the socket input stream.
     * @throws SocketException
     *             if the input stream is already closed.
     */
    CARAPI ShutdownInput();

    /**
     * Closes the output stream of this socket. All buffered data will be sent
     * followed by the termination sequence. Writing to the closed output stream
     * will cause an {@code IOException}.
     *
     * @throws IOException
     *             if an error occurs while closing the socket output stream.
     * @throws SocketException
     *             if the output stream is already closed.
     */
    CARAPI ShutdownOutput();

    /**
     * Gets the local address and port of this socket as a SocketAddress or
     * {@code null} if the socket is unbound. This is useful on multihomed
     * hosts.
     *
     * @return the bound local socket address and port.
     */
    CARAPI GetLocalSocketAddress(
        /* [out] */ ISocketAddress** address);

    /**
     * Gets the remote address and port of this socket as a {@code
     * SocketAddress} or {@code null} if the socket is not connected.
     *
     * @return the remote socket address and port.
     */
    CARAPI GetRemoteSocketAddress(
        /* [out] */ ISocketAddress** address);

    /**
     * Returns whether this socket is bound to a local address and port.
     *
     * @return {@code true} if the socket is bound to a local address, {@code
     *         false} otherwise.
     */
    CARAPI IsBound(
        /* [out] */ Boolean* isBound);

    /**
     * Returns whether this socket is connected to a remote host.
     *
     * @return {@code true} if the socket is connected, {@code false} otherwise.
     */
    CARAPI IsConnected(
        /* [out] */ Boolean* isConnected);

    /**
     * Returns whether this socket is closed.
     *
     * @return {@code true} if the socket is closed, {@code false} otherwise.
     */
    CARAPI IsClosed(
        /* [out] */ Boolean* isClosed);

    /**
     * Binds this socket to the given local host address and port specified by
     * the SocketAddress {@code localAddr}. If {@code localAddr} is set to
     * {@code null}, this socket will be bound to an available local address on
     * any free port.
     *
     * @param localAddr
     *            the specific address and port on the local machine to bind to.
     * @throws IllegalArgumentException
     *             if the given SocketAddress is invalid or not supported.
     * @throws IOException
     *             if the socket is already bound or an error occurs while
     *             binding.
     */
    CARAPI Bind(
        /* [in] */ ISocketAddress* localAddr);

    /**
     * Connects this socket to the given remote host address and port specified
     * by the SocketAddress {@code remoteAddr}.
     *
     * @param remoteAddr
     *            the address and port of the remote host to connect to.
     * @throws IllegalArgumentException
     *             if the given SocketAddress is invalid or not supported.
     * @throws IOException
     *             if the socket is already connected or an error occurs while
     *             connecting.
     */
    CARAPI Connect(
        /* [in] */ ISocketAddress* remoteAddr);

    /**
     * Connects this socket to the given remote host address and port specified
     * by the SocketAddress {@code remoteAddr} with the specified timeout. The
     * connecting method will block until the connection is established or an
     * error occurred.
     *
     * @param remoteAddr
     *            the address and port of the remote host to connect to.
     * @param timeout
     *            the timeout value in milliseconds or {@code 0} for an infinite
     *            timeout.
     * @throws IllegalArgumentException
     *             if the given SocketAddress is invalid or not supported or the
     *             timeout value is negative.
     * @throws IOException
     *             if the socket is already connected or an error occurs while
     *             connecting.
     */
    CARAPI ConnectEx(
        /* [in] */ ISocketAddress* remoteAddr,
        /* [in] */ Int32 timeout);

    /**
     * Returns whether the incoming channel of the socket has already been
     * closed.
     *
     * @return {@code true} if reading from this socket is not possible anymore,
     *         {@code false} otherwise.
     */
    CARAPI IsInputShutdown(
        /* [out] */ Boolean* isInputShutdown);

    /**
     * Returns whether the outgoing channel of the socket has already been
     * closed.
     *
     * @return {@code true} if writing to this socket is not possible anymore,
     *         {@code false} otherwise.
     */
    CARAPI IsOutputShutdown(
        /* [out] */ Boolean* isOutputShutdown);

    /**
     * Sets the state of the {@code SocketOptions.SO_REUSEADDR} for this socket.
     *
     * @param reuse
     *            the state whether this option is enabled or not.
     * @throws SocketException
     *             if an error occurs while setting the option.
     * @see SocketOptions#SO_REUSEADDR
     */
    CARAPI SetReuseAddress(
        /* [in] */ Boolean reuse);

    /**
     * Gets the setting of the socket option {@code SocketOptions.SO_REUSEADDR}.
     *
     * @return {@code true} if the {@code SocketOptions.SO_REUSEADDR} is
     *         enabled, {@code false} otherwise.
     * @throws SocketException
     *             if an error occurs while reading the socket option.
     * @see SocketOptions#SO_REUSEADDR
     */
    CARAPI GetReuseAddress(
        /* [out] */ Boolean* reuse);

    /**
     * Sets the state of the {@code SocketOptions.SO_OOBINLINE} for this socket.
     * When this option is enabled urgent data can be received in-line with
     * normal data.
     *
     * @param oobinline
     *            whether this option is enabled or not.
     * @throws SocketException
     *             if an error occurs while setting the option.
     * @see SocketOptions#SO_OOBINLINE
     */
    CARAPI SetOOBInline(
        /* [in] */ Boolean oobinline);

    /**
     * Gets the setting of the socket option {@code SocketOptions.SO_OOBINLINE}.
     *
     * @return {@code true} if the {@code SocketOptions.SO_OOBINLINE} is
     *         enabled, {@code false} otherwise.
     * @throws SocketException
     *             if an error occurs while reading the socket option.
     * @see SocketOptions#SO_OOBINLINE
     */
    CARAPI GetOOBInline(
        /* [out] */ Boolean* oobinline);

    /**
     * Sets the {@see SocketOptions#IP_TOS} value for every packet sent by this socket.
     *
     * @throws SocketException
     *             if the socket is closed or the option could not be set.
     */
    CARAPI SetTrafficClass(
        /* [in] */ Int32 value);

    /**
     * Returns this socket's {@see SocketOptions#IP_TOS} setting.
     *
     * @throws SocketException
     *             if the socket is closed or the option is invalid.
     */
    CARAPI GetTrafficClass(
        /* [out] */ Int32* value);

    /**
     * Sends the given single byte data which is represented by the lowest octet
     * of {@code value} as "TCP urgent data".
     *
     * @param value
     *            the byte of urgent data to be sent.
     * @throws IOException
     *             if an error occurs while sending urgent data.
     */
    CARAPI SendUrgentData(
        /* [in] */ Int32 value);

    /**
     * Set the appropriate flags for a socket created by {@code
     * ServerSocket.accept()}.
     *
     * @see ServerSocket#implAccept
     */
    CARAPI_(void) Accepted();

    /**
     * Gets the SocketChannel of this socket, if one is available. The current
     * implementation of this method returns always {@code null}.
     *
     * @return the related SocketChannel or {@code null} if no channel exists.
     */
    CARAPI GetChannel(
        /* [out] */ ISocketChannel** channel);

    /**
     * Sets performance preferences for connectionTime, latency and bandwidth.
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
    CARAPI SetPerformancePreferences(
        /* [in] */ Int32 connectionTime,
        /* [in] */ Int32 latency,
        /* [in] */ Int32 bandwidth);

protected:
    Socket();

    /**
     * Creates a new unconnected socket. When a SocketImplFactory is defined it
     * creates the internal socket implementation, otherwise the default socket
     * implementation will be used for this socket.
     *
     * @see SocketImplFactory
     * @see SocketImpl
     */
    CARAPI Init();

    /**
     * Creates a new unconnected socket using the given proxy type. When a
     * {@code SocketImplFactory} is defined it creates the internal socket
     * implementation, otherwise the default socket implementation will be used
     * for this socket.
     * <p>
     * Example that will create a socket connection through a {@code SOCKS}
     * proxy server: <br>
     * {@code Socket sock = new Socket(new Proxy(Proxy.Type.SOCKS, new
     * InetSocketAddress("test.domain.org", 2130)));}
     *
     * @param proxy
     *            the specified proxy for this socket.
     * @throws IllegalArgumentException
     *             if the argument {@code proxy} is {@code null} or of an
     *             invalid type.
     * @throws SecurityException
     *             if a security manager exists and it denies the permission to
     *             connect to the given proxy.
     * @see SocketImplFactory
     * @see SocketImpl
     */
    CARAPI Init(
        /* [in] */ IProxy* proxy);

    /**
     * Creates a new streaming socket connected to the target host specified by
     * the parameters {@code dstName} and {@code dstPort}. The socket is bound
     * to any available port on the local host.
     * <p><strong>Implementation note:</strong> this implementation tries each
     * IP address for the given hostname until it either connects successfully
     * or it exhausts the set. It will try both IPv4 and IPv6 addresses in the
     * order specified by the system property {@code "java.net.preferIPv6Addresses"}.
     *
     * @param dstName
     *            the target host name or IP address to connect to.
     * @param dstPort
     *            the port on the target host to connect to.
     * @throws UnknownHostException
     *             if the host name could not be resolved into an IP address.
     * @throws IOException
     *             if an error occurs while creating the socket.
     * @throws SecurityException
     *             if a security manager exists and it denies the permission to
     *             connect to the given address and port.
     */
    CARAPI Init(
        /* [in] */ const String& dstName,
        /* [in] */ Int32 dstPort);

    /**
     * Creates a new streaming socket connected to the target host specified by
     * the parameters {@code dstName} and {@code dstPort}. On the local endpoint
     * the socket is bound to the given address {@code localAddress} on port
     * {@code localPort}.
     *
     * If {@code host} is {@code null} a loopback address is used to connect to.
     * <p><strong>Implementation note:</strong> this implementation tries each
     * IP address for the given hostname until it either connects successfully
     * or it exhausts the set. It will try both IPv4 and IPv6 addresses in the
     * order specified by the system property {@code "java.net.preferIPv6Addresses"}.
     *
     * @param dstName
     *            the target host name or IP address to connect to.
     * @param dstPort
     *            the port on the target host to connect to.
     * @param localAddress
     *            the address on the local host to bind to.
     * @param localPort
     *            the port on the local host to bind to.
     * @throws UnknownHostException
     *             if the host name could not be resolved into an IP address.
     * @throws IOException
     *             if an error occurs while creating the socket.
     * @throws SecurityException
     *             if a security manager exists and it denies the permission to
     *             connect to the given address and port.
     */
    CARAPI Init(
        /* [in] */ const String& dstName,
        /* [in] */ Int32 dstPort,
        /* [in] */ IInetAddress* localAddress,
        /* [in] */ Int32 localPort);

    /**
     * Creates a new streaming or datagram socket connected to the target host
     * specified by the parameters {@code hostName} and {@code port}. The socket
     * is bound to any available port on the local host.
     * <p><strong>Implementation note:</strong> this implementation tries each
     * IP address for the given hostname until it either connects successfully
     * or it exhausts the set. It will try both IPv4 and IPv6 addresses in the
     * order specified by the system property {@code "java.net.preferIPv6Addresses"}.
     *
     * @param hostName
     *            the target host name or IP address to connect to.
     * @param port
     *            the port on the target host to connect to.
     * @param streaming
     *            if {@code true} a streaming socket is returned, a datagram
     *            socket otherwise.
     * @throws UnknownHostException
     *             if the host name could not be resolved into an IP address.
     * @throws IOException
     *             if an error occurs while creating the socket.
     * @throws SecurityException
     *             if a security manager exists and it denies the permission to
     *             connect to the given address and port.
     * @deprecated Use {@code Socket(String, int)} instead of this for streaming
     *             sockets or an appropriate constructor of {@code
     *             DatagramSocket} for UDP transport.
     */
    // @Deprecated
    CARAPI Init(
        /* [in] */ const String& hostName,
        /* [in] */ Int32 port,
        /* [in] */ Boolean streaming);

    /**
     * Creates a new streaming socket connected to the target host specified by
     * the parameters {@code dstAddress} and {@code dstPort}. The socket is
     * bound to any available port on the local host.
     *
     * @param dstAddress
     *            the target host address to connect to.
     * @param dstPort
     *            the port on the target host to connect to.
     * @throws IOException
     *             if an error occurs while creating the socket.
     * @throws SecurityException
     *             if a security manager exists and it denies the permission to
     *             connect to the given address and port.
     */
    CARAPI Init(
        /* [in] */ IInetAddress* dstAddress,
        /* [in] */ Int32 dstPort);

    /**
     * Creates a new streaming socket connected to the target host specified by
     * the parameters {@code dstAddress} and {@code dstPort}. On the local
     * endpoint the socket is bound to the given address {@code localAddress} on
     * port {@code localPort}.
     *
     * @param dstAddress
     *            the target host address to connect to.
     * @param dstPort
     *            the port on the target host to connect to.
     * @param localAddress
     *            the address on the local host to bind to.
     * @param localPort
     *            the port on the local host to bind to.
     * @throws IOException
     *             if an error occurs while creating the socket.
     * @throws SecurityException
     *             if a security manager exists and it denies the permission to
     *             connect to the given address and port.
     */
    CARAPI Init(
        /* [in] */ IInetAddress* dstAddress,
        /* [in] */ Int32 dstPort,
        /* [in] */ IInetAddress* localAddress,
        /* [in] */ Int32 localPort);

    /**
     * Creates a new streaming or datagram socket connected to the target host
     * specified by the parameters {@code addr} and {@code port}. The socket is
     * bound to any available port on the local host.
     *
     * @param addr
     *            the Internet address to connect to.
     * @param port
     *            the port on the target host to connect to.
     * @param streaming
     *            if {@code true} a streaming socket is returned, a datagram
     *            socket otherwise.
     * @throws IOException
     *             if an error occurs while creating the socket.
     * @throws SecurityException
     *             if a security manager exists and it denies the permission to
     *             connect to the given address and port.
     * @deprecated Use {@code Socket(InetAddress, int)} instead of this for
     *             streaming sockets or an appropriate constructor of {@code
     *             DatagramSocket} for UDP transport.
     */
    // @Deprecated
    CARAPI Init(
        /* [in] */ IInetAddress* addr,
        /* [in] */ Int32 port,
        /* [in] */ Boolean streaming);

    virtual CARAPI_(Mutex*) GetSelfLock() = 0;

private:
    // BEGIN android-added
    /**
     * Tries to connect a socket to all IP addresses of the given hostname.
     *
     * @param dstName
     *            the target host name or IP address to connect to.
     * @param dstPort
     *            the port on the target host to connect to.
     * @param localAddress
     *            the address on the local host to bind to.
     * @param localPort
     *            the port on the local host to bind to.
     * @param streaming
     *            if {@code true} a streaming socket is returned, a datagram
     *            socket otherwise.
     * @throws UnknownHostException
     *             if the host name could not be resolved into an IP address.
     * @throws IOException
     *             if an error occurs while creating the socket.
     * @throws SecurityException
     *             if a security manager exists and it denies the permission to
     *             connect to the given address and port.
     */
    CARAPI TryAllAddresses(
        /* [in] */ const String& dstName,
        /* [in] */ Int32 dstPort,
        /* [in] */ IInetAddress* localAddress,
        /* [in] */ Int32 localPort,
        /* [in] */ Boolean streaming);

    /**
     * Checks whether the connection destination satisfies the security policy
     * and the validity of the port range.
     *
     * @param destAddr
     *            the destination host address.
     * @param dstPort
     *            the port on the destination host.
     */
    CARAPI CheckDestination(
        /* [in] */ IInetAddress* destAddr,
        /* [in] */ Int32 dstPort);

    /**
     * Checks whether the connection destination satisfies the security policy.
     *
     * @param hostname
     *            the destination hostname.
     * @param dstPort
     *            the port on the destination host.
     */
    CARAPI CheckConnectPermission(
        /* [in] */ const String& hostname,
        /* [in] */ Int32 dstPort);

    /**
     * Creates a stream socket, binds it to the nominated local address/port,
     * then connects it to the nominated destination address/port.
     *
     * @param dstAddress
     *            the destination host address.
     * @param dstPort
     *            the port on the destination host.
     * @param localAddress
     *            the address on the local machine to bind.
     * @param localPort
     *            the port on the local machine to bind.
     * @throws IOException
     *             thrown if an error occurs during the bind or connect
     *             operations.
     */
    CARAPI StartupSocket(
        /* [in] */ IInetAddress* dstAddress,
        /* [in] */ Int32 dstPort,
        /* [in] */ IInetAddress* localAddress,
        /* [in] */ Int32 localPort,
        /* [in] */ Boolean streaming);

    CARAPI_(Boolean) UsingSocks();

    /**
     * Checks whether the socket is closed, and throws an exception. Otherwise
     * creates the underlying SocketImpl.
     *
     * @throws SocketException
     *             if the socket is closed.
     */
    CARAPI CheckOpenAndCreate(
        /* [in] */ Boolean create);

    CARAPI_(void) CacheLocalAddress();

public:
    AutoPtr<ISocketImpl> mImpl;

private:
    static AutoPtr<ISocketImplFactory> sFactory;
    static Mutex sLock;

    AutoPtr<IProxy> mProxy;

    Boolean mIsCreated;
    Boolean mIsBound;
    Boolean mIsConnected;
    Boolean mIsClosed;
    Boolean mIsInputShutdown;
    Boolean mIsOutputShutdown;

    AutoPtr<IInetAddress> mLocalAddress;
    Mutex mConnectLock;
};


#endif //__SOCKET_H__
