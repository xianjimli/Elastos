#include "SimpleDateFormat.h"
#include "CDateFormatSymbols.h"
#include "NumberFormat.h"
#include "CAttributedString.h"

const String SimpleDateFormat::PATTERN_CHARS = String("GyMdkHmsSEDFwWahKzZLc");

PInterface SimpleDateFormat::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

ECode SimpleDateFormat::Init()
{
    AutoPtr<ILocaleHelper> pILocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pILocaleHelper);
    AutoPtr<ILocale> pILocale;
    pILocaleHelper->GetDefault((ILocale**)&pILocale);
    Init((ILocale*)pILocale);
    DefaultPattern(&pattern);
    CDateFormatSymbols::New((ILocale*)pILocale, (IDateFormatSymbols**)&formatData);
    return NOERROR;
}

ECode SimpleDateFormat::Init(
    /* [in] */ String pattern)
{
    AutoPtr<ILocaleHelper> pILocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pILocaleHelper);
    AutoPtr<ILocale> pILocale;
    pILocaleHelper->GetDefault((ILocale**)&pILocale);
    Init(pattern, (ILocale*)pILocale);
    return NOERROR;
}

ECode SimpleDateFormat::ValidateFormat(
    /* [in] */ Char32 format)
{
    Int32 index = PATTERN_CHARS.IndexOf(format);
    if (index == -1) {
        //throw new IllegalArgumentException("Unknown pattern character '" + format + "'");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode SimpleDateFormat::ValidatePattern(
    /* [in] */ String tem)
{
    Boolean quote = FALSE;
    Int32 next, last = -1, count = 0;

    const Int32 patternLength = (Int32)tem.GetLength();
    for (Int32 i = 0; i < patternLength; i++) {
        next = (tem.GetChar(i));
        if (next == '\'') {
            if (count > 0) {
                ValidateFormat((Char32) last);
                count = 0;
            }
            if (last == next) {
                last = -1;
            } else {
                last = next;
            }
            quote = !quote;
            continue;
        }
        if (!quote
                && (last == next || (next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z'))) {
            if (last == next) {
                count++;
            } else {
                if (count > 0) {
                    ValidateFormat((Char32) last);
                }
                last = next;
                count = 1;
            }
        } else {
            if (count > 0) {
                ValidateFormat((Char32) last);
                count = 0;
            }
            last = -1;
        }
    }
    if (count > 0) {
        ValidateFormat((Char32) last);
    }

    if (quote) {
        //throw new IllegalArgumentException("Unterminated quote");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode SimpleDateFormat::Init(
    /* [in] */ String tem,
    /* [in] */ IDateFormatSymbols* value)
{
    AutoPtr<ILocaleHelper> pILocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pILocaleHelper);
    AutoPtr<ILocale> pILocale;
    pILocaleHelper->GetDefault((ILocale**)&pILocale);
    Init((ILocale*)pILocale);
    ValidatePattern(tem);
    pattern = tem;
    //formatData = (DateFormatSymbols) value.clone();
    formatData = (IDateFormatSymbols*)(value->Probe(EIID_IDateFormatSymbols) );
    return NOERROR;
}

ECode SimpleDateFormat::Init(
    /* [in] */ String tem,
    /* [in] */ ILocale* locale)
{
    this->Init(locale);
    ValidatePattern(tem);
    pattern = tem;
    AutoPtr<ILocaleHelper> pILocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pILocaleHelper);
    AutoPtr<ILocale> pILocale;
    pILocaleHelper->GetDefault((ILocale**)&pILocale);
    CDateFormatSymbols::New((ILocale*)pILocale, (IDateFormatSymbols**)&formatData);
    return NOERROR;
}

ECode SimpleDateFormat::Init(
    /* [in] */ ILocale* locale)
{
    NumberFormat::GetInstance(locale, (INumberFormat**)&mNumberFormat);
    mNumberFormat->SetParseIntegerOnly(TRUE);
    mNumberFormat->SetGroupingUsed(FALSE);
    CGregorianCalendar::New(locale, (IGregorianCalendar**)&mCalendar);
    mCalendar->Add(Calendar_YEAR, -80);
    mCalendar->Get(Calendar_YEAR, &creationYear);
    mCalendar->GetTime((IDate**)&defaultCenturyStart);
    return NOERROR;
}

SimpleDateFormat::~SimpleDateFormat()
{

}

ECode SimpleDateFormat::ApplyLocalizedPattern(
    /* [in] */ String tem)
{
    String fromChars;
    formatData->GetLocalPatternChars(&fromChars);
    ConvertPattern(tem, fromChars, PATTERN_CHARS, TRUE, &pattern);
    return NOERROR;
}

ECode SimpleDateFormat::ApplyPattern(
    /* [in] */ String tem)
{
    ValidatePattern(tem);
    pattern = tem;
    return NOERROR;
}

ECode SimpleDateFormat::DefaultPattern(
    /* [out] */ String* pattern)
{
//    LocaleData localeData = LocaleData.get(Locale.getDefault());
//    return localeData.getDateFormat(SHORT) + " " + localeData.getTimeFormat(SHORT);
    return NOERROR;
}

ECode SimpleDateFormat::FormatToCharacterIterator(
    /* [in] */ IInterface* object,
    /* [out] */ IAttributedCharacterIterator** charactorIterator)
{
    if (object == NULL) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if (object->Probe(EIID_IDate) != NULL) {
        FormatToCharacterIteratorImpl((IDate*) object, charactorIterator);
        return NOERROR;
    }
    if (object->Probe(EIID_INumber) != NULL ) {
        AutoPtr<IDate> pIDate;
        Int64 pValue;
        AutoPtr<INumber> pINumber = reinterpret_cast<INumber*>(object->Probe(EIID_INumber));
        pINumber->Int64Value(&pValue);
        CDate::New(pValue, (IDate**)&pIDate);
        return FormatToCharacterIteratorImpl(pIDate, charactorIterator);
    }
    //throw new IllegalArgumentException();
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode SimpleDateFormat::FormatToCharacterIteratorImpl(
    /* [in] */ IDate* date,
    /* [out] */ IAttributedCharacterIterator** charactorIterator)
{
    StringBuffer* buffer = new StringBuffer(NULL);
    Vector<IFieldPosition* >* fields = new Vector<IFieldPosition* >();

    StringBuffer* formattedDate = new StringBuffer("");
    // format the date, and find fields
    FormatImpl(date, buffer, NULL, fields, formattedDate);

    // create and AttributedString with the formatted buffer
    AutoPtr<IAttributedString> as;
    CAttributedString::New(buffer->Substring(0, buffer->GetLength()), (IAttributedString**)&as);

    // add DateFormat field attributes to the AttributedString
    for (Int32 i = 0; i < (Int32)(fields->GetSize() ); i++) {
        AutoPtr<IFieldPosition> pos = (*fields)[i];
        AutoPtr<IFormatField> attribute;
        pos->GetFieldAttribute((IFormatField**)&attribute);
        IInterface * object = reinterpret_cast<IInterface*>(attribute->Probe(EIID_IInterface));
        Int32 bi, ei;
        pos->GetBeginIndex(&bi);
        pos->GetEndIndex(&ei);
        as->AddAttributeEx(attribute, object, bi, ei);
    }

    // return the CharacterIterator from AttributedString
    return as->GetIterator(charactorIterator);
}

ECode SimpleDateFormat::FormatImpl(
    /* [in] */ IDate* date,
    /* [in] */ StringBuffer* buffer,
    /* [in] */ IFieldPosition* field,
    /* [in] */ Vector< IFieldPosition* >* fields,
    /* [out] */ StringBuffer* formattedDate)
{
    VALIDATE_NOT_NULL(formattedDate);

    Boolean quote = FALSE;
    Int32 next, last = -1, count = 0;
    mCalendar->SetTime(date);
    if (field != NULL) {
        ((CFieldPosition*)field)->Clear();
    }

    const Int32 patternLength = (Int32)(pattern.GetLength() );
    for (Int32 i = 0; i < patternLength; i++) {
        next = (pattern.GetChar(i));
        if (next == '\'') {
            if (count > 0) {
                Append(buffer, field, fields, (Char32) last, count);
                count = 0;
            }
            if (last == next) {
                (*buffer) += '\'';
                last = -1;
            } else {
                last = next;
            }
            quote = !quote;
            continue;
        }
        if (!quote
                && (last == next || (next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z'))) {
            if (last == next) {
                count++;
            } else {
                if (count > 0) {
                    Append(buffer, field, fields, (Char32) last, count);
                }
                last = next;
                count = 1;
            }
        } else {
            if (count > 0) {
                Append(buffer, field, fields, (Char32) last, count);
                count = 0;
            }
            last = -1;
            (*buffer) += (Char32)next;
        }
    }
    if (count > 0) {
        Append(buffer, field, fields, (Char32) last, count);
    }
    *formattedDate = *buffer;
    return NOERROR;
}

ECode SimpleDateFormat::Append(
    /* [in] */ StringBuffer* buffer,
    /* [in] */ IFieldPosition* position,
    /* [in] */ Vector< IFieldPosition* >* fields,
    /* [in] */ Char32 format,
    /* [in] */ Int32 count)
{
    Int32 field = -1;
    Int32 index = PATTERN_CHARS.IndexOf(format);
    if (index == -1) {
        //throw new IllegalArgumentException("Unknown pattern character '" + format + "'");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 beginPosition = buffer->GetLength();
    AutoPtr<IDateFormatField> dateFormatField;
    switch (index) {
        case IDateFormat_ERA_FIELD:
            dateFormatField = DateFormat::Field::ERA;
            Int32 pValue;
            mCalendar->Get(Calendar_ERA, &pValue);
            ArrayOf<String> * arrayOfStrings;
            formatData->GetEras(&arrayOfStrings);
            (*buffer) += (*arrayOfStrings)[pValue];
            break;
        case IDateFormat_YEAR_FIELD:
            dateFormatField = DateFormat::Field::YEAR;
            Int32 year;
            mCalendar->Get(Calendar_YEAR, &year);
            /*
             * For 'y' and 'yyy', we're consistent with Unicode and previous releases
             * of Android. But this means we're inconsistent with the RI.
             *     http://unicode.org/reports/tr35/
             */

            if (count == 2) {
                AppendNumber(buffer, 2, year %= 100);
            } else {
                AppendNumber(buffer, count, year);
            }
            break;
        case STAND_ALONE_MONTH_FIELD: // L
            dateFormatField = DateFormat::Field::MONTH;
            ArrayOf<String> *longStandAloneMonths, *shortStandAloneMonths;
            formatData->GetLongStandAloneMonths(&longStandAloneMonths);
            formatData->GetShortStandAloneMonths(&shortStandAloneMonths);
            AppendMonth(buffer, count, longStandAloneMonths, shortStandAloneMonths);
            break;
        case IDateFormat_MONTH_FIELD: // M
            dateFormatField = DateFormat::Field::MONTH;
            ArrayOf<String> *arrayOfStrings1, *arrayOfStrings2;
            formatData->GetMonths(&arrayOfStrings1);
            formatData->GetShortMonths(&arrayOfStrings2);
            AppendMonth(buffer, count, arrayOfStrings1, arrayOfStrings2);
            break;
        case IDateFormat_DATE_FIELD:
            dateFormatField = DateFormat::Field::DAY_OF_MONTH;
            field = Calendar_DATE;
            break;
        case IDateFormat_HOUR_OF_DAY1_FIELD: // k
            dateFormatField = DateFormat::Field::HOUR_OF_DAY1;
            Int32 hour;
            mCalendar->Get(Calendar_HOUR_OF_DAY, &hour);
            AppendNumber(buffer, count, hour == 0 ? 24 : hour);
            break;
        case IDateFormat_HOUR_OF_DAY0_FIELD: // H
            dateFormatField = DateFormat::Field::HOUR_OF_DAY0;
            field = Calendar_HOUR_OF_DAY;
            break;
        case IDateFormat_MINUTE_FIELD:
            dateFormatField = DateFormat::Field::MINUTE;
            field = Calendar_MINUTE;
            break;
        case IDateFormat_SECOND_FIELD:
            dateFormatField = DateFormat::Field::SECOND;
            field = Calendar_SECOND;
            break;
        case IDateFormat_MILLISECOND_FIELD:
            dateFormatField = DateFormat::Field::MILLISECOND;
            Int32 value;
            mCalendar->Get(Calendar_MILLISECOND, &value);
            AppendNumber(buffer, count, value);
            break;
        case STAND_ALONE_DAY_OF_WEEK_FIELD:
            dateFormatField = DateFormat::Field::DAY_OF_WEEK;
            ArrayOf<String> *longStandAloneWeekdays, *shortStandAloneWeekdays;
            formatData->GetLongStandAloneWeekdays(&longStandAloneWeekdays);
            formatData->GetShortStandAloneWeekdays(&shortStandAloneWeekdays);
            AppendDayOfWeek(buffer, count, longStandAloneWeekdays, shortStandAloneWeekdays);
            break;
        case IDateFormat_DAY_OF_WEEK_FIELD:
            dateFormatField = DateFormat::Field::DAY_OF_WEEK;
            ArrayOf<String> *arrayOfStrings3, *arrayOfStrings4;
            formatData->GetWeekdays(&arrayOfStrings3);
            formatData->GetShortWeekdays(&arrayOfStrings4);
            AppendDayOfWeek(buffer, count, arrayOfStrings3, arrayOfStrings4);
            break;
        case IDateFormat_DAY_OF_YEAR_FIELD:
            dateFormatField = DateFormat::Field::DAY_OF_YEAR;
            field = Calendar_DAY_OF_YEAR;
            break;
        case IDateFormat_DAY_OF_WEEK_IN_MONTH_FIELD:
            dateFormatField = DateFormat::Field::DAY_OF_WEEK_IN_MONTH;
            field = Calendar_DAY_OF_WEEK_IN_MONTH;
            break;
        case IDateFormat_WEEK_OF_YEAR_FIELD:
            dateFormatField = DateFormat::Field::WEEK_OF_YEAR;
            field = Calendar_WEEK_OF_YEAR;
            break;
        case IDateFormat_WEEK_OF_MONTH_FIELD:
            dateFormatField = DateFormat::Field::WEEK_OF_MONTH;
            field = Calendar_WEEK_OF_MONTH;
            break;
        case IDateFormat_AM_PM_FIELD:
            dateFormatField = DateFormat::Field::AM_PM;
            Int32 pValue2;
            mCalendar->Get(Calendar_AM_PM, &pValue2);
            ArrayOf<String> * arrayOfStrings5;
            formatData->GetAmPmStrings(&arrayOfStrings5);
            (*buffer) += (*arrayOfStrings5)[pValue2];
            break;
        case IDateFormat_HOUR1_FIELD: // h
            dateFormatField = DateFormat::Field::HOUR1;
            mCalendar->Get(Calendar_HOUR, &hour);
            AppendNumber(buffer, count, hour == 0 ? 12 : hour);
            break;
        case IDateFormat_HOUR0_FIELD: // K
            dateFormatField = DateFormat::Field::HOUR0;
            field = Calendar_HOUR;
            break;
        case IDateFormat_TIMEZONE_FIELD: // z
            dateFormatField = DateFormat::Field::TIME_ZONE;
            AppendTimeZone(buffer, count, TRUE);
            break;
        case RFC_822_TIMEZONE_FIELD: // Z
            dateFormatField = DateFormat::Field::TIME_ZONE;
            AppendNumericTimeZone(buffer, FALSE);
            break;
    }
    if (field != -1) {
        Int32 value;
        mCalendar->Get(field, &value);
        AppendNumber(buffer, count, value);
    }

    if (fields != NULL) {
        CFieldPosition::New(dateFormatField, &position);
        position->SetBeginIndex(beginPosition);
        position->SetEndIndex(buffer->GetLength());
        fields->PushBack(position);
    } else {
        // Set to the first occurrence
        AutoPtr<IFormatField> ff;
        position->GetFieldAttribute((IFormatField**)&ff);
        Int32 f, ei;
        position->GetField(&f);
        position->GetEndIndex(&ei);
        if ( (ff == dateFormatField || ff == NULL) && (f == index) && (ei == 0) ) {
            position->SetBeginIndex(beginPosition);
            position->SetEndIndex(buffer->GetLength());
        }
    }
    return NOERROR;
}

ECode SimpleDateFormat::AppendDayOfWeek(
    /* [in] */ StringBuffer* buffer,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<String>* longs,
    /* [in] */ ArrayOf<String>* shorts)
{
    Boolean isLong = (count > 3);
    ArrayOf<String>* days = isLong ? longs->Clone() : shorts->Clone();
    Int32 pValue;
    mCalendar->Get(Calendar_DAY_OF_WEEK, &pValue);
    (*buffer) += (*days)[pValue];
    return NOERROR;
}

ECode SimpleDateFormat::AppendMonth(
    /* [in] */ StringBuffer* buffer,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<String>* longs,
    /* [in] */ ArrayOf<String>* shorts)
{
    Int32 month;
    mCalendar->Get(Calendar_MONTH, &month);
    if (count <= 2) {
        AppendNumber(buffer, count, month + 1);
        return NULL;
    }

    Boolean isLong = (count > 3);
    ArrayOf<String>* months = isLong ? longs->Clone() : shorts->Clone();
    (*buffer) += (*months)[month];
    return NOERROR;
}

ECode SimpleDateFormat::AppendTimeZone(
    /* [in] */ StringBuffer* buffer,
    /* [in] */ Int32 count,
    /* [in] */ Boolean generalTimeZone)
{
    if (generalTimeZone) {
        AutoPtr<ITimeZone> tz;
        mCalendar->GetTimeZone((ITimeZone**)&tz);
        Int32 pValue;
        mCalendar->Get(Calendar_DST_OFFSET, &pValue);
        Boolean daylight = (pValue != 0) ? TRUE : FALSE;
        Int32 style = (count < 4) ? 0 : 1;
        Boolean customZoneStrings;
        formatData->GetCustomZoneStrings(&customZoneStrings);
        if (!customZoneStrings) {
            String *name = new String(NULL);
            AutoPtr<ILocale> locale;
            formatData->GetLocale((ILocale**)&locale);
            tz->GetDisplayNameEx3(daylight, style, (ILocale*)locale, name);
            (*buffer) += (*name);
            return NULL;
        }
        // We can't call TimeZone.getDisplayName() because it would not use
        // the custom DateFormatSymbols of this SimpleDateFormat.
        String *ID = new String(NULL);
        tz->GetID(ID);
        //String custom = TimeZones.lookupDisplayName(formatData.zoneStrings, tz.getID(), daylight, style);
        //if (custom != null) {
        //    buffer.append(custom);
        //    return;
        //}
    }
    // We didn't find what we were looking for, so default to a numeric time zone.
    AppendNumericTimeZone(buffer, generalTimeZone);
    return NOERROR;
}

ECode SimpleDateFormat::AppendNumericTimeZone(
    /* [in] */ StringBuffer* buffer,
    /* [in] */ Boolean generalTimeZone)
{
    Int32 pValue1, pValue2;
    mCalendar->Get(Calendar_ZONE_OFFSET, &pValue1);
    mCalendar->Get(Calendar_DST_OFFSET, &pValue2);
    Int32 offset = pValue1 + pValue2;
    Char32 sign = '+';
    if (offset < 0) {
        sign = '-';
        offset = -offset;
    }
    if (generalTimeZone) {
        (*buffer) += "GMT";
    }
    (*buffer) += sign;
    AppendNumber(buffer, 2, offset / 3600000);
    if (generalTimeZone) {
        (*buffer) += ':';
    }
    AppendNumber(buffer, 2, (offset % 3600000) / 60000);
    return NOERROR;
}

ECode SimpleDateFormat::AppendNumber(
    /* [in] */ StringBuffer* buffer,
    /* [in] */ Int32 count,
    /* [in] */ Int32 value)
{
    Int32 minimumIntegerDigits;
    mNumberFormat->GetMinimumIntegerDigits(&minimumIntegerDigits);
    mNumberFormat->SetMinimumIntegerDigits(count);
    AutoPtr<IFieldPosition> fp;
    CFieldPosition::New(0, (IFieldPosition**)&fp);
    String result("");
    //mNumberFormat->FormatObjectEx(Integer.valueOf(value), buffer, fp, &result);
    mNumberFormat->SetMinimumIntegerDigits(minimumIntegerDigits);
    return NOERROR;
}

ECode SimpleDateFormat::Error(
    /* [in] */ IParsePosition* position,
    /* [in] */ Int32 offset,
    /* [in] */ ITimeZone* zone,
    /* [out] */ IDate** date)
{
    position->SetErrorIndex(offset);
    mCalendar->SetTimeZone(zone);
    *date = NULL;
    return NOERROR;
}

ECode SimpleDateFormat::FormatDateEx(
    /* [in] */ IDate* date,
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* fieldPos,
    /* [out] */ String* formatString)
{
    VALIDATE_NOT_NULL(formatString);
    // Harmony delegates to ICU's SimpleDateFormat, we implement it directly
    StringBuffer* sb = new StringBuffer(buffer);
    StringBuffer* result = new StringBuffer(*formatString);
    FormatImpl(date, sb, fieldPos, NULL, result);
    *formatString = result->Substring(0, result->GetLength());
    return NOERROR;
}

ECode SimpleDateFormat::Get2DigitYearStart(
    /* [out] */ IDate** start)
{
    *start = defaultCenturyStart;
    (*start)->AddRef();
    return NOERROR;
}

ECode SimpleDateFormat::GetDateFormatSymbols(
    /* [out] */ IDateFormatSymbols** symbols)
{
    *symbols = formatData;
    (*symbols)->AddRef();
    return NOERROR;
}

ECode SimpleDateFormat::Parse(
    /* [in] */ String string,
    /* [in] */ Int32 offset,
    /* [in] */ Char32 format,
    /* [in] */ Int32 count,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 index = PATTERN_CHARS.IndexOf(format);
    if (index == -1) {
        //throw new IllegalArgumentException("Unknown pattern character '" + format + "'");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 field = -1;
    // TODO: what's 'absolute' for? when is 'count' negative, and why?
    Int32 absolute = 0;
    if (count < 0) {
        count = -count;
        absolute = count;
    }
    AutoPtr<IParsePosition> position;
    AutoPtr<INumber> result;
    Int32 errorIndex;
    ArrayOf<String> *array1, *array2;
    switch (index) {
        case IDateFormat_ERA_FIELD:
            formatData->GetEras(&array1);
            return ParseText(string, offset, array1, Calendar_ERA, value);
        case IDateFormat_YEAR_FIELD:
            if (count >= 3) {
                field = Calendar_YEAR;
            } else {
                CParsePosition::New(offset, (IParsePosition**)&position);
                ParseNumber(absolute, string, position, (INumber**)&result);
                if (result == NULL) {
                    position->GetErrorIndex(&errorIndex);
                    *value = -errorIndex - 1;
                    return NOERROR;
                }
                Int32 year;
                result->Int32Value(&year);
                // A two digit year must be exactly two digits, i.e. 01
                position->GetIndex(&index);
                if ((index - offset) == 2 && year >= 0) {
                    year += creationYear / 100 * 100;
                    if (year < creationYear) {
                        year += 100;
                    }
                }
                mCalendar->Set(Calendar_YEAR, year);
                position->GetIndex(&index);
                *value = index;
                return NOERROR;
            }
            break;
        case STAND_ALONE_MONTH_FIELD:
            formatData->GetLongStandAloneMonths(&array1);
            formatData->GetShortStandAloneMonths(&array2);
            return ParseMonth(string, offset, count, absolute, array1, array2, value);
        case IDateFormat_MONTH_FIELD:
            formatData->GetMonths(&array1);
            formatData->GetShortMonths(&array2);
            return ParseMonth(string, offset, count, absolute, array1, array2, value);
        case IDateFormat_DATE_FIELD:
            field = Calendar_DATE;
            break;
        case IDateFormat_HOUR_OF_DAY1_FIELD:
            CParsePosition::New(offset, (IParsePosition**)&position);
            ParseNumber(absolute, string, position, (INumber**)&result);
            if (result == NULL) {
                position->GetErrorIndex(&errorIndex);
                *value = -errorIndex - 1;
                return NOERROR;
            }
            Int32 hour;
            result->Int32Value(&hour);
            if (hour == 24) {
                hour = 0;
            }
            mCalendar->Set(Calendar_HOUR_OF_DAY, hour);
            position->GetIndex(&index);
            *value = index;
            return NOERROR;
        case IDateFormat_HOUR_OF_DAY0_FIELD:
            field = Calendar_HOUR_OF_DAY;
            break;
        case IDateFormat_MINUTE_FIELD:
            field = Calendar_MINUTE;
            break;
        case IDateFormat_SECOND_FIELD:
            field = Calendar_SECOND;
            break;
        case IDateFormat_MILLISECOND_FIELD:
            field = Calendar_MILLISECOND;
            break;
        case STAND_ALONE_DAY_OF_WEEK_FIELD:
            formatData->GetLongStandAloneWeekdays(&array1);
            formatData->GetShortStandAloneWeekdays(&array2);
            return ParseDayOfWeek(string, offset, array1, array2, value);
        case IDateFormat_DAY_OF_WEEK_FIELD:
            formatData->GetWeekdays(&array1);
            formatData->GetShortWeekdays(&array2);
            return ParseDayOfWeek(string, offset, array1, array2, value);
        case IDateFormat_DAY_OF_YEAR_FIELD:
            field = Calendar_DAY_OF_YEAR;
            break;
        case IDateFormat_DAY_OF_WEEK_IN_MONTH_FIELD:
            field = Calendar_DAY_OF_WEEK_IN_MONTH;
            break;
        case IDateFormat_WEEK_OF_YEAR_FIELD:
            field = Calendar_WEEK_OF_YEAR;
            break;
        case IDateFormat_WEEK_OF_MONTH_FIELD:
            field = Calendar_WEEK_OF_MONTH;
            break;
        case IDateFormat_AM_PM_FIELD:
            formatData->GetAmPmStrings(&array1);
            return ParseText(string, offset, array1, Calendar_AM_PM, value);
        case IDateFormat_HOUR1_FIELD:
            CParsePosition::New(offset, (IParsePosition**)&position);
            ParseNumber(absolute, string, position, (INumber**)&result);
            if (result == NULL) {
                position->GetErrorIndex(&errorIndex);
                *value = -errorIndex - 1;
                return NOERROR;
            }
            result->Int32Value(&hour);
            if (hour == 12) {
                hour = 0;
            }
            mCalendar->Set(Calendar_HOUR, hour);
            position->GetIndex(&index);
            *value = index;
            return NOERROR;
        case IDateFormat_HOUR0_FIELD:
            field = Calendar_HOUR;
            break;
        case IDateFormat_TIMEZONE_FIELD:
            return ParseTimeZone(string, offset, value);
        case RFC_822_TIMEZONE_FIELD:
            return ParseTimeZone(string, offset, value);
    }
    if (field != -1) {
        return ParseNumber(absolute, string, offset, field, 0, value);
    }
    *value = offset;
    return NOERROR;
}

ECode SimpleDateFormat::ParseDayOfWeek(
    /* [in] */ String string,
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<String>* longs,
    /* [in] */ ArrayOf<String>* shorts,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 index;
    ParseText(string, offset, longs, Calendar_DAY_OF_WEEK, &index);
    if (index < 0) {
        ParseText(string, offset, shorts, Calendar_DAY_OF_WEEK, &index);
    }
    *value = index;
    return NOERROR;
}

ECode SimpleDateFormat::ParseMonth(
    /* [in] */ String string,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ Int32 absolute,
    /* [in] */ ArrayOf<String>* longs,
    /* [in] */ ArrayOf<String>* shorts,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    if (count <= 2) {
        return ParseNumber(absolute, string, offset, Calendar_MONTH, -1, value);
    }
    Int32 index;
    ParseText(string, offset, longs, Calendar_MONTH, &index);
    if (index < 0) {
        ParseText(string, offset, shorts, Calendar_MONTH, &index);
    }
    *value = index;
    return NOERROR;
}

ECode SimpleDateFormat::ParseEx(
    /* [in] */ String string,
    /* [in] */ IParsePosition* position,
    /* [out] */ IDate** date)
{
    // Harmony delegates to ICU's SimpleDateFormat, we implement it directly
    Boolean quote = FALSE;
    Int32 next, last = -1, count = 0, offset;
    position->GetIndex(&offset);
    Int32 length = string.GetLength();
    mCalendar->Clear();
    AutoPtr<ITimeZone> zone;
    mCalendar->GetTimeZone((ITimeZone**)&zone);
    const Int32 patternLength = pattern.GetLength();
    for (Int32 i = 0; i < patternLength; i++) {
        next = pattern.GetChar(i);
        if (next == '\'') {
            if (count > 0) {
                Int32 v;
                Parse(string, offset, (Char32) last, count, &v);
                if ((offset = v) < 0) {
                    return Error(position, -offset - 1, zone, date);
                }
                count = 0;
            }
            if (last == next) {
                if (offset >= length || string.GetChar(offset) != '\'') {
                    return Error(position, offset, zone, date);
                }
                offset++;
                last = -1;
            } else {
                last = next;
            }
            quote = !quote;
            continue;
        }
        if (!quote
                && (last == next || (next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z'))) {
            if (last == next) {
                count++;
            } else {
                if (count > 0) {
                    Int32 v;
                    Parse(string, offset, (Char32) last, -count, &v);
                    if ((offset = v) < 0) {
                        return Error(position, -offset - 1, zone, date);
                    }
                }
                last = next;
                count = 1;
            }
        } else {
            if (count > 0) {
                Int32 v;
                Parse(string, offset, (Char32) last, count, &v);
                if ((offset = v) < 0) {
                    return Error(position, -offset - 1, zone, date);
                }
                count = 0;
            }
            last = -1;
            if (offset >= length || string.GetChar(offset) != next) {
                return Error(position, offset, (ITimeZone*)zone, date);
            }
            offset++;
        }
    }
    if (count > 0) {
        Int32 v;
        Parse(string, offset, (Char32) last, count, &v);
        if ((offset = v) < 0) {
            return Error(position, -offset - 1, (ITimeZone*)zone, date);
        }
    }
    //try {
        AutoPtr<IDate> d;
        mCalendar->GetTime((IDate**)&d);
    //} catch (IllegalArgumentException e) {
    //    return error(position, offset, zone);
    //}
    position->SetIndex(offset);
    mCalendar->SetTimeZone(zone);
    *date = (IDate*)d;
    return NOERROR;
}

ECode SimpleDateFormat::ParseNumber(
    /* [in] */ Int32 max,
    /* [in] */ String string,
    /* [in] */ IParsePosition* position,
    /* [out] */ INumber** number)
{
    Int32 digit, length = string.GetLength(), result = 0;
    Int32 index;
    position->GetIndex(&index);
    if (max > 0 && max < length - index) {
        length = index + max;
    }
    while (index < length
            && (string.GetChar(index) == ' ' || string.GetChar(index) == '\t')) {
        index++;
    }
    if (max == 0) {
        position->SetIndex(index);
        return mNumberFormat->ParseEx(string, position, number);
    }

    while (index < length
            && (digit = Character::ToDigit(string.GetChar(index), 10)) != -1) {
        index++;
        result = result * 10 + digit;
    }
    Int32 i;
    position->GetIndex(&i);
    if (index == i) {
        position->SetErrorIndex(index);
        *number = NULL;
        return NOERROR;
    }
    position->SetIndex(index);
    //return Integer.valueOf(result);
    return NOERROR;
}

ECode SimpleDateFormat::ParseNumber(
    /* [in] */ Int32 max,
    /* [in] */ String string,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 field,
    /* [in] */ Int32 skew,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<IParsePosition> position;
    CParsePosition::New(offset, (IParsePosition**)&position);
    AutoPtr<INumber> result;
    ParseNumber(max, string, position, (INumber**)&result);
    if (result == NULL) {
        position->GetErrorIndex(value);
        *value = -(*value) - 1;
        return NOERROR;
    }
    Int32 v;
    result->Int32Value(&v);
    mCalendar->Set(field, v + skew);
    return position->GetIndex(value);
}

ECode SimpleDateFormat::ParseText(
    /* [in] */ String string,
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<String>* text,
    /* [in] */ Int32 field,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 found = -1;
    for (Int32 i = 0; i < text->GetLength(); i++) {
        if ((*text)[i].IsEmpty()) {
            continue;
        }
        if ( string.Substring( offset, (*text)[i].GetLength() )
                .EqualsIgnoreCase( (*text)[i].Substring(0, (*text)[i].GetLength() ) ) ) {
            // Search for the longest match, in case some fields are subsets
            if (found == -1 || (*text)[i].GetLength() > (*text)[found].GetLength()) {
                found = i;
            }
        }


    }
    if (found != -1) {
        mCalendar->Set(field, found);
        *value = offset + (*text)[found].GetLength();
        return NOERROR;
    }
    *value = -offset - 1;
    return NOERROR;
}

ECode SimpleDateFormat::ParseTimeZone(
    /* [in] */ String string,
    /* [in] */ Int32 offset,
    /* [out] */ Int32* value)
{

    VALIDATE_NOT_NULL(value);
    ArrayOf< ArrayOf<String>* > * zones;
    //formatData->InternalZoneStrings(&zones);
    Boolean foundGMT;
    String str("GMT");
    if (string.Substring(offset, 3).EqualsIgnoreCase(str.Substring(0, 3))) {
        foundGMT = TRUE;
    } else {
        foundGMT = FALSE;
    }
    if (foundGMT) {
        offset += 3;
    }
    Char32 sign;
    if (offset < (Int32)string.GetLength()
            && ((sign = string.GetChar(offset)) == '+' || sign == '-')) {
        AutoPtr<IParsePosition> position;
        CParsePosition::New(offset + 1, (IParsePosition**)&position);
        AutoPtr<INumber> result;
        mNumberFormat->ParseEx(string, position, (INumber**)&result);
        if (result == NULL) {
            Int32 ei;
            position->GetErrorIndex(&ei);
            *value = -ei - 1;
            return NOERROR;
        }
        Int32 hour;
        result->Int32Value(&hour);
        Int32 raw = hour * 3600000;
        Int32 index;
        position->GetIndex(&index);
        if (index < (Int32)string.GetLength() && string.GetChar(index) == ':') {
            position->SetIndex(index + 1);
            mNumberFormat->ParseEx(string, position, (INumber**)&result);
            if (result == NULL) {
                Int32 ei;
                position->GetErrorIndex(&ei);
                *value = -ei - 1;
                return NOERROR;
            }
            Int32 minute;
            result->Int32Value(&minute);
            raw += minute * 60000;
        } else if (hour >= 24) {
            raw = (hour / 100 * 3600000) + (hour % 100 * 60000);
        }
        if (sign == '-') {
            raw = -raw;
        }
        AutoPtr<ISimpleTimeZone> stz;
        CSimpleTimeZone::New(raw, String(""), (ISimpleTimeZone**)&stz);
        mCalendar->SetTimeZone(stz);
        position->GetIndex(value);
        return NOERROR;
    }
    if (foundGMT) {
        AutoPtr<ITimeZoneHelper> tzh;
        CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&tzh);
        AutoPtr<ITimeZone> tz;
        tzh->GetTimeZone(String("GMT"), (ITimeZone**)&tz);
        mCalendar->SetTimeZone(tz);
        *value = offset;
        return NOERROR;
    }
    AutoFree<ArrayOf<String> > element;
    for (Int32 i = 0; i < zones->GetLength(); ++i) {
        for (Int32 j = 1; j < 5; ++j) {
            if ( string.Substring( offset, (*element.Get())[j].GetLength() )
                    .EqualsIgnoreCase( (*element.Get())[j].Substring(0, (*element.Get())[j].GetLength() ) ) ) {
                AutoPtr<ITimeZoneHelper> tzh;
                CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&tzh);
                AutoPtr<ITimeZone> zone;
                tzh->GetTimeZone((*element.Get())[0], (ITimeZone**)&zone);
                if (zone == NULL) {
                    *value = -offset - 1;
                    return NOERROR;
                }
                Int32 raw;
                zone->GetRawOffset(&raw);
                Boolean isUsed;
                zone->UseDaylightTime(&isUsed);
                if (j >= 3 && isUsed) {
                    raw += 3600000;
                }
                AutoPtr<ISimpleTimeZone> stz;
                CSimpleTimeZone::New(raw, String(""), (ISimpleTimeZone**)&stz);
                mCalendar->SetTimeZone(stz);
                *value = offset + (*element.Get())[j].GetLength();
                return NOERROR;
            }
        }
    }
    *value = -offset - 1;
    return NOERROR;
}

ECode SimpleDateFormat::Set2DigitYearStart(
    /* [in] */ IDate* date)
{
    defaultCenturyStart = date;
    AutoPtr<ICalendar> cal;
    CGregorianCalendar::New((IGregorianCalendar**)&cal);
    cal->SetTime((IDate*)defaultCenturyStart);
    cal->Get(Calendar_YEAR, &creationYear);
    return NOERROR;
}

ECode SimpleDateFormat::SetDateFormatSymbols(
    /* [in] */ IDateFormatSymbols* value)
{
    formatData = value;
    return NOERROR;
}

ECode SimpleDateFormat::ToLocalizedPattern(
    /* [out] */ String* patterns)
{
    String* localPatternChars = new String(NULL);
    formatData->GetLocalPatternChars(localPatternChars);
    return ConvertPattern(this->pattern, PATTERN_CHARS, *localPatternChars, FALSE, patterns);
}

ECode SimpleDateFormat::ConvertPattern(
    /* [in] */ String tem,
    /* [in] */ String fromChars,
    /* [in] */ String toChars,
    /* [in] */ Boolean check,
    /* [out] */ String* patterns)
{
    VALIDATE_NOT_NULL(patterns);
    if (!check && fromChars.Equals(toChars)) {
        *patterns = tem;
        return NOERROR;
    }
    Boolean quote = FALSE;
    StringBuffer* output = new StringBuffer("");
    Int32 length = (Int32)(tem.GetLength() );
    for (Int32 i = 0; i < length; i++) {
        Int32 index;
        Char32 next = tem.GetChar(i);
        if (next == '\'') {
            quote = !quote;
        }
        if (!quote && (index = fromChars.IndexOf(next)) != -1) {
            *output += toChars.GetChar(index);
        } else if (check && !quote && ((next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z'))) {
            //throw new IllegalArgumentException("Invalid pattern character '" + next + "' in " + "'" + template + "'");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        } else {
            *output += next;
        }
    }
    if (quote) {
        //throw new IllegalArgumentException("Unterminated quote");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *patterns = output->Substring(0, output->GetLength() );
    return NOERROR;
}

ECode SimpleDateFormat::ToPattern(
    /* [out] */ String* pattern)
{
    VALIDATE_NOT_NULL(pattern);
    *pattern = String(this->pattern);
    return NOERROR;
}
