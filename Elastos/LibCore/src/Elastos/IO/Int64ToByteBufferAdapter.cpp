
#include "cmdef.h"
#include "Int64ToByteBufferAdapter.h"

PInterface Int64ToByteBufferAdapter::Probe(
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


UInt32 Int64ToByteBufferAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Int64ToByteBufferAdapter::Release()
{
    return ElRefBase::Release();
}

ECode Int64ToByteBufferAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode Int64ToByteBufferAdapter::Wrap(
    /* [in] */ IByteBuffer* byteBuffer,
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> buf;
    byteBuffer->Slice((IByteBuffer**)&buf);
    Int32 cap;
    buf->Capacity(&cap);
    Int64ToByteBufferAdapter* adapter = new Int64ToByteBufferAdapter(cap, buf);
    if (adapter->Probe(EIID_IInt64Buffer) != NULL) {
        *buffer = (IInt64Buffer*)adapter->Probe(EIID_IInt64Buffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

Int64ToByteBufferAdapter::Int64ToByteBufferAdapter(
    /* [in] */ Int32 capacity,
    /* [in] */ IByteBuffer* byteBuffer)
    : Int64Buffer(capacity >> 3)
    , mByteBuffer(byteBuffer)
{
    mByteBuffer->Clear();
}

Int32 Int64ToByteBufferAdapter::GetByteCapacity()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     return ((DirectBuffer) byteBuffer).getByteCapacity();
    // }
    // assert false : byteBuffer;
    return -1;
}

//    PlatformAddress getEffectiveAddress();

//    PlatformAddress getBaseAddress();

Boolean Int64ToByteBufferAdapter::IsAddressValid()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     return ((DirectBuffer) byteBuffer).isAddressValid();
    // }
    // assert false : byteBuffer;
    return FALSE;
}

void Int64ToByteBufferAdapter::AddressValidityCheck()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     ((DirectBuffer) byteBuffer).addressValidityCheck();
    // } else {
    //     assert false : byteBuffer;
    // }
}

void Int64ToByteBufferAdapter::Free()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     ((DirectBuffer) byteBuffer).free();
    // } else {
    //     assert false : byteBuffer;
    // }
}

ECode Int64ToByteBufferAdapter::Array(
    /* [out, callee] */ ArrayOf<Int64>** array)
{
    return Int64Buffer::Array(array);
}

ECode Int64ToByteBufferAdapter::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return Int64Buffer::ArrayOffset(offset);
}

ECode Int64ToByteBufferAdapter::AsReadOnlyBuffer(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> byteBuf;
    mByteBuffer->AsReadOnlyBuffer((IByteBuffer**)&byteBuf);
    Int32 cap;
    byteBuf->Capacity(&cap);
    Int64ToByteBufferAdapter* buf = new Int64ToByteBufferAdapter(cap, byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    if (buf->Probe(EIID_IInt64Buffer) != NULL) {
        *buffer = (IInt64Buffer*)buf->Probe(EIID_IInt64Buffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode Int64ToByteBufferAdapter::Compact()
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

ECode Int64ToByteBufferAdapter::CompareTo(
    /* [in] */ IInt64Buffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return Int64Buffer::CompareTo(otherBuffer, result);
}

ECode Int64ToByteBufferAdapter::Duplicate(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> byteBuf;
    mByteBuffer->Duplicate((IByteBuffer**)&byteBuf);
    Int32 cap;
    byteBuf->Capacity(&cap);
    Int64ToByteBufferAdapter* buf = new Int64ToByteBufferAdapter(cap, byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    if (buf->Probe(EIID_IInt64Buffer) != NULL) {
        *buffer = (IInt64Buffer*)buf->Probe(EIID_IInt64Buffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode Int64ToByteBufferAdapter::GetInt64(
    /* [out] */ Int64* value)
{
    if (mPosition == mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    return mByteBuffer->GetInt64Ex(mPosition++ << 3, value);
}

ECode Int64ToByteBufferAdapter::GetInt64Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    return mByteBuffer->GetInt64Ex(index << 3, value);
}

ECode Int64ToByteBufferAdapter::GetInt64s(
    /* [out] */ ArrayOf<Int64>* dst)
{
    return Int64Buffer::GetInt64s(dst);
}

ECode Int64ToByteBufferAdapter::GetInt64sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Int64>* dst)
{
    return Int64Buffer::GetInt64sEx(off, len, dst);
}

ECode Int64ToByteBufferAdapter::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return mByteBuffer->GetOrder(order);
}

ECode Int64ToByteBufferAdapter::PutInt64(
    /* [in] */ Int64 c)
{
    if (mPosition == mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    return mByteBuffer->PutInt64Ex(mPosition++ << 3, c);
}

ECode Int64ToByteBufferAdapter::PutInt64Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int64 c)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    return mByteBuffer->PutInt64Ex(index << 3, c);
}

ECode Int64ToByteBufferAdapter::PutInt64s(
    /* [in] */ const ArrayOf<Int64>& src)
{
    return Int64Buffer::PutInt64s(src);
}

ECode Int64ToByteBufferAdapter::PutInt64sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Int64>& src)
{
    return Int64Buffer::PutInt64sEx(off, len, src);
}

ECode Int64ToByteBufferAdapter::PutInt64Buffer(
    /* [in] */ IInt64Buffer* src)
{
    return Int64Buffer::PutInt64Buffer(src);
}

ECode Int64ToByteBufferAdapter::Slice(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    mByteBuffer->SetLimit(mLimit << 3);
    mByteBuffer->SetPosition(mPosition << 3);
    AutoPtr<IByteBuffer> byteBuf;
    mByteBuffer->Slice((IByteBuffer**)&byteBuf);
    assert(byteBuf);
    Int32 cap;
    byteBuf->Capacity(&cap);
    Int64ToByteBufferAdapter* result = new Int64ToByteBufferAdapter(cap, byteBuf);
    if (result->Probe(EIID_IInt64Buffer) != NULL) {
        *buffer = (IInt64Buffer*)result->Probe(EIID_IInt64Buffer);
    }
    else {
        *buffer = NULL;
    }
    mByteBuffer->Clear();

    return NOERROR;
}

ECode Int64ToByteBufferAdapter::Capacity(
    /* [out] */ Int32* cap)
{
    return Int64Buffer::Capacity(cap);
}

ECode Int64ToByteBufferAdapter::Clear()
{
    return Int64Buffer::Clear();
}

ECode Int64ToByteBufferAdapter::Flip()
{
    return Int64Buffer::Flip();
}

ECode Int64ToByteBufferAdapter::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return Int64Buffer::HasArray(hasArray);
}

ECode Int64ToByteBufferAdapter::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return Int64Buffer::HasRemaining(hasRemaining);
}

ECode Int64ToByteBufferAdapter::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return mByteBuffer->IsDirect(isDirect);
}

ECode Int64ToByteBufferAdapter::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    return mByteBuffer->IsReadOnly(isReadOnly);
}

CARAPI Int64ToByteBufferAdapter::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int64>** array)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
//    throw new UnsupportedOperationException();
}

CARAPI Int64ToByteBufferAdapter::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
//    throw new UnsupportedOperationException();
}

CARAPI Int64ToByteBufferAdapter::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode Int64ToByteBufferAdapter::GetLimit(
    /* [out] */ Int32* limit)
{
    return Int64Buffer::GetLimit(limit);
}

ECode Int64ToByteBufferAdapter::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return Int64Buffer::SetLimit(newLimit);
}

ECode Int64ToByteBufferAdapter::Mark()
{
    return Int64Buffer::Mark();
}

ECode Int64ToByteBufferAdapter::GetPosition(
    /* [out] */ Int32* pos)
{
    return Int64Buffer::GetPosition(pos);
}

ECode Int64ToByteBufferAdapter::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return Int64Buffer::SetPosition(newPosition);
}

ECode Int64ToByteBufferAdapter::Remaining(
    /* [out] */ Int32* remaining)
{
    return Int64Buffer::Remaining(remaining);
}

ECode Int64ToByteBufferAdapter::Reset()
{
    return Int64Buffer::Reset();
}

ECode Int64ToByteBufferAdapter::Rewind()
{
    return Int64Buffer::Rewind();
}
