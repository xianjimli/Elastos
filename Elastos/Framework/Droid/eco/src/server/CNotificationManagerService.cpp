
#include "server/CNotificationManagerService.h"
#include "server/CLightsService.h"
#include "os/CVibrator.h"
#include "app/ActivityManagerNative.h"
#include "app/StatusBarManager.h"
#include "content/CIntentFilter.h"
#include "ext/frameworkerr.h"
#include "utils/log.h"
#include <StringBuffer.h>

CString CNotificationManagerService::TAG = "CNotificationService";
const Boolean CNotificationManagerService::DBG;
const Int32 CNotificationManagerService::MAX_PACKAGE_NOTIFICATIONS;
const Int32 CNotificationManagerService::MESSAGE_TIMEOUT;
const Int32 CNotificationManagerService::LONG_DELAY; // 3.5 seconds
const Int32 CNotificationManagerService::SHORT_DELAY; // 2 seconds
const Int64 CNotificationManagerService::DEFAULT_VIBRATE_PATTERN[] = {0, 250, 250, 250};
const Int32 CNotificationManagerService::DEFAULT_STREAM_TYPE;
const Int32 CNotificationManagerService::BATTERY_LOW_ARGB; // Charging Low - red solid on
const Int32 CNotificationManagerService::BATTERY_MEDIUM_ARGB;    // Charging - orange solid on
const Int32 CNotificationManagerService::BATTERY_FULL_ARGB; // Charging Full - green solid on
const Int32 CNotificationManagerService::BATTERY_BLINK_ON;
const Int32 CNotificationManagerService::BATTERY_BLINK_OFF;

CNotificationManagerService::NotificationRecord::NotificationRecord(
    /* [in] */ String pkg,
    /* [in] */ String tag,
    /* [in] */ Int32 id,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 initialPid,
    /* [in] */ INotification* notification)
    : mPkg(pkg),
    mTag(tag),
    mId(id),
    mUid(uid),
    mInitialPid(initialPid),
    mNotification(notification)
{
}

CNotificationManagerService::ToastRecord::ToastRecord(
    /* [in] */ Int32 pid,
    /* [in] */ String pkg,
    /* [in] */ ITransientNotification* callback,
    /* [in] */ Int32 duration)
    : mPid(pid),
    mPkg(pkg),
    mCallback(callback),
    mDuration(duration)
{
}

void CNotificationManagerService::ToastRecord::Update(
    /* [in] */ Int32 duration)
{
    mDuration = duration;
}

IInterface* CNotificationManagerService::WorkerHandler::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IHandler) {
        return (IHandler*)this;
    }

    return NULL;
}

UInt32 CNotificationManagerService::WorkerHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CNotificationManagerService::WorkerHandler::Release()
{
    return ElRefBase::Release();
}

ECode CNotificationManagerService::WorkerHandler::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return NOERROR;
}

CNotificationManagerService::MyNotificationCallbacks::MyNotificationCallbacks(
    /* [in] */ CNotificationManagerService* nms)
    : mNMService(nms)
{
}

void CNotificationManagerService::MyNotificationCallbacks::OnSetDisabled(
    /* [in] */ Int32 status)
{
    Mutex::Autolock lock(mNMService->mNotificationListLock);

    mNMService->mDisabledNotifications = status;
    if ((mNMService->mDisabledNotifications & StatusBarManager::DISABLE_NOTIFICATION_ALERTS) != 0) {
        // cancel whatever's going on
//        Int32 identity = Binder.clearCallingIdentity();
//        try {
//        mNMService->mSound->Stop();
//        }
//        finally {
//        Binder.restoreCallingIdentity(identity);
//        }

//        identity = Binder.clearCallingIdentity();
//        try {
        mNMService->mVibrator->Cancel();
//        }
//        finally {
//        Binder.restoreCallingIdentity(identity);
//        }
    }
}

void CNotificationManagerService::MyNotificationCallbacks::OnClearAll()
{
    mNMService->CancelAll();
}

void CNotificationManagerService::MyNotificationCallbacks::OnNotificationClick(
    /* [in] */ String pkg,
    /* [in] */ String tag,
    /* [in] */ Int32 id)
{
    mNMService->CancelNotification(pkg, tag, id, Notification_FLAG_AUTO_CANCEL,
            Notification_FLAG_FOREGROUND_SERVICE);
}

void CNotificationManagerService::MyNotificationCallbacks::OnPanelRevealed()
{
    Mutex::Autolock lock(mNMService->mNotificationListLock);
    // sound
    mNMService->mSoundNotification = NULL;
//    long identity = Binder.clearCallingIdentity();
//    try {
//    mSound->Stop();
//    }
//    finally {
//    Binder.restoreCallingIdentity(identity);
//    }

    // vibrate
    mNMService->mVibrateNotification = NULL;
//    identity = Binder.clearCallingIdentity();
//    try {
    mNMService->mVibrator->Cancel();
//    }
//    finally {
//    Binder.restoreCallingIdentity(identity);
//    }

    // light
    mNMService->mLights.Clear();
    mNMService->mLedNotification = NULL;
    mNMService->UpdateLightsLocked();
}

void CNotificationManagerService::MyNotificationCallbacks::OnNotificationError(
    /* [in] */ String pkg,
    /* [in] */ String tag,
    /* [in] */ Int32 id,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 initialPid,
    /* [in] */ String message)
{
//    Slog.d(TAG, "onNotification error pkg=" + pkg + " tag=" + tag + " id=" + id
//            + "; will crashApplication(uid=" + uid + ", pid=" + initialPid + ")");
    mNMService->CancelNotification(pkg, tag, id, 0, 0);
//    Int64 ident = Binder.clearCallingIdentity();
//    try {
    AutoPtr<IActivityManager> activityMgr;
    ActivityManagerNative::GetDefault((IActivityManager**)&activityMgr);

    if (activityMgr) {
        activityMgr->CrashApplication(uid, initialPid, pkg,
                    (StringBuffer("Bad notification posted from package ") + pkg
                    + ": " + message));
    }
//    } catch (RemoteException e) {
//    }
//    Binder.restoreCallingIdentity(ident);
}

//void CNotificationManagerService::MyIntentReceiver::OnReceive(
//    /* [in] */ IContext* context,
//    /* [in] */ IIntent* intent)
//{
//    String action;
//    intent->GetAction(&action);
//
//    Boolean queryRestart = FALSE;
//
//    if (action == Intent_ACTION_BATTERY_CHANGED) {
////        Boolean batteryCharging = (intent->GetIntExtra("plugged", 0) != 0);
////        Int32 level = intent.getIntExtra("level", -1);
////        Boolean batteryLow = (level >= 0 && level <= Power.LOW_BATTERY_THRESHOLD);
////        Int32 status = intent.getIntExtra("status", BatteryManager.BATTERY_STATUS_UNKNOWN);
////        Boolean batteryFull = (status == BatteryManager.BATTERY_STATUS_FULL || level >= 90);
//
////        if (batteryCharging != mBatteryCharging ||
////                batteryLow != mBatteryLow ||
////                batteryFull != mBatteryFull) {
////            mBatteryCharging = batteryCharging;
////            mBatteryLow = batteryLow;
////            mBatteryFull = batteryFull;
////            UpdateLights();
////        }
//    }
////    else if (action == UsbManager_ACTION_USB_STATE)) {
////        AutoPtr<IBundle> extras;
////        intent->GetExtras((IBundle**)&extras);
////        Boolean usbConnected;
////        extras->GetBoolean(UsbManager_USB_CONNECTED, &usbConnected);
////        String usbFunction;
////        extras->GetString(UsbManager_USB_FUNCTION_ADB, &usbFunction);
////        Boolean adbEnabled = UsbManager_USB_FUNCTION_ENABLED == usbFunction;
////        UpdateAdbNotification(usbConnected && adbEnabled);
////    }
//    else if (action == Intent_ACTION_PACKAGE_REMOVED
//            || action == Intent_ACTION_PACKAGE_RESTARTED
//            || (queryRestart = action == Intent_ACTION_QUERY_PACKAGE_RESTART)
//            || action = Intent_ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE) {
//        ArrayOf<String> pkgList;
//        if (action == Intent_ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE)) {
//            intent->GetStringArrayExtra(Intent_EXTRA_CHANGED_PACKAGE_LIST, &pkgList);
//        }
//        else if (queryRestart) {
//            intent->GetStringArrayExtra(Intent_EXTRA_PACKAGES, &pkgList);
//        }
//        else {
//            AutoPtr<IUri> uri;
//            intent->GetData((IUri**)&uri);
//            if (uri == NULL) {
//                return;
//            }
//            String pkgName;
//            uri->GetSchemeSpecificPart(&pkgName);
//            if (pkgName.IsNull()) {
//                return;
//            }
//            Int32 len = pkgName.GetLength();
//            pkgList(pkgName, len);
//        }
//
//        Int32 len = pkgList.GetLength();
//        if (pkgList.IsNull() && len > 0)) {
////            for (String pkgName : pkgList) {
////                CancelAllNotificationsInt(pkgName, 0, 0, !queryRestart);
////            }
//        }
//    }
//    else if (action == Intent_ACTION_SCREEN_ON)) {
//        mScreenOn = TRUE;
//        UpdateNotificationPulse();
//    }
//    else if (action == Intent_ACTION_SCREEN_OFF) {
//        mScreenOn = FALSE;
//        UpdateNotificationPulse();
//    }
////    else if (action == TelephonyManager_ACTION_PHONE_STATE_CHANGED)) {
////        String telMgrExtraState;
////        intent->GetStringExtra(TelephonyManager_EXTRA_STATE, &telMgrExtraState);
////        mInCall = telMgrExtraState == TelephonyManager_EXTRA_STATE_OFFHOOK;
////        UpdateNotificationPulse();
////    }
//}

CNotificationManagerService::CNotificationManagerService() :
    mHandler(NULL),
    mDefaultNotificationColor(0),
    mDefaultNotificationLedOn(0),
    mDefaultNotificationLedOff(0),
    mSoundNotification(NULL),
    mSystemReady(FALSE),
    mDisabledNotifications(0),
    mVibrateNotification(NULL),
    mScreenOn(TRUE),
    mInCall(FALSE),
    mNotificationPulseEnabled(FALSE),
    mPendingPulseNotification(FALSE),
    mAdbNotificationShown(FALSE),
    mBatteryCharging(FALSE),
    mBatteryLow(FALSE),
    mBatteryFull(FALSE),
    mLedNotification(NULL)
{
    ASSERT_SUCCEEDED(CVibrator::New((IVibrator**)&mVibrator));

    mNotificationCallbacks = new MyNotificationCallbacks(this);
//    mIntentReceiver = new MyIntentReceiver();
}

ECode CNotificationManagerService::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IStatusBarService* statusBar,
    /* [in] */ ILightsManager* lights)
{
//    super();
    mContext = ctx;
    mLightsService = lights;
    ActivityManagerNative::GetDefault((IActivityManager**)&mAm);
//    mSound = new NotificationPlayer(TAG);
//    mSound.setUsesWakeLock(context);
    mHandler = new WorkerHandler();

    mStatusBar = statusBar;
//    statusBar->SetNotificationCallbacks(mNotificationCallbacks);

    lights->GetLight(CLightsService::LIGHT_ID_BATTERY, (ILight**)&mBatteryLight);
    lights->GetLight(CLightsService::LIGHT_ID_NOTIFICATIONS, (ILight**)&mNotificationLight);
    lights->GetLight(CLightsService::LIGHT_ID_ATTENTION, (ILight**)&mAttentionLight);

    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);
    resources->GetColor(0x01060032/*com.android.internal.R.color.config_defaultNotificationColor*/
            , &mDefaultNotificationColor);
    resources->GetInteger(0x010e000d/*com.android.internal.R.integer.config_defaultNotificationLedOn*/
            , &mDefaultNotificationLedOn);
    resources->GetInteger(0x010e000e/*com.android.internal.R.integer.config_defaultNotificationLedOff*/
            , &mDefaultNotificationLedOff);

    // Don't start allowing notifications until the setup wizard has run once.
    // After that, including subsequent boots, init with notifications turned on.
    // This works on the first boot because the setup wizard will toggle this
    // flag at least once and we'll go back to 0 after that.
//    if (0 == Settings.Secure.getInt(mContext.getContentResolver(),
//                Settings.Secure.DEVICE_PROVISIONED, 0)) {
//        mDisabledNotifications = StatusBarManager.DISABLE_NOTIFICATION_ALERTS;
//    }

    // register for battery changed notifications
    AutoPtr<IIntentFilter> filter;
    ASSERT_SUCCEEDED(CIntentFilter::New((IIntentFilter**)&filter));
    filter->AddAction(String(Intent_ACTION_BATTERY_CHANGED));
//    filter->AddAction(UsbManager.ACTION_USB_STATE);
    filter->AddAction(String(Intent_ACTION_SCREEN_ON));
    filter->AddAction(String(Intent_ACTION_SCREEN_OFF));
//    filter->AddAction(TelephonyManager.ACTION_PHONE_STATE_CHANGED);
//    mContext->RegisterReceiver(mIntentReceiver, filter);
    AutoPtr<IIntentFilter> pkgFilter;
    ASSERT_SUCCEEDED(CIntentFilter::New((IIntentFilter**)&pkgFilter));
    pkgFilter->AddAction(String(Intent_ACTION_PACKAGE_REMOVED));
    pkgFilter->AddAction(String(Intent_ACTION_PACKAGE_RESTARTED));
    pkgFilter->AddAction(String(Intent_ACTION_QUERY_PACKAGE_RESTART));
    pkgFilter->AddDataScheme(String("package"));
//    mContext->RegisterReceiver(mIntentReceiver, pkgFilter);
    AutoPtr<IIntentFilter> sdFilter;
    ASSERT_SUCCEEDED(CIntentFilter::New(
            String(Intent_ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE),
            (IIntentFilter**)&sdFilter));
//    mContext->RegisterReceiver(mIntentReceiver, sdFilter);

//    SettingsObserver observer = new SettingsObserver(mHandler);
//    observer.observe();

    return NOERROR;
}

void CNotificationManagerService::SystemReady()
{
    // no beeping until we're basically done booting
    mSystemReady = true;
}

ECode CNotificationManagerService::EnqueueToast(
    /* [in] */ const String& cap,
    /* [in] */ ITransientNotification* cb,
    /* [in] */ Int32 duration)
{
//    if (DBG) Slog.i(TAG, "enqueueToast pkg=" + pkg + " callback=" + callback + " duration=" + duration);

    if (cap == NULL || cb == NULL) {
//        Slog.e(TAG, "Not doing toast. pkg=" + pkg + " callback=" + callback);
        return NOERROR;
    }

    Mutex::Autolock lock(mToastQueueLock);
//    int callingPid = Binder.getCallingPid();
//    long callingId = Binder.clearCallingIdentity();
//    try {
    ToastRecord* record;
    List<ToastRecord*>::Iterator it = IteratorOfToastLocked(cap, cb);
    // If it's already in the queue, we update it in place, we don't
    // move it to the end of the queue.
    if (it != List<ToastRecord*>::Iterator(NULL)) {
        record = *it;
        record->Update(duration);
    }
    else {
        record = new ToastRecord(0/*callingPid*/, cap, cb, duration);
        mToastQueue->PushBack(record);
        it = --(mToastQueue->End());
//        KeepProcessAliveLocked(callingPid);
    }
    // If it's at index 0, it's the current toast.  It doesn't matter if it's
    // new or just been updated.  Call back and tell it to show itself.
    // If the callback fails, this will remove it from the list, so don't
    // assume that it's valid after this.
    if (it == mToastQueue->Begin()) {
        ShowNextToastLocked();
    }
//    } finally {
//        Binder.restoreCallingIdentity(callingId);
//    }

    return NOERROR;
}

ECode CNotificationManagerService::CancelToast(
    /* [in] */ const String& cap,
    /* [in] */ ITransientNotification* cb)
{
//    Slog.i(TAG, "cancelToast pkg=" + pkg + " callback=" + callback);

    if (cap == NULL || cb == NULL) {
//        Slog.e(TAG, "Not cancelling notification. pkg=" + pkg + " callback=" + callback);
        return NOERROR;
    }

    Mutex::Autolock lock(mToastQueueLock);
//    Int64 callingId = Binder.clearCallingIdentity();
//    try {
    List<ToastRecord*>::Iterator it = IteratorOfToastLocked(cap, cb);
    if (it != List<ToastRecord*>::Iterator(NULL)) {
        CancelToastLocked(it);
    }
    else {
//        Slog.w(TAG, "Toast already cancelled. pkg=" + pkg + " callback=" + callback);
    }
//    } finally {
//        Binder.restoreCallingIdentity(callingId);
//    }

    return NOERROR;
}

void CNotificationManagerService::ShowNextToastLocked()
{
    ToastRecord* record = NULL;
    List<ToastRecord*>::Iterator it(NULL);
    if (mToastQueue->Begin() != mToastQueue->End()) {
        it = mToastQueue->Begin();
        record = *(mToastQueue->Begin());
    }

    while (record != NULL) {
//        if (DBG) Slog.d(TAG, "Show pkg=" + record.pkg + " callback=" + record.callback);
//        try {
        record->mCallback->Show();

        ECode ec = ScheduleTimeoutLocked(record, FALSE);
        if (FAILED(ec)) {
//            Slog.w(TAG, "Object died trying to show notification " + record.callback
//                    + " in package " + record.pkg);
            // remove it from the list and let the process die
//            List<ToastRecord*>::Iterator it = mToastQueue.indexOf(record);
            if (it != List<ToastRecord*>::Iterator(NULL)) {
                mToastQueue->Erase(it);
            }
            KeepProcessAliveLocked(record->mPid);
            if (mToastQueue->Begin() != mToastQueue->End()) {
                it = mToastQueue->Begin();
                record = *(mToastQueue->Begin());
            }
            else {
                it = NULL;
                record = NULL;
            }
        }

        return;
    }
}

void CNotificationManagerService::CancelToastLocked(
    List<CNotificationManagerService::ToastRecord*>::Iterator it)
{
}

ECode CNotificationManagerService::ScheduleTimeoutLocked(
    /* [in] */ ToastRecord* r,
    /* [in] */ Boolean immediate)
{
    return NOERROR;
}

List<CNotificationManagerService::ToastRecord*>::Iterator
CNotificationManagerService::IteratorOfToastLocked(
    /* [in] */ String cap,
    /* [in] */ ITransientNotification* callback)
{
    return 0;
}

void CNotificationManagerService::KeepProcessAliveLocked(
    /* [in] */ Int32 pid)
{
}

ECode CNotificationManagerService::EnqueueNotification(
    /* [in] */ const String& cap,
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification,
    /* [in, out] */ const ArrayOf<Int32>& idReceived)
{
    return E_NOT_IMPLEMENTED;
}

ECode CNotificationManagerService::CancelNotification(
    /* [in] */ const String& cap,
    /* [in] */ Int32 id)
{
    return E_NOT_IMPLEMENTED;
}

ECode CNotificationManagerService::CancelAllNotifications(
    /* [in] */ const String& cap)
{
    return E_NOT_IMPLEMENTED;
}

ECode CNotificationManagerService::EnqueueNotificationWithTag(
    /* [in] */ const String& cap,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification,
    /* [in, out]*/ const ArrayOf<Int32>& idReceived)
{
    return E_NOT_IMPLEMENTED;
}

ECode CNotificationManagerService::CancelNotificationWithTag(
    /* [in] */ const String& cap,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id)
{
    return E_NOT_IMPLEMENTED;
}

void CNotificationManagerService::CancelNotification(
    /* [in] */ String pkg,
    /* [in] */ String tag,
    /* [in] */ Int32 id,
    /* [in] */ Int32 mustHaveFlags,
    /* [in] */ Int32 mustNotHaveFlags)
{

}

void CNotificationManagerService::CancelAll()
{
}

void CNotificationManagerService::UpdateLightsLocked()
{
}
