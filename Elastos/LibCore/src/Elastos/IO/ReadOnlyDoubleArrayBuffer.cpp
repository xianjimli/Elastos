
#include "cmdef.h"
#include "ReadOnlyDoubleArrayBuffer.h"

PInterface ReadOnlyDoubleArrayBuffer::Probe(
        /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IDoubleBuffer) {
        return (IDoubleBuffer*)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_DoubleBuffer) {
        return reinterpret_cast<PInterface>((DoubleBuffer*)this);
    }

    return NULL;
}


UInt32 ReadOnlyDoubleArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadOnlyDoubleArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadOnlyDoubleArrayBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ReadOnlyDoubleArrayBuffer* ReadOnlyDoubleArrayBuffer::Copy(
    /* [in] */ DoubleArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    Int32 capacity;
    other->Capacity(&capacity);
    ReadOnlyDoubleArrayBuffer* buf = new ReadOnlyDoubleArrayBuffer(capacity,
            other->mBackingArray, other->mOffset);
    other->GetLimit(&buf->mLimit);
    other->GetPosition(&buf->mPosition);
    buf->mMark = markOfOther;
    return buf;
}

ReadOnlyDoubleArrayBuffer::ReadOnlyDoubleArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Double>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : DoubleArrayBuffer(capacity, backingArray, arrayOffset)
{}

ECode ReadOnlyDoubleArrayBuffer::Array(
    /* [out, callee] */ ArrayOf<Double>** array)
{
    return DoubleArrayBuffer::Array(array);
}

ECode ReadOnlyDoubleArrayBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return DoubleArrayBuffer::ArrayOffset(offset);
}

ECode ReadOnlyDoubleArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    return Duplicate(buffer);
}

ECode ReadOnlyDoubleArrayBuffer::Compact()
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDoubleArrayBuffer::CompareTo(
    /* [in] */ IDoubleBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return DoubleArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadOnlyDoubleArrayBuffer::Duplicate(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadOnlyDoubleArrayBuffer* buf = Copy((DoubleArrayBuffer*)this, mMark);
    if (buf->Probe(EIID_IDoubleBuffer) != NULL) {
        *buffer = (IDoubleBuffer*)buf->Probe(EIID_IDoubleBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadOnlyDoubleArrayBuffer::GetDouble(
    /* [out] */ Double* value)
{
    return DoubleArrayBuffer::GetDouble(value);
}

ECode ReadOnlyDoubleArrayBuffer::GetDoubleEx(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    return DoubleArrayBuffer::GetDoubleEx(index, value);
}

ECode ReadOnlyDoubleArrayBuffer::GetDoubles(
    /* [out] */ ArrayOf<Double>* dst)
{
    return DoubleArrayBuffer::GetDoubles(dst);
}

ECode ReadOnlyDoubleArrayBuffer::GetDoublesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Double>* dst)
{
    return DoubleArrayBuffer::GetDoublesEx(off, len, dst);
}

ECode ReadOnlyDoubleArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return DoubleArrayBuffer::GetOrder(order);
}

ECode ReadOnlyDoubleArrayBuffer::PutDouble(
    /* [in] */ Double c)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDoubleArrayBuffer::PutDoubleEx(
    /* [in] */ Int32 index,
    /* [in] */ Double c)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDoubleArrayBuffer::PutDoubles(
    /* [in] */ const ArrayOf<Double>& src)
{
    return DoubleArrayBuffer::PutDoubles(src);
}

ECode ReadOnlyDoubleArrayBuffer::PutDoublesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Double>& src)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDoubleArrayBuffer::PutDoubleBuffer(
    /* [in] */ IDoubleBuffer* src)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyDoubleArrayBuffer::Slice(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining;
    Remaining(&remaining);
    ReadOnlyDoubleArrayBuffer* buf = new ReadOnlyDoubleArrayBuffer(remaining, mBackingArray,
            mOffset + mPosition);
    if (buf->Probe(EIID_IDoubleBuffer) != NULL) {
        *buffer = (IDoubleBuffer*)buf->Probe(EIID_IDoubleBuffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

ECode ReadOnlyDoubleArrayBuffer::Capacity(
    /* [out] */ Int32* cap)
{
    return DoubleArrayBuffer::Capacity(cap);
}

ECode ReadOnlyDoubleArrayBuffer::Clear()
{
    return DoubleArrayBuffer::Clear();
}

ECode ReadOnlyDoubleArrayBuffer::Flip()
{
    return DoubleArrayBuffer::Flip();
}

ECode ReadOnlyDoubleArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return DoubleArrayBuffer::HasArray(hasArray);
}

ECode ReadOnlyDoubleArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return DoubleArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadOnlyDoubleArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return DoubleArrayBuffer::IsDirect(isDirect);
}

ECode ReadOnlyDoubleArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

CARAPI ReadOnlyDoubleArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Double>** array)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

CARAPI ReadOnlyDoubleArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

CARAPI ReadOnlyDoubleArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode ReadOnlyDoubleArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return DoubleArrayBuffer::GetLimit(limit);
}

ECode ReadOnlyDoubleArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return DoubleArrayBuffer::SetLimit(newLimit);
}

ECode ReadOnlyDoubleArrayBuffer::Mark()
{
    return DoubleArrayBuffer::Mark();
}

ECode ReadOnlyDoubleArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return DoubleArrayBuffer::GetPosition(pos);
}

ECode ReadOnlyDoubleArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return DoubleArrayBuffer::SetPosition(newPosition);
}

ECode ReadOnlyDoubleArrayBuffer::Remaining(
    /* [out] */ Int32* remaining)
{
    return DoubleArrayBuffer::Remaining(remaining);
}

ECode ReadOnlyDoubleArrayBuffer::Reset()
{
    return DoubleArrayBuffer::Reset();
}

ECode ReadOnlyDoubleArrayBuffer::Rewind()
{
    return DoubleArrayBuffer::Rewind();
}
