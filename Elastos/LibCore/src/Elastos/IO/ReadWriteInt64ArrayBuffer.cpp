
#include "cmdef.h"
#include "ReadWriteInt64ArrayBuffer.h"
#include "ReadOnlyInt64ArrayBuffer.h"

PInterface ReadWriteInt64ArrayBuffer::Probe(
        /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IInt64Buffer) {
        return (IInt64Buffer*)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_Int64Buffer) {
        return reinterpret_cast<PInterface>((Int64Buffer*)this);
    }

    return NULL;
}


UInt32 ReadWriteInt64ArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadWriteInt64ArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadWriteInt64ArrayBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ReadWriteInt64ArrayBuffer* ReadWriteInt64ArrayBuffer::Copy(
    /* [in] */ Int64ArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    Int32 capacity;
    other->Capacity(&capacity);
    ReadWriteInt64ArrayBuffer* buf = new ReadWriteInt64ArrayBuffer(capacity,
            other->mBackingArray, other->mOffset);
    other->GetLimit(&buf->mLimit);
    other->GetPosition(&buf->mPosition);
    buf->mMark = markOfOther;
    return buf;
}

ReadWriteInt64ArrayBuffer::ReadWriteInt64ArrayBuffer(
    /* [in] */ ArrayOf<Int64>* array)
    : Int64ArrayBuffer(array)
{}

ReadWriteInt64ArrayBuffer::ReadWriteInt64ArrayBuffer(
    /* [in] */ Int32 capacity)
    : Int64ArrayBuffer(capacity)
{}

ReadWriteInt64ArrayBuffer::ReadWriteInt64ArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Int64>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : Int64ArrayBuffer(capacity, backingArray, arrayOffset)
{}

ECode ReadWriteInt64ArrayBuffer::Array(
    /* [out, callee] */ ArrayOf<Int64>** array)
{
    return Int64ArrayBuffer::Array(array);
}

ECode ReadWriteInt64ArrayBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return Int64ArrayBuffer::ArrayOffset(offset);
}

ECode ReadWriteInt64ArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadOnlyInt64ArrayBuffer* buf = ReadOnlyInt64ArrayBuffer::Copy(
            (Int64ArrayBuffer*)this, mMark);
    if (buf->Probe(EIID_IInt64Buffer) != NULL) {
        *buffer = (IInt64Buffer*)buf->Probe(EIID_IInt64Buffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadWriteInt64ArrayBuffer::Compact()
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

ECode ReadWriteInt64ArrayBuffer::CompareTo(
    /* [in] */ IInt64Buffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return Int64ArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadWriteInt64ArrayBuffer::Duplicate(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadWriteInt64ArrayBuffer* buf = Copy((Int64ArrayBuffer*)this, mMark);
    if (buf->Probe(EIID_IInt64Buffer) != NULL) {
        *buffer = (IInt64Buffer*)buf->Probe(EIID_IInt64Buffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadWriteInt64ArrayBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    return Int64ArrayBuffer::GetInt64(value);
}

ECode ReadWriteInt64ArrayBuffer::GetInt64Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    return Int64ArrayBuffer::GetInt64Ex(index, value);
}

ECode ReadWriteInt64ArrayBuffer::GetInt64s(
    /* [out] */ ArrayOf<Int64>* dst)
{
    return Int64ArrayBuffer::GetInt64s(dst);
}

ECode ReadWriteInt64ArrayBuffer::GetInt64sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Int64>* dst)
{
    return Int64ArrayBuffer::GetInt64sEx(off, len, dst);
}

ECode ReadWriteInt64ArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return Int64ArrayBuffer::GetOrder(order);
}

ECode ReadWriteInt64ArrayBuffer::PutInt64(
    /* [in] */ Int64 c)
{
    if (mPosition == mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    (*mBackingArray)[mOffset + mPosition++] = c;
    return NOERROR;
}

ECode ReadWriteInt64ArrayBuffer::PutInt64Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int64 c)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    (*mBackingArray)[mOffset + index] = c;
    return NOERROR;
}

ECode ReadWriteInt64ArrayBuffer::PutInt64s(
    /* [in] */ const ArrayOf<Int64>& src)
{
    return Int64ArrayBuffer::PutInt64s(src);
}

ECode ReadWriteInt64ArrayBuffer::PutInt64sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Int64>& src)
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

ECode ReadWriteInt64ArrayBuffer::PutInt64Buffer(
    /* [in] */ IInt64Buffer* src)
{
    return Int64ArrayBuffer::PutInt64Buffer(src);
}

ECode ReadWriteInt64ArrayBuffer::Slice(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining;
    Remaining(&remaining);
    ReadWriteInt64ArrayBuffer* buf = new ReadWriteInt64ArrayBuffer(remaining, mBackingArray,
            mOffset + mPosition);
    if (buf->Probe(EIID_IInt64Buffer) != NULL) {
        *buffer = (IInt64Buffer*)buf->Probe(EIID_IInt64Buffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

ECode ReadWriteInt64ArrayBuffer::Capacity(
    /* [out] */ Int32* cap)
{
    return Int64ArrayBuffer::Capacity(cap);
}

ECode ReadWriteInt64ArrayBuffer::Clear()
{
    return Int64ArrayBuffer::Clear();
}

ECode ReadWriteInt64ArrayBuffer::Flip()
{
    return Int64ArrayBuffer::Flip();
}

ECode ReadWriteInt64ArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return Int64ArrayBuffer::HasArray(hasArray);
}

ECode ReadWriteInt64ArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return Int64ArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadWriteInt64ArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return Int64ArrayBuffer::IsDirect(isDirect);
}

ECode ReadWriteInt64ArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = FALSE;
    return NOERROR;
}

CARAPI ReadWriteInt64ArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int64>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = mBackingArray;
    return NOERROR;
}

CARAPI ReadWriteInt64ArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = mOffset;
    return NOERROR;
}

CARAPI ReadWriteInt64ArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

ECode ReadWriteInt64ArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return Int64ArrayBuffer::GetLimit(limit);
}

ECode ReadWriteInt64ArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return Int64ArrayBuffer::SetLimit(newLimit);
}

ECode ReadWriteInt64ArrayBuffer::Mark()
{
    return Int64ArrayBuffer::Mark();
}

ECode ReadWriteInt64ArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return Int64ArrayBuffer::GetPosition(pos);
}

ECode ReadWriteInt64ArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return Int64ArrayBuffer::SetPosition(newPosition);
}

ECode ReadWriteInt64ArrayBuffer::Remaining(
    /* [out] */ Int32* remaining)
{
    return Int64ArrayBuffer::Remaining(remaining);
}

ECode ReadWriteInt64ArrayBuffer::Reset()
{
    return Int64ArrayBuffer::Reset();
}

ECode ReadWriteInt64ArrayBuffer::Rewind()
{
    return Int64ArrayBuffer::Rewind();
}
