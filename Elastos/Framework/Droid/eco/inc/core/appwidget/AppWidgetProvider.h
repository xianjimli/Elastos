
#ifndef __APPWIDGETPROVIDER_H__
#define __APPWIDGETPROVIDER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

class AppWidgetProvider
{
    /**
     * Implements {@link BroadcastReceiver#onReceive} to dispatch calls to the various
     * other methods on AppWidgetProvider.
     *
     * @param context The Context in which the receiver is running.
     * @param intent The Intent being received.
     */
    // BEGIN_INCLUDE(onReceive)
    CARAPI OnReceive(
        /* [in] */ IContext* context,
        /* [in] */ IIntent* intent);

    /**
     * Called in response to the {@link AppWidgetManager#ACTION_APPWIDGET_UPDATE} broadcast when
     * this AppWidget provider is being asked to provide {@link android.widget.RemoteViews RemoteViews}
     * for a set of AppWidgets.  Override this method to implement your own AppWidget functionality.
     *
     * {@more}
     *
     * @param context   The {@link android.content.Context Context} in which this receiver is
     *                  running.
     * @param appWidgetManager A {@link AppWidgetManager} object you can call {@link
     *                  AppWidgetManager#updateAppWidget} on.
     * @param appWidgetIds The appWidgetIds for which an update is needed.  Note that this
     *                  may be all of the AppWidget instances for this provider, or just
     *                  a subset of them.
     *
     * @see AppWidgetManager#ACTION_APPWIDGET_UPDATE
     */
    CARAPI OnUpdate(
        /* [in] */ IContext* context,
        /* [in] */ IAppWidgetManager* appWidgetManager,
        /* [in] */ const ArrayOf<Int32>& appWidgetIds);

    /**
     * Called in response to the {@link AppWidgetManager#ACTION_APPWIDGET_DELETED} broadcast when
     * one or more AppWidget instances have been deleted.  Override this method to implement
     * your own AppWidget functionality.
     *
     * {@more}
     *
     * @param context   The {@link android.content.Context Context} in which this receiver is
     *                  running.
     * @param appWidgetIds The appWidgetIds that have been deleted from their host.
     *
     * @see AppWidgetManager#ACTION_APPWIDGET_DELETED
     */
    CARAPI OnDeleted(
        /* [in] */ IContext* context,
        /* [in] */ const ArrayOf<Int32>& appWidgetIds);

    /**
     * Called in response to the {@link AppWidgetManager#ACTION_APPWIDGET_ENABLED} broadcast when
     * the a AppWidget for this provider is instantiated.  Override this method to implement your
     * own AppWidget functionality.
     *
     * {@more}
     * When the last AppWidget for this provider is deleted,
     * {@link AppWidgetManager#ACTION_APPWIDGET_DISABLED} is sent by the AppWidget manager, and
     * {@link #onDisabled} is called.  If after that, an AppWidget for this provider is created
     * again, onEnabled() will be called again.
     *
     * @param context   The {@link android.content.Context Context} in which this receiver is
     *                  running.
     *
     * @see AppWidgetManager#ACTION_APPWIDGET_ENABLED
     */
    CARAPI OnEnabled(
        /* [in] */ IContext* context);

    /**
     * Called in response to the {@link AppWidgetManager#ACTION_APPWIDGET_DISABLED} broadcast, which
     * is sent when the last AppWidget instance for this provider is deleted.  Override this method
     * to implement your own AppWidget functionality.
     *
     * {@more}
     *
     * @param context   The {@link android.content.Context Context} in which this receiver is
     *                  running.
     *
     * @see AppWidgetManager#ACTION_APPWIDGET_DISABLED
     */
    CARAPI OnDisabled(
        /* [in] */ IContext* context);
};

#endif //__APPWIDGETPROVIDER_H__
