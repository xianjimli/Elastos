
#ifndef __CONTENTURIS_H__
#define __CONTENTURIS_H__

#include "ext/frameworkext.h"
#include "elastos/AutoPtr.h"

/**
 * Utility methods useful for working with content {@link android.net.Uri}s,
 * those with a "content" scheme.
 */
class ContentUris 
{
public:
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
    static CARAPI_(Int64) ParseId(
        /* [in] */ IUri* contentUri);

    /**
     * Appends the given ID to the end of the path.
     *
     * @param builder to append the ID to
     * @param id to append
     *
     * @return the given builder
     */
    static CARAPI_(AutoPtr<IUriBuilder>) AppendId(
        /* [in] */ IUriBuilder* builder, 
        /* [in] */ Int64 id);

    /**
     * Appends the given ID to the end of the path.
     *
     * @param contentUri to start with
     * @param id to append
     *
     * @return a new URI with the given ID appended to the end of the path
     */
    static CARAPI_(AutoPtr<IUri>) WithAppendedId(
        /* [in] */ IUri* contentUri, 
        /* [in] */ Int64 id);
};

#endif