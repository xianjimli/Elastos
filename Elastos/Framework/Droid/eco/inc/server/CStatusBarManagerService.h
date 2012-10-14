
#ifndef __CSTATUSBARSERVICE_H__
#define __CSTATUSBARSERVICE_H__

#include "_CStatusBarManagerService.h"
#include "ext/frameworkext.h"
#include "os/Runnable.h"
#include <elastos/HashMap.h>
#include <elastos/List.h>
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

CarClass(CStatusBarManagerService)
{
public:
    interface INotificationCallbacks : public IInterface
    {
        virtual CARAPI OnSetDisabled(
            /* [in] */ Int32 status) = 0;

        virtual CARAPI OnClearAll() = 0;

        virtual CARAPI OnNotificationClick(
            /* [in] */ const String& cap,
            /* [in] */ const String& tag,
            /* [in] */ Int32 id) = 0;

        virtual CARAPI OnPanelRevealed() = 0;

        virtual CARAPI OnNotificationError(
            /* [in] */ const String& cap,
            /* [in] */ const String& tag,
            /* [in] */ Int32 id,
            /* [in] */ Int32 uid,
            /* [in] */ Int32 initialPid,
            /* [in] */ CString message) = 0;
    };

private:
    class DisableRecord
        : public ElRefBase
        , public IBinderDeathRecipient
    {
    public:
        CARAPI BinderDied();

    public:
        String mCap;
        Int32 mWhat;
        AutoPtr<IBinder> mToken;
        CStatusBarManagerService* mOwner;
    };

    class DisableRunnable : public Runnable
    {
    public:
        DisableRunnable(
            /* [in] */ Int32 net,
            /* [in] */ CStatusBarManagerService* owner)
        : mNet(net)
        , mOwner(owner)
        {}

        CARAPI Run()
        {
            return mOwner->mNotificationCallbacks->OnSetDisabled(mNet);
        }

    private:
        Int32 mNet;
        CStatusBarManagerService* mOwner;
    };

public:
    CStatusBarManagerService();

    ~CStatusBarManagerService();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI_(void) SetNotificationCallbacks(
        /* [in] */ INotificationCallbacks* listener);

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

    // ================================================================================
    // Callbacks from the status bar service.
    // ================================================================================
    CARAPI RegisterStatusBar(
        /* [in] */ IStatusBar* bar,
        /* [out] */ IStatusBarIconList** iconList,
        /* [out] */ IObjectContainer** notificationKeys,
        /* [out] */ IObjectContainer** notifications);

    /**
     * The status bar service should call this each time the user brings the panel from
     * invisible to visible in order to clear the notification light.
     */
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
        /* [in] */ CString message);

    CARAPI OnClearAllNotifications();

    // ================================================================================
    // Callbacks for NotificationManagerService.
    // ================================================================================
    CARAPI AddNotification(
        /* [in] */ IStatusBarNotification* notification,
        /* [out] */ IBinder** key);

    CARAPI UpdateNotification(
        /* [in] */ IBinder* key,
        /* [out] */ IStatusBarNotification* notification);

    CARAPI RemoveNotification(
        /* [in] */ IBinder* key);

    // ================================================================================
    // Can be called from any thread
    // ================================================================================

    // lock on mDisableRecords
    CARAPI_(void) ManageDisableListLocked(
        /* [in] */ Int32 what,
        /* [in] */ IBinder* token,
        /* [in] */ const String& cap);

    // lock on mDisableRecords
    CARAPI_(Int32) GatherDisableActionsLocked();

private:
    CARAPI_(void) EnforceStatusBar();

    CARAPI_(void) EnforceExpandStatusBar();

    CARAPI_(void) EnforceStatusBarService();

    CARAPI Post(
        /* [in] */ IRunnable* action);

public:
    static const CString TAG;
    static const Boolean SPEW = FALSE;

    AutoPtr<IContext> mContext;
//    Handler mHandler = new Handler();
    AutoPtr<IApartment> mHandler;
    AutoPtr<INotificationCallbacks> mNotificationCallbacks;
    AutoPtr<IStatusBar> mBar;
    AutoPtr<IStatusBarIconList> mIcons;
    Mutex mIconsLock;

    struct HashBinder
    {
        size_t operator()(const AutoPtr<IBinder> s) const
        {
            return (size_t)s.Get();
        }
    };

    struct BinderEq
    {
        Boolean operator()(const AutoPtr<IBinder> x,
                           const AutoPtr<IBinder> y) const
        {
            return x.Get() == y.Get();
        }
    };
    HashMap< AutoPtr<IBinder>, AutoPtr<IStatusBarNotification>,
            HashBinder, BinderEq > mNotifications;
    Mutex mNotificationsLock;

    // for disabling the status bar
    List<DisableRecord*> mDisableRecords;
    Mutex mDisableRecordsLock;
    Int32 mDisabled;
};

#endif // __CSTATUSBARSERVICE_H__

