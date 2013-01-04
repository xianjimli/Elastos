
#include "view/CSurfaceViewWindow.h"
#include "view/CSurfaceView.h"

CSurfaceViewWindow::CSurfaceViewWindow():
    mCurWidth(-1),
    mCurHeight(-1)
{

}

ECode CSurfaceViewWindow::constructor(
    /* [in] */ ISurfaceView* surfaceView)

{
	mSurfaceView = surfaceView;
    //mSurfaceView = new WeakReference<SurfaceView>(surfaceView);
    return NOERROR;
}

ECode CSurfaceViewWindow::Resized(
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

        //TODO
        //Mutex::Autolock lock(((CSurfaceView*)surfaceView.Get())->mSurfaceLock);
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
                    (Handle32)mSurfaceView.Get(), *(Handle32*)&pHandlerFunc, params, 0);

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
                    (Handle32)mSurfaceView.Get(), *(Handle32*)&pHandlerFunc, params, 0);
        }
    }

    return NOERROR;
}

ECode CSurfaceViewWindow::DispatchAppVisibility(
    /* [in] */ Boolean visible)
{
    // The point of SurfaceView is to let the app control the surface.
    return NOERROR;
}

ECode CSurfaceViewWindow::DispatchGetNewSurface()
{
    if (mSurfaceView != NULL) {
        void (STDCALL SurfaceView::*pHandlerFunc)();
        pHandlerFunc = &SurfaceView::HandleGetNewSurface;

        ((CSurfaceView*)mSurfaceView.Get())->mHandler->PostCppCallback(
                (Handle32)mSurfaceView.Get(), *(Handle32*)&pHandlerFunc, NULL, 0);
    }

    return NOERROR;
}

ECode CSurfaceViewWindow::WindowFocusChanged(
    /* [in] */ Boolean hasFocus,
    /* [in] */ Boolean touchEnabled)
{
    //Log.w("SurfaceView", "Unexpected focus in surface: focus=" + hasFocus + ", touchEnabled=" + touchEnabled);
    return NOERROR;
}

ECode CSurfaceViewWindow::ExecuteCommand(
    /* [in] */ const String& command,
    /* [in] */ const String& parameters,
    /* [in] */ IParcelFileDescriptor* out)
{
    return NOERROR;
}

ECode CSurfaceViewWindow::CloseSystemDialogs(
    /* [in] */ const String& reason)
{
    return NOERROR;
}

ECode CSurfaceViewWindow::DispatchWallpaperOffsets(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float xStep,
    /* [in] */ Float yStep,
    /* [in] */ Boolean sync)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSurfaceViewWindow::DispatchWallpaperCommand(
    /* [in] */ const String& action,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean sync)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSurfaceViewWindow::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}
