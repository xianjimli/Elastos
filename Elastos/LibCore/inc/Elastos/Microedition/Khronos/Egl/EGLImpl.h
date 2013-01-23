
#ifndef __EGLIMPL_H__
#define __EGLIMPL_H__

#include "Elastos.Microedition.Khronos.Egl_server.h"
#include "EGLContextImpl.h"
#include "EGLDisplayImpl.h"
#include "EGLSurfaceImpl.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

class EGLImpl
    : public ElRefBase
    , public IEGL10
{
public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI EglInitialize(
        /* [in] */ IEGLDisplay* display,
        /* [in] */ ArrayOf<Int32>* major_minor,
        /* [out] */ Boolean* result);

    CARAPI EglQueryContext(
        /* [in] */ IEGLDisplay* display,
        /* [in] */ IEGLContext* context,
        /* [in] */ Int32 attribute,
        /* [in] */ ArrayOf<Int32>* value,
        /* [out] */ Boolean* result);

    CARAPI EglQuerySurface(
        /* [in] */ IEGLDisplay* display,
        /* [in] */ IEGLSurface* surface,
        /* [in] */ Int32 attribute,
        /* [in] */ ArrayOf<Int32>* value,
        /* [out] */ Boolean* result);

    CARAPI EglChooseConfig(
        /* [in] */ IEGLDisplay* display,
        /* [in] */ ArrayOf<Int32>* attrib_list,
        /* [in] */ ArrayOf<IEGLConfig*>* configs,
        /* [in] */ Int32 config_size,
        /* [in] */ ArrayOf<Int32>* num_config,
        /* [out] */ Boolean* result);

    CARAPI EglGetConfigAttrib(
        /* [in] */ IEGLDisplay* display,
        /* [in] */ IEGLConfig* config,
        /* [in] */ Int32 attribute,
        /* [in] */ ArrayOf<Int32>* value,
        /* [out] */ Boolean* result);

    CARAPI EglGetConfigs(
        /* [in] */ IEGLDisplay* display,
        /* [in] */ ArrayOf<IEGLConfig*>* configs,
        /* [in] */ Int32 config_size,
        /* [in] */ ArrayOf<Int32>* num_config,
        /* [out] */ Boolean* result);

    CARAPI EglGetError(
        /* [out] */ Int32* error);

    CARAPI EglDestroyContext(
        /* [in] */ IEGLDisplay* display,
        /* [in] */ IEGLContext* context,
        /* [out] */ Boolean* result);

    CARAPI EglDestroySurface(
        /* [in] */ IEGLDisplay* display,
        /* [in] */ IEGLSurface* surface,
        /* [out] */ Boolean* result);

    CARAPI EglMakeCurrent(
        /* [in] */ IEGLDisplay* display,
        /* [in] */ IEGLSurface* draw,
        /* [in] */ IEGLSurface* read,
        /* [in] */ IEGLContext* context,
        /* [out] */ Boolean* result);

    CARAPI EglQueryString(
        /* [in] */ IEGLDisplay* display,
        /* [in] */ Int32 name,
        /* [out] */ String* str);

    CARAPI EglSwapBuffers(
        /* [in] */ IEGLDisplay* display,
        /* [in] */ IEGLSurface* surface,
        /* [out] */ Boolean* result);

    CARAPI EglTerminate(
        /* [in] */ IEGLDisplay* display,
        /* [out] */ Boolean* result);

    CARAPI EglCopyBuffers(
        /* [in] */ IEGLDisplay* display,
        /* [in] */ IEGLSurface* surface,
        /* [in] */ IInterface* native_pixmap,
        /* [out] */ Boolean* result);

    CARAPI EglWaitGL(
        /* [out] */ Boolean* result);

    CARAPI EglWaitNative(
        /* [in] */ Int32 engine,
        /* [in] */ IInterface* bindTarget,
        /* [out] */ Boolean* result);

    CARAPI EglCreateContext(
        /* [in] */ IEGLDisplay* display,
        /* [in] */ IEGLConfig* config,
        /* [in] */ IEGLContext* share_context,
        /* [in] */ ArrayOf<Int32>* attrib_list,
        /* [out] */ IEGLContext** context);

    CARAPI EglCreatePbufferSurface(
        /* [in] */ IEGLDisplay* display,
        /* [in] */ IEGLConfig* config,
        /* [in] */ ArrayOf<Int32>* attrib_list,
        /* [out] */ IEGLSurface** surface);

    CARAPI EglCreatePixmapSurface(
        /* [in] */ IEGLDisplay* display,
        /* [in] */ IEGLConfig* config,
        /* [in] */ IInterface* native_pixmap,
        /* [in] */ ArrayOf<Int32>* attrib_list,
        /* [out] */ IEGLSurface** surface);

    CARAPI EglCreateWindowSurface(
        /* [in] */ IEGLDisplay* display,
        /* [in] */ IEGLConfig* config,
        /* [in] */ IInterface* native_window,
        /* [in] */ ArrayOf<Int32>* attrib_list,
        /* [out] */ IEGLSurface** surface);

    CARAPI EglGetDisplay(
        /* [in] */ IInterface* native_display,
        /* [out] */ IEGLDisplay** display);

    CARAPI EglGetCurrentContext(
        /* [out] */ IEGLContext** context);

    CARAPI EglGetCurrentDisplay(
        /* [out] */ IEGLDisplay** display);

    CARAPI EglGetCurrentSurface(
        /* [in] */ Int32 readdraw,
        /* [out] */ IEGLSurface** surface);

private:
    AutoPtr<EGLContextImpl> mContext;
    AutoPtr<EGLDisplayImpl> mDisplay;
    AutoPtr<EGLSurfaceImpl> mSurface;
};

#endif //__EGLIMPL_H__
