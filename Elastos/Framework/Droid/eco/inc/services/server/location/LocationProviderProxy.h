
#ifndef __LOCATIONPROVIDERPROXY_H__
#define __LOCATIONPROVIDERPROXY_H__

#include "server/location/LocationProviderInterface.h"
#include "location/DummyLocationProvider.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

class LocationProviderProxy : public ElRefBase, public ILocationProviderInterface
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

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    /**
     * When unbundled NetworkLocationService package is updated, we
     * need to unbind from the old version and re-bind to the new one.
     */
    CARAPI_(void) Reconnect();

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

    CARAPI GetAccuracy(
        /* [out] */ Int32* accuracy);

    CARAPI MeetsCriteria(
        /* [in] */ ICriteria* criteria,
        /* [out] */ Boolean* result);

    CARAPI Enable();

    CARAPI Disable();

    CARAPI IsEnabled(
        /* [out] */ Boolean* isEnabled);

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

    CARAPI_(Boolean) IsLocationTracking();

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
    CARAPI_(DummyLocationProvider*) GetCachedAttributes();

private:
    AutoPtr<IContext> mContext;
    String mName;
    AutoPtr<IIntent> mIntent;
    AutoPtr<IApartment> mHandler;
    Mutex mLock;
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
