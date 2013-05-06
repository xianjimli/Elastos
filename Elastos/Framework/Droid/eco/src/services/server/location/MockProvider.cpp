
#include "ext/frameworkdef.h"
#include "server/location/MockProvider.h"

const CString MockProvider::TAG = "MockProvider";

MockProvider::MockProvider(
    /* [in] */ const String& name,
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

ECode MockProvider::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    *name = mName;
    return NOERROR;
}

ECode MockProvider::Disable()
{
    mEnabled = FALSE;
    return NOERROR;
}

ECode MockProvider::Enable()
{
    mEnabled = TRUE;
    return NOERROR;
}

ECode MockProvider::IsEnabled(
    /* [out] */ Boolean* isEnabled)
{
    VALIDATE_NOT_NULL(isEnabled);

    *isEnabled = mEnabled;
    return NOERROR;
}

ECode MockProvider::GetStatus(
    /* [in] */ IBundle* extras,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);

    if (mHasStatus) {
        extras->Clear();
        extras->PutAll(mExtras);
        *status = mStatus;
        return NOERROR;
    }
    else {
        *status = LocationProvider_AVAILABLE;
        return NOERROR;
    }
}

ECode MockProvider::GetStatusUpdateTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);

    *time = mStatusUpdateTime;
    return NOERROR;
}

ECode MockProvider::GetAccuracy(
    /* [out] */ Int32* accuracy)
{
    VALIDATE_NOT_NULL(accuracy);

    *accuracy = mAccuracy;
    return NOERROR;
}

ECode MockProvider::GetPowerRequirement(
    /* [out] */ Int32* requirement)
{
    VALIDATE_NOT_NULL(requirement);

    *requirement = mPowerRequirement;
    return NOERROR;
}

ECode MockProvider::HasMonetaryCost(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mHasMonetaryCost;
    return NOERROR;
}

ECode MockProvider::RequiresCell(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    *supported = mRequiresCell;
    return NOERROR;
}

ECode MockProvider::RequiresNetwork(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    *supported = mRequiresNetwork;
    return NOERROR;
}

ECode MockProvider::RequiresSatellite(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    *supported = mRequiresSatellite;
    return NOERROR;
}

ECode MockProvider::SupportsAltitude(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    *supported = mSupportsAltitude;
    return NOERROR;
}

ECode MockProvider::SupportsBearing(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    *supported = mSupportsBearing;
    return NOERROR;
}

ECode MockProvider::SupportsSpeed(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    *supported = mSupportsSpeed;
    return NOERROR;
}

ECode MockProvider::MeetsCriteria(
    /* [in] */ ICriteria* criteria,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 accuracy;
    criteria->GetAccuracy(&accuracy);
    if ((accuracy != Criteria_NO_REQUIREMENT) &&
        (accuracy < mAccuracy)) {
        *result = FALSE;
        return NOERROR;
    }

    Int32 criteriaPower;
    criteria->GetPowerRequirement(&criteriaPower);
    if ((criteriaPower != Criteria_NO_REQUIREMENT) &&
        (criteriaPower < mPowerRequirement)) {
        *result = FALSE;
        return NOERROR;
    }

    Boolean _result;
    criteria->IsAltitudeRequired(&_result);
    if (_result && !mSupportsAltitude) {
        *result = FALSE;
        return NOERROR;
    }

    criteria->IsSpeedRequired(&_result);
    if (_result && !mSupportsSpeed) {
        *result = FALSE;
        return NOERROR;
    }

    criteria->IsBearingRequired(&_result);
    if (_result && !mSupportsBearing) {
        *result = FALSE;
        return NOERROR;
    }
    *result = TRUE;
    return NOERROR;
}

ECode MockProvider::SetLocation(
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
    return ec;
}

ECode MockProvider::ClearLocation()
{
    mHasLocation = FALSE;
    return NOERROR;
}

ECode MockProvider::SetStatus(
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
    return NOERROR;
}

ECode MockProvider::ClearStatus()
{
    mHasStatus = FALSE;
    mStatusUpdateTime = 0;
    return NOERROR;
}

ECode MockProvider::GetInternalState(
    /* [out] */ String* state)
{
    VALIDATE_NOT_NULL(state);

    *state = NULL;
    return NOERROR;
}

ECode MockProvider::EnableLocationTracking(
    /* [in] */ Boolean enable)
{
    return NOERROR;
}

ECode MockProvider::RequestSingleShotFix(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = FALSE;
    return NOERROR;
}

ECode MockProvider::SetMinTime(
    /* [in] */ Int64 minTime,
    /* [in] */ IWorkSource* ws)
{
    return NOERROR;
}

ECode MockProvider::UpdateNetworkState(
    /* [in] */ Int32 state,
    /* [in] */ INetworkInfo* info)
{
    return NOERROR;
}

ECode MockProvider::UpdateLocation(
    /* [in] */ ILocation* location)
{
    return NOERROR;
}

ECode MockProvider::SendExtraCommand(
    /* [in] */ const String& command,
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** outExtras,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = FALSE;
    return NOERROR;
}

ECode MockProvider::AddListener(
    /* [in] */ Int32 uid)
{
    return NOERROR;
}

ECode MockProvider::RemoveListener(
    /* [in] */ Int32 uid)
{
    return NOERROR;
}
