
#ifndef __LOCATIONPROVIDERINTERFACE_H__
#define __LOCATIONPROVIDERINTERFACE_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>

extern "C" const InterfaceID EIID_ILocationProviderInterface;

interface ILocationProviderInterface : public IInterface
{
public:
    static CARAPI_(ILocationProviderInterface*) Probe(PInterface pObj)
    {
        return (ILocationProviderInterface*)pObj->Probe(EIID_ILocationProviderInterface);
    }

    virtual CARAPI GetName(
        /* [out] */ String* name) = 0;

    virtual CARAPI RequiresNetwork(
        /* [out] */ Boolean* required) = 0;

    virtual CARAPI RequiresSatellite(
        /* [out] */ Boolean* required) = 0;

    virtual CARAPI RequiresCell(
        /* [out] */ Boolean* required) = 0;

    virtual CARAPI HasMonetaryCost(
        /* [out] */ Boolean* result) = 0;

    virtual CARAPI SupportsAltitude(
        /* [out] */ Boolean* supported) = 0;

    virtual CARAPI SupportsSpeed(
        /* [out] */ Boolean* supported) = 0;

    virtual CARAPI SupportsBearing(
        /* [out] */ Boolean* supported) = 0;

    virtual CARAPI GetPowerRequirement(
        /* [out] */ Int32* requirement) = 0;

    virtual CARAPI MeetsCriteria(
        /* [in] */ ICriteria* criteria,
        /* [out] */ Boolean* result) = 0;

    virtual CARAPI GetAccuracy(
        /* [out] */ Int32* accuracy) = 0;

    virtual CARAPI IsEnabled(
        /* [out] */ Boolean* isEnabled) = 0;

    virtual CARAPI Enable() = 0;

    virtual CARAPI Disable() = 0;

    virtual CARAPI GetStatus(
        /* [in] */ IBundle* extras,
        /* [out] */ Int32* status) = 0;

    virtual CARAPI GetStatusUpdateTime(
        /* [out] */ Int64* time) = 0;

    virtual CARAPI EnableLocationTracking(
        /* [in] */ Boolean enable) = 0;

    /* returns false if single shot is not supported */
    virtual CARAPI RequestSingleShotFix(
        /* [out] */ Boolean* result) = 0;

    virtual CARAPI GetInternalState(
        /* [out] */ String* state) = 0;

    virtual CARAPI SetMinTime(
        /* [in] */ Int64 minTime,
        /* [in] */ IWorkSource* ws) = 0;

    virtual CARAPI UpdateNetworkState(
        /* [in] */ Int32 state,
        /* [in] */ INetworkInfo* info) = 0;

    virtual CARAPI UpdateLocation(
        /* [in] */ ILocation* location) = 0;

    virtual CARAPI SendExtraCommand(
        /* [in] */ const String& command,
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** outExtras,
        /* [out] */ Boolean* result) = 0;

    virtual CARAPI AddListener(
        /* [in] */ Int32 uid) = 0;

    virtual CARAPI RemoveListener(
        /* [in] */ Int32 uid) = 0;
};

#endif //__LOCATIONPROVIDERINTERFACE_H__
