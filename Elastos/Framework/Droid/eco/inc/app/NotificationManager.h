
#ifndef __NOTIFICATIONMANAGER_H__
#define __NOTIFICATIONMANAGER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

class NotificationManager : public ElRefBase, public INotificationManagerProxy
{
public:
    /** @hide */
    static AutoPtr<INotificationManager> GetService();

    /* package */ NotificationManager(
        /* [in] */ IContext* context,
        /* [in] */ IApartment* handler);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    /**
     * Persistent notification on the status bar,
     *
     * @param id An identifier for this notification unique within your
     *        application.
     * @param notification A {@link Notification} object describing how to
     *        notify the user, other than the view you're providing. Must not be null.
     */
    CARAPI Notify(
        /* [in] */ Int32 id,
        /* [in] */ INotification* notification);

    /**
     * Persistent notification on the status bar,
     *
     * @param tag An string identifier for this notification unique within your
     *        application.
     * @param notification A {@link Notification} object describing how to
     *        notify the user, other than the view you're providing. Must not be null.
     * @return the id of the notification that is associated with the string identifier that
     * can be used to cancel the notification
     */
    CARAPI NotifyEx(
        /* [in] */ const String& tag,
        /* [in] */ Int32 id,
        /* [in] */ INotification* notification);

    /**
     * Cancel a previously shown notification.  If it's transient, the view
     * will be hidden.  If it's persistent, it will be removed from the status
     * bar.
     */
    CARAPI Cancel(
        /* [in] */ Int32 id);

    /**
     * Cancel a previously shown notification.  If it's transient, the view
     * will be hidden.  If it's persistent, it will be removed from the status
     * bar.
     */
    CARAPI CancelEx(
        /* [in] */ const String& tag,
        /* [in] */ Int32 id);

    /**
     * Cancel all previously shown notifications. See {@link #cancel} for the
     * detailed behavior.
     */
    CARAPI CancelAll();

private:
    static const CString TAG;
    static const Boolean DEBUG = FALSE;
    static const Boolean localLOGV = DEBUG/* || android.util.Config.LOGV*/;

    static AutoPtr<INotificationManager> sService;

    AutoPtr<IContext> mContext;
};

#endif //__NOTIFICATIONMANAGER_H__

