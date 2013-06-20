
#include "CSSLRecordProtocol.h"
ECode CSSLRecordProtocol::GetSession(
    /* [out] */ ISSLSessionImpl ** ppSession)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSSLRecordProtocol::GetMinRecordSize(
    /* [out] */ Int32 * pSize)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSSLRecordProtocol::GetRecordSize(
    /* [in] */ Int32 data_size,
    /* [out] */ Int32 * pSize)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSSLRecordProtocol::GetDataSize(
    /* [in] */ Int32 record_size,
    /* [out] */ Int32 * pSize)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSSLRecordProtocol::Wrap(
    /* [in] */ Byte content_type,
    /* [in] */ IDataStream * pDataStream,
    /* [out, callee] */ ArrayOf<Byte> ** ppArray)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSSLRecordProtocol::WrapEx(
    /* [in] */ Byte content_type,
    /* [in] */ const ArrayOf<Byte> & fragment,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 len,
    /* [out, callee] */ ArrayOf<Byte> ** ppArray)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSSLRecordProtocol::GetChangeCipherSpecMesage(
    /* [in] */ ISSLSessionImpl * pSession,
    /* [out, callee] */ ArrayOf<Byte> ** ppArray)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSSLRecordProtocol::Unwrap(
    /* [out] */ Int32 * pWrap)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSSLRecordProtocol::Alert(
    /* [in] */ Byte level,
    /* [in] */ Byte description)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSSLRecordProtocol::SetVersion(
    /* [in] */ const ArrayOf<Byte> & ver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSSLRecordProtocol::Shutdown()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSSLRecordProtocol::constructor(
    /* [in] */ IHandshakeProtocol * pHandshakeProtocol,
    /* [in] */ IAlertProtocol * pAlertProtocol,
    /* [in] */ ISSLInputStream * pStream,
    /* [in] */ IAppendable * pAppData)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

