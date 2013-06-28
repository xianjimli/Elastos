
#ifndef __CHANDSHAKECOMPLETEDEVENT_H__
#define __CHANDSHAKECOMPLETEDEVENT_H__

#include "_CHandshakeCompletedEvent.h"

CarClass(CHandshakeCompletedEvent)
{
public:
    CARAPI GetSession(
        /* [out] */ ISSLSession ** ppSession);

    CARAPI GetCipherSuite(
        /* [out] */ String * pCipher);

    CARAPI GetLocalCertificates(
        /* [out, callee] */ ArrayOf<ICertificate *> ** ppArray);

    CARAPI GetPeerCertificates(
        /* [out, callee] */ ArrayOf<ICertificate *> ** ppArray);

    CARAPI GetPeerPrincipal(
        /* [out] */ IPrincipal ** ppPrincipal);

    CARAPI GetLocalPrincipal(
        /* [out] */ IPrincipal ** ppPrincipal);

    CARAPI GetSocket(
        /* [out] */ ISSLSocket ** ppSslSocket);

    CARAPI constructor(
        /* [in] */ ISSLSocket * pSock,
        /* [in] */ ISSLSession * pS);

private:
    // TODO: Add your private member variables here.
};

#endif // __CHANDSHAKECOMPLETEDEVENT_H__
