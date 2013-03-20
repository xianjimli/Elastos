#ifndef __ICUCOLLATOR_H__
#define __ICUCOLLATOR_H__

#include "cmdef.h"
#include "Elastos.Text_server.h"
#include <elastos.h>

using namespace Elastos;

class ICUCollator {
public:
    static CARAPI_(IICUCollator*) GetInstance(
        /* [in] */ ILocale* locale);

    virtual CARAPI Equals(
        /* [in] */ String source,
        /* [in] */ String target,
        /* [out] */ Boolean* result);

    virtual CARAPI EqualsEx(
        /* [in] */ IInterface* target,
        /* [out] */ Boolean* result) = 0;

    virtual CARAPI Clone(
        /* [out] */ IInterface** instance) = 0;

    /**
     * The comparison function compares the character data stored in two
     * different strings. Returns information about whether a string is less
     * than, greater than or equal to another string.
     * <p>Example of use:
     * <pre>
     * .  Collator myCollation = Collator.getInstance(Locale::US);
     * .  myCollation.setStrength(CollationAttribute.VALUE_PRIMARY);
     * .  // result would be CollationAttribute.VALUE_EQUAL
     * .  // ("abc" == "ABC")
     * .  // (no primary difference between "abc" and "ABC")
     * .  int result = myCollation.compare("abc", "ABC",3);
     * .  myCollation.setStrength(CollationAttribute.VALUE_TERTIARY);
     * .  // result would be Collation.LESS (abc" &lt;&lt;&lt; "ABC")
     * .  // (with tertiary difference between "abc" and "ABC")
     * .  int result = myCollation.compare("abc", "ABC",3);
     * </pre>
     * @stable ICU 2.4
     */
    virtual CARAPI Compare(
        /* [in] */ String source,
        /* [in] */ String target,
        /* [out] */ Int32* value) = 0;

    /**
     * Get the decomposition mode of this Collator.
     * @return the decomposition mode
     * @see #CANONICAL_DECOMPOSITION
     * @see #NO_DECOMPOSITION
     * @stable ICU 2.4
     */
    virtual CARAPI GetDecomposition(
        /* [out] */ Int32* decomposition) = 0;

    /**
     * Set the normalization mode used int this object
     * The normalization mode influences how strings are compared.
     * @param mode desired normalization mode
     * @see #CANONICAL_DECOMPOSITION
     * @see #NO_DECOMPOSITION
     * @stable ICU 2.4
     */
    virtual CARAPI SetDecomposition(
        /* [in] */ Int32 mode) = 0;

    /**
     * Determines the minimum strength that will be use in comparison or
     * transformation.
     * <p>
     * E.g. with strength == SECONDARY, the tertiary difference is ignored
     * </p>
     * <p>
     * E.g. with strength == PRIMARY, the secondary and tertiary difference
     * are ignored.
     * </p>
     * @return the current comparison level.
     * @see #PRIMARY
     * @see #SECONDARY
     * @see #TERTIARY
     * @see #QUATERNARY
     * @see #IDENTICAL
     * @stable ICU 2.4
     */
    virtual CARAPI GetStrength(
        /* [out] */ Int32* strength) = 0;

    /**
     * Gets the attribute to be used in comparison or transformation.
     * @param type the attribute to be set from CollationAttribute
     * @return value attribute value from CollationAttribute
     * @stable ICU 2.4
     */
    virtual CARAPI GetAttribute(
        /* [in] */ Int32 type,
        /* [out] */ Int32* attribute) = 0;

    /**
     * Sets the minimum strength to be used in comparison or transformation.
     * <p>Example of use:
     * <pre>
     * . Collator myCollation = Collator.createInstance(Locale::US);
     * . myCollation.setStrength(PRIMARY);
     * . // result will be "abc" == "ABC"
     * . // tertiary differences will be ignored
     * . int result = myCollation->compare("abc", "ABC");
     * </pre>
     * @param strength the new comparison level.
     * @see #PRIMARY
     * @see #SECONDARY
     * @see #TERTIARY
     * @see #QUATERNARY
     * @see #IDENTICAL
     * @stable ICU 2.4
     */
    virtual CARAPI SetStrength(
        /* [in] */ Int32 strength) = 0;

    /**
     * Sets the attribute to be used in comparison or transformation.
     * <p>Example of use:
     * <pre>
     * . Collator myCollation = Collator.createInstance(Locale::US);
     * . myCollation.setAttribute(CollationAttribute.CASE_LEVEL,
     * .                          CollationAttribute.VALUE_ON);
     * . int result = myCollation->compare("\\u30C3\\u30CF",
     * .                                   "\\u30C4\\u30CF");
     * . // result will be -1.
     * </pre>
     * @param type the attribute to be set from CollationAttribute
     * @param value attribute value from CollationAttribute
     * @stable ICU 2.4
     */
    virtual CARAPI SetAttribute(
        /* [in] */ Int32 type,
        /* [in] */ Int32 value) = 0;

    /**
     * Get the sort key as an CollationKey object from the argument string.
     * To retrieve sort key in terms of byte arrays, use the method as below<br>
     * <code>
     * Collator collator = Collator.getInstance();
     * CollationKey collationKey = collator.getCollationKey("string");
     * byte[] array = collationKey.toByteArray();
     * </code><br>
     * Byte array result are zero-terminated and can be compared using
     * java.util.Arrays.equals();
     * @param source string to be processed.
     * @return the sort key
     * @stable ICU 2.4
     */
    virtual CARAPI GetCollationKey(
        /* [in] */ String source,
        /* [out] */ IICUCollationKey** instance) = 0;

    /**
     * Returns a hash of this collation object
     * @return hash of this collation object
     * @stable ICU 2.4
     */
    virtual CARAPI HashCode(
        /* [out] */ Int32* value) = 0;
};
#endif //__ICUCOLLATOR_H__