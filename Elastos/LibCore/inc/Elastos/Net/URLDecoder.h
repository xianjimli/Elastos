
#ifndef __URLDECODER_H__
#define __URLDECODER_H__

#include "cmdef.h"
#include <elastos.h>

using namespace Elastos;

/**
 * This class is used to decode a string which is encoded in the {@code
 * application/x-www-form-urlencoded} MIME content type.
 */
class URLDecoder
{
public:
    /**
     * Decodes the argument which is assumed to be encoded in the {@code
     * x-www-form-urlencoded} MIME content type.
     * <p>
     *'+' will be converted to space, '%' and two following hex digit
     * characters are converted to the equivalent byte value. All other
     * characters are passed through unmodified. For example "A+B+C %24%25" ->
     * "A B C $%".
     *
     * @param s
     *            the encoded string.
     * @return the decoded clear-text representation of the given string.
     * @deprecated use {@link #decode(String, String)} instead.
     */
    //@Deprecated
    static CARAPI Decode(
        /* [in] */ const String& s,
        /* [out] */ String* decodedStr);

    /**
     * Decodes the argument which is assumed to be encoded in the {@code
     * x-www-form-urlencoded} MIME content type using the specified encoding
     * scheme.
     * <p>
     *'+' will be converted to space, '%' and two following hex digit
     * characters are converted to the equivalent byte value. All other
     * characters are passed through unmodified. For example "A+B+C %24%25" ->
     * "A B C $%".
     *
     * @param s
     *            the encoded string.
     * @param encoding
     *            the encoding scheme to be used.
     * @return the decoded clear-text representation of the given string.
     * @throws UnsupportedEncodingException
     *             if the specified encoding scheme is invalid.
     */
    static CARAPI Decode(
        /* [in] */ const String& s,
        /* [in] */ const String& encoding,
        /* [out] */ String* decodedStr);

private:
    URLDecoder();

//    static CARAPI Decode(
//        /* [in] */ const String& s,
//        /* [in] */ Charset charset,
//        /* [out] */ String* decodedStr);
};

#endif //__URLDECODER_H__
