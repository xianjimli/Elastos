
#include "cmdef.h"
#include "Int64ArrayBuffer.h"

Int64ArrayBuffer::Int64ArrayBuffer(
    /* [in] */ ArrayOf<Int64>* array)
    : Int64Buffer(array->GetLength())
    , mBackingArray(array)
    , mOffset(0)
{}

Int64ArrayBuffer::Int64ArrayBuffer(
    /* [in] */ Int32 capacity)
    : Int64Buffer(capacity)
    , mBackingArray(ArrayOf<Int64>::Alloc(capacity))
    , mOffset(0)
{}


Int64ArrayBuffer::Int64ArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Int64>* backingArray,
    /* [in] */ Int32 offset)
    : Int64Buffer(capacity)
    , mBackingArray(backingArray)
    , mOffset(offset)
{}

ECode Int64ArrayBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    if (mPosition == mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    *value = (*mBackingArray)[mOffset + mPosition++];

    return NOERROR;
}

ECode Int64ArrayBuffer::GetInt64Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    *value = (*mBackingArray)[mOffset + index];

    return NOERROR;
}

ECode Int64ArrayBuffer::GetInt64sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Int64>* dst)
{
    VALIDATE_NOT_NULL(dst);

    Int32 length = dst->GetLength();
    if ((off < 0) || (len < 0) || (Int64) off + (Int64) len > length) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    Int32 remaining;
    Remaining(&remaining);
    if (len > remaining) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
//    System.arraycopy(backingArray, offset + position, dst, off, len);
    for(Int32 i = 0; i < len; ++i) {
        (*dst)[i + off] = (*mBackingArray)[mOffset + mPosition + i];
    }
    mPosition += len;
    return NOERROR;
}

ECode Int64ArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    *isDirect = FALSE;
    return NOERROR;
}

ECode Int64ArrayBuffer::GetOrder(
   /* [out] */ ByteOrder* byteOrder)
{
    return E_NOT_IMPLEMENTED;
}
