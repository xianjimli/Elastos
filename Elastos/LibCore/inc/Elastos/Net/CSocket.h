
#ifndef __CSOCKET_H__
#define __CSOCKET_H__

#include "_CSocket.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

CarClass(CSocket)
{
public:
    CSocket();

    CARAPI Close();

    CARAPI GetInetAddress(
        /* [out] */ IInetAddress** address);

    CARAPI GetInputStream(
        /* [out] */ IInputStream** is);

    CARAPI GetKeepAlive(
        /* [out] */ Boolean* keepAlive);

    CARAPI GetLocalAddress(
        /* [out] */ IInetAddress** address);

    CARAPI GetLocalPort(
        /* [out] */ Int32* localPort);

    CARAPI GetOutputStream(
        /* [out] */ IOutputStream** os);

    CARAPI GetPort(
        /* [out] */ Int32* port);

    CARAPI GetSoLinger(
        /* [out] */ Int32* value);

    CARAPI GetReceiveBufferSize(
        /* [out] */ Int32* size);

    CARAPI GetSendBufferSize(
        /* [out] */ Int32* size);

    CARAPI GetSoTimeout(
        /* [out] */ Int32* timeout);

    CARAPI GetTcpNoDelay(
        /* [out] */ Boolean* noDelay);

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

    CARAPI SetSendBufferSize(
        /* [in] */ Int32 size);

    CARAPI SetReceiveBufferSize(
        /* [in] */ Int32 size);

    CARAPI SetSoLinger(
        /* [in] */ Boolean on,
        /* [in] */ Int32 timeout);

    CARAPI SetSoTimeout(
        /* [in] */ Int32 timeout);

    CARAPI SetTcpNoDelay(
        /* [in] */ Boolean on);

    CARAPI ShutdownInput();

    CARAPI ShutdownOutput();

    CARAPI GetLocalSocketAddress(
        /* [out] */ ISocketAddress** address);

    CARAPI GetRemoteSocketAddress(
        /* [out] */ ISocketAddress** address);

    CARAPI IsBound(
        /* [out] */ Boolean* isBound);

    CARAPI IsConnected(
        /* [out] */ Boolean* isConnected);

    CARAPI IsClosed(
        /* [out] */ Boolean* isClosed);

    CARAPI Bind(
        /* [in] */ ISocketAddress* localAddr);

    CARAPI Connect(
        /* [in] */ ISocketAddress* remoteAddr);

    CARAPI ConnectEx(
        /* [in] */ ISocketAddress* remoteAddr,
        /* [in] */ Int32 timeout);

    CARAPI IsInputShutdown(
        /* [out] */ Boolean* isInputShutdown);

    CARAPI IsOutputShutdown(
        /* [out] */ Boolean* isOutputShutdown);

    CARAPI SetReuseAddress(
        /* [in] */ Boolean reuse);

    CARAPI GetReuseAddress(
        /* [out] */ Boolean* reuse);

    CARAPI SetOOBInline(
        /* [in] */ Boolean oobinline);

    CARAPI GetOOBInline(
        /* [out] */ Boolean* oobinline);

    CARAPI SetTrafficClass(
        /* [in] */ Int32 value);

    CARAPI GetTrafficClass(
        /* [out] */ Int32* value);

    CARAPI SendUrgentData(
        /* [in] */ Int32 value);

    /**
     * Set the appropriate flags for a socket created by {@code
     * ServerSocket.accept()}.
     *
     * @see ServerSocket#implAccept
     */
    CARAPI_(void) Accepted();

    CARAPI GetChannel(
        /* [out] */ ISocketChannel** channel);

    CARAPI SetPerformancePreferences(
        /* [in] */ Int32 connectionTime,
        /* [in] */ Int32 latency,
        /* [in] */ Int32 bandwidth);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IProxy* proxy);

    CARAPI constructor(
        /* [in] */ const String& dstName,
        /* [in] */ Int32 dstPort);

    CARAPI constructor(
        /* [in] */ const String& dstName,
        /* [in] */ Int32 dstPort,
        /* [in] */ IInetAddress* localAddress,
        /* [in] */ Int32 localPort);

    CARAPI constructor(
        /* [in] */ const String& hostName,
        /* [in] */ Int32 port,
        /* [in] */ Boolean streaming);

    CARAPI constructor(
        /* [in] */ IInetAddress* dstAddress,
        /* [in] */ Int32 dstPort);

    CARAPI constructor(
        /* [in] */ IInetAddress* dstAddress,
        /* [in] */ Int32 dstPort,
        /* [in] */ IInetAddress* localAddress,
        /* [in] */ Int32 localPort);

    CARAPI constructor(
        /* [in] */ IInetAddress* addr,
        /* [in] */ Int32 port,
        /* [in] */ Boolean streaming);

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
    CARAPI_(void) TryAllAddresses(
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
    CARAPI_(void) CheckConnectPermission(
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
    static AutoPtr<ISocketImplFactory> mFactory;

    AutoPtr<IProxy> mProxy;

    Boolean mIsCreated;
    Boolean mIsBound;
    Boolean mIsConnected;
    Boolean mIsClosed;
    Boolean mIsInputShutdown;
    Boolean mIsOutputShutdown;

    AutoPtr<IInetAddress> mLocalAddress;
    Mutex* mLock;
};

#endif //__CSOCKET_H__
