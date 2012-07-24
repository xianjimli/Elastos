
#include "server/CStatusBarManagerService.h"

ECode CStatusBarManagerService::constructor(
    /* [in] */ IContext* ctx)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

void CStatusBarManagerService::SetNotificationCallbacks(
    /* [in] */ CStatusBarManagerService::NotificationCallbacks* listener)
{
}

ECode CStatusBarManagerService::Expand()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStatusBarManagerService::Collapse()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStatusBarManagerService::Disable(
    /* [in] */ Int32 what,
    /* [in] */ IBinder* token,
    /* [in] */ const String& cap)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStatusBarManagerService::SetIcon(
    /* [in] */ const String& slot,
    /* [in] */ const String& iconCapsule,
    /* [in] */ Int32 iconId,
    /* [in] */ Int32 iconLevel)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStatusBarManagerService::SetIconVisibility(
    /* [in] */ const String& slot,
    /* [in] */ Boolean visible)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStatusBarManagerService::RemoveIcon(
    /* [in] */ const String& slot)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

// ---- Methods below are for use by the status bar policy services ----
// You need the STATUS_BAR_SERVICE permission
ECode CStatusBarManagerService::RegisterStatusBar(
    /* [in] */ IStatusBar* cbs,
    /* [out] */ IStatusBarIconList** iconList,
    /* [out] */ IObjectContainer** notificationKeys,
    /* [out] */ IObjectContainer** notifications)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStatusBarManagerService::OnPanelRevealed()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStatusBarManagerService::OnNotificationClick(
    /* [in] */ const String& cap,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStatusBarManagerService::OnNotificationError(
    /* [in] */ const String& cap,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 initialPid,
    /* [in] */ const String& message)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStatusBarManagerService::OnClearAllNotifications()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStatusBarManagerService::AddNotification(
    /* [in] */ IStatusBarNotification* notification,
    /* [out] */ IBinder** key)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStatusBarManagerService::UpdateNotification(
    /* [in] */ IBinder* key,
    /* [out] */ IStatusBarNotification* notification)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStatusBarManagerService::RemoveNotification(
    /* [in] */ IBinder* key)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}
