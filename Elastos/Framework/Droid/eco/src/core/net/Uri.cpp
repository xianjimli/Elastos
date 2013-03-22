
#include "net/Uri.h"
#include "net/CPart.h"
#include "net/CEmptyPart.h"
#include "net/CPathPart.h"
#include "net/CStringUri.h"
#include "net/CHierarchicalUri.h"
#include "net/COpaqueUri.h"
#include "net/CUriBuilder.h"
#include <elastos/Character.h>

const String LOG = String("Uri"); // Uri.class.getSimpleName();
const String Uri::NOT_CACHED = String("NOT_CACHED");

/**
 * Error message presented when a user tries to treat an opaque URI as
 * hierarchical.
 */
static String NOT_HIERARCHICAL = String("This isn't a hierarchical URI.");

/** Default encoding. */
static String DEFAULT_ENCODING = String("UTF-8");

static const Char8 HEX_DIGITS[] = "0123456789ABCDEF";

ECode Uri::Equals(
    /* [in] */ IUri* other,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);

    // if (!(o instanceof Uri)) {
    //     return false;
    // }

    // Uri other = (Uri) o;
    String str1;
    FAIL_RETURN(ToString(&str1));

    String str2;
    FAIL_RETURN(other->ToString(&str2));

    *isEqual = str1.Equals(str2);

    return NOERROR;
}

ECode Uri::HashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);

    String str1;
    FAIL_RETURN(ToString(&str1));

    *hashCode = str1.GetHashCode();

    return NOERROR;
}

ECode Uri::CompareTo(
    /* [in] */ IUri* other,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    String str1;
    FAIL_RETURN(ToString(&str1));

    String str2;
    FAIL_RETURN(other->ToString(&str2));

    *result = str1.Compare(str2);

    return NOERROR;
}

/**
 * Encodes characters in the given string as '%'-escaped octets
 * using the UTF-8 scheme. Leaves letters ("A-Z", "a-z"), numbers
 * ("0-9"), and unreserved characters ("_-!.~'()*") intact. Encodes
 * all other characters.
 *
 * @param s string to encode
 * @return an encoded version of s suitable for use as a URI component,
 *  or null if s is null
 */
ECode Uri::Encode(
    /* [in] */ const String& s,
    /* [out] */ String* encoded)
{
    return Encode(s, String(NULL), encoded);
}

/**
 * Encodes characters in the given string as '%'-escaped octets
 * using the UTF-8 scheme. Leaves letters ("A-Z", "a-z"), numbers
 * ("0-9"), and unreserved characters ("_-!.~'()*") intact. Encodes
 * all other characters with the exception of those specified in the
 * allow argument.
 *
 * @param s string to encode
 * @param allow set of additional characters to allow in the encoded form,
 *  null if no characters should be skipped
 * @return an encoded version of s suitable for use as a URI component,
 *  or null if s is null
 */
ECode Uri::Encode(
    /* [in] */ const String& s,
    /* [in] */ const String& allow,
    /* [out] */ String* result)
{
    if (!result) return E_INVALID_ARGUMENT;

    if (s.IsNull()) {
        result->SetTo(NULL);
        return NOERROR;
    }

    // Lazily-initialized buffers.
    String encoded;

    Int32 oldLength = s.GetLength();

    // This loop alternates between copying over allowed characters and
    // encoding in chunks. This results in fewer method calls and
    // allocations than encoding one character at a time.
    Int32 current = 0;
    while (current < oldLength) {
        // Start in "copying" mode where we copy over allowed chars.

        // Find the next character which needs to be encoded.
        Int32 nextToEncode = current;
        while (nextToEncode < oldLength
                && IsAllowed(s[nextToEncode], allow)) {
            nextToEncode++;
        }

        // If there's nothing more to encode...
        if (nextToEncode == oldLength) {
            if (current == 0) {
                // We didn't need to encode anything!
                *result = s;
                return NOERROR;
            } else {
                // Presumably, we've already done some encoding.
                encoded.Append((const char *)s + current, oldLength);
                *result = encoded;
                return NOERROR;
            }
        }

        if (nextToEncode > current) {
            // Append allowed characters leading up to this point.
            encoded.Append((const char *)s + current, nextToEncode);
        } else {
            // assert nextToEncode == current
        }

        // Switch to "encoding" mode.

        // Find the next allowed character.
        current = nextToEncode;
        Int32 nextAllowed = current + 1;
        while (nextAllowed < oldLength
                && !IsAllowed(s[nextAllowed], allow)) {
            nextAllowed++;
        }

        // Convert the substring to bytes and encode the bytes as
        // '%'-escaped octets.
        String toEncode = s.Substring(current, nextAllowed);
        // try {
            // TODO: ALEX fix getBytes
            // byte[] bytes = toEncode.getBytes(DEFAULT_ENCODING);
            // int bytesLength = bytes.length;
            Int32 bytesLength = toEncode.GetLength();
            for (Int32 i = 0; i < bytesLength; i++) {
                encoded.Append("%");
                encoded.AppendFormat("%c", HEX_DIGITS[(toEncode[i] & 0xf0) >> 4]);
                encoded.AppendFormat("%c", HEX_DIGITS[toEncode[i] & 0xf]);
            }
        // } catch (UnsupportedEncodingException e) {
        //     throw new AssertionError(e);
        // }

        current = nextAllowed;
    }

    // Encoded could still be null at this point if s is empty.
    *result = encoded.IsNullOrEmpty() ? s : encoded;
    return NOERROR;
}

/**
 * Returns true if the given character is allowed.
 *
 * @param c character to check
 * @param allow characters to allow
 * @return true if the character is allowed or false if it should be
 *  encoded
 */
Boolean Uri::IsAllowed(
    /* [in] */ Char8 c,
    /* [in] */ const String& allow)
{
    return (c >= 'A' && c <= 'Z')
            || (c >= 'a' && c <= 'z')
            || (c >= '0' && c <= '9')
            || String("_-!.~'()*").IndexOf(c) != NOT_FOUND
            || (!allow.IsNull() && allow.IndexOf(c) != NOT_FOUND);
}

/** Unicode replacement character: \\uFFFD. */
static const Byte REPLACEMENT[] = { 0xFF, 0xFD };

/**
 * Decodes '%'-escaped octets in the given string using the UTF-8 scheme.
 * Replaces invalid octets with the unicode replacement character
 * ("\\uFFFD").
 *
 * @param s encoded string to decode
 * @return the given string with escaped octets decoded, or null if
 *  s is null
 */
ECode Uri::Decode(
    /* [in] */ const String& s,
    /* [out] */ String* result)
{
    if (!result) return E_INVALID_ARGUMENT;

    /*
    Compared to java.net.URLEncoderDecoder.decode(), this method decodes a
    chunk at a time instead of one character at a time, and it doesn't
    throw exceptions. It also only allocates memory when necessary--if
    there's nothing to decode, this method won't do much.
    */

    if (s.IsNull()) {
        result->SetTo(NULL);
        return NOERROR;
    }

    // Lazily-initialized buffers.
    String decoded;
    AutoPtr<IByteArrayOutputStream> out;

    Int32 oldLength = s.GetLength();

    // This loop alternates between copying over normal characters and
    // escaping in chunks. This results in fewer method calls and
    // allocations than decoding one character at a time.
    Int32 current = 0;
    while (current < oldLength) {
        // Start in "copying" mode where we copy over normal characters.

        // Find the next escape sequence.
        Int32 nextEscape = s.IndexOf('%', current);

        if (nextEscape == NOT_FOUND) {
            if (decoded.IsNull()) {
                // We didn't actually decode anything.
                *result = s;
                return NOERROR;
            } else {
                // Append the remainder and return the decoded string.
                decoded.Append((const Char8*)s + current, oldLength);
                *result = decoded;
                return NOERROR;
            }
        }

        // Prepare buffers.
        if (decoded.IsNull()) {
            // Looks like we're going to need the buffers...
            // We know the new string will be shorter. Using the old length
            // may overshoot a bit, but it will save us from resizing the
            // buffer.
            // decoded = new StringBuilder(oldLength);
            FAIL_RETURN(
                CByteArrayOutputStream::New(4, (IByteArrayOutputStream**)&out));
        } else {
            // Clear decoding buffer.
            out->Reset();
        }

        // Append characters leading up to the escape.
        if (nextEscape > current) {
            decoded.Append((const Char8*)s + current, nextEscape);

            current = nextEscape;
        } else {
            // assert current == nextEscape
        }

        // Switch to "decoding" mode where we decode a string of escape
        // sequences.

        // Decode and append escape sequences. Escape sequences look like
        // "%ab" where % is literal and a and b are hex digits.
        // try {
        do {
            if (current + 2 >= oldLength) {
                // Truncated escape sequence.
                out->Write(REPLACEMENT[0]);
                out->Write(REPLACEMENT[1]);
            } else {
                Int32 a = Elastos::Core::Character::ToDigit(s[current + 1], 16);
                Int32 b = Elastos::Core::Character::ToDigit(s[current + 2], 16);

                if (a == -1 || b == -1) {
                    // Non hex digits.
                    out->Write(REPLACEMENT[0]);
                    out->Write(REPLACEMENT[1]);
                } else {
                    // Combine the hex digits into one byte and write.
                    out->Write((a << 4) + b);
                }
            }

            // Move passed the escape sequence.
            current += 3;
        } while (current < oldLength && s[current] == '%');

        // Decode UTF-8 bytes into a string and append it.
        // TODO: ALEX fix ToString(DEFAULT_ENCODING)
        String outString;
        FAIL_RETURN(out->ToString(&outString));

        decoded.Append(outString);
        // } catch (UnsupportedEncodingException e) {
        //     throw new AssertionError(e);
        // } catch (IOException e) {
        //     throw new AssertionError(e);
        // }
    }

    // If we don't have a buffer, we didn't have to decode anything.
    *result = decoded.IsNullOrEmpty() ? s : decoded;
    return NOERROR;
}

ECode AbstractPart::Init(
    /* [in] */ const String& encoded,
    /* [in] */ const String& decoded)
{
    mEncoded = encoded;
    mDecoded = decoded;

    return NOERROR;
}

ECode AbstractPart::GetDecoded(
    /* [out] */ String* decoded)
{
    if (decoded == NULL) return E_INVALID_ARGUMENT;

    if (!mDecoded.Compare(Uri::NOT_CACHED)) {
        return Uri::Decode(mEncoded, decoded);
    } else {
        *decoded = mDecoded;
        return NOERROR;
    }
}

ECode AbstractPart::WriteTo(
    /* [in] */ IParcel* parcel)
{
    // @SuppressWarnings("StringEquality")
    Boolean hasEncoded = !mEncoded.Equals(Uri::NOT_CACHED);

    // @SuppressWarnings("StringEquality")
    Boolean hasDecoded = !mDecoded.Equals(Uri::NOT_CACHED);

    if (hasEncoded && hasDecoded) {
        parcel->WriteInt32(Representation::BOTH);
        parcel->WriteString(mEncoded);
        parcel->WriteString(mDecoded);
    } else if (hasEncoded) {
        parcel->WriteInt32(Representation::ENCODED);
        parcel->WriteString(mEncoded);
    } else if (hasDecoded) {
        parcel->WriteInt32(Representation::DECODED);
        parcel->WriteString(mDecoded);
    } else {
        // throw new AssertionError();
        return E_ASSERTION_ERROR;
    }

    return NOERROR;
}

ECode Uri::IsOpaque(
    /* [out] */ Boolean* isOpaque)
{
    VALIDATE_NOT_NULL(isOpaque);

    Boolean isHierachical;
    FAIL_RETURN(IsHierarchical(&isHierachical));
    *isOpaque = !isHierachical;

    return NOERROR;
}

ECode Uri::IsAbsolute(
    /* [out] */ Boolean* isAbsolute)
{
    VALIDATE_NOT_NULL(isAbsolute);

    Boolean isRelative;
    FAIL_RETURN(IsRelative(&isRelative));
    *isAbsolute = !isRelative;

    return NOERROR;
}

ECode Uri::Parse(
    /* [in] */ const String& uriString,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return CStringUri::New(uriString, uri);
}

CARAPI Uri::FromFile(
    /* [in] */ IFile* file,
    /* [out] */ IUri** uri)
{
    if (file == NULL) {
        // throw new NullPointerException("file");
        return E_NULL_POINTER_EXCEPTION;
    }

    String str;
    FAIL_RETURN(file->GetAbsolutePath(&str));

    AutoPtr<IPathPart> path;
    FAIL_RETURN(PathPart::FromDecoded(str, (IPathPart**)&path));

    return CHierarchicalUri::New(
        String("file"), Part::EMPTY, path, Part::PartNULL, Part::PartNULL, uri);
}

ECode Uri::FromParts(
    /* [in] */ const String& scheme,
    /* [in] */ const String& ssp,
    /* [in] */ const String& fragment,
    /* [out] */ IUri** uri)
{
    if (scheme.IsNull()) {
        // throw new NullPointerException("scheme");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (ssp.IsNull()) {
        // throw new NullPointerException("ssp");
        return E_NULL_POINTER_EXCEPTION;
    }

    AutoPtr<IPart> part1;
    FAIL_RETURN(Part::FromDecoded(ssp, (IPart**)&part1));

    AutoPtr<IPart> part2;
    FAIL_RETURN(Part::FromDecoded(fragment, (IPart**)&part2));

    return COpaqueUri::New(scheme, part1, part2, uri);
}

ECode Uri::WithAppendedPath(
    /* [in] */ IUri* baseUri,
    /* [in] */ const String& pathSegment,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUriBuilder> builder;
    FAIL_RETURN(baseUri->BuildUpon((IUriBuilder**)&builder));

    FAIL_RETURN(builder->AppendEncodedPath(pathSegment));

    return builder->Build(uri);
}

AbstractHierarchicalUri::AbstractHierarchicalUri()
    : mHost(Uri::NOT_CACHED)
    , mPort(NOT_CALCULATED)
{
}

ECode AbstractHierarchicalUri::GetHost(
    /* [out] */ String* host)
{
    VALIDATE_NOT_NULL(host);

    // @SuppressWarnings("StringEquality")
    Boolean cached = (!mHost.Equals(Uri::NOT_CACHED));
    if (!cached) {
        FAIL_RETURN(ParseHost(&mHost));
    }

    *host = mHost;

    return NOERROR;
}

ECode AbstractHierarchicalUri::GetUserInfoPart(
    /* [out] */ IPart** part)
{
    VALIDATE_NOT_NULL(part);

    if (mUserInfo == NULL) {
        String result;
        FAIL_RETURN(ParseUserInfo(&result));
        FAIL_RETURN(Part::FromEncoded(
            result, (IPart**)&mUserInfo));
    }

    *part = mUserInfo;

    return NOERROR;
}

ECode AbstractHierarchicalUri::ParseUserInfo(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    String authority;
    FAIL_RETURN(GetEncodedAuthority(&authority));
    if (authority.IsNull()) {
        *result = NULL;
        return NOERROR;
    }

    Int32 end = authority.IndexOf('@');
    if (end == NOT_FOUND) {
        result->SetTo(NULL);
    }
    else {
        *result = authority.Substring(0, end);
    }

    return NOERROR;
}

ECode AbstractHierarchicalUri::ParseHost(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    String authority;
    FAIL_RETURN(GetEncodedAuthority(&authority));
    if (authority.IsNull()) {
        result->SetTo(NULL);
        return NOERROR;
    }

    // Parse out user info and then port.
    Int32 userInfoSeparator = authority.IndexOf('@');
    Int32 portSeparator = authority.IndexOf(':', userInfoSeparator);

    String encodedHost = portSeparator == NOT_FOUND
            ? authority.Substring(userInfoSeparator + 1)
            : authority.Substring(userInfoSeparator + 1, portSeparator);

    return Decode(encodedHost, result);
}

ECode AbstractHierarchicalUri::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);

    if (mPort == NOT_CALCULATED) {
        FAIL_RETURN(ParsePort(&mPort));
    }

    *port = mPort;

    return NOERROR;
}

ECode AbstractHierarchicalUri::ParsePort(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    String authority;
    FAIL_RETURN(GetEncodedAuthority(&authority));
    if (authority.IsNull()) {
        *result = -1;
        return NOERROR;
    }

    // Make sure we look for the port separtor *after* the user info
    // separator. We have URLs with a ':' in the user info.
    Int32 userInfoSeparator = authority.IndexOf('@');
    Int32 portSeparator = authority.IndexOf(':', userInfoSeparator);

    if (portSeparator == NOT_FOUND) {
        *result = -1;
        return NOERROR;
    }

    String portString;
    FAIL_RETURN(Decode(authority.Substring(portSeparator + 1), &portString));

    // try {
    *result = portString.ToInt32(10);
    return NOERROR;
    // } catch (NumberFormatException e) {
    //     Log.w(LOG, "Error parsing port string.", e);
    //     return -1;
    // }
}


