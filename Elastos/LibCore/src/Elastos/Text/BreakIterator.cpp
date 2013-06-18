
#include "BreakIterator.h"
#include "NativeBreakIterator.h"
#include "RuleBasedBreakIterator.h"

BreakIterator::BreakIterator()
{ }

BreakIterator::BreakIterator(
    /* [in] */ NativeBreakIterator* iterator)
    : mWrapped(iterator)
{ }

ECode BreakIterator::GetAvailableLocales(
    /* [out] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);
    AutoPtr<IICUHelper> ICUHelper;
    CICUHelper::AcquireSingleton((IICUHelper**)&ICUHelper);
    return ICUHelper->GetAvailableBreakIteratorLocales(locales);
}

ECode BreakIterator::GetCharacterInstance(
    /* [out] */ IBreakIterator** instance)
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return GetCharacterInstance(locale, instance);
}

ECode BreakIterator::GetCharacterInstance(
    /* [in] */ ILocale* where,
    /* [out] */ IBreakIterator** instance)
{
    VALIDATE_NOT_NULL(instance);
    RuleBasedBreakIterator* rbbi = new RuleBasedBreakIterator(
            NativeBreakIterator::GetCharacterInstance(where));
    *instance = (IBreakIterator*)rbbi->Probe(EIID_IBreakIterator);
    if (*instance != NULL) {
        (*instance)->AddRef();
    }
    return NOERROR;
}

ECode BreakIterator::GetLineInstance(
    /* [out] */ IBreakIterator** instance)
{
    VALIDATE_NOT_NULL(instance);
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return GetLineInstance(locale, instance);
}

ECode BreakIterator::GetLineInstance(
    /* [in] */ ILocale* where,
    /* [out] */ IBreakIterator** instance)
{
    VALIDATE_NOT_NULL(instance);
    RuleBasedBreakIterator * rbbi = new RuleBasedBreakIterator(
            NativeBreakIterator::GetLineInstance(where));
    *instance = (IBreakIterator* )rbbi->Probe(EIID_IBreakIterator);
    if (*instance != NULL) {
        (*instance)->AddRef();
    }
    return NOERROR;
}

ECode BreakIterator::GetSentenceInstance(
    /* [out] */ IBreakIterator** instance)
{
    VALIDATE_NOT_NULL(instance);
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return GetSentenceInstance(locale, instance);
}

ECode BreakIterator::GetSentenceInstance(
    /* [in] */ ILocale* where,
    /* [out] */ IBreakIterator** instance)
{
    VALIDATE_NOT_NULL(instance);
    RuleBasedBreakIterator* rbbi = new RuleBasedBreakIterator(
            NativeBreakIterator::GetSentenceInstance(where));
    *instance = (IBreakIterator* )rbbi->Probe(EIID_IBreakIterator);
    if (*instance != NULL) {
        (*instance)->AddRef();
    }
    return NOERROR;
}

ECode BreakIterator::GetWordInstance(
    /* [out] */ IBreakIterator** instance)
{
    VALIDATE_NOT_NULL(instance);
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return GetWordInstance(locale, instance);
}

ECode BreakIterator::GetWordInstance(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance)
{
    VALIDATE_NOT_NULL(instance);
    RuleBasedBreakIterator * rbbi = new RuleBasedBreakIterator(
        NativeBreakIterator::GetWordInstance(where));
    *instance = (IBreakIterator* )rbbi->Probe(EIID_IBreakIterator);
    if (*instance != NULL) {
        (*instance)->AddRef();
    }
    return NOERROR;
}

ECode BreakIterator::IsBoundary(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* isBoundary)
{
    VALIDATE_NOT_NULL(isBoundary);
    *isBoundary = mWrapped->IsBoundary(offset);
    return NOERROR;
}

ECode BreakIterator::Preceding(
    /* [in] */ Int32 offset,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = mWrapped->Preceding(offset);
    return NOERROR;
}

ECode BreakIterator::SetText(
    /* [in] */ String newText)
{
    mWrapped->SetText(newText);
    return NOERROR;
}