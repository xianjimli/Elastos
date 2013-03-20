
#include "CICUCollationKey.h"
ECode CICUCollationKey::constructor(
    /* [in] */ const String& source,
    /* [in] */ ArrayOf<Byte> * pBytes)
{
    return ICUCollationKey::Init(source, pBytes);
}

ECode CICUCollationKey::CompareTo(
    /* [in] */ ICollationKey * pValue,
    /* [out] */ Int32 * pResult)
{
    return ICUCollationKey::CompareTo(pValue, pResult);
}

ECode CICUCollationKey::GetSourceString(
    /* [out] */ String * pSourceString)
{
    return ICUCollationKey::GetSourceString(pSourceString);
}

ECode CICUCollationKey::ToByteArray(
    /* [out, callee] */ ArrayOf<Byte> ** ppArray)
{
    return ICUCollationKey::ToByteArray(ppArray);
}

ECode CICUCollationKey::Equals(
    /* [in] */ IInterface * pObject,
    /* [out] */ Boolean * pResult)
{
    return ICUCollationKey::Equals(pObject, pResult);
}

ECode CICUCollationKey::HashCode(
    /* [out] */ Int32 * pValue)
{
    return ICUCollationKey::HashCode(pValue);
}



