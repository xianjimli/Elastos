
#include "URIEncoderDecoder.h"
#include "Elastos.IO.h"
#include <elastos/Character.h>
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>

using namespace Elastos::Core;

const String URIEncoderDecoder::mDigits = String("0123456789ABCDEF");

ECode URIEncoderDecoder::Validate(
    /* [in] */ const String& s,
    /* [in] */ const String& legal)
{
    for (UInt32 i = 0; i < s.GetLength();) {
        Char32 ch = s[i];
        if (ch == '%') {
            do {
                if (i + 2 >= s.GetLength()) {
                    return E_URI_SYNTAX_EXCEPTION;
//                    throw new URISyntaxException(s, "Incomplete % sequence", i);
                }
                Int32 d1 = Character::ToDigit(s[i + 1], 16);
                Int32 d2 = Character::ToDigit(s[i + 2], 16);
                if (d1 == -1 || d2 == -1) {
                    return E_URI_SYNTAX_EXCEPTION;
//                    throw new URISyntaxException(s, "Invalid % sequence: " +
//                            s.substring(i, i + 3), i);
                }

                i += 3;
            } while (i < s.GetLength() && s[i] == '%');

            continue;
        }
        if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
                || (ch >= '0' && ch <= '9') || legal.IndexOf(ch) > -1 || (ch > 127
                && !Character::IsSpaceChar(ch) && !Character::IsISOControl(ch)))) {
            return E_URI_SYNTAX_EXCEPTION;
//            throw new URISyntaxException(s, "Illegal character", i);
        }
        i++;
    }

    return NOERROR;
}

ECode URIEncoderDecoder::ValidateSimple(
    /* [in] */ const String& s,
    /* [in] */ const String& legal)
{
    for (UInt32 i = 0; i < s.GetLength();) {
        Char32 ch = s[i];
        if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
                || (ch >= '0' && ch <= '9') || legal.IndexOf(ch) > -1)) {
            return E_URI_SYNTAX_EXCEPTION;
//            throw new URISyntaxException(s, "Illegal character", i);
        }
        i++;
    }

    return NOERROR;
}

ECode URIEncoderDecoder::QuoteIllegal(
    /* [in] */ const String& s,
    /* [in] */ const String& legal,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuffer buf;
    for (UInt32 i = 0; i < s.GetLength(); i++) {
        Char32 ch = s[i];
        if ((ch >= 'a' && ch <= 'z')
                || (ch >= 'A' && ch <= 'Z')
                || (ch >= '0' && ch <= '9')
                || legal.IndexOf(ch) > -1
                || (ch > 127 && !Character::IsSpaceChar(ch) && !Character::IsISOControl(ch))) {
            buf += ch;
        }
        else {
            //todo:have question!!!!!!!
            //begin from this
//            byte[] bytes = new String(new char[] { ch }).getBytes(Charsets.UTF_8);
            Byte b = Character::GetDirectionality(ch);
            buf += '%';
            buf += mDigits[(b & 0xf0) >> 4];
            buf += mDigits[b & 0xf];
            // ArrayOf<Byte>* bytes;
            // for (Int32 j = 0; j < bytes->GetLength(); j++) {
            //     buf += '%';
            //     buf += mDigits[((*bytes)[j] & 0xf0) >> 4];
            //     buf += mDigits[(*bytes)[j] & 0xf];
            // }
        }
    }
    *str = String(buf);
    return NOERROR;
}

ECode URIEncoderDecoder::EncodeOthers(
    /* [in] */ const String& s,
    /* [out] */ String* encodeS)
{
    VALIDATE_NOT_NULL(encodeS);

    StringBuffer buf;
    for (UInt32 i = 0; i < s.GetLength(); i++) {
        Char32 ch = s[i];
        if (ch <= 127) {
            buf += ch;
        }
        else {
//            byte[] bytes = new String(new char[] { ch }).getBytes(Charsets.UTF_8);
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

ECode URIEncoderDecoder::Decode(
    /* [in] */ const String& s,
    /* [out] */ String* decodeS)
{
    VALIDATE_NOT_NULL(decodeS);

    StringBuffer result;
    AutoPtr<IByteArrayOutputStream> out;
    CByteArrayOutputStream::New((IByteArrayOutputStream**)&out);
    for (UInt32 i = 0; i < s.GetLength();) {
        Char32 c = s[i];
        if (c == '%') {
            out->Reset();
            do {
                if (i + 2 >= s.GetLength()) {
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
//                    throw new IllegalArgumentException("Incomplete % sequence at: " + i);
                }
                Int32 d1 = Character::ToDigit(s[i + 1], 16);
                Int32 d2 = Character::ToDigit(s[i + 2], 16);
                if (d1 == -1 || d2 == -1) {
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
//                    throw new IllegalArgumentException("Invalid % sequence " +
//                            s.substring(i, i + 3) + " at " + i);
                }
                out->Write((byte) ((d1 << 4) + d2));
                i += 3;
            } while (i < s.GetLength() && s[i] == '%');
            String s;
            out->ToString(&s);
            result += s;
            continue;
        }
        result += c;
        i++;
    }
    *decodeS = String(result);

    return NOERROR;
}
