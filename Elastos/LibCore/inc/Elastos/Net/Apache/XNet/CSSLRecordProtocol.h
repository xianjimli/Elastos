
#ifndef __CSSLRECORDPROTOCOL_H__
#define __CSSLRECORDPROTOCOL_H__

#include "_CSSLRecordProtocol.h"

CarClass(CSSLRecordProtocol)
{
public:
    CARAPI GetSession(
        /* [out] */ ISSLSessionImpl ** ppSession);

    CARAPI GetMinRecordSize(
        /* [out] */ Int32 * pSize);

    CARAPI GetRecordSize(
        /* [in] */ Int32 data_size,
        /* [out] */ Int32 * pSize);

    CARAPI GetDataSize(
        /* [in] */ Int32 record_size,
        /* [out] */ Int32 * pSize);

    CARAPI Wrap(
        /* [in] */ Byte content_type,
        /* [in] */ IDataStream * pDataStream,
        /* [out, callee] */ ArrayOf<Byte> ** ppArray);

    CARAPI WrapEx(
        /* [in] */ Byte content_type,
        /* [in] */ const ArrayOf<Byte> & fragment,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 len,
        /* [out, callee] */ ArrayOf<Byte> ** ppArray);

    CARAPI GetChangeCipherSpecMesage(
        /* [in] */ ISSLSessionImpl * pSession,
        /* [out, callee] */ ArrayOf<Byte> ** ppArray);

    CARAPI Unwrap(
        /* [out] */ Int32 * pWrap);

    CARAPI Alert(
        /* [in] */ Byte level,
        /* [in] */ Byte description);

    CARAPI SetVersion(
        /* [in] */ const ArrayOf<Byte> & ver);

    CARAPI Shutdown();

    CARAPI constructor(
        /* [in] */ IHandshakeProtocol * pHandshakeProtocol,
        /* [in] */ IAlertProtocol * pAlertProtocol,
        /* [in] */ ISSLInputStream * pStream,
        /* [in] */ IAppendable * pAppData);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSSLRECORDPROTOCOL_H__
