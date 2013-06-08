
#ifndef __CSSLSOCKETFACTORYIMPL_H__
#define __CSSLSOCKETFACTORYIMPL_H__

#include "_CSSLSocketFactoryImpl.h"

CarClass(CSSLSocketFactoryImpl)
{
public:
    CARAPI CreateSocket(
        /* [out] */ ISocket ** ppSocket);

    CARAPI CreateSocketEx(
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [out] */ ISocket ** ppSocket);

    CARAPI CreateSocketEx2(
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ IInetAddress * pLocalHost,
        /* [in] */ Int32 localPort,
        /* [out] */ ISocket ** ppSocket);

    CARAPI CreateSocketEx3(
        /* [in] */ IInetAddress * pHost,
        /* [in] */ Int32 port,
        /* [out] */ ISocket ** ppSocket);

    CARAPI CreateSocketEx4(
        /* [in] */ IInetAddress * pAddress,
        /* [in] */ Int32 port,
        /* [in] */ IInetAddress * pLocalAddress,
        /* [in] */ Int32 localPort,
        /* [out] */ ISocket ** ppSocket);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ISSLParametersImpl * pSslParameters);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSSLSOCKETFACTORYIMPL_H__
