
#include "CModifiedUtf8.h"
#include <cmdef.h>

ECode CModifiedUtf8::Decode(
    /* [in] */ const ArrayOf<Byte>& bytes,
    /* [out, callee] */ ArrayOf<Char8>** outChars,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 utfSize,
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(outChars);
    VALIDATE_NOT_NULL(string);
    if (utfSize <= 0) return E_ILLEGAL_ARGUMENT_EXCEPTION;

    Int32 count = 0;
    Int32 s = 0;
    Int32 a;

    ArrayOf<Char8>* out = ArrayOf<Char8>::Alloc(utfSize);

    while (count < utfSize) {
        (*out)[s] = bytes[offset + count++];
        if ((*out)[s] < 0x0080) {
            s++;
        } else if ((a = (*out)[s] & 0xe0) == 0xc0) {
            if (count >= utfSize) {
                // throw new UTFDataFormatException("bad second byte at " + count);
                return E_UTF_DATA_FORMAT_EXCEPTION;
            }
            Int32 b = bytes[offset + count++];
            if ((b & 0xC0) != 0x80) {
                // throw new UTFDataFormatException("bad second byte at " + (count - 1));
                return E_UTF_DATA_FORMAT_EXCEPTION;
            }
            (*out)[s++] = (((a & 0x1F) << 6) | (b & 0x3F));
        } else if ((a & 0xf0) == 0xe0) {
            if (count + 1 >= utfSize) {
                // throw new UTFDataFormatException("bad third byte at " + (count + 1));
                return E_UTF_DATA_FORMAT_EXCEPTION;
            }
            Int32 b = bytes[offset + count++];
            Int32 c = bytes[offset + count++];
            if (((b & 0xC0) != 0x80) || ((c & 0xC0) != 0x80)) {
                // throw new UTFDataFormatException("bad second or third byte at " + (count - 2));
                return E_UTF_DATA_FORMAT_EXCEPTION;
            }
            (*out)[s++] = (((a & 0x0F) << 12) | ((b & 0x3F) << 6) | (c & 0x3F));
        } else {
            // throw new UTFDataFormatException("bad byte at " + (count - 1));
            return E_UTF_DATA_FORMAT_EXCEPTION;
        }
    }

    *outChars = out;
    *string = String(out->GetPayload(), s);
    return NOERROR;
}
