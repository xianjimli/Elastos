
#ifndef __CHTTPCOOKIE_H__
#define __CHTTPCOOKIE_H__

#include "cmdef.h"
#include "_CHttpCookie.h"
#include <elastos/AutoPtr.h>
#include <elastos/HashSet.h>
#include <elastos/List.h>
#include <StringBuffer.h>

CarClass(CHttpCookie)
{
public:
    class CookieParser
    {
    public:
        CookieParser(
            /* [in] */ const String& input);

        CARAPI Parse(
            /* [out] */ List<AutoPtr<IHttpCookie> >* httpCookies);

        CARAPI_(void) SetAttribute(
            /* [in] */ CHttpCookie* cookie,
            /* [in] */ const String& name,
            /* [in] */ const String& value);

        CARAPI_(AutoPtr<IDate>) ParseHttpDate(
            /* [in] */ const String& value);

        /**
         * Returns the next attribute name, or null if the input has been
         * exhausted. Returns wth the cursor on the delimiter that follows.
         */
        CARAPI_(String) ReadAttributeName(
            /* [in] */ Boolean returnLowerCase);

        /**
         * Returns true if an equals sign was read and consumed.
         */
        CARAPI_(Boolean) ReadEqualsSign();

        /**
         * Reads an attribute value, by parsing either a quoted string or until
         * the next character in {@code terminators}. The terminator character
         * is not consumed.
         */
        CARAPI ReadAttributeValue(
            /* [in] */ const String& terminators,
            /* [out] */ String* value);

        /**
         * Returns the index of the next character in {@code chars}, or the end
         * of the string.
         */
        CARAPI_(Int32) Find(
            /* [in] */ const String& chars);

        CARAPI_(void) SkipWhitespace();

    private:
        static const String ATTRIBUTE_NAME_TERMINATORS;
        static const String WHITESPACE;
        String mInput;
        String mInputLowerCase;
        Int32 mPos;

    public:
        /*
         * The cookie's version is set based on an overly complex heuristic:
         * If it has an expires attribute, the version is 0.
         * Otherwise, if it has a max-age attribute, the version is 1.
         * Otherwise, if the cookie started with "Set-Cookie2", the version is 1.
         * Otherwise, if it has any explicit version attributes, use the first one.
         * Otherwise, the version is 0.
         */
        Boolean mHasExpires;
        Boolean mHasMaxAge;
        Boolean mHasVersion;
    };

public:
    CHttpCookie();

    /**
     * Returns true if {@code host} matches the domain pattern {@code domain}.
     *
     * @param domainPattern a host name (like {@code android.com} or {@code
     *     localhost}), or a pattern to match subdomains of a domain name (like
     *     {@code .android.com}). A special case pattern is {@code .local},
     *     which matches all hosts without a TLD (like {@code localhost}).
     * @param host the host name or IP address from an HTTP request.
     */
    static CARAPI_(Boolean) DomainMatches(
        /* [in] */ const String& domainPattern,
        /* [in] */ const String& host);

    /**
     * Returns true if {@code cookie} should be sent to or accepted from {@code uri} with respect
     * to the cookie's path. Cookies match by directory prefix: URI "/foo" matches cookies "/foo",
     * "/foo/" and "/foo/bar", but not "/" or "/foobar".
     */
    static CARAPI_(Boolean) PathMatches(
        /* [in] */ IHttpCookie* cookie,
        /* [in] */ IURI* uri);

    /**
     * Returns true if {@code cookie} should be sent to {@code uri} with respect to the cookie's
     * secure attribute. Secure cookies should not be sent in insecure (ie. non-HTTPS) requests.
     */
    static CARAPI_(Boolean) SecureMatches(
        /* [in] */ IHttpCookie* cookie,
        /* [in] */ IURI* uri);

    /**
     * Returns true if {@code cookie} should be sent to {@code uri} with respect to the cookie's
     * port list.
     */
    static CARAPI_(Boolean) PortMatches(
        /* [in] */ IHttpCookie* cookie,
        /* [in] */ IURI* uri);

    /**
     * Constructs a cookie from a string. The string should comply with
     * set-cookie or set-cookie2 header format as specified in RFC 2965. Since
     * set-cookies2 syntax allows more than one cookie definitions in one
     * header, the returned object is a list.
     *
     * @param header
     *            a set-cookie or set-cookie2 header.
     * @return a list of constructed cookies
     * @throws IllegalArgumentException
     *             if the string does not comply with cookie specification, or
     *             the cookie name contains illegal characters, or reserved
     *             tokens of cookie specification appears
     * @throws NullPointerException
     *             if header is null
     */
    static CARAPI Parse(
        /* [in] */ const String& header,
        /* [out] */ List<AutoPtr<IHttpCookie> >* httpCookies);

    CARAPI GetComment(
        /* [out] */ String* comment);

    CARAPI GetCommentURL(
        /* [out] */ String* url);

    CARAPI GetDiscard(
        /* [out] */ Boolean* discard);

    CARAPI GetDomainAttr(
        /* [out] */ String* theDomain);

    CARAPI GetMaxAge(
        /* [out] */ Int64* maxAge);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetPath(
        /* [out] */ String* path);

    CARAPI GetPortList(
        /* [out] */ String* portList);

    CARAPI GetSecure(
        /* [out] */ Boolean* secure);

    CARAPI GetValue(
        /* [out] */ String* value);

    CARAPI GetVersion(
        /* [out] */ Int32* theVersion);

    CARAPI HasExpired(
        /* [out] */ Boolean* expired);

    CARAPI SetComment(
        /* [in] */ const String& comment);

    CARAPI SetCommentURL(
        /* [in] */ const String& commentURL);

    CARAPI SetDiscard(
        /* [in] */ Boolean discard);

    CARAPI SetDomain(
        /* [in] */ const String& pattern);

    CARAPI SetMaxAge(
        /* [in] */ Int64 deltaSeconds);

    CARAPI SetPath(
        /* [in] */ const String& path);

    CARAPI SetPortList(
        /* [in] */ const String& portList);

    CARAPI SetSecure(
        /* [in] */ Boolean secure);

    CARAPI SetValue(
        /* [in] */ const String& value);

    CARAPI SetVersion(
        /* [in] */ Int32 v);

    /**
     * Returns a string representing this cookie in the format used by the
     * {@code Cookie} header line in an HTTP request.
     */
    CARAPI_(String) ToString();

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ const String& value);

private:
    static CARAPI_(HashSet<String>&) InitReservedNames();

    /**
     * Returns a non-null path ending in "/".
     */
    static CARAPI_(String) MatchablePath(
        /* [in] */ const String& path);

    /**
     * Returns true if {@code s.substring(firstCharacter)} contains a dot
     * between its first and last characters, exclusive. This considers both
     * {@code android.com} and {@code co.uk} to be fully qualified domain names,
     * but not {@code android.com.}, {@code .com}. or {@code android}.
     *
     * <p>Although this implements the cookie spec's definition of FQDN, it is
     * not general purpose. For example, this returns true for IPv4 addresses.
     */
    static CARAPI_(Boolean) IsFullyQualifiedDomainName(
        /* [in] */ const String& s,
        /* [in] */ Int32 firstCharacter);

    CARAPI_(Boolean) IsValidName(
        /* [in] */ const String& n);

    CARAPI_(void) SetExpires(
        /* [in] */ IDate* expires);

    CARAPI_(void) AppendAttribute(
        /* [in] */ StringBuffer builder,
        /* [in] */ const String& name,
        /* [in] */ const String& value);

private:
    /**
     * If we fail to parse a date in a non-standard format, try each of these formats in sequence.
     */
    static String BROWSER_COMPATIBLE_DATE_FORMATS[];
    static const ArrayOf<String> ARRAYOF_BROWSER_COMPATIBLE_DATE_FORMATS;

    static HashSet<String> RESERVED_NAMES;

    String mComment;
    String mCommentURL;
    Boolean mDiscard;
    String mDomain;
    Int64 mMaxAge;
    String mName;
    String mPath;
    String mPortList;
    Boolean mSecure;
    String mValue;

    Int32 mVersion;
};

#endif //__CHTTPCOOKIE_H__
