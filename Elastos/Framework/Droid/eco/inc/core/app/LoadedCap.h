
#ifndef __LOADEDCAP_H__
#define __LOADEDCAP_H__

#include "ext/frameworkext.h"

#include "app/CApplicationApartment.h"
#include "content/CApplicationInfo.h"
#include "content/CCompatibilityInfo.h"
#include "content/CInnerReceiver.h"
#include <elastos/HashMap.h>
#include <elastos/Mutex.h>
#include <elastos/AutoPtr.h>

using namespace Elastos::Core::Threading;

class CInnerConnection;

class LoadedCap
{
    friend class CApplicationApartment;

public:
    class ReceiverDispatcher
    {
    public:
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

        CARAPI GetIIntentReceiver(
            /* [out] */ IIntentReceiver** ir);

        CARAPI Validate(
           /* [in] */ IContext* context,
           /* [in] */ IApartment* apartment);
        

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

        class RunConnection /*: public IRunnable */
        {
        public:
            RunConnection(
               /* [in] */ ServiceDispatcher* serviceDispatcher,
               /* [in] */ IComponentName* name,
               /* [in] */ IBinder* service,
               /* [in] */ Int32 command);

            CARAPI Run();

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
           /* [in] */ IApartment* apartment,
           /* [in] */ Int32 flags);

        CARAPI Validate(
           /* [in] */ IContext* context,
           /* [in] */ IApartment* apartment);

        CARAPI_(void) DoForget();

        CARAPI_(IServiceConnectionInner*) GetIServiceConnection();

        CARAPI_(Int32) GetFlags();

        CARAPI_(void) Connected(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service);

        CARAPI DoConnected(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service);

        CARAPI DoDeath(
            /* [in] */ IComponentName* name,
            /* [in] */ IBinder* service);

        CARAPI PerformServiceConnected(
            /* [in] */ RunConnection* con);

        CARAPI HandleServiceConnected(
            /* [in] */ RunConnection* con);

    private:
        AutoPtr<CInnerConnection> mIServiceConnection;
        AutoPtr<IServiceConnection> mConnection;
        AutoPtr<IContext> mContext;
        AutoPtr<IApartment> mApartment;
//            private final ServiceConnectionLeaked mLocation;
        Int32 mFlags;
        Boolean mDied;
        HashMap<AutoPtr<IComponentName>, ConnectionInfo*> mActiveConnections;
    };

public:
    LoadedCap(
        /* [in] */ CApplicationApartment* appApartment,
        /* [in] */ const String& name,
        /* [in] */ IContext* systemContext,
        /* [in] */ CApplicationInfo* info);

    LoadedCap(
        /* [in] */ CApplicationApartment* appApartment,
        /* [in] */ CApplicationInfo* aInfo,
        /* [in] */ CApplicationApartment* mainApartment,
        /* [in] */ Boolean securityViolation,
        /* [in] */ Boolean includeCode);

    ~LoadedCap();

    CARAPI_(AutoPtr<IApplication>) GetApplication();

    CARAPI_(Boolean) IsSecurityViolation();

    CARAPI GetCapsuleName(
        /* [out] */ String* capsuleName);

    CARAPI GetApplicationInfo(
        /* [out] */ IApplicationInfo** info);

    CARAPI GetClassLoader(
        /* [out] */ IClassLoader** loader);

    CARAPI GetAppDir(
        /* [out] */ String* appDir);

    CARAPI GetResDir(
        /* [out] */ String* resDir);

    CARAPI GetDataDirFile(
        /* [out] */ IFile** dirFile);

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

    CARAPI ForgetReceiverDispatcher(
        /* [in] */ IContext* context,
        /* [in] */ IBroadcastReceiver* r,
        /* [out] */ IIntentReceiver** ir);

    CARAPI GetReceiverDispatcher(
        /* [in] */ IBroadcastReceiver* c,
        /* [in] */ IContext* context,
        /* [in] */ IApartment* handler,
        /* [in] */ IInstrumentation* instrumentation,
        /* [in] */ Boolean registered,
        /* [out] */ IIntentReceiver** ir);


    CARAPI_(IServiceConnectionInner*) GetServiceDispatcher(
        /* [in] */ IServiceConnection* c,
        /* [in] */ IContext* context,
        /* [in] */ IApartment* handler,
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
    String mDataDir;
    AutoPtr<IFile> mDataDirFile;
    Boolean mSecurityViolation;
    Boolean mIncludeCode;
    AutoPtr<IClassLoader> mClassLoader;
    AutoPtr<IApplication> mApplication;

    Mutex mServicesLock;
    Mutex mReceiverLock;
    Map<AutoPtr<IContext>,
        Map<AutoPtr<IBroadcastReceiver>, ReceiverDispatcher*>* > mReceivers;
    Map<AutoPtr<IContext>,
        Map<AutoPtr<IBroadcastReceiver>, ReceiverDispatcher*>* > mUnregisteredReceivers;
    Map<AutoPtr<IContext>,
        Map<AutoPtr<IServiceConnection>, ServiceDispatcher*>* > mServices;
    Map<AutoPtr<IContext>,
        Map<AutoPtr<IServiceConnection>, ServiceDispatcher*>* > mUnboundServices;
};

#endif //__LOADEDCAP_H__
