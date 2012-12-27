
#include "cmdef.h"
#include "DoubleToByteBufferAdapter.h"

PInterface DoubleToByteBufferAdapter::Probe(
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


UInt32 DoubleToByteBufferAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 DoubleToByteBufferAdapter::Release()
{
    return ElRefBase::Release();
}

ECode DoubleToByteBufferAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode DoubleToByteBufferAdapter::Wrap(
    /* [in] */ IByteBuffer* byteBuffer,
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> buf;
    byteBuffer->Slice((IByteBuffer**)&buf);
    Int32 cap;
    buf->Capacity(&cap);
    DoubleToByteBufferAdapter* adapter = new DoubleToByteBufferAdapter(cap, buf);
    if (adapter->Probe(EIID_IDoubleBuffer) != NULL) {
        *buffer = (IDoubleBuffer*)adapter->Probe(EIID_IDoubleBuffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

DoubleToByteBufferAdapter::DoubleToByteBufferAdapter(
    /* [in] */ Int32 capacity,
    /* [in] */ IByteBuffer* byteBuffer)
    : DoubleBuffer(capacity >> 3)
    , mByteBuffer(byteBuffer)
{
    mByteBuffer->Clear();
}

Int32 DoubleToByteBufferAdapter::GetByteCapacity()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     return ((DirectBuffer) byteBuffer).getByteCapacity();
    // }
    // assert false : byteBuffer;
    return -1;
}

//    PlatformAddress getEffectiveAddress();

//    PlatformAddress getBaseAddress();

Boolean DoubleToByteBufferAdapter::IsAddressValid()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     return ((DirectBuffer) byteBuffer).isAddressValid();
    // }
    // assert false : byteBuffer;
    return FALSE;
}

void DoubleToByteBufferAdapter::AddressValidityCheck()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     ((DirectBuffer) byteBuffer).addressValidityCheck();
    // } else {
    //     assert false : byteBuffer;
    // }
}

void DoubleToByteBufferAdapter::Free()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     ((DirectBuffer) byteBuffer).free();
    // } else {
    //     assert false : byteBuffer;
    // }
}

ECode DoubleToByteBufferAdapter::Array(
    /* [out, callee] */ ArrayOf<Double>** array)
{
    return DoubleBuffer::Array(array);
}

ECode DoubleToByteBufferAdapter::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return DoubleBuffer::ArrayOffset(offset);
}

ECode DoubleToByteBufferAdapter::AsReadOnlyBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> byteBuf;
    mByteBuffer->AsReadOnlyBuffer((IByteBuffer**)&byteBuf);
    Int32 cap;
    byteBuf->Capacity(&cap);
    DoubleToByteBufferAdapter* buf = new DoubleToByteBufferAdapter(cap, byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    if (buf->Probe(EIID_IDoubleBuffer) != NULL) {
        *buffer = (IDoubleBuffer*)buf->Probe(EIID_IDoubleBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode DoubleToByteBufferAdapter::Compact()
{
    //begin from this
    Boolean isReadOnly;
    mByteBuffer->IsReadOnly(&isReadOnly);
    if (isReadOnly) {
        return E_READ_ONLY_BUFFER_EXCEPTION;
//        throw new ReadOnlyBufferException();
    }
    mByteBuffer->SetLimit(mLimit << 3);
    mByteBuffer->SetPosition(mPosition << 3);
    mByteBuffer->Compact();
    mByteBuffer->Clear();
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = Buffer_UNSET_MARK;

    return NOERROR;
}

ECode DoubleToByteBufferAdapter::CompareTo(
    /* [in] */ IDoubleBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return DoubleBuffer::CompareTo(otherBuffer, result);
}

ECode DoubleToByteBufferAdapter::Duplicate(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> byteBuf;
    mByteBuffer->Duplicate((IByteBuffer**)&byteBuf);
    Int32 cap;
    byteBuf->Capacity(&cap);
    DoubleToByteBufferAdapter* buf = new DoubleToByteBufferAdapter(cap, byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    if (buf->Probe(EIID_IDoubleBuffer) != NULL) {
        *buffer = (IDoubleBuffer*)buf->Probe(EIID_IDoubleBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode DoubleToByteBufferAdapter::GetDouble(
    /* [out] */ Double* value)
{
    if (mPosition == mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    return mByteBuffer->GetDoubleEx(mPosition++ << 3, value);
}

ECode DoubleToByteBufferAdapter::GetDoubleEx(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    return mByteBuffer->GetDoubleEx(index << 3, value);
}

ECode DoubleToByteBufferAdapter::GetDoubles(
    /* [out] */ ArrayOf<Double>* dst)
{
    return DoubleBuffer::GetDoubles(dst);
}

ECode DoubleToByteBufferAdapter::GetDoublesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Double>* dst)
{
    return DoubleBuffer::GetDoublesEx(off, len, dst);
}

ECode DoubleToByteBufferAdapter::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return mByteBuffer->GetOrder(order);
}

ECode DoubleToByteBufferAdapter::PutDouble(
    /* [in] */ Double c)
{
    if (mPosition == mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    return mByteBuffer->PutDoubleEx(mPosition++ << 3, c);
}

ECode DoubleToByteBufferAdapter::PutDoubleEx(
    /* [in] */ Int32 index,
    /* [in] */ Double c)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    return mByteBuffer->PutDoubleEx(index << 3, c);
}

ECode DoubleToByteBufferAdapter::PutDoubles(
    /* [in] */ const ArrayOf<Double>& src)
{
    return DoubleBuffer::PutDoubles(src);
}

ECode DoubleToByteBufferAdapter::PutDoublesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Double>& src)
{
    return DoubleBuffer::PutDoublesEx(off, len, src);
}

ECode DoubleToByteBufferAdapter::PutDoubleBuffer(
    /* [in] */ IDoubleBuffer* src)
{
    return DoubleBuffer::PutDoubleBuffer(src);
}

ECode DoubleToByteBufferAdapter::Slice(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    mByteBuffer->SetLimit(mLimit << 3);
    mByteBuffer->SetPosition(mPosition << 3);
    AutoPtr<IByteBuffer> byteBuf;
    mByteBuffer->Slice((IByteBuffer**)&byteBuf);
    assert(byteBuf);
    Int32 cap;
    byteBuf->Capacity(&cap);
    DoubleToByteBufferAdapter* result = new DoubleToByteBufferAdapter(cap, byteBuf);
    if (result->Probe(EIID_IDoubleBuffer) != NULL) {
        *buffer = (IDoubleBuffer*)result->Probe(EIID_IDoubleBuffer);
    }
    else {
        *buffer = NULL;
    }
    mByteBuffer->Clear();

    return NOERROR;
}

ECode DoubleToByteBufferAdapter::Capacity(
    /* [out] */ Int32* cap)
{
    return DoubleBuffer::Capacity(cap);
}

ECode DoubleToByteBufferAdapter::Clear()
{
    return DoubleBuffer::Clear();
}

ECode DoubleToByteBufferAdapter::Flip()
{
    return DoubleBuffer::Flip();
}

ECode DoubleToByteBufferAdapter::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return DoubleBuffer::HasArray(hasArray);
}

ECode DoubleToByteBufferAdapter::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return DoubleBuffer::HasRemaining(hasRemaining);
}

ECode DoubleToByteBufferAdapter::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return mByteBuffer->IsDirect(isDirect);
}

ECode DoubleToByteBufferAdapter::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    return mByteBuffer->IsReadOnly(isReadOnly);
}

CARAPI DoubleToByteBufferAdapter::ProtectedArray(
    /* [out, callee] */ ArrayOf<Double>** array)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
//    throw new UnsupportedOperationException();
}

CARAPI DoubleToByteBufferAdapter::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
//    throw new UnsupportedOperationException();
}

CARAPI DoubleToByteBufferAdapter::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode DoubleToByteBufferAdapter::GetLimit(
    /* [out] */ Int32* limit)
{
    return DoubleBuffer::GetLimit(limit);
}

ECode DoubleToByteBufferAdapter::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return DoubleBuffer::SetLimit(newLimit);
}

ECode DoubleToByteBufferAdapter::Mark()
{
    return DoubleBuffer::Mark();
}

ECode DoubleToByteBufferAdapter::GetPosition(
    /* [out] */ Int32* pos)
{
    return DoubleBuffer::GetPosition(pos);
}

ECode DoubleToByteBufferAdapter::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return DoubleBuffer::SetPosition(newPosition);
}

ECode DoubleToByteBufferAdapter::Remaining(
    /* [out] */ Int32* remaining)
{
    return DoubleBuffer::Remaining(remaining);
}

ECode DoubleToByteBufferAdapter::Reset()
{
    return DoubleBuffer::Reset();
}

ECode DoubleToByteBufferAdapter::Rewind()
{
    return DoubleBuffer::Rewind();
}
