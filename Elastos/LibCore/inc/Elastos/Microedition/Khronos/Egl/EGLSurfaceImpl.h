
#ifndef __EGLSURFACEIMPL_H__
#define __EGLSURFACEIMPL_H__

#include "_EGLSurface.h"
#include "Elastos.Microedition.Khronos.Egl_server.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <EGL/egl.h>
#include <skia/core/SkPixelRef.h>

class EGLSurfaceImpl
    : public ElRefBase
    , public _EGLSurface
    , public IEGLSurface
{
public:
    EGLSurfaceImpl();

    EGLSurfaceImpl(
        /* [in] */ EGLSurface surface);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI_(EGLSurface) Get();

    CARAPI_(SkPixelRef*) GetNativePixelRef();

private:
    EGLSurface mEGLSurface;
    SkPixelRef* mNativePixelRef;
};

#endif //__EGLSURFACEIMPL_H__

