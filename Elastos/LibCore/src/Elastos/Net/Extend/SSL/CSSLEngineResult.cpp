
#include "CSSLEngineResult.h"
ECode CSSLEngineResult::GetStatus(
    /* [out] */ Status * pStatus)
{
    // TODO: Add your code here
    *pStatus = mStatus;
    return NOERROR;
}

ECode CSSLEngineResult::GetHandshakeStatus(
    /* [out] */ HandshakeStatus * pStatus)
{
    // TODO: Add your code here
    *pStatus = mHandshakeStatus;
    return NOERROR;
}

ECode CSSLEngineResult::BytesConsumed(
    /* [out] */ Int32 * pNumber)
{
    // TODO: Add your code here
    *pNumber = mBytesConsumed;
    return NOERROR;
}

ECode CSSLEngineResult::BytesProduced(
    /* [out] */ Int32 * pNumber)
{
    // TODO: Add your code here
    *pNumber = mBytesProduced;
    return NOERROR;
}

ECode CSSLEngineResult::ToString(
    /* [out] */ String * pStr)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSSLEngineResult::constructor(
    /* [in] */ Status status,
    /* [in] */ HandshakeStatus handshakeStatus,
    /* [in] */ Int32 bytesConsumed,
    /* [in] */ Int32 bytesProduced)
{
    // TODO: Add your code here
    if (bytesConsumed < 0) {
    	return E_INVALID_ARGUMENT;
    }
    if (bytesProduced < 0) {
    	return E_INVALID_ARGUMENT;
    }
    
    mStatus = status;
    mHandshakeStatus = handshakeStatus;
    mBytesConsumed = bytesConsumed;
    mBytesProduced = bytesProduced;    
    return NOERROR;
}

