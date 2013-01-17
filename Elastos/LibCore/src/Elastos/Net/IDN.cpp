
#include "cmdef.h"
#include "IDN.h"
#include <unicode/ucat.h>
#include <unicode/uidna.h>
// #include <unicode/ustring.h>


ECode IDN::ToASCII(
    /* [in] */ const String& input,
    /* [in] */ Int32 flags,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    return Convert(input, flags, TRUE, result);
}

ECode IDN::ToASCII(
    /* [in] */ const String& input,
    /* [out] */ String* result)
{
    return ToASCII(input, 0, result);
}

ECode IDN::ToUnicode(
    /* [in] */ const String& input,
    /* [in] */ Int32 flags,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    // try {
    ECode ec = Convert(input, flags, FALSE, result);
    if (ec == E_ILLEGAL_ARGUMENT_EXCEPTION) {
        *result = input;
        return NOERROR;
    }
    return ec;
    // } catch (IllegalArgumentException ex) {
        // The RI documentation explicitly states that this method can't fail.
        // ICU4C disagrees, as does the RI in practice.
        // The RI just returns the input string if it can't
        // return s;
    // }
}

ECode IDN::ToUnicode(
    /* [in] */ const String& input,
    /* [out] */ String* result)
{
    return ToUnicode(input, 0, result);
}

ECode IDN::Convert(
    /* [in] */ const String& s,
    /* [in] */ Int32 flags,
    /* [in] */ Boolean toAscii,
    /* [out] */ String* result)
{
    if (s.IsNull()) {
        // throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    return NativeConvert(s, flags, toAscii, result);
}

static Boolean IsLabelSeparator(const UChar ch)
{
    switch (ch) {
    case 0x3002: // ideographic full stop
    case 0xff0e: // fullwidth full stop
    case 0xff61: // halfwidth ideographic full stop
        return TRUE;
    default:
        return FALSE;
    }
}

ECode IDN::NativeConvert(
    /* [in] */ const String& s,
    /* [in] */ Int32 flags,
    /* [in] */ Boolean toAscii,
    /* [out] */ String* result)
{
    UnicodeString sus = UnicodeString::fromUTF8(s.string());
    const UChar* src = sus.getBuffer();
    const size_t srcLength = sus.length();
    UChar dst[256];
    UErrorCode status = U_ZERO_ERROR;
    size_t resultLength = toAscii
            ? uidna_IDNToASCII(src, srcLength, &dst[0], sizeof(dst), flags, NULL, &status)
            : uidna_IDNToUnicode(src, srcLength, &dst[0], sizeof(dst), flags, NULL, &status);
    if (U_FAILURE(status)) {
        // jniThrowException(env, "java/lang/IllegalArgumentException", u_errorName(status));
        *result = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (!toAscii) {
        // ICU only translates separators to ASCII for toASCII.
        // Java expects the translation for toUnicode too.
        // We may as well do this here, while the string is still mutable.
        for (size_t i = 0; i < resultLength; ++i) {
            if (IsLabelSeparator(dst[i])) {
                dst[i] = '.';
            }
        }
    }
    //Todo:
    assert(0);
    // *result = String(&dst[0], resultLength);
    return NOERROR;
}
