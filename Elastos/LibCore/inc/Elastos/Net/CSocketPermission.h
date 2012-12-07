
#ifndef __CSOCKETPERMISSION_H__
#define __CSOCKETPERMISSION_H__

#include "_CSocketPermission.h"
#include "Permission.h"
#include <elastos/AutoPtr.h>

CarClass(CSocketPermission), public Permission
{
public:
    CSocketPermission();

    CARAPI GetActions(
        /* [out] */ String* actions);

    CARAPI Implies(
        /* [in] */ IPermission* permission,
        /* [out] */ Boolean* isImplied);

    CARAPI NewPermissionCollection(
        /* [out] */ IPermissionCollection** permissionCollection);

    /**
     * Determines whether or not this permission could refer to the same host as
     * sp.
     */
    CARAPI_(Boolean) CheckHost(
        /* [in] */ ISocketPermission* sp);

    CARAPI constructor(
        /* [in] */ const String& host,
        /* [in] */ const String& action);

private:
    /**
     * Stores the actions for this permission as a bit field.
     *
     * @param actions
     *            java.lang.String the action list
     */
    CARAPI SetActions(
        /* [in] */ const String& actions);

    /**
     * Parse the port, including the minPort, maxPort
     * @param hostPort the host[:port] one
     * @param host the host name we just get
     * @throws IllegalArgumentException If the port is not a positive number or minPort
     *                                  is not less than or equal maxPort
     */
    CARAPI ParsePort(
        /* [in] */ const String& hostPort,
        /* [in] */ const String& host);

    /**
     * Creates a canonical action list.
     *
     * @param action
     *            java.lang.String
     *
     * @return java.lang.String
     */
    CARAPI_(String) ToCanonicalActionString(
        /* [in] */ const String& action);

    CARAPI_(String) GetIPString(
        /* [in] */ Boolean isCheck);

    /**
     * Get the host part from the host[:port] one. The host should be
     *
     * <pre>
     *      host = (hostname | IPv4address | IPv6reference | IPv6 in full uncompressed form)
     * </pre>
     *
     * The wildcard "*" may be included once in a DNS name host specification.
     * If it is included, it must be in the leftmost position
     *
     * @param host
     *            the {@code host[:port]} string.
     * @return the host name.
     * @throws IllegalArgumentException
     *             if the host is invalid.
     */
    CARAPI GetHostString(
        /* [in] */ const String& host,
        /* [out] */ String* hostName);

    static CARAPI_(Boolean) IsValidHexChar(
        /* [in] */ const Char32& c);

    static CARAPI_(Boolean) IsValidIP4Word(
        /* [in] */ const String& word);

    static CARAPI_(Boolean) IsIP6AddressInFullForm(
        /* [in] */ const String& ipAddress);

    static CARAPI_(Boolean) IsValidIP6Address(
        /* [in] */ const String& ipAddress);

public:
    // Bit masks for each of the possible actions
    static const Int32 SP_CONNECT;

    static const Int32 SP_LISTEN;

    static const Int32 SP_ACCEPT;

    static const Int32 SP_RESOLVE;

    String mHostName; // Host name as returned by InetAddress

    String mIpString; // IP address as returned by InetAddress

    Boolean mResolved; // IP address has been resolved

    // the port range;
    Int32 mPortMin;

    Int32 mPortMax;

    Int32 mActionsMask;

private:
    static const Int64 mSerialVersionUID;

    // list of actions permitted for socket permission in order, indexed by mask
    // value
    //@SuppressWarnings("nls")
    static const String mActionNames[];

    // If a wildcard is present store the information
    Boolean mIsPartialWild;

    Boolean mIsWild;

    // The highest port number
    static const Int32 HIGHEST_PORT;

    // The lowest port number
    static const Int32 LOWEST_PORT;

    String mActions; // List of all actions allowed by this permission
};

#endif //__CSOCKETPERMISSION_H__
