#ifndef __ICUCOLLATIONELEMENTITERATOR_H__
#define __ICUCOLLATIONELEMENTITERATOR_H__

#include "cmdef.h"
#include "Elastos.Text_server.h"

#include <elastos.h>
#include "NativeCollation.h"
using namespace Elastos;

class ICUCollationElementIterator {
    // public methods -----------------------------------------------
public:
    /**
     * Reset the collation elements to their initial state.
     * This will move the 'cursor' to the beginning of the text.
     * @stable ICU 2.4
     */
    CARAPI Reset();

    /**
     * Get the ordering priority of the next collation element in the text.
     * A single character may contain more than one collation element.
     * @return next collation elements ordering, or NULLORDER if the end of the
     *         text is reached.
     * @stable ICU 2.4
     */
    CARAPI Next(
            /* [out] */ Int32* nextValue);

    /**
     * Get the ordering priority of the previous collation element in the text.
     * A single character may contain more than one collation element.
     * @return previous collation element ordering, or NULLORDER if the end of
     *         the text is reached.
     * @stable ICU 2.4
     */
    CARAPI Previous(
            /* [out] */ Int32* previousValue);

    /**
     * Get the maximum length of any expansion sequences that end with the
     * specified comparison order.
     * @param order collation order returned by previous or next.
     * @return maximum size of the expansion sequences ending with the collation
     *              element or 1 if collation element does not occur at the end of
     *              any expansion sequence
     * @stable ICU 2.4
     */
    CARAPI GetMaxExpansion(
            /* [in] */ Int32 order,
            /* [out] */ Int32* maxExpansion);

    /**
     * Set the text containing the collation elements.
     * @param source text containing the collation elements.
     * @stable ICU 2.4
     */
    CARAPI SetText(
            /* [in] */ String source);

    // BEGIN android-added
    CARAPI SetTextEx(
            /* [in] */ ICharacterIterator* source);
    // END android-added

    /**
     * Get the offset of the current source character.
     * This is an offset into the text of the character containing the current
     * collation elements.
     * @return offset of the current source character.
     * @stable ICU 2.4
     */
    CARAPI GetOffset(
            /* [out] */ Int32* offset);

    /**
     * Set the offset of the current source character.
     * This is an offset into the text of the character to be processed.
     * @param offset The desired character offset.
     * @stable ICU 2.4
     */
    CARAPI SetOffset(
            /* [in] */ Int32 offset);

    /**
     * Gets the primary order of a collation order.
     * @param order the collation order
     * @return the primary order of a collation order.
     * @stable ICU 2.4
     */
    static CARAPI_(Int32) PrimaryOrder(
        /* [in] */ Int32 order);

    /**
     * Gets the secondary order of a collation order.
     * @param order the collation order
     * @return the secondary order of a collation order.
     * @stable ICU 2.4
     */
    static CARAPI_(Int32) SecondaryOrder(
        /* [in] */ Int32 order);

    /**
     * Gets the tertiary order of a collation order.
     * @param order the collation order
     * @return the tertiary order of a collation order.
     * @stable ICU 2.4
     */
    static CARAPI_(Int32) TertiaryOrder(
        /* [in] */ Int32 order);

protected:
    // protected constructor ----------------------------------------

    /**
     * CollationElementIteratorJNI constructor.
     * The only caller of this class should be
     * RuleBasedCollator.getCollationElementIterator().
     * @param collelemiteratoraddress address of C collationelementiterator
     */
    CARAPI Init(
        /* [in] */ Int32 collelemiteratoraddress);

    // protected methods --------------------------------------------

    /**
     * Garbage collection.
     * Close C collator and reclaim memory.
     * @stable ICU 2.4
     */
    ~ICUCollationElementIterator();

    // private data members -----------------------------------------
private:
    /**
     * C collator
     */
    Int32 m_collelemiterator_;

    /**
     * ICU constant primary order mask for collation elements
     */
    const static Int32 PRIMARY_ORDER_MASK_ = 0xffff0000;
    /**
     * ICU constant secondary order mask for collation elements
     */
    const static Int32 SECONDARY_ORDER_MASK_ = 0x0000ff00;
    /**
     * ICU constant tertiary order mask for collation elements
     */
    const static Int32 TERTIARY_ORDER_MASK_ = 0x000000ff;
    /**
     * ICU constant primary order shift for collation elements
     */
    const static Int32 PRIMARY_ORDER_SHIFT_ = 16;
    /**
     * ICU constant secondary order shift for collation elements
     */
    const static Int32 SECONDARY_ORDER_SHIFT_ = 8;
    /**
     * Unsigned 16 bit mask
     */
    const static Int32 UNSIGNED_16_BIT_MASK_ = 0x0000FFFF;
};
#endif //__ICUCOLLATIONELEMENTITERATOR_H__