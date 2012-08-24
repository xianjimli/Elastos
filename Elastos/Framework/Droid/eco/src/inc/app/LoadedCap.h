
#ifndef __LOADEDCAP_H__
#define __LOADEDCAP_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Map.h>
#include "app/CApplicationApartment.h"
#include "content/CApplicationInfo.h"
#include "content/CCompatibilityInfo.h"
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

class CInnerConnection;

class LoadedCap
{
    friend class CApplicationApartment;

public:
    class ReceiverDispatcher
    {
    public:
        class InnerReceiver : public IIntentReceiver
        {
        public:
            CARAPI PerformReceive(
                /* [in] */ IIntent* intent,
                /* [in] */ Int32 resultCode,
                /* [in] */ const String& data,
                /* [in] */ IBundle* extras,
                /* [in] */ Boolean ordered,
                /* [in] */ Boolean sticky);

        private:
            ReceiverDispatcher* mDispatcher;
        };

        class Args
        {
        public:
            AutoPtr<IIntent> mCurIntent;
            Int32 mCurCode;
            String mCurData;
            AutoPtr<IBundle> mCurMap;
            Boolean mCurOrdered;
            Boolean mCurSticky;
        };

    public:
        ReceiverDispatcher(
            /* [in] */ IBroadcastReceiver* receiver,
            /* [in] */ IContext* context,
            /* [in] */ IApartment* apartment,
            /* [in] */ IInstrumentation* instrumentation,
            /* [in] */ Boolean registered);

        CARAPI PerformReceive(
            /* [in] */ IIntent* intent,
            /* [in] */ Int32 resultCode,
            /* [in] */ const String& data,
            /* [in] */ IBundle* extras,
            /* [in] */ Boolean ordered,
            /* [in] */ Boolean sticky);

        CARAPI HandleReceive(
            /* [in] */ Args* args);

    private:
        AutoPtr<IIntentReceiver> mIIntentReceiver;
        AutoPtr<IContext> mContext;
        AutoPtr<IBroadcastReceiver> mReceiver;
        AutoPtr<IApartment> mApartment;
        AutoPtr<IInstrumentation> mInstrumentation;
        Boolean mRegistered;
    };

    class ServiceDispatcher
    {
    private:
        class ConnectionInfo
        {
        public:
            AutoPtr<IBinder> mBinder;
//                IBinder.DeathRecipient deathMonitor;
        };

        class RunConnection
        {
        public:
            RunConnection(
               /* [in] */ ServiceDispatcher* serviceDispatcher,
               /* [in] */ IComponentName* name,
               /* [in] */ IBinder* service,
               /* [in] */ Int32 command):
                mServiceDispatcher(serviceDispatcher),
                mName(name),
                mService(service)
            {
            }

            CARAPI Run()
            {
                ECode ec = NOERROR;

                if (mCommand == 0) {
                    ec = mServiceDispatcher->DoConnected(mName, mService);
                } else if (mCommand == 1) {
                    ec = mServiceDispatcher->DoDeath(mName, mService);
                }

                return ec;
            }

        private:
            ServiceDispatcher* mServiceDispatcher;
            AutoPtr<IComponentName> mName;
            AutoPtr<IBinder> mService;
            Int32 mCommand;
        };

    public:
        ServiceDispatcher(
           /* [in] */ IServiceConnection* conn,
           /* [in] */ IContext* context,
           /* [in] */ IApplicationApartment* apartment,
           /* [in] */ Int32 flags);

        CARAPI Validate(
           /* [in] */ IContext* context,
           /* [in] */ IApplicationApartment* apartment)
        {
//                if (mContext != context) {
//                    throw new RuntimeException(
//                        "ServiceConnection " + mConnection +
//                        " registered with differing Context (was " +
//                        mContext + " now " + context + ")");
//                }
//                if (mActivityThread != activityThread) {
//                    throw new RuntimeException(
//                        "ServiceConnection " + mConnection +
//                        " registered with differing handler (was " +
//                        mActivityThread + " now " + activityThread + ")");
//                }
            return E_NOT_IMPLEMENTED;
        }

        CARAPI_(void) DoForget()
        {
//                synchronized(this) {
//                    Iterator<ConnectionInfo> it = mActiveConnections.values().iterator();
//                    while (it.hasNext()) {
//                        ConnectionInfo ci = it.next();
//                        ci.binder.unlinkToDeath(ci.deathMonitor, 0);
//                    }
//                    mActiveConnections.clear();
//                }
        }

        CARAPI_(IServiceConnectionInner*) GetIServiceConnection();

        CARAPI_(Int32) GetFlags()
        {
            return mFlags;
        }

        CARAPI_(void) Connected(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service)
        {
            if (mAppApartment != NULL) {
                RunConnection* con = new RunConnection(this, name, service, 0);
                PerformServiceConnected(con);
            } else {
                DoConnected(name, service);
            }
        }

        CARAPI DoConnected(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service)
        {
            ConnectionInfo* old;
            ConnectionInfo* info;
            ECode ec = NOERROR;

            old = mActiveConnections[name];
            if (old != NULL && (IBinder*)old->mBinder == service) {
                // Huh, already have this one.  Oh well!
                return NOERROR;
            }

            if (service != NULL) {
                // A new service is being connected... set it all up.
                mDied = FALSE;
                info = new ConnectionInfo();
                info->mBinder = service;
//                info.deathMonitor = new DeathMonitor(name, service);
//                service.linkToDeath(info.deathMonitor, 0);
                mActiveConnections[name] = info;
            } else {
                // The named service is being disconnected... clean up.
                mActiveConnections.Erase(name);
            }

//            if (old != null) {
//                old.binder.unlinkToDeath(old.deathMonitor, 0);
//            }

            // If there was an old service, it is not disconnected.
            if (old != NULL) {
                ec = mConnection->OnServiceDisconnected(name);
            }
            // If there is a new service, it is now connected.
            if (service != NULL) {
                ec = mConnection->OnServiceConnected(name, service);
            }
            return ec;
        }

        CARAPI DoDeath(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service)
        {
//            mConnection.onServiceDisconnected(name);
            return E_NOT_IMPLEMENTED;
        }

        CARAPI PerformServiceConnected(
            /* [in] */ RunConnection* con);

        CARAPI HandleServiceConnected(
            /* [in] */ RunConnection* con);

    private:
        AutoPtr<CInnerConnection> mIServiceConnection;
        AutoPtr<IServiceConnection> mConnection;
        AutoPtr<IContext> mContext;
        AutoPtr<IApplicationApartment> mAppApartment;
//            private final ServiceConnectionLeaked mLocation;
        Int32 mFlags;
        Boolean mDied;
        Map<AutoPtr<IComponentName>, ConnectionInfo*> mActiveConnections;
    };

public:
    LoadedCap(
        /* [in] */ CApplicationApartment* appApartment,
        /* [in] */ CApplicationInfo* aInfo,
        /* [in] */ CApplicationApartment* mainApartment,
        /* [in] */ Boolean securityViolation,
        /* [in] */ Boolean includeCode);

    ~LoadedCap();

    CARAPI_(Boolean) IsSecurityViolation();

    CARAPI GetCapsuleName(
        /* [out] */ String* capsuleName);

    CARAPI GetClassLoader(
        /* [out] */ IClassLoader** loader);

    CARAPI GetAppDir(
        /* [out] */ String* appDir);

    CARAPI GetResources(
        /* [in] */ CApplicationApartment* apartment,
        /* [out] */ CResources** res);

    CARAPI MakeApplication(
        /* [in] */ Boolean forceDefaultAppClass,
        /* [in] */ IInstrumentation* instrumentation,
        /* [out] */ IApplication** app);

    CARAPI RemoveContextRegistrations(
        /* [in] */ IContext* context,
        /* [in] */ const String& who,
        /* [in] */ const String& what);

    CARAPI_(IServiceConnectionInner*) GetServiceDispatcher(
        /* [in] */ IServiceConnection* c,
        /* [in] */ IContext* context,
        /* [in] */ IApplicationApartment* handler,
        /* [in] */ Int32 flags);

    CARAPI_(IServiceConnectionInner*) ForgetServiceDispatcher(
        /* [in] */ IContext* context,
        /* [in] */ IServiceConnection* c);

public:
    /*package*/ AutoPtr<CResources> mResources;
    /*package*/ AutoPtr<CCompatibilityInfo> mCompatibilityInfo;

private:
    Mutex mLock;

    AutoPtr<CApplicationApartment> mAppApartment;
    AutoPtr<CApplicationInfo> mApplicationInfo;
    String mCapsuleName;
    String mAppDir;
    String mResDir;
    Boolean mSecurityViolation;
    Boolean mIncludeCode;
    AutoPtr<IClassLoader> mClassLoader;
    AutoPtr<IApplication> mApplication;

    Mutex mServicesLock;
    Map<AutoPtr<IContext>,
        Map<AutoPtr<IServiceConnection>, ServiceDispatcher*>* > mServices;
    Map<AutoPtr<IContext>,
        Map<AutoPtr<IServiceConnection>, ServiceDispatcher*>* > mUnboundServices;
};

#endif //__LOADEDCAP_H__
