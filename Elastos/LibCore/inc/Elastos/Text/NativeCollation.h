#ifndef __NATIVECOLLATION_H__
#define __NATIVECOLLATION_H__

#include <elastos.h>

using namespace Elastos;

class NativeCollation {

private:
    NativeCollation();

public:
      /**
      * Method to create a new C Collator using the argument locale rules.
      * @param locale locale name
      * @return new c collator
      * @internal ICU 2.4
      */
    static CARAPI_(Int32) OpenCollator(
            /* [in] */ String locale);

      /**
      * Method to create a new C Collator using the argument rules.
      * @param rules , set of collation rules
      * @param normalizationmode default normalization mode
      * @param collationstrength default collation strength
      * @return new c collator
      * @internal ICU 2.4
      */
    static CARAPI_(Int32) OpenCollatorFromRules(
        /* [in] */ String rules,
        /* [in] */ Int32 normalizationmode,
        /* [in] */ Int32 collationstrength);

      /**
      * Close a C collator
      * Once closed, a UCollatorOld should not be used.
      * @param collatoraddress The UCollatorOld to close
      * @internal ICU 2.4
      */
    static CARAPI_(void) CloseCollator(
        /* [in] */ Int32 collatoraddress);

      /**
      * Compare two strings.
      * The strings will be compared using the normalization mode and options
      * specified in openCollator or openCollatorFromRules
      * @param collatoraddress address of the c collator
      * @param source The source string.
      * @param target The target string.
      * @return result of the comparison, Collation.EQUAL,
      *         Collation.GREATER or Collation.LESS
      * @internal ICU 2.4
      */
    static CARAPI_(Int32) Compare(
        /* [in] */ Int32 collatoraddress, 
        /* [in] */ String source,
        /* [in] */ String target);

      /**
      * Get the normalization mode for this object.
      * The normalization mode influences how strings are compared.
      * @param collatoraddress
      * @return normalization mode; one of the values from Normalization
      * @internal ICU 2.4
      */
    static CARAPI_(Int32) GetNormalization(
        /* [in] */ Int32 collatoraddress);

      /**
      * Set the normalization mode used int this object
      * The normalization mode influences how strings are compared.
      * @param collatoraddress the address of the C collator
      * @param normalizationmode desired normalization mode; one of the values
      *        from Normalization
      * @internal ICU 2.4
      */
    static CARAPI_(void) SetNormalization(
        /* [in] */ Int32 collatoraddress,
        /* [in] */ Int32 normalizationmode);

      /**
      * Get the collation rules from a UCollator.
      * The rules will follow the rule syntax.
      * @param collatoraddress the address of the C collator
      * @return collation rules.
      * @internal ICU 2.4
      */
    static CARAPI_(String) GetRules(
        /* [in] */ Int32 collatoraddress);

      /**
      * Get a sort key for the argument string
      * Sort keys may be compared using java.util.Arrays.equals
      * @param collatoraddress address of the C collator
      * @param source string for key to be generated
      * @return sort key
      * @internal ICU 2.4
      */
    static CARAPI_(ArrayOf<Byte>*) GetSortKey(
        /* [in] */ Int32 collatoraddress, 
        /* [in] */ String source);

      /**
      * Gets the version information for collation.
      * @param collatoraddress address of the C collator
      * @return version information
      * @internal ICU 2.4
      */
      // private native String getVersion(int collatoraddress);

      /**
      * Universal attribute setter.
      * @param collatoraddress address of the C collator
      * @param type type of attribute to be set
      * @param value attribute value
      * @exception RuntimeException when error occurs while setting attribute value
      * @internal ICU 2.4
      */
    static CARAPI_(void) SetAttribute(
        /* [in] */ Int32 collatoraddress, 
        /* [in] */ Int32 type, 
        /* [in] */ Int32 value);

      /**
      * Universal attribute getter
      * @param collatoraddress address of the C collator
      * @param type type of attribute to be set
      * @return attribute value
      * @exception RuntimeException thrown when error occurs while getting attribute value
      * @internal ICU 2.4
      */
    static CARAPI_(Int32) GetAttribute(
        /* [in] */ Int32 collatoraddress, 
        /* [in] */ Int32 type);

      /**
      * Thread safe cloning operation
      * @param collatoraddress address of C collator to be cloned
      * @return address of the new clone
      * @exception RuntimeException thrown when error occurs while cloning
      * @internal ICU 2.4
      */
    static CARAPI_(Int32) SafeClone(
        /* [in] */ Int32 collatoraddress);

      /**
      * Create a CollationElementIterator object that will iterator over the
      * elements in a string, using the collation rules defined in this
      * RuleBasedCollator
      * @param collatoraddress address of C collator
      * @param source string to iterate over
      * @return address of C collationelementiterator
      * @internal ICU 2.4
      */
    static CARAPI_(Int32) GetCollationElementIterator(
        /* [in] */ Int32 collatoraddress,
        /* [in] */ String source);


      // collationelementiterator methods -------------------------------------

      /**
      * Close a C collation element iterator.
      * @param address of C collation element iterator to close.
      * @internal ICU 2.4
      */
    static CARAPI_(void) CloseElements(
        /* [in] */ Int32 address);

      /**
      * Reset the collation elements to their initial state.
      * This will move the 'cursor' to the beginning of the text.
      * @param address of C collation element iterator to reset.
      * @internal ICU 2.4
      */
    static CARAPI_(void) Reset(
        /* [in] */ Int32 address);

      /**
      * Get the ordering priority of the next collation element in the text.
      * A single character may contain more than one collation element.
      * @param address if C collation elements containing the text.
      * @return next collation elements ordering, or NULLORDER if the end of the
      *         text is reached.
      * @internal ICU 2.4
      */
    static CARAPI_(Int32) Next(
        /* [in] */ Int32 address);

      /**
      * Get the ordering priority of the previous collation element in the text.
      * A single character may contain more than one collation element.
      * @param address of the C collation element iterator containing the text.
      * @return previous collation element ordering, or NULLORDER if the end of
      *         the text is reached.
      * @internal ICU 2.4
      */
    static CARAPI_(Int32) Previous(
        /* [in] */ Int32 address);

      /**
      * Get the maximum length of any expansion sequences that end with the
      * specified comparison order.
      * @param address of the C collation element iterator containing the text.
      * @param order collation order returned by previous or next.
      * @return maximum length of any expansion sequences ending with the
      *         specified order.
      * @internal ICU 2.4
      */
    static CARAPI_(Int32) GetMaxExpansion(
        /* [in] */ Int32 address, 
        /* [in] */ Int32 order);

      /**
      * Set the text containing the collation elements.
      * @param address of the C collation element iterator to be set
      * @param source text containing the collation elements.
      * @internal ICU 2.4
      */
    static CARAPI_(void) SetText(
        /* [in] */ Int32 address, 
        /* [in] */ String source);

      /**
      * Get the offset of the current source character.
      * This is an offset into the text of the character containing the current
      * collation elements.
      * @param address of the C collation elements iterator to query.
      * @return offset of the current source character.
      * @internal ICU 2.4
      */
    static CARAPI_(Int32) GetOffset(
        /* [in] */ Int32 address);

      /**
      * Set the offset of the current source character.
      * This is an offset into the text of the character to be processed.
      * @param address of the C collation element iterator to set.
      * @param offset The desired character offset.
      * @internal ICU 2.4
      */
    static CARAPI_(void) SetOffset(
        /* [in] */ Int32 address,
        /* [in] */ Int32 offset);
};
#endif //__NATIVECOLLATION_H__