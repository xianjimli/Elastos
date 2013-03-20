
#ifndef __CDATEFORMATSYMBOLS_H__
#define __CDATEFORMATSYMBOLS_H__

#include "_CDateFormatSymbols.h"
#include "DateFormatSymbols.h"

CarClass(CDateFormatSymbols), public DateFormatSymbols
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ILocale * pLocale);

    CARAPI GetLongStandAloneMonths(
        /* [out, callee] */ ArrayOf<String> ** ppLongStandAloneMonths);

    CARAPI GetShortStandAloneMonths(
        /* [out, callee] */ ArrayOf<String> ** ppShortStandAloneMonths);

    CARAPI GetLongStandAloneWeekdays(
        /* [out, callee] */ ArrayOf<String> ** ppLongStandAloneWeekdays);

    CARAPI GetShortStandAloneWeekdays(
        /* [out, callee] */ ArrayOf<String> ** ppShortStandAloneWeekdays);

    CARAPI GetCustomZoneStrings(
        /* [out] */ Boolean * pCustomZoneStrings);

    CARAPI GetLocale(
        /* [out] */ ILocale ** ppLocale);  

    CARAPI GetAmPmStrings(
        /* [out, callee] */ ArrayOf<String> ** ppAmPmStrings);

    CARAPI GetEras(
        /* [out, callee] */ ArrayOf<String> ** ppEras);

    CARAPI GetLocalPatternChars(
        /* [out] */ String * pLocalPatternChars);

    CARAPI GetMonths(
        /* [out, callee] */ ArrayOf<String> ** ppMonths);

    CARAPI GetShortMonths(
        /* [out, callee] */ ArrayOf<String> ** ppShortMonths);

    CARAPI GetShortWeekdays(
        /* [out, callee] */ ArrayOf<String> ** ppShortWeekdays);

    CARAPI GetWeekdays(
        /* [out, callee] */ ArrayOf<String> ** ppWeekdays);

    CARAPI SetAmPmStrings(
        /* [in] */ ArrayOf<String> * pData);

    CARAPI SetEras(
        /* [in] */ ArrayOf<String> * pData);

    CARAPI SetLocalPatternChars(
        /* [in] */ const String& data);

    CARAPI SetMonths(
        /* [in] */ ArrayOf<String> * pData);

    CARAPI SetShortMonths(
        /* [in] */ ArrayOf<String> * pData);

    CARAPI SetShortWeekdays(
        /* [in] */ ArrayOf<String> * pData);

    CARAPI SetWeekdays(
        /* [in] */ ArrayOf<String> * pData);

private:
    // TODO: Add your private member variables here.
};

#endif // __CDATEFORMATSYMBOLS_H__
