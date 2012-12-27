
#include "cmdef.h"
#include "ReadWriteInt16ArrayBuffer.h"
#include "ReadOnlyInt16ArrayBuffer.h"

PInterface ReadWriteInt16ArrayBuffer::Probe(
        /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IInt16Buffer) {
        return (IInt16Buffer*)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_Int16Buffer) {
        return reinterpret_cast<PInterface>((Int16Buffer*)this);
    }

    return NULL;
}


UInt32 ReadWriteInt16ArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadWriteInt16ArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadWriteInt16ArrayBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ReadWriteInt16ArrayBuffer* ReadWriteInt16ArrayBuffer::Copy(
    /* [in] */ Int16ArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    Int32 capacity;
    other->Capacity(&capacity);
    ReadWriteInt16ArrayBuffer* buf = new ReadWriteInt16ArrayBuffer(capacity,
            other->mBackingArray, other->mOffset);
    other->GetLimit(&buf->mLimit);
    other->GetPosition(&buf->mPosition);
    buf->mMark = markOfOther;
    return buf;
}

ReadWriteInt16ArrayBuffer::ReadWriteInt16ArrayBuffer(
    /* [in] */ ArrayOf<Int16>* array)
    : Int16ArrayBuffer(array)
{}

ReadWriteInt16ArrayBuffer::ReadWriteInt16ArrayBuffer(
    /* [in] */ Int32 capacity)
    : Int16ArrayBuffer(capacity)
{}

ReadWriteInt16ArrayBuffer::ReadWriteInt16ArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Int16>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : Int16ArrayBuffer(capacity, backingArray, arrayOffset)
{}

ECode ReadWriteInt16ArrayBuffer::Array(
    /* [out, callee] */ ArrayOf<Int16>** array)
{
    return Int16ArrayBuffer::Array(array);
}

ECode ReadWriteInt16ArrayBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return Int16ArrayBuffer::ArrayOffset(offset);
}

ECode ReadWriteInt16ArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadOnlyInt16ArrayBuffer* buf = ReadOnlyInt16ArrayBuffer::Copy(
            (Int16ArrayBuffer*)this, mMark);
    if (buf->Probe(EIID_IInt16Buffer) != NULL) {
        *buffer = (IInt16Buffer*)buf->Probe(EIID_IInt16Buffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadWriteInt16ArrayBuffer::Compact()
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

ECode ReadWriteInt16ArrayBuffer::CompareTo(
    /* [in] */ IInt16Buffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return Int16ArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadWriteInt16ArrayBuffer::Duplicate(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadWriteInt16ArrayBuffer* buf = Copy((Int16ArrayBuffer*)this, mMark);
    if (buf->Probe(EIID_IInt16Buffer) != NULL) {
        *buffer = (IInt16Buffer*)buf->Probe(EIID_IInt16Buffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadWriteInt16ArrayBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    return Int16ArrayBuffer::GetInt16(value);
}

ECode ReadWriteInt16ArrayBuffer::GetInt16Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    return Int16ArrayBuffer::GetInt16Ex(index, value);
}

ECode ReadWriteInt16ArrayBuffer::GetInt16s(
    /* [out] */ ArrayOf<Int16>* dst)
{
    return Int16ArrayBuffer::GetInt16s(dst);
}

ECode ReadWriteInt16ArrayBuffer::GetInt16sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Int16>* dst)
{
    return Int16ArrayBuffer::GetInt16sEx(off, len, dst);
}

ECode ReadWriteInt16ArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return Int16ArrayBuffer::GetOrder(order);
}

ECode ReadWriteInt16ArrayBuffer::PutInt16(
    /* [in] */ Int16 c)
{
    if (mPosition == mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    (*mBackingArray)[mOffset + mPosition++] = c;
    return NOERROR;
}

ECode ReadWriteInt16ArrayBuffer::PutInt16Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int16 c)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    (*mBackingArray)[mOffset + index] = c;
    return NOERROR;
}

ECode ReadWriteInt16ArrayBuffer::PutInt16s(
    /* [in] */ const ArrayOf<Int16>& src)
{
    return Int16ArrayBuffer::PutInt16s(src);
}

ECode ReadWriteInt16ArrayBuffer::PutInt16sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Int16>& src)
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

ECode ReadWriteInt16ArrayBuffer::PutInt16Buffer(
    /* [in] */ IInt16Buffer* src)
{
    return Int16ArrayBuffer::PutInt16Buffer(src);
}

ECode ReadWriteInt16ArrayBuffer::Slice(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining;
    Remaining(&remaining);
    ReadWriteInt16ArrayBuffer* buf = new ReadWriteInt16ArrayBuffer(remaining, mBackingArray,
            mOffset + mPosition);
    if (buf->Probe(EIID_IInt16Buffer) != NULL) {
        *buffer = (IInt16Buffer*)buf->Probe(EIID_IInt16Buffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

ECode ReadWriteInt16ArrayBuffer::Capacity(
    /* [out] */ Int32* cap)
{
    return Int16ArrayBuffer::Capacity(cap);
}

ECode ReadWriteInt16ArrayBuffer::Clear()
{
    return Int16ArrayBuffer::Clear();
}

ECode ReadWriteInt16ArrayBuffer::Flip()
{
    return Int16ArrayBuffer::Flip();
}

ECode ReadWriteInt16ArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return Int16ArrayBuffer::HasArray(hasArray);
}

ECode ReadWriteInt16ArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return Int16ArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadWriteInt16ArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return Int16ArrayBuffer::IsDirect(isDirect);
}

ECode ReadWriteInt16ArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = FALSE;
    return NOERROR;
}

CARAPI ReadWriteInt16ArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int16>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = mBackingArray;
    return NOERROR;
}

CARAPI ReadWriteInt16ArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = mOffset;
    return NOERROR;
}

CARAPI ReadWriteInt16ArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

ECode ReadWriteInt16ArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return Int16ArrayBuffer::GetLimit(limit);
}

ECode ReadWriteInt16ArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return Int16ArrayBuffer::SetLimit(newLimit);
}

ECode ReadWriteInt16ArrayBuffer::Mark()
{
    return Int16ArrayBuffer::Mark();
}

ECode ReadWriteInt16ArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return Int16ArrayBuffer::GetPosition(pos);
}

ECode ReadWriteInt16ArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return Int16ArrayBuffer::SetPosition(newPosition);
}

ECode ReadWriteInt16ArrayBuffer::Remaining(
    /* [out] */ Int32* remaining)
{
    return Int16ArrayBuffer::Remaining(remaining);
}

ECode ReadWriteInt16ArrayBuffer::Reset()
{
    return Int16ArrayBuffer::Reset();
}

ECode ReadWriteInt16ArrayBuffer::Rewind()
{
    return Int16ArrayBuffer::Rewind();
}
