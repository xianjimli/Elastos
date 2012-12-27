
#include "cmdef.h"
#include "ReadOnlyDirectByteBuffer.h"

PInterface ReadOnlyDirectByteBuffer::Probe(
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


UInt32 ReadOnlyDirectByteBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadOnlyDirectByteBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadOnlyDirectByteBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ReadOnlyDirectByteBuffer* ReadOnlyDirectByteBuffer::Copy(
    /* [in] */ DirectByteBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    Int32 capacity;
    other->Capacity(&capacity);
    ReadOnlyDirectByteBuffer* buf = new ReadOnlyDirectByteBuffer(
            other->mSafeAddress, capacity, other->mOffset);
    other->GetLimit(&(buf->mLimit));
    other->GetPosition(&(buf->mPosition));
    buf->mMark = markOfOther;
    ByteOrder order;
    other->GetOrder(&order);
    buf->SetOrder(order);
    return buf;
}

ReadOnlyDirectByteBuffer::ReadOnlyDirectByteBuffer(
    /* [in] */ SafeAddress* address,
    /* [in] */ Int32 capacity,
    /* [in] */ Int32 offset)
    : DirectByteBuffer(address, capacity, offset)
{}

ReadOnlyDirectByteBuffer::ReadOnlyDirectByteBuffer(
   /* [in]  IPlatformAddress* address,*/
   /* [in] */ Int32 aCapacity,
   /* [in] */ Int32 anOffset)
    : DirectByteBuffer(new SafeAddress(/*address*/), aCapacity, anOffset)
{}

ECode ReadOnlyDirectByteBuffer::Array(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    return DirectByteBuffer::Array(array);
}

ECode ReadOnlyDirectByteBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return DirectByteBuffer::ArrayOffset(offset);
}

ECode ReadOnlyDirectByteBuffer::AsCharBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    return DirectByteBuffer::AsCharBuffer(buffer);
}

ECode ReadOnlyDirectByteBuffer::AsDoubleBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    return DirectByteBuffer::AsDoubleBuffer(buffer);
}

ECode ReadOnlyDirectByteBuffer::AsFloatBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    return DirectByteBuffer::AsFloatBuffer(buffer);
}

ECode ReadOnlyDirectByteBuffer::AsInt16Buffer(
    /* [out] */ IInt16Buffer** buffer)
{
    return DirectByteBuffer::AsInt16Buffer(buffer);
}

ECode ReadOnlyDirectByteBuffer::AsInt32Buffer(
    /* [out] */ IInt32Buffer** buffer)
{
    return DirectByteBuffer::AsInt32Buffer(buffer);
}

ECode ReadOnlyDirectByteBuffer::AsInt64Buffer(
    /* [out] */ IInt64Buffer** buffer)
{
    return DirectByteBuffer::AsInt64Buffer(buffer);
}

ECode ReadOnlyDirectByteBuffer::AsReadOnlyBuffer(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadOnlyDirectByteBuffer* buf = Copy((DirectByteBuffer*)this, mMark);
    if (buf->Probe(EIID_IByteBuffer) != NULL) {
        *buffer = (IByteBuffer*)buf->Probe(EIID_IByteBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadOnlyDirectByteBuffer::Compact()
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::CompareTo(
    /* [in] */ IByteBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return DirectByteBuffer::CompareTo(otherBuffer, result);
}

ECode ReadOnlyDirectByteBuffer::Duplicate(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadOnlyDirectByteBuffer* buf = Copy(this, mMark);
    if (buf != NULL && buf->Probe(EIID_IByteBuffer) != NULL) {
        *buffer = (IByteBuffer*)buf->Probe(EIID_IByteBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadOnlyDirectByteBuffer::GetByte(
    /* [out] */ Byte* value)
{
    return DirectByteBuffer::GetByte(value);
}

ECode ReadOnlyDirectByteBuffer::GetByteEx(
    /* [in] */ Int32 index,
    /* [out] */ Byte* value)
{
    return DirectByteBuffer::GetByteEx(index, value);
}

ECode ReadOnlyDirectByteBuffer::GetBytes(
    /* [out] */ ArrayOf<Byte>* dst)
{
    return DirectByteBuffer::GetBytes(dst);
}

ECode ReadOnlyDirectByteBuffer::GetBytesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Byte>* dst)
{
    return DirectByteBuffer::GetBytesEx(off, len, dst);
}

ECode ReadOnlyDirectByteBuffer::GetChar(
    /* [out] */ Char32* value)
{
    return DirectByteBuffer::GetChar(value);
}

ECode ReadOnlyDirectByteBuffer::GetCharEx(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    return DirectByteBuffer::GetCharEx(index, value);
}

ECode ReadOnlyDirectByteBuffer::GetDouble(
    /* [out] */ Double* value)
{
    return DirectByteBuffer::GetDouble(value);
}

ECode ReadOnlyDirectByteBuffer::GetDoubleEx(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    return DirectByteBuffer::GetDoubleEx(index, value);
}

ECode ReadOnlyDirectByteBuffer::GetFloat(
    /* [out] */ Float* value)
{
    return DirectByteBuffer::GetFloat(value);
}

ECode ReadOnlyDirectByteBuffer::GetFloatEx(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    return DirectByteBuffer::GetFloatEx(index, value);
}

ECode ReadOnlyDirectByteBuffer::GetInt32(
    /* [out] */ Int32* value)
{
    return DirectByteBuffer::GetInt32(value);
}

ECode ReadOnlyDirectByteBuffer::GetInt32Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    return DirectByteBuffer::GetInt32Ex(index, value);
}

ECode ReadOnlyDirectByteBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    return DirectByteBuffer::GetInt64(value);
}

ECode ReadOnlyDirectByteBuffer::GetInt64Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    return DirectByteBuffer::GetInt64Ex(index, value);
}

ECode ReadOnlyDirectByteBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    return DirectByteBuffer::GetInt16(value);
}

ECode ReadOnlyDirectByteBuffer::GetInt16Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    return DirectByteBuffer::GetInt16Ex(index, value);
}

ECode ReadOnlyDirectByteBuffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    return DirectByteBuffer::GetOrder(byteOrder);
}

ECode ReadOnlyDirectByteBuffer::SetOrder(
    /* [in] */ ByteOrder byteOrder)
{
    return DirectByteBuffer::SetOrder(byteOrder);
}

ECode ReadOnlyDirectByteBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    return DirectByteBuffer::ProtectedArray(array);
}

ECode ReadOnlyDirectByteBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return DirectByteBuffer::ProtectedArrayOffset(offset);
}

ECode ReadOnlyDirectByteBuffer::ProtectedHasArray(
    /* [out] */ Boolean* hasArray)
{
    return DirectByteBuffer::ProtectedHasArray(hasArray);
}

ECode ReadOnlyDirectByteBuffer::PutByte(
    /* [in] */ Byte b)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutByteEx(
    /* [in] */ Int32 index,
    /* [in] */ Byte b)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutBytes(
    /* [in] */ const ArrayOf<Byte>& src)
{
    return DirectByteBuffer::PutBytes(src);
}

ECode ReadOnlyDirectByteBuffer::PutBytesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Byte>& src)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutByteBuffer(
    /* [in] */ IByteBuffer* src)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutChar(
    /* [in] */ Char32 value)
{
    return DirectByteBuffer::PutChar(value);
}

ECode ReadOnlyDirectByteBuffer::PutCharEx(
    /* [in] */ Int32 index,
    /* [in] */ Char32 value)
{
    return DirectByteBuffer::PutCharEx(index, value);
}

ECode ReadOnlyDirectByteBuffer::PutDouble(
    /* [in] */ Double value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutDoubleEx(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutFloat(
    /* [in] */ Float value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutFloatEx(
    /* [in] */ Int32 index,
    /* [in] */ Float value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutInt16(
    /* [in] */ Int16 value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutInt16Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int16 value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutInt32(
    /* [in] */ Int32 value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutInt32Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutInt64(
    /* [in] */ Int64 value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::PutInt64Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDirectByteBuffer::Slice(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    Int32 remaining;
    Remaining(&remaining);
    ReadOnlyDirectByteBuffer* buf = new ReadOnlyDirectByteBuffer(
            mSafeAddress, remaining, mOffset + mPosition);
    buf->mOrder = mOrder;
    if (buf->Probe(EIID_IByteBuffer) != NULL) {
        *buffer = (IByteBuffer*)buf->Probe(EIID_IByteBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadOnlyDirectByteBuffer::Capacity(
    /* [out] */ Int32* cap)
{
    return DirectByteBuffer::Capacity(cap);
}

ECode ReadOnlyDirectByteBuffer::Clear()
{
    return DirectByteBuffer::Clear();
}

ECode ReadOnlyDirectByteBuffer::Flip()
{
    return DirectByteBuffer::Flip();
}

ECode ReadOnlyDirectByteBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return DirectByteBuffer::HasArray(hasArray);
}

ECode ReadOnlyDirectByteBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return DirectByteBuffer::HasRemaining(hasRemaining);
}

ECode ReadOnlyDirectByteBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return DirectByteBuffer::IsDirect(isDirect);
}

ECode ReadOnlyDirectByteBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

ECode ReadOnlyDirectByteBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return DirectByteBuffer::GetLimit(limit);
}

ECode ReadOnlyDirectByteBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return DirectByteBuffer::SetLimit(newLimit);
}

ECode ReadOnlyDirectByteBuffer::Mark()
{
    return DirectByteBuffer::Mark();
}

ECode ReadOnlyDirectByteBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return DirectByteBuffer::GetPosition(pos);
}

ECode ReadOnlyDirectByteBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return DirectByteBuffer::SetPosition(newPosition);
}

ECode ReadOnlyDirectByteBuffer::Remaining(
    /* [out] */ Int32* remaining)
{
    return DirectByteBuffer::Remaining(remaining);
}

ECode ReadOnlyDirectByteBuffer::Reset()
{
    return DirectByteBuffer::Reset();
}

ECode ReadOnlyDirectByteBuffer::Rewind()
{
    return DirectByteBuffer::Rewind();
}

