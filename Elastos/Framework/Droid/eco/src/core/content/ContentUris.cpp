
#include "content/ContentUris.h"


/**
 * Converts the last path segment to a Int64.
 *
 * <p>This supports a common convention for content URIs where an ID is
 * stored in the last segment.
 *
 * @throws UnsupportedOperationException if this isn't a hierarchical URI
 * @throws NumberFormatException if the last segment isn't a number
 *
 * @return the Int64 conversion of the last segment or -1 if the path is
 *  empty
 */
Int64 ContentUris::ParseId(
    /* [in] */ IUri* contentUri) 
{
    String last;
    contentUri->GetLastPathSegment(&last);
    return last.IsNull() ? -1 : last.ToInt64();
}

/**
 * Appends the given ID to the end of the path.
 *
 * @param builder to append the ID to
 * @param id to append
 *
 * @return the given builder
 */
AutoPtr<IUriBuilder> ContentUris::AppendId(
    /* [in] */ IUriBuilder* builder, 
    /* [in] */ Int64 id) 
{
    builder->AppendEncodedPath(String::FromInt64(id));

    return builder;
}

/**
 * Appends the given ID to the end of the path.
 *
 * @param contentUri to start with
 * @param id to append
 *
 * @return a new URI with the given ID appended to the end of the path
 */
AutoPtr<IUri> ContentUris::WithAppendedId(
    /* [in] */ IUri* contentUri, 
    /* [in] */ Int64 id)
{
    AutoPtr<IUri> uri;
    
    AutoPtr<IUriBuilder> ub;
    contentUri->BuildUpon((IUriBuilder**)&ub);
    AppendId(ub, id)->Build((IUri**)&uri);

    return uri;
}

