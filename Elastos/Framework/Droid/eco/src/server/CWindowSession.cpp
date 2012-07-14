
#include "server/CWindowSession.h"

ECode CWindowSession::constructor(
    /* [in] */ IWindowManagerEx* wmService,
    /* [in] */ IInputMethodClient* client,
    /* [in] */ IInputContext* inputContext)
{
    mWMService = (CWindowManagerService*)wmService;
    mNumWindow = 0;
    mClient = client;
    mInputContext = inputContext;

    return NOERROR;
}

ECode CWindowSession::Add(
    /* [in] */ IInnerWindow* window,
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [in] */ Int32 viewVisibility,
    /* [in] */ IRect* inContentInsets,
    /* [out] */ IRect** outContentInsets,
    /* [in] */ IInputChannel* inInputChannel,
    /* [out] */ IInputChannel** outInputChannel,
    /* [out] */ Int32* result)
{
    assert(mWMService);
    *result =  mWMService->AddWindow(this, window, attrs, viewVisibility,
            inContentInsets, inInputChannel, outContentInsets, outInputChannel);

    return NOERROR;
}

ECode CWindowSession::AddWithoutInputChannel(
    /* [in] */ IInnerWindow* window,
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [in] */ Int32 viewVisibility,
    /* [out] */ IRect** outContentInsets,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWindowSession::Remove(
    /* [in] */ IInnerWindow* window)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWindowSession::Relayout(
    /* [in] */ IInnerWindow* window,
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [in] */ Int32 requestedWidth,
    /* [in] */ Int32 requestedHeight,
    /* [in] */ Int32 viewFlags,
    /* [in] */ Boolean insetsPending,
    /* [in] */ IRect* inFrame,
    /* [in] */ IRect* inContentInsets,
    /* [in] */ IRect* inVisibleInsets,
    /* [in] */ IConfiguration* inConfig,
    /* [in] */ ISurface* inSurface,
    /* [out] */ IRect** outFrame,
    /* [out] */ IRect** outContentInsets,
    /* [out] */ IRect** outVisibleInsets,
    /* [out] */ IConfiguration** outConfig,
    /* [out] */ Int32* result,
    /* [out] */ ISurface** outSurface)
{
    //Log.d(TAG, ">>>>>> ENTERED relayout from " + Binder.getCallingPid());
    assert(mWMService);
   *result = mWMService->RelayoutWindow(this, window, attrs,
            requestedWidth, requestedHeight, viewFlags, insetsPending,
            inFrame, inContentInsets, inVisibleInsets, inConfig, inSurface,
            outFrame, outContentInsets, outVisibleInsets, outConfig, outSurface);
    //Log.d(TAG, "<<<<<< EXITING relayout to " + Binder.getCallingPid());
    return NOERROR;
}

ECode CWindowSession::SetTransparentRegion(
    /* [in] */ IInnerWindow* window,
    /* [in] */ IRegion* region)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWindowSession::SetInsets(
    /* [in] */ IInnerWindow* window,
    /* [in] */ Int32 touchableInsets,
    /* [in] */ IRect* contentInsets,
    /* [in] */ IRect* visibleInsets)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWindowSession::GetDisplayFrame(
    /* [in] */ IInnerWindow* window,
    /* [out] */ IRect** outDisplayFrame)
{
    IRect* r;
    assert(SUCCEEDED(CRect::New(&r)));

    mWMService->GetWindowDisplayFrame(this, window, r);
    *outDisplayFrame = r;

    return NOERROR;
}

ECode CWindowSession::FinishDrawing(
    /* [in] */ IInnerWindow* window)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWindowSession::SetInTouchMode(
    /* [in] */ Boolean showFocus)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWindowSession::GetInTouchMode(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWindowSession::PerformHapticFeedback(
    /* [in] */ IInnerWindow* window,
    /* [in] */ Int32 effectId,
    /* [in] */ Boolean always,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWindowSession::SetWallpaperPosition(
    /* [in] */ IBinder* windowToken,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float xstep,
    /* [in] */ Float ystep)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWindowSession::WallpaperOffsetsComplete(
    /* [in] */ IBinder* window)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWindowSession::SendWallpaperCommand(
    /* [in] */ IBinder* window,
    /* [in] */ const String& action,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean sync,
    /* [out] */ IBundle** result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWindowSession::WallpaperCommandComplete(
    /* [in] */ IBinder* window,
    /* [in] */ IBundle* result)
{
    return E_NOT_IMPLEMENTED;
}

void CWindowSession::WindowAddedLocked()
{
     if (mSurfaceSession == NULL) {
//        if (localLOGV) Slog.v(
//            TAG, "First window added to " + this + ", creating SurfaceSession");
        CSurfaceSession::NewByFriend(
            (CSurfaceSession**)&mSurfaceSession);
//        if (SHOW_TRANSACTIONS) Slog.i(
//                TAG, "  NEW SURFACE SESSION " + mSurfaceSession);
        mWMService->mSessions.Insert(this);
    }
    mNumWindow++;
}
