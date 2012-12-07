
#ifndef __CINETSOCKETADDRESS_H__
#define __CINETSOCKETADDRESS_H__

#include "_CInetSocketAddress.h"
#include <elastos/AutoPtr.h>

CarClass(CInetSocketAddress)
{
public:
    CInetSocketAddress();

    /*
     * Internal constructor for InetSocketAddress(String, int) and
     * createUnresolved(String, int);
     */
    CARAPI Init(
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

    CARAPI GetPort(
        /* [out] */ Int32* port);

    CARAPI GetAddress(
        /* [out] */ IInetAddress** address);

    CARAPI GetHostName(
        /* [out] */ String* hostname);

    CARAPI IsUnresolved(
        /* [out] */ Boolean* isUnresolved);

    CARAPI constructor(
        /* [in] */ Int32 port);

    CARAPI constructor(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 port);

    CARAPI constructor(
        /* [in] */ String host,
        /* [in] */ Int32 port);

private:
    static const Int64 sSerialVersionUID;

    String mHostname;

    AutoPtr<IInetAddress> mAddr;

    Int32 mPort;
};

#endif //__CINETSOCKETADDRESS_H__
