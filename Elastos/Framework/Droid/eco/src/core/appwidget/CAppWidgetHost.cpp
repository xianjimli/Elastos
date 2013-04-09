
#include "appwidget/CAppWidgetHost.h"
#include "appwidget/CAppWidgetHostCallbacks.h"
#include "appwidget/CAppWidgetHostView.h"
#include "os/CApartment.h"
#include "os/CServiceManager.h"

AutoPtr<IAppWidgetService> CAppWidgetHost::sService;
Mutex CAppWidgetHost::sServiceLock;

CAppWidgetHost::CAppWidgetHost()
    : mHostId(0)
{
    ASSERT_SUCCEEDED(CApartment::New(TRUE, (IApartment**)&mHandler));
    mHandler->Start(ApartmentAttr_New);
    ASSERT_SUCCEEDED(CAppWidgetHostCallbacks::New((ILocalAppWidgetHost*)this,
            (IAppWidgetHost**)&mCallbacks));
}

ECode CAppWidgetHost::StartListening()
{
    ArrayOf<Int32>* updatedIds;
    AutoPtr<IObjectContainer> updatedViews;

    //try {
    if (mCapsuleName.IsNull()) {
        mContext->GetCapsuleName(&mCapsuleName);
    }
    ECode ec = sService->StartListening(mCallbacks, mCapsuleName, mHostId,
            (IObjectContainer**)&updatedViews, &updatedIds);
    if (FAILED(ec)) {
        return ec;
    }
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }

    AutoPtr<IObjectEnumerator> enumerator;
    updatedViews->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext;
    for (Int32 i = 0; i < updatedIds->GetLength(); i++) {
        AutoPtr<IRemoteViews> views;
        enumerator->MoveNext(&hasNext);
        if (hasNext) {
            enumerator->Current((IInterface**)&views);
        }
        UpdateAppWidgetView((*updatedIds)[i], views);
    }

    return NOERROR;
}

/**
 * Stop receiving onAppWidgetChanged calls for your AppWidgets.  Call this when your activity is
 * no longer visible, i.e. from onStop() in your Activity.
 */
ECode CAppWidgetHost::StopListening()
{
    //try {
    return sService->StopListening(mHostId);
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

/**
 * Get a appWidgetId for a host in the calling process.
 *
 * @return a appWidgetId
 */
ECode CAppWidgetHost::AllocateAppWidgetId(
    /* [out] */ Int32* appWidgetId)
{
    VALIDATE_NOT_NULL(appWidgetId);
    //try {
    if (mCapsuleName.IsNull()) {
        mContext->GetCapsuleName(&mCapsuleName);
    }
    return sService->AllocateAppWidgetId(mCapsuleName, mHostId, appWidgetId);
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

/**
 * Stop listening to changes for this AppWidget.
 */
ECode CAppWidgetHost::DeleteAppWidgetId(
    /* [in] */ Int32 appWidgetId)
{
    Mutex::Autolock lock(mViewsLock);
    mViews.Erase(appWidgetId);
    //try {
    return sService->DeleteAppWidgetId(appWidgetId);
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

/**
 * Remove all records about this host from the AppWidget manager.
 * <ul>
 *   <li>Call this when initializing your database, as it might be because of a data wipe.</li>
 *   <li>Call this to have the AppWidget manager release all resources associated with your
 *   host.  Any future calls about this host will cause the records to be re-allocated.</li>
 * </ul>
 */
ECode CAppWidgetHost::DeleteHost()
{
    //try {
    return sService->DeleteHost(mHostId);
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

ECode CAppWidgetHost::CreateView(
    /* [in] */ IContext* context,
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IAppWidgetProviderInfo* appWidget,
    /* [out] */ IAppWidgetHostView** hostView)
{
    VALIDATE_NOT_NULL(hostView);

    OnCreateView(context, appWidgetId, appWidget, hostView);
    (*hostView)->SetAppWidget(appWidgetId, appWidget);

    {
        Mutex::Autolock lock(mViewsLock);
        mViews[appWidgetId] = *hostView;
    }

    AutoPtr<IRemoteViews> views;
    //try {
    ECode ec = sService->GetAppWidgetViews(appWidgetId, (IRemoteViews**)&views);
    if (FAILED(ec)) {
        return ec;
    }
    // } catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
    (*hostView)->UpdateAppWidget(views);

    return NOERROR;
}

ECode CAppWidgetHost::OnCreateView(
    /* [in] */ IContext* context,
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IAppWidgetProviderInfo* appWidget,
    /* [out] */ IAppWidgetHostView** hostView)
{
    return CAppWidgetHostView::New(context, hostView);
}

void CAppWidgetHost::OnProviderChanged(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IAppWidgetProviderInfo* appWidget)
{
    AutoPtr<IAppWidgetHostView> v;
    {
        Mutex::Autolock lock(mViewsLock);
        HashMap<Int32, AutoPtr<IAppWidgetHostView> >::Iterator it = mViews.Find(appWidgetId);
        if (it != mViews.End()) {
            v = it->mSecond;
        }
    }
    if (v != NULL) {
        v->ResetAppWidget(appWidget);
    }
}

ECode CAppWidgetHost::UpdateAppWidgetView(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IRemoteViews* views)
{
    AutoPtr<IAppWidgetHostView> v;
    {
        Mutex::Autolock lock(mViewsLock);
        HashMap<Int32, AutoPtr<IAppWidgetHostView> >::Iterator it = mViews.Find(appWidgetId);
        if (it != mViews.End()) {
            v= it->mSecond;
        }
    }
    if (v != NULL) {
        v->UpdateAppWidget(views);
    }
    return NOERROR;
}

ECode CAppWidgetHost::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 hostId)
{
    mContext = context;
    mHostId = hostId;
    Mutex::Autolock lock(sServiceLock);
    if (sService == NULL) {
        AutoPtr<IServiceManager> serviceManager;
        CServiceManager::AcquireSingleton((IServiceManager**)&serviceManager);
        serviceManager->GetService(String("AppWidgetService"),
               (IInterface**)(IAppWidgetService**)&sService);
    }

    return NOERROR;
}

void CAppWidgetHost::HandleUpdate(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IRemoteViews* views)
{
    UpdateAppWidgetView(appWidgetId, views);
}

void CAppWidgetHost::HandleProviderChanged(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IAppWidgetProviderInfo* info)
{
    OnProviderChanged(appWidgetId, info);
}
