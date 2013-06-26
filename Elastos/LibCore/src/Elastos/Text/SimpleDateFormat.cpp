
#include "cmdef.h"
#include "SimpleDateFormat.h"
#include "CDateFormatSymbols.h"
#include "NumberFormat.h"
#include "CAttributedString.h"
#include "CFieldPosition.h"
#include "TimeZones.h"
#include "Character.h"
#include <elastos/AutoFree.h>

const Int32 SimpleDateFormat::RFC_822_TIMEZONE_FIELD;
const Int32 SimpleDateFormat::STAND_ALONE_MONTH_FIELD;
const Int32 SimpleDateFormat::STAND_ALONE_DAY_OF_WEEK_FIELD;

SimpleDateFormat::SimpleDateFormat()
    : mCreationYear(0)
{ }

ECode SimpleDateFormat::Init()
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    Init((ILocale*)locale);
    DefaultPattern(&mPattern);
    ASSERT_SUCCEEDED(CDateFormatSymbols::NewByFriend(locale,
            (CDateFormatSymbols**)&mFormatData));
    return NOERROR;
}

ECode SimpleDateFormat::Init(
    /* [in] */ String pattern)
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return Init(pattern, locale);
}

ECode SimpleDateFormat::ValidateFormat(
    /* [in] */ Char32 format)
{
    Int32 index = String(ISimpleDateFormat_PATTERN_CHARS).IndexOf(format);
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

    Int32 patternLength = tem.GetLength();
    for (Int32 i = 0; i < patternLength; i++) {
        next = tem.GetChar(i);
        if (next == '\'') {
            if (count > 0) {
                FAIL_RETURN(ValidateFormat((Char32)last));
                count = 0;
            }
            if (last == next) {
                last = -1;
            }
            else {
                last = next;
            }
            quote = !quote;
            continue;
        }
        if (!quote
                && (last == next || (next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z'))) {
            if (last == next) {
                count++;
            }
            else {
                if (count > 0) {
                    FAIL_RETURN(ValidateFormat((Char32)last));
                }
                last = next;
                count = 1;
            }
        }
        else {
            if (count > 0) {
                FAIL_RETURN(ValidateFormat((Char32)last));
                count = 0;
            }
            last = -1;
        }
    }
    if (count > 0) {
        FAIL_RETURN(ValidateFormat((Char32)last));
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
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    Init((ILocale*)locale);
    FAIL_RETURN(ValidatePattern(tem));
    mPattern = tem;
    mFormatData = (CDateFormatSymbols*)value;
    return NOERROR;
}

ECode SimpleDateFormat::Init(
    /* [in] */ String tem,
    /* [in] */ ILocale* locale)
{
    Init(locale);
    FAIL_RETURN(ValidatePattern(tem));
    mPattern = tem;
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> l;
    localeHelper->GetDefault((ILocale**)&l);
    ASSERT_SUCCEEDED(CDateFormatSymbols::NewByFriend(l,
            (CDateFormatSymbols**)&mFormatData));
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
    mCalendar->Get(Calendar_YEAR, &mCreationYear);
    mCalendar->GetTime((IDate**)&mDefaultCenturyStart);
    return NOERROR;
}

ECode SimpleDateFormat::ApplyLocalizedPattern(
    /* [in] */ String tem)
{
    String localPatternChars;
    mFormatData->GetLocalPatternChars(&localPatternChars);
    ConvertPattern(tem, localPatternChars,
            String(ISimpleDateFormat_PATTERN_CHARS), TRUE, &mPattern);
    return NOERROR;
}

ECode SimpleDateFormat::ApplyPattern(
    /* [in] */ String tem)
{
    FAIL_RETURN(ValidatePattern(tem));
    mPattern = tem;
    return NOERROR;
}

ECode SimpleDateFormat::DefaultPattern(
    /* [out] */ String* pattern)
{
    VALIDATE_NOT_NULL(pattern);

    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);

    AutoPtr<ILocaleDataHelper> localeDataHelper;
    FAIL_RETURN(CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&localeDataHelper));
    AutoPtr<ILocaleData> localeData;
    FAIL_RETURN(localeDataHelper->Get(locale, (ILocaleData**)&localeData));

    String dateFormat, timeFormat;
    localeData->GetDateFormat(IDateFormat_SHORT, &dateFormat);
    localeData->GetTimeFormat(IDateFormat_SHORT, &timeFormat);
    pattern->Append(dateFormat);
    pattern->Append(" ");
    pattern->Append(timeFormat);

    return NOERROR;
}

ECode SimpleDateFormat::FormatToCharacterIterator(
    /* [in] */ IInterface* object,
    /* [out] */ IAttributedCharacterIterator** charactorIterator)
{
    VALIDATE_NOT_NULL(charactorIterator);

    if (object == NULL) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if (object->Probe(EIID_IDate) != NULL) {
        return FormatToCharacterIteratorImpl((IDate*)object->Probe(EIID_IDate),
                charactorIterator);
    }
    if (object->Probe(EIID_INumber) != NULL ) {
        AutoPtr<INumber> number = (INumber*)object->Probe(EIID_INumber);
        Int64 value;
        number->Int64Value(&value);
        AutoPtr<IDate> date;
        ASSERT_SUCCEEDED(CDate::New(value, (IDate**)&date));
        return FormatToCharacterIteratorImpl(date, charactorIterator);
    }
    //throw new IllegalArgumentException();
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode SimpleDateFormat::FormatToCharacterIteratorImpl(
    /* [in] */ IDate* date,
    /* [out] */ IAttributedCharacterIterator** charactorIterator)
{
    StringBuffer buffer;
    Vector<IFieldPosition*>* fields = new Vector<IFieldPosition*>();

    String result;
    // format the date, and find fields
    FormatImpl(date, buffer, NULL, fields, &result);

    // create and AttributedString with the formatted buffer
    AutoPtr<IAttributedString> as;
    CAttributedString::New(buffer.ToString(), (IAttributedString**)&as);

    // add DateFormat field attributes to the AttributedString
    for (Int32 i = 0; i < (Int32)fields->GetSize(); i++) {
        AutoPtr<IFieldPosition> pos = (*fields)[i];
        AutoPtr<IFormatField> attribute;
        ASSERT_SUCCEEDED(pos->GetFieldAttribute((IFormatField**)&attribute));
        Int32 bi, ei;
        pos->GetBeginIndex(&bi);
        pos->GetEndIndex(&ei);
        as->AddAttributeEx(attribute, (IInterface*)attribute, bi, ei);
    }

    // return the CharacterIterator from AttributedString
    return as->GetIterator(charactorIterator);
}

ECode SimpleDateFormat::FormatImpl(
    /* [in] */ IDate* date,
    /* [in] */ StringBuffer buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ Vector<IFieldPosition*>* fields,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    Boolean quote = FALSE;
    Int32 next, last = -1, count = 0;
    mCalendar->SetTime(date);
    if (field != NULL) {
        ((CFieldPosition*)field)->Clear();
    }

    Int32 patternLength = mPattern.GetLength();
    for (Int32 i = 0; i < patternLength; i++) {
        next = mPattern.GetChar(i);
        if (next == '\'') {
            if (count > 0) {
                FAIL_RETURN(Append(buffer, field, fields, (Char32)last, count));
                count = 0;
            }
            if (last == next) {
                buffer += '\'';
                last = -1;
            }
            else {
                last = next;
            }
            quote = !quote;
            continue;
        }
        if (!quote
                && (last == next || (next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z'))) {
            if (last == next) {
                count++;
            }
            else {
                if (count > 0) {
                    FAIL_RETURN(Append(buffer, field, fields, (Char32)last, count));
                }
                last = next;
                count = 1;
            }
        }
        else {
            if (count > 0) {
                FAIL_RETURN(Append(buffer, field, fields, (Char32)last, count));
                count = 0;
            }
            last = -1;
            buffer += (Char32)next;
        }
    }
    if (count > 0) {
        FAIL_RETURN(Append(buffer, field, fields, (Char32) last, count));
    }

    *result = buffer;
    return NOERROR;
}

ECode SimpleDateFormat::Append(
    /* [in] */ StringBuffer buffer,
    /* [in] */ IFieldPosition* position,
    /* [in] */ Vector<IFieldPosition*>* fields,
    /* [in] */ Char32 format,
    /* [in] */ Int32 count)
{
    Int32 field = -1;
    Int32 index = String(ISimpleDateFormat_PATTERN_CHARS).IndexOf(format);
    if (index == -1) {
        //throw new IllegalArgumentException("Unknown pattern character '" + format + "'");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 beginPosition = buffer.GetLength();
    AutoPtr<IDateFormatField> dateFormatField;
    Int32 value;
    switch (index) {
        case IDateFormat_ERA_FIELD:
            dateFormatField = DateFormat::Field::ERA;
            mCalendar->Get(Calendar_ERA, &value);
            buffer += (*mFormatData->mEras)[value];
            break;
        case IDateFormat_YEAR_FIELD:
            dateFormatField = DateFormat::Field::YEAR;
            mCalendar->Get(Calendar_YEAR, &value);
            /*
             * For 'y' and 'yyy', we're consistent with Unicode and previous releases
             * of Android. But this means we're inconsistent with the RI.
             *     http://unicode.org/reports/tr35/
             */
            if (count == 2) {
                AppendNumber(buffer, 2, value %= 100);
            }
            else {
                AppendNumber(buffer, count, value);
            }
            break;
        case STAND_ALONE_MONTH_FIELD: // L
            dateFormatField = DateFormat::Field::MONTH;
            AppendMonth(buffer, count, mFormatData->mLongStandAloneMonths,
                    mFormatData->mShortStandAloneMonths);
            break;
        case IDateFormat_MONTH_FIELD: // M
            dateFormatField = DateFormat::Field::MONTH;
            AppendMonth(buffer, count, mFormatData->mMonths, mFormatData->mShortMonths);
            break;
        case IDateFormat_DATE_FIELD:
            dateFormatField = DateFormat::Field::DAY_OF_MONTH;
            field = Calendar_DATE;
            break;
        case IDateFormat_HOUR_OF_DAY1_FIELD: // k
            dateFormatField = DateFormat::Field::HOUR_OF_DAY1;
            mCalendar->Get(Calendar_HOUR_OF_DAY, &value);
            AppendNumber(buffer, count, value == 0 ? 24 : value);
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
            mCalendar->Get(Calendar_MILLISECOND, &value);
            AppendNumber(buffer, count, value);
            break;
        case STAND_ALONE_DAY_OF_WEEK_FIELD:
            dateFormatField = DateFormat::Field::DAY_OF_WEEK;
            AppendDayOfWeek(buffer, count, mFormatData->mLongStandAloneWeekdays,
                    mFormatData->mShortStandAloneWeekdays);
            break;
        case IDateFormat_DAY_OF_WEEK_FIELD:
            dateFormatField = DateFormat::Field::DAY_OF_WEEK;
            AppendDayOfWeek(buffer, count, mFormatData->mWeekdays,
                    mFormatData->mShortWeekdays);
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
            mCalendar->Get(Calendar_AM_PM, &value);
            buffer += (*mFormatData->mAmpms)[value];
            break;
        case IDateFormat_HOUR1_FIELD: // h
            dateFormatField = DateFormat::Field::HOUR1;
            mCalendar->Get(Calendar_HOUR, &value);
            AppendNumber(buffer, count, value == 0 ? 12 : value);
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
        Int32 v;
        mCalendar->Get(field, &v);
        AppendNumber(buffer, count, v);
    }

    if (fields != NULL) {
        AutoPtr<IFieldPosition> pos;
        ASSERT_SUCCEEDED(CFieldPosition::New(dateFormatField, (IFieldPosition**)&pos));
        pos->SetBeginIndex(beginPosition);
        pos->SetEndIndex(buffer.GetLength());
        fields->PushBack(pos);
    }
    else {
        // Set to the first occurrence
        AutoPtr<IFormatField> ff;
        position->GetFieldAttribute((IFormatField**)&ff);
        Int32 f, ei;
        position->GetField(&f);
        position->GetEndIndex(&ei);
        if ((ff == dateFormatField || ff == NULL) && (f == index) && (ei == 0) ) {
            position->SetBeginIndex(beginPosition);
            position->SetEndIndex(buffer.GetLength());
        }
    }
    return NOERROR;
}

void SimpleDateFormat::AppendDayOfWeek(
    /* [in] */ StringBuffer buffer,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<String>* longs,
    /* [in] */ ArrayOf<String>* shorts)
{
    Boolean isLong = (count > 3);
    ArrayOf<String>* days = isLong ? longs : shorts;
    Int32 value;
    mCalendar->Get(Calendar_DAY_OF_WEEK, &value);
    buffer += (*days)[value];
}

void SimpleDateFormat::AppendMonth(
    /* [in] */ StringBuffer buffer,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<String>* longs,
    /* [in] */ ArrayOf<String>* shorts)
{
    Int32 month;
    mCalendar->Get(Calendar_MONTH, &month);
    if (count <= 2) {
        AppendNumber(buffer, count, month + 1);
        return;
    }

    Boolean isLong = (count > 3);
    ArrayOf<String>* months = isLong ? longs : shorts;
    buffer += (*months)[month];
}

void SimpleDateFormat::AppendTimeZone(
    /* [in] */ StringBuffer buffer,
    /* [in] */ Int32 count,
    /* [in] */ Boolean generalTimeZone)
{
    if (generalTimeZone) {
        AutoPtr<ITimeZone> tz;
        mCalendar->GetTimeZone((ITimeZone**)&tz);
        Int32 value;
        mCalendar->Get(Calendar_DST_OFFSET, &value);
        Boolean daylight = value != 0;
        Int32 style = count < 4 ? ITimeZone_SHORT : ITimeZone_LONG;
        if (!mFormatData->mCustomZoneStrings) {
            String name;
            tz->GetDisplayNameEx3(daylight, style, mFormatData->mLocale, &name);
            buffer += name;
            return;
        }
        // We can't call TimeZone.getDisplayName() because it would not use
        // the custom DateFormatSymbols of this SimpleDateFormat.
        String id;
        tz->GetID(&id);
        String custom = TimeZones::LookupDisplayName(mFormatData->mZoneStrings,
                id, daylight, style);
        if (!custom.IsNull()) {
           buffer += custom;
           return;
        }
    }
    // We didn't find what we were looking for, so default to a numeric time zone.
    AppendNumericTimeZone(buffer, generalTimeZone);
}

void SimpleDateFormat::AppendNumericTimeZone(
    /* [in] */ StringBuffer buffer,
    /* [in] */ Boolean generalTimeZone)
{
    Int32 value1, value2;
    mCalendar->Get(Calendar_ZONE_OFFSET, &value1);
    mCalendar->Get(Calendar_DST_OFFSET, &value2);
    Int32 offset = value1 + value2;
    Char32 sign = '+';
    if (offset < 0) {
        sign = '-';
        offset = -offset;
    }
    if (generalTimeZone) {
        buffer += "GMT";
    }
    buffer += sign;
    AppendNumber(buffer, 2, offset / 3600000);
    if (generalTimeZone) {
        buffer += ':';
    }
    AppendNumber(buffer, 2, (offset % 3600000) / 60000);
}

void SimpleDateFormat::AppendNumber(
    /* [in] */ StringBuffer buffer,
    /* [in] */ Int32 count,
    /* [in] */ Int32 value)
{

    Int32 minimumIntegerDigits;
    mNumberFormat->GetMinimumIntegerDigits(&minimumIntegerDigits);
    mNumberFormat->SetMinimumIntegerDigits(count);
    AutoPtr<IFieldPosition> fp;
    CFieldPosition::New(0, (IFieldPosition**)&fp);
    String result;
    AutoPtr<IInteger32> integer;
    ASSERT_SUCCEEDED(CInteger32::New(value, (IInteger32**)&integer));
    mNumberFormat->FormatObjectEx((IInterface*)integer, String(buffer), fp, &result);
    mNumberFormat->SetMinimumIntegerDigits(minimumIntegerDigits);
}

AutoPtr<IDate> SimpleDateFormat::Error(
    /* [in] */ IParsePosition* position,
    /* [in] */ Int32 offset,
    /* [in] */ ITimeZone* zone)
{
    position->SetErrorIndex(offset);
    mCalendar->SetTimeZone(zone);
    return NULL;
}

ECode SimpleDateFormat::FormatDateEx(
    /* [in] */ IDate* date,
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* fieldPos,
    /* [out] */ String* formatString)
{
    VALIDATE_NOT_NULL(formatString);
    // Harmony delegates to ICU's SimpleDateFormat, we implement it directly
    StringBuffer sb = StringBuffer(buffer);
    return FormatImpl(date, sb, fieldPos, NULL, formatString);
}

ECode SimpleDateFormat::Get2DigitYearStart(
    /* [out] */ IDate** start)
{
    VALIDATE_NOT_NULL(start);
    *start = mDefaultCenturyStart;
    if (*start != NULL) (*start)->AddRef();
    return NOERROR;
}

ECode SimpleDateFormat::GetDateFormatSymbols(
    /* [out] */ IDateFormatSymbols** symbols)
{
    VALIDATE_NOT_NULL(symbols);
    *symbols = mFormatData;
    if (*symbols != NULL) (*symbols)->AddRef();
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

    Int32 index = String(ISimpleDateFormat_PATTERN_CHARS).IndexOf(format);
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
    Int32 errorIndex;
    AutoPtr<INumber> result;
    switch (index) {
        case IDateFormat_ERA_FIELD:
            *value = ParseText(string, offset, mFormatData->mEras, Calendar_ERA);
            return NOERROR;
        case IDateFormat_YEAR_FIELD:
            if (count >= 3) {
                field = Calendar_YEAR;
            }
            else {
                CParsePosition::New(offset, (IParsePosition**)&position);
                result = ParseNumber(absolute, string, position);
                if (result == NULL) {
                    position->GetErrorIndex(&errorIndex);
                    *value = -errorIndex - 1;
                    return NOERROR;
                }
                Int32 year;
                result->Int32Value(&year);
                // A two digit year must be exactly two digits, i.e. 01
                Int32 i;
                position->GetIndex(&i);
                if ((i - offset) == 2 && year >= 0) {
                    year += mCreationYear / 100 * 100;
                    if (year < mCreationYear) {
                        year += 100;
                    }
                }
                mCalendar->Set(Calendar_YEAR, year);
                *value = i;
                return NOERROR;
            }
            break;
        case STAND_ALONE_MONTH_FIELD:
            *value = ParseMonth(string, offset, count, absolute,
                    mFormatData->mLongStandAloneMonths,
                    mFormatData->mShortStandAloneMonths);
            return NOERROR;
        case IDateFormat_MONTH_FIELD:
            *value = ParseMonth(string, offset, count, absolute, mFormatData->mMonths,
                    mFormatData->mShortMonths);
            return NOERROR;
        case IDateFormat_DATE_FIELD:
            field = Calendar_DATE;
            break;
        case IDateFormat_HOUR_OF_DAY1_FIELD:
            CParsePosition::New(offset, (IParsePosition**)&position);
            result = ParseNumber(absolute, string, position);
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
            position->GetIndex(value);
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
            *value = ParseDayOfWeek(string, offset, mFormatData->mLongStandAloneWeekdays,
                    mFormatData->mShortStandAloneWeekdays);
            return NOERROR;
        case IDateFormat_DAY_OF_WEEK_FIELD:
            *value = ParseDayOfWeek(string, offset, mFormatData->mWeekdays,
                    mFormatData->mShortWeekdays);
            return NOERROR;
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
            *value =  ParseText(string, offset, mFormatData->mAmpms, Calendar_AM_PM);
            return NOERROR;
        case IDateFormat_HOUR1_FIELD:
            CParsePosition::New(offset, (IParsePosition**)&position);
            result = ParseNumber(absolute, string, position);
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
            position->GetIndex(value);
            return NOERROR;
        case IDateFormat_HOUR0_FIELD:
            field = Calendar_HOUR;
            break;
        case IDateFormat_TIMEZONE_FIELD:
            *value = ParseTimeZone(string, offset);
            return NOERROR;
        case RFC_822_TIMEZONE_FIELD:
            *value = ParseTimeZone(string, offset);
            return NOERROR;
    }
    if (field != -1) {
        *value = ParseNumber(absolute, string, offset, field, 0);
        return NOERROR;
    }
    *value = offset;
    return NOERROR;
}

Int32 SimpleDateFormat::ParseDayOfWeek(
    /* [in] */ String string,
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<String>* longs,
    /* [in] */ ArrayOf<String>* shorts)
{
    Int32 index = ParseText(string, offset, longs, Calendar_DAY_OF_WEEK);
    if (index < 0) {
        index = ParseText(string, offset, shorts, Calendar_DAY_OF_WEEK);
    }
    return index;
}

Int32 SimpleDateFormat::ParseMonth(
    /* [in] */ String string,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ Int32 absolute,
    /* [in] */ ArrayOf<String>* longs,
    /* [in] */ ArrayOf<String>* shorts)
{
    if (count <= 2) {
        return ParseNumber(absolute, string, offset, Calendar_MONTH, -1);
    }
    Int32 index = ParseText(string, offset, longs, Calendar_MONTH);
    if (index < 0) {
        index = ParseText(string, offset, shorts, Calendar_MONTH);
    }
    return index;
}

ECode SimpleDateFormat::ParseEx(
    /* [in] */ String string,
    /* [in] */ IParsePosition* position,
    /* [out] */ IDate** date)
{
    VALIDATE_NOT_NULL(date);
    // Harmony delegates to ICU's SimpleDateFormat, we implement it directly
    Boolean quote = FALSE;
    Int32 next, last = -1, count = 0, offset;
    position->GetIndex(&offset);
    Int32 length = string.GetLength();
    mCalendar->Clear();
    AutoPtr<ITimeZone> zone;
    mCalendar->GetTimeZone((ITimeZone**)&zone);
    Int32 patternLength = mPattern.GetLength();
    for (Int32 i = 0; i < patternLength; i++) {
        next = mPattern.GetChar(i);
        if (next == '\'') {
            if (count > 0) {
                Parse(string, offset, (Char32)last, count, &offset);
                if (offset < 0) {
                    Error(position, -offset - 1, zone);
                    *date = NULL;
                    return NOERROR;
                }
                count = 0;
            }
            if (last == next) {
                if (offset >= length || string.GetChar(offset) != '\'') {
                    Error(position, offset, zone);
                    *date = NULL;
                    return NOERROR;
                }
                offset++;
                last = -1;
            }
            else {
                last = next;
            }
            quote = !quote;
            continue;
        }
        if (!quote
                && (last == next || (next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z'))) {
            if (last == next) {
                count++;
            }
            else {
                if (count > 0) {
                    Parse(string, offset, (Char32)last, -count, &offset);
                    if (offset < 0) {
                        Error(position, -offset - 1, zone);
                        *date = NULL;
                        return NOERROR;
                    }
                }
                last = next;
                count = 1;
            }
        }
        else {
            if (count > 0) {
                Parse(string, offset, (Char32)last, count, &offset);
                if (offset < 0) {
                    Error(position, -offset - 1, zone);
                    *date = NULL;
                    return NOERROR;
                }
                count = 0;
            }
            last = -1;
            if (offset >= length || string.GetChar(offset) != next) {
                Error(position, offset, zone);
                *date = NULL;
                return NOERROR;
            }
            offset++;
        }
    }
    if (count > 0) {
        Parse(string, offset, (Char32) last, count, &offset);
        if (offset < 0) {
            Error(position, -offset - 1, zone);
            *date = NULL;
            return NOERROR;
        }
    }
    //try {
    AutoPtr<IDate> d;
    ECode ec = mCalendar->GetTime((IDate**)&d);
    if (FAILED(ec)) {
        Error(position, offset, zone);
        *date = NULL;
        return NOERROR;
    }
    //} catch (IllegalArgumentException e) {
    //    return error(position, offset, zone);
    //}
    position->SetIndex(offset);
    mCalendar->SetTimeZone(zone);
    *date = d;
    (*date)->AddRef();
    return NOERROR;
}

AutoPtr<INumber> SimpleDateFormat::ParseNumber(
    /* [in] */ Int32 max,
    /* [in] */ String string,
    /* [in] */ IParsePosition* position)
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
        AutoPtr<INumber> number;
        mNumberFormat->ParseEx(string, position, (INumber**)&number);
        return number;
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
        return NULL;
    }
    position->SetIndex(index);
    AutoPtr<IInteger32> integer;
    CInteger32::New(result, (IInteger32**)&integer);
    //todo: all interface such as IInteger32 should extends INumber;
    //return (INumber*)integer.Get();
    return NULL;
}

Int32 SimpleDateFormat::ParseNumber(
    /* [in] */ Int32 max,
    /* [in] */ String string,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 field,
    /* [in] */ Int32 skew)
{
    AutoPtr<IParsePosition> position;
    CParsePosition::New(offset, (IParsePosition**)&position);
    AutoPtr<INumber> result = ParseNumber(max, string, position);
    if (result == NULL) {
        Int32 index;
        position->GetErrorIndex(&index);
        return -index - 1;
    }
    Int32 value;
    result->Int32Value(&value);
    mCalendar->Set(field, value + skew);
    position->GetIndex(&value);
    return value;
}

Int32 SimpleDateFormat::ParseText(
    /* [in] */ String string,
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<String>* text,
    /* [in] */ Int32 field)
{
    Int32 found = -1;
    for (Int32 i = 0; i < text->GetLength(); i++) {
        if ((*text)[i].IsEmpty()) {
            continue;
        }
        if (string.Substring(offset, (*text)[i].GetLength())
                .EqualsIgnoreCase((*text)[i])) {
            // Search for the longest match, in case some fields are subsets
            if (found == -1 || (*text)[i].GetLength() > (*text)[found].GetLength()) {
                found = i;
            }
        }


    }
    if (found != -1) {
        mCalendar->Set(field, found);
        return offset + (*text)[found].GetLength();
    }
    return -offset - 1;
}

Int32 SimpleDateFormat::ParseTimeZone(
    /* [in] */ String string,
    /* [in] */ Int32 offset)
{
    ArrayOf<ArrayOf<String>*>* zones = mFormatData->InternalZoneStrings();
    Boolean foundGMT;
    String str("GMT");
    if (string.Substring(offset, 3).Equals(str)) {
        foundGMT = TRUE;
    }
    else {
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
            return -ei - 1;
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
                return -ei - 1;
            }
            Int32 minute;
            result->Int32Value(&minute);
            raw += minute * 60000;
        }
        else if (hour >= 24) {
            raw = (hour / 100 * 3600000) + (hour % 100 * 60000);
        }
        if (sign == '-') {
            raw = -raw;
        }
        AutoPtr<ISimpleTimeZone> stz;
        CSimpleTimeZone::New(raw, String(""), (ISimpleTimeZone**)&stz);
        mCalendar->SetTimeZone(stz);
        position->GetIndex(&index);
        return index;
    }
    if (foundGMT) {
        AutoPtr<ITimeZoneHelper> tzh;
        CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&tzh);
        AutoPtr<ITimeZone> tz;
        tzh->GetTimeZone(String("GMT"), (ITimeZone**)&tz);
        mCalendar->SetTimeZone(tz);
        return offset;
    }

    AutoFree<ArrayOf<String> > element;
    for (Int32 i = 0; i < zones->GetLength(); ++i) {
        AutoFree<ArrayOf<String> > element = (*zones)[i];
        for (Int32 j = 1; j < 5; ++j) {
            if (string.Substring(offset, (*element)[j].GetLength())
                    .EqualsIgnoreCase((*element)[j])) {
                AutoPtr<ITimeZoneHelper> tzh;
                CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&tzh);
                AutoPtr<ITimeZone> zone;
                tzh->GetTimeZone((*element)[0], (ITimeZone**)&zone);
                if (zone == NULL) {
                    return -offset - 1;
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
                return offset + (*element)[j].GetLength();
            }
        }
    }
    return -offset - 1;
}

ECode SimpleDateFormat::Set2DigitYearStart(
    /* [in] */ IDate* date)
{
    mDefaultCenturyStart = date;
    AutoPtr<ICalendar> cal;
    CGregorianCalendar::New((IGregorianCalendar**)&cal);
    cal->SetTime(mDefaultCenturyStart);
    cal->Get(Calendar_YEAR, &mCreationYear);
    return NOERROR;
}

ECode SimpleDateFormat::SetDateFormatSymbols(
    /* [in] */ IDateFormatSymbols* value)
{
    mFormatData = (CDateFormatSymbols*)value;
    return NOERROR;
}

ECode SimpleDateFormat::ToLocalizedPattern(
    /* [out] */ String* patterns)
{
    VALIDATE_NOT_NULL(patterns);
    String localPatternChars;
    mFormatData->GetLocalPatternChars(&localPatternChars);
    return ConvertPattern(mPattern, String(ISimpleDateFormat_PATTERN_CHARS),
            localPatternChars, FALSE, patterns);
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
    StringBuffer output;
    Int32 length = tem.GetLength();
    for (Int32 i = 0; i < length; i++) {
        Int32 index;
        Char32 next = tem.GetChar(i);
        if (next == '\'') {
            quote = !quote;
        }
        if (!quote && (index = fromChars.IndexOf(next)) != -1) {
            output += toChars.GetChar(index);
        }
        else if (check && !quote && ((next >= 'a' && next <= 'z')
                || (next >= 'A' && next <= 'Z'))) {
            //throw new IllegalArgumentException("Invalid pattern character '" + next + "' in " + "'" + template + "'");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        else {
            output += next;
        }
    }
    if (quote) {
        //throw new IllegalArgumentException("Unterminated quote");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *patterns = output.ToString();
    return NOERROR;
}

ECode SimpleDateFormat::ToPattern(
    /* [out] */ String* pattern)
{
    VALIDATE_NOT_NULL(pattern);
    *pattern = mPattern;
    return NOERROR;
}
