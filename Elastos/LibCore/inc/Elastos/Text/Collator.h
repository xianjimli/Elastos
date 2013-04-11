#ifndef __COLLATOR_H__
#define __COLLATOR_H__

#include "cmdef.h"
#include "Elastos.Text_server.h"
#include <elastos.h>
#include <elastos/AutoPtr.h>
//#include "CRuleBasedCollator.h"
#include "ICUCollator.h"


using namespace Elastos;


class Collator {
public:
    /**
     * Returns a new collator with the same decomposition mode and
     * strength value as this collator.
     *
     * @return a shallow copy of this collator.
     * @see java.lang.Cloneable
     */
//    @Override
//    public Object clone() {
//        try {
//            Collator clone = (Collator) super.clone();
//            clone.icuColl = (com.ibm.icu4jni.text.Collator) this.icuColl.clone();
//            return clone;
//        } catch (CloneNotSupportedException e) {
//            throw new AssertionError(e); // android-changed
//        }
//    }

    /**
     * Compares two objects to determine their relative order. The objects must
     * be strings.
     *
     * @param object1
     *            the first string to compare.
     * @param object2
     *            the second string to compare.
     * @return a negative value if {@code object1} is less than {@code object2},
     *         0 if they are equal, and a positive value if {@code object1} is
     *         greater than {@code object2}.
     * @throws ClassCastException
     *         if {@code object1} or {@code object2} is not a {@code String}.
     */
    virtual CARAPI Compare(
        /* [in] */ IInterface* object1,
        /* [in] */ IInterface* object2,
        /* [out] */ Int32* result);

    /**
     * Compares two strings to determine their relative order.
     *
     * @param string1
     *            the first string to compare.
     * @param string2
     *            the second string to compare.
     * @return a negative value if {@code string1} is less than {@code string2},
     *         0 if they are equal and a positive value if {@code string1} is
     *         greater than {@code string2}.
     */
    virtual CARAPI CompareEx(
        /* [in] */ String string1, 
        /* [in] */ String string2,
        /* [out] */ Int32* result) = 0;

    /**
     * Compares this collator with the specified object and indicates if they
     * are equal.
     *
     * @param object
     *            the object to compare with this object.
     * @return {@code true} if {@code object} is a {@code Collator} object and
     *         it has the same strength and decomposition values as this
     *         collator; {@code false} otherwise.
     * @see #hashCode
     */
    //@Override
/*
    public boolean equals(Object object) {
        if (!(object instanceof Collator)) {
            return false;
        }
        Collator collator = (Collator) object;
        return this.icuColl == null ? collator.icuColl == null : this.icuColl
                .equals(collator.icuColl);
    }
*/

    /**
     * Compares two strings using the collation rules to determine if they are
     * equal.
     *
     * @param string1
     *            the first string to compare.
     * @param string2
     *            the second string to compare.
     * @return {@code true} if {@code string1} and {@code string2} are equal
     *         using the collation rules, false otherwise.
     */
    virtual CARAPI Equals(
        /* [in] */ String string1, 
        /* [in] */ String string2,
        /* [out] */ Boolean* result);

    /**
     * Returns an array of locales for which custom {@code Collator} instances
     * are available.
     * <p>Note that Android does not support user-supplied locale service providers.
     */
    static CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    /**
     * Returns a {@link CollationKey} for the specified string for this collator
     * with the current decomposition rule and strength value.
     *
     * @param string
     *            the source string that is converted into a collation key.
     * @return the collation key for {@code string}.
     */
    virtual CARAPI GetCollationKey(
        /* [in] */ String string,
        /* [out] */ IICUCollationKey** key) = 0;

    /**
     * Returns the decomposition rule for this collator.
     *
     * @return the decomposition rule, either {@code NO_DECOMPOSITION} or
     *         {@code CANONICAL_DECOMPOSITION}. {@code FULL_DECOMPOSITION} is
     *         not supported.
     */
    virtual CARAPI GetDecomposition(
        /* [out] */ Int32* decomposition);

    /**
     * Returns a {@code Collator} instance which is appropriate for the user's default
     * {@code Locale}.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     */
    static CARAPI GetInstance(
        /* [out] */ ICollator** instance);

    /**
     * Returns a {@code Collator} instance which is appropriate for {@code locale}.
     */
    static CARAPI GetInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ ICollator** instance);

    /**
     * Returns the strength value for this collator.
     *
     * @return the strength value, either PRIMARY, SECONDARY, TERTIARY or
     *         IDENTICAL.
     */
    virtual CARAPI GetStrength(
        /* [out] */ Int32* strength);

//    @Override
//    public abstract int hashCode();

    /**
     * Sets the decomposition rule for this collator.
     *
     * @param value
     *            the decomposition rule, either {@code NO_DECOMPOSITION} or
     *            {@code CANONICAL_DECOMPOSITION}. {@code FULL_DECOMPOSITION}
     *            is not supported.
     * @throws IllegalArgumentException
     *            if the provided decomposition rule is not valid. This includes
     *            {@code FULL_DECOMPOSITION}.
     */
    virtual CARAPI SetDecomposition(
        /* [in] */ Int32 value);

    /**
     * Sets the strength value for this collator.
     *
     * @param value
     *            the strength value, either PRIMARY, SECONDARY, TERTIARY, or
     *            IDENTICAL.
     * @throws IllegalArgumentException
     *            if the provided strength value is not valid.
     */
    virtual CARAPI SetStrength(
        /* [in] */ Int32 value);

protected:
    CARAPI Init(
        /* [in] */ IICUCollator* wrapper);

    /**
     * Constructs a new {@code Collator} instance.
     */
    Collator();

private:
    CARAPI DecompositionMode_Java_ICU(
        /* [in] */ Int32 mode,
        /* [out] */ Int32* value);

    CARAPI DecompositionMode_ICU_Java(
        /* [in] */ Int32 mode,
        /* [out] */ Int32* value);

    CARAPI Strength_Java_ICU(
        /* [in] */ Int32 value,
        /* [out] */ Int32* result);

    CARAPI Strength_ICU_Java(
        /* [in] */ Int32 value,
        /* [out] */ Int32* result);

protected:
    // Wrapper class of ICU4JNI Collator
    AutoPtr<IICUCollator> icuColl;
};
#endif //__COLLATOR_H__