
#include "appwidget/AppWidgetProvider.h"
#include "appwidget/CAppWidgetManager.h"

ECode AppWidgetProvider::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // Protect against rogue update broadcasts (not really a security issue,
    // just filter bad broacasts out so subclasses are less likely to crash).
    String action;
    intent->GetAction(&action);
    if (String(AppWidgetManager_ACTION_APPWIDGET_UPDATE).Equals(action)) {
        AutoPtr<IBundle> extras;
        intent->GetExtras((IBundle**)&extras);
        if (extras != NULL) {
            ArrayOf<Int32>* appWidgetIds;
            extras->GetInt32Array(
                    String(AppWidgetManager_EXTRA_APPWIDGET_IDS), &appWidgetIds);
            if (appWidgetIds != NULL && appWidgetIds->GetLength() > 0) {
                AutoPtr<IAppWidgetManager> manager;
                CAppWidgetManager::GetInstance(context, (IAppWidgetManager**)&manager);
                OnUpdate(context, manager, *appWidgetIds);
            }
        }
    }
    else if (String(AppWidgetManager_ACTION_APPWIDGET_DELETED).Equals(action)) {
        AutoPtr<IBundle> extras;
        intent->GetExtras((IBundle**)&extras);
        Boolean result;
        extras->ContainsKey(String(AppWidgetManager_EXTRA_APPWIDGET_ID), &result);
        if (extras != NULL && result) {
            Int32 appWidgetId;
            extras->GetInt32(String(AppWidgetManager_EXTRA_APPWIDGET_ID), &appWidgetId);
            Int32 ids[] = { appWidgetId };
            ArrayOf<Int32> idsArray(ids, 1);
            OnDeleted(context, idsArray);
        }
    }
    else if (String(AppWidgetManager_ACTION_APPWIDGET_ENABLED).Equals(action)) {
        OnEnabled(context);
    }
    else if (String(AppWidgetManager_ACTION_APPWIDGET_DISABLED).Equals(action)) {
        OnDisabled(context);
    }

    return NOERROR;
}

ECode AppWidgetProvider::OnUpdate(
    /* [in] */ IContext* context,
    /* [in] */ IAppWidgetManager* appWidgetManager,
    /* [in] */ const ArrayOf<Int32>& appWidgetIds)
{
    return NOERROR;
}

ECode AppWidgetProvider::OnDeleted(
    /* [in] */ IContext* context,
    /* [in] */ const ArrayOf<Int32>& appWidgetIds)
{
    return NOERROR;
}

ECode AppWidgetProvider::OnEnabled(
    /* [in] */ IContext* context)
{
    return NOERROR;
}

ECode AppWidgetProvider::OnDisabled(
    /* [in] */ IContext* context)
{
    return NOERROR;
}
