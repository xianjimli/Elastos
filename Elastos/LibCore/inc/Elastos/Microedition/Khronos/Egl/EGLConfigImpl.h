
#ifndef __EGLCONFIGIMPL_H__
#define __EGLCONFIGIMPL_H__

#include "_EGLConfig.h"
#include "Elastos.Microedition.Khronos.Egl_server.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <EGL/egl.h>

class EGLConfigImpl
    : public ElRefBase
    , public _EGLConfig
    , public IEGLConfigImpl
{
public:
    EGLConfigImpl(
        /* [in] */ EGLConfig config);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI_(EGLConfig) Get();

private:
    EGLConfig mEGLConfig;
};

#endif //__EGLCONFIGIMPL_H__
