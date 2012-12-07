
#ifndef __CURI_H__
#define __CURI_H__

#include "_CURI.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

CarClass(CURI)
{
public:
    CURI();

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

    CARAPI GetAuthority(
        /* [out] */ String* authority);

    CARAPI GetFragment(
        /* [out] */ String* fragment);

    CARAPI GetHost(
        /* [out] */ String* host);

    CARAPI GetPath(
        /* [out] */ String* path);

    CARAPI GetPort(
        /* [out] */ Int32* port);

    CARAPI GetEffectivePort(
        /* [out] */ Int32* port);

    /**
     * Returns the port to use for {@code scheme} connections will use when
     * {@link #getPort} returns {@code specifiedPort}.
     *
     * @hide
     */
    static CARAPI_(Int32) GetEffectivePortEx(
        /* [in] */ String scheme,
        /* [in] */ Int32 specifiedPort);

    CARAPI GetQuery(
        /* [out] */ String* query);

    CARAPI GetRawAuthority(
        /* [out] */ String* authority);

    CARAPI GetRawFragment(
        /* [out] */ String* fragment);

    CARAPI GetRawPath(
        /* [out] */ String* path);

    CARAPI GetRawQuery(
        /* [out] */ String* query);

    CARAPI GetRawSchemeSpecificPart(
        /* [out] */ String* schemeSpecific);

    CARAPI GetRawUserInfo(
        /* [out] */ String* userInfo);

    CARAPI GetScheme(
        /* [out] */ String* scheme);

    CARAPI GetSchemeSpecificPart(
        /* [out] */ String* schemeSpecific);

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

    CARAPI IsAbsolute(
        /* [out] */ Boolean* isAbsolute);

    CARAPI IsOpaque(
        /* [out] */ Boolean* isOpaque);

    CARAPI Normalize(
        /* [out] */ IURI** uri);

    CARAPI ParseServerAuthority();

    CARAPI Relativize(
        /* [in] */ IURI* relative,
        /* [out] */ IURI** uri);

    CARAPI Resolve(
        /* [in] */ IURI* relative,
        /* [out] */ IURI** uri);

    CARAPI ResolveEx(
        /* [in] */ const String& relative,
        /* [out] */ IURI** uri);

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

    CARAPI ToURL(
        /* [out] */ IURL** url);

    CARAPI constructor(
        /* [in] */ const String& uri);

    CARAPI constructor(
        /* [in] */ const String& scheme,
        /* [in] */ const String& ssp,
        /* [in] */ const String& frag);

    CARAPI constructor(
        /* [in] */ const String& scheme,
        /* [in] */ const String& userInfo,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& path,
        /* [in] */ const String& query,
        /* [in] */ const String& fragment);

    CARAPI constructor(
        /* [in] */ const String& scheme,
        /* [in] */ const String& host,
        /* [in] */ const String& path,
        /* [in] */ const String& fragment);

    CARAPI constructor(
        /* [in] */ const String& scheme,
        /* [in] */ const String& authority,
        /* [in] */ const String& path,
        /* [in] */ const String& query,
        /* [in] */ const String& fragment);

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
    CARAPI_(String) NormalizeInner(
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

private:
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
