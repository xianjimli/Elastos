
#ifndef __CCOLLATIONELEMENTITERATORHELPER_H__
#define __CCOLLATIONELEMENTITERATORHELPER_H__

#include "_CCollationElementIteratorHelper.h"

CarClass(CCollationElementIteratorHelper)
{
public:
    /**
     * Obtains the primary order of the specified collation element, i.e. the
     * first 16 bits. This value is unsigned.
     *
     * @param order
     *            the element of the collation.
     * @return the element's 16 bit primary order.
     */
    CARAPI PrimaryOrder(
        /* [in] */ Int32 order,
        /* [out] */ Int32* primaryOrder);

    /**
     * Obtains the secondary order of the specified collation element, i.e. the
     * 16th to 23th bits, inclusive. This value is unsigned.
     *
     * @param order
     *            the element of the collator.
     * @return the 8 bit secondary order of the element.
     */
    CARAPI SecondaryOrder(
        /* [in] */ Int32 order,
        /* [out] */ Int16* secondaryOrder);

    /**
     * Obtains the tertiary order of the specified collation element, i.e. the
     * last 8 bits. This value is unsigned.
     *
     * @param order
     *            the element of the collation.
     * @return the 8 bit tertiary order of the element.
     */
    CARAPI TertiaryOrder(
        /* [in] */ Int32 order,
        /* [out] */ Int16* tertiaryOrder);
};

#endif // __CCOLLATIONELEMENTITERATORHELPER_H__
