
#include "cmdef.h"
#include "CURI.h"
#include "URIEncoderDecoder.h"
#include "InetAddress.h"
#include "CURL.h"
#include <StringBuffer.h>
#include <elastos/Character.h>

CURI::CURI()
    : mPort(-1)
    , mOpaque(FALSE)
    , mAbsolute(FALSE)
    , mServerAuthority(FALSE)
    , mHash(-1)
{
}

ECode CURI::ParseURI(
    /* [in] */ const String& uri,
    /* [in] */ Boolean forceServer)
{
    String temp = uri;
    // assign uri string to the input value per spec
    mString = uri;
    Int32 index, index1, index2, index3;
    // parse into Fragment, Scheme, and SchemeSpecificPart
    // then parse SchemeSpecificPart if necessary

    // Fragment
    index = temp.IndexOf('#');
    if (index != -1) {
        // remove the fragment from the end
        mFragment = temp.Substring(index + 1);
        ValidateFragment(uri, mFragment, index + 1);
        temp = temp.Substring(0, index);
    }

    // Scheme and SchemeSpecificPart
    index = index1 = temp.IndexOf(':');
    index2 = temp.IndexOf('/');
    index3 = temp.IndexOf('?');

    // if a '/' or '?' occurs before the first ':' the uri has no
    // specified scheme, and is therefore not absolute
    if (index != -1 && (index2 >= index || index2 == -1)
            && (index3 >= index || index3 == -1)) {
        // the characters up to the first ':' comprise the scheme
        mAbsolute = TRUE;
        mScheme = temp.Substring(0, index);
        if (mScheme.GetLength() == 0) {
            return E_URI_SYNTAX_EXCEPTION;
//            throw new URISyntaxException(uri, "Scheme expected", index);
        }
        ValidateScheme(uri, mScheme, 0);
        mSchemeSpecificPart = temp.Substring(index + 1);
        if (mSchemeSpecificPart.GetLength() == 0) {
            return E_URI_SYNTAX_EXCEPTION;
//            throw new URISyntaxException(uri, "Scheme-specific part expected", index + 1);
        }
    }
    else {
        mAbsolute = FALSE;
        mSchemeSpecificPart = temp;
    }

    if (mScheme.IsNull() || mSchemeSpecificPart.GetLength() > 0
            && mSchemeSpecificPart[0] == '/') {
        mOpaque = FALSE;
        // the URI is hierarchical

        // Query
        temp = mSchemeSpecificPart;
        index = temp.IndexOf('?');
        if (index != -1) {
            mQuery = temp.Substring(index + 1);
            temp = temp.Substring(0, index);
            ValidateQuery(uri, mQuery, index2 + 1 + index);
        }

        // Authority and Path
        if (temp.StartWith("//")) {
            index = temp.IndexOf('/', 2);
            if (index != -1) {
                mAuthority = temp.Substring(2, index);
                mPath = temp.Substring(index);
            }
            else {
                mAuthority = temp.Substring(2);
                if (mAuthority.GetLength() == 0 && mQuery.IsNull()
                        && mFragment.IsNull()) {
                    return E_URI_SYNTAX_EXCEPTION;
//                    throw new URISyntaxException(uri, "Authority expected", uri.length());
                }

                mPath = "";
                // nothing left, so path is empty (not null, path should
                // never be null)
            }

            if (mAuthority.GetLength() == 0) {
                mAuthority = String(NULL);
            }
            else {
                ValidateAuthority(uri, mAuthority, index1 + 3);
            }
        }
        else { // no authority specified
            mPath = temp;
        }

        Int32 pathIndex = 0;
        if (index2 > -1) {
            pathIndex += index2;
        }
        if (index > -1) {
            pathIndex += index;
        }
        ValidatePath(uri, mPath, pathIndex);
    }
    else { // if not hierarchical, URI is opaque
        mOpaque = TRUE;
        ValidateSsp(uri, mSchemeSpecificPart, index2 + 2 + index);
    }

    return ParseAuthority(forceServer);
}

ECode CURI::ValidateScheme(
    /* [in] */ const String& uri,
    /* [in] */ const String& scheme,
    /* [in] */ Int32 index)
{
    // first char needs to be an alpha char
    Char32 ch = scheme[0];
    if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))) {
        return E_URI_SYNTAX_EXCEPTION;
//        throw new URISyntaxException(uri, "Illegal character in scheme", 0);
    }

//    try {
    return URIEncoderDecoder::ValidateSimple(scheme, String("+-."));
}

ECode CURI::ValidateSsp(
    /* [in] */ const String& uri,
    /* [in] */ const String& ssp,
    /* [in] */ Int32 index)
{
    return URIEncoderDecoder::Validate(ssp, String(URI_ALL_LEGAL));
}

ECode CURI::ValidateAuthority(
    /* [in] */ const String& uri,
    /* [in] */ const String& authority,
    /* [in] */ Int32 index)
{
    return URIEncoderDecoder::Validate(authority, String("@[]") + String(URI_SOME_LEGAL));
}

ECode CURI::ValidatePath(
    /* [in] */ const String& uri,
    /* [in] */ const String& path,
    /* [in] */ Int32 index)
{
    return URIEncoderDecoder::Validate(path, String("/@") + String(URI_SOME_LEGAL));
}

ECode CURI::ValidateQuery(
    /* [in] */ const String& uri,
    /* [in] */ const String& query,
    /* [in] */ Int32 index)
{
    return URIEncoderDecoder::Validate(query, String(URI_ALL_LEGAL));
}

ECode CURI::ValidateFragment(
    /* [in] */ const String& uri,
    /* [in] */ const String& fragment,
    /* [in] */ Int32 index)
{
    return URIEncoderDecoder::Validate(fragment, String(URI_ALL_LEGAL));
}

ECode CURI::ParseAuthority(
    /* [in] */ Boolean forceServer)
{
    if (mAuthority.IsNull()) {
        return NOERROR;
    }

    String tempUserInfo = String(NULL);
    String temp = mAuthority;
    Int32 index = temp.IndexOf('@');
    Int32 hostIndex = 0;
    if (index != -1) {
        // remove user info
        tempUserInfo = temp.Substring(0, index);
        ValidateUserInfo(mAuthority, tempUserInfo, 0);
        temp = temp.Substring(index + 1); // host[:port] is left
        hostIndex = index + 1;
    }

    index = temp.LastIndexOf(':');
    Int32 endIndex = temp.IndexOf(']');

    String tempHost;
    Int32 tempPort = -1;
    if (index != -1 && endIndex < index) {
        // determine port and host
        tempHost = temp.Substring(0, index);

        if ((UInt32)index < (temp.GetLength() - 1)) { // port part is not empty
//            try {
            tempPort = temp.Substring(index + 1).ToInt32();//Integer.parseInt(temp.Substring(index + 1));
            if (tempPort < 0) {
                if (forceServer) {
                    return E_URI_SYNTAX_EXCEPTION;
//                    throw new URISyntaxException(authority,
//                            "Invalid port number", hostIndex + index + 1);
                }
                return NOERROR;
            }
//            } catch (NumberFormatException e) {
//                if (forceServer) {
//                    throw new URISyntaxException(authority,
//                            "Invalid port number", hostIndex + index + 1);
//                }
//                return;
//            }
        }
    }
    else {
        tempHost = temp;
    }

    if (tempHost.IsEmpty()) {
        if (forceServer) {
            return E_URI_SYNTAX_EXCEPTION;
//            throw new URISyntaxException(authority, "Expected host", hostIndex);
        }
        return NOERROR;
    }

    Boolean isValid;
    IsValidHost(forceServer, tempHost, &isValid);
    if (!isValid) {
        return NOERROR;
    }

    // this is a server based uri,
    // fill in the userInfo, host and port fields
    mUserInfo = tempUserInfo;
    mHost = tempHost;
    mPort = tempPort;
    mServerAuthority = TRUE;

    return NOERROR;
}

ECode CURI::ValidateUserInfo(
    /* [in] */ const String& uri,
    /* [in] */ const String& userInfo,
    /* [in] */ Int32 index)
{
    for (Int32 i = 0; (UInt32)i < userInfo.GetLength(); i++) {
        Char32 ch = userInfo[i];
        if (ch == ']' || ch == '[') {
            return E_URI_SYNTAX_EXCEPTION;
//            throw new URISyntaxException(uri, "Illegal character in userInfo", index + i);
        }
    }

    return NOERROR;
}

ECode CURI::IsValidHost(
    /* [in] */ Boolean forceServer,
    /* [in] */ const String& host,
    /* [out] */ Boolean* isValid)
{
    VALIDATE_NOT_NULL(isValid);

    if (host.StartWith("[")) {
        // IPv6 address
        if (!host.EndWith("]")) {
            return E_URI_SYNTAX_EXCEPTION;
//            throw new URISyntaxException(host,
//                    "Expected a closing square bracket for IPv6 address", 0);
        }
//        try {
        ArrayOf<Byte>* bytes;
        InetAddress::IpStringToByteArray(host, &bytes);
        /*
         * The native IP parser may return 4 bytes for addresses like
         * "[::FFFF:127.0.0.1]". This is allowed, but we must not accept
         * IPv4-formatted addresses in square braces like "[127.0.0.1]".
         */
        if (bytes->GetLength() == 16 || bytes->GetLength() == 4 && host.Contains(":")) {
            *isValid = TRUE;
            return NOERROR;
        }
//        } catch (UnknownHostException e) {
//        }
        return E_URI_SYNTAX_EXCEPTION;
//        throw new URISyntaxException(host, "Malformed IPv6 address");
    }

    // '[' and ']' can only be the first char and last char
    // of the host name
    if (host.IndexOf('[') != -1 || host.IndexOf(']') != -1) {
        return E_URI_SYNTAX_EXCEPTION;
//        throw new URISyntaxException(host, "Illegal character in host name", 0);
    }

    Int32 index = host.LastIndexOf('.');
    if (index < 0 || (UInt32)index == host.GetLength() - 1
            || !Character::IsDigit(host[index + 1])) {
        // domain name
        if (IsValidDomainName(host)) {
            *isValid = TRUE;
            return NOERROR;
        }
        if (forceServer) {
            return E_URI_SYNTAX_EXCEPTION;
//            throw new URISyntaxException(host, "Illegal character in host name", 0);
        }
        *isValid = FALSE;
        return NOERROR;
    }

    // IPv4 address
//    try {
    ArrayOf<Byte>* bytes;
    InetAddress::IpStringToByteArray(host, &bytes);
    if (bytes->GetLength() == 4) {
        *isValid = TRUE;
        return NOERROR;
    }
//    } catch (UnknownHostException e) {
//    }

    if (forceServer) {
        return E_URI_SYNTAX_EXCEPTION;
//        throw new URISyntaxException(host, "Malformed IPv4 address", 0);
    }

    *isValid = FALSE;
    return NOERROR;
}

Boolean CURI::IsValidDomainName(
    /* [in] */ const String& host)
{
    ECode ec = URIEncoderDecoder::ValidateSimple(host, String("-."));
    if (ec == E_URI_SYNTAX_EXCEPTION) {
        return FALSE;
    }

    String lastLabel = String(NULL);
    StringTokenizer* st = new StringTokenizer(host, ".");
    while (st->HasMoreTokens()) {
        lastLabel = st->NextToken();
        if (lastLabel.StartWith("-") || lastLabel.EndWith("-")) {
            return FALSE;
        }
    }

    if (lastLabel.IsNull()) {
        return FALSE;
    }

    if (!lastLabel.Equals(host)) {
        Char32 ch = lastLabel[0];
        if (ch >= '0' && ch <= '9') {
            return FALSE;
        }
    }
    return TRUE;
}

String CURI::QuoteComponent(
    /* [in] */ const String& component,
    /* [in] */ const String& legalSet)
{
//    try {
    /*
     * Use a different encoder than URLEncoder since: 1. chars like "/",
     * "#", "@" etc needs to be preserved instead of being encoded, 2.
     * UTF-8 char set needs to be used for encoding instead of default
     * platform one
     */
    String s;
    ASSERT_SUCCEEDED(URIEncoderDecoder::QuoteIllegal(component, legalSet, &s));
    return s;
//    } catch (UnsupportedEncodingException e) {
//        throw new RuntimeException(e.toString());
//    }
}

ECode CURI::CompareTo(
    /* [in] */ IURI* uri,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 ret;

    // compare schemes
    AutoPtr<CURI> uriCls = (CURI*)uri;
    if (mScheme.IsNull() && !uriCls->mScheme.IsNull()) {
        *result = -1;
        return NOERROR;
    }
    else if (!mScheme.IsNull() && uriCls->mScheme.IsNull()) {
        *result = 1;
        return NOERROR;
    }
    else if (!mScheme.IsNull() && !uriCls->mScheme.IsNull()) {
        ret = mScheme.EqualsIgnoreCase(uriCls->mScheme);
        if (ret != 0) {
            *result = ret;
            return NOERROR;
        }
    }

    // compare opacities
    if (!mOpaque && uriCls->mOpaque) {
        *result = -1;
        return NOERROR;
    }
    else if (mOpaque && !uriCls->mOpaque) {
        *result = 1;
        return NOERROR;
    }
    else if (mOpaque && uriCls->mOpaque) {
        ret = mSchemeSpecificPart.Compare(uriCls->mSchemeSpecificPart);
        if (ret != 0) {
            *result = ret;
            return NOERROR;
        }
    }
    else {
        // otherwise both must be hierarchical

        // compare authorities
        if (!mAuthority.IsNull() && uriCls->mAuthority.IsNull()) {
            *result = 1;
            return NOERROR;
        }
        else if (mAuthority.IsNull() && !uriCls->mAuthority.IsNull()) {
            *result = -1;
            return NOERROR;
        }
        else if (!mAuthority.IsNull() && !uriCls->mAuthority.IsNull()) {
            if (!mHost.IsNull() && !uriCls->mHost.IsNull()) {
                // both are server based, so compare userInfo, host, port
                if (!mUserInfo.IsNull() && uriCls->mUserInfo.IsNull()) {
                    *result = 1;
                    return NOERROR;
                }
                else if (mUserInfo.IsNull() && !uriCls->mUserInfo.IsNull()) {
                    *result = -1;
                    return NOERROR;
                }
                else if (!mUserInfo.IsNull() && !uriCls->mUserInfo.IsNull()) {
                    ret = mUserInfo.Compare(uriCls->mUserInfo);
                    if (ret != 0) {
                        *result = ret;
                        return NOERROR;
                    }
                }

                // userInfo's are the same, compare hostname
                ret = mHost.EqualsIgnoreCase(uriCls->mHost);
                if (ret != 0) {
                    *result = ret;
                    return NOERROR;
                }

                // compare port
                if (mPort != uriCls->mPort) {
                    return mPort - uriCls->mPort;
                }
            }
            else { // one or both are registry based, compare the whole
                // authority
                ret = mAuthority.Compare(uriCls->mAuthority);
                if (ret != 0) {
                    *result = ret;
                    return NOERROR;
                }
            }
        }

        // authorities are the same
        // compare paths
        ret = mPath.Compare(uriCls->mPath);
        if (ret != 0) {
            *result = ret;
            return NOERROR;
        }

        // compare queries

        if (!mQuery.IsNull() && uriCls->mQuery.IsNull()) {
            *result = 1;
            return NOERROR;
        }
        else if (mQuery.IsNull() && !uriCls->mQuery.IsNull()) {
            *result = -1;
            return NOERROR;
        }
        else if (!mQuery.IsNull() && !uriCls->mQuery.IsNull()) {
            ret = mQuery.Compare(uriCls->mQuery);
            if (ret != 0) {
                *result = ret;
                return NOERROR;
            }
        }
    }

    // everything else is identical, so compare fragments
    if (!mFragment.IsNull() && uriCls->mFragment.IsNull()) {
        *result = 1;
        return NOERROR;
    }
    else if (mFragment.IsNull() && !uriCls->mFragment.IsNull()) {
        *result = -1;
        return NOERROR;
    }
    else if (!mFragment.IsNull() && !uriCls->mFragment.IsNull()) {
        ret = mFragment.Compare(uriCls->mFragment);
        if (ret != 0) {
            *result = ret;
            return NOERROR;
        }
    }

    // identical
    *result = 0;
    return NOERROR;
}

ECode CURI::Create(
    /* [in] */ const String& uri,
    /* [out] */ IURI** obj)
{
//    try {
    VALIDATE_NOT_NULL(obj);
    ECode ec = CURI::New(uri, obj);
    if (FAILED(ec)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
//    } catch (URISyntaxException e) {
//        throw new IllegalArgumentException(e.getMessage());
//    }
}

AutoPtr<CURI> CURI::Duplicate()
{
    AutoPtr<CURI> clone = new CURI();
    clone->mAbsolute = mAbsolute;
    clone->mAuthority = mAuthority;
    clone->mFragment = mFragment;
    clone->mHost = mHost;
    clone->mOpaque = mOpaque;
    clone->mPath = mPath;
    clone->mPort = mPort;
    clone->mQuery = mQuery;
    clone->mScheme = mScheme;
    clone->mSchemeSpecificPart = mSchemeSpecificPart;
    clone->mUserInfo = mUserInfo;
    clone->mServerAuthority = mServerAuthority;
    return clone;
}

String CURI::ConvertHexToLowerCase(
    /* [in] */ const String& s)
{
    StringBuffer result;
    if (s.IndexOf('%') == -1) {
        return s;
    }

    Int32 index, prevIndex = 0;
    while ((index = s.IndexOf('%', prevIndex)) != -1) {
        result += s.Substring(prevIndex, index + 1);
        String temp;
        temp = s.Substring(index + 1, index + 3);
        temp.ToLowerCase();
        result += temp;
        index += 3;
        prevIndex = index;
    }
    return (String)result;
}

Boolean CURI::EscapedEquals(
    /* [in] */ const String& first,
    /* [in] */ const String& second)
{
    if (first.IndexOf('%') != second.IndexOf('%')) {
        return first.Equals(second);
    }

    Int32 index, prevIndex = 0;
    while ((index = first.IndexOf('%', prevIndex)) != -1
            && second.IndexOf('%', prevIndex) == index) {
        Boolean match = first.Substring(prevIndex, index).Equals(
                second.Substring(prevIndex, index));
        if (!match) {
            return FALSE;
        }

        match = first.Substring(index + 1, index + 3).EqualsIgnoreCase(
                second.Substring(index + 1, index + 3));
        if (!match) {
            return FALSE;
        }

        index += 3;
        prevIndex = index;
    }
    return first.Substring(prevIndex).Equals(second.Substring(prevIndex));
}

//todo:implement the function of comparable
ECode CURI::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (((IURI*)o->Probe(EIID_IURI)) == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<CURI> uri = (CURI*)(IURI*)o->Probe(EIID_IURI);

    if (uri->mFragment.IsNull() && !mFragment.IsNull() || !uri->mFragment.IsNull()
            && mFragment.IsNull()) {
        *result = FALSE;
        return NOERROR;
    }
    else if (!uri->mFragment.IsNull() && !mFragment.IsNull()) {
        if (!EscapedEquals(uri->mFragment, mFragment)) {
            *result = FALSE;
            return NOERROR;
        }
    }

    if (uri->mScheme.IsNull() && !mScheme.IsNull() || !uri->mScheme.IsNull()
            && mScheme.IsNull()) {
        *result = FALSE;
        return NOERROR;
    }
    else if (!uri->mScheme.IsNull() && !mScheme.IsNull()) {
        if (!uri->mScheme.EqualsIgnoreCase(mScheme)) {
            *result = FALSE;
            return NOERROR;
        }
    }

    if (uri->mOpaque && mOpaque) {
        *result = EscapedEquals(uri->mSchemeSpecificPart,
                mSchemeSpecificPart);
        return NOERROR;
    }
    else if (!uri->mOpaque && !mOpaque) {
        if (!EscapedEquals(mPath, uri->mPath)) {
            *result = FALSE;
            return NOERROR;
        }

        if (!uri->mQuery.IsNull() && mQuery.IsNull() || uri->mQuery.IsNull()
                && !mQuery.IsNull()) {
            *result = FALSE;
            return NOERROR;
        }
        else if (!uri->mQuery.IsNull() && !mQuery.IsNull()) {
            if (!EscapedEquals(uri->mQuery, mQuery)) {
                *result = FALSE;
                return NOERROR;
            }
        }

        if (!uri->mAuthority.IsNull() && mAuthority.IsNull()
                || uri->mAuthority.IsNull() && !mAuthority.IsNull()) {
            *result = FALSE;
            return NOERROR;
        }
        else if (!uri->mAuthority.IsNull() && !mAuthority.IsNull()) {
            if (!uri->mHost.IsNull() && mHost.IsNull() || uri->mHost.IsNull()
                    && !mHost.IsNull()) {
                *result = FALSE;
                return NOERROR;
            }
            else if (uri->mHost.IsNull() && mHost.IsNull()) {
                // both are registry based, so compare the whole authority
                *result = EscapedEquals(uri->mAuthority, mAuthority);
            }
            else { // uri.host != null && host != null, so server-based
                if (!mHost.EqualsIgnoreCase(uri->mHost)) {
                    *result = FALSE;
                    return NOERROR;
                }

                if (mPort != uri->mPort) {
                    *result = FALSE;
                    return NOERROR;
                }

                if (!uri->mUserInfo.IsNull() && mUserInfo.IsNull()
                        || uri->mUserInfo.IsNull() && !mUserInfo.IsNull()) {
                    *result = FALSE;
                    return NOERROR;
                }
                else if (!uri->mUserInfo.IsNull() && !mUserInfo.IsNull()) {
                    *result = EscapedEquals(mUserInfo, uri->mUserInfo);
                }
                else {
                    *result = TRUE;
                    return NOERROR;
                }
            }
        }
        else {
            // no authority
            *result = TRUE;
            return NOERROR;
        }

    }
    else {
        // one is opaque, the other hierarchical
        *result = FALSE;
        return NOERROR;
    }

    return NOERROR;
}

ECode CURI::GetAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority);
    return Decode(mAuthority, authority);
}

ECode CURI::GetFragment(
    /* [out] */ String* fragment)
{
    VALIDATE_NOT_NULL(fragment);
    return Decode(mFragment, fragment);
}

ECode CURI::GetHost(
    /* [out] */ String* host)
{
    VALIDATE_NOT_NULL(host);
    *host = mHost;

    return NOERROR;
}

ECode CURI::GetPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    return Decode(mPath, path);
}

ECode CURI::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    *port = mPort;

    return NOERROR;
}

ECode CURI::GetEffectivePort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    *port = GetEffectivePortEx(mScheme, mPort);

    return NOERROR;
}

Int32 CURI::GetEffectivePortEx(
    /* [in] */ String scheme,
    /* [in] */ Int32 specifiedPort)
{
    if (specifiedPort != -1) {
        return specifiedPort;
    }

    if (String("http").EqualsIgnoreCase(scheme)) {
        return 80;
    }
    else if (String("http").EqualsIgnoreCase(scheme)) {
        return 443;
    }
    else {
        return -1;
    }
}

ECode CURI::GetQuery(
    /* [out] */ String* query)
{
    VALIDATE_NOT_NULL(query);
    return Decode(mQuery, query);
}

ECode CURI::GetRawAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority);
    *authority = mAuthority;

    return NOERROR;
}

ECode CURI::GetRawFragment(
    /* [out] */ String* fragment)
{
    VALIDATE_NOT_NULL(fragment);
    *fragment = mFragment;

    return NOERROR;
}

ECode CURI::GetRawPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = mPath;

    return NOERROR;
}

ECode CURI::GetRawQuery(
    /* [out] */ String* query)
{
    VALIDATE_NOT_NULL(query);
    *query = mQuery;

    return NOERROR;
}

ECode CURI::GetRawSchemeSpecificPart(
    /* [out] */ String* schemeSpecific)
{
    VALIDATE_NOT_NULL(schemeSpecific);
    *schemeSpecific = mSchemeSpecificPart;

    return NOERROR;
}

ECode CURI::GetRawUserInfo(
    /* [out] */ String* userInfo)
{
    VALIDATE_NOT_NULL(userInfo);
    *userInfo = mUserInfo;

    return NOERROR;
}

ECode CURI::GetScheme(
    /* [out] */ String* scheme)
{
    VALIDATE_NOT_NULL(scheme);
    *scheme = mScheme;

    return NOERROR;
}

ECode CURI::GetSchemeSpecificPart(
    /* [out] */ String* schemeSpecific)
{
    VALIDATE_NOT_NULL(schemeSpecific);
    return Decode(mSchemeSpecificPart, schemeSpecific);
}

ECode CURI::GetUserInfo(
    /* [out] */ String* userInfo)
{
    VALIDATE_NOT_NULL(userInfo);
    return Decode(mUserInfo, userInfo);
}

ECode CURI::HashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    if (mHash == -1) {
        mHash = GetHashString().GetHashCode();
    }
    *value = mHash;
    return NOERROR;
}

ECode CURI::IsAbsolute(
    /* [out] */ Boolean* isAbsolute)
{
    VALIDATE_NOT_NULL(isAbsolute);
    *isAbsolute = mAbsolute;

    return NOERROR;
}

ECode CURI::IsOpaque(
    /* [out] */ Boolean* isOpaque)
{
    VALIDATE_NOT_NULL(isOpaque);
    *isOpaque = mOpaque;

    return NOERROR;
}

String CURI::NormalizeInner(
    /* [in] */ const String& path)
{
    // count the number of '/'s, to determine number of segments
    Int32 index = -1;
    Int32 pathLength = path.GetLength();
    Int32 size = 0;
    if (pathLength > 0 && path[0] != '/') {
        size++;
    }
    while ((index = path.IndexOf('/', index + 1)) != -1) {
        if (index + 1 < pathLength && path[index + 1] != '/') {
            size++;
        }
    }

    ArrayOf<String>* segList = ArrayOf<String>::Alloc(size);
    ArrayOf<Boolean>* include = ArrayOf<Boolean>::Alloc(size);

    // break the path into segments and store in the list
    Int32 current = 0;
    Int32 index2;
    index = (pathLength > 0 && path[0] == '/') ? 1 : 0;
    while ((index2 = path.IndexOf('/', index + 1)) != -1) {
        (*segList)[current++] = path.Substring(index, index2);
        index = index2 + 1;
    }

    // if current==size, then the last character was a slash
    // and there are no more segments
    if (current < size) {
        (*segList)[current] = path.Substring(index);
    }

    // determine which segments get included in the normalized path
    for (Int32 i = 0; i < size; i++) {
        (*include)[i] = TRUE;
        if ((*segList)[i].Equals("..")) {
            Int32 remove = i - 1;
            // search back to find a segment to remove, if possible
            while (remove > -1 && !(*include)[remove]) {
                remove--;
            }
            // if we find a segment to remove, remove it and the ".."
            // segment
            if (remove > -1 && !(*segList)[remove].Equals("..")) {
                (*include)[remove] = FALSE;
                (*include)[i] = FALSE;
            }
        }
        else if ((*segList)[i].Equals(".")) {
            (*include)[i] = FALSE;
        }
    }

    // put the path back together
//    StringBuilder newPath = new StringBuilder();
    String newPath;
    if (path.StartWith("/")) {
        newPath += "/";
    }

    for (Int32 i = 0; i < segList->GetLength(); i++) {
        if (include[i]) {
            newPath += (*segList)[i];
            newPath += "/";
        }
    }

    // if we used at least one segment and the path previously ended with
    // a slash and the last segment is still used, then delete the extra
    // trailing '/'
    if (!path.EndWith("/") && segList->GetLength() > 0
            && (*include)[segList->GetLength() - 1]) {
        newPath = newPath.Substring(0, newPath.GetLength() - 1);
    }

    // check for a ':' in the first segment if one exists,
    // prepend "./" to normalize
    index = newPath.IndexOf(':');
    index2 = newPath.IndexOf('/');
    if (index != -1 && (index < index2 || index2 == -1)) {
        newPath = String("./") + newPath;
//        newPath.insert(0, "./");
    }

    ArrayOf<String>::Free(segList);
    ArrayOf<Boolean>::Free(include);
    return newPath;
}

ECode CURI::Normalize(
    /* [out] */ IURI** uri)
{
    VALIDATE_NOT_NULL(uri);

    if (mOpaque) {
        *uri = (IURI*)this;
        return NOERROR;
    }
    String normalizedPath = NormalizeInner(mPath);
    // if the path is already normalized, return this
    if (mPath.Equals(normalizedPath)) {
        *uri = (IURI*)this;
        return NOERROR;
    }
    // get an exact copy of the URI re-calculate the scheme specific part
    // since the path of the normalized URI is different from this URI.
    AutoPtr<CURI> result = Duplicate();
    result->mPath = normalizedPath;
    result->SetSchemeSpecificPart();
    *uri = (IURI*)result;

    return NOERROR;
}

ECode CURI::ParseServerAuthority()
{
    if (!mServerAuthority) {
        ParseAuthority(TRUE);
    }

    return NOERROR;
}

ECode CURI::Relativize(
    /* [in] */ IURI* relative,
    /* [out] */ IURI** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<CURI> relativeURI = (CURI*)relative;
    if (relativeURI->mOpaque || mOpaque) {
        *uri = relative;
        return NOERROR;
    }

    if (mScheme.IsNull() ? !relativeURI->mScheme.IsNull() : !mScheme
            .Equals(relativeURI->mScheme)) {
        *uri = relative;
        return NOERROR;
    }

    if (mAuthority.IsNull() ? !relativeURI->mAuthority.IsNull() : !mAuthority
            .Equals(relativeURI->mAuthority)) {
        *uri = relative;
        return NOERROR;
    }

    // normalize both paths
    String thisPath = NormalizeInner(mPath);
    String relativePath = NormalizeInner(relativeURI->mPath);

    /*
     * if the paths aren't equal, then we need to determine if this URI's
     * path is a parent path (begins with) the relative URI's path
     */
    if (!thisPath.Equals(relativePath)) {
        // if this URI's path doesn't end in a '/', add one
        if (!thisPath.EndWith("/")) {
            thisPath = thisPath + String("/");
        }
        /*
         * if the relative URI's path doesn't start with this URI's path,
         * then just return the relative URI; the URIs have nothing in
         * common
         */
        if (!relativePath.StartWith(thisPath)) {
            *uri = relative;
            return NOERROR;
        }
    }

    AutoPtr<CURI> result = new CURI();
    result->mFragment = relativeURI->mFragment;
    result->mQuery = relativeURI->mQuery;
    // the result URI is the remainder of the relative URI's path
    result->mPath = relativePath.Substring(thisPath.GetLength());
    result->SetSchemeSpecificPart();
    *uri = (IURI*)result;

    return NOERROR;
}

ECode CURI::Resolve(
    /* [in] */ IURI* relative,
    /* [out] */ IURI** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<CURI> relativeURI = (CURI*)relative;
    if (relativeURI->mAbsolute || mOpaque) {
        *uri = relative;
        return NOERROR;
    }

    AutoPtr<CURI> result;
    if (relativeURI->mPath.IsEmpty() && relativeURI->mScheme.IsNull()
            && relativeURI->mAuthority.IsNull() && relativeURI->mQuery.IsNull()
            && !relativeURI->mFragment.IsNull()) {
        // if the relative URI only consists of fragment,
        // the resolved URI is very similar to this URI,
        // except that it has the fragment from the relative URI.
        result = Duplicate();
        result->mFragment = relativeURI->mFragment;
        // no need to re-calculate the scheme specific part,
        // since fragment is not part of scheme specific part.
        *uri = (IURI*)result;
        return NOERROR;
    }

    if (!relativeURI->mAuthority.IsNull()) {
        // if the relative URI has authority,
        // the resolved URI is almost the same as the relative URI,
        // except that it has the scheme of this URI.
        result = relativeURI->Duplicate();
        result->mScheme = mScheme;
        result->mAbsolute = mAbsolute;
    }
    else {
        // since relative URI has no authority,
        // the resolved URI is very similar to this URI,
        // except that it has the query and fragment of the relative URI,
        // and the path is different.
        result = Duplicate();
        result->mFragment = relativeURI->mFragment;
        result->mQuery = relativeURI->mQuery;
        if (relativeURI->mPath.StartWith("/")) {
            result->mPath = relativeURI->mPath;
        }
        else {
            // resolve a relative reference
            Int32 endIndex = mPath.LastIndexOf('/') + 1;
            result->mPath = NormalizeInner(mPath.Substring(0, endIndex)
                    + relativeURI->mPath);
        }
        // re-calculate the scheme specific part since
        // query and path of the resolved URI is different from this URI.
        result->SetSchemeSpecificPart();
    }

    *uri = (IURI*)result;
    return NOERROR;
}

void CURI::SetSchemeSpecificPart()
{
    // ssp = [//authority][path][?query]
    StringBuffer ssp;
    if (!mAuthority.IsNull()) {
        ssp += "//" + mAuthority;
    }
    if (!mPath.IsNull()) {
        ssp += mPath;
    }
    if (!mQuery.IsNull()) {
        ssp += "?" + mQuery;
    }
    mSchemeSpecificPart = (String)ssp;
    // reset string, so that it can be re-calculated correctly when asked.
    mString = String(NULL);
}

ECode CURI::ResolveEx(
    /* [in] */ const String& relative,
    /* [out] */ IURI** uri)
{
    AutoPtr<IURI> relativeURI;
    Create(relative, (IURI**)&relativeURI);
    return Resolve(relativeURI, uri);
}

ECode CURI::EncodeNonAscii(
    /* [in] */ const String& s,
    /* [out] */ String* encodeS)
{
//    try {
    /*
     * Use a different encoder than URLEncoder since: 1. chars like "/",
     * "#", "@" etc needs to be preserved instead of being encoded, 2.
     * UTF-8 char set needs to be used for encoding instead of default
     * platform one 3. Only other chars need to be converted
     */
    ECode ec = URIEncoderDecoder::EncodeOthers(s, encodeS);
    if (FAILED(ec)) {
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
//    } catch (UnsupportedEncodingException e) {
//        throw new RuntimeException(e.toString());
//    }
}

ECode CURI::Decode(
    /* [in] */ const String& s,
    /* [out] */ String* decodeS)
{
    if (s.IsNull()) {
        *decodeS = s;
    }

//    try {
    ECode ec = URIEncoderDecoder::Decode(s, decodeS);
    if (ec == E_UNSUPPORTED_ENCODING_EXCEPTION) {
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
//    } catch (UnsupportedEncodingException e) {
//        throw new RuntimeException(e.toString());
//    }
}

ECode CURI::ToASCIIString(
    /* [out] */ String* str)
{
    String s;
    ToString(&s);
    return EncodeNonAscii(s, str);
}

ECode CURI::ToString(
    /* [out] */ String* s)
{
    VALIDATE_NOT_NULL(s);

    if (mString.IsNull()) {
        StringBuffer result;
        if (!mScheme.IsNull()) {
            result += mScheme;
            result += ':';
        }
        if (mOpaque) {
            result += mSchemeSpecificPart;
        }
        else {
            if (!mAuthority.IsNull()) {
                result += "//";
                result += mAuthority;
            }

            if (!mPath.IsNull()) {
                result += mPath;
            }

            if (!mQuery.IsNull()) {
                result += '?';
                result += mQuery;
            }
        }

        if (!mFragment.IsNull()) {
            result += '#';
            result += mFragment;
        }

        mString = (String)result;
    }

    *s = mString;
    return NOERROR;
}

String CURI::GetHashString()
{
    StringBuffer result;
    if (!mScheme.IsNull()) {
        String temp = mScheme;
        temp.ToLowerCase();
        result += temp;
        result += ':';
    }
    if (mOpaque) {
        result += mSchemeSpecificPart;
    }
    else {
        if (!mAuthority.IsNull()) {
            result += "//";
            if (mHost.IsNull()) {
                result += mAuthority;
            }
            else {
                if (!mUserInfo.IsNull()) {
                    result += mUserInfo + "@";
                }
                String temp = mHost;
                temp.ToLowerCase();
                result += temp;
                if (mPort != -1) {
                    result += ":" + mPort;
                }
            }
        }

        if (!mPath.IsNull()) {
            result += mPath;
        }

        if (!mQuery.IsNull()) {
            result += '?';
            result += mQuery;
        }
    }

    if (!mFragment.IsNull()) {
        result += '#';
        result += mFragment;
    }

    return ConvertHexToLowerCase((String)result);
}

ECode CURI::ToURL(
    /* [out] */ IURL** url)
{
    VALIDATE_NOT_NULL(url);

    if (!mAbsolute) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("URI is not absolute: " + toString());
    }

    String s;
    ToString(&s);
    return NOERROR;
    return CURL::New(s, url);
}

ECode CURI::constructor(
    /* [in] */ const String& uri)
{
    return ParseURI(uri, FALSE);
}

ECode CURI::constructor(
    /* [in] */ const String& scheme,
    /* [in] */ const String& ssp,
    /* [in] */ const String& frag)
{
    StringBuffer uri;
    if (!scheme.IsNull()) {
        uri += scheme;
        uri += ':';
    }
    if (!ssp.IsNull()) {
        // QUOTE ILLEGAL CHARACTERS
        uri += QuoteComponent(ssp, String(URI_ALL_LEGAL));
    }
    if (!frag.IsNull()) {
        uri += '#';
        // QUOTE ILLEGAL CHARACTERS
        uri += QuoteComponent(frag, String(URI_ALL_LEGAL));
    }

    return ParseURI(String(uri), FALSE);
}

ECode CURI::constructor(
    /* [in] */ const String& scheme,
    /* [in] */ const String& userInfo,
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ const String& path,
    /* [in] */ const String& query,
    /* [in] */ const String& fragment)
{
    if (scheme.IsNull() && userInfo.IsNull() && host.IsNull() && path.IsNull()
            && query.IsNull() && fragment.IsNull()) {
        mPath = "";
        return NOERROR;
    }

    if (!scheme.IsNull() && !path.IsNull() && path.GetLength() > 0
            && path[0] != '/') {
        return E_URI_SYNTAX_EXCEPTION;
//        throw new URISyntaxException(path, "Relative path");
    }

    StringBuffer uri;
    if (!scheme.IsNull()) {
        uri += scheme;
        uri += ':';
    }

    if (!userInfo.IsNull() || !host.IsNull() || port != -1) {
        uri += "//";
    }

    if (!userInfo.IsNull()) {
        // QUOTE ILLEGAL CHARACTERS in userInfo
        uri += QuoteComponent(userInfo, String(URI_SOME_LEGAL));
        uri += '@';
    }

    if (!host.IsNull()) {
        // check for IPv6 addresses that hasn't been enclosed
        // in square brackets
        String temp = host;
        if (host.IndexOf(':') != -1 && host.IndexOf(']') == -1
                && host.IndexOf('[') == -1) {
//            host = "[" + host + "]";
            //todo the value of host has been changed
            temp = String("[") + host + String("]");
        }
        uri += temp;
    }

    if (port != -1) {
        uri += ':';
        uri += port;
    }

    if (!path.IsNull()) {
        // QUOTE ILLEGAL CHARS
        uri += QuoteComponent(path, String("/@") + String(URI_SOME_LEGAL));
    }

    if (!query.IsNull()) {
        uri += '?';
        // QUOTE ILLEGAL CHARS
        uri += QuoteComponent(query, String(URI_ALL_LEGAL));
    }

    if (!fragment.IsNull()) {
        // QUOTE ILLEGAL CHARS
        uri += '#';
        uri += QuoteComponent(fragment, String(URI_ALL_LEGAL));
    }

    return ParseURI(String(uri), TRUE);
}

ECode CURI::constructor(
    /* [in] */ const String& scheme,
    /* [in] */ const String& host,
    /* [in] */ const String& path,
    /* [in] */ const String& fragment)
{
    return constructor(scheme, String(NULL), host, -1, path, String(NULL), fragment);
}

ECode CURI::constructor(
    /* [in] */ const String& scheme,
    /* [in] */ const String& authority,
    /* [in] */ const String& path,
    /* [in] */ const String& query,
    /* [in] */ const String& fragment)
{
    if (scheme.IsNull() && !path.IsNull() && path.GetLength() > 0
            && path[0] != '/') {
        return E_URI_SYNTAX_EXCEPTION;
//        throw new URISyntaxException(path, "Relative path");
    }

    StringBuffer uri;
    if (!scheme.IsNull()) {
        uri += scheme;
        uri += ':';
    }
    if (!authority.IsNull()) {
        uri += "//";
        // QUOTE ILLEGAL CHARS
        uri += QuoteComponent(authority, String("@[]") + String(URI_SOME_LEGAL));
    }

    if (!path.IsNull()) {
        // QUOTE ILLEGAL CHARS
        uri += QuoteComponent(path, String("/@") + String(URI_SOME_LEGAL));
    }
    if (!query.IsNull()) {
        // QUOTE ILLEGAL CHARS
        uri += '?';
        uri += QuoteComponent(query, String(URI_ALL_LEGAL));
    }
    if (!fragment.IsNull()) {
        // QUOTE ILLEGAL CHARS
        uri += '#';
        uri += QuoteComponent(fragment, String(URI_ALL_LEGAL));
    }

    return ParseURI(String(uri), FALSE);
}


