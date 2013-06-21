
#include "cmdef.h"
#include "DateFormat.h"
#include "CFieldPosition.h"
#include "CParsePosition.h"
#include "CSimpleDateFormat.h"
#include <Elastos.IO.h>
#include <StringBuffer.h>

static AutoPtr<IDateFormatField> sInit(const String& name, Int32 value)
{
    DateFormat::Field* f = new DateFormat::Field();
    f->Init(name, value);
    return (IDateFormatField*)f;
}

const AutoPtr<IDateFormatField> DateFormat::Field::ERA
    = sInit(String("era"), Calendar_ERA);
const AutoPtr<IDateFormatField> DateFormat::Field::YEAR
    = sInit(String("year"), Calendar_YEAR);
const AutoPtr<IDateFormatField> DateFormat::Field::MONTH
    = sInit(String("month"), Calendar_MONTH);
const AutoPtr<IDateFormatField> DateFormat::Field::HOUR_OF_DAY0
    = sInit(String("hour of day"), Calendar_HOUR_OF_DAY);
const AutoPtr<IDateFormatField> DateFormat::Field::HOUR_OF_DAY1
    = sInit(String("hour of day 1"), -1);
const AutoPtr<IDateFormatField> DateFormat::Field::MINUTE
    = sInit(String("minute"), Calendar_MINUTE);
const AutoPtr<IDateFormatField> DateFormat::Field::SECOND
    = sInit(String("second"), Calendar_SECOND);
const AutoPtr<IDateFormatField> DateFormat::Field::MILLISECOND
    = sInit(String("millisecond"), Calendar_MILLISECOND);
const AutoPtr<IDateFormatField> DateFormat::Field::DAY_OF_WEEK
    = sInit(String("day of week"), Calendar_DAY_OF_WEEK);
const AutoPtr<IDateFormatField> DateFormat::Field::DAY_OF_MONTH
    = sInit(String("day of month"), Calendar_DAY_OF_MONTH);
const AutoPtr<IDateFormatField> DateFormat::Field::DAY_OF_YEAR
    = sInit(String("day of year"), Calendar_DAY_OF_YEAR);
const AutoPtr<IDateFormatField> DateFormat::Field::DAY_OF_WEEK_IN_MONTH
    = sInit(String("day of week in month"), Calendar_DAY_OF_WEEK_IN_MONTH);
const AutoPtr<IDateFormatField> DateFormat::Field::WEEK_OF_YEAR
    = sInit(String("week of year"), Calendar_WEEK_OF_YEAR);
const AutoPtr<IDateFormatField> DateFormat::Field::WEEK_OF_MONTH
    = sInit(String("week of month"), Calendar_WEEK_OF_MONTH);
const AutoPtr<IDateFormatField> DateFormat::Field::AM_PM
    = sInit(String("am pm"), Calendar_AM_PM);
const AutoPtr<IDateFormatField> DateFormat::Field::HOUR0
    = sInit(String("hour"), Calendar_HOUR);
const AutoPtr<IDateFormatField> DateFormat::Field::HOUR1
    = sInit(String("hour 1"), -1);
const AutoPtr<IDateFormatField> DateFormat::Field::TIME_ZONE
    = sInit(String("time zone"), -1);

HashMap<Int32, AutoPtr<IDateFormatField> > DateFormat::Field::sTable(11);

PInterface DateFormat::Field::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IDateFormatField*)this;
    }
    else if (riid == EIID_IDateFormatField) {
        return (IDateFormatField*)this;
    }

    return NULL;
}

UInt32 DateFormat::Field::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 DateFormat::Field::Release()
{
    return ElRefBase::Release();
}

ECode DateFormat::Field::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IDateFormatField*)this) {
        *pIID = EIID_IDateFormatField;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode DateFormat::Field::GetClassID(
    /* [out] */ ClassID* clsid)
{
    return E_NOT_IMPLEMENTED;
}

DateFormat::Field::Field()
    : mCalendarField(-1)
{ }

DateFormat::Field::Init(
    /* [in] */ const String& fieldName,
    /* [in] */ Int32 calendarField)
{
    FAIL_RETURN(Format::Field::Init(fieldName));
    mCalendarField = calendarField;
    if (calendarField != -1 && sTable.Find(calendarField) == sTable.End()) {
        sTable[calendarField] = (IDateFormatField*)Probe(EIID_IDateFormatField);
    }

    return NOERROR;
}

ECode DateFormat::Field::GetName(
    /* [out] */ String* name)
{
    return Format::Field::GetName(name);
}

ECode DateFormat::Field::GetCalendarField(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mCalendarField;
    return NOERROR;
}

ECode DateFormat::Field::OfCalendarField(
    /* [in] */ Int32 calendarField,
    /* [out] */ IDateFormatField** field)
{
    VALIDATE_NOT_NULL(field);
    if (calendarField < 0 || calendarField >= Calendar_FIELD_COUNT) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

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
    InterfaceID iid;
    GetInterfaceID((IInterface*)this, &iid);
    if (iid != EIID_IDateFormatField) {
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
    VALIDATE_NOT_NULL(value);

    if (object->Probe(EIID_IDate) != NULL) {
        return FormatDateEx((IDate*)object->Probe(EIID_IDate), buffer, field, value);
    }
    if (object->Probe(EIID_INumber) != NULL) {
        Int64 v;
        ((INumber*)object->Probe(EIID_INumber))->Int64Value(&v);
        AutoPtr<IDate> date;
        CDate::New(v, (IDate**)&date);
        return FormatDateEx(date, buffer, field, value);
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
    /* [out] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);
    AutoPtr<IICUHelper> ICUHelper;
    FAIL_RETURN(CICUHelper::AcquireSingleton((IICUHelper**)&ICUHelper));
    return ICUHelper->GetAvailableDateFormatLocales(locales);
}

ECode DateFormat::GetCalendar(
    /* [out] */ ICalendar** calendar)
{
    VALIDATE_NOT_NULL(calendar);
    *calendar = mCalendar;
    if (*calendar != NULL) (*calendar)->AddRef();
    return NOERROR;
}

ECode DateFormat::GetDateInstance(
    /* [out] */ IDateFormat** instance)
{
    return GetDateInstanceEx(IDateFormat_DEFAULT, instance);
}

ECode DateFormat::GetDateInstanceEx(
    /* [in] */ Int32 style,
    /* [out] */ IDateFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    FAIL_RETURN(CheckDateStyle(style));

    AutoPtr<ILocaleHelper> localeHelper;
    FAIL_RETURN(CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper));
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));

    return GetDateInstanceEx2(style, locale, instance);
}

ECode DateFormat::GetDateInstanceEx2(
    /* [in] */ Int32 style,
    /* [in] */ ILocale* locale,
    /* [out] */ IDateFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    FAIL_RETURN(CheckDateStyle(style));

    AutoPtr<ILocaleDataHelper> localeDataHelper;
    FAIL_RETURN(CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&localeDataHelper));
    AutoPtr<ILocaleData> localeData;
    FAIL_RETURN(localeDataHelper->Get(locale, (ILocaleData**)&localeData));
    String format;
    localeData->GetDateFormat(style, &format);

    AutoPtr<ISimpleDateFormat> f;
    FAIL_RETURN(CSimpleDateFormat::New(format, locale, (ISimpleDateFormat**)&f));
    *instance = (IDateFormat*)f->Probe(EIID_IDateFormat);
    if (*instance != NULL) (*instance)->AddRef();

    return NOERROR;
}

ECode DateFormat::GetDateTimeInstance(
    /* [out] */ IDateFormat** instance)
{
    return GetDateTimeInstanceEx(IDateFormat_DEFAULT, IDateFormat_DEFAULT, instance);
}

ECode DateFormat::GetDateTimeInstanceEx(
    /* [in] */ Int32 dateStyle,
    /* [in] */ Int32 timeStyle,
    /* [out] */ IDateFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    FAIL_RETURN(CheckTimeStyle(timeStyle));
    FAIL_RETURN(CheckDateStyle(dateStyle));

    AutoPtr<ILocaleHelper> localeHelper;
    FAIL_RETURN(CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper));
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);

    return GetDateTimeInstanceEx2(dateStyle, timeStyle, locale, instance);
}

ECode DateFormat::GetDateTimeInstanceEx2(
    /* [in] */ Int32 dateStyle,
    /* [in] */ Int32 timeStyle,
    /* [in] */ ILocale* locale,
    /* [out] */ IDateFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    FAIL_RETURN(CheckTimeStyle(timeStyle));
    FAIL_RETURN(CheckDateStyle(dateStyle));

    AutoPtr<ILocaleDataHelper> localeDataHelper;
    FAIL_RETURN(CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&localeDataHelper));
    AutoPtr<ILocaleData> localeData;
    FAIL_RETURN(localeDataHelper->Get(locale, (ILocaleData**)&localeData));
    String dateFormat, timeFormat;
    localeData->GetDateFormat(dateStyle, &dateFormat);
    localeData->GetTimeFormat(timeStyle, &timeFormat);
    String pattern;
    pattern.Append(dateFormat);
    pattern.Append(" ");
    pattern.Append(timeFormat);

    AutoPtr<ISimpleDateFormat> f;
    FAIL_RETURN(CSimpleDateFormat::New(pattern, locale, (ISimpleDateFormat**)&f));
    *instance = (IDateFormat*)f->Probe(EIID_IDateFormat);
    if (*instance != NULL) (*instance)->AddRef();

    return NOERROR;
}

ECode DateFormat::GetInstance(
    /* [out] */ IDateFormat** instance)
{
    return GetDateTimeInstanceEx(IDateFormat_SHORT, IDateFormat_SHORT, instance);
}

ECode DateFormat::GetNumberFormat(
    /* [out] */ INumberFormat** numberFormat)
{
    VALIDATE_NOT_NULL(numberFormat);
    *numberFormat = mNumberFormat;
    if (*numberFormat != NULL) (*numberFormat)->AddRef();
    return NOERROR;
}

ECode DateFormat::GetTimeInstance(
    /* [out] */ IDateFormat** instance)
{
    return GetTimeInstanceEx(IDateFormat_DEFAULT, instance);
}

ECode DateFormat::GetTimeInstanceEx(
    /* [in] */ Int32 style,
    /* [out] */ IDateFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    FAIL_RETURN(CheckTimeStyle(style));
    AutoPtr<ILocaleHelper> localeHelper;
    FAIL_RETURN(CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper));
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));

    return GetTimeInstanceEx2(style, locale, instance);
}

ECode DateFormat::GetTimeInstanceEx2(
    /* [in] */ Int32 style,
    /* [in] */ ILocale* locale,
    /* [out] */ IDateFormat** instance)
{
    VALIDATE_NOT_NULL(instance);

    FAIL_RETURN(CheckTimeStyle(style));

    AutoPtr<ILocaleDataHelper> localeDataHelper;
    FAIL_RETURN(CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&localeDataHelper));
    AutoPtr<ILocaleData> localeData;
    FAIL_RETURN(localeDataHelper->Get(locale, (ILocaleData**)&localeData));
    String timeFormat;
    localeData->GetTimeFormat(style, &timeFormat);

    AutoPtr<ISimpleDateFormat> f;
    FAIL_RETURN(CSimpleDateFormat::New(timeFormat, locale, (ISimpleDateFormat**)&f));
    *instance = (IDateFormat*)f->Probe(EIID_IDateFormat);
    if (*instance != NULL) (*instance)->AddRef();

    return NOERROR;
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
    VALIDATE_NOT_NULL(date);

    AutoPtr<IParsePosition> position;
    CParsePosition::New(0, (IParsePosition**)&position);
    FAIL_RETURN(ParseEx(string, position, date));
    (*date)->AddRef();
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
