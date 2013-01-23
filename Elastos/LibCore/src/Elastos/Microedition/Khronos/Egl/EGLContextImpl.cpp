
#include "EGLContextImpl.h"

EGLContextImpl::EGLContextImpl(
    /* [in] */ EGLContext ctx)
    : mEGLContext(ctx)
{
    // mGLContext = new GLImpl();
}

PInterface EGLContextImpl::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IEGLContextImpl*)this;
    }
    else if (riid == EIID_IEGLContext) {
        return (IEGLContext*)this;
    }
    else if (riid == EIID_IEGLContextImpl) {
        return (IEGLContextImpl*)this;
    }

    return NULL;
}

UInt32 EGLContextImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 EGLContextImpl::Release()
{
    return ElRefBase::Release();
}

ECode EGLContextImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IEGLContextImpl*)this) {
        *pIID = EIID_IEGLContextImpl;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

EGLContext EGLContextImpl::Get()
{
    return mEGLContext;
}
