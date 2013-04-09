
#include "appwidget/CAppWidgetManager.h"
#include "utils/CTypedValue.h"
#include "os/CServiceManager.h"

const CString CAppWidgetManager::TAG = "AppWidgetManager";
HashMap<AutoPtr<IContext>, AutoPtr<IAppWidgetManager> > CAppWidgetManager::sManagerCache;
Mutex CAppWidgetManager::sManagerCacheLock;
AutoPtr<IAppWidgetService> CAppWidgetManager::sService;

ECode CAppWidgetManager::GetInstance(
    /* [in] */ IContext* context,
    /* [out] */ IAppWidgetManager** appWidgetManager)
{
    VALIDATE_NOT_NULL(appWidgetManager);
    Mutex::Autolock lock(sManagerCacheLock);
    if (sService == NULL) {
        AutoPtr<IServiceManager> serviceManager;
        CServiceManager::AcquireSingleton((IServiceManager**)&serviceManager);
        serviceManager->GetService(String("AppWidgetService"),
                (IInterface**)(IAppWidgetService**)&sService);
    }

    AutoPtr<IAppWidgetManager> result;
    HashMap<AutoPtr<IContext>, AutoPtr<IAppWidgetManager> >::Iterator it
            = sManagerCache.Find(context);
    if (it != sManagerCache.End()) {
        result = it->mSecond;
    }

    if (result == NULL) {
        CAppWidgetManager::New(context, (IAppWidgetManager**)&result);
        sManagerCache[context] = result;
    }
    *appWidgetManager = result;
    return NOERROR;
}

ECode CAppWidgetManager::UpdateAppWidget(
    /* [in] */ const ArrayOf<Int32>& appWidgetIds,
    /* [in] */ IRemoteViews* views)
{
    //try {
    return sService->UpdateAppWidgetIds(appWidgetIds, views);
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

ECode CAppWidgetManager::UpdateAppWidgetEx(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IRemoteViews* views)
{
    Int32 appWidgetIds[] = { appWidgetId };
    ArrayOf<Int32> idsArray(appWidgetIds, 1);
    return UpdateAppWidget(idsArray, views);
}

ECode CAppWidgetManager::UpdateAppWidgetEx2(
    /* [in] */ IComponentName* provider,
    /* [in] */ IRemoteViews* views)
{
    //try {
    return sService->UpdateAppWidgetProvider(provider, views);
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

ECode CAppWidgetManager::GetInstalledProviders(
    /* [out] */ IObjectContainer** providerInfos)
{
    //try {
    return sService->GetInstalledProviders(providerInfos);
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

ECode CAppWidgetManager::GetAppWidgetInfo(
    /* [in] */ Int32 appWidgetId,
    /* [out] */ IAppWidgetProviderInfo** info)
{
    VALIDATE_NOT_NULL(info);
    //try {
    sService->GetAppWidgetInfo(appWidgetId, info);
    if (*info != NULL) {
        // Converting complex to dp.
        Int32 minW;
        (*info)->GetMinWidth(&minW);
        minW = CTypedValue::ComplexToDimensionPixelSize(minW, mDisplayMetrics);
        Int32 minH;
        (*info)->GetMinHeight(&minH);
        minH = CTypedValue::ComplexToDimensionPixelSize(minH, mDisplayMetrics);
    }
    return NOERROR;
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

ECode CAppWidgetManager::BindAppWidgetId(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IComponentName* provider)
{
    //try {
    return sService->BindAppWidgetId(appWidgetId, provider);
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

ECode CAppWidgetManager::GetAppWidgetIds(
    /* [in] */ IComponentName* provider,
    /* [out, callee] */ ArrayOf<Int32>** appWidgetIds)
{
    //try {
    return sService->GetAppWidgetIds(provider, appWidgetIds);
    // }
    // catch (RemoteException e) {
    //     throw new RuntimeException("system server dead?", e);
    // }
}

ECode CAppWidgetManager::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    res->GetDisplayMetrics((IDisplayMetrics**)&mDisplayMetrics);
    return NOERROR;
}

