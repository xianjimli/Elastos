
#include "cmdef.h"
#include "Int16ArrayBuffer.h"

Int16ArrayBuffer::Int16ArrayBuffer(
    /* [in] */ ArrayOf<Int16>* array)
    : Int16Buffer(array->GetLength())
    , mBackingArray(array)
    , mOffset(0)
{}

Int16ArrayBuffer::Int16ArrayBuffer(
    /* [in] */ Int32 capacity)
    : Int16Buffer(capacity)
    , mBackingArray(ArrayOf<Int16>::Alloc(capacity))
    , mOffset(0)
{}


Int16ArrayBuffer::Int16ArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Int16>* backingArray,
    /* [in] */ Int32 offset)
    : Int16Buffer(capacity)
    , mBackingArray(backingArray)
    , mOffset(offset)
{}

ECode Int16ArrayBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    if (mPosition == mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    *value = (*mBackingArray)[mOffset + mPosition++];

    return NOERROR;
}

ECode Int16ArrayBuffer::GetInt16Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    *value = (*mBackingArray)[mOffset + index];

    return NOERROR;
}

ECode Int16ArrayBuffer::GetInt16sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Int16>* dst)
{
    VALIDATE_NOT_NULL(dst);

    Int32 length = dst->GetLength();
    if ((off < 0) || (len < 0) || (Int16) off + (Int16) len > length) {
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

ECode Int16ArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    *isDirect = FALSE;
    return NOERROR;
}

ECode Int16ArrayBuffer::GetOrder(
   /* [out] */ ByteOrder* byteOrder)
{
    return E_NOT_IMPLEMENTED;
}
