
#ifndef __CAPPWIDGETSERVICE_H__
#define __CAPPWIDGETSERVICE_H__

#include "_CAppWidgetService.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/Mutex.h>
#include <elastos/ElRefBase.h>

using namespace Elastos::Core::Threading;

CarClass(CAppWidgetService)
{
public:
    struct AppWidgetId;

    /*
     * When identifying a Host or Provider based on the calling process, use the uid field.
     * When identifying a Host or Provider based on a package manager broadcast, use the
     * package given.
     */

    class Provider : public ElRefBase {
    public:
        Provider()
            : mUid(0)
            , mZombie(FALSE)
            , mTag(0)
        {}

    public:
        Int32 mUid;
        AutoPtr<IAppWidgetProviderInfo> mInfo;
        List<AutoPtr<AppWidgetId> > mInstances;
        AutoPtr<IPendingIntent> mBroadcast;
        Boolean mZombie; // if we're in safe mode, don't prune this just because nobody references it

        Int32 mTag;    // for use while saving state (the index)
    };

    class Host : public ElRefBase {
    public:
        Host()
            : mUid(0)
            , mHostId(0)
            , mZombie(FALSE)
            , mTag(0)
        {}

    public:
        Int32 mUid;
        Int32 mHostId;
        String mCapsuleName;
        List<AutoPtr<AppWidgetId> > mInstances;
        AutoPtr<IAppWidgetHost> mCallbacks;
        Boolean mZombie; // if we're in safe mode, don't prune this just because nobody references it
        Int32 mTag;    // for use while saving state (the index)
    };

    class AppWidgetId : public ElRefBase {
    public:
        AppWidgetId()
            : mAppWidgetId(0)
        {}

    public:
        Int32 mAppWidgetId;
        AutoPtr<Provider> mProvider;
        AutoPtr<IRemoteViews> mViews;
        AutoPtr<Host> mHost;
    };

    // class BroadcastReceiver : public IBroadcastReceiver, public ElRefBase 
    // {
    // public:
    //     CARAPI ReceiveBroadcast(
    //         /* [in] */ IContext* ctx,
    //         /* [in] */ IIntent* intent);

    //     CARAPI SetResultCode(
    //         /* [in] */ Int32 code);

    //     CARAPI GetResultCode(
    //         /* [out] */ Int32* code);

    //     CARAPI SetResultData(
    //         /* [in] */ String data);

    //     CARAPI GetResultData(
    //         /* [out] */ String* data);

    //     CARAPI SetResultExtras(
    //         /* [in] */ IBundle* extras);

    //     CARAPI GetResultExtras(
    //         /* [in] */ Boolean makeMap,
    //         /* [out] */ IBundle** extras);

    //     CARAPI SetResult(
    //         /* [in] */ Int32 code,
    //         /* [in] */ String data,
    //         /* [in] */ IBundle* extras);

    //     CARAPI GetAbortBroadcast(
    //         /* [out] */ Boolean* aborted);

    //     CARAPI AbortBroadcast();

    //     CARAPI ClearAbortBroadcast();

    //     CARAPI SetOrderedHint(
    //         /* [in] */ Boolean isOrdered);

    //     CARAPI SetInitialStickyHint(
    //         /* [in] */ Boolean isInitialSticky);

    //     CARAPI_(PInterface) Probe(
    //         /* [in]  */ REIID riid);

    //     CARAPI_(UInt32) AddRef();

    //     CARAPI_(UInt32) Release();

    //     CARAPI GetInterfaceID(
    //         /* [in] */ IInterface *pObject,
    //         /* [out] */ InterfaceID *pIID);
    // };

public:
    CAppWidgetService();

    CARAPI_(void) SystemReady(
        /* [in] */ Boolean safeMode);

    //
    // for AppWidgetHost
    //
    CARAPI AllocateAppWidgetId(
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 hostId,
        /* [out] */ Int32* appWidgetId);

    CARAPI DeleteAppWidgetId(
        /* [in] */ Int32 appWidgetId);

    CARAPI DeleteHost(
        /* [in] */ Int32 hostId);

    CARAPI DeleteAllHosts();

    CARAPI_(void) DeleteHostLocked(
        /* [in] */ Host* host);

    CARAPI_(void) DeleteAppWidgetLocked(
        /* [in] */ AppWidgetId* id);

    CARAPI_(void) CancelBroadcasts(
        /* [in] */ Provider* p);

    CARAPI BindAppWidgetId(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IComponentName* provider);

    CARAPI GetAppWidgetInfo(
        /* [in] */ Int32 appWidgetId,
        /* [out] */ IAppWidgetProviderInfo** providerInfo);

    CARAPI GetAppWidgetViews(
        /* [in] */ Int32 appWidgetId,
        /* [out] */ IRemoteViews** views);

    CARAPI GetInstalledProviders(
        /* [out] */ IObjectContainer** providers);

    //
    // for AppWidgetManager
    //
    CARAPI UpdateAppWidgetIds(
        /* [in] */ const ArrayOf<Int32>& appWidgetIds,
        /* [in] */ IRemoteViews* views);

    CARAPI UpdateAppWidgetProvider(
        /* [in] */ IComponentName* provider,
        /* [in] */ IRemoteViews* views);

    CARAPI_(void) UpdateAppWidgetInstanceLocked(
        /* [in] */ AppWidgetId* id,
        /* [in] */ IRemoteViews* views);

    CARAPI StartListening(
        /* [in] */ IAppWidgetHost* callbacks,
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 hostId,
        /* [out] */ IObjectContainer** updatedViews,
        /* [out, callee]*/ ArrayOf<Int32>** updatedIds);

    CARAPI StopListening(
        /* [in] */ Int32 hostId);

    CARAPI_(Boolean) CanAccessAppWidgetId(
        /* [in] */ AppWidgetId* id,
        /* [in] */ Int32 callingUid);

    CARAPI_(CAppWidgetService::AppWidgetId*) LookupAppWidgetIdLocked(
        /* [in] */ Int32 appWidgetId);

    CARAPI_(Provider*) LookupProviderLocked(
        /* [in] */ IComponentName* provider);

    CARAPI_(CAppWidgetService::Host*) LookupHostLocked(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 hostId);

    CARAPI_(CAppWidgetService::Host*) LookupOrAddHostLocked(
        /* [in] */ Int32 uid,
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 hostId);

    CARAPI_(void) PruneHostLocked(
        /* [in] */ Host* host);

    CARAPI_(void) LoadAppWidgetList();

    CARAPI_(Boolean) AddProviderLocked(
        /* [in] */ IResolveInfo* ri);

    CARAPI_(void) RemoveProviderLocked(
        /* [in] */ Int32 index,
        /* [in] */ Provider* p);

    CARAPI_(void) SendEnableIntentLocked(
        /* [in] */ Provider* p);

    CARAPI_(void) SendUpdateIntentLocked(
        /* [in] */ Provider* p,
        /* [in] */ ArrayOf<Int32>* appWidgetIds);

    CARAPI_(void) RegisterForBroadcastsLocked(
        /* [in] */ Provider* p,
        /* [in] */ ArrayOf<Int32>* appWidgetIds);

    CARAPI GetAppWidgetIds(
        /* [in] */ IComponentName* provider,
        /* [out, callee] */ ArrayOf<Int32>** appWidgetIds);

    static CARAPI_(ArrayOf<Int32>*) GetAppWidgetIds(
        /* [in] */ Provider* p);

    CARAPI GetUidForCapsule(
        /* [in] */ const String& capsuleName,
        /* [out] */ Int32* uid);

    CARAPI EnforceCallingUid(
        /* [in] */ const String& capsuleName,
        /* [out] */ Int32* uid);

    // only call from initialization -- it assumes that the data structures are all empty
    CARAPI_(void) LoadStateLocked();

    CARAPI_(void) SaveStateLocked();

    CARAPI_(Boolean) WriteStateToFileLocked(
        /* [in] */ IFile* file);

    CARAPI_(void) ReadStateFromFileLocked(
        /* [in] */ IFile* file);

    CARAPI_(AutoPtr<IFile>) SavedStateTempFile();

    CARAPI_(AutoPtr<IFile>) SavedStateRealFile();

    CARAPI constructor(
        /* [in] */ IContext* context);

private:
    CARAPI_(Provider*) ParseProviderInfoXml(
        /* [in] */ IComponentName* component,
        /* [in] */ IResolveInfo* ri);

private:
    static const CString TAG;

    static const CString SETTINGS_FILENAME;
    static const String SETTINGS_TMP_FILENAME;
    static const Int32 MIN_UPDATE_PERIOD = 30 * 60 * 1000; // 30 minutes

public:
    AutoPtr<IContext> mContext;
    AutoPtr<ILocale> mLocale;
    AutoPtr<ILocalCapsuleManager> mCapsuleManager;
    //AutoPtr<IAlarmManager> mAlarmManager;
    List<AutoPtr<Provider> > mInstalledProviders;
    Int32 mNextAppWidgetId;
    List<AutoPtr<AppWidgetId> > mAppWidgetIds;
    Mutex mAppWidgetIdsLock;
    List<AutoPtr<Host> > mHosts;
    Boolean mSafeMode;
    //BroadcastReceiver* mBroadcastReceiver;
};

#endif // __CAPPWIDGETSERVICE_H__

