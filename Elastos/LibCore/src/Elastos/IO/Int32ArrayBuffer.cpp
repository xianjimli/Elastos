
#include "cmdef.h"
#include "Int32ArrayBuffer.h"

Int32ArrayBuffer::Int32ArrayBuffer(
    /* [in] */ ArrayOf<Int32>* array)
    : Int32Buffer(array->GetLength())
    , mBackingArray(array)
    , mOffset(0)
{}

Int32ArrayBuffer::Int32ArrayBuffer(
    /* [in] */ Int32 capacity)
    : Int32Buffer(capacity)
    , mBackingArray(ArrayOf<Int32>::Alloc(capacity))
    , mOffset(0)
{}


Int32ArrayBuffer::Int32ArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Int32>* backingArray,
    /* [in] */ Int32 offset)
    : Int32Buffer(capacity)
    , mBackingArray(backingArray)
    , mOffset(offset)
{}

ECode Int32ArrayBuffer::GetInt32(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    if (mPosition == mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    *value = (*mBackingArray)[mOffset + mPosition++];

    return NOERROR;
}

ECode Int32ArrayBuffer::GetInt32Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    *value = (*mBackingArray)[mOffset + index];

    return NOERROR;
}

ECode Int32ArrayBuffer::GetInt32sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Int32>* dst)
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

ECode Int32ArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    *isDirect = FALSE;
    return NOERROR;
}

ECode Int32ArrayBuffer::GetOrder(
   /* [out] */ ByteOrder* byteOrder)
{
    return E_NOT_IMPLEMENTED;
}
