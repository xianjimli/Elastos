
#include "cmdef.h"
#include "FloatToByteBufferAdapter.h"
#include "ReadWriteDirectByteBuffer.h"

PInterface FloatToByteBufferAdapter::Probe(
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


UInt32 FloatToByteBufferAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 FloatToByteBufferAdapter::Release()
{
    return ElRefBase::Release();
}

ECode FloatToByteBufferAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode FloatToByteBufferAdapter::Wrap(
    /* [in] */ IByteBuffer* byteBuffer,
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> buf;
    byteBuffer->Slice((IByteBuffer**)&buf);
    Int32 cap;
    buf->Capacity(&cap);
    FloatToByteBufferAdapter* adapter = new FloatToByteBufferAdapter(cap, buf);
    if (adapter->Probe(EIID_IFloatBuffer) != NULL) {
        *buffer = (IFloatBuffer*)adapter->Probe(EIID_IFloatBuffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

FloatToByteBufferAdapter::FloatToByteBufferAdapter(
    /* [in] */ Int32 capacity,
    /* [in] */ IByteBuffer* byteBuffer)
    : FloatBuffer(capacity >> 2)
    , mByteBuffer(byteBuffer)
{
    mByteBuffer->Clear();
}

Int32 FloatToByteBufferAdapter::GetByteCapacity()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     return ((DirectBuffer) byteBuffer).getByteCapacity();
    // }
    // assert false : byteBuffer;
    return -1;
}

//    PlatformAddress getEffectiveAddress();

//    PlatformAddress getBaseAddress();

Boolean FloatToByteBufferAdapter::IsAddressValid()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     return ((DirectBuffer) byteBuffer).isAddressValid();
    // }
    // assert false : byteBuffer;
    return FALSE;
}

void FloatToByteBufferAdapter::AddressValidityCheck()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     ((DirectBuffer) byteBuffer).addressValidityCheck();
    // } else {
    //     assert false : byteBuffer;
    // }
}

void FloatToByteBufferAdapter::Free()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     ((DirectBuffer) byteBuffer).free();
    // } else {
    //     assert false : byteBuffer;
    // }
}

ECode FloatToByteBufferAdapter::Array(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    return FloatBuffer::Array(array);
}

ECode FloatToByteBufferAdapter::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return FloatBuffer::ArrayOffset(offset);
}

ECode FloatToByteBufferAdapter::AsReadOnlyBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> byteBuf;
    mByteBuffer->AsReadOnlyBuffer((IByteBuffer**)&byteBuf);
    Int32 cap;
    byteBuf->Capacity(&cap);
    FloatToByteBufferAdapter* buf = new FloatToByteBufferAdapter(cap, byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    if (buf->Probe(EIID_IFloatBuffer) != NULL) {
        *buffer = (IFloatBuffer*)buf->Probe(EIID_IFloatBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode FloatToByteBufferAdapter::Compact()
{
    Boolean isReadOnly;
    mByteBuffer->IsReadOnly(&isReadOnly);
    if (isReadOnly) {
        return E_READ_ONLY_BUFFER_EXCEPTION;
//        throw new ReadOnlyBufferException();
    }
    mByteBuffer->SetLimit(mLimit << 2);
    mByteBuffer->SetPosition(mPosition << 2);
    mByteBuffer->Compact();
    mByteBuffer->Clear();
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = Buffer_UNSET_MARK;

    return NOERROR;
}

ECode FloatToByteBufferAdapter::CompareTo(
    /* [in] */ IFloatBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return FloatBuffer::CompareTo(otherBuffer, result);
}

ECode FloatToByteBufferAdapter::Duplicate(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> byteBuf;
    mByteBuffer->Duplicate((IByteBuffer**)&byteBuf);
    Int32 cap;
    byteBuf->Capacity(&cap);
    FloatToByteBufferAdapter* buf = new FloatToByteBufferAdapter(cap, byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    if (buf->Probe(EIID_IFloatBuffer) != NULL) {
        *buffer = (IFloatBuffer*)buf->Probe(EIID_IFloatBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode FloatToByteBufferAdapter::GetFloat(
    /* [out] */ Float* value)
{
    if (mPosition == mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    return mByteBuffer->GetFloatEx(mPosition++ << 2, value);
}

ECode FloatToByteBufferAdapter::GetFloatEx(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    return mByteBuffer->GetFloatEx(index << 2, value);
}

ECode FloatToByteBufferAdapter::GetFloats(
    /* [out] */ ArrayOf<Float>* dst)
{
    return FloatBuffer::GetFloats(dst);
}

ECode FloatToByteBufferAdapter::GetFloatsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Float>* dst)
{
    return FloatBuffer::GetFloatsEx(off, len, dst);
}

ECode FloatToByteBufferAdapter::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return mByteBuffer->GetOrder(order);
}

ECode FloatToByteBufferAdapter::PutFloat(
    /* [in] */ Float c)
{
    if (mPosition == mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    return mByteBuffer->PutFloatEx(mPosition++ << 2, c);
}

ECode FloatToByteBufferAdapter::PutFloatEx(
    /* [in] */ Int32 index,
    /* [in] */ Float c)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    return mByteBuffer->PutFloatEx(index << 2, c);
}

ECode FloatToByteBufferAdapter::PutFloats(
    /* [in] */ const ArrayOf<Float>& src)
{
    return FloatBuffer::PutFloats(src);
}

ECode FloatToByteBufferAdapter::PutFloatsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Float>& src)
{
    if (mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer) != NULL) {
        mByteBuffer->SetLimit(mLimit << 2);
        mByteBuffer->SetPosition(mPosition << 2);
        ReadWriteDirectByteBuffer* buf =
               (ReadWriteDirectByteBuffer*)mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer);
        buf->PutFloats(off, len, src);
        mPosition += len;
        return NOERROR;
    }
    else {
        return FloatBuffer::PutFloatsEx(off, len, src);
    }
}

ECode FloatToByteBufferAdapter::PutFloatBuffer(
    /* [in] */ IFloatBuffer* src)
{
    return FloatBuffer::PutFloatBuffer(src);
}

ECode FloatToByteBufferAdapter::Slice(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    mByteBuffer->SetLimit(mLimit << 2);
    mByteBuffer->SetPosition(mPosition << 2);
    AutoPtr<IByteBuffer> byteBuf;
    mByteBuffer->Slice((IByteBuffer**)&byteBuf);
    assert(byteBuf);
    Int32 cap;
    byteBuf->Capacity(&cap);
    FloatToByteBufferAdapter* result = new FloatToByteBufferAdapter(cap, byteBuf);
    if (result->Probe(EIID_IFloatBuffer) != NULL) {
        *buffer = (IFloatBuffer*)result->Probe(EIID_IFloatBuffer);
    }
    else {
        *buffer = NULL;
    }
    mByteBuffer->Clear();

    return NOERROR;
}

ECode FloatToByteBufferAdapter::Capacity(
    /* [out] */ Int32* cap)
{
    return FloatBuffer::Capacity(cap);
}

ECode FloatToByteBufferAdapter::Clear()
{
    return FloatBuffer::Clear();
}

ECode FloatToByteBufferAdapter::Flip()
{
    return FloatBuffer::Flip();
}

ECode FloatToByteBufferAdapter::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return FloatBuffer::HasArray(hasArray);
}

ECode FloatToByteBufferAdapter::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return FloatBuffer::HasRemaining(hasRemaining);
}

ECode FloatToByteBufferAdapter::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return mByteBuffer->IsDirect(isDirect);
}

ECode FloatToByteBufferAdapter::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    return mByteBuffer->IsReadOnly(isReadOnly);
}

CARAPI FloatToByteBufferAdapter::ProtectedArray(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
//    throw new UnsupportedOperationException();
}

CARAPI FloatToByteBufferAdapter::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
//    throw new UnsupportedOperationException();
}

CARAPI FloatToByteBufferAdapter::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode FloatToByteBufferAdapter::GetLimit(
    /* [out] */ Int32* limit)
{
    return FloatBuffer::GetLimit(limit);
}

ECode FloatToByteBufferAdapter::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return FloatBuffer::SetLimit(newLimit);
}

ECode FloatToByteBufferAdapter::Mark()
{
    return FloatBuffer::Mark();
}

ECode FloatToByteBufferAdapter::GetPosition(
    /* [out] */ Int32* pos)
{
    return FloatBuffer::GetPosition(pos);
}

ECode FloatToByteBufferAdapter::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return FloatBuffer::SetPosition(newPosition);
}

ECode FloatToByteBufferAdapter::Remaining(
    /* [out] */ Int32* remaining)
{
    return FloatBuffer::Remaining(remaining);
}

ECode FloatToByteBufferAdapter::Reset()
{
    return FloatBuffer::Reset();
}

ECode FloatToByteBufferAdapter::Rewind()
{
    return FloatBuffer::Rewind();
}
