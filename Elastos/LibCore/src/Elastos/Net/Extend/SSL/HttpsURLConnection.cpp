#include "HttpsURLConnection.h"	
	
IHostnameVerifier* HttpsURLConnection::mDefaultHostnameVerifier = NULL;
ISSLSocketFactory* HttpsURLConnection::mDefaultSSLSocketFactory = NULL;

ECode GetPeerPrincipal(
	/* [out] */ IPrincipal** ppPrincipal)
{
	return E_NOT_IMPLEMENTED;
}

ECode GetLocalPrincipal(
	/* [out] */ IPrincipal** ppPrincipal)
{
	return E_NOT_IMPLEMENTED;
}

ECode SetHostnameVerifier(
	/* [in] */ IHostnameVerifier* v)
{
	return E_NOT_IMPLEMENTED;
}
	
ECode GetHostnameVerifier(
	/* [out] */ IHostnameVerifier** ppV)
{
	return E_NOT_IMPLEMENTED;
}
	
ECode SetSSLSocketFactory(
	/* [in] */ ISSLSocketFactory* sf)
{
	return E_NOT_IMPLEMENTED;
}
	
ECode GetSSLSocketFactory(
	/* [out] */ ISSLSocketFactory** ppSf)
{
	return E_NOT_IMPLEMENTED;
}
	
static ECode SetDefaultHostnameVerifier(
	/* [in] */ IHostnameVerifier* v)
{
	return E_NOT_IMPLEMENTED;
}
	
static ECode GetDefaultHostnameVerifier(
	/* [out] */ IHostnameVerifier** ppV)
{
	return E_NOT_IMPLEMENTED;
}
	
static ECode SetDefaultSSLSocketFactory(
	/* [in] */ ISSLSocketFactory* sf)
{
	return E_NOT_IMPLEMENTED;
}
	
static ECode GetDefaultSSLSocketFactory(
	/* [out] */ ISSLSocketFactory** ppSf)
{
	return E_NOT_IMPLEMENTED;
}
			