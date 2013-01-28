#include "Date.h"
#include "cmdef.h"

using namespace Elastos::System;

const Int64  Date::mSerialVersionUID = 1511598038487230103L;
const String Date::PADDING = "0000";

Date::Date(){
}

Date::~Date(){
}

ECode Date::Init(
    /** [in] **/Int32 theYear, 
    /** [in] **/Int32 theMonth, 
    /** [in] **/Int32 theDay)
{
    //super(theYear, theMonth, theDay);
    return NOERROR;
}

ECode Date::Init(
    /** [in] **/Int64 theDate)
{
    //super(normalizeTime(theDate));
    return NOERROR;
}

ECode Date::GetHours(
    /** [out] **/Int32* hours)
{
    return E_SQL_EXCEPTION;
}

ECode Date::GetMinutes(
    /** [out] **/Int32* minutes)
{
    return E_SQL_EXCEPTION;
}

ECode Date::GetSeconds(
    /** [out] **/Int32* seconds)
{
    return E_SQL_EXCEPTION;
}

ECode Date::SetHours(
    /** [in] **/Int32 theHours)
{
    return E_SQL_EXCEPTION;
}

ECode Date::SetMinutes(
    /** [in] **/Int32 theMinutes)
{
    return E_SQL_EXCEPTION;
}

ECode Date::SetSeconds(
    /** [in] **/Int32 theSeconds)
{
    return E_SQL_EXCEPTION;
}

ECode Date::SetTime(
    /** [in] **/Int64 theTime)
{
    /*
     * Store the Date based on the supplied time after removing any time
     * elements finer than the day based on zero GMT
     */
    //super.setTime(normalizeTime(theTime));
     return NOERROR;
}

ECode Date::ToString(
    /** [out] **/String* str)
{
    return E_NOT_IMPLEMENTED;
}

ECode Date::ValueOf(
    /** [in] **/String dateString,
    /** [out] **/IDate** date)
{
    if (dateString.IsNull()) {
        return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 firstIndex = dateString.indexOfChar('-');
    Int32 secondIndex = dateString.indexOfChar('-', firstIndex + 1);
    // secondIndex == -1 means none or only one separator '-' has been
    // found.
    // The string is separated into three parts by two separator characters,
    // if the first or the third part is null string, we should throw
    // IllegalArgumentException to follow RI
    if (secondIndex == -1 || firstIndex == 0
            || secondIndex + 1 == dateString.GetLength()) {
        return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // parse each part of the string
    StringBuf_<20> subYear;
    StringBuf_<20> subMonth;
    StringBuf_<20> subDay;
    String strYear = dateString.substring(0,firstIndex,subYear);
    String strMonth = dateString.substring(firstIndex+1,secondIndex-firstIndex-1,subMonth);
    String strDay = dateString.substring(econdIndex+1,dateString.GetLength()-econdIndex-1,subDay);
    Int32 year = strYear.ToInt32();
    Int32 month = strMonth.ToInt32();
    Int32 day = strDay.ToInt32();

//    return new Date(year - 1900, month - 1, day);
    return ;////////////////////////////
}

void Date::Format(
    /** [in] **/Int32 date, 
    /** [in] **/Int32 digits, 
    /** [in] **/StringBuilder sb)
{
    String str = String.valueOf(date);
    if (digits - str.length() > 0) {
        sb.append(PADDING.substring(0, digits - str.length()));
    }
    sb.append(str);
}

Int64 Date::NormalizeTime(
    /** [in] **/Int64 theTime)
{
    return theTime;
}