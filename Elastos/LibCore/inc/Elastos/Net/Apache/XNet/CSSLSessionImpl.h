
#ifndef __CSSLSESSIONIMPL_H__
#define __CSSLSESSIONIMPL_H__

#include "_CSSLSessionImpl.h"

CarClass(CSSLSessionImpl)
{
public:
    CARAPI GetApplicationBufferSize(
        /* [out] */ Int32 * pSize);

    CARAPI GetCipherSuite(
        /* [out] */ String * pStr);

    CARAPI GetCreationTime(
        /* [out] */ Int64 * pTime);

    CARAPI GetId(
        /* [out, callee] */ ArrayOf<Byte> ** ppId);

    CARAPI GetLastAccessedTime(
        /* [out] */ Int64 * pTime);

    CARAPI GetLocalCertificates(
        /* [out, callee] */ ArrayOf<ICertificate *> ** ppCertificate);

    CARAPI GetPacketBufferSize(
        /* [out] */ Int32 * pSize);

    CARAPI GetPeerCertificates(
        /* [out, callee] */ ArrayOf<ICertificate *> ** ppCertificate);

    CARAPI GetPeerHost(
        /* [out] */ String * pStrHost);

    CARAPI GetPeerPort(
        /* [out] */ Int32 * pPort);

    CARAPI GetPeerPrincipal(
        /* [out] */ IPrincipal ** ppPrincipal);

    CARAPI GetProtocol(
        /* [out] */ String * pStrProtocol);

    CARAPI GetSessionContext(
        /* [out] */ ISSLSessionContext ** ppContext);

    CARAPI GetValue(
        /* [in] */ const String& name,
        /* [out] */ IInterface ** ppObj);

    CARAPI GetValueNames(
        /* [out, callee] */ ArrayOf<String> ** ppNames);

    CARAPI Invalidate();

    CARAPI IsValid(
        /* [out] */ Boolean * pIsValid);

    CARAPI PutValue(
        /* [in] */ const String& name,
        /* [in] */ IInterface * pValue);

    CARAPI RemoveValue(
        /* [in] */ const String& name);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSSLSESSIONIMPL_H__
