
#ifndef __LOCATIONPROVIDERINTERFACE_H__
#define __LOCATIONPROVIDERINTERFACE_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>

extern "C" const InterfaceID EIID_LocationProviderInterface;

interface LocationProviderInterface : public ElRefBase, public IInterface
{
public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    virtual CARAPI_(String) GetName() = 0;

    virtual CARAPI_(Boolean) RequiresNetwork() = 0;

    virtual CARAPI_(Boolean) RequiresSatellite() = 0;

    virtual CARAPI_(Boolean) RequiresCell() = 0;

    virtual CARAPI_(Boolean) HasMonetaryCost() = 0;

    virtual CARAPI_(Boolean) SupportsAltitude() = 0;

    virtual CARAPI_(Boolean) SupportsSpeed() = 0;

    virtual CARAPI_(Boolean) SupportsBearing() = 0;

    virtual CARAPI_(Int32) GetPowerRequirement() = 0;

    virtual CARAPI_(Boolean) MeetsCriteria(
        /* [in] */ ICriteria* criteria) = 0;

    virtual CARAPI_(Int32) GetAccuracy() = 0;

    virtual CARAPI_(Boolean) IsEnabled() = 0;

    virtual CARAPI_(void) Enable() = 0;

    virtual CARAPI_(void) Disable() = 0;

    virtual CARAPI_(Int32) GetStatus(
        /* [in] */ IBundle* extras) = 0;

    virtual CARAPI_(Int64) GetStatusUpdateTime() = 0;

    virtual CARAPI_(void) EnableLocationTracking(
        /* [in] */ Boolean enable) = 0;

    /* returns false if single shot is not supported */
    virtual CARAPI_(Boolean) RequestSingleShotFix() = 0;

    virtual CARAPI_(String) GetInternalState() = 0;

//    virtual CARAPI_(void) SetMinTime(
//        /* [in] */ Int64 minTime,
//        /* [in] */ IWorkSource* ws) = 0;

//    virtual CARAPI_(void) UpdateNetworkState(
//        /* [in] */ Int32 state,
//        /* [in] */ INetworkInfo* info) = 0;

    virtual CARAPI_(void) UpdateLocation(
        /* [in] */ ILocation* location) = 0;

    virtual CARAPI_(Boolean) SendExtraCommand(
        /* [in] */ String command,
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** outExtras) = 0;

    virtual CARAPI_(void) AddListener(
        /* [in] */ Int32 uid) = 0;

    virtual CARAPI_(void) RemoveListener(
        /* [in] */ Int32 uid) = 0;
};

#endif //__LOCATIONPROVIDERINTERFACE_H__
