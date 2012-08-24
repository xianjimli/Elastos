
#include "net/Uri.h"
#include "net/CStringUri.h"

const String Uri::NOT_CACHED = String("NOT_CACHED");

Uri::AbstractPart::AbstractPart(
    /* [in] */ const String& encoded,
    /* [in] */ const String& decoded)
{
    mEncoded = encoded;
    mDecoded = decoded;
}

ECode Uri::AbstractPart::GetDecoded(
    /* [out] */ String* decoded)
{
    if (decoded == NULL) return E_INVALID_ARGUMENT;

    if (!mDecoded.Compare(NOT_CACHED)) {
        return Decode(mEncoded, decoded);
    } else {
        *decoded = mDecoded;
        return NOERROR;
    }
}

Uri::Part::Part(
    /* [in] */ const String& encoded,
    /* [in] */ const String& decoded) :
    AbstractPart(encoded, decoded)
{}

Uri::Part* Uri::Part::FromEncoded(
    /* [in] */ const String& encoded)
{
    return From(encoded, NOT_CACHED);
}

Uri::Part* Uri::Part::From(
    /* [in] */ const String& encoded,
    /* [in] */ const String& decoded)
{
    // We have to check both encoded and decoded in case one is
    // NOT_CACHED.
    if (encoded.IsNull() || encoded.GetLength() == 0
            || decoded.IsNull() || decoded.GetLength() == 0) {
        return NULL;
    }

    return new Part(encoded, decoded);
}

ECode Uri::Parse(
    /* [in] */ const String& uriString,
    /* [out] */ IUri** uri)
{
    if (uri == NULL) return E_INVALID_ARGUMENT;

    return CStringUri::New(uriString, uri);
}

ECode Uri::Decode(
    /* [in] */ const String& str,
    /* [out] */ String* decoded)
{
    if (decoded == NULL ||
            str.IsNull()) {
        return E_INVALID_ARGUMENT;
    }

    *decoded = str;
    return NOERROR;

//    // Lazily-initialized buffers.
//    StringBuilder decoded = null;
//    ByteArrayOutputStream out = null;
//
//    int oldLength = s.length();
//
//    // This loop alternates between copying over normal characters and
//    // escaping in chunks. This results in fewer method calls and
//    // allocations than decoding one character at a time.
//    int current = 0;
//    while (current < oldLength) {
//        // Start in "copying" mode where we copy over normal characters.
//
//        // Find the next escape sequence.
//        int nextEscape = s.indexOf('%', current);
//
//        if (nextEscape == NOT_FOUND) {
//            if (decoded == null) {
//                // We didn't actually decode anything.
//                return s;
//            } else {
//                // Append the remainder and return the decoded string.
//                decoded.append(s, current, oldLength);
//                return decoded.toString();
//            }
//        }
//
//        // Prepare buffers.
//        if (decoded == null) {
//            // Looks like we're going to need the buffers...
//            // We know the new string will be shorter. Using the old length
//            // may overshoot a bit, but it will save us from resizing the
//            // buffer.
//            decoded = new StringBuilder(oldLength);
//            out = new ByteArrayOutputStream(4);
//        } else {
//            // Clear decoding buffer.
//            out.reset();
//        }
//
//        // Append characters leading up to the escape.
//        if (nextEscape > current) {
//            decoded.append(s, current, nextEscape);
//
//            current = nextEscape;
//        } else {
//            // assert current == nextEscape
//        }
//
//        // Switch to "decoding" mode where we decode a string of escape
//        // sequences.
//
//        // Decode and append escape sequences. Escape sequences look like
//        // "%ab" where % is literal and a and b are hex digits.
//        try {
//            do {
//                if (current + 2 >= oldLength) {
//                    // Truncated escape sequence.
//                    out.write(REPLACEMENT);
//                } else {
//                    int a = Character.digit(s.charAt(current + 1), 16);
//                    int b = Character.digit(s.charAt(current + 2), 16);
//
//                    if (a == -1 || b == -1) {
//                        // Non hex digits.
//                        out.write(REPLACEMENT);
//                    } else {
//                        // Combine the hex digits into one byte and write.
//                        out.write((a << 4) + b);
//                    }
//                }
//
//                // Move passed the escape sequence.
//                current += 3;
//            } while (current < oldLength && s.charAt(current) == '%');
//
//            // Decode UTF-8 bytes into a string and append it.
//            decoded.append(out.toString(DEFAULT_ENCODING));
//        } catch (UnsupportedEncodingException e) {
//            throw new AssertionError(e);
//        } catch (IOException e) {
//            throw new AssertionError(e);
//        }
//    }
//
//    // If we don't have a buffer, we didn't have to decode anything.
//    return decoded == null ? s : decoded.toString();
}

AutoPtr<IUri> Uri::FromFile(
    /* [in] */ IFile* file)
{
    return NULL;
}

ECode Uri::FromParts(
    /* [in] */ const String& scheme,
    /* [in] */ const String& ssp,
    /* [in] */ const String& fragment,
    /* [out] */ IUri** uri)
{
    return E_NOT_IMPLEMENTED;
}

