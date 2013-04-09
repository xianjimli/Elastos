
#ifndef __APPWIDGETHOSTVIEW_H__
#define __APPWIDGETHOSTVIEW_H__

#include <elastos/AutoPtr.h>
#include "widget/FrameLayout.h"

class AppWidgetHostView : public FrameLayout
{
protected:
    AppWidgetHostView();

    virtual CARAPI SetAppWidget(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ IAppWidgetProviderInfo* info);

    virtual CARAPI GetAppWidgetId(
        /* [out] */ Int32* appWidgetId);

    virtual CARAPI GetAppWidgetInfo(
        /* [out] */ IAppWidgetProviderInfo** info);

    CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs);

    virtual CARAPI ResetAppWidget(
        /* [in] */ IAppWidgetProviderInfo* info);

    virtual CARAPI UpdateAppWidget(
        /* [in] */ IRemoteViews* remoteViews);

    CARAPI_(Boolean) DrawChild(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IView* child,
        /* [in] */ Int64 drawingTime);

    /**
     * Prepare the given view to be shown. This might include adjusting
     * {@link FrameLayout.LayoutParams} before inserting.
     */
    CARAPI_(void) PrepareView(
        /* [in] */ IView* view);

    /**
     * Inflate and return the default layout requested by AppWidget provider.
     */
    CARAPI GetDefaultView(
        /* [out] */ IView** view);

    /**
     * Inflate and return a view that represents an error state.
     */
    CARAPI GetErrorView(
        /* [out] */ IView** view);

    /**
     * Create a host view.  Uses default fade animations.
     */
    CARAPI Init(
        /* [in] */ IContext* context);

    /**
     * Create a host view. Uses specified animations when pushing
     * {@link #updateAppWidget(RemoteViews)}.
     *
     * @param animationIn Resource ID of in animation to use
     * @param animationOut Resource ID of out animation to use
     */
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Int32 animationIn,
        /* [in] */ Int32 animationOut);

private:
    CARAPI_(Int32) GenerateId();

    /**
     * Build a {@link Context} cloned into another package name, usually for the
     * purposes of reading remote resources.
     */
    CARAPI GetRemoteContext(
        /* [in] */ IRemoteViews* views,
        /* [out] */ IContext** context);

public:
    static const CString TAG;
    static const Boolean AppWidgetHostView_LOGD;
    static const Boolean AppWidgetHostView_CROSSFADE;

    // When we're inflating the initialLayout for a AppWidget, we only allow
    // views that are allowed in RemoteViews.
    // static final LayoutInflater.Filter sInflaterFilter = new LayoutInflater.Filter() {
    //     public boolean onLoadClass(Class clazz) {
    //         return clazz.isAnnotationPresent(RemoteViews.RemoteView.class);
    //     }
    // };

    AutoPtr<IContext> mContext;
    AutoPtr<IContext> mRemoteContext;

    Int32 mAppWidgetId;
    AutoPtr<IAppWidgetProviderInfo> mInfo;
    AutoPtr<IView> mView;
    Int32 mViewMode;
    Int32 mLayoutId;
    Int64 mFadeStartTime;
    AutoPtr<IBitmap> mOld;
    AutoPtr<IPaint> mOldPaint;// = new Paint();
};

#endif //__APPWIDGETHOSTVIEW_H__
