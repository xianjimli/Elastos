
#include "EGLDisplayImpl.h"

EGLDisplayImpl::EGLDisplayImpl(
    /* [in] */ EGLDisplay dpy)
    : mEGLDisplay(dpy)
{
}

EGLDisplay EGLDisplayImpl::Get()
{
    return mEGLDisplay;
}
