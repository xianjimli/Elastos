#include "cmdef.h"
#include "DataInputStream.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

DataInputStream::DataInputStream()
    : mBuff(NULL)
{
}

DataInputStream::~DataInputStream()
{
    if (mBuff != NULL) {
        ArrayOf<Byte>::Free(mBuff);
    }
}

ECode DataInputStream::Init(
    /* [in] */ IInputStream* in)
{
    FAIL_RETURN(FilterInputStream::Init(in));

    mBuff = ArrayOf<Byte>::Alloc(8);
    if (mBuff == NULL) return E_OUT_OF_MEMORY_ERROR;
    return NOERROR;
}

ECode DataInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);
    return mIn->ReadBufferEx(0, buffer->GetLength(), buffer, number);
}

ECode DataInputStream::ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);

    return mIn->ReadBufferEx(offset, length, buffer, number);
}

ECode DataInputStream::ReadBoolean(
    /* [out] */ Boolean* value)
{
    assert(value != NULL);

    Int32 temp;
    FAIL_RETURN(mIn->Read(&temp));
    if (temp < 0) {
//        throw new EOFException();
        return E_EOF_EXCEPTION;
    }
    *value = temp != 0;
    return NOERROR;
}

ECode DataInputStream::ReadByte(
    /* [out] */ Byte* value)
{
    assert(value != NULL);

    Int32 temp;
    FAIL_RETURN(mIn->Read(&temp));
    if (temp < 0) {
//        throw new EOFException();
        return E_EOF_EXCEPTION;
    }
    *value = (Byte)temp;
    return NOERROR;
}

ECode DataInputStream::ReadChar(
    /* [out] */ Char32* value)
{
    assert(value != NULL);

    Int32 number;
    FAIL_RETURN(ReadToBuff(4, &number));
    if (number < 0) {
//        throw new EOFException();
        return E_EOF_EXCEPTION;
    }
    *value = String((const char*)mBuff->GetPayload()).GetChar(0);

    return NOERROR;
}

ECode DataInputStream::ReadToBuff(
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    assert(number != NULL);

    Int32 offset = 0;
    while(offset < count) {
        FAIL_RETURN(ReadBufferEx(offset, count - offset, mBuff, number));
        if(*number == -1) return NOERROR;
        offset += *number;
    }

    return NOERROR;
}

ECode DataInputStream::ReadDouble(
    /* [out] */ Double* value)
{
    assert(value != NULL);

    Int64 int64Value;
    FAIL_RETURN(ReadInt64(&int64Value));
    *value = Math::Int64BitsToDouble(int64Value);
    return NOERROR;
}

ECode DataInputStream::ReadFloat(
    /* [out] */ Float* value)
{
    assert(value != NULL);

    Int32 int32Value;
    FAIL_RETURN(ReadInt32(&int32Value));
    *value = Math::Int32BitsToFloat(int32Value);
    return NOERROR;
}

ECode DataInputStream::ReadFully(
    /* [out] */ ArrayOf<byte> * buffer)
{
    return ReadFullyEx(0, buffer->GetLength(), buffer);
}

ECode DataInputStream::ReadFullyEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<byte>* buffer)
{
    // BEGIN android-removed
    // if (length < 0) {
    //     throw new IndexOutOfBoundsException();
    // }
    // END android-removed
    if(length == 0) return NOERROR;

    if ((mIn == NULL) || (buffer == NULL)) {
        return E_NULL_POINTER_EXCEPTION;
    }

    // BEGIN android-changed
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // used (offset | length) < 0 instead of separate (offset < 0) and
    // (length < 0) check to safe one operation
    if ((offset | length) < 0 || offset > buffer->GetLength() - length) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed
    Int32 number;
    while (length > 0) {
        FAIL_RETURN(mIn->ReadBufferEx(offset, length, buffer, &number));
        if (number < 0) return E_EOF_EXCEPTION;
        offset += number;
        length -= number;
    }
    return NOERROR;
}

ECode DataInputStream::ReadInt32(
    /* [out] */ Int32* value)
{
    assert(value != NULL);

    Int32 number;
    FAIL_RETURN(ReadToBuff(4, &number));
    if (number < 0) return E_EOF_EXCEPTION;
    *value = (((*mBuff)[0] & 0xff) << 24) | (((*mBuff)[1] & 0xff) << 16) |
             (((*mBuff)[2] & 0xff) << 8) | ((*mBuff)[3] & 0xff);

    return NOERROR;
}

ECode DataInputStream::ReadLine(
         /* [out] */ String* str)
{
    assert(str != NULL);

    StringBuf* line = StringBuf::Alloc(80);
    Boolean foundTerminator = FALSE;

    while (TRUE) {
        Int32 nextByte;
        FAIL_RETURN(mIn->Read(&nextByte));
        switch (nextByte) {
            case -1:
                if (line->GetLength() == 0 && !foundTerminator) {
                    *str = NULL;
                    StringBuf::Free(line);
                    return NOERROR;
                }
                *str = line->GetPayload();
                StringBuf::Free(line);
                return NOERROR;
            case (Byte)'\r':
                if (foundTerminator) {
//                    ((PushbackInputStream) mIn).unread(nextByte);///////////not implemented
                    *str = line->GetPayload();
                    StringBuf::Free(line);
                    return NOERROR;
                }
                foundTerminator = TRUE;
                /* Have to be able to peek ahead one byte */ ///////////////not implemented
//                if (!(in.getClass() == PushbackInputStream.class)) {
//                    in = new PushbackInputStream(in);
//                }
                break;
            case (Byte)'\n':
                *str = line->GetPayload();
                StringBuf::Free(line);
                return NOERROR;
            default:
                if (foundTerminator) {
//                    ((PushbackInputStream) in).unread(nextByte);//////////////not implemented
                    *str = line->GetPayload();
                    StringBuf::Free(line);
                    return NOERROR;
                }
                line->Append((Char8)nextByte);
        }
    }
    return NOERROR;
}

ECode DataInputStream::ReadInt64(
    /* [out] */ Int64* value)
{
    assert(value != NULL);

    Int32 number;
    FAIL_RETURN(ReadToBuff(8, &number));
    if (number < 0) return E_EOF_EXCEPTION;
    Int32 i1 = (((*mBuff)[0] & 0xff) << 24) | (((*mBuff)[1] & 0xff) << 16) |
               (((*mBuff)[2] & 0xff) << 8) | ((*mBuff)[3] & 0xff);
    Int32 i2 = (((*mBuff)[4] & 0xff) << 24) | (((*mBuff)[5] & 0xff) << 16) |
               (((*mBuff)[6] & 0xff) << 8) | ((*mBuff)[7] & 0xff);

    *value = ((i1 & 0xffffffffLL) << 32) | (i2 & 0xffffffffLL);

    return NOERROR;
}

ECode DataInputStream::ReadInt16(
        /* [out] */ Int16* value)
{
    assert(value != NULL);

    Int32 number;

    FAIL_RETURN(ReadToBuff(2, &number));
    if (number < 0) return E_EOF_EXCEPTION;
    *value = (Int16)((((*mBuff)[0] & 0xff) << 8) | ((*mBuff)[1] & 0xff));

    return NOERROR;
}

ECode DataInputStream::ReadUnsignedByte(
    /* [out] */ Int32* value)
{
    assert(value != NULL);

    FAIL_RETURN(mIn->Read(value));
    return *value < 0? E_EOF_EXCEPTION : NOERROR;
}

ECode DataInputStream::ReadUnsignedInt16(
    /* [out] */ Int32* value)
{
    assert(value != NULL);

    Int32 number;
    FAIL_RETURN(ReadToBuff(2, &number));
    if (number < 0) return E_EOF_EXCEPTION;
    *value = (UInt16)((((*mBuff)[0] & 0xff) << 8) | ((*mBuff)[1] & 0xff));

    return NOERROR;
}

ECode DataInputStream::SkipBytes(
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    assert(number != NULL);

    Int32 skipped = 0;
    Int64 skip;

    while (skipped < count && (mIn->Skip(count - skipped, &skip), skip != 0)) {
        skipped += skip;
    }
    // BEGIN android-removed
    // if (skipped < 0) {
    //     return E_IO_EXCEPTION;//throw new EOFException();
    // }
    // END android-removed
    *number = skipped;

    return NOERROR;
}
