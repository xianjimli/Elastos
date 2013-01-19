
#include "CNotificationHandler.h"
#include "CApp.h"
#include <stdio.h>

ECode CNotificationHandler::OnReciever(
    /* [in] */ IContext* pContext,
    /* [in] */ IIntent *pIntent)
{
    if (pIntent) {
        return HandleIntent(pContext, pIntent);
    }
    return NOERROR;
}

ECode CNotificationHandler::HandleIntent(
    /* [in] */ IContext* pContext,
    /* [in] */ IIntent* pNotificationIntent)
{
    String action;
    ECode ec = pNotificationIntent->GetAction(&action);
    if (FAILED(ec)) return ec;

    String alertName("");
    String alertCookie("");
    IUri* pData;
    ec = pNotificationIntent->GetData(&pData);
    if (FAILED(ec)) return ec;
    if (pData) {
        pData->GetSchemeSpecificPart(&alertName);
        pData->GetFragment(&alertCookie);
        if (alertCookie.IsNull()) {
            alertCookie = "";
        }
    }
    pData->Release();

    printf("GeckoAppJava", "NotificationHandler.handleIntent\n"
          "- action = '%s'\n- alertName = '%s'\n- alertCookie = '%s'\n",
          action.string(), alertName.string(), alertCookie.string());

    Int32 notificationID = alertName.GetHashCode();

    printf("GeckoAppJava Handle notification ID %d\n", notificationID);

    if (CApp::sAppContext) {
        // This should call the observer, if any
        CApp::sAppContext->HandleNotification(action, alertName, alertCookie);
    } else {
        // The app is not running, just cancel this notification
        INotificationManager* pNotificationManager;
        ec = pContext->GetSystemService(String(Context_NOTIFICATION_SERVICE),
                            (IInterface**)&pNotificationManager);
        if (FAILED(ec)) return ec;
        //ec = pNotificationManager->Cancel(notificationID);
        pNotificationManager->Release();
        if (FAILED(ec)) return ec;
    }

    if (action.Equals(GeckoApp_ACTION_ALERT_CLICK)) {
        // Start or bring to front the main activity
        IIntent* pAppIntent;
        ec = CIntent::New(String(Intent_ACTION_MAIN), &pAppIntent);
        if (FAILED(ec)) return ec;

        //pAppIntent->SetClassName(pContext,
        //         String("org.mozilla.fennec_unofficial.App"));
        pAppIntent->SetFlags(Intent_FLAG_ACTIVITY_NEW_TASK);
        String args("-alert ");
        args += alertName;
        if (alertCookie.GetLength() > 0) {
            args += alertCookie + "#";
        }
        pAppIntent->PutStringExtra(String("args"), args);
        //try {
            //Log.i("GeckoAppJava", "startActivity with intent: Action='" + appIntent.getAction() + "'" +
            //      ", args='" + appIntent.getStringExtra("args") + "'" );
            ec = pContext->StartActivity(pAppIntent);
            pAppIntent->Release();
            return ec;
        //} catch (ActivityNotFoundException e) {
        //    Log.e("GeckoAppJava", "NotificationHandler Exception: ", e);
        //}
    }
    return NOERROR;
}

