#ifndef __SSLENGINE_H__
#define __SSLENGINE_H__

#include <elastos.h>
#include "Elastos.Net_server.h"
#include <elastos/AutoPtr.h>
#include "CSSLParameters.h"

class SSLEngine
{
protected:
	SSLEngine();

    SSLEngine(
    	/* [in] */ const String& host, 
    	/* [in] */ Int32 port);
	
public:
	ECode GetPeerHost(
		/* [out] */ String* host);

	ECode GetPeerPort(
		/* [out] */ Int32* port);
	
	virtual ECode BeginHandshake() = 0;
	
	virtual ECode CloseInbound() = 0;
	
	virtual ECode CloseOutbound() = 0;
	
	virtual ECode GetDelegatedTask(
		/* [out] */ IRunnable** ppRunable) = 0;
		
	virtual ECode GetEnabledCipherSuites(
		/* [out, callee] */ ArrayOf<String>** array) = 0;
	
	virtual ECode GetEnabledProtocols(
		/* [out, callee] */ ArrayOf<String>** array) = 0;
	
	virtual ECode GetEnableSessionCreation(
		/* [out] */ Boolean* pEnable) = 0;
		
	virtual ECode GetHandshakeStatus(
		/* [out] */ HandshakeStatus* ppStatus) = 0;
			
	virtual ECode GetNeedClientAuth(
		/* [out] */ Boolean* pNeed) = 0;

    virtual ECode GetSession(
    	/* [out] */ ISSLSession** ppSession) = 0;
    	
	virtual ECode GetSupportedCipherSuites(
		/* [out, callee] */ ArrayOf<String>** array) = 0;
	
	virtual ECode GetSupportedProtocols(
		/* [out, callee] */ ArrayOf<String>** array) = 0;
	
	virtual ECode GetUseClientMode(
		/* [out] */ Boolean* pUsed) = 0;
	
	virtual ECode GetWantClientAuth(
		/* [out] */ Boolean* pWant) = 0;
	 
	virtual ECode IsInboundDone(
		/* [out] */ Boolean* pIs) = 0;
	 
	virtual ECode IsOutboundDone(
		/* [out] */ Boolean* pIs) = 0;
	 
	virtual ECode SetEnabledCipherSuites(
		/* [in] */ const ArrayOf<String>& suites) = 0; 
	
	virtual ECode SetEnabledProtocols(
		/* [in] */ const ArrayOf<String>& protocols) = 0;
	
	virtual ECode SetEnableSessionCreation(
		/* [in] */ Boolean flag) = 0;
	
	virtual ECode SetNeedClientAuth(
		/* [in] */ Boolean need) = 0;
	
	virtual ECode SetUseClientMode(
		/* [in] */ Boolean mode) = 0;	

	virtual ECode SetWantClientAuth(
		/* [in] */ Boolean want) = 0;

	virtual ECode Unwrap(
		/* [in] */ const ArrayOf<IByteBuffer*>& srcs,
		/* [in] */ const ArrayOf<IByteBuffer*>& dsts,
		/* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ISSLEngineResult** ppResult) = 0;

	virtual ECode Wrap(
		/* [in] */ const ArrayOf<IByteBuffer*>& srcs, 
		/* [in] */ Int32 offset, 
		/* [in] */ Int32 length, 
		/* [in] */ IByteBuffer* dst,
		/* [out] */ ISSLEngineResult** ppResult) = 0;
	
	ECode UnwrapEx(
		/* [in] */ IByteBuffer* src, 
		/* [in] */ IByteBuffer* dst,
		/* [out] */ ISSLEngineResult** ppResult);
	
	ECode UnwrapEx2(
		/* [in] */ IByteBuffer* src, 
		/* [in] */ const ArrayOf<IByteBuffer*>& dsts,
		/* [out] */ ISSLEngineResult** ppResult);
	
	ECode WrapEx(
		/* [in] */ const ArrayOf<IByteBuffer*>& srcs, 
		/* [in] */ IByteBuffer* dst,
		/* [out] */ ISSLEngineResult** ppResult);
		
	ECode WrapEx2(
		/* [in] */ IByteBuffer* src, 
		/* [in] */ IByteBuffer* dst,
		/* [out] */ ISSLEngineResult** ppResult);

	ECode GetSSLParameters(
		/* [out] */ ISSLParameters** ppPara);

	ECode SetSSLParameters(
		/* [in] */ ISSLParameters* p);
				
private:
	String mPeerHost;
    Int32 mPeerPort;	
};

#endif //__SSLENGINE_H__