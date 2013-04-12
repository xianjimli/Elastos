
#include <ext/frameworkext.h>
#include "server/CBatteryService.h"
#include <Elastos.Framework.Core.h>
#include "server/CBatteryStatsService.h"
#include "app/ActivityManagerNative.h"
#include "server/CActivityManagerService.h"
#include "os/FileUtils.h"
#include "os/ServiceManager.h"
#include "os/SystemClock.h"
#include <Slogger.h>
#include <Logger.h>
#include <StringBuffer.h>

//for jni
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

const CString CBatteryService::TAG = "BatteryService";
const Boolean CBatteryService::LOCAL_LOGV = FALSE;
// Used locally for determining when to make a last ditch effort to log
// discharge stats before the device dies.
const Int32 CBatteryService::CRITICAL_BATTERY_LEVEL = 4;
const Int32 CBatteryService::DUMP_MAX_LENGTH = 24 * 1024;
const CString CBatteryService::BATTERY_STATS_SERVICE_NAME = "batteryinfo";
const CString CBatteryService::DUMPSYS_DATA_PATH = "/data/system/";
// This should probably be exposed in the API, though it's not critical
const Int32 CBatteryService::BATTERY_PLUGGED_NONE = 0;


///////////////////////////////////////////////////////////////////////////
//jni codes
//////////////////////////////////////////////////////////////////////////
const CString CBatteryService::POWER_SUPPLY_PATH = "/sys/class/power_supply";

Int32 CBatteryService::NativeGetBatteryStatus(
    /* [in] */ const char* status)
{
    assert(status != NULL);
    switch (status[0]) {
        case 'C': return mConstants.statusCharging;         // Charging
        case 'D': return mConstants.statusDischarging;      // Discharging
        case 'F': return mConstants.statusFull;             // Not charging
        case 'N': return mConstants.statusNotCharging;      // Full
        case 'U': return mConstants.statusUnknown;          // Unknown

        default: {
            String log = String("Unknown battery status ")+ String(status);
            Slogger::W(TAG,CString(log.string()) );
            return mConstants.statusUnknown;
        }
    }
}

Int32 CBatteryService::NativeGetBatteryHealth(
    /* [in] */ const char* status)
{
    assert(status != NULL);
    switch (status[0]) {
        case 'D': return mConstants.healthDead;         // Dead
        case 'G': return mConstants.healthGood;         // Good
        case 'O': {
            if (strcmp(status, "Overheat") == 0) {
                return mConstants.healthOverheat;
            } else if (strcmp(status, "Over voltage") == 0) {
                return mConstants.healthOverVoltage;
            }
            String log = String("Unknown battery health[1] ")+ String(status);
            Slogger::W(TAG,CString(log.string()) );
            return mConstants.healthUnknown;
        }

        case 'U': {
            if (strcmp(status, "Unspecified failure") == 0) {
                return mConstants.healthUnspecifiedFailure;
            } else if (strcmp(status, "Unknown") == 0) {
                return mConstants.healthUnknown;
            }
            // fall through
        }

        default: {
            String log = String("Unknown battery health[2] ") + String(status);
            Slogger::W(TAG,CString(log.string()) );
            return mConstants.healthUnknown;
        }
    }
}

Int32 CBatteryService::NativeReadFromFile(
    /* [in] */ const char* path,
    /* [in, out] */ char* buf,
    /* [in] */ Int32 size)
{
    if (!path)
        return -1;
    Int32 fd = open(path, O_RDONLY, 0);
    if (fd == -1) {
        String log = String("Could not open ")+String(path);
        Slogger::E(TAG,CString(log.string()) );
        return -1;
    }

    Int32 count = (Int32) read(fd, buf, size);
    if (count > 0) {
        count = (count < size) ? count : size - 1;
        while (count > 0 && buf[count-1] == '\n') count--;
        buf[count] = '\0';
    } else {
        buf[0] = '\0';
    }

    close(fd);
    return count;
}

Boolean CBatteryService::NativeSetBooleanField(
    /* [in] */ const char* path)
{
    const Int32 SIZE = 16;
    char buf[SIZE];

    Boolean value = FALSE;
    if (NativeReadFromFile(path, buf, SIZE) > 0) {
        if (buf[0] == '1') {
            value = TRUE;
        }
    }
    return value;
}

Int32 CBatteryService::NativeSetIntField(
    /* [in] */ const char* path)
{
    const Int32 SIZE = 128;
    char buf[SIZE];

    Int32 value = 0;
    if (NativeReadFromFile(path, buf, SIZE) > 0) {
        value = atoi(buf);
    }
    return value;
}

Int32 CBatteryService::NativeSetVoltageField(
    /* [in] */ const char* path)
{
    const Int32 SIZE = 128;
    char buf[SIZE];

    Int32 value = 0;
    if (NativeReadFromFile(path, buf, SIZE) > 0) {
        value = atoi(buf);
        value /= mVoltageDivisor;
    }
    return value;
}

void CBatteryService::NativeUpdate()
{
    mAcOnline = NativeSetBooleanField(mPaths.acOnlinePath);
    mUsbOnline = NativeSetBooleanField(mPaths.usbOnlinePath);
    mBatteryPresent = NativeSetBooleanField(mPaths.batteryPresentPath);

    mBatteryLevel = NativeSetIntField(mPaths.batteryCapacityPath);
    mBatteryVoltage = NativeSetVoltageField(mPaths.batteryVoltagePath);
    mBatteryTemperature = NativeSetIntField(mPaths.batteryTemperaturePath);

    const Int32 SIZE = 128;
    char buf[SIZE];

    if (NativeReadFromFile(mPaths.batteryStatusPath, buf, SIZE) > 0)
        mBatteryStatus = NativeGetBatteryStatus(buf);
    else
        mBatteryStatus = mConstants.statusUnknown;

    if (NativeReadFromFile(mPaths.batteryHealthPath, buf, SIZE) > 0)
        mBatteryHealth = NativeGetBatteryHealth(buf);

    if (NativeReadFromFile(mPaths.batteryTechnologyPath, buf, SIZE) > 0)
        mBatteryTechnology = String(buf);
}

//int register_android_server_BatteryService(JNIEnv* env)
ECode CBatteryService::NativeRegister()
{
    mVoltageDivisor = 1;
    char path[PATH_MAX];
    struct dirent* entry;

    DIR* dir = opendir(POWER_SUPPLY_PATH);
    if (dir == NULL) {
        String log = String("Could not open ")+String(POWER_SUPPLY_PATH) +String(" \n");
        Slogger::E(TAG, CString(log.string()) );
        return E_INVALID_ARGUMENT;
    }
    while ((entry = readdir(dir))) {
        const char* name = entry->d_name;

        // ignore "." and ".."
        if (name[0] == '.' && (name[1] == 0 || (name[1] == '.' && name[2] == 0))) {
            continue;
        }

        char buf[20];
        // Look for "type" file in each subdirectory
        snprintf(path, sizeof(path), "%s/%s/type",(const char *) POWER_SUPPLY_PATH, name);
        int length = NativeReadFromFile(path, buf, sizeof(buf));
        if (length > 0) {
            if (buf[length - 1] == '\n')
                buf[length - 1] = 0;

            if (strcmp(buf, "Mains") == 0) {
                snprintf(path, sizeof(path), "%s/%s/online", (const char *) POWER_SUPPLY_PATH, name);
                if (access(path, R_OK) == 0)
                    mPaths.acOnlinePath = strdup(path);
            }
            else if (strcmp(buf, "USB") == 0) {
                snprintf(path, sizeof(path), "%s/%s/online", (const char *) POWER_SUPPLY_PATH, name);
                if (access(path, R_OK) == 0)
                    mPaths.usbOnlinePath = strdup(path);
            }
            else if (strcmp(buf, "Battery") == 0) {
                snprintf(path, sizeof(path), "%s/%s/status", (const char *) POWER_SUPPLY_PATH, name);
                if (access(path, R_OK) == 0)
                    mPaths.batteryStatusPath = strdup(path);
                snprintf(path, sizeof(path), "%s/%s/health", (const char *) POWER_SUPPLY_PATH, name);
                if (access(path, R_OK) == 0)
                    mPaths.batteryHealthPath = strdup(path);
                snprintf(path, sizeof(path), "%s/%s/present", (const char *) POWER_SUPPLY_PATH, name);
                if (access(path, R_OK) == 0)
                    mPaths.batteryPresentPath = strdup(path);
                snprintf(path, sizeof(path), "%s/%s/capacity", (const char *) POWER_SUPPLY_PATH, name);
                if (access(path, R_OK) == 0)
                    mPaths.batteryCapacityPath = strdup(path);

                snprintf(path, sizeof(path), "%s/%s/voltage_now", (const char *) POWER_SUPPLY_PATH, name);
                if (access(path, R_OK) == 0) {
                    mPaths.batteryVoltagePath = strdup(path);
                    // voltage_now is in microvolts, not millivolts
                    mVoltageDivisor = 1000;
                } else {
                    snprintf(path, sizeof(path), "%s/%s/batt_vol", (const char *) POWER_SUPPLY_PATH, name);
                    if (access(path, R_OK) == 0)
                        mPaths.batteryVoltagePath = strdup(path);
                }

                snprintf(path, sizeof(path), "%s/%s/temp", (const char *) POWER_SUPPLY_PATH, name);
                if (access(path, R_OK) == 0) {
                    mPaths.batteryTemperaturePath = strdup(path);
                } else {
                    snprintf(path, sizeof(path), "%s/%s/batt_temp", (const char *) POWER_SUPPLY_PATH, name);
                    if (access(path, R_OK) == 0)
                        mPaths.batteryTemperaturePath = strdup(path);
                }

                snprintf(path, sizeof(path), "%s/%s/technology", (const char *) POWER_SUPPLY_PATH, name);
                if (access(path, R_OK) == 0)
                    mPaths.batteryTechnologyPath = strdup(path);
            }
        }
    }
    closedir(dir);

    if (!mPaths.acOnlinePath)
        Slogger::E(TAG,CString("acOnlinePath not found"));
    if (!mPaths.usbOnlinePath)
        Slogger::E(TAG,CString("usbOnlinePath not found"));
    if (!mPaths.batteryStatusPath)
        Slogger::E(TAG,CString("batteryStatusPath not found"));
    if (!mPaths.batteryHealthPath)
        Slogger::E(TAG,CString("batteryHealthPath not found"));
    if (!mPaths.batteryPresentPath)
        Slogger::E(TAG,CString("batteryPresentPath not found"));
    if (!mPaths.batteryCapacityPath)
        Slogger::E(TAG,CString("batteryCapacityPath not found"));
    if (!mPaths.batteryVoltagePath)
        Slogger::E(TAG,CString("batteryVoltagePath not found"));
    if (!mPaths.batteryTemperaturePath)
        Slogger::E(TAG,CString("batteryTemperaturePath not found"));
    if (!mPaths.batteryTechnologyPath)
        Slogger::E(TAG,CString("batteryTechnologyPath not found"));

    mConstants.statusUnknown = BatteryManager_BATTERY_STATUS_UNKNOWN;
    mConstants.statusCharging = BatteryManager_BATTERY_STATUS_CHARGING;
    mConstants.statusDischarging = BatteryManager_BATTERY_STATUS_DISCHARGING;
    mConstants.statusNotCharging = BatteryManager_BATTERY_STATUS_NOT_CHARGING;
    mConstants.statusFull = BatteryManager_BATTERY_STATUS_FULL;
    mConstants.healthUnknown = BatteryManager_BATTERY_HEALTH_UNKNOWN;
    mConstants.healthGood = BatteryManager_BATTERY_HEALTH_GOOD;
    mConstants.healthOverheat = BatteryManager_BATTERY_HEALTH_OVERHEAT;
    mConstants.healthDead = BatteryManager_BATTERY_HEALTH_DEAD;
    mConstants.healthOverVoltage = BatteryManager_BATTERY_HEALTH_OVER_VOLTAGE;
    mConstants.healthUnspecifiedFailure = BatteryManager_BATTERY_HEALTH_UNSPECIFIED_FAILURE;

    return NOERROR;
}


/**
 * <p>BatteryService monitors the charging status, and charge level of the device
 * battery.  When these values change this service broadcasts the new values
 * to all {@link android.content.BroadcastReceiver IntentReceivers} that are
 * watching the {@link android.content.Intent#ACTION_BATTERY_CHANGED
 * BATTERY_CHANGED} action.</p>
 * <p>The new values are stored in the Intent data and can be retrieved by
 * calling {@link android.content.Intent#getExtra Intent.getExtra} with the
 * following keys:</p>
 * <p>&quot;scale&quot; - Int32, the maximum value for the charge level</p>
 * <p>&quot;level&quot; - Int32, charge level, from 0 through &quot;scale&quot; inclusive</p>
 * <p>&quot;status&quot; - String, the current charging status.<br />
 * <p>&quot;health&quot; - String, the current battery health.<br />
 * <p>&quot;present&quot; - Boolean, TRUE if the battery is present<br />
 * <p>&quot;icon-small&quot; - Int32, suggested small icon to use for this state</p>
 * <p>&quot;plugged&quot; - Int32, 0 if the device is not plugged in; 1 if plugged
 * into an AC power adapter; 2 if plugged in via USB.</p>
 * <p>&quot;voltage&quot; - Int32, current battery voltage in millivolts</p>
 * <p>&quot;temperature&quot; - Int32, current battery temperature in tenths of
 * a degree Centigrade</p>
 * <p>&quot;technology&quot; - String, the type of battery installed, e.g. "Li-ion"</p>
 */
CBatteryService::CBatteryService()
{
    ArrayOf<String>::Free(DUMPSYS_ARGS);
}

CBatteryService::~CBatteryService()
{
    delete mUEventObserver;
}

ECode CBatteryService::GetDescription(
    /* [out] */ String * description)
{
    VALIDATE_NOT_NULL(description);
    return E_NOT_IMPLEMENTED;
}

ECode CBatteryService::constructor(
    /* [in] */ IContext * context)
{
    VALIDATE_NOT_NULL(context);
    //private static final String[] DUMPSYS_ARGS = new String[] { "--checkin", "-u" };
    DUMPSYS_ARGS = ArrayOf<String>::Alloc(2);
    (*DUMPSYS_ARGS)[0] = String("--checkin");
    (*DUMPSYS_ARGS)[1] = String("-u");

    mLastPlugType = -1; // Extra state so we can detect first run

    mSentLowBatteryBroadcast = FALSE;

    mUEventObserver = new UEventObserverEx();
    mUEventObserver->SetBatteryService((CBatteryService*)this);

    mContext = context;
    context->Release();

    //??
    //mBatteryStats = BatteryStatsService.getService();
    //how to fill this parameter? original java codes does not with this
    //parameter.
    String filename = String(""); //<--//??
    CBatteryStatsService::New(filename,(IBatteryStats **)&mBatteryStats);

    AutoPtr<IResources> res;
    ECode ec = mContext->GetResources((IResources**)&res);
    if (FAILED(ec)) {
        return ec;
    }

    res->GetInteger(0x010e000b, //R::integer::config_lowBatteryWarningLevel //com.android.internal.R.integer.config_lowBatteryWarningLevel
        &mLowBatteryWarningLevel);

    res->GetInteger(0x010e000c, //R::integer::config_lowBatteryCloseWarningLevel //com.android.internal.R.integer.config_lowBatteryCloseWarningLevel
        &mLowBatteryCloseWarningLevel);

    mUEventObserver->StartObserving(String("SUBSYSTEM=power_supply"));

    ec = NativeRegister();
    if (FAILED(ec)) {
        return ec;
    }
    // set initial status
    Update();
}

ECode CBatteryService::IsPowered(
    /* [out] */ Boolean * powered)
{
    VALIDATE_NOT_NULL(powered);
    // assume we are powered if battery state is unknown so the "stay on while plugged in" option will work.
    *powered = (mAcOnline || mUsbOnline || mBatteryStatus == BatteryManager_BATTERY_STATUS_UNKNOWN);
    return NOERROR;
}

ECode CBatteryService::IsPoweredEx(
    /* [in] */ Int32 plugTypeSet,
    /* [out] */ Boolean * powered)
{
    VALIDATE_NOT_NULL(powered);
    // assume we are powered if battery state is unknown so
    // the "stay on while plugged in" option will work.
    if (mBatteryStatus == BatteryManager_BATTERY_STATUS_UNKNOWN) {
        *powered = TRUE;
        return NOERROR;
    }
    if (plugTypeSet == 0) {
        *powered = FALSE;
        return NOERROR;
    }
    Int32 plugTypeBit = 0;
    if (mAcOnline) {
        plugTypeBit |= BatteryManager_BATTERY_PLUGGED_AC;
    }
    if (mUsbOnline) {
        plugTypeBit |= BatteryManager_BATTERY_PLUGGED_USB;
    }
    *powered = (plugTypeSet & plugTypeBit) != 0;
    return NOERROR;
}

ECode CBatteryService::GetPlugType(
    /* [out] */ Int32* plugType)
{
    VALIDATE_NOT_NULL(plugType);
    *plugType = mPlugType;
    return NOERROR;
}

ECode CBatteryService::UEventObserverEx::OnUEvent(
    /* [in] */ UEventObserver::UEvent* event)
{
    mBatteryService->Update();
    return NOERROR;
}

void CBatteryService::UEventObserverEx::SetBatteryService(
    /* [in] */ CBatteryService* batteryService)
{
    assert(batteryService != NULL);
    mBatteryService = batteryService;
}

// returns battery level as a percentage
ECode CBatteryService::GetBatteryLevel(
    /* [out] */ Int32* batteryLevel)
{
    VALIDATE_NOT_NULL(batteryLevel);
    *batteryLevel = mBatteryLevel;
    return NOERROR;
}

ECode CBatteryService::SystemReady()
{
    // check our power situation now that it is safe to display the shutdown dialog.
    ShutdownIfNoPower();
    ShutdownIfOverTemp();
    return NOERROR;
}

void CBatteryService::ShutdownIfNoPower()
{
    // shut down gracefully if our battery is critically low and we are not powered.
    // wait until the system has booted before attempting to display the shutdown dialog.
    Boolean powered;
    IsPowered(&powered);

    //??Not sure if it works
    CActivityManagerService *ams = NULL;
    ActivityManagerNative::GetDefault((IActivityManager **)&ams);
    Boolean isSystemReady =((CActivityManagerService*) ams)->TestIsSystemReady();

    //??
    //if (mBatteryLevel == 0 && !isPowered() && ActivityManagerNative.isSystemReady()) {
    if (mBatteryLevel == 0 && !powered && isSystemReady) {
        IIntent *intent = NULL;
        CIntent::New(String(Intent_ACTION_REQUEST_SHUTDOWN), &intent);
        intent->PutBooleanExtra(String(Intent_EXTRA_KEY_CONFIRM), FALSE);
        intent->SetFlags(Intent_FLAG_ACTIVITY_NEW_TASK);
        mContext->StartActivity(intent);
        intent->Release();
    }

    ams->Release();
}

void CBatteryService::ShutdownIfOverTemp() {
    // shut down gracefully if temperature is too high (> 68.0C)
    // wait until the system has booted before attempting to display the shutdown dialog.

    //??Not sure if it works
    CActivityManagerService *ams = NULL;
    ActivityManagerNative::GetDefault((IActivityManager **)&ams);
    Boolean isSystemReady =((CActivityManagerService*) ams)->TestIsSystemReady();

    //??
    //if (mBatteryTemperature > 680 && ActivityManagerNative.isSystemReady())
    if (mBatteryTemperature > 680 && isSystemReady) {
        IIntent * intent = NULL;
        CIntent::New(String(Intent_ACTION_REQUEST_SHUTDOWN), &intent);
        intent->PutBooleanExtra(String(Intent_EXTRA_KEY_CONFIRM), FALSE);
        intent->SetFlags(Intent_FLAG_ACTIVITY_NEW_TASK);
        mContext->StartActivity(intent);
        intent->Release();
    }

    ams->Release();
}

//private native
void CBatteryService::Native_update()
{
    NativeUpdate();
}

//private synchronized final
void CBatteryService::Update() {
    Mutex::Autolock l(mutexLock);
    Native_update();

    Boolean logOutlier = FALSE;
    Int64 dischargeDuration = 0;

    mBatteryLevelCritical = (mBatteryLevel <= CRITICAL_BATTERY_LEVEL);
    if (mAcOnline) {
        mPlugType = BatteryManager_BATTERY_PLUGGED_AC;
    } else if (mUsbOnline) {
        mPlugType = BatteryManager_BATTERY_PLUGGED_USB;
    } else {
        mPlugType = BATTERY_PLUGGED_NONE;
    }

    // Let the battery stats keep track of the current level.
    //??
    /*
    try {
        mBatteryStats.setBatteryState(mBatteryStatus, mBatteryHealth,
                mPlugType, mBatteryLevel, mBatteryTemperature,
                mBatteryVoltage);
    } catch (RemoteException e) {
        // Should never happen.
    }
    */

    ShutdownIfNoPower();
    ShutdownIfOverTemp();

    if (mBatteryStatus != mLastBatteryStatus ||
            mBatteryHealth != mLastBatteryHealth ||
            mBatteryPresent != mLastBatteryPresent ||
            mBatteryLevel != mLastBatteryLevel ||
            mPlugType != mLastPlugType ||
            mBatteryVoltage != mLastBatteryVoltage ||
            mBatteryTemperature != mLastBatteryTemperature) {

        if (mPlugType != mLastPlugType) {
            if (mLastPlugType == BATTERY_PLUGGED_NONE) {
                // discharging -> charging

                // There's no value in this data unless we've discharged at least once and the
                // battery level has changed; so don't log until it does.
                if (mDischargeStartTime != 0 && mDischargeStartLevel != mBatteryLevel) {
                    dischargeDuration = SystemClock::GetElapsedRealtime() - mDischargeStartTime;
                    logOutlier = TRUE;

                    //??
                    //EventLog.writeEvent(EventLogTags.BATTERY_DISCHARGE, dischargeDuration,
                    //        mDischargeStartLevel, mBatteryLevel);
                    // make sure we see a discharge event before logging again
                    mDischargeStartTime = 0;
                }
            } else if (mPlugType == BATTERY_PLUGGED_NONE) {
                // charging -> discharging or we just powered up
                mDischargeStartTime = SystemClock::GetElapsedRealtime();
                mDischargeStartLevel = mBatteryLevel;
            }
        }
        if (mBatteryStatus != mLastBatteryStatus ||
                mBatteryHealth != mLastBatteryHealth ||
                mBatteryPresent != mLastBatteryPresent ||
                mPlugType != mLastPlugType) {
            //??
            //EventLog.writeEvent(EventLogTags.BATTERY_STATUS,
            //        mBatteryStatus, mBatteryHealth, mBatteryPresent ? 1 : 0,
            //        mPlugType, mBatteryTechnology);
        }
        if (mBatteryLevel != mLastBatteryLevel ||
                mBatteryVoltage != mLastBatteryVoltage ||
                mBatteryTemperature != mLastBatteryTemperature) {
            //??
            //EventLog.writeEvent(EventLogTags.BATTERY_LEVEL,
            //        mBatteryLevel, mBatteryVoltage, mBatteryTemperature);
        }
        if (mBatteryLevelCritical && !mLastBatteryLevelCritical &&
                mPlugType == BATTERY_PLUGGED_NONE) {
            // We want to make sure we log discharge cycle outliers
            // if the battery is about to die.
            dischargeDuration = SystemClock::GetElapsedRealtime() - mDischargeStartTime;
            logOutlier = TRUE;
        }

        Boolean plugged = (mPlugType != BATTERY_PLUGGED_NONE);
        Boolean oldPlugged = (mLastPlugType != BATTERY_PLUGGED_NONE);

        /* The ACTION_BATTERY_LOW broadcast is sent in these situations:
         * - is just un-plugged (previously was plugged) and battery level is
         *   less than or equal to WARNING, or
         * - is not plugged and battery level falls to WARNING boundary
         *   (becomes <= mLowBatteryWarningLevel).
         */
        Boolean sendBatteryLow = !plugged
            && mBatteryStatus != BatteryManager_BATTERY_STATUS_UNKNOWN
            && mBatteryLevel <= mLowBatteryWarningLevel
            && (oldPlugged || mLastBatteryLevel > mLowBatteryWarningLevel);

        SendIntent();

        // Separate broadcast is sent for power connected / not connected
        // since the standard intent will not wake any applications and some
        // applications may want to have smart behavior based on this.
        IIntent * statusIntent = NULL;
        CIntent::New(&statusIntent);
        statusIntent->SetFlags(Intent_FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
        if (mPlugType != 0 && mLastPlugType == 0) {
            statusIntent->SetAction(String(Intent_ACTION_POWER_CONNECTED));
            mContext->SendBroadcast(statusIntent);
        }
        else if (mPlugType == 0 && mLastPlugType != 0) {
            statusIntent->SetAction(String(Intent_ACTION_POWER_DISCONNECTED));
            mContext->SendBroadcast(statusIntent);
        }

        if (sendBatteryLow) {
            mSentLowBatteryBroadcast = TRUE;
            statusIntent->SetAction(String(Intent_ACTION_BATTERY_LOW));
            mContext->SendBroadcast(statusIntent);
        } else if (mSentLowBatteryBroadcast && mLastBatteryLevel >= mLowBatteryCloseWarningLevel) {
            mSentLowBatteryBroadcast = FALSE;
            statusIntent->SetAction(String(Intent_ACTION_BATTERY_OKAY));
            mContext->SendBroadcast(statusIntent);
        }

        statusIntent->Release();
        // This needs to be done after sendIntent() so that we get the lastest battery stats.
        if (logOutlier && dischargeDuration != 0) {
            LogOutlier(dischargeDuration);
        }

        mLastBatteryStatus = mBatteryStatus;
        mLastBatteryHealth = mBatteryHealth;
        mLastBatteryPresent = mBatteryPresent;
        mLastBatteryLevel = mBatteryLevel;
        mLastPlugType = mPlugType;
        mLastBatteryVoltage = mBatteryVoltage;
        mLastBatteryTemperature = mBatteryTemperature;
        mLastBatteryLevelCritical = mBatteryLevelCritical;
    }
}

//private final
void CBatteryService::SendIntent() {
    //  Pack up the values and broadcast them to everyone
    IIntent *intent = NULL;
    CIntent::New(String(Intent_ACTION_BATTERY_CHANGED), &intent);
    intent->AddFlags(Intent_FLAG_RECEIVER_REGISTERED_ONLY
            | Intent_FLAG_RECEIVER_REPLACE_PENDING);

    Int32 icon = GetIcon(mBatteryLevel);

    intent->PutInt32Extra(String(BatteryManager_EXTRA_STATUS), mBatteryStatus);
    intent->PutInt32Extra(String(BatteryManager_EXTRA_HEALTH), mBatteryHealth);
    intent->PutBooleanExtra(String(BatteryManager_EXTRA_PRESENT), mBatteryPresent);
    intent->PutInt32Extra(String(BatteryManager_EXTRA_LEVEL), mBatteryLevel);
    intent->PutInt32Extra(String(BatteryManager_EXTRA_SCALE), BatteryService_BATTERY_SCALE);
    intent->PutInt32Extra(String(BatteryManager_EXTRA_ICON_SMALL), icon);
    intent->PutInt32Extra(String(BatteryManager_EXTRA_PLUGGED), mPlugType);
    intent->PutInt32Extra(String(BatteryManager_EXTRA_VOLTAGE), mBatteryVoltage);
    intent->PutInt32Extra(String(BatteryManager_EXTRA_TEMPERATURE), mBatteryTemperature);
    intent->PutStringExtra(String(BatteryManager_EXTRA_TECHNOLOGY), mBatteryTechnology);

    if (FALSE) {
        //Slog.d
        String log = String("updateBattery level:") + String::FromInt32(mBatteryLevel) +
                String(" scale:") + String::FromInt32(BatteryService_BATTERY_SCALE) +
                String(" status:") + String::FromInt32(mBatteryStatus) +
                String(" health:") + String::FromInt32(mBatteryHealth) +
                String(" present:") + String::FromBoolean(mBatteryPresent) +
                String(" voltage: ") + String::FromInt32(mBatteryVoltage) +
                String(" temperature: ") + String::FromInt32(mBatteryTemperature) +
                String(" technology: ") + mBatteryTechnology +
                String(" AC powered:") + String::FromBoolean(mAcOnline) +
                String(" USB powered:") + String::FromBoolean(mUsbOnline) +
                String(" icon:") + String::FromInt32(icon);
        Slogger::D(TAG, CString(log.string()) );
    }

    //??
    //ActivityManagerNative.BroadcastStickyIntent(intent, null);

    intent->Release();
}

//private final
ECode CBatteryService::LogBatteryStats()
{
    //IBinder* batteryInfoService = NULL;
    //batteryInfoService = ServiceManager::GetService(String(BATTERY_STATS_SERVICE_NAME));
    //if (batteryInfoService == NULL) {
    //    return NOERROR;
    //}

    //??
    //DropBoxManager db = (DropBoxManager) mContext.getSystemService(Context.DROPBOX_SERVICE);
    //if (db == null || !db.isTagEnabled("BATTERY_DISCHARGE_INFO")) return;

    IFile * dumpFile = NULL;
    IFileOutputStream  *dumpStream = NULL;
    //try {
        // dump the service to a file
    String str = String(DUMPSYS_DATA_PATH) + String(BATTERY_STATS_SERVICE_NAME) + String(".dump");
    ECode ec = CFile::New(str, &dumpFile);
    if (FAILED(ec)) {
        String log = String("failed to dump battery service CFile::New error ")+str;
        Slogger::E(TAG,CString(log.string()) );
        return ec;
    }

    ec = CFileOutputStream::New(dumpFile, &dumpStream);
    if (FAILED(ec)) {
        String log = String("failed to dump battery service CFileOutputStream::New error")+str;
        Slogger::E(TAG,CString(log.string()) );
        return ec;
    }
    Dump(NULL, NULL,(*DUMPSYS_ARGS));

    if (!FileUtils::Sync(dumpStream))
    {
        String log = String("failed to write dumpsys file")+str;
        Slogger::E(TAG,CString(log.string()) );
        return E_IO;
    }
    // add dump file to drop box
    //??
    //db.addFile("BATTERY_DISCHARGE_INFO", dumpFile, DropBoxManager.IS_TEXT);

    // make sure we clean up
    if (dumpStream != NULL) {
        dumpStream->Close();
        Slogger::E(TAG, CString("failed to close dumpsys output stream"));
    }

    Boolean b;
    dumpFile->Delete(&b);
    if (dumpFile != NULL && !b) {
        String absPath;
        dumpFile->GetAbsolutePath(&absPath);
        String log = String("failed to delete temporary dumpsys file: ") + absPath;
        Slogger::E(TAG, CString(log.string()) );
    }
    return NOERROR;

    /*
        IBinder batteryInfoService = ServiceManager.getService(BATTERY_STATS_SERVICE_NAME);
        if (batteryInfoService == null) return;

        DropBoxManager db = (DropBoxManager) mContext.getSystemService(Context.DROPBOX_SERVICE);
        if (db == null || !db.isTagEnabled("BATTERY_DISCHARGE_INFO")) return;

        File dumpFile = null;
        FileOutputStream dumpStream = null;
        try {
            // dump the service to a file
            dumpFile = new File(DUMPSYS_DATA_PATH + BATTERY_STATS_SERVICE_NAME + ".dump");
            dumpStream = new FileOutputStream(dumpFile);
            batteryInfoService.dump(dumpStream.getFD(), DUMPSYS_ARGS);
            FileUtils.sync(dumpStream);

            // add dump file to drop box
            db.addFile("BATTERY_DISCHARGE_INFO", dumpFile, DropBoxManager.IS_TEXT);
        } catch (RemoteException e) {
            Slog.e(TAG, "failed to dump battery service", e);
        } catch (IOException e) {
            Slog.e(TAG, "failed to write dumpsys file", e);
        } finally {
            // make sure we clean up
            if (dumpStream != null) {
                try {
                    dumpStream.close();
                } catch (IOException e) {
                    Slog.e(TAG, "failed to close dumpsys output stream");
                }
            }
            if (dumpFile != null && !dumpFile.delete()) {
                Slog.e(TAG, "failed to delete temporary dumpsys file: "
                        + dumpFile.getAbsolutePath());
            }
        }
     */
}

//private final
void CBatteryService::LogOutlier(
    /* [in] */ Int64 duration)
{
    IContentResolver * cr = NULL;
    mContext->GetContentResolver(&cr);
    String dischargeThresholdString = String("");
    String durationThresholdString = String("");
    ISettingsSecure *ss = NULL;
    //??
    //CSettingsSecure::New(&ss);
    CSettingsSecure::AcquireSingleton(&ss);
    if (ss != NULL) {
        ss->GetString(cr,
                String(SettingsSecure_BATTERY_DISCHARGE_THRESHOLD),
                &dischargeThresholdString);
        ss->GetString(cr,
                String(SettingsSecure_BATTERY_DISCHARGE_DURATION_THRESHOLD),
                &durationThresholdString);

        ss->Release();
    }
    cr->Release();

    if (!dischargeThresholdString.IsNullOrEmpty() && !durationThresholdString.IsNullOrEmpty()) {
        Int64 durationThreshold = durationThresholdString.ToInt64();
        Int32 dischargeThreshold = dischargeThresholdString.ToInt32();
        if (duration <= durationThreshold &&
            mDischargeStartLevel - mBatteryLevel >= dischargeThreshold) {
            // If the discharge cycle is bad enough we want to know about it.
            LogBatteryStats();
        }
        if (LOCAL_LOGV) {
            //Slog.v
            String log =  String("duration threshold: ") + durationThresholdString +
                    String(" discharge threshold: ") + dischargeThresholdString;
            Slogger::V(TAG, CString(log.string()) );
        }

        if (LOCAL_LOGV) {
            //Slog.v
            String log = String("duration: ") + String::FromInt64(duration) +
                    String(" discharge: ") + String::FromInt32( mDischargeStartLevel- mBatteryLevel);
            Slogger::V(TAG, CString(log.string()) );
        }
    }
}

//private final
Int32 CBatteryService::GetIcon(Int32 level)
{
    if (mBatteryStatus == BatteryManager_BATTERY_STATUS_CHARGING) {
        return 0x010802ac; //R::drawable::stat_sys_battery_charge; //com.android.internal.R.drawable.stat_sys_battery_charge;
    } else if (mBatteryStatus == BatteryManager_BATTERY_STATUS_DISCHARGING ||
            mBatteryStatus == BatteryManager_BATTERY_STATUS_NOT_CHARGING ||
            mBatteryStatus == BatteryManager_BATTERY_STATUS_FULL) {
        return 0x01080296; //R::drawable::stat_sys_battery; //com.android.internal.R.drawable.stat_sys_battery;
    } else {
        return 0x010802b3; //R::drawable::stat_sys_battery_unknown;//com.android.internal.R.drawable.stat_sys_battery_unknown;
    }
}

//@Override
//protected
void CBatteryService::Dump(
    /* [in] */ IFileDescriptor * fd,
    /* [in] */ IPrintWriter * pw,
    /* [in] */ const ArrayOf<String> &args)
{
    //??
    //if (mContext.checkCallingOrSelfPermission(android.Manifest.permission.DUMP)
    //        != PackageManager.PERMISSION_GRANTED) {

    //    pw.println("Permission Denial: can't dump Battery service from from pid="
    //            + Binder.getCallingPid()
    //            + ", uid=" + Binder.getCallingUid());
    //    return;
    //}

    //synchronized (this) {
    //    pw.println("Current Battery Service state:");
    //    pw.println("  AC powered: " + mAcOnline);
    //    pw.println("  USB powered: " + mUsbOnline);
    //    pw.println("  status: " + mBatteryStatus);
    //    pw.println("  health: " + mBatteryHealth);
    //    pw.println("  present: " + mBatteryPresent);
    //    pw.println("  level: " + mBatteryLevel);
    //    pw.println("  scale: " + BatteryService_BATTERY_SCALE);
    //    pw.println("  voltage:" + mBatteryVoltage);
    //    pw.println("  temperature: " + mBatteryTemperature);
    //    pw.println("  technology: " + mBatteryTechnology);
    //}

    Slogger::I(TAG, StringBuffer("Current Battery Service state:"));
    Slogger::I(TAG, StringBuffer("  AC powered: ") + String::FromBoolean(mAcOnline));
    Slogger::I(TAG, StringBuffer("  USB powered: ") + String::FromBoolean(mUsbOnline));
    Slogger::I(TAG, StringBuffer("  status: ") + String::FromInt32(mBatteryStatus));
    Slogger::I(TAG, StringBuffer("  health: ") + String::FromInt32(mBatteryHealth));
    Slogger::I(TAG, StringBuffer("  present: ") + String::FromBoolean(mBatteryPresent));
    Slogger::I(TAG, StringBuffer("  level: ") + String::FromInt32(mBatteryLevel));
    Slogger::I(TAG, StringBuffer("  scale: ") + String::FromInt32(BatteryService_BATTERY_SCALE));
    Slogger::I(TAG, StringBuffer("  voltage:") + String::FromInt32(mBatteryVoltage));
    Slogger::I(TAG, StringBuffer("  temperature: ") + String::FromInt32(mBatteryTemperature));
    Slogger::I(TAG, StringBuffer("  technology: ") + mBatteryTechnology);

}

