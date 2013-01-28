#include "Timestamp.h"
#include "cmdef.h"

using namespace Elastos::System;

const Int64  Timestamp::mSerialVersionUID = 2745179027874758501L;
const String Timestamp::PADDING = "000000000";
const String Timestamp::TIME_FORMAT_REGEX = "[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}.*";


Timestamp::Timestamp(){
}

Timestamp::~Timestamp(){
}

ECode Timestamp::Init(
    /**[in]**/Int32 theYear, 
    /**[in]**/Int32 theMonth, 
    /**[in]**/Int32 theDate, 
    /**[in]**/Int32 theHour,
    /**[in]**/Int32 theMinute, 
    /**[in]**/Int32 theSecond, 
    /**[in]**/Int32 theNano)
{

}

ECode Timestamp::Init(
    /**[in]**/Int64 theTime)
{

}

ECode Timestamp::After(
    /**[in]**/ITimestamp* theTimestamp,
    /**[out]**/Boolean* result)
{

}

ECode Timestamp::Before(
    /**[in]**/ITimestamp* theTimestamp,
    /**[out]**/Boolean* result)
{

}

ECode Timestamp::CompareTo(
    /**[in]**/IDate* theObject,
    /**[out]**/Int32* result)
{

}

ECode Timestamp::CompareToEx(
    /**[in]**/ITimestamp* theTimestamp,
    /**[out]**/Int32* result)
{

}

ECode Timestamp::Equals(
    /**[in]**/IInterface* theObject,
    /**[out]**/Boolean* result)
{

}

ECode Timestamp::EqualsEx(
    /**[in]**/IInterface* theObject,
    /**[out]**/Boolean* result)
{

}

ECode Timestamp::GetNanos(
    /**[out]**/Int32* result)
{

}

ECode Timestamp::GetTime(
    /**[out]**/Int64* result)
{

}

ECode Timestamp::SetNanos(
    /**[in]**/Int32 n)
{

}

ECode Timestamp::SetTime(
    /**[in]**/Int64 theTime)
{

}

ECode Timestamp::ToString(
    /**[out]**/String* str)
{

}

ECode Timestamp::ValueOf(
    /**[in]**/String s,
    /**[out]**/ITimestamp** result)
{

}

void Timestamp::SetTimeImpl(
    /**[in]**/Int64 theTime)
{

}

void Timestamp::Format(
    /**[in]**/Int32 date, 
    /**[in]**/Int32 digits, 
    /**[in]**/StringBuilder sb)
{

}