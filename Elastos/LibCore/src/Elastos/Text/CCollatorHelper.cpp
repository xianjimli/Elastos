
#include "CCollatorHelper.h"
#include "CRuleBasedCollator.h"
#include "stdio.h"

ECode CCollatorHelper::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale *> ** locales)
{
    AutoPtr<IICUHelper> picu;
    CICUHelper::AcquireSingleton((IICUHelper**)&picu);
    return picu->GetAvailableCollatorLocales(locales);
}

ECode CCollatorHelper::GetInstance(
    /* [out] */ ICollator ** instance)
{
printf("%s %d\n", __FILE__, __LINE__);
    AutoPtr<ILocaleHelper> localeHelper;
printf("%s %d\n", __FILE__, __LINE__);
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
printf("%s %d\n", __FILE__, __LINE__);
    AutoPtr<ILocale> locale;
printf("%s %d\n", __FILE__, __LINE__);
    localeHelper->GetDefault((ILocale**)&locale);
printf("%s %d\n", __FILE__, __LINE__);
    return GetInstanceEx(locale, instance);
}

ECode CCollatorHelper::GetInstanceEx(
    /* [in] */ ILocale * locale,
    /* [out] */ ICollator ** instance)
{
printf("%s %d\n", __FILE__, __LINE__);
    if (locale == NULL) {
printf("%s %d\n", __FILE__, __LINE__);
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
printf("%s %d\n", __FILE__, __LINE__);
    AutoPtr<IRuleBasedCollator> rbc;
printf("%s %d\n", __FILE__, __LINE__);
    CRuleBasedCollator::New(ICUCollator::GetInstance(locale), (IRuleBasedCollator**)&rbc);
printf("%s %d\n", __FILE__, __LINE__);
    *instance = reinterpret_cast<ICollator*>(rbc->Probe(EIID_ICollator));
printf("%s %d\n", __FILE__, __LINE__);
    return NOERROR;
}


