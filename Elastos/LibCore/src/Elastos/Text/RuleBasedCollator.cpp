#include "RuleBasedCollator.h"

ECode RuleBasedCollator::Init(
        /* [in] */ IICUCollator* wrapper)
{
    return Collator::Init(wrapper);
}

RuleBasedCollator::RuleBasedCollator(){}

RuleBasedCollator::~RuleBasedCollator(){}

ECode RuleBasedCollator::Init(
        /* [in] */ String rules)
{
    if (rules.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    if (rules.IsEmpty()) {
        //throw new ParseException("empty rules", 0);
        return NOERROR;
    }
    //try {
        CICURuleBasedCollator::New(rules, (IICURuleBasedCollator**)&(this->icuColl) );
        this->icuColl->SetDecomposition(Collator_CANONICAL_DECOMPOSITION);
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
    /*
    AutoPtr<IICUCollationElementIterator> pIICUCollationElementIterator;
    ((IICURuleBasedCollator*)(this->icuColl) )
                    ->GetCollationElementIteratorEx(source, 
                        (IICUCollationElementIterator**)&pIICUCollationElementIterator);
    CCollationElementIterator::New(pIICUCollationElementIterator, collationElementIterator);
    */
    return NOERROR;
}

ECode RuleBasedCollator::GetCollationElementIterator(
            /* [in] */ String source,
            /* [out] */ ICollationElementIterator** collationElementIterator)
{
    if (source.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    /*
    AutoPtr<IICUCollationElementIterator> pIICUCollationElementIterator;
    ((IICURuleBasedCollator*)(this->icuColl) )
                    ->GetCollationElementIterator(source, 
                        (IICUCollationElementIterator**)&pIICUCollationElementIterator);
    CCollationElementIterator::New(pIICUCollationElementIterator, collationElementIterator);
    */
    return NOERROR;
}

ECode RuleBasedCollator::GetRules(
            /* [out] */ String* rules)
{
    VALIDATE_NOT_NULL(rules);
//    return ((IICURuleBasedCollator*)(this->icuColl) )->GetRules(rules);
    return NOERROR;
}

ECode RuleBasedCollator::CompareEx(
        /* [in] */ String source,
        /* [in] */ String target,
        /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value);
    if (source.IsNull() || target.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    return this->icuColl->Compare(source, target, value);
}

ECode RuleBasedCollator::GetCollationKey(
        /* [in] */ String source,
        /* [out] */ ICollationKey ** collationKey)
{
//    return icuColl->GetCollationKey(source, collationKey);
    return NOERROR;
}