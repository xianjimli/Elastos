
#include "CHttpCookie.h"
#include "InetAddress.h"
#include <elastos/Character.h>
#include <elastos/System.h>

using namespace Elastos;
using namespace Elastos::Core;

String CHttpCookie::BROWSER_COMPATIBLE_DATE_FORMATS[] =
        {String("EEEE, dd-MMM-yy HH:mm:ss zzz") // RFC 1036
        , String("EEE MMM d HH:mm:ss yyyy") // ANSI C asctime()
        , String("EEE, dd-MMM-yyyy HH:mm:ss z")
        , String("EEE, dd-MMM-yyyy HH-mm-ss z")
        , String("EEE, dd MMM yy HH:mm:ss z")
        , String("EEE dd-MMM-yyyy HH:mm:ss z")
        , String("EEE dd MMM yyyy HH:mm:ss z")
        , String("EEE dd-MMM-yyyy HH-mm-ss z")
        , String("EEE dd-MMM-yy HH:mm:ss z")
        , String("EEE dd MMM yy HH:mm:ss z")
        , String("EEE,dd-MMM-yy HH:mm:ss z")
        , String("EEE,dd-MMM-yyyy HH:mm:ss z")
        , String("EEE, dd-MM-yyyy HH:mm:ss z")};

const ArrayOf<String> CHttpCookie::ARRAYOF_BROWSER_COMPATIBLE_DATE_FORMATS(BROWSER_COMPATIBLE_DATE_FORMATS, 13);

HashSet<String>& CHttpCookie::InitReservedNames()
{
    RESERVED_NAMES.Insert(String("comment"));   //           RFC 2109  RFC 2965
    RESERVED_NAMES.Insert(String("commenturl"));//                     RFC 2965
    RESERVED_NAMES.Insert(String("discard"));   //                     RFC 2965
    RESERVED_NAMES.Insert(String("domain"));    // Netscape  RFC 2109  RFC 2965
    RESERVED_NAMES.Insert(String("expires"));   // Netscape
    RESERVED_NAMES.Insert(String("max-age"));   //           RFC 2109  RFC 2965
    RESERVED_NAMES.Insert(String("path"));      // Netscape  RFC 2109  RFC 2965
    RESERVED_NAMES.Insert(String("port"));      //                     RFC 2965
    RESERVED_NAMES.Insert(String("secure"));    // Netscape  RFC 2109  RFC 2965
    RESERVED_NAMES.Insert(String("version"));   //           RFC 2109  RFC 2965

    return RESERVED_NAMES;
}

HashSet<String> CHttpCookie::RESERVED_NAMES = CHttpCookie::InitReservedNames();

const String CHttpCookie::CookieParser::ATTRIBUTE_NAME_TERMINATORS(",;= \t");
const String CHttpCookie::CookieParser::WHITESPACE(" \t");

CHttpCookie::CookieParser::CookieParser(
    /* [in] */ const String& input)
    : mInput(input)
    , mPos(0)
    , mHasExpires(FALSE)
    , mHasMaxAge(FALSE)
    , mHasVersion(FALSE)
{
    mInputLowerCase = mInput;
    mInputLowerCase.ToLowerCase();
}

ECode CHttpCookie::CookieParser::Parse(
    /* [out] */ List<AutoPtr<IHttpCookie> >* httpCookies)
{
    List<AutoPtr<IHttpCookie> >* cookies = new List<AutoPtr<IHttpCookie> >(2);

    // The RI permits input without either the "Set-Cookie:" or "Set-Cookie2" headers.
    Boolean pre2965 = TRUE;
    if (mInputLowerCase.StartWith("set-cookie2:")) {
        mPos += String("set-cookie2:").GetLength();
        pre2965 = FALSE;
        mHasVersion = TRUE;
    }
    else if (mInputLowerCase.StartWith("set-cookie:")) {
        mPos += String("set-cookie:").GetLength();
    }

    /*
     * Read a comma-separated list of cookies. Note that the values may contain commas!
     *   <NAME> "=" <VALUE> ( ";" <ATTR NAME> ( "=" <ATTR VALUE> )? )*
     */
    while (TRUE) {
        String name = ReadAttributeName(FALSE);
        if (name.IsNull()) {
            if (cookies->IsEmpty()) {
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
//                throw new IllegalArgumentException("No cookies in " + input);
            }
            httpCookies = cookies;
            return NOERROR;
        }

        if (!ReadEqualsSign()) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
//            throw new IllegalArgumentException(
//                    "Expected '=' after " + name + " in " + input);
        }

        String value;
        ReadAttributeValue(pre2965 ? String(";") : String(",;"), &value);
        AutoPtr<CHttpCookie> cookie;
        CHttpCookie::NewByFriend(name, value, (CHttpCookie**)&cookie);
        cookie->mVersion = pre2965 ? 0 : 1;
        cookies->PushBack((IHttpCookie*)cookie);

        /*
         * Read the attributes of the current cookie. Each iteration of this loop should
         * enter with input either exhausted or prefixed with ';' or ',' as in ";path=/"
         * and ",COOKIE2=value2".
         */
        while (TRUE) {
            SkipWhitespace();
            if ((UInt32)mPos == mInput.GetLength()) {
                break;
            }

            if (mInput[mPos] == ',') {
                mPos++;
                break; // a true comma delimiter; the current cookie is complete.
            }
            else if (mInput[mPos] == ';') {
                mPos++;
            }

            String attributeName = ReadAttributeName(TRUE);
            if (attributeName.IsNull()) {
                continue; // for empty attribute as in "Set-Cookie: foo=Foo;;path=/"
            }

            /*
             * Since expires and port attributes commonly include comma delimiters, always
             * scan until a semicolon when parsing these attributes.
             */
            String terminators = pre2965
                    || String("expires").Equals(attributeName) || String("port").Equals(attributeName)
                    ? String(";")
                    : String(";,");
            String attributeValue = String(NULL);
            if (ReadEqualsSign()) {
                ReadAttributeValue(terminators, &attributeValue);
            }
            SetAttribute(cookie, attributeName, attributeValue);
        }

        if (mHasExpires) {
            cookie->mVersion = 0;
        }
        else if (mHasMaxAge) {
            cookie->mVersion = 1;
        }
    }

    httpCookies = cookies;
    return NOERROR;
}

void CHttpCookie::CookieParser::SetAttribute(
    /* [in] */ CHttpCookie* cookie,
    /* [in] */ const String& name,
    /* [in] */ const String& value)
{
    if (name.Equals(String("comment")) && cookie->mComment.IsNull()) {
        cookie->mComment = value;
    }
    else if (name.Equals(String("commenturl")) && cookie->mCommentURL.IsNull()) {
        cookie->mCommentURL = value;
    }
    else if (name.Equals(String("discard"))) {
        cookie->mDiscard = TRUE;
    }
    else if (name.Equals(String("domain")) && cookie->mDomain.IsNull()) {
        cookie->mDomain = value;
    }
    else if (name.Equals(String("expires"))) {
        mHasExpires = TRUE;
        if (cookie->mMaxAge == -1L) {
            AutoPtr<IDate> date = ParseHttpDate(value);
            if (date != NULL) {
                cookie->SetExpires(date);
            }
            else {
                cookie->mMaxAge = 0;
            }
        }
    }
    else if (name.Equals(String("max-age")) && cookie->mMaxAge == -1L) {
        mHasMaxAge = TRUE;
        cookie->mMaxAge = value.ToInt64();
    }
    else if (name.Equals(String("path")) && cookie->mPath == NULL) {
        cookie->mPath = value;
    }
    else if (name.Equals(String("port")) && cookie->mPortList == NULL) {
        cookie->mPortList = value != NULL ? value : String("");
    }
    else if (name.Equals(String("secure"))) {
        cookie->mSecure = TRUE;
    }
    else if (name.Equals(String("version")) && !mHasVersion) {
        cookie->mVersion = value.ToInt32();
    }
}

AutoPtr<IDate> CHttpCookie::CookieParser::ParseHttpDate(
    /* [in] */ const String& value)
{
//    try {
//        return STANDARD_DATE_FORMAT.get().parse(value);
//    } catch (ParseException ignore) {
//    }
//    String formatString;
//    for (Int32 i = 0; i < ARRAYOF_BROWSER_COMPATIBLE_DATE_FORMATS.GetLength(); i++) {
//        formatString = ARRAYOF_BROWSER_COMPATIBLE_DATE_FORMATSs[i];
//        try {
//            return new SimpleDateFormat(formatString, Locale.US).parse(value);
//        } catch (ParseException ignore) {
//        }
//    }
//    return null;

    return NULL;
}

String CHttpCookie::CookieParser::ReadAttributeName(
    /* [in] */ Boolean returnLowerCase)
{
    SkipWhitespace();
    Int32 c = Find(ATTRIBUTE_NAME_TERMINATORS);
    String forSubstring = returnLowerCase ? mInputLowerCase : mInput;
    String result = mPos < c ? forSubstring.Substring(mPos, c) : String(NULL);
    mPos = c;
    return result;
}

Boolean CHttpCookie::CookieParser::ReadEqualsSign()
{
    SkipWhitespace();
    if ((UInt32)mPos < mInput.GetLength() && mInput[mPos] == '=') {
        mPos++;
        return TRUE;
    }

    return FALSE;
}

ECode CHttpCookie::CookieParser::ReadAttributeValue(
    /* [in] */ const String& terminators,
    /* [out] */ String* value)
{
    SkipWhitespace();

    /*
     * Quoted string: read 'til the close quote. The spec mentions only "double quotes"
     * but RI bug 6901170 claims that 'single quotes' are also used.
     */
    if ((UInt32)mPos < mInput.GetLength() && (mInput[mPos] == '"' || mInput[mPos] == '\'')) {
        Char32 quoteCharacter = mInput[mPos++];
        Int32 closeQuote = mInput.IndexOf(quoteCharacter, mPos);
        if (closeQuote == -1) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
//            throw new IllegalArgumentException("Unterminated string literal in " + input);
        }
        *value = mInput.Substring(mPos, closeQuote);
        mPos = closeQuote + 1;
        return NOERROR;
    }

    Int32 c = Find(terminators);
    *value = mInput.Substring(mPos, c);
    mPos = c;
    return NOERROR;
}

Int32 CHttpCookie::CookieParser::Find(
    /* [in] */ const String& chars)
{
    for (Int32 c = mPos; (UInt32)c < mInput.GetLength(); c++) {
        if (chars.IndexOf(mInput[c]) != -1) {
            return c;
        }
    }
    return mInput.GetLength();
}

void CHttpCookie::CookieParser::SkipWhitespace()
{
    for (; (UInt32)mPos < mInput.GetLength(); mPos++) {
        if (WHITESPACE.IndexOf(mInput[mPos]) == -1) {
            break;
        }
    }
}

CHttpCookie::CHttpCookie()
    : mDiscard(FALSE)
    , mMaxAge(-1l)
    , mSecure(FALSE)
    , mVersion(1)
{
}

Boolean CHttpCookie::DomainMatches(
    /* [in] */ const String& domainPattern,
    /* [in] */ const String& host)
{
    if (domainPattern.IsNull() || host.IsNull()) {
        return FALSE;
    }

    String a = host;
    a.ToLowerCase();
    String b = domainPattern;
    b.ToLowerCase();

    /*
     * From the spec: "both host names are IP addresses and their host name strings match
     * exactly; or both host names are FQDN strings and their host name strings match exactly"
     */
    if (a.Equals(b) && (IsFullyQualifiedDomainName(a, 0) || !InetAddress::IsHostName(a))) {
        return TRUE;
    }
    if (!IsFullyQualifiedDomainName(a, 0)) {
        return b.Equals(".local");
    }

    /*
     * Not in the spec! If prefixing a hostname with "." causes it to equal the domain pattern,
     * then it should match. This is necessary so that the pattern ".google.com" will match the
     * host "google.com".
     */
    if (b.GetLength() == 1 + a.GetLength()
            && b.StartWith(".")
            && b.EndWith(a)
            && IsFullyQualifiedDomainName(b, 1)) {
        return TRUE;
    }

    /*
     * From the spec: "A is a HDN string and has the form NB, where N is a
     * non-empty name string, B has the form .B', and B' is a HDN string.
     * (So, x.y.com domain-matches .Y.com but not Y.com.)
     */
    return a.GetLength() > b.GetLength()
            && a.EndWith(b)
            && ((b.StartWith(".") && IsFullyQualifiedDomainName(b, 1)) || b.Equals(".local"));
}

Boolean CHttpCookie::PathMatches(
    /* [in] */ IHttpCookie* cookie,
    /* [in] */ IURI* uri)
{
    String path;
    uri->GetPath(&path);
    String uriPath = MatchablePath(path);
    cookie->GetPath(&path);
    String cookiePath = MatchablePath(path);
    return uriPath.StartWith(cookiePath);
}

Boolean CHttpCookie::SecureMatches(
    /* [in] */ IHttpCookie* cookie,
    /* [in] */ IURI* uri)
{
    Boolean secure;
    cookie->GetSecure(&secure);
    String scheme;
    uri->GetScheme(&scheme);
    return !secure || String("https").EqualsIgnoreCase(scheme);
}

Boolean CHttpCookie::PortMatches(
    /* [in] */ IHttpCookie* cookie,
    /* [in] */ IURI* uri)
{
    String portList;
    cookie->GetPortList(&portList);
    if (portList.IsNull()) {
        return TRUE;
    }
    Int32 port;
    uri->GetEffectivePort(&port);

    return FALSE;
//    return Arrays.asList(cookie.getPortlist().split(","))
//            .contains(Integer.toString());
}

String CHttpCookie::MatchablePath(
    /* [in] */ const String& path)
{
    if (path.IsNull()) {
        return String("/");
    }
    else if (path.EndWith("/")) {
        return path;
    }
    else {
        return path + String("/");
    }
}

Boolean CHttpCookie::IsFullyQualifiedDomainName(
    /* [in] */ const String& s,
    /* [in] */ Int32 firstCharacter)
{
    Int32 dotPosition = s.IndexOf('.', firstCharacter + 1);
    return dotPosition != -1 && (UInt32)dotPosition < s.GetLength() - 1;
}

ECode CHttpCookie::Parse(
    /* [in] */ const String& header,
    /* [out] */ List<AutoPtr<IHttpCookie> >* httpCookies)
{
    CookieParser* parser = new CookieParser(header);
    return parser->Parse(httpCookies);
}

Boolean CHttpCookie::IsValidName(
    /* [in] */ const String& n)
{
    // name cannot be empty or begin with '$' or equals the reserved
    // attributes (case-insensitive)
    String temp = n;
    temp.ToLowerCase();
    HashSet<String>::Iterator it;
    Boolean isContain = FALSE;
    for (it = RESERVED_NAMES.Begin(); it != RESERVED_NAMES.End(); ++it) {
        if (temp.Equals(*it)) {
            isContain = TRUE;
            break;
        }
    }
    Boolean isValid = !(n.GetLength() == 0 || n.StartWith("$") || isContain);
    if (isValid) {
        for (Int32 i = 0; (UInt32)i < n.GetLength(); i++) {
            Char32 nameChar = n[i];
            // name must be ASCII characters and cannot contain ';', ',' and
            // whitespace
            if (nameChar < 0
                    || nameChar >= 127
                    || nameChar == ';'
                    || nameChar == ','
                    || (Character::IsWhitespace(nameChar) && nameChar != ' ')) {
                isValid = FALSE;
                break;
            }
        }
    }
    return isValid;
}

ECode CHttpCookie::GetComment(
    /* [out] */ String* comment)
{
    VALIDATE_NOT_NULL(comment);
    *comment = mComment;

    return NOERROR;
}

ECode CHttpCookie::GetCommentURL(
    /* [out] */ String* url)
{
    VALIDATE_NOT_NULL(url);
    *url = mCommentURL;

    return NOERROR;
}

ECode CHttpCookie::GetDiscard(
    /* [out] */ Boolean* discard)
{
    VALIDATE_NOT_NULL(discard);
    *discard = mDiscard;

    return NOERROR;
}

ECode CHttpCookie::GetDomainAttr(
    /* [out] */ String* theDomain)
{
    VALIDATE_NOT_NULL(theDomain);
    *theDomain = mDomain;

    return NOERROR;
}

ECode CHttpCookie::GetMaxAge(
    /* [out] */ Int64* maxAge)
{
    VALIDATE_NOT_NULL(maxAge);
    *maxAge = mMaxAge;

    return NOERROR;
}

ECode CHttpCookie::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;

    return NOERROR;
}

ECode CHttpCookie::GetPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = mPath;

    return NOERROR;
}

ECode CHttpCookie::GetPortList(
    /* [out] */ String* portList)
{
    VALIDATE_NOT_NULL(portList);
    *portList = mPortList;

    return NOERROR;
}

ECode CHttpCookie::GetSecure(
    /* [out] */ Boolean* secure)
{
    VALIDATE_NOT_NULL(secure);
    *secure = mSecure;

    return NOERROR;
}

ECode CHttpCookie::GetValue(
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mValue;

    return NOERROR;
}

ECode CHttpCookie::GetVersion(
    /* [out] */ Int32* theVersion)
{
    VALIDATE_NOT_NULL(theVersion);
    *theVersion = mVersion;

    return NOERROR;
}

ECode CHttpCookie::HasExpired(
    /* [out] */ Boolean* expired)
{
    VALIDATE_NOT_NULL(expired);

    // -1 indicates the cookie will persist until browser shutdown
    // so the cookie is not expired.
    if (mMaxAge == -1l) {
        *expired = FALSE;
        return NOERROR;
    }

    if (mMaxAge <= 0l) {
        *expired = TRUE;
    }
    *expired = FALSE;
    return NOERROR;
}

ECode CHttpCookie::SetComment(
    /* [in] */ const String& comment)
{
    mComment = comment;
    return NOERROR;
}

ECode CHttpCookie::SetCommentURL(
    /* [in] */ const String& commentURL)
{
    mCommentURL = commentURL;
    return NOERROR;
}

ECode CHttpCookie::SetDiscard(
    /* [in] */ Boolean discard)
{
    mDiscard = discard;
    return NOERROR;
}

ECode CHttpCookie::SetDomain(
    /* [in] */ const String& pattern)
{
    if (pattern.IsNull()) {
        mDomain = String(NULL);
    }
    else {
        mDomain = pattern;
        mDomain.ToLowerCase();
    }
    return NOERROR;
}

ECode CHttpCookie::SetMaxAge(
    /* [in] */ Int64 deltaSeconds)
{
    mMaxAge = deltaSeconds;
    return NOERROR;
}

void CHttpCookie::SetExpires(
    /* [in] */ IDate* expires)
{
    Int64 time;
    expires->GetTime(&time);
    mMaxAge = (time - System::GetCurrentTimeMillis()) / 1000;
}

ECode CHttpCookie::SetPath(
    /* [in] */ const String& path)
{
    mPath = path;
    return NOERROR;
}

ECode CHttpCookie::SetPortList(
    /* [in] */ const String& portList)
{
    mPortList = portList;
    return NOERROR;
}

ECode CHttpCookie::SetSecure(
    /* [in] */ Boolean secure)
{
    mSecure = secure;
    return NOERROR;
}

ECode CHttpCookie::SetValue(
    /* [in] */ const String& value)
{
    // FIXME: According to spec, version 0 cookie value does not allow many
    // symbols. But RI does not implement it. Follow RI temporarily.
    mValue = value;
    return NOERROR;
}

ECode CHttpCookie::SetVersion(
    /* [in] */ Int32 v)
{
    if (v != 0 && v != 1) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }
    mVersion = v;
    return NOERROR;
}

/**
 * Returns a string representing this cookie in the format used by the
 * {@code Cookie} header line in an HTTP request.
 */
String CHttpCookie::ToString()
{
    if (mVersion == 0) {
        return mName + String("=") + mValue;
    }

    StringBuffer result;
    result += mName;
    result += "=";
    result += "\"";
    result += mValue;
    result += "\"";
    AppendAttribute(result, String("Path"), mPath);
    AppendAttribute(result, String("Domain"), mDomain);
    AppendAttribute(result, String("Port"), mPortList);
    return String(result);
}

void CHttpCookie::AppendAttribute(
    /* [in] */ StringBuffer builder,
    /* [in] */ const String& name,
    /* [in] */ const String& value)
{
    if (!value.IsNull() && !builder.IsNull()) {
        builder += ";$";
        builder += name;
        builder += "=\"";
        builder += value;
        builder += "\"";
    }
}

ECode CHttpCookie::constructor(
    /* [in] */ const String& name,
    /* [in] */ const String& value)
{
    String ntrim = name.Trim(); // erase leading and trailing whitespace
    if (!IsValidName(ntrim)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }

    mName = ntrim;
    mValue = value;

    return NOERROR;
}

