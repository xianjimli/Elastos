
#ifndef __CBATTERYSERVICE_H__
#define __CBATTERYSERVICE_H__

#include "_CBatteryService.h"
#include "ext/frameworkext.h"
#include "os/UEventObserver.h"
#include <elastos/Mutex.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;
using namespace Elastos::Core;

/**
 * <p>BatteryService monitors the charging status, and charge level of the device
 * battery.  When these values change this service broadcasts the new values
 * to all {@link android.content.BroadcastReceiver IntentReceivers} that are
 * watching the {@link android.content.Intent#ACTION_BATTERY_CHANGED
 * BATTERY_CHANGED} action.</p>
 * <p>The new values are stored in the Intent data and can be retrieved by
 * calling {@link android.content.Intent#getExtra Intent.getExtra} with the
 * following keys:</p>
 * <p>&quot;scale&quot; - int, the maximum value for the charge level</p>
 * <p>&quot;level&quot; - int, charge level, from 0 through &quot;scale&quot; inclusive</p>
 * <p>&quot;status&quot; - String, the current charging status.<br />
 * <p>&quot;health&quot; - String, the current battery health.<br />
 * <p>&quot;present&quot; - boolean, true if the battery is present<br />
 * <p>&quot;icon-small&quot; - int, suggested small icon to use for this state</p>
 * <p>&quot;plugged&quot; - int, 0 if the device is not plugged in; 1 if plugged
 * into an AC power adapter; 2 if plugged in via USB.</p>
 * <p>&quot;voltage&quot; - int, current battery voltage in millivolts</p>
 * <p>&quot;temperature&quot; - int, current battery temperature in tenths of
 * a degree Centigrade</p>
 * <p>&quot;technology&quot; - String, the type of battery installed, e.g. "Li-ion"</p>
 */
CarClass(CBatteryService)
{
public:
    class UEventObserverEx : public UEventObserver
    {
    public:
        //@Override
        CARAPI OnUEvent(
            /* [in] */ UEventObserver::UEvent * event);

        CARAPI_(void) SetBatteryService(
            /* [in] */ CBatteryService * batteryService);
    private:
        CBatteryService * mBatteryService;
    };

    //from jni codes
    struct PowerSupplyPaths {
        char* acOnlinePath;
        char* usbOnlinePath;
        char* batteryStatusPath;
        char* batteryHealthPath;
        char* batteryPresentPath;
        char* batteryCapacityPath;
        char* batteryVoltagePath;
        char* batteryTemperaturePath;
        char* batteryTechnologyPath;
    };

    //from jni codes
    struct BatteryManagerConstants {
        Int32 statusUnknown;
        Int32 statusCharging;
        Int32 statusDischarging;
        Int32 statusNotCharging;
        Int32 statusFull;
        Int32 healthUnknown;
        Int32 healthGood;
        Int32 healthOverheat;
        Int32 healthDead;
        Int32 healthOverVoltage;
        Int32 healthUnspecifiedFailure;
    };

    CARAPI GetDescription(
        /* [out] */ String * description);

    CARAPI IsPowered(
        /* [out] */ Boolean * powered);

    CARAPI IsPoweredEx(
        /* [in] */ Int32 plugTypeSet,
        /* [out] */ Boolean * powered);

    CARAPI GetPlugType(
        /* [out] */ Int32 * plugType);

    CARAPI GetBatteryLevel(
        /* [out] */ Int32 * batteryLevel);

    CARAPI SystemReady();

    CARAPI constructor(
        /* [in] */ IContext * context);

    CBatteryService();

    virtual ~CBatteryService();

protected:
    //@Override
    CARAPI_(void) Dump(
        /* [in] */ IFileDescriptor *fd,
        /* [in] */ IPrintWriter * pw,
        /* [in] */ const ArrayOf<String> &args);

    //private synchronized final void update();
    CARAPI_(void) Update();

private:
    static const CString TAG;// = BatteryService.class.getSimpleName();

    static const Boolean LOCAL_LOGV;// = false;

    // Used locally for determining when to make a last ditch effort to log
    // discharge stats before the device dies.
    static const Int32 CRITICAL_BATTERY_LEVEL;// = 4;

    static const Int32 DUMP_MAX_LENGTH;// = 24 * 1024;
    //private static final String[] DUMPSYS_ARGS = new String[] { "--checkin", "-u" };
    ArrayOf<String> * DUMPSYS_ARGS;

    static const CString BATTERY_STATS_SERVICE_NAME;// = "batteryinfo";

    static const CString DUMPSYS_DATA_PATH;// = "/data/system/";

    // This should probably be exposed in the API, though it's not critical
    static const Int32 BATTERY_PLUGGED_NONE;// = 0;

    AutoPtr<IContext> mContext;
    AutoPtr<IBatteryStats> mBatteryStats;

    Boolean mAcOnline;
    Boolean mUsbOnline;
    Int32 mBatteryStatus;
    Int32 mBatteryHealth;
    Boolean mBatteryPresent;
    Int32 mBatteryLevel;
    Int32 mBatteryVoltage;
    Int32 mBatteryTemperature;
    String mBatteryTechnology;
    Boolean mBatteryLevelCritical;

    Int32 mLastBatteryStatus;
    Int32 mLastBatteryHealth;
    Boolean mLastBatteryPresent;
    Int32 mLastBatteryLevel;
    Int32 mLastBatteryVoltage;
    Int32 mLastBatteryTemperature;
    Boolean mLastBatteryLevelCritical;

    Int32 mLowBatteryWarningLevel;
    Int32 mLowBatteryCloseWarningLevel;

    Int32 mPlugType;
    Int32 mLastPlugType;// = -1; // Extra state so we can detect first run

    Int64 mDischargeStartTime;
    Int32 mDischargeStartLevel;

    Boolean mSentLowBatteryBroadcast;// = false;

    UEventObserverEx * mUEventObserver;

    Mutex mutexLock;

    CARAPI_(void) ShutdownIfNoPower();

    CARAPI_(void) ShutdownIfOverTemp();

    //private native void native_update();
    CARAPI_(void) Native_update();

    CARAPI_(void) SendIntent();

    CARAPI LogBatteryStats();

    CARAPI_(void) LogOutlier(
        /* [in] */ Int64 duration);

    CARAPI_(Int32) GetIcon(
        /* [in] */ Int32 level);
private:
    //for jni codes
    static const CString POWER_SUPPLY_PATH;
    Int32 mVoltageDivisor;// = 1;
    PowerSupplyPaths mPaths;
    BatteryManagerConstants mConstants;

    CARAPI_(Int32) NativeGetBatteryStatus(
        /* [in] */ const char* status);

    CARAPI_(Int32) NativeGetBatteryHealth(
        /* [in] */ const char* status);

    CARAPI_(Int32) NativeReadFromFile(
        /* [in] */ const char* path,
        /* [in, out] */ char* buf,
        /* [in] */ Int32 size);

    CARAPI_(Boolean) NativeSetBooleanField(
        /* [in] */ const char* path);

    CARAPI_(Int32) NativeSetIntField(
        /* [in] */ const char* path);

    CARAPI_(Int32) NativeSetVoltageField(
        /* [in] */ const char* path);

    CARAPI_(void) NativeUpdate();

    CARAPI NativeRegister();
};

#endif // __CBATTERYSERVICE_H__
