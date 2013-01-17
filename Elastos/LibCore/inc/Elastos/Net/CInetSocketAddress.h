
#ifndef __CINETSOCKETADDRESS_H__
#define __CINETSOCKETADDRESS_H__

#include "_CInetSocketAddress.h"
#include <elastos/AutoPtr.h>

CarClass(CInetSocketAddress)
{
public:
    /**
     * Creates a socket endpoint with the given port number {@code port} and
     * no specified address. The range for valid port numbers is between 0 and
     * 65535 inclusive.
     *
     * @param port
     *            the specified port number to which this socket is bound.
     */
    CARAPI constructor(
        /* [in] */ Int32 port);

    /**
     * Creates a socket endpoint with the given port number {@code port} and
     * {@code address}. The range for valid port numbers is between 0 and 65535
     * inclusive. If {@code address} is {@code null} this socket is bound to the
     * IPv4 wildcard address.
     *
     * @param port
     *            the specified port number to which this socket is bound.
     * @param address
     *            the specified address to which this socket is bound.
     */
    CARAPI constructor(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 port);

    /**
     * Creates a socket endpoint with the given port number {@code port} and the
     * hostname {@code host}. The hostname is tried to be resolved and cannot be
     * {@code null}. The range for valid port numbers is between 0 and 65535
     * inclusive.
     *
     * @param port
     *            the specified port number to which this socket is bound.
     * @param host
     *            the specified hostname to which this socket is bound.
     * @throws SecurityException
     *             if a {@link SecurityManager} is installed and its {@code
     *             checkConnect()} method does not allow the resolving of the
     *             host name.
     */
    CARAPI constructor(
        /* [in] */ const String& host,
        /* [in] */ Int32 port);

    /*
     * Internal constructor for InetSocketAddress(String, int) and
     * createUnresolved(String, int);
     */
    CARAPI constructor(
        /* [in] */ const String& hostname,
        /* [in] */ Int32 port,
        /* [in] */ Boolean needResolved);

    /**
     * Creates an {@code InetSocketAddress} without trying to resolve the
     * hostname into an {@code InetAddress}. The address field is marked as
     * unresolved.
     *
     * @param host
     *            the specified hostname to which this socket is bound.
     * @param port
     *            the specified port number to which this socket is bound.
     * @return the created InetSocketAddress instance.
     * @throws IllegalArgumentException
     *             if the hostname {@code host} is {@code null} or the port is
     *             not in the range between 0 and 65535.
     */
    static CARAPI CreateUnresolved(
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [out] */ IInetSocketAddress** addr);

    /**
     * Gets the port number of this socket.
     *
     * @return the socket endpoint port number.
     */
    CARAPI GetPort(
        /* [out] */ Int32* port);

    /**
     * Gets the address of this socket.
     *
     * @return the socket endpoint address.
     */
    CARAPI GetAddress(
        /* [out] */ IInetAddress** address);

    /**
     * Gets the hostname of this socket.
     *
     * @return the socket endpoint hostname.
     */
    CARAPI GetHostName(
        /* [out] */ String* hostname);

    /**
     * Returns whether this socket address is unresolved or not.
     *
     * @return {@code true} if this socket address is unresolved, {@code false}
     *         otherwise.
     */
    CARAPI IsUnresolved(
        /* [out] */ Boolean* isUnresolved);


private:
    static const Int64 sSerialVersionUID;

    String mHostname;

    AutoPtr<IInetAddress> mAddr;

    Int32 mPort;
};

#endif //__CINETSOCKETADDRESS_H__
