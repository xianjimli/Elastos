
#include "EGLConfigImpl.h"


EGLConfigImpl::EGLConfigImpl(
    /* [in] */ EGLConfig config)
    : mEGLConfig(config)
{}

PInterface EGLConfigImpl::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IEGLConfigImpl*)this;
    }
    else if (riid == EIID_IEGLConfig) {
        return (IEGLConfig*)this;
    }
    else if (riid == EIID_IEGLConfigImpl) {
        return (IEGLConfigImpl*)this;
    }

    return NULL;
}

UInt32 EGLConfigImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 EGLConfigImpl::Release()
{
    return ElRefBase::Release();
}

ECode EGLConfigImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IEGLConfigImpl*)this) {
        *pIID = EIID_IEGLConfigImpl;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

EGLConfig EGLConfigImpl::Get()
{
    return mEGLConfig;
}
