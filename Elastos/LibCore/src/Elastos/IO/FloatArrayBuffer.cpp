
#include "cmdef.h"
#include "FloatArrayBuffer.h"

FloatArrayBuffer::FloatArrayBuffer(
    /* [in] */ ArrayOf<Float>* array)
    : FloatBuffer(array->GetLength())
    , mBackingArray(array)
    , mOffset(0)
{}

FloatArrayBuffer::FloatArrayBuffer(
    /* [in] */ Int32 capacity)
    : FloatBuffer(capacity)
    , mBackingArray(ArrayOf<Float>::Alloc(capacity))
    , mOffset(0)
{}


FloatArrayBuffer::FloatArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Float>* backingArray,
    /* [in] */ Int32 offset)
    : FloatBuffer(capacity)
    , mBackingArray(backingArray)
    , mOffset(offset)
{}

ECode FloatArrayBuffer::GetFloat(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    if (mPosition == mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    *value = (*mBackingArray)[mOffset + mPosition++];

    return NOERROR;
}

ECode FloatArrayBuffer::GetFloatEx(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    *value = (*mBackingArray)[mOffset + index];

    return NOERROR;
}

ECode FloatArrayBuffer::GetFloatsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Float>* dst)
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

ECode FloatArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    *isDirect = FALSE;
    return NOERROR;
}

ECode FloatArrayBuffer::GetOrder(
   /* [out] */ ByteOrder* byteOrder)
{
    return E_NOT_IMPLEMENTED;
}
