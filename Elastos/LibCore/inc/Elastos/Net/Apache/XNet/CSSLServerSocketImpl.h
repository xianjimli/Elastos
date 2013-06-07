
#ifndef __CSSLSERVERSOCKETIMPL_H__
#define __CSSLSERVERSOCKETIMPL_H__

#include "_CSSLServerSocketImpl.h"

CarClass(CSSLServerSocketImpl)
{
public:
    CARAPI Accept(
        /* [out] */ ISocket ** ppSocket);

    CARAPI Close();

    CARAPI GetInetAddress(
        /* [out] */ IInetAddress ** ppAddress);

    CARAPI GetLocalPort(
        /* [out] */ Int32 * pPort);

    CARAPI GetSoTimeout(
        /* [out] */ Int32 * pTimeout);

    CARAPI SetSoTimeout(
        /* [in] */ Int32 timeout);

    CARAPI Bind(
        /* [in] */ ISocketAddress * pLocalAddr);

    CARAPI BindEx(
        /* [in] */ ISocketAddress * pLocalAddr,
        /* [in] */ Int32 backlog);

    CARAPI GetLocalSocketAddress(
        /* [out] */ ISocketAddress ** ppAddress);

    CARAPI IsBound(
        /* [out] */ Boolean * pIsBound);

    CARAPI IsClosed(
        /* [out] */ Boolean * pIsClosed);

    CARAPI SetReuseAddress(
        /* [in] */ Boolean reuse);

    CARAPI GetReuseAddress(
        /* [out] */ Boolean * pReuse);

    CARAPI SetReceiveBufferSize(
        /* [in] */ Int32 size);

    CARAPI GetReceiveBufferSize(
        /* [out] */ Int32 * pSize);

    CARAPI GetChannel(
        /* [out] */ IServerSocketChannel ** ppChannel);

    CARAPI SetPerformancePreferences(
        /* [in] */ Int32 connectionTime,
        /* [in] */ Int32 latency,
        /* [in] */ Int32 bandwidth);

    CARAPI constructor(
        /* [in] */ ISSLParametersImpl * pSslParameters);

    CARAPI constructor(
        /* [in] */ Int32 port,
        /* [in] */ ISSLParametersImpl * pSslParameters);

    CARAPI constructor(
        /* [in] */ Int32 port,
        /* [in] */ Int32 backlog,
        /* [in] */ ISSLParametersImpl * pSslParameters);

    CARAPI constructor(
        /* [in] */ Int32 port,
        /* [in] */ Int32 backlog,
        /* [in] */ IInetAddress * pIAddress,
        /* [in] */ ISSLParametersImpl * pSslParameters);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSSLSERVERSOCKETIMPL_H__
