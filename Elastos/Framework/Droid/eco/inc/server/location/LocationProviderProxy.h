
#ifndef __LOCATIONPROVIDERPROXY_H__
#define __LOCATIONPROVIDERPROXY_H__

#include "server/location/LocationProviderInterface.h"
#include "location/internal/DummyLocationProvider.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

class LocationProviderProxy : public LocationProviderInterface
{
private:
    static const CString TAG;

private:
    class Connection : public ElRefBase, IServiceConnection, IRunnable
    {
    public:
        Connection(
            /* [in])*/ LocationProviderProxy* lpProxy);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnServiceConnected(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service);//todo service should be IInterface

        CARAPI OnServiceDisconnected(
            /* [in] */ IComponentName* name);

        CARAPI Post(
            /* [in] */ IRunnable* action);

        CARAPI_(AutoPtr<ILocationProvider>) GetProvider();

        CARAPI_(DummyLocationProvider*) GetCachedAttributes();

        CARAPI Run();

    private:
        LocationProviderProxy* mLPProxy;
        AutoPtr<ILocationProvider> mProvider;

        // for caching requiresNetwork, requiresSatellite, etc.
        DummyLocationProvider* mCachedAttributes;  // synchronized by mMutex
        Mutex mSync;
    };

public:
    LocationProviderProxy(
        /* [in] */ IContext* context,
        /* [in] */ String name,
        /* [in] */ String serviceName,
        /* [in] */ IApartment* handler);

    /**
     * When unbundled NetworkLocationService package is updated, we
     * need to unbind from the old version and re-bind to the new one.
     */
    CARAPI_(void) Reconnect();

    CARAPI_(String) GetName();

    CARAPI_(Boolean) RequiresNetwork();

    CARAPI_(Boolean) RequiresSatellite();

    CARAPI_(Boolean) RequiresCell();

    CARAPI_(Boolean) HasMonetaryCost();

    CARAPI_(Boolean) SupportsAltitude();

    CARAPI_(Boolean) SupportsSpeed();

    CARAPI_(Boolean) SupportsBearing();

    CARAPI_(Int32) GetPowerRequirement();

    CARAPI_(Int32) GetAccuracy();

    CARAPI_(Boolean) MeetsCriteria(
        /* [in] */ ICriteria* criteria);

    CARAPI_(void) Enable();

    CARAPI_(void) Disable();

    CARAPI_(Boolean) IsEnabled();

    CARAPI_(Int32) GetStatus(
        /* [in] */ IBundle* extras);

    CARAPI_(Int64) GetStatusUpdateTime();

    CARAPI_(void) EnableLocationTracking(
        /* [in] */ Boolean enable);

    CARAPI_(Boolean) RequestSingleShotFix();

    CARAPI_(String) GetInternalState();

    CARAPI_(Boolean) IsLocationTracking();

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
    CARAPI_(DummyLocationProvider*) GetCachedAttributes();

private:
    AutoPtr<IContext> mContext;
    String mName;
    AutoPtr<IIntent> mIntent;
    AutoPtr<IApartment> mHandler;
    Mutex mMutex;
    AutoPtr<Connection> mServiceConnection;  // never null

    // cached values set by the location manager
    Boolean mLocationTracking;
    Boolean mEnabled;
    Int64 mMinTime;
    AutoPtr<IWorkSource> mMinTimeSource;
    Int32 mNetworkState;
    AutoPtr<INetworkInfo> mNetworkInfo;
};

#endif //__LOCATIONPROVIDERPROXY_H__
