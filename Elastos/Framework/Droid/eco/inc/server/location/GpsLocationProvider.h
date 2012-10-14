
#ifndef __GPSLOCATIONPROVIDER_H__
#define __GPSLOCATIONPROVIDER_H__

#include "server/location/LocationProviderInterface.h"
#include "location/GpsNetInitiatedHandler.h"
//#include "hardware/gps.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/HashMap.h>
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

class GpsLocationProvider : public LocationProviderInterface
{
private:
    static const CString TAG;// = "GpsLocationProvider";

    static const Boolean DEBUG = FALSE;
    static const Boolean VERBOSE = FALSE;

    // these need to match GpsPositionMode enum in gps.h
    static const Int32 GPS_POSITION_MODE_STANDALONE = 0;
    static const Int32 GPS_POSITION_MODE_MS_BASED = 1;
    static const Int32 GPS_POSITION_MODE_MS_ASSISTED = 2;

    // these need to match GpsPositionRecurrence enum in gps.h
    static const Int32 GPS_POSITION_RECURRENCE_PERIODIC = 0;
    static const Int32 GPS_POSITION_RECURRENCE_SINGLE = 1;

    // these need to match GpsStatusValue defines in gps.h
    static const Int32 GPS_STATUS_NONE = 0;
    static const Int32 GPS_STATUS_SESSION_BEGIN = 1;
    static const Int32 GPS_STATUS_SESSION_END = 2;
    static const Int32 GPS_STATUS_ENGINE_ON = 3;
    static const Int32 GPS_STATUS_ENGINE_OFF = 4;

    // these need to match GpsApgsStatusValue defines in gps.h
    /** AGPS status event values. */
    static const Int32 GPS_REQUEST_AGPS_DATA_CONN = 1;
    static const Int32 GPS_RELEASE_AGPS_DATA_CONN = 2;
    static const Int32 GPS_AGPS_DATA_CONNECTED = 3;
    static const Int32 GPS_AGPS_DATA_CONN_DONE = 4;
    static const Int32 GPS_AGPS_DATA_CONN_FAILED = 5;

    // these need to match GpsLocationFlags enum in gps.h
    static const Int32 LOCATION_INVALID = 0;
    static const Int32 LOCATION_HAS_LAT_LONG = 1;
    static const Int32 LOCATION_HAS_ALTITUDE = 2;
    static const Int32 LOCATION_HAS_SPEED = 4;
    static const Int32 LOCATION_HAS_BEARING = 8;
    static const Int32 LOCATION_HAS_ACCURACY = 16;

// IMPORTANT - the GPS_DELETE_* symbols here must match constants in gps.h
    static const Int32 GPS_DELETE_EPHEMERIS = 0x0001;
    static const Int32 GPS_DELETE_ALMANAC = 0x0002;
    static const Int32 GPS_DELETE_POSITION = 0x0004;
    static const Int32 GPS_DELETE_TIME = 0x0008;
    static const Int32 GPS_DELETE_IONO = 0x0010;
    static const Int32 GPS_DELETE_UTC = 0x0020;
    static const Int32 GPS_DELETE_HEALTH = 0x0040;
    static const Int32 GPS_DELETE_SVDIR = 0x0080;
    static const Int32 GPS_DELETE_SVSTEER = 0x0100;
    static const Int32 GPS_DELETE_SADATA = 0x0200;
    static const Int32 GPS_DELETE_RTI = 0x0400;
    static const Int32 GPS_DELETE_CELLDB_INFO = 0x8000;
    static const Int32 GPS_DELETE_ALL = 0xFFFF;

    // The GPS_CAPABILITY_* flags must match the values in gps.h
    static const Int32 GPS_CAPABILITY_SCHEDULING = 0x0000001;
    static const Int32 GPS_CAPABILITY_MSB = 0x0000002;
    static const Int32 GPS_CAPABILITY_MSA = 0x0000004;
    static const Int32 GPS_CAPABILITY_SINGLE_SHOT = 0x0000008;


    // these need to match AGpsType enum in gps.h
    static const Int32 AGPS_TYPE_SUPL = 1;
    static const Int32 AGPS_TYPE_C2K = 2;

    // for mAGpsDataConnectionState
    static const Int32 AGPS_DATA_CONNECTION_CLOSED = 0;
    static const Int32 AGPS_DATA_CONNECTION_OPENING = 1;
    static const Int32 AGPS_DATA_CONNECTION_OPEN = 2;

    // Handler messages
    static const Int32 CHECK_LOCATION = 1;
    static const Int32 ENABLE = 2;
    static const Int32 ENABLE_TRACKING = 3;
    static const Int32 UPDATE_NETWORK_STATE = 4;
    static const Int32 INJECT_NTP_TIME = 5;
    static const Int32 DOWNLOAD_XTRA_DATA = 6;
    static const Int32 UPDATE_LOCATION = 7;
    static const Int32 ADD_LISTENER = 8;
    static const Int32 REMOVE_LISTENER = 9;
    static const Int32 REQUEST_SINGLE_SHOT = 10;

    // Request setid
    static const Int32 AGPS_RIL_REQUEST_SETID_IMSI = 1;
    static const Int32 AGPS_RIL_REQUEST_SETID_MSISDN = 2;

    // Request ref location
    static const Int32 AGPS_RIL_REQUEST_REFLOC_CELLID = 1;
    static const Int32 AGPS_RIL_REQUEST_REFLOC_MAC = 2;

    // ref. location info
    static const Int32 AGPS_REF_LOCATION_TYPE_GSM_CELLID = 1;
    static const Int32 AGPS_REF_LOCATION_TYPE_UMTS_CELLID = 2;
    static const Int32 AGPS_REG_LOCATION_TYPE_MAC        = 3;

    // set id info
    static const Int32 AGPS_SETID_TYPE_NONE = 0;
    static const Int32 AGPS_SETID_TYPE_IMSI = 1;
    static const Int32 AGPS_SETID_TYPE_MSISDN = 2;

    static const CString PROPERTIES_FILE;// = "/etc/gps.conf";

    // turn off GPS fix icon if we haven't received a fix in 10 seconds
    static const Int64 RECENT_FIX_TIMEOUT = 10000;

    // stop trying if we do not receive a fix within 60 seconds
    static const Int32 NO_FIX_TIMEOUT = 60000;

    // Alarms
    static const CString ALARM_WAKEUP;
    static const CString ALARM_TIMEOUT;

    // Wakelocks
    static const CString WAKELOCK_KEY;

    // how often to request NTP time, in milliseconds
    // current setting 4 hours
    static const Int64 NTP_INTERVAL = 14400000;
    // how long to wait if we have a network error in NTP or XTRA downloading
    // current setting - 5 minutes
    static const Int64 RETRY_INTERVAL = 300000;

    // to avoid injecting bad NTP time, we reject any time fixes that differ from system time
    // by more than 5 minutes.
    static const Int64 MAX_NTP_SYSTEM_TIME_OFFSET = 300000;

    // for GPS SV statistics
    static const Int32 MAX_SVS = 32;
    static const Int32 EPHEMERIS_MASK = 0;
    static const Int32 ALMANAC_MASK = 1;
    static const Int32 USED_FOR_FIX_MASK = 2;

private:
    class MyGpsStatusProvider : public ElRefBase, IGpsStatusProvider
    {
    public:
        MyGpsStatusProvider(
            /* [in] */ GpsLocationProvider* gpsLocProvider);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI AddGpsStatusListener(
            /* [in] */ IGpsStatusListener* listener);

        CARAPI RemoveGpsStatusListener(
            /* [in] */ IGpsStatusListener* listener);

    private:
        GpsLocationProvider* mGpsLocationProvider;
    };

    class Listener : public ElRefBase, IBinderDeathRecipient
    {
    public:
        Listener(
            /* [in] */ GpsLocationProvider* gpsLocProvider,
            /* [in] */ IGpsStatusListener* listener);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI BinderDied();

    private:
        GpsLocationProvider* mGpsLocationProvider;

    public:
        AutoPtr<IGpsStatusListener> mListener;
        Int32 mSensors;
    };

    //=============================================================
    // NI Client support
    //=============================================================
    class NetInitiatedListener :  public ElRefBase, INetInitiatedListener
    {
    public:
        NetInitiatedListener(
            /* [in] */ GpsLocationProvider* gpsLocProvider);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        // Sends a response for an NI reqeust to HAL.
        CARAPI SendNiResponse(
            /* [in] */ Int32 notificationId,
            /* [in] */ Int32 userResponse,
            /* [out] */ Boolean* result);

    private:
        GpsLocationProvider* mGpsLocationProvider;
    };

private:
        static void* EntryRoutine(void *arg);

public:
    GpsLocationProvider(
        /* [in] */ IContext* context,
        /* [in] */ ILocationManager* locationManager);

    ~GpsLocationProvider();

    CARAPI_(AutoPtr<IGpsStatusProvider>) GetGpsStatusProvider();

    static CARAPI_(Boolean) IsSupported();

    CARAPI_(String) GetName();

    CARAPI_(Boolean) RequiresNetwork();

    CARAPI_(void) UpdateNetworkState(
        /* [in] */ Int32 state,
        /* [in] */ INetworkInfo* info);

    CARAPI_(void) UpdateLocation(
        /* [in] */ ILocation* location);

    CARAPI_(Boolean) RequiresSatellite();

    CARAPI_(Boolean) RequiresCell();

    CARAPI_(Boolean) HasMonetaryCost();

    CARAPI_(Boolean) SupportsAltitude();

    CARAPI_(Boolean) SupportsSpeed();

    CARAPI_(Boolean) SupportsBearing();

    CARAPI_(Int32) GetPowerRequirement();

    CARAPI_(Boolean) MeetsCriteria(
        /* [in] */ ICriteria* criteria);

    CARAPI_(Int32) GetAccuracy();

    CARAPI_(Boolean) IsEnabled();

    CARAPI_(void) Enable();

    CARAPI_(void) Disable();

    CARAPI_(Int32) GetStatus(
        /* [in] */ IBundle* extras);

    CARAPI_(Int64) GetStatusUpdateTime();

    CARAPI_(void) EnableLocationTracking(
        /* [in] */ Boolean enable);

    CARAPI_(Boolean) RequestSingleShotFix();

    CARAPI_(String) GetInternalState();

    CARAPI_(void) SetMinTime(
        /* [in] */ Int64 minTime,
        /* [in] */ IWorkSource* ws);

    CARAPI_(void) AddListener(
        /* [in] */ Int32 uid);

    CARAPI_(void) RemoveListener(
        /* [in] */ Int32 uid);

    CARAPI_(Boolean) SendExtraCommand(
        /* [in] */ String command,
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** outExtras);

    CARAPI_(NetInitiatedListener*) GetNetInitiatedListener();

    // Called by JNI function to report an NI request.
    CARAPI_(void) ReportNiNotification(
        /* [in] */ Int32 notificationId,
        /* [in] */ Int32 niType,
        /* [in] */ Int32 notifyFlags,
        /* [in] */ Int32 timeout,
        /* [in] */ Int32 defaultResponse,
        /* [in] */ String requestorId,
        /* [in] */ String text,
        /* [in] */ Int32 requestorIdEncoding,
        /* [in] */ Int32 textEncoding,
        /* [in] */ String extras);

private:
    CARAPI_(void) CheckSmsSuplInit(
        /* [in] */ IIntent* intent);

    CARAPI_(void) CheckWapSuplInit(
        /* [in] */ IIntent* intent);

    CARAPI_(void) Initialize();

    CARAPI_(void) HandleUpdateNetworkState(
        /* [in] */ Int32 state,
        /* [in] */ INetworkInfo* info);

    CARAPI_(void) HandleInjectNtpTime();

    CARAPI_(void) HandleDownloadXtraData();

    CARAPI_(void) HandleUpdateLocation(
        /* [in] */ ILocation* location);

    CARAPI_(void) HandleEnable();

    CARAPI_(void) HandleDisable();

    CARAPI_(void) UpdateStatus(
        /* [in] */ Int32 status,
        /* [in] */ Int32 svCount);

    CARAPI_(void) HandleEnableLocationTracking(
        /* [in] */ Boolean enable);

    CARAPI_(void) HandleRequestSingleShot();

    CARAPI_(void) HandleAddListener(
        /* [in] */ Int32 uid);

    CARAPI_(void) HandleRemoveListener(
        /* [in] */ Int32 uid);

    CARAPI_(Boolean) DeleteAidingData(
        /* [in] */ IBundle* extras);

    CARAPI_(void) StartNavigating(
        /* [in] */ Boolean singleShot);

    CARAPI_(void) StopNavigating();

    CARAPI_(void) Hibernate();

    CARAPI_(Boolean) HasCapability(
        /* [in] */ Int32 capability);

    /**
     * called from native code to update our position.
     */
    CARAPI_(void) ReportLocation(
        /* [in] */ Int32 flags,
        /* [in] */ Double latitude,
        /* [in] */ Double longitude,
        /* [in] */ Double altitude,
        /* [in] */ Float speed,
        /* [in] */ Float bearing,
        /* [in] */ Float accuracy,
        /* [in] */ Int64 timestamp);

    /**
     * called from native code to update our status
     */
    CARAPI_(void) ReportStatus(
        /* [in] */ Int32 status);

    /**
     * called from native code to update SV info
     */
    CARAPI_(void) ReportSvStatus();

    /**
     * called from native code to request XTRA data
     */
    CARAPI_(void) XtraDownloadRequest();

    /**
     * Called from native code to request set id info.
     * We should be careful about receiving null string from the TelephonyManager,
     * because sending null String to JNI function would cause a crash.
     */
    CARAPI_(void) RequestSetID(
        /* [in] */ Int32 flags);

    /**
     * Called from native code to request reference location info
     */
    CARAPI_(void) RequestRefLocation(
        /* [in] */ Int32 flags);

    CARAPI_(void) SendCallback(
        /* [in] */ Handle32 func,
        /* [in] */ Int32 message,
        /* [in] */ Int32 arg,
        /* [in] */ IInterface* value);

    CARAPI_(Boolean) Init();

    CARAPI_(void) Cleanup();

    CARAPI_(Boolean) SetPositionMode(
        /* [in] */ Int32 mode,
        /* [in] */ Int32 recurrence,
        /* [in] */ Int32 min_interval,
        /* [in] */ Int32 preferred_accuracy,
        /* [in] */ Int32 preferred_time);

    CARAPI_(Boolean) Start();

    CARAPI_(Boolean) Stop();

    CARAPI_(void) NativeDeleteAidingData(
        /* [in] */ Int32 flags);

    // returns number of SVs
    // mask[0] is ephemeris mask and mask[1] is almanac mask
    CARAPI_(Int32) ReadSvStatus(
        /* [in] */ const ArrayOf<Int32>& svs,
        /* [in] */ const ArrayOf<Float>& snrs,
        /* [in] */ const ArrayOf<Float>& elevations,
        /* [in] */ const ArrayOf<Float>& azimuths,
        /* [in] */ const ArrayOf<Int32>& masks);

    /**
     * called from native code to update AGPS status
     */
    CARAPI_(void) ReportAGpsStatus(
        /* [in] */ Int32 type,
        /* [in] */ Int32 status);

    /**
     * called from native code to report NMEA data received
     */
    CARAPI_(void) ReportNmea(
        /* [in] */ Int64 timestamp);

    /**
     * called from native code to inform us what the GPS engine capabilities are
     */
    CARAPI_(void) SetEngineCapabilities(
        /* [in] */ Int32 capabilities);

    CARAPI_(Int32) ReadNmea(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 bufferSize);

    CARAPI_(void) InjectLocation(
        /* [in] */ Double latitude,
        /* [in] */ Double longitude,
        /* [in] */ Float accuracy);

    CARAPI_(void) InjectTime(
        /* [in] */ Int64 time,
        /* [in] */ Int64 timeReference,
        /* [in] */ Int32 uncertainty);

    CARAPI_(Boolean) SupportsXtra();

    CARAPI_(void) InjectXtraData(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 length);

    // AGPS Support
    CARAPI_(void) AgpsDataConnOpen(
        /* [in] */ String apn);

    CARAPI_(void) AgpsDataConnClosed();

    CARAPI_(void) AgpsDataConnFailed();

    CARAPI_(void) AgpsNiMessage(
        /* [in] */ const ArrayOf<Byte>& msg,
        /* [in] */ Int32 length);

    CARAPI_(void) SetAgpsServer(
        /* [in] */ Int32 type,
        /* [in] */ String hostname,
        /* [in] */ Int32 port);

    // Network-initiated (NI) Support
    CARAPI_(void) SendNiResponse(
        /* [in] */ Int32 notificationId,
        /* [in] */ Int32 userResponse);

    // AGPS ril suport
    CARAPI_(void) AgpsSetRefLocationCellid(
        /* [in] */ Int32 type,
        /* [in] */ Int32 mcc,
        /* [in] */ Int32 mnc,
        /* [in] */ Int32 lac,
        /* [in] */ Int32 cid);

    CARAPI_(void) AgpsSetId(
        /* [in] */ Int32 type,
        /* [in] */ String setid);

    CARAPI_(void) NativeUpdateNetworkState(
        /* [in] */ Boolean connected,
        /* [in] */ Int32 type,
        /* [in] */ Boolean roaming,
        /* [in] */ String extraInfo);

protected:
//    static const GpsInterface* sGpsInterface;

private:
    Int32 mLocationFlags;

    // current status
    Int32 mStatus;

    // time for last status update
    Millisecond64 mStatusUpdateTime;

    // true if we are enabled
    Boolean mEnabled;

    // true if we have network connectivity
    Boolean mNetworkAvailable;

    // flags to trigger NTP or XTRA data download when network becomes available
    // initialized to true so we do NTP and XTRA when the network comes up after booting
    Boolean mInjectNtpTimePending;
    Boolean mDownloadXtraDataPending;

    // true if GPS is navigating
    Boolean mNavigating;

    // true if GPS engine is on
    Boolean mEngineOn;

    // requested frequency of fixes, in milliseconds
    Int32 mFixInterval;

    // true if we started navigation
    Boolean mStarted;

    // true if single shot request is in progress
    Boolean mSingleShot;

    // capabilities of the GPS engine
    Int32 mEngineCapabilities;

    // true if XTRA is supported
    Boolean mSupportsXtra;

    // for calculating time to first fix
    Int64 mFixRequestTime;
    // time to first fix for most recent session
    Int32 mTTFF;
    // time we received our last fix
    Int64 mLastFixTime;

    Int32 mPositionMode;

    // properties loaded from PROPERTIES_FILE
//    Properties mProperties;
    String mNtpServer;
    String mSuplServerHost;
    Int32 mSuplServerPort;
    String mC2KServerHost;
    Int32 mC2KServerPort;

    AutoPtr<IContext> mContext;
    AutoPtr<ILocationManager> mLocationManager;
    AutoPtr<ILocation> mLocation;
    Mutex mLocationLock;
    AutoPtr<IBundle> mLocationExtras;
    List<AutoPtr<Listener> > mListeners;
    Mutex mListenersLock;

    // GpsLocationProvider's handler thread
    pthread_t mThread;
    // Handler for processing events in mThread.
    AutoPtr<IApartment> mHandler;
    Mutex mHandlerLock;
    // Used to signal when our main thread has initialized everything
//    private final CountDownLatch mInitializedLatch = new CountDownLatch(1);

    String mAGpsApn;
    Int32 mAGpsDataConnectionState;
    AutoPtr<ILocalConnectivityManager> mConnMgr;
    GpsNetInitiatedHandler* mNIHandler;

//    PowerManager.WakeLock mWakeLock;
    // bitfield of pending messages to our Handler
    // used only for messages that cannot have multiple instances queued
    Int32 mPendingMessageBits;
    // separate counter for ADD_LISTENER and REMOVE_LISTENER messages,
    // which might have multiple instances queued
    Int32 mPendingListenerMessages;

    // Alarms
//    AlarmManager mAlarmManager;
    AutoPtr<IPendingIntent> mWakeupIntent;
    AutoPtr<IPendingIntent> mTimeoutIntent;

    AutoPtr<IBatteryStats> mBatteryStats;
    HashMap<Int32, Int32> mClientUids;

    MyGpsStatusProvider* mGpsStatusProvider;
//    BroadcastReceiver mBroadcastReciever = new BroadcastReceiver();
    NetInitiatedListener* mNetInitiatedListener;

    // preallocated arrays, to avoid memory allocation in reportStatus()
    ArrayOf<Int32>* mSvs;
    ArrayOf<Float>* mSnrs;
    ArrayOf<Float>* mSvElevations;
    ArrayOf<Float>* mSvAzimuths;
    ArrayOf<Int32>* mSvMasks;
    Int32 mSvCount;

    // preallocated to avoid memory allocation in reportNmea()
    ArrayOf<Byte>* mNmeaBuffer;
};

#endif //__GPSLOCATIONPROVIDER_H__
