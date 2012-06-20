
#include "net/CStringUri.h"
#include <StringBuffer.h>
#include <elastos/etl_hash_fun.h>

using namespace Elastos::System;

CStringUri::CStringUri() :
    mScheme(Uri::NOT_CACHED),
    mCachedSsi(Uri::NOT_CALCULATED)
{}

CStringUri::~CStringUri()
{}

ECode CStringUri::constructor(
    /* [in] */ String uriString)
{
    mUriString = String::Duplicate(uriString);

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

ECode CStringUri::ParseAuthority(
    /* [in] */ String uriString,
    /* [in] */ Int32 ssi,
    /* [out] */ String * authority)
{
    if (authority == NULL) return E_INVALID_ARGUMENT;

    Int32 length = uriString.GetLength();

    if (length > ssi + 2
            && uriString.GetChar(ssi + 1) == '/'
            && uriString.GetChar(ssi + 2) == '/') {
        Int32 end = ssi + 3;
        while (end < length) {
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
    } else {
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
    } else {
        *scheme = String::Duplicate(mScheme);
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
    } else {
        StringBuffer strBuf(mUriString);
        *scheme = strBuf.Substring(0, ssi);
    }

    return NOERROR;
}

ECode CStringUri::GetSchemeSpecificPart(
    /* [out] */ String* ssp)
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
        String::Free(encodedAuthority);
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

ECode CStringUri::GetPath(
    /* [out] */ String* path)
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
        String::Free(selfSig);
        String::Free(otherSig);
    }

    return NOERROR;
}

ECode CStringUri::GetDescription(
    /* [out] */ String* uriString)
{
    *uriString = String::Duplicate(mUriString);

    return NOERROR;
}

ECode CStringUri::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    if (hashCode == NULL) return E_INVALID_ARGUMENT;

    *hashCode = (Int32)HashString(mUriString);
    return NOERROR;
}
