
#include "cmdef.h"
#include "ReadWriteDoubleArrayBuffer.h"
#include "ReadOnlyDoubleArrayBuffer.h"

PInterface ReadWriteDoubleArrayBuffer::Probe(
        /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IDoubleBuffer) {
        return (IDoubleBuffer*)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_DoubleBuffer) {
        return reinterpret_cast<PInterface>((DoubleBuffer*)this);
    }

    return NULL;
}


UInt32 ReadWriteDoubleArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadWriteDoubleArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadWriteDoubleArrayBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ReadWriteDoubleArrayBuffer* ReadWriteDoubleArrayBuffer::Copy(
    /* [in] */ DoubleArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    Int32 capacity;
    other->Capacity(&capacity);
    ReadWriteDoubleArrayBuffer* buf = new ReadWriteDoubleArrayBuffer(capacity,
            other->mBackingArray, other->mOffset);
    other->GetLimit(&buf->mLimit);
    other->GetPosition(&buf->mPosition);
    buf->mMark = markOfOther;
    return buf;
}

ReadWriteDoubleArrayBuffer::ReadWriteDoubleArrayBuffer(
    /* [in] */ ArrayOf<Double>* array)
    : DoubleArrayBuffer(array)
{}

ReadWriteDoubleArrayBuffer::ReadWriteDoubleArrayBuffer(
    /* [in] */ Int32 capacity)
    : DoubleArrayBuffer(capacity)
{}

ReadWriteDoubleArrayBuffer::ReadWriteDoubleArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Double>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : DoubleArrayBuffer(capacity, backingArray, arrayOffset)
{}

ECode ReadWriteDoubleArrayBuffer::Array(
    /* [out, callee] */ ArrayOf<Double>** array)
{
    return DoubleArrayBuffer::Array(array);
}

ECode ReadWriteDoubleArrayBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return DoubleArrayBuffer::ArrayOffset(offset);
}

ECode ReadWriteDoubleArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadOnlyDoubleArrayBuffer* buf = ReadOnlyDoubleArrayBuffer::Copy(
            (DoubleArrayBuffer*)this, mMark);
    if (buf->Probe(EIID_IDoubleBuffer) != NULL) {
        *buffer = (IDoubleBuffer*)buf->Probe(EIID_IDoubleBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadWriteDoubleArrayBuffer::Compact()
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

ECode ReadWriteDoubleArrayBuffer::CompareTo(
    /* [in] */ IDoubleBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return DoubleArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadWriteDoubleArrayBuffer::Duplicate(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadWriteDoubleArrayBuffer* buf = Copy((DoubleArrayBuffer*)this, mMark);
    if (buf->Probe(EIID_IDoubleBuffer) != NULL) {
        *buffer = (IDoubleBuffer*)buf->Probe(EIID_IDoubleBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadWriteDoubleArrayBuffer::GetDouble(
    /* [out] */ Double* value)
{
    return DoubleArrayBuffer::GetDouble(value);
}

ECode ReadWriteDoubleArrayBuffer::GetDoubleEx(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    return DoubleArrayBuffer::GetDoubleEx(index, value);
}

ECode ReadWriteDoubleArrayBuffer::GetDoubles(
    /* [out] */ ArrayOf<Double>* dst)
{
    return DoubleArrayBuffer::GetDoubles(dst);
}

ECode ReadWriteDoubleArrayBuffer::GetDoublesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Double>* dst)
{
    return DoubleArrayBuffer::GetDoublesEx(off, len, dst);
}

ECode ReadWriteDoubleArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return DoubleArrayBuffer::GetOrder(order);
}

ECode ReadWriteDoubleArrayBuffer::PutDouble(
    /* [in] */ Double c)
{
    if (mPosition == mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    (*mBackingArray)[mOffset + mPosition++] = c;
    return NOERROR;
}

ECode ReadWriteDoubleArrayBuffer::PutDoubleEx(
    /* [in] */ Int32 index,
    /* [in] */ Double c)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    (*mBackingArray)[mOffset + index] = c;
    return NOERROR;
}

ECode ReadWriteDoubleArrayBuffer::PutDoubles(
    /* [in] */ const ArrayOf<Double>& src)
{
    return DoubleArrayBuffer::PutDoubles(src);
}

ECode ReadWriteDoubleArrayBuffer::PutDoublesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Double>& src)
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

ECode ReadWriteDoubleArrayBuffer::PutDoubleBuffer(
    /* [in] */ IDoubleBuffer* src)
{
    return DoubleArrayBuffer::PutDoubleBuffer(src);
}

ECode ReadWriteDoubleArrayBuffer::Slice(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining;
    Remaining(&remaining);
    ReadWriteDoubleArrayBuffer* buf = new ReadWriteDoubleArrayBuffer(remaining, mBackingArray,
            mOffset + mPosition);
    if (buf->Probe(EIID_IDoubleBuffer) != NULL) {
        *buffer = (IDoubleBuffer*)buf->Probe(EIID_IDoubleBuffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

ECode ReadWriteDoubleArrayBuffer::Capacity(
    /* [out] */ Int32* cap)
{
    return DoubleArrayBuffer::Capacity(cap);
}

ECode ReadWriteDoubleArrayBuffer::Clear()
{
    return DoubleArrayBuffer::Clear();
}

ECode ReadWriteDoubleArrayBuffer::Flip()
{
    return DoubleArrayBuffer::Flip();
}

ECode ReadWriteDoubleArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return DoubleArrayBuffer::HasArray(hasArray);
}

ECode ReadWriteDoubleArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return DoubleArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadWriteDoubleArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return DoubleArrayBuffer::IsDirect(isDirect);
}

ECode ReadWriteDoubleArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = FALSE;
    return NOERROR;
}

CARAPI ReadWriteDoubleArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Double>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = mBackingArray;
    return NOERROR;
}

CARAPI ReadWriteDoubleArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = mOffset;
    return NOERROR;
}

CARAPI ReadWriteDoubleArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

ECode ReadWriteDoubleArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return DoubleArrayBuffer::GetLimit(limit);
}

ECode ReadWriteDoubleArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return DoubleArrayBuffer::SetLimit(newLimit);
}

ECode ReadWriteDoubleArrayBuffer::Mark()
{
    return DoubleArrayBuffer::Mark();
}

ECode ReadWriteDoubleArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return DoubleArrayBuffer::GetPosition(pos);
}

ECode ReadWriteDoubleArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return DoubleArrayBuffer::SetPosition(newPosition);
}

ECode ReadWriteDoubleArrayBuffer::Remaining(
    /* [out] */ Int32* remaining)
{
    return DoubleArrayBuffer::Remaining(remaining);
}

ECode ReadWriteDoubleArrayBuffer::Reset()
{
    return DoubleArrayBuffer::Reset();
}

ECode ReadWriteDoubleArrayBuffer::Rewind()
{
    return DoubleArrayBuffer::Rewind();
}
