
#ifndef __CNOTIFICATION_H__
#define __CNOTIFICATION_H__

#include "_CNotification.h"
#include <elastos/AutoPtr.h>
#include <elastos/AutoFree.h>

CarClass(CNotification)
{
public:
    CNotification();

    CARAPI GetWhen(
        /* [out] */ Int64* when);

    CARAPI SetWhen(
        /* [in] */ Int64 when);

    CARAPI GetIcon(
        /* [out] */ Int32* icon);

    CARAPI SetIcon(
        /* [in] */ Int32 icon);

    CARAPI GetContentIntent(
        /* [out] */ IPendingIntent** intent);

    CARAPI SetContentIntent(
        /* [in] */ IPendingIntent* intent);

    CARAPI GetDeleteIntent(
        /* [out] */ IPendingIntent** intent);

    CARAPI SetDeleteIntent(
        /* [in] */ IPendingIntent* intent);

    CARAPI GetTickerText(
        /* [out] */ ICharSequence** text);

    CARAPI SetTickerText(
        /* [in] */ ICharSequence* text);

    CARAPI GetContentView(
        /* [out] */ IRemoteViews** view);

    CARAPI SetContentView(
        /* [in] */ IRemoteViews* view);

    CARAPI GetSound(
        /* [out] */ IUri** sound);

    CARAPI SetSound(
        /* [in] */ IUri* sound);

    CARAPI GetAudioStreamType(
        /* [out] */ Int32* type);

    CARAPI SetAudioStreamType(
        /* [in] */ Int32 type);

    CARAPI GetVibrate(
        /* [out, callee] */ ArrayOf<Int64>** vibrate);

    CARAPI SetVibrate(
        /* [in] */ ArrayOf<Int64>* vibrate);

    CARAPI GetLedARGB(
        /* [out] */ Int32* argb);

    CARAPI SetLedARGB(
        /* [in] */ Int32 argb);

    CARAPI GetLedOnMS(
        /* [out] */ Int32* onMS);

    CARAPI SetLedOnMS(
        /* [in] */ Int32 onMS);

    CARAPI GetLedOffMS(
        /* [out] */ Int32* offMS);

    CARAPI SetLedOffMS(
        /* [in] */ Int32 offMS);

    CARAPI GetDefaults(
        /* [out] */ Int32* defaults);

    CARAPI SetDefaults(
        /* [in] */ Int32 defaults);

    CARAPI GetFlags(
        /* [out] */ Int32* flags);

    CARAPI SetFlags(
        /* [in] */ Int32 flags);

    CARAPI SetLatestEventInfo(
        /* [in] */ IContext* context,
        /* [in] */ ICharSequence* contentTitle,
        /* [in] */ ICharSequence* contentText,
        /* [in] */ IPendingIntent* contentIntent);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 icon,
        /* [in] */ ICharSequence* tickerText,
        /* [in] */ Int64 when);

    CARAPI constructor(
        /* [in] */ IParcel* parcel);

public:
    /**
     * The timestamp for the notification.  The icons and expanded views
     * are sorted by this key.
     */
    Int64 mWhen;

    /**
     * The resource id of a drawable to use as the icon in the status bar.
     */
    Int32 mIcon;

    /**
     * The number of events that this notification represents.  For example, in a new mail
     * notification, this could be the number of unread messages.  This number is superimposed over
     * the icon in the status bar.  If the number is 0 or negative, it is not shown in the status
     * bar.
     */
    Int32 mNumber;

    /**
     * The intent to execute when the expanded status entry is clicked.  If
     * this is an activity, it must include the
     * {@link android.content.Intent#FLAG_ACTIVITY_NEW_TASK} flag, which requires
     * that you take care of task management as described in the <em>Activities and Tasks</em>
     * section of the <a href="{@docRoot}guide/topics/fundamentals.html#acttask">Application
     * Fundamentals</a> document.
     */
    AutoPtr<IPendingIntent> mContentIntent;

    /**
     * The intent to execute when the status entry is deleted by the user
     * with the "Clear All Notifications" button. This probably shouldn't
     * be launching an activity since several of those will be sent at the
     * same time.
     */
    AutoPtr<IPendingIntent> mDeleteIntent;

    /**
     * Text to scroll across the screen when this item is added to
     * the status bar.
     */
    AutoPtr<ICharSequence> mTickerText;

    /**
     * The view that will represent this notification in the expanded status bar.
     */
    AutoPtr<IRemoteViews> mContentView;

    /**
     * If the icon in the status bar is to have more than one level, you can set this.  Otherwise,
     * leave it at its default value of 0.
     *
     * @see android.widget.ImageView#setImageLevel
     * @see android.graphics.drawable#setLevel
     */
    Int32 mIconLevel;

    /**
     * The sound to play.
     *
     * <p>
     * To play the default notification sound, see {@link #defaults}.
     * </p>
     */
    AutoPtr<IUri> mSound;

    /**
     * The audio stream type to use when playing the sound.
     * Should be one of the STREAM_ constants from
     * {@link android.media.AudioManager}.
     */
    Int32 mAudioStreamType;

    /**
     * The pattern with which to vibrate.
     *
     * <p>
     * To vibrate the default pattern, see {@link #defaults}.
     * </p>
     *
     * @see android.os.Vibrator#vibrate(long[],int)
     */
    AutoFree< ArrayOf<Int64> > mVibrate;

     /**
     * The color of the led.  The hardware will do its best approximation.
     *
     * @see #FLAG_SHOW_LIGHTS
     * @see #flags
     */
    Int32 mLedARGB;

    /**
     * The number of milliseconds for the LED to be on while it's flashing.
     * The hardware will do its best approximation.
     *
     * @see #FLAG_SHOW_LIGHTS
     * @see #flags
     */
    Int32 mLedOnMS;

    /**
     * The number of milliseconds for the LED to be off while it's flashing.
     * The hardware will do its best approximation.
     *
     * @see #FLAG_SHOW_LIGHTS
     * @see #flags
     */
    Int32 mLedOffMS;

    /**
     * Specifies which values should be taken from the defaults.
     * <p>
     * To set, OR the desired from {@link #DEFAULT_SOUND},
     * {@link #DEFAULT_VIBRATE}, {@link #DEFAULT_LIGHTS}. For all default
     * values, use {@link #DEFAULT_ALL}.
     * </p>
     */
    Int32 mDefaults;

    Int32 mFlags;
};

#endif //__CNOTIFICATION_H__
