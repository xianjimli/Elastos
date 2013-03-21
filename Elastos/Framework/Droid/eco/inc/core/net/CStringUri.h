
#ifndef __CSTRINGURI_H__
#define __CSTRINGURI_H__

#include "_CStringUri.h"
#include "net/Uri.h"

using namespace Elastos;

CarClass(CStringUri)
{
public:
    CStringUri();

    ~CStringUri();

    CARAPI IsHierarchical(
        /* [out] */ Boolean* isHierarchical);

    CARAPI IsOpaque(
        /* [out] */ Boolean* isOpaque);

    CARAPI IsRelative(
        /* [out] */ Boolean* isRelative);

    CARAPI IsAbsolute(
        /* [out] */ Boolean* isAbsolute);

    CARAPI GetScheme(
        /* [out] */ String* scheme);

    CARAPI GetSchemeSpecificPart(
        /* [out] */ String* ssp);

    CARAPI GetEncodedSchemeSpecificPart(
        /* [out] */ String* essp);

    CARAPI GetAuthority(
        /* [out] */ String* authority);

    CARAPI GetEncodedAuthority(
        /* [out] */ String* authority);

    CARAPI GetUserInfo(
        /* [out] */ String* userInfo);

    CARAPI GetEncodedUserInfo(
        /* [out] */ String* userInfo);

    CARAPI GetHost(
        /* [out] */ String* host);

    CARAPI GetPort(
        /* [out] */ Int32* port);

    CARAPI GetPath(
        /* [out] */ String* path);

    CARAPI GetEncodedPath(
        /* [out] */ String* path);

    CARAPI GetQuery(
        /* [out] */ String* query);

    CARAPI GetEncodedQuery(
        /* [out] */ String* query);

    CARAPI GetFragment(
        /* [out] */ String* fragment);

    CARAPI GetEncodedFragment(
        /* [out] */ String* fragment);

    CARAPI GetPathSegments(
        /* [out, callee] */ ArrayOf<String>** pathSegments);

    CARAPI GetLastPathSegment(
        /* [out] */ String* pathSegment);

    CARAPI Equals(
        /* [in] */ IUri* other,
        /* [out] */ Boolean* isEqual);

    CARAPI CompareTo(
        /* [in] */ IUri* other,
        /* [out] */ Int32* result);

    CARAPI GetDescription(
        /* [out] */ String* uriString);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI ToString(
        /* [out] */ String* uriString);

    CARAPI constructor(
        /* [in] */ const String& uriString);

    CARAPI constructor(
        /* [in] */ Handle32 uri);

private:
    /** Finds the first ':'. Returns -1 if none found. */
    CARAPI_(Int32) FindSchemeSeparator();

    /**
     * Parses an authority out of the given URI string.
     *
     * @param uriString URI string
     * @param ssi scheme separator index, -1 for a relative URI
     *
     * @return the authority or null if none is found
     */
    static CARAPI ParseAuthority(
        /* [in] */ const String& uriString,
        /* [in] */ Int32 ssi,
        /* [out] */ String * authority);

    CARAPI_(Uri::Part*) GetAuthorityPart();

    CARAPI ParseScheme(
        /* [out] */ String * scheme);

private:
    String mUriString;
    String mScheme;
    Uri::Part* mAuthority;
    Int32 mCachedSsi;
};

#endif // __CSTRINGURI_H__
