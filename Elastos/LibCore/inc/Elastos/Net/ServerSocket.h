
#ifndef __SERVERSOCKET_H__
#define __SERVERSOCKET_H__

#include "Elastos.Net_server.h"
#include "CSocket.h"

using namespace Elastos;
using namespace Elastos::Core::Threading;

class ServerSocket
{
public:
    ServerSocket();

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

    virtual CARAPI Close();

    /**
     * Returns the default number of pending connections on a server socket. If
     * the backlog value maximum is reached, any subsequent incoming request is
     * rejected.
     *
     * @return int the default number of pending connection requests
     */
    static CARAPI_(Int32) DefaultBacklog();

    virtual CARAPI GetInetAddress(
        /* [out] */ IInetAddress** address);

    virtual CARAPI GetLocalPort(
        /* [out] */ Int32* port);

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

    virtual CARAPI SetSoTimeout(
        /* [in] */ Int32 timeout);

    virtual CARAPI Bind(
        /* [in] */ ISocketAddress* localAddr);

    virtual CARAPI BindEx(
        /* [in] */ ISocketAddress* localAddr,
        /* [in] */ Int32 backlog);

    virtual CARAPI GetLocalSocketAddress(
        /* [out] */ ISocketAddress** address);

    virtual CARAPI IsBound(
        /* [out] */ Boolean* isBound);

    virtual CARAPI IsClosed(
        /* [out] */ Boolean* isClosed);

    virtual CARAPI SetReuseAddress(
        /* [in] */ Boolean reuse);

    virtual CARAPI GetReuseAddress(
        /* [out] */ Boolean* reuse);

    virtual CARAPI SetReceiveBufferSize(
        /* [in] */ Int32 size);

    virtual CARAPI GetReceiveBufferSize(
        /* [out] */ Int32* size);

    virtual CARAPI GetChannel(
        /* [out] */ IServerSocketChannel** channel);

    virtual CARAPI SetPerformancePreferences(
        /* [in] */ Int32 connectionTime,
        /* [in] */ Int32 latency,
        /* [in] */ Int32 bandwidth);

    CARAPI Init();

    CARAPI Init(
        /* [in] */ Int32 aPort);

    CARAPI Init(
        /* [in] */ Int32 aPort,
        /* [in] */ Int32 backlog);

    CARAPI Init(
        /* [in] */ Int32 aPort,
        /* [in] */ Int32 backlog,
        /* [in] */ IInetAddress* localAddr);

protected:
    /**
     * Unspecified constructor needed by ServerSocketChannelImpl.ServerSocketAdapter.
     *
     * @hide
     */
    CARAPI Init(
        /* [in] */ ISocketImpl* impl);

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

    static AutoPtr<ISocketImplFactory> mFactory;

private:
    Boolean mIsCreated;

    Boolean mIsBound;

    Boolean mIsClosed;

    static Mutex* mLock;
};

#endif //__SERVERSOCKET_H__
