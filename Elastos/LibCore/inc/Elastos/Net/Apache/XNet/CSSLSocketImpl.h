
#ifndef __CSSLSOCKETIMPL_H__
#define __CSSLSOCKETIMPL_H__

#include "_CSSLSocketImpl.h"

CarClass(CSSLSocketImpl)
{
public:
    CARAPI GetSupportedCipherSuites(
        /* [out, callee] */ ArrayOf<String> ** ppSuites);

    CARAPI GetEnabledCipherSuites(
        /* [out, callee] */ ArrayOf<String> ** ppSuites);

    CARAPI SetEnabledCipherSuites(
        /* [in] */ const ArrayOf<String> & suites);

    CARAPI GetSupportedProtocols(
        /* [out, callee] */ ArrayOf<String> ** ppProtocols);

    CARAPI GetEnabledProtocols(
        /* [out, callee] */ ArrayOf<String> ** ppProtocols);

    CARAPI SetEnabledProtocols(
        /* [in] */ const ArrayOf<String> & protocols);

    CARAPI GetSession(
        /* [out] */ ISSLSession ** ppSession);

    CARAPI StartHandshake();

    CARAPI SetUseClientMode(
        /* [in] */ Boolean mode);

    CARAPI GetUseClientMode(
        /* [out] */ Boolean * pMode);

    CARAPI SetNeedClientAuth(
        /* [in] */ Boolean need);

    CARAPI GetNeedClientAuth(
        /* [out] */ Boolean * pIsNeed);

    CARAPI GetWantClientAuth(
        /* [out] */ Boolean * pIsWant);

    CARAPI SetEnableSessionCreation(
        /* [in] */ Boolean flag);

    CARAPI GetEnableSessionCreation(
        /* [out] */ Boolean * pIsEnable);

    CARAPI GetSSLParameters(
        /* [out] */ ISSLParameters ** ppParameters);

    CARAPI SetSSLParameters(
        /* [in] */ ISSLParameters * pPara);

    CARAPI constructor(
        /* [in] */ ISSLParametersImpl * pSslParameters);

    CARAPI constructor(
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ ISSLParametersImpl * pSslParameters);

    CARAPI constructor(
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ IInetAddress * pLocalHost,
        /* [in] */ Int32 localPort,
        /* [in] */ ISSLParametersImpl * pSslParameters);

    CARAPI constructor(
        /* [in] */ IInetAddress * pHost,
        /* [in] */ Int32 port,
        /* [in] */ ISSLParametersImpl * pSslParameters);

    CARAPI constructor(
        /* [in] */ IInetAddress * pAddress,
        /* [in] */ Int32 port,
        /* [in] */ IInetAddress * pLocalAddress,
        /* [in] */ Int32 localPort,
        /* [in] */ ISSLParametersImpl * pSslParameters);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSSLSOCKETIMPL_H__
