
#include "CCharsets.h"
#include "Charset.h"
#include "CCharsetHelper.h"
#include <cmdef.h>

#define U_IS_SURROGATE(c) (((c)&0xfffff800)==0xd800)
#define U16_IS_SURROGATE(c) U_IS_SURROGATE(c)
#define U16_IS_SURROGATE_LEAD(c) (((c)&0x400)==0)
#define U16_IS_SURROGATE_TRAIL(c) (((c)&0x400)!=0)
#define U16_SURROGATE_OFFSET ((0xd800<<10UL)+0xdc00-0x10000)
#define U16_GET_SUPPLEMENTARY(lead, trail) \
    (((Char32)(lead)<<10UL)+(Char32)(trail)-U16_SURROGATE_OFFSET)

AutoPtr<ICharset> CreateCharSet(
    /* [in] */ String name)
{
    AutoPtr<ICharset> charset;
    Charset::ForName(name, (ICharset **)&charset);
    return charset;
}

const AutoPtr<ICharset> CCharsets::ISO_8859_1 = CreateCharSet(String("ISO-8859-1"));
const AutoPtr<ICharset> CCharsets::US_ASCII = CreateCharSet(String("US-ASCII"));
const AutoPtr<ICharset> CCharsets::UTF_8 = CreateCharSet(String("UTF-8"));

CCharsets::NativeUnsafeByteSequence::NativeUnsafeByteSequence()
    : mRawArray(NULL), mSize(-1), mOffset(0)
{}

CCharsets::NativeUnsafeByteSequence::~NativeUnsafeByteSequence()
{
    if (mRawArray != NULL) {
        ArrayOf<Byte>::Free(mRawArray);
    }
}

Boolean CCharsets::NativeUnsafeByteSequence::Append(Byte b)
{
    if (mOffset == mSize && !Resize(mSize * 2)) {
        return TRUE;
    }
    (*mRawArray)[mOffset++] = b;
    return TRUE;
}

Boolean CCharsets::NativeUnsafeByteSequence::Resize(Int32 newSize) {
    if (newSize == mSize) {
        return TRUE;
    }

    // Allocate a new array.
    ArrayOf<Byte>* newarray = ArrayOf<Byte>::Alloc(newSize);
    if (newarray == NULL) {
        return FALSE;
    }

    // Copy data out of the old array and then let go of it.
    // Note that we may be trimming the array.
    if (mRawArray != NULL) {
        memcpy(newarray->GetPayload(), mRawArray->GetPayload(), mOffset);
        ArrayOf<Byte>::Free(mRawArray);
    }

    // Point ourselves at the new array.
    mRawArray = newarray;
    mSize = newSize;
    return TRUE;
}

ArrayOf<Byte>* CCharsets::NativeUnsafeByteSequence::ToByteArray() {
    // Trim any unused space, if necessary.
    Boolean okay = Resize(mOffset);
    return okay ? mRawArray : NULL;
}

ECode CCharsets::GetISO_8859_1(
    /* [out] */ ICharset** charset)
{
    VALIDATE_NOT_NULL(charset);
    *charset = ISO_8859_1;
    return NOERROR;
}

ECode CCharsets::GetUS_ASCII(
    /* [out] */ ICharset** charset)
{
    VALIDATE_NOT_NULL(charset);
    *charset = US_ASCII;
    return NOERROR;
}

ECode CCharsets::GetUTF_8(
    /* [out] */ ICharset** charset)
{
    VALIDATE_NOT_NULL(charset);
    *charset = UTF_8;
    return NOERROR;
}

ECode CCharsets::ToAsciiBytes(
    /* [in] */ const ArrayOf<Char8>& chars,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out, callee] */ ArrayOf<Byte>** bytes)
{
    VALIDATE_NOT_NULL(bytes);
    return CCharsets::CharsToBytes(chars, offset, length, 0x7f, bytes);
}

ECode CCharsets::ToIsoLatin1Bytes(
    /* [in] */ const ArrayOf<Char8>& chars,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out, callee] */ ArrayOf<Byte>** bytes)
{
    VALIDATE_NOT_NULL(bytes);
    return CCharsets::CharsToBytes(chars, offset, length, 0xff, bytes);
}

ECode CCharsets::ToUtf8Bytes(
    /* [in] */ const ArrayOf<Char8>& chars,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out, callee] */ ArrayOf<Byte>** bytes)
{
    VALIDATE_NOT_NULL(bytes);
    if (length <= 0) return E_ILLEGAL_ARGUMENT_EXCEPTION;

    ECode result = NOERROR;
    const Int32 end = offset + length;

    NativeUnsafeByteSequence* out = new NativeUnsafeByteSequence();
    if (!out->Resize(length)) {
        result = E_RUNTIME_EXCEPTION;
        goto Exit;
    }

    for (Int32 i = offset; i < end; ++i) {
        Char8 ch = chars[i];
        if (ch < 0x80) {
            // One byte.
            if (!out->Append(ch)) {
                result = E_RUNTIME_EXCEPTION;
                goto Exit;
            }
        } else if (ch < 0x800) {
            // Two bytes.
            if (!out->Append((ch >> 6) | 0xc0) || !out->Append((ch & 0x3f) | 0x80)) {
                result = E_RUNTIME_EXCEPTION;
                goto Exit;
            }
        } else if (U16_IS_SURROGATE(ch)) {
            // A supplementary character.
            Char8 high = ch;
            Char8 low = (i + 1 != end) ? chars[i + 1] : 0;
            if (!U16_IS_SURROGATE_LEAD(high) || !U16_IS_SURROGATE_TRAIL(low)) {
                if (!out->Append('?')) {
                    result = E_RUNTIME_EXCEPTION;
                    goto Exit;
                }
                continue;
            }
            // Now we know we have a *valid* surrogate pair, we can consume the low surrogate.
            ++i;
            ch = U16_GET_SUPPLEMENTARY(high, low);
            // Four bytes.
            Byte b1 = (ch >> 18) | 0xf0;
            Byte b2 = ((ch >> 12) & 0x3f) | 0x80;
            Byte b3 = ((ch >> 6) & 0x3f) | 0x80;
            Byte b4 = (ch & 0x3f) | 0x80;
            if (!out->Append(b1) || !out->Append(b2) || !out->Append(b3) || !out->Append(b4)) {
                result = E_RUNTIME_EXCEPTION;
                goto Exit;
            }
        } else {
            // Three bytes.
            Byte b1 = (ch >> 12) | 0xe0;
            Byte b2 = ((ch >> 6) & 0x3f) | 0x80;
            Byte b3 = (ch & 0x3f) | 0x80;
            if (!out->Append(b1) || !out->Append(b2) || !out->Append(b3)) {
                result = E_RUNTIME_EXCEPTION;
                goto Exit;
            }
        }
    }

Exit:
    if (out != NULL)
    {
        delete out;
        out = NULL;
    }
    return NOERROR;
}

ECode CCharsets::AsciiBytesToChars(
    /* [in] */ const ArrayOf<Byte>& bytes,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out, callee] */ ArrayOf<Char8>** chars)
{
    VALIDATE_NOT_NULL(chars);

    *chars = ArrayOf<Char8>::Alloc(length);

    const Byte* src = &bytes[offset];
    Char8* dst = (*chars)->GetPayload();
    static const Char16 REPLACEMENT_CHAR = 0xfffd;
    for (int i = length - 1; i >= 0; --i) {
        Char8 ch = static_cast<Char8>(*src++ & 0xff);
        *dst++ = (ch <= 0x7f) ? ch : REPLACEMENT_CHAR;
    }
    return NOERROR;
}

ECode CCharsets::IsoLatin1BytesToChars(
    /* [in] */ const ArrayOf<Byte>& bytes,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out, callee] */ ArrayOf<Char8>** chars)
{
    VALIDATE_NOT_NULL(chars);

    *chars = ArrayOf<Char8>::Alloc(length);

    const Byte* src = &bytes[offset];
    Char8* dst = (*chars)->GetPayload();

    for (int i = length - 1; i >= 0; --i) {
        *dst++ = static_cast<Char8>(*src++ & 0xff);
    }
    return NOERROR;
}

ECode CCharsets::CharsToBytes(
    /* [in] */ const ArrayOf<Char8>& chars,
    /* [in] */ const Int32 offset,
    /* [in] */ const Int32 length,
    /* [in] */ const Char8 maxValidChar,
    /* [out, callee] */ ArrayOf<Byte>** bytes)
{
    VALIDATE_NOT_NULL(bytes);

    *bytes = ArrayOf<Byte>::Alloc(length);

    const Char8* src = &chars[offset];
    Byte* dst = (*bytes)->GetPayload();
    for (int i = length - 1; i >= 0; --i) {
        Char8 ch = *src++;
        if (ch > maxValidChar) {
            ch = '?';
        }
        *dst++ = static_cast<Byte>(ch);
    }
    return NOERROR;
}
