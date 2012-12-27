
#include "cmdef.h"
#include "DoubleArrayBuffer.h"

DoubleArrayBuffer::DoubleArrayBuffer(
    /* [in] */ ArrayOf<Double>* array)
    : DoubleBuffer(array->GetLength())
    , mBackingArray(array)
    , mOffset(0)
{}

DoubleArrayBuffer::DoubleArrayBuffer(
    /* [in] */ Int32 capacity)
    : DoubleBuffer(capacity)
    , mBackingArray(ArrayOf<Double>::Alloc(capacity))
    , mOffset(0)
{}


DoubleArrayBuffer::DoubleArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Double>* backingArray,
    /* [in] */ Int32 offset)
    : DoubleBuffer(capacity)
    , mBackingArray(backingArray)
    , mOffset(offset)
{}

ECode DoubleArrayBuffer::GetDouble(
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    if (mPosition == mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    *value = (*mBackingArray)[mOffset + mPosition++];

    return NOERROR;
}

ECode DoubleArrayBuffer::GetDoubleEx(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    *value = (*mBackingArray)[mOffset + index];

    return NOERROR;
}

ECode DoubleArrayBuffer::GetDoublesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Double>* dst)
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

ECode DoubleArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    *isDirect = FALSE;
    return NOERROR;
}

ECode DoubleArrayBuffer::GetOrder(
   /* [out] */ ByteOrder* byteOrder)
{
    return E_NOT_IMPLEMENTED;
}

String DoubleArrayBuffer::ToString()
{
    Int32 remaining;
    Remaining(&remaining);
    String result((char*)mBackingArray->GetPayload());
    return result.Substring(mOffset + mPosition, remaining);
}
