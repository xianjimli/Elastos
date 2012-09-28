
#include "cmdef.h"
#include "CInet6Address.h"


static AutoPtr<IInetAddress> InitANY()
{
    ArrayOf<Byte>* ipAddress = ArrayOf<Byte>::Alloc(16);
    memset(ipAddress->GetPayload(), 0, 16);
    AutoPtr<CInet6Address> ipv6Addr;
    CInet6Address::NewByFriend(*ipAddress, (CInet6Address**)&ipv6Addr);
    return (IInetAddress*)(IInet6Address*)ipv6Addr.Get();
}

static AutoPtr<IInetAddress> InitLOOPBACK()
{
    ArrayOf<Byte>* ipAddress = ArrayOf<Byte>::Alloc(16);
    memset(ipAddress->GetPayload(), 0, 16);
    (*ipAddress)[15] = 1;
    AutoPtr<CInet6Address> ipv6Addr;
    CInet6Address::NewByFriend(*ipAddress, String("localhost"),
            (CInet6Address**)&ipv6Addr);
    return (IInetAddress*)(IInet6Address*)ipv6Addr.Get();
}

const Int32 CInet6Address::AF_INET6;
AutoPtr<IInetAddress> CInet6Address::ANY = InitANY();
AutoPtr<IInetAddress> CInet6Address::LOOPBACK = InitLOOPBACK();


CInet6Address::CInet6Address()
    : mScopeIdSet(FALSE)
    , mScopeIfnameSet(FALSE)
{}

PInterface CInet6Address::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_InetAddress) {
        return reinterpret_cast<PInterface>((InetAddress*)this);
    }
    else return _CInet6Address::Probe(riid);
}

/**
 * Constructs an IPv6 address according to the given {@code host}, {@code
 * addr} and {@code scope_id}.
 *
 * @param host
 *            the host name associated with the address.
 * @param addr
 *            the network address.
 * @param scope_id
 *            the scope id for link- or site-local addresses.
 * @return the Inet6Address instance representing the IP address.
 * @throws UnknownHostException
 *             if the address is null or has an invalid length.
 */
ECode CInet6Address::GetByAddress(
    /* [in] */ const String& host,
    /* [in] */ const ArrayOf<Byte>& addr,
    /* [in] */ Int32 scopeId,
    /* [out] */ IInet6Address** address)
{
    VALIDATE_NOT_NULL(address);
    if (addr.GetLength() != 16) {
//        throw new UnknownHostException("Illegal IPv6 address");
        return E_UNKNOWN_HOST_EXCEPTION;
    }
    if (scopeId < 0) {
        scopeId = 0;
    }
    CInet6Address* ipv6Addr;
    FAIL_RETURN(CInet6Address::NewByFriend(addr, host, scopeId, &ipv6Addr));
    *address = (IInet6Address*)ipv6Addr;
    return NOERROR;
}

/**
 * Gets an IPv6 address instance according to the given {@code host},
 * {@code addr} and {@code nif}. {@code scope_id} is set according to the
 * given {@code nif} and the {@code addr} type (for example site-local or
 * link-local).
 *
 * @param host
 *            the hostname associated with the address.
 * @param addr
 *            the network address.
 * @param nif
 *            the network interface that this address is associated with.
 * @return the Inet6Address instance representing the IP address.
 * @throws UnknownHostException
 *             if the address is {@code null} or has an invalid length or
 *             the interface doesn't have a numeric scope id for the given
 *             address type.
 */
ECode CInet6Address::GetByAddress(
    /* [in] */ const String& host,
    /* [in] */ const ArrayOf<Byte>& addr,
    /* [in] */ INetworkInterface* nif,
    /* [out] */ IInet6Address** address)
{
    VALIDATE_NOT_NULL(address);

    FAIL_RETURN(GetByAddress(host, addr, 0, address));

    // if nif is null, nothing needs to be set.
    if (NULL == nif) {
        return NOERROR;
    }

    // find the first address which matches the type addr,
    // then set the scope_id, ifname and scopedIf.
//    Enumeration<InetAddress> addressList = nif.getInetAddresses();
//    while (addressList.hasMoreElements()) {
//        InetAddress ia = addressList.nextElement();
//        if (ia.getAddress().length == 16) {
//            Inet6Address v6ia = (Inet6Address) ia;
//            boolean isSameType = v6ia.compareLocalType(address);
//            if (isSameType) {
//                address.scope_id_set = true;
//                address.scope_id = v6ia.scope_id;
//                address.scope_ifname_set = true;
//                address.ifname = nif.getName();
//                address.scopedIf = nif;
//                break;
//            }
//        }
//    }
    // if no address matches the type of addr, throws an
    // UnknownHostException.
    if (!((CInet6Address*)*address)->mScopeIdSet) {
//        throw new UnknownHostException("Scope id not found for the given address");
        return E_UNKNOWN_HOST_EXCEPTION;
    }
    return NOERROR;
}

/**
 * Returns {@code true} if one of following cases applies:
 * <p>
 * <ol>
 *  <li>both addresses are site local</li>
 *  <li>both addresses are link local</li>
 *  <li>{@code ia} is neither site local nor link local</li>
 * </ol>
 */
Boolean CInet6Address::CompareLocalType(
    /* [in] */ IInet6Address* ia)
{
    Boolean iaSLA, isSLA;
    ia->IsSiteLocalAddress(&iaSLA);
    IsSiteLocalAddress(&isSLA);
    if (iaSLA && isSLA) {
        return TRUE;
    }
    Boolean iaLLA, isLLA;
    ia->IsLinkLocalAddress(&iaLLA);
    IsLinkLocalAddress(&isLLA);
    if (iaLLA && isLLA) {
        return TRUE;
    }
    if (!iaSLA && !iaLLA) {
        return TRUE;
    }
    return FALSE;
}

ECode CInet6Address::GetAddress(
    /* [out, callee] */ ArrayOf<Byte>** address)
{
    VALIDATE_NOT_NULL(address);
    return InetAddress::GetAddress(address);
}

ECode CInet6Address::GetHostAddress(
    /* [out] */ String* hostAddress)
{
    VALIDATE_NOT_NULL(hostAddress);
    return InetAddress::GetHostAddress(hostAddress);
}

ECode CInet6Address::GetHostName(
    /* [out] */ String* hostName)
{
    VALIDATE_NOT_NULL(hostName);
    return InetAddress::GetHostName(hostName);
}

ECode CInet6Address::GetCanonicalHostName(
    /* [out] */ String* hostName)
{
    VALIDATE_NOT_NULL(hostName);
    return InetAddress::GetCanonicalHostName(hostName);
}

ECode CInet6Address::IsMulticastAddress(
    /* [out] */ Boolean* isMulticastAddress)
{
    VALIDATE_NOT_NULL(isMulticastAddress);
    // Multicast addresses are prefixed with 11111111 (255)
    *isMulticastAddress = (*mIpAddress)[0] == -1;
    return NOERROR;
}

ECode CInet6Address::IsLoopbackAddress(
    /* [out] */ Boolean* isLoopbackAddress)
{
    VALIDATE_NOT_NULL(isLoopbackAddress);
    // The last word must be 1
    if ((*mIpAddress)[15] != 1) {
        *isLoopbackAddress = FALSE;
        return NOERROR;
    }

    // All other words must be 0
    for (Int32 i = 0; i < 15; i++) {
        if ((*mIpAddress)[i] != 0) {
            *isLoopbackAddress = FALSE;
            return NOERROR;
        }
    }

    *isLoopbackAddress = TRUE;
    return NOERROR;
}

ECode CInet6Address::IsLinkLocalAddress(
    /* [out] */ Boolean* isLinkLocalAddress)
{
    VALIDATE_NOT_NULL(isLinkLocalAddress);
    // the first 10 bits need to be 1111111010 (1018)
    *isLinkLocalAddress = ((*mIpAddress)[0] == -2) &&
            ((UInt32)((*mIpAddress)[1] & 255) >> 6) == 2;
    return NOERROR;
}

ECode CInet6Address::IsSiteLocalAddress(
    /* [out] */ Boolean* isSiteLocalAddress)
{
    VALIDATE_NOT_NULL(isSiteLocalAddress);
    // the first 10 bits need to be 1111111011 (1019)
    *isSiteLocalAddress = ((*mIpAddress)[0] == -2) &&
            ((UInt32)((*mIpAddress)[1] & 255) >> 6) == 3;
    return NOERROR;
}

ECode CInet6Address::IsMCGlobal(
    /* [out] */ Boolean* isMCGlobal)
{
    VALIDATE_NOT_NULL(isMCGlobal);
    // the first byte should be 0xFF and the lower 4 bits
    // of the second byte should be 0xE
    *isMCGlobal = ((*mIpAddress)[0] == -1) && ((*mIpAddress)[1] & 15) == 14;
    return NOERROR;
}

ECode CInet6Address::IsMCNodeLocal(
    /* [out] */ Boolean* isMCNodeLocal)
{
    VALIDATE_NOT_NULL(isMCNodeLocal);
    // the first byte should be 0xFF and the lower 4 bits
    // of the second byte should be 0x1
    *isMCNodeLocal = ((*mIpAddress)[0] == -1) && ((*mIpAddress)[1] & 15) == 1;
    return NOERROR;
}

ECode CInet6Address::IsMCLinkLocal(
    /* [out] */ Boolean* isMCLinkLocal)
{
    VALIDATE_NOT_NULL(isMCLinkLocal);
    // the first byte should be 0xFF and the lower 4 bits
    // of the second byte should be 0x2
    *isMCLinkLocal = ((*mIpAddress)[0] == -1) && ((*mIpAddress)[1] & 15) == 2;
    return NOERROR;
}

ECode CInet6Address::IsMCSiteLocal(
    /* [out] */ Boolean* isMCSiteLocal)
{
    VALIDATE_NOT_NULL(isMCSiteLocal);
    // the first byte should be 0xFF and the lower 4 bits
    // of the second byte should be 0x5
    *isMCSiteLocal = ((*mIpAddress)[0] == -1) && ((*mIpAddress)[1] & 15) == 5;
    return NOERROR;
}

ECode CInet6Address::IsMCOrgLocal(
    /* [out] */ Boolean* isMCOrgLocal)
{
    VALIDATE_NOT_NULL(isMCOrgLocal);
    // the first byte should be 0xFF and the lower 4 bits
    // of the second byte should be 0x8
    *isMCOrgLocal = ((*mIpAddress)[0] == -1) && ((*mIpAddress)[1] & 15) == 8;
    return NOERROR;
}

ECode CInet6Address::IsAnyLocalAddress(
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

ECode CInet6Address::IsReachable(
    /* [in] */ Int32 timeout,
    /* [out] */ Boolean* isReachable)
{
    VALIDATE_NOT_NULL(isReachable);
    return InetAddress::IsReachable(timeout, isReachable);
}

ECode CInet6Address::IsReachableEx(
    /* [in] */ INetworkInterface* networkInterface,
    /* [in] */ Int32 ttl,
    /* [in] */ Int32 timeout,
    /* [out] */ Boolean* isReachable)
{
    VALIDATE_NOT_NULL(isReachable);
    return InetAddress::IsReachableEx(networkInterface, ttl, timeout, isReachable);
}

ECode CInet6Address::GetScopeId(
    /* [out] */ Int32* scopeId)
{
    VALIDATE_NOT_NULL(scopeId);
    if (mScopeIdSet) {
        *scopeId = mScopeId;
        return NOERROR;
    }
    *scopeId = 0;
    return NOERROR;
}

/**
 * Gets the network interface if this address is instanced with a scoped
 * network interface. Otherwise returns {@code null}.
 *
 * @return the scoped network interface of this address.
 */
ECode CInet6Address::GetScopedInterface(
    /* [out] */ INetworkInterface** scopedInterface)
{
    VALIDATE_NOT_NULL(scopedInterface);
    if (mScopeIfnameSet) {
        *scopedInterface = mScopedIf;
        if (*scopedInterface != NULL) (*scopedInterface)->AddRef();
        return NOERROR;
    }
    *scopedInterface = NULL;
    return NOERROR;
}

/**
 * Returns whether this address is IPv4 compatible or not. An IPv4
 * compatible address is prefixed with 96 bits of 0's. The last 32-bits are
 * varied corresponding with the 32-bit IPv4 address space.
 *
 * @return {@code true} if this instance represents an IPv4 compatible
 *         address, {@code false} otherwise.
 */
ECode CInet6Address::IsIPv4CompatibleAddress(
    /* [out] */ Boolean* isIPv4Compatible)
{
    VALIDATE_NOT_NULL(isIPv4Compatible);
    for (Int32 i = 0; i < 12; i++) {
        if ((*mIpAddress)[i] != 0) {
            *isIPv4Compatible = FALSE;
            return NOERROR;
        }
    }
    *isIPv4Compatible = TRUE;
    return NOERROR;
}


// private void writeObject(ObjectOutputStream stream) throws IOException {
//     ObjectOutputStream.PutField fields = stream.putFields();
//     if (ipaddress == null) {
//         fields.put("ipaddress", null);
//     } else {
//         fields.put("ipaddress", ipaddress);
//     }

//     fields.put("scope_id", scope_id);
//     fields.put("scope_id_set", scope_id_set);
//     fields.put("scope_ifname_set", scope_ifname_set);
//     fields.put("ifname", ifname);
//     stream.writeFields();
// }

// private void readObject(ObjectInputStream stream) throws IOException,
//         ClassNotFoundException {
//     ObjectInputStream.GetField fields = stream.readFields();
//     ipaddress = (byte[]) fields.get("ipaddress", null);
//     scope_id = fields.get("scope_id", 0);
//     scope_id_set = fields.get("scope_id_set", false);
//     ifname = (String) fields.get("ifname", null);
//     scope_ifname_set = fields.get("scope_ifname_set", false);
//     if (scope_ifname_set && null != ifname) {
//         scopedIf = NetworkInterface.getByName(ifname);
//     }
// }

// /**
//  * Returns a string containing a concise, human-readable description of this
//  * IP address.
//  *
//  * @return the description, as host/address.
//  */
// @Override
// public String toString() {
//     if (ifname != null) {
//         return super.toString() + "%" + ifname;
//     }
//     if (scope_id != 0) {
//         return super.toString() + "%" + scope_id;
//     }
//     return super.toString();
// }

ECode CInet6Address::constructor(
    /* [in] */ const ArrayOf<Byte>& address)
{
    mFamily = AF_INET6;
    mIpAddress = const_cast<ArrayOf<Byte>*>(&address);
    mScopeId = 0;
    return NOERROR;
}

ECode CInet6Address::constructor(
    /* [in] */ const ArrayOf<Byte>& address,
    /* [in] */ const String& name)
{
    mFamily = AF_INET6;
    mHostName = name;
    mIpAddress = const_cast<ArrayOf<Byte>*>(&address);
    mScopeId = 0;
    return NOERROR;
}

/**
 * Constructs an {@code InetAddress} representing the {@code address} and
 * {@code name} and {@code scope_id}.
 *
 * @param address
 *            the network address.
 * @param name
 *            the name associated with the address.
 * @param scope_id
 *            the scope id for link- or site-local addresses.
 */
ECode CInet6Address::constructor(
    /* [in] */ const ArrayOf<Byte>& address,
    /* [in] */ const String& name,
    /* [in] */ Int32 scopeId)
{
    mFamily = AF_INET6;
    mHostName = name;
    mIpAddress = const_cast<ArrayOf<Byte>*>(&address);
    mScopeId = scopeId;
    if (scopeId != 0) {
        mScopeIdSet = TRUE;
    }
    return NOERROR;
}

/**
 * Constructs an {@code InetAddress} representing the {@code address} and
 * {@code scope_id}.
 *
 * @param address
 *            the network address.
 * @param scope_id
 *            the scope id for link- or site-local addresses.
 */
ECode CInet6Address::constructor(
    /* [in] */ const ArrayOf<Byte>& address,
    /* [in] */ Int32 scopeId)
{
    mIpAddress = const_cast<ArrayOf<Byte>*>(&address);
    mScopeId = scopeId;
    if (scopeId != 0) {
        mScopeIdSet = TRUE;
    }
    return NOERROR;
}
