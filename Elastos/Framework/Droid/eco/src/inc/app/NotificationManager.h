
#ifndef __NOTIFICATIONMANAGER_H__
#define __NOTIFICATIONMANAGER_H__

#include "ext/frameworkext.h"

using namespace Elastos;

class NotificationManager : public INotificationManager
{
public:
    /**
     * Persistent notification on the status bar,
     *
     * @param id An identifier for this notification unique within your
     *        application.
     * @param notification A {@link Notification} object describing how to
     *        notify the user, other than the view you're providing. Must not be null.
     */
    CARAPI_(void) Notify(
        /* [in] */ Int32 id,
        /* [in] */ INotification* notification);

    /**
     * Cancel a previously shown notification.  If it's transient, the view
     * will be hidden.  If it's persistent, it will be removed from the status
     * bar.
     */
    CARAPI_(void) Cancel(
        /* [in] */ Int32 id);
};

#endif //__NOTIFICATIONMANAGER_H__

