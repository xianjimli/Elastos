
#include "cmdef.h"
#include "ByteBuffer.h"
#include "BufferFactory.h"
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>

// {AAD41A09-77D1-491c-A2A0-0C7DB0B379E6}
extern "C" const InterfaceID EIID_ByteBuffer =
    { 0xaad41a09, 0x77d1, 0x491c, { 0xa2, 0xa0, 0xc, 0x7d, 0xb0, 0xb3, 0x79, 0xe6 } };

ECode ByteBuffer::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ IByteBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    if (capacity < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }
    return BufferFactory::NewByteBufferEx(capacity, buf);
}

ECode ByteBuffer::AllocateDirect(
    /* [in] */ Int32 capacity,
    /* [out] */ IByteBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    if (capacity < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }
    return BufferFactory::NewDirectByteBuffer(capacity, buf);
}

ECode ByteBuffer::Wrap(
    /* [in] */ ArrayOf<Byte>* array,
    /* [out] */ IByteBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);
    return BufferFactory::NewByteBuffer(array, buf);
}

ECode ByteBuffer::WrapEx(
    /* [in] */ ArrayOf<Byte>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 len,
    /* [out] */ IByteBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    Int32 length = array->GetLength();
    if ((start < 0) || (len < 0) || ((Int64) start + (Int64) len > length)) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    AutoPtr<IByteBuffer> byteBuf;
    BufferFactory::NewByteBuffer(array, (IByteBuffer**)&byteBuf);
    ByteBuffer* bufCls = (ByteBuffer*)byteBuf->Probe(EIID_ByteBuffer);
    bufCls->mPosition = start;
    bufCls->mLimit = start + len;

    *buf = byteBuf;
    return NOERROR;
}

ByteBuffer::ByteBuffer(
    /* [in] */ Int32 capacity)
    : Buffer(capacity)
    , mOrder(ByteOrder_BIG_ENDIAN)
{
    mElementSizeShift = 0;
}

ECode ByteBuffer::Array(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    return ProtectedArray(array);
}

ECode ByteBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return ProtectedArrayOffset(offset);
}

ECode ByteBuffer::CompareTo(
    /* [in] */ IByteBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 remaining, otherRemaining;
    Remaining(&remaining);
    otherBuffer->Remaining(&otherRemaining);
    Int32 compareRemaining = (remaining < otherRemaining) ? remaining : otherRemaining;
    Int32 thisPos = mPosition;
    Int32 otherPos;
    otherBuffer->GetPosition(&otherPos);
    Byte thisByte, otherByte;
    while (compareRemaining > 0) {
        GetByteEx(thisPos, &thisByte);
        otherBuffer->GetByteEx(otherPos, &otherByte);
        if (thisByte != otherByte) {
            *result = thisByte < otherByte ? -1 : 1;
            return NOERROR;
        }
        thisPos++;
        otherPos++;
        compareRemaining--;
    }
    *result = remaining - otherRemaining;

    return NOERROR;
}

ECode ByteBuffer::GetBytes(
    /* [out] */ ArrayOf<Byte>* dst)
{
    return GetBytesEx(0, dst->GetLength(), dst);
}

ECode ByteBuffer::GetBytesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Byte>* dst)
{
    VALIDATE_NOT_NULL(dst);

    Int32 length = dst->GetLength();
    if ((off < 0) || (len < 0) || ((Int64) off + (Int64) len > length)) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 remaining;
    Remaining(&remaining);
    if (len > remaining) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    for (Int32 i = off; i < off + len; i++) {
        GetByte(&(*dst)[i]);
    }

    return NOERROR;
}

ECode ByteBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return ProtectedHasArray(hasArray);
}

ECode ByteBuffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    VALIDATE_NOT_NULL(byteOrder);
    *byteOrder = mOrder;
    return NOERROR;
}

ECode ByteBuffer::SetOrder(
    /* [in] */ ByteOrder byteOrder)
{
    return SetOrderImpl(byteOrder);
}

ECode ByteBuffer::SetOrderImpl(
    /* [in] */ ByteOrder byteOrder)
{
    mOrder = byteOrder;
    return NOERROR;
}

ECode ByteBuffer::PutBytes(
    /* [in] */ const ArrayOf<Byte>& src)
{
    return PutBytesEx(0, src.GetLength(), src);
}

ECode ByteBuffer::PutBytesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Byte>& src)
{
    Int32 length = src.GetLength();
    if ((off < 0 ) || (len < 0) || ((Int64)off + (Int64)len > length)) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    Int32 remaining;
    Remaining(&remaining);
    if (len > remaining) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    for (Int32 i = off; i < off + len; i++) {
        PutByte(src[i]);
    }

    return NOERROR;
}

ECode ByteBuffer::PutByteBuffer(
    /* [in] */ IByteBuffer* src)
{
    if (src == (IByteBuffer*)this->Probe(EIID_IByteBuffer)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }
    Int32 srcRemaining, remaining;
    src->Remaining(&srcRemaining);
    Remaining(&remaining);
    if (srcRemaining > remaining) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    ArrayOf<Byte>* contents = ArrayOf<Byte>::Alloc(remaining);
    src->GetBytes(contents);
    PutBytes(*contents);
    ArrayOf<Byte>::Free(contents);

    return NOERROR;
}

ECode ByteBuffer::Equals(
    /* [in]  */ IInterface* other,
    /* [out] */ Boolean* isEquals)
{
    ByteBuffer* otherObj = (ByteBuffer*)(other->Probe(EIID_ByteBuffer));
    if (NULL == otherObj) {
        *isEquals = false;
        return NOERROR;
    }

    Int32 remaining;
    Int32 otherRemaining;
    Remaining(&remaining);
    otherObj->Remaining(&otherRemaining);
    if (remaining != otherRemaining) {
        *isEquals = false;
        return NOERROR;
    }

    int myPosition      = mPosition;
    int otherPosition   = otherObj->mPosition;

    *isEquals = true;
    Byte value;
    Byte otherValue;
    while (*isEquals && (myPosition < mLimit)) {
        GetByteEx(myPosition++, &value);
        otherObj->GetByteEx(otherPosition++, &otherValue);
        *isEquals = (value == otherValue);
    }

    return NOERROR;
}

String ByteBuffer::ToString()
{
    StringBuffer buf;
//    buf.append(getClass().getName());
    buf += "ByteBuffer";
    buf += ", status: capacity=";
    Int32 cap, pos, limit;
    Capacity(&cap);
    GetPosition(&pos);
    GetLimit(&limit);
    buf += cap;
    buf += " position=";
    buf += pos;
    buf += " limit=";
    buf += limit;
    return String(buf);
}
