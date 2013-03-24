
#include "cmdef.h"
#include "DateFormat.h"
#include <Elastos.IO.h>


const AutoPtr<IDateFormatField> DateFormat::Field::ERA;
const AutoPtr<IDateFormatField> DateFormat::Field::YEAR;
const AutoPtr<IDateFormatField> DateFormat::Field::MONTH;
const AutoPtr<IDateFormatField> DateFormat::Field::HOUR_OF_DAY0;
const AutoPtr<IDateFormatField> DateFormat::Field::HOUR_OF_DAY1;
const AutoPtr<IDateFormatField> DateFormat::Field::MINUTE;
const AutoPtr<IDateFormatField> DateFormat::Field::SECOND;
const AutoPtr<IDateFormatField> DateFormat::Field::MILLISECOND;
const AutoPtr<IDateFormatField> DateFormat::Field::DAY_OF_WEEK;
const AutoPtr<IDateFormatField> DateFormat::Field::DAY_OF_MONTH;
const AutoPtr<IDateFormatField> DateFormat::Field::DAY_OF_YEAR;
const AutoPtr<IDateFormatField> DateFormat::Field::DAY_OF_WEEK_IN_MONTH;
const AutoPtr<IDateFormatField> DateFormat::Field::WEEK_OF_YEAR;
const AutoPtr<IDateFormatField> DateFormat::Field::WEEK_OF_MONTH;
const AutoPtr<IDateFormatField> DateFormat::Field::AM_PM;
const AutoPtr<IDateFormatField> DateFormat::Field::HOUR0;
const AutoPtr<IDateFormatField> DateFormat::Field::HOUR1;
const AutoPtr<IDateFormatField> DateFormat::Field::TIME_ZONE;

HashMap<Int32, AutoPtr<IDateFormatField> > DateFormat::Field::sTable(11);

DateFormat::Field::Field()
    : mCalendarField(-1)
{}

ECode DateFormat::Field::Init(
   /* [in] */ const String& fieldName,
   /* [in] */ Int32 calendarField)
{
    FAIL_RETURN(Format::Field::Init(fieldName));
    mCalendarField = calendarField;
    if (calendarField != -1 && sTable.Find(calendarField) == sTable.End()) {
        sTable[calendarField] = reinterpret_cast<IDateFormatField*>(this->Probe(EIID_IDateFormatField));
    }
    return NOERROR;
}

ECode DateFormat::Field::GetCalendarField(
    /* [out] */ Int32* value)
{
    *value = mCalendarField;
    return NOERROR;
}

ECode DateFormat::Field::OfCalendarField(
    /* [in] */ Int32 calendarField,
    /* [out] */ IDateFormatField** field)
{
    if (calendarField < 0 || calendarField >= Calendar_FIELD_COUNT) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *field = NULL;
    HashMap<Int32, AutoPtr<IDateFormatField> >::Iterator it = sTable.Find(calendarField);
    if (it != sTable.End()) {
        *field = it->mSecond;
        if (*field != NULL) (*field)->AddRef();
    }
    return NOERROR;
}

ECode DateFormat::Field::ReadResolve(
    /* [out] */ IInterface** resolvedField)
{
    ClassID clsid;
    GetClassID(&clsid);
    if (clsid != ECLSID_CDateFormatField) {
    //    throw new InvalidObjectException("cannot resolve subclasses");
        return E_INVALID_OBJECT_EXCEPTION;
    }
    if (mCalendarField != -1) {
        //try {
            AutoPtr<IDateFormatField> result;
            FAIL_RETURN(OfCalendarField(mCalendarField, (IDateFormatField**)&result));

            String name1, name2;
            if (result != NULL &&
                    (this->GetName(&name1), result->GetName(&name2), name1.Equals(name2))) {
                *resolvedField = (IInterface*)result.Get();
                (*resolvedField)->AddRef();
                return NOERROR;
            }
        //} catch (IllegalArgumentException e) {
        //    throw new InvalidObjectException("Unknown attribute");
        //}
    }
    else {
        if (this->Probe(EIID_IDateFormatField) == TIME_ZONE.Get()) {
            *resolvedField = (IInterface*)TIME_ZONE.Get();
            (*resolvedField)->AddRef();
            return NOERROR;
        }
        if (this->Probe(EIID_IDateFormatField) == HOUR1) {
            *resolvedField = (IInterface*)HOUR1.Get();
            (*resolvedField)->AddRef();
            return NOERROR;
        }
        if (this->Probe(EIID_IDateFormatField) == HOUR_OF_DAY1) {
            *resolvedField = (IInterface*)HOUR_OF_DAY1.Get();
            (*resolvedField)->AddRef();
            return NOERROR;
        }
    }
    //throw new InvalidObjectException("Unknown attribute");
    return E_INVALID_OBJECT_EXCEPTION;
}


DateFormat::DateFormat()
{}

ECode DateFormat::FormatObjectEx(
    /* [in] */ IInterface* object,
    /* [in] */ const String& buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ String* value)
{
    if (object->Probe(EIID_IDate) != NULL) {
        return FormatDateEx(reinterpret_cast<IDate*>(object->Probe(EIID_IDate)), buffer, field, value);
    }
    if (object->Probe(EIID_INumber) != NULL) {
        //return format(new Date(((Number) object).longValue()), buffer,
        //                field);
        assert(0);
        return E_NOT_IMPLEMENTED;
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode DateFormat::FormatDate(
    /* [in] */ IDate* date,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);

    AutoPtr<IFieldPosition> field;
    CFieldPosition::New(0, (IFieldPosition**)&field);
    return FormatDateEx(date, String(NULL), field, value);
}

ECode DateFormat::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    AutoPtr<IICUHelper> ICUHelper;
    FAIL_RETURN(CICUHelper::AcquireSingleton((IICUHelper**)&ICUHelper));
    return ICUHelper->GetAvailableDateFormatLocales(locales);
}

ECode DateFormat::GetCalendar(
    /* [out] */ ICalendar** calendar)
{
    *calendar = mCalendar;
    if (*calendar != NULL) (*calendar)->AddRef();
    return NOERROR;
}

ECode DateFormat::GetDateInstance(
    /* [out] */ IDateFormat** instance)
{
    return GetDateInstance(IDateFormat_DEFAULT, instance);
}

ECode DateFormat::GetDateInstance(
    /* [in] */ Int32 style,
    /* [out] */ IDateFormat** instance)
{
    FAIL_RETURN(CheckDateStyle(style));
    AutoPtr<ILocaleHelper> localeHelper;
    FAIL_RETURN(CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper));
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    return GetDateInstance(style, locale, instance);
}

ECode DateFormat::GetDateInstance(
    /* [in] */ Int32 style,
    /* [in] */ ILocale* locale,
    /* [out] */ IDateFormat** instance)
{
    assert(0);
    FAIL_RETURN(CheckDateStyle(style));
    //return new SimpleDateFormat(LocaleData.get(locale).getDateFormat(style), locale);
    return E_NOT_IMPLEMENTED;
}

ECode DateFormat::GetDateTimeInstance(
    /* [out] */ IDateFormat** instance)
{
    return GetDateTimeInstance(IDateFormat_DEFAULT, IDateFormat_DEFAULT, instance);
}

ECode DateFormat::GetDateTimeInstance(
    /* [in] */ Int32 dateStyle,
    /* [in] */ Int32 timeStyle,
    /* [out] */ IDateFormat** instance)
{
    FAIL_RETURN(CheckTimeStyle(timeStyle));
    FAIL_RETURN(CheckDateStyle(dateStyle));
    AutoPtr<ILocaleHelper> localeHelper;
    FAIL_RETURN(CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper));
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return GetDateTimeInstance(dateStyle, timeStyle, locale, instance);
}

ECode DateFormat::GetDateTimeInstance(
    /* [in] */ Int32 dateStyle,
    /* [in] */ Int32 timeStyle,
    /* [in] */ ILocale* locale,
    /* [out] */ IDateFormat** instance)
{
    assert(0);
    FAIL_RETURN(CheckTimeStyle(timeStyle));
    FAIL_RETURN(CheckDateStyle(dateStyle));
    //LocaleData localeData = LocaleData.get(locale);
    //String pattern = localeData.getDateFormat(dateStyle) + " " + localeData.getTimeFormat(timeStyle);
    //return new SimpleDateFormat(pattern, locale);
    return E_NOT_IMPLEMENTED;
}

ECode DateFormat::GetInstance(
    /* [out] */ IDateFormat** instance)
{
    return GetDateTimeInstance(IDateFormat_SHORT, IDateFormat_SHORT, instance);
}

ECode DateFormat::GetNumberFormat(
    /* [out] */ INumberFormat** numberFormat)
{
    *numberFormat = mNumberFormat;
    if (*numberFormat != NULL) (*numberFormat)->AddRef();
    return NOERROR;
}

ECode DateFormat::GetTimeInstance(
    /* [out] */ IDateFormat** instance)
{
    return GetTimeInstance(IDateFormat_DEFAULT, instance);
}

ECode DateFormat::GetTimeInstance(
    /* [in] */ Int32 style,
    /* [out] */ IDateFormat** instance)
{
    FAIL_RETURN(CheckTimeStyle(style));
    AutoPtr<ILocaleHelper> localeHelper;
    FAIL_RETURN(CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper));
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    return GetTimeInstance(style, locale, instance);
}

ECode DateFormat::GetTimeInstance(
    /* [in] */ Int32 style,
    /* [in] */ ILocale* locale,
    /* [out] */ IDateFormat** instance)
{
    assert(0);
    FAIL_RETURN(CheckTimeStyle(style));
    //return new SimpleDateFormat(LocaleData.get(locale).getTimeFormat(style), locale);
    return E_NOT_IMPLEMENTED;
}

ECode DateFormat::GetTimeZone(
    /* [out] */ ITimeZone** tz)
{
    return mCalendar->GetTimeZone(tz);
}

ECode DateFormat::IsLenient(
    /* [out] */ Boolean* isLenient)
{
    return mCalendar->IsLenient(isLenient);
}

ECode DateFormat::Parse(
    /* [in] */ const String& string,
    /* [out] */ IDate** date)
{
    AutoPtr<IParsePosition> position;
    CParsePosition::New(0, (IParsePosition**)&position);
    FAIL_RETURN(ParseEx(string, position, date));
    Int32 index;
    position->GetIndex(&index);
    if (index == 0) {
        //throw new ParseException("Unparseable date: \"" + string + "\"",
        //        position.getErrorIndex());
        return E_PARSE_EXCEPTION;
    }
    return NOERROR;
}

ECode DateFormat::ParseObjectEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ IInterface** object)
{
    return ParseEx(string, position, (IDate**)object);
}

ECode DateFormat::SetCalendar(
    /* [in] */ ICalendar* cal)
{
    mCalendar = cal;
    return NOERROR;
}

ECode DateFormat::SetLenient(
    /* [in] */ Boolean value)
{
    return mCalendar->SetLenient(value);
}

ECode DateFormat::SetNumberFormat(
   /* [in] */ INumberFormat* format)
{
    mNumberFormat = format;
    return NOERROR;
}

ECode DateFormat::SetTimeZone(
    /* [in] */ ITimeZone* timezone)
{
    return mCalendar->SetTimeZone(timezone);
}

ECode DateFormat::CheckDateStyle(
    /* [in] */ Int32 style)
{
    if (!(style == IDateFormat_SHORT
            || style == IDateFormat_MEDIUM
            || style == IDateFormat_LONG
            || style == IDateFormat_FULL
            || style == IDateFormat_DEFAULT)) {
        //throw new IllegalArgumentException("Illegal date style " + style);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode DateFormat::CheckTimeStyle(
    /* [in] */ Int32 style)
{
    if (!(style == IDateFormat_SHORT
            || style == IDateFormat_MEDIUM
            || style == IDateFormat_LONG
            || style == IDateFormat_FULL
            || style == IDateFormat_DEFAULT)) {
        //throw new IllegalArgumentException("Illegal time style " + style);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}
