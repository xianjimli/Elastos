#ifndef __RULEBASEDCOLLATOR_H__
#define __RULEBASEDCOLLATOR_H__

#include "cmdef.h"
#include "Elastos.Text_server.h"

#include <elastos.h>
#include "Collator.h"
#include "CICURuleBasedCollator.h"
#include "CCollationElementIterator.h"

using namespace Elastos;

class RuleBasedCollator : public Collator {
protected:

    CARAPI Init(
        /* [in] */ IICUCollator* wrapper);

public:

    RuleBasedCollator();

    virtual ~RuleBasedCollator();
    /**
     * Constructs a new instance of {@code RuleBasedCollator} using the
     * specified {@code rules}. The {@code rules} are usually either
     * hand-written based on the {@link RuleBasedCollator class description} or
     * the result of a former {@link #getRules()} call.
     * <p>
     * Note that the {@code rules} are actually interpreted as a delta to the
     * standard Unicode Collation Algorithm (UCA). This differs
     * slightly from other implementations which work with full {@code rules}
     * specifications and may result in different behavior.
     *
     * @param rules
     *            the collation rules.
     * @throws NullPointerException
     *             if {@code rules == null}.
     * @throws ParseException
     *             if {@code rules} contains rules with invalid collation rule
     *             syntax.
     */
    CARAPI Init(
        /* [in] */ String rules);

    /**
     * Obtains a {@code CollationElementIterator} for the given
     * {@code CharacterIterator}. The source iterator's integrity will be
     * preserved since a new copy will be created for use.
     *
     * @param source
     *            the source character iterator.
     * @return a {@code CollationElementIterator} for {@code source}.
     */
    virtual CARAPI GetCollationElementIteratorEx(
            /* [in] */ ICharacterIterator* source,
            /* [out] */ ICollationElementIterator** collationElementIterator);

    /**
     * Obtains a {@code CollationElementIterator} for the given string.
     *
     * @param source
     *            the source string.
     * @return the {@code CollationElementIterator} for {@code source}.
     */
    virtual CARAPI GetCollationElementIterator(
            /* [in] */ String source,
            /* [out] */ ICollationElementIterator** collationElementIterator);

    /**
     * Returns the collation rules of this collator. These {@code rules} can be
     * fed into the {@code RuleBasedCollator(String)} constructor.
     * <p>
     * Note that the {@code rules} are actually interpreted as a delta to the
     * standard Unicode Collation Algorithm (UCA). Hence, an empty {@code rules}
     * string results in the default UCA rules being applied. This differs
     * slightly from other implementations which work with full {@code rules}
     * specifications and may result in different behavior.
     *
     * @return the collation rules.
     */
    virtual CARAPI GetRules(
            /* [out] */ String* rules);

    /**
     * Returns a new collator with the same collation rules, decomposition mode and
     * strength value as this collator.
     *
     * @return a shallow copy of this collator.
     * @see java.lang.Cloneable
     */
/*
    @Override
    public Object clone() {
        RuleBasedCollator clone = (RuleBasedCollator) super.clone();
        return clone;
    }*/

    /**
     * Compares the {@code source} text to the {@code target} text according to
     * the collation rules, strength and decomposition mode for this
     * {@code RuleBasedCollator}. See the {@code Collator} class description
     * for an example of use.
     * <p>
     * General recommendation: If comparisons are to be done with the same strings
     * multiple times, it is more efficient to generate {@code CollationKey}
     * objects for the strings and use
     * {@code CollationKey.compareTo(CollationKey)} for the comparisons. If each
     * string is compared to only once, using
     * {@code RuleBasedCollator.compare(String, String)} has better performance.
     *
     * @param source
     *            the source text.
     * @param target
     *            the target text.
     * @return an integer which may be a negative value, zero, or else a
     *         positive value depending on whether {@code source} is less than,
     *         equivalent to, or greater than {@code target}.
     */
    //@Override
    CARAPI CompareEx(
        /* [in] */ String source,
        /* [in] */ String target,
        /* [out] */ Int32 * value);

    /**
     * Returns the {@code CollationKey} for the given source text.
     *
     * @param source
     *            the specified source text.
     * @return the {@code CollationKey} for the given source text.
     */
    //@Override
    CARAPI GetCollationKey(
        /* [in] */ String source,
        /* [out] */ IICUCollationKey ** collationKey);

/*
    @Override
    public int hashCode() {
        return ((com.ibm.icu4jni.text.RuleBasedCollator) this.icuColl).getRules().hashCode();
    }*/

    /**
     * Compares the specified object with this {@code RuleBasedCollator} and
     * indicates if they are equal. In order to be equal, {@code object} must be
     * an instance of {@code Collator} with the same collation rules and the
     * same attributes.
     *
     * @param obj
     *            the object to compare with this object.
     * @return {@code true} if the specified object is equal to this
     *         {@code RuleBasedCollator}; {@code false} otherwise.
     * @see #hashCode
     */
    /*
    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof Collator)) {
            return false;
        }
        return super.equals(obj);
    }*/
};
#endif //__RULEBASEDCOLLATOR_H__