
#include "cmdef.h"
#include "ReadOnlyInt32ArrayBuffer.h"

PInterface ReadOnlyInt32ArrayBuffer::Probe(
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


UInt32 ReadOnlyInt32ArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadOnlyInt32ArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadOnlyInt32ArrayBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ReadOnlyInt32ArrayBuffer* ReadOnlyInt32ArrayBuffer::Copy(
    /* [in] */ Int32ArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    Int32 capacity;
    other->Capacity(&capacity);
    ReadOnlyInt32ArrayBuffer* buf = new ReadOnlyInt32ArrayBuffer(capacity,
            other->mBackingArray, other->mOffset);
    other->GetLimit(&buf->mLimit);
    other->GetPosition(&buf->mPosition);
    buf->mMark = markOfOther;
    return buf;
}

ReadOnlyInt32ArrayBuffer::ReadOnlyInt32ArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Int32>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : Int32ArrayBuffer(capacity, backingArray, arrayOffset)
{}

ECode ReadOnlyInt32ArrayBuffer::Array(
    /* [out, callee] */ ArrayOf<Int32>** array)
{
    return Int32ArrayBuffer::Array(array);
}

ECode ReadOnlyInt32ArrayBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return Int32ArrayBuffer::ArrayOffset(offset);
}

ECode ReadOnlyInt32ArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IInt32Buffer** buffer)
{
    return Duplicate(buffer);
}

ECode ReadOnlyInt32ArrayBuffer::Compact()
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyInt32ArrayBuffer::CompareTo(
    /* [in] */ IInt32Buffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return Int32ArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadOnlyInt32ArrayBuffer::Duplicate(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadOnlyInt32ArrayBuffer* buf = Copy((Int32ArrayBuffer*)this, mMark);
    if (buf->Probe(EIID_IInt32Buffer) != NULL) {
        *buffer = (IInt32Buffer*)buf->Probe(EIID_IInt32Buffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadOnlyInt32ArrayBuffer::GetInt32(
    /* [out] */ Int32* value)
{
    return Int32ArrayBuffer::GetInt32(value);
}

ECode ReadOnlyInt32ArrayBuffer::GetInt32Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    return Int32ArrayBuffer::GetInt32Ex(index, value);
}

ECode ReadOnlyInt32ArrayBuffer::GetInt32s(
    /* [out] */ ArrayOf<Int32>* dst)
{
    return Int32ArrayBuffer::GetInt32s(dst);
}

ECode ReadOnlyInt32ArrayBuffer::GetInt32sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Int32>* dst)
{
    return Int32ArrayBuffer::GetInt32sEx(off, len, dst);
}

ECode ReadOnlyInt32ArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return Int32ArrayBuffer::GetOrder(order);
}

ECode ReadOnlyInt32ArrayBuffer::PutInt32(
    /* [in] */ Int32 c)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyInt32ArrayBuffer::PutInt32Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int32 c)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyInt32ArrayBuffer::PutInt32s(
    /* [in] */ const ArrayOf<Int32>& src)
{
    return Int32ArrayBuffer::PutInt32s(src);
}

ECode ReadOnlyInt32ArrayBuffer::PutInt32sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Int32>& src)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyInt32ArrayBuffer::PutInt32Buffer(
    /* [in] */ IInt32Buffer* src)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyInt32ArrayBuffer::Slice(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining;
    Remaining(&remaining);
    ReadOnlyInt32ArrayBuffer* buf = new ReadOnlyInt32ArrayBuffer(remaining, mBackingArray,
            mOffset + mPosition);
    if (buf->Probe(EIID_IInt32Buffer) != NULL) {
        *buffer = (IInt32Buffer*)buf->Probe(EIID_IInt32Buffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

ECode ReadOnlyInt32ArrayBuffer::Capacity(
    /* [out] */ Int32* cap)
{
    return Int32ArrayBuffer::Capacity(cap);
}

ECode ReadOnlyInt32ArrayBuffer::Clear()
{
    return Int32ArrayBuffer::Clear();
}

ECode ReadOnlyInt32ArrayBuffer::Flip()
{
    return Int32ArrayBuffer::Flip();
}

ECode ReadOnlyInt32ArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return Int32ArrayBuffer::HasArray(hasArray);
}

ECode ReadOnlyInt32ArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return Int32ArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadOnlyInt32ArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return Int32ArrayBuffer::IsDirect(isDirect);
}

ECode ReadOnlyInt32ArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

CARAPI ReadOnlyInt32ArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int32>** array)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

CARAPI ReadOnlyInt32ArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

CARAPI ReadOnlyInt32ArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode ReadOnlyInt32ArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return Int32ArrayBuffer::GetLimit(limit);
}

ECode ReadOnlyInt32ArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return Int32ArrayBuffer::SetLimit(newLimit);
}

ECode ReadOnlyInt32ArrayBuffer::Mark()
{
    return Int32ArrayBuffer::Mark();
}

ECode ReadOnlyInt32ArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return Int32ArrayBuffer::GetPosition(pos);
}

ECode ReadOnlyInt32ArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return Int32ArrayBuffer::SetPosition(newPosition);
}

ECode ReadOnlyInt32ArrayBuffer::Remaining(
    /* [out] */ Int32* remaining)
{
    return Int32ArrayBuffer::Remaining(remaining);
}

ECode ReadOnlyInt32ArrayBuffer::Reset()
{
    return Int32ArrayBuffer::Reset();
}

ECode ReadOnlyInt32ArrayBuffer::Rewind()
{
    return Int32ArrayBuffer::Rewind();
}
