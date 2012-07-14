
#ifndef __CSTATUSBARSERVICE_H__
#define __CSTATUSBARSERVICE_H__

#include "_CStatusBarManagerService.h"

using namespace Elastos;

CarClass(CStatusBarManagerService)
{
public:
    interface NotificationCallbacks
    {
        virtual CARAPI_(void) OnSetDisabled(
            /* [in] */ Int32 status) = 0;

        virtual CARAPI_(void) OnClearAll() = 0;

        virtual CARAPI_(void) OnNotificationClick(
            /* [in] */ String pkg,
            /* [in] */ String tag,
            /* [in] */ Int32 id) = 0;

        virtual CARAPI_(void) OnPanelRevealed() = 0;

        virtual CARAPI_(void) OnNotificationError(
            /* [in] */ String pkg,
            /* [in] */ String tag,
            /* [in] */ Int32 id,
            /* [in] */ Int32 uid,
            /* [in] */ Int32 initialPid,
            /* [in] */ String message) = 0;
    };

public:
    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI_(void) SetNotificationCallbacks(
        /* [in] */ NotificationCallbacks* listener);

    CARAPI Expand();

    CARAPI Collapse();

    CARAPI Disable(
        /* [in] */ Int32 what,
        /* [in] */ IBinder* token,
        /* [in] */ const String& cap);

    CARAPI SetIcon(
        /* [in] */ const String& slot,
        /* [in] */ const String& iconCapsule,
        /* [in] */ Int32 iconId,
        /* [in] */ Int32 iconLevel);

    CARAPI SetIconVisibility(
        /* [in] */ const String& slot,
        /* [in] */ Boolean visible);

    CARAPI RemoveIcon(
        /* [in] */ const String& slot);

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

private:
    // TODO: Add your private member variables here.
};

#endif // __CSTATUSBARSERVICE_H__

