#ifndef __ICURULEBASEDCOLLATOR_H__
#define __ICURULEBASEDCOLLATOR_H__

#include "cmdef.h"
#include "Elastos.Text_server.h"
#include "ICUCollator.h"
#include "NativeCollation.h"
#include <elastos/AutoPtr.h>
#include "CICUCollationKey.h"
#include <StringBuffer.h>

class ICURuleBasedCollator : public ICUCollator
{
public:
    ICURuleBasedCollator();

    /**
     * RuleBasedCollator constructor. This takes the table rules and builds a
     * collation table out of them. Please see RuleBasedCollator class
     * description for more details on the collation rule syntax.
     * @param rules the collation rules to build the collation table from.
     * @exception ParseException thrown if rules are empty or a Runtime error
     *            if collator can not be created.
     * @stable ICU 2.4
     */
    CARAPI Init(
        /* [in] */ String rules);

    /**
     * RuleBasedCollator constructor. This takes the table rules and builds a
     * collation table out of them. Please see RuleBasedCollator class
     * description for more details on the collation rule syntax.
     * @param rules the collation rules to build the collation table from.
     * @param strength collation strength
     * @exception ParseException thrown if rules are empty or a Runtime error
     *            if collator can not be created.
     * @see #PRIMARY
     * @see #SECONDARY
     * @see #TERTIARY
     * @see #QUATERNARY
     * @see #IDENTICAL
     * @stable ICU 2.4
     */
    CARAPI Init(
        /* [in] */ String rules,
        /* [in] */ Int32 strength);

    /**
     * RuleBasedCollator constructor. This takes the table rules and builds a
     * collation table out of them. Please see RuleBasedCollator class
     * description for more details on the collation rule syntax.
     * <p>Note API change starting from release 2.4. Prior to release 2.4, the
     * normalizationMode argument values are from the class
     * com.ibm.icu4jni.text.Normalization. In 2.4,
     * the valid normalizationMode arguments for this API are
     * CollationAttribute.VALUE_ON and CollationAttribute.VALUE_OFF.
     * </p>
     * @param rules the collation rules to build the collation table from.
     * @param strength collation strength
     * @param normalizationMode normalization mode
     * @exception IllegalArgumentException thrown when constructor error occurs
     * @see #PRIMARY
     * @see #SECONDARY
     * @see #TERTIARY
     * @see #QUATERNARY
     * @see #IDENTICAL
     * @see #CANONICAL_DECOMPOSITION
     * @see #NO_DECOMPOSITION
     * @stable ICU 2.4
     */
    CARAPI Init(
        /* [in] */ String rules,
        /* [in] */ Int32 normalizationMode,
        /* [in] */ Int32 strength);

    /**
     * Makes a complete copy of the current object.
     * @return a copy of this object if data clone is a success, otherwise null
     * @stable ICU 2.4
     */
    CARAPI Clone(
        /* [out] */ IInterface ** Instance);

    /**
     * The comparison function compares the character data stored in two
     * different strings. Returns information about whether a string is less
     * than, greater than or equal to another string.
     * <p>Example of use:
     * <br>
     * <code>
     *   Collator myCollation = Collator.createInstance(Locale::US);
     *   myCollation.setStrength(CollationAttribute.VALUE_PRIMARY);
     *   // result would be 0 ("abc" == "ABC")
     *   // (no primary difference between "abc" and "ABC")
     *   int result = myCollation.compare("abc", "ABC",3);
     *   myCollation.setStrength(CollationAttribute.VALUE_TERTIARY);
     *   // result would be -1 (abc" &lt;&lt;&lt; "ABC")
     *   // (with tertiary difference between "abc" and "ABC")
     *   int result = myCollation.compare("abc", "ABC",3);
     * </code>
     */
    CARAPI Compare(
        /* [in] */ String source,
        /* [in] */ String target,
        /* [out] */ Int32 * value);

    /**
     * Get the normalization mode for this object.
     * The normalization mode influences how strings are compared.
     * @see #CANONICAL_DECOMPOSITION
     * @see #NO_DECOMPOSITION
     * @stable ICU 2.4
     */
    CARAPI GetDecomposition(
        /* [out] */ Int32 * decomposition);

    /**
     * <p>Sets the decomposition mode of the Collator object on or off.
     * If the decomposition mode is set to on, string would be decomposed into
     * NFD format where necessary before sorting.</p>
     * </p>
     * @param decompositionmode the new decomposition mode
     * @see #CANONICAL_DECOMPOSITION
     * @see #NO_DECOMPOSITION
     * @stable ICU 2.4
     */
    CARAPI SetDecomposition(
        /* [in] */ Int32 decompositionmode);

    /**
     * Determines the minimum strength that will be use in comparison or
     * transformation.
     * <p>
     * E.g. with strength == CollationAttribute.VALUE_SECONDARY, the tertiary difference
     * is ignored
     * </p>
     * <p>
     * E.g. with strength == PRIMARY, the secondary and tertiary difference are
     * ignored.
     * </p>
     * @return the current comparison level.
     * @see #PRIMARY
     * @see #SECONDARY
     * @see #TERTIARY
     * @see #QUATERNARY
     * @see #IDENTICAL
     * @stable ICU 2.4
     */
    CARAPI GetStrength(
        /* [out] */ Int32 * strength);

    /**
     * Sets the minimum strength to be used in comparison or transformation.
     * <p>Example of use:
     * <br>
     * <code>
     * Collator myCollation = Collator.createInstance(Locale::US);
     * myCollation.setStrength(PRIMARY);
     * // result will be "abc" == "ABC"
     * // tertiary differences will be ignored
     * int result = myCollation->compare("abc", "ABC");
     * </code>
     * @param strength the new comparison level.
     * @exception IllegalArgumentException when argument does not belong to any collation strength
     *            mode or error occurs while setting data.
     * @see #PRIMARY
     * @see #SECONDARY
     * @see #TERTIARY
     * @see #QUATERNARY
     * @see #IDENTICAL
     * @stable ICU 2.4
     */
    CARAPI SetStrength(
        /* [in] */ Int32 strength);

    /**
     * Sets the attribute to be used in comparison or transformation.
     * <p>Example of use:
     * <br>
     * <code>
     *  Collator myCollation = Collator.createInstance(Locale::US);
     *  myCollation.setAttribute(CollationAttribute.CASE_LEVEL,
     *                           CollationAttribute.VALUE_ON);
     *  int result = myCollation->compare("\\u30C3\\u30CF",
     *                                    "\\u30C4\\u30CF");
     * // result will be -1
     * </code>
     * @param type the attribute to be set from CollationAttribute
     * @param value attribute value from CollationAttribute
     * @stable ICU 2.4
     */
    CARAPI SetAttribute(
        /* [in] */ Int32 type,
        /* [in] */ Int32 value);

    /**
     * Gets the attribute to be used in comparison or transformation.
     * @param type the attribute to be set from CollationAttribute
     * @return value attribute value from CollationAttribute
     * @stable ICU 2.4
     */
    CARAPI GetAttribute(
        /* [in] */ Int32 type,
        /* [out] */ Int32 * attribute);

    CARAPI GetCollationKey(
        /* [in] */ String source,
        /* [out] */ IICUCollationKey ** instance);

    /**
     * Get the collation rules of this Collation object
     * The rules will follow the rule syntax.
     * @return collation rules.
     * @stable ICU 2.4
     */
    CARAPI GetRules(
        /* [out] */ String* rules);

    /**
     * Create a CollationElementIterator object that will iterator over the
     * elements in a string, using the collation rules defined in this
     * RuleBasedCollator
     * @param source string to iterate over
     * @return address of C collationelement
     * @exception IllegalArgumentException thrown when error occurs
     * @stable ICU 2.4
     */
    CARAPI GetCollationElementIterator(
        /* [in] */ String source,
        /* [out] */ IICUCollationElementIterator** coleitr);

    CARAPI GetCollationElementIteratorEx(
        /* [in] */ ICharacterIterator * it,
        /* [out] */ IICUCollationElementIterator ** collationElementIterator);
/*
    @Override
    public int hashCode() {
        return 42; // No-one uses RuleBasedCollator as a hash key.
    }*/

    /**
     * Checks if argument object is equals to this object.
     * @param target object
     * @return true if source is equivalent to target, false otherwise
     * @stable ICU 2.4
     */
    CARAPI EqualsEx(
        /* [in] */ IInterface * object,
        /* [out] */ Boolean * result);

protected:
    CARAPI Init(
        /* [in] */ ILocale* locale);

    ~ICURuleBasedCollator();

private:
    CARAPI CharacterIteratorToString(
        /* [in] */ ICharacterIterator* it,
        /* [out] */ String* value);

    ICURuleBasedCollator(
        /* [in] */ Int32 addr);
private:
    Int32 m_collator_;
    Int32 m_hashcode_;

};
#endif //__ICURULEBASEDCOLLATOR_H__