
#include "ext/frameworkdef.h"
#include "appwidget/CAppWidgetManagerHelper.h"
#include "appwidget/CAppWidgetManager.h"
#include "os/CServiceManager.h"

ECode CAppWidgetManagerHelper::GetInstance(
    /* [in] */ IContext* context,
    /* [out] */ IAppWidgetManager** appWidgetManager)
{
    VALIDATE_NOT_NULL(appWidgetManager);
    Mutex::Autolock lock(CAppWidgetManager::sManagerCacheLock);
    if (CAppWidgetManager::sService == NULL) {
        AutoPtr<IServiceManager> serviceManager;
        CServiceManager::AcquireSingleton((IServiceManager**)&serviceManager);
        serviceManager->GetService(String("AppWidgetService"),
                (IInterface**)(IAppWidgetService**)&(CAppWidgetManager::sService));
    }

    AutoPtr<IAppWidgetManager> result;
    HashMap<AutoPtr<IContext>, AutoPtr<IAppWidgetManager> >::Iterator it
            = CAppWidgetManager::sManagerCache.Find(context);
    if (it != CAppWidgetManager::sManagerCache.End()) {
        result = it->mSecond;
    }

    if (result == NULL) {
        ASSERT_SUCCEEDED(CAppWidgetManager::New(context, (IAppWidgetManager**)&result));
        CAppWidgetManager::sManagerCache[context] = result;
    }
    *appWidgetManager = result;
    return NOERROR;
}
