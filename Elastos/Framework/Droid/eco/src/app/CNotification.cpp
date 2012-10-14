
#include "ext/frameworkdef.h"
#include "app/CNotification.h"


CNotification::CNotification()
    : mAudioStreamType(Notification_STREAM_DEFAULT)
{
}

ECode CNotification::GetWhen(
    /* [out] */ Int64* when)
{
    VALIDATE_NOT_NULL(when);
    *when = mWhen;
    return NOERROR;
}

ECode CNotification::SetWhen(
    /* [in] */ Int64 when)
{
    mWhen = when;
    return NOERROR;
}

ECode CNotification::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    *icon = mIcon;
    return NOERROR;
}

ECode CNotification::SetIcon(
    /* [in] */ Int32 icon)
{
    mIcon = icon;
    return NOERROR;
}

ECode CNotification::GetContentIntent(
    /* [out] */ IPendingIntent** intent)
{
    VALIDATE_NOT_NULL(intent);
    *intent = mContentIntent;
    if (*intent != NULL) (*intent)->AddRef();
    return NOERROR;
}

ECode CNotification::SetContentIntent(
    /* [in] */ IPendingIntent* intent)
{
    mContentIntent = intent;
    return NOERROR;
}

ECode CNotification::GetDeleteIntent(
    /* [out] */ IPendingIntent** intent)
{
    VALIDATE_NOT_NULL(intent);
    *intent = mDeleteIntent;
    if (*intent != NULL) (*intent)->AddRef();
    return NOERROR;
}

ECode CNotification::SetDeleteIntent(
    /* [in] */ IPendingIntent* intent)
{
    mDeleteIntent = intent;
    return NOERROR;
}

ECode CNotification::GetTickerText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    *text = mTickerText;
    if (*text != NULL) (*text)->AddRef();
    return NOERROR;
}

ECode CNotification::SetTickerText(
    /* [in] */ ICharSequence* text)
{
    mTickerText = text;
    return NOERROR;
}

ECode CNotification::GetContentView(
    /* [out] */ IRemoteViews** view)
{
    VALIDATE_NOT_NULL(view);
    *view = mContentView;
    if (*view != NULL) (*view)->AddRef();
    return NOERROR;
}

ECode CNotification::SetContentView(
    /* [in] */ IRemoteViews* view)
{
    mContentView = view;
    return NOERROR;
}

ECode CNotification::GetSound(
    /* [out] */ IUri** sound)
{
    VALIDATE_NOT_NULL(sound);
    *sound = mSound;
    if (*sound != NULL) (*sound)->AddRef();
    return NOERROR;
}

ECode CNotification::SetSound(
    /* [in] */ IUri* sound)
{
    mSound = sound;
    return NOERROR;
}

ECode CNotification::GetAudioStreamType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mAudioStreamType;
    return NOERROR;
}

ECode CNotification::SetAudioStreamType(
    /* [in] */ Int32 type)
{
    mAudioStreamType = type;
    return NOERROR;
}

ECode CNotification::GetVibrate(
    /* [out, callee] */ ArrayOf<Int64>** vibrate)
{
    VALIDATE_NOT_NULL(vibrate);
    *vibrate = mVibrate != NULL ? mVibrate->Clone() : NULL;
    return NOERROR;
}

ECode CNotification::SetVibrate(
    /* [in] */ ArrayOf<Int64>* vibrate)
{
    mVibrate = vibrate != NULL ? vibrate->Clone() : NULL;
    return NOERROR;
}

ECode CNotification::GetLedARGB(
    /* [out] */ Int32* argb)
{
    VALIDATE_NOT_NULL(argb);
    *argb = mLedARGB;
    return NOERROR;
}

ECode CNotification::SetLedARGB(
    /* [in] */ Int32 argb)
{
    mLedARGB = argb;
    return NOERROR;
}

ECode CNotification::GetLedOnMS(
    /* [out] */ Int32* onMS)
{
    VALIDATE_NOT_NULL(onMS);
    *onMS = mLedOnMS;
    return NOERROR;
}

ECode CNotification::SetLedOnMS(
    /* [in] */ Int32 onMS)
{
    mLedOnMS = onMS;
    return NOERROR;
}

ECode CNotification::GetLedOffMS(
    /* [out] */ Int32* offMS)
{
    VALIDATE_NOT_NULL(offMS);
    *offMS = mLedOffMS;
    return NOERROR;
}

ECode CNotification::SetLedOffMS(
    /* [in] */ Int32 offMS)
{
    mLedOffMS = offMS;
    return NOERROR;
}

ECode CNotification::GetDefaults(
    /* [out] */ Int32* defaults)
{
    VALIDATE_NOT_NULL(defaults);
    *defaults = mDefaults;
    return NOERROR;
}

ECode CNotification::SetDefaults(
    /* [in] */ Int32 defaults)
{
    mDefaults = defaults;
    return NOERROR;
}

ECode CNotification::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;
    return NOERROR;
}

ECode CNotification::SetFlags(
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return NOERROR;
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

