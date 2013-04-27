
#include "server/CAppWidgetService.h"
#include "os/Binder.h"
#include "os/Process.h"
#include "utils/Xml.h"
#include <elastos/HashMap.h>

const CString CAppWidgetService::TAG = "AppWidgetService";
const CString CAppWidgetService::SETTINGS_FILENAME = "appwidgets.xml";
const String CAppWidgetService::SETTINGS_TMP_FILENAME
        = String(CAppWidgetService::SETTINGS_FILENAME) + String(".tmp");
const Int32 CAppWidgetService::MIN_UPDATE_PERIOD;

CAppWidgetService::CAppWidgetService()
    : mNextAppWidgetId(AppWidgetManager_INVALID_APPWIDGET_ID + 1)
    , mSafeMode(FALSE)
{
    //mBroadcastReceiver = new BroadcastReceiver();
}

void CAppWidgetService::SystemReady(
    /* [in] */ Boolean safeMode)
{
    mSafeMode = safeMode;

    LoadAppWidgetList();
    LoadStateLocked();

    // Register for the boot completed broadcast, so we can send the
    // ENABLE broacasts.  If we try to send them now, they time out,
    // because the system isn't ready to handle them yet.
    // mContext.registerReceiver(mBroadcastReceiver,
    //         new IntentFilter(Intent.ACTION_BOOT_COMPLETED), null, null);

    // // Register for configuration changes so we can update the names
    // // of the widgets when the locale changes.
    // mContext.registerReceiver(mBroadcastReceiver,
    //         new IntentFilter(Intent.ACTION_CONFIGURATION_CHANGED), null, null);

    // // Register for broadcasts about package install, etc., so we can
    // // update the provider list.
    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New((IIntentFilter**)&filter);
    filter->AddAction(String(Intent_ACTION_CAPSULE_ADDED));
    filter->AddAction(String(Intent_ACTION_CAPSULE_REMOVED));
    filter->AddDataScheme(String("capsule"));
    // mContext.registerReceiver(mBroadcastReceiver, filter);
    // // Register for events related to sdcard installation.
    AutoPtr<IIntentFilter> sdFilter;
    CIntentFilter::New((IIntentFilter**)&sdFilter);
    sdFilter->AddAction(String(Intent_ACTION_EXTERNAL_APPLICATIONS_AVAILABLE));
    sdFilter->AddAction(String(Intent_ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE));
    // mContext.registerReceiver(mBroadcastReceiver, sdFilter);
}

ECode CAppWidgetService::AllocateAppWidgetId(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 hostId,
    /* [out] */ Int32* appWidgetId)
{
    VALIDATE_NOT_NULL(appWidgetId);
    Int32 callingUid;
    EnforceCallingUid(capsuleName, &callingUid);
    Mutex::Autolock lock(mAppWidgetIdsLock);
    Int32 awid = mNextAppWidgetId++;

    Host* host = LookupOrAddHostLocked(callingUid, capsuleName, hostId);

    AppWidgetId* id = new AppWidgetId();
    id->mAppWidgetId = awid;
    id->mHost = host;

    host->mInstances.PushBack(id);
    mAppWidgetIds.PushBack(id);

    SaveStateLocked();

    *appWidgetId = awid;

    return NOERROR;
}

ECode CAppWidgetService::DeleteAppWidgetId(
    /* [in] */ Int32 appWidgetId)
{
    Mutex::Autolock lock(mAppWidgetIdsLock);
    AppWidgetId* id = LookupAppWidgetIdLocked(appWidgetId);
    if (id != NULL) {
        DeleteAppWidgetLocked(id);
        SaveStateLocked();
    }

    return NOERROR;
}

ECode CAppWidgetService::DeleteHost(
    /* [in] */ Int32 hostId)
{
    Mutex::Autolock lock(mAppWidgetIdsLock);
    Int32 callingUid = 0;//getCallingUid();
    Host* host = LookupHostLocked(callingUid, hostId);
    if (host != NULL) {
        DeleteHostLocked(host);
        SaveStateLocked();
    }

    return NOERROR;
}

ECode CAppWidgetService::DeleteAllHosts()
{
    Mutex::Autolock lock(mAppWidgetIdsLock);
    Int32 callingUid = 0;//getCallingUid();
    Boolean changed = FALSE;
    List<AutoPtr<Host> >::ReverseIterator rit;
    for (rit = mHosts.RBegin(); rit != mHosts.REnd(); ++rit) {
        AutoPtr<Host> host = *rit;
        if (host->mUid == callingUid) {
            DeleteHostLocked(host);
            changed = TRUE;
        }
    }
    if (changed) {
        SaveStateLocked();
    }

    return NOERROR;
}

void CAppWidgetService::DeleteHostLocked(
    /* [in] */ Host* host)
{
    List<AutoPtr<AppWidgetId> >::ReverseIterator rit;
    for (rit = host->mInstances.RBegin(); rit != host->mInstances.REnd(); ++rit) {
        AutoPtr<AppWidgetId> id = *rit;
        DeleteAppWidgetLocked(id);
    }
    host->mInstances.Clear();
    mHosts.Remove(host);
    // it's gone or going away, abruptly drop the callback connection
    //host->mCallbacks = NULL;
}

void CAppWidgetService::DeleteAppWidgetLocked(
    /* [in] */ AppWidgetId* id)
{
    Host* host = id->mHost;
    host->mInstances.Remove(id);
    PruneHostLocked(host);

    mAppWidgetIds.Remove(id);

    Provider* p = id->mProvider;
    if (p != NULL) {
        p->mInstances.Remove(id);
        if (!p->mZombie) {
            // send the broacast saying that this appWidgetId has been deleted
            AutoPtr<IIntent> intent;
            CIntent::New(String(AppWidgetManager_ACTION_APPWIDGET_DELETED), (IIntent**)&intent);
            AutoPtr<IComponentName> provider;
            p->mInfo->GetProvider((IComponentName**)&provider);
            intent->SetComponent(provider);
            intent->PutInt32Extra(String(AppWidgetManager_EXTRA_APPWIDGET_ID), id->mAppWidgetId);
            mContext->SendBroadcast(intent);
            if (p->mInstances.Begin() == p->mInstances.End()) {
                // cancel the future updates
                CancelBroadcasts(p);

                // send the broacast saying that the provider is not in use any more
                CIntent::New(String(AppWidgetManager_ACTION_APPWIDGET_DISABLED), (IIntent**)&intent);
                intent->SetComponent(provider);
                mContext->SendBroadcast(intent);
            }
        }
    }
}

void CAppWidgetService::CancelBroadcasts(
    /* [in] */ Provider* p)
{
    if (p->mBroadcast != NULL) {
        //mAlarmManager.cancel(p.broadcast);
        Int64 token = Binder::ClearCallingIdentity();
        //try {
        p->mBroadcast->Cancel();
        //} finally {
        Binder::RestoreCallingIdentity(token);
        //}
        p->mBroadcast = NULL;
    }
}

ECode CAppWidgetService::BindAppWidgetId(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IComponentName* provider)
{
    // mContext.enforceCallingPermission(android.Manifest.permission.BIND_APPWIDGET,
    //         "bindGagetId appWidgetId=" + appWidgetId + " provider=" + provider);
    Mutex::Autolock lock(mAppWidgetIdsLock);
    AppWidgetId* id = LookupAppWidgetIdLocked(appWidgetId);
    if (id == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("bad appWidgetId");
    }
    if (id->mProvider != NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalArgumentException("appWidgetId " + appWidgetId + " already bound to "
        //         + id.provider.info.provider);
    }
    Provider* p = LookupProviderLocked(provider);
    if (p == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("not a appwidget provider: " + provider);
    }
    if (p->mZombie) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalArgumentException("can't bind to a 3rd party provider in"
        //         + " safe mode: " + provider);
    }

    id->mProvider = p;
    p->mInstances.PushBack(id);
    Int32 instancesSize = p->mInstances.GetSize();
    if (instancesSize == 1) {
        // tell the provider that it's ready
        SendEnableIntentLocked(p);
    }

    // send an update now -- We need this update now, and just for this appWidgetId.
    // It's less critical when the next one happens, so when we schdule the next one,
    // we add updatePeriodMillis to its start time.  That time will have some slop,
    // but that's okay.
    Int32 ids[] = { appWidgetId };
    ArrayOf<Int32> idsArray(ids, 1);
    SendUpdateIntentLocked(p, &idsArray);

    // schedule the future updates
    RegisterForBroadcastsLocked(p, GetAppWidgetIds(p));
    SaveStateLocked();

    return NOERROR;
}

ECode CAppWidgetService::GetAppWidgetInfo(
    /* [in] */ Int32 appWidgetId,
    /* [out] */ IAppWidgetProviderInfo** providerInfo)
{
    VALIDATE_NOT_NULL(providerInfo);
    Mutex::Autolock lock(mAppWidgetIdsLock);
    AppWidgetId* id = LookupAppWidgetIdLocked(appWidgetId);
    if (id != NULL && id->mProvider != NULL && !id->mProvider->mZombie) {
        *providerInfo = id->mProvider->mInfo;
        return NOERROR;
    }
    *providerInfo = NULL;
    return NOERROR;
}

ECode CAppWidgetService::GetAppWidgetViews(
    /* [in] */ Int32 appWidgetId,
    /* [out] */ IRemoteViews** views)
{
    VALIDATE_NOT_NULL(views);
    Mutex::Autolock lock(mAppWidgetIdsLock);
    AppWidgetId* id = LookupAppWidgetIdLocked(appWidgetId);
    if (id != NULL) {
        *views = id->mViews;
        return NOERROR;
    }
    *views = NULL;
    return NOERROR;
}

ECode CAppWidgetService::GetInstalledProviders(
    /* [out] */ IObjectContainer** providers)
{
    VALIDATE_NOT_NULL(providers);
    Mutex::Autolock lock(mAppWidgetIdsLock);
    AutoPtr<IObjectContainer> result;
    CObjectContainer::New((IObjectContainer**)&result);
    List<AutoPtr<Provider> >::Iterator it;
    for (it = mInstalledProviders.Begin(); it != mInstalledProviders.End(); ++it) {
        AutoPtr<Provider> p = *it;
        if (!p->mZombie) {
            result->Add(p->mInfo);
        }
    }
    *providers = result;
    return NOERROR;
}

ECode CAppWidgetService::UpdateAppWidgetIds(
    /* [in] */ const ArrayOf<Int32>& appWidgetIds,
    /* [in] */ IRemoteViews* views)
{
    // if (appWidgetIds == null) {
    //     return;
    // }
    if (appWidgetIds.GetLength() == 0) {
        return NOERROR;
    }
    Int32 N = appWidgetIds.GetLength();

    Mutex::Autolock lock(mAppWidgetIdsLock);
    for (Int32 i = 0; i < N; i++) {
        AppWidgetId* id = LookupAppWidgetIdLocked(appWidgetIds[i]);
        UpdateAppWidgetInstanceLocked(id, views);
    }

    return NOERROR;
}

ECode CAppWidgetService::UpdateAppWidgetProvider(
    /* [in] */ IComponentName* provider,
    /* [in] */ IRemoteViews* views)
{
    Mutex::Autolock lock(mAppWidgetIdsLock);
    Provider* p = LookupProviderLocked(provider);
    if (p == NULL) {
        //Slog.w(TAG, "updateAppWidgetProvider: provider doesn't exist: " + provider);
        return NOERROR;
    }
    List<AutoPtr<AppWidgetId> > instances = p->mInstances;
    List<AutoPtr<AppWidgetId> >::Iterator it;
    for (it = instances.Begin(); it != instances.End(); ++it) {
        AppWidgetId* id = *it;
        UpdateAppWidgetInstanceLocked(id, views);
    }

    return NOERROR;
}

void CAppWidgetService::UpdateAppWidgetInstanceLocked(
    /* [in] */ AppWidgetId* id,
    /* [in] */ IRemoteViews* views)
{
    // allow for stale appWidgetIds and other badness
    // lookup also checks that the calling process can access the appWidgetId
    // drop unbound appWidgetIds (shouldn't be possible under normal circumstances)
    if (id != NULL && id->mProvider != NULL && !id->mProvider->mZombie && !id->mHost->mZombie) {
        id->mViews = views;

        // is anyone listening?
        if (id->mHost->mCallbacks != NULL) {
            //try {
                // the lock is held, but this is a oneway call
            ECode ec = id->mHost->mCallbacks->UpdateAppWidget(id->mAppWidgetId, views);
            if (FAILED(ec)) {
                id->mHost->mCallbacks = NULL;
            }
            // } catch (RemoteException e) {
            //     // It failed; remove the callback. No need to prune because
            //     // we know that this host is still referenced by this instance.
            //     id.host.callbacks = null;
            // }
        }
    }
}

ECode CAppWidgetService::StartListening(
    /* [in] */ IAppWidgetHost* callbacks,
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 hostId,
    /* [out] */ IObjectContainer** updatedViews,
    /* [out, callee]*/ ArrayOf<Int32>** updatedIds)
{
    VALIDATE_NOT_NULL(updatedViews);
    VALIDATE_NOT_NULL(updatedIds);

    Int32 callingUid;
    EnforceCallingUid(capsuleName, &callingUid);
    Mutex::Autolock lock(mAppWidgetIdsLock);
    Host* host = LookupOrAddHostLocked(callingUid, capsuleName, hostId);
    host->mCallbacks = callbacks;

    AutoPtr<IObjectContainer> result;
    CObjectContainer::New((IObjectContainer**)&result);

    List<AutoPtr<AppWidgetId> > instances = host->mInstances;
    List<AutoPtr<AppWidgetId> >::Iterator it;
    ArrayOf<Int32>* resultArray = ArrayOf<Int32>::Alloc(instances.GetSize());
    Int32 i = 0;
    for (it = instances.Begin(); it != instances.End(); ++it, ++i) {
        AutoPtr<AppWidgetId> id = *it;
        (*resultArray)[i] = id->mAppWidgetId;
        result->Add(id->mViews);
    }
    *updatedViews = result;
    *updatedIds = resultArray;
    return NOERROR;
}

ECode CAppWidgetService::StopListening(
    /* [in] */ Int32 hostId)
{
    Mutex::Autolock lock(mAppWidgetIdsLock);
    Host* host = LookupHostLocked(0/*getCallingUid()*/, hostId);
    if (host != NULL) {
        host->mCallbacks = NULL;
        PruneHostLocked(host);
    }
    return NOERROR;
}

Boolean CAppWidgetService::CanAccessAppWidgetId(
    /* [in] */ AppWidgetId* id,
    /* [in] */ Int32 callingUid)
{
    if (id->mHost->mUid == callingUid) {
        // Apps hosting the AppWidget have access to it.
        return TRUE;
    }
    if (id->mProvider != NULL && id->mProvider->mUid == callingUid) {
        // Apps providing the AppWidget have access to it (if the appWidgetId has been bound)
        return TRUE;
    }

    Int32 perm;
    mContext->CheckCallingOrSelfPermission(String("android.Manifest.permission.BIND_APPWIDGET"), &perm);
    if (perm == CapsuleManager_PERMISSION_GRANTED) {
        // Apps that can bind have access to all appWidgetIds.
        return TRUE;
    }
    // Nobody else can access it.

    return FALSE;
}

CAppWidgetService::AppWidgetId* CAppWidgetService::LookupAppWidgetIdLocked(
    /* [in] */ Int32 appWidgetId)
{
    Int32 callingUid = 0;//getCallingUid();
    List<AutoPtr<AppWidgetId> >::Iterator it;
    for (it = mAppWidgetIds.Begin(); it != mAppWidgetIds.End(); ++it) {
        AutoPtr<AppWidgetId> id = *it;
        if (id->mAppWidgetId == appWidgetId && CanAccessAppWidgetId(id, callingUid)) {
            return id;
        }
    }
    return NULL;
}

CAppWidgetService::Provider* CAppWidgetService::LookupProviderLocked(
    /* [in] */ IComponentName* provider)
{
    String className;
    provider->GetClassName(&className);
    List<AutoPtr<Provider> >::Iterator it;
    for (it = mInstalledProviders.Begin(); it != mInstalledProviders.End(); ++it) {
        AutoPtr<Provider> p = *it;
        AutoPtr<IComponentName> result;
        p->mInfo->GetProvider((IComponentName**)&result);
        String oldName;
        p->mInfo->GetPreviousName(&oldName);
        if (result.Get() == provider || className.Equals(oldName)) {
            return p;
        }
    }
    return NULL;
}

CAppWidgetService::Host* CAppWidgetService::LookupHostLocked(
    /* [in] */ Int32 uid,
    /* [in] */ Int32 hostId)
{
    List<AutoPtr<Host> >::Iterator it;
    for (it = mHosts.Begin(); it != mHosts.End(); ++it) {
        AutoPtr<Host> h = *it;
        if (h->mUid == uid && h->mHostId == hostId) {
            return h;
        }
    }
    return NULL;
}

CAppWidgetService::Host* CAppWidgetService::LookupOrAddHostLocked(
    /* [in] */ Int32 uid,
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 hostId)
{
    List<AutoPtr<Host> >::Iterator it;
    for (it = mHosts.Begin(); it != mHosts.End(); ++it) {
        AutoPtr<Host> h = *it;
        if (h->mHostId == hostId && h->mCapsuleName.Equals(capsuleName)) {
            return h;
        }
    }
    Host* host = new Host();
    host->mCapsuleName = capsuleName;
    host->mUid = uid;
    host->mHostId = hostId;
    mHosts.PushBack(host);
    return host;
}

void CAppWidgetService::PruneHostLocked(
    /* [in] */ Host* host)
{
    if (host->mInstances.Begin() == host->mInstances.End() && host->mCallbacks == NULL) {
        mHosts.Remove(host);
    }
}

void CAppWidgetService::LoadAppWidgetList()
{
    AutoPtr<ILocalCapsuleManager> cm = mCapsuleManager;

    AutoPtr<IIntent> intent;
    CIntent::New(String(AppWidgetManager_ACTION_APPWIDGET_UPDATE), (IIntent**)&intent);
    AutoPtr<IObjectContainer> broadcastReceivers;
    mCapsuleManager->QueryBroadcastReceivers(intent,
            CapsuleManager_GET_META_DATA, (IObjectContainer**)&broadcastReceivers);
    if (broadcastReceivers != NULL) {
        AutoPtr<IObjectEnumerator> enumerator;
        broadcastReceivers->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
        Boolean hasNext;
        while(enumerator->MoveNext(&hasNext), hasNext) {
            AutoPtr<IResolveInfo> ri;
            enumerator->Current((IInterface**)&ri);
            AddProviderLocked(ri);
        }
    }
}

Boolean CAppWidgetService::AddProviderLocked(
    /* [in] */ IResolveInfo* ri)
{
    AutoPtr<IActivityInfo> ai;
    ri->GetActivityInfo((IActivityInfo**)&ai);
    String capsuleName, name;
    ai->GetCapsuleName(&capsuleName);
    ai->GetName(&name);
    AutoPtr<IComponentName> cn;
    CComponentName::New(capsuleName, name, (IComponentName**)&cn);
    Provider* p = ParseProviderInfoXml(cn, ri);
    if (p != NULL) {
        mInstalledProviders.PushBack(p);
        return TRUE;
    }
    else {
        return FALSE;
    }
}

void CAppWidgetService::RemoveProviderLocked(
    /* [in] */ Int32 index,
    /* [in] */ Provider* p)
{
    List<AutoPtr<AppWidgetId> >::Iterator it;
    for (it = p->mInstances.Begin(); it != p->mInstances.End(); ++it) {
        AutoPtr<AppWidgetId> id = *it;
        // Call back with empty RemoteViews
        UpdateAppWidgetInstanceLocked(id, NULL);
        // Stop telling the host about updates for this from now on
        CancelBroadcasts(p);
        // clear out references to this appWidgetId
        id->mHost->mInstances.Remove(id);
        mAppWidgetIds.Remove(id);
        id->mProvider = NULL;
        PruneHostLocked(id->mHost);
        id->mHost = NULL;
    }
    p->mInstances.Clear();
    mInstalledProviders.Remove(index);
    // no need to send the DISABLE broadcast, since the receiver is gone anyway
    CancelBroadcasts(p);
}

void CAppWidgetService::SendEnableIntentLocked(
    /* [in] */ Provider* p)
{
    AutoPtr<IIntent> intent;
    CIntent::New(String(AppWidgetManager_ACTION_APPWIDGET_ENABLED), (IIntent**)&intent);
    AutoPtr<IComponentName> provider;
    p->mInfo->GetProvider((IComponentName**)&provider);
    intent->SetComponent(provider);
    mContext->SendBroadcast(intent);
}

void CAppWidgetService::SendUpdateIntentLocked(
    /* [in] */ Provider* p,
    /* [in] */ ArrayOf<Int32>* appWidgetIds)
{
    if (appWidgetIds != NULL && appWidgetIds->GetLength() > 0) {
        AutoPtr<IIntent> intent;
        CIntent::New(String(AppWidgetManager_ACTION_APPWIDGET_UPDATE), (IIntent**)&intent);
        //intent->PutExtra(AppWidgetManager.EXTRA_APPWIDGET_IDS, appWidgetIds);
        AutoPtr<IComponentName> provider;
        p->mInfo->GetProvider((IComponentName**)&provider);
        intent->SetComponent(provider);
        mContext->SendBroadcast(intent);
    }
}

void CAppWidgetService::RegisterForBroadcastsLocked(
    /* [in] */ Provider* p,
    /* [in] */ ArrayOf<Int32>* appWidgetIds)
{
    Int32 updatePeriodMillis;
    p->mInfo->GetUpdatePeriodMillis(&updatePeriodMillis);
    if (updatePeriodMillis > 0) {
        // if this is the first instance, set the alarm.  otherwise,
        // rely on the fact that we've already set it and that
        // PendingIntent.getBroadcast will update the extras.
        Boolean alreadyRegistered = p->mBroadcast != NULL;
        AutoPtr<IIntent> intent;
        CIntent::New(String(AppWidgetManager_ACTION_APPWIDGET_UPDATE), (IIntent**)&intent);
        //intent.putExtra(AppWidgetManager.EXTRA_APPWIDGET_IDS, appWidgetIds);
        AutoPtr<IComponentName> provider;
        p->mInfo->GetProvider((IComponentName**)&provider);
        intent->SetComponent(provider);
        Int64 token = Binder::ClearCallingIdentity();
        //try {
        AutoPtr<IPendingIntentHelper> helper;
        CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&helper);
        helper->GetBroadcast(mContext, 1, intent,
                    PendingIntent_FLAG_UPDATE_CURRENT, (IPendingIntent**)&(p->mBroadcast));
        //} finally {
        Binder::RestoreCallingIdentity(token);
        //}
        if (!alreadyRegistered) {
            if (updatePeriodMillis < MIN_UPDATE_PERIOD) {
                updatePeriodMillis = MIN_UPDATE_PERIOD;
            }
            // mAlarmManager.setInexactRepeating(AlarmManager.ELAPSED_REALTIME_WAKEUP,
            //         SystemClock.elapsedRealtime() + period, period, p.broadcast);
        }
    }
}

ArrayOf<Int32>* CAppWidgetService::GetAppWidgetIds(
    /* [in] */ Provider* p)
{   
    ArrayOf<Int32>* appWidgetIds = ArrayOf<Int32>::Alloc(p->mInstances.GetSize());
    List<AutoPtr<AppWidgetId> >::Iterator it;
    Int32 i = 0;
    for (it = p->mInstances.Begin(); it != p->mInstances.End(); ++it, ++i) {
        (*appWidgetIds)[i] = (*it)->mAppWidgetId;
    }
    return appWidgetIds;
}

ECode CAppWidgetService::GetAppWidgetIds(
    /* [in] */ IComponentName* provider,
    /* [out, callee] */ ArrayOf<Int32>** appWidgetIds)
{
    VALIDATE_NOT_NULL(appWidgetIds);
    Mutex::Autolock lock(mAppWidgetIdsLock);
    Provider* p = LookupProviderLocked(provider);
    if (p != NULL && 0/*getCallingUid()*/ == p->mUid) {
        *appWidgetIds = GetAppWidgetIds(p); 
        return NOERROR;               
    }
    else {
        *appWidgetIds = ArrayOf<Int32>::Alloc(0);
        return NOERROR;
    }
}

CAppWidgetService::Provider* CAppWidgetService::ParseProviderInfoXml(
    /* [in] */ IComponentName* component,
    /* [in] */ IResolveInfo* ri)
{
    Provider* p = NULL;

    AutoPtr<IActivityInfo> activityInfo;
    ri->GetActivityInfo((IActivityInfo**)&activityInfo);
    AutoPtr<IXmlResourceParser> parser;
    //try {
    activityInfo->LoadXmlMetaData(mCapsuleManager,
            String(AppWidgetManager_META_DATA_APPWIDGET_PROVIDER), (IXmlResourceParser**)&parser);
    if (parser == NULL) {
        // Slog.w(TAG, "No " + AppWidgetManager.META_DATA_APPWIDGET_PROVIDER + " meta-data for "
        //         + "AppWidget provider '" + component + '\'');
        return NULL;
    }

    AutoPtr<IAttributeSet> attrs = Xml::AsAttributeSet(parser);
    
    Int32 type;
    while (parser->Next(&type) && type != IXmlPullParser_END_DOCUMENT
            && type != IXmlPullParser_START_TAG) {
        // drain whitespace, comments, etc.
    }
    
    String nodeName;
    parser->GetName(&nodeName);
    if (!String("appwidget-provider").Equals(nodeName)) {
        // Slog.w(TAG, "Meta-data does not start with appwidget-provider tag for"
        //         + " AppWidget provider '" + component + '\'');
        return NULL;
    }

    p = new Provider();
    ASSERT_SUCCEEDED(CAppWidgetProviderInfo::New((IAppWidgetProviderInfo**)&(p->mInfo)));
    // If metaData was null, we would have returned earlier when getting
    // the parser No need to do the check here
    AutoPtr<IBundle> metaData;
    activityInfo->GetMetaData((IBundle**)&metaData);
    String oldName;
    metaData->GetString(String(AppWidgetManager_META_DATA_APPWIDGET_OLD_NAME), &oldName);
    p->mInfo->SetPreviousName(oldName);

    p->mInfo->SetProvider(component);
    AutoPtr<IApplicationInfo> appInfo;
    activityInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    appInfo->GetUid(&(p->mUid));

    AutoPtr<IResources> res;
    mCapsuleManager->GetResourcesForApplication(appInfo, (IResources**)&res);
    
    AutoPtr<ITypedArray> sa;
    Int32 awpInfo[] = {0x0101013f, 0x01010140, 0x01010250, 0x01010251, 0x0101025d};/*com.android.internal.R.styleable.AppWidgetProviderInfo*/
    ArrayOf<Int32> awpInfoArray(awpInfo, 5);
    res->ObtainAttributes(attrs, awpInfoArray, (ITypedArray**)&sa);
    
    // These dimensions has to be resolved in the application's context.
    // We simply send back the raw complex data, which will be
    // converted to dp in {@link AppWidgetManager#getAppWidgetInfo}.
    AutoPtr<ITypedValue> value;
    sa->PeekValue(0/*com.android.internal.R.styleable.AppWidgetProviderInfo_minWidth*/,
            (ITypedValue**)&value);
    if (value != NULL) {
        Int32 minW;
        value->GetData(&minW);
        p->mInfo->SetMinWidth(minW);
    }
    else {
        p->mInfo->SetMinWidth(0);
    }
    
    sa->PeekValue(1/*com.android.internal.R.styleable.AppWidgetProviderInfo_minHeight*/,
            (ITypedValue**)&value);
    if (value != NULL) {
        Int32 minH;
        value->GetData(&minH);
        p->mInfo->SetMinHeight(minH);
    }
    else {
        p->mInfo->SetMinHeight(0);
    }

    Int32 updatePeriodMillis;
    sa->GetInt32(2/*com.android.internal.R.styleable.AppWidgetProviderInfo_updatePeriodMillis*/,
            0, &updatePeriodMillis);
    p->mInfo->SetUpdatePeriodMillis(updatePeriodMillis);

    Int32 initialLayout;
    sa->GetResourceId(3/*com.android.internal.R.styleable.AppWidgetProviderInfo_initialLayout*/,
            0, &initialLayout);
    p->mInfo->SetInitialLayout(initialLayout);

    String className;
    sa->GetString(4/*com.android.internal.R.styleable.AppWidgetProviderInfo_configure*/, &className);
    if (!className.IsNull()) {
        String capsuleName;
        component->GetCapsuleName(&capsuleName);
        AutoPtr<IComponentName> configure;
        CComponentName::New(capsuleName, className, (IComponentName**)&configure);
        p->mInfo->SetConfigure(configure);
    }

    AutoPtr<ICharSequence> cs;
    activityInfo->LoadLabel(mCapsuleManager, (ICharSequence**)&cs);
    String label;
    cs->ToString(&label);
    p->mInfo->SetLabel(label);

    Int32 icon;
    ri->GetIcon(&icon);
    p->mInfo->SetIcon(icon);
    sa->Recycle();

    if (parser != NULL) parser->Close();
    // } catch (Exception e) {
    //     // Ok to catch Exception here, because anything going wrong because
    //     // of what a client process passes to us should not be fatal for the
    //     // system process.
    //     Slog.w(TAG, "XML parsing failed for AppWidget provider '" + component + '\'', e);
    //     return null;
    // } finally {
    //     if (parser != null) parser.close();
    // }
    return p;
}

ECode CAppWidgetService::GetUidForCapsule(
    /* [in] */ const String& capsuleName,
    /* [out] */ Int32* uid)
{
    VALIDATE_NOT_NULL(uid);
    AutoPtr<ICapsuleInfo> capInfo;
    mCapsuleManager->GetCapsuleInfo(capsuleName, 0, (ICapsuleInfo**)&capInfo);
    AutoPtr<IApplicationInfo> appInfo;
    capInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    if (capInfo == NULL || appInfo == NULL) {
        return E_NAME_NOT_FOUND_EXCEPTION;
//        throw new PackageManager.NameNotFoundException();
    }

    return appInfo->GetUid(uid);
}

ECode CAppWidgetService::EnforceCallingUid(
    /* [in] */ const String& capsuleName,
    /* [out] */ Int32* uid)
{
    VALIDATE_NOT_NULL(uid);
    Int32 callingUid = 0;//getCallingUid();
    Int32 capsuleUid;
    //try {
    ECode ec = GetUidForCapsule(capsuleName, &capsuleUid);
    if (FAILED(ec)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // } catch (PackageManager.NameNotFoundException ex) {
    //     throw new IllegalArgumentException("packageName and uid don't match packageName="
    //             + packageName);
    // }
    if (callingUid != capsuleUid && Process::SupportsProcesses()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalArgumentException("packageName and uid don't match packageName="
        //         + packageName);
    }
    *uid = callingUid;
    return NOERROR;
}

void CAppWidgetService::SendInitialBroadcasts()
{
    Mutex::Autolock lock(mAppWidgetIdsLock);
    List<AutoPtr<Provider> >::Iterator it = mInstalledProviders.Begin();
    for (; it != mInstalledProviders.End(); ++it) {
        AutoPtr<Provider> p = *it;
        if (p->mInstances.Begin() != p->mInstances.End()) {
            SendEnableIntentLocked(p);
            ArrayOf<Int32>* appWidgetIds = GetAppWidgetIds(p);
            SendUpdateIntentLocked(p, appWidgetIds);
            RegisterForBroadcastsLocked(p, appWidgetIds);
        }
    }
}

// only call from initialization -- it assumes that the data structures are all empty
void CAppWidgetService::LoadStateLocked()
{
    AutoPtr<IFile> temp = SavedStateTempFile();
    AutoPtr<IFile> real = SavedStateRealFile();

    // prefer the real file.  If it doesn't exist, use the temp one, and then copy it to the
    // real one.  if there is both a real file and a temp one, assume that the temp one isn't
    // fully written and delete it.
    Boolean isExists, tempIsExists;
    real->Exists(&isExists);
    temp->Exists(&tempIsExists);
    if (isExists) {
        ReadStateFromFileLocked(real);
        if (tempIsExists) {
            //noinspection ResultOfMethodCallIgnored
            Boolean succeeded;
            temp->Delete(&succeeded);
        }
    }
    else if (tempIsExists) {
        ReadStateFromFileLocked(temp);
        //noinspection ResultOfMethodCallIgnored
        Boolean succeeded;
        temp->RenameTo(real, &succeeded);
    }
}

void CAppWidgetService::SaveStateLocked()
{
    AutoPtr<IFile> temp = SavedStateTempFile();
    AutoPtr<IFile> real = SavedStateRealFile();

    Boolean isExists;
    real->Exists(&isExists);
    if (!isExists) {
        // If the real one doesn't exist, it's either because this is the first time
        // or because something went wrong while copying them.  In this case, we can't
        // trust anything that's in temp.  In order to have the loadState code not
        // use the temporary one until it's fully written, create an empty file
        // for real, which will we'll shortly delete.
        //try {
            //noinspection ResultOfMethodCallIgnored
        Boolean succeeded;
        real->CreateNewFile(&succeeded);
        // } catch (IOException e) {
        //     // Ignore
        // }
    }

    Boolean tempIsExists;
    temp->Exists(&tempIsExists);
    if (tempIsExists) {
        //noinspection ResultOfMethodCallIgnored
        Boolean succeeded;
        temp->Delete(&succeeded);
    }

    if (!WriteStateToFileLocked(temp)) {
        //Slog.w(TAG, "Failed to persist new settings");
        return;
    }

    //noinspection ResultOfMethodCallIgnored
    Boolean succeeded;
    real->Delete(&succeeded);
    //noinspection ResultOfMethodCallIgnored
    temp->RenameTo(real, &succeeded);
}

Boolean CAppWidgetService::WriteStateToFileLocked(
    /* [in] */ IFile* file)
{
    // AutoPtr<IFileOutputStream> stream = NULL;
    // int N;

    // //try {
    // CFileOutputStream::New(file, FALSE, (IFileOutputStream**)&stream);
    // XmlSerializer out = new FastXmlSerializer();
    // out.setOutput(stream, "utf-8");
    // out.startDocument(null, true);

    
    // out.startTag(null, "gs");

    // int providerIndex = 0;
    // N = mInstalledProviders.size();
    // for (int i=0; i<N; i++) {
    //     Provider p = mInstalledProviders.get(i);
    //     if (p.instances.size() > 0) {
    //         out.startTag(null, "p");
    //         out.attribute(null, "pkg", p.info.provider.getPackageName());
    //         out.attribute(null, "cl", p.info.provider.getClassName());
    //         out.endTag(null, "p");
    //         p.tag = providerIndex;
    //         providerIndex++;
    //     }
    // }

    // N = mHosts.size();
    // for (int i=0; i<N; i++) {
    //     Host host = mHosts.get(i);
    //     out.startTag(null, "h");
    //     out.attribute(null, "pkg", host.packageName);
    //     out.attribute(null, "id", Integer.toHexString(host.hostId));
    //     out.endTag(null, "h");
    //     host.tag = i;
    // }

    // N = mAppWidgetIds.size();
    // for (int i=0; i<N; i++) {
    //     AppWidgetId id = mAppWidgetIds.get(i);
    //     out.startTag(null, "g");
    //     out.attribute(null, "id", Integer.toHexString(id.appWidgetId));
    //     out.attribute(null, "h", Integer.toHexString(id.host.tag));
    //     if (id.provider != null) {
    //         out.attribute(null, "p", Integer.toHexString(id.provider.tag));
    //     }
    //     out.endTag(null, "g");
    // }

    // out.endTag(null, "gs");

    // out.endDocument();
    // stream.close();
    // return true;
    // } catch (IOException e) {
    //     try {
    //         if (stream != null) {
    //             stream.close();
    //         }
    //     } catch (IOException ex) {
    //         // Ignore
    //     }
    //     if (file.exists()) {
    //         //noinspection ResultOfMethodCallIgnored
    //         file.delete();
    //     }
    //     return false;
    // }

    return FALSE;
}

void CAppWidgetService::ReadStateFromFileLocked(
    /* [in] */ IFile* file)
{
    AutoPtr<IFileInputStream> stream;

    Boolean success = FALSE;

    //try {
    CFileInputStream::New(file, (IFileInputStream**)&stream);
    AutoPtr<IXmlPullParser> parser = Xml::NewPullParser();
    parser->SetInputEx((IInputStream*)stream->Probe(EIID_IInputStream), String(NULL));

    Int32 type;
    Int32 providerIndex = 0;
    HashMap<Int32, AutoPtr<Provider> > loadedProviders;
    do {
        parser->Next(&type);
        if (type == IXmlPullParser_START_TAG) {
            String tag;
            parser->GetName(&tag);
            if (String("p").Equals(tag)) {
                // TODO: do we need to check that this package has the same signature
                // as before?
                String cap, cl;
                parser->GetAttributeValueEx(NULL, String("cap"), &cap);
                parser->GetAttributeValueEx(NULL, String("cl"), &cl);

                AutoPtr<ILocalCapsuleManager> capsuleManager;
                mContext->GetCapsuleManager((ILocalCapsuleManager**)&capsuleManager);
                //try {
                AutoPtr<IComponentName> cn;
                CComponentName::New(cap, cl, (IComponentName**)&cn);
                AutoPtr<IActivityInfo> aInfo;
                ECode ec = capsuleManager->GetReceiverInfo(cn, 0, (IActivityInfo**)&aInfo);
                if (FAILED(ec)) {
                    String names[] = { cap };
                    ArrayOf<String> namesArray(names, 1);
                    ArrayOf<String>* caps;
                    capsuleManager->CurrentToCanonicalCapsuleNames(namesArray, &caps);
                    cap = (*caps)[0];
                }
                // } catch (PackageManager.NameNotFoundException e) {
                //     String[] pkgs = packageManager.currentToCanonicalPackageNames(
                //             new String[] { pkg });
                //     pkg = pkgs[0];
                // }

                Provider* p = LookupProviderLocked(cn);
                if (p == NULL && mSafeMode) {
                    // if we're in safe mode, make a temporary one
                    p = new Provider();
                    ASSERT_SUCCEEDED(CAppWidgetProviderInfo::New((IAppWidgetProviderInfo**)&(p->mInfo)));
                    p->mInfo->SetProvider(cn);
                    p->mZombie = TRUE;
                    mInstalledProviders.PushBack(p);
                }
                if (p != NULL) {
                    // if it wasn't uninstalled or something
                    loadedProviders[providerIndex] = p;
                }
                providerIndex++;
            }
            else if (String("h").Equals(tag)) {
                Host* host = new Host();

                // TODO: do we need to check that this package has the same signature
                // as before?
                parser->GetAttributeValueEx(NULL, String("cap"), &(host->mCapsuleName));
                //try {
                ECode ec = GetUidForCapsule(host->mCapsuleName, &(host->mUid));
                if (FAILED(ec)) {
                    host->mZombie = TRUE;
                }
                // } catch (PackageManager.NameNotFoundException ex) {
                //     host.zombie = true;
                // }
                if (!host->mZombie || mSafeMode) {
                    // In safe mode, we don't discard the hosts we don't recognize
                    // so that they're not pruned from our list.  Otherwise, we do.
                    String id;
                    parser->GetAttributeValueEx(NULL, String("id"), &id);
                    host->mHostId = id.ToInt32();
                    mHosts.PushBack(host);
                }
            }
            else if (String("g").Equals(tag)) {
                AppWidgetId* id = new AppWidgetId();
                String idString;
                parser->GetAttributeValueEx(NULL, String("id"), &idString);
                id->mAppWidgetId = idString.ToInt32();
                if (id->mAppWidgetId >= mNextAppWidgetId) {
                    mNextAppWidgetId = id->mAppWidgetId + 1;
                }

                String providerString;
                parser->GetAttributeValueEx(NULL, String("p"), &providerString);
                if (!providerString.IsNull()) {
                    // there's no provider if it hasn't been bound yet.
                    // maybe we don't have to save this, but it brings the system
                    // to the state it was in.
                    Int32 pIndex = providerString.ToInt32();
                    HashMap<Int32, AutoPtr<Provider> >::Iterator it = loadedProviders.Find(pIndex);
                    if (it != loadedProviders.End()) {
                        id->mProvider = it->mSecond;
                    }
                    // if (false) {
                    //     Slog.d(TAG, "bound appWidgetId=" + id.appWidgetId + " to provider "
                    //             + pIndex + " which is " + id.provider);
                    // }
                    if (id->mProvider == NULL) {
                        // This provider is gone.  We just let the host figure out
                        // that this happened when it fails to load it.
                        continue;
                    }
                }

                String hString;
                parser->GetAttributeValueEx(NULL, String("h"), &hString);
                Int32 hIndex = hString.ToInt32();
                id->mHost = mHosts[hIndex];
                if (id->mHost == NULL) {
                    // This host is gone.
                    continue;
                }

                if (id->mProvider != NULL) {
                    id->mProvider->mInstances.PushBack(id);
                }
                id->mHost->mInstances.PushBack(id);
                mAppWidgetIds.PushBack(id);
            }
        }
    }
    while (type != IXmlPullParser_END_DOCUMENT);
    success = TRUE;
    // } catch (NullPointerException e) {
    //     Slog.w(TAG, "failed parsing " + file, e);
    // } catch (NumberFormatException e) {
    //     Slog.w(TAG, "failed parsing " + file, e);
    // } catch (XmlPullParserException e) {
    //     Slog.w(TAG, "failed parsing " + file, e);
    // } catch (IOException e) {
    //     Slog.w(TAG, "failed parsing " + file, e);
    // } catch (IndexOutOfBoundsException e) {
    //     Slog.w(TAG, "failed parsing " + file, e);
    // }
    //try {
    if (stream != NULL) {
        stream->Close();
    }
    // } catch (IOException e) {
    //     // Ignore
    // }

    if (success) {
        // delete any hosts that didn't manage to get connected (should happen)
        // if it matters, they'll be reconnected.
        List<AutoPtr<Host> >::ReverseIterator rit = mHosts.RBegin();
        for (; rit != mHosts.REnd(); ++rit) {
            PruneHostLocked(*rit);
        }
    }
    else {
        // failed reading, clean up
        mAppWidgetIds.Clear();
        mHosts.Clear();
        List<AutoPtr<Provider> >::Iterator it = mInstalledProviders.Begin();
        for (; it != mInstalledProviders.End(); ++it) {
            (*it)->mInstances.Clear();
        }
    }
}

AutoPtr<IFile> CAppWidgetService::SavedStateTempFile()
{
    AutoPtr<IFile> f;
    CFile::New(String("/data/system/") + SETTINGS_TMP_FILENAME, (IFile**)&f);
    //return new File(mContext.getFilesDir(), SETTINGS_FILENAME);
    return f;
}

AutoPtr<IFile> CAppWidgetService::SavedStateRealFile()
{
    AutoPtr<IFile> f;
    CFile::New(String("/data/system/") + SETTINGS_FILENAME, (IFile**)&f);
    //return new File(mContext.getFilesDir(), SETTINGS_TMP_FILENAME);
    return f;
}

ECode CAppWidgetService::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    context->GetCapsuleManager((ILocalCapsuleManager**)&mCapsuleManager);
    //mAlarmManager = (AlarmManager)mContext.getSystemService(Context.ALARM_SERVICE);
    return NOERROR;
}
