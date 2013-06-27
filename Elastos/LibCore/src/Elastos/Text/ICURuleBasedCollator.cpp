#include "ICURuleBasedCollator.h"
#include "CRuleBasedCollator.h"
#include "ICUCollationElementIterator.h"

ICURuleBasedCollator::ICURuleBasedCollator()
{

}

ECode ICURuleBasedCollator::Init(
    /* [in] */ String rules)
{
    m_hashcode_ = 0;
    if (rules.IsNull()) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    return NativeCollation::OpenCollatorFromRules(rules, ICollationAttribute_VALUE_OFF,
            ICollationAttribute_VALUE_DEFAULT_STRENGTH, &m_collator_);
}

ECode ICURuleBasedCollator::Init(
    /* [in] */ String rules,
    /* [in] */ Int32 strength)
{
    m_hashcode_ = 0;
    if (rules.IsNull()) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    return NativeCollation::OpenCollatorFromRules(rules,
            ICollationAttribute_VALUE_OFF, strength, &m_collator_);
}

ECode ICURuleBasedCollator::Init(
    /* [in] */ String rules,
    /* [in] */ Int32 normalizationMode,
    /* [in] */ Int32 strength)
{
    m_hashcode_ = 0;
    if (rules.IsNull()) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    return NativeCollation::OpenCollatorFromRules(rules, normalizationMode,
            strength, &m_collator_);
    return NOERROR;
}

ECode ICURuleBasedCollator::Clone(
    /* [out] */ IInterface ** instance)
{
    AutoPtr<IRuleBasedCollator> result = NULL;
    Int32 collatoraddress;
    FAIL_RETURN(NativeCollation::SafeClone(m_collator_, &collatoraddress));
    ASSERT_SUCCEEDED(CRuleBasedCollator::New(String::FromInt32(collatoraddress),
            (IRuleBasedCollator**)&result));
    *instance = (IInterface*)result->Probe(EIID_IInterface);
    (*instance)->AddRef();
    return NOERROR;
}

ECode ICURuleBasedCollator::Compare(
    /* [in] */ String source,
    /* [in] */ String target,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = NativeCollation::Compare(m_collator_, source, target);
    return NOERROR;
}

ECode ICURuleBasedCollator::GetDecomposition(
    /* [out] */ Int32* decomposition)
{
    VALIDATE_NOT_NULL(decomposition);
    return NativeCollation::GetNormalization(m_collator_, decomposition);
}

ECode ICURuleBasedCollator::SetDecomposition(
    /* [in] */ Int32 decompositionmode)
{
    return NativeCollation::SetAttribute(m_collator_,
            ICollationAttribute_NORMALIZATION_MODE, decompositionmode);
}

ECode ICURuleBasedCollator::GetStrength(
    /* [out] */ Int32 * strength)
{
    VALIDATE_NOT_NULL(strength);
    return NativeCollation::GetAttribute(m_collator_,
            ICollationAttribute_STRENGTH, strength);
}

ECode ICURuleBasedCollator::SetStrength(
    /* [in] */ Int32 strength)
{
    return NativeCollation::SetAttribute(m_collator_, ICollationAttribute_STRENGTH, strength);
}

ECode ICURuleBasedCollator::SetAttribute(
    /* [in] */ Int32 type,
    /* [in] */ Int32 value)
{
    return NativeCollation::SetAttribute(m_collator_, type, value);
}

ECode ICURuleBasedCollator::GetAttribute(
    /* [in] */ Int32 type,
    /* [out] */ Int32* attribute)
{
    VALIDATE_NOT_NULL(attribute);
    return NativeCollation::GetAttribute(m_collator_, type, attribute);
}

ECode ICURuleBasedCollator::GetCollationKey(
    /* [in] */ String source,
    /* [out] */ IICUCollationKey ** instance)
{
    if (source.IsNull()) {
        *instance = NULL;
        return NOERROR;
    }
    ArrayOf<Byte>* key = NativeCollation::GetSortKey(m_collator_, source);
    if (key == NULL) {
        *instance = NULL;
        return NOERROR;
    }
    CICUCollationKey::New(source, key, instance);
    return NOERROR;
}

ECode ICURuleBasedCollator::GetRules(
    /* [out] */ String* rules)
{
    VALIDATE_NOT_NULL(rules);
    *rules = NativeCollation::GetRules(m_collator_);
    return NOERROR;
}

ECode ICURuleBasedCollator::GetCollationElementIterator(
    /* [in] */ String source,
    /* [out] */ IICUCollationElementIterator** coleitr)
{
    AutoPtr<ICollationElementIterator> result;
    Int32 addr;
    FAIL_RETURN(NativeCollation::GetCollationElementIterator(m_collator_, source, &addr));
    *coleitr = (IICUCollationElementIterator*)new ICUCollationElementIterator(addr);
    (*coleitr)->AddRef();
    return NOERROR;
}

ECode ICURuleBasedCollator::GetCollationElementIteratorEx(
    /* [in] */ ICharacterIterator * it,
    /* [out] */ IICUCollationElementIterator ** collationElementIterator)
{
    // We only implement the String-based API, so build a string from the iterator.
    String value = String("");
    CharacterIteratorToString(it, &value);
    return GetCollationElementIterator(value, collationElementIterator);
}

ECode ICURuleBasedCollator::EqualsEx(
    /* [in] */ IInterface * object,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    if (object == (IInterface*)this) {
        *result = TRUE;
        return NOERROR;
    }
    if (object->Probe(EIID_IICURuleBasedCollator) == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<IICURuleBasedCollator> rhs = (IICURuleBasedCollator*) object;
    String rules = String("");
    GetRules(&rules);
    String rhs_rules = String("");
    rhs->GetRules(&rhs_rules);

    Int32 strength, rhs_strength;
    GetStrength(&strength);
    rhs->GetStrength(&rhs_strength);

    Int32 decomposition, rhs_decomposition;
    GetDecomposition(&decomposition);
    rhs->GetDecomposition(&rhs_decomposition);

    *result = rules.Equals(rhs_rules) &&
            strength == rhs_strength &&
            decomposition == rhs_decomposition;
    return NOERROR;
}

ECode ICURuleBasedCollator::Init(
    /* [in] */ ILocale* locale)
{
    m_hashcode_ = 0;
    String name = String("");
    locale->GetDisplayName(&name);
    return NativeCollation::OpenCollator(name, &m_collator_);
}

ICURuleBasedCollator::~ICURuleBasedCollator()
{
    //try {
        NativeCollation::CloseCollator(m_collator_);
    //} finally {
    //    super.finalize();
    //}
}

ECode ICURuleBasedCollator::CharacterIteratorToString(
    /* [in] */ ICharacterIterator* it,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    StringBuffer *result = new StringBuffer("");
    Char32 ch;
    it->Current(&ch);
    while(ch != IBreakIterator_DONE) {
        (*result) += ch;
        it->Next(&ch);
    }
    *value = result->Substring(0, result->GetLength());
    return NOERROR;
}

ICURuleBasedCollator::ICURuleBasedCollator(
    /* [in] */ Int32 addr)
{
    m_hashcode_ = 0;
    m_collator_ = addr;
}