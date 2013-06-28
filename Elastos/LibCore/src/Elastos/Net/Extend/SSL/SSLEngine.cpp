#include "SSLEngine.h"


SSLEngine::SSLEngine()
{
}

SSLEngine::SSLEngine(
	/* [in] */ const String& host, 
	/* [in] */ Int32 port)
{
}


ECode SSLEngine::GetPeerHost(
	/* [out] */ String* host)
{
	*host = mPeerHost;
	return NOERROR;
}

ECode SSLEngine::GetPeerPort(
	/* [out] */ Int32* port)
{
	*port = mPeerPort;
	return NOERROR;
}

ECode SSLEngine::UnwrapEx(
	/* [in] */ IByteBuffer* src, 
	/* [in] */ IByteBuffer* dst,
	/* [out] */ ISSLEngineResult** ppResult)
{
	return E_NOT_IMPLEMENTED;
}

ECode SSLEngine::UnwrapEx2(
	/* [in] */ IByteBuffer* src, 
	/* [in] */ const ArrayOf<IByteBuffer*>& dsts,
	/* [out] */ ISSLEngineResult** ppResult)
{
	return E_NOT_IMPLEMENTED;
}

ECode SSLEngine::WrapEx(
	/* [in] */ const ArrayOf<IByteBuffer*>& srcs, 
	/* [in] */ IByteBuffer* dst,
	/* [out] */ ISSLEngineResult** ppResult)
{
	return E_NOT_IMPLEMENTED;	
}
	
ECode SSLEngine::WrapEx2(
	/* [in] */ IByteBuffer* src, 
	/* [in] */ IByteBuffer* dst,
	/* [out] */ ISSLEngineResult** ppResult)
{
	return E_NOT_IMPLEMENTED;	
}

ECode SSLEngine::GetSSLParameters(
	/* [out] */ ISSLParameters** ppPara)
{
	ISSLParameters* p; 
	CSSLParameters::New(&p);

	ArrayOf<String>* array;
	GetEnabledCipherSuites(&array);
	p->SetCipherSuites(*array);

	GetEnabledProtocols(&array);
	p->SetProtocols(*array);

	Boolean need;
	GetNeedClientAuth(&need);
	p->SetNeedClientAuth(need);
	
	Boolean want;
	GetWantClientAuth(&want);
	p->SetWantClientAuth(want);
	
	*ppPara = p;
	return NOERROR;	
}

ECode SSLEngine::SetSSLParameters(
	/* [in] */ ISSLParameters* p)
{
	ArrayOf<String>* cipherSuites;
	p->GetCipherSuites(&cipherSuites);
	
	if (cipherSuites != NULL) {
	    SetEnabledCipherSuites(*cipherSuites);
	}
	
	ArrayOf<String>* protocols;
	p->GetProtocols(&protocols);
	
	if (protocols != NULL) {
	    SetEnabledProtocols(*protocols);
	}
	
	Boolean need;
	p->GetNeedClientAuth(&need);
	Boolean want;
	p->GetWantClientAuth(&want);
	if (need) {
	    SetNeedClientAuth(TRUE);
	} else if (want) {
	    SetWantClientAuth(TRUE);
	} else {
	    SetWantClientAuth(FALSE);
	}	
	
	return NOERROR;
}	
	
	