
#include "cmdef.h"
#include "ReadOnlyInt64ArrayBuffer.h"

PInterface ReadOnlyInt64ArrayBuffer::Probe(
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


UInt32 ReadOnlyInt64ArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadOnlyInt64ArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadOnlyInt64ArrayBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ReadOnlyInt64ArrayBuffer* ReadOnlyInt64ArrayBuffer::Copy(
    /* [in] */ Int64ArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    Int32 capacity;
    other->Capacity(&capacity);
    ReadOnlyInt64ArrayBuffer* buf = new ReadOnlyInt64ArrayBuffer(capacity,
            other->mBackingArray, other->mOffset);
    other->GetLimit(&buf->mLimit);
    other->GetPosition(&buf->mPosition);
    buf->mMark = markOfOther;
    return buf;
}

ReadOnlyInt64ArrayBuffer::ReadOnlyInt64ArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Int64>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : Int64ArrayBuffer(capacity, backingArray, arrayOffset)
{}

ECode ReadOnlyInt64ArrayBuffer::Array(
    /* [out, callee] */ ArrayOf<Int64>** array)
{
    return Int64ArrayBuffer::Array(array);
}

ECode ReadOnlyInt64ArrayBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return Int64ArrayBuffer::ArrayOffset(offset);
}

ECode ReadOnlyInt64ArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IInt64Buffer** buffer)
{
    return Duplicate(buffer);
}

ECode ReadOnlyInt64ArrayBuffer::Compact()
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

ECode ReadOnlyInt64ArrayBuffer::CompareTo(
    /* [in] */ IInt64Buffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return Int64ArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadOnlyInt64ArrayBuffer::Duplicate(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadOnlyInt64ArrayBuffer* buf = Copy((Int64ArrayBuffer*)this, mMark);
    if (buf->Probe(EIID_IInt64Buffer) != NULL) {
        *buffer = (IInt64Buffer*)buf->Probe(EIID_IInt64Buffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadOnlyInt64ArrayBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    return Int64ArrayBuffer::GetInt64(value);
}

ECode ReadOnlyInt64ArrayBuffer::GetInt64Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    return Int64ArrayBuffer::GetInt64Ex(index, value);
}

ECode ReadOnlyInt64ArrayBuffer::GetInt64s(
    /* [out] */ ArrayOf<Int64>* dst)
{
    return Int64ArrayBuffer::GetInt64s(dst);
}

ECode ReadOnlyInt64ArrayBuffer::GetInt64sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Int64>* dst)
{
    return Int64ArrayBuffer::GetInt64sEx(off, len, dst);
}

ECode ReadOnlyInt64ArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return Int64ArrayBuffer::GetOrder(order);
}

ECode ReadOnlyInt64ArrayBuffer::PutInt64(
    /* [in] */ Int64 c)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyInt64ArrayBuffer::PutInt64Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int64 c)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyInt64ArrayBuffer::PutInt64s(
    /* [in] */ const ArrayOf<Int64>& src)
{
    return Int64ArrayBuffer::PutInt64s(src);
}

ECode ReadOnlyInt64ArrayBuffer::PutInt64sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Int64>& src)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyInt64ArrayBuffer::PutInt64Buffer(
    /* [in] */ IInt64Buffer* src)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyInt64ArrayBuffer::Slice(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining;
    Remaining(&remaining);
    ReadOnlyInt64ArrayBuffer* buf = new ReadOnlyInt64ArrayBuffer(remaining, mBackingArray,
            mOffset + mPosition);
    if (buf->Probe(EIID_IInt64Buffer) != NULL) {
        *buffer = (IInt64Buffer*)buf->Probe(EIID_IInt64Buffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

ECode ReadOnlyInt64ArrayBuffer::Capacity(
    /* [out] */ Int32* cap)
{
    return Int64ArrayBuffer::Capacity(cap);
}

ECode ReadOnlyInt64ArrayBuffer::Clear()
{
    return Int64ArrayBuffer::Clear();
}

ECode ReadOnlyInt64ArrayBuffer::Flip()
{
    return Int64ArrayBuffer::Flip();
}

ECode ReadOnlyInt64ArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return Int64ArrayBuffer::HasArray(hasArray);
}

ECode ReadOnlyInt64ArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return Int64ArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadOnlyInt64ArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return Int64ArrayBuffer::IsDirect(isDirect);
}

ECode ReadOnlyInt64ArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

CARAPI ReadOnlyInt64ArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int64>** array)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

CARAPI ReadOnlyInt64ArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

CARAPI ReadOnlyInt64ArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode ReadOnlyInt64ArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return Int64ArrayBuffer::GetLimit(limit);
}

ECode ReadOnlyInt64ArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return Int64ArrayBuffer::SetLimit(newLimit);
}

ECode ReadOnlyInt64ArrayBuffer::Mark()
{
    return Int64ArrayBuffer::Mark();
}

ECode ReadOnlyInt64ArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return Int64ArrayBuffer::GetPosition(pos);
}

ECode ReadOnlyInt64ArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return Int64ArrayBuffer::SetPosition(newPosition);
}

ECode ReadOnlyInt64ArrayBuffer::Remaining(
    /* [out] */ Int32* remaining)
{
    return Int64ArrayBuffer::Remaining(remaining);
}

ECode ReadOnlyInt64ArrayBuffer::Reset()
{
    return Int64ArrayBuffer::Reset();
}

ECode ReadOnlyInt64ArrayBuffer::Rewind()
{
    return Int64ArrayBuffer::Rewind();
}
