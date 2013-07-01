
#include "cmdef.h"
#include "ICURuleBasedCollator.h"
#include "ICUCollationElementIterator.h"
#include "NativeCollation.h"
#include "ICUCollationKey.h"
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>

ICURuleBasedCollator::ICURuleBasedCollator(
    /* [in] */ const String& rules)
    : mHashcode(0)
{
    if (rules.IsNull()) {
        assert(0);
        //throw new NullPointerException();
    }
    NativeCollation::OpenCollatorFromRules(rules, ICollationAttribute_VALUE_OFF,
            ICollationAttribute_VALUE_DEFAULT_STRENGTH, &mCollator);
}

ICURuleBasedCollator::ICURuleBasedCollator(
    /* [in] */ const String& rules,
    /* [in] */ Int32 strength)
    : mHashcode(0)
{
    if (rules.IsNull()) {
        assert(0);
        //throw new NullPointerException();
    }
    NativeCollation::OpenCollatorFromRules(rules,
            ICollationAttribute_VALUE_OFF, strength, &mCollator);
}

ICURuleBasedCollator::ICURuleBasedCollator(
    /* [in] */ const String& rules,
    /* [in] */ Int32 normalizationMode,
    /* [in] */ Int32 strength)
    : mHashcode(0)
{
    if (rules.IsNull()) {
        assert(0);
        //throw new NullPointerException();
    }
    NativeCollation::OpenCollatorFromRules(rules, normalizationMode, strength, &mCollator);
}

PInterface ICURuleBasedCollator::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IICURuleBasedCollator*)this;
    }
    else if (riid == EIID_IICURuleBasedCollator) {
        return (IICURuleBasedCollator*)this;
    }
    else if (riid == EIID_IICUCollator) {
        return (IICUCollator*)this;
    }
    return NULL;
}

UInt32 ICURuleBasedCollator::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ICURuleBasedCollator::Release()
{
    return ElRefBase::Release();
}

ECode ICURuleBasedCollator::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* IID)
{
    VALIDATE_NOT_NULL(IID);

    if (object == (IInterface*)(ICollationKey*)this) {
        *IID = EIID_ICollationKey;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode ICURuleBasedCollator::Clone(
    /* [out] */ IInterface** instance)
{
    VALIDATE_NOT_NULL(instance);
    Int32 collatoraddress;
    FAIL_RETURN(NativeCollation::SafeClone(mCollator, &collatoraddress));
    AutoPtr<IICUCollator> result = (IICUCollator*)new ICURuleBasedCollator(
            collatoraddress);
    *instance = (IInterface*)result->Probe(EIID_IInterface);
    (*instance)->AddRef();
    return NOERROR;
}

ECode ICURuleBasedCollator::Compare(
    /* [in] */ const String& source,
    /* [in] */ const String& target,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = NativeCollation::Compare(mCollator, source, target);
    return NOERROR;
}

ECode ICURuleBasedCollator::GetDecomposition(
    /* [out] */ Int32* decomposition)
{
    VALIDATE_NOT_NULL(decomposition);
    return NativeCollation::GetNormalization(mCollator, decomposition);
}

ECode ICURuleBasedCollator::SetDecomposition(
    /* [in] */ Int32 decompositionmode)
{
    return NativeCollation::SetAttribute(mCollator,
            ICollationAttribute_NORMALIZATION_MODE, decompositionmode);
}

ECode ICURuleBasedCollator::GetStrength(
    /* [out] */ Int32 * strength)
{
    VALIDATE_NOT_NULL(strength);
    return NativeCollation::GetAttribute(mCollator,
            ICollationAttribute_STRENGTH, strength);
}

ECode ICURuleBasedCollator::SetStrength(
    /* [in] */ Int32 strength)
{
    return NativeCollation::SetAttribute(mCollator, ICollationAttribute_STRENGTH, strength);
}

ECode ICURuleBasedCollator::SetAttribute(
    /* [in] */ Int32 type,
    /* [in] */ Int32 value)
{
    return NativeCollation::SetAttribute(mCollator, type, value);
}

ECode ICURuleBasedCollator::GetAttribute(
    /* [in] */ Int32 type,
    /* [out] */ Int32* attribute)
{
    VALIDATE_NOT_NULL(attribute);
    return NativeCollation::GetAttribute(mCollator, type, attribute);
}

ECode ICURuleBasedCollator::GetCollationKey(
    /* [in] */ const String& source,
    /* [out] */ ICollationKey** instance)
{
    if (source.IsNull()) {
        *instance = NULL;
        return NOERROR;
    }
    ArrayOf<Byte>* key = NativeCollation::GetSortKey(mCollator, source);
    if (key == NULL) {
        *instance = NULL;
        return NOERROR;
    }
    *instance = (ICollationKey*)new ICUCollationKey(source, *key);
    (*instance)->AddRef();
    return NOERROR;
}

ECode ICURuleBasedCollator::GetRules(
    /* [out] */ String* rules)
{
    VALIDATE_NOT_NULL(rules);
    *rules = NativeCollation::GetRules(mCollator);
    return NOERROR;
}

ECode ICURuleBasedCollator::GetCollationElementIterator(
    /* [in] */ const String& source,
    /* [out] */ IICUCollationElementIterator** coleitr)
{
    VALIDATE_NOT_NULL(coleitr);
    AutoPtr<ICollationElementIterator> result;
    Int32 addr;
    FAIL_RETURN(NativeCollation::GetCollationElementIterator(mCollator, source, &addr));
    *coleitr = (IICUCollationElementIterator*)new ICUCollationElementIterator(addr);
    (*coleitr)->AddRef();
    return NOERROR;
}

ECode ICURuleBasedCollator::GetCollationElementIteratorEx(
    /* [in] */ ICharacterIterator* it,
    /* [out] */ IICUCollationElementIterator** coleitr)
{
    // We only implement the String-based API, so build a string from the iterator.
    return GetCollationElementIterator(CharacterIteratorToString(it), coleitr);
}

ECode ICURuleBasedCollator::HashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = 42;
    return NOERROR;
}

ECode ICURuleBasedCollator::Equals(
    /* [in] */ const String& source,
    /* [in] */ const String& target,
    /* [out] */ Boolean* result)
{
    return ICUCollator::Equals(source, target, result);
}

ECode ICURuleBasedCollator::EqualsEx(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
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
    AutoPtr<IICURuleBasedCollator> rhs
            =(IICURuleBasedCollator*)object->Probe(EIID_IICURuleBasedCollator);
    String rules;
    GetRules(&rules);
    String rhs_rules;
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

ICURuleBasedCollator::ICURuleBasedCollator(
    /* [in] */ ILocale* locale)
    : mHashcode(0)
{
    String name;
    locale->GetDisplayName(&name);
    NativeCollation::OpenCollator(name, &mCollator);
}

ICURuleBasedCollator::~ICURuleBasedCollator()
{
    //try {
    NativeCollation::CloseCollator(mCollator);
    //} finally {
    //    super.finalize();
    //}
}

String ICURuleBasedCollator::CharacterIteratorToString(
    /* [in] */ ICharacterIterator* it)
{
    StringBuffer result;
    Char32 ch;
    it->Current(&ch);
    while(ch != ICharacterIterator_DONE) {
        result += ch;
        it->Next(&ch);
    }
    return result.ToString();
}

ICURuleBasedCollator::ICURuleBasedCollator(
    /* [in] */ Int32 addr)
    : mHashcode(0)
    , mCollator(addr)
{ }