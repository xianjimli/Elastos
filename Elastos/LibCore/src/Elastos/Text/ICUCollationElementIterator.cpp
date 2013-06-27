
#include "cmdef.h"
#include "ICUCollationElementIterator.h"
#include "NativeCollation.h"

ICUCollationElementIterator::ICUCollationElementIterator(
    /* [in] */ Int32 collelemiteratoraddress)
    : mCollelemiterator(collelemiteratoraddress)
{ }

ICUCollationElementIterator::~ICUCollationElementIterator()
{
    //try {
    NativeCollation::CloseElements(mCollelemiterator);
    //} finally {
    //    super.finalize();
    //}
}

PInterface ICUCollationElementIterator::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IICUCollationElementIterator*)this;
    }
    else if (riid == EIID_IICUCollationElementIterator) {
        return (IICUCollationElementIterator*)this;
    }
    return NULL;
}

UInt32 ICUCollationElementIterator::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ICUCollationElementIterator::Release()
{
    return ElRefBase::Release();
}

ECode ICUCollationElementIterator::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IICUCollationElementIterator*)this) {
        *pIID = EIID_IICUCollationElementIterator;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode ICUCollationElementIterator::Reset()
{
    NativeCollation::Reset(mCollelemiterator);
    return NOERROR;
}

ECode ICUCollationElementIterator::Next(
    /* [out] */ Int32* nextValue)
{
    VALIDATE_NOT_NULL(nextValue);
    return NativeCollation::Next(mCollelemiterator, nextValue);
}

ECode ICUCollationElementIterator::Previous(
    /* [out] */ Int32* previousValue)
{
    VALIDATE_NOT_NULL(previousValue);
    return NativeCollation::Previous(mCollelemiterator, previousValue);
}

ECode ICUCollationElementIterator::GetMaxExpansion(
    /* [in] */ Int32 order,
    /* [out] */ Int32* maxExpansion)
{
    VALIDATE_NOT_NULL(maxExpansion);
    NativeCollation::GetMaxExpansion(mCollelemiterator, order);
    return NOERROR;
}

ECode ICUCollationElementIterator::SetText(
    /* [in] */ const String& source)
{
    return NativeCollation::SetText(mCollelemiterator, source);
}

ECode ICUCollationElementIterator::SetTextEx(
    /* [in] */ ICharacterIterator* source)
{
    //return NativeCollation::SetText(mCollelemiterator, source.toString());
    return E_NOT_IMPLEMENTED;
}

ECode ICUCollationElementIterator::GetOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = NativeCollation::GetOffset(mCollelemiterator);
    return NOERROR;
}

ECode ICUCollationElementIterator::SetOffset(
    /* [in] */ Int32 offset)
{
    return NativeCollation::SetOffset(mCollelemiterator, offset);
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
