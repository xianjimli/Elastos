
#ifndef __CURIBUILDER_H__
#define __CURIBUILDER_H__

#include "_CUriBuilder.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include "net/Uri.h"
#include "net/CPart.h"
#include "net/CPathPart.h"

/**
 * Helper class for building or manipulating URI references. Not safe for
 * concurrent use.
 *
 * <p>An absolute hierarchical URI reference follows the pattern:
 * {@code &lt;scheme&gt;://&lt;authority&gt;&lt;absolute path&gt;?&lt;query&gt;#&lt;fragment&gt;}
 *
 * <p>Relative URI references (which are always hierarchical) follow one
 * of two patterns: {@code &lt;relative or absolute path&gt;?&lt;query&gt;#&lt;fragment&gt;}
 * or {@code //&lt;authority&gt;&lt;absolute path&gt;?&lt;query&gt;#&lt;fragment&gt;}
 *
 * <p>An opaque URI follows this pattern:
 * {@code &lt;scheme&gt;:&lt;opaque part&gt;#&lt;fragment&gt;}
 */
class Builder
{
public:
    String mScheme;
    AutoPtr<IPart> mOpaquePart;
    AutoPtr<IPart> mAuthority;
    AutoPtr<IPathPart> mPath;
    AutoPtr<IPart> mQuery;
    AutoPtr<IPart> mFragment;

public:

    /**
     * Constructs a new Builder.
     */
    Builder() {}

    /**
     * Sets the scheme.
     *
     * @param scheme name or {@code null} if this is a relative Uri
     */
    Builder* Scheme(
        /* [in] */ const String& scheme)
    {
        mScheme = scheme;
        return this;
    }

    Builder* OpaquePart(
        /* [in] */ IPart* opaquePart)
    {
        mOpaquePart = opaquePart;
        return this;
    }

    /**
     * Encodes and sets the given opaque scheme-specific-part.
     *
     * @param opaquePart decoded opaque part
     */
    Builder* OpaquePart(
        /* [in] */ const String& opaquePart)
    {
        AutoPtr<IPart> part;
        FAIL_RETURN_NULL(Part::FromDecoded(
            opaquePart, (IPart**)&part));

        return OpaquePart(part);
    }

    /**
     * Sets the previously encoded opaque scheme-specific-part.
     *
     * @param opaquePart encoded opaque part
     */
    Builder* EncodedOpaquePart(
        /* [in] */ const String& opaquePart)
    {
        AutoPtr<IPart> part;
        FAIL_RETURN_NULL(Part::FromEncoded(
            opaquePart, (IPart**)&part));

        return OpaquePart(part);
    }

    Builder* Authority(
        /* [in] */ IPart* authority)
    {
        // This URI will be hierarchical.
        mOpaquePart = NULL;

        mAuthority = authority;
        return this;
    }

    /**
     * Encodes and sets the authority.
     */
    Builder* Authority(
        /* [in] */ const String& authority)
    {
        AutoPtr<IPart> part;
        FAIL_RETURN_NULL(Part::FromDecoded(
            authority, (IPart**)&part));

        return Authority(part);
    }

    /**
     * Sets the previously encoded authority.
     */
    Builder* EncodedAuthority(
        /* [in] */ const String& authority)
    {
        AutoPtr<IPart> part;
        FAIL_RETURN_NULL(Part::FromEncoded(
            authority, (IPart**)&part));

        return Authority(part);
    }

    Builder* Path(
        /* [in] */ IPathPart* path)
    {
        // This URI will be hierarchical.
        mOpaquePart = NULL;

        mPath = path;
        return this;
    }

    /**
     * Sets the path. Leaves '/' characters intact but encodes others as
     * necessary.
     *
     * <p>If the path is not null and doesn't start with a '/', and if
     * you specify a scheme and/or authority, the builder will prepend the
     * given path with a '/'.
     */
    Builder* Path(
        /* [in] */ const String& path)
    {
        AutoPtr<IPathPart> pathPart;
        FAIL_RETURN_NULL(PathPart::FromDecoded(
            path, (IPathPart**)&pathPart));

        return Path(pathPart);
    }

    /**
     * Sets the previously encoded path.
     *
     * <p>If the path is not null and doesn't start with a '/', and if
     * you specify a scheme and/or authority, the builder will prepend the
     * given path with a '/'.
     */
    Builder* EncodedPath(
        /* [in] */ const String& path)
    {
        AutoPtr<IPathPart> pathPart;
        FAIL_RETURN_NULL(PathPart::FromEncoded(
            path, (IPathPart**)&pathPart));

        return Path(pathPart);
    }

    /**
     * Encodes the given segment and appends it to the path.
     */
    Builder* AppendPath(
        /* [in] */ const String& newSegment)
    {
        AutoPtr<IPathPart> pathPart;
        FAIL_RETURN_NULL(PathPart::AppendDecodedSegment(
            mPath, newSegment, (IPathPart**)&pathPart));

        return Path(pathPart);
    }

    /**
     * Appends the given segment to the path.
     */
    Builder* AppendEncodedPath(
        /* [in] */ const String& newSegment)
    {
        AutoPtr<IPathPart> pathPart;
        FAIL_RETURN_NULL(PathPart::AppendEncodedSegment(
            mPath, newSegment, (IPathPart**)&pathPart));

        return Path(pathPart);
    }

    Builder* Query(
        /* [in] */ IPart* query)
    {
        // This URI will be hierarchical.
        mOpaquePart = NULL;

        mQuery = query;
        return this;
    }

    /**
     * Encodes and sets the query.
     */
    Builder* Query(
        /* [in] */ const String& query)
    {
        AutoPtr<IPart> part;
        FAIL_RETURN_NULL(Part::FromDecoded(
            query, (IPart**)&part));

        return Query(part);
    }

    /**
     * Sets the previously encoded query.
     */
    Builder* EncodedQuery(
        /* [in] */ const String& query)
    {
        AutoPtr<IPart> part;
        FAIL_RETURN_NULL(Part::FromEncoded(
            query, (IPart**)&part));

        return Query(part);
    }

    Builder* Fragment(
        /* [in] */ IPart* fragment)
    {
        mFragment = fragment;
        return this;
    }

    /**
     * Encodes and sets the fragment.
     */
    Builder* Fragment(
        /* [in] */ const String& fragment)
    {
        AutoPtr<IPart> part;
        FAIL_RETURN_NULL(Part::FromDecoded(
            fragment, (IPart**)&part));

        return Fragment(part);
    }

    /**
     * Sets the previously encoded fragment.
     */
    Builder* EncodedFragment(
        /* [in] */ const String& fragment)
    {
        AutoPtr<IPart> part;
        FAIL_RETURN_NULL(Part::FromEncoded(
            fragment, (IPart**)&part));

        return Fragment(part);
    }

    /**
     * Encodes the key and value and then appends the parameter to the
     * query string.
     *
     * @param key which will be encoded
     * @param value which will be encoded
     */
    Builder* AppendQueryParameter(
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    /**
     * Constructs a Uri with the current attributes.
     *
     * @throws UnsupportedOperationException if the URI is opaque and the
     *  scheme is null
     */
    CARAPI Build(
        /* [out] */ IUri** uri);

    Boolean HasSchemeOrAuthority()
    {
        return (mScheme != NULL)
            || (mAuthority != NULL && mAuthority != Part::PartNULL);

    }

    // @Override
    CARAPI ToString(
        /* [out] */ String* result)
    {
        // AutoPtr<IUri> uri;
        // FAIL_RETURN(Build((IUri**)&uri));

        // TODO: ALEX need ToString
        // return uri->ToString(result);
        return E_NOT_IMPLEMENTED;
    }
};

CarClass(CUriBuilder), public Builder
{
public:
    CARAPI Scheme(
        /* [in] */ const String& scheme);

    CARAPI OpaquePart(
        /* [in] */ IPart * pOpaquePart);

    CARAPI OpaquePart2(
        /* [in] */ const String& opaquePart);

    CARAPI EncodedOpaquePart(
        /* [in] */ const String& opaquePart);

    CARAPI Authority(
        /* [in] */ IPart * pAuthority);

    CARAPI Authority2(
        /* [in] */ const String& authority);

    CARAPI EncodedAuthority(
        /* [in] */ const String& authority);

    CARAPI Path(
        /* [in] */ IPathPart * pPath);

    CARAPI Path2(
        /* [in] */ const String& path);

    CARAPI EncodedPath(
        /* [in] */ const String& path);

    CARAPI AppendPath(
        /* [in] */ const String& newSegment);

    CARAPI AppendEncodedPath(
        /* [in] */ const String& newSegment);

    CARAPI Query(
        /* [in] */ IPart * pQuery);

    CARAPI Query2(
        /* [in] */ const String& query);

    CARAPI EncodedQuery(
        /* [in] */ const String& query);

    CARAPI Fragment(
        /* [in] */ IPart * pFragment);

    CARAPI Fragment2(
        /* [in] */ const String& fragment);

    CARAPI EncodedFragment(
        /* [in] */ const String& fragment);

    CARAPI AppendQueryParameter(
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    CARAPI Build(
        /* [out] */ IUri ** ppUri);

private:
    // TODO: Add your private member variables here.
};

#endif // __CURIBUILDER_H__
