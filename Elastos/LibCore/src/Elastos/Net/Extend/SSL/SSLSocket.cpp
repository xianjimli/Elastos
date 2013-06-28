#include "SSLSocket.h"

SSLSocket::SSLSocket()
{
}

SSLSocket::SSLSocket(
	/* [in] */ const String& host, 
	/* [in] */ Int32 port)
{
}
SSLSocket::SSLSocket(
	/* [in] */ IInetAddress* address, 
	/* [in] */ Int32 port)
{
}

SSLSocket::SSLSocket(
	/* [in] */ const String& host, 
	/* [in] */ Int32 port, 
	/* [in] */ IInetAddress* clientAddress, 
	/* [in] */ Int32 clientPort)
{
}

SSLSocket::SSLSocket(
	/* [in] */ IInetAddress* address, 
	/* [in] */ Int32 port, 
	/* [in] */ IInetAddress* clientAddress, 
	/* [in] */ Int32 clientPort)
{
}

ECode SSLSocket::GetSSLParameters(
	/* [out] */ ISSLParameters** ppPara)
{
	return E_NOT_IMPLEMENTED;
}

ECode SSLSocket::SetSSLParameters(
	/* [in] */ ISSLParameters* p)
{
	return E_NOT_IMPLEMENTED;
}