
#include "app/NotificationManager.h"
#include "os/ServiceManager.h"

const CString NotificationManager::TAG = "NotificationManager";
const Boolean NotificationManager::DEBUG;
const Boolean NotificationManager::localLOGV;

AutoPtr<INotificationManager> NotificationManager::sService;

/** @hide */
AutoPtr<INotificationManager> NotificationManager::GetService()
{
    if (sService != NULL) {
        return sService;
    }

    sService = INotificationManager::Probe(
            ServiceManager::GetService(String("notification")).Get());
    return sService;
}

ECode NotificationManager::Notify(
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification)
{
    return NotifyEx(String(NULL), id, notification);
}

ECode NotificationManager::NotifyEx(
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification)
{
    ArrayOf_<Int32, 1> idOut;
    INotificationManager* service = GetService();
    String cap;
    mContext->GetCapsuleName(&cap);
//    if (localLOGV) Log.v(TAG, pkg + ": notify(" + id + ", " + notification + ")");
    FAIL_RETURN(service->EnqueueNotificationWithTag(
            cap, tag, id, notification, &idOut));
    if (id != idOut[0]) {
//        Log.w(TAG, "notify: id corrupted: sent " + id + ", got back " + idOut[0]);
    }
    return NOERROR;
}

ECode NotificationManager::Cancel(
    /* [in] */ Int32 id)
{
    return CancelEx(String(NULL), id);
}

ECode NotificationManager::CancelEx(
    /* [in] */ const String& tag,
    /* [in] */ Int32 id)
{
    INotificationManager* service = GetService();
    String cap;
    mContext->GetCapsuleName(&cap);
//    if (localLOGV) Log.v(TAG, pkg + ": cancel(" + id + ")");
    return service->CancelNotificationWithTag(cap, tag, id);
}

ECode NotificationManager::CancelAll()
{
    INotificationManager* service = GetService();
    String cap;
    mContext->GetCapsuleName(&cap);
//    if (localLOGV) Log.v(TAG, pkg + ": cancelAll()");
    return service->CancelAllNotifications(cap);
}
