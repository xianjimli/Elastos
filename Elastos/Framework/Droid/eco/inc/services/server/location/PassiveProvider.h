
#ifndef __PASSIVEPROVIDER_H__
#define __PASSIVEPROVIDER_H__

#include "server/location/LocationProviderInterface.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

class PassiveProvider : public ElRefBase, public ILocationProviderInterface
{
private:
    static const CString TAG;

public:
    PassiveProvider(
        /* [in] */ ILocationManager* locationManager);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI RequiresNetwork(
        /* [out] */ Boolean* required);

    CARAPI RequiresSatellite(
        /* [out] */ Boolean* required);

    CARAPI RequiresCell(
        /* [out] */ Boolean* required);

    CARAPI HasMonetaryCost(
        /* [out] */ Boolean* result);

    CARAPI SupportsAltitude(
        /* [out] */ Boolean* supported);

    CARAPI SupportsSpeed(
        /* [out] */ Boolean* supported);

    CARAPI SupportsBearing(
        /* [out] */ Boolean* supported);

    CARAPI GetPowerRequirement(
        /* [out] */ Int32* requirement);

    CARAPI MeetsCriteria(
        /* [in] */ ICriteria* criteria,
        /* [out] */ Boolean* result);

    CARAPI GetAccuracy(
        /* [out] */ Int32* accuracy);

    CARAPI IsEnabled(
        /* [out] */ Boolean* isEnabled);

    CARAPI Enable();

    CARAPI Disable();

    CARAPI GetStatus(
        /* [in] */ IBundle* extras,
        /* [out] */ Int32* status);

    CARAPI GetStatusUpdateTime(
        /* [out] */ Int64* time);

    CARAPI EnableLocationTracking(
        /* [in] */ Boolean enable);

    CARAPI RequestSingleShotFix(
        /* [out] */ Boolean* result);

    CARAPI GetInternalState(
        /* [out] */ String* state);

    CARAPI SetMinTime(
        /* [in] */ Int64 minTime,
        /* [in] */ IWorkSource* ws);

    CARAPI UpdateNetworkState(
        /* [in] */ Int32 state,
        /* [in] */ INetworkInfo* info);

    CARAPI UpdateLocation(
        /* [in] */ ILocation* location);

    CARAPI SendExtraCommand(
        /* [in] */ const String& command,
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** outExtras,
        /* [out] */ Boolean* result);

    CARAPI AddListener(
        /* [in] */ Int32 uid);

    CARAPI RemoveListener(
        /* [in] */ Int32 uid);

private:
    AutoPtr<ILocationManager> mLocationManager;
    Boolean mTracking;
};

#endif //__PASSIVEPROVIDER_H__
