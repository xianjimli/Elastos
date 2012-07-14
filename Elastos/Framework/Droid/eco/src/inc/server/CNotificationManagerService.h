
#ifndef __CNOTIFICATIONMANAGERSERVICE_H__
#define __CNOTIFICATIONMANAGERSERVICE_H__

#include "_CNotificationManagerService.h"
#include "server/CStatusBarManagerService.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/ElRefBase.h>

CarClass(CNotificationManagerService)
{
private:
    static CString TAG;
    static const Boolean DBG = FALSE;

    static const Int32 MAX_PACKAGE_NOTIFICATIONS = 50;

    // message codes
    static const Int32 MESSAGE_TIMEOUT = 2;

    static const Int32 LONG_DELAY = 3500; // 3.5 seconds
    static const Int32 SHORT_DELAY = 2000; // 2 seconds

    static const Int64 DEFAULT_VIBRATE_PATTERN[];

    static const Int32 DEFAULT_STREAM_TYPE = 5;//AudioManager.STREAM_NOTIFICATION;

    static const Int32 BATTERY_LOW_ARGB = 0xFFFF0000; // Charging Low - red solid on
    static const Int32 BATTERY_MEDIUM_ARGB = 0xFFFFFF00;    // Charging - orange solid on
    static const Int32 BATTERY_FULL_ARGB = 0xFF00FF00; // Charging Full - green solid on
    static const Int32 BATTERY_BLINK_ON = 125;
    static const Int32 BATTERY_BLINK_OFF = 2875;

private:
    class NotificationRecord
    {
    public:
        NotificationRecord(
            /* [in] */ String pkg,
            /* [in] */ String tag,
            /* [in] */ Int32 id,
            /* [in] */ Int32 uid,
            /* [in] */ Int32 initialPid,
            /* [in] */ INotification* notification);

    public:
        String mPkg;
        String mTag;
        Int32 mId;
        Int32 mUid;
        Int32 mInitialPid;
        AutoPtr<ITransientNotification> mCallback;
        Int32 mDuration;
        AutoPtr<INotification> mNotification;
        AutoPtr<IBinder> mStatusBarKey;
    };

    class ToastRecord
    {
    public:
        ToastRecord(
            /* [in] */ Int32 pid,
            /* [in] */ String pkg,
            /* [in] */ ITransientNotification* callback,
            /* [in] */ Int32 duration);

        CARAPI_(void) Update(
            /* [in] */ Int32 duration);

    public:
        Int32 mPid;
        String mPkg;
        AutoPtr<ITransientNotification> mCallback;
        Int32 mDuration;
    };

    class WorkerHandler : public IHandler, public ElRefBase
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

//        CARAPI_(void) HandleMessage(
//            /* [in] */ IMessage* msg);
    };

    class MyNotificationCallbacks : public CStatusBarManagerService::NotificationCallbacks
    {
    public:
        MyNotificationCallbacks(
            /* [in] */ CNotificationManagerService* nms);

        CARAPI_(void) OnSetDisabled(
            /* [in] */ Int32 status);

        CARAPI_(void) OnClearAll();

        CARAPI_(void) OnNotificationClick(
            /* [in] */ String pkg,
            /* [in] */ String tag,
            /* [in] */ Int32 id);

        CARAPI_(void) OnPanelRevealed();

        CARAPI_(void) OnNotificationError(
            /* [in] */ String pkg,
            /* [in] */ String tag,
            /* [in] */ Int32 id,
            /* [in] */ Int32 uid,
            /* [in] */ Int32 initialPid,
            /* [in] */ String message);
    private:
        AutoPtr<CNotificationManagerService> mNMService;
    };

//    class MyIntentReceiver : public IBroadcastReceiver
//    {
//    public:
//        CARAPI_(void) OnReceive(
//            /* [in] */ IContext* context,
//            /* [in] */ IIntent* intent);
//    };

public:
//    class SettingsObserver : public ContentObserver
//    {
//    public:
//        SettingsObserver(
//            /* [in] */ IHandler* handler);
//
//        CARAPI_(void) Observe();
//
//        CARAPI_(void) onChange(
//            /* [in] */ Boolean selfChange);
//
//        CARAPI_(void) Update();
//    };

public:
    CNotificationManagerService();

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IStatusBarService* statusBar,
        /* [in] */ ILightsManager* lights);

    CARAPI_(void) SystemReady();

    CARAPI EnqueueNotification(
        /* [in] */ const String& cap,
        /* [in] */ Int32 id,
        /* [in] */ INotification* notification,
        /* [in, out] */ const ArrayOf<Int32>& idReceived);

    CARAPI CancelNotification(
        /* [in] */ const String& cap,
        /* [in] */ Int32 id);

    CARAPI CancelAllNotifications(
        /* [in] */ const String& cap);

    CARAPI EnqueueToast(
        /* [in] */ const String& cap,
        /* [in] */ ITransientNotification* cb,
        /* [in] */ Int32 duration);

    CARAPI CancelToast(
        /* [in] */ const String& cap,
        /* [in] */ ITransientNotification* cb);

    CARAPI EnqueueNotificationWithTag(
        /* [in] */ const String& cap,
        /* [in] */ const String& tag,
        /* [in] */ Int32 id,
        /* [in] */ INotification* notification,
        /* [in, out]*/ const ArrayOf<Int32>& idReceived);

    CARAPI CancelNotificationWithTag(
        /* [in] */ const String& cap,
        /* [in] */ const String& tag,
        /* [in] */ Int32 id);

    CARAPI_(void) CancelAll();

private:
    CARAPI_(void) ShowNextToastLocked();

    CARAPI_(void) CancelToastLocked(
        List<ToastRecord*>::Iterator it);

    CARAPI ScheduleTimeoutLocked(
        /* [in] */ ToastRecord* r,
        /* [in] */ Boolean immediate);

    // lock on mToastQueue
    CARAPI_(List<ToastRecord*>::Iterator) IteratorOfToastLocked(
        /* [in] */ String cap,
        /* [in] */ ITransientNotification* callback);

    // lock on mToastQueue
    CARAPI_(void) KeepProcessAliveLocked(
        /* [in] */ Int32 pid);

    /**
     * Cancels a notification ONLY if it has all of the {@code mustHaveFlags}
     * and none of the {@code mustNotHaveFlags}.
     */
    CARAPI_(void) CancelNotification(
        /* [in] */ String pkg,
        /* [in] */ String tag,
        /* [in] */ Int32 id,
        /* [in] */ Int32 mustHaveFlags,
        /* [in] */ Int32 mustNotHaveFlags);

    // lock on mNotificationList
    CARAPI_(void) UpdateLightsLocked();

public:
    AutoPtr<IContext> mContext;
    AutoPtr<IActivityManager> mAm;
    AutoPtr<IBinder> mForegroundToken;// = new Binder();

private:
    WorkerHandler* mHandler;
    AutoPtr<IStatusBarService> mStatusBar;
    AutoPtr<ILightsManager> mLightsService;
    AutoPtr<ILight> mBatteryLight;
    AutoPtr<ILight> mNotificationLight;
    AutoPtr<ILight> mAttentionLight;

    Int32 mDefaultNotificationColor;
    Int32 mDefaultNotificationLedOn;
    Int32 mDefaultNotificationLedOff;

    NotificationRecord* mSoundNotification;
//    NotificationPlayer* mSound;
    Boolean mSystemReady;
    Int32 mDisabledNotifications;

    NotificationRecord* mVibrateNotification;
    AutoPtr<IVibrator> mVibrator;

    // for enabling and disabling notification pulse behavior
    Boolean mScreenOn;
    Boolean mInCall;
    Boolean mNotificationPulseEnabled;
    // This is true if we have received a new notification while the screen is off
    // (that is, if mLedNotification was set while the screen was off)
    // This is reset to false when the screen is turned on.
    Boolean mPendingPulseNotification;

    // for adb connected notifications
    Boolean mAdbNotificationShown;
    AutoPtr<INotification> mAdbNotification;

    List<NotificationRecord*> mNotificationList;
    Mutex mNotificationListLock;

    List<ToastRecord*>* mToastQueue;
    Mutex mToastQueueLock;

    List<NotificationRecord*> mLights;

    Boolean mBatteryCharging;
    Boolean mBatteryLow;
    Boolean mBatteryFull;
    NotificationRecord* mLedNotification;

    MyNotificationCallbacks* mNotificationCallbacks;
//    MyIntentReceiver* mIntentReceiver;
};

#endif //__CNOTIFICATIONMANAGERSERVICE_H__
