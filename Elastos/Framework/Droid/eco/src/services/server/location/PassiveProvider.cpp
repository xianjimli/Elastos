
#include "server/location/PassiveProvider.h"

const CString PassiveProvider::TAG = "PassiveProvider";

PassiveProvider::PassiveProvider(
    /* [in] */ ILocationManager* locationManager)
    : mLocationManager(locationManager)
{}

PInterface PassiveProvider::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ILocationProviderInterface*)this;
    }
    else if (riid == EIID_ILocationProviderInterface) {
        return (ILocationProviderInterface*)this;
    }

    return NULL;
}

UInt32 PassiveProvider::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 PassiveProvider::Release()
{
    return ElRefBase::Release();
}

ECode PassiveProvider::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode PassiveProvider::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    *name = LocationManager_PASSIVE_PROVIDER;
    return NOERROR;
}

ECode PassiveProvider::RequiresNetwork(
    /* [out] */ Boolean* required)
{
    VALIDATE_NOT_NULL(required);

    *required = FALSE;
    return NOERROR;
}

ECode PassiveProvider::RequiresSatellite(
    /* [out] */ Boolean* required)
{
    VALIDATE_NOT_NULL(required);

    *required = FALSE;
    return NOERROR;
}

ECode PassiveProvider::RequiresCell(
    /* [out] */ Boolean* required)
{
    VALIDATE_NOT_NULL(required);

    *required = FALSE;
    return NOERROR;
}

ECode PassiveProvider::HasMonetaryCost(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = FALSE;
    return NOERROR;
}

ECode PassiveProvider::SupportsAltitude(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    *supported = FALSE;
    return NOERROR;
}

ECode PassiveProvider::SupportsSpeed(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    *supported = FALSE;
    return NOERROR;
}

ECode PassiveProvider::SupportsBearing(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    *supported = FALSE;
    return NOERROR;
}

ECode PassiveProvider::GetPowerRequirement(
    /* [out] */ Int32* requirement)
{
    VALIDATE_NOT_NULL(requirement);

    *requirement = -1;
    return NOERROR;
}

ECode PassiveProvider::MeetsCriteria(
    /* [in] */ ICriteria* criteria,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // We do not want to match the special passive provider based on criteria.
    *result = FALSE;
    return NOERROR;
}

ECode PassiveProvider::GetAccuracy(
    /* [out] */ Int32* accuracy)
{
    VALIDATE_NOT_NULL(accuracy);

    *accuracy = -1;
    return NOERROR;
}

ECode PassiveProvider::IsEnabled(
    /* [out] */ Boolean* isEnabled)
{
    VALIDATE_NOT_NULL(isEnabled);

    *isEnabled = TRUE;
    return NOERROR;
}

ECode PassiveProvider::Enable()
{
    return NOERROR;
}

ECode PassiveProvider::Disable()
{
    return NOERROR;
}

ECode PassiveProvider::GetStatus(
    /* [in] */ IBundle* extras,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);

    if (mTracking) {
        *status = LocationProvider_AVAILABLE;
        return NOERROR;
    }
    else {
        *status = LocationProvider_TEMPORARILY_UNAVAILABLE;
        return NOERROR;
    }
}

ECode PassiveProvider::GetStatusUpdateTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);

    *time = -1;
    return NOERROR;
}

ECode PassiveProvider::EnableLocationTracking(
    /* [in] */ Boolean enable)
{
    mTracking = enable;
    return NOERROR;
}

ECode PassiveProvider::RequestSingleShotFix(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = FALSE;
    return NOERROR;
}

ECode PassiveProvider::GetInternalState(
    /* [out] */ String* state)
{
    VALIDATE_NOT_NULL(state);

    *state = NULL;
    return NOERROR;
}

ECode PassiveProvider::SetMinTime(
    /* [in] */ Int64 minTime,
    /* [in] */ IWorkSource* ws)
{
    return NOERROR;
}

ECode PassiveProvider::UpdateNetworkState(
    /* [in] */ Int32 state,
    /* [in] */ INetworkInfo* info)
{
    return NOERROR;
}

ECode PassiveProvider::UpdateLocation(
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
    return NOERROR;
}

ECode PassiveProvider::SendExtraCommand(
    /* [in] */ const String& command,
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** outExtras,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = FALSE;
    return NOERROR;
}

ECode PassiveProvider::AddListener(
    /* [in] */ Int32 uid)
{
    return NOERROR;
}

ECode PassiveProvider::RemoveListener(
    /* [in] */ Int32 uid)
{
    return NOERROR;
}
