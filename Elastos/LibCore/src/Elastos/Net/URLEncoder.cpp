
#include "URLEncoder.h"
#include <Elastos.Core.h>
#include <elastos/Character.h>

using namespace Elastos::Core;

const CString URLEncoder::sDigits = "123456789ABCDEF";

URLEncoder::URLEncoder()
{}

ECode URLEncoder::Encode(
    /* [in] */ const String& s,
    /* [out] */ String* encodedS)
{
    VALIDATE_NOT_NULL(encodedS);

    // Guess a bit bigger for encoded form
    StringBuffer buf;
    for (Int32 i = 0; i < s.GetCharCount(); i++) {
        Char32 ch = s.GetChar(i);
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
                || (ch >= '0' && ch <= '9') || CString(".-*_").IndexOf(ch) > -1) {
            buf += ch;
        }
        else if (ch == ' ') {
            buf += '+';
        }
        else {
            Int32 length;
            ArrayOf_<Char8, 4> bytes;
            Character::ToChars(ch, bytes, 0, &length);
            for (Int32 j = 0; j < length; j++) {
                buf += '%';
                buf += sDigits[(bytes[j] & 0xf0) >> 4];
                buf += sDigits[bytes[j] & 0xf];
            }
        }
    }
    *encodedS = String(buf);
    return NOERROR;
}

ECode URLEncoder::Encode(
    /* [in] */ const String& s,
    /* [in] */  const String& enc,
    /* [out] */ String* encodedS)
{
    VALIDATE_NOT_NULL(encodedS);

    if (s.IsNull() || enc.IsNull()) {
//        throw new NullPointerException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // check for UnsupportedEncodingException
//    "".getBytes(enc);

    // Guess a bit bigger for encoded form
    StringBuffer buf;
    Int32 start = -1;
    for (Int32 i = 0; i < s.GetCharCount(); i++) {
        Char32 ch = s.GetChar(i);
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
                || (ch >= '0' && ch <= '9') || CString(" .-*_").IndexOf(ch) > -1) {
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
    *encodedS = String(buf);
    return NOERROR;
}

void URLEncoder::Convert(
    /* [in] */ const String& s,
    /* [in] */ StringBuffer& buf,
    /* [in] */ const String& enc)
{
    // byte[] bytes = s.getBytes(enc);
    // for (int j = 0; j < bytes.length; j++) {
    //     buf.append('%');
    //     buf.append(digits.charAt((bytes[j] & 0xf0) >> 4));
    //     buf.append(digits.charAt(bytes[j] & 0xf));
    // }
    assert(0);
}
