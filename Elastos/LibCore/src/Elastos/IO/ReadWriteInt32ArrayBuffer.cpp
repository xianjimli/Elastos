
#include "cmdef.h"
#include "ReadWriteInt32ArrayBuffer.h"
#include "ReadOnlyInt32ArrayBuffer.h"

PInterface ReadWriteInt32ArrayBuffer::Probe(
        /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IInt32Buffer) {
        return (IInt32Buffer*)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_Int32Buffer) {
        return reinterpret_cast<PInterface>((Int32Buffer*)this);
    }

    return NULL;
}


UInt32 ReadWriteInt32ArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadWriteInt32ArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadWriteInt32ArrayBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ReadWriteInt32ArrayBuffer* ReadWriteInt32ArrayBuffer::Copy(
    /* [in] */ Int32ArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    Int32 capacity;
    other->Capacity(&capacity);
    ReadWriteInt32ArrayBuffer* buf = new ReadWriteInt32ArrayBuffer(capacity,
            other->mBackingArray, other->mOffset);
    other->GetLimit(&buf->mLimit);
    other->GetPosition(&buf->mPosition);
    buf->mMark = markOfOther;
    return buf;
}

ReadWriteInt32ArrayBuffer::ReadWriteInt32ArrayBuffer(
    /* [in] */ ArrayOf<Int32>* array)
    : Int32ArrayBuffer(array)
{}

ReadWriteInt32ArrayBuffer::ReadWriteInt32ArrayBuffer(
    /* [in] */ Int32 capacity)
    : Int32ArrayBuffer(capacity)
{}

ReadWriteInt32ArrayBuffer::ReadWriteInt32ArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Int32>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : Int32ArrayBuffer(capacity, backingArray, arrayOffset)
{}

ECode ReadWriteInt32ArrayBuffer::Array(
    /* [out, callee] */ ArrayOf<Int32>** array)
{
    return Int32ArrayBuffer::Array(array);
}

ECode ReadWriteInt32ArrayBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return Int32ArrayBuffer::ArrayOffset(offset);
}

ECode ReadWriteInt32ArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadOnlyInt32ArrayBuffer* buf = ReadOnlyInt32ArrayBuffer::Copy(
            (Int32ArrayBuffer*)this, mMark);
    if (buf->Probe(EIID_IInt32Buffer) != NULL) {
        *buffer = (IInt32Buffer*)buf->Probe(EIID_IInt32Buffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadWriteInt32ArrayBuffer::Compact()
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

ECode ReadWriteInt32ArrayBuffer::CompareTo(
    /* [in] */ IInt32Buffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return Int32ArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadWriteInt32ArrayBuffer::Duplicate(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadWriteInt32ArrayBuffer* buf = Copy((Int32ArrayBuffer*)this, mMark);
    if (buf->Probe(EIID_IInt32Buffer) != NULL) {
        *buffer = (IInt32Buffer*)buf->Probe(EIID_IInt32Buffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadWriteInt32ArrayBuffer::GetInt32(
    /* [out] */ Int32* value)
{
    return Int32ArrayBuffer::GetInt32(value);
}

ECode ReadWriteInt32ArrayBuffer::GetInt32Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    return Int32ArrayBuffer::GetInt32Ex(index, value);
}

ECode ReadWriteInt32ArrayBuffer::GetInt32s(
    /* [out] */ ArrayOf<Int32>* dst)
{
    return Int32ArrayBuffer::GetInt32s(dst);
}

ECode ReadWriteInt32ArrayBuffer::GetInt32sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Int32>* dst)
{
    return Int32ArrayBuffer::GetInt32sEx(off, len, dst);
}

ECode ReadWriteInt32ArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return Int32ArrayBuffer::GetOrder(order);
}

ECode ReadWriteInt32ArrayBuffer::PutInt32(
    /* [in] */ Int32 c)
{
    if (mPosition == mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    (*mBackingArray)[mOffset + mPosition++] = c;
    return NOERROR;
}

ECode ReadWriteInt32ArrayBuffer::PutInt32Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int32 c)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    (*mBackingArray)[mOffset + index] = c;
    return NOERROR;
}

ECode ReadWriteInt32ArrayBuffer::PutInt32s(
    /* [in] */ const ArrayOf<Int32>& src)
{
    return Int32ArrayBuffer::PutInt32s(src);
}

ECode ReadWriteInt32ArrayBuffer::PutInt32sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Int32>& src)
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

ECode ReadWriteInt32ArrayBuffer::PutInt32Buffer(
    /* [in] */ IInt32Buffer* src)
{
    return Int32ArrayBuffer::PutInt32Buffer(src);
}

ECode ReadWriteInt32ArrayBuffer::Slice(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining;
    Remaining(&remaining);
    ReadWriteInt32ArrayBuffer* buf = new ReadWriteInt32ArrayBuffer(remaining, mBackingArray,
            mOffset + mPosition);
    if (buf->Probe(EIID_IInt32Buffer) != NULL) {
        *buffer = (IInt32Buffer*)buf->Probe(EIID_IInt32Buffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

ECode ReadWriteInt32ArrayBuffer::Capacity(
    /* [out] */ Int32* cap)
{
    return Int32ArrayBuffer::Capacity(cap);
}

ECode ReadWriteInt32ArrayBuffer::Clear()
{
    return Int32ArrayBuffer::Clear();
}

ECode ReadWriteInt32ArrayBuffer::Flip()
{
    return Int32ArrayBuffer::Flip();
}

ECode ReadWriteInt32ArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return Int32ArrayBuffer::HasArray(hasArray);
}

ECode ReadWriteInt32ArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return Int32ArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadWriteInt32ArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return Int32ArrayBuffer::IsDirect(isDirect);
}

ECode ReadWriteInt32ArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = FALSE;
    return NOERROR;
}

CARAPI ReadWriteInt32ArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int32>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = mBackingArray;
    return NOERROR;
}

CARAPI ReadWriteInt32ArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = mOffset;
    return NOERROR;
}

CARAPI ReadWriteInt32ArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

ECode ReadWriteInt32ArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return Int32ArrayBuffer::GetLimit(limit);
}

ECode ReadWriteInt32ArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return Int32ArrayBuffer::SetLimit(newLimit);
}

ECode ReadWriteInt32ArrayBuffer::Mark()
{
    return Int32ArrayBuffer::Mark();
}

ECode ReadWriteInt32ArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return Int32ArrayBuffer::GetPosition(pos);
}

ECode ReadWriteInt32ArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return Int32ArrayBuffer::SetPosition(newPosition);
}

ECode ReadWriteInt32ArrayBuffer::Remaining(
    /* [out] */ Int32* remaining)
{
    return Int32ArrayBuffer::Remaining(remaining);
}

ECode ReadWriteInt32ArrayBuffer::Reset()
{
    return Int32ArrayBuffer::Reset();
}

ECode ReadWriteInt32ArrayBuffer::Rewind()
{
    return Int32ArrayBuffer::Rewind();
}
