#include "CollationElementIterator.h"
ECode CollationElementIterator::Init(
        /* [in] */ IICUCollationElementIterator* iterator)
{
    this->icuIterator = iterator;
    return NOERROR;
}

ECode CollationElementIterator::GetMaxExpansion(
            /* [in] */ Int32 order,
            /* [out] */ Int32* maxExpansion)
{
    VALIDATE_NOT_NULL(maxExpansion);
    return this->icuIterator->GetMaxExpansion(order, maxExpansion);
}

ECode CollationElementIterator::GetOffset(
            /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    return this->icuIterator->GetOffset(offset);
}

ECode CollationElementIterator::Next(
            /* [out] */ Int32* nextValue)
{
    VALIDATE_NOT_NULL(nextValue);
    return this->icuIterator->Next(nextValue);
}

ECode CollationElementIterator::Previous(
            /* [out] */ Int32* previousValue)
{
    VALIDATE_NOT_NULL(previousValue);
    return this->icuIterator->Previous(previousValue);
}

Int32 CollationElementIterator::PrimaryOrder(
        /* [in] */ Int32 order)
{
    return ICUCollationElementIterator::PrimaryOrder(order);
}

ECode CollationElementIterator::Reset()
{
    return this->icuIterator->Reset();
}

Int16 CollationElementIterator::SecondaryOrder(
        /* [in] */ Int32 order)
{
    return (Int16)( ICUCollationElementIterator::SecondaryOrder(order) );
}

ECode CollationElementIterator::SetOffset(
            /* [in] */ Int32 newOffset)
{
    return this->icuIterator->SetOffset(newOffset);
}

ECode CollationElementIterator::SetTextEx(
            /* [in] */ ICharacterIterator* source)
{
    return this->icuIterator->SetTextEx(source);
}

ECode CollationElementIterator::SetText(
            /* [in] */ String source)
{
    return this->icuIterator->SetText(source);
}

Int16 CollationElementIterator::TertiaryOrder(
        /* [in] */ Int32 order)
{
    return (Int16)( ICUCollationElementIterator::TertiaryOrder(order) );
}