
#include "net/CHierarchicalUri.h"
#include "net/COpaqueUri.h"
#include "net/CUriBuilder.h"

Builder* Builder::AppendQueryParameter(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    // This URI will be hierarchical.
    mOpaquePart = NULL;

    String str1;
    FAIL_RETURN_NULL(Uri::Encode(key, String(NULL), &str1));

    String str2;
    FAIL_RETURN_NULL(Uri::Encode(value, String(NULL), &str2));

    String encodedParameter = str1 + "=" + str2;

    if (mQuery == NULL) {
        FAIL_RETURN_NULL(Part::FromEncoded(
            encodedParameter, (IPart**)&mQuery));
        return this;
    }

    String oldQuery;
    FAIL_RETURN_NULL(mQuery->GetEncoded(&oldQuery));

    if (oldQuery.IsNullOrEmpty()) {
        FAIL_RETURN_NULL(Part::FromEncoded(
            encodedParameter, (IPart**)&mQuery));
    } else {
        FAIL_RETURN_NULL(Part::FromEncoded(
            oldQuery + "&" + encodedParameter, (IPart**)&mQuery));
    }

    return this;
}

ECode Builder::Build(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    if (mOpaquePart != NULL) {
        if (mScheme.IsNull()) {
            // throw new UnsupportedOperationException(
            //        "An opaque URI must have a scheme.");
            return E_UNSUPPORTED_OPERATION_EXCEPTION;
        }

        return COpaqueUri::New(mScheme, mOpaquePart, mFragment, uri);
    } else {
        // Hierarchical URIs should not return null for getPath().
        AutoPtr<IPathPart> path = mPath;
        if (path == NULL || path == PathPart::PathPartNULL) {
            path = PathPart::EMPTY;
        } else {
            // If we have a scheme and/or authority, the path must
            // be absolute. Prepend it with a '/' if necessary.
            if (HasSchemeOrAuthority()) {
                FAIL_RETURN(PathPart::MakeAbsolute(
                    path, (IPathPart**)&path));
            }
        }

        return CHierarchicalUri::New(
            mScheme, mAuthority, path, mQuery, mFragment, uri);
    }
}

ECode CUriBuilder::Scheme(
    /* [in] */ const String& scheme)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUriBuilder::OpaquePart(
    /* [in] */ IPart * pOpaquePart)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUriBuilder::OpaquePart2(
    /* [in] */ const String& opaquePart)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUriBuilder::EncodedOpaquePart(
    /* [in] */ const String& opaquePart)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUriBuilder::Authority(
    /* [in] */ IPart * pAuthority)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUriBuilder::Authority2(
    /* [in] */ const String& authority)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUriBuilder::EncodedAuthority(
    /* [in] */ const String& authority)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUriBuilder::Path(
    /* [in] */ IPathPart * pPath)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUriBuilder::Path2(
    /* [in] */ const String& path)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUriBuilder::EncodedPath(
    /* [in] */ const String& path)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUriBuilder::AppendPath(
    /* [in] */ const String& newSegment)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUriBuilder::AppendEncodedPath(
    /* [in] */ const String& newSegment)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUriBuilder::Query(
    /* [in] */ IPart * pQuery)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUriBuilder::Query2(
    /* [in] */ const String& query)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUriBuilder::EncodedQuery(
    /* [in] */ const String& query)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUriBuilder::Fragment(
    /* [in] */ IPart * pFragment)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUriBuilder::Fragment2(
    /* [in] */ const String& fragment)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUriBuilder::EncodedFragment(
    /* [in] */ const String& fragment)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUriBuilder::AppendQueryParameter(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CUriBuilder::Build(
    /* [out] */ IUri ** ppUri)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

