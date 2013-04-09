
#include "ext/frameworkdef.h"
#include "appwidget/CAppWidgetHostView.h"

IVIEW_METHODS_IMPL(CAppWidgetHostView, AppWidgetHostView, AppWidgetHostView);

IVIEWGROUP_METHODS_IMPL(CAppWidgetHostView, AppWidgetHostView, AppWidgetHostView);

PInterface CAppWidgetHostView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CAppWidgetHostView::Probe(riid);
}

ECode CAppWidgetHostView::SetAppWidget(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IAppWidgetProviderInfo* info)
{
    return AppWidgetHostView::SetAppWidget(appWidgetId, info);
}

ECode CAppWidgetHostView::GetAppWidgetId(
    /* [out] */ Int32* appWidgetId)
{
    return AppWidgetHostView::GetAppWidgetId(appWidgetId);
}

ECode CAppWidgetHostView::GetAppWidgetInfo(
    /* [out] */ IAppWidgetProviderInfo** info)
{
    return AppWidgetHostView::GetAppWidgetInfo(info);
}

ECode CAppWidgetHostView::ResetAppWidget(
    /* [in] */ IAppWidgetProviderInfo* info)
{
    return AppWidgetHostView::ResetAppWidget(info);
}

ECode CAppWidgetHostView::UpdateAppWidget(
    /* [in] */ IRemoteViews* remoteViews)
{
    return AppWidgetHostView::UpdateAppWidget(remoteViews);
}

ECode CAppWidgetHostView::SetForegroundGravity(
    /* [in] */ Int32 foregroundGravity)
{
    return AppWidgetHostView::SetForegroundGravity(foregroundGravity);
}

ECode CAppWidgetHostView::SetForeground(
    /* [in] */ IDrawable* drawable)
{
    return AppWidgetHostView::SetForeground(drawable);
}

ECode CAppWidgetHostView::GetForeground(
    /* [out] */ IDrawable** foreground)
{
    VALIDATE_NOT_NULL(foreground);
    AutoPtr<IDrawable> d = AppWidgetHostView::GetForeground();
    *foreground = d.Get();
    if (*foreground) {
        (*foreground)->AddRef();
    }

    return NOERROR;
}

ECode CAppWidgetHostView::SetMeasureAllChildren(
    /* [in] */ Boolean measureAll)
{
    return AppWidgetHostView::SetMeasureAllChildren(measureAll);
}

ECode CAppWidgetHostView::GetConsiderGoneChildrenWhenMeasuring(
    /* [out] */ Boolean* measureAll)
{
    VALIDATE_NOT_NULL(measureAll);
    *measureAll = FrameLayout::GetConsiderGoneChildrenWhenMeasuring();

    return NOERROR;
}

ECode CAppWidgetHostView::constructor(
    /* [in] */ IContext* context)
{
    return AppWidgetHostView::Init(context);
}

ECode CAppWidgetHostView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 animationIn,
    /* [in] */ Int32 animationOut)
{
    return AppWidgetHostView::Init(context, animationIn, animationOut);
}
