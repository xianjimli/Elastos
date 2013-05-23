#ifndef __HTTPCONNECTION_H__
#define __HTTPCONNECTION_H__

//#include "CSSLSocket.h"
//#include "CSSLSocketFactory.h"
#include "CProxy.h"
#include "CURI.h"
#include "Elastos.IO.h"

class HttpConnection
{
public:
    class Address {
    private:
        AutoPtr<IProxy> mProxy;
        Boolean mRequiresTunnel;
        String mUriHost;
        Int32 mUriPort;
        String mSocketHost;
        Int32 mSocketPort;

    public:
        Address(
            /* [in] */ IURI* pUri);

        /**
         * @param requiresTunnel true if the HTTP connection needs to tunnel one
         *     protocol over another, such as when using HTTPS through an HTTP
         *     proxy. When doing so, we must avoid buffering bytes intended for
         *     the higher-level protocol.
         */
        Address(
            /* [in] */ IURI* pUri,
            /* [in] */ IProxy* pProxy,
            /* [in] */ Boolean requiresTunnel);

        Boolean Equals(
            /* [in] */ IInterface* pOther);

        Int32 HashCode();

        ECode Connect(
            /* [in] */ Int32 connectTimeout,
            /* [out] */ HttpConnection** ppConnection);
    };

public:
    HttpConnection(
        /* [in] */ Address config,
        /* [in] */ Int32 connectTimeout);

    void CloseSocketAndStreams();

    void SetSoTimeout(
        /* [in] */ Int32 readTimeout);

    ECode GetOutputStream(
        /* [out] */ IOutputStream** ppOs);

    ECode GetInputStream(
        /* [out] */ IInputStream** ppIs);

    Address* GetAddress();

    /**
     * Create an {@code SSLSocket} and perform the SSL handshake
     * (performing certificate validation.
     *
     * @param sslSocketFactory Source of new {@code SSLSocket} instances.
     * @param tlsTolerant If true, assume server can handle common
     * TLS extensions and SSL deflate compression. If false, use
     * an SSL3 only fallback mode without compression.
     */
    void SetupSecureSocket(
        /* [in] */ ISSLSocketFactory* sslSocketFactory,
        /* [in] */ Boolean tlsTolerant);

    /**
     * Return an {@code SSLSocket} that is not only connected but has
     * also passed hostname verification.
     *
     * @param hostnameVerifier Used to verify the hostname we
     * connected to is an acceptable match for the peer certificate
     * chain of the SSLSession.
     */
    ISSLSocket* VerifySecureSocketHostname(
        /* [in] */ IHostnameVerifier* hostnameVerifier);

    /**
     * Return an {@code SSLSocket} if already connected, otherwise null.
     */
    ISSLSocket* GetSecureSocketIfConnected();

    /**
     * Returns true if the connection is functional. This uses a shameful hack
     * to peek a byte from the socket.
     */
    ECode IsStale(
        /* [out] */ Boolean* isStale);

protected:
    /**
     * Returns true if this connection is eligible to be recycled. This
     * is like {@link #isStale} except that it doesn't try to actually
     * perform any I/O.
     */
    Boolean IsEligibleForRecycling();

private:
    void CloseQuietly(
        /* [in] */ ISocket* socket);

    void CloseQuietly();


    AutoPtr<ISocket> GetSocket();

private:
    Address* mAddress;

    AutoPtr<ISocket> mSocket;
    AutoPtr<IInputStream> mInputStream;
    AutoPtr<IOutputStream> mOutputStream;

    AutoPtr<ISSLSocket> mUnverifiedSocket;
    AutoPtr<ISSLSocket> mSslSocket;
    AutoPtr<IInputStream> mSslInputStream;
    AutoPtr<IOutputStream> mSslOutputStream;
};

#endif //__HTTPCONNECTION_H__
