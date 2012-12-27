
#include "cmdef.h"
#include "ReadWriteFloatArrayBuffer.h"
#include "ReadOnlyFloatArrayBuffer.h"

PInterface ReadWriteFloatArrayBuffer::Probe(
        /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IFloatBuffer) {
        return (IFloatBuffer*)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_FloatBuffer) {
        return reinterpret_cast<PInterface>((FloatBuffer*)this);
    }

    return NULL;
}


UInt32 ReadWriteFloatArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadWriteFloatArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadWriteFloatArrayBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ReadWriteFloatArrayBuffer* ReadWriteFloatArrayBuffer::Copy(
    /* [in] */ FloatArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    Int32 capacity;
    other->Capacity(&capacity);
    ReadWriteFloatArrayBuffer* buf = new ReadWriteFloatArrayBuffer(capacity,
            other->mBackingArray, other->mOffset);
    other->GetLimit(&buf->mLimit);
    other->GetPosition(&buf->mPosition);
    buf->mMark = markOfOther;
    return buf;
}

ReadWriteFloatArrayBuffer::ReadWriteFloatArrayBuffer(
    /* [in] */ ArrayOf<Float>* array)
    : FloatArrayBuffer(array)
{}

ReadWriteFloatArrayBuffer::ReadWriteFloatArrayBuffer(
    /* [in] */ Int32 capacity)
    : FloatArrayBuffer(capacity)
{}

ReadWriteFloatArrayBuffer::ReadWriteFloatArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Float>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : FloatArrayBuffer(capacity, backingArray, arrayOffset)
{}

ECode ReadWriteFloatArrayBuffer::Array(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    return FloatArrayBuffer::Array(array);
}

ECode ReadWriteFloatArrayBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return FloatArrayBuffer::ArrayOffset(offset);
}

ECode ReadWriteFloatArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadOnlyFloatArrayBuffer* buf = ReadOnlyFloatArrayBuffer::Copy(
            (FloatArrayBuffer*)this, mMark);
    if (buf->Probe(EIID_IFloatBuffer) != NULL) {
        *buffer = (IFloatBuffer*)buf->Probe(EIID_IFloatBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadWriteFloatArrayBuffer::Compact()
{
//    System.arraycopy(backingArray, position + offset, backingArray, offset,
//            remaining());
    Int32 remaining;
    Remaining(&remaining);
    for(Int32 i = 0; i < remaining; ++i) {
        (*mBackingArray)[i + mOffset] = (*mBackingArray)[mOffset + mPosition + i];
    }
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = Buffer_UNSET_MARK;
    return NOERROR;
}

ECode ReadWriteFloatArrayBuffer::CompareTo(
    /* [in] */ IFloatBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return FloatArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadWriteFloatArrayBuffer::Duplicate(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadWriteFloatArrayBuffer* buf = Copy((FloatArrayBuffer*)this, mMark);
    if (buf->Probe(EIID_IFloatBuffer) != NULL) {
        *buffer = (IFloatBuffer*)buf->Probe(EIID_IFloatBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadWriteFloatArrayBuffer::GetFloat(
    /* [out] */ Float* value)
{
    return FloatArrayBuffer::GetFloat(value);
}

ECode ReadWriteFloatArrayBuffer::GetFloatEx(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    return FloatArrayBuffer::GetFloatEx(index, value);
}

ECode ReadWriteFloatArrayBuffer::GetFloats(
    /* [out] */ ArrayOf<Float>* dst)
{
    return FloatArrayBuffer::GetFloats(dst);
}

ECode ReadWriteFloatArrayBuffer::GetFloatsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Float>* dst)
{
    return FloatArrayBuffer::GetFloatsEx(off, len, dst);
}

ECode ReadWriteFloatArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return FloatArrayBuffer::GetOrder(order);
}

ECode ReadWriteFloatArrayBuffer::PutFloat(
    /* [in] */ Float c)
{
    if (mPosition == mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    (*mBackingArray)[mOffset + mPosition++] = c;
    return NOERROR;
}

ECode ReadWriteFloatArrayBuffer::PutFloatEx(
    /* [in] */ Int32 index,
    /* [in] */ Float c)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    (*mBackingArray)[mOffset + index] = c;
    return NOERROR;
}

ECode ReadWriteFloatArrayBuffer::PutFloats(
    /* [in] */ const ArrayOf<Float>& src)
{
    return FloatArrayBuffer::PutFloats(src);
}

ECode ReadWriteFloatArrayBuffer::PutFloatsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Float>& src)
{
    Int32 length = src.GetLength();
    if (off < 0 || len < 0 || (Int64)len + (Int64)off > length) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    Int32 remaining;
    Remaining(&remaining);
    if (len > remaining) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
//    System.arraycopy(src, off, backingArray, offset + position, len);
    for(Int32 i = 0; i < len; ++i) {
        (*mBackingArray)[i + mOffset + mPosition] = src[off + i];
    }
    mPosition += len;
    return NOERROR;
}

ECode ReadWriteFloatArrayBuffer::PutFloatBuffer(
    /* [in] */ IFloatBuffer* src)
{
    return FloatArrayBuffer::PutFloatBuffer(src);
}

ECode ReadWriteFloatArrayBuffer::Slice(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining;
    Remaining(&remaining);
    ReadWriteFloatArrayBuffer* buf = new ReadWriteFloatArrayBuffer(remaining, mBackingArray,
            mOffset + mPosition);
    if (buf->Probe(EIID_IFloatBuffer) != NULL) {
        *buffer = (IFloatBuffer*)buf->Probe(EIID_IFloatBuffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

ECode ReadWriteFloatArrayBuffer::Capacity(
    /* [out] */ Int32* cap)
{
    return FloatArrayBuffer::Capacity(cap);
}

ECode ReadWriteFloatArrayBuffer::Clear()
{
    return FloatArrayBuffer::Clear();
}

ECode ReadWriteFloatArrayBuffer::Flip()
{
    return FloatArrayBuffer::Flip();
}

ECode ReadWriteFloatArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return FloatArrayBuffer::HasArray(hasArray);
}

ECode ReadWriteFloatArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return FloatArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadWriteFloatArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return FloatArrayBuffer::IsDirect(isDirect);
}

ECode ReadWriteFloatArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = FALSE;
    return NOERROR;
}

CARAPI ReadWriteFloatArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = mBackingArray;
    return NOERROR;
}

CARAPI ReadWriteFloatArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = mOffset;
    return NOERROR;
}

CARAPI ReadWriteFloatArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

ECode ReadWriteFloatArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return FloatArrayBuffer::GetLimit(limit);
}

ECode ReadWriteFloatArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return FloatArrayBuffer::SetLimit(newLimit);
}

ECode ReadWriteFloatArrayBuffer::Mark()
{
    return FloatArrayBuffer::Mark();
}

ECode ReadWriteFloatArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return FloatArrayBuffer::GetPosition(pos);
}

ECode ReadWriteFloatArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return FloatArrayBuffer::SetPosition(newPosition);
}

ECode ReadWriteFloatArrayBuffer::Remaining(
    /* [out] */ Int32* remaining)
{
    return FloatArrayBuffer::Remaining(remaining);
}

ECode ReadWriteFloatArrayBuffer::Reset()
{
    return FloatArrayBuffer::Reset();
}

ECode ReadWriteFloatArrayBuffer::Rewind()
{
    return FloatArrayBuffer::Rewind();
}
