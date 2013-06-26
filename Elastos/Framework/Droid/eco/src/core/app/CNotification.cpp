
#include "ext/frameworkdef.h"
#include "app/CNotification.h"
#include "widget/CRemoteViews.h"
#include <elastos/System.h>

using namespace Elastos::Core;

CNotification::CNotification()
    : mWhen(0)
    , mIcon(0)
    , mNumber(0)
    , mIconLevel(0)
    , mAudioStreamType(Notification_STREAM_DEFAULT)
    , mLedARGB(0)
    , mLedOnMS(0)
    , mLedOffMS(0)
    , mDefaults(0)
    , mFlags(0)
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

ECode CNotification::SetLatestEventInfo(
    /* [in] */ IContext* context,
    /* [in] */ ICharSequence* contentTitle,
    /* [in] */ ICharSequence* contentText,
    /* [in] */ IPendingIntent* contentIntent)
{
    AutoPtr<IRemoteViews> contentView;
    String capsuleName;

    FAIL_RETURN(context->GetCapsuleName(&capsuleName));
    FAIL_RETURN(CRemoteViews::New(
        capsuleName, 0x01090061, (IRemoteViews**)&contentView)); // com.android.internal.R.layout.status_bar_latest_event_content

    if (mIcon != 0) {
        contentView->SetImageViewResource(0x01020006, mIcon); // com.android.internal.R.id.icon
    }
    if (contentTitle != NULL) {
        contentView->SetTextViewText(0x01020016, contentTitle); // com.android.internal.R.id.title
    }
    if (contentText != NULL) {
        contentView->SetTextViewText(0x01020040, contentText); // com.android.internal.R.id.text
    }
    if (mWhen != 0) {
        contentView->SetInt64(0x0102005b, String("setTime"), mWhen); // com.android.internal.R.id.time
    }

    mContentView = contentView;
    mContentIntent = contentIntent;

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

ECode CNotification::constructor()
{
    mWhen = System::GetCurrentTimeMillis();

    return NOERROR;
}

ECode CNotification::constructor(
    /* [in] */ Int32 icon,
    /* [in] */ ICharSequence* tickerText,
    /* [in] */ Int64 when)
{
    mIcon = icon;
    mTickerText = tickerText;
    mWhen = when;

    return NOERROR;
}

ECode CNotification::constructor(
    /* [in] */ IParcel* parcel)
{
    Int32 version;
    FAIL_RETURN(parcel->ReadInt32(&version));

    parcel->ReadInt64(&mWhen);
    parcel->ReadInt32(&mIcon);
    parcel->ReadInt32(&mNumber);

    Int32 temp;
    FAIL_RETURN(parcel->ReadInt32(&temp));
    if (temp != 0) {
        // TODO: ALEX need IParcel ReadStrongBinder
        /*
        AutoPtr<IBinder> target;
        FAIL_RETURN(parcel->ReadStrongBinder((IBinder**)&target));
        FAIL_RETURN(CPendingIntent::New(
            target, (IPendingIntent**)&mContentIntent));
        FAIL_RETURN(mContentIntent->ReadFromParcel(parcel));
        */
        return E_NOT_IMPLEMENTED;
    }

    FAIL_RETURN(parcel->ReadInt32(&temp));
    if (temp != 0) {
        // TODO: ALEX need IParcel ReadStrongBinder
        /*
        AutoPtr<IBinder> target;
        FAIL_RETURN(parcel->ReadStrongBinder((IBinder**)&target));
        FAIL_RETURN(CPendingIntent::New(
            target, (IPendingIntent**)&mDeleteIntent));
        FAIL_RETURN(mDeleteIntent->ReadFromParcel(parcel));
        */
        return E_NOT_IMPLEMENTED;
    }

    FAIL_RETURN(parcel->ReadInt32(&temp));
    if (temp != 0) {
        // TODO: ALEX need TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel
        // tickerText = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(parcel);
        return E_NOT_IMPLEMENTED;
    }

    FAIL_RETURN(parcel->ReadInt32(&temp));
    if (temp != 0) {
        // TODO: ALEX need RemoteViews
        // contentView = RemoteViews.CREATOR.createFromParcel(parcel);
        return E_NOT_IMPLEMENTED;
    }

    FAIL_RETURN(parcel->ReadInt32(&mDefaults));
    FAIL_RETURN(parcel->ReadInt32(&mFlags));

    FAIL_RETURN(parcel->ReadInt32(&temp));
    if (temp != 0) {
        // TODO: ALEX need Uri.CREATOR.createFromParcel
        // sound = Uri.CREATOR.createFromParcel(parcel);
        return E_NOT_IMPLEMENTED;
    }

    FAIL_RETURN(parcel->ReadInt32(&mAudioStreamType));
    // TODO: ALEX need IParcel::createLongArray
    // vibrate = parcel.createLongArray();
    return E_NOT_IMPLEMENTED;
    FAIL_RETURN(parcel->ReadInt32(&mLedARGB));
    FAIL_RETURN(parcel->ReadInt32(&mLedOnMS));
    FAIL_RETURN(parcel->ReadInt32(&mLedOffMS));
    FAIL_RETURN(parcel->ReadInt32(&mIconLevel));

    FAIL_RETURN(parcel->ReadInt32(&temp));
    if (temp != 0) {
        // TODO: ALEX need IParcel ReadStrongBinder
        /*
        AutoPtr<IBinder> target;
        FAIL_RETURN(parcel->ReadStrongBinder((IBinder**)&target));
        FAIL_RETURN(CPendingIntent::New(
            target, (IPendingIntent**)&mFullScreenIntent));
        FAIL_RETURN(mFullScreenIntent->ReadFromParcel(parcel));
        */
        return E_NOT_IMPLEMENTED;
    }

    return NOERROR;
}