
#ifndef __CCIPHERSUITE_H__
#define __CCIPHERSUITE_H__

#include "_CCipherSuite.h"

CarClass(CCipherSuite)
{
public:
    CARAPI IsAnonymous(
        /* [out] */ Boolean * pIsAnonymous);

    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI ToBytes(
        /* [out, callee] */ ArrayOf<Byte> ** ppArray);

    CARAPI ToString(
        /* [out] */ String * pDescription);

    CARAPI GetBulkEncryptionAlgorithm(
        /* [out] */ String * pName);

    CARAPI GetBlockSize(
        /* [out] */ Int32 * pSize);

    CARAPI GetHmacName(
        /* [out] */ String * pName);

    CARAPI GetHashName(
        /* [out] */ String * pName);

    CARAPI GetMACLength(
        /* [out] */ Int32 * pLen);

    CARAPI IsExportable(
        /* [out] */ Boolean * pIsExportable);

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ Boolean isExportable,
        /* [in] */ Int32 keyExchange,
        /* [in] */ const String& authType,
        /* [in] */ const String& cipherName,
        /* [in] */ const String& hash,
        /* [in] */ const ArrayOf<Byte> & code);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCIPHERSUITE_H__
