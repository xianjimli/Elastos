
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

    CARAPI GetScheme(
        /* [out] */ String* scheme);

    CARAPI GetSchemeSpecificPart(
        /* [out] */ String* ssp);

    CARAPI GetAuthority(
        /* [out] */ String* authority);

    CARAPI GetPath(
        /* [out] */ String* path);

    CARAPI Equals(
        /* [in] */ IUri* other,
        /* [out] */ Boolean* isEqual);

    CARAPI GetDescription(
        /* [out] */ String* uriString);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI constructor(
        /* [in] */ String uriString);

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
        /* [in] */ String uriString,
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
