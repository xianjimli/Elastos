
#ifndef __SURFACEVIEW_H__
#define __SURFACEVIEW_H__

#include "view/View.h"
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>

/**
 * Provides a dedicated drawing surface embedded inside of a view hierarchy.
 * You can control the format of this surface and, if you like, its size; the
 * SurfaceView takes care of placing the surface at the correct location on the
 * screen
 *
 * <p>The surface is Z ordered so that it is behind the window holding its
 * SurfaceView; the SurfaceView punches a hole in its window to allow its
 * surface to be displayed.  The view hierarchy will take care of correctly
 * compositing with the Surface any siblings of the SurfaceView that would
 * normally appear on top of it.  This can be used to place overlays such as
 * buttons on top of the Surface, though note however that it can have an
 * impact on performance since a full alpha-blended composite will be performed
 * each time the Surface changes.
 *
 * <p>Access to the underlying surface is provided via the SurfaceHolder interface,
 * which can be retrieved by calling {@link #getHolder}.
 *
 * <p>The Surface will be created for you while the SurfaceView's window is
 * visible; you should implement {@link SurfaceHolder.Callback#surfaceCreated}
 * and {@link SurfaceHolder.Callback#surfaceDestroyed} to discover when the
 * Surface is created and destroyed as the window is shown and hidden.
 *
 * <p>One of the purposes of this class is to provide a surface in which a
 * secondary thread can render in to the screen.  If you are going to use it
 * this way, you need to be aware of some threading semantics:
 *
 * <ul>
 * <li> All SurfaceView and
 * {@link SurfaceHolder.Callback SurfaceHolder.Callback} methods will be called
 * from the thread running the SurfaceView's window (typically the main thread
 * of the application).  They thus need to correctly synchronize with any
 * state that is also touched by the drawing thread.
 * <li> You must ensure that the drawing thread only touches the underlying
 * Surface while it is valid -- between
 * {@link SurfaceHolder.Callback#surfaceCreated SurfaceHolder.Callback.surfaceCreated()}
 * and
 * {@link SurfaceHolder.Callback#surfaceDestroyed SurfaceHolder.Callback.surfaceDestroyed()}.
 * </ul>
 */
class SurfaceView : public View
{
public:
    SurfaceView();

    SurfaceView(
        /* [in] */ IContext* context);

    SurfaceView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    SurfaceView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    ~SurfaceView();

    /**
     * Return the SurfaceHolder providing access and control over this
     * SurfaceView's underlying surface.
     *
     * @return SurfaceHolder The holder of the surface.
     */
    virtual CARAPI_(AutoPtr<ISurfaceHolder>) GetHolder();

    virtual CARAPI SetVisibility(
        /* [in] */ Int32 visibility);

    virtual CARAPI_(Boolean) GatherTransparentRegion(
        /* [in] */ IRegion* region);

    virtual CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    /**
     * Control whether the surface view's surface is placed on top of another
     * regular surface view in the window (but still behind the window itself).
     * This is typically used to place overlays on top of an underlying media
     * surface view.
     *
     * <p>Note that this must be set before the surface view's containing
     * window is attached to the window manager.
     *
     * <p>Calling this overrides any previous call to {@link #setZOrderOnTop}.
     */
    virtual CARAPI SetZOrderMediaOverlay(
        /* [in] */ Boolean isMediaOverlay);

    /**
     * Control whether the surface view's surface is placed on top of its
     * window.  Normally it is placed behind the window, to allow it to
     * (for the most part) appear to composite with the views in the
     * hierarchy.  By setting this, you cause it to be placed above the
     * window.  This means that none of the contents of the window this
     * SurfaceView is in will be visible on top of its surface.
     *
     * <p>Note that this must be set before the surface view's containing
     * window is attached to the window manager.
     *
     * <p>Calling this overrides any previous call to {@link #setZOrderMediaOverlay}.
     */
    virtual CARAPI SetZOrderOnTop(
        /* [in] */ Boolean onTop);

    /**
     * Hack to allow special layering of windows.  The type is one of the
     * types in WindowManager.LayoutParams.  This is a hack so:
     * @hide
     */
    virtual CARAPI SetWindowType(
        /* [in] */ Int32 type);

    /**
     * Check to see if the surface has fixed size dimensions or if the surface's
     * dimensions are dimensions are dependent on its current layout.
     *
     * @return TRUE if the surface has dimensions that are fixed in size
     * @hide
     */
    virtual CARAPI_(Boolean) IsFixedSize();

protected:
    virtual CARAPI_(void) OnAttachedToWindow();

    virtual CARAPI_(void) OnWindowVisibilityChanged(
        /* [in] */ Int32 visibility);

    /**
     * This method is not intended for general use. It was created
     * temporarily to improve performance of 3D layers in Launcher
     * and should be removed and fixed properly.
     *
     * Do not call this method. Ever.
     *
     * @hide
     */
    virtual CARAPI_(void) ShowSurface();

    /**
     * This method is not intended for general use. It was created
     * temporarily to improve performance of 3D layers in Launcher
     * and should be removed and fixed properly.
     *
     * Do not call this method. Ever.
     *
     * @hide
     */
    virtual CARAPI_(void) HideSurface();

    virtual CARAPI_(void) OnDetachedFromWindow();

    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    /** @hide */
    virtual CARAPI_(Boolean) SetFrame(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI_(void) DispatchDraw(
        /* [in] */ ICanvas* canvas);

protected:
    CARAPI_(void) Init();

private:
    virtual CARAPI_(void) UpdateWindow(
        /* [in] */ Boolean force,
        /* [in] */ Boolean redrawNeeded);

    virtual CARAPI_(void) ReportSurfaceDestroyed();

    virtual CARAPI_(void) HandleGetNewSurface();

    class MyWindow : /*public IBaseIWindow,*/
        public ElRefBase,
        public IInnerWindow
    {
    public:
        MyWindow(
            /* [in] */ ISurfaceView* surfaceView);

        CARAPI Resized(
            /* [in] */ Int32 w,
            /* [in] */ Int32 h,
            /* [in] */ IRect* coveredInsets,
            /* [in] */ IRect* visibleInsets,
            /* [in] */ Boolean reportDraw,
            /* [in] */ IConfiguration* newConfig);

        CARAPI DispatchAppVisibility(
            /* [in] */ Boolean visible);

        CARAPI DispatchGetNewSurface();

        CARAPI WindowFocusChanged(
            /* [in] */ Boolean hasFocus,
            /* [in] */ Boolean touchEnabled);

        CARAPI ExecuteCommand(
            /* [in] */ const String& command,
            /* [in] */ const String& parameters,
            /* [in] */ IParcelFileDescriptor* out);

        CARAPI CloseSystemDialogs(
            /* [in] */ const String& reason);

        CARAPI DispatchWallpaperOffsets(
            /* [in] */ Float x,
            /* [in] */ Float y,
            /* [in] */ Float xStep,
            /* [in] */ Float yStep,
            /* [in] */ Boolean sync);

        CARAPI DispatchWallpaperCommand(
            /* [in] */ const String& action,
            /* [in] */ Int32 x,
            /* [in] */ Int32 y,
            /* [in] */ Int32 z,
            /* [in] */ IBundle* extras,
            /* [in] */ Boolean sync);

        CARAPI GetDescription(
                /* [out] */ String* description);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    private:
        AutoPtr<ISurfaceView> mSurfaceView;

        Int32 mCurWidth;// = -1;
        Int32 mCurHeight;// = -1;
    };

public:
    Mutex mSurfaceLock;
    AutoPtr<IApartment> mHandler;

private:
    AutoPtr<ISurfaceHolder> mSurfaceHolder;

    //static const String TAG = "SurfaceView";
    static const Boolean DEBUG = FALSE;
    //static const Boolean localLOGV = DEBUG ? TRUE : Config.LOGV;

    /*ArrayList<SurfaceHolder.Callback> mCallbacks
            = new ArrayList<SurfaceHolder.Callback>();*/
    ArrayOf<AutoPtr<ISurfaceHolderCallback> >* mCallbacks;

    ArrayOf<Int32>* mLocation;// = new Int32[2];

    //ReentrantLock mSurfaceLock = new ReentrantLock();
    AutoPtr<ISurface> mSurface;// = new Surface();
    Boolean mDrawingStopped;// = TRUE;

    AutoPtr<IWindowManagerLayoutParams> mLayout;
            //= new WindowManager.LayoutParams();
    AutoPtr<IWindowSession> mSession;
    AutoPtr<MyWindow> mWindow;
    AutoPtr<IRect> mVisibleInsets;// = new Rect();
    AutoPtr<IRect> mWinFrame;// = new Rect();
    AutoPtr<IRect> mContentInsets;// = new Rect();
    AutoPtr<IConfiguration> mConfiguration;// = new Configuration();

    static const Int32 KEEP_SCREEN_ON_MSG = 1;
    static const Int32 GET_NEW_SURFACE_MSG = 2;
    static const Int32 UPDATE_WINDOW_MSG = 3;

    Int32 mWindowType;// = WindowManager.LayoutParams.TYPE_APPLICATION_MEDIA;

    Boolean mIsCreating;// = FALSE;

    /*AutoPtr<IApartment> mHandler;*//* = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case KEEP_SCREEN_ON_MSG: {
                    SetKeepScreenOn(msg.arg1 != 0);
                } break;
                case GET_NEW_SURFACE_MSG: {
                    HandleGetNewSurface();
                } break;
                case UPDATE_WINDOW_MSG: {
                    UpdateWindow(FALSE, FALSE);
                } break;
            }
        }
    };*/

    AutoPtr<IOnScrollChangedListener> mScrollChangedListener;
            /*= new ViewTreeObserver.OnScrollChangedListener() {
                    public void onScrollChanged() {
                        updateWindow(FALSE, FALSE);
                    }
            };*/

    Boolean mRequestedVisible;// = FALSE;
    Boolean mWindowVisibility;// = FALSE;
    Boolean mViewVisibility;// = FALSE;
    Int32 mRequestedWidth;// = -1;
    Int32 mRequestedHeight;// = -1;
    /* Set SurfaceView's format to 565 by default to maintain backward
     * compatibility with applications assuming this format.
     */
    Int32 mRequestedFormat;// = PixelFormat.RGB_565;
    Int32 mRequestedType;// = -1;

    Boolean mHaveFrame;// = FALSE;
    Boolean mDestroyReportNeeded;// = FALSE;
    Boolean mNewSurfaceNeeded;// = FALSE;
    Int64 mLastLockTime;// = 0;

    Boolean mVisible;// = FALSE;
    Int32 mLeft;// = -1;
    Int32 mTop;// = -1;
    Int32 mWidth;// = -1;
    Int32 mHeight;// = -1;
    Int32 mFormat;// = -1;
    Int32 mType;// = -1;
    AutoPtr<CRect> mSurfaceFrame;// = new Rect();
    Int32 mLastSurfaceWidth;// = -1;
    Int32 mLastSurfaceHeight;// = -1;
    Boolean mUpdateWindowNeeded;
    Boolean mReportDrawNeeded;
    AutoPtr<ITranslator> mTranslator;

    Mutex mCallbacksLock;
};

#endif
