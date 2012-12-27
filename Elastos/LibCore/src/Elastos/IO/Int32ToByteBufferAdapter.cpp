
#include "cmdef.h"
#include "Int32ToByteBufferAdapter.h"
#include "ReadWriteDirectByteBuffer.h"

PInterface Int32ToByteBufferAdapter::Probe(
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


UInt32 Int32ToByteBufferAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Int32ToByteBufferAdapter::Release()
{
    return ElRefBase::Release();
}

ECode Int32ToByteBufferAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode Int32ToByteBufferAdapter::Wrap(
    /* [in] */ IByteBuffer* byteBuffer,
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> buf;
    byteBuffer->Slice((IByteBuffer**)&buf);
    Int32 cap;
    buf->Capacity(&cap);
    Int32ToByteBufferAdapter* adapter = new Int32ToByteBufferAdapter(cap, buf);
    if (adapter->Probe(EIID_IInt32Buffer) != NULL) {
        *buffer = (IInt32Buffer*)adapter->Probe(EIID_IInt32Buffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

Int32ToByteBufferAdapter::Int32ToByteBufferAdapter(
    /* [in] */ Int32 capacity,
    /* [in] */ IByteBuffer* byteBuffer)
    : Int32Buffer(capacity >> 2)
    , mByteBuffer(byteBuffer)
{
    mByteBuffer->Clear();
}

Int32 Int32ToByteBufferAdapter::GetByteCapacity()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     return ((DirectBuffer) byteBuffer).getByteCapacity();
    // }
    // assert false : byteBuffer;
    return -1;
}

//    PlatformAddress getEffectiveAddress();

//    PlatformAddress getBaseAddress();

Boolean Int32ToByteBufferAdapter::IsAddressValid()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     return ((DirectBuffer) byteBuffer).isAddressValid();
    // }
    // assert false : byteBuffer;
    return FALSE;
}

void Int32ToByteBufferAdapter::AddressValidityCheck()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     ((DirectBuffer) byteBuffer).addressValidityCheck();
    // } else {
    //     assert false : byteBuffer;
    // }
}

void Int32ToByteBufferAdapter::Free()
{
    // if (byteBuffer instanceof DirectBuffer) {
    //     ((DirectBuffer) byteBuffer).free();
    // } else {
    //     assert false : byteBuffer;
    // }
}

ECode Int32ToByteBufferAdapter::Array(
    /* [out, callee] */ ArrayOf<Int32>** array)
{
    return Int32Buffer::Array(array);
}

ECode Int32ToByteBufferAdapter::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return Int32Buffer::ArrayOffset(offset);
}

ECode Int32ToByteBufferAdapter::AsReadOnlyBuffer(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> byteBuf;
    mByteBuffer->AsReadOnlyBuffer((IByteBuffer**)&byteBuf);
    Int32 cap;
    byteBuf->Capacity(&cap);
    Int32ToByteBufferAdapter* buf = new Int32ToByteBufferAdapter(cap, byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    if (buf->Probe(EIID_IInt32Buffer) != NULL) {
        *buffer = (IInt32Buffer*)buf->Probe(EIID_IInt32Buffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode Int32ToByteBufferAdapter::Compact()
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

ECode Int32ToByteBufferAdapter::CompareTo(
    /* [in] */ IInt32Buffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return Int32Buffer::CompareTo(otherBuffer, result);
}

ECode Int32ToByteBufferAdapter::Duplicate(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    AutoPtr<IByteBuffer> byteBuf;
    mByteBuffer->Duplicate((IByteBuffer**)&byteBuf);
    Int32 cap;
    byteBuf->Capacity(&cap);
    Int32ToByteBufferAdapter* buf = new Int32ToByteBufferAdapter(cap, byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    if (buf->Probe(EIID_IInt32Buffer) != NULL) {
        *buffer = (IInt32Buffer*)buf->Probe(EIID_IInt32Buffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode Int32ToByteBufferAdapter::GetInt32(
    /* [out] */ Int32* value)
{
    if (mPosition == mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    return mByteBuffer->GetInt32Ex(mPosition++ << 2, value);
}

ECode Int32ToByteBufferAdapter::GetInt32Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    return mByteBuffer->GetInt32Ex(index << 2, value);
}

ECode Int32ToByteBufferAdapter::GetInt32s(
    /* [out] */ ArrayOf<Int32>* dst)
{
    return Int32Buffer::GetInt32s(dst);
}

ECode Int32ToByteBufferAdapter::GetInt32sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Int32>* dst)
{
    return Int32Buffer::GetInt32sEx(off, len, dst);
}

ECode Int32ToByteBufferAdapter::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return mByteBuffer->GetOrder(order);
}

ECode Int32ToByteBufferAdapter::PutInt32(
    /* [in] */ Int32 c)
{
    if (mPosition == mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    return mByteBuffer->PutInt32Ex(mPosition++ << 2, c);
}

ECode Int32ToByteBufferAdapter::PutInt32Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int32 c)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    return mByteBuffer->PutInt32Ex(index << 2, c);
}

ECode Int32ToByteBufferAdapter::PutInt32s(
    /* [in] */ const ArrayOf<Int32>& src)
{
    return Int32Buffer::PutInt32s(src);
}

ECode Int32ToByteBufferAdapter::PutInt32sEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Int32>& src)
{
    if (mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer) != NULL) {
        mByteBuffer->SetLimit(mLimit << 2);
        mByteBuffer->SetPosition(mPosition << 2);
        ReadWriteDirectByteBuffer* buf =
               (ReadWriteDirectByteBuffer*)mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer);
        buf->PutInt32s(off, len, src);
        mPosition += len;
        return NOERROR;
    }
    else {
        return Int32Buffer::PutInt32sEx(off, len, src);
    }
}

ECode Int32ToByteBufferAdapter::PutInt32Buffer(
    /* [in] */ IInt32Buffer* src)
{
    return Int32Buffer::PutInt32Buffer(src);
}

ECode Int32ToByteBufferAdapter::Slice(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    mByteBuffer->SetLimit(mLimit << 2);
    mByteBuffer->SetPosition(mPosition << 2);
    AutoPtr<IByteBuffer> byteBuf;
    mByteBuffer->Slice((IByteBuffer**)&byteBuf);
    assert(byteBuf);
    Int32 cap;
    byteBuf->Capacity(&cap);
    Int32ToByteBufferAdapter* result = new Int32ToByteBufferAdapter(cap, byteBuf);
    if (result->Probe(EIID_IInt32Buffer) != NULL) {
        *buffer = (IInt32Buffer*)result->Probe(EIID_IInt32Buffer);
    }
    else {
        *buffer = NULL;
    }
    mByteBuffer->Clear();

    return NOERROR;
}

ECode Int32ToByteBufferAdapter::Capacity(
    /* [out] */ Int32* cap)
{
    return Int32Buffer::Capacity(cap);
}

ECode Int32ToByteBufferAdapter::Clear()
{
    return Int32Buffer::Clear();
}

ECode Int32ToByteBufferAdapter::Flip()
{
    return Int32Buffer::Flip();
}

ECode Int32ToByteBufferAdapter::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return Int32Buffer::HasArray(hasArray);
}

ECode Int32ToByteBufferAdapter::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return Int32Buffer::HasRemaining(hasRemaining);
}

ECode Int32ToByteBufferAdapter::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return mByteBuffer->IsDirect(isDirect);
}

ECode Int32ToByteBufferAdapter::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    return mByteBuffer->IsReadOnly(isReadOnly);
}

CARAPI Int32ToByteBufferAdapter::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int32>** array)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
//    throw new UnsupportedOperationException();
}

CARAPI Int32ToByteBufferAdapter::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
//    throw new UnsupportedOperationException();
}

CARAPI Int32ToByteBufferAdapter::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode Int32ToByteBufferAdapter::GetLimit(
    /* [out] */ Int32* limit)
{
    return Int32Buffer::GetLimit(limit);
}

ECode Int32ToByteBufferAdapter::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return Int32Buffer::SetLimit(newLimit);
}

ECode Int32ToByteBufferAdapter::Mark()
{
    return Int32Buffer::Mark();
}

ECode Int32ToByteBufferAdapter::GetPosition(
    /* [out] */ Int32* pos)
{
    return Int32Buffer::GetPosition(pos);
}

ECode Int32ToByteBufferAdapter::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return Int32Buffer::SetPosition(newPosition);
}

ECode Int32ToByteBufferAdapter::Remaining(
    /* [out] */ Int32* remaining)
{
    return Int32Buffer::Remaining(remaining);
}

ECode Int32ToByteBufferAdapter::Reset()
{
    return Int32Buffer::Reset();
}

ECode Int32ToByteBufferAdapter::Rewind()
{
    return Int32Buffer::Rewind();
}
