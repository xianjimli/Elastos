
#include "cmdef.h"


#include "InetAddress.h"
#include "CInet4Address.h"
#include "CInet6Address.h"
#include "NetworkUtilities.h"
#include <Droid.System.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>


// {6E01A794-9ED6-4D1C-BBD5-A9D8855C9D42}
extern "C" const InterfaceID EIID_InetAddress =
    { 0x6e01a794, 0x9ed6, 0x4d1c, { 0xbb, 0xd5, 0xa9, 0xd8, 0x85, 0x5c, 0x9d, 0x42 } };


AddressCache* InitAddressCache()
{
    return new AddressCache;
}

AutoPtr<AddressCache> InetAddress::ADDRESS_CACHE = InitAddressCache();

AutoPtr<INetworkSystem> InetAddress::NETIMPL;

const CString InetAddress::ERRMSG_CONNECTION_REFUSED = "Connection refused";


/**
 * Constructs an {@code InetAddress}.
 *
 * Note: this constructor should not be used. Creating an InetAddress
 * without specifying whether it's an IPv4 or IPv6 address does not make
 * sense, because subsequent code cannot know which of of the subclasses'
 * methods need to be called to implement a given InetAddress method. The
 * proper way to create an InetAddress is to call new Inet4Address or
 * Inet6Address or to use one of the static methods that return
 * InetAddresses (e.g., getByAddress). That is why the API does not have
 * public constructors for any of these classes.
 */
InetAddress::InetAddress()
    : mFamily(0)
    , mIpAddress(NULL)
    , mReached(FALSE)
{}

InetAddress::~InetAddress()
{
    if (mIpAddress != NULL) ArrayOf<Byte>::Free(mIpAddress);
}

/**
 * Compares this {@code InetAddress} instance against the specified address
 * in {@code obj}. Two addresses are equal if their address byte arrays have
 * the same length and if the bytes in the arrays are equal.
 *
 * @param obj
 *            the object to be tested for equality.
 * @return {@code true} if both objects are equal, {@code false} otherwise.
 */
//@Override
//public boolean equals(Object obj) {
//    if (!(obj instanceof InetAddress)) {
//        return false;
//    }
//    return Arrays.equals(this.ipaddress, ((InetAddress) obj).ipaddress);
//}

/**
 * Returns the IP address represented by this {@code InetAddress} instance
 * as a byte array. The elements are in network order (the highest order
 * address byte is in the zeroth element).
 *
 * @return the address in form of a byte array.
 */
ECode InetAddress::GetAddress(
    /* [out, callee] */ ArrayOf<Byte>** address)
{
    *address = mIpAddress->Clone();
    return NOERROR;
}

static void BubbleSortWithShortestFirst(ArrayOf<ArrayOf<Byte>*>* rawAddresses)
{
    Int32 i, j, N;
    N = rawAddresses->GetLength();
    for (i = 0; i < N; ++i) {
        Boolean exchanged = FALSE;
        for (j = N - 1; j <= i; --j) {
            ArrayOf<Byte>* tmp;
            if ((*rawAddresses)[j - 1]->GetLength() >
                (*rawAddresses)[j]->GetLength()) {
                tmp = (*rawAddresses)[j - 1];
                (*rawAddresses)[j - 1] = (*rawAddresses)[j];
                (*rawAddresses)[j] = tmp;
                exchanged = TRUE;
            }
        }
        if (!exchanged) return;
    }
}

/**
 * Converts an array of byte arrays representing raw IP addresses of a host
 * to an array of InetAddress objects, sorting to respect the value of the
 * system property {@code "java.net.preferIPv6Addresses"}.
 *
 * @param rawAddresses the raw addresses to convert.
 * @param hostName the hostname corresponding to the IP address.
 * @return the corresponding InetAddresses, appropriately sorted.
 */
ECode InetAddress::BytesToInetAddresses(
    /* [in] */ ArrayOf<ArrayOf<Byte>*>* rawAddresses,
    /* [in] */ const String& hostName,
    /* [out, callee] */ ArrayOf<IInetAddress*>** addresses)
{
    // If we prefer IPv4, ignore the RFC3484 ordering we get from getaddrinfo
    // and always put IPv4 addresses first. Arrays.sort() is stable, so the
    // internal ordering will not be changed.
    if (!PreferIPv6Addresses()) {
        //bubble  sort
        BubbleSortWithShortestFirst(rawAddresses);
    }

    // Convert the byte arrays to InetAddresses.
    *addresses = ArrayOf<IInetAddress*>::Alloc(rawAddresses->GetLength());
    for (Int32 i = 0; i < rawAddresses->GetLength(); i++) {
        ArrayOf<Byte>* rawAddress = (*rawAddresses)[i];
        if (rawAddress->GetLength() == 16) {
            CInet6Address* inet6;
            CInet6Address::NewByFriend(*rawAddress, hostName, &inet6);
            (**addresses)[i] = (IInetAddress*)inet6->Probe(EIID_IInetAddress);
        }
        else if (rawAddress->GetLength() == 4) {
            CInet4Address* inet4;
            CInet4Address::NewByFriend(*rawAddress, hostName, &inet4);
            (**addresses)[i] = (IInetAddress*)inet4->Probe(EIID_IInetAddress);
        }
        else {
          // Cannot happen, because the underlying code only returns
          // addresses that are 4 or 16 bytes long.
//          throw new AssertionError("Impossible address length " +
//                                   rawAddress.length);
            return E_ASSERTION_ERROR;
        }
    }
    return NOERROR;
}

/**
 * Gets all IP addresses associated with the given {@code host} identified
 * by name or literal IP address. The IP address is resolved by the
 * configured name service. If the host name is empty or {@code null} an
 * {@code UnknownHostException} is thrown. If the host name is a literal IP
 * address string an array with the corresponding single {@code InetAddress}
 * is returned.
 *
 * @param host the hostname or literal IP string to be resolved.
 * @return the array of addresses associated with the specified host.
 * @throws UnknownHostException if the address lookup fails.
 */
ECode InetAddress::GetAllByName(
    /* [in] */ const String& host,
    /* [out, callee] */ ArrayOf<IInetAddress*>** addresses)
{
    return GetAllByNameImpl(host, addresses);
}

/**
 * Returns the InetAddresses for {@code host}. The returned array is shared
 * and must be cloned before it is returned to application code.
 */
ECode InetAddress::GetAllByNameImpl(
    /* [in] */ const String& host,
    /* [out, callee] */ ArrayOf<IInetAddress*>** addresses)
{
    if (host.IsNullOrEmpty()) {
        if (PreferIPv6Addresses()) {
            *addresses = ArrayOf<IInetAddress*>::Alloc(2);
            (**addresses)[0] = CInet6Address::ANY.Get();
            (**addresses)[0]->AddRef();
            (**addresses)[1] = CInet4Address::ANY.Get();
            (**addresses)[1]->AddRef();
            return NOERROR;
        }
        else {
            *addresses = ArrayOf<IInetAddress*>::Alloc(2);
            (**addresses)[0] = CInet4Address::LOOPBACK.Get();
            (**addresses)[0]->AddRef();
            (**addresses)[1] = CInet6Address::LOOPBACK.Get();
            (**addresses)[1]->AddRef();
            return NOERROR;
        }
    }

    // Special-case "0" for legacy IPv4 applications.
    if (host.Equals("0")) {
        *addresses = ArrayOf<IInetAddress*>::Alloc(1);
        (**addresses)[0] = CInet4Address::ANY.Get();
        (**addresses)[0]->AddRef();
        return NOERROR;
    }

//    try {
    ArrayOf<Byte>* hBytes;
    IpStringToByteArray(host, &hBytes);
    if (hBytes->GetLength() == 4) {
        IInet4Address* addr;
        CInet4Address::New(*hBytes, &addr);
        *addresses = ArrayOf<IInetAddress*>::Alloc(1);
        (**addresses)[0] = (IInetAddress*)addr;
        (**addresses)[0]->AddRef();
        return NOERROR;
    }
    else if (hBytes->GetLength() == 16) {
        IInet6Address* addr;
        CInet6Address::New(*hBytes, &addr);
        *addresses = ArrayOf<IInetAddress*>::Alloc(1);
        (**addresses)[0] = (IInetAddress*)addr;
        (**addresses)[0]->AddRef();
        return NOERROR;
    }
    else {
//        throw new UnknownHostException(wrongAddressLength());
    }
//    } catch (UnknownHostException e) {
//    }

//    SecurityManager security = System.getSecurityManager();
//    if (security != null) {
//        security.checkConnect(host, -1);
//    }

    return LookupHostByName(host, addresses);
}

ECode InetAddress::ByteArrayToIpString(
    /* [in] */ const ArrayOf<Byte>& address,
    /* [out] */ String* ipString)
{
    sockaddr_storage ss;
    ECode ec = ByteArrayToSocketAddress(address, 0, &ss);
    if (FAILED(ec)) {
        *ipString = NULL;
        return ec;
    }
    // TODO: getnameinfo seems to want its length parameter to be exactly
    // sizeof(sockaddr_in) for an IPv4 address and sizeof (sockaddr_in6) for an
    // IPv6 address. Fix getnameinfo so it accepts sizeof(sockaddr_storage), and
    // then remove this hack.
    Int32 sa_size;
    if (ss.ss_family == AF_INET) {
        sa_size = sizeof(sockaddr_in);
    }
    else if (ss.ss_family == AF_INET6) {
        sa_size = sizeof(sockaddr_in6);
    }
    else {
        // ByteArrayToSocketAddress already threw.
        *ipString = NULL;
        return NOERROR;
    }
    Char8 _ipString[INET6_ADDRSTRLEN];
    Int32 rc = getnameinfo(reinterpret_cast<sockaddr*>(&ss), sa_size,
            _ipString, sizeof(_ipString), NULL, 0, NI_NUMERICHOST);
    if (rc != 0) {
//        jniThrowException(env, "java/net/UnknownHostException", gai_strerror(rc));
        *ipString = NULL;
        return E_UNKNOWN_HOST_EXCEPTION;
    }
    *ipString = _ipString;
    return NOERROR;
}

/**
 * Convert a Java string representing an IP address to a Java byte array.
 * The formats accepted are:
 * - IPv4:
 *   - 1.2.3.4
 *   - 1.2.4
 *   - 1.4
 *   - 4
 * - IPv6
 *   - Compressed form (2001:db8::1)
 *   - Uncompressed form (2001:db8:0:0:0:0:0:1)
 *   - IPv4-compatible (::192.0.2.0)
 *   - With an embedded IPv4 address (2001:db8::192.0.2.0).
 * IPv6 addresses may appear in square brackets.
 *
 * @param addressByteArray the byte array to convert.
 * @return a string with the textual representation of the address.
 * @throws UnknownHostException the IP address was invalid.
 */
ECode InetAddress::IpStringToByteArray(
    /* [in] */ const String& ipString,
    /* [out, callee] */ ArrayOf<Byte>** address)
{
    ECode ec = NOERROR;

    // Convert the String to UTF-8 bytes.
    if (ipString.IsNull()) {
        *address = NULL;
        return ec;
    }
    size_t byteCount = ipString.GetSize();
    Char8* _ipStr = (Char8*)malloc(byteCount + 1);
    strcpy(_ipStr, (const char*)ipString);

    // Accept IPv6 addresses (only) in square brackets for compatibility.
    if (_ipStr[0] == '[' && _ipStr[byteCount - 1] == ']' && strchr(_ipStr, ':') != NULL) {
        memmove(_ipStr, _ipStr + 1, byteCount - 2);
        _ipStr[byteCount - 2] = '\0';
    }

    *address = NULL;
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_NUMERICHOST;

    sockaddr_storage ss;
    memset(&ss, 0, sizeof(ss));

    addrinfo* res = NULL;
    Int32 ret = getaddrinfo(_ipStr, NULL, &hints, &res);
    if (ret == 0 && res) {
        // Convert IPv4-mapped addresses to IPv4 addresses.
        // The RI states "Java will never return an IPv4-mapped address".
        sockaddr_in6* sin6 = reinterpret_cast<sockaddr_in6*>(res->ai_addr);
        if (res->ai_family == AF_INET6 && IN6_IS_ADDR_V4MAPPED(&sin6->sin6_addr)) {
            sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(&ss);
            sin->sin_family = AF_INET;
            sin->sin_port = sin6->sin6_port;
            memcpy(&sin->sin_addr.s_addr, &sin6->sin6_addr.s6_addr[12], 4);
            *address = ArrayOf<Byte>::Alloc(4);
            ec = SocketAddressToByteArray(&ss, *address);
        }
        else {
            sockaddr_storage* _ss = reinterpret_cast<sockaddr_storage*>(res->ai_addr);
            if (_ss->ss_family == AF_INET) *address = ArrayOf<Byte>::Alloc(4);
            else if (_ss->ss_family == AF_INET6) *address = ArrayOf<Byte>::Alloc(16);
            ec = SocketAddressToByteArray(_ss, *address);
        }
    }
    else {
        // For backwards compatibility, deal with address formats that
        // getaddrinfo does not support. For example, 1.2.3, 1.3, and even 3 are
        // valid IPv4 addresses according to the Java API. If getaddrinfo fails,
        // try to use inet_aton.
        sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(&ss);
        if (inet_aton(ipString, &sin->sin_addr)) {
            sin->sin_family = AF_INET;
            sin->sin_port = 0;
            *address = ArrayOf<Byte>::Alloc(4);
            ec = SocketAddressToByteArray(&ss, *address);
        }
    }

    if (res) {
        freeaddrinfo(res);
    }

    if (*address == NULL) {
//        jniThrowException(env, "java/net/UnknownHostException", gai_strerror(ret));
        ec = E_UNKNOWN_HOST_EXCEPTION;
    }

    if (FAILED(ec) && (*address != NULL))
    {
        ArrayOf<Byte>::Free(*address);
        *address = NULL;
    }

    free(_ipStr);
    return NOERROR;
}

CString InetAddress::WrongAddressLength()
{
    return "Invalid IP Address is neither 4 or 16 bytes";
}

Boolean InetAddress::PreferIPv6Addresses()
{
//    String propertyName = "java.net.preferIPv6Addresses";
//    String propertyValue = AccessController.doPrivileged(new PriviAction<String>(propertyName));
//    return Boolean.parseBoolean(propertyValue);
    return FALSE;
}

/**
 * Returns the address of a host according to the given host string name
 * {@code host}. The host string may be either a machine name or a dotted
 * string IP address. If the latter, the {@code hostName} field is
 * determined upon demand. {@code host} can be {@code null} which means that
 * an address of the loopback interface is returned.
 *
 * @param host
 *            the hostName to be resolved to an address or {@code null}.
 * @return the {@code InetAddress} instance representing the host.
 * @throws UnknownHostException
 *             if the address lookup fails.
 */
ECode InetAddress::GetByName(
        /* [in] */ const String& host,
        /* [out] */ IInetAddress** address)
{
    ArrayOf<IInetAddress*>* addresses;
    FAIL_RETURN(GetAllByNameImpl(host, &addresses));
    *address = (*addresses)[0];
    (*address)->AddRef();
    for(Int32 i = 0; i < addresses->GetLength(); ++i) {
        (*addresses)[i]->Release();
    }
    ArrayOf<IInetAddress*>::Free(addresses);

    return NOERROR;
}

/**
 * Gets the textual representation of this IP address.
 *
 * @return the textual representation of host's IP address.
 */
ECode InetAddress::GetHostAddress(
    /* [out] */ String* address)
{
    return ByteArrayToIpString(*mIpAddress, address);
}

/**
 * Gets the host name of this IP address. If the IP address could not be
 * resolved, the textual representation in a dotted-quad-notation is
 * returned.
 *
 * @return the corresponding string name of this IP address.
 */
ECode InetAddress::GetHostName(
    /* [out] */ String* name)
{
//    try {
    if (mHostName.IsNull()) {
        Int32 address = 0;
        if (mIpAddress->GetLength() == 4) {
            address = BytesToInt32(*mIpAddress, 0);
            if (address == 0) {
                FAIL_RETURN(ByteArrayToIpString(*mIpAddress, &mHostName));
                *name = mHostName;
                return NOERROR;
            }
        }
        AutoPtr<IInetAddress> netAddr;
        GetHostByAddrImpl(*mIpAddress, (IInetAddress**)&netAddr);
        mHostName = ((InetAddress*)netAddr->Probe(EIID_InetAddress))->mHostName;
        if (mHostName.Equals("localhost") && mIpAddress->GetLength() == 4
                && address != 0x7f000001) {
            FAIL_RETURN(ByteArrayToIpString(*mIpAddress, &mHostName));
            *name = mHostName;
            return NOERROR;
        }
    }
//    } catch (UnknownHostException e) {
//        return hostName = byteArrayToIpString(ipaddress);
//    }
//    SecurityManager security = System.getSecurityManager();
//    try {
//        // Only check host names, not addresses
//        if (security != null && isHostName(hostName)) {
//            security.checkConnect(hostName, -1);
//        }
//    } catch (SecurityException e) {
//        return byteArrayToIpString(ipaddress);
//    }
    *name = mHostName;
    return NOERROR;
}

/**
 * Gets the fully qualified domain name for the host associated with this IP
 * address. If a security manager is set, it is checked if the method caller
 * is allowed to get the hostname. Otherwise, the textual representation in
 * a dotted-quad-notation is returned.
 *
 * @return the fully qualified domain name of this IP address.
 */
ECode InetAddress::GetCanonicalHostName(
    /* [out] */ String* canonicalName)
{
//    try {
    Int32 address = 0;
    if (mIpAddress->GetLength() == 4) {
        address = BytesToInt32(*mIpAddress, 0);
        if (address == 0) {
            return ByteArrayToIpString(*mIpAddress, canonicalName);
        }
    }
    AutoPtr<IInetAddress> netAddr;
    GetHostByAddrImpl(*mIpAddress, (IInetAddress**)&netAddr);
    *canonicalName = ((InetAddress*)netAddr->Probe(EIID_InetAddress))->mHostName;

//    } catch (UnknownHostException e) {
//        return byteArrayToIpString(ipaddress);
//    }
//    SecurityManager security = System.getSecurityManager();
//    try {
//        // Only check host names, not addresses
//        if (security != null && isHostName(canonicalName)) {
//            security.checkConnect(canonicalName, -1);
//        }
//    } catch (SecurityException e) {
//        return byteArrayToIpString(ipaddress);
//    }
    return NOERROR;
}

/**
 * Returns an {@code InetAddress} for the local host if possible, or the
 * loopback address otherwise. This method works by getting the hostname,
 * performing a DNS lookup, and then taking the first returned address.
 * For devices with multiple network interfaces and/or multiple addresses
 * per interface, this does not necessarily return the {@code InetAddress}
 * you want.
 *
 * <p>Multiple interface/address configurations were relatively rare
 * when this API was designed, but multiple interfaces are the default for
 * modern mobile devices (with separate wifi and radio interfaces), and
 * the need to support both IPv4 and IPv6 has made multiple addresses
 * commonplace. New code should thus avoid this method except where it's
 * basically being used to get a loopback address or equivalent.
 *
 * <p>There are two main ways to get a more specific answer:
 * <ul>
 * <li>If you have a connected socket, you should probably use
 * {@link Socket#getLocalAddress} instead: that will give you the address
 * that's actually in use for that connection. (It's not possible to ask
 * the question "what local address would a connection to a given remote
 * address use?"; you have to actually make the connection and see.)</li>
 * <li>For other use cases, see {@link NetworkInterface}, which lets you
 * enumerate all available network interfaces and their addresses.</li>
 * </ul>
 *
 * <p>Note that if the host doesn't have a hostname set&nbsp;&ndash; as
 * Android devices typically don't&nbsp;&ndash; this method will
 * effectively return the loopback address, albeit by getting the name
 * {@code localhost} and then doing a lookup to translate that to
 * {@code 127.0.0.1}.
 *
 * @return an {@code InetAddress} representing the local host, or the
 * loopback address.
 * @throws UnknownHostException
 *             if the address lookup fails.
 */
ECode InetAddress::GetLocalHost(
    /* [out] */ IInetAddress** address)
{
    String host = Gethostname();
//    SecurityManager security = System.getSecurityManager();
//    try {
//        if (security != null) {
//            security.checkConnect(host, -1);
//        }
//    } catch (SecurityException e) {
//        return Inet4Address.LOOPBACK;
//    }
    ArrayOf<IInetAddress*>* addresses;
    FAIL_RETURN(LookupHostByName(host, &addresses));
    *address = (*addresses)[0];
    (*address)->AddRef();
    for(Int32 i = 0; i < addresses->GetLength(); ++i) {
        (*addresses)[i]->Release();
    }
    ArrayOf<IInetAddress*>::Free(addresses);

    return NOERROR;
}

String InetAddress::Gethostname()
{
    Char8 name[256];
    Int32 r = gethostname(name, 256);
    if (r == 0) {
        return String(name);
    }
    else {
        return String(NULL);
    }
}

/**
 * Gets the hashcode of the represented IP address.
 *
 * @return the appropriate hashcode value.
 */
//@Override
//public int hashCode() {
//    return Arrays.hashCode(ipaddress);
//}

/*
 * Returns whether this address is an IP multicast address or not. This
 * implementation returns always {@code false}.
 *
 * @return {@code true} if this address is in the multicast group, {@code
 *         false} otherwise.
 */
ECode InetAddress::IsMulticastAddress(
    /* [out] */ Boolean* isMulticastAddress)
{
    *isMulticastAddress = FALSE;
    return NOERROR;
}

/**
 * Resolves a hostname to its IP addresses using a cache.
 *
 * @param host the hostname to resolve.
 * @return the IP addresses of the host.
 */
ECode InetAddress::LookupHostByName(
    /* [in] */ const String& host,
    /* [out, callee] */ ArrayOf<IInetAddress*>** addresses)
{
    AutoPtr<IBlockGuard> blockGuard;
    AutoPtr<IPolicy> policy;
    CBlockGuard::AcquireSingleton((IBlockGuard**)&blockGuard);
    blockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnNetwork());
    // Do we have a result cached?
    ArrayOf<IInetAddress*>* cachedResult = ADDRESS_CACHE->Get(host);
    if (cachedResult != NULL) {
        if (cachedResult->GetLength() > 0) {
            // A cached positive result.
            *addresses = cachedResult->Clone();
            for (Int32 i = 0; i < (*addresses)->GetLength(); ++i) {
                (**addresses)[i]->AddRef();
            }
            return NOERROR;
        }
        else {
            // A cached negative result.
//            throw new UnknownHostException(host);
            return E_UNKNOWN_HOST_EXCEPTION;
        }
    }
//    try {
    ArrayOf<ArrayOf<Byte>*>* addrInfos = NULL;
    ECode ec = GetAddrInfo(host, &addrInfos);
    if (SUCCEEDED(ec)) {
        ec = BytesToInetAddresses(addrInfos, host, addresses);
    }
    if (FAILED(ec)) {
        if (addrInfos != NULL) {
            for (Int32 i = 0; i < addrInfos->GetLength(); ++i) {
                ArrayOf<Byte>::Free((*addrInfos)[i]);
            }
        }
        ArrayOf<ArrayOf<Byte>*>::Free(addrInfos);
        ADDRESS_CACHE->PutUnknownHost(host);
        return E_UNKNOWN_HOST_EXCEPTION;
    }
    ADDRESS_CACHE->Put(host, *addresses);
    *addresses = (*addresses)->Clone();
    for (Int32 i = 0; i < (*addresses)->GetLength(); ++i) {
        (**addresses)[i]->AddRef();
    }
    return NOERROR;
//    } catch (UnknownHostException e) {
//        addressCache.putUnknownHost(host);
//        throw new UnknownHostException(host);
//    }
}

#if LOG_DNS
static void LogIpString(addrinfo* ai, const char* name)
{
    char ipString[INET6_ADDRSTRLEN];
    int result = getnameinfo(ai->ai_addr, ai->ai_addrlen, ipString,
                             sizeof(ipString), NULL, 0, NI_NUMERICHOST);
    if (result == 0) {
//        LOGD("%s: %s (family %d, proto %d)", name, ipString, ai->ai_family,
//             ai->ai_protocol);
    } else {
//        LOGE("%s: getnameinfo: %s", name, gai_strerror(result));
    }
}
#else
static inline void LogIpString(addrinfo*, const char*)
{
}
#endif

ECode InetAddress::GetAddrInfo(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<ArrayOf<Byte>*>** addrInfos)
{
    ECode ec = NOERROR;

    if (name.IsNull()) {
        *addrInfos = NULL;
        return NOERROR;
    }

    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_ADDRCONFIG;
    /*
     * If we don't specify a socket type, every address will appear twice, once
     * for SOCK_STREAM and one for SOCK_DGRAM. Since we do not return the family
     * anyway, just pick one.
     */
    hints.ai_socktype = SOCK_STREAM;

    addrinfo* addressList = NULL;
    *addrInfos = NULL;
    Int32 result = getaddrinfo((const char*)name, NULL, &hints, &addressList);
    if (result == 0 && addressList) {
        // Count results so we know how to size the output array.
        Int32 addressCount = 0;
        for (addrinfo* ai = addressList; ai != NULL; ai = ai->ai_next) {
            if (ai->ai_family == AF_INET || ai->ai_family == AF_INET6) {
                addressCount++;
            }
        }

        // Prepare output array.
        *addrInfos = ArrayOf<ArrayOf<Byte>*>::Alloc(addressCount);
        if (*addrInfos == NULL) {
            // Appropriate exception will be thrown.
//            LOGE("getaddrinfo: could not allocate array of size %i", addressCount);
            freeaddrinfo(addressList);
            return E_OUT_OF_MEMORY_ERROR;
        }

        // Examine returned addresses one by one, save them in the output array.
        Int32 index = 0;
        for (addrinfo* ai = addressList; ai != NULL; ai = ai->ai_next) {
            sockaddr* address = ai->ai_addr;
            size_t addressLength = 0;
            void* rawAddress;

            switch (ai->ai_family) {
                // Find the raw address length and start pointer.
                case AF_INET6:
                    addressLength = 16;
                    rawAddress = &reinterpret_cast<sockaddr_in6*>(address)->sin6_addr.s6_addr;
                    LogIpString(ai, (const char*)name);
                    break;
                case AF_INET:
                    addressLength = 4;
                    rawAddress = &reinterpret_cast<sockaddr_in*>(address)->sin_addr.s_addr;
                    LogIpString(ai, (const char*)name);
                    break;
                default:
                    // Unknown address family. Skip this address.
//                    LOGE("getaddrinfo: Unknown address family %d", ai->ai_family);
                    continue;
            }

            // Convert each IP address into a Java byte array.
            ArrayOf<Byte>* byteArray = ArrayOf<Byte>::Alloc(addressLength);
            if (byteArray == NULL) {
                // Out of memory error will be thrown on return.
//                LOGE("getaddrinfo: Can't allocate %d-byte array", addressLength);
                for (Int32 i = 0; i < index; ++i) {
                    ArrayOf<Byte>::Free((**addrInfos)[i]);
                }
                ArrayOf<ArrayOf<Byte>*>::Free(*addrInfos);
                *addrInfos = NULL;
                break;
            }
            memcpy(byteArray->GetPayload(), rawAddress, addressLength);
            (**addrInfos)[index] = byteArray;
            index++;
        }
    }
    else if (result == EAI_SYSTEM && errno == EACCES) {
        /* No permission to use network */
//        jniThrowException(env, "java/lang/SecurityException",
//            "Permission denied (maybe missing INTERNET permission)");
//        ec = E_SECURITY_EXCEPTION;
    }
    else {
//        jniThrowException(env, "java/net/UnknownHostException", gai_strerror(result));
        ec = E_UNKNOWN_HOST_EXCEPTION;
    }

    if (addressList) {
        freeaddrinfo(addressList);
    }

    return ec;
}

/**
 * Query the IP stack for the host address. The host is in address form.
 *
 * @param addr
 *            the host address to lookup.
 * @throws UnknownHostException
 *             if an error occurs during lookup.
 */
ECode InetAddress::GetHostByAddrImpl(
    /* [in] */ const ArrayOf<Byte>& addr,
    /* [out] */ IInetAddress** address)
{
    AutoPtr<IBlockGuard> blockGuard;
    AutoPtr<IPolicy> policy;
    CBlockGuard::AcquireSingleton((IBlockGuard**)&blockGuard);
    blockGuard->GetThreadPolicy((IPolicy**)&policy);
    FAIL_RETURN(policy->OnNetwork());
    if (addr.GetLength() == 4) {
        String hostName;
        FAIL_RETURN(GetNameInfo(addr, &hostName));
        CInet4Address* _4addr;
        CInet4Address::NewByFriend(addr, hostName, &_4addr);
        *address = (IInetAddress*)_4addr->Probe(EIID_IInetAddress);
        return NOERROR;
    }
    else if (addr.GetLength() == 16) {
        String hostName;
        FAIL_RETURN(GetNameInfo(addr, &hostName));
        CInet6Address* _6addr;
        CInet6Address::NewByFriend(addr, hostName, &_6addr);
        *address = (IInetAddress*)_6addr->Probe(EIID_IInetAddress);
        return NOERROR;
    }
    else {
        *address = NULL;
//        throw new UnknownHostException(wrongAddressLength());
        return E_UNKNOWN_HOST_EXCEPTION;
    }
}

/**
 * Looks up the name corresponding to an IP address.
 *
 * @param javaAddress: a byte array containing the raw IP address bytes. Must be
 *         4 or 16 bytes long.
 * @return the hostname.
 * @throws UnknownHostException: the IP address has no associated hostname.
 */
ECode InetAddress::GetNameInfo(
    /* [in] */ const ArrayOf<Byte>& addr,
    /* [out] */ String* hostname)
{
    // Convert the raw address bytes into a socket address structure.
    sockaddr_storage ss;
    memset(&ss, 0, sizeof(ss));

    size_t socklen;
    const size_t addressLength = addr.GetLength();
    if (addressLength == 4) {
        sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(&ss);
        sin->sin_family = AF_INET;
        socklen = sizeof(sockaddr_in);
        Byte* dst = reinterpret_cast<Byte*>(&sin->sin_addr.s_addr);
        memcpy(dst, addr.GetPayload(), 4);
    }
    else if (addressLength == 16) {
        sockaddr_in6* sin6 = reinterpret_cast<sockaddr_in6*>(&ss);
        sin6->sin6_family = AF_INET6;
        socklen = sizeof(sockaddr_in6);
        Byte* dst = reinterpret_cast<Byte*>(&sin6->sin6_addr.s6_addr);
        memcpy(dst, addr.GetPayload(), 16);
    }
    else {
        // The caller already throws an exception in this case. Don't worry
        // about it here.
        *hostname = NULL;
        return NOERROR;
    }

    // Look up the host name from the IP address.
    Char8 name[NI_MAXHOST];
    Int32 ret = getnameinfo(reinterpret_cast<sockaddr*>(&ss), socklen,
                          name, sizeof(name), NULL, 0, NI_NAMEREQD);
    if (ret != 0) {
//        jniThrowException(env, "java/net/UnknownHostException", gai_strerror(ret));
        *hostname = NULL;
        return E_UNKNOWN_HOST_EXCEPTION;
    }

    *hostname = name;
    return NOERROR;
}

ECode InetAddress::GetHostNameInternal(
        /* [in] */ const String& host,
        /* [in] */ Boolean isCheck,
        /* [out] */ String* hostName)
{
    if (host.IsNullOrEmpty()) {
        return CInet4Address::LOOPBACK->GetHostAddress(hostName);
    }
    if (IsHostName(host)) {
        if (isCheck) {
//            SecurityManager sm = System.getSecurityManager();
//            if (sm != null) {
//                sm.checkConnect(host, -1);
//            }
        }
        ArrayOf<IInetAddress*>* addresses;
        FAIL_RETURN(LookupHostByName(host, &addresses));
        ECode ec = (*addresses)[0]->GetHostAddress(hostName);
        for(Int32 i = 0; i < addresses->GetLength(); ++i) {
            (*addresses)[i]->Release();
        }
        ArrayOf<IInetAddress*>::Free(addresses);

        return ec;
    }
    *hostName = host;
    return NOERROR;
}

/**
 * Returns a string containing a concise, human-readable description of this
 * IP address.
 *
 * @return the description, as host/address.
 */
//@Override
//public String toString() {
//    return (hostName == null ? "" : hostName) + "/" + getHostAddress();
//}

/**
 * Returns true if the string is a host name, false if it is an IP Address.
 */
Boolean InetAddress::IsHostName(
    /* [in] */ const String& value)
{
    ArrayOf<Byte>* address;
    if (SUCCEEDED(IpStringToByteArray(value, &address))) {
        ArrayOf<Byte>::Free(address);
        return FALSE;
    }
    else return TRUE;
}

/**
 * Returns whether this address is a loopback address or not. This
 * implementation returns always {@code false}. Valid IPv4 loopback
 * addresses are 127.d.d.d The only valid IPv6 loopback address is ::1.
 *
 * @return {@code true} if this instance represents a loopback address,
 *         {@code false} otherwise.
 */
ECode InetAddress::IsLoopbackAddress(
    /* [out] */ Boolean* isLoopbackAddress)
{
    *isLoopbackAddress = FALSE;
    return NOERROR;
}

/**
 * Returns whether this address is a link-local address or not. This
 * implementation returns always {@code false}.
 * <p>
 * Valid IPv6 link-local addresses are FE80::0 through to
 * FEBF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF.
 * <p>
 * There are no valid IPv4 link-local addresses.
 *
 * @return {@code true} if this instance represents a link-local address,
 *         {@code false} otherwise.
 */
ECode InetAddress::IsLinkLocalAddress(
    /* [out] */ Boolean* isLinkLocalAddress)
{
    *isLinkLocalAddress = FALSE;
    return NOERROR;
}

/**
 * Returns whether this address is a site-local address or not. This
 * implementation returns always {@code false}.
 * <p>
 * Valid IPv6 site-local addresses are FEC0::0 through to
 * FEFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF.
 * <p>
 * There are no valid IPv4 site-local addresses.
 *
 * @return {@code true} if this instance represents a site-local address,
 *         {@code false} otherwise.
 */
ECode InetAddress::IsSiteLocalAddress(
    /* [out] */ Boolean* isSiteLocalAddress)
{
    *isSiteLocalAddress = FALSE;
    return NOERROR;
}

/**
 * Returns whether this address is a global multicast address or not. This
 * implementation returns always {@code false}.
 * <p>
 * Valid IPv6 link-global multicast addresses are FFxE:/112 where x is a set
 * of flags, and the additional 112 bits make up the global multicast
 * address space.
 * <p>
 * Valid IPv4 global multicast addresses are between: 224.0.1.0 to
 * 238.255.255.255.
 *
 * @return {@code true} if this instance represents a global multicast
 *         address, {@code false} otherwise.
 */
ECode InetAddress::IsMCGlobal(
    /* [out] */ Boolean* isMCGlobal)
{
    *isMCGlobal = FALSE;
    return NOERROR;
}

/**
 * Returns whether this address is a node-local multicast address or not.
 * This implementation returns always {@code false}.
 * <p>
 * Valid IPv6 node-local multicast addresses are FFx1:/112 where x is a set
 * of flags, and the additional 112 bits make up the node-local multicast
 * address space.
 * <p>
 * There are no valid IPv4 node-local multicast addresses.
 *
 * @return {@code true} if this instance represents a node-local multicast
 *         address, {@code false} otherwise.
 */
ECode InetAddress::IsMCNodeLocal(
    /* [out] */ Boolean* isMCNodeLocal)
{
    *isMCNodeLocal = FALSE;
    return NOERROR;
}

/**
 * Returns whether this address is a link-local multicast address or not.
 * This implementation returns always {@code false}.
 * <p>
 * Valid IPv6 link-local multicast addresses are FFx2:/112 where x is a set
 * of flags, and the additional 112 bits make up the link-local multicast
 * address space.
 * <p>
 * Valid IPv4 link-local addresses are between: 224.0.0.0 to 224.0.0.255
 *
 * @return {@code true} if this instance represents a link-local multicast
 *         address, {@code false} otherwise.
 */
ECode InetAddress::IsMCLinkLocal(
    /* [out] */ Boolean* isMCLinkLocal)
{
    *isMCLinkLocal = FALSE;
    return NOERROR;
}

/**
 * Returns whether this address is a site-local multicast address or not.
 * This implementation returns always {@code false}.
 * <p>
 * Valid IPv6 site-local multicast addresses are FFx5:/112 where x is a set
 * of flags, and the additional 112 bits make up the site-local multicast
 * address space.
 * <p>
 * Valid IPv4 site-local addresses are between: 239.252.0.0 to
 * 239.255.255.255
 *
 * @return {@code true} if this instance represents a site-local multicast
 *         address, {@code false} otherwise.
 */
ECode InetAddress::IsMCSiteLocal(
    /* [out] */ Boolean* isMCSiteLocal)
{
    *isMCSiteLocal = FALSE;
    return NOERROR;
}

/**
 * Returns whether this address is a organization-local multicast address or
 * not. This implementation returns always {@code false}.
 * <p>
 * Valid IPv6 organization-local multicast addresses are FFx8:/112 where x
 * is a set of flags, and the additional 112 bits make up the
 * organization-local multicast address space.
 * <p>
 * Valid IPv4 organization-local addresses are between: 239.192.0.0 to
 * 239.251.255.255
 *
 * @return {@code true} if this instance represents a organization-local
 *         multicast address, {@code false} otherwise.
 */
ECode InetAddress::IsMCOrgLocal(
    /* [out] */ Boolean* isMCOrgLocal)
{
    *isMCOrgLocal = FALSE;
    return NOERROR;
}

/**
 * Returns whether this is a wildcard address or not. This implementation
 * returns always {@code false}.
 *
 * @return {@code true} if this instance represents a wildcard address,
 *         {@code false} otherwise.
 */
ECode InetAddress::IsAnyLocalAddress(
    /* [out] */ Boolean* isAnyLocalAddress)
{
    *isAnyLocalAddress = FALSE;
    return NOERROR;
}

/**
 * Tries to reach this {@code InetAddress}. This method first tries to use
 * ICMP <i>(ICMP ECHO REQUEST)</i>. When first step fails, a TCP connection
 * on port 7 (Echo) of the remote host is established.
 *
 * @param timeout
 *            timeout in milliseconds before the test fails if no connection
 *            could be established.
 * @return {@code true} if this address is reachable, {@code false}
 *         otherwise.
 * @throws IOException
 *             if an error occurs during an I/O operation.
 * @throws IllegalArgumentException
 *             if timeout is less than zero.
 */
ECode InetAddress::IsReachable(
    /* [in] */ Int32 timeout,
    /* [out] */ Boolean* isReachable)
{
    return IsReachableEx(NULL, 0, timeout, isReachable);
}

/**
 * Tries to reach this {@code InetAddress}. This method first tries to use
 * ICMP <i>(ICMP ECHO REQUEST)</i>. When first step fails, a TCP connection
 * on port 7 (Echo) of the remote host is established.
 *
 * @param networkInterface
 *            the network interface on which to connection should be
 *            established.
 * @param ttl
 *            the maximum count of hops (time-to-live).
 * @param timeout
 *            timeout in milliseconds before the test fails if no connection
 *            could be established.
 * @return {@code true} if this address is reachable, {@code false}
 *         otherwise.
 * @throws IOException
 *             if an error occurs during an I/O operation.
 * @throws IllegalArgumentException
 *             if ttl or timeout is less than zero.
 */
ECode InetAddress::IsReachableEx(
    /* [in] */ INetworkInterface* networkInterface,
    /* [in] */ Int32 ttl,
    /* [in] */ Int32 timeout,
    /* [out] */ Boolean* isReachable)
{
    if (ttl < 0 || timeout < 0) {
//        throw new IllegalArgumentException("ttl < 0 || timeout < 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (networkInterface == NULL) {
        return IsReachableByTCP((IInetAddress*)this->Probe(EIID_IInetAddress),
                NULL, timeout);
    }
    else {
        return IsReachableByMultiThread(networkInterface, ttl, timeout);
    }
}

/*
 * Uses multi-Thread to try if isReachable, returns true if any of threads
 * returns in time
 */
Boolean InetAddress::IsReachableByMultiThread(
    /* [in] */ INetworkInterface* netif,
    /* [in] */ Int32 ttl,
    /* [in] */ Int32 timeout)
{
//     List<InetAddress> addresses = Collections.list(netif.getInetAddresses());
//     if (addresses.isEmpty()) {
//         return false;
//     }
//     reached = false;
//     addrCount = addresses.size();
//     boolean needWait = false;
//     for (final InetAddress addr : addresses) {
//         // loopback interface can only reach to local addresses
//         if (addr.isLoopbackAddress()) {
//             Enumeration<NetworkInterface> NetworkInterfaces = NetworkInterface
//                     .getNetworkInterfaces();
//             while (NetworkInterfaces.hasMoreElements()) {
//                 NetworkInterface networkInterface = NetworkInterfaces
//                         .nextElement();
//                 Enumeration<InetAddress> localAddresses = networkInterface
//                         .getInetAddresses();
//                 while (localAddresses.hasMoreElements()) {
//                     if (InetAddress.this.equals(localAddresses
//                             .nextElement())) {
//                         return true;
//                     }
//                 }
//             }

//             synchronized (waitReachable) {
//                 addrCount--;

//                 if (addrCount == 0) {
//                     // if count equals zero, all thread
//                     // expired,notifies main thread
//                     waitReachable.notifyAll();
//                 }
//             }
//             continue;
//         }

//         needWait = true;
//         new Thread() {
//             @Override public void run() {
//                 /*
//                  * Spec violation! This implementation doesn't attempt an
//                  * ICMP; it skips right to TCP echo.
//                  */
//                 boolean threadReached = false;
//                 try {
//                     threadReached = isReachableByTCP(addr, InetAddress.this, timeout);
//                 } catch (IOException e) {
//                 }

//                 synchronized (waitReachable) {
//                     if (threadReached) {
//                         // if thread reached this address, sets reached to
//                         // true and notifies main thread
//                         reached = true;
//                         waitReachable.notifyAll();
//                     } else {
//                         addrCount--;
//                         if (0 == addrCount) {
//                             // if count equals zero, all thread
//                             // expired,notifies main thread
//                             waitReachable.notifyAll();
//                         }
//                     }
//                 }
//             }
//         }.start();
//     }

//     if (needWait) {
//         synchronized (waitReachable) {
//             try {
//                 while (!reached && (addrCount != 0)) {
//                     // wait for notification
//                     waitReachable.wait(1000);
//                 }
//             } catch (InterruptedException e) {
//                 // do nothing
//             }
//             return reached;
//         }
//     }

     return FALSE;
}

Boolean InetAddress::IsReachableByTCP(
    /* [in] */ IInetAddress* destination,
    /* [in] */ IInetAddress* source,
    /* [in] */ Int32 timeout)
{
    Boolean reached = FALSE;
    Int32 fd;
    NETIMPL->Socket(TRUE, &fd);
    InetAddress* srcObj = NULL;
    InetAddress* dstObj = NULL;
//    try {
    if (NULL != source) {
        srcObj = (InetAddress*)source->Probe(EIID_InetAddress);
        if (FAILED(NETIMPL->Bind(fd, *srcObj->mIpAddress, 0))) goto Exit;
    }
    dstObj = (InetAddress*)destination->Probe(EIID_InetAddress);
    if (FAILED(NETIMPL->Connect(fd, *dstObj->mIpAddress, 7, timeout))) goto Exit;
    reached = TRUE;
//    } catch (IOException e) {
//        if (ERRMSG_CONNECTION_REFUSED.Equals(e.getMessage())) {
//            // Connection refused means the IP is reachable
//            reached = TRUE;
//        }
//    }

Exit:
    NETIMPL->Close(fd);

    return reached;
}

/**
 * Returns the {@code InetAddress} corresponding to the array of bytes. In
 * the case of an IPv4 address there must be exactly 4 bytes and for IPv6
 * exactly 16 bytes. If not, an {@code UnknownHostException} is thrown.
 * <p>
 * The IP address is not validated by a name service.
 * <p>
 * The high order byte is {@code ipAddress[0]}.
 *
 * @param ipAddress
 *            is either a 4 (IPv4) or 16 (IPv6) byte long array.
 * @return an {@code InetAddress} instance representing the given IP address
 *         {@code ipAddress}.
 * @throws UnknownHostException
 *             if the given byte array has no valid length.
 */
ECode InetAddress::GetByAddress(
        /* [in] */ const ArrayOf<Byte>& ipAddress,
        /* [out] */ IInetAddress** address)
{
    // simply call the method by the same name specifying the default scope
    // id of 0
    return GetByAddressInternal(NULL, ipAddress, 0, address);
}

/**
 * Returns the {@code InetAddress} corresponding to the array of bytes. In
 * the case of an IPv4 address there must be exactly 4 bytes and for IPv6
 * exactly 16 bytes. If not, an {@code UnknownHostException} is thrown. The
 * IP address is not validated by a name service. The high order byte is
 * {@code ipAddress[0]}.
 *
 * @param ipAddress
 *            either a 4 (IPv4) or 16 (IPv6) byte array.
 * @param scope_id
 *            the scope id for an IPV6 scoped address. If not a scoped
 *            address just pass in 0.
 * @return the InetAddress
 * @throws UnknownHostException
 */
ECode InetAddress::GetByAddress(
    /* [in] */ const ArrayOf<Byte>& ipAddress,
    /* [in] */ Int32 scopeId,
    /* [out] */ IInetAddress** address)
{
    return GetByAddressInternal(NULL, ipAddress, scopeId, address);
}

Boolean InetAddress::IsIPv4MappedAddress(
    /* [in] */ const ArrayOf<Byte>& ipAddress)
{
    // Check if the address matches ::FFFF:d.d.d.d
    // The first 10 bytes are 0. The next to are -1 (FF).
    // The last 4 bytes are varied.
    if (ipAddress.GetLength() != 16) {
        return FALSE;
    }
    for (Int32 i = 0; i < 10; i++) {
        if (ipAddress[i] != 0) {
            return FALSE;
        }
    }
    if (ipAddress[10] != -1 || ipAddress[11] != -1) {
        return FALSE;
    }
    return TRUE;
}

void InetAddress::IPv4MappedToIPv4(
    /* [in] */ const ArrayOf<Byte>& mappedAddress,
    /* [out, callee] */ ArrayOf<Byte>** ipv4Address)
{
    *ipv4Address = ArrayOf<Byte>::Alloc(4);
    for(Int32 i = 0; i < 4; i++) {
        (**ipv4Address)[i] = mappedAddress[12 + i];
    }
}

/**
 * Returns the {@code InetAddress} corresponding to the array of bytes, and
 * the given hostname. In the case of an IPv4 address there must be exactly
 * 4 bytes and for IPv6 exactly 16 bytes. If not, an {@code
 * UnknownHostException} will be thrown.
 * <p>
 * The host name and IP address are not validated.
 * <p>
 * The hostname either be a machine alias or a valid IPv6 or IPv4 address
 * format.
 * <p>
 * The high order byte is {@code ipAddress[0]}.
 *
 * @param hostName
 *            the string representation of hostname or IP address.
 * @param ipAddress
 *            either a 4 (IPv4) or 16 (IPv6) byte long array.
 * @return an {@code InetAddress} instance representing the given IP address
 *         and hostname.
 * @throws UnknownHostException
 *             if the given byte array has no valid length.
 */
ECode InetAddress::GetByAddress(
    /* [in] */ const String& hostName,
    /* [in] */ const ArrayOf<Byte>& ipAddress,
    /* [out] */ IInetAddress** address)
{
    // just call the method by the same name passing in a default scope id
    // of 0
    return GetByAddressInternal(hostName, ipAddress, 0, address);
}

/**
 * Returns the {@code InetAddress} corresponding to the array of bytes, and
 * the given hostname. In the case of an IPv4 address there must be exactly
 * 4 bytes and for IPv6 exactly 16 bytes. If not, an {@code
 * UnknownHostException} is thrown. The host name and IP address are not
 * validated. The hostname either be a machine alias or a valid IPv6 or IPv4
 * address format. The high order byte is {@code ipAddress[0]}.
 *
 * @param hostName
 *            string representation of hostname or IP address.
 * @param ipAddress
 *            either a 4 (IPv4) or 16 (IPv6) byte array.
 * @param scope_id
 *            the scope id for a scoped address. If not a scoped address
 *            just pass in 0.
 * @return the InetAddress
 * @throws UnknownHostException
 */
ECode InetAddress::GetByAddressInternal(
    /* [in] */ CString hostName,
    /* [in] */ const ArrayOf<Byte>& ipAddress,
    /* [in] */ Int32 scopeId,
    /* [out] */ IInetAddress** address)
{
    ECode ec;
    ArrayOf<Byte>* ipAddr;
    switch (ipAddress.GetLength()) {
        case 4:
            ipAddr = ipAddress.Clone();
            CInet4Address* ipv4Address;
            ec = CInet4Address::NewByFriend(*ipAddr, &ipv4Address);
            *address = (IInetAddress*)ipv4Address->Probe(EIID_IInetAddress);
            return ec;
        case 16:
            // First check to see if the address is an IPv6-mapped
            // IPv4 address. If it is, then we can make it a IPv4
            // address, otherwise, we'll create an IPv6 address.
            if (IsIPv4MappedAddress(ipAddress)) {
                IPv4MappedToIPv4(ipAddress, &ipAddr);
                CInet4Address* ipv4Address;
                ec = CInet4Address::NewByFriend(*ipAddr, &ipv4Address);
                *address = (IInetAddress*)ipv4Address->Probe(EIID_IInetAddress);
                return ec;
            }
            else {
                ipAddr = ipAddress.Clone();
                CInet6Address* ipv6Address;
                ec = CInet6Address::NewByFriend(*ipAddr, scopeId, &ipv6Address);
                *address = (IInetAddress*)ipv6Address->Probe(EIID_IInetAddress);
                return ec;
            }
        default:
//            throw new UnknownHostException(
//                    "Invalid IP Address is neither 4 or 16 bytes: " + hostName);
            return E_UNKNOWN_HOST_EXCEPTION;
    }
}

/**
 * Takes the integer and chops it into 4 bytes, putting it into the byte
 * array starting with the high order byte at the index start. This method
 * makes no checks on the validity of the parameters.
 */
void InetAddress::Int32ToBytes(
    /* [in] */ Int32 value,
    /* [in] */ ArrayOf<Byte>& bytes,
    /* [in] */ Int32 start)
{
    // Shift the int so the current byte is right-most
    // Use a byte mask of 255 to single out the last byte.
    bytes[start] = (Byte)((value >> 24) & 255);
    bytes[start + 1] = (Byte)((value >> 16) & 255);
    bytes[start + 2] = (Byte)((value >> 8) & 255);
    bytes[start + 3] = (Byte)(value & 255);
}

/**
 * Takes the byte array and creates an integer out of four bytes starting at
 * start as the high-order byte. This method makes no checks on the validity
 * of the parameters.
 */
Int32 InetAddress::BytesToInt32(
    /* [in] */ const ArrayOf<Byte>& bytes,
    /* [in] */ Int32 start)
{
    // First mask the byte with 255, as when a negative
    // signed byte converts to an integer, it has bits
    // on in the first 3 bytes, we are only concerned
    // about the right-most 8 bits.
    // Then shift the rightmost byte to align with its
    // position in the integer.
    Int32 value = ((bytes[start + 3] & 255))
            | ((bytes[start + 2] & 255) << 8)
            | ((bytes[start + 1] & 255) << 16)
            | ((bytes[start] & 255) << 24);
    return value;
}

// private void writeObject(ObjectOutputStream stream) throws IOException {
//     ObjectOutputStream.PutField fields = stream.putFields();
//     if (ipaddress == null) {
//         fields.put("address", 0);
//     } else {
//         fields.put("address", bytesToInt(ipaddress, 0));
//     }
//     fields.put("family", family);
//     fields.put("hostName", hostName);

//     stream.writeFields();
// }

// private void readObject(ObjectInputStream stream) throws IOException,
//         ClassNotFoundException {
//     ObjectInputStream.GetField fields = stream.readFields();
//     int addr = fields.get("address", 0);
//     ipaddress = new byte[4];
//     intToBytes(addr, ipaddress, 0);
//     hostName = (String) fields.get("hostName", null);
//     family = fields.get("family", 2);
// }

// /*
//  * The spec requires that if we encounter a generic InetAddress in
//  * serialized form then we should interpret it as an Inet4 address.
//  */
// private Object readResolve() throws ObjectStreamException {
//     return new Inet4Address(ipaddress, hostName);
// }
