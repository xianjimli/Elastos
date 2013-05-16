#include "HttpConnection.h"

HttpConnection::Address::Address(
    /* [in] */ IURI* pUri)
{
}

HttpConnection::Address::Address(
    /* [in] */ IURI* pUri,
    /* [in] */ IProxy* pProxy,
    /* [in] */ Boolean requiresTunnel)
{
}

Boolean HttpConnection::Address::Equals(
    /* [in] */ IInterface* pOther)
{
}

Int32 HttpConnection::Address::HashCode()
{
}

ECode HttpConnection::Address::Connect(
    /* [in] */ Int32 connectTimeout,
    /* [out] */ HttpConnection** ppConnection)
{
}
//HttpConnectionStart  -----------------

HttpConnection::HttpConnection(
    /* [in] */ Address config,
    /* [in] */ Int32 connectTimeout)
{
}

void HttpConnection::CloseSocketAndStreams()
{
}

void HttpConnection::SetSoTimeout(
    /* [in] */ Int32 readTimeout)
{
}

ECode HttpConnection::GetOutputStream(
    /* [out] */ IOutputStream** ppOs)
{
}

ECode HttpConnection::GetInputStream(
    /* [out] */ IInputStream** ppIs)
{
}

HttpConnection::Address* HttpConnection::GetAddress()
{
}

void HttpConnection::SetupSecureSocket(
    /* [in] */ ISSLSocketFactory* sslSocketFactory,
    /* [in] */ Boolean tlsTolerant)
{
}

ISSLSocket* HttpConnection::VerifySecureSocketHostname(
    /* [in] */ IHostnameVerifier* hostnameVerifier)
{
}

ISSLSocket* HttpConnection::GetSecureSocketIfConnected()
{
}

ECode HttpConnection::IsStale(
    /* [out] */ Boolean* isStale)
{
}

Boolean HttpConnection::IsEligibleForRecycling()
{
}


void HttpConnection::CloseQuietly(
    /* [in] */ ISocket* socket)
{
}

void HttpConnection::CloseQuietly()
{
}


AutoPtr<ISocket> HttpConnection::GetSocket()
{
}
