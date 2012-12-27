
#include "cmdef.h"
#include "ReadWriteDirectByteBuffer.h"
#include "ReadOnlyDirectByteBuffer.h"

// {D9F5D1A4-E93E-4512-BF39-05E76DF67EA2}
extern "C" const InterfaceID EIID_ReadWriteDirectByteBuffer =
    { 0xd9f5d1a4, 0xe93e, 0x4512, { 0xbf, 0x39, 0x5, 0xe7, 0x6d, 0xf6, 0x7e, 0xa2 } };

PInterface ReadWriteDirectByteBuffer::Probe(
        /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_IByteBuffer) {
        return (IByteBuffer*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_ByteBuffer) {
        return reinterpret_cast<PInterface>((ByteBuffer*)this);
    }
    else if (riid == EIID_ReadWriteDirectByteBuffer) {
        return reinterpret_cast<PInterface>(this);
    }

    return NULL;
}


UInt32 ReadWriteDirectByteBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadWriteDirectByteBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadWriteDirectByteBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ReadWriteDirectByteBuffer* ReadWriteDirectByteBuffer::Copy(
    /* [in] */ DirectByteBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    Int32 capacity;
    other->Capacity(&capacity);
    ReadWriteDirectByteBuffer* buf = new ReadWriteDirectByteBuffer(
            other->mSafeAddress, capacity, other->mOffset);
    other->GetLimit(&(buf->mLimit));
    other->GetPosition(&(buf->mPosition));
    buf->mMark = markOfOther;
    ByteOrder order;
    other->GetOrder(&order);
    buf->SetOrder(order);
    return buf;
}

ReadWriteDirectByteBuffer::ReadWriteDirectByteBuffer(
    /* [in] */ Int32 capacity)
    : DirectByteBuffer(capacity)
{}

// BEGIN android-added
ReadWriteDirectByteBuffer::ReadWriteDirectByteBuffer(
    /* [in] */ Int32 pointer,
    /* [in] */ Int32 capacity)
    : DirectByteBuffer(NULL, capacity, 0)
{
//    this(PlatformAddressFactory.on(pointer, capacity), capacity, 0);
}

ReadWriteDirectByteBuffer::ReadWriteDirectByteBuffer(
    /* [in] */ SafeAddress* address,
    /* [in] */ Int32 capacity,
    /* [in] */ Int32 offset)
    : DirectByteBuffer(address, capacity, offset)
{}

//ReadWriteDirectByteBuffer::ReadWriteDirectByteBuffer(
//    /* [in] */ IPlatformAddress* address,
//    /* [in] */ Int32 aCapacity,
//    /* [in] */ Int32 anOffset)
// {
//     DirectByteBuffer::Init(new SafeAddress(address), aCapacity, anOffset);
// }

ECode ReadWriteDirectByteBuffer::GetAddress(
    /* [out] */ Int32* addr)
{
//    return mSafeAddress->mAddress->ToInt(addr);
    return E_NOT_IMPLEMENTED;
}

ECode ReadWriteDirectByteBuffer::Array(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    return DirectByteBuffer::Array(array);
}

ECode ReadWriteDirectByteBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return DirectByteBuffer::ArrayOffset(offset);
}

ECode ReadWriteDirectByteBuffer::AsCharBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    return DirectByteBuffer::AsCharBuffer(buffer);
}

ECode ReadWriteDirectByteBuffer::AsDoubleBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    return DirectByteBuffer::AsDoubleBuffer(buffer);
}

ECode ReadWriteDirectByteBuffer::AsFloatBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    return DirectByteBuffer::AsFloatBuffer(buffer);
}

ECode ReadWriteDirectByteBuffer::AsInt16Buffer(
    /* [out] */ IInt16Buffer** buffer)
{
    return DirectByteBuffer::AsInt16Buffer(buffer);
}

ECode ReadWriteDirectByteBuffer::AsInt32Buffer(
    /* [out] */ IInt32Buffer** buffer)
{
    return DirectByteBuffer::AsInt32Buffer(buffer);
}

ECode ReadWriteDirectByteBuffer::AsInt64Buffer(
    /* [out] */ IInt64Buffer** buffer)
{
    return DirectByteBuffer::AsInt64Buffer(buffer);
}

ECode ReadWriteDirectByteBuffer::AsReadOnlyBuffer(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadOnlyDirectByteBuffer* buf = ReadOnlyDirectByteBuffer::Copy((DirectByteBuffer*)this, mMark);
    if (buf->Probe(EIID_IByteBuffer) != NULL) {
        *buffer = (IByteBuffer*)buf->Probe(EIID_IByteBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::Compact()
{
//    PlatformAddress effectiveAddress = getEffectiveAddress();
//    effectiveAddress.offsetBytes(position).moveTo(effectiveAddress, remaining());
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = Buffer_UNSET_MARK;

    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::CompareTo(
    /* [in] */ IByteBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return DirectByteBuffer::CompareTo(otherBuffer, result);
}

ECode ReadWriteDirectByteBuffer::Duplicate(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadWriteDirectByteBuffer* buf = Copy((DirectByteBuffer*)this, mMark);
    if (buf->Probe(EIID_IByteBuffer) != NULL) {
        *buffer = (IByteBuffer*)buf->Probe(EIID_IByteBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::GetByte(
    /* [out] */ Byte* value)
{
    return DirectByteBuffer::GetByte(value);
}

ECode ReadWriteDirectByteBuffer::GetByteEx(
    /* [in] */ Int32 index,
    /* [out] */ Byte* value)
{
    return DirectByteBuffer::GetByteEx(index, value);
}

ECode ReadWriteDirectByteBuffer::GetBytes(
    /* [out] */ ArrayOf<Byte>* dst)
{
    return DirectByteBuffer::GetBytes(dst);
}

ECode ReadWriteDirectByteBuffer::GetBytesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Byte>* dst)
{
    return DirectByteBuffer::GetBytesEx(off, len, dst);
}

ECode ReadWriteDirectByteBuffer::GetChar(
    /* [out] */ Char32* value)
{
    return DirectByteBuffer::GetChar(value);
}

ECode ReadWriteDirectByteBuffer::GetCharEx(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    return DirectByteBuffer::GetCharEx(index, value);
}

ECode ReadWriteDirectByteBuffer::GetDouble(
    /* [out] */ Double* value)
{
    return DirectByteBuffer::GetDouble(value);
}

ECode ReadWriteDirectByteBuffer::GetDoubleEx(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    return DirectByteBuffer::GetDoubleEx(index, value);
}

ECode ReadWriteDirectByteBuffer::GetFloat(
    /* [out] */ Float* value)
{
    return DirectByteBuffer::GetFloat(value);
}

ECode ReadWriteDirectByteBuffer::GetFloatEx(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    return DirectByteBuffer::GetFloatEx(index, value);
}

ECode ReadWriteDirectByteBuffer::GetInt32(
    /* [out] */ Int32* value)
{
    return DirectByteBuffer::GetInt32(value);
}

ECode ReadWriteDirectByteBuffer::GetInt32Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    return DirectByteBuffer::GetInt32Ex(index, value);
}

ECode ReadWriteDirectByteBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    return DirectByteBuffer::GetInt64(value);
}

ECode ReadWriteDirectByteBuffer::GetInt64Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    return DirectByteBuffer::GetInt64Ex(index, value);
}

ECode ReadWriteDirectByteBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    return DirectByteBuffer::GetInt16(value);
}

ECode ReadWriteDirectByteBuffer::GetInt16Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    return DirectByteBuffer::GetInt16Ex(index, value);
}

ECode ReadWriteDirectByteBuffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    return DirectByteBuffer::GetOrder(byteOrder);
}

ECode ReadWriteDirectByteBuffer::SetOrder(
    /* [in] */ ByteOrder byteOrder)
{
    return DirectByteBuffer::SetOrder(byteOrder);
}

ECode ReadWriteDirectByteBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    return DirectByteBuffer::ProtectedArray(array);
}

ECode ReadWriteDirectByteBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return DirectByteBuffer::ProtectedArrayOffset(offset);
}

ECode ReadWriteDirectByteBuffer::ProtectedHasArray(
    /* [out] */ Boolean* hasArray)
{
    return DirectByteBuffer::ProtectedHasArray(hasArray);
}

ECode ReadWriteDirectByteBuffer::PutByte(
    /* [in] */ Byte b)
{
    if (mPosition == mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
//    GetBaseAddress().setByte(offset + position++, value);
    return E_NOT_IMPLEMENTED;
}

ECode ReadWriteDirectByteBuffer::PutByteEx(
    /* [in] */ Int32 index,
    /* [in] */ Byte b)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
//    getBaseAddress().setByte(offset + index, value);
    return E_NOT_IMPLEMENTED;
}

ECode ReadWriteDirectByteBuffer::PutBytes(
    /* [in] */ const ArrayOf<Byte>& src)
{
    return DirectByteBuffer::PutBytes(src);
}

ECode ReadWriteDirectByteBuffer::PutBytesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Byte>& src)
{
    Int32 length = src.GetLength();
    if (off < 0 || len < 0 || (Int64) off + (Int64) len > length) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    Int32 remaining;
    Remaining(&remaining);
    if (len > remaining) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
//    getBaseAddress().setByteArray(offset + position, src, off, len);
    mPosition += len;

    return E_NOT_IMPLEMENTED;
}

ECode ReadWriteDirectByteBuffer::PutByteBuffer(
    /* [in] */ IByteBuffer* src)
{
    return DirectByteBuffer::PutByteBuffer(src);
}

ECode ReadWriteDirectByteBuffer::PutInt16s(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Int16>& src)
{
    Int32 length = src.GetLength();
    if (off < 0 || len < 0 || (Int64)off + (Int64)len > length) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    Int32 remaining;
    Remaining(&remaining);
    if (len << 1 > remaining) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    Boolean isReadOnly;
    IsReadOnly(&isReadOnly);
    if (isReadOnly) {
        return E_READ_ONLY_BUFFER_EXCEPTION;
//        throw new ReadOnlyBufferException();
    }
//    Boolean swap = order() != ByteOrder.nativeOrder();
//    getBaseAddress().setShortArray(offset + position, src, off, len, swap);
    mPosition += len << 1;
    return E_NOT_IMPLEMENTED;
}

ECode ReadWriteDirectByteBuffer::PutInt32s(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Int32>& src)
{
    Int32 length = src.GetLength();
    if (off < 0 || len < 0 || (Int64)off + (Int64)len > length) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    Int32 remaining;
    Remaining(&remaining);
    if (len << 2 > remaining) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    Boolean isReadOnly;
    IsReadOnly(&isReadOnly);
    if (isReadOnly) {
        return E_READ_ONLY_BUFFER_EXCEPTION;
//        throw new ReadOnlyBufferException();
    }
//    boolean swap = order() != ByteOrder.nativeOrder();
//    getBaseAddress().setIntArray(offset + position, src, off, len, swap);
    mPosition += len << 2;
    return E_NOT_IMPLEMENTED;
}

ECode ReadWriteDirectByteBuffer::PutFloats(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Float>& src)
{
    Int32 length = src.GetLength();
    if (off < 0 || len < 0 || (Int64)off + (Int64)len > length) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    Int32 remaining;
    Remaining(&remaining);
    if (len << 2 > remaining) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    Boolean isReadOnly;
    IsReadOnly(&isReadOnly);
    if (isReadOnly) {
        return E_READ_ONLY_BUFFER_EXCEPTION;
//        throw new ReadOnlyBufferException();
    }
//    boolean swap = order() != ByteOrder.nativeOrder();
//    getBaseAddress().setFloatArray(offset + position, src, off, len, swap);
    mPosition += len << 2;
    return E_NOT_IMPLEMENTED;
}

ECode ReadWriteDirectByteBuffer::PutChar(
    /* [in] */ Char32 value)
{
    return DirectByteBuffer::PutChar(value);
}

ECode ReadWriteDirectByteBuffer::PutCharEx(
    /* [in] */ Int32 index,
    /* [in] */ Char32 value)
{
    return DirectByteBuffer::PutCharEx(index, value);
}

ECode ReadWriteDirectByteBuffer::PutDouble(
    /* [in] */ Double value)
{
    Int32 newPosition = mPosition + 8;
    if (newPosition > mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
//    getBaseAddress().setDouble(offset + position, value, order);
    mPosition = newPosition;

    return E_NOT_IMPLEMENTED;
}

ECode ReadWriteDirectByteBuffer::PutDoubleEx(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    if (index < 0 || (Int64) index + 8 > mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
//    getBaseAddress().setDouble(offset + index, value, order);
    return E_NOT_IMPLEMENTED;
}

ECode ReadWriteDirectByteBuffer::PutFloat(
    /* [in] */ Float value)
{
    Int32 newPosition = mPosition + 4;
    if (newPosition > mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
//    getBaseAddress().setFloat(offset + position, value, order);
    mPosition = newPosition;
    return E_NOT_IMPLEMENTED;
}

ECode ReadWriteDirectByteBuffer::PutFloatEx(
    /* [in] */ Int32 index,
    /* [in] */ Float value)
{
    if (index < 0 || (Int64) index + 4 > mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
//    getBaseAddress().setFloat(offset + index, value, order);
    return E_NOT_IMPLEMENTED;
}

ECode ReadWriteDirectByteBuffer::PutInt16(
    /* [in] */ Int16 value)
{
    Int32 newPosition = mPosition + 2;
    if (newPosition > mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
//    getBaseAddress().putShort(offset + position, value, order);
    mPosition = newPosition;
    return E_NOT_IMPLEMENTED;
}

ECode ReadWriteDirectByteBuffer::PutInt16Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int16 value)
{
    if (index < 0 || (Int64) index + 2 > mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
//    getBaseAddress().setShort(offset + index, value, order);
    return E_NOT_IMPLEMENTED;
}

ECode ReadWriteDirectByteBuffer::PutInt32(
    /* [in] */ Int32 value)
{
    Int32 newPosition = mPosition + 4;
    if (newPosition > mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
//    getBaseAddress().setInt(offset + position, value, order);
    mPosition = newPosition;
    return E_NOT_IMPLEMENTED;
}

ECode ReadWriteDirectByteBuffer::PutInt32Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    if (index < 0 || (Int64) index + 4 > mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
//    getBaseAddress().setInt(offset + index, value, order);
    return E_NOT_IMPLEMENTED;
}

ECode ReadWriteDirectByteBuffer::PutInt64(
    /* [in] */ Int64 value)
{
    Int32 newPosition = mPosition + 8;
    if (newPosition > mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
//    getBaseAddress().setLong(offset + position, value, order);
    mPosition = newPosition;
    return E_NOT_IMPLEMENTED;
}

ECode ReadWriteDirectByteBuffer::PutInt64Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    if (index < 0 || (Int64) index + 8 > mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
//    getBaseAddress().setLong(offset + index, value, order);
    return E_NOT_IMPLEMENTED;
}

ECode ReadWriteDirectByteBuffer::Slice(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    Int32 remaining;
    Remaining(&remaining);
    ReadWriteDirectByteBuffer* buf = new ReadWriteDirectByteBuffer(
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

ECode ReadWriteDirectByteBuffer::Capacity(
    /* [out] */ Int32* cap)
{
    return DirectByteBuffer::Capacity(cap);
}

ECode ReadWriteDirectByteBuffer::Clear()
{
    return DirectByteBuffer::Clear();
}

ECode ReadWriteDirectByteBuffer::Flip()
{
    return DirectByteBuffer::Flip();
}

ECode ReadWriteDirectByteBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return DirectByteBuffer::HasArray(hasArray);
}

ECode ReadWriteDirectByteBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return DirectByteBuffer::HasRemaining(hasRemaining);
}

ECode ReadWriteDirectByteBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return DirectByteBuffer::IsDirect(isDirect);
}

ECode ReadWriteDirectByteBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = FALSE;
    return NOERROR;
}

ECode ReadWriteDirectByteBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return DirectByteBuffer::GetLimit(limit);
}

ECode ReadWriteDirectByteBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return DirectByteBuffer::SetLimit(newLimit);
}

ECode ReadWriteDirectByteBuffer::Mark()
{
    return DirectByteBuffer::Mark();
}

ECode ReadWriteDirectByteBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return DirectByteBuffer::GetPosition(pos);
}

ECode ReadWriteDirectByteBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return DirectByteBuffer::SetPosition(newPosition);
}

ECode ReadWriteDirectByteBuffer::Remaining(
    /* [out] */ Int32* remaining)
{
    return DirectByteBuffer::Remaining(remaining);
}

ECode ReadWriteDirectByteBuffer::Reset()
{
    return DirectByteBuffer::Reset();
}

ECode ReadWriteDirectByteBuffer::Rewind()
{
    return DirectByteBuffer::Rewind();
}

