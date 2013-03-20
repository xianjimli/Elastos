
#ifndef __CICUCOLLATIONKEY_H__
#define __CICUCOLLATIONKEY_H__

#include "_CICUCollationKey.h"
#include "ICUCollationKey.h"

CarClass(CICUCollationKey), public ICUCollationKey
{
public:
    CARAPI constructor(
        /* [in] */ const String& source,
        /* [in] */ ArrayOf<Byte> * pBytes);

    CARAPI CompareTo(
        /* [in] */ ICollationKey * pValue,
        /* [out] */ Int32 * pResult);

    CARAPI GetSourceString(
        /* [out] */ String * pSourceString);

    CARAPI ToByteArray(
        /* [out, callee] */ ArrayOf<Byte> ** ppArray);

    CARAPI Equals(
        /* [in] */ IInterface * pObject,
        /* [out] */ Boolean * pResult);

    CARAPI HashCode(
        /* [out] */ Int32 * pValue);

private:
    // TODO: Add your private member variables here.
};

#endif // __CICUCOLLATIONKEY_H__
