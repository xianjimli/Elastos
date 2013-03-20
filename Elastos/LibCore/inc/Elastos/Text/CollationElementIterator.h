#ifndef __COLLATIONELEMENTITERATOR_H__
#define __COLLATIONELEMENTITERATOR_H__

#include "cmdef.h"
#include "Elastos.Text_server.h"
#include <elastos.h>
#include <elastos/AutoPtr.h>
#include "ICUCollationElementIterator.h"
using namespace Elastos;

class CollationElementIterator {
private:
    AutoPtr<IICUCollationElementIterator> icuIterator;

protected:
    CARAPI Init(
        /* [in] */ IICUCollationElementIterator* iterator);
public:
    /**
     * Obtains the maximum length of any expansion sequence that ends with the
     * specified collation element. Returns {@code 1} if there is no expansion
     * with this collation element as the last element.
     *
     * @param order
     *            a collation element that has been previously obtained from a
     *            call to either the {@link #next()} or {@link #previous()}
     *            method.
     * @return the maximum length of any expansion sequence ending with the
     *         specified collation element.
     */
    CARAPI GetMaxExpansion(
            /* [in] */ Int32 order,
            /* [out] */ Int32* maxExpansion);

    /**
     * Obtains the character offset in the source string corresponding to the
     * next collation element. This value could be any of:
     * <ul>
     * <li>The index of the first character in the source string that matches
     * the value of the next collation element. This means that if
     * {@code setOffset(offset)} sets the index in the middle of a contraction,
     * {@code getOffset()} returns the index of the first character in the
     * contraction, which may not be equal to the original offset that was set.
     * Hence calling {@code getOffset()} immediately after
     * {@code setOffset(offset)} does not guarantee that the original offset set
     * will be returned.</li>
     * <li>If normalization is on, the index of the immediate subsequent
     * character, or composite character with the first character, having a
     * combining class of 0.</li>
     * <li>The length of the source string, if iteration has reached the end.
     * </li>
     * </ul>
     *
     * @return The position of the collation element in the source string that
     *         will be returned by the next invocation of the {@link #next()}
     *         method.
     */
    CARAPI GetOffset(
            /* [out] */ Int32* offset);

    /**
     * Obtains the next collation element in the source string.
     *
     * @return the next collation element or {@code NULLORDER} if the end
     *         of the iteration has been reached.
     */
    CARAPI Next(
            /* [out] */ Int32* nextValue);

    /**
     * Obtains the previous collation element in the source string.
     *
     * @return the previous collation element, or {@code NULLORDER} when
     *         the start of the iteration has been reached.
     */
    CARAPI Previous(
            /* [out] */ Int32* previousValue);

    /**
     * Obtains the primary order of the specified collation element, i.e. the
     * first 16 bits. This value is unsigned.
     *
     * @param order
     *            the element of the collation.
     * @return the element's 16 bit primary order.
     */
    static CARAPI_(Int32) PrimaryOrder(
        /* [in] */ Int32 order);

    /**
     * Repositions the cursor to point at the first element of the current
     * string. The next call to {@link #next()} or {@link #previous()} will
     * return the first and last collation element in the string, respectively.
     * <p>
     * If the {@code RuleBasedCollator} used by this iterator has had its
     * attributes changed, calling {@code reset()} reinitializes the iterator to
     * use the new attributes.
     */
    CARAPI Reset();

    /**
     * Obtains the secondary order of the specified collation element, i.e. the
     * 16th to 23th bits, inclusive. This value is unsigned.
     *
     * @param order
     *            the element of the collator.
     * @return the 8 bit secondary order of the element.
     */
    static CARAPI_(Int16) SecondaryOrder(
        /* [in] */ Int32 order);

    /**
     * Points the iterator at the collation element associated with the
     * character in the source string which is found at the supplied offset.
     * After this call completes, an invocation of the {@link #next()} method
     * will return this collation element.
     * <p>
     * If {@code newOffset} corresponds to a character which is part of a
     * sequence that maps to a single collation element then the iterator is
     * adjusted to the start of that sequence. As a result of this, any
     * subsequent call made to {@code getOffset()} may not return the same value
     * set by this method.
     * <p>
     * If the decomposition mode is on, and offset is in the middle of a
     * decomposable range of source text, the iterator may not return a correct
     * result for the next forwards or backwards iteration. The user must ensure
     * that the offset is not in the middle of a decomposable range.
     *
     * @param newOffset
     *            the character offset into the original source string to set.
     *            Note that this is not an offset into the corresponding
     *            sequence of collation elements.
     */
    CARAPI SetOffset(
            /* [in] */ Int32 newOffset);

    /**
     * Sets a new source string iterator for iteration, and resets the offset to
     * the beginning of the text.
     *
     * @param source
     *            the new source string iterator for iteration.
     */
    CARAPI SetTextEx(
            /* [in] */ ICharacterIterator* source);

    /**
     * Sets a new source string for iteration, and resets the offset to the
     * beginning of the text.
     *
     * @param source
     *            the new source string for iteration.
     */
    CARAPI SetText(
            /* [in] */ String source);

    /**
     * Obtains the tertiary order of the specified collation element, i.e. the
     * last 8 bits. This value is unsigned.
     *
     * @param order
     *            the element of the collation.
     * @return the 8 bit tertiary order of the element.
     */
    static CARAPI_(Int16) TertiaryOrder(
        /* [in] */ Int32 order);
};
#endif //__COLLATIONELEMENTITERATOR_H__

