
#include "CDateFormatSymbols.h"

ECode CDateFormatSymbols::constructor()
{
    return DateFormatSymbols::Init();
}

ECode CDateFormatSymbols::constructor(
    /* [in] */ ILocale * pLocale)
{
    return DateFormatSymbols::Init(pLocale);
}

ECode CDateFormatSymbols::GetLongStandAloneMonths(
    /* [out, callee] */ ArrayOf<String> ** ppLongStandAloneMonths)
{
    return DateFormatSymbols::GetLongStandAloneMonths(ppLongStandAloneMonths);
}

ECode CDateFormatSymbols::GetShortStandAloneMonths(
    /* [out, callee] */ ArrayOf<String> ** ppShortStandAloneMonths)
{
    return DateFormatSymbols::GetShortStandAloneMonths(ppShortStandAloneMonths);
}

ECode CDateFormatSymbols::GetLongStandAloneWeekdays(
    /* [out, callee] */ ArrayOf<String> ** ppLongStandAloneWeekdays)
{
    return DateFormatSymbols::GetLongStandAloneMonths(ppLongStandAloneWeekdays);
}

ECode CDateFormatSymbols::GetShortStandAloneWeekdays(
    /* [out, callee] */ ArrayOf<String> ** ppShortStandAloneWeekdays)
{
    return DateFormatSymbols::GetShortStandAloneWeekdays(ppShortStandAloneWeekdays);
}

ECode CDateFormatSymbols::GetCustomZoneStrings(
    /* [out] */ Boolean * pCustomZoneStrings)
{
    return DateFormatSymbols::GetCustomZoneStrings(pCustomZoneStrings);
}

ECode CDateFormatSymbols::GetLocale(
    /* [out] */ ILocale ** ppLocale)
{
    return DateFormatSymbols::GetLocale(ppLocale);
}

ECode CDateFormatSymbols::GetAmPmStrings(
    /* [out, callee] */ ArrayOf<String> ** ppAmPmStrings)
{
    return DateFormatSymbols::GetAmPmStrings(ppAmPmStrings);
}

ECode CDateFormatSymbols::GetEras(
    /* [out, callee] */ ArrayOf<String> ** ppEras)
{
    return DateFormatSymbols::GetEras(ppEras);
}

ECode CDateFormatSymbols::GetLocalPatternChars(
    /* [out] */ String * pLocalPatternChars)
{
    return DateFormatSymbols::GetLocalPatternChars(pLocalPatternChars);
}

ECode CDateFormatSymbols::GetMonths(
    /* [out, callee] */ ArrayOf<String> ** ppMonths)
{
    return DateFormatSymbols::GetMonths(ppMonths);
}

ECode CDateFormatSymbols::GetShortMonths(
    /* [out, callee] */ ArrayOf<String> ** ppShortMonths)
{
    return DateFormatSymbols::GetShortMonths(ppShortMonths);
}

ECode CDateFormatSymbols::GetShortWeekdays(
    /* [out, callee] */ ArrayOf<String> ** ppShortWeekdays)
{
    return DateFormatSymbols::GetShortWeekdays(ppShortWeekdays);
}

ECode CDateFormatSymbols::GetWeekdays(
    /* [out, callee] */ ArrayOf<String> ** ppWeekdays)
{
    return DateFormatSymbols::GetWeekdays(ppWeekdays);
}

ECode CDateFormatSymbols::SetAmPmStrings(
    /* [in] */ ArrayOf<String> * pData)
{
    return DateFormatSymbols::SetAmPmStrings(pData);
}

ECode CDateFormatSymbols::SetEras(
    /* [in] */ ArrayOf<String> * pData)
{
    return DateFormatSymbols::SetEras(pData);
}

ECode CDateFormatSymbols::SetLocalPatternChars(
    /* [in] */ const String& data)
{
    return DateFormatSymbols::SetLocalPatternChars(data);
}

ECode CDateFormatSymbols::SetMonths(
    /* [in] */ ArrayOf<String> * pData)
{
    return DateFormatSymbols::SetMonths(pData);
}

ECode CDateFormatSymbols::SetShortMonths(
    /* [in] */ ArrayOf<String> * pData)
{
    return DateFormatSymbols::SetShortMonths(pData);
}

ECode CDateFormatSymbols::SetShortWeekdays(
    /* [in] */ ArrayOf<String> * pData)
{
    return DateFormatSymbols::SetShortWeekdays(pData);
}

ECode CDateFormatSymbols::SetWeekdays(
    /* [in] */ ArrayOf<String> * pData)
{
    return DateFormatSymbols::SetWeekdays(pData);
}



