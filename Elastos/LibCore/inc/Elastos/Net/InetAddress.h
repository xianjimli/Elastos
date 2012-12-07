
#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__

#include "Elastos.Net_server.h"
#include "AddressCache.h"
#include <Com.Kortide.Platform.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

extern "C" const InterfaceID EIID_InetAddress;

/**
 * An Internet Protocol (IP) address. This can be either an IPv4 address or an IPv6 address, and
 * in practice you'll have an instance of either {@code Inet4Address} or {@code Inet6Address} (this
 * class cannot be instantiated directly). Most code does not need to distinguish between the two
 * families, and should use {@code InetAddress}.
 *
 * <p>An {@code InetAddress} may have a hostname (accessible via {@code getHostName}), but may not,
 * depending on how the {@code InetAddress} was created.
 *
 * <h4>IPv4 numeric address formats</h4>
 * <p>The {@code getAllByName} method accepts IPv4 addresses in the following forms:
 * <ul>
 * <li>{@code "1.2.3.4"} - 1.2.3.4
 * <li>{@code "1.2.3"} - 1.2.0.3
 * <li>{@code "1.2"} - 1.0.0.2
 * <li>{@code "16909060"} - 1.2.3.4
 * </ul>
 * <p>In the first three cases, each number is treated as an 8-bit value between 0 and 255.
 * In the fourth case, the single number is treated as a 32-bit value representing the entire
 * address.
 * <p>Note that each numeric part can be expressed in decimal (as above) or hex. For example,
 * {@code "0x01020304"} is equivalent to 1.2.3.4 and {@code "0xa.0xb.0xc.0xd"} is equivalent
 * to 10.11.12.13.
 *
 * <p>Typically, only the four-dot decimal form ({@code "1.2.3.4"}) is ever used. Any method that
 * <i>returns</i> a textual numeric address will use four-dot decimal form.
 *
 * <h4>IPv6 numeric address formats</h4>
 * <p>The {@code getAllByName} method accepts IPv6 addresses in the following forms (this text
 * comes from <a href="http://www.ietf.org/rfc/rfc2373.txt">RFC 2373</a>, which you should consult
 * for full details of IPv6 addressing):
 * <ul>
 * <li><p>The preferred form is {@code x:x:x:x:x:x:x:x}, where the 'x's are the
 * hexadecimal values of the eight 16-bit pieces of the address.
 * Note that it is not necessary to write the leading zeros in an
 * individual field, but there must be at least one numeral in every
 * field (except for the case described in the next bullet).
 * Examples:
 * <pre>
 *     FEDC:BA98:7654:3210:FEDC:BA98:7654:3210
 *     1080:0:0:0:8:800:200C:417A</pre>
 * </li>
 * <li>Due to some methods of allocating certain styles of IPv6
 * addresses, it will be common for addresses to contain long strings
 * of zero bits.  In order to make writing addresses containing zero
 * bits easier a special syntax is available to compress the zeros.
 * The use of "::" indicates multiple groups of 16-bits of zeros.
 * The "::" can only appear once in an address.  The "::" can also be
 * used to compress the leading and/or trailing zeros in an address.
 *
 * For example the following addresses:
 * <pre>
 *     1080:0:0:0:8:800:200C:417A  a unicast address
 *     FF01:0:0:0:0:0:0:101        a multicast address
 *     0:0:0:0:0:0:0:1             the loopback address
 *     0:0:0:0:0:0:0:0             the unspecified addresses</pre>
 * may be represented as:
 * <pre>
 *     1080::8:800:200C:417A       a unicast address
 *     FF01::101                   a multicast address
 *     ::1                         the loopback address
 *     ::                          the unspecified addresses</pre>
 * </li>
 * <li><p>An alternative form that is sometimes more convenient when dealing
 * with a mixed environment of IPv4 and IPv6 nodes is
 * {@code x:x:x:x:x:x:d.d.d.d}, where the 'x's are the hexadecimal values of
 * the six high-order 16-bit pieces of the address, and the 'd's are
 * the decimal values of the four low-order 8-bit pieces of the
 * address (standard IPv4 representation).  Examples:
 * <pre>
 *     0:0:0:0:0:0:13.1.68.3
 *     0:0:0:0:0:FFFF:129.144.52.38</pre>
 * or in compressed form:
 * <pre>
 *     ::13.1.68.3
 *     ::FFFF:129.144.52.38</pre>
 * </li>
 * </ul>
 * <p>Scopes are given using a trailing {@code %} followed by the scope id, as in
 * {@code 1080::8:800:200C:417A%2} or {@code 1080::8:800:200C:417A%en0}.
 * See <a href="https://www.ietf.org/rfc/rfc4007.txt">RFC 4007</a> for more on IPv6's scoped
 * address architecture.
 *
 * <h4>DNS caching</h4>
 * <p>On Android, addresses are cached for 600 seconds (10 minutes) by default. Failed lookups are
 * cached for 10 seconds. The underlying C library or OS may cache for longer, but you can control
 * the Java-level caching with the usual {@code "networkaddress.cache.ttl"} and
 * {@code "networkaddress.cache.negative.ttl"} system properties. These are parsed as integer
 * numbers of seconds, where the special value 0 means "don't cache" and -1 means "cache forever".
 *
 * <p>Note also that on Android &ndash; unlike the RI &ndash; the cache is not unbounded. The
 * current implementation caches around 512 entries, removed on a least-recently-used basis.
 * (Obviously, you should not rely on these details.)
 *
 * @see Inet4Address
 * @see Inet6Address
 */
class CURI;

class InetAddress
{
    friend class CURI;
public:
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
    InetAddress();

    virtual ~InetAddress();

//    /**
//     * Compares this {@code InetAddress} instance against the specified address
//     * in {@code obj}. Two addresses are equal if their address byte arrays have
//     * the same length and if the bytes in the arrays are equal.
//     *
//     * @param obj
//     *            the object to be tested for equality.
//     * @return {@code true} if both objects are equal, {@code false} otherwise.
//     */
//    @Override
//    public boolean equals(Object obj)

    /**
     * Returns the IP address represented by this {@code InetAddress} instance
     * as a byte array. The elements are in network order (the highest order
     * address byte is in the zeroth element).
     *
     * @return the address in form of a byte array.
     */
    virtual CARAPI GetAddress(
        /* [out, callee] */ ArrayOf<Byte>** address);

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
    static CARAPI GetAllByName(
        /* [in] */ const String& host,
        /* [out, callee] */ ArrayOf<IInetAddress*>** addresses);

    static CARAPI_(Boolean) PreferIPv6Addresses();

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
    static CARAPI GetByName(
        /* [in] */ const String& host,
        /* [out] */ IInetAddress** address);

    /**
     * Gets the textual representation of this IP address.
     *
     * @return the textual representation of host's IP address.
     */
    virtual CARAPI GetHostAddress(
        /* [out] */ String* address);

    /**
     * Gets the host name of this IP address. If the IP address could not be
     * resolved, the textual representation in a dotted-quad-notation is
     * returned.
     *
     * @return the corresponding string name of this IP address.
     */
    virtual CARAPI GetHostName(
        /* [out] */ String* name);

    /**
     * Gets the fully qualified domain name for the host associated with this IP
     * address. If a security manager is set, it is checked if the method caller
     * is allowed to get the hostname. Otherwise, the textual representation in
     * a dotted-quad-notation is returned.
     *
     * @return the fully qualified domain name of this IP address.
     */
    virtual CARAPI GetCanonicalHostName(
        /* [out] */ String* canonicalName);

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
    static CARAPI GetLocalHost(
        /* [out] */ IInetAddress** address);

    /**
     * Gets the hashcode of the represented IP address.
     *
     * @return the appropriate hashcode value.
     */
//    @Override
//    public int hashCode();

    /*
     * Returns whether this address is an IP multicast address or not. This
     * implementation returns always {@code false}.
     *
     * @return {@code true} if this address is in the multicast group, {@code
     *         false} otherwise.
     */
    virtual CARAPI IsMulticastAddress(
        /* [out] */ Boolean* isMulticastAddress);

    static CARAPI GetHostNameInternal(
        /* [in] */ const String& host,
        /* [in] */ Boolean isCheck,
        /* [out] */ String* hostName);

    /**
     * Returns a string containing a concise, human-readable description of this
     * IP address.
     *
     * @return the description, as host/address.
     */
//    @Override
//    public String toString()

     /**
     * Returns true if the string is a host name, false if it is an IP Address.
     */
    static CARAPI_(Boolean) IsHostName(
        /* [in] */ const String& value);

    /**
     * Returns whether this address is a loopback address or not. This
     * implementation returns always {@code false}. Valid IPv4 loopback
     * addresses are 127.d.d.d The only valid IPv6 loopback address is ::1.
     *
     * @return {@code true} if this instance represents a loopback address,
     *         {@code false} otherwise.
     */
    virtual CARAPI IsLoopbackAddress(
        /* [out] */ Boolean* isLoopbackAddress);

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
    virtual CARAPI IsLinkLocalAddress(
        /* [out] */ Boolean* isLinkLocalAddress);

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
    virtual CARAPI IsSiteLocalAddress(
        /* [out] */ Boolean* isSiteLocalAddress);

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
    virtual CARAPI IsMCGlobal(
        /* [out] */ Boolean* isMCGlobal);

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
    virtual CARAPI IsMCNodeLocal(
        /* [out] */ Boolean* isMCNodeLocal);

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
    virtual CARAPI IsMCLinkLocal(
        /* [out] */ Boolean* isMCLinkLocal);

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
    virtual CARAPI IsMCSiteLocal(
        /* [out] */ Boolean* isMCSiteLocal);

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
    virtual CARAPI IsMCOrgLocal(
        /* [out] */ Boolean* isMCOrgLocal);

    /**
     * Returns whether this is a wildcard address or not. This implementation
     * returns always {@code false}.
     *
     * @return {@code true} if this instance represents a wildcard address,
     *         {@code false} otherwise.
     */
    virtual CARAPI IsAnyLocalAddress(
        /* [out] */ Boolean* isAnyLocalAddress);

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
    virtual CARAPI IsReachable(
        /* [in] */ Int32 timeout,
        /* [out] */ Boolean* isReachable);

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
    virtual CARAPI IsReachableEx(
        /* [in] */ INetworkInterface* networkInterface,
        /* [in] */ Int32 ttl,
        /* [in] */ Int32 timeout,
        /* [out] */ Boolean* isReachable);

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
    static CARAPI GetByAddress(
        /* [in] */ const ArrayOf<Byte>& ipAddress,
        /* [out] */ IInetAddress** address);

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
    static CARAPI GetByAddress(
        /* [in] */ const String& hostName,
        /* [in] */ const ArrayOf<Byte>& ipAddress,
        /* [out] */ IInetAddress** address);

protected:
    /**
     * Converts an array of byte arrays representing raw IP addresses of a host
     * to an array of InetAddress objects, sorting to respect the value of the
     * system property {@code "java.net.preferIPv6Addresses"}.
     *
     * @param rawAddresses the raw addresses to convert.
     * @param hostName the hostname corresponding to the IP address.
     * @return the corresponding InetAddresses, appropriately sorted.
     */
    static CARAPI BytesToInetAddresses(
        /* [in] */ ArrayOf<ArrayOf<Byte>*>* rawAddresses,
        /* [in] */ const String& hostName,
        /* [out, callee] */ ArrayOf<IInetAddress*>** addresses);

    /**
     * Returns the InetAddresses for {@code host}. The returned array is shared
     * and must be cloned before it is returned to application code.
     */
    static CARAPI GetAllByNameImpl(
        /* [in] */ const String& host,
        /* [out, callee] */ ArrayOf<IInetAddress*>** addresses);

    static CARAPI IpStringToByteArray(
        /* [in] */ const String& ipString,
        /* [out, callee] */ ArrayOf<Byte>** address);

    /**
     * Query the IP stack for the host address. The host is in address form.
     *
     * @param addr
     *            the host address to lookup.
     * @throws UnknownHostException
     *             if an error occurs during lookup.
     */
    static CARAPI GetHostByAddrImpl(
        /* [in] */ const ArrayOf<Byte>& addr,
        /* [out] */ IInetAddress** address);

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
    static CARAPI GetByAddress(
        /* [in] */ const ArrayOf<Byte>& ipAddress,
        /* [in] */ Int32 scopeId,
        /* [out] */ IInetAddress** address);

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
    static CARAPI GetByAddressInternal(
        /* [in] */ CString hostName,
        /* [in] */ const ArrayOf<Byte>& ipAddress,
        /* [in] */ Int32 scopeId,
        /* [out] */ IInetAddress** address);

    /**
     * Takes the integer and chops it into 4 bytes, putting it into the byte
     * array starting with the high order byte at the index start. This method
     * makes no checks on the validity of the parameters.
     */
    static CARAPI_(void) Int32ToBytes(
        /* [in] */ Int32 value,
        /* [in] */ ArrayOf<Byte>& bytes,
        /* [in] */ Int32 start);

    /**
     * Takes the byte array and creates an integer out of four bytes starting at
     * start as the high-order byte. This method makes no checks on the validity
     * of the parameters.
     */
    static CARAPI_(Int32) BytesToInt32(
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [in] */ Int32 start);

    virtual CARAPI_(PInterface) Probe(
                /* [in]  */ REIID riid) = 0;

private:
    static CARAPI ByteArrayToIpString(
        /* [in] */ const ArrayOf<Byte>& address,
        /* [out] */ String* ipString);

    static CARAPI_(CString) WrongAddressLength();

    static CARAPI_(String) Gethostname();

    /**
     * Resolves a hostname to its IP addresses using a cache.
     *
     * @param host the hostname to resolve.
     * @return the IP addresses of the host.
     */
    static CARAPI LookupHostByName(
        /* [in] */ const String& host,
        /* [out, callee] */ ArrayOf<IInetAddress*>** addresses);

    static CARAPI GetAddrInfo(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<ArrayOf<Byte>*>** addrInfos);

    /**
     * Resolves an IP address to a hostname. Thread safe.
     */
    static CARAPI GetNameInfo(
        /* [in] */ const ArrayOf<Byte>& addr,
        /* [out] */ String* hostname);

    /*
     * Uses multi-Thread to try if isReachable, returns true if any of threads
     * returns in time
     */
    CARAPI_(Boolean) IsReachableByMultiThread(
        /* [in] */ INetworkInterface* netif,
        /* [in] */ Int32 ttl,
        /* [in] */ Int32 timeout);

    CARAPI_(Boolean) IsReachableByTCP(
        /* [in] */ IInetAddress* destination,
        /* [in] */ IInetAddress* source,
        /* [in] */ Int32 timeout);

    static CARAPI_(Boolean) IsIPv4MappedAddress(
        /* [in] */ const ArrayOf<Byte>& ipAddress);

    static CARAPI_(void) IPv4MappedToIPv4(
        /* [in] */ const ArrayOf<Byte>& mappedAddress,
        /* [out, callee] */ ArrayOf<Byte>** ipv4Address);

//    private void writeObject(ObjectOutputStream stream)

//    private void readObject(ObjectInputStream stream)

public:
    String mHostName;
    Int32 mFamily;
    ArrayOf<Byte>* mIpAddress;

private:
    /** Our Java-side DNS cache. */
    static AutoPtr<AddressCache> ADDRESS_CACHE;

    static AutoPtr<INetworkSystem> NETIMPL;

    static const CString ERRMSG_CONNECTION_REFUSED;

//    private static final ObjectStreamField[] serialPersistentFields = {
//            new ObjectStreamField("address", Integer.TYPE),
//            new ObjectStreamField("family", Integer.TYPE),
//            new ObjectStreamField("hostName", String.class) };

//    private static class WaitReachable {
//    };
//
//    private transient Object waitReachable = new WaitReachable();

    Boolean mReached;

    Int32 mAddrCount;
};

#endif //__INETADDRESS_H__
