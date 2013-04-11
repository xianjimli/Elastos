#include "ICURuleBasedCollator.h"
#include "CRuleBasedCollator.h"


PInterface ICURuleBasedCollator::Probe(
            /* [in]  */ REIID riid)
{
    return NULL;
}

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
    m_collator_ = NativeCollation::OpenCollatorFromRules(rules,
            ICollationAttribute_VALUE_OFF, ICollationAttribute_VALUE_DEFAULT_STRENGTH);
    return NOERROR;
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
    m_collator_ = NativeCollation::OpenCollatorFromRules(rules, ICollationAttribute_VALUE_OFF, strength);
    return NOERROR;
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
    m_collator_ = NativeCollation::OpenCollatorFromRules(rules, normalizationMode, strength);
    return NOERROR;
}

ECode ICURuleBasedCollator::Clone(
        /* [out] */ IInterface ** instance)
{
    AutoPtr<IRuleBasedCollator> result = NULL;
    Int32 collatoraddress = NativeCollation::SafeClone(m_collator_);
    CRuleBasedCollator::New(String::FromInt32(collatoraddress), (IRuleBasedCollator**)&result);
    *instance = reinterpret_cast<ICollator*>(result->Probe(EIID_ICollator));
    (*instance)->AddRef();
    return NOERROR;
}

ECode ICURuleBasedCollator::Compare(
        /* [in] */ String source,
        /* [in] */ String target,
        /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value);
    *value = NativeCollation::Compare(m_collator_, source, target);
    return NOERROR;
}

ECode ICURuleBasedCollator::GetDecomposition(
        /* [out] */ Int32 * decomposition)
{
    VALIDATE_NOT_NULL(decomposition);
    *decomposition = NativeCollation::GetNormalization(m_collator_);
    return NOERROR;
}

ECode ICURuleBasedCollator::SetDecomposition(
        /* [in] */ Int32 decompositionmode)
{
    NativeCollation::SetAttribute(m_collator_,
            ICollationAttribute_NORMALIZATION_MODE, decompositionmode);
    return NOERROR;
}

ECode ICURuleBasedCollator::GetStrength(
        /* [out] */ Int32 * strength)
{
    VALIDATE_NOT_NULL(strength);
    *strength = NativeCollation::GetAttribute(m_collator_, ICollationAttribute_STRENGTH);
    return NOERROR;
}

ECode ICURuleBasedCollator::SetStrength(
        /* [in] */ Int32 strength)
{
    NativeCollation::SetAttribute(m_collator_, ICollationAttribute_STRENGTH, strength);
    return NOERROR;
}

ECode ICURuleBasedCollator::SetAttribute(
        /* [in] */ Int32 type,
        /* [in] */ Int32 value)
{
    NativeCollation::SetAttribute(m_collator_, type, value);
    return NOERROR;
}

ECode ICURuleBasedCollator::GetAttribute(
        /* [in] */ Int32 type,
        /* [out] */ Int32 * pAttribute)
{
    VALIDATE_NOT_NULL(pAttribute);
    NativeCollation::GetAttribute(m_collator_, type);
    return NOERROR;
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
        /* [out] */ IICUCollationElementIterator ** collationElementIterator)
{
    AutoPtr<IICUCollationElementIterator> result;
    CICUCollationElementIterator::New(
            NativeCollation::GetCollationElementIterator(m_collator_, source), 
            (IICUCollationElementIterator**)&result);
    *collationElementIterator = (IICUCollationElementIterator*)result;
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
    m_collator_ = NativeCollation::OpenCollator(name);
    return NOERROR;
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
    while(ch != DONE) {
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