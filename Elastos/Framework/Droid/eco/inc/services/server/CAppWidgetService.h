
#ifndef __CAPPWIDGETSERVICE_H__
#define __CAPPWIDGETSERVICE_H__

#include "_CAppWidgetService.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>

CarClass(CAppWidgetService)
{
public:
    struct AppWidgetId;

    /*
     * When identifying a Host or Provider based on the calling process, use the uid field.
     * When identifying a Host or Provider based on a package manager broadcast, use the
     * package given.
     */

    struct Provider {
        Int32 mUid;
        AutoPtr<IAppWidgetProviderInfo> mInfo;
        List<AppWidgetId*> mInstances;
        AutoPtr<IPendingIntent> mBroadcast;
        Boolean mZombie; // if we're in safe mode, don't prune this just because nobody references it

        Int32 mTag;    // for use while saving state (the index)
    };

    struct Host {
        Int32 mUid;
        Int32 mHostId;
        String mCapsuleName;
        List<AppWidgetId*> instances;
        AutoPtr<IAppWidgetHost> mCallbacks;
        Boolean mZombie; // if we're in safe mode, don't prune this just because nobody references it
        Int32 mTag;    // for use while saving state (the index)
    };

    struct AppWidgetId {
        Int32 mAppWidgetId;
        Provider* mProvider;
        AutoPtr<IRemoteViews> mViews;
        Host* mHost;
    };

public:
    CAppWidgetService();

    CARAPI_(void) SystemReady(
        /* [in] */ Boolean safeMode);

    //
    // for AppWidgetHost
    //
    CARAPI StartListening(
        /* [in] */ IAppWidgetHost* host,
        /* [in] */ const String& packageName,
        /* [in] */ Int32 hostId,
        /* [out] */ IObjectContainer** updatedViews,
        /* [out, callee]*/ ArrayOf<Int32>** updatedIds);

    CARAPI StopListening(
        /* [in] */ Int32 hostId);

    CARAPI AllocateAppWidgetId(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 hostId,
        /* [out] */ Int32* appWidgetId);

    CARAPI DeleteAppWidgetId(
        /* [in] */ Int32 appWidgetId);

    CARAPI DeleteHost(
        /* [in] */ Int32 hostId);

    CARAPI DeleteAllHosts();

    CARAPI GetAppWidgetViews(
        /* [in] */ Int32 appWidgetId,
        /* [out] */ IRemoteViews** views);

    //
    // for AppWidgetManager
    //
    CARAPI UpdateAppWidgetIds(
        /* [in] */ const ArrayOf<Int32>& appWidgetIds,
        /* [in] */ IRemoteViews* views);

    CARAPI UpdateAppWidgetProvider(
        /* [in] */ IComponentName* provider,
        /* [in] */ IRemoteViews* views);

    CARAPI GetInstalledProviders(
        /* [out] */ IObjectContainer** providers);

    CARAPI GetAppWidgetInfo(
        /* [in] */ Int32 appWidgetId,
        /* [out] */ IAppWidgetProviderInfo** providerInfo);

    CARAPI BindAppWidgetId(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IComponentName* provider);

    CARAPI GetAppWidgetIds(
        /* [in] */ IComponentName* provider,
        /* [out, callee] */ ArrayOf<Int32>** appWidgetIds);

    CARAPI constructor(
        /* [in] */ IContext* context);

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
    List<Provider*> mInstalledProviders;
    Int32 mNextAppWidgetId;
    List<AppWidgetId*> mAppWidgetIds;
    List<Host*> mHosts;
    Boolean mSafeMode;
};

#endif // __CAPPWIDGETSERVICE_H__

