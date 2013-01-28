#include "Time.h"
#include "cmdef.h"

using namespace Elastos::System;

const Int64  Time::mSerialVersionUID = 8397324403548013681L;
const String Time::mPADDING = "00";

Time::Time(){}

Time::~Time(){}

ECode Time::Init(
    /**[in]**/Int32 theHour, 
    /**[in]**/Int32 theMinute, 
    /**[in]**/Int32 theSecond)
{
    //super(70, 0, 1, theHour, theMinute, theSecond);
    return NPERROR;
}

ECode Time::Init(
    /**[in]**/Int64 theTime)
{
    //super(theTime);
    return NPERROR;
}

ECode Time::GetDate(
    /**[out]**/Int32* result)
{
    return E_SQL_EXCEPTION;
}

ECode Time::GetDayEx(
    /**[out]**/Int32* result)
{
    return E_SQL_EXCEPTION;
}

ECode Time::GetMonth(
    /**[out]**/Int32* result)
{
    return E_SQL_EXCEPTION;
}

ECode Time::GetYear(
    /**[out]**/Int32* result)
{
    return E_SQL_EXCEPTION;
}

ECode Time::SetDate(
    /**[in]**/Int32 i)
{
    return E_SQL_EXCEPTION;
}

ECode Time::SetMonth(
    /**[in]**/Int32 i)
{
    return E_SQL_EXCEPTION;
}

ECode Time::SetYear(
    /**[in]**/Int32 i)
{
    return E_SQL_EXCEPTION;
}

ECode Time::SetTime(
    /**[in]**/Int64 time)
{
    //super.setTime(time);
    return NOERROR;
}

ECode Time::ToString(
    /**[out]**/String* str)
{
    return E_NOT_IMPLEMENTED;
}

ECode Time::ValueOf(
    /**[in]**/String timeString,
    /**[out]**/ITime** time)
{/*
    if (timeString == null) {
        throw new IllegalArgumentException();
    }
    int firstIndex = timeString.indexOf(':');
    int secondIndex = timeString.indexOf(':', firstIndex + 1);
    // secondIndex == -1 means none or only one separator '-' has been
    // found.
    // The string is separated into three parts by two separator characters,
    // if the first or the third part is null string, we should throw
    // IllegalArgumentException to follow RI
    if (secondIndex == -1 || firstIndex == 0
            || secondIndex + 1 == timeString.length()) {
        throw new IllegalArgumentException();
    }
    // parse each part of the string
    int hour = Integer.parseInt(timeString.substring(0, firstIndex));
    int minute = Integer.parseInt(timeString.substring(firstIndex + 1,
            secondIndex));
    int second = Integer.parseInt(timeString.substring(secondIndex + 1,
            timeString.length()));
    return new Time(hour, minute, second);*/
    return NOERROR;
}

void Time::Format(
    /**[in]**/Int32 date, 
    /**[in]**/Int32 digits, 
    /**[in]**/StringBuilder sb)
{/*
    String str = String.valueOf(date);
    if (digits - str.length() > 0) {
        sb.append(PADDING.substring(0, digits - str.length()));
    }
    sb.append(str);*/
    return NOERROR;
}