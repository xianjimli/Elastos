
#include "CCollatorHelper.h"
#include "Collator.h"

ECode CCollatorHelper::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    return Collator::GetAvailableLocales(locales);
}

ECode CCollatorHelper::GetInstance(
    /* [out] */ ICollator** instance)
{
    return Collator::GetInstance(instance);
}

ECode CCollatorHelper::GetInstanceEx(
    /* [in] */ ILocale* locale,
    /* [out] */ ICollator** instance)
{
    return Collator::GetInstance(locale, instance);
}
