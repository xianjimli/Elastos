
#include "server/location/GpsLocationProvider.h"
#include "server/CServiceManager.h"
#include "content/CIntentFilter.h"
#include "content/CIntent.h"
#include "os/CBundle.h"
#include "os/SystemClock.h"
#include "location/CLocation.h"
#include "location/internal/GpsNetInitiatedHandler.h"
#include "net/SntpClient.h"
#include "utils/CApartment.h"
#include "server/location/GpsXtraDownloader.h"
#include <elastos/System.h>

using namespace Elastos::Core;

const CString GpsLocationProvider::TAG = "GpsLocationProvider";
const Boolean GpsLocationProvider::DEBUG;
const Boolean GpsLocationProvider::VERBOSE;
const Int32 GpsLocationProvider::GPS_POSITION_MODE_STANDALONE;
const Int32 GpsLocationProvider::GPS_POSITION_MODE_MS_BASED;
const Int32 GpsLocationProvider::GPS_POSITION_MODE_MS_ASSISTED;
const Int32 GpsLocationProvider::GPS_POSITION_RECURRENCE_PERIODIC;
const Int32 GpsLocationProvider::GPS_POSITION_RECURRENCE_SINGLE;
const Int32 GpsLocationProvider::GPS_STATUS_NONE;
const Int32 GpsLocationProvider::GPS_STATUS_SESSION_BEGIN;
const Int32 GpsLocationProvider::GPS_STATUS_SESSION_END;
const Int32 GpsLocationProvider::GPS_STATUS_ENGINE_ON;
const Int32 GpsLocationProvider::GPS_STATUS_ENGINE_OFF;
const Int32 GpsLocationProvider::GPS_REQUEST_AGPS_DATA_CONN;
const Int32 GpsLocationProvider::GPS_RELEASE_AGPS_DATA_CONN;
const Int32 GpsLocationProvider::GPS_AGPS_DATA_CONNECTED;
const Int32 GpsLocationProvider::GPS_AGPS_DATA_CONN_DONE;
const Int32 GpsLocationProvider::GPS_AGPS_DATA_CONN_FAILED;
const Int32 GpsLocationProvider::LOCATION_INVALID;
const Int32 GpsLocationProvider::LOCATION_HAS_LAT_LONG;
const Int32 GpsLocationProvider::LOCATION_HAS_ALTITUDE;
const Int32 GpsLocationProvider::LOCATION_HAS_SPEED;
const Int32 GpsLocationProvider::LOCATION_HAS_BEARING;
const Int32 GpsLocationProvider::LOCATION_HAS_ACCURACY;
const Int32 GpsLocationProvider::GPS_DELETE_EPHEMERIS;
const Int32 GpsLocationProvider::GPS_DELETE_ALMANAC;
const Int32 GpsLocationProvider::GPS_DELETE_POSITION;
const Int32 GpsLocationProvider::GPS_DELETE_TIME;
const Int32 GpsLocationProvider::GPS_DELETE_IONO;
const Int32 GpsLocationProvider::GPS_DELETE_UTC;
const Int32 GpsLocationProvider::GPS_DELETE_HEALTH;
const Int32 GpsLocationProvider::GPS_DELETE_SVDIR;
const Int32 GpsLocationProvider::GPS_DELETE_SVSTEER;
const Int32 GpsLocationProvider::GPS_DELETE_SADATA;
const Int32 GpsLocationProvider::GPS_DELETE_RTI;
const Int32 GpsLocationProvider::GPS_DELETE_CELLDB_INFO;
const Int32 GpsLocationProvider::GPS_DELETE_ALL;
const Int32 GpsLocationProvider::GPS_CAPABILITY_SCHEDULING;
const Int32 GpsLocationProvider::GPS_CAPABILITY_MSB;
const Int32 GpsLocationProvider::GPS_CAPABILITY_MSA;
const Int32 GpsLocationProvider::GPS_CAPABILITY_SINGLE_SHOT;
const Int32 GpsLocationProvider::AGPS_TYPE_SUPL;
const Int32 GpsLocationProvider::AGPS_TYPE_C2K;
const Int32 GpsLocationProvider::AGPS_DATA_CONNECTION_CLOSED;
const Int32 GpsLocationProvider::AGPS_DATA_CONNECTION_OPENING;
const Int32 GpsLocationProvider::AGPS_DATA_CONNECTION_OPEN;
const Int32 GpsLocationProvider::CHECK_LOCATION;
const Int32 GpsLocationProvider::ENABLE;
const Int32 GpsLocationProvider::ENABLE_TRACKING;
const Int32 GpsLocationProvider::UPDATE_NETWORK_STATE;
const Int32 GpsLocationProvider::INJECT_NTP_TIME;
const Int32 GpsLocationProvider::DOWNLOAD_XTRA_DATA;
const Int32 GpsLocationProvider::UPDATE_LOCATION;
const Int32 GpsLocationProvider::ADD_LISTENER;
const Int32 GpsLocationProvider::REMOVE_LISTENER;
const Int32 GpsLocationProvider::REQUEST_SINGLE_SHOT;
const Int32 GpsLocationProvider::AGPS_RIL_REQUEST_SETID_IMSI;
const Int32 GpsLocationProvider::AGPS_RIL_REQUEST_SETID_MSISDN;
const Int32 GpsLocationProvider::AGPS_RIL_REQUEST_REFLOC_CELLID;
const Int32 GpsLocationProvider::AGPS_RIL_REQUEST_REFLOC_MAC;
const Int32 GpsLocationProvider::AGPS_REF_LOCATION_TYPE_GSM_CELLID;
const Int32 GpsLocationProvider::AGPS_REF_LOCATION_TYPE_UMTS_CELLID;
const Int32 GpsLocationProvider::AGPS_REG_LOCATION_TYPE_MAC;
const Int32 GpsLocationProvider::AGPS_SETID_TYPE_NONE;
const Int32 GpsLocationProvider::AGPS_SETID_TYPE_IMSI;
const Int32 GpsLocationProvider::AGPS_SETID_TYPE_MSISDN;
const CString GpsLocationProvider::PROPERTIES_FILE = "/etc/gps.conf";
const Int64 GpsLocationProvider::RECENT_FIX_TIMEOUT;
const Int32 GpsLocationProvider::NO_FIX_TIMEOUT;
const CString GpsLocationProvider::ALARM_WAKEUP = "com.elastos.internal.location.ALARM_WAKEUP";
const CString GpsLocationProvider::ALARM_TIMEOUT = "com.elastos.internal.location.ALARM_TIMEOUT";
const CString GpsLocationProvider::WAKELOCK_KEY = "GpsLocationProvider";
const Int64 GpsLocationProvider::NTP_INTERVAL;
const Int64 GpsLocationProvider::RETRY_INTERVAL;
const Int64 GpsLocationProvider::MAX_NTP_SYSTEM_TIME_OFFSET;
const Int32 GpsLocationProvider::MAX_SVS;
const Int32 GpsLocationProvider::EPHEMERIS_MASK;
const Int32 GpsLocationProvider::ALMANAC_MASK;
const Int32 GpsLocationProvider::USED_FOR_FIX_MASK;

//const GpsInterface* GpsLocationProvider::sGpsInterface = NULL;

PInterface GpsLocationProvider::MyGpsStatusProvider::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IGpsStatusProvider) {
        return (IGpsStatusProvider*)this;
    }

    return NULL;
}

UInt32 GpsLocationProvider::MyGpsStatusProvider::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 GpsLocationProvider::MyGpsStatusProvider::Release()
{
    return ElRefBase::Release();
}

ECode GpsLocationProvider::MyGpsStatusProvider::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

GpsLocationProvider::MyGpsStatusProvider::MyGpsStatusProvider(
    /* [in] */ GpsLocationProvider* gpsLocProvider)
    : mGpsLocationProvider(gpsLocProvider)
{
}

ECode GpsLocationProvider::MyGpsStatusProvider::AddGpsStatusListener(
    /* [in] */ IGpsStatusListener* listener)
{
    if (listener == NULL) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException("listener is null in addGpsStatusListener");
    }

    Mutex::Autolock lock(mGpsLocationProvider->mListenersLock);
//    IBinder binder = listener.asBinder();
    List<AutoPtr<Listener> >::Iterator it;
    for (it = mGpsLocationProvider->mListeners.Begin(); it != mGpsLocationProvider->mListeners.End(); ++it) {
        AutoPtr<Listener> test = *it;
        if (listener == test->mListener) {
            // listener already added
            return NOERROR;
        }
    }

    AutoPtr<Listener> l = new Listener(mGpsLocationProvider, listener);
//    binder.linkToDeath(l, 0);
    mGpsLocationProvider->mListeners.PushBack(l);

    return NOERROR;
}

ECode GpsLocationProvider::MyGpsStatusProvider::RemoveGpsStatusListener(
    /* [in] */ IGpsStatusListener* listener)
{
    if (listener == NULL) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException("listener is null in addGpsStatusListener");
    }

    Mutex::Autolock lock(mGpsLocationProvider->mListenersLock);

//    IBinder binder = listener.asBinder();
    AutoPtr<Listener> l;
    List<AutoPtr<Listener> >::Iterator it;
    for (it = mGpsLocationProvider->mListeners.Begin();
            it != mGpsLocationProvider->mListeners.End() && l == NULL; ++it) {
        AutoPtr<Listener> test = *it;
        if (listener == test->mListener) {
            l = test;
        }
    }

    if (l != NULL) {
        mGpsLocationProvider->mListeners.Remove(l);
//        binder.unlinkToDeath(l, 0);
    }
}

AutoPtr<IGpsStatusProvider> GpsLocationProvider::GetGpsStatusProvider()
{
    return AutoPtr<IGpsStatusProvider>((IGpsStatusProvider*)mGpsStatusProvider);
}

void GpsLocationProvider::CheckSmsSuplInit(
    /* [in] */ IIntent* intent)
{
}

void GpsLocationProvider::CheckWapSuplInit(
    /* [in] */ IIntent* intent)
{
//    byte[] supl_init = (byte[]) intent->GetByteExtra(String("data"), );
//    native_agps_ni_message(supl_init,supl_init.length);
}

Boolean GpsLocationProvider::IsSupported()
{
    return FALSE;
//    return (sGpsInterface != NULL || get_gps_interface() != NULL);
}

GpsLocationProvider::GpsLocationProvider(
    /* [in] */ IContext* context,
    /* [in] */ ILocationManager* locationManager)
    : mLocationFlags(LOCATION_INVALID)
    , mStatus(LocationProvider_TEMPORARILY_UNAVAILABLE)
    , mEnabled(FALSE)
    , mNetworkAvailable(FALSE)
    , mInjectNtpTimePending(TRUE)
    , mDownloadXtraDataPending(TRUE)
    , mNavigating(FALSE)
    , mEngineOn(FALSE)
    , mFixInterval(1000)
    , mStarted(FALSE)
    , mSingleShot(FALSE)
    , mEngineCapabilities(0)
    , mSupportsXtra(FALSE)
    , mFixRequestTime(0)
    , mTTFF(0)
    , mLastFixTime(0)
    , mPositionMode(0)
    , mSuplServerPort(0)
    , mC2KServerPort(0)
    , mContext(context)
    , mLocationManager(locationManager)
    , mPendingMessageBits(0)
    , mPendingListenerMessages(0)
    , mSvCount(0)
{
    mStatusUpdateTime = SystemClock::GetElapsedRealtime();
    ASSERT_SUCCEEDED(CLocation::New(String(LocationManager_GPS_PROVIDER), (ILocation**)&mLocation));
    ASSERT_SUCCEEDED(CBundle::New((IBundle**)&mLocationExtras));
    mGpsStatusProvider = new MyGpsStatusProvider(this);
    mNetInitiatedListener = new NetInitiatedListener(this);

    mNIHandler = new GpsNetInitiatedHandler(context);

    mSvs = ArrayOf<Int32>::Alloc(MAX_SVS);
    mSnrs = ArrayOf<Float>::Alloc(MAX_SVS);
    mSvElevations = ArrayOf<Float>::Alloc(MAX_SVS);
    mSvAzimuths = ArrayOf<Float>::Alloc(MAX_SVS);
    mSvMasks = ArrayOf<Int32>::Alloc(3);
    mNmeaBuffer = ArrayOf<Byte>::Alloc(120);

    mLocation->SetExtras(mLocationExtras);

    // Create a wake lock
//    PowerManager powerManager = (PowerManager) mContext.getSystemService(Context.POWER_SERVICE);
//    mWakeLock = powerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, WAKELOCK_KEY);
//    mWakeLock.setReferenceCounted(false);

//    mAlarmManager = (AlarmManager)mContext.getSystemService(Context.ALARM_SERVICE);
//    mWakeupIntent = PendingIntent.getBroadcast(mContext, 0, new Intent(ALARM_WAKEUP), 0);
//    mTimeoutIntent = PendingIntent.getBroadcast(mContext, 0, new Intent(ALARM_TIMEOUT), 0);

//    AutoPtr<IIntentFilter> intentFilter;
//    CIntentFilter::New((IIntentFilter**)&intentFilter);
//    intentFilter->AddAction(Intents_DATA_SMS_RECEIVED_ACTION);
//    intentFilter->AddDataScheme(String("sms"));
//    intentFilter->AddDataAuthority(String("localhost"),String("7275"));
//    context->RegisterReceiver(mBroadcastReciever, intentFilter);

//    CIntentFilter::New((IIntentFilter**)&intentFilter);
//    intentFilter->AddAction(Intents_WAP_PUSH_RECEIVED_ACTION);
//    try {
//    intentFilter->AddDataType(String("application/vnd.omaloc-supl-init"));
//    } catch (IntentFilter.MalformedMimeTypeException e) {
//        Log.w(TAG, "Malformed SUPL init mime type");
//    }
//    context->RegisterReceiver(mBroadcastReciever, intentFilter);

//    mConnMgr = (ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE);

    // Battery statistics service to be notified when GPS turns on or off
    AutoPtr<IServiceManager> serviceManager;
    CServiceManager::AcquireSingleton((IServiceManager**)&serviceManager);
    serviceManager->GetService(String("batteryinfo"), (IInterface**)&mBatteryStats);

//    mProperties = new Properties();
//    try {
    AutoPtr<IFile> file;
    CFile::New(String(PROPERTIES_FILE), (IFile**)&file);
    AutoPtr<IFileInputStream> stream;
    CFileInputStream::New(file, (IFileInputStream**)&stream);
//    mProperties.load(stream);
    stream->Close();
//    mNtpServer = mProperties.getProperty("NTP_SERVER", null);

//    mSuplServerHost = mProperties.getProperty("SUPL_HOST");
    String portString;// = mProperties.getProperty("SUPL_PORT");
    if (!mSuplServerHost.IsNull() && !portString.IsNull()) {
//        try {
        mSuplServerPort = portString.ToInt32()/*Integer.parseInt(portString)*/;
//        } catch (NumberFormatException e) {
//            Log.e(TAG, "unable to parse SUPL_PORT: " + portString);
//        }
    }

//    mC2KServerHost = mProperties.getProperty("C2K_HOST");
//    portString = mProperties.getProperty("C2K_PORT");
    if (!mC2KServerHost.IsNull() && !portString.IsNull()) {
//        try {
        mC2KServerPort = portString.ToInt32()/*Integer.parseInt(portString)*/;
//        } catch (NumberFormatException e) {
//            Log.e(TAG, "unable to parse C2K_PORT: " + portString);
//        }
    }
//    } catch (IOException e) {
//        Log.w(TAG, "Could not open GPS configuration file " + PROPERTIES_FILE);
//    }

    // wait until we are fully initialized before returning
    pthread_create(&mThread, NULL, EntryRoutine, (void*)this);
//    while (TRUE) {
//        try {
//        mInitializedLatch.await();
//            break;
//        } catch (InterruptedException e) {
//            Thread.currentThread().interrupt();
//        }
//    }
}

GpsLocationProvider::~GpsLocationProvider()
{
    ArrayOf<Int32>::Free(mSvs);
    ArrayOf<Float>::Free(mSnrs);
    ArrayOf<Float>::Free(mSvElevations);
    ArrayOf<Float>::Free(mSvAzimuths);
    ArrayOf<Int32>::Free(mSvMasks);
}

void* GpsLocationProvider::EntryRoutine(void *arg)
{
    if (arg == NULL) {
        pthread_exit((void*)E_THREAD_ABORTED);
    }

    GpsLocationProvider* p = (GpsLocationProvider*)arg;

    CApartment::New(TRUE, (IApartment**)(&(p->mHandler)));
    p->mHandler->Start(ApartmentAttr_Current);

//    Process.setThreadPriority(Process.THREAD_PRIORITY_BACKGROUND);
    p->Initialize();
//    Looper.prepare();
//    mHandler = new ProviderHandler();
//    // signal when we are initialized and ready to go
//    mInitializedLatch.countDown();
//    Looper.loop();
    return (void*)NOERROR;
}

void GpsLocationProvider::Initialize()
{
    // register our receiver on our thread rather than the main thread
    AutoPtr<IIntentFilter> intentFilter;
    CIntentFilter::New((IIntentFilter**)&intentFilter);
    intentFilter->AddAction(String(ALARM_WAKEUP));
    intentFilter->AddAction(String(ALARM_TIMEOUT));
//    mContext.registerReceiver(mBroadcastReciever, intentFilter);
}

String GpsLocationProvider::GetName()
{
    return String(LocationManager_GPS_PROVIDER);
}

Boolean GpsLocationProvider::RequiresNetwork()
{
    return TRUE;
}

void GpsLocationProvider::UpdateNetworkState(
    /* [in] */ Int32 state,
    /* [in] */ INetworkInfo* info)
{
    void (STDCALL GpsLocationProvider::*pHandlerFunc)(Int32, INetworkInfo*);
    pHandlerFunc = &GpsLocationProvider::HandleUpdateNetworkState;
    SendCallback(*(Handle32*)&pHandlerFunc, UPDATE_NETWORK_STATE, state, (IInterface*)info);
}

void GpsLocationProvider::HandleUpdateNetworkState(
    /* [in] */ Int32 state,
    /* [in] */ INetworkInfo* info)
{
    mNetworkAvailable = (state == LocationProvider_AVAILABLE);

//    if (DEBUG) {
//        Log.d(TAG, "updateNetworkState " + (mNetworkAvailable ? "available" : "unavailable")
//            + " info: " + info);
//    }

    if (info != NULL) {
        Boolean isConnected, roaming;
        Int32 type;
        String extraInfo;
        info->IsConnected(&isConnected);
        info->GetType(&type);
        info->IsRoaming(&roaming);
        info->GetExtraInfo(&extraInfo);
        NativeUpdateNetworkState(isConnected, type, roaming, extraInfo);
    }

    if (info != NULL) {
        Int32 type;
        info->GetType(&type);
        if (type == ConnectivityManager_TYPE_MOBILE_SUPL
                && mAGpsDataConnectionState == AGPS_DATA_CONNECTION_OPENING) {
            String apnName(NULL);
            info->GetExtraInfo(&apnName);
            if (mNetworkAvailable && !apnName.IsNull() && apnName.GetLength() > 0) {
                mAGpsApn = apnName;
//                if (DEBUG) Log.d(TAG, "call native_agps_data_conn_open");
                AgpsDataConnOpen(apnName);
                mAGpsDataConnectionState = AGPS_DATA_CONNECTION_OPEN;
            }
            else {
//                if (DEBUG) Log.d(TAG, "call native_agps_data_conn_failed");
                mAGpsApn = NULL;
                mAGpsDataConnectionState = AGPS_DATA_CONNECTION_CLOSED;
                AgpsDataConnFailed();
            }
        }
    }

    if (mNetworkAvailable) {
        if (mInjectNtpTimePending) {
            void (STDCALL GpsLocationProvider::*pHandlerFunc)();
            pHandlerFunc = &GpsLocationProvider::HandleInjectNtpTime;
            mHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, NULL, 0);
        }
        if (mDownloadXtraDataPending) {
            void (STDCALL GpsLocationProvider::*pHandlerFunc)();
            pHandlerFunc = &GpsLocationProvider::HandleDownloadXtraData;
            mHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, NULL, 0);
        }
    }
}

void GpsLocationProvider::HandleInjectNtpTime()
{
    if (!mNetworkAvailable) {
        // try again when network is up
        mInjectNtpTimePending = TRUE;
        return;
    }
    mInjectNtpTimePending = FALSE;

    SntpClient* client = new SntpClient();
    Int64 delay;

    if (client->RequestTime(mNtpServer, 10000)) {
        Int64 time = client->GetNtpTime();
        Int64 timeReference = client->GetNtpTimeReference();
        Int32 certainty = (Int32)(client->GetRoundTripTime() / 2);
//        Int64 now = System::GetCurrentTimeMillis();

//        Log.d(TAG, "NTP server returned: "
//                + time + " (" + new Date(time)
//                + ") reference: " + timeReference
//                + " certainty: " + certainty
//                + " system time offset: " + (time - now));

        InjectTime(time, timeReference, certainty);
        delay = NTP_INTERVAL;
    }
    else {
//        if (DEBUG) Log.d(TAG, "requestTime failed");
        delay = RETRY_INTERVAL;
    }

    // send delayed message for next NTP injection
    // since this is delayed and not urgent we do not hold a wake lock here
    void (STDCALL GpsLocationProvider::*pHandlerFunc)();
    pHandlerFunc = &GpsLocationProvider::HandleInjectNtpTime;
    mHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);
    mHandler->PostCppCallbackDelayed((Handle32)this, *(Handle32*)&pHandlerFunc, NULL, 0, delay);
}

void GpsLocationProvider::HandleDownloadXtraData()
{
    if (!mNetworkAvailable) {
        // try again when network is up
        mDownloadXtraDataPending = TRUE;
        return;
    }
    mDownloadXtraDataPending = FALSE;


    GpsXtraDownloader* xtraDownloader = new GpsXtraDownloader(mContext/*, mProperties*/);
    ArrayOf<Byte>* data = xtraDownloader->DownloadXtraData();
    if (data != NULL) {
//        if (DEBUG) {
//            Log.d(TAG, "calling native_inject_xtra_data");
//        }
        InjectXtraData(data, data->GetLength());
    }
    else {
        // try again later
        // since this is delayed and not urgent we do not hold a wake lock here
        void (STDCALL GpsLocationProvider::*pHandlerFunc)();
        pHandlerFunc = &GpsLocationProvider::HandleDownloadXtraData;
        mHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);
        mHandler->PostCppCallbackDelayed((Handle32)this, *(Handle32*)&pHandlerFunc, NULL, 0, RETRY_INTERVAL);
    }
}

void GpsLocationProvider::UpdateLocation(
    /* [in] */ ILocation* location)
{
    void (STDCALL GpsLocationProvider::*pHandlerFunc)(ILocation*);
    pHandlerFunc = &GpsLocationProvider::HandleUpdateLocation;
    SendCallback(*(Handle32*)&pHandlerFunc, UPDATE_LOCATION, 0, (IInterface*)location);
}

void GpsLocationProvider::HandleUpdateLocation(
    /* [in] */ ILocation* location)
{
    Boolean result;
    location->HasAccuracy(&result);
    if (result) {
        Double latitude, longitude;
        Float accuracy;
        location->GetLatitude(&latitude);
        location->GetLongitude(&longitude);
        location->GetAccuracy(&accuracy);
        InjectLocation(latitude, longitude, accuracy);
    }
}

Boolean GpsLocationProvider::RequiresSatellite()
{
    return TRUE;
}

Boolean GpsLocationProvider::RequiresCell()
{
    return FALSE;
}

Boolean GpsLocationProvider::HasMonetaryCost()
{
    return FALSE;
}

Boolean GpsLocationProvider::SupportsAltitude()
{
    return TRUE;
}

Boolean GpsLocationProvider::SupportsSpeed()
{
    return TRUE;
}

Boolean GpsLocationProvider::SupportsBearing()
{
    return TRUE;
}

Int32 GpsLocationProvider::GetPowerRequirement()
{
    return Criteria_POWER_HIGH;
}

Boolean GpsLocationProvider::MeetsCriteria(
    /* [in] */ ICriteria* criteria)
{
    Int32 requirement;
    criteria->GetPowerRequirement(&requirement);
    return (requirement != Criteria_POWER_LOW);
}

Int32 GpsLocationProvider::GetAccuracy()
{
    return Criteria_ACCURACY_FINE;
}

void GpsLocationProvider::Enable()
{
    Mutex::Autolock lock(mHandlerLock);
    void (STDCALL GpsLocationProvider::*pHandlerFunc)();
    pHandlerFunc = &GpsLocationProvider::HandleEnable;
    SendCallback(*(Handle32*)&pHandlerFunc, ENABLE, 1, NULL);
}

void GpsLocationProvider::HandleEnable()
{
//    if (DEBUG) Log.d(TAG, "handleEnable");
    if (mEnabled) return;
    mEnabled = Init();

    if (mEnabled) {
        mSupportsXtra = SupportsXtra();
        if (mSuplServerHost != NULL) {
            SetAgpsServer(AGPS_TYPE_SUPL, mSuplServerHost, mSuplServerPort);
        }
        if (mC2KServerHost != NULL) {
            SetAgpsServer(AGPS_TYPE_C2K, mC2KServerHost, mC2KServerPort);
        }
    }
    else {
//        Log.w(TAG, "Failed to enable location provider");
    }
}

void GpsLocationProvider::Disable()
{
    Mutex::Autolock lock(mHandlerLock);
    void (STDCALL GpsLocationProvider::*pHandlerFunc)();
    pHandlerFunc = &GpsLocationProvider::HandleDisable;
    SendCallback(*(Handle32*)&pHandlerFunc, ENABLE, 0, NULL);
}

void GpsLocationProvider::HandleDisable()
{
//    if (DEBUG) Log.d(TAG, "handleDisable");
    if (!mEnabled) return;

    mEnabled = FALSE;
    StopNavigating();

    // do this before releasing wakelock
    Cleanup();
}

Boolean GpsLocationProvider::IsEnabled()
{
    return mEnabled;
}

Int32 GpsLocationProvider::GetStatus(
    /* [in] */ IBundle* extras)
{
    if (extras != NULL) {
        extras->PutInt32(String("satellites"), mSvCount);
    }
    return mStatus;
}

void GpsLocationProvider::UpdateStatus(
    /* [in] */ Int32 status,
    /* [in] */ Int32 svCount)
{
    if (status != mStatus || svCount != mSvCount) {
        mStatus = status;
        mSvCount = svCount;
        mLocationExtras->PutInt32(String("satellites"), svCount);
        mStatusUpdateTime = SystemClock::GetElapsedRealtime();
    }
}

Int64 GpsLocationProvider::GetStatusUpdateTime()
{
    return mStatusUpdateTime;
}

void GpsLocationProvider::EnableLocationTracking(
    /* [in] */ Boolean enable)
{
    // FIXME - should set a flag here to avoid race conditions with single shot request
    Mutex::Autolock lock(mHandlerLock);
    void (STDCALL GpsLocationProvider::*pHandlerFunc)(Boolean);
    pHandlerFunc = &GpsLocationProvider::HandleEnableLocationTracking;
    SendCallback(*(Handle32*)&pHandlerFunc, ENABLE_TRACKING, (enable ? 1 : 0), NULL);
}

void GpsLocationProvider::HandleEnableLocationTracking(
    /* [in] */ Boolean enable)
{
    if (enable) {
        mTTFF = 0;
        mLastFixTime = 0;
        StartNavigating(FALSE);
    }
    else {
        if (!HasCapability(GPS_CAPABILITY_SCHEDULING)) {
//            mAlarmManager.cancel(mWakeupIntent);
//            mAlarmManager.cancel(mTimeoutIntent);
        }
        StopNavigating();
    }
}

Boolean GpsLocationProvider::RequestSingleShotFix()
{
    if (mStarted) {
        // cannot do single shot if already navigating
        return FALSE;
    }

    {
        Mutex::Autolock lock(mHandlerLock);

        void (STDCALL GpsLocationProvider::*pHandlerFunc)();
        pHandlerFunc = &GpsLocationProvider::HandleRequestSingleShot;
        mHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);
        mHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, NULL, 0);
    }

    return TRUE;
}

void GpsLocationProvider::HandleRequestSingleShot()
{
    mTTFF = 0;
    mLastFixTime = 0;
    StartNavigating(TRUE);
}

void GpsLocationProvider::SetMinTime(
    /* [in] */ Int64 minTime,
    /* [in] */ IWorkSource* ws)
{
//    if (DEBUG) Log.d(TAG, "setMinTime " + minTime);

    if (minTime >= 0) {
        mFixInterval = (Int32)minTime;

        if (mStarted && HasCapability(GPS_CAPABILITY_SCHEDULING)) {
            if (!SetPositionMode(mPositionMode, GPS_POSITION_RECURRENCE_PERIODIC,
                    mFixInterval, 0, 0)) {
//                Log.e(TAG, "set_position_mode failed in setMinTime()");
            }
        }
    }
}

String GpsLocationProvider::GetInternalState()
{
//    jstring result = NULL;
//    if (sGpsDebugInterface) {
//        const size_t maxLength = 2047;
//        char buffer[maxLength+1];
//        size_t length = sGpsDebugInterface->get_internal_state(buffer, maxLength);
//        if (length > maxLength) length = maxLength;
//        buffer[length] = 0;
//        result = env->NewStringUTF(buffer);
//    }
//    return result;
    return String(NULL);
}

GpsLocationProvider::Listener::Listener(
    /* [in] */ GpsLocationProvider* gpsLocProvider,
    /* [in] */ IGpsStatusListener* listener)
    : mGpsLocationProvider(gpsLocProvider)
    , mListener(listener)
    , mSensors(0)
{
}

PInterface GpsLocationProvider::Listener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IBinderDeathRecipient) {
        return (IBinderDeathRecipient*)this;
    }

    return NULL;
}

UInt32 GpsLocationProvider::Listener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 GpsLocationProvider::Listener::Release()
{
    return ElRefBase::Release();
}

ECode GpsLocationProvider::Listener::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode GpsLocationProvider::Listener::BinderDied()
{
//    if (DEBUG) Log.d(TAG, "GPS status listener died");

    {
        Mutex::Autolock lock(mGpsLocationProvider->mListenersLock);
        mGpsLocationProvider->mListeners.Remove(this);
    }

    if (mListener != NULL) {
//        mListener.asBinder().unlinkToDeath(this, 0);
    }
    return NOERROR;
}

void GpsLocationProvider::AddListener(
    /* [in] */ Int32 uid)
{
//    synchronized (mWakeLock) {
    mPendingListenerMessages++;
//    mWakeLock.acquire();
    void (STDCALL GpsLocationProvider::*pHandlerFunc)(Int32);
    pHandlerFunc = &GpsLocationProvider::HandleAddListener;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(uid);
    mHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
//    }
}

void GpsLocationProvider::HandleAddListener(
    /* [in] */ Int32 uid)
{
    Mutex::Autolock lock(mListenersLock);
    HashMap<Int32, Int32>::Iterator it;
    for (it = mClientUids.Begin(); it != mClientUids.End(); ++it) {
        Int32 key = it->mFirst;
        if (key == uid) {
//            Log.w(TAG, "Duplicate add listener for uid " + uid);
            return;
        }
    }

    mClientUids[uid] = 0;
//    if (mNavigating) {
//        try {
//        mBatteryStats->NoteStartGps(uid);
//        } catch (RemoteException e) {
//            Log.w(TAG, "RemoteException in addListener");
//        }
//    }
}

void GpsLocationProvider::RemoveListener(
    /* [in] */ Int32 uid)
{
//    synchronized (mWakeLock) {
    mPendingListenerMessages++;
//    mWakeLock.acquire();
    void (STDCALL GpsLocationProvider::*pHandlerFunc)(Int32);
    pHandlerFunc = &GpsLocationProvider::HandleRemoveListener;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(uid);
    mHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
//    }
}

void GpsLocationProvider::HandleRemoveListener(
    /* [in] */ Int32 uid)
{
    Mutex::Autolock lock(mListenersLock);
    HashMap<Int32, Int32>::Iterator it;
    for (it = mClientUids.Begin(); it != mClientUids.End(); ++it) {
        Int32 key = it->mFirst;
        if (key == uid) {
            // Shouldn't be here -- don't have this uid.
//            Log.w(TAG, "Unneeded remove listener for uid " + uid);
            break;
        }
    }
    if (it == mClientUids.End()) {
        // Shouldn't be here -- don't have this uid.
//        Log.w(TAG, "Unneeded remove listener for uid " + uid);
        return;
    }

    mClientUids.Erase(it);
    if (mNavigating) {
//        try {
//        mBatteryStats->NoteStopGps(uid);
//        } catch (RemoteException e) {
//            Log.w(TAG, "RemoteException in removeListener");
//        }
    }
}

Boolean GpsLocationProvider::SendExtraCommand(
    /* [in] */ String command,
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** outExtras)
{
//    Int64 identity = Binder.clearCallingIdentity();
    Boolean result = FALSE;

    if (command == String("delete_aiding_data")) {
        result = DeleteAidingData(extras);
    }
    else if (command == String("force_time_injection")) {
        void (STDCALL GpsLocationProvider::*pHandlerFunc)();
        pHandlerFunc = &GpsLocationProvider::HandleInjectNtpTime;
        SendCallback(*(Handle32*)&pHandlerFunc, INJECT_NTP_TIME, 0, NULL);
        result = TRUE;
    }
    else if (command == String("force_xtra_injection")) {
        if (mSupportsXtra) {
            XtraDownloadRequest();
            result = TRUE;
        }
    }
    else {
//        Log.w(TAG, "sendExtraCommand: unknown command " + command);
    }

//    Binder.restoreCallingIdentity(identity);
    return result;
}

Boolean GpsLocationProvider::DeleteAidingData(
    /* [in] */ IBundle* extras)
{
    Int32 flags;

    if (extras == NULL) {
        flags = GPS_DELETE_ALL;
    }
    else {
        flags = 0;
        Boolean value;
        extras->GetBoolean(String("ephemeris"), &value);
        if (value) flags |= GPS_DELETE_EPHEMERIS;
        extras->GetBoolean(String("almanac"), &value);
        if (value) flags |= GPS_DELETE_ALMANAC;
        extras->GetBoolean(String("position"), &value);
        if (value) flags |= GPS_DELETE_POSITION;
        extras->GetBoolean(String("time"), &value);
        if (value) flags |= GPS_DELETE_TIME;
        extras->GetBoolean(String("iono"), &value);
        if (value) flags |= GPS_DELETE_IONO;
        extras->GetBoolean(String("utc"), &value);
        if (value) flags |= GPS_DELETE_UTC;
        extras->GetBoolean(String("health"), &value);
        if (value) flags |= GPS_DELETE_HEALTH;
        extras->GetBoolean(String("svdir"), &value);
        if (value) flags |= GPS_DELETE_SVDIR;
        extras->GetBoolean(String("svsteer"), &value);
        if (value) flags |= GPS_DELETE_SVSTEER;
        extras->GetBoolean(String("sadata"), &value);
        if (value) flags |= GPS_DELETE_SADATA;
        extras->GetBoolean(String("rti"), &value);
        if (value) flags |= GPS_DELETE_RTI;
        extras->GetBoolean(String("celldb-info"), &value);
        if (value) flags |= GPS_DELETE_CELLDB_INFO;
        extras->GetBoolean(String("all"), &value);
        if (value) flags |= GPS_DELETE_ALL;
    }

    if (flags != 0) {
        NativeDeleteAidingData(flags);
        return TRUE;
    }

    return FALSE;
}

void GpsLocationProvider::StartNavigating(
    /* [in] */ Boolean singleShot)
{
    if (!mStarted) {
//        if (DEBUG) Log.d(TAG, "startNavigating");
        mStarted = TRUE;
        mSingleShot = singleShot;
        mPositionMode = GPS_POSITION_MODE_STANDALONE;

//         if (Settings.Secure.getInt(mContext.getContentResolver(),
//                Settings.Secure.ASSISTED_GPS_ENABLED, 1) != 0) {
//            if (singleShot && HasCapability(GPS_CAPABILITY_MSA)) {
//                mPositionMode = GPS_POSITION_MODE_MS_ASSISTED;
//            }
//            else if (HasCapability(GPS_CAPABILITY_MSB)) {
//                mPositionMode = GPS_POSITION_MODE_MS_BASED;
//            }
//        }

        Int32 interval = (HasCapability(GPS_CAPABILITY_SCHEDULING) ? mFixInterval : 1000);
        if (!SetPositionMode(mPositionMode, GPS_POSITION_RECURRENCE_PERIODIC,
                interval, 0, 0)) {
            mStarted = FALSE;
//            Log.e(TAG, "set_position_mode failed in startNavigating()");
            return;
        }
        if (!Start()) {
            mStarted = FALSE;
//            Log.e(TAG, "native_start failed in startNavigating()");
            return;
        }

        // reset SV count to zero
        UpdateStatus(LocationProvider_TEMPORARILY_UNAVAILABLE, 0);
        mFixRequestTime = System::GetCurrentTimeMillis();
        if (!HasCapability(GPS_CAPABILITY_SCHEDULING)) {
            // set timer to give up if we do not receive a fix within NO_FIX_TIMEOUT
            // and our fix interval is not short
            if (mFixInterval >= NO_FIX_TIMEOUT) {
//                mAlarmManager.set(AlarmManager.ELAPSED_REALTIME_WAKEUP,
//                        SystemClock.elapsedRealtime() + NO_FIX_TIMEOUT, mTimeoutIntent);
            }
        }
    }
}

void GpsLocationProvider::StopNavigating()
{
//    if (DEBUG) Log.d(TAG, "stopNavigating");
    if (mStarted) {
        mStarted = FALSE;
        mSingleShot = FALSE;
        Stop();
        mTTFF = 0;
        mLastFixTime = 0;
        mLocationFlags = LOCATION_INVALID;

        // reset SV count to zero
        UpdateStatus(LocationProvider_TEMPORARILY_UNAVAILABLE, 0);
    }
}

void GpsLocationProvider::Hibernate()
{
    // stop GPS until our next fix interval arrives
    StopNavigating();
//    mAlarmManager.cancel(mTimeoutIntent);
//    mAlarmManager.cancel(mWakeupIntent);
//    long now = SystemClock.elapsedRealtime();
//    mAlarmManager.set(AlarmManager.ELAPSED_REALTIME_WAKEUP,
//            SystemClock.elapsedRealtime() + mFixInterval, mWakeupIntent);
}

Boolean GpsLocationProvider::HasCapability(
    /* [in] */ Int32 capability)
{
    return ((mEngineCapabilities & capability) != 0);
}

void GpsLocationProvider::ReportLocation(
    /* [in] */ Int32 flags,
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Double altitude,
    /* [in] */ Float speed,
    /* [in] */ Float bearing,
    /* [in] */ Float accuracy,
    /* [in] */ Int64 timestamp)
{
//    if (VERBOSE) Log.v(TAG, "reportLocation lat: " + latitude + " long: " + longitude +
//            " timestamp: " + timestamp);

    {
        Mutex::Autolock lock(mLocationLock);

        mLocationFlags = flags;
        if ((flags & LOCATION_HAS_LAT_LONG) == LOCATION_HAS_LAT_LONG) {
            mLocation->SetLatitude(latitude);
            mLocation->SetLongitude(longitude);
            mLocation->SetTime(timestamp);
        }

        if ((flags & LOCATION_HAS_ALTITUDE) == LOCATION_HAS_ALTITUDE) {
            mLocation->SetAltitude(altitude);
        }
        else {
            mLocation->RemoveAltitude();
        }

        if ((flags & LOCATION_HAS_SPEED) == LOCATION_HAS_SPEED) {
            mLocation->SetSpeed(speed);
        }
        else {
            mLocation->RemoveSpeed();
        }

        if ((flags & LOCATION_HAS_BEARING) == LOCATION_HAS_BEARING) {
            mLocation->SetBearing(bearing);
        }
        else {
            mLocation->RemoveBearing();
        }

        if ((flags & LOCATION_HAS_ACCURACY) == LOCATION_HAS_ACCURACY) {
            mLocation->SetAccuracy(accuracy);
        }
        else {
            mLocation->RemoveAccuracy();
        }

//        try {
        mLocationManager->ReportLocation(mLocation, FALSE);
//        } catch (RemoteException e) {
//            Log.e(TAG, "RemoteException calling reportLocation");
//        }
    }

    mLastFixTime = System::GetCurrentTimeMillis();
    // report time to first fix
    if (mTTFF == 0 && (flags & LOCATION_HAS_LAT_LONG) == LOCATION_HAS_LAT_LONG) {
        mTTFF = (Int32)(mLastFixTime - mFixRequestTime);
//        if (DEBUG) Log.d(TAG, "TTFF: " + mTTFF);

        // notify status listeners
        {
            Mutex::Autolock lock(mListenersLock);

            List<AutoPtr<Listener> >::Iterator it;
            for (it = mListeners.Begin(); it != mListeners.End(); ++it) {
                AutoPtr<Listener> listener = *it;
//                try {
                ECode ec = listener->mListener->OnFirstFix(mTTFF);
                if (FAILED(ec)) {
//                    Log.w(TAG, "RemoteException in stopNavigating");
                    it = mListeners.Erase(it);
                    // adjust for size of list changing
                    it--;
                }
            }
        }
    }

    if (mSingleShot) {
        StopNavigating();
    }
    if (mStarted && mStatus != LocationProvider_AVAILABLE) {
        // we want to time out if we do not receive a fix
        // within the time out and we are requesting infrequent fixes
        if (!HasCapability(GPS_CAPABILITY_SCHEDULING) && mFixInterval < NO_FIX_TIMEOUT) {
//            mAlarmManager.cancel(mTimeoutIntent);
        }

        // send an intent to notify that the GPS is receiving fixes.
        AutoPtr<IIntent> intent;
        CIntent::New(String(LocationManager_GPS_FIX_CHANGE_ACTION), (IIntent**)&intent);
        intent->PutBooleanExtra(String(LocationManager_EXTRA_GPS_ENABLED), TRUE);
        mContext->SendBroadcast(intent);
        UpdateStatus(LocationProvider_AVAILABLE, mSvCount);
    }

   if (!HasCapability(GPS_CAPABILITY_SCHEDULING) && mStarted && mFixInterval > 1000) {
//        if (DEBUG) Log.d(TAG, "got fix, hibernating");
        Hibernate();
    }
}

void GpsLocationProvider::ReportStatus(
    /* [in] */ Int32 status)
{
//    if (DEBUG) Log.v(TAG, "reportStatus status: " + status);

    Mutex::Autolock lock(mListenersLock);
    Boolean wasNavigating = mNavigating;

    switch (status) {
        case GPS_STATUS_SESSION_BEGIN:
            mNavigating = TRUE;
            mEngineOn = TRUE;
            break;
        case GPS_STATUS_SESSION_END:
            mNavigating = FALSE;
            break;
        case GPS_STATUS_ENGINE_ON:
            mEngineOn = TRUE;
            break;
        case GPS_STATUS_ENGINE_OFF:
            mEngineOn = FALSE;
            mNavigating = FALSE;
            break;
    }

    if (wasNavigating != mNavigating) {
        List<AutoPtr<Listener> >::Iterator it;
        for (it = mListeners.Begin(); it != mListeners.End(); ++it) {
            AutoPtr<Listener> listener = *it;
            ECode ec = NOERROR;
            if (mNavigating) {
                ec = listener->mListener->OnGpsStarted();
                if (FAILED(ec)) {
//                    Log.w(TAG, "RemoteException in reportStatus");
                    it = mListeners.Erase(it);
                    // adjust for size of list changing
                    it--;
                }
            }
            else {
                ec = listener->mListener->OnGpsStopped();
                if (FAILED(ec)) {
//                    Log.w(TAG, "RemoteException in reportStatus");
                    it = mListeners.Erase(it);
                    // adjust for size of list changing
                    it--;
                }
            }
        }

//        try {
            // update battery stats
        HashMap<Int32, Int32>::Iterator hmit;
        for (hmit = mClientUids.Begin(); hmit != mClientUids.End(); ++hmit) {
            Int32 uid = hmit->mFirst;
            if (mNavigating) {
//                mBatteryStats.noteStartGps(uid);
            }
            else {
//                mBatteryStats.noteStopGps(uid);
            }
        }
//        } catch (RemoteException e) {
//            Log.w(TAG, "RemoteException in reportStatus");
//        }

        // send an intent to notify that the GPS has been enabled or disabled.
        AutoPtr<IIntent> intent;
        CIntent::New(String(LocationManager_GPS_ENABLED_CHANGE_ACTION), (IIntent**)&intent);
        intent->PutBooleanExtra(String(LocationManager_EXTRA_GPS_ENABLED), mNavigating);
        mContext->SendBroadcast(intent);
    }
}

void GpsLocationProvider::ReportSvStatus()
{
    Int32 svCount = ReadSvStatus(*mSvs, *mSnrs, *mSvElevations, *mSvAzimuths, *mSvMasks);

    {
        Mutex::Autolock lock(mListenersLock);

        List<AutoPtr<Listener> >::Iterator it;
        for (it = mListeners.Begin(); it != mListeners.End(); ++it) {
            AutoPtr<Listener> listener = *it;
            ECode ec = listener->mListener->OnSvStatusChanged(svCount, *mSvs, *mSnrs,
                    *mSvElevations, *mSvAzimuths, (*mSvMasks)[EPHEMERIS_MASK],
                    (*mSvMasks)[ALMANAC_MASK], (*mSvMasks)[USED_FOR_FIX_MASK]);
            if (FAILED(ec)) {
//                Log.w(TAG, "RemoteException in reportSvInfo");
                it = mListeners.Erase(it);
                // adjust for size of list changing
                --it;
            }
        }
    }

//    if (VERBOSE) {
//        Log.v(TAG, "SV count: " + svCount +
//                " ephemerisMask: " + Integer.toHexString(mSvMasks[EPHEMERIS_MASK]) +
//                " almanacMask: " + Integer.toHexString(mSvMasks[ALMANAC_MASK]));
//        for (Int32 i = 0; i < svCount; i++) {
//            Log.v(TAG, "sv: " + mSvs[i] +
//                    " snr: " + (float)mSnrs[i]/10 +
//                    " elev: " + mSvElevations[i] +
//                    " azimuth: " + mSvAzimuths[i] +
//                    ((mSvMasks[EPHEMERIS_MASK] & (1 << (mSvs[i] - 1))) == 0 ? "  " : " E") +
//                    ((mSvMasks[ALMANAC_MASK] & (1 << (mSvs[i] - 1))) == 0 ? "  " : " A") +
//                    ((mSvMasks[USED_FOR_FIX_MASK] & (1 << (mSvs[i] - 1))) == 0 ? "" : "U"));
//        }
//    }

    // return number of sets used in fix instead of total
//    UpdateStatus(mStatus, Integer.bitCount(mSvMasks[USED_FOR_FIX_MASK]));

    if (mNavigating && mStatus == LocationProvider_AVAILABLE && mLastFixTime > 0 &&
        System::GetCurrentTimeMillis() - mLastFixTime > RECENT_FIX_TIMEOUT) {
        // send an intent to notify that the GPS is no longer receiving fixes.
        AutoPtr<IIntent> intent;
        CIntent::New(String(LocationManager_GPS_FIX_CHANGE_ACTION), (IIntent**)&intent);
        intent->PutBooleanExtra(String(LocationManager_EXTRA_GPS_ENABLED), FALSE);
        mContext->SendBroadcast(intent);
        UpdateStatus(LocationProvider_TEMPORARILY_UNAVAILABLE, mSvCount);
    }
}

void GpsLocationProvider::ReportAGpsStatus(
    /* [in] */ Int32 type,
    /* [in] */ Int32 status)
{
    switch (status) {
        case GPS_REQUEST_AGPS_DATA_CONN:
//            if (DEBUG) Log.d(TAG, "GPS_REQUEST_AGPS_DATA_CONN");
            // Set mAGpsDataConnectionState before calling startUsingNetworkFeature
            //  to avoid a race condition with handleUpdateNetworkState()
            mAGpsDataConnectionState = AGPS_DATA_CONNECTION_OPENING;
            Int32 result;
            mConnMgr->StartUsingNetworkFeature(
                    ConnectivityManager_TYPE_MOBILE, String("enableSUPL")/*Phone_FEATURE_ENABLE_SUPL*/, &result);
            if (result == 0/*Phone_APN_ALREADY_ACTIVE*/) {
//                if (DEBUG) Log.d(TAG, "Phone.APN_ALREADY_ACTIVE");
                if (mAGpsApn != NULL) {
                    AgpsDataConnOpen(mAGpsApn);
                    mAGpsDataConnectionState = AGPS_DATA_CONNECTION_OPEN;
                }
                else {
//                    Log.e(TAG, "mAGpsApn not set when receiving Phone.APN_ALREADY_ACTIVE");
                    mAGpsDataConnectionState = AGPS_DATA_CONNECTION_CLOSED;
                    AgpsDataConnFailed();
                }
            }
            else if (result == 1/*Phone_APN_REQUEST_STARTED*/) {
//                if (DEBUG) Log.d(TAG, "Phone.APN_REQUEST_STARTED");
                // Nothing to do here
            }
            else {
//                if (DEBUG) Log.d(TAG, "startUsingNetworkFeature failed");
                mAGpsDataConnectionState = AGPS_DATA_CONNECTION_CLOSED;
                AgpsDataConnFailed();
            }
            break;
        case GPS_RELEASE_AGPS_DATA_CONN:
//            if (DEBUG) Log.d(TAG, "GPS_RELEASE_AGPS_DATA_CONN");
            if (mAGpsDataConnectionState != AGPS_DATA_CONNECTION_CLOSED) {
                Int32 result;
                mConnMgr->StopUsingNetworkFeature(
                        ConnectivityManager_TYPE_MOBILE, String("enableSUPL")/*Phone_FEATURE_ENABLE_SUPL*/, &result);
                AgpsDataConnClosed();
                mAGpsDataConnectionState = AGPS_DATA_CONNECTION_CLOSED;
            }
            break;
        case GPS_AGPS_DATA_CONNECTED:
//            if (DEBUG) Log.d(TAG, "GPS_AGPS_DATA_CONNECTED");
            break;
        case GPS_AGPS_DATA_CONN_DONE:
//            if (DEBUG) Log.d(TAG, "GPS_AGPS_DATA_CONN_DONE");
            break;
        case GPS_AGPS_DATA_CONN_FAILED:
//            if (DEBUG) Log.d(TAG, "GPS_AGPS_DATA_CONN_FAILED");
            break;
    }
}

void GpsLocationProvider::ReportNmea(
    /* [in] */ Int64 timestamp)
{
    Mutex::Autolock lock(mListenersLock);

    if (mListeners.Begin() != mListeners.End()) {
        // don't bother creating the String if we have no listeners
        Int32 length = ReadNmea(*mNmeaBuffer, mNmeaBuffer->GetLength());
        String nmea((const char*)mNmeaBuffer->GetPayload(), length);

        List<AutoPtr<Listener> >::Iterator it;
        for (it = mListeners.Begin(); it != mListeners.End(); ++it) {
            AutoPtr<Listener> listener = *it;
            ECode ec = listener->mListener->OnNmeaReceived(timestamp, nmea);
            if (FAILED(ec)) {
//                Log.w(TAG, "RemoteException in reportNmea");
                it = mListeners.Erase(it);
                // adjust for size of list changing
                --it;
            }
        }
    }
}

void GpsLocationProvider::SetEngineCapabilities(
    /* [in] */ Int32 capabilities)
{
    mEngineCapabilities = capabilities;
}

void GpsLocationProvider::XtraDownloadRequest()
{
//    if (DEBUG) Log.d(TAG, "xtraDownloadRequest");
    void (STDCALL GpsLocationProvider::*pHandlerFunc)();
    pHandlerFunc = &GpsLocationProvider::HandleDownloadXtraData;
    SendCallback(*(Handle32*)&pHandlerFunc, DOWNLOAD_XTRA_DATA, 0, NULL);
}

void GpsLocationProvider::SendCallback(
    /* [in] */ Handle32 func,
    /* [in] */ Int32 message,
    /* [in] */ Int32 arg,
    /* [in] */ IInterface* value)
{
    // hold a wake lock while messages are pending
//    synchronized (mWakeLock) {
    mPendingMessageBits |= (1 << message);
//    mWakeLock.acquire();
    mHandler->RemoveCppCallbacks((Handle32)this, func);
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(arg);
    params->WriteInterfacePtr(value);
    mHandler->PostCppCallback((Handle32)this, func, params, 0);
//    }
}

GpsLocationProvider::NetInitiatedListener::NetInitiatedListener(
    /* [in] */ GpsLocationProvider* gpsLocProvider)
    : mGpsLocationProvider(gpsLocProvider)
{
}

PInterface GpsLocationProvider::NetInitiatedListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_INetInitiatedListener) {
        return (INetInitiatedListener*)this;
    }

    return NULL;
}

UInt32 GpsLocationProvider::NetInitiatedListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 GpsLocationProvider::NetInitiatedListener::Release()
{
    return ElRefBase::Release();
}

ECode GpsLocationProvider::NetInitiatedListener::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode GpsLocationProvider::NetInitiatedListener::SendNiResponse(
    /* [in] */ Int32 notificationId,
    /* [in] */ Int32 userResponse,
    /* [out] */ Boolean* result)
{
    // TODO Add Permission check

//    StringBuilder extrasBuf = new StringBuilder();

//    if (DEBUG) Log.d(TAG, "sendNiResponse, notifId: " + notificationId +
//            ", response: " + userResponse);
    mGpsLocationProvider->SendNiResponse(notificationId, userResponse);
    *result = TRUE;
    return NOERROR;
}

GpsLocationProvider::NetInitiatedListener* GpsLocationProvider::GetNetInitiatedListener()
{
    return mNetInitiatedListener;
}

void GpsLocationProvider::ReportNiNotification(
    /* [in] */ Int32 notificationId,
    /* [in] */ Int32 niType,
    /* [in] */ Int32 notifyFlags,
    /* [in] */ Int32 timeout,
    /* [in] */ Int32 defaultResponse,
    /* [in] */ String requestorId,
    /* [in] */ String text,
    /* [in] */ Int32 requestorIdEncoding,
    /* [in] */ Int32 textEncoding,
    /* [in] */ String extras)
{
//    Log.i(TAG, "reportNiNotification: entered");
//    Log.i(TAG, "notificationId: " + notificationId +
//            ", niType: " + niType +
//            ", notifyFlags: " + notifyFlags +
//            ", timeout: " + timeout +
//            ", defaultResponse: " + defaultResponse);

//    Log.i(TAG, "requestorId: " + requestorId +
//            ", text: " + text +
//            ", requestorIdEncoding: " + requestorIdEncoding +
//            ", textEncoding: " + textEncoding);

    GpsNetInitiatedHandler::GpsNiNotification* notification = new GpsNetInitiatedHandler::GpsNiNotification();

    notification->mNotificationId = notificationId;
    notification->mNiType = niType;
    notification->mNeedNotify = (notifyFlags & GpsNetInitiatedHandler::GPS_NI_NEED_NOTIFY) != 0;
    notification->mNeedVerify = (notifyFlags & GpsNetInitiatedHandler::GPS_NI_NEED_VERIFY) != 0;
    notification->mPrivacyOverride = (notifyFlags & GpsNetInitiatedHandler::GPS_NI_PRIVACY_OVERRIDE) != 0;
    notification->mTimeout = timeout;
    notification->mDefaultResponse = defaultResponse;
    notification->mRequestorId = requestorId;
    notification->mText = text;
    notification->mRequestorIdEncoding = requestorIdEncoding;
    notification->mTextEncoding = textEncoding;

    // Process extras, assuming the format is
    // one of more lines of "key = value"
    AutoPtr<IBundle> bundle;
    CBundle::New((IBundle**)&bundle);

    if (extras.IsNull()) extras = String("");
//    Properties extraProp = new Properties();

//    try {
//        extraProp.load(new StringBufferInputStream(extras));
//    }
//    catch (IOException e)
//    {
//        Log.e(TAG, "reportNiNotification cannot parse extras data: " + extras);
//    }

//    for (Entry<Object, Object> ent : extraProp.entrySet())
//    {
//        bundle.putString((String) ent.getKey(), (String) ent.getValue());
//    }

    notification->mExtras = bundle;

    mNIHandler->HandleNiNotification(notification);
}

void GpsLocationProvider::RequestSetID(
    /* [in] */ Int32 flags)
{
//    TelephonyManager phone = (TelephonyManager)
//            mContext.getSystemService(Context.TELEPHONY_SERVICE);
    Int32 type = AGPS_SETID_TYPE_NONE;
    String data("");

    if ((flags & AGPS_RIL_REQUEST_SETID_IMSI) == AGPS_RIL_REQUEST_SETID_IMSI) {
        String data_temp(NULL);// = phone.getSubscriberId();
        if (data_temp.IsNull()) {
            // This means the framework does not have the SIM card ready.
        }
        else {
            // This means the framework has the SIM card.
            data = data_temp;
            type = AGPS_SETID_TYPE_IMSI;
        }
    }
    else if ((flags & AGPS_RIL_REQUEST_SETID_MSISDN) == AGPS_RIL_REQUEST_SETID_MSISDN) {
        String data_temp(NULL);// = phone.getLine1Number();
        if (data_temp.IsNull()) {
            // This means the framework does not have the SIM card ready.
        }
        else {
            // This means the framework has the SIM card.
            data = data_temp;
            type = AGPS_SETID_TYPE_MSISDN;
        }
    }
    AgpsSetId(type, data);
}

void GpsLocationProvider::RequestRefLocation(
    /* [in] */ Int32 flags)
{
}

Boolean GpsLocationProvider::Init()
{
    return FALSE;
}

void GpsLocationProvider::Cleanup()
{
}

Boolean GpsLocationProvider::SetPositionMode(
    /* [in] */ Int32 mode,
    /* [in] */ Int32 recurrence,
    /* [in] */ Int32 min_interval,
    /* [in] */ Int32 preferred_accuracy,
    /* [in] */ Int32 preferred_time)
{
    return FALSE;
}

Boolean GpsLocationProvider::Start()
{
    return FALSE;
}

Boolean GpsLocationProvider::Stop()
{
    return FALSE;
}

void GpsLocationProvider::NativeDeleteAidingData(
    /* [in] */ Int32 flags)
{

}

Int32 GpsLocationProvider::ReadSvStatus(
    /* [in] */ const ArrayOf<Int32>& svs,
    /* [in] */ const ArrayOf<Float>& snrs,
    /* [in] */ const ArrayOf<Float>& elevations,
    /* [in] */ const ArrayOf<Float>& azimuths,
    /* [in] */ const ArrayOf<Int32>& masks)
{
    return 0;
}

Int32 GpsLocationProvider::ReadNmea(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 bufferSize)
{
    return 0;
}

void GpsLocationProvider::InjectLocation(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Float accuracy)
{

}

void GpsLocationProvider::InjectTime(
    /* [in] */ Int64 time,
    /* [in] */ Int64 timeReference,
    /* [in] */ Int32 uncertainty)
{

}

Boolean GpsLocationProvider::SupportsXtra()
{
    return FALSE;
}

void GpsLocationProvider::InjectXtraData(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 length)
{

}

void GpsLocationProvider::AgpsDataConnOpen(
    /* [in] */ String apn)
{

}

void GpsLocationProvider::AgpsDataConnClosed()
{

}

void GpsLocationProvider::AgpsDataConnFailed()
{

}

void GpsLocationProvider::AgpsNiMessage(
    /* [in] */ const ArrayOf<Byte>& msg,
    /* [in] */ Int32 length)
{

}

void GpsLocationProvider::SetAgpsServer(
    /* [in] */ Int32 type,
    /* [in] */ String hostname,
    /* [in] */ Int32 port)
{

}

void GpsLocationProvider::SendNiResponse(
    /* [in] */ Int32 notificationId,
    /* [in] */ Int32 userResponse)
{

}

void GpsLocationProvider::AgpsSetRefLocationCellid(
    /* [in] */ Int32 type,
    /* [in] */ Int32 mcc,
    /* [in] */ Int32 mnc,
    /* [in] */ Int32 lac,
    /* [in] */ Int32 cid)
{

}

void GpsLocationProvider::AgpsSetId(
    /* [in] */ Int32 type,
    /* [in] */ String setid)
{

}

void GpsLocationProvider::NativeUpdateNetworkState(
    /* [in] */ Boolean connected,
    /* [in] */ Int32 type,
    /* [in] */ Boolean roaming,
    /* [in] */ String extraInfo)
{

}
