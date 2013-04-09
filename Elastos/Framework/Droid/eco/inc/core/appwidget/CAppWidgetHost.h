
#ifndef __CAPPWIDGETHOST_H__
#define __CAPPWIDGETHOST_H__

#include "_CAppWidgetHost.h"
#include "ext/frameworkdef.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include <elastos/HashMap.h>

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<IAppWidgetHostView> >
{
    size_t operator()(AutoPtr<IAppWidgetHostView> s) const
    {
        assert(s != NULL);
        return (size_t)s.Get();
    }
};

_ELASTOS_NAMESPACE_END

using namespace Elastos::Core::Threading;

CarClass(CAppWidgetHost)
{
public:
    CAppWidgetHost();

    /**
     * Start receiving onAppWidgetChanged calls for your AppWidgets.  Call this when your activity
     * becomes visible, i.e. from onStart() in your Activity.
     */
    CARAPI StartListening();

    /**
     * Stop receiving onAppWidgetChanged calls for your AppWidgets.  Call this when your activity is
     * no longer visible, i.e. from onStop() in your Activity.
     */
    CARAPI StopListening();

    /**
     * Get a appWidgetId for a host in the calling process.
     *
     * @return a appWidgetId
     */
    CARAPI AllocateAppWidgetId(
        /* [out] */ Int32* appWidgetId);

    /**
     * Stop listening to changes for this AppWidget.
     */
    CARAPI DeleteAppWidgetId(
        /* [in] */ Int32 appWidgetId);

    /**
     * Remove all records about this host from the AppWidget manager.
     * <ul>
     *   <li>Call this when initializing your database, as it might be because of a data wipe.</li>
     *   <li>Call this to have the AppWidget manager release all resources associated with your
     *   host.  Any future calls about this host will cause the records to be re-allocated.</li>
     * </ul>
     */
    CARAPI DeleteHost();

    CARAPI CreateView(
        /* [in] */ IContext* context,
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IAppWidgetProviderInfo* appWidget,
        /* [out] */ IAppWidgetHostView** hostView);

    CARAPI UpdateAppWidgetView(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IRemoteViews* views);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 hostId);

    CARAPI_(void) HandleUpdate(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IRemoteViews* views);

    CARAPI_(void) HandleProviderChanged(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IAppWidgetProviderInfo* info);

protected:
    /**
     * Called to create the AppWidgetHostView.  Override to return a custom subclass if you
     * need it.  {@more}
     */
    CARAPI OnCreateView(
        /* [in] */ IContext* context,
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IAppWidgetProviderInfo* appWidget,
        /* [out] */ IAppWidgetHostView** hostView);

    /**
     * Called when the AppWidget provider for a AppWidget has been upgraded to a new apk.
     */
    CARAPI_(void) OnProviderChanged(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IAppWidgetProviderInfo* appWidget);

public:
    static AutoPtr<IAppWidgetService> sService;
    static Mutex sServiceLock;

    AutoPtr<IContext> mContext;
    String mCapsuleName;
    AutoPtr<IApartment> mHandler;
    Int32 mHostId;
    AutoPtr<IAppWidgetHost> mCallbacks;
    HashMap<Int32, AutoPtr<IAppWidgetHostView> > mViews;
    Mutex mViewsLock;
};

#endif //__CAPPWIDGETHOST_H__
