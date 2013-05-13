#include "Util.h"


ECode Util::Decode(
    /* [in] */ const String& s,
    /* [in] */ Boolean convertPlus,
    /* [out] */ String* str)
{
    return Decode(s, convertPlus, NULL, str);
}

ECode Util::Decode(
    /* [in] */ const String &s,
    /* [in] */ Boolean convertPlus,
    /* [in] */ const String& encoding,
    /* [out] */ String* str)
{
    if (!convertPlus && s.IndexOf('%') == -1)
        return s;

    StringBuffer result;
    ECode ec = NOERROR;
    AutoPtr<IByteArrayOutputStream> out;

    ec = CByteArrayOutputStream::New((IByteArrayOutputStream**) &out);
    if (FAILED(ec)){
        return ec;
    }

    for (int32 i = 0; i < s.GetLength();) {
        char c = s.CharAt(i);
        if (convertPlus && c == '+')
            result += ' ';
        else if (c == '%') {
            out->Reset();
            do {
                if (i + 2 >= s.GetLength()) {
                    return E_INVALID_ARGUMENT;
                }
                Int32 d1 = Character::ToDigit(s.CharAt(i + 1), 16);
                Int32 d2 = Character::ToDigit(s.CharAt(i + 2), 16);
                if (d1 == -1 || d2 == -1) {
                    return E_INVALID_ARGUMENT;
                }
                out->Write((Byte) ((d1 << 4) + d2));
                i += 3;
            } while (i < s.GetLength() && s.CharAt(i) == '%');

            String outstr;
            if (encoding.IsNull()) {
                out->ToString(&outstr);
                result += outstr;
            } else {
                ec = out->ToString(encoding, &outstr);
                if (FAILED(ec)) {
                    return ec;
                }
                result += outstr;

            }
            continue;
        } else {
            result += c;
        }
        i++;
    }

    *str = result.ToString();

    return NOERROR;
}

ECode Util::ToASCIILowerCase(
    /* [in] */ const String &s)
{
    Int32 len = s.GetLength();

    StringBuffer buffer;
    for (Int32 i = 0; i < len; i++) {
        char c = s.CharAt(i);
        if ('A' <= c && c <= 'Z') {
            buffer += (const char)(c + ('a' - 'A'));
        } else {
            buffer += c;
        }
    }

    return buffer.ToString();
}

ECode Util::ToASCIIUpperCase(
    /* [in] */ const String &s)
{
    Int32 len = s.GetLength();
    StringBuilder buffer;

    for (Int32 i = 0; i < len; i++) {
        char c = s.CharAt(i);
        if ('a' <= c && c <= 'z') {
            buffer += (const char)(c - ('a' - 'A'));
        } else {
            buffer += c;;
        }
    }
    return buffer.ToString();
}
