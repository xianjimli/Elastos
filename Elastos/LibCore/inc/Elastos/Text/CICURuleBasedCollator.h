
#ifndef __CICURULEBASEDCOLLATOR_H__
#define __CICURULEBASEDCOLLATOR_H__

#include "_CICURuleBasedCollator.h"
#include "ICURuleBasedCollator.h"

CarClass(CICURuleBasedCollator), public ICURuleBasedCollator
{
public:
    CARAPI constructor(
        /* [in] */ const String& rules);

    CARAPI constructor(
        /* [in] */ const String& rules,
        /* [in] */ Int32 strength);

    CARAPI constructor(
        /* [in] */ const String& rules,
        /* [in] */ Int32 normalizationMode,
        /* [in] */ Int32 strength);

    CARAPI constructor(
        /* [in] */ ILocale * pLocale);

    CARAPI Equals(
        /* [in] */ const String& source,
        /* [in] */ const String& target,
        /* [out] */ Boolean * pResult);

    CARAPI EqualsEx(
        /* [in] */ IInterface * pTarget,
        /* [out] */ Boolean * pResult);

    CARAPI Clone(
        /* [out] */ IInterface ** ppInstance);

    CARAPI Compare(
        /* [in] */ const String& source,
        /* [in] */ const String& target,
        /* [out] */ Int32 * pValue);

    CARAPI GetDecomposition(
        /* [out] */ Int32 * pDecomposition);

    CARAPI SetDecomposition(
        /* [in] */ Int32 mode);

    CARAPI GetStrength(
        /* [out] */ Int32 * pStrength);

    CARAPI GetAttribute(
        /* [in] */ Int32 type,
        /* [out] */ Int32 * pAttribute);

    CARAPI SetStrength(
        /* [in] */ Int32 strength);

    CARAPI SetAttribute(
        /* [in] */ Int32 type,
        /* [in] */ Int32 value);

    CARAPI GetCollationKey(
        /* [in] */ const String& source,
        /* [out] */ IICUCollationKey ** ppInstance);

    CARAPI HashCode(
        /* [out] */ Int32 * pValue);

    CARAPI GetRules(
        /* [out] */ String * pRules);

    CARAPI GetCollationElementIterator(
        /* [in] */ const String& source,
        /* [out] */ IICUCollationElementIterator ** ppCollationElementIterator);

    CARAPI GetCollationElementIteratorEx(
        /* [in] */ ICharacterIterator * pIt,
        /* [out] */ IICUCollationElementIterator ** ppCollationElementIterator);


private:
    // TODO: Add your private member variables here.
};

#endif // __CICURULEBASEDCOLLATOR_H__
