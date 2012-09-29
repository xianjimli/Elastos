
#include "location/internal/DummyLocationProvider.h"

const CString DummyLocationProvider::TAG = "DummyLocationProvider";

DummyLocationProvider::DummyLocationProvider(
    /* [in] */ String name,
    /* [in] */ ILocationManager* service)
    : LocationProvider(name, service)
{
}

void DummyLocationProvider::SetRequiresNetwork(
    /* [in] */ Boolean requiresNetwork)
{
    mRequiresNetwork = requiresNetwork;
}

void DummyLocationProvider::SetRequiresSatellite(
    /* [in] */ Boolean requiresSatellite)
{
    mRequiresSatellite = requiresSatellite;
}

void DummyLocationProvider::SetRequiresCell(
    /* [in] */ Boolean requiresCell)
{
    mRequiresCell = requiresCell;
}

void DummyLocationProvider::SetHasMonetaryCost(
    /* [in] */ Boolean hasMonetaryCost)
{
    mHasMonetaryCost = hasMonetaryCost;
}

void DummyLocationProvider::SetSupportsAltitude(
    /* [in] */ Boolean supportsAltitude)
{
    mSupportsAltitude = supportsAltitude;
}

void DummyLocationProvider::SetSupportsSpeed(
    /* [in] */ Boolean supportsSpeed)
{
    mSupportsSpeed = supportsSpeed;
}

void DummyLocationProvider::SetSupportsBearing(
    /* [in] */ Boolean supportsBearing)
{
    mSupportsBearing = supportsBearing;
}

void DummyLocationProvider::SetPowerRequirement(
    /* [in] */ Boolean powerRequirement)
{
    mPowerRequirement = powerRequirement;
}

void DummyLocationProvider::SetAccuracy(
    /* [in] */ Boolean accuracy)
{
    mAccuracy = accuracy;
}

/**
 * Returns true if the provider requires access to a
 * data network (e.g., the Internet), false otherwise.
 */
ECode DummyLocationProvider::RequiresNetwork(
	/* [out] */ Boolean* result)
{
	*result = mRequiresNetwork;

	return NOERROR;
}

/**
 * Returns true if the provider requires access to a
 * satellite-based positioning system (e.g., GPS), false
 * otherwise.
 */
ECode DummyLocationProvider::RequiresSatellite(
	/* [out] */ Boolean* result)
{
    *result = mRequiresSatellite;

    return NOERROR;
}

/**
 * Returns true if the provider requires access to an appropriate
 * cellular network (e.g., to make use of cell tower IDs), false
 * otherwise.
 */
ECode DummyLocationProvider::RequiresCell(
	/* [out] */ Boolean* result)
{
	*result = mRequiresCell;

	return NOERROR;
}

/**
 * Returns true if the use of this provider may result in a
 * monetary charge to the user, false if use is free.  It is up to
 * each provider to give accurate information.
 */
ECode DummyLocationProvider::HasMonetaryCost(
	/* [out] */ Boolean* result)
{
	*result = mHasMonetaryCost;

	return NOERROR;
}

/**
 * Returns true if the provider is able to provide altitude
 * information, false otherwise.  A provider that reports altitude
 * under most circumstances but may occassionally not report it
 * should return true.
 */
ECode DummyLocationProvider::SupportsAltitude(
	/* [out] */ Boolean* result)
{
	*result = mSupportsAltitude;

	return NOERROR;
}

/**
 * Returns true if the provider is able to provide speed
 * information, false otherwise.  A provider that reports speed
 * under most circumstances but may occassionally not report it
 * should return true.
 */
ECode DummyLocationProvider::SupportsSpeed(
	/* [out] */ Boolean* result)
{
	*result = mSupportsSpeed;

	return NOERROR;
}

/**
 * Returns true if the provider is able to provide bearing
 * information, false otherwise.  A provider that reports bearing
 * under most circumstances but may occassionally not report it
 * should return true.
 */
ECode DummyLocationProvider::SupportsBearing(
	/* [out] */ Boolean* result)
{
    *result = mSupportsBearing;

    return NOERROR;
}

/**
 * Returns the power requirement for this provider.
 *
 * @return the power requirement for this provider, as one of the
 * constants Criteria.POWER_REQUIREMENT_*.
 */
ECode DummyLocationProvider::GetPowerRequirement(
	/* [out] */ Int32* requirement)
{
	*requirement = mPowerRequirement;

	return NOERROR;
}

/**
 * Returns a constant describing the horizontal accuracy returned
 * by this provider.
 *
 * @return the horizontal accuracy for this provider, as one of the
 * constants Criteria.ACCURACY_*.
 */
ECode DummyLocationProvider::GetAccuracy(
	/* [out] */ Int32* accuracy)
{
	*accuracy = mAccuracy;

	return NOERROR;
}
