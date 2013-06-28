
#ifndef __CSSLENGINERESULT_H__
#define __CSSLENGINERESULT_H__

#include "_CSSLEngineResult.h"

CarClass(CSSLEngineResult)
{
public:
    CARAPI GetStatus(
        /* [out] */ Status * pStatus);

    CARAPI GetHandshakeStatus(
        /* [out] */ HandshakeStatus * pStatus);

    CARAPI BytesConsumed(
        /* [out] */ Int32 * pNumber);

    CARAPI BytesProduced(
        /* [out] */ Int32 * pNumber);

    CARAPI ToString(
        /* [out] */ String * pStr);

    CARAPI constructor(
        /* [in] */ Status status,
        /* [in] */ HandshakeStatus handshakeStatus,
        /* [in] */ Int32 bytesConsumed,
        /* [in] */ Int32 bytesProduced);

private:
    // TODO: Add your private member variables here.
    // Store Status object
    Status mStatus;

    // Store HandshakeStatus object
    HandshakeStatus mHandshakeStatus;

    // Store bytesConsumed
    Int32 mBytesConsumed;

    // Store bytesProduced
    Int32 mBytesProduced;    
};

#endif // __CSSLENGINERESULT_H__
