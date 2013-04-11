
#include "CRuleBasedCollator.h"

ECode CRuleBasedCollator::constructor(
    /* [in] */ IICUCollator * pWrapper)
{
    return RuleBasedCollator::Init(pWrapper);
}

ECode CRuleBasedCollator::constructor(
    /* [in] */ const String& rules)
{
    return RuleBasedCollator::Init(rules);
}

ECode CRuleBasedCollator::Compare(
    /* [in] */ IInterface * pObject1,
    /* [in] */ IInterface * pObject2,
    /* [out] */ Int32 * pResult)
{
    return RuleBasedCollator::Compare(pObject1, pObject2, pResult);
}

ECode CRuleBasedCollator::CompareEx(
    /* [in] */ const String& string1,
    /* [in] */ const String& string2,
    /* [out] */ Int32 * pValue)
{
    return RuleBasedCollator::CompareEx(string1, string2, pValue);
}

ECode CRuleBasedCollator::Equals(
    /* [in] */ const String& string1,
    /* [in] */ const String& string2,
    /* [out] */ Boolean * pRecult)
{
    return RuleBasedCollator::Equals(string1, string2, pRecult);
}

ECode CRuleBasedCollator::GetCollationKey(
    /* [in] */ const String& string,
    /* [out] */ IICUCollationKey ** ppCollationKey)
{
    return RuleBasedCollator::GetCollationKey(string, ppCollationKey);
}

ECode CRuleBasedCollator::GetDecomposition(
    /* [out] */ Int32 * pDecomposition)
{
    return RuleBasedCollator::GetDecomposition(pDecomposition);
}

ECode CRuleBasedCollator::GetStrength(
    /* [out] */ Int32 * pStrength)
{
    return RuleBasedCollator::GetStrength(pStrength);
}

ECode CRuleBasedCollator::SetDecomposition(
    /* [in] */ Int32 value)
{
    return RuleBasedCollator::SetDecomposition(value);
}

ECode CRuleBasedCollator::SetStrength(
    /* [in] */ Int32 value)
{
    return RuleBasedCollator::SetStrength(value);
}

ECode CRuleBasedCollator::GetCollationElementIteratorEx(
    /* [in] */ ICharacterIterator * pSource,
    /* [out] */ ICollationElementIterator ** ppCollationElementIterator)
{
    return RuleBasedCollator::GetCollationElementIteratorEx(pSource, ppCollationElementIterator);
}

ECode CRuleBasedCollator::GetCollationElementIterator(
    /* [in] */ const String& source,
    /* [out] */ ICollationElementIterator ** ppCollationElementIterator)
{
    return RuleBasedCollator::GetCollationElementIterator(source, ppCollationElementIterator);
}

ECode CRuleBasedCollator::GetRules(
    /* [out] */ String * pRules)
{
    return RuleBasedCollator::GetRules(pRules);
}


