
#include "RuleBasedCollator.h"
#include "ICURuleBasedCollator.h"
#include "CCollationElementIterator.h"

ECode RuleBasedCollator::Init(
    /* [in] */ IICUCollator* wrapper)
{
    return Collator::Init(wrapper);
}

ECode RuleBasedCollator::Init(
    /* [in] */ const String& rules)
{
    if (rules.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    if (rules.IsEmpty()) {
        //throw new ParseException("empty rules", 0);
        return E_PARSE_EXCEPTION;;
    }
    //try {
    mICUColl = (IICUCollator*)new ICURuleBasedCollator(rules);
    mICUColl->SetDecomposition(IICUCollator_CANONICAL_DECOMPOSITION);
    //} catch (Exception e) {
    //    if (e instanceof ParseException) {
    //        throw (ParseException) e;
    //    }
        /*
         * -1 means it's not a ParseException. Maybe IOException thrown when
         * an error occurred while reading internal data.
         */
    //    throw new ParseException(e.getMessage(), -1);
    //}
    return NOERROR;
}

ECode RuleBasedCollator::GetCollationElementIteratorEx(
    /* [in] */ ICharacterIterator* source,
    /* [out] */ ICollationElementIterator** collationElementIterator)
{
    if (source == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    AutoPtr<IICUCollationElementIterator> icuCollationElementIterator;
    AutoPtr<IICURuleBasedCollator> icurbc = (IICURuleBasedCollator*)mICUColl->Probe(
            EIID_IICURuleBasedCollator);
    icurbc->GetCollationElementIteratorEx(source,
            (IICUCollationElementIterator**)&icuCollationElementIterator);
    return CCollationElementIterator::New(icuCollationElementIterator,
            collationElementIterator);
}

ECode RuleBasedCollator::GetCollationElementIterator(
    /* [in] */ const String& source,
    /* [out] */ ICollationElementIterator** collationElementIterator)
{
    if (source.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<IICUCollationElementIterator> icuCollationElementIterator;
    AutoPtr<IICURuleBasedCollator> icurbc = (IICURuleBasedCollator*)mICUColl->Probe(
            EIID_IICURuleBasedCollator);
    icurbc->GetCollationElementIterator(source,
            (IICUCollationElementIterator**)&icuCollationElementIterator);
    return CCollationElementIterator::New(icuCollationElementIterator,
            collationElementIterator);
}

ECode RuleBasedCollator::GetRules(
    /* [out] */ String* rules)
{
    VALIDATE_NOT_NULL(rules);
    AutoPtr<IICURuleBasedCollator> icurbc = (IICURuleBasedCollator*)mICUColl->Probe(
            EIID_IICURuleBasedCollator);
    return icurbc->GetRules(rules);
}

ECode RuleBasedCollator::CompareEx(
    /* [in] */ const String& source,
    /* [in] */ const String& target,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    if (source.IsNull() || target.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    return mICUColl->Compare(source, target, value);
}

ECode RuleBasedCollator::GetCollationKey(
    /* [in] */ const String& source,
    /* [out] */ ICollationKey ** collationKey)
{
    return mICUColl->GetCollationKey(source, collationKey);
}

ECode RuleBasedCollator::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (object == NULL || object->Probe(EIID_ICollator) == NULL) {
        *result = false;
        return NOERROR;
    }
    return Collator::Equals(object, result);
}
