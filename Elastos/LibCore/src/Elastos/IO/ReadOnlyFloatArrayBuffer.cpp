
#include "cmdef.h"
#include "ReadOnlyFloatArrayBuffer.h"

PInterface ReadOnlyFloatArrayBuffer::Probe(
        /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IFloatBuffer) {
        return (IFloatBuffer*)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_FloatBuffer) {
        return reinterpret_cast<PInterface>((FloatBuffer*)this);
    }

    return NULL;
}


UInt32 ReadOnlyFloatArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadOnlyFloatArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadOnlyFloatArrayBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ReadOnlyFloatArrayBuffer* ReadOnlyFloatArrayBuffer::Copy(
    /* [in] */ FloatArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    Int32 capacity;
    other->Capacity(&capacity);
    ReadOnlyFloatArrayBuffer* buf = new ReadOnlyFloatArrayBuffer(capacity,
            other->mBackingArray, other->mOffset);
    other->GetLimit(&buf->mLimit);
    other->GetPosition(&buf->mPosition);
    buf->mMark = markOfOther;
    return buf;
}

ReadOnlyFloatArrayBuffer::ReadOnlyFloatArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Float>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : FloatArrayBuffer(capacity, backingArray, arrayOffset)
{}

ECode ReadOnlyFloatArrayBuffer::Array(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    return FloatArrayBuffer::Array(array);
}

ECode ReadOnlyFloatArrayBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return FloatArrayBuffer::ArrayOffset(offset);
}

ECode ReadOnlyFloatArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    return Duplicate(buffer);
}

ECode ReadOnlyFloatArrayBuffer::Compact()
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

ECode ReadOnlyFloatArrayBuffer::CompareTo(
    /* [in] */ IFloatBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return FloatArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadOnlyFloatArrayBuffer::Duplicate(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadOnlyFloatArrayBuffer* buf = Copy((FloatArrayBuffer*)this, mMark);
    if (buf->Probe(EIID_IFloatBuffer) != NULL) {
        *buffer = (IFloatBuffer*)buf->Probe(EIID_IFloatBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadOnlyFloatArrayBuffer::GetFloat(
    /* [out] */ Float* value)
{
    return FloatArrayBuffer::GetFloat(value);
}

ECode ReadOnlyFloatArrayBuffer::GetFloatEx(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    return FloatArrayBuffer::GetFloatEx(index, value);
}

ECode ReadOnlyFloatArrayBuffer::GetFloats(
    /* [out] */ ArrayOf<Float>* dst)
{
    return FloatArrayBuffer::GetFloats(dst);
}

ECode ReadOnlyFloatArrayBuffer::GetFloatsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Float>* dst)
{
    return FloatArrayBuffer::GetFloatsEx(off, len, dst);
}

ECode ReadOnlyFloatArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return FloatArrayBuffer::GetOrder(order);
}

ECode ReadOnlyFloatArrayBuffer::PutFloat(
    /* [in] */ Float c)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyFloatArrayBuffer::PutFloatEx(
    /* [in] */ Int32 index,
    /* [in] */ Float c)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyFloatArrayBuffer::PutFloats(
    /* [in] */ const ArrayOf<Float>& src)
{
    return FloatArrayBuffer::PutFloats(src);
}

ECode ReadOnlyFloatArrayBuffer::PutFloatsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Float>& src)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyFloatArrayBuffer::PutFloatBuffer(
    /* [in] */ IFloatBuffer* src)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyFloatArrayBuffer::Slice(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining;
    Remaining(&remaining);
    ReadOnlyFloatArrayBuffer* buf = new ReadOnlyFloatArrayBuffer(remaining, mBackingArray,
            mOffset + mPosition);
    if (buf->Probe(EIID_IFloatBuffer) != NULL) {
        *buffer = (IFloatBuffer*)buf->Probe(EIID_IFloatBuffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

ECode ReadOnlyFloatArrayBuffer::Capacity(
    /* [out] */ Int32* cap)
{
    return FloatArrayBuffer::Capacity(cap);
}

ECode ReadOnlyFloatArrayBuffer::Clear()
{
    return FloatArrayBuffer::Clear();
}

ECode ReadOnlyFloatArrayBuffer::Flip()
{
    return FloatArrayBuffer::Flip();
}

ECode ReadOnlyFloatArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return FloatArrayBuffer::HasArray(hasArray);
}

ECode ReadOnlyFloatArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return FloatArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadOnlyFloatArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return FloatArrayBuffer::IsDirect(isDirect);
}

ECode ReadOnlyFloatArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

CARAPI ReadOnlyFloatArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

CARAPI ReadOnlyFloatArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

CARAPI ReadOnlyFloatArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode ReadOnlyFloatArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return FloatArrayBuffer::GetLimit(limit);
}

ECode ReadOnlyFloatArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return FloatArrayBuffer::SetLimit(newLimit);
}

ECode ReadOnlyFloatArrayBuffer::Mark()
{
    return FloatArrayBuffer::Mark();
}

ECode ReadOnlyFloatArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return FloatArrayBuffer::GetPosition(pos);
}

ECode ReadOnlyFloatArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return FloatArrayBuffer::SetPosition(newPosition);
}

ECode ReadOnlyFloatArrayBuffer::Remaining(
    /* [out] */ Int32* remaining)
{
    return FloatArrayBuffer::Remaining(remaining);
}

ECode ReadOnlyFloatArrayBuffer::Reset()
{
    return FloatArrayBuffer::Reset();
}

ECode ReadOnlyFloatArrayBuffer::Rewind()
{
    return FloatArrayBuffer::Rewind();
}
