
#include "_EGLContext.h"
#include "EGLImpl.h"

const AutoPtr<IEGL> _EGLContext::EGL_INSTANCE = new EGLImpl();

AutoPtr<IEGL> _EGLContext::GetEGL()
{
    return EGL_INSTANCE;
}
