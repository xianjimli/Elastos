
#ifndef __CALERTPROTOCOL_H__
#define __CALERTPROTOCOL_H__

#include "_CAlertProtocol.h"

CarClass(CAlertProtocol)
{
public:
    CARAPI SetRecordProtocol(
        /* [in] */ ISSLRecordProtocol * pRecordProtocol);

    CARAPI Alert(
        /* [in] */ Byte level,
        /* [in] */ Byte description);

    CARAPI GetDescriptionCode(
        /* [out] */ Byte * pCode);

    CARAPI SetProcessed();

    CARAPI HasAlert(
        /* [out] */ Boolean * pAlert);

    CARAPI IsFatalAlert(
        /* [out] */ Boolean * pFatalAlert);

    CARAPI GetAlertDescription(
        /* [out] */ String * pDescription);

    CARAPI Wrap(
        /* [out, callee] */ ArrayOf<Byte> ** ppArray);

    CARAPI Shutdown();

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CALERTPROTOCOL_H__
