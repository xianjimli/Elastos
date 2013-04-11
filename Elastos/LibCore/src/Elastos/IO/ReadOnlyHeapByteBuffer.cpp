
#include "cmdef.h"
#include "ReadOnlyHeapByteBuffer.h"

PInterface ReadOnlyHeapByteBuffer::Probe(
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

UInt32 ReadOnlyHeapByteBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadOnlyHeapByteBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadOnlyHeapByteBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ReadOnlyHeapByteBuffer* ReadOnlyHeapByteBuffer::Copy(
    /* [in] */ HeapByteBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    Int32 cap;
    other->Capacity(&cap);
    ReadOnlyHeapByteBuffer* buf = new ReadOnlyHeapByteBuffer(
            other->mBackingArray, cap, other->mOffset);
    other->GetLimit(&(buf->mLimit));
    other->GetPosition(&(buf->mPosition));
    buf->mMark = markOfOther;
    ByteOrder order;
    other->GetOrder(&order);
    buf->SetOrder(order);

    return buf;
}

ReadOnlyHeapByteBuffer::ReadOnlyHeapByteBuffer(
    /* [in] */ ArrayOf<Byte>* backingArray,
    /* [in] */ Int32 capacity,
    /* [in] */ Int32 arrayOffset)
    : HeapByteBuffer(backingArray, capacity, arrayOffset)
{}

ECode ReadOnlyHeapByteBuffer::Array(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    return HeapByteBuffer::Array(array);
}

ECode ReadOnlyHeapByteBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return HeapByteBuffer::ArrayOffset(offset);
}

ECode ReadOnlyHeapByteBuffer::AsCharBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    return HeapByteBuffer::AsCharBuffer(buffer);
}

ECode ReadOnlyHeapByteBuffer::AsDoubleBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    return HeapByteBuffer::AsDoubleBuffer(buffer);
}

ECode ReadOnlyHeapByteBuffer::AsFloatBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    return HeapByteBuffer::AsFloatBuffer(buffer);
}

ECode ReadOnlyHeapByteBuffer::AsInt16Buffer(
    /* [out] */ IInt16Buffer** buffer)
{
    return HeapByteBuffer::AsInt16Buffer(buffer);
}

ECode ReadOnlyHeapByteBuffer::AsInt32Buffer(
    /* [out] */ IInt32Buffer** buffer)
{
    return HeapByteBuffer::AsInt32Buffer(buffer);
}

ECode ReadOnlyHeapByteBuffer::AsInt64Buffer(
    /* [out] */ IInt64Buffer** buffer)
{
    return HeapByteBuffer::AsInt64Buffer(buffer);
}

ECode ReadOnlyHeapByteBuffer::AsReadOnlyBuffer(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadOnlyHeapByteBuffer* buf = Copy((HeapByteBuffer*)this, mMark);
    if (buf->Probe(EIID_IByteBuffer)) {
        *buffer = (IByteBuffer*)buf->Probe(EIID_IByteBuffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

ECode ReadOnlyHeapByteBuffer::Compact()
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyHeapByteBuffer::CompareTo(
    /* [in] */ IByteBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return HeapByteBuffer::CompareTo(otherBuffer, result);
}

ECode ReadOnlyHeapByteBuffer::Duplicate(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadOnlyHeapByteBuffer* buf = Copy((HeapByteBuffer*)this, mMark);
    if (buf->Probe(EIID_IByteBuffer)) {
        *buffer = (IByteBuffer*)buf->Probe(EIID_IByteBuffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

ECode ReadOnlyHeapByteBuffer::GetByte(
    /* [out] */ Byte* value)
{
    return HeapByteBuffer::GetByte(value);
}

ECode ReadOnlyHeapByteBuffer::GetByteEx(
    /* [in] */ Int32 index,
    /* [out] */ Byte* value)
{
    return HeapByteBuffer::GetByteEx(index, value);
}

ECode ReadOnlyHeapByteBuffer::GetBytes(
    /* [out] */ ArrayOf<Byte>* dst)
{
    return HeapByteBuffer::GetBytes(dst);
}

ECode ReadOnlyHeapByteBuffer::GetBytesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Byte>* dst)
{
    return HeapByteBuffer::GetBytesEx(off, len, dst);
}

ECode ReadOnlyHeapByteBuffer::GetChar(
    /* [out] */ Char32* value)
{
    return HeapByteBuffer::GetChar(value);
}

ECode ReadOnlyHeapByteBuffer::GetCharEx(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    return HeapByteBuffer::GetCharEx(index, value);
}

ECode ReadOnlyHeapByteBuffer::GetDouble(
    /* [out] */ Double* value)
{
    return HeapByteBuffer::GetDouble(value);
}

ECode ReadOnlyHeapByteBuffer::GetDoubleEx(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    return HeapByteBuffer::GetDoubleEx(index, value);
}

ECode ReadOnlyHeapByteBuffer::GetFloat(
    /* [out] */ Float* value)
{
    return HeapByteBuffer::GetFloat(value);
}

ECode ReadOnlyHeapByteBuffer::GetFloatEx(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    return HeapByteBuffer::GetFloatEx(index, value);
}

ECode ReadOnlyHeapByteBuffer::GetInt32(
    /* [out] */ Int32* value)
{
    return HeapByteBuffer::GetInt32(value);
}

ECode ReadOnlyHeapByteBuffer::GetInt32Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    return HeapByteBuffer::GetInt32Ex(index, value);
}

ECode ReadOnlyHeapByteBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    return HeapByteBuffer::GetInt64(value);
}

ECode ReadOnlyHeapByteBuffer::GetInt64Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    return HeapByteBuffer::GetInt64Ex(index, value);
}

ECode ReadOnlyHeapByteBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    return HeapByteBuffer::GetInt16(value);
}

ECode ReadOnlyHeapByteBuffer::GetInt16Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    return HeapByteBuffer::GetInt16Ex(index, value);
}

ECode ReadOnlyHeapByteBuffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    return HeapByteBuffer::GetOrder(byteOrder);
}

ECode ReadOnlyHeapByteBuffer::SetOrder(
    /* [in] */ ByteOrder byteOrder)
{
    return HeapByteBuffer::SetOrder(byteOrder);
}

ECode ReadOnlyHeapByteBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyHeapByteBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyHeapByteBuffer::ProtectedHasArray(
    /* [out] */ Boolean* hasArray)
{
    VALIDATE_NOT_NULL(hasArray);
    *hasArray = FALSE;
    return NOERROR;
}

ECode ReadOnlyHeapByteBuffer::PutByte(
    /* [in] */ Byte b)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyHeapByteBuffer::PutByteEx(
    /* [in] */ Int32 index,
    /* [in] */ Byte b)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyHeapByteBuffer::PutBytes(
    /* [in] */ const ArrayOf<Byte>& src)
{
    return HeapByteBuffer::PutBytes(src);
}

ECode ReadOnlyHeapByteBuffer::PutBytesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Byte>& src)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyHeapByteBuffer::PutByteBuffer(
    /* [in] */ IByteBuffer* src)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyHeapByteBuffer::PutChar(
    /* [in] */ Char32 value)
{
    return HeapByteBuffer::PutChar(value);
}

ECode ReadOnlyHeapByteBuffer::PutCharEx(
    /* [in] */ Int32 index,
    /* [in] */ Char32 value)
{
    return HeapByteBuffer::PutCharEx(index, value);
}

ECode ReadOnlyHeapByteBuffer::PutDouble(
    /* [in] */ Double value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyHeapByteBuffer::PutDoubleEx(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyHeapByteBuffer::PutFloat(
    /* [in] */ Float value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyHeapByteBuffer::PutFloatEx(
    /* [in] */ Int32 index,
    /* [in] */ Float value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyHeapByteBuffer::PutInt16(
    /* [in] */ Int16 value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyHeapByteBuffer::PutInt16Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int16 value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyHeapByteBuffer::PutInt32(
    /* [in] */ Int32 value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyHeapByteBuffer::PutInt32Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyHeapByteBuffer::PutInt64(
    /* [in] */ Int64 value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyHeapByteBuffer::PutInt64Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyHeapByteBuffer::Slice(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    Int32 remaining;
    Remaining(&remaining);
    ReadOnlyHeapByteBuffer* slice = new ReadOnlyHeapByteBuffer(mBackingArray,
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

ECode ReadOnlyHeapByteBuffer::Capacity(
    /* [out] */ Int32* cap)
{
    return HeapByteBuffer::Capacity(cap);
}

ECode ReadOnlyHeapByteBuffer::Clear()
{
    return HeapByteBuffer::Clear();
}

ECode ReadOnlyHeapByteBuffer::Flip()
{
    return HeapByteBuffer::Flip();
}

ECode ReadOnlyHeapByteBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return HeapByteBuffer::HasArray(hasArray);
}

ECode ReadOnlyHeapByteBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return HeapByteBuffer::HasRemaining(hasRemaining);
}

ECode ReadOnlyHeapByteBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return HeapByteBuffer::IsDirect(isDirect);
}

ECode ReadOnlyHeapByteBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

ECode ReadOnlyHeapByteBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return HeapByteBuffer::GetLimit(limit);
}

ECode ReadOnlyHeapByteBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return HeapByteBuffer::SetLimit(newLimit);
}

ECode ReadOnlyHeapByteBuffer::Mark()
{
    return HeapByteBuffer::Mark();
}

ECode ReadOnlyHeapByteBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return HeapByteBuffer::GetPosition(pos);
}

ECode ReadOnlyHeapByteBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return HeapByteBuffer::SetPosition(newPosition);
}

ECode ReadOnlyHeapByteBuffer::Remaining(
    /* [out] */ Int32* remaining)
{
    return HeapByteBuffer::Remaining(remaining);
}

ECode ReadOnlyHeapByteBuffer::Reset()
{
    return HeapByteBuffer::Reset();
}

ECode ReadOnlyHeapByteBuffer::Rewind()
{
    return HeapByteBuffer::Rewind();
}

ECode ReadOnlyHeapByteBuffer::Equals(
    /* [in]  */ IInterface* other,
    /* [out] */ Boolean* isEquals)
{
    ByteBuffer::Equals(other, isEquals);
    return NOERROR;
}

