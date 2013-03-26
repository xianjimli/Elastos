
#ifndef __CPHONEWINDOWMANAGER_H__
#define __CPHONEWINDOWMANAGER_H__

#include "_CPhoneWindowManager.h"
#include "ext/frameworkext.h"
#include "graphics/CRect.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>

CarClass(CPhoneWindowManager)
{
public:
    static const char* SYSTEM_DIALOG_REASON_KEY;
    static const char* SYSTEM_DIALOG_REASON_GLOBAL_ACTIONS;
    static const char* SYSTEM_DIALOG_REASON_RECENT_APPS;
    static const char* SYSTEM_DIALOG_REASON_HOME_KEY;

private:
    static const char* TAG;
    static const Boolean DEBUG = FALSE;
    static const Boolean localLOGV = DEBUG; //? Config.LOGD : Config.LOGV;
    static const Boolean DEBUG_LAYOUT = FALSE;
    static const Boolean SHOW_STARTING_ANIMATIONS = TRUE;
    static const Boolean SHOW_PROCESSES_ON_ALT_MENU = FALSE;

    // wallpaper is at the bottom, though the window manager may move it.
    static const Int32 WALLPAPER_LAYER = 2;
    static const Int32 APPLICATION_LAYER = 2;
    static const Int32 PHONE_LAYER = 3;
    static const Int32 SEARCH_BAR_LAYER = 4;
    static const Int32 STATUS_BAR_PANEL_LAYER = 5;
    static const Int32 SYSTEM_DIALOG_LAYER = 6;
    // toasts and the plugged-in battery thing
    static const Int32 TOAST_LAYER = 7;
    static const Int32 STATUS_BAR_LAYER = 8;
    // SIM errors and unlock.  Not sure if this really should be in a high layer.
    static const Int32 PRIORITY_PHONE_LAYER = 9;
    // like the ANR / app crashed dialogs
    static const Int32 SYSTEM_ALERT_LAYER = 10;
    // system-level error dialogs
    static const Int32 SYSTEM_ERROR_LAYER = 11;
    // on-screen keyboards and other such input method user Int32erfaces go here.
    static const Int32 INPUT_METHOD_LAYER = 12;
    // on-screen keyboards and other such input method user Int32erfaces go here.
    static const Int32 INPUT_METHOD_DIALOG_LAYER = 13;
    // the keyguard; nothing on top of these can take focus, since they are
    // responsible for power management when displayed.
    static const Int32 KEYGUARD_LAYER = 14;
    static const Int32 KEYGUARD_DIALOG_LAYER = 15;
    // things in here CAN NOT take focus, but are shown on top of everything else.
    static const Int32 SYSTEM_OVERLAY_LAYER = 16;
    static const Int32 SECURE_SYSTEM_OVERLAY_LAYER = 17;
    // the navigation bar, if available, shows atop most things
    static const Int32 NAVIGATION_BAR_LAYER = 18;

    static const Int32 APPLICATION_MEDIA_SUBLAYER = -2;
    static const Int32 APPLICATION_MEDIA_OVERLAY_SUBLAYER = -1;
    static const Int32 APPLICATION_PANEL_SUBLAYER = 1;
    static const Int32 APPLICATION_SUB_PANEL_SUBLAYER = 2;

    // Debugging: set this to have the system act like there is no hard keyboard.
    static const Boolean KEYBOARD_ALWAYS_HIDDEN = FALSE;

    // Useful scan codes.
    static const Int32 SW_LID = 0x00;
    static const Int32 BTN_MOUSE = 0x110;

    static const Int32 DEFAULT_ACCELEROMETER_ROTATION = 0;

    static const Boolean PRINT_ANIM = FALSE;

    static const Int32 WINDOW_TYPES_WHERE_HOME_DOESNT_WORK[];

public:
    CPhoneWindowManager();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IWindowManager* windowManager
        /* [in] */ /*LocalPowerManager powerManager*/);

    CARAPI CheckAddPermission(
        /* [in] */ IWindowManagerLayoutParams* attrs,
        /* [out] */ Int32* addPermisssion);

    CARAPI AdjustWindowParamsLw(
        /* [in] */ IWindowManagerLayoutParams* attrs);

    CARAPI AdjustConfigurationLw(
        /* [in] */ IConfiguration* config);

    CARAPI WindowTypeToLayerLw(
        /* [in] */ Int32 type,
        /* [out] */ Int32* layer);

    CARAPI SubWindowTypeToLayerLw(
        /* [in] */ Int32 type,
        /* [out] */ Int32* layer);

    CARAPI GetMaxWallpaperLayer(
        /* [out] */ Int32* layer);

    CARAPI DoesForceHide(
        /* [in] */ IWindowState* win,
        /* [in] */ IWindowManagerLayoutParams* attrs,
        /* [out] */ Boolean* hiden);

    CARAPI CanBeForceHidden(
        /* [in] */ IWindowState* win,
        /* [in] */ IWindowManagerLayoutParams* attrs,
        /* [out] */ Boolean* canHiden);

    CARAPI AddStartingWindow(
        /* [in] */ IBinder* appToken,
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 theme,
        /* [in] */ ICharSequence* nonLocalizedLabel,
        /* [in] */ Int32 labelRes,
        /* [in] */ Int32 icon,
        /* [out] */ IView** window);

    CARAPI RemoveStartingWindow(
        /* [in] */ IBinder* appToken,
        /* [in] */ IView* window);

    CARAPI PrepareAddWindowLw(
        /* [in] */ IWindowState* win,
        /* [in] */ IWindowManagerLayoutParams* attrs,
        /* [out] */ Int32* added);

    CARAPI RemoveWindowLw(
        /* [in] */ IWindowState* win);

    CARAPI SelectAnimationLw(
        /* [in] */ IWindowState* win,
        /* [in] */ Int32 transit,
        /* [out] */ Int32* id);

    //Animation CreateForceHideEnterAnimation();

    CARAPI InterceptKeyBeforeQueueing(
        /* [in] */ Int64 whenNanos,
        /* [in] */ Int32 action,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 scanCode,
        /* [in] */ Int32 policyFlags,
        /* [in] */ Boolean isScreenOn,
        /* [out] */ Int32* bitwise);

    CARAPI InterceptKeyBeforeDispatching(
        /* [in] */ IWindowState* win,
        /* [in] */ Int32 action,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 scanCode,
        /* [in] */ Int32 metaState,
        /* [in] */ Int32 repeatCount,
        /* [in] */ Int32 policyFlags,
        /* [out] */ Boolean* consumed);

    CARAPI BeginLayoutLw(
        /* [in] */ Int32 displayWidth,
        /* [in] */ Int32 displayHeight);

    CARAPI LayoutWindowLw(
        /* [in] */ IWindowState* win,
        /* [in] */ IWindowManagerLayoutParams* attrs,
        /* [in] */ IWindowState* attached);

    CARAPI GetContentInsetHintLw(
        /* [in] */ IWindowManagerLayoutParams* attrs,
        /* [in] */ IRect* contentInset);

    CARAPI FinishLayoutLw(
        /* [out] */ Int32* bitSet);

    CARAPI BeginAnimationLw(
        /* [in] */ Int32 displayWidth,
        /* [in] */ Int32 displayHeight);

    CARAPI AnimatingWindowLw(
        /* [in] */ IWindowState* win,
        /* [in] */ IWindowManagerLayoutParams* attrs);

    CARAPI FinishAnimationLw(
        /* [out] */ Int32* bitSet);

    CARAPI AllowAppAnimationsLw(
        /* [out] */ Boolean* allowed);

    CARAPI ScreenTurnedOff(
        /* [in] */ Int32 why);

    CARAPI ScreenTurnedOn();

    CARAPI IsScreenOn(
        /* [out] */ Boolean* isScreenOn);

    CARAPI NotifyLidSwitchChanged(
        /* [in] */ Int64 whenNanos,
        /* [in] */ Boolean lidOpen);

    CARAPI EnableKeyguard(
        /* [in] */ Boolean enabled);

    //CARAPI ExitKeyguardSecurely(
    //    /* [in] */ OnKeyguardExitResult callback);

    CARAPI InKeyguardRestrictedKeyInputMode(
        /* [out] */ Boolean* restricted);

    CARAPI RotationForOrientationLw(
        /* [in] */ Int32 orientation,
        /* [in] */ Int32 lastRotation,
        /* [in] */ Boolean displayEnabled,
        /* [out] */ Int32* surfaceRotation);

    CARAPI DetectSafeMode(
        /* [out] */ Boolean* isSafe);

    CARAPI SystemReady();

    CARAPI UserActivity();

    CARAPI EnableScreenAfterBoot();

    CARAPI SetCurrentOrientationLw(
        /* [in] */ Int32 newOrientation);

    CARAPI PerformHapticFeedbackLw(
        /* [in] */ IWindowState* win,
        /* [in] */ Int32 effectId,
        /* [in] */ Boolean always,
        /* [out] */ Boolean* isSucceed);

    CARAPI ScreenOnStoppedLw();

    CARAPI AllowKeyRepeat(
        /* [out] */ Boolean* allowed);

    CARAPI_(void) UpdateSettings();

protected:
    //static ITelephony GetTelephonyService();

    //static IAudioService GetAudioService();

    static CARAPI_(void) SendCloseSystemWindows(
        /* [in] */ IContext* context,
        /* [in] */ const char* reason);

    static CARAPI_(ArrayOf<Int64>*) GetLongIntArray(
        /* [in] */ IResources* r,
        /* [in] */ Int32 resid);

    CARAPI_(Boolean) UseSensorForOrientationLp(
        /* [in] */ Int32 appOrientation);

    /*
    * We always let the sensor be switched on by default except when
    * the user has explicitly disabled sensor based rotation or when the
    * screen is switched off.
    */
    CARAPI_(Boolean) NeedSensorRunningLp();

    /*
    * Various use cases for invoking this function
    * screen turning off, should always disable listeners if already enabled
    * screen turned on and current app has sensor based orientation, enable listeners
    * if not already enabled
    * screen turned on and current app does not have sensor orientation, disable listeners if
    * already enabled
    * screen turning on and current app has sensor based orientation, enable listeners if needed
    * screen turning on and current app has nosensor based orientation, do nothing
    */
    CARAPI_(void) UpdateOrientationListenerLp();

    CARAPI_(void) ShowGlobalActionsDialog();

    CARAPI_(Boolean) IsDeviceProvisioned();

    /**
    * Create (if necessary) and launch the recent apps dialog
    */
    CARAPI_(void) ShowRecentAppsDialog();

    CARAPI_(void) ReadLidState();

    CARAPI_(Boolean) KeyguardOn();

    /**
    * A home key -> launch home action was detected.  Take the appropriate action
    * given the situation with the keyguard.
    */
    CARAPI_(void) LaunchHomeFromHotKey();

    CARAPI_(void) SetAttachedWindowFrames(
        /* [in] */ IWindowState* win,
        /* [in] */ Int32 fl,
        /* [in] */ Int32 sim,
        /* [in] */ IWindowState* attached,
        /* [in] */ Boolean insetDecors,
        /* [in] */ IRect* pf,
        /* [in] */ IRect* df,
        /* [in] */ IRect* cf,
        /* [in] */ IRect* vf);

    /**
    * @return Whether music is being played right now.
    */
    CARAPI_(Boolean) IsMusicActive();

    /**
    * Tell the audio service to adjust the volume appropriate to the event.
    * @param keycode
    */
    CARAPI_(void) HandleVolumeKey(
        /* [in] */ Int32 stream,
        /* [in] */ Int32 keycode);

    CARAPI_(void) SendCloseSystemWindows();

    CARAPI_(void) SendCloseSystemWindows(
        /* [in] */ const char* reason);

    CARAPI_(void) UpdateRotation(
        /* [in] */ Int32 animFlags);

    /**
    * Return an Intent to launch the currently active dock as home.  Returns
    * NULL if the standard home should be launched.
    * @return
    */
    CARAPI_(void) CreateHomeDockIntent(
        /* [out] */ IIntent** intent);

    CARAPI_(void) StartDockOrHome();

    /**
    * goes to the home screen
    * @return whether it did anything
    */
    CARAPI_(Boolean) GoHome();

private:
    CARAPI_(void) InterceptPowerKeyDown(
        /* [in] */ Boolean handled);

    CARAPI_(Boolean) InterceptPowerKeyUp(
        /* [in] */ Boolean canceled);

    CARAPI_(Int32) ReadRotation(
        /* [in] */  Int32 resID);

    CARAPI_(Int32) DetermineHiddenState(
        /* [in] */ Boolean lidOpen,
        /* [in] */ Int32 mode,
        /* [in] */ Int32 hiddenValue,
        /* [in] */ Int32 visibleValue);

    CARAPI_(Boolean) KeyguardIsShowingTq();

    CARAPI_(Int32) GetCurrentLandscapeRotation(
        /* [in] */ Int32 lastRotation);

    CARAPI_(Boolean) IsLandscapeOrSeascape(
        /* [in] */ Int32 sensorRotation);

    CARAPI_(Int32) GetCurrentPortraitRotation(
        /* [in] */ Int32 lastRotation);

    CARAPI_(Boolean) IsAnyPortrait(
        /* [in] */ Int32 sensorRotation);

    CARAPI_(void) UpdateLockScreenTimeout();

protected:
    //class SettingsObserver : public ContentObserver
    //{
    //    SettingsObserver(Handler handler) {
    //        super(handler);
    //    }

    //    void observe() {
    //        ContentResolver resolver = mContext.getContentResolver();
    //        resolver.registerContentObserver(Settings.System.getUriFor(
    //            Settings.System.END_BUTTON_BEHAVIOR), false, this);
    //        resolver.registerContentObserver(Settings.Secure.getUriFor(
    //            Settings.Secure.INCALL_POWER_BUTTON_BEHAVIOR), false, this);
    //        resolver.registerContentObserver(Settings.System.getUriFor(
    //            Settings.System.ACCELEROMETER_ROTATION), false, this);
    //        resolver.registerContentObserver(Settings.System.getUriFor(
    //            Settings.System.SCREEN_OFF_TIMEOUT), false, this);
    //        resolver.registerContentObserver(Settings.System.getUriFor(
    //            Settings.System.POINTER_LOCATION), false, this);
    //        resolver.registerContentObserver(Settings.Secure.getUriFor(
    //            Settings.Secure.DEFAULT_INPUT_METHOD), false, this);
    //        resolver.registerContentObserver(Settings.System.getUriFor(
    //            "fancy_rotation_anim"), false, this);
    //        updateSettings();
    //    }

    //    @Override public void onChange(boolean selfChange) {
    //        updateSettings();
    //        try {
    //            mWindowManager.setRotation(USE_LAST_ROTATION, false,
    //                mFancyRotationAnimation);
    //        } catch (RemoteException e) {
    //            // Ignore
    //        }
    //    }
    //};

    //class MyOrientationListener extends WindowOrientationListener
    //{
    //    MyOrientationListener(Context context) {
    //        super(context);
    //    }

    //    @Override
    //        public void onOrientationChanged(int rotation) {
    //            // Send updates based on orientation value
    //            if (localLOGV) Log.v(TAG, "onOrientationChanged, rotation changed to " +rotation);
    //            try {
    //                mWindowManager.setRotation(rotation, false,
    //                    mFancyRotationAnimation);
    //            } catch (RemoteException e) {
    //                // Ignore

    //            }
    //    }
    //};


    //class PassHeadsetKey implements Runnable {
    //    KeyEvent mKeyEvent;

    //    PassHeadsetKey(KeyEvent keyEvent) {
    //        mKeyEvent = keyEvent;
    //    }

    //    public CARAPI_(void) run() {
    //        if (ActivityManagerNative.isSystemReady()) {
    //            Intent intent = new Intent(Intent.ACTION_MEDIA_BUTTON, NULL);
    //            intent.putExtra(Intent.EXTRA_KEY_EVENT, mKeyEvent);
    //            mContext.sendOrderedBroadcast(intent, NULL, mBroadcastDone,
    //                mHandler, Activity.RESULT_OK, NULL, NULL);
    //        }
    //    }
    //}

    //BroadcastReceiver mBroadcastDone = new BroadcastReceiver() {
    //    public CARAPI_(void) onReceive(Context context, Intent intent) {
    //        mBroadcastWakeLock.release();
    //    }
    //};

    //BroadcastReceiver mDockReceiver = new BroadcastReceiver() {
    //    public CARAPI_(void) onReceive(Context context, Intent intent) {
    //        if (Intent.ACTION_DOCK_EVENT.equals(intent.getAction())) {
    //            mDockMode = intent.getIntExtra(Intent.EXTRA_DOCK_STATE,
    //                Intent.EXTRA_DOCK_STATE_UNDOCKED);
    //        } else {
    //            try {
    //                IUiModeManager uiModeService = IUiModeManager.Stub.asInterface(
    //                    ServiceManager.getService(Context.UI_MODE_SERVICE));
    //                mUiMode = uiModeService.getCurrentModeType();
    //            } catch (RemoteException e) {
    //            }
    //        }
    //        updateRotation(Surface.FLAGS_ORIENTATION_ANIMATION_DISABLE);
    //        updateOrientationListenerLp();
    //    }
    //};

    //Runnable mScreenLockTimeout = new Runnable() {
    //    public CARAPI_(void) run() {
    //        synchronized (this) {
    //            if (localLOGV) Log.v(TAG, "mScreenLockTimeout activating keyguard");
    //            mKeyguardMediator.doKeyguardTimeout();
    //            mLockScreenTimerActive = FALSE;
    //        }
    //    }
    //};

private:
    Mutex mLock;

    AutoPtr<IContext> mContext;
    AutoPtr<IWindowManager> mWindowManager;
    //LocalPowerManager mPowerManager;
    //Vibrator mVibrator; // Vibrator for giving feedback of orientation changes

    // Vibrator pattern for haptic feedback of a Int64 press.
    ArrayOf<Int64>* mLongPressVibePattern;

    // Vibrator pattern for haptic feedback of virtual key press.
    ArrayOf<Int64>* mVirtualKeyVibePattern;

    // Vibrator pattern for a short vibration.
    ArrayOf<Int64>* mKeyboardTapVibePattern;

    // Vibrator pattern for haptic feedback during boot when safe mode is disabled.
    ArrayOf<Int64>* mSafeModeDisabledVibePattern;

    // Vibrator pattern for haptic feedback during boot when safe mode is enabled.
    ArrayOf<Int64>* mSafeModeEnabledVibePattern;

    /** If TRUE, hitting shift & menu will broadcast Intent.ACTION_BUG_REPORT */
    Boolean mEnableShiftMenuBugReports;

    Boolean mSafeMode;
    AutoPtr<IWindowState> mStatusBar;
    AutoPtr<IWindowState> mNavigationBar;
    List<AutoPtr<IWindowState> > mStatusBarPanels;
    AutoPtr<IWindowState> mKeyguard;
    //KeyguardViewMediator mKeyguardMediator;
    //GlobalActions mGlobalActions;
    volatile Boolean mPowerKeyHandled;
    //RecentApplicationsDialog mRecentAppsDialog;
    //Handler mHandler;

    Boolean mSystemReady;
    Boolean mLidOpen;
    Int32 mUiMode;
    Int32 mDockMode;
    Int32 mLidOpenRotation;
    Int32 mCarDockRotation;
    Int32 mDeskDockRotation;
    Boolean mCarDockEnablesAccelerometer;
    Boolean mDeskDockEnablesAccelerometer;
    Int32 mLidKeyboardAccessibility;
    Int32 mLidNavigationAccessibility;
    Boolean mScreenOn;
    Boolean mOrientationSensorEnabled;
    Int32 mCurrentAppOrientation;
    Int32 mAccelerometerDefault;
    Boolean mHasSoftInput;

    Int32 mPointerLocationMode;
    //PointerLocationView mPointerLocationView = NULL;
    AutoPtr<IInputChannel> mPointerLocationInputChannel;

    //private final InputHandler mPointerLocationInputHandler = new BaseInputHandler() {
    //    @Override
    //        public CARAPI_(void) handleMotion(MotionEvent event, Runnable finishedCallback) {
    //            finishedCallback.run();

    //            synchronized (mLock) {
    //                if (mPointerLocationView != NULL) {
    //                    mPointerLocationView.addTouchEvent(event);
    //                }
    //            }
    //    }
    //};

    // The current size of the screen.
    Int32 mW;
    Int32 mH;

    // During layout, the current screen borders with all outer decoration
    // (status bar, input method dock) accounted for.
    Int32 mCurLeft;
    Int32 mCurTop;
    Int32 mCurRight;
    Int32 mCurBottom;

    // During layout, the frame in which content should be displayed
    // to the user, accounting for all screen decoration except for any
    // space they deem as available for other content.  This is usually
    // the same as mCur*, but may be larger if the screen decor has supplied
    // content insets.
    Int32 mContentLeft;
    Int32 mContentTop;
    Int32 mContentRight;
    Int32 mContentBottom;

    // During layout, the current screen borders aInt64 with input method
    // windows are placed.
    Int32 mDockLeft;
    Int32 mDockTop;
    Int32 mDockRight;
    Int32 mDockBottom;

    // During layout, the layer at which the doc window is placed.
    Int32 mDockLayer;

    static AutoPtr<CRect> mTmpParentFrame;
    static AutoPtr<CRect> mTmpDisplayFrame;
    static AutoPtr<CRect> mTmpContentFrame;
    static AutoPtr<CRect> mTmpVisibleFrame;
    static AutoPtr<CRect> mTmpNavigationFrame;

    AutoPtr<IWindowState> mTopFullscreenOpaqueWindowState;
    Boolean mForceStatusBar;
    Boolean mHideLockScreen;
    Boolean mDismissKeyguard;
    Boolean mHomePressed;
    AutoPtr<IIntent> mHomeIntent;
    AutoPtr<IIntent> mCarDockIntent;
    AutoPtr<IIntent> mDeskDockIntent;
    Boolean mSearchKeyPressed;
    Boolean mConsumeSearchKeyUp;

    // support for activating the lock screen while the screen is on
    Boolean mAllowLockscreenWhenOn;
    Int32 mLockScreenTimeout;
    Boolean mLockScreenTimerActive;

    // Behavior of ENDCALL Button.  (See Settings.System.END_BUTTON_BEHAVIOR.)
    Int32 mEndcallBehavior;

    // Behavior of POWER button while in-call and screen on.
    // (See Settings.Secure.INCALL_POWER_BUTTON_BEHAVIOR.)
    Int32 mIncallPowerBehavior;

    Int32 mLandscapeRotation; // default landscape rotation
    Int32 mSeascapeRotation; // "other" landscape rotation, 180 degrees from mLandscapeRotation
    Int32 mPortraitRotation; // default portrait rotation
    Int32 mUpsideDownRotation; // "other" portrait rotation

    // Nothing to see here, move aInt64...
    Int32 mFancyRotationAnimation;

    //ShortcutManager mShortcutManager;
    //PowerManager.WakeLock mBroadcastWakeLock;

    //MyOrientationListener mOrientationListener;

    //private final Runnable mPowerLongPress = new Runnable() {
    //    public CARAPI_(void) run() {
    //        if (!mPowerKeyHandled) {
    //            mPowerKeyHandled = TRUE;
    //            performHapticFeedbackLw(null, HapticFeedbackConstants.LONG_PRESS, FALSE);
    //            sendCloseSystemWindows(SYSTEM_DIALOG_REASON_GLOBAL_ACTIONS);
    //            showGlobalActionsDialog();
    //        }
    //    }
    //};

    /**
    * When a home-key longpress expires, close other system windows and launch the recent apps
    */
    //Runnable mHomeLongPress = new Runnable() {
    //    public CARAPI_(void) run() {
    //        /*
    //        * Eat the longpress so it won't dismiss the recent apps dialog when
    //        * the user lets go of the home key
    //        */
    //        mHomePressed = FALSE;
    //        performHapticFeedbackLw(NULL, HapticFeedbackConstants.LONG_PRESS, FALSE);
    //        sendCloseSystemWindows(SYSTEM_DIALOG_REASON_RECENT_APPS);
    //        showRecentAppsDialog();
    //    }
    //};
};

#endif // __CPHONEWINDOWMANAGER_H__
