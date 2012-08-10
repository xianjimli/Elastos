
#include "server/CNotificationManagerService.h"
#include "server/CLightsService.h"
#include "os/CVibrator.h"
#include "os/SystemProperties.h"
#include "app/ActivityManagerNative.h"
#include "app/StatusBarManager.h"
#include "app/NotificationManager.h"
#include "content/CIntent.h"
#include "content/CIntentFilter.h"
#include "content/CComponentName.h"
#include "statusbar/CStatusBarNotification.h"
#include "ext/frameworkerr.h"
#include "utils/log.h"
#include "utils/EventLogTags.h"
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

PInterface CNotificationManagerService::NotificationRecord::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

UInt32 CNotificationManagerService::NotificationRecord::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CNotificationManagerService::NotificationRecord::Release()
{
    return ElRefBase::Release();
}

ECode CNotificationManagerService::NotificationRecord::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
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

PInterface CNotificationManagerService::ToastRecord::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

UInt32 CNotificationManagerService::ToastRecord::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CNotificationManagerService::ToastRecord::Release()
{
    return ElRefBase::Release();
}

ECode CNotificationManagerService::ToastRecord::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
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
//        mDisabledNotifications = StatusBarManager::DISABLE_NOTIFICATION_ALERTS;
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
    AutoPtr<ToastRecord> record;
    List<AutoPtr<ToastRecord> >::Iterator it = IteratorOfToastLocked(cap, cb);
    // If it's already in the queue, we update it in place, we don't
    // move it to the end of the queue.
    if (it != List<AutoPtr<ToastRecord> >::Iterator(NULL)) {
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
    List<AutoPtr<ToastRecord> >::Iterator it = IteratorOfToastLocked(cap, cb);
    if (it != List<AutoPtr<ToastRecord> >::Iterator(NULL)) {
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
    AutoPtr<ToastRecord> record;
    List<AutoPtr<ToastRecord> >::Iterator it(NULL);
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
//            List<AutoPtr<ToastRecord> >::Iterator it = mToastQueue->indexOf(record);
            if (it != List<AutoPtr<ToastRecord> >::Iterator(NULL)) {
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
    List<AutoPtr<CNotificationManagerService::ToastRecord> >::Iterator it)
{
    AutoPtr<ToastRecord> record = *it;
//    try {
    record->mCallback->Hide();
//    } catch (RemoteException e) {
//        Slog.w(TAG, "Object died trying to hide notification " + record.callback
//                + " in package " + record.pkg);
//        // don't worry about this, we're about to remove it from
//        // the list anyway
//    }
    mToastQueue->Erase(it);
    KeepProcessAliveLocked(record->mPid);//todo autoptr
    if (mToastQueue->Begin() != mToastQueue->End()) {
        // Show the next one. If the callback fails, this will remove
        // it from the list, so don't assume that the list hasn't changed
        // after this point.
        ShowNextToastLocked();
    }
}

ECode CNotificationManagerService::ScheduleTimeoutLocked(
    /* [in] */ ToastRecord* r,
    /* [in] */ Boolean immediate)
{
//    AutoPtr<IMessage> m = Message.obtain(mHandler, MESSAGE_TIMEOUT, r);
//    Int64 delay = immediate ? 0 : (r->mDuration ==
//            1/*Toast.LENGTH_LONG*/ ? LONG_DELAY : SHORT_DELAY);
//    ECode ec = mHandler->RemoveCallbacksAndMessages(r);
//    if (FAILED(ec)) {
//        return ec;
//    }

//    return mHandler->SendMessageDelayed(m, delay);
    return NOERROR;
}

List<AutoPtr<CNotificationManagerService::ToastRecord> >::Iterator
CNotificationManagerService::IteratorOfToastLocked(
    /* [in] */ String cap,
    /* [in] */ ITransientNotification* callback)
{
//    IBinder cbak = callback.asBinder();
    List<AutoPtr<ToastRecord> >* list = mToastQueue;
    List<AutoPtr<ToastRecord> >::Iterator it;
    for (it = list->Begin(); it != list->End(); it++) {
        AutoPtr<ToastRecord> r = *it;
        if (r->mPkg == cap && r->mCallback.Get()/*.asBinder()*/ == callback) {
            return it;
        }
    }
    return NULL;
}

void CNotificationManagerService::KeepProcessAliveLocked(
    /* [in] */ Int32 pid)
{
    Int32 toastCount = 0; // toasts from this pid
    List<AutoPtr<ToastRecord> >* list = mToastQueue;
    List<AutoPtr<ToastRecord> >::Iterator it;
    for (it = list->Begin(); it != list->End(); it++) {
        AutoPtr<ToastRecord> r = *it;
        if (r->mPid == pid) {
            toastCount++;
        }
    }
//    try {
    mAm->SetProcessForeground(mForegroundToken, pid, toastCount > 0);
//    } catch (RemoteException e) {
        // Shouldn't happen.
//    }
}

ECode CNotificationManagerService::EnqueueNotification(
    /* [in] */ const String& cap,
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification,
    /* [in, out] */ ArrayOf<Int32>* idOut)
{
    return EnqueueNotificationWithTag(cap, String(NULL) /* tag */, id, notification, idOut);
}

ECode CNotificationManagerService::EnqueueNotificationWithTag(
    /* [in] */ const String& cap,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification,
    /* [in, out]*/ ArrayOf<Int32>* idOut)
{
    return EnqueueNotificationInternal(cap, 0/*Binder.getCallingUid()*/,
            0/*Binder.getCallingPid()*/, tag, id, notification, idOut);
}

ECode CNotificationManagerService::EnqueueNotificationInternal(
    /* [in] */ const String& cap,
    /* [in] */ Int32 callingUid,
    /* [in] */ Int32 callingPid,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification,
    /* [in] */ ArrayOf<Int32>* idOut)
{
    CheckIncomingCall(cap);

    // Limit the number of notifications that any given package except the android
    // package can enqueue.  Prevents DOS attacks and deals with leaks.
    if (!(String("android") == cap)) {
        Mutex::Autolock lock(mNotificationListLock);
        Int32 count = 0;
        List<AutoPtr<NotificationRecord> >::Iterator it;
        for (it = mNotificationList.Begin(); it != mNotificationList.End(); it++) {
            AutoPtr<NotificationRecord> r = *it;
            if (r->mPkg == cap) {
                count++;
                if (count >= MAX_PACKAGE_NOTIFICATIONS) {
 //                   Slog.e(TAG, "Package has already posted " + count
 //                           + " notifications.  Not showing more.  package=" + pkg);
                    return NOERROR;
                }
            }
        }
    }

    // This conditional is a dirty hack to limit the logging done on
    //     behalf of the download manager without affecting other apps.
    if (!(cap == String("com.android.providers.downloads"))
            /*|| Log.isLoggable("DownloadManager", Log.VERBOSE)*/) {
//        EventLog.writeEvent(EventLogTags.NOTIFICATION_ENQUEUE, pkg, id, notification.toString());
    }

    if (cap.IsNull() || notification == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("null not allowed: pkg=" + pkg
//                + " id=" + id + " notification=" + notification);
    }

    AutoPtr<CNotification> notificationCls = (CNotification*)notification;
    if (notificationCls->mIcon != 0) {
//        if (((CNotification*)notification)->mContentView == NULL) {
//            return E_ILLEGAL_ARGUMENT_EXCEPTION;
//            throw new IllegalArgumentException("contentView required: pkg=" + pkg
//                    + " id=" + id + " notification=" + notification);
//        }
        if (notificationCls->mContentIntent == NULL) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
//            throw new IllegalArgumentException("contentIntent required: pkg=" + pkg
//                    + " id=" + id + " notification=" + notification);
        }
    }

    Mutex::Autolock lock(mNotificationListLock);
    AutoPtr<NotificationRecord> r = new NotificationRecord(cap, tag, id,
            callingUid, callingPid, notification);
    AutoPtr<NotificationRecord> old;

    List<AutoPtr<NotificationRecord> >::Iterator it = IteratorOfNotificationLocked(cap, tag, id);
    if (it != List<AutoPtr<NotificationRecord> >::Iterator(NULL)) {
        mNotificationList.PushBack(r);
    }
    else {
        old = *it;
        List<AutoPtr<NotificationRecord> >::Iterator newIt = mNotificationList.Erase(it);
        mNotificationList.Insert(newIt, r);
        // Make sure we don't lose the foreground service state.
        if (old != NULL) {
            notificationCls->mFlags |=
                ((CNotification*)old->mNotification.Get())->mFlags & Notification_FLAG_FOREGROUND_SERVICE;
        }
    }

    // Ensure if this is a foreground service that the proper additional
    // flags are set.
    if ((notificationCls->mFlags & Notification_FLAG_FOREGROUND_SERVICE) != 0) {
        notificationCls->mFlags |= Notification_FLAG_ONGOING_EVENT
                | Notification_FLAG_NO_CLEAR;
    }

    if (notificationCls->mIcon != 0) {
        AutoPtr<IStatusBarNotification> n;
        CStatusBarNotification::New(cap, id, tag,
                r->mUid, r->mInitialPid, notification, (IStatusBarNotification**)&n);
        if (old != NULL && old->mStatusBarKey != NULL) {
            r->mStatusBarKey = old->mStatusBarKey;
//            long identity = Binder.clearCallingIdentity();
//            try {
            mStatusBar->UpdateNotification(r->mStatusBarKey, n);
//            }
//            finally {
//                Binder.restoreCallingIdentity(identity);
//            }
        }
        else {
//            long identity = Binder.clearCallingIdentity();
//            try {
            mStatusBar->AddNotification(n, (IBinder**)&(r->mStatusBarKey));
            mAttentionLight->Pulse();
//            }
//            finally {
//                Binder.restoreCallingIdentity(identity);
//            }
        }
        SendAccessibilityEvent(notification, cap);
    }
    else {
        if (old != NULL && old->mStatusBarKey != NULL) {
//            long identity = Binder.clearCallingIdentity();
//            try {
            mStatusBar->RemoveNotification(old->mStatusBarKey);
//            }
//            finally {
//                Binder.restoreCallingIdentity(identity);
//            }
        }
    }

    // If we're not supposed to beep, vibrate, etc. then don't.
    if (((mDisabledNotifications & StatusBarManager::DISABLE_NOTIFICATION_ALERTS) == 0)
            && (!(old != NULL
                && (notificationCls->mFlags & Notification_FLAG_ONLY_ALERT_ONCE) != 0 ))
            && mSystemReady) {

//        AudioManager audioManager = (AudioManager) mContext
//        .getSystemService(Context.AUDIO_SERVICE);
        // sound
        Boolean useDefaultSound =
            (notificationCls->mDefaults & Notification_DEFAULT_SOUND) != 0;
        if (useDefaultSound || notificationCls->mSound != NULL) {
            AutoPtr<IUri> uri;
            if (useDefaultSound) {
//                uri = Settings.System.DEFAULT_NOTIFICATION_URI;
            }
            else {
                uri = notificationCls->mSound;
            }
            Boolean looping = (notificationCls->mFlags & Notification_FLAG_INSISTENT) != 0;
            Int32 audioStreamType;
            if (notificationCls->mAudioStreamType >= 0) {
                audioStreamType = notificationCls->mAudioStreamType;
            }
            else {
                audioStreamType = DEFAULT_STREAM_TYPE;
            }
            mSoundNotification = r;
            // do not play notifications if stream volume is 0
            // (typically because ringer mode is silent).
//            if (audioManager.getStreamVolume(audioStreamType) != 0) {
//                long identity = Binder.clearCallingIdentity();
//                try {
//                    mSound.play(mContext, uri, looping, audioStreamType);
//                }
//                finally {
//                    Binder.restoreCallingIdentity(identity);
//                }
//            }
        }

        // vibrate
        Boolean useDefaultVibrate =
            (notificationCls->mDefaults & Notification_DEFAULT_VIBRATE) != 0;
        if ((useDefaultVibrate || notificationCls->mVibrate != NULL)
                /*&& audioManager.shouldVibrate(AudioManager.VIBRATE_TYPE_NOTIFICATION)*/) {
//            mVibrateNotification = r;

//            ArrayOf<Int64>& vibrate;
//            if (useDefaultVibrate) {
//                vibrate = ArrayOf<Int64>(const_cast<Int64*>(DEFAULT_VIBRATE_PATTERN), 4)
//            }
//            else {
//                vibrate = *(notificationCls->mVibrate);
//            }
//            mVibrator->VibrateEx(vibrate,
//                       ((notificationCls->mFlags & Notification_FLAG_INSISTENT) != 0) ? 0: -1);
        }
    }

    // this option doesn't shut off the lights

    // light
    // the most recent thing gets the light
    mLights.Remove(old);
    if (mLedNotification == old) {
        mLedNotification = NULL;
    }
    //Slog.i(TAG, "notification.lights="
    //        + ((old.notification.lights.flags & Notification.FLAG_SHOW_LIGHTS) != 0));
    if ((notificationCls->mFlags & Notification_FLAG_SHOW_LIGHTS) != 0) {
        mLights.PushBack(r);
        UpdateLightsLocked();
    }
    else {
        if (old != NULL
                && ((((CNotification*)old->mNotification.Get())->mFlags & Notification_FLAG_SHOW_LIGHTS) != 0)) {
            UpdateLightsLocked();
        }
    }

    (*idOut)[0] = id;

    return NOERROR;
}

void CNotificationManagerService::SendAccessibilityEvent(
    /* [in] */ INotification* notification,
    /* [in] */ String capsuleName)
{
//    AccessibilityManager manager = AccessibilityManager.getInstance(mContext);
//    if (!manager.isEnabled()) {
//        return;
//    }

//    AutoPtr<IAccessibilityEvent> event;
//    AccessibilityEvent.obtain(AccessibilityEvent.TYPE_NOTIFICATION_STATE_CHANGED);
//    event.setPackageName(packageName);
//    event.setClassName(Notification.class.getName());
//    event.setParcelableData(notification);
//    CharSequence tickerText = notification.tickerText;
//    if (!TextUtils.isEmpty(tickerText)) {
//        event.getText().add(tickerText);
//    }

//    manager.sendAccessibilityEvent(event);
}

void CNotificationManagerService::CancelNotificationLocked(
    /* [in] */ NotificationRecord* r)
{
    // status bar
    if (((CNotification*)r->mNotification.Get())->mIcon != 0) {
//        long identity = Binder.clearCallingIdentity();
//        try {
        mStatusBar->RemoveNotification(r->mStatusBarKey);
//        }
//        finally {
//            Binder.restoreCallingIdentity(identity);
//        }
        r->mStatusBarKey = NULL;
    }

    // sound
    if (mSoundNotification.Get() == r) {
        mSoundNotification = NULL;
//        long identity = Binder.clearCallingIdentity();
//        try {
//            mSound.stop();
//        }
//        finally {
//            Binder.restoreCallingIdentity(identity);
//        }
    }

    // vibrate
    if (mVibrateNotification.Get() == r) {
        mVibrateNotification = NULL;
//        long identity = Binder.clearCallingIdentity();
//        try {
        mVibrator->Cancel();
//        }
//        finally {
//            Binder.restoreCallingIdentity(identity);
//        }
    }

    // light
    mLights.Remove(r);
    if (mLedNotification.Get() == r) {
        mLedNotification = NULL;
    }
}

void CNotificationManagerService::CancelNotification(
    /* [in] */ String cap,
    /* [in] */ String tag,
    /* [in] */ Int32 id,
    /* [in] */ Int32 mustHaveFlags,
    /* [in] */ Int32 mustNotHaveFlags)
{
//    EventLog.writeEvent(EventLogTags.NOTIFICATION_CANCEL, pkg, id, mustHaveFlags);

    Mutex::Autolock lock(mNotificationListLock);
    List<AutoPtr<NotificationRecord> >::Iterator it
            = IteratorOfNotificationLocked(cap, tag, id);
    if (it != List<AutoPtr<NotificationRecord> >::Iterator(NULL)) {
        AutoPtr<NotificationRecord> r = *it;

        AutoPtr<CNotification> notificationCls = (CNotification*)r->mNotification.Get();
        if ((notificationCls->mFlags & mustHaveFlags) != mustHaveFlags) {
            return;
        }
        if ((notificationCls->mFlags & mustNotHaveFlags) != 0) {
            return;
        }

        mNotificationList.Erase(it);

        CancelNotificationLocked(r);
        UpdateLightsLocked();
    }
}

Boolean CNotificationManagerService::CancelAllNotificationsInt(
    /* [in] */ String cap,
    /* [in] */ Int32 mustHaveFlags,
    /* [in] */ Int32 mustNotHaveFlags,
    /* [in] */ Boolean doit)
{
//    EventLog.writeEvent(EventLogTags.NOTIFICATION_CANCEL_ALL, pkg, mustHaveFlags);

    Mutex::Autolock lock(mNotificationListLock);
    List<AutoPtr<NotificationRecord> >::ReverseIterator rit
            = mNotificationList.RBegin();
    Boolean canceledSomething = FALSE;
    while (rit != mNotificationList.REnd()) {
        AutoPtr<NotificationRecord> r = *rit;
        AutoPtr<CNotification> notificationCls = (CNotification*)r->mNotification.Get();
        if ((notificationCls->mFlags & mustHaveFlags) != mustHaveFlags) {
            continue;
        }
        if ((notificationCls->mFlags & mustNotHaveFlags) != 0) {
            continue;
        }
        if (!(r->mPkg == cap)) {
            continue;
        }
        canceledSomething = TRUE;
        if (!doit) {
            return TRUE;
        }

        List<AutoPtr<NotificationRecord> >::Iterator it = rit.GetBase();
        ++rit;
        mNotificationList.Erase(it);
        CancelNotificationLocked(r);
    }
    if (canceledSomething) {
        UpdateLightsLocked();
    }
    return canceledSomething;
}

ECode CNotificationManagerService::CancelNotification(
    /* [in] */ const String& cap,
    /* [in] */ Int32 id)
{
    return CancelNotificationWithTag(cap, String(NULL) /* tag */, id);
}

ECode CNotificationManagerService::CancelNotificationWithTag(
    /* [in] */ const String& cap,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id)
{
    CheckIncomingCall(cap);
    // Don't allow client applications to cancel foreground service notis.
    CancelNotification(cap, tag, id, 0,
            0/*Binder.getCallingUid() == Process.SYSTEM_UID*/
            ? 0 : Notification_FLAG_FOREGROUND_SERVICE);

    return NOERROR;
}

ECode CNotificationManagerService::CancelAllNotifications(
    /* [in] */ const String& cap)
{
    CheckIncomingCall(cap);

    // Calling from user space, don't allow the canceling of actively
    // running foreground services.
    CancelAllNotificationsInt(cap, 0, Notification_FLAG_FOREGROUND_SERVICE, TRUE);

    return NOERROR;
}

void CNotificationManagerService::CheckIncomingCall(
    /* [in] */ const String& cap)
{
//    int uid = Binder.getCallingUid();
//    if (uid == Process.SYSTEM_UID || uid == 0) {
//        return;
//    }
//    try {
//    AutoPtr<IApplicationInfo> ai;
//    mContext.getPackageManager().getApplicationInfo(
//            pkg, 0);
//    if (ai.uid != uid) {
//        throw new SecurityException("Calling uid " + uid + " gave package"
//                + pkg + " which is owned by uid " + ai.uid);
//    }
//    } catch (PackageManager.NameNotFoundException e) {
//        throw new SecurityException("Unknown package " + pkg);
//    }
}

void CNotificationManagerService::CancelAll()
{
    Mutex::Autolock lock(mNotificationListLock);

    List<AutoPtr<NotificationRecord> >::ReverseIterator rit = mNotificationList.RBegin();
    while (rit != mNotificationList.REnd()) {
        AutoPtr<NotificationRecord> r = *rit;

        AutoPtr<CNotification> notificationCls = (CNotification*)r->mNotification.Get();
        if ((notificationCls->mFlags & (Notification_FLAG_ONGOING_EVENT
                        | Notification_FLAG_NO_CLEAR)) == 0) {
            if (notificationCls->mDeleteIntent != NULL) {
//                try {
//                notificationCls->mDeleteIntent->Send();
//                } catch (PendingIntent.CanceledException ex) {
                    // do nothing - there's no relevant way to recover, and
                    //     no reason to let this propagate
//                    Slog.w(TAG, "canceled PendingIntent for " + r.pkg, ex);
//                }
            }
            List<AutoPtr<NotificationRecord> >::Iterator it = rit.GetBase();
            ++rit;
            mNotificationList.Erase(it);
            CancelNotificationLocked(r);
        }
    }

    UpdateLightsLocked();
}

void CNotificationManagerService::UpdateLights()
{
    Mutex::Autolock lock(mNotificationListLock);

    UpdateLightsLocked();
}

void CNotificationManagerService::UpdateLightsLocked()
{
    // Battery low always shows, other states only show if charging.
    if (mBatteryLow) {
        if (mBatteryCharging) {
            mBatteryLight->SetColor(BATTERY_LOW_ARGB);
        }
        else {
            // Flash when battery is low and not charging
            mBatteryLight->SetFlashing(BATTERY_LOW_ARGB, 1/*LightsService.LIGHT_FLASH_TIMED*/,
                    BATTERY_BLINK_ON, BATTERY_BLINK_OFF);
        }
    }
    else if (mBatteryCharging) {
        if (mBatteryFull) {
            mBatteryLight->SetColor(BATTERY_FULL_ARGB);
        }
        else {
            mBatteryLight->SetColor(BATTERY_MEDIUM_ARGB);
        }
    }
    else {
        mBatteryLight->TurnOff();
    }

    // clear pending pulse notification if screen is on
    if (mScreenOn || mLedNotification == NULL) {
        mPendingPulseNotification = FALSE;
    }

    // handle notification lights
    if (mLedNotification == NULL) {
        // get next notification, if any
        if (mLights.Begin() != mLights.End()) {
            mLedNotification = *(mLights.RBegin());
        }
        if (mLedNotification != NULL && !mScreenOn) {
            mPendingPulseNotification = TRUE;
        }
    }

    // we only flash if screen is off and persistent pulsing is enabled
    // and we are not currently in a call
    if (!mPendingPulseNotification || mScreenOn || mInCall) {
        mNotificationLight->TurnOff();
    }
    else {
        AutoPtr<CNotification> notificationCls = (CNotification*)mLedNotification->mNotification.Get();
        Int32 ledARGB = notificationCls->mLedARGB;
        Int32 ledOnMS = notificationCls->mLedOnMS;
        Int32 ledOffMS = notificationCls->mLedOffMS;
        if ((notificationCls->mDefaults & Notification_DEFAULT_LIGHTS) != 0) {
            ledARGB = mDefaultNotificationColor;
            ledOnMS = mDefaultNotificationLedOn;
            ledOffMS = mDefaultNotificationLedOff;
        }
        if (mNotificationPulseEnabled) {
            // pulse repeatedly
            mNotificationLight->SetFlashing(ledARGB, 1/*LightsService.LIGHT_FLASH_TIMED*/,
                    ledOnMS, ledOffMS);
        }
        else {
            // pulse only once
            mNotificationLight->PulseEx(ledARGB, ledOnMS);
        }
    }
}

List<AutoPtr<CNotificationManagerService::NotificationRecord> >::Iterator
CNotificationManagerService::IteratorOfNotificationLocked(
    /* [in] */ String cap,
    /* [in] */ String tag,
    /* [in] */ Int32 id)
{
    List<AutoPtr<NotificationRecord> > list = mNotificationList;
    List<AutoPtr<NotificationRecord> >::Iterator it;
    for (it = list.Begin(); it != list.End(); it++) {
        AutoPtr<NotificationRecord> r = *it;
        if (tag.IsNull()) {
            if (!(r->mTag.IsNull())) {
                continue;
            }
        }
        else {
            if (tag != r->mTag) {
                continue;
            }
        }
        if (r->mId == id && r->mPkg == cap) {
            return it;
        }
    }

    return NULL;
}

void CNotificationManagerService::UpdateAdbNotification(
    /* [in] */ Boolean adbEnabled)
{
    if (adbEnabled) {
        if (String("0") == SystemProperties::Get("persist.adb.notify")) {
            return;
        }
        if (!mAdbNotificationShown) {
            AutoPtr<INotificationManager> notificationManager;

            mContext->GetSystemService(Context_NOTIFICATION_SERVICE,
                    (IInterface**)&notificationManager);
            if (notificationManager != NULL) {
                AutoPtr<IResources> r;
                mContext->GetResources((IResources**)&r);
                AutoPtr<ICharSequence> title;
                r->GetText(0x01040342/*com.android.internal.R.string.adb_active_notification_title*/,
                        (ICharSequence**)&title);
                AutoPtr<ICharSequence> message;
                r->GetText(0x01040343/*com.android.internal.R.string.adb_active_notification_message*/,
                        (ICharSequence**)&message);

                if (mAdbNotification.Get() == NULL) {
                    CNotification::NewByFriend((CNotification**)&mAdbNotification);
                    mAdbNotification->mIcon = 0x01080295/*com.android.internal.R.drawable.stat_sys_adb*/;
                    mAdbNotification->mWhen = 0;
                    mAdbNotification->mFlags = Notification_FLAG_ONGOING_EVENT;
                    mAdbNotification->mTickerText = title;
                    mAdbNotification->mDefaults = 0; // please be quiet
                    mAdbNotification->mSound = NULL;
                    mAdbNotification->mVibrate = NULL;
                }

                AutoPtr<IIntent> intent;
                CIntent::New(String("elastos.settings.APPLICATION_DEVELOPMENT_SETTINGS")
                        /*Settings.ACTION_APPLICATION_DEVELOPMENT_SETTINGS*/,
                        (IIntent**)&intent);
                intent->SetFlags(Intent_FLAG_ACTIVITY_NEW_TASK |
                        Intent_FLAG_ACTIVITY_RESET_TASK_IF_NEEDED);
                // Note: we are hard-coding the component because this is
                // an important security UI that we don't want anyone
                // intercepting.
                AutoPtr<IComponentName> componentName;
                CComponentName::New(String("com.elastos.settings"),
                        String("com.elastos.settings.DevelopmentSettings"),
                        (IComponentName**)&componentName);
                intent->SetComponent(componentName);
//                AutoPtr<IPendingIntent> pi = PendingIntent.getActivity(mContext, 0,
//                        intent, 0);

//                mAdbNotification->SetLatestEventInfo(mContext, title, message, pi);

                mAdbNotificationShown = TRUE;
                ((NotificationManager*)notificationManager.Get())->Notify(0x01040342
                        /*com.android.internal.R.string.adb_active_notification_title*/,
                        mAdbNotification);
            }
        }

    }
    else if (mAdbNotificationShown) {
        AutoPtr<INotificationManager> notificationManager;
        mContext->GetSystemService(Context_NOTIFICATION_SERVICE, (IInterface**)&notificationManager);
        if (notificationManager != NULL) {
            mAdbNotificationShown = FALSE;
            ((NotificationManager*)notificationManager.Get())->Cancel(0x01040342
                    /*com.android.internal.R.string.adb_active_notification_title*/);
        }
    }
}

void CNotificationManagerService::UpdateNotificationPulse()
{
}
