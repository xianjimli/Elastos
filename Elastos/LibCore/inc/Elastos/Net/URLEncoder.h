
#ifndef __URLENCODER_H__
#define __URLENCODER_H__

#include "cmdef.h"
#include <elastos.h>
#include <StringBuffer.h>

using namespace Elastos;

/**
 * This class is used to encode a string using the format required by
 * {@code application/x-www-form-urlencoded} MIME content type.
 */
class URLEncoder
{
public:
    /**
     * Encodes a given string {@code s} in a x-www-form-urlencoded string using
     * the specified encoding scheme {@code enc}.
     * <p>
     * All characters except letters ('a'..'z', 'A'..'Z') and numbers ('0'..'9')
     * and characters '.', '-', '*', '_' are converted into their hexadecimal
     * value prepended by '%'. For example: '#' -> %23. In addition, spaces are
     * substituted by '+'
     *
     * @param s
     *            the string to be encoded.
     * @return the encoded string.
     * @deprecated use {@link #encode(String, String)} instead.
     */
    //@Deprecated
    static CARAPI Encode(
        /* [in] */ const String& s,
        /* [out] */ String* encodedStr);

    /**
     * Encodes the given string {@code s} in a x-www-form-urlencoded string
     * using the specified encoding scheme {@code enc}.
     * <p>
     * All characters except letters ('a'..'z', 'A'..'Z') and numbers ('0'..'9')
     * and characters '.', '-', '*', '_' are converted into their hexadecimal
     * value prepended by '%'. For example: '#' -> %23. In addition, spaces are
     * substituted by '+'
     *
     * @param s
     *            the string to be encoded.
     * @param enc
     *            the encoding scheme to be used.
     * @return the encoded string.
     * @throws UnsupportedEncodingException
     *             if the specified encoding scheme is invalid.
     */
    static CARAPI Encode(
        /* [in] */ const String& s,
        /* [in] */ const String& enc,
        /* [out] */ String* encodedStr);

private:
    /**
     * Prevents this class from being instantiated.
     */
    URLEncoder();

    static CARAPI_(void) Convert(
        /* [in] */ const String& s,
        /* [in] */ StringBuffer& buf,
        /* [in] */ const String& enc);

public:
    static const CString sDigits;
};

#endif //__URLENCODER_H__
