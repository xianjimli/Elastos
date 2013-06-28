#include "HttpsURLConnection.h"	
	
IHostnameVerifier* HttpsURLConnection::mDefaultHostnameVerifier = NULL;
ISSLSocketFactory* HttpsURLConnection::mDefaultSSLSocketFactory = NULL;

ECode HttpsURLConnection::GetPeerPrincipal(
	/* [out] */ IPrincipal** ppPrincipal)
{
	return E_NOT_IMPLEMENTED;
}

ECode HttpsURLConnection::GetLocalPrincipal(
	/* [out] */ IPrincipal** ppPrincipal)
{
	return E_NOT_IMPLEMENTED;
}

ECode HttpsURLConnection::SetHostnameVerifier(
	/* [in] */ IHostnameVerifier* v)
{
	if (v == NULL) {
		return  E_INVALID_ARGUMENT;
	}
	
	mHostnameVerifier = v;
	return NOERROR;
}
	
ECode HttpsURLConnection::GetHostnameVerifier(
	/* [out] */ IHostnameVerifier** ppV)
{
	*ppV = mHostnameVerifier;
	return NOERROR;
}
	
ECode HttpsURLConnection::SetSSLSocketFactory(
	/* [in] */ ISSLSocketFactory* sf)
{
    if (sf == NULL) {
    	return E_INVALID_ARGUMENT;
    }
    mSslSocketFactory = sf;
    return NOERROR;
}
	
ECode HttpsURLConnection::GetSSLSocketFactory(
	/* [out] */ ISSLSocketFactory** ppSf)
{
	*ppSf = mSslSocketFactory;	
	return NOERROR;
}
	
ECode HttpsURLConnection::SetDefaultHostnameVerifier(
	/* [in] */ IHostnameVerifier* v)
{
	if (v == NULL) {
		return  E_INVALID_ARGUMENT;
	}
	
	mDefaultHostnameVerifier = v;
	return NOERROR;
}
	
ECode HttpsURLConnection::GetDefaultHostnameVerifier(
	/* [out] */ IHostnameVerifier** ppV)
{
	*ppV = mDefaultHostnameVerifier;
	return NOERROR;
}
	
ECode HttpsURLConnection::SetDefaultSSLSocketFactory(
	/* [in] */ ISSLSocketFactory* sf)
{
    if (sf == NULL) {
    	return E_INVALID_ARGUMENT;
    }
    mDefaultSSLSocketFactory = sf;	
	return NOERROR;
}
	
ECode HttpsURLConnection::GetDefaultSSLSocketFactory(
	/* [out] */ ISSLSocketFactory** ppSf)
{
	*ppSf = mDefaultSSLSocketFactory;
	return NOERROR;
}
			