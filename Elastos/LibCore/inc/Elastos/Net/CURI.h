
#ifndef __CURI_H__
#define __CURI_H__

#include "_CURI.h"
#include <Com.Kortide.Platform.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

/**
 * This class represents an instance of a URI as defined by RFC 2396.
 */
CarClass(CURI)
{
public:
    CURI();

    CARAPI constructor();

    /**
     * Creates a new URI instance according to the given string {@code uri}.
     *
     * @param uri
     *            the textual URI representation to be parsed into a URI object.
     * @throws URISyntaxException
     *             if the given string {@code uri} doesn't fit to the
     *             specification RFC2396 or could not be parsed correctly.
     */
    CARAPI constructor(
        /* [in] */ const String& uri);

    /**
     * Creates a new URI instance using the given arguments. This constructor
     * first creates a temporary URI string from the given components. This
     * string will be parsed later on to create the URI instance.
     * <p>
     * {@code [scheme:]scheme-specific-part[#fragment]}
     *
     * @param scheme
     *            the scheme part of the URI.
     * @param ssp
     *            the scheme-specific-part of the URI.
     * @param frag
     *            the fragment part of the URI.
     * @throws URISyntaxException
     *             if the temporary created string doesn't fit to the
     *             specification RFC2396 or could not be parsed correctly.
     */
    CARAPI constructor(
        /* [in] */ const String& scheme,
        /* [in] */ const String& ssp,
        /* [in] */ const String& frag);

    /**
     * Creates a new URI instance using the given arguments. This constructor
     * first creates a temporary URI string from the given components. This
     * string will be parsed later on to create the URI instance.
     * <p>
     * {@code [scheme:][user-info@]host[:port][path][?query][#fragment]}
     *
     * @param scheme
     *            the scheme part of the URI.
     * @param userInfo
     *            the user information of the URI for authentication and
     *            authorization.
     * @param host
     *            the host name of the URI.
     * @param port
     *            the port number of the URI.
     * @param path
     *            the path to the resource on the host.
     * @param query
     *            the query part of the URI to specify parameters for the
     *            resource.
     * @param fragment
     *            the fragment part of the URI.
     * @throws URISyntaxException
     *             if the temporary created string doesn't fit to the
     *             specification RFC2396 or could not be parsed correctly.
     */
    CARAPI constructor(
        /* [in] */ const String& scheme,
        /* [in] */ const String& userInfo,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& path,
        /* [in] */ const String& query,
        /* [in] */ const String& fragment);

    /**
     * Creates a new URI instance using the given arguments. This constructor
     * first creates a temporary URI string from the given components. This
     * string will be parsed later on to create the URI instance.
     * <p>
     * {@code [scheme:]host[path][#fragment]}
     *
     * @param scheme
     *            the scheme part of the URI.
     * @param host
     *            the host name of the URI.
     * @param path
     *            the path to the resource on the host.
     * @param fragment
     *            the fragment part of the URI.
     * @throws URISyntaxException
     *             if the temporary created string doesn't fit to the
     *             specification RFC2396 or could not be parsed correctly.
     */
    CARAPI constructor(
        /* [in] */ const String& scheme,
        /* [in] */ const String& host,
        /* [in] */ const String& path,
        /* [in] */ const String& fragment);

    /**
     * Creates a new URI instance using the given arguments. This constructor
     * first creates a temporary URI string from the given components. This
     * string will be parsed later on to create the URI instance.
     * <p>
     * {@code [scheme:][//authority][path][?query][#fragment]}
     *
     * @param scheme
     *            the scheme part of the URI.
     * @param authority
     *            the authority part of the URI.
     * @param path
     *            the path to the resource on the host.
     * @param query
     *            the query part of the URI to specify parameters for the
     *            resource.
     * @param fragment
     *            the fragment part of the URI.
     * @throws URISyntaxException
     *             if the temporary created string doesn't fit to the
     *             specification RFC2396 or could not be parsed correctly.
     */
    CARAPI constructor(
        /* [in] */ const String& scheme,
        /* [in] */ const String& authority,
        /* [in] */ const String& path,
        /* [in] */ const String& query,
        /* [in] */ const String& fragment);

    /**
     * Compares this URI with the given argument {@code uri}. This method will
     * return a negative value if this URI instance is less than the given
     * argument and a positive value if this URI instance is greater than the
     * given argument. The return value {@code 0} indicates that the two
     * instances represent the same URI. To define the order the single parts of
     * the URI are compared with each other. String components will be ordered
     * in the natural case-sensitive way. A hierarchical URI is less than an
     * opaque URI and if one part is {@code null} the URI with the undefined
     * part is less than the other one.
     *
     * @param uri
     *            the URI this instance has to compare with.
     * @return the value representing the order of the two instances.
     */
    CARAPI CompareTo(
        /* [in] */ IURI* uri,
        /* [out] */ Int32* result);

    /**
     * Returns the URI formed by parsing {@code uri}. This method behaves
     * identically to the string constructor but throws a different exception
     * on failure. The constructor fails with a checked {@link
     * URISyntaxException}; this method fails with an unchecked {@link
     * IllegalArgumentException}.
     */
    static CARAPI Create(
        /* [in] */ const String& uri,
        /* [out] */ IURI** obj);

    /**
     * Gets the decoded authority part of this URI.
     *
     * @return the decoded authority part or {@code null} if undefined.
     */
    CARAPI GetAuthority(
        /* [out] */ String* authority);

    /**
     * Gets the decoded fragment part of this URI.
     *
     * @return the decoded fragment part or {@code null} if undefined.
     */
    CARAPI GetFragment(
        /* [out] */ String* fragment);

    /**
     * Gets the host part of this URI.
     *
     * @return the host part or {@code null} if undefined.
     */
    CARAPI GetHost(
        /* [out] */ String* host);

    /**
     * Gets the decoded path part of this URI.
     *
     * @return the decoded path part or {@code null} if undefined.
     */
    CARAPI GetPath(
        /* [out] */ String* path);

    /**
     * Gets the port number of this URI.
     *
     * @return the port number or {@code -1} if undefined.
     */
    CARAPI GetPort(
        /* [out] */ Int32* port);

    /** @hide */
    CARAPI GetEffectivePort(
        /* [out] */ Int32* port);

    /**
     * Returns the port to use for {@code scheme} connections will use when
     * {@link #getPort} returns {@code specifiedPort}.
     *
     * @hide
     */
    static CARAPI_(Int32) GetEffectivePort(
        /* [in] */ const String& scheme,
        /* [in] */ Int32 specifiedPort);

    /**
     * Gets the decoded query part of this URI.
     *
     * @return the decoded query part or {@code null} if undefined.
     */
    CARAPI GetQuery(
        /* [out] */ String* query);

    /**
     * Gets the authority part of this URI in raw form.
     *
     * @return the encoded authority part or {@code null} if undefined.
     */
    CARAPI GetRawAuthority(
        /* [out] */ String* authority);

    /**
     * Gets the fragment part of this URI in raw form.
     *
     * @return the encoded fragment part or {@code null} if undefined.
     */
    CARAPI GetRawFragment(
        /* [out] */ String* fragment);

    /**
     * Gets the path part of this URI in raw form.
     *
     * @return the encoded path part or {@code null} if undefined.
     */
    CARAPI GetRawPath(
        /* [out] */ String* path);

    /**
     * Gets the query part of this URI in raw form.
     *
     * @return the encoded query part or {@code null} if undefined.
     */
    CARAPI GetRawQuery(
        /* [out] */ String* query);

    /**
     * Gets the scheme-specific part of this URI in raw form.
     *
     * @return the encoded scheme-specific part or {@code null} if undefined.
     */
    CARAPI GetRawSchemeSpecificPart(
        /* [out] */ String* schemeSpecific);

    /**
     * Gets the user-info part of this URI in raw form.
     *
     * @return the encoded user-info part or {@code null} if undefined.
     */
    CARAPI GetRawUserInfo(
        /* [out] */ String* userInfo);

    /**
     * Gets the scheme part of this URI.
     *
     * @return the scheme part or {@code null} if undefined.
     */
    CARAPI GetScheme(
        /* [out] */ String* scheme);

    /**
     * Gets the decoded scheme-specific part of this URI.
     *
     * @return the decoded scheme-specific part or {@code null} if undefined.
     */
    CARAPI GetSchemeSpecificPart(
        /* [out] */ String* schemeSpecific);

    /**
     * Gets the decoded user-info part of this URI.
     *
     * @return the decoded user-info part or {@code null} if undefined.
     */
    CARAPI GetUserInfo(
        /* [out] */ String* userInfo);

    /**
     * Gets the hashcode value of this URI instance.
     *
     * @return the appropriate hashcode value.
     */
    //Override, todo: inherit from object
    CARAPI HashCode(
        /* [out] */ Int32* value);

    /**
     * Indicates whether this URI is absolute, which means that a scheme part is
     * defined in this URI.
     *
     * @return {@code true} if this URI is absolute, {@code false} otherwise.
     */
    CARAPI IsAbsolute(
        /* [out] */ Boolean* isAbsolute);

    /**
     * Indicates whether this URI is opaque or not. An opaque URI is absolute
     * and has a scheme-specific part which does not start with a slash
     * character. All parts except scheme, scheme-specific and fragment are
     * undefined.
     *
     * @return {@code true} if the URI is opaque, {@code false} otherwise.
     */
    CARAPI IsOpaque(
        /* [out] */ Boolean* isOpaque);

    /**
     * Normalizes the path part of this URI.
     *
     * @return an URI object which represents this instance with a normalized
     *         path.
     */
    CARAPI Normalize(
        /* [out] */ IURI** uri);

    /**
     * Tries to parse the authority component of this URI to divide it into the
     * host, port, and user-info. If this URI is already determined as a
     * ServerAuthority this instance will be returned without changes.
     *
     * @return this instance with the components of the parsed server authority.
     * @throws URISyntaxException
     *             if the authority part could not be parsed as a server-based
     *             authority.
     */
    CARAPI ParseServerAuthority();

    /**
     * Makes the given URI {@code relative} to a relative URI against the URI
     * represented by this instance.
     *
     * @param relative
     *            the URI which has to be relativized against this URI.
     * @return the relative URI.
     */
    CARAPI Relativize(
        /* [in] */ IURI* relative,
        /* [out] */ IURI** uri);

    /**
     * Resolves the given URI {@code relative} against the URI represented by
     * this instance.
     *
     * @param relative
     *            the URI which has to be resolved against this URI.
     * @return the resolved URI.
     */
    CARAPI Resolve(
        /* [in] */ IURI* relative,
        /* [out] */ IURI** uri);

    /**
     * Creates a new URI instance by parsing the given string {@code relative}
     * and resolves the created URI against the URI represented by this
     * instance.
     *
     * @param relative
     *            the given string to create the new URI instance which has to
     *            be resolved later on.
     * @return the created and resolved URI.
     */
    CARAPI ResolveEx(
        /* [in] */ const String& relative,
        /* [out] */ IURI** uri);

    /**
     * Returns the textual string representation of this URI instance using the
     * US-ASCII encoding.
     *
     * @return the US-ASCII string representation of this URI.
     */
    CARAPI ToASCIIString(
        /* [out] */ String* str);

    /**
     * Compares this URI instance with the given argument {@code o} and
     * determines if both are equal. Two URI instances are equal if all single
     * parts are identical in their meaning.
     *
     * @param o
     *            the URI this instance has to be compared with.
     * @return {@code true} if both URI instances point to the same resource,
     *         {@code false} otherwise.
     */
     //todo:implements comparable
     CARAPI Equals(
        /* [in] */ IInterface* o,
        /* [out] */ Boolean* result);

    /**
     * Returns the textual string representation of this URI instance.
     *
     * @return the textual string representation of this URI.
     */
    //Override, todo: inherit from object
    CARAPI ToString(
        /* [out] */ String* s);

    /**
     * Converts this URI instance to a URL.
     *
     * @return the created URL representing the same resource as this URI.
     * @throws MalformedURLException
     *             if an error occurs while creating the URL or no protocol
     *             handler could be found.
     */
    CARAPI ToURL(
        /* [out] */ IURL** url);

private:
    CARAPI ParseURI(
        /* [in] */ const String& uri,
        /* [in] */ Boolean forceServer);

    CARAPI ValidateScheme(
        /* [in] */ const String& uri,
        /* [in] */ const String& scheme,
        /* [in] */ Int32 index);

    CARAPI ValidateSsp(
        /* [in] */ const String& uri,
        /* [in] */ const String& ssp,
        /* [in] */ Int32 index);

    CARAPI ValidateAuthority(
        /* [in] */ const String& uri,
        /* [in] */ const String& authority,
        /* [in] */ Int32 index);

    CARAPI ValidatePath(
        /* [in] */ const String& uri,
        /* [in] */ const String& path,
        /* [in] */ Int32 index);

    CARAPI ValidateQuery(
        /* [in] */ const String& uri,
        /* [in] */ const String& query,
        /* [in] */ Int32 index);

    CARAPI ValidateFragment(
        /* [in] */ const String& uri,
        /* [in] */ const String& fragment,
        /* [in] */ Int32 index);

    /**
     * Parse the authority string into its component parts: user info,
     * host, and port. This operation doesn't apply to registry URIs, and
     * calling it on such <i>may</i> result in a syntax exception.
     *
     * @param forceServer true to always throw if the authority cannot be
     *     parsed. If false, this method may still throw for some kinds of
     *     errors; this unpredictable behaviour is consistent with the RI.
     */
    CARAPI ParseAuthority(
        /* [in] */ Boolean forceServer);

    CARAPI ValidateUserInfo(
        /* [in] */ const String& uri,
        /* [in] */ const String& userInfo,
        /* [in] */ Int32 index);

    /**
     * Returns true if {@code host} is a well-formed host name or IP address.
     *
     * @param forceServer true to always throw if the host cannot be parsed. If
     *     false, this method may still throw for some kinds of errors; this
     *     unpredictable behaviour is consistent with the RI.
     */
    CARAPI IsValidHost(
        /* [in] */ Boolean forceServer,
        /* [in] */ const String& host,
        /* [out] */ Boolean* isValid);

    CARAPI_(Boolean) IsValidDomainName(
        /* [in] */ const String& host);

    /**
     * Quote illegal chars for each component, but not the others
     *
     * @param component java.lang.String the component to be converted
     * @param legalSet the legal character set allowed in the component
     * @return java.lang.String the converted string
     */
    CARAPI_(String) QuoteComponent(
        /* [in] */ const String& component,
        /* [in] */ const String& legalSet);

    CARAPI_(AutoPtr<CURI>) Duplicate();

    /*
     * Takes a string that may contain hex sequences like %F1 or %2b and
     * converts the hex values following the '%' to lowercase
     */
    CARAPI_(String) ConvertHexToLowerCase(
        /* [in] */ const String& s);

    /**
     * Returns true if {@code first} and {@code second} are equal after
     * unescaping hex sequences like %F1 and %2b.
     */
    CARAPI_(Boolean) EscapedEquals(
        /* [in] */ const String& first,
        /* [in] */ const String& second);

    /*
     * normalize path, and return the resulting string
     */
    CARAPI_(String) Normalize(
        /* [in] */ const String& path);

    /**
     * Helper method used to re-calculate the scheme specific part of the
     * resolved or normalized URIs
     */
    CARAPI_(void) SetSchemeSpecificPart();

    /**
     * Encode unicode chars that are not part of US-ASCII char set into the
     * escaped form
     *
     * i.e. The Euro currency symbol is encoded as "%E2%82%AC".
     */
    CARAPI EncodeNonAscii(
        /* [in] */ const String& s,
        /* [out] */ String* encodeS);

    CARAPI Decode(
        /* [in] */ const String& s,
        /* [out] */ String* decodeS);

    /*
     * Form a string from the components of this URI, similarly to the
     * toString() method. But this method converts scheme and host to lowercase,
     * and converts escaped octets to lowercase.
     */
    CARAPI_(String) GetHashString();

//    void readObject(ObjectInputStream in);
//    void writeObject(ObjectOutputStream out);

public:
    static const String UNRESERVED;
    static const String PUNCTUATION;
    static const String RESERVED;
    static const String SOME_LEGAL;
    static const String ALL_LEGAL;

private:
    static AutoPtr<INetworkSystem> NETWORK_SYSTEM;

    String mString;
    String mScheme;
    String mSchemeSpecificPart;
    String mAuthority;
    String mUserInfo;
    String mHost;
    Int32 mPort;
    String mPath;
    String mQuery;
    String mFragment;
    Boolean mOpaque;
    Boolean mAbsolute;
    Boolean mServerAuthority;

    Int32 mHash;

};

#endif //__CURI_H__
