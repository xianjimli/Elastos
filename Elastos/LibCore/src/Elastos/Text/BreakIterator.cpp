
#include "BreakIterator.h"
#include "NativeBreakIterator.h"
#include "CRuleBasedBreakIterator.h"
BreakIterator::BreakIterator(){}

BreakIterator::BreakIterator(
        /* [in] */ NativeBreakIterator* iterator)
{
    wrapped = iterator;
}

ECode BreakIterator::GetAvailableLocales(
        /* [out] */ ArrayOf<ILocale*>** availableLocales)
{
    AutoPtr<IICUHelper> pICUHelper;
    CICUHelper::AcquireSingleton((IICUHelper**)&pICUHelper);
    return pICUHelper->GetAvailableBreakIteratorLocales(availableLocales);
}

ECode BreakIterator::GetCharacterInstance(
        /* [out] */ IBreakIterator** instance)
{
    AutoPtr<ILocaleHelper> pILocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pILocaleHelper);
    AutoPtr<ILocale> pILocale;
    pILocaleHelper->GetDefault((ILocale**)&pILocale);
    return GetCharacterInstance(pILocale, instance);
}

ECode BreakIterator::GetCharacterInstance(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance)
{
    RuleBasedBreakIterator * rbbi = new RuleBasedBreakIterator(
        NativeBreakIterator::GetCharacterInstance(where));
    IRuleBasedBreakIterator * irbbi = reinterpret_cast<IRuleBasedBreakIterator * >(
        rbbi->Probe(EIID_IRuleBasedBreakIterator));
    *instance = (IBreakIterator*)irbbi;
    return NOERROR;
}

ECode BreakIterator::GetLineInstance(
        /* [out] */ IBreakIterator** instance) 
{
    AutoPtr<ILocaleHelper> pILocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pILocaleHelper);
    AutoPtr<ILocale> pILocale;
    pILocaleHelper->GetDefault((ILocale**)&pILocale);
    return GetLineInstance(pILocale, instance);
}

ECode BreakIterator::GetLineInstance(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance)
{
    RuleBasedBreakIterator * rbbi = new RuleBasedBreakIterator(
        NativeBreakIterator::GetLineInstance(where));
    IRuleBasedBreakIterator * irbbi = reinterpret_cast<IRuleBasedBreakIterator * >(
        rbbi->Probe(EIID_IRuleBasedBreakIterator));
    *instance = (IBreakIterator*)irbbi;
    return NOERROR;
}

ECode BreakIterator::GetSentenceInstance(
        /* [out] */ IBreakIterator** instance)
{
    AutoPtr<ILocaleHelper> pILocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pILocaleHelper);
    AutoPtr<ILocale> pILocale;
    pILocaleHelper->GetDefault((ILocale**)&pILocale);
    return GetSentenceInstance(pILocale, instance);
}

ECode BreakIterator::GetSentenceInstance(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance)
{
    RuleBasedBreakIterator * rbbi = new RuleBasedBreakIterator(
        NativeBreakIterator::GetSentenceInstance(where));
    IRuleBasedBreakIterator * irbbi = reinterpret_cast<IRuleBasedBreakIterator * >(
        rbbi->Probe(EIID_IRuleBasedBreakIterator));
    *instance = (IBreakIterator*)irbbi;
    return NOERROR;
}

ECode BreakIterator::GetWordInstance(
        /* [out] */ IBreakIterator** instance)
{
    AutoPtr<ILocaleHelper> pILocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pILocaleHelper);
    AutoPtr<ILocale> pILocale;
    pILocaleHelper->GetDefault((ILocale**)&pILocale);
    return GetWordInstance(pILocale, instance);
}

ECode BreakIterator::GetWordInstance(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance)
{
    RuleBasedBreakIterator * rbbi = new RuleBasedBreakIterator(
        NativeBreakIterator::GetWordInstance(where));
    IRuleBasedBreakIterator * irbbi = reinterpret_cast<IRuleBasedBreakIterator * >(
        rbbi->Probe(EIID_IRuleBasedBreakIterator));
    *instance = (IBreakIterator*)irbbi;
    return NOERROR;
}

ECode BreakIterator::IsBoundary(
        /* [in] */ Int32 offset,
        /* [out] */ Boolean* isBoundary)
{
    VALIDATE_NOT_NULL(isBoundary);
    *isBoundary = wrapped->IsBoundary(offset);
    return NOERROR;
}

ECode BreakIterator::Preceding(
        /* [in] */ Int32 offset,
        /* [out] */ Int32* precedingValue)
{
    VALIDATE_NOT_NULL(precedingValue);
    *precedingValue = wrapped->Preceding(offset);
    return NOERROR;
}

ECode BreakIterator::SetText(
        /* [in] */ String newText)
{
    wrapped->SetText(newText);
    return NOERROR;
}