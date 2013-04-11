
#ifndef __CRULEBASEDCOLLATOR_H__
#define __CRULEBASEDCOLLATOR_H__

#include "_CRuleBasedCollator.h"
#include "RuleBasedCollator.h"

CarClass(CRuleBasedCollator), public RuleBasedCollator
{
public:

    CARAPI constructor(
        /* [in] */ IICUCollator * pWrapper);

    CARAPI constructor(
        /* [in] */ const String& rules);

    CARAPI Compare(
        /* [in] */ IInterface * pObject1,
        /* [in] */ IInterface * pObject2,
        /* [out] */ Int32 * pResult);

    CARAPI CompareEx(
        /* [in] */ const String& string1,
        /* [in] */ const String& string2,
        /* [out] */ Int32 * pValue);

    CARAPI Equals(
        /* [in] */ const String& string1,
        /* [in] */ const String& string2,
        /* [out] */ Boolean * pRecult);

    CARAPI GetCollationKey(
        /* [in] */ const String& string,
        /* [out] */ IICUCollationKey ** ppCollationKey);

    CARAPI GetDecomposition(
        /* [out] */ Int32 * pDecomposition);

    CARAPI GetStrength(
        /* [out] */ Int32 * pStrength);

    CARAPI SetDecomposition(
        /* [in] */ Int32 value);

    CARAPI SetStrength(
        /* [in] */ Int32 value);

    CARAPI GetCollationElementIteratorEx(
        /* [in] */ ICharacterIterator * pSource,
        /* [out] */ ICollationElementIterator ** ppCollationElementIterator);

    CARAPI GetCollationElementIterator(
        /* [in] */ const String& source,
        /* [out] */ ICollationElementIterator ** ppCollationElementIterator);

    CARAPI GetRules(
        /* [out] */ String * pRules);


private:
    // TODO: Add your private member variables here.
};

#endif // __CRULEBASEDCOLLATOR_H__
