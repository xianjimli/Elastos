
#ifndef __AUTHENTICATOR_H__
#define __AUTHENTICATOR_H__

#include "Elastos.Net_server.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include <elastos/ElRefBase.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

extern "C" const InterfaceID EIID_Authenticator;

/**
 * An implementation of this class is able to obtain authentication information
 * for a connection in several ways. For this purpose it has to set the default
 * authenticator which extends {@code Authenticator} by {@code
 * setDefault(Authenticator a)}. Then it should override {@code
 * getPasswordAuthentication()} which dictates how the authentication info is
 * obtained. Usually, it prompts the user for the required input.
 *
 * @see #setDefault
 * @see #getPasswordAuthentication
 */
class Authenticator : public ElRefBase
{
public:
    Authenticator();

    /**
     * Returns the collected username and password for authorization. The
     * subclass has to override this method to return a value different to the
     * default which is {@code null}.
     * <p>
     * Returns {@code null} by default.
     *
     * @return collected password authentication data.
     */
    virtual CARAPI GetPasswordAuthentication(
        /* [out] */ IPasswordAuthentication** passwordAuthentication);

    /**
     * Returns the port of the connection that requests authorization.
     *
     * @return port of the connection.
     */
    virtual CARAPI GetRequestingPort(
        /* [out] */ Int32* port);

    /**
     * Returns the address of the connection that requests authorization or
     * {@code null} if unknown.
     *
     * @return address of the connection.
     */
    virtual CARAPI GetRequestingSite(
        /* [out] */ IInetAddress** address);

    /**
     * Returns the realm (prompt string) of the connection that requests
     * authorization.
     *
     * @return prompt string of the connection.
     */
    virtual CARAPI GetRequestingPrompt(
        /* [out] */ String* prompt);

    /**
     * Returns the protocol of the connection that requests authorization.
     *
     * @return protocol of the connection.
     */
    virtual CARAPI GetRequestingProtocol(
        /* [out] */ String* protocol);

    /**
     * Returns the scheme of the connection that requests authorization, for
     * example HTTP Basic Authentication.
     *
     * @return scheme of the connection.
     */
    virtual CARAPI GetRequestingScheme(
        /* [out] */ String* scheme);

    /**
     * If the permission check of the security manager does not result in a
     * security exception, this method invokes the methods of the registered
     * authenticator to get the authentication info.
     *
     * @return password authentication info or {@code null} if no authenticator
     *         exists.
     * @param rAddr
     *            address of the connection that requests authentication.
     * @param rPort
     *            port of the connection that requests authentication.
     * @param rProtocol
     *            protocol of the connection that requests authentication.
     * @param rPrompt
     *            realm of the connection that requests authentication.
     * @param rScheme
     *            scheme of the connection that requests authentication.
     * @throws SecurityException
     *             if a security manager denies the password authentication
     *             permission.
     */
    static CARAPI RequestPasswordAuthentication(
        /* [in] */ IInetAddress* rAddr,
        /* [in] */ Int32 rPort,
        /* [in] */ const String& rProtocol,
        /* [in] */ const String& rPrompt,
        /* [in] */ const String& rScheme,
        /* [out] */ IPasswordAuthentication** passwordAuthentication);

    /**
     * Sets {@code a} as the default authenticator. It will be called whenever
     * the realm that the URL is pointing to requires authorization. If there is
     * a security manager set then the caller must have the appropriate {@code
     * NetPermission}.
     *
     * @param a
     *            authenticator which has to be set as default.
     * @throws SecurityException
     *             if a security manager denies the password authentication
     *             permission.
     */
    static CARAPI_(void) SetDefault(
        /* [in] */ IAuthenticator* a);

    /**
     * If the permission check of the security manager does not result in a
     * security exception, this method invokes the methods of the registered
     * authenticator to get the authentication info.
     *
     * @return password authentication info or {@code null} if no authenticator
     *         exists.
     * @param rHost
     *            host name of the connection that requests authentication.
     * @param rAddr
     *            address of the connection that requests authentication.
     * @param rPort
     *            port of the connection that requests authentication.
     * @param rProtocol
     *            protocol of the connection that requests authentication.
     * @param rPrompt
     *            realm of the connection that requests authentication.
     * @param rScheme
     *            scheme of the connection that requests authentication.
     * @throws SecurityException
     *             if a security manager denies the password authentication
     *             permission.
     */
    static CARAPI RequestPasswordAuthenticationEx(
        /* [in] */ const String& rHost,
        /* [in] */ IInetAddress* rAddr,
        /* [in] */ Int32 rPort,
        /* [in] */ const String& rProtocol,
        /* [in] */ const String& rPrompt,
        /* [in] */ const String& rScheme,
        /* [out] */ IPasswordAuthentication** passwordAuthentication);

    /**
     * If the permission check of the security manager does not result in a
     * security exception, this method invokes the methods of the registered
     * authenticator to get the authentication info.
     *
     * @return password authentication info or {@code null} if no authenticator
     *         exists.
     * @param rHost
     *            host name of the connection that requests authentication.
     * @param rAddr
     *            address of the connection that requests authentication.
     * @param rPort
     *            port of the connection that requests authentication.
     * @param rProtocol
     *            protocol of the connection that requests authentication.
     * @param rPrompt
     *            realm of the connection that requests authentication.
     * @param rScheme
     *            scheme of the connection that requests authentication.
     * @param rURL
     *            url of the connection that requests authentication.
     * @param reqType
     *            requestor type of the connection that requests authentication.
     * @throws SecurityException
     *             if a security manager denies the password authentication
     *             permission.
     */
    static CARAPI RequestPasswordAuthenticationEx2(
        /* [in] */ const String& rHost,
        /* [in] */ IInetAddress* rAddr,
        /* [in] */ Int32 rPort,
        /* [in] */ const String& rProtocol,
        /* [in] */ const String& rPrompt,
        /* [in] */ const String& rScheme,
        /* [in] */ IURL* rURL,
        /* [in] */ AuthenticatorRequestorType reqType,
        /* [out] */ IPasswordAuthentication** passwordAuthentication);

    /**
     * Returns the host name of the connection that requests authentication or
     * {@code null} if unknown.
     *
     * @return name of the requesting host or {@code null}.
     */
    virtual CARAPI GetRequestingHost(
        /* [out] */ String* hostName);

    /**
     * Returns the URL of the authentication request.
     *
     * @return authentication request url.
     */
    virtual CARAPI GetRequestingURL(
        /* [out] */ IURL** url);

    /**
     * Returns the type of this request, it can be {@code PROXY} or {@code SERVER}.
     *
     * @return RequestorType of the authentication request.
     */
    virtual CARAPI GetRequestorType(
        /* [out] */ AuthenticatorRequestorType* requestorType);

private:
    // the default authenticator that needs to be set
    static AutoPtr<Authenticator> sThisAuthenticator;

    static AutoPtr<INetPermission> sRequestPasswordAuthenticationPermission;

    static AutoPtr<INetPermission> sSetDefaultAuthenticatorPermission;

    // the requester connection info
    String mHost;

    AutoPtr<IInetAddress> mAddr;

    Int32 mPort;

    String mProtocol;

    String mPrompt;

    String mScheme;

    AutoPtr<IURL> mUrl;

    AuthenticatorRequestorType mRt;

    static Mutex sLock;
};

#endif //__AUTHENTICATOR_H__
