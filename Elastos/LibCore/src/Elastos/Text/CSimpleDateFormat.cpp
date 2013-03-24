
#include "CSimpleDateFormat.h"

ECode CSimpleDateFormat::constructor()
{
    return SimpleDateFormat::Init();
}

ECode CSimpleDateFormat::constructor(
    /* [in] */ const String& pattern)
{
    return SimpleDateFormat::Init(pattern);
}

ECode CSimpleDateFormat::constructor(
    /* [in] */ const String& tem,
    /* [in] */ IDateFormatSymbols * pValue)
{
    return SimpleDateFormat::Init(tem, pValue);
}

ECode CSimpleDateFormat::constructor(
    /* [in] */ const String& tem,
    /* [in] */ ILocale * pLocale)
{
    return SimpleDateFormat::Init(tem, pLocale);
}

ECode CSimpleDateFormat::FormatObject(
    /* [in] */ IInterface * pObject,
    /* [out] */ String * pValue)
{
    return SimpleDateFormat::FormatObject(pObject, pValue);
}

ECode CSimpleDateFormat::FormatObjectEx(
    /* [in] */ IInterface* object,
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ String* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSimpleDateFormat::FormatToCharacterIterator(
    /* [in] */ IInterface * pObject,
    /* [out] */ IAttributedCharacterIterator ** ppCharactorIterator)
{
    return SimpleDateFormat::FormatToCharacterIterator(pObject, ppCharactorIterator);
}

ECode CSimpleDateFormat::ParseObject(
    /* [in] */ const String& string,
    /* [out] */ IInterface ** ppObject)
{
    return SimpleDateFormat::ParseObject(string, ppObject);
}

ECode CSimpleDateFormat::ParseObjectEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition * pPosition,
    /* [out] */ IInterface ** ppObject)
{
    return SimpleDateFormat::ParseObjectEx(string, pPosition, ppObject);
}

ECode CSimpleDateFormat::FormatDate(
    /* [in] */ IDate * pDate,
    /* [out] */ String * pFormatString)
{
    return SimpleDateFormat::FormatDate(pDate, pFormatString);
}

ECode CSimpleDateFormat::FormatDateEx(
    /* [in] */ IDate* date,
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSimpleDateFormat::GetCalendar(
    /* [out] */ ICalendar ** ppCalendar)
{
    return SimpleDateFormat::GetCalendar(ppCalendar);
}

ECode CSimpleDateFormat::GetNumberFormat(
    /* [out] */ INumberFormat** numberFormat)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSimpleDateFormat::GetTimeZone(
    /* [out] */ ITimeZone ** ppTimezone)
{
    return SimpleDateFormat::GetTimeZone(ppTimezone);
}

ECode CSimpleDateFormat::IsLenient(
    /* [out] */ Boolean * pIsLenient)
{
    return SimpleDateFormat::IsLenient(pIsLenient);
}

ECode CSimpleDateFormat::Parse(
    /* [in] */ const String& string,
    /* [out] */ IDate ** ppDate)
{
    return DateFormat::Parse(string, ppDate);
}

ECode CSimpleDateFormat::ParseEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition * pPosition,
    /* [out] */ IDate ** ppDate)
{
    return SimpleDateFormat::ParseEx(string, pPosition, ppDate);
}

ECode CSimpleDateFormat::SetCalendar(
    /* [in] */ ICalendar * pCal)
{
    return SimpleDateFormat::SetCalendar(pCal);
}

ECode CSimpleDateFormat::SetLenient(
    /* [in] */ Boolean value)
{
    return SimpleDateFormat::SetLenient(value);
}

ECode CSimpleDateFormat::SetNumberFormat(
    /* [in] */ INumberFormat* format)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSimpleDateFormat::SetTimeZone(
    /* [in] */ ITimeZone * pTimezone)
{
    return SimpleDateFormat::SetTimeZone(pTimezone);
}

ECode CSimpleDateFormat::ApplyLocalizedPattern(
    /* [in] */ const String& tem)
{
    return SimpleDateFormat::ApplyLocalizedPattern(tem);
}

ECode CSimpleDateFormat::ApplyPattern(
    /* [in] */ const String& tem)
{
    return SimpleDateFormat::ApplyPattern(tem);
}

ECode CSimpleDateFormat::Get2DigitYearStart(
    /* [out] */ IDate ** ppDate)
{
    return SimpleDateFormat::Get2DigitYearStart(ppDate);
}

ECode CSimpleDateFormat::GetDateFormatSymbols(
    /* [out] */ IDateFormatSymbols ** ppDateFormatSymbols)
{
    return SimpleDateFormat::GetDateFormatSymbols(ppDateFormatSymbols);
}

ECode CSimpleDateFormat::Set2DigitYearStart(
    /* [in] */ IDate * pDate)
{
    return SimpleDateFormat::Set2DigitYearStart(pDate);
}

ECode CSimpleDateFormat::SetDateFormatSymbols(
    /* [in] */ IDateFormatSymbols * pValue)
{
    return SimpleDateFormat::SetDateFormatSymbols(pValue);
}

ECode CSimpleDateFormat::ToLocalizedPattern(
    /* [out] */ String * pPattern)
{
    return SimpleDateFormat::ToLocalizedPattern(pPattern);
}

ECode CSimpleDateFormat::ToPattern(
    /* [out] */ String * pPattern)
{
    return SimpleDateFormat::ToPattern(pPattern);
}



