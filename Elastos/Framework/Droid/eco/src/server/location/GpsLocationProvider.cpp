
#include "server/location/GpsLocationProvider.h"
#include "os/CBundle.h"
#include "os/SystemClock.h"
#include "location/CLocation.h"

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

ECode GpsLocationProvider::MyGpsStatusProvider::AddGpsStatusListener(
    /* [in] */ IGpsStatusListener* listener)
{
    return E_NOT_IMPLEMENTED;
}

ECode GpsLocationProvider::MyGpsStatusProvider::RemoveGpsStatusListener(
    /* [in] */ IGpsStatusListener* listener)
{
    return E_NOT_IMPLEMENTED;
}

GpsLocationProvider::GpsLocationProvider(
    /* [in] */ IContext* context,
    /* [in] */ ILocationManagerEx* locationManager)
    : mLocationFlags(LOCATION_INVALID),
      mStatus(LocationProvider_TEMPORARILY_UNAVAILABLE),
      mEnabled(FALSE),
      mNetworkAvailable(FALSE),
      mInjectNtpTimePending(TRUE),
      mDownloadXtraDataPending(TRUE),
      mNavigating(FALSE),
      mEngineOn(FALSE),
      mFixInterval(1000),
      mStarted(FALSE),
      mSingleShot(FALSE),
      mEngineCapabilities(0),
      mSupportsXtra(FALSE),
      mFixRequestTime(0),
      mTTFF (0),
      mLastFixTime(0),
      mPositionMode(0),
      mSuplServerPort(0),
      mC2KServerPort(0),
      mContext(context),
      mLocationManager(locationManager),
      mPendingMessageBits(0),
      mPendingListenerMessages(0)
{
    mStatusUpdateTime = SystemClock::GetElapsedRealtime();
    ASSERT_SUCCEEDED(CLocation::New(String(LocationManager_GPS_PROVIDER), (ILocation**)&mLocation));
    ASSERT_SUCCEEDED(CBundle::New((IBundle**)&mLocationExtras));
    mGpsStatusProvider = new MyGpsStatusProvider();
}

AutoPtr<IGpsStatusProvider> GpsLocationProvider::GetGpsStatusProvider()
{
    return AutoPtr<IGpsStatusProvider>((IGpsStatusProvider*)mGpsStatusProvider);
}

Boolean GpsLocationProvider::IsSupported()
{
    return FALSE;
}

String GpsLocationProvider::GetName()
{
    return String(NULL);
}

Boolean GpsLocationProvider::RequiresNetwork()
{
    return FALSE;
}

Boolean GpsLocationProvider::RequiresSatellite()
{
    return FALSE;
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
    return FALSE;
}

Boolean GpsLocationProvider::SupportsSpeed()
{
    return FALSE;
}

Boolean GpsLocationProvider::SupportsBearing()
{
    return FALSE;
}

Int32 GpsLocationProvider::GetPowerRequirement()
{
    return 0;
}

Boolean GpsLocationProvider::MeetsCriteria(
    /* [in] */ ICriteria* criteria)
{
    return FALSE;
}

Int32 GpsLocationProvider::GetAccuracy()
{
    return 0;
}

Boolean GpsLocationProvider::IsEnabled()
{
    return FALSE;
}

void GpsLocationProvider::Enable()
{
}

void GpsLocationProvider::Disable()
{
}

Int32 GpsLocationProvider::GetStatus(
    /* [in] */ IBundle* extras)
{
    return 0;
}

Int64 GpsLocationProvider::GetStatusUpdateTime()
{
    return 0;
}

void GpsLocationProvider::EnableLocationTracking(
    /* [in] */ Boolean enable)
{
}

Boolean GpsLocationProvider::RequestSingleShotFix()
{
    return FALSE;
}

String GpsLocationProvider::GetInternalState()
{
    return String(NULL);
}

//    void GpsLocationProvider::SetMinTime(
//        /* [in] */ Int64 minTime,
//        /* [in] */ IWorkSource* ws)

//    void GpsLocationProvider::UpdateNetworkState(
//        /* [in] */ Int32 state,
//        /* [in] */ INetworkInfo* info)

void GpsLocationProvider::UpdateLocation(
    /* [in] */ ILocation* location)
{
}

Boolean GpsLocationProvider::SendExtraCommand(
    /* [in] */ String command,
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** outExtras)
{
    return FALSE;
}

void GpsLocationProvider::AddListener(
    /* [in] */ Int32 uid)
{
}

void GpsLocationProvider::RemoveListener(
    /* [in] */ Int32 uid)
{
}
