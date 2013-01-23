
#include "EGLSurfaceImpl.h"

EGLSurfaceImpl::EGLSurfaceImpl()
    : mEGLSurface(NULL)
    , mNativePixelRef(NULL)
{}

EGLSurfaceImpl::EGLSurfaceImpl(
    /* [in] */ EGLSurface surface)
    : mEGLSurface(surface)
    , mNativePixelRef(NULL)
{}

EGLSurface EGLSurfaceImpl::Get()
{
    return mEGLSurface;
}

SkPixelRef* EGLSurfaceImpl::GetNativePixelRef()
{
    return mNativePixelRef;
}
