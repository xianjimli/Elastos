
#include "server/CServiceRecord.h"
#include "os/SystemClock.h"

const Int32 CServiceRecord::MAX_DELIVERY_COUNT;
const Int32 CServiceRecord::MAX_DONE_EXECUTING_COUNT;

CServiceRecord::StartItem::StartItem(
    /* [in] */ CServiceRecord* sr,
    /* [in] */ Int32 id,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 targetPermissionUid) :
    mSr(sr),
    mId(id),
    mIntent(intent),
    mTargetPermissionUid(targetPermissionUid)
{}

UriPermissionOwner*
CServiceRecord::StartItem::GetUriPermissionsLocked()
{
//    if (uriPermissions == null) {
//        uriPermissions = new UriPermissionOwner(sr.ams, this);
//    }
//    return uriPermissions;
    return NULL;
}

void CServiceRecord::StartItem::RemoveUriPermissionsLocked()
{
//    if (uriPermissions != null) {
//        uriPermissions.removeUriPermissionsLocked();
//        uriPermissions = null;
//    }
}

CServiceRecord::CServiceRecord()
{
}

CServiceRecord::~CServiceRecord()
{
}

void CServiceRecord::Init(
    /* [in] */ /* [in] */ CActivityManagerService* ams,
    /* [in] */ BatteryStatsImpl::Uid::Cap::Serv* servStats,
    /* [in] */ IComponentName* name,
    /* [in] */ IIntentFilterComparison* intent,
    /* [in] */ IServiceInfo* sInfo,
    /* [in] */ IRunnable* restarter)
{
    mAms = ams;
    mStats = servStats;
    mName = name;
    name->FlattenToShortString(&mShortName);
    mIntent = intent;
    mServiceInfo = sInfo;
    sInfo->GetApplicationInfo((IApplicationInfo**)&mAppInfo);
    mAppInfo->GetCapsuleName(&mCapsuleName);
    sInfo->GetProcessName(&mProcessName);
    sInfo->GetPermission(&mPermission);
    mAppInfo->GetSourceDir(&mBaseDir);
    mAppInfo->GetPublicSourceDir(&mResDir);
    mAppInfo->GetDataDir(&mDataDir);
    sInfo->IsExported(&mExported);
    mRestarter = restarter;
    mCreateTime = SystemClock::GetElapsedRealtime();
    mLastActivity = SystemClock::GetUptimeMillis();
}

AppBindRecord* CServiceRecord::RetrieveAppBindingLocked(
    /* [in] */ IIntent* intent,
    /* [in] */ ProcessRecord* app)
{
    AutoPtr<IIntentFilterComparison> filter;
    CIntentFilterComparison::New(intent, (IIntentFilterComparison**)&filter);
    IntentBindRecord* i = mBindings[filter];
    if (i == NULL) {
        i = new IntentBindRecord(this, filter);
        mBindings[filter] = i;
    }
    AppBindRecord* a = i->mApps[app];
    if (a != NULL) {
        return a;
    }
    a = new AppBindRecord(this, i, app);
    i->mApps[app] = a;
    return a;
}

void CServiceRecord::ResetRestartCounter()
{
    mRestartCount = 0;
    mRestartDelay = 0;
    mRestartTime = 0;
}

CServiceRecord::StartItem* CServiceRecord::FindDeliveredStart(
    /* [in] */ Int32 id,
    /* [in] */ Boolean remove)
{
    List<StartItem*>::Iterator it = mDeliveredStarts.Begin();
    for (; it != mDeliveredStarts.End(); ++it) {
        StartItem* si = *it;
        if (si->mId == id) {
            if (remove) mDeliveredStarts.Erase(it);
            return si;
        }
    }

    return NULL;
}

void CServiceRecord::PostNotification()
{
//final int appUid = appInfo.uid;
//        final int appPid = app.pid;
//        if (foregroundId != 0 && foregroundNoti != null) {
//            // Do asynchronous communication with notification manager to
//            // avoid deadlocks.
//            final String localPackageName = packageName;
//            final int localForegroundId = foregroundId;
//            final Notification localForegroundNoti = foregroundNoti;
//            ams.mHandler.post(new Runnable() {
//                public void run() {
//                    NotificationManagerService nm =
//                            (NotificationManagerService) NotificationManager.getService();
//                    if (nm == null) {
//                        return;
//                    }
//                    try {
//                        int[] outId = new int[1];
//                        nm.enqueueNotificationInternal(localPackageName, appUid, appPid,
//                                null, localForegroundId, localForegroundNoti, outId);
//                    } catch (RuntimeException e) {
//                        Slog.w(ActivityManagerService.TAG,
//                                "Error showing notification for service", e);
//                        // If it gave us a garbage notification, it doesn't
//                        // get to be foreground.
//                        ams.setServiceForeground(name, ServiceRecord.this,
//                                0, null, true);
//                        ams.crashApplication(appUid, appPid, localPackageName,
//                                "Bad notification for startForeground: " + e);
//                    }
//                }
//            });
//        }
}

void CServiceRecord::CancelNotification()
{
//    if (foregroundId != 0) {
//        // Do asynchronous communication with notification manager to
//        // avoid deadlocks.
//        final String localPackageName = packageName;
//        final int localForegroundId = foregroundId;
//        ams.mHandler.post(new Runnable() {
//            public void run() {
//                INotificationManager inm = NotificationManager.getService();
//                if (inm == null) {
//                    return;
//                }
//                try {
//                    inm.cancelNotification(localPackageName, localForegroundId);
//                } catch (RuntimeException e) {
//                    Slog.w(ActivityManagerService.TAG,
//                            "Error canceling notification for service", e);
//                } catch (RemoteException e) {
//                }
//            }
//        });
//    }
}

void CServiceRecord::ClearDeliveredStartsLocked()
{
//    for (int i=deliveredStarts.size()-1; i>=0; i--) {
//        deliveredStarts.get(i).removeUriPermissionsLocked();
//    }
//    deliveredStarts.clear();
}

ECode CServiceRecord::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}
