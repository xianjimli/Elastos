#include "DateFormat.h"
#include "cmdef.h"

const AutoPtr<IDateFormat_Field> DateFormat::DateFormat_Field::ERA;

const AutoPtr<IDateFormat_Field> DateFormat::DateFormat_Field::YEAR;

const AutoPtr<IDateFormat_Field> DateFormat::DateFormat_Field::MONTH;

const AutoPtr<IDateFormat_Field> DateFormat::DateFormat_Field::HOUR_OF_DAY0;

const AutoPtr<IDateFormat_Field> DateFormat::DateFormat_Field::HOUR_OF_DAY1;

const AutoPtr<IDateFormat_Field> DateFormat::DateFormat_Field::MINUTE;

const AutoPtr<IDateFormat_Field> DateFormat::DateFormat_Field::SECOND;

const AutoPtr<IDateFormat_Field> DateFormat::DateFormat_Field::MILLISECOND;

const AutoPtr<IDateFormat_Field> DateFormat::DateFormat_Field::DAY_OF_WEEK;

const AutoPtr<IDateFormat_Field> DateFormat::DateFormat_Field::DAY_OF_MONTH;

const AutoPtr<IDateFormat_Field> DateFormat::DateFormat_Field::DAY_OF_YEAR;

const AutoPtr<IDateFormat_Field> DateFormat::DateFormat_Field::DAY_OF_WEEK_IN_MONTH;

const AutoPtr<IDateFormat_Field> DateFormat::DateFormat_Field::WEEK_OF_YEAR;

const AutoPtr<IDateFormat_Field> DateFormat::DateFormat_Field::WEEK_OF_MONTH;

const AutoPtr<IDateFormat_Field> DateFormat::DateFormat_Field::AM_PM;

const AutoPtr<IDateFormat_Field> DateFormat::DateFormat_Field::HOUR0;

const AutoPtr<IDateFormat_Field> DateFormat::DateFormat_Field::HOUR1;

const AutoPtr<IDateFormat_Field> DateFormat::DateFormat_Field::TIME_ZONE;

//Int32 DateFormat::DateFormat_Field::calendarField = -1;

DateFormat::DateFormat()
{
    //mCalendar = new Calendar();
}

DateFormat::~DateFormat()
{
}

ECode DateFormat::formatEx(
        /* [in] */ IInterface* object,
        /* [in] */ StringBuffer* buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ StringBuffer* value)
{
    if (object->Probe(EIID_IDate) != NULL) {
        return formatEx3((IDate*)object, buffer, field, value);
    }
    if (object->Probe(EIID_INumber) != NULL) {
        //return format(new Date(((Number) object).longValue()), buffer,
        //                field);
        return E_NOT_IMPLEMENTED;
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode DateFormat::formatEx2(
        /* [in] */ IDate* date,
        /* [out] */ String* formatString)
{
    VALIDATE_NOT_NULL(formatString);

    AutoPtr<IFieldPosition> field;
    CFieldPosition::New(0, (IFieldPosition**)&field);
    StringBuffer * sb = new StringBuffer(NULL);
    formatEx3(date, new StringBuffer(NULL), field, sb);
    *formatString = sb->Substring(0, sb->GetLength() );

    return NOERROR;
}

ECode DateFormat::GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    AutoPtr<IICUHelper> pICUHelper;
    CICUHelper::AcquireSingleton((IICUHelper**)&pICUHelper);
    return pICUHelper->GetAvailableDateFormatLocales(locales);
}

ECode DateFormat::GetCalendar(
        /* [out] */ ICalendar** calendar)
{
    *calendar = mCalendar;
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
    AutoPtr<ILocale> locale;

    AutoPtr<ILocaleHelper> pLocaleHelper;
    ECode ec = NOERROR;
    ec = CLocaleHelper::AcquireSingleton((ILocaleHelper **)&pLocaleHelper);

    pLocaleHelper->GetDefault((ILocale**)&locale);
    return GetDateInstance(style, locale, instance);
}

ECode DateFormat::GetDateInstance(
    /* [in] */ Int32 style,
    /* [in] */ ILocale* locale,
    /* [out] */ IDateFormat** instance)
{
    CheckDateStyle(style);
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
    CheckTimeStyle(timeStyle);
    CheckDateStyle(dateStyle);
    AutoPtr<ILocaleHelper> pLocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pLocaleHelper);
    AutoPtr<ILocale> locale;
    pLocaleHelper->GetDefault((ILocale**)&locale);
    return GetDateTimeInstance(dateStyle, timeStyle, locale, instance);

}

ECode DateFormat::GetDateTimeInstance(
        /* [in] */ Int32 dateStyle,
        /* [in] */ Int32 timeStyle,
        /* [in] */ ILocale* locale,
        /* [out] */ IDateFormat** instance)
{
    CheckTimeStyle(timeStyle);
    CheckDateStyle(dateStyle);
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

ECode DateFormat::GetTimeInstance(
    /* [out] */ IDateFormat** instance)
{
    return GetTimeInstance(IDateFormat_DEFAULT, instance);
}

ECode DateFormat::GetTimeInstance(
    /* [in] */ Int32 style,
    /* [out] */ IDateFormat** instance)
{
    CheckTimeStyle(style);
    AutoPtr<ILocaleHelper> pLocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pLocaleHelper);
    AutoPtr<ILocale> locale;
    pLocaleHelper->GetDefault((ILocale**)&locale);
    return GetTimeInstance(style, locale, instance);

}

ECode DateFormat::GetTimeInstance(
    /* [in] */ Int32 style,
    /* [in] */ ILocale* locale,
    /* [out] */ IDateFormat** instance)
{
    CheckTimeStyle(style);
    //return new SimpleDateFormat(LocaleData.get(locale).getTimeFormat(style), locale);
    return E_NOT_IMPLEMENTED;
}

ECode DateFormat::GetTimeZone(
    /* [out] */ ITimeZone** tz)
{
    mCalendar->GetTimeZone(tz);
    return NOERROR;
}

ECode DateFormat::IsLenient(
    /* [out] */ Boolean* isLenient)
{
    VALIDATE_NOT_NULL(isLenient);

    mCalendar->IsLenient(isLenient);
    return NOERROR;
}

ECode DateFormat::Parse(
    /* [in] */ String string,
    /* [out] */ IDate** date)
{
    AutoPtr<IParsePosition> position;
    CParsePosition::New(0, (IParsePosition**)&position);
    ParseEx(string, position, date);
    Int32 index;
    position->GetIndex(&index);
    if (index == 0) {
        //throw new ParseException("Unparseable date: \"" + string + "\"",
        //        position.getErrorIndex());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode DateFormat::ParseObjectEx(
        /* [in] */ String string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IInterface** object)
{
    AutoPtr<IDate> date;
    ParseEx(string, position, (IDate**)&date);
    object = (IInterface**)&date;
    return NOERROR;
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
    mCalendar->SetLenient(value);
    return NOERROR;
}

//ECode DateFormat::SetNumberFormat(
//    /* [in] */ INumberFormat* format)
//{
//    numberFormat = format;
//}

ECode DateFormat::SetTimeZone(
    /* [in] */ ITimeZone* timezone)
{
    mCalendar->SetTimeZone(timezone);
    return NOERROR;
}

//DateFormat::DateFormat_Field::DateFormat_Field(
//    /* [in] */ String fieldName,
//    /* [in] */ Int32 calendarField)
//{
    //Format::Format_Field::Format_Field(fieldName);
//    this->calendarField = calendarField;
    //if (calendarField != -1 && table.get(Integer.valueOf(calendarField)) == null) {
    //    table.put(Integer.valueOf(calendarField), this);
    //}
//}

ECode DateFormat::DateFormat_Field::GetCalendarField(
        /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = calendarField;
    return NOERROR;
}

ECode DateFormat::DateFormat_Field::OfCalendarField(
        /* [in] */ Int32 calendarField,
        /* [out] */ IDateFormat_Field** dff)
{
    if (calendarField < 0 || calendarField >= Calendar_FIELD_COUNT) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    //return table.get(Integer.valueOf(calendarField));
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode DateFormat::DateFormat_Field::ReadResolve(
            /* [out] */ IInterface** resolvedField)
{
    if (this->Probe(EIID_IDateFormat_Field) != NULL) {
        //throw new InvalidObjectException("cannot resolve subclasses");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (calendarField != -1) {
        //try {
            AutoPtr<IDateFormat_Field> result;
            OfCalendarField(calendarField, (IDateFormat_Field**)&result);

            if (result != NULL /*&& this.getName().equals(result.getName())*/) {
                *resolvedField = (IInterface*)result;
                return NOERROR;
            }
        //} catch (IllegalArgumentException e) {
        //    throw new InvalidObjectException("Unknown attribute");
        //}
    /*} else {
        if (this == TIME_ZONE) {
            *resolvedField = (IInterface*)TIME_ZONE;
            return NOERROR;
        }
        if (this == HOUR1) {
            *resolvedField = (IInterface*)HOUR1;
            return NOERROR;
        }
        if (this == HOUR_OF_DAY1) {
            *resolvedField = (IInterface*)HOUR_OF_DAY1;
            return NOERROR;
        }*/
    }
    //throw new InvalidObjectException("Unknown attribute");
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
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



