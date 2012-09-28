
#include "cmdef.h"
#include "CInet4Address.h"


static AutoPtr<IInetAddress> InitANY()
{
    ArrayOf<Byte>* ipAddress = ArrayOf<Byte>::Alloc(4);
    (*ipAddress)[0] = (*ipAddress)[1] = 0;
    (*ipAddress)[2] = (*ipAddress)[3] = 0;
    AutoPtr<CInet4Address> ipv4Addr;
    CInet4Address::NewByFriend(*ipAddress, (CInet4Address**)&ipv4Addr);
    return (IInetAddress*)(IInet4Address*)ipv4Addr.Get();
}

static AutoPtr<IInetAddress> InitLOOPBACK()
{
    ArrayOf<Byte>* ipAddress = ArrayOf<Byte>::Alloc(4);
    (*ipAddress)[0] = 127;
    (*ipAddress)[1] = (*ipAddress)[2] = 0;
    (*ipAddress)[3] = 1;
    AutoPtr<CInet4Address> ipv4Addr;
    CInet4Address::NewByFriend(*ipAddress, String("localhost"),
            (CInet4Address**)&ipv4Addr);
    return (IInetAddress*)(IInet4Address*)ipv4Addr.Get();
}

const Int32 CInet4Address::AF_INET;
AutoPtr<IInetAddress> CInet4Address::ANY = InitANY();
AutoPtr<IInetAddress> CInet4Address::LOOPBACK = InitLOOPBACK();

PInterface CInet4Address::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_InetAddress) {
        return reinterpret_cast<PInterface>((InetAddress*)this);
    }
    else return _CInet4Address::Probe(riid);
}

ECode CInet4Address::GetAddress(
    /* [out, callee] */ ArrayOf<Byte>** address)
{
    VALIDATE_NOT_NULL(address);
    return InetAddress::GetAddress(address);
}

ECode CInet4Address::GetHostAddress(
    /* [out] */ String* hostAddress)
{
    VALIDATE_NOT_NULL(hostAddress);
    return InetAddress::GetHostAddress(hostAddress);
}

ECode CInet4Address::GetHostName(
    /* [out] */ String* hostName)
{
    VALIDATE_NOT_NULL(hostName);
    return InetAddress::GetHostName(hostName);
}

ECode CInet4Address::GetCanonicalHostName(
    /* [out] */ String* hostName)
{
    VALIDATE_NOT_NULL(hostName);
    return InetAddress::GetCanonicalHostName(hostName);
}

ECode CInet4Address::IsMulticastAddress(
    /* [out] */ Boolean* isMulticastAddress)
{
    VALIDATE_NOT_NULL(isMulticastAddress);
    *isMulticastAddress = ((*mIpAddress)[0] & 0xF0) == 0xE0;
    return NOERROR;
}

ECode CInet4Address::IsLoopbackAddress(
    /* [out] */ Boolean* isLoopbackAddress)
{
    VALIDATE_NOT_NULL(isLoopbackAddress);
    *isLoopbackAddress = ((*mIpAddress)[0] & 255) == 127;
    return NOERROR;
}

ECode CInet4Address::IsLinkLocalAddress(
    /* [out] */ Boolean* isLinkLocalAddress)
{
    VALIDATE_NOT_NULL(isLinkLocalAddress);
    // The reference implementation does not return true for loopback
    // addresses even though RFC 3484 says to do so
    *isLinkLocalAddress = ((((*mIpAddress)[0] & 255) == 169) &&
                           (((*mIpAddress)[1] & 255) == 254));
    return NOERROR;
}

ECode CInet4Address::IsSiteLocalAddress(
    /* [out] */ Boolean* isSiteLocalAddress)
{
    ArrayOf<Byte>& ipAddress = *mIpAddress;
    VALIDATE_NOT_NULL(isSiteLocalAddress);
    *isSiteLocalAddress = ((ipAddress[0] & 255) == 10) ||
        (((ipAddress[0] & 255) == 172) && ((ipAddress[1] & 255) > 15) && (ipAddress[1] & 255) < 32) ||
        (((ipAddress[0] & 255) == 192) && ((ipAddress[1] & 255) == 168));
    return NOERROR;
}

ECode CInet4Address::IsMCGlobal(
    /* [out] */ Boolean* isMCGlobal)
{
    VALIDATE_NOT_NULL(isMCGlobal);
    // Check if we have a prefix of 1110
    Boolean isMulticastAddress;
    IsMulticastAddress(&isMulticastAddress);
    if (!isMulticastAddress) {
        *isMCGlobal = FALSE;
        return NOERROR;
    }

    Int32 address = InetAddress::BytesToInt32(*mIpAddress, 0);
    /*
     * Now check the boundaries of the global space if we have an address
     * that is prefixed by something less than 111000000000000000000001
     * (fortunately we don't have to worry about sign after shifting 8 bits
     * right) it is not multicast. ( < 224.0.1.0)
     */
    if (((UInt32)address) >> 8 < 0xE00001) {
        *isMCGlobal = FALSE;
        return NOERROR;
    }

    /*
     * Now check the high boundary which is prefixed by 11101110 = 0xEE. If
     * the value is higher than this than it is not MCGlobal ( >
     * 238.255.255.255 )
     */
    if (((UInt32)address) >> 24 > 0xEE) {
        *isMCGlobal = FALSE;
        return NOERROR;
    }

    *isMCGlobal = TRUE;
    return NOERROR;
}

ECode CInet4Address::IsMCNodeLocal(
    /* [out] */ Boolean* isMCNodeLocal)
{
    VALIDATE_NOT_NULL(isMCNodeLocal);
    *isMCNodeLocal = FALSE;
    return NOERROR;
}

ECode CInet4Address::IsMCLinkLocal(
    /* [out] */ Boolean* isMCLinkLocal)
{
    VALIDATE_NOT_NULL(isMCLinkLocal);
    *isMCLinkLocal = ((UInt32)InetAddress::BytesToInt32(*mIpAddress, 0)) >> 8 == 0xE00000;
    return NOERROR;
}

ECode CInet4Address::IsMCSiteLocal(
    /* [out] */ Boolean* isMCSiteLocal)
{
    VALIDATE_NOT_NULL(isMCSiteLocal);
    *isMCSiteLocal = ((UInt32)InetAddress::BytesToInt32(*mIpAddress, 0)) >> 16 == 0xEFFF;
    return NOERROR;
}

ECode CInet4Address::IsMCOrgLocal(
    /* [out] */ Boolean* isMCOrgLocal)
{
    VALIDATE_NOT_NULL(isMCOrgLocal);
    Int32 prefix = ((UInt32)InetAddress::BytesToInt32(*mIpAddress, 0)) >> 16;
    *isMCOrgLocal = prefix >= 0xEFC0 && prefix <= 0xEFC3;
    return NOERROR;
}

ECode CInet4Address::IsAnyLocalAddress(
    /* [out] */ Boolean* isAnyLocalAddress)
{
    VALIDATE_NOT_NULL(isAnyLocalAddress);
    for (Int32 i = 0; i < mIpAddress->GetLength(); i++) {
        if ((*mIpAddress)[i] != 0) {
            *isAnyLocalAddress = FALSE;
            return NOERROR;
        }
    }
    *isAnyLocalAddress = TRUE;
    return NOERROR;
}

// private Object writeReplace() throws ObjectStreamException {
//     return new Inet4Address(ipaddress, hostName);
// }

ECode CInet4Address::IsReachable(
    /* [in] */ Int32 timeout,
    /* [out] */ Boolean* isReachable)
{
    VALIDATE_NOT_NULL(isReachable);
    return InetAddress::IsReachable(timeout, isReachable);
}

ECode CInet4Address::IsReachableEx(
    /* [in] */ INetworkInterface* networkInterface,
    /* [in] */ Int32 ttl,
    /* [in] */ Int32 timeout,
    /* [out] */ Boolean* isReachable)
{
    VALIDATE_NOT_NULL(isReachable);
    return InetAddress::IsReachableEx(networkInterface, ttl, timeout, isReachable);
}

ECode CInet4Address::constructor(
    /* [in] */ const ArrayOf<Byte>& address)
{
    mFamily = AF_INET;
    mIpAddress = const_cast<ArrayOf<Byte>*>(&address);
    return NOERROR;
}

ECode CInet4Address::constructor(
    /* [in] */ const ArrayOf<Byte>& address,
    /* [in] */ const String& name)
{
    mFamily = AF_INET;
    mIpAddress = const_cast<ArrayOf<Byte>*>(&address);
    mHostName = name;
    return NOERROR;
}
