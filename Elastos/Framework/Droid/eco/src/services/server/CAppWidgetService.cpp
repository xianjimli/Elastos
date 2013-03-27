
#include "server/CAppWidgetService.h"

const CString CAppWidgetService::TAG = "AppWidgetService";
const CString CAppWidgetService::SETTINGS_FILENAME = "appwidgets.xml";
const String CAppWidgetService::SETTINGS_TMP_FILENAME
        = String(CAppWidgetService::SETTINGS_FILENAME) + String(".tmp");
const Int32 CAppWidgetService::MIN_UPDATE_PERIOD;

CAppWidgetService::CAppWidgetService()
    : mNextAppWidgetId(AppWidgetManager_INVALID_APPWIDGET_ID + 1)
    , mSafeMode(FALSE)
{}

void CAppWidgetService::SystemReady(
    /* [in] */ Boolean safeMode)
{
}

ECode CAppWidgetService::StartListening(
    /* [in] */ IAppWidgetHost* host,
    /* [in] */ const String& packageName,
    /* [in] */ Int32 hostId,
    /* [out] */ IObjectContainer** updatedViews,
    /* [out, callee]*/ ArrayOf<Int32>** updatedIds)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAppWidgetService::StopListening(
    /* [in] */ Int32 hostId)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAppWidgetService::AllocateAppWidgetId(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 hostId,
    /* [out] */ Int32* appWidgetId)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAppWidgetService::DeleteAppWidgetId(
    /* [in] */ Int32 appWidgetId)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAppWidgetService::DeleteHost(
    /* [in] */ Int32 hostId)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAppWidgetService::DeleteAllHosts()
{
    return E_NOT_IMPLEMENTED;
}

ECode CAppWidgetService::GetAppWidgetViews(
    /* [in] */ Int32 appWidgetId,
    /* [out] */ IRemoteViews** views)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAppWidgetService::UpdateAppWidgetIds(
    /* [in] */ const ArrayOf<Int32>& appWidgetIds,
    /* [in] */ IRemoteViews* views)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAppWidgetService::UpdateAppWidgetProvider(
    /* [in] */ IComponentName* provider,
    /* [in] */ IRemoteViews* views)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAppWidgetService::GetInstalledProviders(
    /* [out] */ IObjectContainer** providers)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAppWidgetService::GetAppWidgetInfo(
    /* [in] */ Int32 appWidgetId,
    /* [out] */ IAppWidgetProviderInfo** providerInfo)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAppWidgetService::BindAppWidgetId(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IComponentName* provider)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAppWidgetService::GetAppWidgetIds(
    /* [in] */ IComponentName* provider,
    /* [out, callee] */ ArrayOf<Int32>** appWidgetIds)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAppWidgetService::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    context->GetCapsuleManager((ILocalCapsuleManager**)&mCapsuleManager);
    //mAlarmManager = (AlarmManager)mContext.getSystemService(Context.ALARM_SERVICE);
    return NOERROR;
}
