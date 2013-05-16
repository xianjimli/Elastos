
#ifndef __CSSLCONTEXT_H__
#define __CSSLCONTEXT_H__

#include "_CSSLContext.h"

CarClass(CSSLContext)
{
public:
    CARAPI GetProtocol(
        /* [out] */ String * pProtocol);

    CARAPI GetSocketFactory(
        /* [out] */ ISocketFactory ** ppFactory);

    CARAPI GetServerSocketFactory(
        /* [out] */ IServerSocketFactory ** ppFactory);

    CARAPI CreateSSLEngine(
        /* [out] */ ISSLEngine ** ppEngine);

    CARAPI CreateSSLEngineEx(
        /* [in] */ const String& peerHost,
        /* [in] */ Int32 peerPort,
        /* [out] */ ISSLEngine ** ppEngine);

    CARAPI GetServerSessionContext(
        /* [out] */ ISSLSessionContext ** ppContext);

    CARAPI GetClientSessionContext(
        /* [out] */ ISSLSessionContext ** ppContext);

    CARAPI GetDefaultSSLParameters(
        /* [out] */ ISSLParameters ** ppPara);

    CARAPI GetSupportedSSLParameters(
        /* [out] */ ISSLParameters ** ppPara);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CSSLCONTEXT_H__
