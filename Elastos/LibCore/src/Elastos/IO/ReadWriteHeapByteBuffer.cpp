
#include "cmdef.h"
#include "ReadWriteHeapByteBuffer.h"
#include "ReadOnlyHeapByteBuffer.h"

PInterface ReadWriteHeapByteBuffer::Probe(
        /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IByteBuffer) {
        return (IByteBuffer*)this;
    }
    else if (riid == EIID_ByteBuffer) {
        return reinterpret_cast<PInterface>((ByteBuffer*)this);
    }

    return NULL;
}

UInt32 ReadWriteHeapByteBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadWriteHeapByteBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadWriteHeapByteBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ReadWriteHeapByteBuffer* ReadWriteHeapByteBuffer::Copy(
    /* [in] */ HeapByteBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    Int32 cap;
    other->Capacity(&cap);
    ReadWriteHeapByteBuffer* buf = new ReadWriteHeapByteBuffer(
            other->mBackingArray, cap, other->mOffset);
    other->GetLimit(&(buf->mLimit));
    other->GetPosition(&(buf->mPosition));
    buf->mMark = markOfOther;
    ByteOrder order;
    other->GetOrder(&order);
    buf->SetOrder(order);

    return buf;
}

ReadWriteHeapByteBuffer::ReadWriteHeapByteBuffer(
    /* [in] */ ArrayOf<Byte>* backingArray)
    : HeapByteBuffer(backingArray, backingArray->GetLength(), 0)
{}

ReadWriteHeapByteBuffer::ReadWriteHeapByteBuffer(
    /* [in] */ Int32 capacity)
    : HeapByteBuffer(ArrayOf<Byte>::Alloc(capacity), capacity, 0)
{}

ReadWriteHeapByteBuffer::ReadWriteHeapByteBuffer(
    /* [in] */ ArrayOf<Byte>* backingArray,
    /* [in] */ Int32 capacity,
    /* [in] */ Int32 arrayOffset)
    : HeapByteBuffer(backingArray, capacity, arrayOffset)
{}

ECode ReadWriteHeapByteBuffer::Array(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    return HeapByteBuffer::Array(array);
}

ECode ReadWriteHeapByteBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return HeapByteBuffer::ArrayOffset(offset);
}

ECode ReadWriteHeapByteBuffer::AsCharBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    return HeapByteBuffer::AsCharBuffer(buffer);
}

ECode ReadWriteHeapByteBuffer::AsDoubleBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    return HeapByteBuffer::AsDoubleBuffer(buffer);
}

ECode ReadWriteHeapByteBuffer::AsFloatBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    return HeapByteBuffer::AsFloatBuffer(buffer);
}

ECode ReadWriteHeapByteBuffer::AsInt16Buffer(
    /* [out] */ IInt16Buffer** buffer)
{
    return HeapByteBuffer::AsInt16Buffer(buffer);
}

ECode ReadWriteHeapByteBuffer::AsInt32Buffer(
    /* [out] */ IInt32Buffer** buffer)
{
    return HeapByteBuffer::AsInt32Buffer(buffer);
}

ECode ReadWriteHeapByteBuffer::AsInt64Buffer(
    /* [out] */ IInt64Buffer** buffer)
{
    return HeapByteBuffer::AsInt64Buffer(buffer);
}

ECode ReadWriteHeapByteBuffer::AsReadOnlyBuffer(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadOnlyHeapByteBuffer* buf = ReadOnlyHeapByteBuffer::Copy((HeapByteBuffer*)this, mMark);
    if (buf->Probe(EIID_IByteBuffer)) {
        *buffer = (IByteBuffer*)buf->Probe(EIID_IByteBuffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::Compact()
{
    // System.arraycopy(backingArray, position + offset, backingArray, offset,
    //         remaining());
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

ECode ReadWriteHeapByteBuffer::CompareTo(
    /* [in] */ IByteBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return HeapByteBuffer::CompareTo(otherBuffer, result);
}

ECode ReadWriteHeapByteBuffer::Duplicate(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadWriteHeapByteBuffer* buf = Copy((HeapByteBuffer*)this, mMark);
    if (buf->Probe(EIID_IByteBuffer)) {
        *buffer = (IByteBuffer*)buf->Probe(EIID_IByteBuffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::GetByte(
    /* [out] */ Byte* value)
{
    return HeapByteBuffer::GetByte(value);
}

ECode ReadWriteHeapByteBuffer::GetByteEx(
    /* [in] */ Int32 index,
    /* [out] */ Byte* value)
{
    return HeapByteBuffer::GetByteEx(index, value);
}

ECode ReadWriteHeapByteBuffer::GetBytes(
    /* [out] */ ArrayOf<Byte>* dst)
{
    return HeapByteBuffer::GetBytes(dst);
}

ECode ReadWriteHeapByteBuffer::GetBytesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Byte>* dst)
{
    return HeapByteBuffer::GetBytesEx(off, len, dst);
}

ECode ReadWriteHeapByteBuffer::GetChar(
    /* [out] */ Char32* value)
{
    return HeapByteBuffer::GetChar(value);
}

ECode ReadWriteHeapByteBuffer::GetCharEx(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    return HeapByteBuffer::GetCharEx(index, value);
}

ECode ReadWriteHeapByteBuffer::GetDouble(
    /* [out] */ Double* value)
{
    return HeapByteBuffer::GetDouble(value);
}

ECode ReadWriteHeapByteBuffer::GetDoubleEx(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    return HeapByteBuffer::GetDoubleEx(index, value);
}

ECode ReadWriteHeapByteBuffer::GetFloat(
    /* [out] */ Float* value)
{
    return HeapByteBuffer::GetFloat(value);
}

ECode ReadWriteHeapByteBuffer::GetFloatEx(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    return HeapByteBuffer::GetFloatEx(index, value);
}

ECode ReadWriteHeapByteBuffer::GetInt32(
    /* [out] */ Int32* value)
{
    return HeapByteBuffer::GetInt32(value);
}

ECode ReadWriteHeapByteBuffer::GetInt32Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    return HeapByteBuffer::GetInt32Ex(index, value);
}

ECode ReadWriteHeapByteBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    return HeapByteBuffer::GetInt64(value);
}

ECode ReadWriteHeapByteBuffer::GetInt64Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    return HeapByteBuffer::GetInt64Ex(index, value);
}

ECode ReadWriteHeapByteBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    return HeapByteBuffer::GetInt16(value);
}

ECode ReadWriteHeapByteBuffer::GetInt16Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    return HeapByteBuffer::GetInt16Ex(index, value);
}

ECode ReadWriteHeapByteBuffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    return HeapByteBuffer::GetOrder(byteOrder);
}

ECode ReadWriteHeapByteBuffer::SetOrder(
    /* [in] */ ByteOrder byteOrder)
{
    return HeapByteBuffer::SetOrder(byteOrder);
}

ECode ReadWriteHeapByteBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = mBackingArray;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = mOffset;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::ProtectedHasArray(
    /* [out] */ Boolean* hasArray)
{
    VALIDATE_NOT_NULL(hasArray);
    *hasArray = TRUE;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutByte(
    /* [in] */ Byte b)
{
    if (mPosition == mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    (*mBackingArray)[mOffset + mPosition++] = b;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutByteEx(
    /* [in] */ Int32 index,
    /* [in] */ Byte b)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    (*mBackingArray)[mOffset + index] = b;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutBytes(
    /* [in] */ const ArrayOf<Byte>& src)
{
    return HeapByteBuffer::PutBytes(src);
}

ECode ReadWriteHeapByteBuffer::PutBytesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Byte>& src)
{
    if (off < 0 || len < 0 || (Int64)len + (Int64)off > src.GetLength()) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    Int32 remaining;
    Remaining(&remaining);
    if (len > remaining) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    Boolean isReadOnly;
    IsReadOnly(&isReadOnly);
    if (isReadOnly) {
        return E_READ_ONLY_BUFFER_EXCEPTION;
//        throw new ReadOnlyBufferException();
    }
//    System.arraycopy(src, off, backingArray, offset + position, len);
    for(Int32 i = 0; i < len; ++i) {
        (*mBackingArray)[i + mOffset + mPosition] = src[off + i];
    }
    mPosition += len;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutByteBuffer(
    /* [in] */ IByteBuffer* src)
{
    return HeapByteBuffer::PutByteBuffer(src);
}

ECode ReadWriteHeapByteBuffer::PutChar(
    /* [in] */ Char32 value)
{
    return HeapByteBuffer::PutChar(value);
}

ECode ReadWriteHeapByteBuffer::PutCharEx(
    /* [in] */ Int32 index,
    /* [in] */ Char32 value)
{
    return HeapByteBuffer::PutCharEx(index, value);
}

ECode ReadWriteHeapByteBuffer::PutDouble(
    /* [in] */ Double value)
{
    return PutInt64((Int64)value);
}

ECode ReadWriteHeapByteBuffer::PutDoubleEx(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    return PutInt64Ex(index, (Int64)value);
}

ECode ReadWriteHeapByteBuffer::PutFloat(
    /* [in] */ Float value)
{
    return PutInt32((Int32)value);
}

ECode ReadWriteHeapByteBuffer::PutFloatEx(
    /* [in] */ Int32 index,
    /* [in] */ Float value)
{
    return PutInt32Ex(index, (Int32)value);
}

ECode ReadWriteHeapByteBuffer::PutInt16(
    /* [in] */ Int16 value)
{
    Int32 newPosition = mPosition + 2;
    if (newPosition > mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    Store(mPosition, value);
    mPosition = newPosition;

    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutInt16Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int16 value)
{
    if (index < 0 || (Int64) index + 2 > mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    Store(index, value);

    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutInt32(
    /* [in] */ Int32 value)
{
    Int32 newPosition = mPosition + 4;
    if (newPosition > mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    Store(mPosition, value);
    mPosition = newPosition;

    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutInt32Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    if (index < 0 || (Int64) index + 4 > mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    Store(index, value);

    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutInt64(
    /* [in] */ Int64 value)
{
    Int32 newPosition = mPosition + 8;
    if (newPosition > mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    Store(mPosition, value);
    mPosition = newPosition;

    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::PutInt64Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    if (index < 0 || (Int64) index + 8 > mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    Store(index, value);

    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::Slice(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    Int32 remaining;
    Remaining(&remaining);
    ReadWriteHeapByteBuffer* slice = new ReadWriteHeapByteBuffer(mBackingArray,
            remaining, mOffset + mPosition);
    slice->mOrder = mOrder;
    if (slice->Probe(EIID_IByteBuffer) != NULL) {
        *buffer = (IByteBuffer*)slice->Probe(EIID_IByteBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::Capacity(
    /* [out] */ Int32* cap)
{
    return HeapByteBuffer::Capacity(cap);
}

ECode ReadWriteHeapByteBuffer::Clear()
{
    return HeapByteBuffer::Clear();
}

ECode ReadWriteHeapByteBuffer::Flip()
{
    return HeapByteBuffer::Flip();
}

ECode ReadWriteHeapByteBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return HeapByteBuffer::HasArray(hasArray);
}

ECode ReadWriteHeapByteBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return HeapByteBuffer::HasRemaining(hasRemaining);
}

ECode ReadWriteHeapByteBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return HeapByteBuffer::IsDirect(isDirect);
}

ECode ReadWriteHeapByteBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = FALSE;
    return NOERROR;
}

ECode ReadWriteHeapByteBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return HeapByteBuffer::GetLimit(limit);
}

ECode ReadWriteHeapByteBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return HeapByteBuffer::SetLimit(newLimit);
}

ECode ReadWriteHeapByteBuffer::Mark()
{
    return HeapByteBuffer::Mark();
}

ECode ReadWriteHeapByteBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return HeapByteBuffer::GetPosition(pos);
}

ECode ReadWriteHeapByteBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return HeapByteBuffer::SetPosition(newPosition);
}

ECode ReadWriteHeapByteBuffer::Remaining(
    /* [out] */ Int32* remaining)
{
    return HeapByteBuffer::Remaining(remaining);
}

ECode ReadWriteHeapByteBuffer::Reset()
{
    return HeapByteBuffer::Reset();
}

ECode ReadWriteHeapByteBuffer::Rewind()
{
    return HeapByteBuffer::Rewind();
}

ECode ReadWriteHeapByteBuffer::Equals(
        /* [in]  */ IInterface* other,
        /* [out] */ Boolean* isEquals)
{
    ByteBuffer::Equals(other, isEquals);
    return NOERROR;
}
