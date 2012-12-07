
#ifndef __CINET4ADDRESS_H__
#define __CINET4ADDRESS_H__

#include "_CInet4Address.h"
#include "InetAddress.h"

CarClass(CInet4Address), public InetAddress
{
public:
    CARAPI_(PInterface) Probe(
                /* [in]  */ REIID riid);

    CARAPI GetAddress(
        /* [out, callee] */ ArrayOf<Byte>** address);

    CARAPI GetHostAddress(
        /* [out] */ String* hostAddress);

    CARAPI GetHostName(
        /* [out] */ String* hostName);

    CARAPI GetCanonicalHostName(
        /* [out] */ String* hostName);

     /**
     * Returns whether the represented address is a multicast address or not.
     * Valid IPv4 multicast addresses are prefixed with 1110 = 0xE.
     *
     * @return {@code true} if this instance represents a multicast address,
     *         {@code false} otherwise.
     */
    //@Override
    CARAPI IsMulticastAddress(
        /* [out] */ Boolean* isMulticastAddress);

    /**
     * Returns whether the represented address is a loopback address or not.
     * Loopback IPv4 addresses are prefixed with: 011111111 = 127.
     *
     * @return {@code true} if this instance represents a lookback address,
     *         {@code false} otherwise.
     */
    //@Override
    CARAPI IsLoopbackAddress(
        /* [out] */ Boolean* isLoopbackAddress);

    /**
     * Returns whether this address has a link-local scope or not.
     * <p>
     * RFC 3484 <br>
     * Default Address Selection for Internet Protocol Version 6 (IPv6) states
     * IPv4 auto-configuration addresses, prefix 169.254/16, IPv4 loopback
     * addresses, prefix 127/8, are assigned link-local scope.
     *
     * @return {@code true} if this instance represents a link-local address,
     *         {@code false} otherwise.
     */
    //@Override
    CARAPI IsLinkLocalAddress(
        /* [out] */ Boolean* isLinkLocalAddress);

    /**
     * Returns whether this address has a site-local scope or not.
     * <p>
     * RFC 3484 <br>
     * Default Address Selection for Internet Protocol Version 6 (IPv6) states
     * IPv4 private addresses, prefixes 10/8, 172.16/12, and 192.168/16, are
     * assigned site-local scope.
     *
     * @return {@code true} if this instance represents a site-local address,
     *         {@code false} otherwise.
     */
    //@Override
    CARAPI IsSiteLocalAddress(
        /* [out] */ Boolean* isSiteLocalAddress);

    /**
     * Returns whether the address is a global multicast address or not. Valid
     * MCGlobal IPv4 addresses are 224.0.1.0 - 238.255.255.255.
     *
     * @return {@code true} if the address is in the global multicast group,
     *         {@code false} otherwise.
     */
    //@Override
    CARAPI IsMCGlobal(
        /* [out] */ Boolean* isMCGlobal);

    /**
     * Returns whether the address has a node-local scope or not. This method
     * returns always {@code false} because there are no valid IPv4 node-local
     * addresses.
     *
     * @return {@code false} for all IPv4 addresses.
     */
    //@Override
    CARAPI IsMCNodeLocal(
        /* [out] */ Boolean* isMCNodeLocal);

    /**
     * Returns whether the address is a link-local multicast address or not. The
     * valid range for IPv4 link-local addresses is: 224.0.0.0 to 239.0.0.255
     * Hence a mask of 111000000000000000000000 = 0xE00000.
     *
     * @return {@code true} if this instance represents a link-local address,
     *         {@code false} otherwise.
     */
    //@Override
    CARAPI IsMCLinkLocal(
        /* [out] */ Boolean* isMCLinkLocal);

    /**
     * Returns whether the address is a site-local multicast address or not. The
     * valid range for IPv4 site-local addresses is: 239.255.0.0 to
     * 239.255.255.255 Hence a mask of 11101111 11111111 = 0xEFFF.
     *
     * @return {@code true} if this instance represents a site-local address,
     *         {@code false} otherwise.
     */
    //@Override
    CARAPI IsMCSiteLocal(
        /* [out] */ Boolean* isMCSiteLocal);

    /**
     * Returns whether the address is a organization-local multicast address or
     * not. The valid range for IPv4 organization-local addresses is:
     * 239.192.0.0 to 239.195.255.255 Hence masks of 11101111 11000000 to
     * 11101111 11000011 are valid. 0xEFC0 to 0xEFC3
     *
     * @return {@code true} if this instance represents a organization-local
     *         address, {@code false} otherwise.
     */
    //@Override
    CARAPI IsMCOrgLocal(
        /* [out] */ Boolean* isMCOrgLocal);

    /**
     * Returns whether the represented address is the local wildcard ANY address
     * or not.
     *
     * @return {@code true} if this instance represents the wildcard ANY
     *         address, {@code false} otherwise.
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

    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte>& address);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte>& address,
        /* [in] */ const String& name);

public:
    static AutoPtr<IInetAddress> ANY;
    static AutoPtr<IInetAddress> LOOPBACK;

private:
    static const Int32 AF_INET = 2;
};

#endif //__CINET4ADDRESS_H__
