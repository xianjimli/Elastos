
#include "server/location/MockProvider.h"

const CString MockProvider::TAG = "MockProvider";

MockProvider::MockProvider(
    /* [in] */ String name,
    /* [in] */ ILocationManager* locationManager,
    /* [in] */ Boolean requiresNetwork,
    /* [in] */ Boolean requiresSatellite,
    /* [in] */ Boolean requiresCell,
    /* [in] */ Boolean hasMonetaryCost,
    /* [in] */ Boolean supportsAltitude,
    /* [in] */ Boolean supportsSpeed,
    /* [in] */ Boolean supportsBearing,
    /* [in] */ Int32 powerRequirement,
    /* [in] */ Int32 accuracy)
    : mName(name)
    , mLocationManager(locationManager)
    , mRequiresNetwork(requiresNetwork)
    , mRequiresSatellite(requiresSatellite)
    , mRequiresCell(requiresCell)
    , mHasMonetaryCost(hasMonetaryCost)
    , mSupportsAltitude(supportsAltitude)
    , mSupportsSpeed(supportsSpeed)
    , mSupportsBearing(supportsBearing)
    , mPowerRequirement(powerRequirement)
    , mAccuracy(accuracy)
    , mStatus(0)
    , mStatusUpdateTime(0)
    , mHasLocation(FALSE)
    , mHasStatus(FALSE)
    , mEnabled(FALSE)

{
    ASSERT_SUCCEEDED(CBundle::New((IBundle**)&mExtras));
    ASSERT_SUCCEEDED(CLocation::New(name, (ILocation**)&mLocation));
}

String MockProvider::GetName()
{
    return mName;
}

void MockProvider::Disable()
{
    mEnabled = FALSE;
}

void MockProvider::Enable()
{
    mEnabled = TRUE;
}

Boolean MockProvider::IsEnabled()
{
    return mEnabled;
}

Int32 MockProvider::GetStatus(
    /* [in] */ IBundle* extras)
{
    if (mHasStatus) {
        extras->Clear();
        extras->PutAll(mExtras);
        return mStatus;
    }
    else {
        return LocationProvider_AVAILABLE;
    }
}

Int64 MockProvider::GetStatusUpdateTime()
{
    return mStatusUpdateTime;
}

Int32 MockProvider::GetAccuracy()
{
    return mAccuracy;
}

Int32 MockProvider::GetPowerRequirement()
{
    return mPowerRequirement;
}

Boolean MockProvider::HasMonetaryCost()
{
    return mHasMonetaryCost;
}

Boolean MockProvider::RequiresCell()
{
    return mRequiresCell;
}

Boolean MockProvider::RequiresNetwork()
{
    return mRequiresNetwork;
}

Boolean MockProvider::RequiresSatellite()
{
    return mRequiresSatellite;
}

Boolean MockProvider::SupportsAltitude()
{
    return mSupportsAltitude;
}

Boolean MockProvider::SupportsBearing()
{
    return mSupportsBearing;
}

Boolean MockProvider::SupportsSpeed()
{
    return mSupportsSpeed;
}

Boolean MockProvider::MeetsCriteria(
    /* [in] */ ICriteria* criteria)
{
    Int32 accuracy;
    criteria->GetAccuracy(&accuracy);
    if ((accuracy != Criteria_NO_REQUIREMENT) &&
        (accuracy < mAccuracy)) {
        return FALSE;
    }

    Int32 criteriaPower;
    criteria->GetPowerRequirement(&criteriaPower);
    if ((criteriaPower != Criteria_NO_REQUIREMENT) &&
        (criteriaPower < mPowerRequirement)) {
        return FALSE;
    }

    Boolean result;
    criteria->IsAltitudeRequired(&result);
    if (result && !mSupportsAltitude) {
        return FALSE;
    }

    criteria->IsSpeedRequired(&result);
    if (result && !mSupportsSpeed) {
        return FALSE;
    }

    criteria->IsBearingRequired(&result);
    if (result && !mSupportsBearing) {
        return FALSE;
    }

    return TRUE;
}

void MockProvider::SetLocation(
    /* [in] */ ILocation* l)
{
    mLocation->Set(l);
    mHasLocation = TRUE;
//    try {
    ECode ec = mLocationManager->ReportLocation(mLocation, FALSE);
    if (FAILED(ec)) {
//        Log.e(TAG, "RemoteException calling reportLocation");
    }
//    } catch (RemoteException e) {
//        Log.e(TAG, "RemoteException calling reportLocation");
//    }
}

void MockProvider::ClearLocation()
{
    mHasLocation = FALSE;
}

void MockProvider::SetStatus(
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 updateTime)
{
    mStatus = status;
    mStatusUpdateTime = updateTime;
    mExtras->Clear();
    if (extras != NULL) {
        mExtras->PutAll(extras);
    }
    mHasStatus = TRUE;
}

void MockProvider::ClearStatus()
{
    mHasStatus = FALSE;
    mStatusUpdateTime = 0;
}

String MockProvider::GetInternalState()
{
    return String(NULL);
}

void MockProvider::EnableLocationTracking(
    /* [in] */ Boolean enable)
{
}

Boolean MockProvider::RequestSingleShotFix()
{
    return FALSE;
}

void MockProvider::SetMinTime(
    /* [in] */ Int64 minTime,
    /* [in] */ IWorkSource* ws)
{
}

void MockProvider::UpdateNetworkState(
    /* [in] */ Int32 state,
    /* [in] */ INetworkInfo* info)
{
}

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
