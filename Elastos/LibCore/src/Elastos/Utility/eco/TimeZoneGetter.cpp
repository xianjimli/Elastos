#include "TimeZoneGetter.h"


TimeZoneGetter* TimeZoneGetter::mInstance = NULL;

TimeZoneGetter* TimeZoneGetter::GetInstance()
{
    return mInstance;
}

void TimeZoneGetter::SetInstance(TimeZoneGetter* getter)
{
    if (mInstance != NULL) {
        return;
    }
    mInstance = getter;
}

String TimeZoneGetter::GetId()
{
    //return SystemProperties.get("persist.sys.timezone");
    return String("");
}
