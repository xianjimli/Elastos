
#include "server/location/LocationProviderInterface.h"

extern "C" const InterfaceID EIID_LocationProviderInterface =
{ 0xd6f5b96d, 0xf1bc, 0x49ee, { 0xbe, 0xec, 0xa4, 0x39, 0x69, 0x1d, 0x33, 0x4d } };

PInterface LocationProviderInterface::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_LocationProviderInterface) {
        return this;
    }

    return NULL;
}

UInt32 LocationProviderInterface::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 LocationProviderInterface::Release()
{
    return ElRefBase::Release();
}

CARAPI LocationProviderInterface::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)this) {
        *pIID = EIID_LocationProviderInterface;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}
