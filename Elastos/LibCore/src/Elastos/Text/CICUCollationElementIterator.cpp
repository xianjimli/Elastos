
#include "CICUCollationElementIterator.h"
ECode CICUCollationElementIterator::constructor(
    /* [in] */ Int32 collelemiteratoraddress)
{
    return ICUCollationElementIterator::Init(collelemiteratoraddress);
}

ECode CICUCollationElementIterator::Reset()
{
    return ICUCollationElementIterator::Reset();
}

ECode CICUCollationElementIterator::Next(
    /* [out] */ Int32 * pNextValue)
{
    return ICUCollationElementIterator::Next(pNextValue);
}

ECode CICUCollationElementIterator::Previous(
    /* [out] */ Int32 * pPreviousValue)
{
    return ICUCollationElementIterator::Previous(pPreviousValue);
}

ECode CICUCollationElementIterator::GetMaxExpansion(
    /* [in] */ Int32 order,
    /* [out] */ Int32 * pMaxExpansion)
{
    return ICUCollationElementIterator::GetMaxExpansion(order, pMaxExpansion);
}

ECode CICUCollationElementIterator::SetText(
    /* [in] */ const String& source)
{
    return ICUCollationElementIterator::SetText(source);
}

ECode CICUCollationElementIterator::SetTextEx(
    /* [in] */ ICharacterIterator * pSource)
{
    return ICUCollationElementIterator::SetTextEx(pSource);
}

ECode CICUCollationElementIterator::GetOffset(
    /* [out] */ Int32 * pOffset)
{
    return ICUCollationElementIterator::GetOffset(pOffset);
}

ECode CICUCollationElementIterator::SetOffset(
    /* [in] */ Int32 offset)
{
    return ICUCollationElementIterator::SetOffset(offset);
}



