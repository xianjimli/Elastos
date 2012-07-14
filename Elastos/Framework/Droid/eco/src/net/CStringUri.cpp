
#include "net/CStringUri.h"
#include <StringBuffer.h>
#include <elastos/etl_hash_fun.h>

using namespace Elastos::Core;

CStringUri::CStringUri() :
    mScheme(Uri::NOT_CACHED),
    mCachedSsi(Uri::NOT_CALCULATED)
{}

CStringUri::~CStringUri()
{}

ECode CStringUri::constructor(
    /* [in] */ const String& uriString)
{
    mUriString = uriString;

    return NOERROR;
}

ECode CStringUri::constructor(
    /* [in] */ Handle32 uri)
{
    return NOERROR;
}

Int32 CStringUri::FindSchemeSeparator()
{
    return mCachedSsi == Uri::NOT_CALCULATED
        ? mCachedSsi = mUriString.IndexOf(":")
        : mCachedSsi;
}

ECode CStringUri::IsHierarchical(
    /* [out] */ Boolean* isHierarchical)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringUri::IsOpaque(
    /* [out] */ Boolean* isOpaque)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringUri::IsRelative(
    /* [out] */ Boolean* isRelative)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringUri::IsAbsolute(
    /* [out] */ Boolean* isAbsolute)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringUri::ParseAuthority(
    /* [in] */ const String& uriString,
    /* [in] */ Int32 ssi,
    /* [out] */ String * authority)
{
    if (authority == NULL) return E_INVALID_ARGUMENT;

    Int32 count = uriString.GetCharCount();

    if (count > ssi + 2
            && uriString.GetChar(ssi + 1) == '/'
            && uriString.GetChar(ssi + 2) == '/') {
        Int32 end = ssi + 3;
        while (end < count) {
            switch (uriString.GetChar(end)) {
                case '/': // Start of path
                case '?': // Start of query
                case '#': // Start of fragment
                    break;
                default:
                    end++;
            }
        }

        StringBuffer strBuf(uriString);
        *authority = strBuf.Substring(ssi + 3, end);
    }
    else {
        *authority = NULL;
    }

    return NOERROR;
}

ECode CStringUri::GetScheme(
    /* [out] */ String* scheme)
{
    if (scheme == NULL) return E_INVALID_ARGUMENT;

    if (!mScheme.Compare(Uri::NOT_CACHED)) {
        return ParseScheme(scheme);
    }
    else {
        *scheme = mScheme;
    }

    return NOERROR;
}

ECode CStringUri::ParseScheme(
    /* [out] */ String* scheme)
{
    if (scheme == NULL) return E_INVALID_ARGUMENT;

    Int32 ssi = FindSchemeSeparator();
    if (ssi == -1) {
        scheme = NULL;
    }
    else {
        *scheme = mUriString.Substring(0, ssi);
    }

    return NOERROR;
}

ECode CStringUri::GetSchemeSpecificPart(
    /* [out] */ String* ssp)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringUri::GetEncodedSchemeSpecificPart(
    /* [out] */ String* essp)
{
    return E_NOT_IMPLEMENTED;
}

Uri::Part* CStringUri::GetAuthorityPart()
{
    if (mAuthority == NULL) {
        String encodedAuthority;
        ECode ec = ParseAuthority(this->mUriString,
            FindSchemeSeparator(), &encodedAuthority);
        if (FAILED(ec)) {
            return NULL;
        }

        mAuthority = Uri::Part::FromEncoded(encodedAuthority);
        return mAuthority;
    }

    return mAuthority;
}

ECode CStringUri::GetAuthority(
    /* [out] */ String* authority)
{
    if (authority == NULL) return E_INVALID_ARGUMENT;

    return GetAuthorityPart()->GetDecoded(authority);

}

ECode CStringUri::GetEncodedAuthority(
    /* [out] */ String* authority)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringUri::GetUserInfo(
    /* [out] */ String* userInfo)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringUri::GetEncodedUserInfo(
    /* [out] */ String* userInfo)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringUri::GetHost(
    /* [out] */ String* host)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringUri::GetPort(
    /* [out] */ Int32* port)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringUri::GetPath(
    /* [out] */ String* path)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringUri::GetEncodedPath(
    /* [out] */ String* path)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringUri::GetQuery(
    /* [out] */ String* query)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringUri::GetEncodedQuery(
    /* [out] */ String* query)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringUri::GetFragment(
    /* [out] */ String* fragment)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringUri::GetEncodedFragment(
    /* [out] */ String* fragment)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringUri::GetPathSegments(
    /* [out, callee] */ ArrayOf<String>** pathSegments)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringUri::GetLastPathSegment(
    /* [out] */ String* pathSegment)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringUri::Equals(
    /* [in] */ IUri* other,
    /* [out] */ Boolean* isEqual)
{
    if (isEqual == NULL) return E_INVALID_ARGUMENT;

    *isEqual = FALSE;
    if (other != NULL) {
        String selfSig;
        String otherSig;
        GetDescription(&selfSig);
        other->GetDescription(&otherSig);
        if (!selfSig.Compare(otherSig)) {
            *isEqual = TRUE;
        }
    }

    return NOERROR;
}

ECode CStringUri::CompareTo(
    /* [in] */ IUri* other,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStringUri::GetDescription(
    /* [out] */ String* uriString)
{
    *uriString = mUriString;

    return NOERROR;
}

ECode CStringUri::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    if (hashCode == NULL) return E_INVALID_ARGUMENT;

    *hashCode = (Int32)HashString(mUriString);
    return NOERROR;
}
