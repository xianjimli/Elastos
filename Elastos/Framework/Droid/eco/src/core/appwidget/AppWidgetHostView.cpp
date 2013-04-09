
#include "ext/frameworkdef.h"
#include "appwidget/AppWidgetHostView.h"
#include "graphics/CPaint.h"
#include "graphics/CBitmapFactory.h"
#include "graphics/CColor.h"
#include "widget/CFrameLayoutLayoutParams.h"
#include "widget/CTextView.h"
#include "os/SystemClock.h"

const CString AppWidgetHostView::TAG = "AppWidgetHostView";
const Boolean AppWidgetHostView::AppWidgetHostView_LOGD = FALSE;
const Boolean AppWidgetHostView::AppWidgetHostView_CROSSFADE = FALSE;

AppWidgetHostView::AppWidgetHostView()
    : mAppWidgetId(0)
    , mViewMode(AppWidgetHostView_VIEW_MODE_NOINIT)
    , mLayoutId(-1)
    , mFadeStartTime(-1)
{
    CPaint::New((IPaint**)&mOldPaint);
}

ECode AppWidgetHostView::SetAppWidget(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ IAppWidgetProviderInfo* info)
{
    mAppWidgetId = appWidgetId;
    mInfo = info;
    return NOERROR;
}

ECode AppWidgetHostView::GetAppWidgetId(
    /* [out] */ Int32* appWidgetId)
{
    VALIDATE_NOT_NULL(appWidgetId);
    *appWidgetId = mAppWidgetId;
    return NOERROR;
}

ECode AppWidgetHostView::GetAppWidgetInfo(
    /* [out] */ IAppWidgetProviderInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = mInfo;
    return NOERROR;
}

Int32 AppWidgetHostView::GenerateId()
{
    Int32 id = GetId();
    return id == View_NO_ID ? mAppWidgetId : id;
}

AutoPtr<IViewGroupLayoutParams> AppWidgetHostView::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs)
{
    // We're being asked to inflate parameters, probably by a LayoutInflater
    // in a remote Context. To help resolve any remote references, we
    // inflate through our last mRemoteContext when it exists.
    AutoPtr<IContext> context = mRemoteContext != NULL ? mRemoteContext : mContext;
    AutoPtr<IViewGroupLayoutParams> params;
    ASSERT_SUCCEEDED((CFrameLayoutLayoutParams::New(context, attrs,
        (IFrameLayoutLayoutParams**)&params)));
    return params;
}

ECode AppWidgetHostView::ResetAppWidget(
    /* [in] */ IAppWidgetProviderInfo* info)
{
    mInfo = info;
    mViewMode = AppWidgetHostView_VIEW_MODE_NOINIT;
    return UpdateAppWidget(NULL);
}

ECode AppWidgetHostView::UpdateAppWidget(
    /* [in] */ IRemoteViews* remoteViews)
{
    //if (LOGD) Log.d(TAG, "updateAppWidget called mOld=" + mOld);

    Boolean recycled = FALSE;
    AutoPtr<IView> content;

    // Capture the old view into a bitmap so we can do the crossfade.
    if (AppWidgetHostView_CROSSFADE) {
        if (mFadeStartTime < 0) {
            if (mView != NULL) {
                Int32 width, height;
                mView->GetWidth(&width);
                mView->GetHeight(&height);
                //try {
                AutoPtr<IBitmapFactory> factory;
                ASSERT_SUCCEEDED(CBitmapFactory::AcquireSingleton(
                            (IBitmapFactory**)&factory));
                ECode ec = factory->CreateBitmapEx3(width, height,
                        BitmapConfig_ARGB_8888, (IBitmap**)&mOld);
                if (FAILED(ec)) {
                    mOld = NULL;
                }
                // } catch (OutOfMemoryError e) {
                //     // we just won't do the fade
                //     mOld = null;
                // }
                if (mOld != NULL) {
                    //mView.drawIntoBitmap(mOld);
                }
            }
        }
    }

    if (remoteViews == NULL) {
        if (mViewMode == AppWidgetHostView_VIEW_MODE_DEFAULT) {
            // We've already done this -- nothing to do.
            return NOERROR;
        }
        GetDefaultView((IView**)&content);
        mLayoutId = -1;
        mViewMode = AppWidgetHostView_VIEW_MODE_DEFAULT;
    }
    else {
        // Prepare a local reference to the remote Context so we're ready to
        // inflate any requested LayoutParams.
        GetRemoteContext(remoteViews, (IContext**)&mRemoteContext);
        //Int32 layoutId;
        //remoteViews->GetLayoutId(&layoutId);

        // If our stale view has been prepared to match active, and the new
        // layout matches, try recycling it
        if (content == NULL /*&& layoutId == mLayoutId*/) {
            //try {
            //remoteViews->Reapply(mContext, mView);
            content = mView;
            recycled = TRUE;
            //if (LOGD) Log.d(TAG, "was able to recycled existing layout");
            // } catch (RuntimeException e) {
            //     exception = e;
            // }
        }

        // Try normal RemoteView inflation
        if (content == NULL) {
            //try {
            //remoteViews->Apply(mContext, this, (IView**)*content);
                //if (LOGD) Log.d(TAG, "had to inflate new layout");
            // } catch (RuntimeException e) {
            //     exception = e;
            // }
        }

        //mLayoutId = layoutId;
        mViewMode = AppWidgetHostView_VIEW_MODE_CONTENT;
    }

    if (content == NULL) {
        if (mViewMode == AppWidgetHostView_VIEW_MODE_ERROR) {
            // We've already done this -- nothing to do.
            return NOERROR;
        }
        //Log.w(TAG, "updateAppWidget couldn't find any view, using error view", exception);
        GetErrorView((IView**)&content);
        mViewMode = AppWidgetHostView_VIEW_MODE_ERROR;
    }

    if (!recycled) {
        PrepareView(content);
        AddView(content);
    }

    if (mView != content) {
        RemoveView(mView);
        mView = content;
    }

    if (AppWidgetHostView_CROSSFADE) {
        if (mFadeStartTime < 0) {
            // if there is already an animation in progress, don't do anything --
            // the new view will pop in on top of the old one during the cross fade,
            // and that looks okay.
            mFadeStartTime = SystemClock::GetUptimeMillis();
            Invalidate();
        }
    }

    return NOERROR;
}

ECode AppWidgetHostView::GetRemoteContext(
    /* [in] */ IRemoteViews* views,
    /* [out] */ IContext** context)
{
    // Bail if missing package name
    String capsuleName;
    //views->GetCapsule(&capsuleName);
    // if (capsuleName.IsNull()) {
    //     *context = mContext;
    // }

    //try {
        // Return if cloned successfully, otherwise default
    ECode ec = mContext->CreateCapsuleContext(capsuleName, Context_CONTEXT_RESTRICTED, context);
    if (FAILED(ec)) {
        //Log.e(TAG, "Package name " + packageName + " not found");
        *context = mContext;
    }
    // } catch (NameNotFoundException e) {
    //     Log.e(TAG, "Package name " + packageName + " not found");
    //     return mContext;
    // }
    return NOERROR;
}

Boolean AppWidgetHostView::DrawChild(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IView* child,
    /* [in] */ Int64 drawingTime)
{
    if (AppWidgetHostView_CROSSFADE) {
        Int32 alpha;
        Int32 l, t;
        child->GetLeft(&l);
        child->GetTop(&t);
        if (mFadeStartTime > 0) {
            alpha = (Int32)(((drawingTime-mFadeStartTime) * 255) / AppWidgetHostView_FADE_DURATION);
            if (alpha > 255) {
                alpha = 255;
            }
            // Log.d(TAG, "drawChild alpha=" + alpha + " l=" + l + " t=" + t
            //         + " w=" + child.getWidth());
            if (alpha != 255 && mOld != NULL) {
                mOldPaint->SetAlpha(255 - alpha);
                //canvas.drawBitmap(mOld, l, t, mOldPaint);
            }
        }
        else {
            alpha = 255;
        }
        Int32 restoreTo;
        Int32 w, h;
        child->GetWidth(&w);
        child->GetHeight(&h);
        canvas->SaveLayerAlphaEx(l, t, w, h, alpha,
                Canvas_HAS_ALPHA_LAYER_SAVE_FLAG | Canvas_CLIP_TO_LAYER_SAVE_FLAG,
                & restoreTo);
        Boolean rv = FrameLayout::DrawChild(canvas, child, drawingTime);
        canvas->RestoreToCount(restoreTo);
        if (alpha < 255) {
            Invalidate();
        }
        else {
            mFadeStartTime = -1;
            if (mOld != NULL) {
                mOld->Recycle();
                mOld = NULL;
            }
        }
        return rv;
    }
    else {
        return FrameLayout::DrawChild(canvas, child, drawingTime);
    }
}

void AppWidgetHostView::PrepareView(
    /* [in] */ IView* view)
{
    // Take requested dimensions from child, but apply default gravity.
    AutoPtr<IFrameLayoutLayoutParams> requested;
    view->GetLayoutParams((IViewGroupLayoutParams**)&requested);
    if (requested == NULL) {
        CFrameLayoutLayoutParams::New(ViewGroupLayoutParams_MATCH_PARENT,
                ViewGroupLayoutParams_MATCH_PARENT, (IFrameLayoutLayoutParams**)&requested);
    }

    //requested.gravity = Gravity.CENTER;
    view->SetLayoutParams(requested);
}

ECode AppWidgetHostView::GetDefaultView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    // if (LOGD) {
    //     Log.d(TAG, "getDefaultView");
    // }
    AutoPtr<IView> defaultView;

    //try {
    if (mInfo != NULL) {
        AutoPtr<IContext> theirContext;
        AutoPtr<IComponentName> provider;
        mInfo->GetProvider((IComponentName**)&provider);
        String capsuleName;
        provider->GetCapsuleName(&capsuleName);
        mContext->CreateCapsuleContext(capsuleName,
                Context_CONTEXT_RESTRICTED, (IContext**)&theirContext);
        mRemoteContext = theirContext;
        AutoPtr<ILayoutInflater> inflater;
        theirContext->GetSystemService(Context_LAYOUT_INFLATER_SERVICE,
                (IInterface**)&inflater);
        inflater->CloneInContext(theirContext, (ILayoutInflater**)&inflater);
        //inflater.setFilter(sInflaterFilter);
        Int32 layout;
        mInfo->GetInitialLayout(&layout);
        AutoPtr<IViewGroup> root = (IViewGroup*)this->Probe(EIID_IViewGroup);
        inflater->InflateEx2(layout, root, FALSE, (IView**)&defaultView);
    }
    else {
        //Log.w(TAG, "can't inflate defaultView because mInfo is missing");
    }
    // } catch (PackageManager.NameNotFoundException e) {
    //     exception = e;
    // } catch (RuntimeException e) {
    //     exception = e;
    // }

    // if (exception != null) {
    //     Log.w(TAG, "Error inflating AppWidget " + mInfo + ": " + exception.toString());
    // }

    if (defaultView == NULL) {
        //if (LOGD) Log.d(TAG, "getDefaultView couldn't find any view, so inflating error");
        GetErrorView((IView**)&defaultView);
    }

    *view = defaultView;
    return NOERROR;
}

ECode AppWidgetHostView::GetErrorView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<ITextView> tv;
    CTextView::New(mContext, (ITextView**)&tv);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(String("Error inflating widget"/*com.android.internal.R.string.gadget_host_error_inflating*/),
            (ICharSequence**)&cs);
    tv->SetText(cs);
    // TODO: get this color from somewhere.
    AutoPtr<IColor> c;
    CColor::AcquireSingleton((IColor**)&c);
    Int32 rgb;
    c->Argb(127, 0, 0, 0, &rgb);
    tv->SetBackgroundColor(rgb);
    *view = (IView*)tv->Probe(EIID_IView);
    return NOERROR;
}

ECode AppWidgetHostView::Init(
    /* [in] */ IContext* context)
{
    return Init(context, 0x010a0000/*android.R.anim.fade_in*/, 0x010a0001/*android.R.anim.fade_out*/);
}

ECode AppWidgetHostView::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 animationIn,
    /* [in] */ Int32 animationOut)
{
    FrameLayout::Init(context);
    mContext = context;
    return NOERROR;
}
