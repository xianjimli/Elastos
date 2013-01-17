
#include "cmdef.h"
#include "InterfaceAddress.h"
#include "CInet4Address.h"


InterfaceAddress::InterfaceAddress(
    /* [in] */ Int32 index,
    /* [in] */ const String& name,
    /* [in] */ IInetAddress* address,
    /* [in] */ IInetAddress* mask)
{
    assert((address == NULL && mask == NULL) ||
            (address != NULL && mask != NULL &&
                IInet4Address::Probe(address) == IInet4Address::Probe(mask)));

    mIndex = index;
    mName = name;
    mAddress = address;
    mBroadcastAddress = MakeBroadcastAddress(address, mask);
    mPrefixLength = CountPrefixLength(mask);
}

PInterface InterfaceAddress::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IInterfaceAddress*)this;
    }
    else if (riid == EIID_IInterfaceAddress) {
        return (IInterfaceAddress*)this;
    }

    return NULL;
}

UInt32 InterfaceAddress::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 InterfaceAddress::Release()
{
    return ElRefBase::Release();
}

ECode InterfaceAddress::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

AutoPtr<IInetAddress> InterfaceAddress::MakeBroadcastAddress(
    /* [in] */ IInetAddress* address,
    /* [in] */ IInetAddress* mask)
{
    if (address == NULL || IInet4Address::Probe(address) == NULL) {
        return NULL;
    }

    ArrayOf_<Byte, 4> broadcast;
    ArrayOf<Byte>* maskBytes = ((InetAddress*)mask->Probe(EIID_InetAddress))->mIpAddress;
    ArrayOf<Byte>* addrBytes = ((InetAddress*)address->Probe(EIID_InetAddress))->mIpAddress;
    if ((*maskBytes)[0] != 0) {
        for (Int32 i = 0; i < broadcast.GetLength(); ++i) {
            broadcast[i] = (*addrBytes)[i] | ~(*maskBytes)[i];
        }
    }
    AutoPtr<IInet4Address> addr;
    ASSERT_SUCCEEDED(CInet4Address::New(broadcast, (IInet4Address**)&addr));
    return IInetAddress::Probe(addr.Get());
}

Int16 InterfaceAddress::CountPrefixLength(
    /* [in] */ IInetAddress* mask)
{
    Int16 count = 0;
    ArrayOf<Byte>* maskBytes = ((InetAddress*)mask->Probe(EIID_InetAddress))->mIpAddress;
    for (Int32 i = 0; i < maskBytes->GetLength(); ++i) {
        Byte b = (*maskBytes)[i];
        for (Int32 j = 0; j < 8; ++j) {
            if ((b & (1 << j)) != 0) {
                ++count;
            }
        }
    }

    return count;
}

ECode InterfaceAddress::GetAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);
    *address = mAddress;
    if (*address != NULL) (*address)->AddRef();
    return NOERROR;
}

ECode InterfaceAddress::GetBroadcast(
    /* [out] */ IInetAddress** broadcast)
{
    VALIDATE_NOT_NULL(broadcast);
    *broadcast = mBroadcastAddress;
    if (*broadcast != NULL) (*broadcast)->AddRef();
    return NOERROR;
}

ECode InterfaceAddress::GetNetworkPrefixLength(
    /* [out] */ Int16* length)
{
    VALIDATE_NOT_NULL(length);
    *length = mPrefixLength;
    return NOERROR;
}
