
#include "CICURuleBasedCollator.h"

ECode CICURuleBasedCollator::constructor(
    /* [in] */ const String& rules)
{
    return ICURuleBasedCollator::Init(rules);
}

ECode CICURuleBasedCollator::constructor(
    /* [in] */ const String& rules,
    /* [in] */ Int32 strength)
{
    return ICURuleBasedCollator::Init(rules, strength);
}

ECode CICURuleBasedCollator::constructor(
    /* [in] */ const String& rules,
    /* [in] */ Int32 normalizationMode,
    /* [in] */ Int32 strength)
{
    return ICURuleBasedCollator::Init(rules, normalizationMode, strength);
}

ECode CICURuleBasedCollator::constructor(
    /* [in] */ ILocale * pLocale)
{
    return ICURuleBasedCollator::Init(pLocale);
}

ECode CICURuleBasedCollator::Equals(
    /* [in] */ const String& source,
    /* [in] */ const String& target,
    /* [out] */ Boolean * pResult)
{
    return ICURuleBasedCollator::Equals(source, target, pResult);
}

ECode CICURuleBasedCollator::EqualsEx(
    /* [in] */ IInterface * pTarget,
    /* [out] */ Boolean * pResult)
{
    return ICURuleBasedCollator::EqualsEx(pTarget, pResult);
}

ECode CICURuleBasedCollator::Clone(
    /* [out] */ IInterface ** ppInstance)
{
    return ICURuleBasedCollator::Clone(ppInstance);
}

ECode CICURuleBasedCollator::Compare(
    /* [in] */ const String& source,
    /* [in] */ const String& target,
    /* [out] */ Int32 * pValue)
{
    return ICURuleBasedCollator::Compare(source, target, pValue);
}

ECode CICURuleBasedCollator::GetDecomposition(
    /* [out] */ Int32 * pDecomposition)
{
    return ICURuleBasedCollator::GetDecomposition(pDecomposition);
}

ECode CICURuleBasedCollator::SetDecomposition(
    /* [in] */ Int32 mode)
{
    return ICURuleBasedCollator::SetDecomposition(mode);
}

ECode CICURuleBasedCollator::GetStrength(
    /* [out] */ Int32 * pStrength)
{
    return ICURuleBasedCollator::GetStrength(pStrength);
}

ECode CICURuleBasedCollator::GetAttribute(
    /* [in] */ Int32 type,
    /* [out] */ Int32 * pAttribute)
{
    return ICURuleBasedCollator::GetAttribute(type, pAttribute);
}

ECode CICURuleBasedCollator::SetStrength(
    /* [in] */ Int32 strength)
{
    return ICURuleBasedCollator::SetStrength(strength);
}

ECode CICURuleBasedCollator::SetAttribute(
    /* [in] */ Int32 type,
    /* [in] */ Int32 value)
{
    return ICURuleBasedCollator::SetAttribute(type, value);
}

ECode CICURuleBasedCollator::GetCollationKey(
    /* [in] */ const String& source,
    /* [out] */ IICUCollationKey ** ppInstance)
{
    return ICURuleBasedCollator::GetCollationKey(source, ppInstance);
}

ECode CICURuleBasedCollator::HashCode(
    /* [out] */ Int32 * pValue)
{
    return E_NOT_IMPLEMENTED;
}

ECode CICURuleBasedCollator::GetRules(
    /* [out] */ String * pRules)
{
    return ICURuleBasedCollator::GetRules(pRules);
}

ECode CICURuleBasedCollator::GetCollationElementIterator(
    /* [in] */ const String& source,
    /* [out] */ IICUCollationElementIterator ** ppCollationElementIterator)
{
    return ICURuleBasedCollator::GetCollationElementIterator(source, ppCollationElementIterator);
}

ECode CICURuleBasedCollator::GetCollationElementIteratorEx(
    /* [in] */ ICharacterIterator * pIt,
    /* [out] */ IICUCollationElementIterator ** ppCollationElementIterator)
{
    return ICURuleBasedCollator::GetCollationElementIteratorEx(pIt, ppCollationElementIterator);
}


