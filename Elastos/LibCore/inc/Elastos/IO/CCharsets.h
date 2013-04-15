
#ifndef __CCHARSETS_H__
#define __CCHARSETS_H__

#include "_CCharsets.h"
#include <elastos/AutoPtr.h>

CarClass(CCharsets)
{
private:
    /**
     * Approximates java.lang.UnsafeByteSequence so we don't have to pay the cost of calling back into
     * Java when converting a char[] to a UTF-8 byte[]. This lets us have UTF-8 conversions slightly
     * faster than ICU for large char[]s without paying for the NIO overhead with small char[]s.
     *
     * We could avoid this by keeping the UTF-8 bytes on the native heap until we're done and only
     * creating a byte[] on the Java heap when we know how big it needs to be, but one shouldn't lie
     * to the garbage collector (nor hide potentially large allocations from it).
     *
     * Because a call to append might require an allocation, it might fail. Callers should always
     * check the return value of append.
     */
    class NativeUnsafeByteSequence {
    public:
        NativeUnsafeByteSequence();

        ~NativeUnsafeByteSequence();

        Boolean Append(
            /* [in] */ Byte b);

        Boolean Resize(
            /* [in] */ Int32 newSize);

        ArrayOf<Byte>* ToByteArray();

    private:
        // Disallow copy and assignment.
        NativeUnsafeByteSequence(const NativeUnsafeByteSequence& sequence);
        void operator=(const NativeUnsafeByteSequence&);

    private:
        ArrayOf<Byte>* mRawArray;
        Int32 mSize;
        Int32 mOffset;
    };

public:
    CARAPI GetISO_8859_1(
        /* [out] */ ICharset** charset);

    CARAPI GetUS_ASCII(
        /* [out] */ ICharset** charset);

    CARAPI GetUTF_8(
        /* [out] */ ICharset** charset);

    CARAPI ToAsciiBytes(
        /* [in] */ const ArrayOf<Char8>& chars,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out, callee] */ ArrayOf<Byte>** bytes);

    CARAPI ToIsoLatin1Bytes(
        /* [in] */ const ArrayOf<Char8>& chars,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out, callee] */ ArrayOf<Byte>** bytes);

    CARAPI ToUtf8Bytes(
        /* [in] */ const ArrayOf<Char8>& chars,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out, callee] */ ArrayOf<Byte>** bytes);

    CARAPI AsciiBytesToChars(
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out, callee] */ ArrayOf<Char8>** chars);

    CARAPI IsoLatin1BytesToChars(
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out, callee] */ ArrayOf<Char8>** chars);

private:
    /**
     * Translates the given characters to US-ASCII or ISO-8859-1 bytes, using the fact that
     * Unicode code points between U+0000 and U+007f inclusive are identical to US-ASCII, while
     * U+0000 to U+00ff inclusive are identical to ISO-8859-1.
     */
    static CARAPI CharsToBytes(
        /* [in] */ const ArrayOf<Char8>& chars,
        /* [in] */ const Int32 offset,
        /* [in] */ const Int32 length,
        /* [in] */ const Char8 maxValidChar,
        /* [out, callee] */ ArrayOf<Byte>** bytes);

public:
    static const AutoPtr<ICharset> ISO_8859_1;

    static const AutoPtr<ICharset> US_ASCII;

    static const AutoPtr<ICharset> UTF_8;
};

#endif // __CCHARSETS_H__
