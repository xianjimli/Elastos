
#include "cmdef.h"
#include "CURI.h"
#include "URIEncoderDecoder.h"
#include "InetAddress.h"
#include "CURL.h"
#include <StringBuffer.h>
#include <elastos/Character.h>
#include <stdio.h>

const String CURI::UNRESERVED = String("_-!.~\'()*");
const String CURI::PUNCTUATION = String(",;:$&+=");
const String CURI::RESERVED = String(",;:$&+=?/[]@");
const String CURI::SOME_LEGAL = String("_-!.~\'()*,;:$&+=");
const String CURI::ALL_LEGAL = String("_-!.~\'()*,;:$&+=?/[]@");

//AutoPtr<INetworkSystem> InitNetworkSystem()
//{
//    printf("++%s, %d\n", __FILE__, __LINE__);
//    AutoPtr<IPlatform> platform;
//    ASSERT_SUCCEEDED(CPlatform::AcquireSingleton((IPlatform**)&platform));
//    printf("++%s, %d\n", __FILE__, __LINE__);
//    AutoPtr<INetworkSystem> networkSystem;
//    printf("++%s, %d\n", __FILE__, __LINE__);
//    platform->GetNetworkSystem((INetworkSystem**)&networkSystem);
//    printf("++%s, %d\n", __FILE__, __LINE__);
//    return networkSystem;
//}
//
//AutoPtr<INetworkSystem> CURI::NETWORK_SYSTEM = InitNetworkSystem();

CURI::CURI()
    : mPort(-1)
    , mOpaque(FALSE)
    , mAbsolute(FALSE)
    , mServerAuthority(FALSE)
    , mHash(-1)
{
    AutoPtr<IPlatform> platform;
    ASSERT_SUCCEEDED(CPlatform::AcquireSingleton((IPlatform**)&platform));
    platform->GetNetworkSystem((INetworkSystem**)&NETWORK_SYSTEM);
}

ECode CURI::constructor()
{
    return NOERROR;
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
        uri += QuoteComponent(ssp, ALL_LEGAL);
    }
    if (!frag.IsNull()) {
        uri += '#';
        // QUOTE ILLEGAL CHARACTERS
        uri += QuoteComponent(frag, ALL_LEGAL);
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

    if (!scheme.IsNull() && !path.IsNull() && path.GetCharCount() > 0
            && path.GetChar(0) != '/') {
//        throw new URISyntaxException(path, "Relative path");
        return E_URI_SYNTAX_EXCEPTION;
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
        uri += QuoteComponent(userInfo, SOME_LEGAL);
        uri += '@';
    }

    if (!host.IsNull()) {
        // check for IPv6 addresses that hasn't been enclosed
        // in square brackets
        String temp = host;
        if (host.IndexOf(':') != -1 && host.IndexOf(']') == -1
                && host.IndexOf('[') == -1) {
//            host = "[" + host + "]";
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
        uri += QuoteComponent(path, String("/@") + SOME_LEGAL);
    }

    if (!query.IsNull()) {
        uri += '?';
        // QUOTE ILLEGAL CHARS
        uri += QuoteComponent(query, ALL_LEGAL);
    }

    if (!fragment.IsNull()) {
        // QUOTE ILLEGAL CHARS
        uri += '#';
        uri += QuoteComponent(fragment, ALL_LEGAL);
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
    if (scheme.IsNull() && !path.IsNull() && path.GetCharCount() > 0
            && path.GetChar(0) != '/') {
//        throw new URISyntaxException(path, "Relative path");
        return E_URI_SYNTAX_EXCEPTION;
    }

    StringBuffer uri;
    if (!scheme.IsNull()) {
        uri += scheme;
        uri += ':';
    }
    if (!authority.IsNull()) {
        uri += "//";
        // QUOTE ILLEGAL CHARS
        uri += QuoteComponent(authority, String("@[]") + SOME_LEGAL);
    }

    if (!path.IsNull()) {
        // QUOTE ILLEGAL CHARS
        uri += QuoteComponent(path, String("/@") + SOME_LEGAL);
    }
    if (!query.IsNull()) {
        // QUOTE ILLEGAL CHARS
        uri += '?';
        uri += QuoteComponent(query, ALL_LEGAL);
    }
    if (!fragment.IsNull()) {
        // QUOTE ILLEGAL CHARS
        uri += '#';
        uri += QuoteComponent(fragment, ALL_LEGAL);
    }

    return ParseURI(String(uri), FALSE);
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
        FAIL_RETURN(ValidateFragment(uri, mFragment, index + 1));
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
//            throw new URISyntaxException(uri, "Scheme expected", index);
            return E_URI_SYNTAX_EXCEPTION;
        }
        FAIL_RETURN(ValidateScheme(uri, mScheme, 0));
        mSchemeSpecificPart = temp.Substring(index + 1);
        if (mSchemeSpecificPart.GetLength() == 0) {
//            throw new URISyntaxException(uri, "Scheme-specific part expected", index + 1);
            return E_URI_SYNTAX_EXCEPTION;
        }
    }
    else {
        mAbsolute = FALSE;
        mSchemeSpecificPart = temp;
    }

    if (mScheme.IsNull() || mSchemeSpecificPart.GetCharCount() > 0
            && mSchemeSpecificPart.GetChar(0) == '/') {
        mOpaque = FALSE;
        // the URI is hierarchical

        // Query
        temp = mSchemeSpecificPart;
        index = temp.IndexOf('?');
        if (index != -1) {
            mQuery = temp.Substring(index + 1);
            temp = temp.Substring(0, index);
            FAIL_RETURN(ValidateQuery(uri, mQuery, index2 + 1 + index));
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
//                    throw new URISyntaxException(uri, "Authority expected", uri.length());
                    return E_URI_SYNTAX_EXCEPTION;
                }

                mPath = "";
                // nothing left, so path is empty (not null, path should
                // never be null)
            }

            if (mAuthority.GetLength() == 0) {
                mAuthority = NULL;
            }
            else {
                FAIL_RETURN(ValidateAuthority(uri, mAuthority, index1 + 3));
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
        FAIL_RETURN(ValidatePath(uri, mPath, pathIndex));
    }
    else { // if not hierarchical, URI is opaque
        mOpaque = TRUE;
        FAIL_RETURN(ValidateSsp(uri, mSchemeSpecificPart, index2 + 2 + index));
    }

    return ParseAuthority(forceServer);
}

ECode CURI::ValidateScheme(
    /* [in] */ const String& uri,
    /* [in] */ const String& scheme,
    /* [in] */ Int32 index)
{
    // first char needs to be an alpha char
    Char32 ch = scheme.GetChar(0);
    if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))) {
//        throw new URISyntaxException(uri, "Illegal character in scheme", 0);
        return E_URI_SYNTAX_EXCEPTION;
    }

//    try {
    return URIEncoderDecoder::ValidateSimple(scheme, String("+-."));
    // } catch (URISyntaxException e) {
    //     throw new URISyntaxException(uri, "Illegal character in scheme", index + e.getIndex());
    // }
}

ECode CURI::ValidateSsp(
    /* [in] */ const String& uri,
    /* [in] */ const String& ssp,
    /* [in] */ Int32 index)
{
    // try {
    return URIEncoderDecoder::Validate(ssp, ALL_LEGAL);
    // } catch (URISyntaxException e) {
    //     throw new URISyntaxException(uri,
    //             e.getReason() + " in schemeSpecificPart", index + e.getIndex());
    // }
}

ECode CURI::ValidateAuthority(
    /* [in] */ const String& uri,
    /* [in] */ const String& authority,
    /* [in] */ Int32 index)
{
    // try {
    return URIEncoderDecoder::Validate(authority, String("@[]") + SOME_LEGAL);
    // } catch (URISyntaxException e) {
    //     throw new URISyntaxException(uri, e.getReason() + " in authority", index + e.getIndex());
    // }
}

ECode CURI::ValidatePath(
    /* [in] */ const String& uri,
    /* [in] */ const String& path,
    /* [in] */ Int32 index)
{
    // try {
    return URIEncoderDecoder::Validate(path, String("/@") + SOME_LEGAL);
    // } catch (URISyntaxException e) {
    //     throw new URISyntaxException(uri, e.getReason() + " in path", index + e.getIndex());
    // }
}

ECode CURI::ValidateQuery(
    /* [in] */ const String& uri,
    /* [in] */ const String& query,
    /* [in] */ Int32 index)
{
    // try {
    return URIEncoderDecoder::Validate(query, ALL_LEGAL);
    // } catch (URISyntaxException e) {
    //     throw new URISyntaxException(uri, e.getReason() + " in query", index + e.getIndex());
    // }
}

ECode CURI::ValidateFragment(
    /* [in] */ const String& uri,
    /* [in] */ const String& fragment,
    /* [in] */ Int32 index)
{
    // try {
    return URIEncoderDecoder::Validate(fragment, ALL_LEGAL);
    // } catch (URISyntaxException e) {
    //     throw new URISyntaxException(uri, e.getReason() + " in fragment", index + e.getIndex());
    // }
}

ECode CURI::ParseAuthority(
    /* [in] */ Boolean forceServer)
{
    if (mAuthority.IsNull()) {
        return NOERROR;
    }

    String tempUserInfo;
    String temp = mAuthority;
    Int32 index = temp.IndexOf('@');
    Int32 hostIndex = 0;
    if (index != -1) {
        // remove user info
        tempUserInfo = temp.Substring(0, index);
        FAIL_RETURN(ValidateUserInfo(mAuthority, tempUserInfo, 0));
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
//                    throw new URISyntaxException(authority,
//                            "Invalid port number", hostIndex + index + 1);S
                    return E_URI_SYNTAX_EXCEPTION;
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
//            throw new URISyntaxException(authority, "Expected host", hostIndex);
            return E_URI_SYNTAX_EXCEPTION;
        }
        return NOERROR;
    }

    Boolean isValid;
    FAIL_RETURN(IsValidHost(forceServer, tempHost, &isValid));
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
    Int32 count = userInfo.GetCharCount();
    for (Int32 i = 0; i < count; i++) {
        Char32 ch = userInfo.GetChar(i);
        if (ch == ']' || ch == '[') {
//            throw new URISyntaxException(uri, "Illegal character in userInfo", index + i);
            return E_URI_SYNTAX_EXCEPTION;
        }
    }

    return NOERROR;
}

ECode CURI::IsValidHost(
    /* [in] */ Boolean forceServer,
    /* [in] */ const String& host,
    /* [out] */ Boolean* isValid)
{
    assert(isValid != NULL);

    if (host.StartWith("[")) {
        // IPv6 address
        if (!host.EndWith("]")) {
//            throw new URISyntaxException(host,
//                    "Expected a closing square bracket for IPv6 address", 0);
            return E_URI_SYNTAX_EXCEPTION;
        }
//        try {
        ArrayOf<Byte>* bytes;
        FAIL_RETURN(InetAddress::IpStringToByteArray(host, &bytes));
        /*
         * The native IP parser may return 4 bytes for addresses like
         * "[::FFFF:127.0.0.1]". This is allowed, but we must not accept
         * IPv4-formatted addresses in square braces like "[127.0.0.1]".
         */
        if (bytes->GetLength() == 16 || bytes->GetLength() == 4 && host.Contains(":")) {
            *isValid = TRUE;
            ArrayOf<Byte>::Free(bytes);
            return NOERROR;
        }
//        } catch (UnknownHostException e) {
//        }
//        throw new URISyntaxException(host, "Malformed IPv6 address");
        ArrayOf<Byte>::Free(bytes);
        return E_URI_SYNTAX_EXCEPTION;
    }

    // '[' and ']' can only be the first char and last char
    // of the host name
    if (host.IndexOf('[') != -1 || host.IndexOf(']') != -1) {
//        throw new URISyntaxException(host, "Illegal character in host name", 0);
        return E_URI_SYNTAX_EXCEPTION;
    }

    Int32 index = host.LastIndexOf('.');
    if (index < 0 || (UInt32)index == host.GetCharCount() - 1
            || !Character::IsDigit(host.GetChar(index + 1))) {
        // domain name
        if (IsValidDomainName(host)) {
            *isValid = TRUE;
            return NOERROR;
        }
        if (forceServer) {
//            throw new URISyntaxException(host, "Illegal character in host name", 0);
            return E_URI_SYNTAX_EXCEPTION;
        }
        *isValid = FALSE;
        return NOERROR;
    }

    // IPv4 address
//    try {
    ArrayOf<Byte>* bytes;
    FAIL_RETURN(InetAddress::IpStringToByteArray(host, &bytes));
    if (bytes->GetLength() == 4) {
        *isValid = TRUE;
        ArrayOf<Byte>::Free(bytes);
        return NOERROR;
    }
//    } catch (UnknownHostException e) {
//    }

    ArrayOf<Byte>::Free(bytes);
    if (forceServer) {
//        throw new URISyntaxException(host, "Malformed IPv4 address", 0);
        return E_URI_SYNTAX_EXCEPTION;
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

    String lastLabel;
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
        Char32 ch = lastLabel.GetChar(0);
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
    CURI* uriObj = (CURI*)uri;
    if (mScheme.IsNull() && !uriObj->mScheme.IsNull()) {
        *result = -1;
        return NOERROR;
    }
    else if (!mScheme.IsNull() && uriObj->mScheme.IsNull()) {
        *result = 1;
        return NOERROR;
    }
    else if (!mScheme.IsNull() && !uriObj->mScheme.IsNull()) {
        ret = mScheme.Compare(uriObj->mScheme, StringCase_Insensitive);
        if (ret != 0) {
            *result = ret;
            return NOERROR;
        }
    }

    // compare opacities
    if (!mOpaque && uriObj->mOpaque) {
        *result = -1;
        return NOERROR;
    }
    else if (mOpaque && !uriObj->mOpaque) {
        *result = 1;
        return NOERROR;
    }
    else if (mOpaque && uriObj->mOpaque) {
        ret = mSchemeSpecificPart.Compare(uriObj->mSchemeSpecificPart);
        if (ret != 0) {
            *result = ret;
            return NOERROR;
        }
    }
    else {
        // otherwise both must be hierarchical

        // compare authorities
        if (!mAuthority.IsNull() && uriObj->mAuthority.IsNull()) {
            *result = 1;
            return NOERROR;
        }
        else if (mAuthority.IsNull() && !uriObj->mAuthority.IsNull()) {
            *result = -1;
            return NOERROR;
        }
        else if (!mAuthority.IsNull() && !uriObj->mAuthority.IsNull()) {
            if (!mHost.IsNull() && !uriObj->mHost.IsNull()) {
                // both are server based, so compare userInfo, host, port
                if (!mUserInfo.IsNull() && uriObj->mUserInfo.IsNull()) {
                    *result = 1;
                    return NOERROR;
                }
                else if (mUserInfo.IsNull() && !uriObj->mUserInfo.IsNull()) {
                    *result = -1;
                    return NOERROR;
                }
                else if (!mUserInfo.IsNull() && !uriObj->mUserInfo.IsNull()) {
                    ret = mUserInfo.Compare(uriObj->mUserInfo);
                    if (ret != 0) {
                        *result = ret;
                        return NOERROR;
                    }
                }

                // userInfo's are the same, compare hostname
                ret = mHost.Compare(uriObj->mHost, StringCase_Insensitive);
                if (ret != 0) {
                    *result = ret;
                    return NOERROR;
                }

                // compare port
                if (mPort != uriObj->mPort) {
                    return mPort - uriObj->mPort;
                }
            }
            else { // one or both are registry based, compare the whole
                // authority
                ret = mAuthority.Compare(uriObj->mAuthority);
                if (ret != 0) {
                    *result = ret;
                    return NOERROR;
                }
            }
        }

        // authorities are the same
        // compare paths
        ret = mPath.Compare(uriObj->mPath);
        if (ret != 0) {
            *result = ret;
            return NOERROR;
        }

        // compare queries

        if (!mQuery.IsNull() && uriObj->mQuery.IsNull()) {
            *result = 1;
            return NOERROR;
        }
        else if (mQuery.IsNull() && !uriObj->mQuery.IsNull()) {
            *result = -1;
            return NOERROR;
        }
        else if (!mQuery.IsNull() && !uriObj->mQuery.IsNull()) {
            ret = mQuery.Compare(uriObj->mQuery);
            if (ret != 0) {
                *result = ret;
                return NOERROR;
            }
        }
    }

    // everything else is identical, so compare fragments
    if (!mFragment.IsNull() && uriObj->mFragment.IsNull()) {
        *result = 1;
        return NOERROR;
    }
    else if (mFragment.IsNull() && !uriObj->mFragment.IsNull()) {
        *result = -1;
        return NOERROR;
    }
    else if (!mFragment.IsNull() && !uriObj->mFragment.IsNull()) {
        ret = mFragment.Compare(uriObj->mFragment);
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
    VALIDATE_NOT_NULL(obj);

//    try {
    ECode ec = CURI::New(uri, obj);
    if (ec == E_URI_SYNTAX_EXCEPTION) {
        ec = E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return ec;
//    } catch (URISyntaxException e) {
//        throw new IllegalArgumentException(e.getMessage());
//    }
}

AutoPtr<CURI> CURI::Duplicate()
{
    AutoPtr<CURI> clone;
    CURI::NewByFriend((CURI**)&clone);
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

    if (o == NULL || IURI::Probe(o) == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    CURI* uri = (CURI*)IURI::Probe(o);

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
                return NOERROR;
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
                    return NOERROR;
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
    *port = GetEffectivePort(mScheme, mPort);

    return NOERROR;
}

Int32 CURI::GetEffectivePort(
    /* [in] */ const String& scheme,
    /* [in] */ Int32 specifiedPort)
{
    if (specifiedPort != -1) {
        return specifiedPort;
    }

    if (CString("http").EqualsIgnoreCase(scheme)) {
        return 80;
    }
    else if (CString("https").EqualsIgnoreCase(scheme)) {
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

String CURI::Normalize(
    /* [in] */ const String& path)
{
    // count the number of '/'s, to determine number of segments
    Int32 index = -1;
    Int32 pathLength = path.GetCharCount();
    Int32 size = 0;
    if (pathLength > 0 && path.GetChar(0) != '/') {
        size++;
    }
    while ((index = path.IndexOf('/', index + 1)) != -1) {
        if (index + 1 < pathLength && path.GetChar(index + 1) != '/') {
            size++;
        }
    }

    ArrayOf<String>* segList = ArrayOf<String>::Alloc(size);
    ArrayOf<Boolean>* include = ArrayOf<Boolean>::Alloc(size);

    // break the path into segments and store in the list
    Int32 current = 0;
    Int32 index2;
    index = (pathLength > 0 && path.GetChar(0) == '/') ? 1 : 0;
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
    StringBuffer newPath;
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
        newPath.DeleteChar(newPath.GetCharCount() - 1);
    }

    String result = String(newPath);

    // check for a ':' in the first segment if one exists,
    // prepend "./" to normalize
    index = result.IndexOf(':');
    index2 = result.IndexOf('/');
    if (index != -1 && (index < index2 || index2 == -1)) {
        newPath.Insert(0, "./");
        result = newPath;
    }

    for (Int32 i = 0; i < segList->GetLength(); ++i) {
        (*segList)[i] = NULL;
    }
    ArrayOf<String>::Free(segList);
    ArrayOf<Boolean>::Free(include);
    return result;
}

ECode CURI::Normalize(
    /* [out] */ IURI** uri)
{
    VALIDATE_NOT_NULL(uri);

    if (mOpaque) {
        *uri = (IURI*)this;
        (*uri)->AddRef();
        return NOERROR;
    }
    String normalizedPath = Normalize(mPath);
    // if the path is already normalized, return this
    if (mPath.Equals(normalizedPath)) {
        *uri = (IURI*)this;
        (*uri)->AddRef();
        return NOERROR;
    }
    // get an exact copy of the URI re-calculate the scheme specific part
    // since the path of the normalized URI is different from this URI.
    AutoPtr<CURI> result = Duplicate();
    result->mPath = normalizedPath;
    result->SetSchemeSpecificPart();
    *uri = (IURI*)result.Get();
    (*uri)->AddRef();
    return NOERROR;
}

ECode CURI::ParseServerAuthority()
{
    if (!mServerAuthority) {
        return ParseAuthority(TRUE);
    }
    return NOERROR;
}

ECode CURI::Relativize(
    /* [in] */ IURI* relative,
    /* [out] */ IURI** uri)
{
    VALIDATE_NOT_NULL(uri);

    CURI* relativeObj = (CURI*)relative;
    if (relativeObj->mOpaque || mOpaque) {
        *uri = relative;
        (*uri)->AddRef();
        return NOERROR;
    }

    if (mScheme.IsNull() ? !relativeObj->mScheme.IsNull() : !mScheme
            .Equals(relativeObj->mScheme)) {
        *uri = relative;
        (*uri)->AddRef();
        return NOERROR;
    }

    if (mAuthority.IsNull() ? !relativeObj->mAuthority.IsNull() : !mAuthority
            .Equals(relativeObj->mAuthority)) {
        *uri = relative;
        (*uri)->AddRef();
        return NOERROR;
    }

    // normalize both paths
    String thisPath = Normalize(mPath);
    String relativePath = Normalize(relativeObj->mPath);

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
            (*uri)->AddRef();
            return NOERROR;
        }
    }

    AutoPtr<CURI> result;
    CURI::NewByFriend((CURI**)&result);
    result->mFragment = relativeObj->mFragment;
    result->mQuery = relativeObj->mQuery;
    // the result URI is the remainder of the relative URI's path
    result->mPath = relativePath.Substring(thisPath.GetLength());
    result->SetSchemeSpecificPart();
    *uri = (IURI*)result.Get();
    (*uri)->AddRef();
    return NOERROR;
}

ECode CURI::Resolve(
    /* [in] */ IURI* relative,
    /* [out] */ IURI** uri)
{
    VALIDATE_NOT_NULL(uri);

    CURI* relativeObj = (CURI*)relative;
    if (relativeObj->mAbsolute || mOpaque) {
        *uri = relative;
        (*uri)->AddRef();
        return NOERROR;
    }

    AutoPtr<CURI> result;
    if (relativeObj->mPath.IsEmpty() && relativeObj->mScheme.IsNull()
            && relativeObj->mAuthority.IsNull() && relativeObj->mQuery.IsNull()
            && !relativeObj->mFragment.IsNull()) {
        // if the relative URI only consists of fragment,
        // the resolved URI is very similar to this URI,
        // except that it has the fragment from the relative URI.
        result = Duplicate();
        result->mFragment = relativeObj->mFragment;
        // no need to re-calculate the scheme specific part,
        // since fragment is not part of scheme specific part.
        *uri = (IURI*)result.Get();
        (*uri)->AddRef();
        return NOERROR;
    }

    if (!relativeObj->mAuthority.IsNull()) {
        // if the relative URI has authority,
        // the resolved URI is almost the same as the relative URI,
        // except that it has the scheme of this URI.
        result = relativeObj->Duplicate();
        result->mScheme = mScheme;
        result->mAbsolute = mAbsolute;
    }
    else {
        // since relative URI has no authority,
        // the resolved URI is very similar to this URI,
        // except that it has the query and fragment of the relative URI,
        // and the path is different.
        result = Duplicate();
        result->mFragment = relativeObj->mFragment;
        result->mQuery = relativeObj->mQuery;
        if (relativeObj->mPath.StartWith("/")) {
            result->mPath = relativeObj->mPath;
        }
        else {
            // resolve a relative reference
            Int32 endIndex = mPath.LastIndexOf('/') + 1;
            result->mPath = Normalize(mPath.Substring(0, endIndex)
                    + relativeObj->mPath);
        }
        // re-calculate the scheme specific part since
        // query and path of the resolved URI is different from this URI.
        result->SetSchemeSpecificPart();
    }

    *uri = (IURI*)result.Get();
    (*uri)->AddRef();
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
    mString = NULL;
}

ECode CURI::ResolveEx(
    /* [in] */ const String& relative,
    /* [out] */ IURI** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IURI> relativeURI;
    FAIL_RETURN(Create(relative, (IURI**)&relativeURI));
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
    if (ec == E_UNSUPPORTED_ENCODING_EXCEPTION) {
        ec = E_RUNTIME_EXCEPTION;
    }
    return ec;
//    } catch (UnsupportedEncodingException e) {
//        throw new RuntimeException(e.toString());
//    }
}

ECode CURI::Decode(
    /* [in] */ const String& s,
    /* [out] */ String* decodedS)
{
    if (s.IsNull()) {
        *decodedS = s;
    }

//    try {
    ECode ec = URIEncoderDecoder::Decode(s, decodedS);
    if (ec == E_UNSUPPORTED_ENCODING_EXCEPTION) {
        ec = E_RUNTIME_EXCEPTION;
    }
    return ec;
//    } catch (UnsupportedEncodingException e) {
//        throw new RuntimeException(e.toString());
//    }
}

ECode CURI::ToASCIIString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

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
//        throw new IllegalArgumentException("URI is not absolute: " + toString());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    String s;
    ToString(&s);
    return CURL::New(s, url);
}
