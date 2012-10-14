
#ifndef __MOCKPROVIDER_H__
#define __MOCKPROVIDER_H__

#include "server/location/LocationProviderInterface.h"
#include <elastos/AutoPtr.h>

class MockProvider : public LocationProviderInterface
{
private:
    static const CString TAG;

public:
    MockProvider(
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
        /* [in] */ Int32 accuracy);

    CARAPI_(String) GetName();

    CARAPI_(void) Disable();

    CARAPI_(void) Enable();

    CARAPI_(Boolean) IsEnabled();

    CARAPI_(Int32) GetStatus(
        /* [in] */ IBundle* extras);

    CARAPI_(Int64) GetStatusUpdateTime();

    CARAPI_(Int32) GetAccuracy();

    CARAPI_(Int32) GetPowerRequirement();

    CARAPI_(Boolean) HasMonetaryCost();

    CARAPI_(Boolean) RequiresCell();

    CARAPI_(Boolean) RequiresNetwork();

    CARAPI_(Boolean) RequiresSatellite();

    CARAPI_(Boolean) SupportsAltitude();

    CARAPI_(Boolean) SupportsBearing();

    CARAPI_(Boolean) SupportsSpeed();

    CARAPI_(Boolean) MeetsCriteria(
        /* [in] */ ICriteria* criteria);

    CARAPI_(void) SetLocation(
        /* [in] */ ILocation* l);

    CARAPI_(void) ClearLocation();

    CARAPI_(void) SetStatus(
        /* [in] */ Int32 status,
        /* [in] */ IBundle* extras,
        /* [in] */ Int64 updateTime);

    CARAPI_(void) ClearStatus();

    CARAPI_(String) GetInternalState();

    CARAPI_(void) EnableLocationTracking(
        /* [in] */ Boolean enable);

    CARAPI_(Boolean) RequestSingleShotFix();

    CARAPI_(void) SetMinTime(
        /* [in] */ Int64 minTime,
        /* [in] */ IWorkSource* ws);

    CARAPI_(void) UpdateNetworkState(
        /* [in] */ Int32 state,
        /* [in] */ INetworkInfo* info);

    CARAPI_(void) UpdateLocation(
        /* [in] */ ILocation* location);

    CARAPI_(Boolean) SendExtraCommand(
        /* [in] */ String command,
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** outExtras);

    CARAPI_(void) AddListener(
        /* [in] */ Int32 uid);

    CARAPI_(void) RemoveListener(
        /* [in] */ Int32 uid);

private:
    String mName;
    AutoPtr<ILocationManager> mLocationManager;
    Boolean mRequiresNetwork;
    Boolean mRequiresSatellite;
    Boolean mRequiresCell;
    Boolean mHasMonetaryCost;
    Boolean mSupportsAltitude;
    Boolean mSupportsSpeed;
    Boolean mSupportsBearing;
    Int32 mPowerRequirement;
    Int32 mAccuracy;
    AutoPtr<ILocation> mLocation;
    Int32 mStatus;
    Int64 mStatusUpdateTime;
    AutoPtr<IBundle> mExtras;// = new Bundle();
    Boolean mHasLocation;
    Boolean mHasStatus;
    Boolean mEnabled;
};

#endif //__MOCKPROVIDER_H__
