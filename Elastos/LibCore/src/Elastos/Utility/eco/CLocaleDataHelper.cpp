
#include "cmdef.h"
#include "CLocaleDataHelper.h"
#include "LocaleData.h"

ECode CLocaleDataHelper::Get(
    /* [in] */ ILocale* locale,
    /* [out] */ ILocaleData** localeData)
{
    VALIDATE_NOT_NULL(localeData);

    *localeData = LocaleData::Get(locale);
    if (*localeData) {
        (*localeData)->AddRef();
    }

    return NOERROR;
}
