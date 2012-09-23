
#include "cmdef.h"
#include "AsynchronousSocketCloseMonitor.h"
#include "OSNetworkSystem.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

/*
 * TODO: The multicast code is highly platform-dependent, and for now
 * we just punt on anything but Linux.
 */
#ifdef __linux__
#define ENABLE_MULTICAST
#endif

#define SOCKOPT_IP_MULTICAST_IF 16
#define SOCKOPT_IP_MULTICAST_IF2 31
#define SOCKOPT_IP_MULTICAST_LOOP 18
#define SOCKOPT_IP_TOS 3
#define SOCKOPT_MCAST_JOIN_GROUP 19
#define SOCKOPT_MCAST_LEAVE_GROUP 20
#define SOCKOPT_MULTICAST_TTL 17
#define SOCKOPT_SO_BROADCAST 32
#define SOCKOPT_SO_KEEPALIVE 8
#define SOCKOPT_SO_LINGER 128
#define SOCKOPT_SO_OOBINLINE  4099
#define SOCKOPT_SO_RCVBUF 4098
#define SOCKOPT_SO_TIMEOUT  4102
#define SOCKOPT_SO_REUSEADDR 4
#define SOCKOPT_SO_SNDBUF 4097
#define SOCKOPT_TCP_NODELAY 1

/* constants for OSNetworkSystem_selectImpl */
#define SOCKET_OP_NONE 0
#define SOCKET_OP_READ 1
#define SOCKET_OP_WRITE 2

/**
 * Used to retry syscalls that can return EINTR. This differs from TEMP_FAILURE_RETRY in that
 * it also considers the case where the reason for failure is that another thread called
 * Socket.close.
 */
#define NET_FAILURE_RETRY(fd, exp) ({               \
    typeof (exp) _rc;                               \
    do {                                            \
        _rc = (exp);                                \
        if (_rc == -1) {                            \
            if (fd == -1 || errno != EINTR) {       \
                break;                              \
            }                                       \
        }                                           \
    } while (_rc == -1);                            \
    _rc; })


ECode ByteArrayToSocketAddress(const ArrayOf<Byte>& byteArray, Int32 port, sockaddr_storage* ss)
{
    // Convert the IP address bytes to the proper IP address type.
    size_t addressLength = byteArray.GetLength();
    memset(ss, 0, sizeof(*ss));
    if (addressLength == 4) {
        // IPv4 address.
        sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(ss);
        sin->sin_family = AF_INET;
        sin->sin_port = htons(port);
        Byte* dst = reinterpret_cast<Byte*>(&sin->sin_addr.s_addr);
        memcpy(dst, byteArray.GetPayload(), 4 * sizeof(Byte));
    }
    else if (addressLength == 16) {
        // IPv6 address.
        sockaddr_in6* sin6 = reinterpret_cast<sockaddr_in6*>(ss);
        sin6->sin6_family = AF_INET6;
        sin6->sin6_port = htons(port);
        Byte* dst = reinterpret_cast<Byte*>(&sin6->sin6_addr.s6_addr);
        memcpy(dst, byteArray.GetPayload(), 16 * sizeof(Byte));
    }
    else {
        // We can't throw SocketException. We aren't meant to see bad addresses, so seeing one
        // really does imply an internal error.
        // TODO: fix the code (native and Java) so we don't paint ourselves into this corner.
//        char buf[64];
//        snprintf(buf, sizeof(buf), "byteArrayToSocketAddress bad array length (%i)", addressLength);
//        jniThrowException(env, "java/lang/IllegalArgumentException", buf);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode SocketAddressToByteArray(sockaddr_storage* ss, ArrayOf<Byte>* ipAddress)
{
    void* rawAddress;
    size_t addressLength;
    if (ss->ss_family == AF_INET) {
        sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(ss);
        rawAddress = &sin->sin_addr.s_addr;
        addressLength = 4;
    }
    else if (ss->ss_family == AF_INET6) {
        sockaddr_in6* sin6 = reinterpret_cast<sockaddr_in6*>(ss);
        rawAddress = &sin6->sin6_addr.s6_addr;
        addressLength = 16;
    }
    else {
        // We can't throw SocketException. We aren't meant to see bad addresses, so seeing one
        // really does imply an internal error.
        // TODO: fix the code (native and Java) so we don't paint ourselves into this corner.
//        char buf[64];
//        snprintf(buf, sizeof(buf), "socketAddressToByteArray bad ss_family (%i)", ss->ss_family);
//        jniThrowException(env, "java/lang/IllegalArgumentException", buf);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (ipAddress->GetLength() < addressLength) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    memcpy(ipAddress->GetPayload(), rawAddress, addressLength);
    return NOERROR;
}

Boolean SetBlocking(Int32 fd, Boolean blocking)
{
    Int32 flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        return FALSE;
    }

    if (!blocking) {
        flags |= O_NONBLOCK;
    }
    else {
        flags &= ~O_NONBLOCK;
    }

    Int32 rc = fcntl(fd, F_SETFL, flags);
    return (rc != -1);
}

/**
 * Returns the port number in a sockaddr_storage structure.
 *
 * @param address the sockaddr_storage structure to get the port from
 *
 * @return the port number, or -1 if the address family is unknown.
 */
static Int32 GetSocketAddressPort(sockaddr_storage* ss)
{
    switch (ss->ss_family) {
    case AF_INET:
        return ntohs(reinterpret_cast<sockaddr_in*>(ss)->sin_port);
    case AF_INET6:
        return ntohs(reinterpret_cast<sockaddr_in6*>(ss)->sin6_port);
    default:
        return -1;
    }
}

/**
 * Obtain the socket address family from an existing socket.
 *
 * @param socket the file descriptor of the socket to examine
 * @return an integer, the address family of the socket
 */
static Int32 GetSocketAddressFamily(Int32 socket)
{
    sockaddr_storage ss;
    socklen_t namelen = sizeof(ss);
    Int32 ret = getsockname(socket, reinterpret_cast<sockaddr*>(&ss), &namelen);
    if (ret != 0) {
        return AF_UNSPEC;
    }
    else {
        return ss.ss_family;
    }
}

// Handles translating between IPv4 and IPv6 addresses so -- where possible --
// we can use either class of address with either an IPv4 or IPv6 socket.
class CompatibleSocketAddress
{
public:
    // Constructs an address corresponding to 'ss' that's compatible with 'fd'.
    CompatibleSocketAddress(Int32 fd, const sockaddr_storage& ss, Boolean mapUnspecified)
    {
        const Int32 desiredFamily = GetSocketAddressFamily(fd);
        if (ss.ss_family == AF_INET6) {
            if (desiredFamily == AF_INET6) {
                // Nothing to do.
                mCompatibleAddress = reinterpret_cast<const sockaddr*>(&ss);
            }
            else {
                sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(&mTmp);
                const sockaddr_in6* sin6 = reinterpret_cast<const sockaddr_in6*>(&ss);
                memset(sin, 0, sizeof(*sin));
                sin->sin_family = AF_INET;
                sin->sin_port = sin6->sin6_port;
                if (IN6_IS_ADDR_V4COMPAT(&sin6->sin6_addr)) {
                    // We have an IPv6-mapped IPv4 address, but need plain old IPv4.
                    // Unmap the mapped address in ss into an IPv6 address in mTmp.
                    memcpy(&sin->sin_addr.s_addr, &sin6->sin6_addr.s6_addr[12], 4);
                    mCompatibleAddress = reinterpret_cast<const sockaddr*>(&mTmp);
                }
                else if (IN6_IS_ADDR_LOOPBACK(&sin6->sin6_addr)) {
                    // Translate the IPv6 loopback address to the IPv4 one.
                    sin->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
                    mCompatibleAddress = reinterpret_cast<const sockaddr*>(&mTmp);
                }
                else {
                    // We can't help you. We return what you gave us, and assume you'll
                    // get a sensible error when you use the address.
                    mCompatibleAddress = reinterpret_cast<const sockaddr*>(&ss);
                }
            }
        }
        else /* ss.ss_family == AF_INET */ {
            if (desiredFamily == AF_INET) {
                // Nothing to do.
                mCompatibleAddress = reinterpret_cast<const sockaddr*>(&ss);
            }
            else {
                // We have IPv4 and need IPv6.
                // Map the IPv4 address in ss into an IPv6 address in mTmp.
                const sockaddr_in* sin = reinterpret_cast<const sockaddr_in*>(&ss);
                sockaddr_in6* sin6 = reinterpret_cast<sockaddr_in6*>(&mTmp);
                memset(sin6, 0, sizeof(*sin6));
                sin6->sin6_family = AF_INET6;
                sin6->sin6_port = sin->sin_port;
                // TODO: mapUnspecified was introduced because kernels < 2.6.31 don't allow
                // you to bind to ::ffff:0.0.0.0. When we move to something >= 2.6.31, we
                // should make the code behave as if mapUnspecified were always true, and
                // remove the parameter.
                if (sin->sin_addr.s_addr != 0 || mapUnspecified) {
                    memset(&(sin6->sin6_addr.s6_addr[10]), 0xff, 2);
                }
                memcpy(&sin6->sin6_addr.s6_addr[12], &sin->sin_addr.s_addr, 4);
                mCompatibleAddress = reinterpret_cast<const sockaddr*>(&mTmp);
            }
        }
    }
    // Returns a pointer to an address compatible with the socket.
    const sockaddr* Get() const {
        return mCompatibleAddress;
    }

private:
    const sockaddr* mCompatibleAddress;
    sockaddr_storage mTmp;
};

/**
 * Converts an InetAddress object and port number to a native address structure.
 */
static ECode IpAddressToSocketAddress(const ArrayOf<Byte>& ipAddress,
        Int32 port, sockaddr_storage* ss)
{
    // Get the byte array that stores the IP address bytes in the InetAddress.
    return ByteArrayToSocketAddress(ipAddress, port, ss);
}

// Converts a number of milliseconds to a timeval.
static timeval ToTimeval(Int64 ms)
{
    timeval tv;
    tv.tv_sec = ms / 1000;
    tv.tv_usec = (ms - tv.tv_sec*1000) * 1000;
    return tv;
}

// Converts a timeval to a number of milliseconds.
static Int64 ToMs(const timeval& tv)
{
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

/**
 * Query OS for timestamp.
 * Retrieve the current value of system clock and convert to milliseconds.
 *
 * @param[in] portLibrary The port library.
 *
 * @return 0 on failure, time value in milliseconds on success.
 * @deprecated Use @ref time_hires_clock and @ref time_hires_delta
 *
 * technically, this should return uint64_t since both timeval.tv_sec and
 * timeval.tv_usec are long
 */
static Int32 time_msec_clock()
{
    timeval tp;
    struct timezone tzp;
    gettimeofday(&tp, &tzp);
    return ToMs(tp);
}

/**
 * Establish a connection to a peer with a timeout.  The member functions are called
 * repeatedly in order to carry out the connect and to allow other tasks to
 * proceed on certain platforms. The caller must first call ConnectHelper::start.
 * if the result is -EINPROGRESS it will then
 * call ConnectHelper::isConnected until either another error or 0 is returned to
 * indicate the connect is complete.  Each time the function should sleep for no
 * more than 'timeout' milliseconds.  If the connect succeeds or an error occurs,
 * the caller must always end the process by calling ConnectHelper::done.
 *
 * Member functions return 0 if no errors occur, otherwise -errno. TODO: use +errno.
 */
class ConnectHelper
{
public:
    Int32 Start(Int32 fd, const ArrayOf<Byte>& ipAddr, Int32 port)
    {
        sockaddr_storage ss;
        if (FAILED(IpAddressToSocketAddress(ipAddr, port, &ss))) {
            return -EINVAL; // Bogus, but clearly a failure, and we've already thrown.
        }

        // Set the socket to non-blocking and initiate a connection attempt...
        const CompatibleSocketAddress compatibleAddress(fd, ss, TRUE);
        if (!SetBlocking(fd, FALSE) ||
                connect(fd, compatibleAddress.Get(), sizeof(sockaddr_storage)) == -1) {
            if (fd == -1) {
                return -EINVAL; // Bogus, but clearly a failure, and we've already thrown.
            }
            if (errno != EINPROGRESS) {
                DidFail(fd, -errno);
            }
            return -errno;
        }
        // We connected straight away!
        DidConnect(fd);
        return 0;
    }

    // Returns 0 if we're connected; -EINPROGRESS if we're still hopeful, -errno if we've failed.
    // 'timeout' the timeout in milliseconds. If timeout is negative, perform a blocking operation.
    Int32 IsConnected(Int32 fd, Int32 timeout)
    {
        timeval passedTimeout(ToTimeval(timeout));

        // Initialize the fd sets for the select.
        fd_set readSet;
        fd_set writeSet;
        FD_ZERO(&readSet);
        FD_ZERO(&writeSet);
        FD_SET(fd, &readSet);
        FD_SET(fd, &writeSet);

        Int32 nfds = fd + 1;
        timeval* tp = timeout >= 0 ? &passedTimeout : NULL;
        Int32 rc = select(nfds, &readSet, &writeSet, NULL, tp);
        if (rc == -1) {
            if (errno == EINTR) {
                // We can't trivially retry a select with TEMP_FAILURE_RETRY, so punt and ask the
                // caller to try again.
                return -EINPROGRESS;
            }
            return -errno;
        }

        // If the fd is just in the write set, we're connected.
        if (FD_ISSET(fd, &writeSet) && !FD_ISSET(fd, &readSet)) {
            return 0;
        }

        // If the fd is in both the read and write set, there was an error.
        if (FD_ISSET(fd, &readSet) || FD_ISSET(fd, &writeSet)) {
            // Get the pending error.
            Int32 error = 0;
            socklen_t errorLen = sizeof(error);
            if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &errorLen) == -1) {
                return -errno; // Couldn't get the real error, so report why not.
            }
            return -error;
        }

        // Timeout expired.
        return -EINPROGRESS;
    }

    void DidConnect(Int32 fd)
    {
        if (fd != -1) {
            SetBlocking(fd, TRUE);
        }
    }

    ECode DidFail(Int32 fd, Int32 result)
    {
        if (fd != -1) {
            SetBlocking(fd, TRUE);
        }

        if (result == -ECONNRESET || result == -ECONNREFUSED || result == -EADDRNOTAVAIL ||
                result == -EADDRINUSE || result == -ENETUNREACH) {
//            jniThrowConnectException(mEnv, -result);
            return E_PLATFORM_CONNECT_EXCEPTION;
        }
        else if (result == -EACCES) {
//            jniThrowSecurityException(mEnv, -result);
            return E_PLATFORM_SECURITY_EXCEPTION;
        }
        else if (result == -ETIMEDOUT) {
//            jniThrowSocketTimeoutException(mEnv, -result);
            return E_PLATFORM_SOCKET_TIMEOUT_EXCEPTION;
        }
        else {
//            jniThrowSocketException(mEnv, -result);
            return E_PLATFORM_SOCKET_EXCEPTION;
        }
    }
};

#ifdef ENABLE_MULTICAST
static ECode McastJoinLeaveGroup(Int32 fd, Int32 optVal, Boolean join)
{

    const ArrayOf<Byte>* gr_group = ((GroupRequest*)optVal)->mGroup;
    Int32 gr_interface = ((GroupRequest*)optVal)->mInterface;

    group_req groupRequest;

    // Get the IPv4 or IPv6 multicast address to join or leave.
    FAIL_RETURN(IpAddressToSocketAddress(*gr_group, 0, &groupRequest.gr_group));

    // Get the interface index to use (or 0 for "whatever").
    groupRequest.gr_interface = gr_interface;

    Int32 level = groupRequest.gr_group.ss_family == AF_INET ? IPPROTO_IP : IPPROTO_IPV6;
    Int32 option = join ? MCAST_JOIN_GROUP : MCAST_LEAVE_GROUP;
    Int32 rc = setsockopt(fd, level, option, &groupRequest, sizeof(groupRequest));
    if (rc == -1) {
//        jniThrowSocketException(env, errno);
        return E_PLATFORM_SOCKET_EXCEPTION;
    }
    return NOERROR;
}
#endif // def ENABLE_MULTICAST

const AutoPtr<OSNetworkSystem> OSNetworkSystem::sSingleton = new OSNetworkSystem();

AutoPtr<INetworkSystem> OSNetworkSystem::GetOSNetworkSystem()
{
    AutoPtr<INetworkSystem> ns = (INetworkSystem*)sSingleton.Get();
    return ns;
}

PInterface OSNetworkSystem::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(INetworkSystem*)this;
    }
    else if (riid == EIID_INetworkSystem) {
        return (INetworkSystem*)this;
    }
    return NULL;
}

UInt32 OSNetworkSystem::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 OSNetworkSystem::Release()
{
    return ElRefBase::Release();
}

ECode OSNetworkSystem::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(INetworkSystem*)this) {
        *pIID = EIID_INetworkSystem;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode OSNetworkSystem::Accept(
    /* [in] */ Int32 serverFd,
    /* [out] */ ArrayOf<Byte>* ipAddress,
    /* [out] */ Int32* port,
    /* [out] */ Int32* localPort,
    /* [out] */ Int32* clientFd)
{
    VALIDATE_NOT_NULL(ipAddress);
    VALIDATE_NOT_NULL(port);
    VALIDATE_NOT_NULL(localPort);
    VALIDATE_NOT_NULL(clientFd);

    if (serverFd == -1) {
        return NOERROR;
    }

    sockaddr_storage ss;
    socklen_t addrLen = sizeof(ss);
    sockaddr* sa = reinterpret_cast<sockaddr*>(&ss);

    {
        AsynchronousSocketCloseMonitor monitor(serverFd);
        *clientFd = NET_FAILURE_RETRY(serverFd, accept(serverFd, sa, &addrLen));
    }
    if (*clientFd == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
//            jniThrowSocketTimeoutException(env, errno);
            return E_PLATFORM_SOCKET_TIMEOUT_EXCEPTION;
        }
        else {
//            jniThrowSocketException(env, errno);
            return E_PLATFORM_SOCKET_EXCEPTION;
        }
    }

    // Reset the inherited read timeout to the Java-specified default of 0.
    timeval timeout(ToTimeval(0));
    Int32 rc = setsockopt(*clientFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    if (rc == -1) {
//        LOGE("couldn't reset SO_RCVTIMEO on accepted socket fd %i: %s", clientFd, strerror(errno));
//        jniThrowSocketException(env, errno);
        return E_PLATFORM_SOCKET_EXCEPTION;
    }

    /*
     * For network sockets, put the peer address and port in instance variables.
     * We don't bother to do this for UNIX domain sockets, since most peers are
     * anonymous anyway.
     */
    if (ss.ss_family == AF_INET || ss.ss_family == AF_INET6) {
        // Remote address and port.
        ECode ec = SocketAddressToByteArray(&ss, ipAddress);
        if (FAILED(ec)) {
            close(*clientFd);
            return ec;
        }
        *port = GetSocketAddressPort(&ss);

        // Local port.
        memset(&ss, 0, addrLen);
        Int32 rc = getsockname(*clientFd, sa, &addrLen);
        if (rc == -1) {
            close(*clientFd);
            return E_PLATFORM_SOCKET_EXCEPTION;
        }
        *localPort = GetSocketAddressPort(&ss);
    }

    return NOERROR;
}

ECode OSNetworkSystem::Bind(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& ipAddress,
    /* [in] */ Int32 port)
{
    sockaddr_storage socketAddress;
    ECode ec = IpAddressToSocketAddress(ipAddress, port, &socketAddress);
    if (FAILED(ec)) {
        return ec;
    }

    if (fd == -1) {
        return  NOERROR;
    }

    const CompatibleSocketAddress compatibleAddress(fd, socketAddress, FALSE);
    Int32 rc = TEMP_FAILURE_RETRY(bind(fd, compatibleAddress.Get(), sizeof(sockaddr_storage)));
    if (rc == -1) {
//        jniThrowBindException(env, errno);
        return E_PLATFORM_BIND_EXCEPTION;
    }
    return NOERROR;
}

ECode OSNetworkSystem::Read(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Byte>* data,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(data);
    VALIDATE_NOT_NULL(number);

    Int32 address = static_cast<Int32>(reinterpret_cast<uintptr_t>(data->GetPayload() + offset));
    return ReadDirect(fd, address, count, number);
}

ECode OSNetworkSystem::ReadDirect(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 address,
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    if (fd == -1) {
        *number = 0;
        return NOERROR;
    }

    Byte* dst = reinterpret_cast<Byte*>(static_cast<uintptr_t>(address));
    ssize_t bytesReceived;
    {
        AsynchronousSocketCloseMonitor monitor(fd);
        bytesReceived = NET_FAILURE_RETRY(fd, read(fd, dst, count));
    }
    if (bytesReceived == 0) {
        *number = -1;
        return NOERROR;
    }
    else if (bytesReceived == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // We were asked to read a non-blocking socket with no data
            // available, so report "no bytes read".
            *number = 0;
            return NOERROR;
        }
        else {
//            jniThrowSocketException(env, errno);
            *number = 0;
            return E_PLATFORM_SOCKET_EXCEPTION;
        }
    }
    else {
        *number = bytesReceived;
        return NOERROR;
    }
}

ECode OSNetworkSystem::Write(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    Int32 address = static_cast<Int32>(reinterpret_cast<uintptr_t>(data.GetPayload()));
    return WriteDirect(fd, address, offset, count, number);
}

ECode OSNetworkSystem::WriteDirect(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 address,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    if (fd == -1 || count <= 0) {
        *number = 0;
        return NOERROR;
    }

    Byte* src = reinterpret_cast<Byte*>(static_cast<uintptr_t>(address + offset));

    ssize_t bytesSent;
    {
        AsynchronousSocketCloseMonitor monitor(fd);
        bytesSent = NET_FAILURE_RETRY(fd, write(fd, src, count));
    }

    if (bytesSent == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // We were asked to write to a non-blocking socket, but were told
            // it would block, so report "no bytes written".
            *number = 0;
            return NOERROR;
        }
        else {
//            jniThrowSocketException(env, errno);
            *number = 0;
            return E_PLATFORM_SOCKET_EXCEPTION;
        }
    }
    *number = bytesSent;
    return NOERROR;
}

ECode OSNetworkSystem::ConnectNonBlocking(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& ipAddress,
    /* [in] */ Int32 port,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    if (fd == -1) {
        *succeeded = FALSE;
        return NOERROR;
    }

    ConnectHelper context;
    *succeeded = context.Start(fd, ipAddress, port) == 0;
    return NOERROR;
}

ECode OSNetworkSystem::IsConnected(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 timeout,
    /* [out] */ Boolean* isConnected)
{
    VALIDATE_NOT_NULL(isConnected);

    if (fd == -1) {
        *isConnected = FALSE;
        return NOERROR;
    }

    ConnectHelper context;
    Int32 result = context.IsConnected(fd, timeout);
    if (result == 0) {
        context.DidConnect(fd);
        *isConnected = TRUE;
        return NOERROR;
    }
    else if (result == -EINPROGRESS) {
        // Not yet connected, but not yet denied either... Try again later.
        *isConnected = FALSE;
        return NOERROR;
    }
    else {
        ECode ec = context.DidFail(fd, result);
        *isConnected = FALSE;
        return ec;
    }
}

ECode OSNetworkSystem::Send(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Int32 port,
    /* [in] */ ArrayOf<Byte>* ipAddress,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return SendDirect(fd,
            reinterpret_cast<uintptr_t>(data.GetPayload()),
            offset, length, port, ipAddress, number);
}

ECode OSNetworkSystem::SendDirect(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 address,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Int32 port,
    /* [in] */ ArrayOf<Byte>* ipAddress,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    if (fd == -1) {
        *number = -1;
        return NOERROR;
    }

    sockaddr_storage receiver;
    if (ipAddress != NULL) {
        ECode ec = IpAddressToSocketAddress(*ipAddress, port, &receiver);
        if (FAILED(ec)) {
            *number = -1;
            return ec;
        }
    }

    Int32 flags = 0;
    char* buf = reinterpret_cast<char*>(static_cast<uintptr_t>(address + offset));
    sockaddr* to = ipAddress ? reinterpret_cast<sockaddr*>(&receiver) : NULL;
    socklen_t toLength = ipAddress ? sizeof(receiver) : 0;

    ssize_t bytesSent;
    {
        AsynchronousSocketCloseMonitor monitor(fd);
        bytesSent = NET_FAILURE_RETRY(fd, sendto(fd, buf, length, flags, to, toLength));
    }
    if (bytesSent == -1) {
        if (errno == ECONNRESET || errno == ECONNREFUSED) {
            *number = 0;
            return NOERROR;
        }
        else {
//            jniThrowSocketException(env, errno);
            return E_PLATFORM_SOCKET_EXCEPTION;
        }
    }
    *number = bytesSent;
    return NOERROR;
}

ECode OSNetworkSystem::Recv(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Boolean peek,
    /* [in] */ Boolean connected,
    /* [out] */ ArrayOf<Byte>* data,
    /* [out] */ ArrayOf<Byte>* ipAddress,
    /* [out] */ Int32* port,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(data);
    VALIDATE_NOT_NULL(ipAddress);
    VALIDATE_NOT_NULL(port);
    VALIDATE_NOT_NULL(number);

    uintptr_t address = reinterpret_cast<uintptr_t>(data->GetPayload());
    return RecvDirect(fd, address, offset, length, peek,
            connected, ipAddress, port, number);
}

ECode OSNetworkSystem::RecvDirect(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 address,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Boolean peek,
    /* [in] */ Boolean connected,
    /* [out] */ ArrayOf<Byte>* ipAddress,
    /* [out] */ Int32* port,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    if (fd == -1) {
        *number = 0;
        return NOERROR;
    }

    char* buf = reinterpret_cast<char*>(static_cast<uintptr_t>(address + offset));
    const Int32 flags = peek ? MSG_PEEK : 0;
    sockaddr_storage ss;
    memset(&ss, 0, sizeof(ss));
    socklen_t sockAddrLen = sizeof(ss);
    sockaddr* from = connected ? NULL : reinterpret_cast<sockaddr*>(&ss);
    socklen_t* fromLength = connected ? NULL : &sockAddrLen;

    ssize_t bytesReceived;
    {
        AsynchronousSocketCloseMonitor monitor(fd);
        bytesReceived = NET_FAILURE_RETRY(fd, recvfrom(fd, buf, length, flags, from, fromLength));
    }
    if (bytesReceived == -1) {
        if (connected && errno == ECONNREFUSED) {
//            jniThrowException(env, "java/net/PortUnreachableException", "");
            return E_PLATFORM_PORT_UNREACHABLE_EXCEPTION;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
//            jniThrowSocketTimeoutException(env, errno);
            return E_PLATFORM_SOCKET_TIMEOUT_EXCEPTION;
        } else {
//            jniThrowSocketException(env, errno);
            return E_PLATFORM_SOCKET_EXCEPTION;
        }
        *number = 0;
        return NOERROR;
    }

    if (ipAddress != NULL && port != NULL) {
        if (!connected) {
            ECode ec = SocketAddressToByteArray(&ss, ipAddress);
            if (FAILED(ec)) {
                *number = 0;
                return ec;
            }
            *port = GetSocketAddressPort(&ss);
        }
    }
    *number = bytesReceived;
    return NOERROR;
}

ECode OSNetworkSystem::DisconnectDatagram(
    /* [in] */ Int32 fd)
{
    if (fd == -1) {
        return NOERROR;
    }

    // To disconnect a datagram socket, we connect to a bogus address with
    // the family AF_UNSPEC.
    sockaddr_storage ss;
    memset(&ss, 0, sizeof(ss));
    ss.ss_family = AF_UNSPEC;
    const sockaddr* sa = reinterpret_cast<const sockaddr*>(&ss);
    Int32 rc = TEMP_FAILURE_RETRY(connect(fd, sa, sizeof(ss)));
    if (rc == -1) {
//        jniThrowSocketException(env, errno);
        return E_PLATFORM_SOCKET_EXCEPTION;
    }
    return NOERROR;
}

ECode OSNetworkSystem::Socket(
    /* [in] */ Boolean stream,
    /* [out] */ Int32* fd)
{
    VALIDATE_NOT_NULL(fd);

    // Try IPv6 but fall back to IPv4...
    Int32 type = stream ? SOCK_STREAM : SOCK_DGRAM;
    *fd = socket(AF_INET6, type, 0);
    if (*fd == -1 && errno == EAFNOSUPPORT) {
        *fd = socket(AF_INET, type, 0);
    }
    if (*fd == -1) {
//        jniThrowSocketException(env, errno);
        return E_PLATFORM_SOCKET_TIMEOUT_EXCEPTION;
    }

#ifdef __linux__
    // The RFC (http://www.ietf.org/rfc/rfc3493.txt) says that IPV6_MULTICAST_HOPS defaults to 1.
    // The Linux kernel (at least up to 2.6.32) accidentally defaults to 64 (which would be correct
    // for the *unicast* hop limit). See http://www.spinics.net/lists/netdev/msg129022.html.
    // When that bug is fixed, we can remove this code. Until then, we manually set the hop
    // limit on IPv6 datagram sockets. (IPv4 is already correct.)
    if (type == SOCK_DGRAM && GetSocketAddressFamily(*fd) == AF_INET6) {
        Int32 ttl = 1;
        setsockopt(*fd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &ttl, sizeof(Int32));
    }
#endif

    return NOERROR;
}

static ECode DoShutdown(Int32 fd, Int32 how)
{
    if (fd == -1) {
        return NOERROR;
    }
    Int32 rc = shutdown(fd, how);
    if (rc == -1) {
//        jniThrowSocketException(env, errno);
        return E_PLATFORM_SOCKET_EXCEPTION;
    }
    return NOERROR;
}

ECode OSNetworkSystem::ShutdownInput(
    /* [in] */ Int32 fd)
{
    return DoShutdown(fd, SHUT_RD);
}

ECode OSNetworkSystem::ShutdownOutput(
    /* [in] */ Int32 fd)
{
    return DoShutdown(fd, SHUT_WR);
}

ECode OSNetworkSystem::SendUrgentData(
    /* [in] */ Int32 fd,
    /* [in] */ Byte value)
{
    if (fd == - 1) {
        return NOERROR;
    }

    Int32 rc = send(fd, &value, 1, MSG_OOB);
    if (rc == -1) {
//        jniThrowSocketException(env, errno);
        return E_PLATFORM_SOCKET_EXCEPTION;
    }
    return NOERROR;
}

ECode OSNetworkSystem::Listen(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 backlog)
{
    if (fd == -1) {
        return NOERROR;
    }

    Int32 rc = listen(fd, backlog);
    if (rc == -1) {
//        jniThrowSocketException(env, errno);
        return E_PLATFORM_SOCKET_EXCEPTION;
    }
    return NOERROR;
}

ECode OSNetworkSystem::Connect(
    /* [in] */ Int32 fd,
    /* [in] */ const ArrayOf<Byte>& ipAddress,
    /* [in] */ Int32 port,
    /* [in] */ Int32 timeout)
{
    /* if a timeout was specified calculate the finish time value */
    Boolean hasTimeout = timeout > 0;
    Int32 finishTime = 0;
    if (hasTimeout)  {
        finishTime = time_msec_clock() + (Int32)timeout;
    }

    if (fd == -1) {
        return NOERROR;
    }

    ConnectHelper context;
    Int32 result = context.Start(fd, ipAddress, port);
    Int32 remainingTimeout = timeout;
    while (result == -EINPROGRESS) {
        /*
         * ok now try and connect. Depending on the platform this may sleep
         * for up to passedTimeout milliseconds
         */
        result = context.IsConnected(fd, remainingTimeout);
        if (fd == -1) {
            return NOERROR;
        }
        if (result == 0) {
            context.DidConnect(fd);
            return NOERROR;
        }
        else if (result != -EINPROGRESS) {
            return context.DidFail(fd, result);
        }

        /* check if the timeout has expired */
        if (hasTimeout) {
            remainingTimeout = finishTime - time_msec_clock();
            if (remainingTimeout <= 0) {
                return context.DidFail(fd, -ETIMEDOUT);
            }
        }
        else {
            remainingTimeout = 100;
        }
    }
    return NOERROR;
}

ECode OSNetworkSystem::GetSocketLocalAddress(
    /* [in] */ Int32 fd,
    /* [out] */ ArrayOf<Byte>* ipAddress)
{
    VALIDATE_NOT_NULL(ipAddress);

    if (fd == -1) {
        return NOERROR;
    }

    sockaddr_storage ss;
    socklen_t ssLen = sizeof(ss);
    memset(&ss, 0, ssLen);
    Int32 rc = getsockname(fd, reinterpret_cast<sockaddr*>(&ss), &ssLen);
    if (rc == -1) {
        // TODO: the public API doesn't allow failure, so this whole method
        // represents a broken design. In practice, though, getsockname can't
        // fail unless we give it invalid arguments.
//        LOGE("getsockname failed: %s (errno=%i)", strerror(errno), errno);
        return E_PLATFORM_SOCKET_EXCEPTION;
    }
    return SocketAddressToByteArray(&ss, ipAddress);
}

static Boolean IsValidFd(Int32 fd)
{
    return fd >= 0 && fd < FD_SETSIZE;
}

static Boolean InitFdSet(const ArrayOf<Int32>& fdArray, Int32 count, fd_set* fdSet, Int32* maxFd)
{
    for (Int32 i = 0; i < count; ++i) {
        const Int32 fd = fdArray[i];
        if (!IsValidFd(fd)) {
//            LOGE("selectImpl: ignoring invalid fd %i", fd);
            continue;
        }

        FD_SET(fd, fdSet);

        if (fd > *maxFd) {
            *maxFd = fd;
        }
    }
    return TRUE;
}

/*
 * Note: fdSet has to be non-const because although on Linux FD_ISSET() is sane
 * and takes a const fd_set*, it takes fd_set* on Mac OS. POSIX is not on our
 * side here:
 *   http://www.opengroup.org/onlinepubs/000095399/functions/select.html
 */
static Boolean TranslateFdSet(const ArrayOf<Int32>& fdArray, Int32 count, fd_set& fdSet,
        ArrayOf<Int32>* flagArray, size_t offset, Int32 op)
{
    for (Int32 i = 0; i < count; ++i) {
        const Int32 fd = fdArray[i];
        if (IsValidFd(fd) && FD_ISSET(fd, &fdSet)) {
            (*flagArray)[i + offset] = op;
        }
        else {
            (*flagArray)[i + offset] = SOCKET_OP_NONE;
        }
    }
    return TRUE;
}

ECode OSNetworkSystem::Select(
    /* [in] */ const ArrayOf<Int32>& readFDs,
    /* [in] */ const ArrayOf<Int32>& writeFDs,
    /* [in] */ Int32 numReadable,
    /* [in] */ Int32 numWritable,
    /* [in] */ Int64 timeout,
    /* [out] */ ArrayOf<Int32>* flags,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(flags);
    VALIDATE_NOT_NULL(succeeded);

    if (numReadable < 0 || numWritable < 0) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 total = numReadable + numWritable;
    if (total == 0) {
        *succeeded = TRUE;
        return NOERROR;
    }

    // Initialize the fd_sets.
    Int32 maxFd = -1;
    fd_set readFds;
    fd_set writeFds;
    FD_ZERO(&readFds);
    FD_ZERO(&writeFds);
    Boolean initialized = InitFdSet(readFDs, numReadable, &readFds, &maxFd) &&
                          InitFdSet(writeFDs, numWritable, &writeFds, &maxFd);
    if (!initialized) {
        *succeeded = FALSE;
        return NOERROR;
    }

    // Initialize the timeout, if any.
    timeval tv;
    timeval* tvp = NULL;
    if (timeout >= 0) {
        tv = ToTimeval(timeout);
        tvp = &tv;
    }

    // Perform the select.
    Int32 result = select(maxFd + 1, &readFds, &writeFds, NULL, tvp);
    if (result == 0) {
        // Timeout.
        *succeeded = FALSE;
        return NOERROR;
    }
    else if (result == -1) {
        // Error.
        if (errno == EINTR) {
            *succeeded = FALSE;
            return NOERROR;
        }
        else {
            *succeeded = FALSE;
//            jniThrowSocketException(env, errno);
            return E_PLATFORM_SOCKET_EXCEPTION;
        }
    }

    *succeeded = TranslateFdSet(readFDs, numReadable, readFds, flags, 0, SOCKET_OP_READ) &&
              TranslateFdSet(writeFDs, numWritable, writeFds, flags, numReadable, SOCKET_OP_WRITE);
    return NOERROR;
}

ECode OSNetworkSystem::GetSocketLocalPort(
    /* [in] */ Int32 fd,
    /* [out] */ Int32* localPort)
{
    VALIDATE_NOT_NULL(localPort);

    if (fd == -1) {
        *localPort = 0;
        return NOERROR;
    }

    sockaddr_storage ss;
    socklen_t ssLen = sizeof(ss);
    memset(&ss, 0, sizeof(ss));
    Int32 rc = getsockname(fd, reinterpret_cast<sockaddr*>(&ss), &ssLen);
    if (rc == -1) {
        // TODO: the public API doesn't allow failure, so this whole method
        // represents a broken design. In practice, though, getsockname can't
        // fail unless we give it invalid arguments.
//        LOGE("getsockname failed: %s (errno=%i)", strerror(errno), errno);
        *localPort = 0;
        return E_PLATFORM_SOCKET_EXCEPTION;
    }
    *localPort = GetSocketAddressPort(&ss);
    return NOERROR;
}

template <typename T>
static ECode GetSocketOptionImpl(Int32 fd, Int32 level, Int32 option, T* value)
{
    socklen_t size = sizeof(*value);
    Int32 rc = getsockopt(fd, level, option, value, &size);
    if (rc == -1) {
//        LOGE("getSocketOption(fd=%i, level=%i, option=%i) failed: %s (errno=%i)",
//                fd.get(), level, option, strerror(errno), errno);
//        jniThrowSocketException(env, errno);
        return E_PLATFORM_SOCKET_EXCEPTION;
    }
    return NOERROR;
}

ECode OSNetworkSystem::GetSocketOption(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 option,
    /* [out] */ Int32* optVal)
{
    VALIDATE_NOT_NULL(optVal);

    if (fd == -1) {
        return NOERROR;
    }

    Int32 family = GetSocketAddressFamily(fd);
    if (family != AF_INET && family != AF_INET6) {
//        jniThrowSocketException(env, EAFNOSUPPORT);
        return E_PLATFORM_SOCKET_EXCEPTION;
    }

    switch (option) {
    case SOCKOPT_TCP_NODELAY:
        return GetSocketOptionImpl(fd, IPPROTO_TCP, TCP_NODELAY, optVal);
    case SOCKOPT_SO_SNDBUF:
        return GetSocketOptionImpl(fd, SOL_SOCKET, SO_SNDBUF, optVal);
    case SOCKOPT_SO_RCVBUF:
        return GetSocketOptionImpl(fd, SOL_SOCKET, SO_RCVBUF, optVal);
    case SOCKOPT_SO_BROADCAST:
        return GetSocketOptionImpl(fd, SOL_SOCKET, SO_BROADCAST, optVal);
    case SOCKOPT_SO_REUSEADDR:
        return GetSocketOptionImpl(fd, SOL_SOCKET, SO_REUSEADDR, optVal);
    case SOCKOPT_SO_KEEPALIVE:
        return GetSocketOptionImpl(fd, SOL_SOCKET, SO_KEEPALIVE, optVal);
    case SOCKOPT_SO_OOBINLINE:
        return GetSocketOptionImpl(fd, SOL_SOCKET, SO_OOBINLINE, optVal);
    case SOCKOPT_IP_TOS:
        if (family == AF_INET) {
            return GetSocketOptionImpl(fd, IPPROTO_IP, IP_TOS, optVal);
        }
        else {
            return GetSocketOptionImpl(fd, IPPROTO_IPV6, IPV6_TCLASS, optVal);
        }
    case SOCKOPT_SO_LINGER:
        {
            linger lingr;
            FAIL_RETURN(GetSocketOptionImpl(fd, SOL_SOCKET, SO_LINGER, &lingr));

            if (!lingr.l_onoff) {
                *optVal = -1;
                return NOERROR;
            }
            else {
                *optVal = lingr.l_linger;
                return NOERROR;
            }
        }
    case SOCKOPT_SO_TIMEOUT:
        {
            timeval timeout;
            FAIL_RETURN(GetSocketOptionImpl(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout));
            *optVal = ToMs(timeout);
            return NOERROR;
        }
#ifdef ENABLE_MULTICAST
    case SOCKOPT_IP_MULTICAST_IF:
        {
            // Although setsockopt(2) can take an ip_mreqn for IP_MULTICAST_IF, getsockopt(2)
            // always returns an in_addr.
            sockaddr_storage ss;
            memset(&ss, 0, sizeof(ss));
            ss.ss_family = AF_INET; // This call is IPv4-only.
            sockaddr_in* sa = reinterpret_cast<sockaddr_in*>(&ss);
            FAIL_RETURN(GetSocketOptionImpl(fd, IPPROTO_IP, IP_MULTICAST_IF, &sa->sin_addr));

            return SocketAddressToByteArray(&ss, reinterpret_cast<ArrayOf<Byte>*>(optVal));
        }
    case SOCKOPT_IP_MULTICAST_IF2:
        if (family == AF_INET) {
            // The caller's asking for an interface index, but that's not how IPv4 works.
            // Our Java should never get here, because we'll try IP_MULTICAST_IF first and
            // that will satisfy us.
//            jniThrowSocketException(env, EAFNOSUPPORT);
            return E_PLATFORM_SOCKET_EXCEPTION;
        }
        else {
            return GetSocketOptionImpl(fd, IPPROTO_IPV6, IPV6_MULTICAST_IF, optVal);
        }
    case SOCKOPT_IP_MULTICAST_LOOP:
        if (family == AF_INET) {
            // Although IPv6 was cleaned up to use int, IPv4 multicast loopback uses a byte.
            u_char loopback;
            FAIL_RETURN(GetSocketOptionImpl(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &loopback));
            *optVal = loopback;
            return NOERROR;
        }
        else {
            return GetSocketOptionImpl(fd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, optVal);
        }
    case SOCKOPT_MULTICAST_TTL:
        if (family == AF_INET) {
            // Although IPv6 was cleaned up to use int, and IPv4 non-multicast TTL uses int,
            // IPv4 multicast TTL uses a byte.
            u_char ttl;
            FAIL_RETURN(GetSocketOptionImpl(fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl));
            *optVal = ttl;
            return NOERROR;
        }
        else {
            return GetSocketOptionImpl(fd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, optVal);
        }
#else
    case SOCKOPT_MULTICAST_TTL:
    case SOCKOPT_IP_MULTICAST_IF:
    case SOCKOPT_IP_MULTICAST_IF2:
    case SOCKOPT_IP_MULTICAST_LOOP:
//        jniThrowException(env, "java/lang/UnsupportedOperationException", NULL);
        return E_PLATFORM_UNSUPPORTED_OPERATION_EXCEPTION;
#endif // def ENABLE_MULTICAST
    default:
//        jniThrowSocketException(env, ENOPROTOOPT);
        return E_PLATFORM_SOCKET_EXCEPTION;
    }
}

template <typename T>
static ECode SetSocketOptionImpl(Int32 fd, Int32 level, Int32 option, T* value)
{
    Int32 rc = setsockopt(fd, level, option, value, sizeof(*value));
    if (rc == -1) {
//        LOGE("setSocketOption(fd=%i, level=%i, option=%i) failed: %s (errno=%i)",
//                fd.get(), level, option, strerror(errno), errno);
//        jniThrowSocketException(env, errno);
        return E_PLATFORM_SOCKET_EXCEPTION;
    }
    return NOERROR;
}

ECode OSNetworkSystem::SetSocketOption(
    /* [in] */ Int32 fd,
    /* [in] */ Int32 option,
    /* [in] */ Int32 optVal)
{
    if (fd == -1) {
        return NOERROR;
    }

    Int32 intVal = optVal;

    Int32 family = GetSocketAddressFamily(fd);
    if (family != AF_INET && family != AF_INET6) {
//        jniThrowSocketException(env, EAFNOSUPPORT);
        return E_PLATFORM_SOCKET_EXCEPTION;
    }

    // Since we expect to have a AF_INET6 socket even if we're communicating via IPv4, we always
    // set the IPPROTO_IP options. As long as we fall back to creating IPv4 sockets if creating
    // an IPv6 socket fails, we need to make setting the IPPROTO_IPV6 options conditional.
    switch (option) {
    case SOCKOPT_IP_TOS:
        SetSocketOptionImpl(fd, IPPROTO_IP, IP_TOS, &intVal);
        if (family == AF_INET6) {
            SetSocketOptionImpl(fd, IPPROTO_IPV6, IPV6_TCLASS, &intVal);
        }
        return NOERROR;
    case SOCKOPT_SO_BROADCAST:
        SetSocketOptionImpl(fd, SOL_SOCKET, SO_BROADCAST, &intVal);
        return NOERROR;
    case SOCKOPT_SO_KEEPALIVE:
        SetSocketOptionImpl(fd, SOL_SOCKET, SO_KEEPALIVE, &intVal);
        return NOERROR;
    case SOCKOPT_SO_LINGER:
        {
            linger l;
            l.l_onoff = !(intVal == -1);
            l.l_linger = intVal <= 65535 ? intVal : 65535;
            SetSocketOptionImpl(fd, SOL_SOCKET, SO_LINGER, &l);
            return NOERROR;
        }
    case SOCKOPT_SO_OOBINLINE:
        SetSocketOptionImpl(fd, SOL_SOCKET, SO_OOBINLINE, &intVal);
        return NOERROR;
    case SOCKOPT_SO_RCVBUF:
        SetSocketOptionImpl(fd, SOL_SOCKET, SO_RCVBUF, &intVal);
        return NOERROR;
    case SOCKOPT_SO_REUSEADDR:
        SetSocketOptionImpl(fd, SOL_SOCKET, SO_REUSEADDR, &intVal);
        return NOERROR;
    case SOCKOPT_SO_SNDBUF:
        SetSocketOptionImpl(fd, SOL_SOCKET, SO_SNDBUF, &intVal);
        return NOERROR;
    case SOCKOPT_SO_TIMEOUT:
        {
            timeval timeout(ToTimeval(intVal));
            SetSocketOptionImpl(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout);
            return NOERROR;
        }
    case SOCKOPT_TCP_NODELAY:
        SetSocketOptionImpl(fd, IPPROTO_TCP, TCP_NODELAY, &intVal);
        return NOERROR;
#ifdef ENABLE_MULTICAST
    case SOCKOPT_MCAST_JOIN_GROUP:
        McastJoinLeaveGroup(fd, optVal, TRUE);
        return NOERROR;
    case SOCKOPT_MCAST_LEAVE_GROUP:
        McastJoinLeaveGroup(fd, optVal, FALSE);
        return NOERROR;
    case SOCKOPT_IP_MULTICAST_IF:
        {
            sockaddr_storage sockVal;
            FAIL_RETURN(IpAddressToSocketAddress(*(ArrayOf<Byte>*)optVal, 0, &sockVal));

            // This call is IPv4 only. The socket may be IPv6, but the address
            // that identifies the interface to join must be an IPv4 address.
            if (sockVal.ss_family != AF_INET) {
//                jniThrowSocketException(env, EAFNOSUPPORT);
                return E_PLATFORM_SOCKET_EXCEPTION;
            }
            ip_mreqn mcast_req;
            memset(&mcast_req, 0, sizeof(mcast_req));
            mcast_req.imr_address = reinterpret_cast<sockaddr_in*>(&sockVal)->sin_addr;
            SetSocketOptionImpl(fd, IPPROTO_IP, IP_MULTICAST_IF, &mcast_req);
            return NOERROR;
        }
    case SOCKOPT_IP_MULTICAST_IF2:
        // TODO: is this right? should we unconditionally set the IPPROTO_IP state in case
        // we have an IPv6 socket communicating via IPv4?
        if (family == AF_INET) {
            // IP_MULTICAST_IF expects a pointer to an ip_mreqn struct.
            ip_mreqn multicastRequest;
            memset(&multicastRequest, 0, sizeof(multicastRequest));
            multicastRequest.imr_ifindex = intVal;
            SetSocketOptionImpl(fd, IPPROTO_IP, IP_MULTICAST_IF, &multicastRequest);
        }
        else {
            // IPV6_MULTICAST_IF expects a pointer to an integer.
            SetSocketOptionImpl(fd, IPPROTO_IPV6, IPV6_MULTICAST_IF, &intVal);
        }
        return NOERROR;
    case SOCKOPT_MULTICAST_TTL:
        {
            // Although IPv6 was cleaned up to use int, and IPv4 non-multicast TTL uses int,
            // IPv4 multicast TTL uses a byte.
            u_char ttl = intVal;
            SetSocketOptionImpl(fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl);
            if (family == AF_INET6) {
                SetSocketOptionImpl(fd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &intVal);
            }
            return NOERROR;
        }
    case SOCKOPT_IP_MULTICAST_LOOP:
        {
            // Although IPv6 was cleaned up to use int, IPv4 multicast loopback uses a byte.
            u_char loopback = intVal;
            SetSocketOptionImpl(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &loopback);
            if (family == AF_INET6) {
                SetSocketOptionImpl(fd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &intVal);
            }
            return NOERROR;
        }
#else
    case SOCKOPT_MULTICAST_TTL:
    case SOCKOPT_MCAST_JOIN_GROUP:
    case SOCKOPT_MCAST_LEAVE_GROUP:
    case SOCKOPT_IP_MULTICAST_IF:
    case SOCKOPT_IP_MULTICAST_IF2:
    case SOCKOPT_IP_MULTICAST_LOOP:
//        jniThrowException(env, "java/lang/UnsupportedOperationException", NULL);
        return E_PLATFORM_UNSUPPORTED_OPERATION_EXCEPTION;
#endif // def ENABLE_MULTICAST
    default:
//        jniThrowSocketException(env, ENOPROTOOPT);
        return E_PLATFORM_SOCKET_EXCEPTION;
    }
}

ECode OSNetworkSystem::Close(
    /* [in] */ Int32 fd)
{
    if (fd == -1) {
        return NOERROR;
    }

    AsynchronousSocketCloseMonitor::SignalBlockedThreads(fd);
    close(fd);
    return NOERROR;
}
