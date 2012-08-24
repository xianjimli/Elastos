
#include "server/location/MockProvider.h"

String MockProvider::GetName()
{
    return String(NULL);
}

Boolean MockProvider::RequiresNetwork()
{
    return FALSE;
}

Boolean MockProvider::RequiresSatellite()
{
    return FALSE;
}

Boolean MockProvider::RequiresCell()
{
    return FALSE;
}

Boolean MockProvider::HasMonetaryCost()
{
    return FALSE;
}

Boolean MockProvider::SupportsAltitude()
{
    return FALSE;
}

Boolean MockProvider::SupportsSpeed()
{
    return FALSE;
}

Boolean MockProvider::SupportsBearing()
{
    return FALSE;
}

Int32 MockProvider::GetPowerRequirement()
{
    return 0;
}

Boolean MockProvider::MeetsCriteria(
    /* [in] */ ICriteria* criteria)
{
    return FALSE;
}

Int32 MockProvider::GetAccuracy()
{
    return 0;
}

Boolean MockProvider::IsEnabled()
{
    return FALSE;
}

void MockProvider::Enable()
{
}

void MockProvider::Disable()
{
}

Int32 MockProvider::GetStatus(
    /* [in] */ IBundle* extras)
{
    return 0;
}

Int64 MockProvider::GetStatusUpdateTime()
{
    return 0;
}

void MockProvider::EnableLocationTracking(
    /* [in] */ Boolean enable)
{
}

Boolean MockProvider::RequestSingleShotFix()
{
    return FALSE;
}

String MockProvider::GetInternalState()
{
    return String(NULL);
}

//    void MockProvider::SetMinTime(
//        /* [in] */ Int64 minTime,
//        /* [in] */ IWorkSource* ws)

//    void MockProvider::UpdateNetworkState(
//        /* [in] */ Int32 state,
//        /* [in] */ INetworkInfo* info)

void MockProvider::UpdateLocation(
    /* [in] */ ILocation* location)
{
}

Boolean MockProvider::SendExtraCommand(
    /* [in] */ String command,
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** outExtras)
{
    return FALSE;
}

void MockProvider::AddListener(
    /* [in] */ Int32 uid)
{
}

void MockProvider::RemoveListener(
    /* [in] */ Int32 uid)
{
}
