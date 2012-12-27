
#include "cmdef.h"
#include "HeapByteBuffer.h"

// HeapByteBuffer::HeapByteBuffer(
//     /* [in] */ ArrayOf<Byte>* backingArray)
// {
//     this(backingArray, backingArray.length, 0);
// }

// HeapByteBuffer::HeapByteBuffer(
//     /* [in] */ Int32 capacity)
// {
//     this(new byte[capacity], capacity, 0);
// }

HeapByteBuffer::HeapByteBuffer(
    /* [in] */ ArrayOf<Byte>* backingArray,
    /* [in] */ Int32 capacity,
    /* [in] */ Int32 offset)
    : BaseByteBuffer(capacity)
    , mBackingArray(backingArray)
    , mOffset(offset)
{
    assert(offset + capacity < backingArray->GetLength());
}

ECode HeapByteBuffer::GetBytesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Byte>* dst)
{
    VALIDATE_NOT_NULL(dst);

    Int32 length = dst->GetLength();
    if (off < 0 || len < 0 || (Int64) off + (Int64) len > length) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    Int32 remaining;
    Remaining(&remaining);
    if (len > remaining) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
//    System.arraycopy(mBackingArray, offset + position, dst, off, len);
    for(Int32 i = 0; i < len; ++i) {
        (*dst)[i + off] = (*mBackingArray)[mOffset + mPosition + i];
    }
    mPosition += len;

    return E_NOT_IMPLEMENTED;
}

ECode HeapByteBuffer::GetByte(
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);
    if (mPosition == mOffset) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    *value = (*mBackingArray)[mOffset + mPosition++];

    return NOERROR;
}

ECode HeapByteBuffer::GetByteEx(
    /* [in] */ Int32 index,
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    *value = (*mBackingArray)[mOffset + index];

    return NOERROR;
}

ECode HeapByteBuffer::GetDouble(
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    Int64 val;
    GetInt64(&val);
//    return Double.longBitsToDouble(val);

    return E_NOT_IMPLEMENTED;
}

ECode HeapByteBuffer::GetDoubleEx(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    Int64 val;
    GetInt64Ex(index,&val);
//    return Double.longBitsToDouble(val);

    return E_NOT_IMPLEMENTED;
}

ECode HeapByteBuffer::GetFloat(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 val;
    GetInt32(&val);
//    return Float.intBitsToFloat(val);

    return E_NOT_IMPLEMENTED;
}

ECode HeapByteBuffer::GetFloatEx(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 val;
    GetInt32Ex(index, &val);
//   return Float.intBitsToFloat(val);

    return E_NOT_IMPLEMENTED;
}

ECode HeapByteBuffer::GetInt32(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 newPosition = mPosition + 4;
    if (newPosition > mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    *value = LoadInt32(mPosition);
    mPosition = newPosition;

    return NOERROR;
}

ECode HeapByteBuffer::GetInt32Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    if (index < 0 || index + 4 > mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    *value = LoadInt32(index);

    return NOERROR;
}

ECode HeapByteBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 newPosition = mPosition + 8;
    if (newPosition > mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    *value = LoadInt64(mPosition);
    mPosition = newPosition;

    return NOERROR;
}

ECode HeapByteBuffer::GetInt64Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    if (index < 0 || index + 8 > mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    *value = LoadInt64(index);

    return NOERROR;
}

ECode HeapByteBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 newPosition = mPosition + 2;
    if (newPosition > mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    *value = LoadInt16(mPosition);
    mPosition = newPosition;

    return NOERROR;
}

ECode HeapByteBuffer::GetInt16Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    if (index < 0 || index + 2 > mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    *value = LoadInt16(index);

    return NOERROR;
}

ECode HeapByteBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    *isDirect = FALSE;
    return NOERROR;
}

Int32 HeapByteBuffer::LoadInt32(
    /* [in] */ Int32 index)
{
    Int32 baseOffset = mOffset + index;
    Int32 bytes = 0;
    if (mOrder == ByteOrder_BIG_ENDIAN) {
        for (Int32 i = 0; i < 4; i++) {
            bytes = bytes << 8;
            bytes = bytes | ((*mBackingArray)[baseOffset + i] & 0xFF);
        }
    }
    else {
        for (Int32 i = 3; i >= 0; i--) {
            bytes = bytes << 8;
            bytes = bytes | ((*mBackingArray)[baseOffset + i] & 0xFF);
        }
    }
    return bytes;
}

Int64 HeapByteBuffer::LoadInt64(
    /* [in] */ Int32 index)
{
    Int32 baseOffset = mOffset + index;
    Int64 bytes = 0;
    if (mOrder == ByteOrder_BIG_ENDIAN) {
        for (Int32 i = 0; i < 8; i++) {
            bytes = bytes << 8;
            bytes = bytes | ((*mBackingArray)[baseOffset + i] & 0xFF);
        }
    }
    else {
        for (Int32 i = 7; i >= 0; i--) {
            bytes = bytes << 8;
            bytes = bytes | ((*mBackingArray)[baseOffset + i] & 0xFF);
        }
    }
    return bytes;
}

Int16 HeapByteBuffer::LoadInt16(
    /* [in] */ Int32 index)
{
    Int32 baseOffset = mOffset + index;
    Int16 bytes = 0;
    if (mOrder == ByteOrder_BIG_ENDIAN) {
        bytes = (Int16)((*mBackingArray)[baseOffset] << 8);
        bytes |= ((*mBackingArray)[baseOffset + 1] & 0xFF);
    }
    else {
        bytes = (Int16)((*mBackingArray)[baseOffset + 1] << 8);
        bytes |= ((*mBackingArray)[baseOffset] & 0xFF);
    }
    return bytes;
}

void HeapByteBuffer::Store(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    Int32 baseOffset = mOffset + index;
    if (mOrder == ByteOrder_BIG_ENDIAN) {
        for (Int32 i = 3; i >= 0; i--) {
            (*mBackingArray)[baseOffset + i] = (Byte)(value & 0xFF);
            value = value >> 8;
        }
    }
    else {
        for (Int32 i = 0; i <= 3; i++) {
            (*mBackingArray)[baseOffset + i] = (Byte) (value & 0xFF);
            value = value >> 8;
        }
    }
}

void HeapByteBuffer::StoreEx(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    Int32 baseOffset = mOffset + index;
    if (mOrder == ByteOrder_BIG_ENDIAN) {
        for (Int32 i = 7; i >= 0; i--) {
            (*mBackingArray)[baseOffset + i] = (Byte)(value & 0xFF);
            value = value >> 8;
        }
    }
    else {
        for (Int32 i = 0; i <= 7; i++) {
            (*mBackingArray)[baseOffset + i] = (Byte)(value & 0xFF);
            value = value >> 8;
        }
    }
}

void HeapByteBuffer::StoreEx2(
    /* [in] */ Int32 index,
    /* [in] */ Int16 value)
{
    Int32 baseOffset = mOffset + index;
    if (mOrder == ByteOrder_BIG_ENDIAN) {
        (*mBackingArray)[baseOffset] = (Byte)((value >> 8) & 0xFF);
        (*mBackingArray)[baseOffset + 1] = (Byte)(value & 0xFF);
    }
    else {
        (*mBackingArray)[baseOffset + 1] = (Byte)((value >> 8) & 0xFF);
        (*mBackingArray)[baseOffset] = (Byte)(value & 0xFF);
    }
}
