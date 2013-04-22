#include "Collator.h"
#include "CRuleBasedCollator.h"
#include "ICUCollator.h"


Collator::Collator()
{
//    super();
    // BEGIN android-added
    AutoPtr<ILocaleHelper> pILocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pILocaleHelper);
    AutoPtr<ILocale> pILocale;
    pILocaleHelper->GetDefault((ILocale**)&pILocale);
    icuColl = ICUCollator::GetInstance((ILocale*)pILocale);
    // END android-added
}

ECode Collator::Init(
        /* [in] */ IICUCollator* wrapper)
{
    icuColl = wrapper;
    return NOERROR;
}

ECode Collator::Compare(
        /* [in] */ IInterface* object1,
        /* [in] */ IInterface* object2,
        /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    //return CompareEx((String)(*object1), (String)(*object2), result);
    return NOERROR;
}

ECode Collator::Equals(
        /* [in] */ String string1, 
        /* [in] */ String string2,
        /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 i;
    CompareEx(string1, string2, &i);
    *result = (i == 0) ? TRUE : FALSE;
    return  NOERROR;
}

//ECode Collator::GetAvailableLocales(
//        /* [out, callee] */ ArrayOf<ILocale*>** locales)
//{
//    AutoPtr<IICUHelper> picu;
//    CICUHelper::AcquireSingleton((IICUHelper**)&picu);
//    return picu->GetAvailableCollatorLocales(locales);
//}

ECode Collator::GetDecomposition(
        /* [out] */ Int32* decomposition)
{
    Int32 i;
    this->icuColl->GetDecomposition(&i);
    return DecompositionMode_ICU_Java(i, decomposition);
}

//ECode Collator::GetInstance(
//        /* [out] */ ICollator** instance)
//{
//    AutoPtr<ILocaleHelper> pLocaleHelper;
//    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pLocaleHelper);
//    AutoPtr<ILocale> pLocale;
//    pLocaleHelper->GetDefault((ILocale**)&pLocale);
//    return GetInstance((ILocale*)pLocale, instance);
//}

//ECode Collator::GetInstance(
//        /* [in] */ ILocale* locale,
//        /* [out] */ ICollator** instance)
//{
//    if (locale == NULL) {
        //throw new NullPointerException();
//        return E_NULL_POINTER_EXCEPTION;
//    }
//    AutoPtr<IRuleBasedCollator> rbc;
//    CRuleBasedCollator::New(ICUCollator::GetInstance(locale), (IRuleBasedCollator**)&rbc);
//    *instance = reinterpret_cast<ICollator*>(rbc->Probe(EIID_ICollator));
//    return NOERROR;
//}

ECode Collator::GetStrength(
        /* [out] */ Int32* strength)
{
    Int32 i;
    this->icuColl->GetStrength(&i);
    return Strength_ICU_Java(i, strength);
}

ECode Collator::SetDecomposition(
        /* [in] */ Int32 value)
{
    Int32 i;
    DecompositionMode_Java_ICU(value, &i);
    return this->icuColl->SetDecomposition(i);
}

ECode Collator::SetStrength(
        /* [in] */ Int32 value)
{
    Int32 i;
    Strength_Java_ICU(value, &i);
    return this->icuColl->SetStrength(i);
}

ECode Collator::DecompositionMode_Java_ICU(
        /* [in] */ Int32 mode,
        /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    switch (mode) {
    case Collator_CANONICAL_DECOMPOSITION:
        *value = 17;
        return NOERROR;
    case Collator_NO_DECOMPOSITION:
        *value = 16;
        return NOERROR;
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode Collator::DecompositionMode_ICU_Java(
        /* [in] */ Int32 mode,
        /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 javaMode = mode;
    switch (mode) {
        case 16:
            javaMode = Collator_NO_DECOMPOSITION;
            break;
        case 17:
            javaMode = Collator_CANONICAL_DECOMPOSITION;
            break;
    }
    *value = javaMode;
    return NOERROR;
}

ECode Collator::Strength_Java_ICU(
        /* [in] */ Int32 value,
        /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    switch (value) {
    case Collator_PRIMARY:
        *result = 0;
        return NOERROR;
    case Collator_SECONDARY:
        *result = 1;
        return NOERROR;
    case Collator_TERTIARY:
        *result = 2;
        return NOERROR;
    case Collator_IDENTICAL:
        *result = 15;
        return NOERROR;
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode Collator::Strength_ICU_Java(
        /* [in] */ Int32 value,
        /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 javaValue = value;
    switch (value) {
        case 0:
            javaValue = Collator_PRIMARY;
            break;
        case 1:
            javaValue = Collator_SECONDARY;
            break;
        case 2:
            javaValue = Collator_TERTIARY;
            break;
        case 15:
            javaValue = Collator_IDENTICAL;
            break;
    }
    *result = javaValue;
    return NOERROR;
}