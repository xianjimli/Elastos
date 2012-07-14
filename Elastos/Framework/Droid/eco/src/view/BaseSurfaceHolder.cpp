
#include "view/BaseSurfaceHolder.h"
#include "graphics/ElPixelFormat.h"
#include "view/CSurface.h"
#include "os/SystemClock.h"
#include <Logger.h>
#include <StringBuffer.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

const Boolean BaseSurfaceHolder::DEBUG;
const char* BaseSurfaceHolder::TAG = "BaseSurfaceHolder";

BaseSurfaceHolder::BaseSurfaceHolder() :
    mHaveGottenCallbacks(FALSE),
    mRequestedWidth(-1),
    mRequestedHeight(-1),
    mRequestedFormat(ElPixelFormat::OPAQUE),
    mRequestedType(-1),
    mLastLockTime(0),
    mType(-1)
{
    assert(SUCCEEDED(CSurface::New((ISurface**)&mSurface)));
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&mSurfaceFrame)));
}

PInterface BaseSurfaceHolder::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IRunnable*)this;
    }
    else if (riid == EIID_ISurfaceHolder) {
        return (ISurfaceHolder*)this;
    }
    return NULL;
}

UInt32 BaseSurfaceHolder::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 BaseSurfaceHolder::Release()
{
    return ElRefBase::Release();
}

ECode BaseSurfaceHolder::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}


Int32 BaseSurfaceHolder::GetRequestedWidth()
{
    return mRequestedWidth;
}

Int32 BaseSurfaceHolder::GetRequestedHeight()
{
    return mRequestedHeight;
}

Int32 BaseSurfaceHolder::GetRequestedFormat()
{
    return mRequestedFormat;
}

Int32 BaseSurfaceHolder::GetRequestedType()
{
    return mRequestedType;
}

ECode BaseSurfaceHolder::AddCallback(
    /* [in] */ ISurfaceHolderCallback* cback)
{
    Mutex::Autolock lock(mCallbackLock);

    // This is a linear search, but in practice we'll
    // have only a couple callbacks, so it doesn't matter.
    //
    List<AutoPtr<ISurfaceHolderCallback> >::Iterator iter = mCallbacks.Begin();
    for (; iter != mCallbacks.End(); ++iter) {
        if (iter->Get() == cback) {
            return NOERROR;
        }
    }

    mCallbacks.PushBack(cback);

    return NOERROR;
}

ECode BaseSurfaceHolder::RemoveCallback(
    /* [in] */ ISurfaceHolderCallback* cback)
{
    Mutex::Autolock lock(mCallbackLock);

    List<AutoPtr<ISurfaceHolderCallback> >::Iterator iter = mCallbacks.Begin();
    for (; iter != mCallbacks.End(); ++iter) {
        if (iter->Get() == cback) {
            mCallbacks.Erase(iter);
            break;
        }
    }

    return NOERROR;
}

Vector<AutoPtr<ISurfaceHolderCallback> >& BaseSurfaceHolder::GetCallbacks()
{
    if (mHaveGottenCallbacks) {
        return mGottenCallbacks;
    }

    Mutex::Autolock lock(mCallbackLock);

    mGottenCallbacks.Clear();
    if (mCallbacks.GetSize() > 0) {
        //List<AutoPtr<ISurfaceHolderCallback> >::Iterator iter = mCallbacks.Begin();
        //for (; iter!=mCallbacks.End(); ++iter) {
        //    mGottenCallbacks.PushBack(*iter);
        //}
        mGottenCallbacks.Assign<List<AutoPtr<ISurfaceHolderCallback> >::Iterator>(
            mCallbacks.Begin(), mCallbacks.End());
    }
    mHaveGottenCallbacks = TRUE;

    return mGottenCallbacks;
}

void BaseSurfaceHolder::UngetCallbacks()
{
    mHaveGottenCallbacks = FALSE;
}

ECode BaseSurfaceHolder::SetFixedSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (mRequestedWidth != width || mRequestedHeight != height) {
        mRequestedWidth = width;
        mRequestedHeight = height;
        OnRelayoutContainer();
    }

    return NOERROR;
}

ECode BaseSurfaceHolder::SetSizeFromLayout()
{
    if (mRequestedWidth != -1 || mRequestedHeight != -1) {
        mRequestedWidth = mRequestedHeight = -1;
        OnRelayoutContainer();
    }

    return NOERROR;
}

ECode BaseSurfaceHolder::SetFormat(
    /* [in] */ Int32 format)
{
    if (mRequestedFormat != format) {
        mRequestedFormat = format;
        OnUpdateSurface();
    }

    return NOERROR;
}

ECode BaseSurfaceHolder::SetType(
    /* [in] */ Int32 type)
{
    switch (type) {
        case SurfaceHolder_SURFACE_TYPE_HARDWARE:
        case SurfaceHolder_SURFACE_TYPE_GPU:
            // these are deprecated, treat as "NORMAL"
            type = SurfaceHolder_SURFACE_TYPE_NORMAL;
            break;
        default:
            break;
    }

    switch (type) {
        case SurfaceHolder_SURFACE_TYPE_NORMAL:
        case SurfaceHolder_SURFACE_TYPE_PUSH_BUFFERS:
            if (mRequestedType != type) {
                mRequestedType = type;
                OnUpdateSurface();
            }
            break;
        default:
            break;
    }

    return NOERROR;
}

ECode BaseSurfaceHolder::LockCanvas(
    /* [out] */ ICanvas** canvas)
{
    if (canvas == NULL) {
        return E_INVALID_ARGUMENT;
    }

    InternalLockCanvas(NULL, canvas);

    return NOERROR;
}

ECode BaseSurfaceHolder::LockCanvasEx(
    /* [in] */ IRect* dirty,
    /* [out] */ ICanvas** canvas)
{
    if (canvas == NULL) {
        return E_INVALID_ARGUMENT;
    }

    InternalLockCanvas(dirty, canvas);

    return NOERROR;
}

ECode BaseSurfaceHolder::UnlockCanvasAndPost(
    /* [in] */ ICanvas* canvas)
{
    mSurface->UnlockCanvasAndPost(canvas);
    mSurfaceLock.Unlock();

    return NOERROR;
}

ECode BaseSurfaceHolder::GetSurface(
    /* [out] */ ISurface** surface)
{
    if (surface == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *surface = mSurface;
    (*surface)->AddRef();

    return NOERROR;
}

ECode BaseSurfaceHolder::GetSurfaceFrame(
    /* [out] */ IRect** rect)
{
    if (rect == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *rect = mSurfaceFrame;
    (*rect)->AddRef();

    return NOERROR;
}

ECode BaseSurfaceHolder::InternalLockCanvas(
    /* [in] */ IRect* dirty,
    /* [out] */ ICanvas** canvas)
{
    if (mType == SurfaceHolder_SURFACE_TYPE_PUSH_BUFFERS) {
        //throw new BadSurfaceTypeException(
        //    "Surface type is SURFACE_TYPE_PUSH_BUFFERS");
        Logger::E(TAG, "Surface type is SURFACE_TYPE_PUSH_BUFFERS");
        return E_BAD_SURFACE_TYPE_EXCEPTION;
    }

    mSurfaceLock.Lock();

    if (DEBUG) {
        Logger::D(TAG, "Locking canvas..,");
    }

    if (OnAllowLockCanvas()) {
        IRect* frame = (dirty!=NULL) ? dirty : mSurfaceFrame.Get();
        ECode ec = mSurface->LockCanvas(frame, canvas);
        if (FAILED(ec)) {
            Logger::E(TAG, "Exception locking surface");
        }
    }

    if (DEBUG) {
        Logger::D(TAG, StringBuffer("Returned canvas: ") + Int32(*canvas));
    }

    if (*canvas != NULL) {
        mLastLockTime = SystemClock::GetUptimeMillis();
        return NOERROR;
    }

    // If the Surface is not ready to be drawn, then return NULL,
    // but throttle calls to this function so it isn't called more
    // than every 100ms.
    Int64 now = SystemClock::GetUptimeMillis();
    Int64 nextTime = mLastLockTime + 100;
    if (nextTime > now) {
        //try {
        //    Thread.sleep(nextTime-now);
        //} catch (InterruptedException e) {
        //}
        usleep(nextTime - now);
        now = SystemClock::GetUptimeMillis();
    }
    mLastLockTime = now;
    mSurfaceLock.Unlock();

    return NOERROR;
}
