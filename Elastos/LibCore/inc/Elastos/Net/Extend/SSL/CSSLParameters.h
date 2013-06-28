
#ifndef __CSSLPARAMETERS_H__
#define __CSSLPARAMETERS_H__

#include "_CSSLParameters.h"

CarClass(CSSLParameters)
{
public:
    CARAPI GetCipherSuites(
        /* [out, callee] */ ArrayOf<String> ** ppSuites);

    CARAPI SetCipherSuites(
        /* [in] */ const ArrayOf<String> & cipherSuites);

    CARAPI GetProtocols(
        /* [out, callee] */ ArrayOf<String> ** ppProtocols);

    CARAPI SetProtocols(
        /* [in] */ const ArrayOf<String> & protocols);

    CARAPI GetNeedClientAuth(
        /* [out] */ Boolean * pIsNeed);

    CARAPI SetNeedClientAuth(
        /* [in] */ Boolean needClientAuth);

    CARAPI GetWantClientAuth(
        /* [out] */ Boolean * pIsWant);

    CARAPI SetWantClientAuth(
        /* [in] */ Boolean wantClientAuth);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const ArrayOf<String> & cipherSuites);

    CARAPI constructor(
        /* [in] */ const ArrayOf<String> & cipherSuites,
        /* [in] */ const ArrayOf<String> & protocols);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSSLPARAMETERS_H__
