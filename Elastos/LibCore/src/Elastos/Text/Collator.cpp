#include "Collator.h"
#include "CRuleBasedCollator.h"
#include "ICUCollator.h"

ECode Collator::Init()
{
    // BEGIN android-added
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    mICUColl = ICUCollator::GetInstance(locale);
    // END android-added
    return NOERROR;
}

ECode Collator::Init(
        /* [in] */ IICUCollator* wrapper)
{
    mICUColl = wrapper;
    return NOERROR;
}

ECode Collator::Compare(
    /* [in] */ IInterface* object1,
    /* [in] */ IInterface* object2,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ICharSequence> cs;
    String s1(NULL), s2(NULL);
    if (object1 != NULL && object1->Probe(EIID_ICharSequence) != NULL) {
        cs = (ICharSequence*)object1->Probe(EIID_ICharSequence);
        cs->ToString(&s1);
    }
    if (object2 != NULL && object2->Probe(EIID_ICharSequence) != NULL) {
        cs = (ICharSequence*)object1->Probe(EIID_ICharSequence);
        cs->ToString(&s2);
    }
    return CompareEx(s1, s2, result);
}

ECode Collator::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (object == NULL || object->Probe(EIID_ICollator) == NULL) {
        *result = false;
        return NOERROR;
    }

    AutoPtr<ICollator> collator = (ICollator*)object->Probe(EIID_ICollator);
    AutoPtr<IICUCollator> icuCollator;
    collator->GetICUCollator((IICUCollator**)&icuCollator);
    if (mICUColl == NULL) {
        *result = icuCollator == NULL;
        return NOERROR;
    }
    else {
        return mICUColl->EqualsEx((IInterface*)icuCollator.Get(), result);
    }
}

ECode Collator::EqualsEx(
    /* [in] */ const String& string1,
    /* [in] */ const String& string2,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 i;
    CompareEx(string1, string2, &i);
    *result = i == 0;
    return  NOERROR;
}

ECode Collator::GetAvailableLocales(
    /* [out] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);
    AutoPtr<IICUHelper> icuHelper;
    CICUHelper::AcquireSingleton((IICUHelper**)&icuHelper);
    return icuHelper->GetAvailableCollatorLocales(locales);
}

ECode Collator::GetDecomposition(
    /* [out] */ Int32* decomposition)
{
    VALIDATE_NOT_NULL(decomposition);
    Int32 i;
    mICUColl->GetDecomposition(&i);
    return DecompositionMode_ICU_Java(i, decomposition);
}

ECode Collator::GetInstance(
    /* [out] */ ICollator** instance)
{
    VALIDATE_NOT_NULL(instance);
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return GetInstance(locale, instance);
}

ECode Collator::GetInstance(
    /* [in] */ ILocale* locale,
    /* [out] */ ICollator** instance)
{
    VALIDATE_NOT_NULL(instance);
    if (locale == NULL) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<IRuleBasedCollator> rbc;
    FAIL_RETURN(CRuleBasedCollator::New(ICUCollator::GetInstance(locale),
            (IRuleBasedCollator**)&rbc));
    *instance = (ICollator*)rbc->Probe(EIID_ICollator);
    if (*instance != NULL) {
        (*instance)->AddRef();
    }
    return NOERROR;
}

ECode Collator::GetStrength(
    /* [out] */ Int32* strength)
{
    VALIDATE_NOT_NULL(strength);
    Int32 i;
    mICUColl->GetStrength(&i);
    return Strength_ICU_Java(i, strength);
}

ECode Collator::SetDecomposition(
    /* [in] */ Int32 value)
{
    Int32 i;
    DecompositionMode_Java_ICU(value, &i);
    return mICUColl->SetDecomposition(i);
}

ECode Collator::SetStrength(
    /* [in] */ Int32 value)
{
    Int32 i;
    Strength_Java_ICU(value, &i);
    return mICUColl->SetStrength(i);
}

ECode Collator::DecompositionMode_Java_ICU(
    /* [in] */ Int32 mode,
    /* [out] */ Int32* value)
{
    switch (mode) {
        case ICollator_CANONICAL_DECOMPOSITION:
            *value = IICUCollator_CANONICAL_DECOMPOSITION;
            return NOERROR;
        case ICollator_NO_DECOMPOSITION:
            *value = IICUCollator_NO_DECOMPOSITION;
            return NOERROR;
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode Collator::DecompositionMode_ICU_Java(
    /* [in] */ Int32 mode,
    /* [out] */ Int32* value)
{
    Int32 javaMode = mode;
    switch (mode) {
        case IICUCollator_NO_DECOMPOSITION:
            javaMode = ICollator_NO_DECOMPOSITION;
            break;
        case IICUCollator_CANONICAL_DECOMPOSITION:
            javaMode = ICollator_CANONICAL_DECOMPOSITION;
            break;
    }
    *value = javaMode;
    return NOERROR;
}

ECode Collator::Strength_Java_ICU(
    /* [in] */ Int32 value,
    /* [out] */ Int32* result)
{
    switch (value) {
        case ICollator_PRIMARY:
            *result = IICUCollator_PRIMARY;
            return NOERROR;
        case ICollator_SECONDARY:
            *result = IICUCollator_SECONDARY;
            return NOERROR;
        case ICollator_TERTIARY:
            *result = IICUCollator_TERTIARY;
            return NOERROR;
        case ICollator_IDENTICAL:
            *result = IICUCollator_IDENTICAL;
            return NOERROR;
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode Collator::Strength_ICU_Java(
    /* [in] */ Int32 value,
    /* [out] */ Int32* result)
{
    switch (value) {
        case IICUCollator_PRIMARY:
            *result = ICollator_PRIMARY;
            break;
        case IICUCollator_SECONDARY:
            *result = ICollator_SECONDARY;
            break;
        case IICUCollator_TERTIARY:
            *result = ICollator_TERTIARY;
            break;
        case IICUCollator_IDENTICAL:
            *result = ICollator_IDENTICAL;
            break;
    }
    return NOERROR;
}

ECode Collator::GetICUCollator(
    /* [out] */ IICUCollator** icuCollator)
{
    VALIDATE_NOT_NULL(icuCollator);
    *icuCollator = mICUColl;
    if (*icuCollator != NULL) {
        (*icuCollator)->AddRef();
    }
    return NOERROR;
}