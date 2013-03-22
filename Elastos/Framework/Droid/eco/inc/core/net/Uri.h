
#ifndef __URI_H__
#define __URI_H__

#ifdef _FRAMEWORK_CORE
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/etl_hash_fun.h>
#else
#include "Elastos.Framework.Core.h"
#endif

using namespace Elastos;

/**
 * Support for part implementations.
 */
class AbstractPart
{
public:

    /**
     * Enum which indicates which representation of a given part we have.
     */
    class Representation
    {
    public:
        static const Int32 BOTH = 0;
        static const Int32 ENCODED = 1;
        static const Int32 DECODED = 2;
    };

public:
    String mEncoded;
    String mDecoded;

public:
    virtual CARAPI Init(
        /* [in] */ const String& encoded,
        /* [in] */ const String& decoded);

    virtual CARAPI GetEncoded(
        /* [out] */ String* encoded) = 0;

    virtual CARAPI GetDecoded(
        /* [out] */ String* decoded);

    virtual CARAPI WriteTo(
        /* [in] */ IParcel* parcel);
};

class Uri
{
#ifdef _FRAMEWORK_CORE
public:
    virtual CARAPI IsHierarchical(
        /* [out] */ Boolean* isHierarchical) = 0;

    virtual CARAPI IsOpaque(
        /* [out] */ Boolean* isOpaque);

    virtual CARAPI IsRelative(
        /* [out] */ Boolean* isRelative) = 0;

    virtual CARAPI IsAbsolute(
        /* [out] */ Boolean* isAbsolute);

    virtual CARAPI GetScheme(
        /* [out] */ String* scheme) = 0;

    virtual CARAPI GetSchemeSpecificPart(
        /* [out] */ String* ssp) = 0;

    virtual CARAPI GetEncodedSchemeSpecificPart(
        /* [out] */ String* essp) = 0;

    virtual CARAPI GetAuthority(
        /* [out] */ String* authority) = 0;

    virtual CARAPI GetEncodedAuthority(
        /* [out] */ String* authority) = 0;

    virtual CARAPI GetUserInfo(
        /* [out] */ String* userInfo) = 0;

    virtual CARAPI GetEncodedUserInfo(
        /* [out] */ String* userInfo) = 0;

    virtual CARAPI GetHost(
        /* [out] */ String* host) = 0;

    virtual CARAPI GetPort(
        /* [out] */ Int32* port) = 0;

    virtual CARAPI GetPath(
        /* [out] */ String* path) = 0;

    virtual CARAPI GetEncodedPath(
        /* [out] */ String* path) = 0;

    virtual CARAPI GetQuery(
        /* [out] */ String* query) = 0;

    virtual CARAPI GetEncodedQuery(
        /* [out] */ String* query) = 0;

    virtual CARAPI GetFragment(
        /* [out] */ String* fragment) = 0;

    virtual CARAPI GetEncodedFragment(
        /* [out] */ String* fragment) = 0;

    virtual CARAPI GetPathSegments(
        /* [out, callee] */ ArrayOf<String>** pathSegments) = 0;

    virtual CARAPI GetLastPathSegment(
        /* [out] */ String* pathSegment) = 0;

    virtual CARAPI ToString(
        /* [out] */ String* result) = 0;

    /**
     * Compares this Uri to another object for equality. Returns true if the
     * encoded string representations of this Uri and the given Uri are
     * equal. Case counts. Paths are not normalized. If one Uri specifies a
     * default port explicitly and the other leaves it implicit, they will not
     * be considered equal.
     */
    virtual CARAPI Equals(
        /* [in] */ IUri* other,
        /* [out] */ Boolean* isEqual);

    /**
     * Hashes the encoded string represention of this Uri consistently with
     * {@link #equals(Object)}.
     */
    virtual CARAPI HashCode(
        /* [out] */ Int32* hashCode);

    /**
     * Compares the string representation of this Uri with that of
     * another.
     */
    virtual CARAPI CompareTo(
        /* [in] */ IUri* other,
        /* [out] */ Int32* result);

    static CARAPI Encode(
        /* [in] */ const String& s,
        /* [out] */ String* encoded);

    static CARAPI Encode(
        /* [in] */ const String& s,
        /* [in] */ const String& allow,
        /* [out] */ String* result);

    static CARAPI_(Boolean) IsAllowed(
        /* [in] */ Char8 c,
        /* [in] */ const String& allow);

    static CARAPI Decode(
        /* [in] */ const String& s,
        /* [out] */ String* result);
#endif

public:
#ifndef _FRAMEWORK_CORE
    static CARAPI Parse(
        /* [in] */ const String& uriString,
        /* [out] */ IUri** uri)
    {
        if (uri == NULL) return E_INVALID_ARGUMENT;

        return CStringUri::New(uriString, uri);
    }

    /**
     * Creates a Uri from a file. The URI has the form
     * "file://<absolute path>". Encodes path characters with the exception of
     * '/'.
     *
     * <p>Example: "file:///tmp/android.txt"
     *
     * @throws NullPointerException if file is null
     * @return a Uri for the given file
     */
    static CARAPI FromFile(
        /* [in] */ IFile* file,
        /* [out] */ IUri** uri)
    {
        return E_NOT_IMPLEMENTED;
    }

    /**
     * Creates an opaque Uri from the given components. Encodes the ssp
     * which means this method cannot be used to create hierarchical URIs.
     *
     * @param scheme of the URI
     * @param ssp scheme-specific-part, everything between the
     *  scheme separator (':') and the fragment separator ('#'), which will
     *  get encoded
     * @param fragment fragment, everything after the '#', null if undefined,
     *  will get encoded
     *
     * @throws NullPointerException if scheme or ssp is null
     * @return Uri composed of the given scheme, ssp, and fragment
     *
     * @see Builder if you don't want the ssp and fragment to be encoded
     */
    static CARAPI FromParts(
        /* [in] */ const String& scheme,
        /* [in] */ const String& ssp,
        /* [in] */ const String& fragment,
        /* [out] */ IUri** uri)
    {
        return E_NOT_IMPLEMENTED;
    }

    static CARAPI WithAppendedPath(
        /* [in] */ IUri* baseUri,
        /* [in] */ const String& pathSegment,
        /* [out] */ IUri** uri)
    {
        return E_NOT_IMPLEMENTED;
    }

#else
    static CARAPI Parse(
        /* [in] */ const String& uriString,
        /* [out] */ IUri** uri);
    /**
     * Creates a Uri from a file. The URI has the form
     * "file://<absolute path>". Encodes path characters with the exception of
     * '/'.
     *
     * <p>Example: "file:///tmp/android.txt"
     *
     * @throws NullPointerException if file is null
     * @return a Uri for the given file
     */
    static CARAPI FromFile(
        /* [in] */ IFile* file,
        /* [out] */ IUri** uri);

    /**
     * Creates an opaque Uri from the given components. Encodes the ssp
     * which means this method cannot be used to create hierarchical URIs.
     *
     * @param scheme of the URI
     * @param ssp scheme-specific-part, everything between the
     *  scheme separator (':') and the fragment separator ('#'), which will
     *  get encoded
     * @param fragment fragment, everything after the '#', null if undefined,
     *  will get encoded
     *
     * @throws NullPointerException if scheme or ssp is null
     * @return Uri composed of the given scheme, ssp, and fragment
     *
     * @see Builder if you don't want the ssp and fragment to be encoded
     */
    static CARAPI FromParts(
        /* [in] */ const String& scheme,
        /* [in] */ const String& ssp,
        /* [in] */ const String& fragment,
        /* [out] */ IUri** uri);

    static CARAPI WithAppendedPath(
        /* [in] */ IUri* baseUri,
        /* [in] */ const String& pathSegment,
        /* [out] */ IUri** uri);

private:
    /** Log tag. */
    static const String LOG;

    /**
     * NOTE: EMPTY accesses this field during its own initialization, so this
     * field *must* be initialized first, or else EMPTY will see a null value!
     *
     * Placeholder for strings which haven't been cached. This enables us
     * to cache null. We intentionally create a new String instance so we can
     * compare its identity and there is no chance we will confuse it with
     * user data.
     */
    static const String NOT_CACHED;

    /** Placeholder value for an index which hasn't been calculated yet. */
    static const Int32 NOT_CALCULATED = -2;

    /** Index of a component which was not found. */
    static const Int32 NOT_FOUND = -1;

    friend class AbstractPart;
    friend class Part;
    friend class PathPart;
    friend class StringUri;
    friend class OpaqueUri;
    friend class HierarchicalUri;
    friend class AbstractHierarchicalUri;
#endif
};

#ifdef _FRAMEWORK_CORE

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<IUri*>
{
    size_t operator()(IUri* uri) const
    {
        Int32 hashCode;
        assert(uri != NULL);
        uri->GetHashCode(&hashCode);
        return (size_t)hashCode;
    }
};

_ELASTOS_NAMESPACE_END

/**
 * Support for hierarchical URIs.
 */
class AbstractHierarchicalUri : public Uri
{
public:
    AbstractHierarchicalUri();

    virtual CARAPI GetLastPathSegment(
        /* [out] */ String* pathSegment)
    {
        VALIDATE_NOT_NULL(pathSegment);

        // TODO: If we haven't parsed all of the segments already, just
        // grab the last one directly so we only allocate one string.

        ArrayOf<String>* segments;
        FAIL_RETURN(GetPathSegments(&segments));

        Int32 size = segments->GetLength();
        if (size == 0) {
            pathSegment->SetTo(NULL);
            return NOERROR;
        }

        *pathSegment = (*segments)[size - 1];
        return NOERROR;
    }

    virtual CARAPI GetEncodedUserInfo(
        /* [out] */ String* userInfo)
    {
        AutoPtr<IPart> part;
        FAIL_RETURN(GetUserInfoPart((IPart**)&part));

        return part->GetEncoded(userInfo);
    }

    virtual CARAPI GetUserInfo(
        /* [out] */ String* userInfo)
    {
        AutoPtr<IPart> part;
        FAIL_RETURN(GetUserInfoPart((IPart**)&part));

        return part->GetDecoded(userInfo);
    }

    virtual CARAPI GetHost(
        /* [out] */ String* host);

    virtual CARAPI GetPort(
        /* [out] */ Int32* port);

private:
    CARAPI GetUserInfoPart(
        /* [out] */ IPart** part);

    CARAPI ParseUserInfo(
        /* [out] */ String* result);

    CARAPI ParseHost(
        /* [out] */ String* result);

    CARAPI ParsePort(
        /* [out] */ Int32* result);

private:
    AutoPtr<IPart> mUserInfo;
    String mHost;
    Int32 mPort;
};

#endif

#endif //__URI_H__
