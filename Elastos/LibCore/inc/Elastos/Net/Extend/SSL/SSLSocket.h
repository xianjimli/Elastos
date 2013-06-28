#ifndef __SSLSOCKET_H__
#define __SSLSOCKET_H__

#include "Socket.h"
#include <elastos.h>

class SSLSocket : public Socket
{
protected:
	SSLSocket();
	
	SSLSocket(
		/* [in] */ const String& host, 
		/* [in] */ Int32 port);
	
	SSLSocket(
		/* [in] */ IInetAddress* address, 
		/* [in] */ Int32 port);
	
	SSLSocket(
		/* [in] */ const String& host, 
		/* [in] */ Int32 port, 
		/* [in] */ IInetAddress* clientAddress, 
		/* [in] */ Int32 clientPort);
	
	SSLSocket(
		/* [in] */ IInetAddress* address, 
		/* [in] */ Int32 port, 
		/* [in] */ IInetAddress* clientAddress, 
		/* [in] */ Int32 clientPort);

public:
	virtual ECode GetSupportedCipherSuites(
		/* [out, callee] */ ArrayOf<String>** array) = 0;
	
	virtual ECode GetEnabledCipherSuites(
		/* [out, callee] */ ArrayOf<String>** array) = 0;
	
	virtual ECode SetEnabledCipherSuites(
		/* [in] */ const ArrayOf<String>& suites) = 0;
	
	virtual ECode GetSupportedProtocols(
		/* [out, callee] */ ArrayOf<String>** array) = 0;
		
	virtual ECode GetEnabledProtocols(
		/* [out, callee] */ ArrayOf<String>** array) = 0;
	
	virtual ECode SetEnabledProtocols(
		/* [in] */ const ArrayOf<String>& protocols);
	
	virtual ECode GetSession(
		/* [out] */ ISSLSession** session) = 0;
	
//	ECode AddHandshakeCompletedListener(
//		/* [in] */ IHandshakeCompletedListener* listener) = 0;
//	
//	ECode RemoveHandshakeCompletedListener(
//		/* [in] */ IHandshakeCompletedListener listener) = 0;
	
	virtual ECode StartHandshake() = 0;
	
	virtual ECode SetUseClientMode(
		/* [in] */ Boolean mode) = 0;
		
	virtual ECode GetUseClientMode(
		/* [out] */ Boolean* used) = 0;
	
	virtual ECode SetNeedClientAuth(
		/* [in] */ Boolean need) = 0;
	
	virtual ECode GetNeedClientAuth(
		/* [out] */ Boolean* need) = 0;
	
	virtual ECode SetWantClientAuth(
		/* [in] */ Boolean want) = 0;
	
	virtual ECode GetWantClientAuth(
		/* [out] */ Boolean* want) = 0;
	
	virtual ECode SetEnableSessionCreation(
		/* [in] */ Boolean flag) = 0;
	
	virtual ECode GetEnableSessionCreation(
		/* [out] */ Boolean* enable) = 0;
	
	ECode GetSSLParameters(
		/* [out] */ ISSLParameters** ppPara);
	
	ECode SetSSLParameters(
		/* [in] */ ISSLParameters* p);
		
};

#endif //__SSLSOCKET_H__