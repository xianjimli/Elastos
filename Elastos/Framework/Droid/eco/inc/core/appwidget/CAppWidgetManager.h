
#ifndef __CAPPWIDGETMANAGER_H__
#define __CAPPWIDGETMANAGER_H__

#include "_CAppWidgetManager.h"
#include "ext/frameworkdef.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include <elastos/HashMap.h>

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<IContext> >
{
    size_t operator()(AutoPtr<IContext> s) const
    {
        assert(s != NULL);
        return (size_t)s.Get();
    }
};

_ELASTOS_NAMESPACE_END

using namespace Elastos::Core::Threading;

CarClass(CAppWidgetManager)
{
public:
    /**
     * Get the AppWidgetManager instance to use for the supplied {@link android.content.Context
     * Context} object.
     */
    static CARAPI GetInstance(
        /* [in] */ IContext* context,
        /* [out] */ IAppWidgetManager** appWidgetManager);

    /**
     * Set the RemoteViews to use for the specified appWidgetIds.
     *
     * <p>
     * It is okay to call this method both inside an {@link #ACTION_APPWIDGET_UPDATE} broadcast,
     * and outside of the handler.
     * This method will only work when called from the uid that owns the AppWidget provider.
     *
     * @param appWidgetIds     The AppWidget instances for which to set the RemoteViews.
     * @param views         The RemoteViews object to show.
     */
    CARAPI UpdateAppWidget(
        /* [in] */ const ArrayOf<Int32>& appWidgetIds,
        /* [in] */ IRemoteViews* views);

    /**
     * Set the RemoteViews to use for the specified appWidgetId.
     *
     * <p>
     * It is okay to call this method both inside an {@link #ACTION_APPWIDGET_UPDATE} broadcast,
     * and outside of the handler.
     * This method will only work when called from the uid that owns the AppWidget provider.
     *
     * @param appWidgetId      The AppWidget instance for which to set the RemoteViews.
     * @param views         The RemoteViews object to show.
     */
    CARAPI UpdateAppWidgetEx(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IRemoteViews* views);

    /**
     * Set the RemoteViews to use for all AppWidget instances for the supplied AppWidget provider.
     *
     * <p>
     * It is okay to call this method both inside an {@link #ACTION_APPWIDGET_UPDATE} broadcast,
     * and outside of the handler.
     * This method will only work when called from the uid that owns the AppWidget provider.
     *
     * @param provider      The {@link ComponentName} for the {@link
     * android.content.BroadcastReceiver BroadcastReceiver} provider
     *                      for your AppWidget.
     * @param views         The RemoteViews object to show.
     */
    CARAPI UpdateAppWidgetEx2(
        /* [in] */ IComponentName* provider,
        /* [in] */ IRemoteViews* views);

    /**
     * Return a list of the AppWidget providers that are currently installed.
     */
    CARAPI GetInstalledProviders(
        /* [out] */ IObjectContainer** providerInfos);

    /**
     * Get the available info about the AppWidget.
     *
     * @return A appWidgetId.  If the appWidgetId has not been bound to a provider yet, or
     * you don't have access to that appWidgetId, null is returned.
     */
    CARAPI GetAppWidgetInfo(
        /* [in] */ Int32 appWidgetId,
        /* [out] */ IAppWidgetProviderInfo** info);

    /**
     * Set the component for a given appWidgetId.
     *
     * <p class="note">You need the APPWIDGET_LIST permission.  This method is to be used by the
     * AppWidget picker.
     *
     * @param appWidgetId     The AppWidget instance for which to set the RemoteViews.
     * @param provider      The {@link android.content.BroadcastReceiver} that will be the AppWidget
     *                      provider for this AppWidget.
     */
    CARAPI BindAppWidgetId(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IComponentName* provider);

    /**
     * Get the list of appWidgetIds that have been bound to the given AppWidget
     * provider.
     *
     * @param provider The {@link android.content.BroadcastReceiver} that is the
     *            AppWidget provider to find appWidgetIds for.
     */
    CARAPI GetAppWidgetIds(
        /* [in] */ IComponentName* provider,
        /* [out, callee] */ ArrayOf<Int32>** appWidgetIds);

    CARAPI constructor(
        /* [in] */ IContext* context);

public:
    static const CString TAG;
    static HashMap<AutoPtr<IContext>, AutoPtr<IAppWidgetManager> > sManagerCache;
    static Mutex sManagerCacheLock;
    static AutoPtr<IAppWidgetService> sService;

    AutoPtr<IContext> mContext;

private:
    AutoPtr<IDisplayMetrics> mDisplayMetrics;
};

#endif //__CAPPWIDGETMANAGER_H__
