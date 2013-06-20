
#include "CCollationElementIterator.h"

ECode CCollationElementIterator::constructor(
    /* [in] */ IICUCollationElementIterator* iterator)
{
    mIcuIterator = iterator;
    return NOERROR;
}

ECode CCollationElementIterator::GetMaxExpansion(
    /* [in] */ Int32 order,
    /* [out] */ Int32* maxExpansion)
{
    return mIcuIterator->GetMaxExpansion(order, maxExpansion);
}

ECode CCollationElementIterator::GetOffset(
    /* [out] */ Int32* offset)
{
    return mIcuIterator->GetOffset(offset);
}

ECode CCollationElementIterator::Next(
    /* [out] */ Int32* nextValue)
{
    return mIcuIterator->Next(nextValue);
}

ECode CCollationElementIterator::Previous(
    /* [out] */ Int32* previousValue)
{
    return mIcuIterator->Previous(previousValue);
}

ECode CCollationElementIterator::Reset()
{
    return mIcuIterator->Reset();
}

ECode CCollationElementIterator::SetOffset(
    /* [in] */ Int32 newOffset)
{
    return mIcuIterator->SetOffset(newOffset);
}

ECode CCollationElementIterator::SetTextEx(
    /* [in] */ ICharacterIterator* source)
{
    return mIcuIterator->SetTextEx(source);
}

ECode CCollationElementIterator::SetText(
    /* [in] */ const String& source)
{
    return mIcuIterator->SetText(source);
}
