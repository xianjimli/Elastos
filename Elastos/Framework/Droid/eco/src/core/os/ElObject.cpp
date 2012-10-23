
#include "os/ElObject.h"
#include "ext/frameworkdef.h"

PInterface ElObject::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }

    return NULL;
}

UInt32 ElObject::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ElObject::Release()
{
    return ElRefBase::Release();
}

ECode ElObject::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)this) {
        *pIID = EIID_IInterface;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}
