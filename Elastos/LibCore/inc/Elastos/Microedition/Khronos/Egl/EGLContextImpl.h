
#ifndef __EGLCONTEXTIMPL_H__
#define __EGLCONTEXTIMPL_H__

#include "_EGLContext.h"
#include "Elastos.Microedition.Khronos.Egl_server.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <EGL/egl.h>

class EGLContextImpl
    : public ElRefBase
    , public _EGLContext
    , public IEGLContext
{
public:
    EGLContextImpl(
        /* [in] */ EGLContext ctx);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    //@Override
    // public GL getGL() {
    //     return mGLContext;
    // }

    CARAPI_(EGLContext) Get();

private:
    EGLContext mEGLContext;
    // private GLImpl mGLContext;
};

#endif //__EGLCONTEXTIMPL_H__
