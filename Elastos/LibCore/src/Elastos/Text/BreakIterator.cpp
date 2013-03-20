#include "BreakIterator.h"

BreakIterator::BreakIterator(){}

BreakIterator::BreakIterator(
        /* [in] */ NativeBreakIterator* iterator)
{
    wrapped = iterator;
}

ECode BreakIterator::GetAvailableLocales(
        /* [out] */ ArrayOf<ILocale*>** availableLocales)
{
//    return ICU::GetAvailableBreakIteratorLocales(availableLocales);
    return E_NOT_IMPLEMENTED;
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
//    return new RuleBasedBreakIterator(NativeBreakIterator.getCharacterInstance(where));
    return E_NOT_IMPLEMENTED;
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
//    return new RuleBasedBreakIterator(NativeBreakIterator.getLineInstance(where));
    return E_NOT_IMPLEMENTED;
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
//    return new RuleBasedBreakIterator(NativeBreakIterator.getSentenceInstance(where));
    return E_NOT_IMPLEMENTED;
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
//    return new RuleBasedBreakIterator(NativeBreakIterator.getWordInstance(where));
    return E_NOT_IMPLEMENTED;
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