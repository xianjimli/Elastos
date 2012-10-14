
#include "server/location/PassiveProvider.h"

const CString PassiveProvider::TAG = "PassiveProvider";

PassiveProvider::PassiveProvider(
    /* [in] */ ILocationManager* locationManager)
    : mLocationManager(locationManager)
{
}

String PassiveProvider::GetName()
{
    return String(LocationManager_PASSIVE_PROVIDER);
}

Boolean PassiveProvider::RequiresNetwork()
{
    return FALSE;
}

Boolean PassiveProvider::RequiresSatellite()
{
    return FALSE;
}

Boolean PassiveProvider::RequiresCell()
{
    return FALSE;
}

Boolean PassiveProvider::HasMonetaryCost()
{
    return FALSE;
}

Boolean PassiveProvider::SupportsAltitude()
{
    return FALSE;
}

Boolean PassiveProvider::SupportsSpeed()
{
    return FALSE;
}

Boolean PassiveProvider::SupportsBearing()
{
    return FALSE;
}

Int32 PassiveProvider::GetPowerRequirement()
{
    return -1;
}

Boolean PassiveProvider::MeetsCriteria(
    /* [in] */ ICriteria* criteria)
{
    // We do not want to match the special passive provider based on criteria.
    return FALSE;
}

Int32 PassiveProvider::GetAccuracy()
{
    return -1;
}

Boolean PassiveProvider::IsEnabled()
{
    return TRUE;
}

void PassiveProvider::Enable()
{
}

void PassiveProvider::Disable()
{
}

Int32 PassiveProvider::GetStatus(
    /* [in] */ IBundle* extras)
{
    if (mTracking) {
        return LocationProvider_AVAILABLE;
    }
    else {
        return LocationProvider_TEMPORARILY_UNAVAILABLE;
    }
}

Int64 PassiveProvider::GetStatusUpdateTime()
{
    return -1;
}

void PassiveProvider::EnableLocationTracking(
    /* [in] */ Boolean enable)
{
    mTracking = enable;
}

Boolean PassiveProvider::RequestSingleShotFix()
{
    return FALSE;
}

String PassiveProvider::GetInternalState()
{
    return String(NULL);
}

void PassiveProvider::SetMinTime(
    /* [in] */ Int64 minTime,
    /* [in] */ IWorkSource* ws)
{
}

void PassiveProvider::UpdateNetworkState(
    /* [in] */ Int32 state,
    /* [in] */ INetworkInfo* info)
{

}

void PassiveProvider::UpdateLocation(
    /* [in] */ ILocation* location)
{
    if (mTracking) {
 //       try {
        // pass the location back to the location manager
        mLocationManager->ReportLocation(location, TRUE);
//        } catch (RemoteException e) {
//            Log.e(TAG, "RemoteException calling reportLocation");
//        }
    }
}

Boolean PassiveProvider::SendExtraCommand(
    /* [in] */ String command,
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** outExtras)
{
    return FALSE;
}

void PassiveProvider::AddListener(
    /* [in] */ Int32 uid)
{
}

void PassiveProvider::RemoveListener(
    /* [in] */ Int32 uid)
{
}
