
#include "view/ViewRoot.h"
#include "view/SurfaceView.h"
#include "view/CSurfaceView.h"
#include "view/CWindowManagerLayoutParams.h"
#include "content/CConfiguration.h"
#include "graphics/ElPixelFormat.h"
#include "utils/CDisplayMetrics.h"
#include "os/CApartment.h"

//const String SurfaceView::TAG;
const Boolean SurfaceView::DEBUG;
const Int32 SurfaceView::KEEP_SCREEN_ON_MSG;
const Int32 SurfaceView::GET_NEW_SURFACE_MSG;
const Int32 SurfaceView::UPDATE_WINDOW_MSG;

SurfaceView::SurfaceView()
{

}

SurfaceView::SurfaceView(
    /* [in] */ IContext* context) :
    View(context)
{
    Init();
}

SurfaceView::SurfaceView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs) :
    View(context)
{
    Init();
}

SurfaceView::SurfaceView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    Init();
}

SurfaceView::~SurfaceView()
{
    if (mCallbacks != NULL) {
        ArrayOf<AutoPtr<ISurfaceHolderCallback> >::Free(mCallbacks);
        mCallbacks = NULL;
    }
}

void SurfaceView::Init()
{
    mCallbacks = NULL;
    mLocation = ArrayOf<Int32>::Alloc(2);// = new Int32[2];

    //ReentrantLock mSurfaceLock = new ReentrantLock();
    CSurface::New((ISurface**)&mSurface);
    mDrawingStopped = TRUE;

    CWindowManagerLayoutParams::New((IWindowManagerLayoutParams**)&mLayout);
    // CRect::NewByFriend((CRect**)&mVisibleInsets);
    // CRect::NewByFriend((CRect**)&mWinFrame);
    // CRect::NewByFriend((CRect**)&mContentInsets);
    CRect::New((IRect**)&mVisibleInsets);
    CRect::New((IRect**)&mWinFrame);
    CRect::New((IRect**)&mContentInsets);

    CConfiguration::New((IConfiguration**)&mConfiguration);

    mWindowType = WindowManagerLayoutParams_TYPE_APPLICATION_MEDIA;

    mIsCreating = FALSE;

    CApartment::New(TRUE, (IApartment**) &mHandler);
    mHandler->Start(ApartmentAttr_New);

    mRequestedVisible = FALSE;
    mWindowVisibility = FALSE;
    mViewVisibility = FALSE;
    mRequestedWidth = -1;
    mRequestedHeight = -1;

    /* Set SurfaceView's format to 565 by default to maintain backward
     * compatibility with applications assuming this format.
     */
    mRequestedFormat = ElPixelFormat::RGB_565;
    mRequestedType = -1;

    mHaveFrame = FALSE;
    mDestroyReportNeeded = FALSE;
    mNewSurfaceNeeded = FALSE;
    mLastLockTime = 0;

    mVisible = FALSE;
    mLeft = -1;
    mTop = -1;
    mWidth = -1;
    mHeight = -1;
    mFormat = -1;
    mType = -1;
    CRect::NewByFriend((CRect**)&mSurfaceFrame);
    mLastSurfaceWidth = -1;
    mLastSurfaceHeight = -1;

    SetWillNotDraw(TRUE);
}

/**
 * Return the SurfaceHolder providing access and control over this
 * SurfaceView's underlying surface.
 *
 * @return SurfaceHolder The holder of the surface.
 */
AutoPtr<ISurfaceHolder> SurfaceView::GetHolder()
{
    return mSurfaceHolder;
}

void SurfaceView::OnAttachedToWindow()
{
    View::OnAttachedToWindow();
    mParent->RequestTransparentRegion((IView*)this->Probe(EIID_IView));
    mSession = GetWindowSession();
    ((CWindowManagerLayoutParams*)mLayout.Get())->mToken = GetWindowToken();

    AutoPtr<ICharSequence> text;
    CStringWrapper::New(String("SurfaceView"), (ICharSequence**) &text);
    mLayout->SetTitle(text);
    mViewVisibility = GetVisibility() == VISIBLE;
    GetViewTreeObserver()->AddOnScrollChangedListener(mScrollChangedListener);
}

void SurfaceView::OnWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    View::OnWindowVisibilityChanged(visibility);
    mWindowVisibility = visibility == VISIBLE;
    mRequestedVisible = mWindowVisibility && mViewVisibility;
    UpdateWindow(FALSE, FALSE);
}

ECode SurfaceView::SetVisibility(
    /* [in] */ Int32 visibility)
{
    View::SetVisibility(visibility);
    mViewVisibility = visibility == VISIBLE;
    mRequestedVisible = mWindowVisibility && mViewVisibility;
    UpdateWindow(FALSE, FALSE);

    return NOERROR;
}

/**
 * This method is not intended for general use. It was created
 * temporarily to improve performance of 3D layers in Launcher
 * and should be removed and fixed properly.
 *
 * Do not call this method. Ever.
 *
 * @hide
 */
void SurfaceView::ShowSurface()
{
    if (mSession != NULL) {
        UpdateWindow(TRUE, FALSE);
    }
}

/**
 * This method is not intended for general use. It was created
 * temporarily to improve performance of 3D layers in Launcher
 * and should be removed and fixed properly.
 *
 * Do not call this method. Ever.
 *
 * @hide
 */
void SurfaceView::HideSurface()
{
    if (mSession != NULL && mWindow != NULL) {
        {
            Mutex::Autolock lock(mSurfaceLock);
            AutoPtr<IDisplayMetrics> metrics;
            GetResources()->GetDisplayMetrics((IDisplayMetrics**)&metrics);
            ((CWindowManagerLayoutParams*)mLayout.Get())->mX = ((CDisplayMetrics*)metrics.Get())->mWidthPixels * 3;

            AutoPtr<IRect> outFrame, outContentInsets, outVisibleInsets;
            AutoPtr<IConfiguration> outConfig;
            Int32 result = 0;
            AutoPtr<ISurface> outSurface;

            mSession->Relayout(mWindow, mLayout, mWidth, mHeight, VISIBLE, FALSE,
                    mWinFrame, mContentInsets, mVisibleInsets, mConfiguration, mSurface,
                    (IRect**) &outFrame, (IRect**) &outContentInsets,
                    (IRect**) &outVisibleInsets, (IConfiguration**) &outConfig,
                    &result, (ISurface**)&outSurface);
        }
    }
}

void SurfaceView::OnDetachedFromWindow()
{
    GetViewTreeObserver()->RemoveOnScrollChangedListener(mScrollChangedListener);
    mRequestedVisible = FALSE;
    UpdateWindow(FALSE, FALSE);
    mHaveFrame = FALSE;
    if (mWindow != NULL) {
        mSession->Remove(mWindow);
        mWindow = NULL;
    }

    mSession = NULL;
    ((CWindowManagerLayoutParams*)mLayout.Get())->mToken = NULL;

    View::OnDetachedFromWindow();
}

void SurfaceView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 width = GetDefaultSize(mRequestedWidth, widthMeasureSpec);
    Int32 height = GetDefaultSize(mRequestedHeight, heightMeasureSpec);
    SetMeasuredDimension(width, height);
}

/** @hide */
Boolean SurfaceView::SetFrame(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    Boolean result = View::SetFrame(left, top, right, bottom);
    UpdateWindow(FALSE, FALSE);
    return result;
}

Boolean SurfaceView::GatherTransparentRegion(
    /* [in] */ IRegion* region)
{
    if (mWindowType == WindowManagerLayoutParams_TYPE_APPLICATION_PANEL) {
        return View::GatherTransparentRegion(region);
    }

    Boolean opaque = TRUE;
    if ((mPrivateFlags & SKIP_DRAW) == 0) {
        // this view draws, remove it from the transparent region
        opaque = View::GatherTransparentRegion(region);
    } else if (region != NULL) {
        Int32 w = GetWidth();
        Int32 h = GetHeight();
        if (w>0 && h>0) {
            GetLocationInWindow(&(*mLocation)[0], &(*mLocation)[1]);
            // otherwise, punch a hole in the whole hierarchy
            Int32 l = (*mLocation)[0];
            Int32 t = (*mLocation)[1];

            Boolean res;
            region->OpEx(l, t, l+w, t+h, RegionOp_UNION, &res);
        }
    }
    if (ElPixelFormat::FormatHasAlpha(mRequestedFormat)) {
        opaque = FALSE;
    }
    return opaque;
}

ECode SurfaceView::Draw(
    /* [in] */ ICanvas* canvas)
{
    if (mWindowType != WindowManagerLayoutParams_TYPE_APPLICATION_PANEL) {
        // draw() is not called when SKIP_DRAW is set
        if ((mPrivateFlags & SKIP_DRAW) == 0) {
            // punch a whole in the view-hierarchy below us
            canvas->DrawColorEx(0, PorterDuffMode_CLEAR);
        }
    }
    return View::Draw(canvas);
}

void SurfaceView::DispatchDraw(
    /* [in] */ ICanvas* canvas)
{
    if (mWindowType != WindowManagerLayoutParams_TYPE_APPLICATION_PANEL) {
        // if SKIP_DRAW is cleared, draw() has already punched a hole
        if ((mPrivateFlags & SKIP_DRAW) == SKIP_DRAW) {
            // punch a whole in the view-hierarchy below us
            canvas->DrawColorEx(0, PorterDuffMode_CLEAR);
        }
    }
    // reposition ourselves where the surface is
    mHaveFrame = TRUE;
    UpdateWindow(FALSE, FALSE);
    View::DispatchDraw(canvas);
}

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
ECode SurfaceView::SetZOrderMediaOverlay(
    /* [in] */ Boolean isMediaOverlay)
{
    mWindowType = isMediaOverlay
            ? WindowManagerLayoutParams_TYPE_APPLICATION_MEDIA_OVERLAY
            : WindowManagerLayoutParams_TYPE_APPLICATION_MEDIA;

    return NOERROR;
}

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
ECode SurfaceView::SetZOrderOnTop(
    /* [in] */ Boolean onTop)
{
    if (onTop) {
        mWindowType = WindowManagerLayoutParams_TYPE_APPLICATION_PANEL;
        // ensures the surface is placed below the IME
        ((CWindowManagerLayoutParams*)mLayout.Get())->mFlags |= WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM;
    } else {
        mWindowType = WindowManagerLayoutParams_TYPE_APPLICATION_MEDIA;
        ((CWindowManagerLayoutParams*)mLayout.Get())->mFlags &= ~WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM;
    }

    return NOERROR;
}

/**
 * Hack to allow special layering of windows.  The type is one of the
 * types in WindowManager.LayoutParams.  This is a hack so:
 * @hide
 */
ECode SurfaceView::SetWindowType(
    /* [in] */ Int32 type)
{
    mWindowType = type;

    return NOERROR;
}

void SurfaceView::UpdateWindow(
    /* [in] */ Boolean force,
    /* [in] */ Boolean redrawNeeded)
{
    if (!mHaveFrame) {
        return;
    }

    AutoPtr<ViewRoot> viewRoot;
    GetRootView()->GetParent((IViewParent**)&viewRoot);
    if (viewRoot != NULL) {
        mTranslator = viewRoot->mTranslator;
    }

    AutoPtr<IResources> res;
    GetContext()->GetResources((IResources**)&res);

    Boolean isSupportScreen;
    AutoPtr<ICompatibilityInfo> info;
    res->GetCompatibilityInfo((ICompatibilityInfo**)&info);
    info->SupportsScreen(&isSupportScreen);
    if (mTranslator != NULL || !isSupportScreen) {
        AutoPtr<IDisplayMetrics> dm;
        res->GetDisplayMetrics((IDisplayMetrics**)&dm);
        mSurface->SetCompatibleDisplayMetrics(dm, mTranslator);
    }

    Int32 myWidth = mRequestedWidth;
    if (myWidth <= 0) myWidth = GetWidth();
    Int32 myHeight = mRequestedHeight;
    if (myHeight <= 0) myHeight = GetHeight();

    GetLocationInWindow(&(*mLocation)[0], &(*mLocation)[1]);
    Boolean creating = mWindow == NULL;
    Boolean formatChanged = mFormat != mRequestedFormat;
    Boolean sizeChanged = mWidth != myWidth || mHeight != myHeight;
    Boolean visibleChanged = mVisible != mRequestedVisible
      || mNewSurfaceNeeded;
    Boolean typeChanged = mType != mRequestedType;
    if (force || creating || formatChanged || sizeChanged || visibleChanged
        || typeChanged || mLeft != (*mLocation)[0] || mTop != (*mLocation)[1]
        || mUpdateWindowNeeded || mReportDrawNeeded || redrawNeeded) {

        /*if (localLOGV) Log.i(TAG, "Changes: creating=" + creating
                + " format=" + formatChanged + " size=" + sizeChanged
                + " visible=" + visibleChanged
                + " left=" + (mLeft != mLocation[0])
                + " top=" + (mTop != mLocation[1]));*/

        //try {
            Boolean visible = mVisible = mRequestedVisible;
            mLeft = (*mLocation)[0];
            mTop = (*mLocation)[1];
            mWidth = myWidth;
            mHeight = myHeight;
            mFormat = mRequestedFormat;
            mType = mRequestedType;

            // Scaling/Translate window's layout here because mLayout is not used elsewhere.

            // Places the window relative
            ((CWindowManagerLayoutParams*)mLayout.Get())->mX = mLeft;
            ((CWindowManagerLayoutParams*)mLayout.Get())->mY = mTop;
            ((CWindowManagerLayoutParams*)mLayout.Get())->mWidth = GetWidth();
            ((CWindowManagerLayoutParams*)mLayout.Get())->mHeight = GetHeight();
            if (mTranslator != NULL) {
                //TODO
                //mTranslator->TranslateLayoutParamsInAppWindowToScreen(mLayout);
            }

            ((CWindowManagerLayoutParams*)mLayout.Get())->mFormat = mRequestedFormat;
            ((CWindowManagerLayoutParams*)mLayout.Get())->mFlags |=WindowManagerLayoutParams_FLAG_NOT_TOUCHABLE
                          | WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE
                          | WindowManagerLayoutParams_FLAG_LAYOUT_NO_LIMITS
                          | WindowManagerLayoutParams_FLAG_SCALED
                          | WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE
                          | WindowManagerLayoutParams_FLAG_NOT_TOUCHABLE
                          ;

            //TODO
            //if (!GetContext()->GetResources()->GetCompatibilityInfo()->SupportsScreen()) {
                ((CWindowManagerLayoutParams*)mLayout.Get())->mFlags |= WindowManagerLayoutParams_FLAG_COMPATIBLE_WINDOW;
            //}

            ((CWindowManagerLayoutParams*)mLayout.Get())->mMemoryType = mRequestedType;

            if (mWindow == NULL) {
                mWindow = new MyWindow((ISurfaceView*)this->Probe(EIID_ISurfaceView));
                ((CWindowManagerLayoutParams*)mLayout.Get())->mType = mWindowType;
                ((CWindowManagerLayoutParams*)mLayout.Get())->mGravity = Gravity_LEFT|Gravity_TOP;

                Int32 result = 0;
                mSession->AddWithoutInputChannel(mWindow, mLayout,
                        mVisible ? VISIBLE : GONE, (IRect**)&mContentInsets, &result);
            }

            if (visibleChanged && (!visible || mNewSurfaceNeeded)) {
                ReportSurfaceDestroyed();
            }

            mNewSurfaceNeeded = FALSE;

            Boolean realSizeChanged;
            Boolean reportDrawNeeded;

            {
                Mutex::Autolock lock(mSurfaceLock);
                mUpdateWindowNeeded = FALSE;
                reportDrawNeeded = mReportDrawNeeded;
                mReportDrawNeeded = FALSE;
                mDrawingStopped = !visible;

                Int32 relayoutResult = 0;

                AutoPtr<IRect> outFrame, outContentInsets, outVisibleInsets;
                AutoPtr<IConfiguration> outConfig;
                AutoPtr<ISurface> outSurface;

                mSession->Relayout(mWindow, mLayout, mWidth, mHeight, visible ? VISIBLE : GONE, FALSE,
                        mWinFrame, mContentInsets, mVisibleInsets, mConfiguration, mSurface,
                        (IRect**) &outFrame, (IRect**) &outContentInsets,
                        (IRect**) &outVisibleInsets, (IConfiguration**) &outConfig,
                        &relayoutResult, (ISurface**)&outSurface);

                if ((relayoutResult & WindowManagerImpl_RELAYOUT_FIRST_TIME) != 0) {
                    mReportDrawNeeded = TRUE;
                }

                /*if (localLOGV) Log.i(TAG, "New surface: " + mSurface
                        + ", vis=" + visible + ", frame=" + mWinFrame);*/

                mSurfaceFrame->mLeft = 0;
                mSurfaceFrame->mTop = 0;
                if (mTranslator == NULL) {
                    mWinFrame->GetWidth(&mSurfaceFrame->mRight);
                    mWinFrame->GetHeight(&mSurfaceFrame->mBottom);
                } else {
                    //TODO
                    // Float appInvertedScale = ((CTranslator*)mTranslator.Get())->ApplicationInvertedScale;
                    // mSurfaceFrame->mRight = (Int32) (mWinFrame.width() * appInvertedScale + 0.5f);
                    // mSurfaceFrame->mBottom = (Int32) (mWinFrame.height() * appInvertedScale + 0.5f);
                }

                Int32 surfaceWidth = mSurfaceFrame->mRight;
                Int32 surfaceHeight = mSurfaceFrame->mBottom;
                realSizeChanged = mLastSurfaceWidth != surfaceWidth
                        || mLastSurfaceHeight != surfaceHeight;
                mLastSurfaceWidth = surfaceWidth;
                mLastSurfaceHeight = surfaceHeight;
            }

            //try {
                redrawNeeded |= creating | reportDrawNeeded;

                if (visible) {
                    mDestroyReportNeeded = TRUE;

                    if (visibleChanged) {
                        assert(mCallbacks != NULL);
                        mIsCreating = TRUE;

                        AutoPtr<ISurfaceHolderCallback> c;
                        for (Int32 i = 0; i < mCallbacks->GetLength(); ++i)
                        {
                            c = (*mCallbacks)[i];
                            assert(c != NULL);
                            c->SurfaceCreated(mSurfaceHolder.Get());
                        }
                    }

                    if (creating || formatChanged || sizeChanged
                            || visibleChanged || realSizeChanged) {
                        assert(mCallbacks != NULL);
                        AutoPtr<ISurfaceHolderCallback> c;
                        for (Int32 i = 0; i < mCallbacks->GetLength(); ++i)
                        {
                            c = (*mCallbacks)[i];
                            assert(c != NULL);
                            c->SurfaceChanged(mSurfaceHolder.Get(), mFormat, myWidth, myHeight);
                        }
                    }

                    if (redrawNeeded) {
                        assert(mCallbacks != NULL);
                        AutoPtr<ISurfaceHolderCallback> c;
                        for (Int32 i = 0; i < mCallbacks->GetLength(); ++i)
                        {
                            c = (*mCallbacks)[i];
                            if (ISurfaceHolderCallback2::Probe(c) != NULL) {
                                ISurfaceHolderCallback2::Probe(c)->SurfaceRedrawNeeded(
                                        mSurfaceHolder);
                            }
                        }
                    }
                } else {
                    //TODO
                    //mSurface.release();
                }

                mIsCreating = FALSE;
                if (redrawNeeded) {
                    mSession->FinishDrawing(mWindow);
                }
            //} finally {
                /*mIsCreating = FALSE;
                if (redrawNeeded) {
                    mSession->FinishDrawing(mWindow);
                }*/
            //}
        //} catch (RemoteException ex) {
        //}
        /*if (localLOGV) Log.v(
            TAG, "Layout: x=" + mLayout.x + " y=" + mLayout.y +
            " w=" + mLayout.width + " h=" + mLayout.height +
            ", frame=" + mSurfaceFrame);*/
    }
}

void SurfaceView::ReportSurfaceDestroyed()
{
    if (mDestroyReportNeeded) {
        mDestroyReportNeeded = FALSE;

        assert(mCallbacks != NULL);
        AutoPtr<ISurfaceHolderCallback> c;
        for (Int32 i = 0; i < mCallbacks->GetLength(); ++i)
        {
            c = (*mCallbacks)[i];
            assert(c != NULL);
            c->SurfaceDestroyed(mSurfaceHolder);
        }
    }

    View::OnDetachedFromWindow();
}

void SurfaceView::HandleGetNewSurface()
{
    mNewSurfaceNeeded = TRUE;
    UpdateWindow(FALSE, FALSE);
}

/**
 * Check to see if the surface has fixed size dimensions or if the surface's
 * dimensions are dimensions are dependent on its current layout.
 *
 * @return TRUE if the surface has dimensions that are fixed in size
 * @hide
 */
Boolean SurfaceView::IsFixedSize()
{
    return (mRequestedWidth != -1 || mRequestedHeight != -1);
}

SurfaceView::MyWindow::MyWindow(
    /* [in] */ ISurfaceView* surfaceView):
    mSurfaceView(surfaceView),
    mCurWidth(-1),
    mCurHeight(-1)
{
    //mSurfaceView = new WeakReference<SurfaceView>(surfaceView);
}

ECode SurfaceView::MyWindow::Resized(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ IRect* coveredInsets,
    /* [in] */ IRect* visibleInsets,
    /* [in] */ Boolean reportDraw,
    /* [in] */ IConfiguration* newConfig)
{
    AutoPtr<ISurfaceView> surfaceView = mSurfaceView;
    if (surfaceView != NULL) {
        /*if (localLOGV) Log.v(
                "SurfaceView", surfaceView + " got resized: w=" +
                        w + " h=" + h + ", cur w=" + mCurWidth + " h=" + mCurHeight);*/
        Mutex::Autolock lock(((CSurfaceView*)surfaceView.Get())->mSurfaceLock);

        Int32 width, height;
        if (reportDraw) {
            ((CSurfaceView*)surfaceView.Get())->mUpdateWindowNeeded = TRUE;
            ((CSurfaceView*)surfaceView.Get())->mReportDrawNeeded = TRUE;

            void (STDCALL SurfaceView::*pHandlerFunc)(Boolean, Boolean);
            pHandlerFunc = &SurfaceView::UpdateWindow;
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteBoolean(FALSE);
            params->WriteBoolean(FALSE);

            ((CSurfaceView*)surfaceView.Get())->mHandler->PostCppCallback(
                    (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);

        } else if ((((CSurfaceView*)surfaceView.Get())->mWinFrame->GetWidth(&width), width) != w
                || (((CSurfaceView*)surfaceView.Get())->mWinFrame->GetHeight(&height), height) != h) {
            ((CSurfaceView*)surfaceView.Get())->mUpdateWindowNeeded = TRUE;

            void (STDCALL SurfaceView::*pHandlerFunc)(Boolean, Boolean);
            pHandlerFunc = &SurfaceView::UpdateWindow;
            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteBoolean(FALSE);
            params->WriteBoolean(FALSE);

            ((CSurfaceView*)surfaceView.Get())->mHandler->PostCppCallback(
                    (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
        }
    }

    return NOERROR;
}

ECode SurfaceView::MyWindow::DispatchAppVisibility(
    /* [in] */ Boolean visible)
{
    // The point of SurfaceView is to let the app control the surface.
    return NOERROR;
}

ECode SurfaceView::MyWindow::DispatchGetNewSurface()
{
    if (mSurfaceView != NULL) {
        void (STDCALL SurfaceView::*pHandlerFunc)();
        pHandlerFunc = &SurfaceView::HandleGetNewSurface;

        ((CSurfaceView*)mSurfaceView.Get())->mHandler->PostCppCallback(
                (Handle32)this, *(Handle32*)&pHandlerFunc, NULL, 0);
    }

    return NOERROR;
}

ECode SurfaceView::MyWindow::WindowFocusChanged(
    /* [in] */ Boolean hasFocus,
    /* [in] */ Boolean touchEnabled)
{
    //Log.w("SurfaceView", "Unexpected focus in surface: focus=" + hasFocus + ", touchEnabled=" + touchEnabled);
    return NOERROR;
}

ECode SurfaceView::MyWindow::ExecuteCommand(
    /* [in] */ const String& command,
    /* [in] */ const String& parameters,
    /* [in] */ IParcelFileDescriptor* out)
{
    return NOERROR;
}

ECode SurfaceView::MyWindow::CloseSystemDialogs(
    /* [in] */ const String& reason)
{
    return NOERROR;
}

ECode SurfaceView::MyWindow::DispatchWallpaperOffsets(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float xStep,
    /* [in] */ Float yStep,
    /* [in] */ Boolean sync)
{
    return NOERROR;
}

ECode SurfaceView::MyWindow::DispatchWallpaperCommand(
    /* [in] */ const String& action,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean sync)
{
    return NOERROR;
}

ECode SurfaceView::MyWindow::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

PInterface SurfaceView::MyWindow::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }

    return NULL;
}

UInt32 SurfaceView::MyWindow::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 SurfaceView::MyWindow::Release()
{
    return ElRefBase::Release();
}

ECode SurfaceView::MyWindow::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    return NOERROR;
}
