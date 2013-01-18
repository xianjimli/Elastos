
#include "CAlertNotification.h"
#include "GeckoApp.h"
#include <elastos/Math.h>

const Double CAlertNotification::UPDATE_THRESHOLD = .01;

CAlertNotification::~CAlertNotification()
{
    if (mContext) {
        mContext->Release();
    }
    if (mNotification) {
        mNotification->Release();
    }
    if (mIconUri) {
        mIconUri->Release();
    }
    if (mNotificationManager) {
        mNotificationManager->Release();
    }
}

ECode CAlertNotification::constructor(
    /* [in] */ IContext* pContext,
    /* [in] */ Int32 aNotificationId,
    /* [in] */ Int32 aIcon,
    /* [in] */ const String& aTitle,
    /* [in] */ const String& aText,
    /* [in] */ Int64 when)
{
    /*ECode ec = CNotification::New(aIcon,
        (aText.GetLength() > 0) ? aText : aTitle, when, &mNotification);
    if (FAILED(ec)) return ec;*/

    mContext = pContext;
    mContext->AddRef();

    mIcon = aIcon;
    mTitle = aTitle;
    mText = aText;
    mProgressStyle = FALSE;
    mId = aNotificationId;

    mPrevPercent = -1;
    mPrevAlertText = "";
    mIconUri = NULL;

    return mContext->GetSystemService(
            Context_NOTIFICATION_SERVICE, (IInterface**)&mNotificationManager);
}

ECode CAlertNotification::IsProgressStyle(
    /* [out] */ Boolean* pIsProgressStyle)
{
    if (!pIsProgressStyle) return E_INVALID_ARGUMENT;
    *pIsProgressStyle = mProgressStyle;
    return NOERROR;
}

ECode CAlertNotification::Show()
{
    
    return E_NOT_IMPLEMENTED;
    //return mNotificationManager->Notify(mId, this);
}

ECode CAlertNotification::SetCustomIcon(
    /* [in] */ IUri* aIconUri)
{
    mIconUri = aIconUri;
    // Custom view
    Int32 layout = 0x7f030001;//R.layout.notification_icon_text;
    String packageName;
    GeckoApp::mAppContext->GetPackageName(&packageName);
    IRemoteViews* pView;
    ECode ec;
    //ec = CRemoteViews::New(packageName, layout, &pView);
    //if (FAILED(ec)) return ec;

    //try {
    //IURL* pUrl;
    //ec = CURL::New(aIconUri.ToString(), &pUrl);
    //if (FAILED(ec)) goto exit;
    IBitmap* pBm;
    //ec = BitmapFactory::DecodeStream(url->OpenStream(), &pBm);
    //pUrl->Release();
    //if (FAILED(ec)) goto exit;

    //ec = pView->SetImageViewBitmap(0x7f070004, pBm);//R.id.notificationImage
    //pBm->Release();
    //if (FAILED(ec)) goto exit;
    //ec = pView->SetTextViewText(0x7f070005, mTitle); //R.id.notificationTitle
    //if (FAILED(ec)) goto exit;

    if (mText.GetLength() > 0) {
        //ec = pView->SetTextViewText(0x7f070006, mText);//R.id.notificationText
        //if (FAILED(ec)) goto exit;
    }
    ec = SetContentView(pView);
    if (FAILED(ec)) goto exit;

    //ec = mNotificationManager->Notify(mId, this);
    //} catch(Exception ex) {
    //    Log.e("GeckoAlert", "failed to create bitmap", ex);
    //}
exit:
    pView->Release();
    return ec;
}

ECode CAlertNotification::UpdateProgress(
    /* [in] */ const String& aAlertText,
    /* [in] */ Int32 aProgress,
    /* [in] */ Int32 aProgressMax)
{
    ECode ec;
    if (!mProgressStyle) {
        // Custom view
        Int32 layout = aAlertText.GetLength() > 0 ?
                0x7f030003 : 0x7f030002; //R.layout.notification_progress_text : R.layout.notification_progress;

        String packageName;
        GeckoApp::mAppContext->GetPackageName(&packageName);
        IRemoteViews* pView;
        //ec = CRemoteViews::New(packageName, layout, &pView);
        //if (FAILED(ec)) return ec;

        //pView->SetImageViewResource(0x7f070004, mIcon); //R.id.notificationImage
        //pView->SetTextViewText(0x7f070005, mTitle);//R.id.notificationTitle

        SetContentView(pView);
        pView->Release();
        
        Int32 flags;
        mNotification->GetFlags(&flags);
        flags |= Notification_FLAG_ONGOING_EVENT;
        mNotification->SetFlags(flags);

        mProgressStyle = TRUE;
    }

    String text;
    Double percent = 0;
    if (aProgressMax > 0) {
        percent = ((Double)aProgress / (Double)aProgressMax);
    }

    if (aAlertText.GetLength() > 0) {
        text = aAlertText;
    }
    else {
        /*INumberFormat* pFormat;
        ec = NumberFormat::GetPercentInstance(&pFormat);
        if (FAILED(ec)) return ec;
        ec = pFormat->Format(percent, &text);
        pFormat->Release();
        if (FAILED(ec)) return ec;*/
    }

    if (mPrevAlertText.Equals(text)
        && Math::Abs(mPrevPercent - percent) < UPDATE_THRESHOLD) {
        return NOERROR;
    }

    IRemoteViews* pIRView;
    ec = GetContentView(&pIRView);
    if (FAILED(ec)) return ec;
    /*ec = pIRView->SetTextViewText(0x7f070006, text);//R.id.notificationText
    if (FAILED(ec)) {
        pIRView->Release();
        return ec;
    }
    ec = pIRView->SetProgressBar(0x7f070007, //R.id.notificationProgressbar,
                     (Int32)aProgressMax, (Int32)aProgress, FALSE);
    pIRView->Release();
    if (FAILED(ec)) return ec;

    // Update the notification
    ec = mNotificationManager->Notify(mId, this);
    if (FAILED(ec)) return ec;*/

    mPrevPercent = percent;
    mPrevAlertText = text;
    return NOERROR;
}

ECode CAlertNotification::GetWhen(
    /* [out] */ Int64 * pWhen)
{
    return mNotification->GetWhen(pWhen);
}

ECode CAlertNotification::SetWhen(
    /* [in] */ Int64 when)
{
    return mNotification->SetWhen(when);
}

ECode CAlertNotification::GetIcon(
    /* [out] */ Int32 * pIcon)
{
    return mNotification->GetIcon(pIcon);
}

ECode CAlertNotification::SetIcon(
    /* [in] */ Int32 icon)
{
    return mNotification->SetIcon(icon);
}

ECode CAlertNotification::GetContentIntent(
    /* [out] */ IPendingIntent ** ppIntent)
{
    return mNotification->GetContentIntent(ppIntent);
}

ECode CAlertNotification::SetContentIntent(
    /* [in] */ IPendingIntent * pIntent)
{
    return mNotification->SetContentIntent(pIntent);
}

ECode CAlertNotification::GetDeleteIntent(
    /* [out] */ IPendingIntent ** ppIntent)
{
    return mNotification->GetDeleteIntent(ppIntent);
}

ECode CAlertNotification::SetDeleteIntent(
    /* [in] */ IPendingIntent * pIntent)
{
    return mNotification->SetDeleteIntent(pIntent);
}

ECode CAlertNotification::GetTickerText(
    /* [out] */ ICharSequence ** ppText)
{
    return mNotification->GetTickerText(ppText);
}

ECode CAlertNotification::SetTickerText(
    /* [in] */ ICharSequence * pText)
{
    return mNotification->SetTickerText(pText);
}

ECode CAlertNotification::GetContentView(
    /* [out] */ IRemoteViews ** ppView)
{
    return mNotification->GetContentView(ppView);
}

ECode CAlertNotification::SetContentView(
    /* [in] */ IRemoteViews * pView)
{
    return mNotification->SetContentView(pView);
}

ECode CAlertNotification::GetSound(
    /* [out] */ IUri ** ppSound)
{
    return mNotification->GetSound(ppSound);
}

ECode CAlertNotification::SetSound(
    /* [in] */ IUri * pSound)
{
    return mNotification->SetSound(pSound);
}

ECode CAlertNotification::GetAudioStreamType(
    /* [out] */ Int32 * pType)
{
    return mNotification->GetAudioStreamType(pType);
}

ECode CAlertNotification::SetAudioStreamType(
    /* [in] */ Int32 type)
{
    return mNotification->SetAudioStreamType(type);
}

ECode CAlertNotification::GetVibrate(
    /* [out, callee] */ ArrayOf<Int64> ** ppVibrate)
{
    return mNotification->GetVibrate(ppVibrate);
}

ECode CAlertNotification::SetVibrate(
    /* [in] */ ArrayOf<Int64> * pVibrate)
{
    return mNotification->SetVibrate(pVibrate);
}

ECode CAlertNotification::GetLedARGB(
    /* [out] */ Int32 * pArgb)
{
    return mNotification->GetLedARGB(pArgb);
}

ECode CAlertNotification::SetLedARGB(
    /* [in] */ Int32 argb)
{
    return mNotification->SetLedARGB(argb);
}

ECode CAlertNotification::GetLedOnMS(
    /* [out] */ Int32 * pOnMS)
{
    return mNotification->GetLedOnMS(pOnMS);
}

ECode CAlertNotification::SetLedOnMS(
    /* [in] */ Int32 onMS)
{
    return mNotification->SetLedOnMS(onMS);
}

ECode CAlertNotification::GetLedOffMS(
    /* [out] */ Int32 * pOffMS)
{
    return mNotification->GetLedOffMS(pOffMS);
}

ECode CAlertNotification::SetLedOffMS(
    /* [in] */ Int32 offMS)
{
    return mNotification->SetLedOffMS(offMS);
}

ECode CAlertNotification::GetDefaults(
    /* [out] */ Int32 * pDefaults)
{
    return mNotification->GetDefaults(pDefaults);
}

ECode CAlertNotification::SetDefaults(
    /* [in] */ Int32 defaults)
{
    return mNotification->SetDefaults(defaults);
}

ECode CAlertNotification::GetFlags(
    /* [out] */ Int32 * pFlags)
{
    return mNotification->GetFlags(pFlags);
}

ECode CAlertNotification::SetFlags(
    /* [in] */ Int32 flags)
{
    return mNotification->SetFlags(flags);
}

