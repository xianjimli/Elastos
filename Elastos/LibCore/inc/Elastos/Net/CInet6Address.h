
#ifndef __CINET6ADDRESS_H__
#define __CINET6ADDRESS_H__

#include "_CInet6Address.h"
#include "InetAddress.h"
#include <elastos/AutoPtr.h>

CarClass(CInet6Address), public InetAddress
{
public:
    CInet6Address();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

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
    static CARAPI GetByAddress(
        /* [in] */ const String& host,
        /* [in] */ const ArrayOf<Byte>& addr,
        /* [in] */ Int32 scopeId,
        /* [out] */ IInet6Address** address);

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
    static CARAPI GetByAddress(
        /* [in] */ const String& host,
        /* [in] */ const ArrayOf<Byte>& addr,
        /* [in] */ INetworkInterface* nif,
        /* [out] */ IInet6Address** address);

    CARAPI GetAddress(
        /* [out, callee] */ ArrayOf<Byte>** address);

    CARAPI GetHostAddress(
        /* [out] */ String* hostAddress);

    CARAPI GetHostName(
        /* [out] */ String* hostName);

    CARAPI GetCanonicalHostName(
        /* [out] */ String* hostName);

    /**
     * Returns whether this address is an IP multicast address or not. Valid
     * IPv6 multicast addresses are binary prefixed with 11111111 or FF (hex).
     *
     * @return {@code true} if this address is in the multicast group, {@code
     *         false} otherwise.
     */
    //@Override
    CARAPI IsMulticastAddress(
        /* [out] */ Boolean* isMulticastAddress);

    /**
     * Returns whether this address is the loopback address or not. The only
     * valid IPv6 loopback address is "::1".
     *
     * @return {@code true} if this instance represents the loopback address,
     *         {@code false} otherwise.
     */
    //@Override
    CARAPI IsLoopbackAddress(
        /* [out] */ Boolean* isLoopbackAddress);

    /**
     * Returns whether this address is a link-local address or not. A valid IPv6
     * link-local address is prefixed with 1111111010.
     *
     * @return {@code true} if this instance represents a link-local address,
     *         {@code false} otherwise.
     */
    //@Override
    CARAPI IsLinkLocalAddress(
        /* [out] */ Boolean* isLinkLocalAddress);

    /**
     * Returns whether this address is a site-local address or not. A valid IPv6
     * site-local address is prefixed with 1111111011.
     *
     * @return {@code true} if this instance represents a site-local address,
     *         {@code false} otherwise.
     */
    //@Override
    CARAPI IsSiteLocalAddress(
        /* [out] */ Boolean* isSiteLocalAddress);

    /**
     * Returns whether this address is a global multicast address or not. A
     * valid IPv6 global multicast address is 11111111xxxx1110 or FF0E hex.
     *
     * @return {@code true} if this instance represents a global multicast
     *         address, {@code false} otherwise.
     */
    //@Override
    CARAPI IsMCGlobal(
        /* [out] */ Boolean* isMCGlobal);

    /**
     * Returns whether this address is a node-local multicast address or not. A
     * valid IPv6 node-local multicast address is prefixed with
     * 11111111xxxx0001.
     *
     * @return {@code true} if this instance represents a node-local multicast
     *         address, {@code false} otherwise.
     */
    //@Override
    CARAPI IsMCNodeLocal(
        /* [out] */ Boolean* isMCNodeLocal);

    /**
     * Returns whether this address is a link-local multicast address or not. A
     * valid IPv6 link-local multicast address is prefixed with
     * 11111111xxxx0010.
     *
     * @return {@code true} if this instance represents a link-local multicast
     *         address, {@code false} otherwise.
     */
    //@Override
    CARAPI IsMCLinkLocal(
        /* [out] */ Boolean* isMCLinkLocal);

    /**
     * Returns whether this address is a site-local multicast address or not. A
     * valid IPv6 site-local multicast address is prefixed with
     * 11111111xxxx0101.
     *
     * @return {@code true} if this instance represents a site-local multicast
     *         address, {@code false} otherwise.
     */
    //@Override
    CARAPI IsMCSiteLocal(
        /* [out] */ Boolean* isMCSiteLocal);

    /**
     * Returns whether this address is a organization-local multicast address or
     * not. A valid IPv6 org-local multicast address is prefixed with
     * 11111111xxxx1000.
     *
     * @return {@code true} if this instance represents a org-local multicast
     *         address, {@code false} otherwise.
     */
    //@Override
    CARAPI IsMCOrgLocal(
        /* [out] */ Boolean* isMCOrgLocal);

    /**
     * Returns whether this address is a unspecified wildcard address "::" or
     * not.
     *
     * @return {@code true} if this instance represents a wildcard address,
     *         {@code false} otherwise.
     */
    //@Override
    CARAPI IsAnyLocalAddress(
        /* [out] */ Boolean* isAnyLocalAddress);

    CARAPI IsReachable(
        /* [in] */ Int32 timeout,
        /* [out] */ Boolean* isReachable);

    CARAPI IsReachableEx(
        /* [in] */ INetworkInterface* networkInterface,
        /* [in] */ Int32 ttl,
        /* [in] */ Int32 timeout,
        /* [out] */ Boolean* isReachable);

    /**
     * Gets the scope id as a number if this address is linked to an interface.
     * Otherwise returns {@code 0}.
     *
     * @return the scope_id of this address or 0 when not linked with an
     *         interface.
     */
    CARAPI GetScopeId(
        /* [out] */ Int32* scopeId);

    /**
     * Gets the network interface if this address is instanced with a scoped
     * network interface. Otherwise returns {@code null}.
     *
     * @return the scoped network interface of this address.
     */
    CARAPI GetScopedInterface(
        /* [out] */ INetworkInterface** scopedInterface);

    /**
     * Returns whether this address is IPv4 compatible or not. An IPv4
     * compatible address is prefixed with 96 bits of 0's. The last 32-bits are
     * varied corresponding with the 32-bit IPv4 address space.
     *
     * @return {@code true} if this instance represents an IPv4 compatible
     *         address, {@code false} otherwise.
     */
    CARAPI IsIPv4CompatibleAddress(
        /* [out] */ Boolean* isIPv4Compatible);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte>& address);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte>& address,
        /* [in] */ const String& name);

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
    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte>& address,
        /* [in] */ const String& name,
        /* [in] */ Int32 scopeId);

    /**
     * Constructs an {@code InetAddress} representing the {@code address} and
     * {@code scope_id}.
     *
     * @param address
     *            the network address.
     * @param scope_id
     *            the scope id for link- or site-local addresses.
     */
    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte>& address,
        /* [in] */ Int32 scopeId);

private:
    /**
     * Returns {@code true} if one of following cases applies:
     * <p>
     * <ol>
     *  <li>both addresses are site local</li>
     *  <li>both addresses are link local</li>
     *  <li>{@code ia} is neither site local nor link local</li>
     * </ol>
     */
    CARAPI_(Boolean) CompareLocalType(
        /* [in] */ IInet6Address* ia);

public:
    static AutoPtr<IInetAddress> ANY;
    static AutoPtr<IInetAddress> LOOPBACK;

    Int32 mScopeId;

    Boolean mScopeIdSet;

    Boolean mScopeIfnameSet;

    String mIfname;

    /*
     * scoped interface.
     */
    AutoPtr<INetworkInterface> mScopedIf;

private:
    static const Int32 AF_INET6 = 10;

//    private static final ObjectStreamField[] serialPersistentFields = {
//            new ObjectStreamField("ipaddress", new byte[0].getClass()),
//            new ObjectStreamField("scope_id", Integer.TYPE),
//            new ObjectStreamField("scope_id_set", Boolean.TYPE),
//            new ObjectStreamField("scope_ifname_set", Boolean.TYPE),
//            new ObjectStreamField("ifname", String.class), };
};

#endif //__CINET6ADDRESS_H__