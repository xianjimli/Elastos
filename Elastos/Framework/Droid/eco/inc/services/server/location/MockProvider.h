
#ifndef __MOCKPROVIDER_H__
#define __MOCKPROVIDER_H__

#include "server/location/LocationProviderInterface.h"
#include <elastos/AutoPtr.h>

class MockProvider : public ILocationProviderInterface
{
private:
    static const CString TAG;

public:
    MockProvider(
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
        /* [in] */ Int32 accuracy);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI Disable();

    CARAPI Enable();

    CARAPI IsEnabled(
        /* [out] */ Boolean* isEnabled);

    CARAPI GetStatus(
        /* [in] */ IBundle* extras,
        /* [out] */ Int32* status);

    CARAPI GetStatusUpdateTime(
        /* [out] */ Int64* time);

    CARAPI GetAccuracy(
        /* [out] */ Int32* accuracy);

    CARAPI GetPowerRequirement(
        /* [out] */ Int32* requirement);

    CARAPI HasMonetaryCost(
        /* [out] */ Boolean* result);

    CARAPI RequiresCell(
        /* [out] */ Boolean* required);

    CARAPI RequiresNetwork(
        /* [out] */ Boolean* required);

    CARAPI RequiresSatellite(
        /* [out] */ Boolean* required);

    CARAPI SupportsAltitude(
        /* [out] */ Boolean* supported);

    CARAPI SupportsBearing(
        /* [out] */ Boolean* supported);

    CARAPI SupportsSpeed(
        /* [out] */ Boolean* supported);

    CARAPI MeetsCriteria(
        /* [in] */ ICriteria* criteria,
        /* [out] */ Boolean* result);

    CARAPI SetLocation(
        /* [in] */ ILocation* l);

    CARAPI ClearLocation();

    CARAPI SetStatus(
        /* [in] */ Int32 status,
        /* [in] */ IBundle* extras,
        /* [in] */ Int64 updateTime);

    CARAPI ClearStatus();

    CARAPI GetInternalState(
        /* [out] */ String* state);

    CARAPI EnableLocationTracking(
        /* [in] */ Boolean enable);

    CARAPI RequestSingleShotFix(
        /* [out] */ Boolean* result);

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
