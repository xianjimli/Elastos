
#include "URLEncoder.h"
#include <elastos/Character.h>

using namespace Elastos::Core;

const String URLEncoder::mDigits = String("123456789ABCDEF");

URLEncoder::URLEncoder()
{}

ECode URLEncoder::Encode(
    /* [in] */ const String& s,
    /* [out] */ String* encodeS)
{
    VALIDATE_NOT_NULL(encodeS);

    // Guess a bit bigger for encoded form
    StringBuffer buf;
    for (UInt32 i = 0; i < s.GetLength(); i++) {
        Char32 ch = s[i];
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
                || (ch >= '0' && ch <= '9') || String(".-*_").IndexOf(ch) > -1) {
            buf += ch;
        }
        else if (ch == ' ') {
            buf += '+';
        }
        else {
            //byte[] bytes = new String(new char[] { ch }).getBytes();
            Byte b = Character::GetDirectionality(ch);
            buf += '%';
            buf += mDigits[(b & 0xf0) >> 4];
            buf += mDigits[b & 0xf];
            // for (int j = 0; j < bytes.length; j++) {
            //     buf.append('%');
            //     buf.append(digits.charAt((bytes[j] & 0xf0) >> 4));
            //     buf.append(digits.charAt(bytes[j] & 0xf));
            // }
        }
    }
    *encodeS = String(buf);
    return NOERROR;
}

ECode URLEncoder::EncodeEx(
    /* [in] */ const String& s,
    /* [in] */  const String& enc,
    /* [out] */ String* encodeS)
{
    VALIDATE_NOT_NULL(encodeS);

    if (s.IsNull() || enc.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new NullPointerException();
    }
    // check for UnsupportedEncodingException
//    "".getBytes(enc);

    // Guess a bit bigger for encoded form
    StringBuffer buf;
    Int32 start = -1;
    for (UInt32 i = 0; i < s.GetLength(); i++) {
        Char32 ch = s[i];
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
                || (ch >= '0' && ch <= '9') || String(" .-*_").IndexOf(ch) > -1) {
            if (start >= 0) {
                Convert(s.Substring(start, i), buf, enc);
                start = -1;
            }
            if (ch != ' ') {
                buf += ch;
            }
            else {
                buf += '+';
            }
        }
        else {
            if (start < 0) {
                start = i;
            }
        }
    }
    if (start >= 0) {
        Convert(s.Substring(start, s.GetLength()), buf, enc);
    }
    *encodeS = String(buf);
    return NOERROR;
}

void URLEncoder::Convert(
    /* [in] */ const String& s,
    /* [in] */ StringBuffer buf,
    /* [in] */ const String& enc)
{
    // byte[] bytes = s.getBytes(enc);
    // for (int j = 0; j < bytes.length; j++) {
    //     buf.append('%');
    //     buf.append(digits.charAt((bytes[j] & 0xf0) >> 4));
    //     buf.append(digits.charAt(bytes[j] & 0xf));
    // }
}
