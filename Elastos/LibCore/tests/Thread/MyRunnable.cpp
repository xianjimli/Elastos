
#include "MyRunnable.h"
#include "stdio.h"

MyRunnable::MyRunnable()
{
}

UInt32 MyRunnable::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MyRunnable::Release()
{
    return ElRefBase::Release();
}

PInterface MyRunnable::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IRunnable) {
        return (IRunnable*)this;
    }

    return NULL;
}

ECode MyRunnable::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IRunnable*)this) {
        *pIID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode MyRunnable::Run()
{
    printf("==========runrunrunrunrunrun=========\n");
    return NOERROR;
}

Mutex* MyRunnable::GetSelfLock()
{
    return &mLock;
}
