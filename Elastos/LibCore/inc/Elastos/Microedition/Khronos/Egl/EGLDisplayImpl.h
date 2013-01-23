
#ifndef __EGLDISPLAYIMPL_H__
#define __EGLDISPLAYIMPL_H__

#include "_EGLDisplay.h"
#include "Elastos.Microedition.Khronos.Egl_server.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <EGL/egl.h>

class EGLDisplayImpl
    : public ElRefBase
    , public _EGLDisplay
    , public IEGLDisplayImpl
{
public:
    EGLDisplayImpl(
        /* [in] */ EGLDisplay dpy);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI_(EGLDisplay) Get();

private:
    EGLDisplay mEGLDisplay;
};

#endif //__EGLDISPLAYIMPL_H__
