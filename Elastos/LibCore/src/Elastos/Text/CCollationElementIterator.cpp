
#include "CCollationElementIterator.h"
ECode CCollationElementIterator::constructor(
    /* [in] */ IICUCollationElementIterator * pIterator)
{
    return CollationElementIterator::Init(pIterator);
}

ECode CCollationElementIterator::GetMaxExpansion(
    /* [in] */ Int32 order,
    /* [out] */ Int32 * pMaxExpansion)
{
    return CollationElementIterator::GetMaxExpansion(order, pMaxExpansion);
}

ECode CCollationElementIterator::GetOffset(
    /* [out] */ Int32 * pOffset)
{
    return CollationElementIterator::GetOffset(pOffset);
}

ECode CCollationElementIterator::Next(
    /* [out] */ Int32 * pNextValue)
{
    return CollationElementIterator::Next(pNextValue);
}

ECode CCollationElementIterator::Previous(
    /* [out] */ Int32 * pPreviousValue)
{
    return CollationElementIterator::Previous(pPreviousValue);
}

ECode CCollationElementIterator::Reset()
{
    return CollationElementIterator::Reset();
}

ECode CCollationElementIterator::SetOffset(
    /* [in] */ Int32 newOffset)
{
    return CollationElementIterator::SetOffset(newOffset);
}

ECode CCollationElementIterator::SetTextEx(
    /* [in] */ ICharacterIterator * pSource)
{
    return CollationElementIterator::SetTextEx(pSource);
}

ECode CCollationElementIterator::SetText(
    /* [in] */ const String& source)
{
    return CollationElementIterator::SetText(source);
}



