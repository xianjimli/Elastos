
#ifndef __CPATHPART_H__
#define __CPATHPART_H__

#include "_CPathPart.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include "net/Uri.h"

/**
 * Immutable wrapper of encoded and decoded versions of a path part. Lazily
 * creates the encoded or decoded version from the other.
 */
class PathPart : public AbstractPart
{
public:
    /** A part with null values. */
    static AutoPtr<IPathPart> PathPartNULL;

    /** A part with empty strings for values. */
    static AutoPtr<IPathPart> EMPTY;

    virtual CARAPI Init(
        /* [in] */ const String& encoded,
        /* [in] */ const String& decoded)
    {
        return AbstractPart::Init(encoded, decoded);
    }

    virtual CARAPI GetEncoded(
        /* [out] */ String* encoded);

    /**
     * Gets the individual path segments. Parses them if necessary.
     *
     * @return parsed path segments or null if this isn't a hierarchical
     *  URI
     */
    virtual CARAPI GetPathSegments(
        /* [out] */ IPathSegments** pathSegments);

    static CARAPI AppendEncodedSegment(
        /* [in] */ IPathPart* oldPart,
        /* [in] */ const String& newSegment,
        /* [out] */ IPathPart** result);

    static CARAPI AppendDecodedSegment(
        /* [in] */ IPathPart* oldPart,
        /* [in] */ const String& decoded,
        /* [out] */ IPathPart** result);

    static CARAPI ReadFrom(
        /* [in] */ IParcel* parcel,
        /* [out] */ IPathPart** result);

    /**
     * Creates a path from the encoded string.
     *
     * @param encoded part string
     */
    static CARAPI FromEncoded(
        /* [in] */ const String& encoded,
        /* [out] */ IPathPart** result);

    /**
     * Creates a path from the decoded string.
     *
     * @param decoded part string
     */
    static CARAPI FromDecoded(
        /* [in] */ const String& decoded,
        /* [out] */ IPathPart** result);

    /**
     * Creates a path from the encoded and decoded strings.
     *
     * @param encoded part string
     * @param decoded part string
     */
    static CARAPI From(
        /* [in] */ const String& encoded,
        /* [in] */ const String& decoded,
        /* [out] */ IPathPart** result);

    /**
     * Prepends path values with "/" if they're present, not empty, and
     * they don't already start with "/".
     */
    static CARAPI MakeAbsolute(
        /* [in] */ IPathPart* oldPart,
        /* [out] */ IPathPart** result);

private:
    /**
     * Cached path segments. This doesn't need to be volatile--we don't
     * care if other threads see the result.
     */
    AutoPtr<IPathSegments> mPathSegments;
};

CarClass(CPathPart), public PathPart
{
public:
    CARAPI GetEncoded(
        /* [out] */ String* encoded);

    CARAPI GetDecoded(
        /* [out] */ String* decoded);

    CARAPI GetPathSegments(
        /* [out] */ IPathSegments** pathSegments);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ const String& encoded,
        /* [in] */ const String& decoded);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPATHPART_H__
