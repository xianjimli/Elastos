
#include "CCurrencyHelper.h"
#include "Currency.h"

ECode CCurrencyHelper::GetInstance(
    /* [in] */ const String& currencyCode,
    /* [out] */ ICurrency** currency)
{
    VALIDATE_NOT_NULL(currency);
    *currency = Currency::GetInstance(currencyCode);
    if (*currency) {
        (*currency)->AddRef();
    }
    return NOERROR;
}

ECode CCurrencyHelper::GetInstanceEx(
    /* [in] */ ILocale* locale,
    /* [out] */ ICurrency** currency)
{
    return E_NOT_IMPLEMENTED;
    // VALIDATE_NOT_NULL(currency);
    // *currency = Currency::GetInstance(locale);
    // if (*currency) {
    //     (*currency)->AddRef();
    // }
    // return NOERROR;
}
