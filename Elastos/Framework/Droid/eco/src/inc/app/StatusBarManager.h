
#ifndef __STATUSBARMANAGER_H__
#define __STATUSBARMANAGER_H__

#include "ext/frameworkext.h"

using namespace Elastos;

class StatusBarManager : public IStatusBarService
{
public:
    /**
     * Flag for {@link #disable} to make the status bar not expandable.  Unless you also
     * set {@link #DISABLE_NOTIFICATION_ICONS}, new notifications will continue to show.
     */
    static const Int32 DISABLE_EXPAND = 0x00000001;

    /**
     * Flag for {@link #disable} to hide notification icons and scrolling ticker text.
     */
    static const Int32 DISABLE_NOTIFICATION_ICONS = 0x00000002;

    /**
     * Flag for {@link #disable} to disable incoming notification alerts.  This will not block
     * icons, but it will block sound, vibrating and other visual or aural notifications.
     */
    static const Int32 DISABLE_NOTIFICATION_ALERTS = 0x00000004;

    /**
     * Flag for {@link #disable} to hide only the scrolling ticker.  Note that
     * {@link #DISABLE_NOTIFICATION_ICONS} implies {@link #DISABLE_NOTIFICATION_TICKER}.
     */
    static const Int32 DISABLE_NOTIFICATION_TICKER = 0x00000008;

    /**
     * Re-enable all of the status bar features that you've disabled.
     */
    static const Int32 DISABLE_NONE = 0x00000000;

public:
    StatusBarManager(
        /* [in] */ IContext* context);

    CARAPI Disable(
        /* [in] */ Int32 what,
        /* [in] */ IBinder* token,
        /* [in] */ const String& cap);

    CARAPI Expand();

    CARAPI Collapse();

    CARAPI SetIcon(
        /* [in] */ const String& slot,
        /* [in] */ const String& iconCapsule,
        /* [in] */ Int32 iconId,
        /* [in] */ Int32 iconLevel);

    CARAPI RemoveIcon(
        /* [in] */ const String& slot);

    CARAPI SetIconVisibility(
        /* [in] */ const String& slot,
        /* [in] */ Boolean visible);



    // ---- Methods below are for use by the status bar policy services ----
    // You need the STATUS_BAR_SERVICE permission
    CARAPI RegisterStatusBar(
        /* [in] */ IStatusBar* cbs,
        /* [out] */ IStatusBarIconList** iconList,
        /* [out] */ IObjectContainer** notificationKeys,
        /* [out] */ IObjectContainer** notifications);

    CARAPI OnPanelRevealed();

    CARAPI OnNotificationClick(
        /* [in] */ const String& cap,
        /* [in] */ const String& tag,
        /* [in] */ Int32 id);

    CARAPI OnNotificationError(
        /* [in] */ const String& cap,
        /* [in] */ const String& tag,
        /* [in] */ Int32 id,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 initialPid,
        /* [in] */ const String& message);

    CARAPI OnClearAllNotifications();

};

#endif //__STATUSBARMANAGER_H__

