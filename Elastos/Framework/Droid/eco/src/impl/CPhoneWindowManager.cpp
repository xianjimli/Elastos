
#include "impl/CPhoneWindowManager.h"
#include "capsule/CActivityInfo.h"
#include "content/CIntent.h"
#include "view/CWindowManagerLayoutParams.h"
#include "view/CWindowManagerImpl.h"
#include "view/WindowManagerPolicy.h"
#include "view/HapticFeedbackConstants.h"
#include "impl/CPolicyManager.h"
#include <Logger.h>
#include <StringBuffer.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

const char* CPhoneWindowManager::SYSTEM_DIALOG_REASON_KEY = "reason";
const char* CPhoneWindowManager::SYSTEM_DIALOG_REASON_GLOBAL_ACTIONS = "globalactions";
const char* CPhoneWindowManager::SYSTEM_DIALOG_REASON_RECENT_APPS = "recentapps";
const char* CPhoneWindowManager::SYSTEM_DIALOG_REASON_HOME_KEY = "homekey";
const char* CPhoneWindowManager::TAG = "WindowManager";
const Boolean CPhoneWindowManager::DEBUG;
const Boolean CPhoneWindowManager::localLOGV;
const Boolean CPhoneWindowManager::DEBUG_LAYOUT;
const Boolean CPhoneWindowManager::SHOW_STARTING_ANIMATIONS;
const Boolean CPhoneWindowManager::SHOW_PROCESSES_ON_ALT_MENU;
const Int32 CPhoneWindowManager::WALLPAPER_LAYER;
const Int32 CPhoneWindowManager::APPLICATION_LAYER;
const Int32 CPhoneWindowManager::PHONE_LAYER;
const Int32 CPhoneWindowManager::SEARCH_BAR_LAYER;
const Int32 CPhoneWindowManager::STATUS_BAR_PANEL_LAYER;
const Int32 CPhoneWindowManager::SYSTEM_DIALOG_LAYER;
const Int32 CPhoneWindowManager::TOAST_LAYER;
const Int32 CPhoneWindowManager::STATUS_BAR_LAYER;
const Int32 CPhoneWindowManager::PRIORITY_PHONE_LAYER;
const Int32 CPhoneWindowManager::SYSTEM_ALERT_LAYER;
const Int32 CPhoneWindowManager::SYSTEM_ERROR_LAYER;
const Int32 CPhoneWindowManager::INPUT_METHOD_LAYER;
const Int32 CPhoneWindowManager::INPUT_METHOD_DIALOG_LAYER;
const Int32 CPhoneWindowManager::KEYGUARD_LAYER;
const Int32 CPhoneWindowManager::KEYGUARD_DIALOG_LAYER;
const Int32 CPhoneWindowManager::SYSTEM_OVERLAY_LAYER;
const Int32 CPhoneWindowManager::SECURE_SYSTEM_OVERLAY_LAYER;
const Int32 CPhoneWindowManager::APPLICATION_MEDIA_SUBLAYER;
const Int32 CPhoneWindowManager::APPLICATION_MEDIA_OVERLAY_SUBLAYER;
const Int32 CPhoneWindowManager::APPLICATION_PANEL_SUBLAYER;
const Int32 CPhoneWindowManager::APPLICATION_SUB_PANEL_SUBLAYER;
const Boolean CPhoneWindowManager::KEYBOARD_ALWAYS_HIDDEN;
const Int32 CPhoneWindowManager::SW_LID;
const Int32 CPhoneWindowManager::BTN_MOUSE;
const Int32 CPhoneWindowManager::DEFAULT_ACCELEROMETER_ROTATION;
const Boolean CPhoneWindowManager::PRINT_ANIM;
const Int32 CPhoneWindowManager::WINDOW_TYPES_WHERE_HOME_DOESNT_WORK[] = {
    WindowManagerLayoutParams_TYPE_SYSTEM_ALERT,
    WindowManagerLayoutParams_TYPE_SYSTEM_ERROR,
};

AutoPtr<CRect> CPhoneWindowManager::mTmpParentFrame;
AutoPtr<CRect> CPhoneWindowManager::mTmpDisplayFrame;
AutoPtr<CRect> CPhoneWindowManager::mTmpContentFrame;
AutoPtr<CRect> CPhoneWindowManager::mTmpVisibleFrame;

CPhoneWindowManager::CPhoneWindowManager() :
    mLongPressVibePattern(NULL),
    mVirtualKeyVibePattern(NULL),
    mKeyboardTapVibePattern(NULL),
    mSafeModeDisabledVibePattern(NULL),
    mSafeModeEnabledVibePattern(NULL),
    mEnableShiftMenuBugReports(FALSE),
    mSafeMode(FALSE),
    mPowerKeyHandled(FALSE),
    mSystemReady(FALSE),
    mLidOpen(FALSE),
    mUiMode(Configuration_UI_MODE_TYPE_NORMAL),
    mDockMode(Intent_EXTRA_DOCK_STATE_UNDOCKED),
    mCarDockEnablesAccelerometer(FALSE),
    mDeskDockEnablesAccelerometer(FALSE),
    mScreenOn(FALSE),
    mOrientationSensorEnabled(FALSE),
    mCurrentAppOrientation(CActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED),
    mAccelerometerDefault(DEFAULT_ACCELEROMETER_ROTATION),
    mHasSoftInput(FALSE),
    mPointerLocationMode(0),
    mForceStatusBar(FALSE),
    mHideLockScreen(FALSE),
    mDismissKeyguard(FALSE),
    mHomePressed(FALSE),
    mSearchKeyPressed(FALSE),
    mConsumeSearchKeyUp(FALSE),
    mAllowLockscreenWhenOn(FALSE),
    mLockScreenTimerActive(FALSE),
    mLandscapeRotation(-1),
    mSeascapeRotation(-1),
    mPortraitRotation(-1),
    mUpsideDownRotation(-1)
{
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&mTmpParentFrame)));
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&mTmpDisplayFrame)));
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&mTmpContentFrame)));
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&mTmpVisibleFrame)));
}

Boolean CPhoneWindowManager::UseSensorForOrientationLp(
    /* [in] */ Int32 appOrientation)
{
    // The app says use the sensor.
    if (appOrientation == CActivityInfo::SCREEN_ORIENTATION_SENSOR
        || appOrientation == CActivityInfo::SCREEN_ORIENTATION_FULL_SENSOR
        || appOrientation == CActivityInfo::SCREEN_ORIENTATION_SENSOR_LANDSCAPE
        || appOrientation == CActivityInfo::SCREEN_ORIENTATION_SENSOR_PORTRAIT) {
            return TRUE;
    }

    // The user preference says we can rotate, and the app is willing to rotate.
    if (mAccelerometerDefault != 0 &&
        (appOrientation == CActivityInfo::SCREEN_ORIENTATION_USER
        || appOrientation == CActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED)) {
            return TRUE;
    }

    // We're in a dock that has a rotation affinity, and the app is willing to rotate.
    if ((mCarDockEnablesAccelerometer && mDockMode == Intent_EXTRA_DOCK_STATE_CAR)
        || (mDeskDockEnablesAccelerometer && mDockMode == Intent_EXTRA_DOCK_STATE_DESK)) {
            // Note we override the nosensor flag here.
            if (appOrientation == CActivityInfo::SCREEN_ORIENTATION_USER
                || appOrientation == CActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED
                || appOrientation == CActivityInfo::SCREEN_ORIENTATION_NOSENSOR) {
                    return TRUE;
            }
    }

    // Else, don't use the sensor.
    return FALSE;
}

/*
* We always let the sensor be switched on by default except when
* the user has explicitly disabled sensor based rotation or when the
* screen is switched off.
*/
Boolean CPhoneWindowManager::NeedSensorRunningLp()
{
    if (mCurrentAppOrientation == CActivityInfo::SCREEN_ORIENTATION_SENSOR
        || mCurrentAppOrientation == CActivityInfo::SCREEN_ORIENTATION_FULL_SENSOR
        || mCurrentAppOrientation == CActivityInfo::SCREEN_ORIENTATION_SENSOR_PORTRAIT
        || mCurrentAppOrientation == CActivityInfo::SCREEN_ORIENTATION_SENSOR_LANDSCAPE) {
            // If the application has explicitly requested to follow the
            // orientation, then we need to turn the sensor or.
            return TRUE;
    }

    if ((mCarDockEnablesAccelerometer && mDockMode == Intent_EXTRA_DOCK_STATE_CAR) ||
        (mDeskDockEnablesAccelerometer && mDockMode == Intent_EXTRA_DOCK_STATE_DESK)) {
            // enable accelerometer if we are docked in a dock that enables accelerometer
            // orientation management,
            return TRUE;
    }

    if (mAccelerometerDefault == 0) {
        // If the setting for using the sensor by default is enabled, then
        // we will always leave it on.  Note that the user could go to
        // a window that forces an orientation that does not use the
        // sensor and in theory we could turn it off... however, when next
        // turning it on we won't have a good value for the current
        // orientation for a little bit, which can cause orientation
        // changes to lag, so we'd like to keep it always on.  (It will
        // still be turned off when the screen is off.)
        return FALSE;
    }

    return TRUE;
}

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
void CPhoneWindowManager::UpdateOrientationListenerLp()
{
    //if (!mOrientationListener.canDetectOrientation()) {
    //    // If sensor is turned off or nonexistent for some reason
    //    return;
    //}
    ////Could have been invoked due to screen turning on or off or
    ////change of the currently visible window's orientation
    //if (localLOGV) Log.v(TAG, "Screen status="+mScreenOn+
    //    ", current orientation="+mCurrentAppOrientation+
    //    ", SensorEnabled="+mOrientationSensorEnabled);
    //Boolean disable = TRUE;
    //if (mScreenOn) {
    //    if (needSensorRunningLp()) {
    //        disable = FALSE;
    //        //enable listener if not already enabled
    //        if (!mOrientationSensorEnabled) {
    //            mOrientationListener.enable();
    //            if(localLOGV) Log.v(TAG, "Enabling listeners");
    //            mOrientationSensorEnabled = TRUE;
    //        }
    //    }
    //}
    ////check if sensors need to be disabled
    //if (disable && mOrientationSensorEnabled) {
    //    mOrientationListener.disable();
    //    if(localLOGV) Log.v(TAG, "Disabling listeners");
    //    mOrientationSensorEnabled = FALSE;
    //}
}

void CPhoneWindowManager::InterceptPowerKeyDown(
    /* [in] */ Boolean handled)
{
    mPowerKeyHandled = handled;
    if (!handled) {
        //mHandler.postDelayed(mPowerLongPress, ViewConfiguration.getGlobalActionKeyTimeout());
    }
}

Boolean CPhoneWindowManager::InterceptPowerKeyUp(
    /* [in] */ Boolean canceled)
{
    if (!mPowerKeyHandled) {
        //mHandler.removeCallbacks(mPowerLongPress);
        return !canceled;
    } else {
        mPowerKeyHandled = TRUE;
        return FALSE;
    }
}

void CPhoneWindowManager::ShowGlobalActionsDialog()
{
    //if (mGlobalActions == NULL) {
    //    mGlobalActions = new GlobalActions(mContext);
    //}
    //final Boolean keyguardShowing = mKeyguardMediator.isShowingAndNotHidden();
    //mGlobalActions.showDialog(keyguardShowing, isDeviceProvisioned());
    //if (keyguardShowing) {
    //    // since it took two seconds of Int64 press to bring this up,
    //    // poke the wake lock so they have some time to see the dialog.
    //    mKeyguardMediator.pokeWakelock();
    //}
}

Boolean CPhoneWindowManager::IsDeviceProvisioned()
{
    //return Settings.Secure.getInt(
    //    mContext.getContentResolver(), Settings.Secure.DEVICE_PROVISIONED, 0) != 0;
    return TRUE;
}

/**
* Create (if necessary) and launch the recent apps dialog
*/
void CPhoneWindowManager::ShowRecentAppsDialog()
{
    //if (mRecentAppsDialog == NULL) {
    //    mRecentAppsDialog = new RecentApplicationsDialog(mContext);
    //}
    //mRecentAppsDialog.show();
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::Init(
    /* [in] */ IContext* context,
    /* [in] */ IWindowManagerEx* windowManager
    /* [in] */ /*LocalPowerManager powerManager*/)
{
    mContext = context;
    mWindowManager = windowManager;
    //mPowerManager = powerManager;
    //mKeyguardMediator = new KeyguardViewMediator(context, this, powerManager);
    //mHandler = new Handler();
    //mOrientationListener = new MyOrientationListener(mContext);
    //SettingsObserver settingsObserver = new SettingsObserver(mHandler);
    //settingsObserver.observe();
    //mShortcutManager = new ShortcutManager(context, mHandler);
    //mShortcutManager.observe();
    ECode ec = CIntent::New(
        String(Intent_ACTION_MAIN), NULL, (IIntent**)&mHomeIntent);
    if (FAILED(ec)) {
        return ec;
    }

    mHomeIntent->AddCategory(String(Intent_CATEGORY_HOME));
    mHomeIntent->AddFlags(Intent_FLAG_ACTIVITY_NEW_TASK
        | Intent_FLAG_ACTIVITY_RESET_TASK_IF_NEEDED);

    ec = CIntent::New(
        String(Intent_ACTION_MAIN), NULL, (IIntent**)&mCarDockIntent);
    if (FAILED(ec)) {
        return ec;
    }

    mCarDockIntent->AddCategory(String(Intent_CATEGORY_CAR_DOCK));
    mCarDockIntent->AddFlags(Intent_FLAG_ACTIVITY_NEW_TASK
        | Intent_FLAG_ACTIVITY_RESET_TASK_IF_NEEDED);

    ec = CIntent::New(
        String(Intent_ACTION_MAIN), NULL, (IIntent**)&mDeskDockIntent);
    if (FAILED(ec)) {
        return ec;
    }

    mDeskDockIntent->AddCategory(String(Intent_CATEGORY_DESK_DOCK));
    mDeskDockIntent->AddFlags(Intent_FLAG_ACTIVITY_NEW_TASK
        | Intent_FLAG_ACTIVITY_RESET_TASK_IF_NEEDED);

    //PowerManager pm = (PowerManager)context->GetSystemService(Context_POWER_SERVICE);
    //mBroadcastWakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK,
    //    "PhoneWindowManager.mBroadcastWakeLock");
    //mEnableShiftMenuBugReports = "1".equals(SystemProperties.get("ro.debuggable"));

    //mLidOpenRotation = readRotation(
    //    com.android.internal.R.integer.config_lidOpenRotation);
    //mCarDockRotation = readRotation(
    //    com.android.internal.R.integer.config_carDockRotation);
    //mDeskDockRotation = readRotation(
    //    com.android.internal.R.integer.config_deskDockRotation);
    //mCarDockEnablesAccelerometer = mContext.getResources().getBoolean(
    //    com.android.internal.R.bool.config_carDockEnablesAccelerometer);
    //mDeskDockEnablesAccelerometer = mContext.getResources().getBoolean(
    //    com.android.internal.R.bool.config_deskDockEnablesAccelerometer);
    //mLidKeyboardAccessibility = mContext.getResources().getInteger(
    //    com.android.internal.R.integer.config_lidKeyboardAccessibility);
    //mLidNavigationAccessibility = mContext.getResources().getInteger(
    //    com.android.internal.R.integer.config_lidNavigationAccessibility);

    //// register for dock events
    //IntentFilter filter = new IntentFilter();
    //filter.addAction(UiModeManager.ACTION_ENTER_CAR_MODE);
    //filter.addAction(UiModeManager.ACTION_EXIT_CAR_MODE);
    //filter.addAction(UiModeManager.ACTION_ENTER_DESK_MODE);
    //filter.addAction(UiModeManager.ACTION_EXIT_DESK_MODE);
    //filter.addAction(Intent.ACTION_DOCK_EVENT);
    //Intent intent = context.registerReceiver(mDockReceiver, filter);
    //if (intent != NULL) {
    //    // Retrieve current sticky dock event broadcast.
    //    mDockMode = intent.getIntExtra(Intent.EXTRA_DOCK_STATE,
    //        Intent.EXTRA_DOCK_STATE_UNDOCKED);
    //}
    //mVibrator = new Vibrator();
    //mLongPressVibePattern = getLongIntArray(mContext.getResources(),
    //    com.android.internal.R.array.config_longPressVibePattern);
    //mVirtualKeyVibePattern = getLongIntArray(mContext.getResources(),
    //    com.android.internal.R.array.config_virtualKeyVibePattern);
    //mKeyboardTapVibePattern = getLongIntArray(mContext.getResources(),
    //    com.android.internal.R.array.config_keyboardTapVibePattern);
    //mSafeModeDisabledVibePattern = getLongIntArray(mContext.getResources(),
    //    com.android.internal.R.array.config_safeModeDisabledVibePattern);
    //mSafeModeEnabledVibePattern = getLongIntArray(mContext.getResources(),
    //    com.android.internal.R.array.config_safeModeEnabledVibePattern);

    return NOERROR;
}

void CPhoneWindowManager::UpdateSettings()
{
    //ContentResolver resolver = mContext.getContentResolver();
    //Boolean updateRotation = FALSE;
    //View addView = NULL;
    //View removeView = NULL;
    //synchronized (mLock) {
    //    mEndcallBehavior = Settings.System.getInt(resolver,
    //        Settings.System.END_BUTTON_BEHAVIOR,
    //        Settings.System.END_BUTTON_BEHAVIOR_DEFAULT);
    //    mIncallPowerBehavior = Settings.Secure.getInt(resolver,
    //        Settings.Secure.INCALL_POWER_BUTTON_BEHAVIOR,
    //        Settings.Secure.INCALL_POWER_BUTTON_BEHAVIOR_DEFAULT);
    //    mFancyRotationAnimation = Settings.System.getInt(resolver,
    //        "fancy_rotation_anim", 0) != 0 ? 0x80 : 0;
    //    Int32 accelerometerDefault = Settings.System.getInt(resolver,
    //        Settings.System.ACCELEROMETER_ROTATION, DEFAULT_ACCELEROMETER_ROTATION);
    //    if (mAccelerometerDefault != accelerometerDefault) {
    //        mAccelerometerDefault = accelerometerDefault;
    //        updateOrientationListenerLp();
    //    }
    //    if (mSystemReady) {
    //        Int32 pointerLocation = Settings.System.getInt(resolver,
    //            Settings.System.POINTER_LOCATION, 0);
    //        if (mPointerLocationMode != pointerLocation) {
    //            mPointerLocationMode = pointerLocation;
    //            if (pointerLocation != 0) {
    //                if (mPointerLocationView == NULL) {
    //                    mPointerLocationView = new PointerLocationView(mContext);
    //                    mPointerLocationView.setPrintCoords(FALSE);
    //                    addView = mPointerLocationView;
    //                }
    //            } else {
    //                removeView = mPointerLocationView;
    //                mPointerLocationView = NULL;
    //            }
    //        }
    //    }
    //    // use screen off timeout setting as the timeout for the lockscreen
    //    mLockScreenTimeout = Settings.System.getInt(resolver,
    //        Settings.System.SCREEN_OFF_TIMEOUT, 0);
    //    String imId = Settings.Secure.getString(resolver,
    //        Settings.Secure.DEFAULT_INPUT_METHOD);
    //    Boolean hasSoftInput = imId != NULL && imId.length() > 0;
    //    if (mHasSoftInput != hasSoftInput) {
    //        mHasSoftInput = hasSoftInput;
    //        updateRotation = TRUE;
    //    }
    //}
    //if (updateRotation) {
    //    updateRotation(0);
    //}
    //if (addView != NULL) {
    //    WindowManager.LayoutParams lp = new WindowManager.LayoutParams(
    //        ViewGroupLayoutParams_MATCH_PARENT,
    //        ViewGroupLayoutParams_MATCH_PARENT);
    //    lp.type = WindowManagerLayoutParams_TYPE_SECURE_SYSTEM_OVERLAY;
    //    lp.flags =
    //        WindowManagerLayoutParams_FLAG_NOT_TOUCHABLE|
    //        WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE|
    //        WindowManagerLayoutParams_FLAG_LAYOUT_IN_SCREEN;
    //    lp.format = PixelFormat.TRANSLUCENT;
    //    lp.setTitle("PointerLocation");
    //    WindowManagerImpl wm = (WindowManagerImpl)
    //        mContext.getSystemService(Context.WINDOW_SERVICE);
    //    wm.addView(addView, lp);

    //    if (mPointerLocationInputChannel == NULL) {
    //        try {
    //            mPointerLocationInputChannel =
    //                mWindowManager.monitorInput("PointerLocationView");
    //            InputQueue.registerInputChannel(mPointerLocationInputChannel,
    //                mPointerLocationInputHandler, mHandler.getLooper().getQueue());
    //        } catch (RemoteException ex) {
    //            Slog.e(TAG, "Could not set up input monitoring channel for PointerLocation.",
    //                ex);
    //        }
    //    }
    //}
    //if (removeView != NULL) {
    //    if (mPointerLocationInputChannel != NULL) {
    //        InputQueue.unregisterInputChannel(mPointerLocationInputChannel);
    //        mPointerLocationInputChannel.dispose();
    //        mPointerLocationInputChannel = NULL;
    //    }

    //    WindowManagerImpl wm = (WindowManagerImpl)
    //        mContext.getSystemService(Context.WINDOW_SERVICE);
    //    wm.removeView(removeView);
    //}
}

Int32 CPhoneWindowManager::ReadRotation(
    /* [in] */  Int32 resID)
{
    //try {
    //    Int32 rotation = mContext.getResources().getInteger(resID);
    //    switch (rotation) {
    //            case 0:
    //                return Surface.ROTATION_0;
    //            case 90:
    //                return Surface.ROTATION_90;
    //            case 180:
    //                return Surface.ROTATION_180;
    //            case 270:
    //                return Surface.ROTATION_270;
    //    }
    //} catch (Resources.NotFoundException e) {
    //    // fall through
    //}
    return -1;
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::CheckAddPermission(
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [out] */ Int32* addPermisssion)
{
    if (addPermisssion == NULL) {
        return E_INVALID_ARGUMENT;
    }

    Int32 type = ((CWindowManagerLayoutParams*)attrs)->mType;

    if (type < WindowManagerLayoutParams_FIRST_SYSTEM_WINDOW
        || type > WindowManagerLayoutParams_LAST_SYSTEM_WINDOW) {
        *addPermisssion = CWindowManagerImpl::ADD_OKAY;
        return NOERROR;
    }

    String permission;
    switch (type) {
        case WindowManagerLayoutParams_TYPE_TOAST:
            // XXX right now the app process has complete control over
            // this...  should introduce a token to let the system
            // monitor/control what they are doing.
            break;
        case WindowManagerLayoutParams_TYPE_INPUT_METHOD:
        case WindowManagerLayoutParams_TYPE_WALLPAPER:
            // The window manager will check these.
            break;
        case WindowManagerLayoutParams_TYPE_PHONE:
        case WindowManagerLayoutParams_TYPE_PRIORITY_PHONE:
        case WindowManagerLayoutParams_TYPE_SYSTEM_ALERT:
        case WindowManagerLayoutParams_TYPE_SYSTEM_ERROR:
        case WindowManagerLayoutParams_TYPE_SYSTEM_OVERLAY:
            //permission = android.Manifest.permission.SYSTEM_ALERT_WINDOW;
            break;
        default:
            //permission = android.Manifest.permission.INTERNAL_SYSTEM_WINDOW;
            break;
    }

    if (permission.IsNullOrEmpty()) {
        //if (mContext->CheckCallingOrSelfPermission(permission)
        //    != PackageManager.PERMISSION_GRANTED) {
        //        return WindowManagerImpl.ADD_PERMISSION_DENIED;
        //}
    }

    *addPermisssion = CWindowManagerImpl::ADD_OKAY;

    return NOERROR;
}

ECode CPhoneWindowManager::AdjustWindowParamsLw(
    /* [in] */ IWindowManagerLayoutParams* attrs)
{
    switch (((CWindowManagerLayoutParams*)attrs)->mType) {
        case WindowManagerLayoutParams_TYPE_SYSTEM_OVERLAY:
        case WindowManagerLayoutParams_TYPE_SECURE_SYSTEM_OVERLAY:
        case WindowManagerLayoutParams_TYPE_TOAST:
            // These types of windows can't receive input events.
            ((CWindowManagerLayoutParams*)attrs)->mFlags |=
                WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE |
                WindowManagerLayoutParams_FLAG_NOT_TOUCHABLE;
            break;
    }

    return NOERROR;
}

void CPhoneWindowManager::ReadLidState()
{
    Int32 sw = 0;
    mWindowManager->GetSwitchState(SW_LID, &sw);

    if (sw >= 0) {
        mLidOpen = sw == 0;
    }
}

Int32 CPhoneWindowManager::DetermineHiddenState(
    /* [in] */ Boolean lidOpen,
    /* [in] */ Int32 mode,
    /* [in] */ Int32 hiddenValue,
    /* [in] */ Int32 visibleValue)
{
    switch (mode) {
        case 1:
            return lidOpen ? visibleValue : hiddenValue;
        case 2:
            return lidOpen ? hiddenValue : visibleValue;
    }

    return visibleValue;
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::AdjustConfigurationLw(
    /* [in] */ IConfiguration* config)
{
    assert(config);
    CConfiguration* cConfig = (CConfiguration*)config;

    ReadLidState();
    const Boolean lidOpen = !KEYBOARD_ALWAYS_HIDDEN && mLidOpen;
    //mPowerManager.setKeyboardVisibility(lidOpen);

    cConfig->mHardKeyboardHidden = DetermineHiddenState(lidOpen,
        mLidKeyboardAccessibility, Configuration_HARDKEYBOARDHIDDEN_YES,
        Configuration_HARDKEYBOARDHIDDEN_NO);

    cConfig->mNavigationHidden = DetermineHiddenState(lidOpen,
        mLidNavigationAccessibility, Configuration_NAVIGATIONHIDDEN_YES,
        Configuration_NAVIGATIONHIDDEN_NO);

    cConfig->mKeyboardHidden = (cConfig->mHardKeyboardHidden
        == Configuration_HARDKEYBOARDHIDDEN_NO || mHasSoftInput)
        ? Configuration_KEYBOARDHIDDEN_NO
        : Configuration_KEYBOARDHIDDEN_YES;

    return NOERROR;
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::WindowTypeToLayerLw(
    /* [in] */ Int32 type,
    /* [out] */ Int32* layer)
{
    if (layer == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (type >= WindowManagerLayoutParams_FIRST_APPLICATION_WINDOW
        && type <= WindowManagerLayoutParams_LAST_APPLICATION_WINDOW) {
        *layer = APPLICATION_LAYER;
        return NOERROR;
    }

    switch (type) {
        case WindowManagerLayoutParams_TYPE_STATUS_BAR:
            *layer = STATUS_BAR_LAYER;
            break;
        case WindowManagerLayoutParams_TYPE_STATUS_BAR_PANEL:
            *layer = STATUS_BAR_PANEL_LAYER;
            break;
        case WindowManagerLayoutParams_TYPE_SYSTEM_DIALOG:
            *layer = SYSTEM_DIALOG_LAYER;
            break;
        case WindowManagerLayoutParams_TYPE_SEARCH_BAR:
            *layer = SEARCH_BAR_LAYER;
            break;
        case WindowManagerLayoutParams_TYPE_PHONE:
            *layer = PHONE_LAYER;
            break;
        case WindowManagerLayoutParams_TYPE_KEYGUARD:
            *layer = KEYGUARD_LAYER;
            break;
        case WindowManagerLayoutParams_TYPE_KEYGUARD_DIALOG:
            *layer = KEYGUARD_DIALOG_LAYER;
            break;
        case WindowManagerLayoutParams_TYPE_SYSTEM_ALERT:
            *layer = SYSTEM_ALERT_LAYER;
            break;
        case WindowManagerLayoutParams_TYPE_SYSTEM_ERROR:
            *layer = SYSTEM_ERROR_LAYER;
            break;
        case WindowManagerLayoutParams_TYPE_INPUT_METHOD:
            *layer = INPUT_METHOD_LAYER;
            break;
        case WindowManagerLayoutParams_TYPE_INPUT_METHOD_DIALOG:
            *layer = INPUT_METHOD_DIALOG_LAYER;
            break;
        case WindowManagerLayoutParams_TYPE_SYSTEM_OVERLAY:
            *layer = SYSTEM_OVERLAY_LAYER;
            break;
        case WindowManagerLayoutParams_TYPE_SECURE_SYSTEM_OVERLAY:
            *layer = SECURE_SYSTEM_OVERLAY_LAYER;
            break;
        case WindowManagerLayoutParams_TYPE_PRIORITY_PHONE:
            *layer = PRIORITY_PHONE_LAYER;
            break;
        case WindowManagerLayoutParams_TYPE_TOAST:
            *layer = TOAST_LAYER;
            break;
        case WindowManagerLayoutParams_TYPE_WALLPAPER:
            *layer = WALLPAPER_LAYER;
            break;
        default:
            Logger::E(TAG, StringBuffer("Unknown window type: ") + type);
            *layer = APPLICATION_LAYER;
    }

    return NOERROR;
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::SubWindowTypeToLayerLw(
    /* [in] */ Int32 type,
    /* [out] */ Int32* layer)
{
    if (layer == NULL) {
        return E_INVALID_ARGUMENT;
    }

    switch (type) {
        case WindowManagerLayoutParams_TYPE_APPLICATION_PANEL:
        case WindowManagerLayoutParams_TYPE_APPLICATION_ATTACHED_DIALOG:
            *layer = APPLICATION_PANEL_SUBLAYER;
            break;
        case WindowManagerLayoutParams_TYPE_APPLICATION_MEDIA:
            *layer = APPLICATION_MEDIA_SUBLAYER;
            break;
        case WindowManagerLayoutParams_TYPE_APPLICATION_MEDIA_OVERLAY:
            *layer = APPLICATION_MEDIA_OVERLAY_SUBLAYER;
            break;
        case WindowManagerLayoutParams_TYPE_APPLICATION_SUB_PANEL:
            *layer = APPLICATION_SUB_PANEL_SUBLAYER;
            break;
        default:
            Logger::D(TAG, StringBuffer("Unknown sub-window type: ") + type);
            *layer = 0;
    }

    return NOERROR;
}

ECode CPhoneWindowManager::GetMaxWallpaperLayer(
    /* [out] */ Int32* layer)
{
    if (!layer) {
        return E_INVALID_ARGUMENT;
    }

    *layer = STATUS_BAR_LAYER;

    return NOERROR;
}

ECode CPhoneWindowManager::DoesForceHide(
    /* [in] */ IWindowState* win,
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [out] */ Boolean* hiden)
{
    if (hiden == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hiden = ((CWindowManagerLayoutParams*)attrs)->mType
        == WindowManagerLayoutParams_TYPE_KEYGUARD;

    return NOERROR;
}

ECode CPhoneWindowManager::CanBeForceHidden(
    /* [in] */ IWindowState* win,
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [out] */ Boolean* hiden)
{
    if (hiden == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hiden = ((CWindowManagerLayoutParams*)attrs)->mType
        != WindowManagerLayoutParams_TYPE_STATUS_BAR
        && ((CWindowManagerLayoutParams*)attrs)->mType
        != WindowManagerLayoutParams_TYPE_WALLPAPER;

    return NOERROR;
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::AddStartingWindow(
    /* [in] */ IBinder* appToken,
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 theme,
    /* [in] */ const ArrayOf<Char8>& nonLocalizedLabel,
    /* [in] */ Int32 labelRes,
    /* [in] */ Int32 icon,
    /* [out] */ IView** window)
{
    if (window == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *window = NULL;

    if (!SHOW_STARTING_ANIMATIONS) {
        return NOERROR;
    }

    if (capsuleName.IsNullOrEmpty()) {
        return NOERROR;
    }

    ECode ec;
    //try {
        AutoPtr<IContext> context = mContext;
        Boolean setTheme = FALSE;
        //Log.i(TAG, "addStartingWindow " + capsuleName + ": nonLocalizedLabel="
        //        + nonLocalizedLabel + " theme=" + Integer.toHexString(theme));
        if (theme != 0 || labelRes != 0) {
            AutoPtr<IContext> temp;
            ec = context->CreateCapsuleContext(capsuleName, 0, (IContext**)&temp);
            if (FAILED(ec)) {
                return ec;
            }

            context = temp;
            if (theme != 0) {
                //context->SetTheme(theme);
                //setTheme = TRUE;
            }
        }

        if (!setTheme) {
            //context->SetTheme(com.android.internal.R.style.Theme);
        }

        AutoPtr<IPolicyManager> pm;
        ec = CPolicyManager::AcquireSingleton((IPolicyManager**)&pm);
        if (FAILED(ec)) {
            return ec;
        }

        AutoPtr<IWindow> win;
        ec = pm->MakeNewWindow(context, (IWindow**)&win);
        if (FAILED(ec)) {
            return ec;
        }

        //if (win->GetWindowStyle().getBoolean(
        //    com.android.internal.R.styleable.Window_windowDisablePreview, FALSE)) {
        //        return NULL;
        //}

        AutoPtr<IResources> r;
        context->GetResources((IResources**)&r);
        //win->SetTitle(r.getText(labelRes, nonLocalizedLabel));

        //win->SetType(WindowManagerLayoutParams_TYPE_APPLICATION_STARTING);
        // Force the window flags: this is a fake window, so it is not really
        // touchable or focusable by the user.  We also add in the ALT_FOCUSABLE_IM
        // flag because we do know that the next window will take input
        // focus, so we want to get the IME window up on top of us right away.
        //win->SetFlags(
        //    WindowManagerLayoutParams_FLAG_NOT_TOUCHABLE|
        //    WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE|
        //    WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM,
        //    WindowManagerLayoutParams_FLAG_NOT_TOUCHABLE|
        //    WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE|
        //    WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM);

        //win->SetLayout(ViewGroupLayoutParams_MATCH_PARENT,
        //    ViewGroupLayoutParams_MATCH_PARENT);

        AutoPtr<CWindowManagerLayoutParams> params;
        win->GetAttributes((IWindowManagerLayoutParams**)&params);
        params->mToken = appToken;
        params->mCapsuleName = capsuleName;
        //params->mWindowAnimations = win.getWindowStyle().getResourceId(
        //    com.android.internal.R.styleable.Window_windowAnimationStyle, 0);
        AutoPtr<ICharSequence> tl;
        CStringWrapper::New(String("Starting ") + capsuleName, (ICharSequence**)&tl);
        params->SetTitle(tl);

        AutoPtr<IWindowManager> wm;
        context->GetSystemService(Context_WINDOW_SERVICE, (IInterface**)&wm);

        AutoPtr<IView> view;
        win->GetDecorView((IView**)&view);

        //if (win->IsFloating()) {
        //    // Whoops, there is no way to display an animation/preview
        //    // of such a thing!  After all that work...  let's skip it.
        //    // (Note that we must do this here because it is in
        //    // getDecorView() where the theme is evaluated...  maybe
        //    // we should peek the floating attribute from the theme
        //    // earlier.)
        //    return NOERROR;
        //}

        //if (localLOGV) Log.v(
        //    TAG, "Adding starting window for " + capsuleName
        //    + " / " + appToken + ": "
        //    + (view.getParent() != NULL ? view : NULL));

        ec = wm->AddViewEx5(view, params);
        if (FAILED(ec)) {
            return ec;
        }

        // Only return the view if it was successfully added to the
        // window manager... which we can tell by it having a parent.
        //
        AutoPtr<IViewParent> parent;
        view->GetParent((IViewParent**)&parent);
        *window = parent != NULL ? view : NULL;
    //} catch (WindowManagerImpl.BadTokenException e) {
    //    // ignore
    //    Log.w(TAG, appToken + " already running, starting window not displayed");
    //} catch (RuntimeException e) {
    //    // don't crash if something else bad happens, for example a
    //    // failure loading resources because we are loading from an app
    //    // on external storage that has been unmounted.
    //    Log.w(TAG, appToken + " failed creating starting window", e);
    //}

    return NOERROR;
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::RemoveStartingWindow(
    /* [in] */ IBinder* appToken,
    /* [in] */ IView* window)
{
    //// RuntimeException e = new RuntimeException();
    //// Log.i(TAG, "remove " + appToken + " " + window, e);

    //if (localLOGV) Log.v(
    //    TAG, "Removing starting window for " + appToken + ": " + window);

    if (window != NULL) {
        AutoPtr<IWindowManager> wm;
        mContext->GetSystemService(Context_WINDOW_SERVICE, (IInterface**)&wm);
        wm->RemoveView(window);
    }

    return NOERROR;
}

/**
* Preflight adding a window to the system.
*
* Currently enforces that three window types are singletons:
* <ul>
* <li>STATUS_BAR_TYPE</li>
* <li>KEYGUARD_TYPE</li>
* </ul>
*
* @param win The window to be added
* @param attrs Information about the window to be added
*
* @return If ok, WindowManagerImpl.ADD_OKAY.  If too many singletons, WindowManagerImpl.ADD_MULTIPLE_SINGLETON
*/
ECode CPhoneWindowManager::PrepareAddWindowLw(
    /* [in] */ IWindowState* win,
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [out] */ Int32* added)
{
    if (added == NULL) {
        return E_INVALID_ARGUMENT;
    }

    switch (((CWindowManagerLayoutParams*)attrs)->mType) {
        case WindowManagerLayoutParams_TYPE_STATUS_BAR:
            //mContext->EnforceCallingOrSelfPermission(
            //    android.Manifest.permission.STATUS_BAR_SERVICE,
            //    "PhoneWindowManager");

            // TODO: Need to handle the race condition of the status bar proc
            // dying and coming back before the removeWindowLw cleanup has happened.
            if (mStatusBar != NULL) {
                *added = CWindowManagerImpl::ADD_MULTIPLE_SINGLETON;
                return NOERROR;
            }

            mStatusBar = win;
            break;
        case WindowManagerLayoutParams_TYPE_STATUS_BAR_PANEL:
            //mContext.enforceCallingOrSelfPermission(
            //    android.Manifest.permission.STATUS_BAR_SERVICE,
            //    "PhoneWindowManager");
            mStatusBarPanels.PushBack(win);
            break;
        case WindowManagerLayoutParams_TYPE_KEYGUARD:
            if (mKeyguard != NULL) {
                *added = CWindowManagerImpl::ADD_MULTIPLE_SINGLETON;
                return NOERROR;
            }
            mKeyguard = win;
            break;
    }
    *added = CWindowManagerImpl::ADD_OKAY;

    return NOERROR;
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::RemoveWindowLw(
    /* [in] */ IWindowState* win)
{
    if (mStatusBar.Get() == win) {
        mStatusBar = NULL;
    }
    else if (mKeyguard.Get() == win) {
        mKeyguard = NULL;
    }
    else {
        mStatusBarPanels.Remove(win);
    }

    return NOERROR;
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::SelectAnimationLw(
    /* [in] */ IWindowState* win,
    /* [in] */ Int32 transit,
    /* [out] */ Int32* id)
{
    //if (PRINT_ANIM) Log.i(TAG, "selectAnimation in " + win
    //    + ": transit=" + transit);
    //if (transit == TRANSIT_PREVIEW_DONE) {
    //    if (win.hasAppShownWindows()) {
    //        if (PRINT_ANIM) Log.i(TAG, "**** STARTING EXIT");
    //        return com.android.internal.R.anim.app_starting_exit;
    //    }
    //}

    //return 0;

    return E_NOT_IMPLEMENTED;
}

//public Animation CreateForceHideEnterAnimation()
//{
//    return AnimationUtils.loadAnimation(mContext,
//        com.android.internal.R.anim.lock_screen_behind_enter);
//}
//
//static ITelephony GetTelephonyService()
//{
//    ITelephony telephonyService = ITelephony.Stub.asInterface(
//        ServiceManager.checkService(Context.TELEPHONY_SERVICE));
//    if (telephonyService == NULL) {
//        Log.w(TAG, "Unable to find ITelephony interface.");
//    }
//    return telephonyService;
//}

//static IAudioService GetAudioService()
//{
//    IAudioService audioService = IAudioService.Stub.asInterface(
//        ServiceManager.checkService(Context.AUDIO_SERVICE));
//    if (audioService == NULL) {
//        Log.w(TAG, "Unable to find IAudioService interface.");
//    }
//    return audioService;
//}

Boolean CPhoneWindowManager::KeyguardOn()
{
    Boolean restricted;
    InKeyguardRestrictedKeyInputMode(&restricted);

    return KeyguardIsShowingTq() || restricted;
}

/** {@inheritDoc} */

ECode CPhoneWindowManager::InterceptKeyBeforeDispatching(
    /* [in] */ IWindowState* win,
    /* [in] */ Int32 action,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 scanCode,
    /* [in] */ Int32 metaState,
    /* [in] */ Int32 repeatCount,
    /* [in] */ Int32 policyFlags,
    /* [out] */ Boolean* consumed)
{
    //final Boolean keyguardOn = keyguardOn();
    //final Boolean down = (action == KeyEvent_ACTION_DOWN);
    //final Boolean canceled = ((flags & KeyEvent_FLAG_CANCELED) != 0);

    //if (FALSE) {
    //    Log.d(TAG, "interceptKeyTi keyCode=" + keyCode + " down=" + down + " repeatCount="
    //        + repeatCount + " keyguardOn=" + keyguardOn + " mHomePressed=" + mHomePressed);
    //}

    //// Clear a pending HOME longpress if the user releases Home
    //// TODO: This could probably be inside the next bit of logic, but that code
    //// turned out to be a bit fragile so I'm doing it here explicitly, for now.
    //if ((keyCode == KeyEvent_KEYCODE_HOME) && !down) {
    //    mHandler.removeCallbacks(mHomeLongPress);
    //}

    //// If the HOME button is currently being held, then we do special
    //// chording with it.
    //if (mHomePressed) {

    //    // If we have released the home key, and didn't do anything else
    //    // while it was pressed, then it is time to go home!
    //    if (keyCode == KeyEvent_KEYCODE_HOME) {
    //        if (!down) {
    //            mHomePressed = FALSE;

    //            if (!canceled) {
    //                // If an incoming call is ringing, HOME is totally disabled.
    //                // (The user is already on the InCallScreen at this point,
    //                // and his ONLY options are to answer or reject the call.)
    //                Boolean incomingRinging = FALSE;
    //                try {
    //                    ITelephony telephonyService = getTelephonyService();
    //                    if (telephonyService != NULL) {
    //                        incomingRinging = telephonyService.isRinging();
    //                    }
    //                } catch (RemoteException ex) {
    //                    Log.w(TAG, "RemoteException from getPhoneInterface()", ex);
    //                }

    //                if (incomingRinging) {
    //                    Log.i(TAG, "Ignoring HOME; there's a ringing incoming call.");
    //                } else {
    //                    launchHomeFromHotKey();
    //                }
    //            } else {
    //                Log.i(TAG, "Ignoring HOME; event canceled.");
    //            }
    //        }
    //    }

    //    return TRUE;
    //}

    //// First we always handle the home key here, so applications
    //// can never break it, although if keyguard is on, we do let
    //// it handle it, because that gives us the correct 5 second
    //// timeout.
    //if (keyCode == KeyEvent_KEYCODE_HOME) {

    //    // If a system window has focus, then it doesn't make sense
    //    // right now to interact with applications.
    //    WindowManager.LayoutParams attrs = win != NULL ? win.getAttrs() : NULL;
    //    if (attrs != NULL) {
    //        final Int32 type = attrs.type;
    //        if (type == WindowManagerLayoutParams_TYPE_KEYGUARD
    //            || type == WindowManagerLayoutParams_TYPE_KEYGUARD_DIALOG) {
    //                // the "app" is keyguard, so give it the key
    //                return FALSE;
    //        }
    //        final Int32 typeCount = WINDOW_TYPES_WHERE_HOME_DOESNT_WORK.length;
    //        for (Int32 i=0; i<typeCount; i++) {
    //            if (type == WINDOW_TYPES_WHERE_HOME_DOESNT_WORK[i]) {
    //                // don't do anything, but also don't pass it to the app
    //                return TRUE;
    //            }
    //        }
    //    }

    //    if (down && repeatCount == 0) {
    //        if (!keyguardOn) {
    //            mHandler.postDelayed(mHomeLongPress, ViewConfiguration.getGlobalActionKeyTimeout());
    //        }
    //        mHomePressed = TRUE;
    //    }
    //    return TRUE;
    //} else if (keyCode == KeyEvent_KEYCODE_MENU) {
    //    // Hijack modified menu keys for debugging features
    //    final Int32 chordBug = KeyEvent_META_SHIFT_ON;

    //    if (down && repeatCount == 0) {
    //        if (mEnableShiftMenuBugReports && (metaState & chordBug) == chordBug) {
    //            Intent intent = new Intent(Intent.ACTION_BUG_REPORT);
    //            mContext.sendOrderedBroadcast(intent, NULL);
    //            return TRUE;
    //        } else if (SHOW_PROCESSES_ON_ALT_MENU &&
    //            (metaState & KeyEvent_META_ALT_ON) == KeyEvent_META_ALT_ON) {
    //                Intent service = new Intent();
    //                service.setClassName(mContext, "com.android.server.LoadAverageService");
    //                ContentResolver res = mContext.getContentResolver();
    //                Boolean shown = Settings.System.getInt(
    //                    res, Settings.System.SHOW_PROCESSES, 0) != 0;
    //                if (!shown) {
    //                    mContext.startService(service);
    //                } else {
    //                    mContext.stopService(service);
    //                }
    //                Settings.System.putInt(
    //                    res, Settings.System.SHOW_PROCESSES, shown ? 0 : 1);
    //                return TRUE;
    //        }
    //    }
    //} else if (keyCode == KeyEvent_KEYCODE_SEARCH) {
    //    if (down) {
    //        if (repeatCount == 0) {
    //            mSearchKeyPressed = TRUE;
    //        }
    //    } else {
    //        mSearchKeyPressed = FALSE;

    //        if (mConsumeSearchKeyUp) {
    //            // Consume the up-event
    //            mConsumeSearchKeyUp = FALSE;
    //            return TRUE;
    //        }
    //    }
    //}

    //// Shortcuts are invoked through Search+key, so intercept those here
    //if (mSearchKeyPressed) {
    //    if (down && repeatCount == 0 && !keyguardOn) {
    //        Intent shortcutIntent = mShortcutManager.getIntent(keyCode, metaState);
    //        if (shortcutIntent != NULL) {
    //            shortcutIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
    //            mContext.startActivity(shortcutIntent);

    //            /*
    //            * We launched an app, so the up-event of the search key
    //            * should be consumed
    //            */
    //            mConsumeSearchKeyUp = TRUE;
    //            return TRUE;
    //        }
    //    }
    //}

    //return FALSE;

    return E_NOT_IMPLEMENTED;
}

/**
* A home key -> launch home action was detected.  Take the appropriate action
* given the situation with the keyguard.
*/
void CPhoneWindowManager::LaunchHomeFromHotKey()
{
    //if (mKeyguardMediator.isShowingAndNotHidden()) {
    //    // don't launch home if keyguard showing
    //} else if (!mHideLockScreen && mKeyguardMediator.isInputRestricted()) {
    //    // when in keyguard restricted mode, must first verify unlock
    //    // before launching home
    //    mKeyguardMediator.verifyUnlock(new OnKeyguardExitResult() {
    //        public void onKeyguardExitResult(Boolean success) {
    //            if (success) {
    //                try {
    //                    ActivityManagerNative.getDefault().stopAppSwitches();
    //                } catch (RemoteException e) {
    //                }
    //                sendCloseSystemWindows(SYSTEM_DIALOG_REASON_HOME_KEY);
    //                startDockOrHome();
    //            }
    //        }
    //    });
    //} else {
    //    // no keyguard stuff to worry about, just launch home!
    //    try {
    //        ActivityManagerNative.getDefault().stopAppSwitches();
    //    } catch (RemoteException e) {
    //    }
    //    sendCloseSystemWindows(SYSTEM_DIALOG_REASON_HOME_KEY);
    //    startDockOrHome();
    //}
}

ECode CPhoneWindowManager::GetContentInsetHintLw(
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [in] */ IRect* contentInset)
{
    const Int32 fl = ((CWindowManagerLayoutParams*)attrs)->mFlags;

    if ((fl & (WindowManagerLayoutParams_FLAG_LAYOUT_IN_SCREEN
        | WindowManagerLayoutParams_FLAG_FULLSCREEN
        | WindowManagerLayoutParams_FLAG_LAYOUT_INSET_DECOR))
        == (WindowManagerLayoutParams_FLAG_LAYOUT_IN_SCREEN
        | WindowManagerLayoutParams_FLAG_LAYOUT_INSET_DECOR)) {
            contentInset->Set(mCurLeft, mCurTop, mW - mCurRight, mH - mCurBottom);
    } else {
        contentInset->SetEmpty();
    }

    return NOERROR;
}


/** {@inheritDoc} */
ECode CPhoneWindowManager::BeginLayoutLw(
    /* [in] */ Int32 displayWidth,
    /* [in] */ Int32 displayHeight)
{
    mW = displayWidth;
    mH = displayHeight;
    mDockLeft = mContentLeft = mCurLeft = 0;
    mDockTop = mContentTop = mCurTop = 0;
    mDockRight = mContentRight = mCurRight = displayWidth;
    mDockBottom = mContentBottom = mCurBottom = displayHeight;
    mDockLayer = 0x10000000;

    // decide where the status bar goes ahead of time
    if (mStatusBar != NULL) {
        CRect* pf = mTmpParentFrame;
        CRect* df = mTmpDisplayFrame;
        CRect* vf = mTmpVisibleFrame;
        pf->mLeft = df->mLeft = vf->mLeft = 0;
        pf->mTop = df->mTop = vf->mTop = 0;
        pf->mRight = df->mRight = vf->mRight = displayWidth;
        pf->mBottom = df->mBottom = vf->mBottom = displayHeight;

        //mStatusBar->ComputeFrameLw(pf, df, vf, vf);
        //if (mStatusBar.IsVisibleLw()) {
        //    // If the status bar is hidden, we don't want to cause
        //    // windows behind it to scroll.
        //    mDockTop = mContentTop = mCurTop = mStatusBar.getFrameLw()->mBottom;
        //    if (DEBUG_LAYOUT) Log.v(TAG, "Status bar: mDockBottom="
        //        + mDockBottom + " mContentBottom="
        //        + mContentBottom + " mCurBottom=" + mCurBottom);
        //}
    }

    return NOERROR;
}

void CPhoneWindowManager::SetAttachedWindowFrames(
    /* [in] */ IWindowState* win,
    /* [in] */ Int32 fl,
    /* [in] */ Int32 sim,
    /* [in] */ IWindowState* attached,
    /* [in] */ Boolean insetDecors,
    /* [in] */ IRect* pf,
    /* [in] */ IRect* df,
    /* [in] */ IRect* cf,
    /* [in] */ IRect* vf)
{
     //if (win.getSurfaceLayer() > mDockLayer && attached.getSurfaceLayer() < mDockLayer) {
     //    // Here's a special case: if this attached window is a panel that is
     //    // above the dock window, and the window it is attached to is below
     //    // the dock window, then the frames we computed for the window it is
     //    // attached to can not be used because the dock is effectively part
     //    // of the underlying window and the attached window is floating on top
     //    // of the whole thing.  So, we ignore the attached window and explicitly
     //    // compute the frames that would be appropriate without the dock.
     //    df->mLeft = cf->mLeft = vf->mLeft = mDockLeft;
     //    df->mTop = cf->mTop = vf->mTop = mDockTop;
     //    df->mRight = cf->mRight = vf->mRight = mDockRight;
     //    df->mBottom = cf->mBottom = vf->mBottom = mDockBottom;
     //} else {
     //    // The effective display frame of the attached window depends on
     //    // whether it is taking care of insetting its content.  If not,
     //    // we need to use the parent's content frame so that the entire
     //    // window is positioned within that content.  Otherwise we can use
     //    // the display frame and let the attached window take care of
     //    // positioning its content appropriately.
     //    if ((sim & SOFT_INPUT_MASK_ADJUST) != SOFT_INPUT_ADJUST_RESIZE) {
     //        cf.set(attached.getDisplayFrameLw());
     //    } else {
     //        // If the window is resizing, then we want to base the content
     //        // frame on our attached content frame to resize...  however,
     //        // things can be tricky if the attached window is NOT in resize
     //        // mode, in which case its content frame will be larger.
     //        // Ungh.  So to deal with that, make sure the content frame
     //        // we end up using is not covering the IM dock.
     //        cf.set(attached.getContentFrameLw());
     //        if (attached.getSurfaceLayer() < mDockLayer) {
     //            if (cf->mLeft < mContentLeft) cf->mLeft = mContentLeft;
     //            if (cf->mTop < mContentTop) cf->mTop = mContentTop;
     //            if (cf->mRight > mContentRight) cf->mRight = mContentRight;
     //            if (cf->mBottom > mContentBottom) cf->mBottom = mContentBottom;
     //        }
     //    }
     //    df.set(insetDecors ? attached.getDisplayFrameLw() : cf);
     //    vf.set(attached.getVisibleFrameLw());
     //}
     //// The LAYOUT_IN_SCREEN flag is used to determine whether the attached
     //// window should be positioned relative to its parent or the entire
     //// screen.
     //pf.set((fl & FLAG_LAYOUT_IN_SCREEN) == 0
     //    ? attached.getFrameLw() : df);
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::LayoutWindowLw(
    /* [in] */ IWindowState* win,
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [in] */ IWindowState* attached)
{
    // we've already done the status bar
    if (win == mStatusBar) {
       return NOERROR;
    }

    //if (FALSE) {
    //   if ("com.google.android.youtube".equals(attrs.capsuleName)
    //       && attrs.type == WindowManagerLayoutParams_TYPE_APPLICATION_PANEL) {
    //           Log.i(TAG, "GOTCHA!");
    //   }
    //}

    CWindowManagerLayoutParams* attrsObj = (CWindowManagerLayoutParams*)attrs;
    Int32 fl = attrsObj->mFlags;
    Int32 sim = attrsObj->mSoftInputMode;

    CRect* pf = mTmpParentFrame;
    CRect* df = mTmpDisplayFrame;
    CRect* cf = mTmpContentFrame;
    CRect* vf = mTmpVisibleFrame;

    if (attrsObj->mType == WindowManagerLayoutParams_TYPE_INPUT_METHOD) {
       pf->mLeft = df->mLeft = cf->mLeft = vf->mLeft = mDockLeft;
       pf->mTop = df->mTop = cf->mTop = vf->mTop = mDockTop;
       pf->mRight = df->mRight = cf->mRight = vf->mRight = mDockRight;
       pf->mBottom = df->mBottom = cf->mBottom = vf->mBottom = mDockBottom;
       // IM dock windows always go to the bottom of the screen.
       attrsObj->mGravity = Gravity_BOTTOM;
       win->GetSurfaceLayer(&mDockLayer);
    }
    else {
       if ((fl & (WindowManagerLayoutParams_FLAG_LAYOUT_IN_SCREEN
           | WindowManagerLayoutParams_FLAG_FULLSCREEN
           | WindowManagerLayoutParams_FLAG_LAYOUT_INSET_DECOR))
           == (WindowManagerLayoutParams_FLAG_LAYOUT_IN_SCREEN
           | WindowManagerLayoutParams_FLAG_LAYOUT_INSET_DECOR)) {
           // This is the case for a normal activity window: we want it
           // to cover all of the screen space, and it can take care of
           // moving its contents to account for screen decorations that
           // intrude into that space.
           //
           if (attached != NULL) {
               // If this window is attached to another, our display
               // frame is the same as the one we are attached to.
               SetAttachedWindowFrames(win, fl, sim, attached, TRUE, pf, df, cf, vf);
           }
           else {
               pf->mLeft = df->mLeft = 0;
               pf->mTop = df->mTop = 0;
               pf->mRight = df->mRight = mW;
               pf->mBottom = df->mBottom = mH;
               if ((sim & WindowManagerLayoutParams_SOFT_INPUT_MASK_ADJUST)
                   != WindowManagerLayoutParams_SOFT_INPUT_ADJUST_RESIZE) {
                   cf->mLeft = mDockLeft;
                   cf->mTop = mDockTop;
                   cf->mRight = mDockRight;
                   cf->mBottom = mDockBottom;
               }
               else {
                   cf->mLeft = mContentLeft;
                   cf->mTop = mContentTop;
                   cf->mRight = mContentRight;
                   cf->mBottom = mContentBottom;
               }
               vf->mLeft = mCurLeft;
               vf->mTop = mCurTop;
               vf->mRight = mCurRight;
               vf->mBottom = mCurBottom;
           }
       }
       else if ((fl & WindowManagerLayoutParams_FLAG_LAYOUT_IN_SCREEN) != 0) {
           // A window that has requested to fill the entire screen just
           // gets everything, period.
           pf->mLeft = df->mLeft = cf->mLeft = 0;
           pf->mTop = df->mTop = cf->mTop = 0;
           pf->mRight = df->mRight = cf->mRight = mW;
           pf->mBottom = df->mBottom = cf->mBottom = mH;
           vf->mLeft = mCurLeft;
           vf->mTop = mCurTop;
           vf->mRight = mCurRight;
           vf->mBottom = mCurBottom;
       }
       else if (attached != NULL) {
           // A child window should be placed inside of the same visible
           // frame that its parent had.
           SetAttachedWindowFrames(win, fl, sim, attached, FALSE, pf, df, cf, vf);
       }
       else {
           // Otherwise, a normal window must be placed inside the content
           // of all screen decorations.
           pf->mLeft = mContentLeft;
           pf->mTop = mContentTop;
           pf->mRight = mContentRight;
           pf->mBottom = mContentBottom;
           if ((sim & WindowManagerLayoutParams_SOFT_INPUT_MASK_ADJUST)
               != WindowManagerLayoutParams_SOFT_INPUT_ADJUST_RESIZE) {
               df->mLeft = cf->mLeft = mDockLeft;
               df->mTop = cf->mTop = mDockTop;
               df->mRight = cf->mRight = mDockRight;
               df->mBottom = cf->mBottom = mDockBottom;
           }
           else {
               df->mLeft = cf->mLeft = mContentLeft;
               df->mTop = cf->mTop = mContentTop;
               df->mRight = cf->mRight = mContentRight;
               df->mBottom = cf->mBottom = mContentBottom;
           }
           vf->mLeft = mCurLeft;
           vf->mTop = mCurTop;
           vf->mRight = mCurRight;
           vf->mBottom = mCurBottom;
       }
    }

    if ((fl & WindowManagerLayoutParams_FLAG_LAYOUT_NO_LIMITS) != 0) {
       df->mLeft = df->mTop = cf->mLeft = cf->mTop = vf->mLeft = vf->mTop = -10000;
       df->mRight = df->mBottom = cf->mRight = cf->mBottom = vf->mRight = vf->mBottom = 10000;
    }

    //if (DEBUG_LAYOUT) Log.v(TAG, "Compute frame " + attrsCls->getTitle()
    //   + ": sim=#" + Integer.toHexString(sim)
    //   + " pf=" + pf.toShortString() + " df=" + df.toShortString()
    //   + " cf=" + cf.toShortString() + " vf=" + vf.toShortString());

    //if (FALSE) {
    //   if ("com.google.android.youtube".equals(attrsCls->capsuleName)
    //       && attrsObj->type == WindowManagerLayoutParams_TYPE_APPLICATION_PANEL) {
    //           if (TRUE || localLOGV) Log.v(TAG, "Computing frame of " + win +
    //               ": sim=#" + Integer.toHexString(sim)
    //               + " pf=" + pf.toShortString() + " df=" + df.toShortString()
    //               + " cf=" + cf.toShortString() + " vf=" + vf.toShortString());
    //   }
    //}

    win->ComputeFrameLw(pf, df, cf, vf);

    // Dock windows carve out the bottom of the screen, so normal windows
    // can't appear underneath them.
    //
    if (attrsObj->mType == WindowManagerLayoutParams_TYPE_INPUT_METHOD) {
        Boolean result;
        win->GetGivenInsetsPendingLw(&result);
        if (!result) {
            AutoPtr<IRect> contentFrame;
            win->GetContentFrameLw((IRect**)&contentFrame);
            AutoPtr<IRect> contentInsets;
            win->GetGivenContentInsetsLw((IRect**)&contentInsets);

            Int32 top = ((CRect*)contentFrame.Get())->mTop;
            top += ((CRect*)contentInsets.Get())->mTop;
            if (mContentBottom > top) {
                mContentBottom = top;
            }

            top = ((CRect*)contentFrame.Get())->mTop;
            top += ((CRect*)contentInsets.Get())->mTop;
            if (mCurBottom > top) {
                mCurBottom = top;
            }

            if (DEBUG_LAYOUT) {
                Logger::V(TAG, StringBuffer("Input method: mDockBottom=")
                + mDockBottom + " mContentBottom="
                + mContentBottom + " mCurBottom=" + mCurBottom);
            }
        }
    }

    return NOERROR;
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::FinishLayoutLw(
    /* [out] */ Int32* bitSet)
{
    if (!bitSet) {
        return E_INVALID_ARGUMENT;
    }

    *bitSet = 0;

    return NOERROR;
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::BeginAnimationLw(
    /* [in] */ Int32 displayWidth,
    /* [in] */ Int32 displayHeight)
{
    mTopFullscreenOpaqueWindowState = NULL;
    mForceStatusBar = FALSE;

    mHideLockScreen = FALSE;
    mAllowLockscreenWhenOn = FALSE;
    mDismissKeyguard = FALSE;

    return NOERROR;
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::AnimatingWindowLw(
    /* [in] */ IWindowState* win,
    /* [in] */ IWindowManagerLayoutParams* attrs)
{
    //if (mTopFullscreenOpaqueWindowState == NULL &&
    //  win.isVisibleOrBehindKeyguardLw()) {
    //      if ((attrs.flags & FLAG_FORCE_NOT_FULLSCREEN) != 0) {
    //          mForceStatusBar = TRUE;
    //      }
    //      if (attrs.type >= FIRST_APPLICATION_WINDOW
    //          && attrs.type <= LAST_APPLICATION_WINDOW
    //          && attrs.x == 0 && attrs.y == 0
    //          && attrs.width == ViewGroupLayoutParams_MATCH_PARENT
    //          && attrs.height == ViewGroupLayoutParams_MATCH_PARENT) {
    //              if (DEBUG_LAYOUT) Log.v(TAG, "Fullscreen window: " + win);
    //              mTopFullscreenOpaqueWindowState = win;
    //              if ((attrs.flags & FLAG_SHOW_WHEN_LOCKED) != 0) {
    //                  if (localLOGV) Log.v(TAG, "Setting mHideLockScreen to TRUE by win " + win);
    //                  mHideLockScreen = TRUE;
    //              }
    //              if ((attrs.flags & FLAG_DISMISS_KEYGUARD) != 0) {
    //                  if (localLOGV) Log.v(TAG, "Setting mDismissKeyguard to TRUE by win " + win);
    //                  mDismissKeyguard = TRUE;
    //              }
    //              if ((attrs.flags & FLAG_ALLOW_LOCK_WHILE_SCREEN_ON) != 0) {
    //                  mAllowLockscreenWhenOn = TRUE;
    //              }
    //      }
    //}
    return E_NOT_IMPLEMENTED;
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::FinishAnimationLw(
    /* [out] */ Int32* bitSet)
{
    //Int32 changes = 0;

    //Boolean hiding = FALSE;
    //if (mStatusBar != NULL) {
    //    if (localLOGV) Log.i(TAG, "force=" + mForceStatusBar
    //        + " top=" + mTopFullscreenOpaqueWindowState);
    //    if (mForceStatusBar) {
    //        if (DEBUG_LAYOUT) Log.v(TAG, "Showing status bar");
    //        if (mStatusBar.showLw(TRUE)) changes |= FINISH_LAYOUT_REDO_LAYOUT;
    //    } else if (mTopFullscreenOpaqueWindowState != NULL) {
    //        //Log.i(TAG, "frame: " + mTopFullscreenOpaqueWindowState.getFrameLw()
    //        //        + " shown frame: " + mTopFullscreenOpaqueWindowState.getShownFrameLw());
    //        //Log.i(TAG, "attr: " + mTopFullscreenOpaqueWindowState.getAttrs());
    //        WindowManager.LayoutParams lp =
    //            mTopFullscreenOpaqueWindowState.getAttrs();
    //        Boolean hideStatusBar =
    //            (lp.flags & WindowManagerLayoutParams_FLAG_FULLSCREEN) != 0;
    //        if (hideStatusBar) {
    //            if (DEBUG_LAYOUT) Log.v(TAG, "Hiding status bar");
    //            if (mStatusBar.hideLw(TRUE)) changes |= FINISH_LAYOUT_REDO_LAYOUT;
    //            hiding = TRUE;
    //        } else {
    //            if (DEBUG_LAYOUT) Log.v(TAG, "Showing status bar");
    //            if (mStatusBar.showLw(TRUE)) changes |= FINISH_LAYOUT_REDO_LAYOUT;
    //        }
    //    }
    //}

    //if (changes != 0 && hiding) {
    //    IStatusBarService sbs = IStatusBarService.Stub.asInterface(ServiceManager.getService("statusbar"));
    //    if (sbs != NULL) {
    //        try {
    //            // Make sure the window shade is hidden.
    //            sbs.collapse();
    //        } catch (RemoteException e) {
    //        }
    //    }
    //}

    //// Hide the key guard if a visible window explicitly specifies that it wants to be displayed
    //// when the screen is locked
    //if (mKeyguard != NULL) {
    //    if (localLOGV) Log.v(TAG, "finishLayoutLw::mHideKeyguard="+mHideLockScreen);
    //    if (mDismissKeyguard && !mKeyguardMediator.isSecure()) {
    //        if (mKeyguard.hideLw(TRUE)) {
    //            changes |= FINISH_LAYOUT_REDO_LAYOUT
    //                | FINISH_LAYOUT_REDO_CONFIG
    //                | FINISH_LAYOUT_REDO_WALLPAPER;
    //        }
    //        if (mKeyguardMediator.isShowing()) {
    //            mHandler.post(new Runnable() {
    //                ECode CPhoneWindowManager::run() {
    //                    mKeyguardMediator.keyguardDone(FALSE, FALSE);
    //                }
    //            });
    //        }
    //    } else if (mHideLockScreen) {
    //        if (mKeyguard.hideLw(TRUE)) {
    //            changes |= FINISH_LAYOUT_REDO_LAYOUT
    //                | FINISH_LAYOUT_REDO_CONFIG
    //                | FINISH_LAYOUT_REDO_WALLPAPER;
    //        }
    //        mKeyguardMediator.setHidden(TRUE);
    //    } else {
    //        if (mKeyguard.showLw(TRUE)) {
    //            changes |= FINISH_LAYOUT_REDO_LAYOUT
    //                | FINISH_LAYOUT_REDO_CONFIG
    //                | FINISH_LAYOUT_REDO_WALLPAPER;
    //        }
    //        mKeyguardMediator.setHidden(FALSE);
    //    }
    //}

    //// update since mAllowLockscreenWhenOn might have changed
    //updateLockScreenTimeout();
    //return changes;
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindowManager::AllowAppAnimationsLw(
    /* [out] */ Boolean* allowed)
{
    //if (mKeyguard != NULL && mKeyguard.isVisibleLw()) {
    //    // If keyguard is currently visible, no reason to animate
    //    // behind it.
    //    return FALSE;
    //}
    //if (mStatusBar != NULL && mStatusBar.isVisibleLw()) {
    //    IRect* rect = new IRect*(mStatusBar.getShownFrameLw());
    //    for (Int32 i=mStatusBarPanels.size()-1; i>=0; i--) {
    //        IWindowState* w = mStatusBarPanels.get(i);
    //        if (w.isVisibleLw()) {
    //            rect.union(w.getShownFrameLw());
    //        }
    //    }
    //    final Int32 insetw = mW/10;
    //    final Int32 inseth = mH/10;
    //    if (rect.contains(insetw, inseth, mW-insetw, mH-inseth)) {
    //        // All of the status bar windows put together cover the
    //        // screen, so the app can't be seen.  (Note this test doesn't
    //        // work if the rects of these windows are at off offsets or
    //        // sizes, causing gaps in the rect union we have computed.)
    //        return FALSE;
    //    }
    //}
    //return TRUE;
    return E_NOT_IMPLEMENTED;
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::NotifyLidSwitchChanged(
    /* [in] */ Int64 whenNanos,
    /* [in] */ Boolean lidOpen)
{
    //// lid changed state
    //mLidOpen = lidOpen;
    //Boolean awakeNow = mKeyguardMediator.doLidChangeTq(mLidOpen);
    //updateRotation(Surface.FLAGS_ORIENTATION_ANIMATION_DISABLE);
    //if (awakeNow) {
    //    // If the lid opening and we don't have to keep the
    //    // keyguard up, then we can turn on the screen
    //    // immediately.
    //    mKeyguardMediator.pokeWakelock();
    //} else if (keyguardIsShowingTq()) {
    //    if (mLidOpen) {
    //        // If we are opening the lid and not hiding the
    //        // keyguard, then we need to have it turn on the
    //        // screen once it is shown.
    //        mKeyguardMediator.onWakeKeyWhenKeyguardShowingTq(
    //            KeyEvent_KEYCODE_POWER);
    //    }
    //} else {
    //    // Light up the keyboard if we are sliding up.
    //    if (mLidOpen) {
    //        mPowerManager.userActivity(SystemClock.uptimeMillis(), FALSE,
    //            LocalPowerManager.BUTTON_EVENT);
    //    } else {
    //        mPowerManager.userActivity(SystemClock.uptimeMillis(), FALSE,
    //            LocalPowerManager.OTHER_EVENT);
    //    }
    //}
    return E_NOT_IMPLEMENTED;
}

/**
* @return Whether music is being played right now.
*/
Boolean CPhoneWindowManager::IsMusicActive()
{
    //final AudioManager am = (AudioManager)mContext.getSystemService(Context.AUDIO_SERVICE);
    //if (am == NULL) {
    //    Log.w(TAG, "isMusicActive: couldn't get AudioManager reference");
    //    return FALSE;
    //}
    //return am.isMusicActive();
    return FALSE;
}

/**
* Tell the audio service to adjust the volume appropriate to the event.
* @param keycode
*/
void CPhoneWindowManager::HandleVolumeKey(
    /* [in] */ Int32 stream,
    /* [in] */ Int32 keycode)
{
    //IAudioService audioService = getAudioService();
    //if (audioService == NULL) {
    //    return;
    //}
    //try {
    //    // since audio is playing, we shouldn't have to hold a wake lock
    //    // during the call, but we do it as a precaution for the rare possibility
    //    // that the music stops right before we call this
    //    mBroadcastWakeLock.acquire();
    //    audioService.adjustStreamVolume(stream,
    //        keycode == KeyEvent_KEYCODE_VOLUME_UP
    //        ? AudioManager.ADJUST_RAISE
    //        : AudioManager.ADJUST_LOWER,
    //        0);
    //} catch (RemoteException e) {
    //    Log.w(TAG, "IAudioService.adjustStreamVolume() threw RemoteException " + e);
    //} finally {
    //    mBroadcastWakeLock.release();
    //}
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::InterceptKeyBeforeQueueing(
    /* [in] */ Int64 whenNanos,
    /* [in] */ Int32 action,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 scanCode,
    /* [in] */ Int32 policyFlags,
    /* [in] */ Boolean isScreenOn,
    /* [out] */ Int32* bitwise)
{
    if (bitwise == NULL) {
        return E_INVALID_ARGUMENT;
    }

    Boolean down = action == KeyEvent_ACTION_DOWN;
    Boolean canceled = (flags & KeyEvent_FLAG_CANCELED) != 0;

    Boolean isInjected = (policyFlags & WindowManagerPolicy::FLAG_INJECTED) != 0;

    // If screen is off then we treat the case where the keyguard is open but hidden
    // the same as if it were open and in front.
    // This will prevent any keys other than the power button from waking the screen
    // when the keyguard is hidden by another activity.
    //
    //Boolean keyguardActive = (isScreenOn ?
    //    mKeyguardMediator.isShowingAndNotHidden() :
    //    mKeyguardMediator.isShowing());
    Boolean keyguardActive = FALSE;

    if (DEBUG) {
        Logger::D(TAG, StringBuffer("interceptKeyTq keycode=") + keyCode
            + " screenIsOn=" + isScreenOn + " keyguardActive=" + keyguardActive);
    }

    if (down && (policyFlags & WindowManagerPolicy::FLAG_VIRTUAL) != 0) {
        Boolean isSucceed;
        PerformHapticFeedbackLw(NULL, HapticFeedbackConstants::VIRTUAL_KEY, FALSE, &isSucceed);
    }

    // Basic policy based on screen state and keyguard.
    // FIXME: This policy isn't quite correct.  We shouldn't care whether the screen
    //        is on or off, really.  We should care about whether the device is in an
    //        interactive state or is in suspend pretending to be "off".
    //        The primary screen might be turned off due to proximity sensor or
    //        because we are presenting media on an auxiliary screen or remotely controlling
    //        the device some other way (which is why we have an exemption here for injected
    //        events).
    //
    Int32 result;
    if (isScreenOn || isInjected) {
        // When the screen is on or if the key is injected pass the key to the application.
        result = WindowManagerPolicy::ACTION_PASS_TO_USER;
    }
    else {
        // When the screen is off and the key is not injected, determine whether
        // to wake the device but don't pass the key to the application.
        result = 0;

        Boolean isWakeKey = (policyFlags
            & (WindowManagerPolicy::FLAG_WAKE
            | WindowManagerPolicy::FLAG_WAKE_DROPPED)) != 0;

        if (down && isWakeKey) {
            if (keyguardActive) {
                // If the keyguard is showing, let it decide what to do with the wake key.
                //
                //mKeyguardMediator.onWakeKeyWhenKeyguardShowingTq(keyCode);
            }
            else {
                // Otherwise, wake the device ourselves.
                result |= WindowManagerPolicy::ACTION_POKE_USER_ACTIVITY;
            }
        }
    }

    // Handle special keys.
    switch (keyCode) {
    case KeyEvent_KEYCODE_VOLUME_DOWN:
    case KeyEvent_KEYCODE_VOLUME_UP:
        {
            if (down) {
                //ITelephony telephonyService = GetTelephonyService();
                //if (telephonyService != NULL) {
                //    try {
                //        if (telephonyService.isRinging()) {
                //            // If an incoming call is ringing, either VOLUME key means
                //            // "silence ringer".  We handle these keys here, rather than
                //            // in the InCallScreen, to make sure we'll respond to them
                //            // even if the InCallScreen hasn't come to the foreground yet.
                //            // Look for the DOWN event here, to agree with the "fallback"
                //            // behavior in the InCallScreen.
                //            Log.i(TAG, "interceptKeyBeforeQueueing:"
                //                + " VOLUME key-down while ringing: Silence ringer!");

                //            // Silence the ringer.  (It's safe to call this
                //            // even if the ringer has already been silenced.)
                //            telephonyService.silenceRinger();

                //            // And *don't* pass this key thru to the current activity
                //            // (which is probably the InCallScreen.)
                //            result &= ~ACTION_PASS_TO_USER;
                //            break;
                //        }
                //        if (telephonyService.isOffhook()
                //            && (result & ACTION_PASS_TO_USER) == 0) {
                //                // If we are in call but we decided not to pass the key to
                //                // the application, handle the volume change here.
                //                handleVolumeKey(AudioManager.STREAM_VOICE_CALL, keyCode);
                //                break;
                //        }
                //    } catch (RemoteException ex) {
                //        Log.w(TAG, "ITelephony threw RemoteException", ex);
                //    }
                //}

                if (IsMusicActive() && (result
                    & WindowManagerPolicy::ACTION_PASS_TO_USER) == 0) {
                    // If music is playing but we decided not to pass the key to the
                    // application, handle the volume change here.
                    //
                    HandleVolumeKey(3/*AudioManager.STREAM_MUSIC*/, keyCode);
                }
            }
        }
        break;
    case KeyEvent_KEYCODE_ENDCALL:
        {
            result &= ~WindowManagerPolicy::ACTION_PASS_TO_USER;
            if (down) {
                //ITelephony telephonyService = getTelephonyService();
                Boolean hungUp = FALSE;
                //if (telephonyService != NULL) {
                //    try {
                //        hungUp = telephonyService.endCall();
                //    } catch (RemoteException ex) {
                //        Log.w(TAG, "ITelephony threw RemoteException", ex);
                //    }
                //}
                InterceptPowerKeyDown(!isScreenOn || hungUp);
            }
            else {
                if (InterceptPowerKeyUp(canceled)) {
                    if ((mEndcallBehavior
                        & 0x1/*Settings.System.END_BUTTON_BEHAVIOR_HOME*/) != 0) {
                        if (GoHome()) {
                            break;
                        }
                    }

                    if ((mEndcallBehavior
                        & 0x2/*Settings.System.END_BUTTON_BEHAVIOR_SLEEP*/) != 0) {
                        result = (result &
                            ~WindowManagerPolicy::ACTION_POKE_USER_ACTIVITY)
                            | WindowManagerPolicy::ACTION_GO_TO_SLEEP;
                    }
                }
            }
        }
        break;
    case KeyEvent_KEYCODE_POWER:
        {
            result &= ~WindowManagerPolicy::ACTION_PASS_TO_USER;
            if (down) {
                //ITelephony telephonyService = getTelephonyService();
                Boolean hungUp = FALSE;
                //if (telephonyService != NULL) {
                //    try {
                //        if (telephonyService.isRinging()) {
                //            // Pressing Power while there's a ringing incoming
                //            // call should silence the ringer.
                //            telephonyService.silenceRinger();
                //        } else if ((mIncallPowerBehavior
                //            & Settings.Secure.INCALL_POWER_BUTTON_BEHAVIOR_HANGUP) != 0
                //            && telephonyService.isOffhook()) {
                //                // Otherwise, if "Power button ends call" is enabled,
                //                // the Power button will hang up any current active call.
                //                hungUp = telephonyService.endCall();
                //        }
                //    } catch (RemoteException ex) {
                //        Log.w(TAG, "ITelephony threw RemoteException", ex);
                //    }
                //}
                InterceptPowerKeyDown(!isScreenOn || hungUp);
            }
            else {
                if (InterceptPowerKeyUp(canceled)) {
                    result = (result & ~WindowManagerPolicy::ACTION_POKE_USER_ACTIVITY)
                        | WindowManagerPolicy::ACTION_GO_TO_SLEEP;
                }
            }
        }
        break;
    case KeyEvent_KEYCODE_HEADSETHOOK:
    case KeyEvent_KEYCODE_MEDIA_PLAY_PAUSE:
    case KeyEvent_KEYCODE_MEDIA_STOP:
    case KeyEvent_KEYCODE_MEDIA_NEXT:
    case KeyEvent_KEYCODE_MEDIA_PREVIOUS:
    case KeyEvent_KEYCODE_MEDIA_REWIND:
    case KeyEvent_KEYCODE_MEDIA_FAST_FORWARD:
        {
            if ((result & WindowManagerPolicy::ACTION_PASS_TO_USER) == 0) {
                // Only do this if we would otherwise not pass it to the user. In that
                // case, the PhoneWindow class will do the same thing, except it will
                // only do it if the showing app doesn't process the key on its own.
                //
                //Int64 when = whenNanos / 1000000;
                //AutoPtr<IKeyEvent> keyEvent;
                //CKeyEvent::New(when, when, action, keyCode, 0, 0,
                //    0, scanCode, flags, CInputDevice::SOURCE_KEYBOARD,
                //    (IKeyEvent**)&keyEvent);
                //mBroadcastWakeLock.acquire();
                //mHandler.post(new PassHeadsetKey(keyEvent));
            }
        }
        break;
    case KeyEvent_KEYCODE_CALL:
        {
            if (down) {
                //ITelephony telephonyService = getTelephonyService();
                //if (telephonyService != NULL) {
                //    try {
                //        if (telephonyService.isRinging()) {
                //            Log.i(TAG, "interceptKeyBeforeQueueing:"
                //                + " CALL key-down while ringing: Answer the call!");
                //            telephonyService.answerRingingCall();

                //            // And *don't* pass this key thru to the current activity
                //            // (which is presumably the InCallScreen.)
                //            result &= ~ACTION_PASS_TO_USER;
                //        }
                //    } catch (RemoteException ex) {
                //        Log.w(TAG, "ITelephony threw RemoteException", ex);
                //    }
                //}
            }
        }
        break;
    default:
        break;
    }

    *bitwise = result;

    return NOERROR;
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::ScreenTurnedOff(
    /* [in] */ Int32 why)
{
    //EventLog.writeEvent(70000, 0);
    //mKeyguardMediator.onScreenTurnedOff(why);
    //synchronized (mLock) {
    //    mScreenOn = FALSE;
    //    updateOrientationListenerLp();
    //    updateLockScreenTimeout();
    //}
    return E_NOT_IMPLEMENTED;
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::ScreenTurnedOn()
{
    //EventLog.writeEvent(70000, 1);
    //mKeyguardMediator.onScreenTurnedOn();
    //synchronized (mLock) {
    //    mScreenOn = TRUE;
    //    updateOrientationListenerLp();
    //    updateLockScreenTimeout();
    //}
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindowManager::IsScreenOn(
    /* [out] */ Boolean* isScreenOn)
{
    if (!isScreenOn) {
        return E_INVALID_ARGUMENT;
    }

    *isScreenOn = mScreenOn;

    return NOERROR;
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::EnableKeyguard(
    /* [in] */ Boolean enabled)
{
    //mKeyguardMediator.setKeyguardEnabled(enabled);
    return E_NOT_IMPLEMENTED;
}

/** {@inheritDoc} */
//ECode CPhoneWindowManager::ExitKeyguardSecurely(
//    /* [in] */ OnKeyguardExitResult callback)
//{
//    mKeyguardMediator.verifyUnlock(callback);
//}

Boolean CPhoneWindowManager::KeyguardIsShowingTq()
{
    //return mKeyguardMediator.isShowingAndNotHidden();
    return FALSE;
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::InKeyguardRestrictedKeyInputMode(
    /* [out] */ Boolean* restricted)
{
    //return mKeyguardMediator.isInputRestricted();
    return E_NOT_IMPLEMENTED;
}

void CPhoneWindowManager::SendCloseSystemWindows()
{
    SendCloseSystemWindows(mContext, NULL);
}

void CPhoneWindowManager::SendCloseSystemWindows(
    /* [in] */ const char* reason)
{
    SendCloseSystemWindows(mContext, reason);
}

void CPhoneWindowManager::SendCloseSystemWindows(
    /* [in] */ IContext* context,
    /* [in] */ const char* reason)
{
    //if (ActivityManagerNative.isSystemReady()) {
    //    try {
    //        ActivityManagerNative.getDefault().closeSystemDialogs(reason);
    //    } catch (RemoteException e) {
    //    }
    //}
}

ECode CPhoneWindowManager::RotationForOrientationLw(
    /* [in] */ Int32 orientation,
    /* [in] */ Int32 lastRotation,
    /* [in] */ Boolean displayEnabled,
    /* [out] */ Int32* surfaceRotation)
{
    //if (mPortraitRotation < 0) {
    //    // Initialize the rotation angles for each orientation once.
    //    Display d = ((WindowManager)mContext.getSystemService(Context.WINDOW_SERVICE))
    //        .getDefaultDisplay();
    //    if (d.getWidth() > d.getHeight()) {
    //        mPortraitRotation = Surface.ROTATION_90;
    //        mLandscapeRotation = Surface.ROTATION_0;
    //        mUpsideDownRotation = Surface.ROTATION_270;
    //        mSeascapeRotation = Surface.ROTATION_180;
    //    } else {
    //        mPortraitRotation = Surface.ROTATION_0;
    //        mLandscapeRotation = Surface.ROTATION_90;
    //        mUpsideDownRotation = Surface.ROTATION_180;
    //        mSeascapeRotation = Surface.ROTATION_270;
    //    }
    //}

    //synchronized (mLock) {
    //    switch (orientation) {
    //            case CActivityInfo::SCREEN_ORIENTATION_PORTRAIT:
    //                //always return portrait if orientation set to portrait
    //                return mPortraitRotation;
    //            case CActivityInfo::SCREEN_ORIENTATION_LANDSCAPE:
    //                //always return landscape if orientation set to landscape
    //                return mLandscapeRotation;
    //            case CActivityInfo::SCREEN_ORIENTATION_REVERSE_PORTRAIT:
    //                //always return portrait if orientation set to portrait
    //                return mUpsideDownRotation;
    //            case CActivityInfo::SCREEN_ORIENTATION_REVERSE_LANDSCAPE:
    //                //always return seascape if orientation set to reverse landscape
    //                return mSeascapeRotation;
    //            case CActivityInfo::SCREEN_ORIENTATION_SENSOR_LANDSCAPE:
    //                //return either landscape rotation based on the sensor
    //                mOrientationListener.setAllow180Rotation(
    //                    isLandscapeOrSeascape(Surface.ROTATION_180));
    //                return getCurrentLandscapeRotation(lastRotation);
    //            case CActivityInfo::SCREEN_ORIENTATION_SENSOR_PORTRAIT:
    //                mOrientationListener.setAllow180Rotation(
    //                    !isLandscapeOrSeascape(Surface.ROTATION_180));
    //                return getCurrentPortraitRotation(lastRotation);
    //    }

    //    mOrientationListener.setAllow180Rotation(
    //        orientation == CActivityInfo::SCREEN_ORIENTATION_FULL_SENSOR);

    //    // case for nosensor meaning ignore sensor and consider only lid
    //    // or orientation sensor disabled
    //    //or case.unspecified
    //    if (mLidOpen) {
    //        return mLidOpenRotation;
    //    } else if (mDockMode == Intent.EXTRA_DOCK_STATE_CAR && mCarDockRotation >= 0) {
    //        return mCarDockRotation;
    //    } else if (mDockMode == Intent.EXTRA_DOCK_STATE_DESK && mDeskDockRotation >= 0) {
    //        return mDeskDockRotation;
    //    } else {
    //        if (useSensorForOrientationLp(orientation)) {
    //            return mOrientationListener.getCurrentRotation(lastRotation);
    //        }
    //        return Surface.ROTATION_0;
    //    }
    //}
    return E_NOT_IMPLEMENTED;
}

Int32 CPhoneWindowManager::GetCurrentLandscapeRotation(
    /* [in] */ Int32 lastRotation)
{
    //Int32 sensorRotation = mOrientationListener.getCurrentRotation(lastRotation);
    //if (IsLandscapeOrSeascape(sensorRotation)) {
    //    return sensorRotation;
    //}
    //// try to preserve the old rotation if it was landscape
    //if (IsLandscapeOrSeascape(lastRotation)) {
    //    return lastRotation;
    //}
    // default to one of the primary landscape rotation
    return mLandscapeRotation;
}

Boolean CPhoneWindowManager::IsLandscapeOrSeascape(
    /* [in] */ Int32 sensorRotation)
{
    return sensorRotation == mLandscapeRotation || sensorRotation == mSeascapeRotation;
}

Int32 CPhoneWindowManager::GetCurrentPortraitRotation(
    /* [in] */ Int32 lastRotation)
{
    //Int32 sensorRotation = mOrientationListener.getCurrentRotation(lastRotation);
    //if (IsAnyPortrait(sensorRotation)) {
    //    return sensorRotation;
    //}
    //// try to preserve the old rotation if it was portrait
    //if (IsAnyPortrait(lastRotation)) {
    //    return lastRotation;
    //}
    //// default to one of the primary portrait rotations
    return mPortraitRotation;
}

Boolean CPhoneWindowManager::IsAnyPortrait(
    /* [in] */ Int32 sensorRotation)
{
    return sensorRotation == mPortraitRotation || sensorRotation == mUpsideDownRotation;
}

ECode CPhoneWindowManager::DetectSafeMode(
    /* [out] */ Boolean* isSafe)
{
    //try {
    //    Int32 menuState = mWindowManager.getKeycodeState(KeyEvent_KEYCODE_MENU);
    //    Int32 sState = mWindowManager.getKeycodeState(KeyEvent_KEYCODE_S);
    //    Int32 dpadState = mWindowManager.getDPadKeycodeState(KeyEvent_KEYCODE_DPAD_CENTER);
    //    Int32 trackballState = mWindowManager.getTrackballScancodeState(BTN_MOUSE);
    //    mSafeMode = menuState > 0 || sState > 0 || dpadState > 0 || trackballState > 0;
    //    performHapticFeedbackLw(NULL, mSafeMode
    //        ? HapticFeedbackConstants.SAFE_MODE_ENABLED
    //        : HapticFeedbackConstants.SAFE_MODE_DISABLED, TRUE);
    //    if (mSafeMode) {
    //        Log.i(TAG, "SAFE MODE ENABLED (menu=" + menuState + " s=" + sState
    //            + " dpad=" + dpadState + " trackball=" + trackballState + ")");
    //    } else {
    //        Log.i(TAG, "SAFE MODE not enabled");
    //    }
    //    return mSafeMode;
    //} catch (RemoteException e) {
    //    // Doom! (it's also local)
    //    throw new RuntimeException("window manager dead");
    //}
    return E_NOT_IMPLEMENTED;
}

ArrayOf<Int64>* CPhoneWindowManager::GetLongIntArray(
    /* [in] */ IResources* r,
    /* [in] */ Int32 resid)
{
    //Int32[] ar = r.getIntArray(resid);
    //if (ar == NULL) {
    //    return NULL;
    //}
    //Int64[] out = new Int64[ar.length];
    //for (Int32 i=0; i<ar.length; i++) {
    //    out[i] = ar[i];
    //}
    //return out;
    return NULL;
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::SystemReady()
{
    //// tell the keyguard
    //mKeyguardMediator.onSystemReady();
    //android.os.SystemProperties.set("dev.bootcomplete", "1");
    //synchronized (mLock) {
    //    updateOrientationListenerLp();
    //    mSystemReady = TRUE;
    //    mHandler.post(new Runnable() {
    //        ECode CPhoneWindowManager::run() {
    //            updateSettings();
    //        }
    //    });
    //}
    return E_NOT_IMPLEMENTED;
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::UserActivity()
{
    //synchronized (mScreenLockTimeout) {
    //    if (mLockScreenTimerActive) {
    //        // reset the timer
    //        mHandler.removeCallbacks(mScreenLockTimeout);
    //        mHandler.postDelayed(mScreenLockTimeout, mLockScreenTimeout);
    //    }
    //}
    return E_NOT_IMPLEMENTED;
}

void CPhoneWindowManager::UpdateLockScreenTimeout()
{
    //synchronized (mScreenLockTimeout) {
    //    Boolean enable = (mAllowLockscreenWhenOn && mScreenOn && mKeyguardMediator.isSecure());
    //    if (mLockScreenTimerActive != enable) {
    //        if (enable) {
    //            if (localLOGV) Log.v(TAG, "setting lockscreen timer");
    //            mHandler.postDelayed(mScreenLockTimeout, mLockScreenTimeout);
    //        } else {
    //            if (localLOGV) Log.v(TAG, "clearing lockscreen timer");
    //            mHandler.removeCallbacks(mScreenLockTimeout);
    //        }
    //        mLockScreenTimerActive = enable;
    //    }
    //}
}

/** {@inheritDoc} */
ECode CPhoneWindowManager::EnableScreenAfterBoot()
{
    ReadLidState();
    UpdateRotation(Surface_FLAGS_ORIENTATION_ANIMATION_DISABLE);

    return NOERROR;
}

void CPhoneWindowManager::UpdateRotation(
    /* [in] */ Int32 animFlags)
{
    //mPowerManager.setKeyboardVisibility(mLidOpen);
    //Int32 rotation = Surface.ROTATION_0;
    //if (mLidOpen) {
    //    rotation = mLidOpenRotation;
    //} else if (mDockMode == Intent.EXTRA_DOCK_STATE_CAR && mCarDockRotation >= 0) {
    //    rotation = mCarDockRotation;
    //} else if (mDockMode == Intent.EXTRA_DOCK_STATE_DESK && mDeskDockRotation >= 0) {
    //    rotation = mDeskDockRotation;
    //}
    ////if lid is closed orientation will be portrait
    //try {
    //    //set orientation on WindowManager
    //    mWindowManager.setRotation(rotation, TRUE,
    //        mFancyRotationAnimation | animFlags);
    //} catch (RemoteException e) {
    //    // Ignore
    //}
}

/**
* Return an Intent to launch the currently active dock as home.  Returns
* NULL if the standard home should be launched.
* @return
*/
void CPhoneWindowManager::CreateHomeDockIntent(
    /* [out] */ IIntent** intent)
{
    assert(intent);

    *intent = NULL;
    // What home does is based on the mode, not the dock state.  That
    // is, when in car mode you should be taken to car home regardless
    // of whether we are actually in a car dock.
    if (mUiMode == Configuration_UI_MODE_TYPE_CAR) {
        *intent = mCarDockIntent;
        (*intent)->AddRef();
    } else if (mUiMode == Configuration_UI_MODE_TYPE_DESK) {
        *intent = mDeskDockIntent;
        (*intent)->AddRef();
    } else {
        return;
    }

    AutoPtr<CActivityInfo> ai;
    //intent->ResolveActivityInfo(
    //    mContext.getPackageManager(), PackageManager.GET_META_DATA);
    if (ai == NULL) {
        *intent = NULL;
        return;
    }

    //if (ai->mMetaData != NULL && ai.metaData.getBoolean(Intent.METADATA_DOCK_HOME)) {
    //    intent = new Intent(intent);
    //    intent.setClassName(ai.capsuleName, ai.name);
    //    return intent;
    //}

    *intent = NULL;

    return;
}

void CPhoneWindowManager::StartDockOrHome()
{
    AutoPtr<IIntent> dock;
    CreateHomeDockIntent((IIntent**)&dock);

    if (dock != NULL) {
        mContext->StartActivity(dock);
        return;
    }

    mContext->StartActivity(mHomeIntent);
}

/**
* goes to the home screen
* @return whether it did anything
*/
Boolean CPhoneWindowManager::GoHome()
{
    //if (FALSE) {
    //    // This code always brings home to the front.
    //    try {
    //        ActivityManagerNative.getDefault().stopAppSwitches();
    //    } catch (RemoteException e) {
    //    }
    //    sendCloseSystemWindows();
    //    startDockOrHome();
    //} else {
    //    // This code brings home to the front or, if it is already
    //    // at the front, puts the device to sleep.
    //    try {
    //        if (SystemProperties.getInt("persist.sys.uts-test-mode", 0) == 1) {
    //            /// Roll back EndcallBehavior as the cupcake design to pass P1 lab entry.
    //            Log.d(TAG, "UTS-TEST-MODE");
    //        } else {
    //            ActivityManagerNative.getDefault().stopAppSwitches();
    //            sendCloseSystemWindows();
    //            Intent dock = createHomeDockIntent();
    //            if (dock != NULL) {
    //                Int32 result = ActivityManagerNative.getDefault()
    //                    .startActivity(NULL, dock,
    //                    dock.resolveTypeIfNeeded(mContext.getContentResolver()),
    //                    NULL, 0, NULL, NULL, 0, TRUE /* onlyIfNeeded*/, FALSE);
    //                if (result == IActivityManager.START_RETURN_INTENT_TO_CALLER) {
    //                    return FALSE;
    //                }
    //            }
    //        }
    //        Int32 result = ActivityManagerNative.getDefault()
    //            .startActivity(NULL, mHomeIntent,
    //            mHomeIntent.resolveTypeIfNeeded(mContext.getContentResolver()),
    //            NULL, 0, NULL, NULL, 0, TRUE /* onlyIfNeeded*/, FALSE);
    //        if (result == IActivityManager.START_RETURN_INTENT_TO_CALLER) {
    //            return FALSE;
    //        }
    //    } catch (RemoteException ex) {
    //        // bummer, the activity manager, which is in this process, is dead
    //    }
    //}
    return TRUE;
}

ECode CPhoneWindowManager::SetCurrentOrientationLw(
    /* [in] */ Int32 newOrientation)
{
    Mutex::Autolock lock(mLock);

    if (newOrientation != mCurrentAppOrientation) {
        mCurrentAppOrientation = newOrientation;
        UpdateOrientationListenerLp();
    }

    return NOERROR;
}

ECode CPhoneWindowManager::PerformHapticFeedbackLw(
    /* [in] */ IWindowState* win,
    /* [in] */ Int32 effectId,
    /* [in] */ Boolean always,
    /* [out] */ Boolean* isSucceed)
{
    //final Boolean hapticsDisabled = Settings.System.getInt(mContext.getContentResolver(),
    //    Settings.System.HAPTIC_FEEDBACK_ENABLED, 0) == 0;
    //if (!always && (hapticsDisabled || mKeyguardMediator.isShowingAndNotHidden())) {
    //    return FALSE;
    //}
    //Int64[] pattern = NULL;
    //switch (effectId) {
    //        case HapticFeedbackConstants.LONG_PRESS:
    //            pattern = mLongPressVibePattern;
    //            break;
    //        case HapticFeedbackConstants.VIRTUAL_KEY:
    //            pattern = mVirtualKeyVibePattern;
    //            break;
    //        case HapticFeedbackConstants.KEYBOARD_TAP:
    //            pattern = mKeyboardTapVibePattern;
    //            break;
    //        case HapticFeedbackConstants.SAFE_MODE_DISABLED:
    //            pattern = mSafeModeDisabledVibePattern;
    //            break;
    //        case HapticFeedbackConstants.SAFE_MODE_ENABLED:
    //            pattern = mSafeModeEnabledVibePattern;
    //            break;
    //        default:
    //            return FALSE;
    //}
    //if (pattern.length == 1) {
    //    // One-shot vibration
    //    mVibrator.vibrate(pattern[0]);
    //} else {
    //    // Pattern vibration
    //    mVibrator.vibrate(pattern, -1);
    //}
    //return TRUE;
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindowManager::ScreenOnStoppedLw()
{
    //if (!mKeyguardMediator.isShowingAndNotHidden() && mPowerManager.isScreenOn()) {
    //    Int64 curTime = SystemClock.uptimeMillis();
    //    mPowerManager.userActivity(curTime, FALSE, LocalPowerManager.OTHER_EVENT);
    //}
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneWindowManager::AllowKeyRepeat(
    /* [out] */ Boolean* allowed)
{
    // disable key repeat when screen is off
    //
    if (!allowed) {
        return E_INVALID_ARGUMENT;
    }

    *allowed = mScreenOn;

    return NOERROR;
}
