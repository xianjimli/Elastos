
#include "cmdef.h"
#include "CCollationElementIteratorHelper.h"
#include "ICUCollationElementIterator.h"

ECode CCollationElementIteratorHelper::PrimaryOrder(
    /* [in] */ Int32 order,
    /* [out] */ Int32* primaryOrder)
{
    VALIDATE_NOT_NULL(primaryOrder);
    *primaryOrder = ICUCollationElementIterator::PrimaryOrder(order);
    return NOERROR;
}

ECode CCollationElementIteratorHelper::SecondaryOrder(
    /* [in] */ Int32 order,
    /* [out] */ Int16* secondaryOrder)
{
    VALIDATE_NOT_NULL(secondaryOrder);
    *secondaryOrder = ICUCollationElementIterator::SecondaryOrder(order);
    return NOERROR;
}

ECode CCollationElementIteratorHelper::TertiaryOrder(
    /* [in] */ Int32 order,
    /* [out] */ Int16* tertiaryOrder)
{
    VALIDATE_NOT_NULL(tertiaryOrder);
    *tertiaryOrder = ICUCollationElementIterator::TertiaryOrder(order);
    return NOERROR;
}