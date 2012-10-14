
#ifndef __DUMMYLOCATIONPROVIDER_H__
#define __DUMMYLOCATIONPROVIDER_H__

#include "ext/frameworkext.h"
#include "location/LocationProvider.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

class DummyLocationProvider : public LocationProvider
{
private:
	static const CString TAG;
public:
    DummyLocationProvider(
        /* [in] */ String name,
        /* [in] */ ILocationManager* service);

    CARAPI_(void) SetRequiresNetwork(
        /* [in] */ Boolean requiresNetwork);

    CARAPI_(void) SetRequiresSatellite(
        /* [in] */ Boolean requiresSatellite);

    CARAPI_(void) SetRequiresCell(
        /* [in] */ Boolean requiresCell);

    CARAPI_(void) SetHasMonetaryCost(
        /* [in] */ Boolean hasMonetaryCost);

    CARAPI_(void) SetSupportsAltitude(
        /* [in] */ Boolean supportsAltitude);

    CARAPI_(void) SetSupportsSpeed(
        /* [in] */ Boolean supportsSpeed);

    CARAPI_(void) SetSupportsBearing(
        /* [in] */ Boolean supportsBearing);

    CARAPI_(void) SetPowerRequirement(
        /* [in] */ Boolean powerRequirement);

    CARAPI_(void) SetAccuracy(
        /* [in] */ Boolean accuracy);

    CARAPI RequiresNetwork(
        /* [out] */ Boolean* result);

    CARAPI RequiresSatellite(
        /* [out] */ Boolean* result);

    CARAPI RequiresCell(
        /* [out] */ Boolean* result);

    CARAPI HasMonetaryCost(
        /* [out] */ Boolean* result);

    CARAPI SupportsAltitude(
        /* [out] */ Boolean* result);

    CARAPI SupportsSpeed(
        /* [out] */ Boolean* result);

    CARAPI SupportsBearing(
        /* [out] */ Boolean* result);

    CARAPI GetPowerRequirement(
        /* [out] */ Int32* requirement);

    CARAPI GetAccuracy(
        /* [out] */ Int32* accuracy);

public:
    String mName;
    Boolean mRequiresNetwork;
    Boolean mRequiresSatellite;
    Boolean mRequiresCell;
    Boolean mHasMonetaryCost;
    Boolean mSupportsAltitude;
    Boolean mSupportsSpeed;
    Boolean mSupportsBearing;
    Int32 mPowerRequirement;
    Int32 mAccuracy;
};

#endif //__DUMMYLOCATIONPROVIDER_H__
