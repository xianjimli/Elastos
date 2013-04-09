
#ifndef __CAPPWIDGETHOSTVIEW_H__
#define __CAPPWIDGETHOSTVIEW_H__

#include "_CAppWidgetHostView.h"
#include "appwidget/AppWidgetHostView.h"
#include "view/ViewMacro.h"
#include <elastos/AutoPtr.h>

CarClass(CAppWidgetHostView) , public AppWidgetHostView
{
public:
    IVIEW_METHODS_DECL();

    IVIEWGROUP_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
     * Set the AppWidget that will be displayed by this view.
     */
    CARAPI SetAppWidget(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IAppWidgetProviderInfo* info);

    CARAPI GetAppWidgetId(
        /* [out] */ Int32* appWidgetId);

    CARAPI GetAppWidgetInfo(
        /* [out] */ IAppWidgetProviderInfo** info);

    /**
     * Update the AppWidgetProviderInfo for this view, and reset it to the
     * initial layout.
     */
    CARAPI ResetAppWidget(
        /* [in] */ IAppWidgetProviderInfo* info);

    /**
     * Process a set of {@link RemoteViews} coming in as an update from the
     * AppWidget provider. Will animate into these new views as needed
     */
    CARAPI UpdateAppWidget(
        /* [in] */ IRemoteViews* remoteViews);

    CARAPI SetForegroundGravity(
        /* [in] */ Int32 foregroundGravity);

    CARAPI SetForeground(
        /* [in] */ IDrawable* drawable);

    CARAPI GetForeground(
        /* [out] */ IDrawable** foreground);

    CARAPI SetMeasureAllChildren(
        /* [in] */ Boolean measureAll);

    CARAPI GetConsiderGoneChildrenWhenMeasuring(
        /* [out] */ Boolean* measureAll);

    /**
     * Create a host view.  Uses default fade animations.
     */
    CARAPI constructor(
        /* [in] */ IContext* context);

    /**
     * Create a host view. Uses specified animations when pushing
     * {@link #updateAppWidget(RemoteViews)}.
     *
     * @param animationIn Resource ID of in animation to use
     * @param animationOut Resource ID of out animation to use
     */
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 animationIn,
        /* [in] */ Int32 animationOut);
};

#endif //__CAPPWIDGETHOSTVIEW_H__
