
#ifndef ___EGLCONTEXT_H__
#define ___EGLCONTEXT_H__

#include "Elastos.Microedition.Khronos.Egl_server.h"
#include <elastos/AutoPtr.h>

class _EGLContext
{
public:
    static CARAPI_(AutoPtr<IEGL>) GetEGL();

    // public abstract GL getGL();

private:
    static const AutoPtr<IEGL> EGL_INSTANCE;
};

#endif //___EGLCONTEXT_H__
