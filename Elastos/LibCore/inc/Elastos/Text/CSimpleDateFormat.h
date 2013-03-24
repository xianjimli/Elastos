
#ifndef __CSIMPLEDATEFORMAT_H__
#define __CSIMPLEDATEFORMAT_H__

#include "_CSimpleDateFormat.h"
#include "SimpleDateFormat.h"

CarClass(CSimpleDateFormat), public SimpleDateFormat
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& pattern);

    CARAPI constructor(
        /* [in] */ const String& tem,
        /* [in] */ IDateFormatSymbols * pValue);

    CARAPI constructor(
        /* [in] */ const String& tem,
        /* [in] */ ILocale * pLocale);

    CARAPI FormatObject(
        /* [in] */ IInterface * pObject,
        /* [out] */ String * pValue);

    CARAPI FormatObjectEx(
        /* [in] */ IInterface* object,
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ String* value);

    CARAPI FormatToCharacterIterator(
        /* [in] */ IInterface * pObject,
        /* [out] */ IAttributedCharacterIterator ** ppCharactorIterator);

    CARAPI ParseObject(
        /* [in] */ const String& string,
        /* [out] */ IInterface ** ppObject);

    CARAPI ParseObjectEx(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition * pPosition,
        /* [out] */ IInterface ** ppObject);

    CARAPI FormatDate(
        /* [in] */ IDate * pDate,
        /* [out] */ String * pFormatString);

    CARAPI FormatDateEx(
        /* [in] */ IDate* date,
        /* [in] */ const String& buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ String* result);

    CARAPI GetCalendar(
        /* [out] */ ICalendar ** ppCalendar);

    CARAPI GetNumberFormat(
        /* [out] */ INumberFormat** numberFormat);

    CARAPI GetTimeZone(
        /* [out] */ ITimeZone ** ppTimezone);

    CARAPI IsLenient(
        /* [out] */ Boolean * pIsLenient);

    CARAPI Parse(
        /* [in] */ const String& string,
        /* [out] */ IDate ** ppDate);

    CARAPI ParseEx(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition * pPosition,
        /* [out] */ IDate ** ppDate);

    CARAPI SetCalendar(
        /* [in] */ ICalendar * pCal);

    CARAPI SetLenient(
        /* [in] */ Boolean value);

    CARAPI SetNumberFormat(
       /* [in] */ INumberFormat* format);

    CARAPI SetTimeZone(
        /* [in] */ ITimeZone * pTimezone);

    CARAPI ApplyLocalizedPattern(
        /* [in] */ const String& tem);

    CARAPI ApplyPattern(
        /* [in] */ const String& tem);

    CARAPI Get2DigitYearStart(
        /* [out] */ IDate ** ppDate);

    CARAPI GetDateFormatSymbols(
        /* [out] */ IDateFormatSymbols ** ppDateFormatSymbols);

    CARAPI Set2DigitYearStart(
        /* [in] */ IDate * pDate);

    CARAPI SetDateFormatSymbols(
        /* [in] */ IDateFormatSymbols * pValue);

    CARAPI ToLocalizedPattern(
        /* [out] */ String * pPattern);

    CARAPI ToPattern(
        /* [out] */ String * pPattern);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSIMPLEDATEFORMAT_H__
