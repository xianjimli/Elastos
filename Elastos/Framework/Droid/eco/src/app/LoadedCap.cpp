
#include "app/LoadedCap.h"
#include "app/ActivityManagerNative.h"
#include "app/CContextImpl.h"
#include "app/CInnerConnection.h"
#include "os/CClassLoader.h"
#include <Slogger.h>
#include <StringBuffer.h>

using namespace Elastos::Utility::Logging;

ECode
LoadedCap::ReceiverDispatcher::
InnerReceiver::PerformReceive(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean ordered,
    /* [in] */ Boolean sticky)
{
    if (CApplicationApartment::DEBUG_BROADCAST) {
        Int32 seq = -1;
        intent->GetInt32Extra(String("seq"), &seq);
        String action;
        intent->GetAction(&action);
        Slogger::I(CApplicationApartment::TAG,
                StringBuffer("Receiving broadcast ") + action +
                " seq=" + seq + " to " + (Int32)mDispatcher);
    }
    if (mDispatcher != NULL) {
        return mDispatcher->PerformReceive(intent, resultCode, data, extras,
                ordered, sticky);
    }
    else {
        // The activity manager dispatched a broadcast to a registered
        // receiver in this process, but before it could be delivered the
        // receiver was unregistered.  Acknowledge the broadcast on its
        // behalf so that the system's broadcast sequence can continue.
        if (CApplicationApartment::DEBUG_BROADCAST) {
            Slogger::I(CApplicationApartment::TAG,
                "Finishing broadcast to unregistered receiver");
        }
        AutoPtr<IActivityManager> mgr;
        ActivityManagerNative::GetDefault((IActivityManager**)&mgr);
        ECode ec = mgr->FinishReceiver((IBinder*)(IIntentReceiver*)this,
                resultCode, data, extras, FALSE);
        if (FAILED(ec)) {
            Slogger::W(CApplicationApartment::TAG,
                    "Couldn't finish broadcast to unregistered receiver");
        }
    }
    return NOERROR;
}

LoadedCap::ReceiverDispatcher::ReceiverDispatcher(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IContext* context,
    /* [in] */ IApartment* apartment,
    /* [in] */ IInstrumentation* instrumentation,
    /* [in] */ Boolean registered) :
    mApartment(apartment)
{}

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
        intent->GetInt32Extra(String("seq"), &seq);
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
        args->mCurIntent->GetInt32Extra(String("seq"), &seq);
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
                        StringBuffer("Finishing null broadcast to ") +
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
    mResDir = aInfo->mSourceDir;
    mSecurityViolation = securityViolation;
    mIncludeCode = includeCode;
    CCompatibilityInfo::NewByFriend((IApplicationInfo*)aInfo,
            (CCompatibilityInfo**)&mCompatibilityInfo);
}

LoadedCap::~LoadedCap()
{
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
    if (appDir == NULL) return E_INVALID_ARGUMENT;
    *appDir = mAppDir;
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
        ECode ec = apartment->GetTopLevelResources(
            resPath/*mResDir*/, this, (CResources**)&mResources);
        if (FAILED(ec)) return ec;
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
        module = "../Elastos.Framework.eco";
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

IServiceConnectionInner* LoadedCap::GetServiceDispatcher(
    /* [in] */ IServiceConnection* c,
    /* [in] */ IContext* context,
    /* [in] */ IApplicationApartment* apartment,
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

LoadedCap::ServiceDispatcher::ServiceDispatcher(
    /* [in] */ IServiceConnection* conn,
    /* [in] */ IContext* context,
    /* [in] */ IApplicationApartment* apartment,
    /* [in] */ Int32 flags)
{
    CInnerConnection::NewByFriend((CInnerConnection**)&mIServiceConnection);
    mIServiceConnection->Init(this);
    mConnection = conn;
    mContext = context;
    mAppApartment = apartment;
//                mLocation = new ServiceConnectionLeaked(null);
//                mLocation.fillInStackTrace();
    mFlags = flags;
}

IServiceConnectionInner*
LoadedCap::ServiceDispatcher::GetIServiceConnection()
{
   return mIServiceConnection;
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
    return ((CApplicationApartment*)mAppApartment.Get())->mApartment->PostCppCallback(
            (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode LoadedCap::ServiceDispatcher::HandleServiceConnected(
    /* [in] */ RunConnection* con)
{
    ECode ec = NOERROR;

    if (con) {
        ec = con->Run();
    }

    return ec;
}
