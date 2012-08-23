
#include "server/location/PassiveProvider.h"

PassiveProvider::PassiveProvider(
    /* [in] */ ILocationManagerEx* locationManager)
    : mLocationManager(locationManager)
{
}

ECode PassiveProvider::HandleKey(
    /* [in] */ IKeyEvent* event,
    /* [in] */ IRunnable* finishedCallback)
{
    return E_NOT_IMPLEMENTED;
}

String PassiveProvider::GetName()
{
    return String(NULL);
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
    return 0;
}

Boolean PassiveProvider::MeetsCriteria(
    /* [in] */ ICriteria* criteria)
{
    return FALSE;
}

Int32 PassiveProvider::GetAccuracy()
{
    return 0;
}

Boolean PassiveProvider::IsEnabled()
{
    return FALSE;
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
    return 0;
}

Int64 PassiveProvider::GetStatusUpdateTime()
{
    return 0;
}

void PassiveProvider::EnableLocationTracking(
    /* [in] */ Boolean enable)
{
}

Boolean PassiveProvider::RequestSingleShotFix()
{
    return FALSE;
}

String PassiveProvider::GetInternalState()
{
    return String(NULL);
}

//    void PassiveProvider::SetMinTime(
//        /* [in] */ Int64 minTime,
//        /* [in] */ IWorkSource* ws)

//    void PassiveProvider::UpdateNetworkState(
//        /* [in] */ Int32 state,
//        /* [in] */ INetworkInfo* info)

void PassiveProvider::UpdateLocation(
    /* [in] */ ILocation* location)
{
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
