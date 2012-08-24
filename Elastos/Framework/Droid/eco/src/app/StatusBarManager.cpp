
#include "app/StatusBarManager.h"

const Int32 StatusBarManager::DISABLE_EXPAND;
const Int32 StatusBarManager::DISABLE_NOTIFICATION_ICONS;
const Int32 StatusBarManager::DISABLE_NOTIFICATION_ALERTS;
const Int32 StatusBarManager::DISABLE_NOTIFICATION_TICKER;
const Int32 StatusBarManager::DISABLE_NONE;


StatusBarManager::StatusBarManager(
    /* [in] */ IContext* context)
{
}

ECode StatusBarManager::Expand()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode StatusBarManager::Collapse()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode StatusBarManager::Disable(
    /* [in] */ Int32 what,
    /* [in] */ IBinder* token,
    /* [in] */ const String& cap)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode StatusBarManager::SetIcon(
    /* [in] */ const String& slot,
    /* [in] */ const String& iconCapsule,
    /* [in] */ Int32 iconId,
    /* [in] */ Int32 iconLevel)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode StatusBarManager::SetIconVisibility(
    /* [in] */ const String& slot,
    /* [in] */ Boolean visible)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode StatusBarManager::RemoveIcon(
    /* [in] */ const String& slot)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

// ---- Methods below are for use by the status bar policy services ----
// You need the STATUS_BAR_SERVICE permission
ECode StatusBarManager::RegisterStatusBar(
    /* [in] */ IStatusBar* cbs,
    /* [out] */ IStatusBarIconList** iconList,
    /* [out] */ IObjectContainer** notificationKeys,
    /* [out] */ IObjectContainer** notifications)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode StatusBarManager::OnPanelRevealed()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode StatusBarManager::OnNotificationClick(
    /* [in] */ const String& cap,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode StatusBarManager::OnNotificationError(
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

ECode StatusBarManager::OnClearAllNotifications()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}
