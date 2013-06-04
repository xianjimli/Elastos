
#include "cmdef.h"
#include "CLocaleDataHelper.h"
#include "LocaleData.h"

ECode CLocaleDataHelper::Get(
    /* [in] */ ILocale* locale,
    /* [out] */ ILocaleData** localeData)
{
    VALIDATE_NOT_NULL(localeData);

    AutoPtr<ILocaleData> temp = LocaleData::Get(locale);
    *localeData = temp;
    if (*localeData) {
        (*localeData)->AddRef();
    }

    return NOERROR;
}
