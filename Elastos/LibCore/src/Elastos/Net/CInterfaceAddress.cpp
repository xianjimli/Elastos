
#include "cmdef.h"
#include "CInterfaceAddress.h"
#include "CInet4Address.h"

CInterfaceAddress::CInterfaceAddress()
    : mIndex(0)
    , mPrefixLength(0)
{
}

void CInterfaceAddress::MakeBroadcastAddress(
    /* [in] */ IInetAddress* address,
    /* [in] */ IInetAddress* mask,
    /* [out] */ IInetAddress** broadcastAddress)
{
    if (address->Probe(EIID_IInet4Address) == NULL) {
        *broadcastAddress = NULL;
        return;
    }
    ArrayOf<Byte>* broadcast = ArrayOf<Byte>::Alloc(4);
    ArrayOf<Byte>* maskBytes = ((InetAddress*)mask->Probe(EIID_InetAddress))->mIpAddress;
    ArrayOf<Byte>* addrBytes = ((InetAddress*)address->Probe(EIID_InetAddress))->mIpAddress;
    if ((*maskBytes)[0] != 0) {
        for (Int32 i = 0; i < broadcast->GetLength(); ++i) {
            (*broadcast)[i] = (Byte) ((*addrBytes)[i] | ~(*maskBytes)[i]);
        }
    }
    AutoPtr<IInet4Address> addr;
    ASSERT_SUCCEEDED(CInet4Address::New(*broadcast, (IInet4Address**)&addr));
    if (addr != NULL && addr->Probe(EIID_IInetAddress)) {
        *broadcastAddress = (IInetAddress*)addr->Probe(EIID_IInetAddress);
    }
    else {
        *broadcastAddress = NULL;
    }
}

Int16 CInterfaceAddress::CountPrefixLength(
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

ECode CInterfaceAddress::GetAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);
    *address = mAddress;
    return NOERROR;
}

ECode CInterfaceAddress::GetBroadcast(
    /* [out] */ IInetAddress** broadcast)
{
    VALIDATE_NOT_NULL(broadcast);
    *broadcast = mBroadcastAddress;
    return NOERROR;
}

ECode CInterfaceAddress::GetNetworkPrefixLength(
    /* [out] */ Int16* length)
{
    VALIDATE_NOT_NULL(length);
    *length = mPrefixLength;
    return NOERROR;
}

ECode CInterfaceAddress::constructor(
    /* [in] */ Int32 index,
    /* [in] */ const String& name,
    /* [in] */ IInetAddress* address,
    /* [in] */ IInetAddress* mask)
{
    assert((address->Probe(EIID_IInet4Address)) == (mask->Probe(EIID_IInet4Address)));
    mIndex = index;
    mName = name;
    mAddress = address;
    MakeBroadcastAddress(address, mask, (IInetAddress**)&mBroadcastAddress);
    mPrefixLength = CountPrefixLength(mask);

    return NOERROR;
}
