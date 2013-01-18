
#include "CTimeZone.h"


const Int32 TIMEZONE_LONG = 1;
const Int32 TIMEZONE_SHORT = 0;

ECode CTimeZone::GetDisplayName(
    /* [out] */ String * pName)
{
    // TODO: Add your code here
    AutoPtr<ILocale> locale;
    return GetDisplayNameEx2(FALSE, TIMEZONE_LONG, (ILocale *)locale, pName);
}

ECode CTimeZone::GetDisplayNameEx(
    /* [in] */ ILocale * pLocale,
    /* [out] */ String * pName)
{
    // TODO: Add your code here
    return GetDisplayNameEx2(FALSE, TIMEZONE_LONG, pLocale, pName);
}

ECode CTimeZone::GetDisplayNameEx1(
    /* [in] */ Boolean daylightTime,
    /* [in] */ Int32 style,
    /* [out] */ String * pName)
{
    // TODO: Add your code here
    AutoPtr<ILocale> locale;
    return GetDisplayNameEx2(daylightTime, style, (ILocale *)locale, pName);
}

ECode CTimeZone::GetDisplayNameEx2(
    /* [in] */ Boolean daylightTime,
    /* [in] */ Int32 style,
    /* [in] */ ILocale * pLocale,
    /* [out] */ String * pName)
{
    // TODO: Add your code here
    if (style != TIMEZONE_SHORT && style != TIMEZONE_LONG) {
        return E_INVALID_ARGUMENT;
    }

    Boolean isUsed;
    UseDaylightTimeVir(&isUsed);

    Boolean useDaylight = daylightTime && isUsed;

//    String result = TimeZones.getDisplayName(getID(), daylightTime, style, locale);
//    if (result != null) {
//        return result;
//    }

    Int32 offset;
    GetRawOffsetVir(&offset);

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


void CTimeZone::AppendNumber(
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


ECode CTimeZone::GetID(
    /* [out] */ String * pId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTimeZone::GetDSTSavings(
    /* [out] */ Int32 * pSavings)
{
    // TODO: Add your code here
    Boolean isUsed;
    UseDaylightTimeVir(&isUsed);
    if (isUsed) {
        *pSavings = 360000;
    } else {
        *pSavings = 0;
    }
    return NOERROR;
}

ECode CTimeZone::GetOffset(
    /* [in] */ Int64 time,
    /* [out] */ Int32 * pOffset)
{
    // TODO: Add your code here
    Boolean isIn;
    AutoPtr<IDate> date;
    CDate::New(time, (IDate **) &date);
    InDaylightTimeVir(date, &isIn);
    Int32 offset, dst;
    GetRawOffsetVir(&offset);

    if (isIn) {
        GetDSTSavings(&dst);
        *pOffset = offset + dst;
        delete date;
    } else {
        *pOffset = offset;
    }
    return NOERROR;
}

//ECode CTimeZone::GetOffsetVir(
//    /* [in] */ Int32 era,
//    /* [in] */ Int32 year,
//    /* [in] */ Int32 month,
//    /* [in] */ Int32 day,
//    /* [in] */ Int32 dayOfWeek,
//    /* [in] */ Int32 timeOfDayMillis,
//    /* [out] */ Int32 * pOffset)
//{
//    // TODO: Add your code here
//    return E_NOT_IMPLEMENTED;
//}

//ECode CTimeZone::GetRawOffsetVir(
//    /* [out] */ Int32 * pRawOffset)
//{
//    // TODO: Add your code here
//    return E_NOT_IMPLEMENTED;
//}

ECode CTimeZone::HasSameRules(
    /* [in] */ ITimeZone * pTimeZone,
    /* [out] */ Boolean * pHasRules)
{
    // TODO: Add your code here
    if (pTimeZone == NULL) {
        return E_INVALID_ARGUMENT;
    }

    Int32 offset, toffset;
    GetRawOffsetVir(&offset);
    pTimeZone->GetRawOffsetVir(&toffset);
    *pHasRules = (offset == toffset);
    return NOERROR;
}

//ECode CTimeZone::InDaylightTimeVir(
//    /* [in] */ IDate * pTime,
//    /* [out] */ Boolean * pIsIn)
//{
//    // TODO: Add your code here
//    return E_NOT_IMPLEMENTED;
//}

ECode CTimeZone::SetID(
    /* [in] */ const String& id)
{
    if (id.IsNull()) {
        return E_INVALID_ARGUMENT;
    }

    mID = id;
    // TODO: Add your code here
    return NOERROR;
}

//ECode CTimeZone::SetRawOffsetVir(
//    /* [in] */ Int32 offsetMillis)
//{
//    // TODO: Add your code here
//    return E_NOT_IMPLEMENTED;
//}

//ECode CTimeZone::UseDaylightTimeVir(
//    /* [out] */ Boolean * pIsUsed)
//{
//    // TODO: Add your code here
//    return E_NOT_IMPLEMENTED;
//}

