
#include "cmdef.h"
#include "EGLImpl.h"
#include "EGLConfigImpl.h"
#include <Elastos.Core.h>
#include <EGL/egl.h>
#include <skia/core/SkBitmap.h>
#include <ui/PixelFormat.h>

PInterface EGLImpl::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IEGL10*)this;
    }
    else if (riid == EIID_IEGL10) {
        return (IEGL10*)this;
    }

    return NULL;
}

UInt32 EGLImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 EGLImpl::Release()
{
    return ElRefBase::Release();
}

ECode EGLImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IEGL10*)this) {
        *pIID = EIID_IEGL10;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

static inline EGLDisplay getDisplay(IEGLDisplay* o)
{
    if (!o) return EGL_NO_DISPLAY;
    IEGLDisplayImpl* impl = IEGLDisplayImpl::Probe(o);
    assert(impl != NULL);
    return ((EGLDisplayImpl*)impl)->Get();
}

static inline EGLSurface getSurface(IEGLSurface* o)
{
    if (!o) return EGL_NO_SURFACE;
    IEGLSurfaceImpl* impl = IEGLSurfaceImpl::Probe(o);
    assert(impl != NULL);
    return ((EGLSurfaceImpl*)impl)->Get();
}

static inline EGLContext getContext(IEGLContext* o)
{
    if (!o) return EGL_NO_CONTEXT;
    IEGLContextImpl* impl = IEGLContextImpl::Probe(o);
    assert(impl != NULL);
    return ((EGLContextImpl*)impl)->Get();
}

static inline EGLConfig getConfig(IEGLConfig* o)
{
    if (!o) return 0;
    IEGLConfigImpl* impl = IEGLConfigImpl::Probe(o);
    assert(impl != NULL);
    return ((EGLConfigImpl*)impl)->Get();
}

static const Int32 gNull_attrib_base[] = { EGL_NONE };

static Boolean validAttribList(ArrayOf<Int32>* attrib_list)
{
    if (attrib_list == NULL) {
        return TRUE;
    }
    Int32 len = attrib_list->GetLength();
    if (len < 1) {
        return FALSE;
    }
    Int32 item = (*attrib_list)[len - 1];
    return item == EGL_NONE;
}

static Int32* beginNativeAttribList(ArrayOf<Int32>* attrib_list)
{
    if (attrib_list != NULL) {
        return attrib_list->GetPayload();
    }
    else {
        return (Int32*)gNull_attrib_base;
    }
}

static void endNativeAttributeList(ArrayOf<Int32>* attrib_list)
{
}

ECode EGLImpl::EglInitialize(
    /* [in] */ IEGLDisplay* display,
    /* [in] */ ArrayOf<Int32>* major_minor,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (display == NULL || (major_minor != NULL &&
            major_minor->GetLength() < 2)) {
        *result = FALSE;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    EGLDisplay dpy = getDisplay(display);
    Boolean success = eglInitialize(dpy, NULL, NULL);
    if (success && major_minor) {
        Int32 len = major_minor->GetLength();
        if (len) {
            // we're exposing only EGL 1.0
            if (len >= 1) (*major_minor)[0] = 1;
            if (len >= 2) (*major_minor)[1] = 0;
        }
    }
    *result = success;
    return NOERROR;
}

ECode EGLImpl::EglQueryContext(
    /* [in] */ IEGLDisplay* display,
    /* [in] */ IEGLContext* context,
    /* [in] */ Int32 attribute,
    /* [in] */ ArrayOf<Int32>* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (display == NULL || context == NULL || value == NULL
        || value->GetLength() < 1) {
        *result = FALSE;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    EGLDisplay dpy = getDisplay(display);
    EGLContext ctx = getContext(context);
    Boolean success = FALSE;
    Int32 len = value->GetLength();
    if (len) {
        int* base = (int *)value->GetPayload();
        success = eglQueryContext(dpy, ctx, attribute, base);
    }
    *result = success;
    return NOERROR;
}

ECode EGLImpl::EglQuerySurface(
    /* [in] */ IEGLDisplay* display,
    /* [in] */ IEGLSurface* surface,
    /* [in] */ Int32 attribute,
    /* [in] */ ArrayOf<Int32>* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (display == NULL || surface == NULL || value == NULL
        || value->GetLength() < 1) {
        *result = FALSE;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    EGLDisplay dpy = getDisplay(display);
    EGLSurface sur = getSurface(surface);

    Boolean success = FALSE;
    Int32 len = value->GetLength();
    if (len) {
        int* base = (int *)value->GetPayload();
        success = eglQuerySurface(dpy, sur, attribute, base);
    }
    *result = success;
    return NOERROR;
}

ECode EGLImpl::EglChooseConfig(
    /* [in] */ IEGLDisplay* display,
    /* [in] */ ArrayOf<Int32>* attrib_list,
    /* [in] */ ArrayOf<IEGLConfig*>* configs,
    /* [in] */ Int32 config_size,
    /* [in] */ ArrayOf<Int32>* num_config,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (display == NULL
        || !validAttribList(attrib_list)
        || (configs != NULL && configs->GetLength() < config_size)
        || (num_config != NULL && num_config->GetLength() < 1)) {
        *result = FALSE;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    EGLDisplay dpy = getDisplay(display);
    Boolean success = FALSE;

    if (configs == NULL) {
        config_size = 0;
    }
    EGLConfig nativeConfigs[config_size];

    int num = 0;
    Int32* attrib_base = beginNativeAttribList(attrib_list);
    success = eglChooseConfig(dpy, (int*)attrib_base, configs ? nativeConfigs : 0, config_size, &num);

    if (num_config != NULL) {
        (*num_config)[0] = num;
    }

    if (success && configs != NULL) {
        for (Int32 i = 0 ; i < num ; i++) {
            (*configs)[i] = new EGLConfigImpl(nativeConfigs[i]);
            (*configs)[i]->AddRef();
        }
    }
    *result = success;
    return NOERROR;
}

ECode EGLImpl::EglGetConfigAttrib(
    /* [in] */ IEGLDisplay* display,
    /* [in] */ IEGLConfig* config,
    /* [in] */ Int32 attribute,
    /* [in] */ ArrayOf<Int32>* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (display == NULL || config == NULL
        || (value == NULL || value->GetLength() < 1)) {
        *result = FALSE;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    EGLDisplay dpy = getDisplay(display);
    EGLConfig cnf = getConfig(config);
    Boolean success = FALSE;
    int localValue;
    success = eglGetConfigAttrib(dpy, cnf, attribute, &localValue);
    if (success) {
        (*value)[0] = localValue;
    }
    *result = success;
    return NOERROR;
}

ECode EGLImpl::EglGetConfigs(
    /* [in] */ IEGLDisplay* display,
    /* [in] */ ArrayOf<IEGLConfig*>* configs,
    /* [in] */ Int32 config_size,
    /* [in] */ ArrayOf<Int32>* num_config,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (display == NULL || (configs != NULL && configs->GetLength() < config_size)
        || (num_config != NULL && num_config->GetLength() < 1)) {
        *result = FALSE;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    EGLDisplay dpy = getDisplay(display);
    Boolean success = FALSE;
    if (configs == NULL) {
        config_size = 0;
    }
    EGLConfig nativeConfigs[config_size];
    int num;
    success = eglGetConfigs(dpy, configs ? nativeConfigs : 0, config_size, &num);
    if (num_config != NULL) {
        (*num_config)[0] = num;
    }
    if (success && configs) {
        for (int i = 0 ; i < num ; i++) {
            (*configs)[i] = new EGLConfigImpl(nativeConfigs[i]);
            (*configs)[i]->AddRef();
        }
    }
    return success;
}

ECode EGLImpl::EglGetError(
    /* [out] */ Int32* error)
{
    VALIDATE_NOT_NULL(error);

    *error = (Int32)eglGetError();
    return NOERROR;
}

ECode EGLImpl::EglDestroyContext(
    /* [in] */ IEGLDisplay* display,
    /* [in] */ IEGLContext* context,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (display == NULL || context == NULL) {
        *result = FALSE;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    EGLDisplay dpy = getDisplay(display);
    EGLContext ctx = getContext(context);
    *result = eglDestroyContext(dpy, ctx);
    return NOERROR;
}

ECode EGLImpl::EglDestroySurface(
    /* [in] */ IEGLDisplay* display,
    /* [in] */ IEGLSurface* surface,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (display == NULL || surface == NULL) {
        *result = FALSE;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    EGLDisplay dpy = getDisplay(display);
    EGLSurface sur = getSurface(surface);

    if (sur) {
        SkPixelRef* ref = ((EGLSurfaceImpl*)surface)->GetNativePixelRef();
        if (ref) {
            ref->unlockPixels();
            ref->safeUnref();
        }
    }
    *result = eglDestroySurface(dpy, sur);
    return NOERROR;
}

ECode EGLImpl::EglMakeCurrent(
    /* [in] */ IEGLDisplay* display,
    /* [in] */ IEGLSurface* draw,
    /* [in] */ IEGLSurface* read,
    /* [in] */ IEGLContext* context,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (display == NULL || draw == NULL || read == NULL || context == NULL) {
        *result = FALSE;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    EGLDisplay dpy = getDisplay(display);
    EGLSurface sdr = getSurface(draw);
    EGLSurface srd = getSurface(read);
    EGLContext ctx = getContext(context);
    *result = eglMakeCurrent(dpy, sdr, srd, ctx);
    return NOERROR;
}

ECode EGLImpl::EglQueryString(
    /* [in] */ IEGLDisplay* display,
    /* [in] */ Int32 name,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    if (display == NULL) {
        *str = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    EGLDisplay dpy = getDisplay(display);
    const char* chars = eglQueryString(dpy, name);
    *str = chars;
    return NOERROR;
}

ECode EGLImpl::EglSwapBuffers(
    /* [in] */ IEGLDisplay* display,
    /* [in] */ IEGLSurface* surface,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (display == NULL || surface == NULL) {
        *result = FALSE;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    EGLDisplay dpy = getDisplay(display);
    EGLSurface sur = getSurface(surface);
    *result = eglSwapBuffers(dpy, sur);
    return NOERROR;
}

ECode EGLImpl::EglTerminate(
    /* [in] */ IEGLDisplay* display,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (display == NULL) {
        *result = FALSE;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    EGLDisplay dpy = getDisplay(display);
    *result = eglTerminate(dpy);
    return NOERROR;
}

ECode EGLImpl::EglCopyBuffers(
    /* [in] */ IEGLDisplay* display,
    /* [in] */ IEGLSurface* surface,
    /* [in] */ IInterface* native_pixmap,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (display == NULL || surface == NULL || native_pixmap == NULL) {
        *result = FALSE;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // TODO: Implement this
    *result = FALSE;
    return NOERROR;
}

ECode EGLImpl::EglWaitGL(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = eglWaitGL();
    return NOERROR;
}

ECode EGLImpl::EglWaitNative(
    /* [in] */ Int32 engine,
    /* [in] */ IInterface* bindTarget,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = eglWaitNative(engine);
    return NOERROR;
}

ECode EGLImpl::EglCreateContext(
    /* [in] */ IEGLDisplay* display,
    /* [in] */ IEGLConfig* config,
    /* [in] */ IEGLContext* share_context,
    /* [in] */ ArrayOf<Int32>* attrib_list,
    /* [out] */ IEGLContext** context)
{
    VALIDATE_NOT_NULL(context);

    if (display == NULL || config == NULL || share_context == NULL
        || !validAttribList(attrib_list)) {
        *context = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    EGLDisplay dpy = getDisplay(display);
    EGLConfig  cnf = getConfig(config);
    EGLContext shr = getContext(share_context);
    Int32* base = beginNativeAttribList(attrib_list);
    EGLContext ctx = eglCreateContext(dpy, cnf, shr, base);
    endNativeAttributeList(attrib_list);
    if (ctx == NULL) {
        assert(0);
        // return EGL10.EGL_NO_CONTEXT;
        *context = NULL;
        return NOERROR;
    }
    *context = new EGLContextImpl(ctx);
    (*context)->AddRef();
    return NOERROR;
}

ECode EGLImpl::EglCreatePbufferSurface(
    /* [in] */ IEGLDisplay* display,
    /* [in] */ IEGLConfig* config,
    /* [in] */ ArrayOf<Int32>* attrib_list,
    /* [out] */ IEGLSurface** surface)
{
    VALIDATE_NOT_NULL(surface);

    if (display == NULL || config == NULL
        || !validAttribList(attrib_list)) {
        *surface = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    EGLDisplay dpy = getDisplay(display);
    EGLConfig  cnf = getConfig(config);
    Int32* base = beginNativeAttribList(attrib_list);
    EGLSurface sur = eglCreatePbufferSurface(dpy, cnf, base);
    endNativeAttributeList(attrib_list);
    if (sur == 0) {
        assert(0);
        // return EGL10.EGL_NO_SURFACE;
        *surface = NULL;
        return NOERROR;
    }
    *surface = new EGLSurfaceImpl(sur);
    (*surface)->AddRef();
    return NOERROR;
}

static android::PixelFormat convertPixelFormat(SkBitmap::Config format)
{
    switch (format) {
    case SkBitmap::kARGB_8888_Config:   return android::PIXEL_FORMAT_RGBA_8888;
    case SkBitmap::kARGB_4444_Config:   return android::PIXEL_FORMAT_RGBA_4444;
    case SkBitmap::kRGB_565_Config:     return android::PIXEL_FORMAT_RGB_565;
    case SkBitmap::kA8_Config:          return android::PIXEL_FORMAT_A_8;
    default:                            return android::PIXEL_FORMAT_NONE;
    }
}

ECode EGLImpl::EglCreatePixmapSurface(
    /* [in] */ IEGLDisplay* display,
    /* [in] */ IEGLConfig* config,
    /* [in] */ IInterface* native_pixmap,
    /* [in] */ ArrayOf<Int32>* attrib_list,
    /* [out] */ IEGLSurface** surface)
{
    VALIDATE_NOT_NULL(surface);

    if (display == NULL || config == NULL || native_pixmap == NULL
        || !validAttribList(attrib_list)) {
        *surface = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    EGLSurfaceImpl sur = new EGLSurfaceImpl();

    EGLDisplay dpy = getDisplay(display);
    EGLConfig  cnf = getConfig(config);
    Int32* base = 0;

    SkBitmap const * nativeBitmap =
            (SkBitmap const *)_env->GetIntField(native_pixmap,
                    gBitmap_NativeBitmapFieldID);
    SkPixelRef* ref = nativeBitmap ? nativeBitmap->pixelRef() : 0;
    if (ref == NULL) {
        *surface = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    ref->safeRef();
    ref->lockPixels();

    egl_native_pixmap_t pixmap;
    pixmap.version = sizeof(pixmap);
    pixmap.width  = nativeBitmap->width();
    pixmap.height = nativeBitmap->height();
    pixmap.stride = nativeBitmap->rowBytes() / nativeBitmap->bytesPerPixel();
    pixmap.format = convertPixelFormat(nativeBitmap->config());
    pixmap.data   = (uint8_t*)ref->pixels();

    base = beginNativeAttribList(attrib_list);
    EGLSurface sur = eglCreatePixmapSurface(dpy, cnf, &pixmap, base);
    endNativeAttributeList(attrib_list);

    if (sur != EGL_NO_SURFACE) {
        _env->SetIntField(out_sur, gSurface_EGLSurfaceFieldID, (int)sur);
        _env->SetIntField(out_sur, gSurface_NativePixelRefFieldID, (int)ref);
    }
    else {
        ref->unlockPixels();
        ref->safeUnref();
    }

    _eglCreatePixmapSurface(sur, display, config, native_pixmap, attrib_list);
    if (sur.mEGLSurface == 0) {
        delete sur;
        assert(0);
        // return EGL10.EGL_NO_SURFACE;
        *surface == NULL;
        return NOERROR;
    }
    return sur;
}

CARAPI EglCreateWindowSurface(
    /* [in] */ IEGLDisplay* display,
    /* [in] */ IEGLConfig* config,
    /* [in] */ IInterface* native_window,
    /* [in] */ ArrayOf<Int32>* attrib_list,
    /* [out] */ IEGLSurface** surface)
{
    Surface sur;
    if (native_window instanceof SurfaceView) {
        SurfaceView surfaceView = (SurfaceView)native_window;
        sur = surfaceView.getHolder().getSurface();
    } else if (native_window instanceof SurfaceHolder) {
        SurfaceHolder holder = (SurfaceHolder)native_window;
        sur = holder.getSurface();
    } else {
        throw new java.lang.UnsupportedOperationException(
            "eglCreateWindowSurface() can only be called with an instance of " +
            "SurfaceView or SurfaceHolder at the moment, this will be fixed later.");
    }
    int eglSurfaceId = _eglCreateWindowSurface(display, config, sur, attrib_list);
    if (eglSurfaceId == 0) {
        return EGL10.EGL_NO_SURFACE;
    }
    return new EGLSurfaceImpl( eglSurfaceId );
}

CARAPI EglGetDisplay(
    /* [in] */ IInterface* native_display,
    /* [out] */ IEGLDisplay** display)
{
    int value = _eglGetDisplay(native_display);
    if (value == 0) {
        return EGL10.EGL_NO_DISPLAY;
    }
    if (mDisplay.mEGLDisplay != value)
        mDisplay = new EGLDisplayImpl(value);
    return mDisplay;
}

CARAPI EglGetCurrentContext(
    /* [out] */ IEGLContext** context)
{
    int value = _eglGetCurrentContext();
    if (value == 0) {
        return EGL10.EGL_NO_CONTEXT;
    }
    if (mContext.mEGLContext != value)
        mContext = new EGLContextImpl(value);
    return mContext;
}

CARAPI EglGetCurrentDisplay(
    /* [out] */ IEGLDisplay** display)
{
    int value = _eglGetCurrentDisplay();
    if (value == 0) {
        return EGL10.EGL_NO_DISPLAY;
    }
    if (mDisplay.mEGLDisplay != value)
        mDisplay = new EGLDisplayImpl(value);
    return mDisplay;
}

CARAPI EglGetCurrentSurface(
    /* [in] */ Int32 readdraw,
    /* [out] */ IEGLSurface** surface)
{
    int value = _eglGetCurrentSurface(readdraw);
    if (value == 0) {
        return EGL10.EGL_NO_SURFACE;
    }
    if (mSurface.mEGLSurface != value)
        mSurface = new EGLSurfaceImpl(value);
    return mSurface;
}
