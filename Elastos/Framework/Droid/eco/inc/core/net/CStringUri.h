
#ifndef __CSTRINGURI_H__
#define __CSTRINGURI_H__

#include "_CStringUri.h"
#include "net/Uri.h"
#include "net/CPart.h"
#include "net/CPathPart.h"

using namespace Elastos;

/**
 * An implementation which wraps a String URI. This URI can be opaque or
 * hierarchical, but we extend AbstractHierarchicalUri in case we need
 * the hierarchical functionality.
 */
class StringUri : public AbstractHierarchicalUri
{
protected:
    /** Used in parcelling. */
    static const Int32 TYPE_ID = 1;

    /** URI string representation. */
    String mUriString;

    /** Cached scheme separator index. */
    Int32 mCachedSsi;

    /** Cached fragment separator index. */
    Int32 mCachedFsi;

    String mScheme;
    AutoPtr<IPart> mSsp;
    AutoPtr<IPathPart> mPath;
    AutoPtr<IPart> mAuthority;
    AutoPtr<IPart> mQuery;
    AutoPtr<IPart> mFragment;

    StringUri()
        : mCachedSsi(Uri::NOT_CALCULATED)
        , mCachedFsi(Uri::NOT_CALCULATED)
        , mScheme(Uri::NOT_CACHED)
    {
    }

    CARAPI Init(
        /* [in] */ String uriString)
    {
        if (uriString.IsNull()) {
            // throw new NullPointerException("uriString");
            return E_NULL_POINTER_EXCEPTION;
        }

        mUriString = uriString;

        return NOERROR;
    }

    static CARAPI ReadFrom(
        /* [in] */ IParcel* parcel,
        /* [out] */ IUri** result);

    CARAPI_(Void) WriteToParcel(
        /* [in] */ IParcel* parcel,
        /* [in] */ Int32 flags)
    {
        parcel->WriteInt32(StringUri::TYPE_ID);
        parcel->WriteString(mUriString);
    }

    virtual CARAPI IsHierarchical(
        /* [out] */ Boolean* isHierarchical)
    {
        VALIDATE_NOT_NULL(isHierarchical);

        Int32 ssi = FindSchemeSeparator();

        if (ssi == Uri::NOT_FOUND) {
            // All relative URIs are hierarchical.
            *isHierarchical = TRUE;
            return NOERROR;
        }

        if (mUriString.GetLength() == (UInt32)(ssi + 1)) {
            // No ssp.
            *isHierarchical = FALSE;
            return NOERROR;
        }

        // If the ssp starts with a '/', this is hierarchical.
        *isHierarchical = mUriString.GetChar(ssi + 1) == '/';
        return NOERROR;
    }

    virtual CARAPI IsRelative(
        /* [out] */ Boolean* isRelative)
    {
        VALIDATE_NOT_NULL(isRelative);

        // Note: We return true if the index is 0
        *isRelative = FindSchemeSeparator() == Uri::NOT_FOUND;

        return NOERROR;
    }

    virtual CARAPI GetScheme(
        /* [out] */ String* scheme)
    {
        // @SuppressWarnings("StringEquality")
        Boolean cached = (!mScheme.Equals(Uri::NOT_CACHED));
        if (!cached) {
            ParseScheme(&mScheme);
        }

        *scheme = mScheme;

        return NOERROR;
    }

    virtual CARAPI GetEncodedSchemeSpecificPart(
        /* [out] */ String* essp)
    {
        AutoPtr<IPart> part;
        FAIL_RETURN(GetSsp((IPart**)&part));

        return part->GetEncoded(essp);
    }

    virtual CARAPI GetSchemeSpecificPart(
        /* [out] */ String* ssp)
    {
        AutoPtr<IPart> part;
        FAIL_RETURN(GetSsp((IPart**)&part));

        return part->GetDecoded(ssp);
    }

    virtual CARAPI GetEncodedAuthority(
        /* [out] */ String* authority)
    {
        AutoPtr<IPart> part;
        FAIL_RETURN(GetAuthorityPart((IPart**)&part));

        return part->GetEncoded(authority);
    }

    virtual CARAPI GetAuthority(
        /* [out] */ String* authority)
    {
        AutoPtr<IPart> part;
        FAIL_RETURN(GetAuthorityPart((IPart**)&part));

        return part->GetDecoded(authority);
    }

    virtual CARAPI GetPath(
        /* [out] */ String* path)
    {
        AutoPtr<IPathPart> pathPart;
        FAIL_RETURN(GetPathPart((IPathPart**)&pathPart));

        return pathPart->GetDecoded(path);
    }

    virtual CARAPI GetEncodedPath(
        /* [out] */ String* path)
    {
        AutoPtr<IPathPart> pathPart;
        FAIL_RETURN(GetPathPart((IPathPart**)&pathPart));

        return pathPart->GetEncoded(path);
    }

    virtual CARAPI GetPathSegments(
        /* [out, callee] */ ArrayOf<String>** pathSegments)
    {
        VALIDATE_NOT_NULL(pathSegments);

        AutoPtr<IPathPart> pathPart;
        FAIL_RETURN(GetPathPart((IPathPart**)&pathPart));

        AutoPtr<IPathSegments> segments;
        FAIL_RETURN(pathPart->GetPathSegments((IPathSegments**)&pathSegments));

        Int32 size;
        FAIL_RETURN(segments->Size(&size));

        ArrayOf<String>* array = ArrayOf<String>::Alloc(size);
        if (!array)
            return E_OUT_OF_MEMORY;

        for (Int32 i = 0; i < size; ++i) {
            String str;
            ECode ec = segments->Get(i, &str);
            if (FAILED(ec)) {
                ArrayOf<String>::Free(array);
                return ec;
            }

            (*array)[i] = str;
        }

        *pathSegments = array;

        return NOERROR;
    }

    virtual CARAPI GetEncodedQuery(
        /* [out] */ String* query)
    {
        AutoPtr<IPart> part;
        FAIL_RETURN(GetQueryPart((IPart**)&part));

        return part->GetEncoded(query);
    }

    virtual CARAPI GetQuery(
        /* [out] */ String* query)
    {
        AutoPtr<IPart> part;
        FAIL_RETURN(GetQueryPart((IPart**)&part));

        return part->GetDecoded(query);
    }

    virtual CARAPI GetEncodedFragment(
        /* [out] */ String* fragment)
    {
        AutoPtr<IPart> part;
        FAIL_RETURN(GetFragmentPart((IPart**)&part));

        return part->GetEncoded(fragment);
    }

    virtual CARAPI GetFragment(
        /* [out] */ String* fragment)
    {
        AutoPtr<IPart> part;
        FAIL_RETURN(GetFragmentPart((IPart**)&part));

        return part->GetDecoded(fragment);
    }

    virtual CARAPI ToString(
        /* [out] */ String* result)
    {
        VALIDATE_NOT_NULL(result);

        *result = mUriString;

        return NOERROR;
    }

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
        /* [out] */ String* result)
    {
        VALIDATE_NOT_NULL(result);

        Int32 length = uriString.GetLength();

        // If "//" follows the scheme separator, we have an authority.
        if (length > ssi + 2
                && uriString.GetChar(ssi + 1) == '/'
                && uriString.GetChar(ssi + 2) == '/') {
            // We have an authority.

            // Look for the start of the path, query, or fragment, or the
            // end of the string.
            Int32 end = ssi + 3;
            L_LOOP:
            while (end < length) {
                switch (uriString.GetChar(end)) {
                    case '/': // Start of path
                    case '?': // Start of query
                    case '#': // Start of fragment
                        goto L_LOOP;
                }
                end++;
            }

            *result = uriString.Substring(ssi + 3, end);
        } else {
            result->SetTo(NULL);
        }

        return NOERROR;
    }

    /**
     * Parses a path out of this given URI string.
     *
     * @param uriString URI string
     * @param ssi scheme separator index, -1 for a relative URI
     *
     * @return the path
     */
    static CARAPI ParsePath(
        /* [in] */ String uriString,
        /* [in] */ Int32 ssi,
        /* [out] */ String* result)
    {
        VALIDATE_NOT_NULL(result);

        Int32 length = uriString.GetLength();

        // Find start of path.
        Int32 pathStart;
        if (length > ssi + 2
                && uriString.GetChar(ssi + 1) == '/'
                && uriString.GetChar(ssi + 2) == '/') {
            // Skip over authority to path.
            pathStart = ssi + 3;
            L_LOOP:
                while (pathStart < length) {
                switch (uriString.GetChar(pathStart)) {
                    case '?': // Start of query
                    case '#': // Start of fragment
                        {
                            result->SetTo(""); // Empty path.
                            return NOERROR;
                        }
                    case '/': // Start of path!
                        goto L_LOOP;
                }
                pathStart++;
            }
        } else {
            // Path starts immediately after scheme separator.
            pathStart = ssi + 1;
        }

        // Find end of path.
        Int32 pathEnd = pathStart;
        L_LOOP_2:
            while (pathEnd < length) {
            switch (uriString.GetChar(pathEnd)) {
                case '?': // Start of query
                case '#': // Start of fragment
                    goto L_LOOP_2;
            }
            pathEnd++;
        }

        *result = uriString.Substring(pathStart, pathEnd);

        return NOERROR;
    }

    CARAPI BuildUpon(
        /* [out] */ IUriBuilder** result);

private:
    /** Finds the first ':'. Returns -1 if none found. */
    CARAPI_(Int32) FindSchemeSeparator()
    {
        return mCachedSsi == Uri::NOT_CALCULATED
                ? mCachedSsi = mUriString.IndexOf(':')
                : mCachedSsi;
    }

    /** Finds the first '#'. Returns -1 if none found. */
    CARAPI_(Int32) FindFragmentSeparator()
    {
        return mCachedFsi == Uri::NOT_CALCULATED
                ? mCachedFsi = mUriString.IndexOf('#', FindSchemeSeparator())
                : mCachedFsi;
    }

    CARAPI_(Void) ParseScheme(
        /* [out] */ String* result)
    {
        Int32 ssi = FindSchemeSeparator();
        *result = ssi == Uri::NOT_FOUND ?
            String(NULL) : mUriString.Substring(0, ssi);
    }

    CARAPI GetSsp(
        /* [out] */ IPart** ssp)
    {
        VALIDATE_NOT_NULL(ssp);

        if (mSsp == NULL) {
            String str;
            ParseSsp(&str);
            FAIL_RETURN(Part::FromEncoded(str, ssp));
            mSsp = *ssp;
        }
        else {
            *ssp = mSsp;
            (*ssp)->AddRef();
        }

        return NOERROR;
    }

    CARAPI_(Void) ParseSsp(
        /* [out] */ String* result)
    {
        Int32 ssi = FindSchemeSeparator();
        Int32 fsi = FindFragmentSeparator();

        // Return everything between ssi and fsi.
        *result = fsi == Uri::NOT_FOUND
                ? mUriString.Substring(ssi + 1)
                : mUriString.Substring(ssi + 1, fsi);
    }

    CARAPI GetAuthorityPart(
        /* [out] */ IPart** part)
    {
        VALIDATE_NOT_NULL(part);

        if (mAuthority == NULL) {
            String encodedAuthority;
            FAIL_RETURN(ParseAuthority(
                mUriString, FindSchemeSeparator(), &encodedAuthority));
            FAIL_RETURN(Part::FromEncoded(encodedAuthority, part));
            mAuthority = *part;
            return NOERROR;
        }

        *part = mAuthority;
        (*part)->AddRef();

        return NOERROR;
    }

    CARAPI GetPathPart(
        /* [out] */ IPathPart** pathPart)
    {
        VALIDATE_NOT_NULL(pathPart);

        if (mPath == NULL) {
            String str;
            ParsePath(&str);
            FAIL_RETURN(PathPart::FromEncoded(str, pathPart));
            mPath = *pathPart;
            return NOERROR;
        }

        *pathPart = mPath;
        (*pathPart)->AddRef();

        return NOERROR;
    }

    CARAPI_(Void) ParsePath(
        /* [out] */ String* result)
    {
        String uriString = mUriString;
        Int32 ssi = FindSchemeSeparator();

        // If the URI is absolute.
        if (ssi > -1) {
            // Is there anything after the ':'?
            Boolean schemeOnly = (UInt32)(ssi + 1) == uriString.GetLength();
            if (schemeOnly) {
                // Opaque URI.
                result->SetTo(NULL);
                return;
            }

            // A '/' after the ':' means this is hierarchical.
            if (uriString.GetChar(ssi + 1) != '/') {
                // Opaque URI.
                result->SetTo(NULL);
                return;
            }
        } else {
            // All relative URIs are hierarchical.
        }

        ParsePath(uriString, ssi, result);
    }

    CARAPI GetQueryPart(
        /* [out] */ IPart** part)
    {
        VALIDATE_NOT_NULL(part);

        if (mQuery == NULL) {
            String str;
            ParseQuery(&str);
            FAIL_RETURN(Part::FromEncoded(str, part));
            mQuery = *part;
            return NOERROR;
        }

        *part = mQuery;
        (*part)->AddRef();

        return NOERROR;
    }

    CARAPI_(Void) ParseQuery(
        /* [out] */ String* result)
    {
        // It doesn't make sense to cache this index. We only ever
        // calculate it once.
        Int32 qsi = mUriString.IndexOf('?', FindSchemeSeparator());
        if (qsi == Uri::NOT_FOUND) {
            result->SetTo(NULL);
            return;
        }

        Int32 fsi = FindFragmentSeparator();

        if (fsi == Uri::NOT_FOUND) {
            *result = mUriString.Substring(qsi + 1);
            return;
        }

        if (fsi < qsi) {
            // Invalid.
            result->SetTo(NULL);
            return;
        }

        *result = mUriString.Substring(qsi + 1, fsi);
    }

    CARAPI GetFragmentPart(
        /* [out] */ IPart** part)
    {
        VALIDATE_NOT_NULL(part);

        if (mFragment == NULL) {
            String str;
            ParseFragment(&str);
            FAIL_RETURN(Part::FromEncoded(str, part));
            mFragment = *part;
            return NOERROR;
        }

        *part = mFragment;
        (*part)->AddRef();

        return NOERROR;
    }

    CARAPI_(Void) ParseFragment(
        /* [out] */ String* result)
    {
        Int32 fsi = FindFragmentSeparator();
        *result = fsi == Uri::NOT_FOUND ?
            String(NULL) : mUriString.Substring(fsi + 1);
    }
};

CarClass(CStringUri), public StringUri
{
public:
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

    CARAPI HashCode(
        /* [out] */ Int32* hashCode);

    CARAPI CompareTo(
        /* [in] */ IUri* other,
        /* [out] */ Int32* result);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI BuildUpon(
        /* [out] */ IUriBuilder** result);

    CARAPI GetDescription(
        /* [out] */ String* uriString);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ const String& uriString);

    CARAPI constructor(
        /* [in] */ Handle32 uri);
};

#endif // __CSTRINGURI_H__
