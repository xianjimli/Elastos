
#include "server/CWindowSession.h"
#include "os/ServiceManager.h"
#include "os/Binder.h"

ECode CWindowSession::constructor(
    /* [in] */ IWindowManagerStub* wmService,
    /* [in] */ IInputMethodClient* client,
    /* [in] */ IInputContext* inputContext)
{
    mWMService = (CWindowManagerService*)wmService;
    mNumWindow = 0;
    mClient = client;
    mInputContext = inputContext;
    mUid = Binder::GetCallingUid();
    mPid = Binder::GetCallingPid();

    {
        Mutex::Autolock lock(mWMService->mWindowMapLock);
        if (mWMService->mInputMethodManager == NULL && TRUE /*mWMService->mHaveInputMethods*/) {
            mWMService->mInputMethodManager = IInputMethodManager::Probe(
                ServiceManager::GetService(String(Context_INPUT_METHOD_SERVICE)).Get());
            assert(mWMService->mInputMethodManager != NULL);
        }
    }
    Int64 ident = Binder::ClearCallingIdentity();
    ECode ec = NOERROR;
    // try {
    // Note: it is safe to call in to the input method manager
    // here because we are not holding our lock.
    if (mWMService->mInputMethodManager != NULL) {
        ec = mWMService->mInputMethodManager->AddClient(client, inputContext,
                mUid, mPid);
    }
    else {
        ec = client->SetUsingInputMethod(FALSE);
    }
    // client.asBinder().LinkToDeath(this, 0);
    // } catch (RemoteException e) {
    //     // The caller has died, so we can just forget about this.
    //     try {
    //         if (mWMService->mInputMethodManager != null) {
    //             mWMService->mInputMethodManager.removeClient(client);
    //         }
    //     } catch (RemoteException ee) {
    //     }
    // } finally {
    //     Binder.restoreCallingIdentity(ident);
    // }

    if (ec == E_REMOTE_EXCEPTION) {
        if (mWMService->mInputMethodManager != NULL) {
            mWMService->mInputMethodManager->RemoveClient(client);
        }
    }

    Binder::RestoreCallingIdentity(ident);
    return ec;
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
    /* [in, out] */ IRect* inOutContentInsets,
    /* [out] */ Int32* result)
{
    assert(inOutContentInsets != NULL);
    assert(mWMService);
    AutoPtr<IRect> tempOut;
    *result =  mWMService->AddWindow(this, window, attrs, viewVisibility,
            inOutContentInsets, NULL, (IRect**)&tempOut, NULL);

    return NOERROR;
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
    FAIL_RETURN(CRect::New(outDisplayFrame));
    mWMService->GetWindowDisplayFrame(this, window, *outDisplayFrame);

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
    return NOERROR;
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
        CSurfaceSession::New(
            (ISurfaceSession**)&mSurfaceSession);
//        if (SHOW_TRANSACTIONS) Slog.i(
//                TAG, "  NEW SURFACE SESSION " + mSurfaceSession);
        mWMService->mSessions.Insert(this);
    }
    mNumWindow++;
}
