#include "ICUCollationElementIterator.h"

ECode ICUCollationElementIterator::Init(
    /* [in] */ Int32 collelemiteratoraddress)
{
    m_collelemiterator_ = collelemiteratoraddress;
    return NOERROR;
}

ICUCollationElementIterator::~ICUCollationElementIterator()
{
    //try {
        NativeCollation::CloseElements(m_collelemiterator_);
    //} finally {
    //    super.finalize();
    //}
}

ECode ICUCollationElementIterator::Reset()
{
    NativeCollation::Reset(m_collelemiterator_);
    return NOERROR;
}

ECode ICUCollationElementIterator::Next(
            /* [out] */ Int32* nextValue)
{
    VALIDATE_NOT_NULL(nextValue);
    *nextValue = NativeCollation::Next(m_collelemiterator_);
    return NOERROR;
}

ECode ICUCollationElementIterator::Previous(
            /* [out] */ Int32* previousValue)
{
    VALIDATE_NOT_NULL(previousValue);
    *previousValue = NativeCollation::Previous(m_collelemiterator_);
    return NOERROR;
}

ECode ICUCollationElementIterator::GetMaxExpansion(
            /* [in] */ Int32 order,
            /* [out] */ Int32* maxExpansion)
{
    VALIDATE_NOT_NULL(maxExpansion);
    NativeCollation::GetMaxExpansion(m_collelemiterator_, order);
    return NOERROR;
}

ECode ICUCollationElementIterator::SetText(
            /* [in] */ String source)
{
    NativeCollation::SetText(m_collelemiterator_, source);
    return NOERROR;
}

ECode ICUCollationElementIterator::SetTextEx(
            /* [in] */ ICharacterIterator* source)
{
    //NativeCollation::SetText(m_collelemiterator_, source.toString());
    return NOERROR;
}

ECode ICUCollationElementIterator::GetOffset(
            /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = NativeCollation::GetOffset(m_collelemiterator_);
    return NOERROR;
}

ECode ICUCollationElementIterator::SetOffset(
            /* [in] */ Int32 offset)
{
    NativeCollation::SetOffset(m_collelemiterator_, offset);
    return NOERROR;
}

Int32 ICUCollationElementIterator::PrimaryOrder(
        /* [in] */ Int32 order)
{
    return ((order & PRIMARY_ORDER_MASK_) >> PRIMARY_ORDER_SHIFT_) &
            UNSIGNED_16_BIT_MASK_;
}

Int32 ICUCollationElementIterator::SecondaryOrder(
        /* [in] */ Int32 order)
{
    return (order & SECONDARY_ORDER_MASK_) >> SECONDARY_ORDER_SHIFT_;
}

Int32 ICUCollationElementIterator::TertiaryOrder(
        /* [in] */ Int32 order)
{
    return order & TERTIARY_ORDER_MASK_;
}