
#include "URIEncoderDecoder.h"
#include "Elastos.IO.h"
#include <elastos/Character.h>
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>
#include <stdio.h>

using namespace Elastos::Core;

const CString URIEncoderDecoder::sDigits = "0123456789ABCDEF";

ECode URIEncoderDecoder::Validate(
    /* [in] */ const String& s,
    /* [in] */ const String& legal)
{
    for (Int32 i = 0; i < s.GetCharCount();) {
        Char32 ch = s.GetChar(i);
        if (ch == '%') {
            do {
                if (i + 2 >= s.GetCharCount()) {
//                    throw new URISyntaxException(s, "Incomplete % sequence", i);
                    return E_URI_SYNTAX_EXCEPTION;
                }
                Int32 d1 = Character::ToDigit(s.GetChar(i + 1), 16);
                Int32 d2 = Character::ToDigit(s.GetChar(i + 2), 16);
                if (d1 == -1 || d2 == -1) {
//                    throw new URISyntaxException(s, "Invalid % sequence: " +
//                            s.substring(i, i + 3), i);
                    return E_URI_SYNTAX_EXCEPTION;
                }

                i += 3;
            } while (i < s.GetCharCount() && s.GetChar(i) == '%');

            continue;
        }
        if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
                || (ch >= '0' && ch <= '9') || legal.IndexOf(ch) > -1 || (ch > 127
                && !Character::IsSpaceChar(ch) && !Character::IsISOControl(ch)))) {
//            throw new URISyntaxException(s, "Illegal character", i);
            return E_URI_SYNTAX_EXCEPTION;
        }
        i++;
    }

    return NOERROR;
}

ECode URIEncoderDecoder::ValidateSimple(
    /* [in] */ const String& s,
    /* [in] */ const String& legal)
{
    for (Int32 i = 0; i < s.GetCharCount();) {
        Char32 ch = s.GetChar(i);
        if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
                || (ch >= '0' && ch <= '9') || legal.IndexOf(ch) > -1)) {
//            throw new URISyntaxException(s, "Illegal character", i);
            return E_URI_SYNTAX_EXCEPTION;
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
    for (Int32 i = 0; i < s.GetCharCount(); i++) {
        Char32 ch = s.GetChar(i);
        if ((ch >= 'a' && ch <= 'z')
                || (ch >= 'A' && ch <= 'Z')
                || (ch >= '0' && ch <= '9')
                || legal.IndexOf(ch) > -1
                || (ch > 127 && !Character::IsSpaceChar(ch) && !Character::IsISOControl(ch))) {
            buf += ch;
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
    *str = buf;
    return NOERROR;
}

ECode URIEncoderDecoder::EncodeOthers(
    /* [in] */ const String& s,
    /* [out] */ String* encodedS)
{
    VALIDATE_NOT_NULL(encodedS);

    StringBuffer buf;
    for (Int32 i = 0; i < s.GetCharCount(); i++) {
        Char32 ch = s.GetChar(i);
        if (ch <= 127) {
            buf += ch;
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
    *encodedS = buf;
    return NOERROR;
}

ECode URIEncoderDecoder::Decode(
    /* [in] */ const String& s,
    /* [out] */ String* decodedS)
{
    printf("1******************************\n");
    VALIDATE_NOT_NULL(decodedS);
    printf("2******************************\n");
    StringBuffer result;
    printf("%s, %d\n", __FILE__, __LINE__);
    AutoPtr<IByteArrayOutputStream> out;
    printf("%s, %d\n", __FILE__, __LINE__);
    FAIL_RETURN(CByteArrayOutputStream::New((IByteArrayOutputStream**)&out));
    printf("%s, %d\n", __FILE__, __LINE__);
    Int32 count = s.GetCharCount();
    printf("the count is %d\n", count);
    for (Int32 i = 0; i < s.GetCharCount();) {
        printf("%s, %d\n", __FILE__, __LINE__);
        Char32 c = s.GetChar(i);
        if (c == '%') {
            printf("%s, %d\n", __FILE__, __LINE__);
            out->Reset();
            do {
                if (i + 2 >= s.GetCharCount()) {
//                    throw new IllegalArgumentException("Incomplete % sequence at: " + i);
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
                }
                Int32 d1 = Character::ToDigit(s.GetChar(i + 1), 16);
                Int32 d2 = Character::ToDigit(s.GetChar(i + 2), 16);
                printf("d1, d2 is %c, %c\n", s.GetChar(i + 1), s.GetChar(i + 2));
                printf("d1, d2 is %d, %d\n", d1, d2);
                if (d1 == -1 || d2 == -1) {
//                    throw new IllegalArgumentException("Invalid % sequence " +
//                            s.substring(i, i + 3) + " at " + i);
                    printf("%s, %d\n", __FILE__, __LINE__);
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
                }
                printf("the (d1 << 4) + d2) %d\n ", ((d1 << 4) + d2));
                out->Write((Byte) ((d1 << 4) + d2));
                i += 3;
            } while (i < s.GetCharCount() && s.GetChar(i) == '%');
            printf("******************************\n");
            String s;
            out->ToString(&s);
            ArrayOf<Byte> *array;
            out->ToByteArray(&array);
            result += s;
            printf("result s is %s \n", (const char *) s);
            printf("result is %s \n", (const char *) result);
            printf("result is %c %d\n", (*array)[0], (*array)[0]);
            continue;
        }
        result += c;
        i++;
    }
    printf("%s, %d\n", __FILE__, __LINE__);
    *decodedS = result;
    printf("%s, %d\n", __FILE__, __LINE__);
    return NOERROR;
}
