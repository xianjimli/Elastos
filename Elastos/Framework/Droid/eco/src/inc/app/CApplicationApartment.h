
#ifndef __CAPPLICATIONDEPARTMENT_H__
#define __CAPPLICATIONDEPARTMENT_H__

#include "ext/frameworkext.h"
#include "_CApplicationApartment.h"
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>
#include <elastos/List.h>
#include <elastos/Map.h>
#include "content/CApplicationInfo.h"
#include "content/CCompatibilityInfo.h"
#include "content/CResources.h"
#include "capsule/CActivityInfo.h"
#include "capsule/CServiceInfo.h"
#include "app/CResultInfo.h"
#include "utils/CBundle.h"
#include <elastos/Mutex.h>

using namespace Elastos::System::Threading;

class LoadedCap;

CarClass(CApplicationApartment)
{
public:
    static const String TAG;
    static const Boolean DEBUG = TRUE;
    static const Boolean localLOGV = DEBUG /*? Config.LOGD : Config.LOGV*/;
    static const Boolean DEBUG_MESSAGES = FALSE;
    static const Boolean DEBUG_BROADCAST = FALSE;
    static const Boolean DEBUG_RESULTS = FALSE;

    static const Int32 CAPSULE_REMOVED = 0;
    static const Int32 EXTERNAL_STORAGE_UNAVAILABLE = 1;

    class ActivityClientRecord
    {
    public:
        ActivityClientRecord() :
            mNextIdle(NULL),
            mCapsuleInfo(NULL),
            mPendingResults(NULL),
            mPendingIntents(NULL)
        {}

        CARAPI GetDescription(
            /* [out] */ String* description);

    public:
        AutoPtr<IBinder> mToken;
        Int32 mIdent;
        AutoPtr<IIntent> mIntent;
        AutoPtr<CBundle> mState;
        AutoPtr<IActivity> mActivity;
        AutoPtr<IWindow> mWindow;
        AutoPtr<IActivity> mParent;
        String mEmbeddedID;
        AutoPtr<IInterface> mLastNonConfigurationInstance;
        AutoPtr<IObjectStringMap> mLastNonConfigurationChildInstances;
        Boolean mPaused;
        Boolean mStopped;
        Boolean mHideForNow;
        AutoPtr<IConfiguration> mNewConfig;
        AutoPtr<IConfiguration> mCreatedConfig;
        ActivityClientRecord* mNextIdle;

        AutoPtr<CActivityInfo> mActivityInfo;
        LoadedCap* mCapsuleInfo;

        List<AutoPtr<CResultInfo> >* mPendingResults;
        List<AutoPtr<IIntent> >* mPendingIntents;

        Boolean mStartsNotResumed;
        Boolean mIsForward;
    };

    class ProviderClientRecord
    {
    public:
        ProviderClientRecord(
            /* [in] */ String name,
            /* [in] */ IContentProvider* provider,
            /* [in] */ IContentProvider* localProvider);

    public:
        String mName;
        AutoPtr<IContentProvider> mProvider;
        AutoPtr<IContentProvider> mLocalProvider;
    };

    class NewIntentData
    {
    public:
        List<AutoPtr<IIntent> > mIntents;
        IBinder* mToken;
    };

    class ReceiverData
    {
    public:
        AutoPtr<IIntent> mIntent;
        AutoPtr<CActivityInfo> mInfo;
        Int32 mResultCode;
        String mResultData;
        AutoPtr<IBundle> mResultExtras;
        Boolean mSync;
        Boolean mResultAbort;
    };

    class CreateServiceData
    {
    public:
        AutoPtr<IBinder> mToken;
        AutoPtr<CServiceInfo> mInfo;
        AutoPtr<IIntent> mIntent;
    };

    class BindServiceData
    {
    public:
        AutoPtr<IBinder> mToken;
        AutoPtr<IIntent> mIntent;
        Boolean mRebind;
    };

    class ServiceArgsData
    {
    public:
        AutoPtr<IBinder> mToken;
        Int32 mStartId;
        Int32 mFlags;
        AutoPtr<IIntent> mArgs;
    };

    class AppBindData
    {
    public:
        AppBindData() : mInfo(NULL) {}

    public:
        LoadedCap* mInfo;
        String mProcessName;
        AutoPtr<CApplicationInfo> mAppInfo;
        AutoPtr<IObjectContainer> mProviders;
        AutoPtr<IComponentName> mInstrumentationName;
        String mProfileFile;
        AutoPtr<IBundle> mInstrumentationArgs;
        AutoPtr<IInstrumentationWatcher> mInstrumentationWatcher;
        Int32 mDebugMode;
        Boolean mRestrictedBackupMode;
        AutoPtr<IConfiguration> mConfig;
        Boolean mHandlingProfiling;
    };

    class ResultData
    {
    public:
        AutoPtr<IBinder> mToken;
        List<AutoPtr<CResultInfo> > mResults;
    };

    class ContextCleanupInfo
    {
    public:
        AutoPtr<IContext> mContext;
        String mWhat;
        String mWho;
    };

    class ResourcesKey
    {
    public:
        ResourcesKey(
            /* [in] */ String resDir,
            /* [in] */ Float scale)
        {
            mResDir = String::Duplicate(resDir);
            mScale = scale;
            mHash = ((Int32)HashString(mResDir) << 2) + (Int32)(mScale * 2);
        }

        ~ResourcesKey()
        {
            String::Free(mResDir);
        }

        CARAPI_(Int32) GetHashCode() const
        {
            return mHash;
        }

        CARAPI_(Boolean) Equals(
            /* [in] */ const ResourcesKey* peer) const
        {
            return !mResDir.Compare(peer->mResDir)
                    && (mScale == peer->mScale);
        }
    private:
        String mResDir;
        Float mScale;
        Int32 mHash;
    };

    class StopInfo
    {
    public:
//        Bitmap thumbnail;
//        CharSequence description;
    };

public:
    CApplicationApartment();

    ~CApplicationApartment();

    CARAPI GetDescription(
        /* [out] */ String* description);

    CARAPI BindApplication(
        /* [in] */ String processName,
        /* [in] */ IApplicationInfo* appInfo,
        /* [in] */ IObjectContainer* providers,
        /* [in] */ IComponentName* instrumentationName,
        /* [in] */ String profileFile,
        /* [in] */ IBundle* instrumentationArgs,
        /* [in] */ IInstrumentationWatcher* instrumentationWatcher,
        /* [in] */ Int32 debugMode,
        /* [in] */ Boolean isRestrictedBackupMode,
        /* [in] */ IConfiguration* config,
        /* [in] */ IObjectStringMap* services);

    CARAPI AcquireProvider(
        /* [in] */ IContext* context,
        /* [in] */ String name,
        /* [out] */ IContentProvider** provider);

    CARAPI AcquireExistingProvider(
        /* [in] */ IContext* context,
        /* [in] */ String name,
        /* [out] */ IContentProvider** provider);

    CARAPI ReleaseProvider(
        /* [in] */ IContentProvider* provider);

    CARAPI ScheduleExit();

    CARAPI ScheduleLaunchActivity(
        /* [in] */ IIntent *pIntent,
        /* [in] */ IBinder *pToken,
        /* [in] */ Int32 ident,
        /* [in] */ IActivityInfo* info,
        /* [in] */ IBundle* state,
        /* [in] */ IObjectContainer* pendingResults,
        /* [in] */ IObjectContainer* pendingNewIntents,
        /* [in] */ Boolean notResumed,
        /* [in] */ Boolean isForward);

    CARAPI ScheduleRelaunchActivity(
        /* [in] */ IBinder* token,
        /* [in] */ IObjectContainer* pendingResults,
        /* [in] */ IObjectContainer* pendingNewIntents,
        /* [in] */ Int32 configChanges,
        /* [in] */ Boolean notResumed,
        /* [in] */ IConfiguration* config);

    CARAPI ScheduleSendResult(
        /* [in] */ IBinder* token,
        /* [in] */ IObjectContainer* results);

    CARAPI ScheduleResumeActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean isForward);

//    CARAPI ScheduleResumeActivity(
//        /* [in] */ IBinder *desActToken,
//        /* [in] */ IBinder *resActToken,
//        /* [in] */ Boolean returnToCaller,
//        /* [in] */ Int32 requestCode,
//        /* [in] */ Int32 resultCode,
//        /* [in] */ IIntent *resultData);

    CARAPI SchedulePauseActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean finished,
        /* [in] */ Boolean userLeaving,
        /* [in] */ Int32 configChanges);

    CARAPI ScheduleStopActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean showWindow,
        /* [in] */ Int32 configChanges);

    CARAPI ScheduleWindowVisibility(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean showWindow);

    CARAPI ScheduleDestroyActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean finishing,
        /* [in] */ Int32 configChanges);

    CARAPI ScheduleReceiver(
        /* [in] */ IIntent* intent,
        /* [in] */ IActivityInfo* info,
        /* [in] */ Int32 resultCode,
        /* [in] */ String data,
        /* [in] */ IBundle* extras,
        /* [in] */ Boolean sync);

    CARAPI ScheduleRegisteredReceiver(
        /* [in] */ IIntentReceiver* receiver,
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 resultCode,
        /* [in] */ String dataStr,
        /* [in] */ IBundle* extras,
        /* [in] */ Boolean ordered,
        /* [in] */ Boolean sticky);

    CARAPI ScheduleNewIntent(
        /* [in] */ IObjectContainer* intents,
        /* [in] */ IBinder* token);

    CARAPI SendActivityResult(
        /* [in] */ IBinder* token,
        /* [in] */ String id,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* data);

    CARAPI ScheduleCreateService(
        /* [in] */ IBinder* token,
        /* [in] */ IServiceInfo* info);

    CARAPI ScheduleBindService(
        /* [in] */ IBinder* token,
        /* [in] */ IIntent* intent,
        /* [in] */ Boolean rebind);

    CARAPI ScheduleUnbindService(
        /* [in] */ IBinder* token,
        /* [in] */ IIntent* intent);

    CARAPI ScheduleServiceArgs(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 startId,
        /* [in] */ Int32 flags,
        /* [in] */ IIntent* args);

    CARAPI ScheduleStopService(
        /* [in] */ IBinder* token);

    CARAPI ScheduleConfigurationChanged(
        /* [in] */ IConfiguration* config);

    CARAPI ScheduleActivityConfigurationChanged(
        /* [in] */ IBinder* token);

    CARAPI DispatchCapsuleBroadcast(
        /* [in] */ Int32 cmd,
        /* [in] */ const ArrayOf<String>& capsules);

    CARAPI Startup(
        /* [in] */ ApartmentAttr attr);

public:
    CARAPI GetDisplayMetricsLocked(
        /* [in] */ Boolean forceUpdate,
        /* [out] */ IDisplayMetrics** dm);

    /**
     * Creates the top level Resources for applications with the given compatibility info.
     *
     * @param resDir the resource directory.
     * @param compInfo the compability info. It will use the default compatibility info when it's
     * null.
     */
    CARAPI GetTopLevelResources(
        /* [in] */ String resDir,
        /* [in] */ CCompatibilityInfo* compInfo,
        /* [out] */ CResources** res);

    /**
     * Creates the top level resources for the given package.
     */
    CARAPI GetTopLevelResources(
        /* [in] */ String resDir,
        /* [in] */ LoadedCap* capInfo,
        /* [out] */ CResources** res);

    CARAPI_(LoadedCap*) GetCapsuleInfo(
        /* [in] */ String capsuleName,
        /* [in] */ Int32 flags);

    CARAPI_(LoadedCap*) GetCapsuleInfo(
        /* [in] */ CApplicationInfo* ai,
        /* [in] */ Int32 flags);

    CARAPI_(IInstrumentation*) GetInstrumentation();

    CARAPI_(IConfiguration*) GetConfiguration();

    CARAPI ScheduleContextCleanup(
        /* [in] */ IContext* context,
        /* [in] */ String who,
        /* [in] */ String what);

private:
    CARAPI HandleBindApplication(
        /* [in] */ AppBindData* data);

    CARAPI HandleLaunchActivity(
        /* [in] */ ActivityClientRecord* r,
        /* [in] */ IIntent* customIntent);

    CARAPI PerformLaunchActivity(
        /* [in] */ ActivityClientRecord* r,
        /* [in] */ IIntent* customIntent,
        /* [out] */ IActivity** activity);

    CARAPI HandleSendResult(
        /* [in] */ ResultData* res);

    CARAPI HandleResumeActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean clearHide,
        /* [in] */ Boolean isForward);

    CARAPI_(ActivityClientRecord*) PerformResumeActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean clearHide);

//    CARAPI PerformResumeActivity(
//        /* [in] */ IBinder *pDesActToken,
//        /* [in] */ IBinder *pResActToken,
//        /* [in] */ Boolean sendResult,
//        /* [in] */ Int32 requestCode,
//        /* [in] */ Int32 resultCode,
//        /* [in] */ IIntent *pResultData);

    CARAPI HandlePauseActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean finished,
        /* [in] */ Boolean userLeaving,
        /* [in] */ Int32 configChanges);

    CARAPI PerformUserLeavingActivity(
        /* [in] */ ActivityClientRecord* r);

    CARAPI PerformPauseActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean finished,
        /* [in] */ Boolean saveState,
        /* [out] */ IBundle** state);

    CARAPI PerformPauseActivity(
        /* [in] */ ActivityClientRecord* r,
        /* [in] */ Boolean finished,
        /* [in] */ Boolean saveState,
        /* [out] */ IBundle** state);

    CARAPI HandleStopActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean show,
        /* [in] */ Int32 configChanges);

    CARAPI PerformStopActivity(
        /* [in] */ IBinder* token);

    CARAPI PerformStopActivityInner(
        /* [in] */ ActivityClientRecord* r,
        /* [in] */ StopInfo* info,
        /* [in] */ Boolean keepShown);

    CARAPI HandleWindowVisibility(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean show);

    CARAPI_(ActivityClientRecord*) PerformDestroyActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean finishing);

    CARAPI_(ActivityClientRecord*) PerformDestroyActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean finishing,
        /* [in] */ Int32 configChanges,
        /* [in] */ Boolean getNonConfigInstance);

    CARAPI HandleDestroyActivity(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean finishing,
        /* [in] */ Int32 configChanges,
        /* [in] */ Boolean getNonConfigInstance);

    CARAPI HandleRelaunchActivity(
        /* [in] */ ActivityClientRecord* tmp,
        /* [in] */ Int32 configChanges);

    CARAPI HandleNewIntent(
        /* [in] */ NewIntentData* data);

    CARAPI_(void) PerformNewIntents(
        /* [in] */ IBinder* token,
        /* [in] */ List<AutoPtr<IIntent> >* intents);

    CARAPI HandleReceiver(
        /* [in] */ ReceiverData* data);

//    CARAPI PerformNewIntent(
//        /* [in] */ IIntent *intent,
//        /* [in] */ IBinder *token);

    CARAPI HandleCreateService(
        /* [in] */ CreateServiceData* data);

    CARAPI HandleBindService(
        /* [in] */ BindServiceData* data);

    CARAPI HandleUnbindService(
        /* [in] */ BindServiceData* data);

    CARAPI HandleServiceArgs(
        /* [in] */ ServiceArgsData* data);

    CARAPI HandleStopService(
        /* [in] */ IBinder* token);

    CARAPI_(LoadedCap*) GetCapsuleInfoNoCheck(
        /* [in] */ CApplicationInfo* ai);

    CARAPI_(void) DeliverNewIntents(
        /* [in] */ ActivityClientRecord* r,
        /* [in] */ List<AutoPtr<IIntent> >* intents);

    CARAPI DeliverResults(
        /* [in] */ ActivityClientRecord* r,
        /* [in] */ List<AutoPtr<CResultInfo> >* results);

    CARAPI_(LoadedCap*) GetCapsuleInfo(
        /* [in] */ CApplicationInfo* aInfo,
        /* [in] */ Boolean securityViolation,
        /* [in] */ Boolean includeCode);

    CARAPI InstallContentProviders(
        /* [in] */ IContext* context,
        /* [in] */ IObjectContainer* providers);

    CARAPI GetExistingProvider(
        /* [in] */ IContext* context,
        /* [in] */ String name,
        /* [out] */ IContentProvider** provider);

    CARAPI GetProvider(
        /* [in] */ IContext* context,
        /* [in] */ String name,
        /* [out] */ IContentProvider** provider);

    CARAPI InstallProvider(
        /* [in] */ IContext* context,
        /* [in] */ IContentProvider* provider,
        /* [in] */ IContentProviderInfo* info,
        /* [in] */ Boolean noisy,
        /* [out] */ IContentProvider** retProvider);

    CARAPI HandleContextCleanup(
        /* [in] */ ContextCleanupInfo* cci);

    CARAPI_(ICapsuleManager*) GetCapsuleManager();

public:
    AutoPtr<IApplication> mInitialApplication;
    List<AutoPtr<IApplication> > mAllApplications;

private:
    friend class LoadedCap;

    AutoPtr<IApartment> mApartment;

    Map<AutoPtr<IBinder>, ActivityClientRecord*> mActivities;
    Map<AutoPtr<IBinder>, AutoPtr<IService> > mServices;
    AppBindData* mBoundApplication;
    AutoPtr<CConfiguration> mConfiguration;

    AutoPtr<IInstrumentation> mInstrumentation;
    String mInstrumentationAppDir;
    String mInstrumentationAppPackage;
    String mInstrumentedAppDir;

    // These can be accessed by multiple threads; mPackages is the lock.
    // XXX For now we keep around information about all packages we have
    // seen, not removing entries from this map.
    HashMap<String, LoadedCap*> mCapsules;
    Mutex mCapsulesLock;
    HashMap<String, LoadedCap*> mResourceCapsules;
    AutoPtr<IDisplay> mDisplay;
    AutoPtr<IDisplayMetrics> mDisplayMetrics;

    struct HashRK
    {
        size_t operator()(const ResourcesKey* s) const
        {
            return (size_t)s->GetHashCode();
        }
    };

    struct RKEq
    {
        Boolean operator()(const ResourcesKey* x,
                           const ResourcesKey* y) const
        {
            return x->Equals(y);
        }
    };
    HashMap<ResourcesKey*, AutoPtr<CResources>, HashRK, RKEq> mActiveResources;
    List<ActivityClientRecord*> mRelaunchingActivities;

    // The lock of mProviderMap protects the following variables.
    Mutex mProviderMapLock;
    HashMap<String, ProviderClientRecord*> mProviderMap;

    struct HashICP
    {
        size_t operator()(const AutoPtr<IContentProvider> s) const
        {
            return (size_t)(IContentProvider*)s;
        }
    };
    struct ICPEq
    {
        Boolean operator()(AutoPtr<IContentProvider> s1,
                AutoPtr<IContentProvider> s2) const
        {
            return s1 == s2;
        }
    };
    HashMap<AutoPtr<IContentProvider>, Int32,
            HashICP, ICPEq>* mProviderRefCountMap;
};

#endif // __CAPPLICATIONDEPARTMENT_H__
