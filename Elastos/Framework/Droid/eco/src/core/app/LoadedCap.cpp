
#include "app/LoadedCap.h"
#include "app/ActivityManagerNative.h"
#include "app/CContextImpl.h"
#include "app/CInnerConnection.h"
#include "content/CInnerReceiver.h"
#include "os/CClassLoader.h"
#include <Slogger.h>
#include <StringBuffer.h>

using namespace Elastos::Utility::Logging;


LoadedCap::ReceiverDispatcher::ReceiverDispatcher(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IContext* context,
    /* [in] */ IApartment* apartment,
    /* [in] */ IInstrumentation* instrumentation,
    /* [in] */ Boolean registered) 
{
    CInnerReceiver::New((Handle32)this, !registered, (IIntentReceiver**)&mIIntentReceiver);
    mReceiver = receiver;
    mContext = context;
    mRegistered = registered;
    mApartment = apartment;
    mInstrumentation = instrumentation;
}

ECode LoadedCap::ReceiverDispatcher::PerformReceive(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean ordered,
    /* [in] */ Boolean sticky)
{
    if (CApplicationApartment::DEBUG_BROADCAST) {
        Int32 seq = -1;
        intent->GetInt32Extra(String("seq"), -1, &seq);
        String action;
        intent->GetAction(&action);
        Slogger::I(CApplicationApartment::TAG,
                StringBuffer("Enqueueing broadcast ") + action +
                " seq=" + seq + " to " + (Int32)(IBroadcastReceiver*)mReceiver);
    }
    ECode (STDCALL LoadedCap::ReceiverDispatcher::*pHandlerFunc)(
            Args* args);
    pHandlerFunc = &LoadedCap::ReceiverDispatcher::HandleReceive;

    Args* args = new Args();
    args->mCurIntent = intent;
    args->mCurCode = resultCode;
    args->mCurData = data;
    args->mCurMap = extras;
    args->mCurOrdered = ordered;
    args->mCurSticky = sticky;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)args);
    ECode ec = mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);

    if (FAILED(ec)) {
//        if (mRegistered && ordered) {
//            IActivityManager mgr = ActivityManagerNative.getDefault();
//            try {
//                if (DEBUG_BROADCAST) Slog.i(TAG,
//                        "Finishing sync broadcast to " + mReceiver);
//                mgr.finishReceiver(mIIntentReceiver, args.mCurCode,
//                        args.mCurData, args.mCurMap, false);
//            } catch (RemoteException ex) {
//            }
//        }
    }

    return ec;
}

ECode LoadedCap::ReceiverDispatcher::HandleReceive(
    /* [in] */ Args* args)
{
    if (CApplicationApartment::DEBUG_BROADCAST) {
        Int32 seq = -1;
        args->mCurIntent->GetInt32Extra(String("seq"), -1, &seq);
        String action;
        args->mCurIntent->GetAction(&action);
        Slogger::I(CApplicationApartment::TAG,
                StringBuffer("Dispatching broadcast ") + action +
                " seq=" + seq + " to " + (Int32)(IBroadcastReceiver*)mReceiver);
        Slogger::I(CApplicationApartment::TAG,
                StringBuffer("  mRegistered=") + mRegistered +
                " mCurOrdered=" + args->mCurOrdered);
    }

    ECode ec;
    AutoPtr<IActivityManager> mgr;
    ActivityManagerNative::GetDefault((IActivityManager**)&mgr);
    AutoPtr<IIntent> intent = args->mCurIntent;
    args->mCurIntent = NULL;

    if (mReceiver == NULL) {
        if (mRegistered && args->mCurOrdered) {
            if (CApplicationApartment::DEBUG_BROADCAST) {
                Slogger::I(CApplicationApartment::TAG,
                        StringBuffer("Finishing NULL broadcast to ") +
                        (Int32)(IBroadcastReceiver*)mReceiver);
            }
            ec = mgr->FinishReceiver((IIntentReceiver*)mIIntentReceiver,
                    args->mCurCode, args->mCurData, args->mCurMap, FALSE);
        }
        return ec;
    }

    mReceiver->SetOrderedHint(TRUE);
    mReceiver->SetResult(args->mCurCode, args->mCurData, args->mCurMap);
    mReceiver->ClearAbortBroadcast();
    mReceiver->SetOrderedHint(args->mCurOrdered);
    mReceiver->SetInitialStickyHint(args->mCurSticky);
    ec = mReceiver->ReceiveBroadcast(mContext, intent);
    if (FAILED(ec)) {
        if (mRegistered && args->mCurOrdered) {
            if (CApplicationApartment::DEBUG_BROADCAST) {
                Slogger::I(CApplicationApartment::TAG,
                        StringBuffer("Finishing failed broadcast to ") +
                        (Int32)(IBroadcastReceiver*)mReceiver);
            }
            ec = mgr->FinishReceiver((IIntentReceiver*)mIIntentReceiver,
                    args->mCurCode, args->mCurData, args->mCurMap, FALSE);
        }
        if (mInstrumentation == NULL) {
            Slogger::E(CApplicationApartment::TAG, "Error receiving broadcast");
            return E_RUNTIME_EXCEPTION;
        }
    }
    if (mRegistered && args->mCurOrdered) {
        if (CApplicationApartment::DEBUG_BROADCAST) {
            Slogger::I(CApplicationApartment::TAG,
                    StringBuffer("Finishing broadcast to ") +
                    (Int32)(IBroadcastReceiver*)mReceiver);
        }
        Int32 rcode;
        String rdata;
        AutoPtr<IBundle> rextras;
        Boolean abort;
        mReceiver->GetResultCode(&rcode);
        mReceiver->GetResultData(&rdata);
        mReceiver->GetResultExtras(FALSE, (IBundle**)&rextras);
        mReceiver->GetAbortBroadcast(&abort);
        ec = mgr->FinishReceiver(mIIntentReceiver,
                rcode, rdata, rextras, abort);
    }
    return ec;
}

ECode LoadedCap::ReceiverDispatcher::GetIIntentReceiver(
    /* [out] */ IIntentReceiver** ir)
{
    *ir = mIIntentReceiver;
    if (*ir != NULL) (*ir)->AddRef();
    return NOERROR;
}

ECode LoadedCap::ReceiverDispatcher::Validate(
   /* [in] */ IContext* context,
   /* [in] */ IApartment* apartment)
{
    if (mContext != context) {
        return E_INVALID_ARGUMENT;
    }
    if (mApartment != apartment) {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}


LoadedCap::ServiceDispatcher::RunConnection::RunConnection(
   /* [in] */ ServiceDispatcher* serviceDispatcher,
   /* [in] */ IComponentName* name,
   /* [in] */ IBinder* service,
   /* [in] */ Int32 command)
    : mServiceDispatcher(serviceDispatcher)
    , mName(name)
    , mService(service)
{}

ECode LoadedCap::ServiceDispatcher::RunConnection::Run()
{
    if (mCommand == 0) {
        return mServiceDispatcher->DoConnected(mName, mService);
    }
    else if (mCommand == 1) {
        return mServiceDispatcher->DoDeath(mName, mService);
    }
}


LoadedCap::ServiceDispatcher::ServiceDispatcher(
    /* [in] */ IServiceConnection* conn,
    /* [in] */ IContext* context,
    /* [in] */ IApartment* apartment,
    /* [in] */ Int32 flags)
{
    CInnerConnection::NewByFriend((CInnerConnection**)&mIServiceConnection);
    mIServiceConnection->Init(this);
    mConnection = conn;
    mContext = context;
    mApartment = apartment;
//                mLocation = new ServiceConnectionLeaked(null);
//                mLocation.fillInStackTrace();
    mFlags = flags;
}

ECode LoadedCap::ServiceDispatcher::Validate(
   /* [in] */ IContext* context,
   /* [in] */ IApartment* apartment)
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
    assert(0);
    return E_NOT_IMPLEMENTED;
}

void LoadedCap::ServiceDispatcher::DoForget()
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

IServiceConnectionInner*
LoadedCap::ServiceDispatcher::GetIServiceConnection()
{
   return mIServiceConnection;
}

Int32 LoadedCap::ServiceDispatcher::GetFlags()
{
    return mFlags;
}

void LoadedCap::ServiceDispatcher::Connected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    // if (mAppApartment != NULL) {
    //     RunConnection* con = new RunConnection(this, name, service, 0);
    //     PerformServiceConnected(con);
    // }
    // else {
        DoConnected(name, service);
    // }
}

ECode LoadedCap::ServiceDispatcher::DoConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    ConnectionInfo* old = NULL;
    ConnectionInfo* info;

    HashMap<AutoPtr<IComponentName>, ConnectionInfo*>::Iterator it =
            mActiveConnections.Find(name);
    if (it != mActiveConnections.End()) {
        old = it->mSecond;
    }
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
    }
    else {
        // The named service is being disconnected... clean up.
        mActiveConnections.Erase(name);
    }

//            if (old != null) {
//                old.binder.unlinkToDeath(old.deathMonitor, 0);
//            }

    // If there was an old service, it is not disconnected.
    if (old != NULL) {
        FAIL_RETURN(mConnection->OnServiceDisconnected(name));
    }
    // If there is a new service, it is now connected.
    if (service != NULL) {
        FAIL_RETURN(mConnection->OnServiceConnected(name, service));
    }
}

ECode LoadedCap::ServiceDispatcher::DoDeath(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    return mConnection->OnServiceDisconnected(name);
}

ECode LoadedCap::ServiceDispatcher::PerformServiceConnected(
    /* [in] */ RunConnection* con)
{
    ECode (STDCALL LoadedCap::ServiceDispatcher::*pHandlerFunc)(
            RunConnection* com);
    pHandlerFunc = &LoadedCap::ServiceDispatcher::HandleServiceConnected;
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)con);
    return mApartment->PostCppCallback(
            (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode LoadedCap::ServiceDispatcher::HandleServiceConnected(
    /* [in] */ RunConnection* con)
{
    if (con != NULL) {
        return con->Run();
    }
    return NOERROR;
}


LoadedCap::LoadedCap(
    /* [in] */ CApplicationApartment* appApartment,
    /* [in] */ const String& name,
    /* [in] */ IContext* systemContext,
    /* [in] */ CApplicationInfo* info)
{
    mAppApartment = appApartment;
    mApplicationInfo = info;
    if (info == NULL) {
        CApplicationInfo::NewByFriend((CApplicationInfo**)&mApplicationInfo);
    }
    mApplicationInfo->mCapsuleName = name;
    //TODO: Add
    mApplicationInfo->mProcessName = name;
    mCapsuleName = name;
    mAppDir = NULL;
    mResDir = NULL;
    //mSharedLibraries = NULL;
    mDataDir = NULL;
    mDataDirFile = NULL;
    //mLibDir = NULL;
    //mBaseClassLoader = NULL;
    mSecurityViolation = FALSE;
    mIncludeCode = TRUE;
    //systemContext->GetClassLoader((IClassLoader**) &mClassLoader);
    GetClassLoader((IClassLoader**)&mClassLoader);
    systemContext->GetResources((IResources**) &mResources);
    CCompatibilityInfo::NewByFriend((IApplicationInfo*)mApplicationInfo,
            (CCompatibilityInfo**)&mCompatibilityInfo);
}

LoadedCap::LoadedCap(
    /* [in] */ CApplicationApartment* appApartment,
    /* [in] */ CApplicationInfo* aInfo,
    /* [in] */ CApplicationApartment* mainApartment,
    /* [in] */ Boolean securityViolation,
    /* [in] */ Boolean includeCode)
{
    mAppApartment = appApartment;
    mApplicationInfo = aInfo;
    mCapsuleName = aInfo->mCapsuleName;
//    mResDir = aInfo.uid == Process.myUid() ? aInfo.sourceDir
//                : aInfo.publicSourceDir;
    mDataDir = aInfo->mDataDir;
    if (!mDataDir.IsNull()) {
        CFile::New(mDataDir, (IFile**)&mDataDirFile);
    }
    else {
        mDataDirFile = NULL;
    }
    mResDir = aInfo->mSourceDir;
    mSecurityViolation = securityViolation;
    mIncludeCode = includeCode;
    CCompatibilityInfo::NewByFriend((IApplicationInfo*)aInfo,
            (CCompatibilityInfo**)&mCompatibilityInfo);
}

LoadedCap::~LoadedCap()
{
}

AutoPtr<IApplication> LoadedCap::GetApplication()
{
    return mApplication;
}

Boolean LoadedCap::IsSecurityViolation()
{
    return mSecurityViolation;
}

ECode LoadedCap::GetCapsuleName(
    /* [out] */ String* capsuleName)
{
    if (capsuleName == NULL) return E_INVALID_ARGUMENT;
    *capsuleName = mCapsuleName;
    return NOERROR;
}

ECode LoadedCap::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = mApplicationInfo;
    if (*info != NULL) (*info)->AddRef();
    return NOERROR;
}

ECode LoadedCap::GetClassLoader(
    /* [out] */ IClassLoader** loader)
{
    VALIDATE_NOT_NULL(loader);

    Mutex::Autolock lock(mLock);

    if (mClassLoader != NULL) {
        *loader = mClassLoader.Get();
        (*loader)->AddRef();
        return NOERROR;
    }

//    if (mIncludeCode && !mPackageName.equals("android")) {
//        String zip = mAppDir;
//
//        /*
//         * The following is a bit of a hack to inject
//         * instrumentation into the system: If the app
//         * being started matches one of the instrumentation names,
//         * then we combine both the "instrumentation" and
//         * "instrumented" app into the path, along with the
//         * concatenation of both apps' shared library lists.
//         */
//
//        String instrumentationAppDir =
//                mActivityThread.mInstrumentationAppDir;
//        String instrumentationAppPackage =
//                mActivityThread.mInstrumentationAppPackage;
//        String instrumentedAppDir =
//                mActivityThread.mInstrumentedAppDir;
//        String[] instrumentationLibs = null;
//
//        if (mAppDir.equals(instrumentationAppDir)
//                || mAppDir.equals(instrumentedAppDir)) {
//            zip = instrumentationAppDir + ":" + instrumentedAppDir;
//            if (! instrumentedAppDir.equals(instrumentationAppDir)) {
//                instrumentationLibs =
//                    getLibrariesFor(instrumentationAppPackage);
//            }
//        }
//
//        if ((mSharedLibraries != null) ||
//                (instrumentationLibs != null)) {
//            zip =
//                combineLibs(mSharedLibraries, instrumentationLibs)
//                + ':' + zip;
//        }
//
//        /*
//         * With all the combination done (if necessary, actually
//         * create the class loader.
//         */
//
//        if (ActivityThread.localLOGV)
//            Slog.v(ActivityThread.TAG, "Class path: " + zip + ", JNI path: " + mLibDir);
//
//        mClassLoader =
//            ApplicationLoaders.getDefault().getClassLoader(
//                zip, mLibDir, mBaseClassLoader);
//        initializeJavaContextClassLoader();
//    } else {
//        if (mBaseClassLoader == null) {
//            mClassLoader = ClassLoader.getSystemClassLoader();
//        } else {
//            mClassLoader = mBaseClassLoader;
//        }
//        return mClassLoader;
//    }
    //todo:
    ASSERT_SUCCEEDED(CClassLoader::New((IClassLoader**)&mClassLoader));
    *loader = mClassLoader.Get();
    (*loader)->AddRef();
    return NOERROR;
}

ECode LoadedCap::GetAppDir(
    /* [out] */ String* appDir)
{
    VALIDATE_NOT_NULL(appDir);
    *appDir = mAppDir;
    return NOERROR;
}

ECode LoadedCap::GetResDir(
    /* [out] */ String* resDir)
{
    VALIDATE_NOT_NULL(resDir);
    *resDir = mResDir;
    return NOERROR;
}

ECode LoadedCap::GetDataDirFile(
    /* [out] */ IFile** dirFile)
{
    VALIDATE_NOT_NULL(dirFile);
    *dirFile = mDataDirFile;
    return NOERROR;
}


ECode LoadedCap::GetResources(
    /* [in] */ CApplicationApartment* apartment,
    /* [out] */ CResources** res)
{
    assert(apartment);
    assert(res);

    if (mResources == NULL) {
        // TODO: delete this line.
        String resPath = mResDir + "/hello.apk";
        FAIL_RETURN(apartment->GetTopLevelResources(
            resPath/*mResDir*/, this, (CResources**)&mResources));
    }
    *res = mResources.Get();
    (*res)->AddRef();
    return NOERROR;
}

ECode LoadedCap::MakeApplication(
    /* [in] */ Boolean forceDefaultAppClass,
    /* [in] */ IInstrumentation* instrumentation,
    /* [out] */ IApplication** app)
{
    assert(app != NULL);
    if (mApplication != NULL) {
        *app = (IApplication*)mApplication;
        (*app)->AddRef();
        return NOERROR;
    }

    String module;
    String appClass = mApplicationInfo->mClassName;
    if (forceDefaultAppClass || appClass.IsNull()) {
        appClass = "CApplication" /* "android.app.Application" */;
//        char cwd[512];
//        kdGetcwdKT(cwd, 512);
        module = "../Elastos.Framework.Core.eco";
    }

//    try {
    AutoPtr<CContextImpl> appContext;
    CContextImpl::NewByFriend((CContextImpl**)&appContext);
    appContext->Init(this, NULL, mAppApartment);
    mAppApartment->mInstrumentation->NewApplication(
            module, appClass, appContext, app);
//        appContext.setOuterContext(app);
//    } catch (Exception e) {
//        if (!mActivityThread.mInstrumentation.onException(app, e)) {
//            throw new RuntimeException(
//                "Unable to instantiate application " + appClass
//                + ": " + e.toString(), e);
//        }
//    }
    mAppApartment->mAllApplications.PushBack(*app);
    mApplication = *app;

    if (instrumentation != NULL) {
//        try {
        instrumentation->CallApplicationOnCreate(*app);
//        } catch (Exception e) {
//            if (!instrumentation.onException(app, e)) {
//                throw new RuntimeException(
//                    "Unable to create application " + app.getClass().getName()
//                    + ": " + e.toString(), e);
//            }
//        }
    }

    return NOERROR;
}

ECode LoadedCap::RemoveContextRegistrations(
    /* [in] */ IContext* context,
    /* [in] */ const String& who,
    /* [in] */ const String& what)
{
//    HashMap<BroadcastReceiver, ReceiverDispatcher> rmap =
//        mReceivers.remove(context);
//    if (rmap != null) {
//        Iterator<ReceiverDispatcher> it = rmap.values().iterator();
//        while (it.hasNext()) {
//            ReceiverDispatcher rd = it.next();
//            IntentReceiverLeaked leak = new IntentReceiverLeaked(
//                    what + " " + who + " has leaked IntentReceiver "
//                    + rd.getIntentReceiver() + " that was " +
//                    "originally registered here. Are you missing a " +
//                    "call to unregisterReceiver()?");
//            leak.setStackTrace(rd.getLocation().getStackTrace());
//            Slog.e(TAG, leak.getMessage(), leak);
//            try {
//                ActivityManagerNative.getDefault().unregisterReceiver(
//                        rd.getIIntentReceiver());
//            } catch (RemoteException e) {
//                // system crashed, nothing we can do
//            }
//        }
//    }
//    mUnregisteredReceivers.remove(context);
//    //Slog.i(TAG, "Receiver registrations: " + mReceivers);
//    HashMap<ServiceConnection, ServiceDispatcher> smap =
//        mServices.remove(context);
//    if (smap != null) {
//        Iterator<ServiceDispatcher> it = smap.values().iterator();
//        while (it.hasNext()) {
//            ServiceDispatcher sd = it.next();
//            ServiceConnectionLeaked leak = new ServiceConnectionLeaked(
//                    what + " " + who + " has leaked ServiceConnection "
//                    + sd.getServiceConnection() + " that was originally bound here");
//            leak.setStackTrace(sd.getLocation().getStackTrace());
//            Slog.e(TAG, leak.getMessage(), leak);
//            try {
//                ActivityManagerNative.getDefault().unbindService(
//                        sd.getIServiceConnection());
//            } catch (RemoteException e) {
//                // system crashed, nothing we can do
//            }
//            sd.doForget();
//        }
//    }
//    mUnboundServices.remove(context);
//    //Slog.i(TAG, "Service registrations: " + mServices);
    return E_NOT_IMPLEMENTED;
}


ECode LoadedCap::ForgetReceiverDispatcher(
    /* [in] */ IContext* context,
    /* [in] */ IBroadcastReceiver* r,
    /* [out] */ IIntentReceiver** ir)
{
    Mutex::Autolock lock(&mReceiverLock);
    
    LoadedCap::ReceiverDispatcher* rd =NULL;
    Map< AutoPtr<IBroadcastReceiver>, LoadedCap::ReceiverDispatcher* >* map = NULL;
    Map< AutoPtr<IBroadcastReceiver>,LoadedCap::ReceiverDispatcher* >* holder = NULL;
    Map<AutoPtr<IContext>, Map<AutoPtr<IBroadcastReceiver>, ReceiverDispatcher*>* >::Iterator it = \
            mReceivers.Find(context);
    if (it != mReceivers.End()) map = it->mSecond;
    if(map != NULL){
        Map<AutoPtr<IBroadcastReceiver>, ReceiverDispatcher*>::Iterator ite = map->Find(r);
        if (ite != map->End()) rd = ite->mSecond;
        if(rd != NULL){
            map->Erase(ite);
            if (map->Begin() == map->End()) {
                mReceivers.Erase(it);
            }
            Boolean b;
            r->GetDebugUnregister(&b);
            if(b){
                Map<AutoPtr<IContext>,Map<AutoPtr<IBroadcastReceiver>, ReceiverDispatcher*>* >::Iterator iter = \
                        mUnregisteredReceivers.Find(context);
                if(iter != mUnregisteredReceivers.End()) holder = iter->mSecond;
                if(holder == NULL){
                    holder = new Map<AutoPtr<IBroadcastReceiver>,LoadedCap::ReceiverDispatcher* >();
                    mUnregisteredReceivers[context] = holder;
                }
//             RuntimeException ex = new IllegalArgumentException(
//                     "Originally unregistered here:");
//             ex.fillInStackTrace();
//             rd.setUnregisterLocation(ex);
                (*holder)[r] = rd;
            }
            return rd->GetIIntentReceiver(ir);
        }
    }
    Map<AutoPtr<IContext>,Map<AutoPtr<IBroadcastReceiver>, ReceiverDispatcher*>* >::Iterator iter = \
            mUnregisteredReceivers.Find(context);
    if(iter != mUnregisteredReceivers.End()) holder = iter->mSecond;
    if(holder != NULL){
        Map<AutoPtr<IBroadcastReceiver>, ReceiverDispatcher*>::Iterator ite = holder->Find(r);
        if (ite != holder->End()) rd = ite->mSecond;
        if (rd != NULL){
        //         RuntimeException ex = rd.getUnregisterLocation();
        //         throw new IllegalArgumentException(
        //                 "Unregistering Receiver " + r
        //                 + " that was already unregistered", ex);
            return E_RUNTIME_EXCEPTION;
        }
    }
    if(context == NULL){
        //     throw new IllegalStateException("Unbinding Receiver " + r
        //             + " from Context that is no longer in use: " + context);
       return E_RUNTIME_EXCEPTION;
    }
    else {
        //     throw new IllegalArgumentException("Receiver not registered: " + r);
        return E_RUNTIME_EXCEPTION;
    }
}

ECode LoadedCap::GetReceiverDispatcher(
    /* [in] */ IBroadcastReceiver* r,
    /* [in] */ IContext* context,
    /* [in] */ IApartment* handler,
    /* [in] */ IInstrumentation* instrumentation,
    /* [in] */ Boolean registered,
    /* [out] */ IIntentReceiver** ir)
{
    Mutex::Autolock lock(&mReceiverLock);
    
    LoadedCap::ReceiverDispatcher* rd = NULL;
    Map< AutoPtr<IBroadcastReceiver>,LoadedCap::ReceiverDispatcher* >* map = NULL;
    if(registered){
        Map<AutoPtr<IContext>,Map<AutoPtr<IBroadcastReceiver>, ReceiverDispatcher*>* >::Iterator it =\
            mReceivers.Find(context);
        if(it != mReceivers.End())map = it->mSecond;
        if(map != NULL){
            Map<AutoPtr<IBroadcastReceiver>, ReceiverDispatcher*>::Iterator ite = NULL;
            ite = map->Find(r);
            if(ite != map->End())rd = ite->mSecond;
        }
    }
    if(rd == NULL){
        rd = new ReceiverDispatcher(r, context, handler, instrumentation, registered);
        if(registered){
            if(map == NULL){
                map = new Map<AutoPtr<IBroadcastReceiver>,LoadedCap::ReceiverDispatcher* >();
                mReceivers[context] = map;
            }
            //map[r] = rd;
            map->Insert(Pair<AutoPtr<IBroadcastReceiver>,LoadedCap::ReceiverDispatcher* >(r,rd));
        }
    }
    else {
        FAIL_RETURN(rd->Validate(context,handler));
    }
    return rd->GetIIntentReceiver(ir);
}


IServiceConnectionInner* LoadedCap::GetServiceDispatcher(
    /* [in] */ IServiceConnection* c,
    /* [in] */ IContext* context,
    /* [in] */ IApartment* apartment,
    /* [in] */ Int32 flags)
{
    Mutex::Autolock lock(mServicesLock);

    ServiceDispatcher* sd = NULL;
    Map<AutoPtr<IServiceConnection>, ServiceDispatcher*>* map = mServices[context];
    if (map != NULL) {
        sd = (*map)[c];
    }
    if (sd == NULL) {
        sd = new ServiceDispatcher(c, context, apartment, flags);
        if (map == NULL) {
            map = new Map<AutoPtr<IServiceConnection>, ServiceDispatcher*>();
            mServices[context] = map;
        }
        (*map)[c] = sd;
    } else {
        sd->Validate(context, apartment);
    }
    return sd->GetIServiceConnection();
}

IServiceConnectionInner* LoadedCap::ForgetServiceDispatcher(
    /* [in] */ IContext* context,
    /* [in] */ IServiceConnection* c)
{
    Mutex::Autolock lock(mServicesLock);

    ServiceDispatcher* sd = NULL;
    Map<AutoPtr<IServiceConnection>, ServiceDispatcher*>* map = mServices[context];
    if (map != NULL) {
        sd = (*map)[c];
        if (sd != NULL) {
            map->Erase(c);
            sd->DoForget();
            if (map->GetSize() == 0) {
                mServices.Erase(context);
            }
            if ((sd->GetFlags() & Context_BIND_DEBUG_UNBIND) != 0) {
                Map<AutoPtr<IServiceConnection>, ServiceDispatcher*>* holder
                        = mUnboundServices[context];
                if (holder == NULL) {
                    holder = new Map<AutoPtr<IServiceConnection>, ServiceDispatcher*>();
                    mUnboundServices[context] = holder;
                }
//                RuntimeException ex = new IllegalArgumentException(
//                        "Originally unbound here:");
//                ex.fillInStackTrace();
//                sd.setUnbindLocation(ex);
                (*holder)[c] = sd;
            }
            return sd->GetIServiceConnection();
        }
    }
    Map<AutoPtr<IServiceConnection>, ServiceDispatcher*>* holder
            = mUnboundServices[context];
    if (holder != NULL) {
        sd = (*holder)[c];
        if (sd != NULL) {
//            RuntimeException ex = sd.getUnbindLocation();
//            throw new IllegalArgumentException(
//                    "Unbinding Service " + c
//                    + " that was already unbound", ex);
            return NULL;
        }
    }
    if (context == NULL) {
//        throw new IllegalStateException("Unbinding Service " + c
//                + " from Context that is no longer in use: " + context);
        return NULL;
    } else {
//        throw new IllegalArgumentException("Service not registered: " + c);
        return NULL;
    }
}

