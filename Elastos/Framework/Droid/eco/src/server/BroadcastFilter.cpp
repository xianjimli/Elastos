
#include "server/CActivityManagerService.h"
#include "server/BroadcastFilter.h"

extern "C" const ClassID ECLSID_BroadcastFilter =
        {{0xaaaaaaaa,0xaaaa,0xaaaa,{0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa}}, 0, 0};

BroadcastFilter::BroadcastFilter()
{
    CIntentFilter::NewByFriend((CIntentFilter**)&mFilter);
}

BroadcastFilter::~BroadcastFilter()
{}

IInterface* BroadcastFilter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IObject*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    return NULL;
}

UInt32 BroadcastFilter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 BroadcastFilter::Release()
{
    return ElRefBase::Release();
}

ECode BroadcastFilter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode BroadcastFilter::Aggregate(
    /* [in] */ AggregateType type,
    /* [in] */ PInterface pObj)
{
    return E_NOT_IMPLEMENTED;
}

ECode BroadcastFilter::GetDomain(
    /* [out] */ PInterface *ppObj)
{
    return E_NOT_IMPLEMENTED;
}

ECode BroadcastFilter::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    if (pCLSID == NULL) return E_INVALID_ARGUMENT;
    *pCLSID = ECLSID_BroadcastFilter;

    return NOERROR;
}

Int32 BroadcastFilter::GetPriority()
{
    Int32 pri;
    mFilter->GetPriority(&pri);
    return pri;
}

ECode BroadcastFilter::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}
