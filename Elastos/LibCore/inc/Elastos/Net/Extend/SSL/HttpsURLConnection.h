#ifndef __HTTPSURLCONNECTION_H__
#define __HTTPSURLCONNECTION_H__

#include <elastos.h>
#include "Elastos.Net_server.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;

class HttpsURLConnection 
{
public:
	static ECode SetDefaultHostnameVerifier(
		/* [in] */ IHostnameVerifier* v);
	
	static ECode GetDefaultHostnameVerifier(
		/* [out] */ IHostnameVerifier** ppV);
	
	static ECode SetDefaultSSLSocketFactory(
		/* [in] */ ISSLSocketFactory* sf);
	
	static ECode GetDefaultSSLSocketFactory(
		/* [out] */ ISSLSocketFactory** ppSf);


public: 
	virtual ECode GetCipherSuite(
		/* [out] */ String* pStr) = 0;
		
	virtual ECode GetLocalCertificates(
		/* [out, callee] */ ArrayOf<ICertificate*>** ppArray) = 0;
	
	virtual ECode GetServerCertificates(
		/* [out, callee] */ ArrayOf<ICertificate*>** ppArray) = 0;	

	ECode GetPeerPrincipal(
		/* [out] */ IPrincipal** ppPrincipal);

	ECode GetLocalPrincipal(
		/* [out] */ IPrincipal** ppPrincipal);
	
	ECode SetHostnameVerifier(
		/* [in] */ IHostnameVerifier* v);
		
	ECode GetHostnameVerifier(
		/* [out] */ IHostnameVerifier** ppV);

	ECode SetSSLSocketFactory(
		/* [in] */ ISSLSocketFactory* sf);
	
	ECode GetSSLSocketFactory(
		/* [out] */ ISSLSocketFactory** ppSf);					

protected:
	HttpsURLConnection(
		/* [in] */ IURL* url);

protected:
	AutoPtr<IHostnameVerifier>	mHostnameVerifier;
	
private:
	static IHostnameVerifier* mDefaultHostnameVerifier;
	static ISSLSocketFactory* mDefaultSSLSocketFactory;
	AutoPtr<ISSLSocketFactory> mSslSocketFactory;
};

#endif //__HTTPURLCONNECTION_H__