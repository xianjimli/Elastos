
#include "cmdef.h"
#include "ReadOnlyInt16ArrayBuffer.h"

PInterface ReadOnlyInt16ArrayBuffer::Probe(
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


UInt32 ReadOnlyInt16ArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadOnlyInt16ArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadOnlyInt16ArrayBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ReadOnlyInt16ArrayBuffer* ReadOnlyInt16ArrayBuffer::Copy(
    /* [in] */ Int16ArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    Int32 capacity;
    other->Capacity(&capacity);
    ReadOnlyInt16ArrayBuffer* buf = new ReadOnlyInt16ArrayBuffer(capacity,
            other->mBackingArray, other->mOffset);
    other->GetLimit(&buf->mLimit);
    other->GetPosition(&buf->mPosition);
    buf->mMark = markOfOther;
    return buf;
}

ReadOnlyInt16ArrayBuffer::ReadOnlyInt16ArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Int16>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : Int16ArrayBuffer(capacity, backingArray, arrayOffset)
{}

ECode ReadOnlyInt16ArrayBuffer::Array(
    /* [out, callee] */ ArrayOf<Int16>** array)
{
    return Int16ArrayBuffer::Array(array);
}

ECode ReadOnlyInt16ArrayBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return Int16ArrayBuffer::ArrayOffset(offset);
}

ECode ReadOnlyInt16ArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IInt16Buffer** buffer)
{
    return Duplicate(buffer);
}

ECode ReadOnlyInt16ArrayBuffer::Compact()
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

ECode ReadOnlyInt16ArrayBuffer::CompareTo(
    /* [in] */ IInt16Buffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return Int16ArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadOnlyInt16ArrayBuffer::Duplicate(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadOnlyInt16ArrayBuffer* buf = Copy((Int16ArrayBuffer*)this, mMark);
    if (buf->Probe(EIID_IInt16Buffer) != NULL) {
        *buffer = (IInt16Buffer*)buf->Probe(EIID_IInt16Buffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadOnlyInt16ArrayBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    return Int16ArrayBuffer::GetInt16(value);
}

ECode ReadOnlyInt16ArrayBuffer::GetInt16Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    return Int16ArrayBuffer::GetInt16Ex(index, value);
}

ECode ReadOnlyInt16ArrayBuffer::GetInt16s(
    /* [out] */ ArrayOf<Int16>* dst)
{
    return Int16ArrayBuffer::GetInt16s(dst);
}

ECode ReadOnlyInt16ArrayBuffer::GetInt16sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Int16>* dst)
{
    return Int16ArrayBuffer::GetInt16sEx(off, len, dst);
}

ECode ReadOnlyInt16ArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return Int16ArrayBuffer::GetOrder(order);
}

ECode ReadOnlyInt16ArrayBuffer::PutInt16(
    /* [in] */ Int16 c)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyInt16ArrayBuffer::PutInt16Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int16 c)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyInt16ArrayBuffer::PutInt16s(
    /* [in] */ const ArrayOf<Int16>& src)
{
    return Int16ArrayBuffer::PutInt16s(src);
}

ECode ReadOnlyInt16ArrayBuffer::PutInt16sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Int16>& src)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyInt16ArrayBuffer::PutInt16Buffer(
    /* [in] */ IInt16Buffer* src)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyInt16ArrayBuffer::Slice(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining;
    Remaining(&remaining);
    ReadOnlyInt16ArrayBuffer* buf = new ReadOnlyInt16ArrayBuffer(remaining, mBackingArray,
            mOffset + mPosition);
    if (buf->Probe(EIID_IInt16Buffer) != NULL) {
        *buffer = (IInt16Buffer*)buf->Probe(EIID_IInt16Buffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

ECode ReadOnlyInt16ArrayBuffer::Capacity(
    /* [out] */ Int32* cap)
{
    return Int16ArrayBuffer::Capacity(cap);
}

ECode ReadOnlyInt16ArrayBuffer::Clear()
{
    return Int16ArrayBuffer::Clear();
}

ECode ReadOnlyInt16ArrayBuffer::Flip()
{
    return Int16ArrayBuffer::Flip();
}

ECode ReadOnlyInt16ArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return Int16ArrayBuffer::HasArray(hasArray);
}

ECode ReadOnlyInt16ArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return Int16ArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadOnlyInt16ArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return Int16ArrayBuffer::IsDirect(isDirect);
}

ECode ReadOnlyInt16ArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

CARAPI ReadOnlyInt16ArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int16>** array)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

CARAPI ReadOnlyInt16ArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

CARAPI ReadOnlyInt16ArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode ReadOnlyInt16ArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return Int16ArrayBuffer::GetLimit(limit);
}

ECode ReadOnlyInt16ArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return Int16ArrayBuffer::SetLimit(newLimit);
}

ECode ReadOnlyInt16ArrayBuffer::Mark()
{
    return Int16ArrayBuffer::Mark();
}

ECode ReadOnlyInt16ArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return Int16ArrayBuffer::GetPosition(pos);
}

ECode ReadOnlyInt16ArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return Int16ArrayBuffer::SetPosition(newPosition);
}

ECode ReadOnlyInt16ArrayBuffer::Remaining(
    /* [out] */ Int32* remaining)
{
    return Int16ArrayBuffer::Remaining(remaining);
}

ECode ReadOnlyInt16ArrayBuffer::Reset()
{
    return Int16ArrayBuffer::Reset();
}

ECode ReadOnlyInt16ArrayBuffer::Rewind()
{
    return Int16ArrayBuffer::Rewind();
}
