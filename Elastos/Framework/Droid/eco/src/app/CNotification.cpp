
#include "app/CNotification.h"

const Int32 CNotification::DEFAULT_ALL;
const Int32 CNotification::DEFAULT_SOUND;
const Int32 CNotification::DEFAULT_VIBRATE;
const Int32 CNotification::DEFAULT_LIGHTS;
const Int32 CNotification::STREAM_DEFAULT;
const Int32 CNotification::FLAG_SHOW_LIGHTS;
const Int32 CNotification::FLAG_ONGOING_EVENT;
const Int32 CNotification::FLAG_INSISTENT;
const Int32 CNotification::FLAG_ONLY_ALERT_ONCE;
const Int32 CNotification::FLAG_AUTO_CANCEL;
const Int32 CNotification::FLAG_NO_CLEAR;
const Int32 CNotification::FLAG_FOREGROUND_SERVICE;

CNotification::CNotification()
    : mAudioStreamType(STREAM_DEFAULT)
{
}

ECode CNotification::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CNotification::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    return E_NOT_IMPLEMENTED;
}

