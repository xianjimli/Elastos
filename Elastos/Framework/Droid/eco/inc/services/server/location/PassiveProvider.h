
#ifndef __PASSIVEPROVIDER_H__
#define __PASSIVEPROVIDER_H__

#include "server/location/LocationProviderInterface.h"
#include <elastos/AutoPtr.h>

class PassiveProvider : public LocationProviderInterface
{
private:
    static const CString TAG;

public:
    PassiveProvider(
        /* [in] */ ILocationManager* locationManager);

    CARAPI_(String) GetName();

    CARAPI_(Boolean) RequiresNetwork();

    CARAPI_(Boolean) RequiresSatellite();

    CARAPI_(Boolean) RequiresCell();

    CARAPI_(Boolean) HasMonetaryCost();

    CARAPI_(Boolean) SupportsAltitude();

    CARAPI_(Boolean) SupportsSpeed();

    CARAPI_(Boolean) SupportsBearing();

    CARAPI_(Int32) GetPowerRequirement();

    CARAPI_(Boolean) MeetsCriteria(
        /* [in] */ ICriteria* criteria);

    CARAPI_(Int32) GetAccuracy();

    CARAPI_(Boolean) IsEnabled();

    CARAPI_(void) Enable();

    CARAPI_(void) Disable();

    CARAPI_(Int32) GetStatus(
        /* [in] */ IBundle* extras);

    CARAPI_(Int64) GetStatusUpdateTime();

    CARAPI_(void) EnableLocationTracking(
        /* [in] */ Boolean enable);

    CARAPI_(Boolean) RequestSingleShotFix();

    CARAPI_(String) GetInternalState();

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
    AutoPtr<ILocationManager> mLocationManager;
    Boolean mTracking;
};

#endif //__PASSIVEPROVIDER_H__
