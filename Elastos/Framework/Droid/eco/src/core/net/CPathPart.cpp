
#include "net/CPathSegments.h"
#include "net/CPathPart.h"
#include "net/CPathSegmentsBuilder.h"

class CStatic
{
public:

    CStatic()
    {
        CPathPart::New(
            String(NULL), String(NULL), (IPathPart**)&PathPart::PathPartNULL);
        CPathPart::New(String(""), String(""), (IPathPart**)&PathPart::EMPTY);
    }
};

static CStatic _static;

/** A part with null values. */
AutoPtr<IPathPart> PathPart::PathPartNULL;

/** A part with empty strings for values. */
AutoPtr<IPathPart> PathPart::EMPTY;

ECode PathPart::GetEncoded(
    /* [out] */ String* encoded)
{
    VALIDATE_NOT_NULL(encoded);

    // @SuppressWarnings("StringEquality")
    Boolean hasEncoded = !mEncoded.Equals(Uri::NOT_CACHED);

    // Don't encode '/'.
    if (!hasEncoded) FAIL_RETURN(Uri::Encode(mDecoded, String("/"), &mEncoded));
    *encoded = mEncoded;

    return NOERROR;
}

ECode PathPart::GetPathSegments(
    /* [out] */ IPathSegments** pathSegments)
{
    VALIDATE_NOT_NULL(pathSegments);

    if (mPathSegments != NULL) {
        *pathSegments = mPathSegments;
        mPathSegments->AddRef();
        return NOERROR;
    }

    String path;
    if (FAILED(GetEncoded(&path)) || path.IsNull()) {
        mPathSegments = PathSegments::EMPTY;
        *pathSegments = mPathSegments;
        mPathSegments->AddRef();
        return NOERROR;
    }

    AutoPtr<IPathSegmentsBuilder> segmentBuilder;
    FAIL_RETURN(CPathSegmentsBuilder::New(
        (IPathSegmentsBuilder**)&segmentBuilder));

    Int32 previous = 0;
    Int32 current;
    while ((current = path.IndexOf('/', previous)) > -1) {
        // This check keeps us from adding a segment if the path starts
        // '/' and an empty segment for "//".
        if (previous < current) {
            String decodedSegment;
            if (FAILED(Uri::Decode(
                path.Substring(previous, current), &decodedSegment))) {
                *pathSegments = NULL;
                return NOERROR;
            }
            segmentBuilder->Add(decodedSegment);
        }
        previous = current + 1;
    }

    // Add in the final path segment.
    if (previous < (Int32)path.GetLength()) {
        String decoded;
        if (FAILED(Uri::Decode(path.Substring(previous), &decoded))) {
            *pathSegments = NULL;
            return NOERROR;
        }
        segmentBuilder->Add(decoded);
    }

    FAIL_RETURN(segmentBuilder->Build(pathSegments));
    mPathSegments = *pathSegments;

    return NOERROR;
}

ECode PathPart::AppendEncodedSegment(
    /* [in] */ IPathPart* oldPart,
    /* [in] */ const String& newSegment,
    /* [out] */ IPathPart** result)
{
    VALIDATE_NOT_NULL(result);

    // If there is no old path, should we make the new path relative
    // or absolute? I pick absolute.

    if (oldPart == NULL) {
        // No old path.
        String temp = String("/") + newSegment;
        return FromEncoded(temp, result);
    }

    String oldPath;
    oldPart->GetEncoded(&oldPath);

    if (FAILED(oldPart->GetEncoded(&oldPath)) || oldPath.IsNullOrEmpty()) {
        oldPath = "";
    }

    UInt32 oldPathLength = oldPath.GetLength();
    String newPath;
    if (oldPathLength == 0) {
        // No old path.
        newPath = String("/") + newSegment;
    } else if (oldPath[(oldPathLength - 1)] == '/') {
        newPath = oldPath + newSegment;
    } else {
        newPath = oldPath + String("/") + newSegment;
    }

    return FromEncoded(newPath, result);
}

ECode PathPart::AppendDecodedSegment(
    /* [in] */ IPathPart* oldPart,
    /* [in] */ const String& decoded,
    /* [out] */ IPathPart** result)
{
    VALIDATE_NOT_NULL(result);

    String encoded;
    FAIL_RETURN(Uri::Encode(decoded, &encoded));

    // TODO: Should we reuse old PathSegments? Probably not.
    return AppendEncodedSegment(oldPart, encoded, result);
}

ECode PathPart::ReadFrom(
    /* [in] */ IParcel* parcel,
    /* [out] */ IPathPart** result)
{
    VALIDATE_NOT_NULL(result);

    Int32 representation;
    FAIL_RETURN(parcel->ReadInt32(&representation));

    switch (representation) {
        case AbstractPart::Representation::BOTH: {
            String str1;
            FAIL_RETURN(parcel->ReadString(&str1));

            String str2;
            FAIL_RETURN(parcel->ReadString(&str2));

            return From(str1, str2, result);
        }
        case AbstractPart::Representation::ENCODED: {
            String str1;
            FAIL_RETURN(parcel->ReadString(&str1));

            return FromEncoded(str1, result);
        }
        case AbstractPart::Representation::DECODED: {
            String str1;
            FAIL_RETURN(parcel->ReadString(&str1));

            return FromDecoded(str1, result);
        }
        default: {
            // throw new AssertionError();
            return E_ASSERTION_ERROR;
        }
    }
}

ECode PathPart::FromEncoded(
    /* [in] */ const String& encoded,
    /* [out] */ IPathPart** result)
{
    return From(encoded, Uri::NOT_CACHED, result);
}

ECode PathPart::FromDecoded(
    /* [in] */ const String& decoded,
    /* [out] */ IPathPart** result)
{
    return From(Uri::NOT_CACHED, decoded, result);
}

ECode PathPart::From(
    /* [in] */ const String& encoded,
    /* [in] */ const String& decoded,
    /* [out] */ IPathPart** result)
{
    VALIDATE_NOT_NULL(result);

    if (encoded.IsNull()) {
        *result = PathPart::PathPartNULL;
        (*result)->AddRef();
        return NOERROR;
    }

    if (encoded.IsEmpty()) {
        *result = PathPart::EMPTY;
        (*result)->AddRef();
        return NOERROR;
    }

    return CPathPart::New(encoded, decoded, result);
}

ECode PathPart::MakeAbsolute(
    /* [in] */ IPathPart* oldPart,
    /* [out] */ IPathPart** result)
{
    VALIDATE_NOT_NULL(result);

    // @SuppressWarnings("StringEquality")
    Boolean encodedCached = !((PathPart*)oldPart)->mEncoded.Equals(
        Uri::NOT_CACHED);

    // We don't care which version we use, and we don't want to force
    // unneccessary encoding/decoding.
    String oldPath = encodedCached ?
        ((PathPart*)oldPart)->mEncoded : ((PathPart*)oldPart)->mDecoded;

    if (oldPath.IsNull() || oldPath.IsEmpty()
            || (oldPath[0] == '/')) {
        *result = oldPart;
        if (oldPart) oldPart->AddRef();
        return NOERROR;
    }

    // Prepend encoded string if present.
    String newEncoded = encodedCached
            ? String("/") + ((PathPart*)oldPart)->mEncoded : Uri::NOT_CACHED;

    // Prepend decoded string if present.
    // @SuppressWarnings("StringEquality")
    Boolean decodedCached = !((PathPart*)oldPart)->mDecoded.Equals(
        Uri::NOT_CACHED);
    String newDecoded = decodedCached
            ? String("/") + ((PathPart*)oldPart)->mDecoded
            : Uri::NOT_CACHED;

    return CPathPart::New(newEncoded, newDecoded, result);
}

ECode CPathPart::GetEncoded(
    /* [out] */ String* encoded)
{
    return PathPart::GetEncoded(encoded);
}

ECode CPathPart::GetDecoded(
    /* [out] */ String* decoded)
{
    return PathPart::GetDecoded(decoded);
}

ECode CPathPart::GetPathSegments(
    /* [out] */ IPathSegments** pathSegments)
{
    return PathPart::GetPathSegments(pathSegments);
}

ECode CPathPart::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPathPart::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return AbstractPart::WriteTo(dest);
}

ECode CPathPart::constructor(
    /* [in] */ const String& encoded,
    /* [in] */ const String& decoded)
{
    return PathPart::Init(encoded, decoded);
}

