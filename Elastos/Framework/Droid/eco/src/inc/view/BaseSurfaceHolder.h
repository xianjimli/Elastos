
#ifndef __BASESURFACEHOLDER_H__
#define __BASESURFACEHOLDER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include "graphics/CRect.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/Vector.h>

class BaseSurfaceHolder : public ElRefBase, public ISurfaceHolder
{
public:
    BaseSurfaceHolder();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    virtual CARAPI OnUpdateSurface() = 0;

    virtual CARAPI_(void) OnRelayoutContainer() = 0;

    virtual CARAPI_(Boolean) OnAllowLockCanvas() = 0;

    CARAPI_(Int32) GetRequestedWidth();

    CARAPI_(Int32) GetRequestedHeight();

    CARAPI_(Int32) GetRequestedFormat();

    CARAPI_(Int32) GetRequestedType();

    virtual CARAPI AddCallback(
        /* [in] */ ISurfaceHolderCallback* cback);

    virtual CARAPI RemoveCallback(
        /* [in] */ ISurfaceHolderCallback* cback);

    CARAPI_(Vector<AutoPtr<ISurfaceHolderCallback> >&) GetCallbacks();

    CARAPI_(void) UngetCallbacks();

    virtual CARAPI SetFixedSize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    virtual CARAPI SetSizeFromLayout();

    virtual CARAPI SetFormat(
        /* [in] */ Int32 format);

    virtual CARAPI SetType(
        /* [in] */ Int32 type);

    virtual CARAPI LockCanvas(
        /* [out] */ ICanvas** canvas);

    virtual CARAPI LockCanvasEx(
        /* [in] */ IRect* dirty,
        /* [out] */ ICanvas** canvas);

    virtual CARAPI UnlockCanvasAndPost(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI GetSurface(
        /* [out] */ ISurface** surface);

    virtual CARAPI GetSurfaceFrame(
        /* [out] */ IRect** rect);

private:
    CARAPI InternalLockCanvas(
        /* [in] */ IRect* dirty,
        /* [out] */ ICanvas** canvas);

public:
    List<AutoPtr<ISurfaceHolderCallback> > mCallbacks;
    //public final ReentrantLock mSurfaceLock = new ReentrantLock();
    Mutex mSurfaceLock;
    AutoPtr<ISurface> mSurface;

protected:
    static const Boolean DEBUG = FALSE;
    Vector<AutoPtr<ISurfaceHolderCallback> > mGottenCallbacks;
    Boolean mHaveGottenCallbacks;
    Int32 mRequestedWidth;
    Int32 mRequestedHeight;
    /** @hide */
    Int32 mRequestedFormat;
    Int32 mRequestedType;
    Int64 mLastLockTime;
    Int32 mType;
    AutoPtr<CRect> mSurfaceFrame;

private:
    static const char* TAG;
    Mutex mCallbackLock;
};
#endif //__BASESURFACEHOLDER_H__
