
#ifndef __CSOCKETPERMISSION_H__
#define __CSOCKETPERMISSION_H__

#include "_CSocketPermission.h"
#include "Permission.h"
#include <elastos/AutoPtr.h>

/**
 * Regulates the access to network operations available through sockets through
 * permissions. A permission consists of a target (a host), and an associated
 * action list. The target should identify the host by either indicating the
 * (possibly wildcarded (eg. {@code .company.com})) DNS style name of the host
 * or its IP address in standard {@code nn.nn.nn.nn} ("dot") notation. The
 * action list can be made up of one or more of the following actions separated
 * by a comma:
 * <dl>
 * <dt>connect</dt>
 * <dd>requests permission to connect to the host</dd>
 * <dt>listen</dt>
 * <dd>requests permission to listen for connections from the host</dd>
 * <dt>accept</dt>
 * <dd>requests permission to accept connections from the host</dd>
 * <dt>resolve</dt>
 * <dd>requests permission to resolve the hostname</dd>
 * </dl>
 * Note that {@code resolve} is implied when any (or none) of the others are
 * present.
 * <p>
 * Access to a particular port can be requested by appending a colon and a
 * single digit to the name (eg. {@code .company.com:7000}). A range of port
 * numbers can also be specified, by appending a pattern of the form
 * <i>LOW-HIGH</i> where <i>LOW</i> and <i>HIGH</i> are valid port numbers. If
 * either <i>LOW</i> or <i>HIGH</i> is omitted it is equivalent to entering the
 * lowest or highest possible value respectively. For example:
 *
 * <pre>
 * {@code SocketPermission(&quot;www.company.com:7000-&quot;, &quot;connect,accept&quot;)}
 * </pre>
 *
 * represents the permission to connect to and accept connections from {@code
 * www.company.com} on ports in the range {@code 7000} to {@code 65535}.
 */
CarClass(CSocketPermission), public Permission
{
public:
    CSocketPermission();

    /**
     * Constructs a new {@code SocketPermission} instance. The hostname can be a
     * DNS name, an individual hostname, an IP address or the empty string which
     * implies {@code localhost}. The port or port range is optional.
     * <p>
     * The action list is a comma-separated list which can consists of the
     * possible operations {@code "connect"}, {@code "listen"}, {@code "accept"}
     * , and {@code "resolve"}. They are case-insensitive and can be put
     * together in any order. {@code "resolve"} is implied per default.
     *
     * @param host
     *            the hostname this permission is valid for.
     * @param action
     *            the action string of this permission.
     */
    CARAPI constructor(
        /* [in] */ const String& host,
        /* [in] */ const String& action);

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
    static const Int64 sSerialVersionUID;

    // list of actions permitted for socket permission in order, indexed by mask
    // value
    //@SuppressWarnings("nls")
    static const CString sActionNames[];

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
