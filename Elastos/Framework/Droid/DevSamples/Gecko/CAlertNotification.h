
#ifndef __CALERTNOTIFICATION_H__
#define __CALERTNOTIFICATION_H__

#include "_CAlertNotification.h"

#include <elastos.h>

using namespace Elastos;
using namespace Elastos::Core;

CarClass(CAlertNotification)
{
public:
    ~CAlertNotification();

    CARAPI IsProgressStyle(
        /* [out] */ Boolean* pIsProgressStyle);

    CARAPI Show();

    CARAPI SetCustomIcon(
        /* [in] */ IUri* aIconUri);

    CARAPI UpdateProgress(
        /* [in] */ const String& aAlertText,
        /* [in] */ Int32 aProgress,
        /* [in] */ Int32 aProgressMax);

public:
    CARAPI constructor(
        /* [in] */ IContext* pContext,
        /* [in] */ Int32 aNotificationId,
        /* [in] */ Int32 aIcon,
        /* [in] */ const String& aTitle,
        /* [in] */ const String& aText,
        /* [in] */ Int64 when);

    CARAPI GetWhen(
        /* [out] */ Int64 * pWhen);

    CARAPI SetWhen(
        /* [in] */ Int64 when);

    CARAPI GetIcon(
        /* [out] */ Int32 * pIcon);

    CARAPI SetIcon(
        /* [in] */ Int32 icon);

    CARAPI GetContentIntent(
        /* [out] */ IPendingIntent ** ppIntent);

    CARAPI SetContentIntent(
        /* [in] */ IPendingIntent * pIntent);

    CARAPI GetDeleteIntent(
        /* [out] */ IPendingIntent ** ppIntent);

    CARAPI SetDeleteIntent(
        /* [in] */ IPendingIntent * pIntent);

    CARAPI GetTickerText(
        /* [out] */ ICharSequence ** ppText);

    CARAPI SetTickerText(
        /* [in] */ ICharSequence * pText);

    CARAPI GetContentView(
        /* [out] */ IRemoteViews ** ppView);

    CARAPI SetContentView(
        /* [in] */ IRemoteViews * pView);

    CARAPI GetSound(
        /* [out] */ IUri ** ppSound);

    CARAPI SetSound(
        /* [in] */ IUri * pSound);

    CARAPI GetAudioStreamType(
        /* [out] */ Int32 * pType);

    CARAPI SetAudioStreamType(
        /* [in] */ Int32 type);

    CARAPI GetVibrate(
        /* [out, callee] */ ArrayOf<Int64> ** ppVibrate);

    CARAPI SetVibrate(
        /* [in] */ ArrayOf<Int64> * pVibrate);

    CARAPI GetLedARGB(
        /* [out] */ Int32 * pArgb);

    CARAPI SetLedARGB(
        /* [in] */ Int32 argb);

    CARAPI GetLedOnMS(
        /* [out] */ Int32 * pOnMS);

    CARAPI SetLedOnMS(
        /* [in] */ Int32 onMS);

    CARAPI GetLedOffMS(
        /* [out] */ Int32 * pOffMS);

    CARAPI SetLedOffMS(
        /* [in] */ Int32 offMS);

    CARAPI GetDefaults(
        /* [out] */ Int32 * pDefaults);

    CARAPI SetDefaults(
        /* [in] */ Int32 defaults);

    CARAPI GetFlags(
        /* [out] */ Int32 * pFlags);

    CARAPI SetFlags(
        /* [in] */ Int32 flags);

private:
    static const Double UPDATE_THRESHOLD;

    INotification* mNotification;
    IContext* mContext;
    Int32 mId;
    Int32 mIcon;
    String mTitle;
    String mText;
    Boolean mProgressStyle;
    INotificationManager* mNotificationManager;
    Double mPrevPercent;
    String mPrevAlertText;
    IUri* mIconUri;
};

#endif // __CALERTNOTIFICATION_H__

