
#ifndef __URIENCODERDECODER_H__
#define __URIENCODERDECODER_H__

#include "cmdef.h"
#include "Elastos.Net_server.h"

/**
 * This class is used to encode a string using the format required by {@code
 * application/x-www-form-urlencoded} MIME content type. It contains helper
 * methods used by the URI class, and performs encoding and decoding in a
 * slightly different way than {@code URLEncoder} and {@code URLDecoder}.
 */
class URIEncoderDecoder
{
public:
    /**
     * Validate a string by checking if it contains any characters other than:
     * 1. letters ('a'..'z', 'A'..'Z') 2. numbers ('0'..'9') 3. characters in
     * the legalset parameter 4. others (unicode characters that are not in
     * US-ASCII set, and are not ISO Control or are not ISO Space characters)
     * <p>
     * called from {@code URI.Helper.parseURI()} to validate each component
     *
     * @param s
     *            {@code java.lang.String} the string to be validated
     * @param legal
     *            {@code java.lang.String} the characters allowed in the String
     *            s
     */
    static CARAPI Validate(
        /* [in] */ const String& s,
        /* [in] */ const String& legal);

    static CARAPI ValidateSimple(
        /* [in] */ const String& s,
        /* [in] */ const String& legal);

    /**
     * All characters except letters ('a'..'z', 'A'..'Z') and numbers ('0'..'9')
     * and legal characters are converted into their hexidecimal value prepended
     * by '%'.
     * <p>
     * For example: '#' -> %23
     * Other characters, which are unicode chars that are not US-ASCII, and are
     * not ISO Control or are not ISO Space chars, are preserved.
     * <p>
     * Called from {@code URI.quoteComponent()} (for multiple argument
     * constructors)
     *
     * @param s
     *            java.lang.String the string to be converted
     * @param legal
     *            java.lang.String the characters allowed to be preserved in the
     *            string s
     * @return java.lang.String the converted string
     */
    static CARAPI QuoteIllegal(
        /* [in] */ const String& s,
        /* [in] */ const String& legal,
        /* [out] */ String* str);

    /**
     * Other characters, which are Unicode chars that are not US-ASCII, and are
     * not ISO Control or are not ISO Space chars are not preserved. They are
     * converted into their hexidecimal value prepended by '%'.
     * <p>
     * For example: Euro currency symbol -> "%E2%82%AC".
     * <p>
     * Called from URI.toASCIIString()
     *
     * @param s
     *            java.lang.String the string to be converted
     * @return java.lang.String the converted string
     */
    static CARAPI EncodeOthers(
        /* [in] */ const String& s,
        /* [out] */ String* encodeS);

    /**
     * Decodes the string argument which is assumed to be encoded in the {@code
     * x-www-form-urlencoded} MIME content type using the UTF-8 encoding scheme.
     * <p>
     *'%' and two following hex digit characters are converted to the
     * equivalent byte value. All other characters are passed through
     * unmodified.
     * <p>
     * e.g. "A%20B%20C %24%25" -> "A B C $%"
     * <p>
     * Called from URI.getXYZ() methods
     *
     * @param s
     *            java.lang.String The encoded string.
     * @return java.lang.String The decoded version.
     */
    static CARAPI Decode(
        /* [in] */ const String& s,
        /* [out] */ String* decodeS);

private:
    static const CString sDigits;
};

#endif //__URIENCODERDECODER_H__
