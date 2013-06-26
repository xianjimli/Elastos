
#include "TimeZone.h"
#include "CSimpleTimeZone.h"
//#include "ZoneInfoDB.h"

ITimeZone* TimeZone::mDefaultTimeZone = NULL;

static AutoPtr<ITimeZone> CreateSimpleZone()
{
    AutoPtr<ISimpleTimeZone> tz;
    CSimpleTimeZone::NewByFriend(0, String("GMT"), (CSimpleTimeZone **) &tz);
    tz->AddRef();
    return (ITimeZone *) tz;
}

AutoPtr<ITimeZone> TimeZone::GMT = CreateSimpleZone();

ECode TimeZone::GetAvailableIDs(
    /* [out, callee] */ ArrayOf<String> ** ppArray)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode TimeZone::GetAvailableIDsEx(
    /* [in] */ Int32 offsetMillis,
    /* [out, callee] */ ArrayOf<String> ** ppArray)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode TimeZone::GetDefault(
    /* [out] */ ITimeZone ** ppTimezone)
{
    // TODO: Add your code here
    *ppTimezone = (ITimeZone *) GMT;
    return NOERROR;
}

ECode TimeZone::GetTimeZone(
    /* [in] */ const String& id,
    /* [out] */ ITimeZone ** ppTimezone)
{
    // TODO: Add your code here
    //ITimeZone* zone = ZoneInfoDB::GetTimeZone(id);
    //if (zone != NULL) {
    //    return zone;
    //}
    //*ppTimezone = zone;
//    if (zone == NULL && id.GetLength() > 3 && id.startsWith("GMT")) {
//        zone = getCustomTimeZone(id);
//    }
//    if (zone == null) {
//        zone = (TimeZone) GMT.clone();
//    }
    return NOERROR;
}

ECode TimeZone::SetDefault(
    /* [in] */ ITimeZone * pTimeZone)
{
    // TODO: Add your code here
    if (pTimeZone == NULL) {
        return E_INVALID_ARGUMENT;
    }

    mDefaultTimeZone = pTimeZone;
    mDefaultTimeZone->AddRef();
    return NOERROR;
}

TimeZone::TimeZone()
{
}

ECode TimeZone::GetDisplayName(
        /* [out] */ String * pName)
{
    AutoPtr<ILocale> locale;
    return GetDisplayNameEx3(FALSE, ITimeZone_LONG, (ILocale *)locale, pName);
}

ECode TimeZone::GetDisplayNameEx(
        /* [in] */ ILocale * pLocale,
        /* [out] */ String * pName)
{
    return GetDisplayNameEx3(FALSE, ITimeZone_LONG, pLocale, pName);
}

ECode TimeZone::GetDisplayNameEx2(
        /* [in] */ Boolean daylightTime,
        /* [in] */ Int32 style,
        /* [out] */ String * pName)
{
    AutoPtr<ILocale> locale;
    return GetDisplayNameEx3(daylightTime, style, (ILocale *)locale, pName);
}

ECode TimeZone::GetDisplayNameEx3(
        /* [in] */ Boolean daylightTime,
        /* [in] */ Int32 style,
        /* [in] */ ILocale * pLocale,
        /* [out] */ String * pName)
{
    if (style != ITimeZone_SHORT && style != ITimeZone_LONG) {
        return E_INVALID_ARGUMENT;
    }

    Boolean isUsed;
    UseDaylightTime(&isUsed);

    Boolean useDaylight = daylightTime && isUsed;

//    String result = TimeZones.getDisplayName(getID(), daylightTime, style, locale);
//    if (result != null) {
//        return result;
//    }

    Int32 offset;
    GetRawOffset(&offset);

    if (useDaylight) {
        Int32 savings;
        GetDSTSavings(&savings);
        offset += savings;
    }

    offset /= 60000;

    Byte sign = '+';
    if (offset < 0) {
        sign = '-';
        offset = -offset;
    }

    StringBuffer builder(9);
    builder += String("GMT");
    builder += String((char *)&sign, 1);
//    builder.append("GMT");
//    builder.append(sign);
    AppendNumber(&builder, 2, offset / 60);
//    builder.append(':');
    builder+= String(":");
    AppendNumber(&builder, 2, offset % 60);
    *pName = builder.Substring(0, 9);
    return NOERROR;
}

ECode TimeZone::GetID(
        /* [out] */ String * pId)
{
    *pId = mID;
    return NOERROR;
}

ECode TimeZone::GetDSTSavings(
        /* [out] */ Int32 * pSavings)
{
    Boolean isUsed;
    UseDaylightTime(&isUsed);
    if (isUsed) {
        *pSavings = 360000;
    } else {
        *pSavings = 0;
    }
    return NOERROR;
}

ECode TimeZone::GetOffset(
        /* [in] */ Int64 time,
        /* [out] */ Int32 * pOffset)
{
    Boolean isIn;
    AutoPtr<IDate> date;
    CDate::New(time, (IDate **) &date);
    InDaylightTime(date, &isIn);
    Int32 offset, dst;
    GetRawOffset(&offset);

    if (isIn) {
        GetDSTSavings(&dst);
        *pOffset = offset + dst;
    } else {
        *pOffset = offset;
    }
    return NOERROR;
}

ECode TimeZone::HasSameRules(
        /* [in] */ ITimeZone * pTimeZone,
        /* [out] */ Boolean * pHasRules)
{
    // TODO: Add your code here
    if (pTimeZone == NULL) {
        return E_INVALID_ARGUMENT;
    }

    Int32 offset, toffset;
    GetRawOffset(&offset);
    pTimeZone->GetRawOffset(&toffset);
    *pHasRules = (offset == toffset);
    return NOERROR;
}

ECode TimeZone::SetID(
        /* [in] */ const String& id)
{
    if (id.IsNull()) {
        return E_INVALID_ARGUMENT;
    }

    mID = id;
    // TODO: Add your code here
    return NOERROR;
}


void TimeZone::AppendNumber(
        /* [out] */ StringBuffer* buffer,
        /* [in] */ Int32 count,
        /* [in] */ Int32 value)
{
    String string;
    for (Int32 i = 0; i < count - string.GetLength(); i++) {
        Byte c = '0';
        (*buffer) += String((char *)&c, 1);
    }
    (*buffer) += string;
}

ITimeZone* TimeZone::GetCustomTimeZone(
        /* [in] */ const String& id)
{
    Char32 sign = id.GetChar(3);
    if (sign != '+' && sign != '-') {
        return NULL;
    }

    Int32 position;
    String formattedName = FormatTimeZoneName(id, 4);
    Int32 hour = ParseNumber(formattedName, 4, position);

    if (hour < 0 || hour > 23) {
        return NULL;
    }

    Int32 index = position;
    if (index == -1) {
        return NULL;
    }

    Int32 raw = hour * 3600000;
    if (index < formattedName.GetLength() && formattedName.GetChar(index) == ':') {
        int minute = ParseNumber(formattedName, index + 1, position);
        if (position == -1 || minute < 0 || minute > 59) {
            return NULL;
        }
        raw += minute * 60000;
    } else if (hour >= 30 || index > 6) {
        raw = (hour / 100 * 3600000) + (hour % 100 * 60000);
    }

    if (sign == '-') {
        raw = -raw;
    }

    ISimpleTimeZone* tz;
    CSimpleTimeZone::NewByFriend(raw, formattedName, (CSimpleTimeZone **) &tz);
    return (ITimeZone*)tz;
}


String TimeZone::FormatTimeZoneName(
    /* [in] */ const String& name,
    /* [in] */ Int32 offset)
{
}

Int32 TimeZone::ParseNumber(
    /* [in] */ const String& string,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 position)
{
}